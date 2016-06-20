#include "clientsocket.h"
#include<QDebug>
ClientSocket::ClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    blockSize = 0;
    info.clientSocket = this;
    connect(this, SIGNAL(readyRead()),
            this,SLOT(receiveMessage()));
    connect(this,SIGNAL(disconnected()),
            this,SLOT(deleteSocket()));
}

ClientSocket::~ClientSocket()
{

}

//接受信息
void ClientSocket::receiveMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    if(blockSize == 0)
    {
        if(bytesAvailable() < (int)sizeof(quint16))
            return ;
        //读入blcok大小
            in >> blockSize;
            qDebug("blockSize %d",blockSize);
    }
    if(bytesAvailable() < blockSize)
        return ;

    in >> info.requestKind;
    qDebug("clientSocket receive %d ", info.requestKind);
    switch(info.requestKind)
    {
        //登录请求
        case LOGIN :
        {
             in >> info.logInf.account;
             in >> info.logInf.password;
             break;
        }
        //连接请求
        case CONNECT:
        {
            in >> info.logInf.account;
           break;
        }
        //聊天信息
        case TALK:
        {
            in >> info.message;
            qDebug("clientSocket receive message");
            qDebug() << info.message.sender << info.message.sender << info.message.text;
            break;
        }
        //注册请求
        case REGISTER:
        {
            in >> info.userInfo;
            break;
        }
    }

    //重置大小
    blockSize = 0;

    //Server返回
    emit sendSignal(info);
}

//发送信息
void ClientSocket::sendMessage(const Order &info)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);

    out <<(qint16)0;
    out << info.replyKind;
        qDebug("ClientSocket send message  replyKind %d",info.replyKind);
    switch (info.replyKind) {
    //登录成功
    case LOGIN_SUCCESS:
      {  qint32 len = info.friends.size();
        out << len;
        for(qint32 i = 0; i < len ; ++i)
        {
            out << info.friends[i].account;
        }
        break;
    }
    //聊天信息
    case TALK_MESSAGE:
      {
        out << info.message;
        qDebug("clientSocket send message");
        break;
      }
    }
    out.device()->seek(0);
    out << (qint16)(block.size()-sizeof(qint16));

    write(block);
    this->flush();
}

//删除Socket
void ClientSocket::deleteSocket()
{
    emit deleteSignal(info.logInf.account);
    deleteLater();
}
