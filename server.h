
public slots:
    void incomingConnection(qintptr sd) override;
    void slotReadyRead();
    void socketDisconnected();

private:
    void sendToClient(const QString& str);

    std::list<QTcpSocket*> sockets;
    QByteArray data;
    quint16 nextBlockSize{0};
    QTcpSocket *socket;


