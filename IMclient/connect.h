#ifndef CONNECT_H
#define CONNECT_H
#include"constants.h"
#include <QObject>

//通信类
class Connect : public QObject
{
    Q_OBJECT
private:

    bool isConnected; //连接状态

    //用于通信信息的临时变量
    LoginInformation logInfo; 
    QString hostAddress;
    QString hostPort;
    qint32 status;
    qint16 blockSize;
    QTcpSocket *tcpClient;
    qint32 replyKind;
    qint32 requestKind;
    UserInfo userInfo;
    Message message;
    QVector<FriendInfo> friendsVec;
public:
    explicit Connect(QString hostAddress, QString hostPort
                     , QObject *parent = 0);

    //登录请求
    void loginRequest(const LoginInformation &logInfo);
    //获取连接状态
    bool getConnectStatu();
    //重新连接到服务器
    void reConnectToServer(const QString hostAddress, const QString hostPort);
    //获取好友列表
    void getFriendsVec(QVector<FriendInfo> &friInfoVec);
    //发送或接收信息请求
    void messageRequest(const Message mes);
    //注册请求
    void registerRequest(const UserInfo &userInfo);
    //注册客户端请求
    void registerClientRequest(const QString &account);
    //获取信息
    Message getMessage() const
    {return message;}
signals:
    void newReply(qint32 replyKind);
    void connectFailed();
    void disconnectedSignal();
    void socketConnectedSignal();
public slots:
    //发送信息
    void sendRequest();
    //处理请求信息
    void readResult();
    //和服务器断开连接
    void ServerDisconnected();
    //检查连接状态
    void checkConnected();
    //和服务器连接成功
    void socketConnectedSlot();
    //socket错误
    void socketError(QAbstractSocket::SocketError);
};

#endif // CONNECT_H
