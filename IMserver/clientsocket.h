#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include "constants.h"
class ClientSocket : public QTcpSocket
{
    Q_OBJECT
private:
    qint16 blockSize;
    Order  info;

public:
    explicit ClientSocket(QObject *parent = 0);
    ~ClientSocket();

signals:
    void userLoginSignal(const UserInfo &user);
    void sendSignal(Order &info);
    void deleteSignal(const QString&);
public slots:
    void deleteSocket();
    void receiveMessage();
    void sendMessage(const Order &info);
};

#endif // CLIENTSOCKET_H
