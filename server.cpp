#include <QTime>
#include <QDir>

#include "server.h"

Server::Server(Menu* menu) :
    menu(menu)
{
    if(listen(QHostAddress::Any, 1234)) {
        appendMessage("Server listening");
        QDir d = QDir::current();
        d.mkdir("Data");

    }
    else {
        appendMessage("Server not listening");
    }
}

void Server::incomingConnection(qintptr sd)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(sd);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
    sockets.push_back(socket);
    appendMessage("Client connected" + QString::number(sd));
}

void Server::socketDisconnected()
{
    socket = static_cast<QTcpSocket*> (sender());
    socket->deleteLater();
    for(auto it = sockets.begin(); it != sockets.end(); ++it) {
        if(*it == socket) {
            sockets.erase(it);
            break;
        }
    }
    appendMessage("Client disconnected");
}

void Server::appendMessage(const QString &text)
{
    menu->textBrowserAppend(QTime::currentTime().toString() + " " + text);
}

void Server::slotReadyRead()
{
    socket = static_cast<QTcpSocket*> (sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok) {
        if(nextBlockSize == 0) {
            if(socket->bytesAvailable() < 2) {
                return;
            }
            in >> nextBlockSize;
        }
        if(socket->bytesAvailable() < nextBlockSize) {
            return;
        }

        Data d;
        in >> d.type;
        in >> d.text;
        nextBlockSize = 0;

        switch (d.type) {
        case Message:
            sendToClient(d);
            break;
        case Info:
            sendToClient(d);
            break;
        case Login: {
            std::optional<QString> f = findAccount(accDataSplit(d.text).first);

            if(f != std::nullopt) {
                if(accDataSplit(d.text).second == accDataSplit(*f).second) {
                    sendToClient(d, socket);
                }
                else {
                    sendToClient(Data("p", Login), socket);
                }
            }
            else {
                sendToClient(Data("n", Login), socket);
            }
            break;
        }
        case Registration: {
                if(writeNewAccount(d.text)) {
                    sendToClient(d, socket);
                }
                else {
                    sendToClient(Data("e", Registration), socket);
                }
            }
            break;
        }
    }
    else {
        appendMessage("DataStream error. Message has not read");
    }
}

void Server::sendToClient(const Data& d, QTcpSocket* client)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0);
    out << d.type;
    out << d.text;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    out << d.type;
    out << d.text;

    if(d.type == Message || d.type == Info) {
        for(auto e : sockets) {
            e->write(data);
        }
    }
    else {
        if(client) {
            client->write(data);
        }
        else {
            qDebug() << __FUNCTION__ << " client == nullptr";
        }
    }
}
