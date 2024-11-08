#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <list>

#include "menu.h"
#include "data.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(Menu* menu);

public slots:
    void incomingConnection(qintptr sd) override;
    void slotReadyRead();
    void socketDisconnected();
    void appendMessage(const QString& text);

private:
    void sendToClient(const QString& str);

    std::list<QTcpSocket*> sockets;
    QByteArray data;
    quint16 nextBlockSize{0};
    QTcpSocket* socket;

    Menu* menu;
};

#endif // SERVER_H
