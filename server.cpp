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
}
