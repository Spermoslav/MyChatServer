#include "server.h"

Server::Server()
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
    qDebug() << "client connected" << sd;
}
}
