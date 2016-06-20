#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QMap>
#include<QCloseEvent>
#include"constants.h"
#include"clientsocket.h"
#include"filedata.h"

class Server : public QTcpServer
{
    Q_OBJECT
private:
    int descriptor;
    Order tmpInfo;
    FileData fileData;
    QMap<QString, ClientSocket*> userMap;
public:
     Server(QObject *parent = 0);
    ~Server();

    //处理好友请求
 //   void friendRequest(const saveStruct &save);
    //改变用户状态
    void changeStatu(const QString &acc, qint32 status);

protected:
    void incomingConnection(int sockDescriptor);

public slots:
        //客户端断开连接
    void clientDisconnected(const QString &acc);
        //要求clientSocket发送信息
    void sendMessage( Order &info);
};

#endif // SERVER_H
