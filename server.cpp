#include "server.h"
#include <QTime>

Server::Server(Menu* menu) :
    menu(menu)
{
    if(listen(QHostAddress::Any, 1234)) {
        appendMessage("Server listening");
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
        for(;;) {
            if(nextBlockSize == 0) {
                if(socket->bytesAvailable() < 2) {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize) {
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            sendToClient(str);
            break;
        }
    }
    else {
        appendMessage("DataStream error. Message has not read");
    }
}

void Server::sendToClient(const QString& str)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    out << str;

    for(auto e : sockets) {
        e->write(data);
    }
}
