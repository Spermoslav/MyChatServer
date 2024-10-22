#include "server.h"

Server::Server(Menu* menu) :
    menu(menu)
{
    if(listen(QHostAddress::Any, 1234)) {
        menu->textBrowserAppend("Server listening");
    }
    else {
        menu->textBrowserAppend("Server not listening");
    }
}

void Server::incomingConnection(qintptr sd)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(sd);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
    sockets.push_back(socket);
    menu->textBrowserAppend("Client connected" + QString::number(sd));
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
    menu->textBrowserAppend("Client disconnected");
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
        menu->textBrowserAppend("DataStream error. Message has not send");
    }
}
}
