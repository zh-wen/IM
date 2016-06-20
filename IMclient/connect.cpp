#include "connect.h"
#include<QDebug>
#include"window.h"
Connect::Connect(QString hostAddress,QString hostPort,QObject *parent) :
    QObject(parent)
{
    //初始化变量
    this->hostAddress = hostAddress;
    this->hostPort = hostPort;
    tcpClient = new QTcpSocket(this);
    isConnected = false;

    //Connect SIGNAL to SLOT
    connect(tcpClient,SIGNAL(connected()), this, SLOT(checkConnected()));
    connect(tcpClient,SIGNAL(connected()),this, SLOT(socketConnectedSlot()));
    connect(tcpClient,SIGNAL(readyRead()), this, SLOT(readResult()));
    connect(tcpClient,SIGNAL(disconnected()),
            this,SLOT(ServerDisconnected()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError(QAbstractSocket::SocketError)));
    tcpClient->connectToHost(hostAddress,hostPort.toInt());
    blockSize = 0;
 }

//处理请求信息
void Connect::readResult()
{
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_4_3)	;

    //获取块大小
    if(blockSize == 0)
    {
        if(tcpClient->bytesAvailable() < (int)sizeof(qint16))
            return ;
        in >> blockSize;
    }

    if(tcpClient->bytesAvailable() < blockSize)
        return;

    in >> replyKind;
    qDebug("the reply num %d",replyKind)
    switch (replyKind)
    {
    //登录成功
    case LOGIN_SUCCESS:
     {
        friendsVec.clear();
        FriendInfo tmpFriends;
        qint32 len;
        in >> len;
        for(int i = 0; i < len ; i++)
        {
            in >> tmpFriends;
            friendsVec.push_back(tmpFriends);
        }
        break;
     }
    //获取聊天信息
    case TALK_MESSAGE:
       {
        in >> message.sender;
        in >> message.receiver;
        in >> message.text;
        qDebug("receive message");
        qDebug() << message.sender << message.receiver << message.text;
        break;
       }
    }
    //重置块大小
    blockSize = 0;
    emit newReply(replyKind);
}

//和服务器断开连接
void Connect::ServerDisconnected()
{
    emit disconnectedSignal();
}

//发送信息
void Connect::sendRequest()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    out << (qint16)0;
    out << requestKind;

    switch (requestKind) {
    //发送登录信息
    case LOGIN:
    {
        out << logInfo.account;
        out << logInfo.password;
        break;
    }
    //发送连接信息
    case CONNECT:
    {
        out << logInfo.account;
        break;
    }
    //发送聊天信息
    case TALK:
    {
        out <<  message;
        qDebug("client send TALK");
        qDebug() << message.sender << message.receiver << message.text;
        break;
    }
    //发送注册信息
    case REGISTER:
    {
        out <<  userInfo;
        break;
    }
    default:
        break;
    }
    //设置块大小
    out .device()->seek(0);
    out << (qint16) (block.size() - sizeof(qint16));

    tcpClient->write(block);
    qDebug("client send message blocksize%d",(qint16)(block.size() - sizeof(qint16)));
    tcpClient->flush();
}

//登录请求
void Connect::loginRequest(const LoginInformation &logInfo)
{
    requestKind  = LOGIN;
    this->logInfo= logInfo;

    sendRequest();
}

//检查连接状态
void Connect::checkConnected()
{
    isConnected = true;
}

//获取链接状态
bool Connect::getConnectStatu()
{
    return this->isConnected;
}

//重新连接到服务器
void Connect::reConnectToServer(const QString hostAddress, const QString hostPort)
{
    this->tcpClient->connectToHost(hostAddress,hostPort.toInt());
}

//socket错误
void Connect::socketError(QAbstractSocket::SocketError)
{
    isConnected = false;
    emit disconnectedSignal();
}

//获取好友列表
void Connect::getFriendsVec(QVector<FriendInfo> &friInfoVec)
{
    friInfoVec = this->friendsVec;
}

//发送信息
void Connect::messageRequest(const Message mes)
{
    requestKind = TALK;
    message = mes;
    sendRequest();
}

//注册客户端请求
void Connect::registerClientRequest(const QString &account)
{
    requestKind = CONNECT;
    logInfo.account = account;
    sendRequest();
}

//和服务器连接成功
void Connect::socketConnectedSlot()
{
    //发送已连接信号
    emit socketConnectedSignal();
}

//注册请求
void Connect::registerRequest(const UserInfo &userInfo)
{
    requestKind = REGISTER;
    this->userInfo = userInfo;
    sendRequest();
}
