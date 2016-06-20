#include "server.h"
#include<iostream>
#include<QDebug>
using std::cout;
Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    this->listen(QHostAddress::Any, 8888);

}

Server::~Server()
{

}

//新的socket连接请求
void Server::incomingConnection(int sockDescriptor)
{
    //建立一个新的socket与客户端socket连接
    ClientSocket * clientSocket = new ClientSocket(this);
    connect(clientSocket,SIGNAL(deleteSignal(const QString &)),
            this, SLOT(clientDisconnected(const QString&)));
    connect(clientSocket,SIGNAL(sendSignal( Order&)),
            this, SLOT(sendMessage( Order&)));
    clientSocket->setSocketDescriptor(sockDescriptor);
}

//发送信息
void Server::sendMessage( Order &info)
{
    qDebug("server requestKind %d",info.requestKind);
    switch (info.requestKind) {
    //登录请求
    case LOGIN:
     {
        tmpInfo.replyKind = fileData.loginRequest(info.logInf,tmpInfo.friends) ;

        info.clientSocket->sendMessage(tmpInfo);
        break;
    }
    //连接请求
    case CONNECT:
    {
      userMap.insert(info.logInf.account,info.clientSocket);
      qDebug("server receive CONNET signal");
      qDebug() << info.logInf.account;
      break;
    }
    //转发信息
    case TALK:
    {
        QMap<QString,ClientSocket*>::iterator iter;
        iter  = userMap.find(info.message.receiver);

        //offline message
        if(iter == userMap.end())
        {
            qDebug("receiver not found ");
            qDebug() << info.message.sender << info.message.receiver << info.message.text;
            return ;
         }
        tmpInfo.replyKind = TALK_MESSAGE;
        tmpInfo.message = info.message;
        iter.value()->sendMessage(tmpInfo);
        qDebug("server send Message");
        break;
    }
    //账号注册请求
    case REGISTER:
    {
        tmpInfo.replyKind = fileData.registerRequest(info.userInfo);
        info.clientSocket->sendMessage(tmpInfo);
    }
    default:
        break;
    }
}

//改变账号的状态
void Server::changeStatu(const QString &acc, qint32 status)
{
    tmpInfo.replyKind = CHANGE_STATUE;
    tmpInfo.status = status;
    tmpInfo.peerAccount = acc;


}

void Server::clientDisconnected(const QString &acc)
{
    userMap.remove(acc);
    fileData.offlineRequest(acc);
}

