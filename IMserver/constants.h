#ifndef CONSTANTS_H
#define CONSTANTS_H
#include<QString>
#include<QDataStream>
#include<QVector>
class ClientSocket;

//用户详细信息
struct UserInfo
{
    QString account;
    QString password;
    int statu;
    UserInfo(){};
   friend QDataStream & operator>> (QDataStream &qis, UserInfo &info)
   {
       qis >> info.account >>info.password;
       return qis;
   }

   friend QDataStream & operator<< (QDataStream &qos, const UserInfo &info)
   {
       qos << info.account << info.password;
       return qos;
    }
};

//用户登录信息
struct LoginInformation
{
    QString account;
    QString password;

    friend QDataStream & operator >>(QDataStream &qis, LoginInformation &logInfo)
    {
        qis >> logInfo.account >> logInfo.password;
        return qis;
    }

    friend QDataStream & operator <<(QDataStream &qos, const LoginInformation logInfo)
    {
        qos << logInfo.account << logInfo.password;
        return qos;
    }
};

//聊天信息
struct Message
{
    QString sender;			//发送者
    QString receiver;		//接收者
    QString text;			//消息内容

    friend QDataStream & operator <<(QDataStream &qos, const Message &mes)
    {
        qos <<  mes.sender << mes.receiver << mes.text;
        return qos;
    }

    friend QDataStream & operator >>(QDataStream &qis , Message &mes)
    {
        qis  >> mes.sender >> mes.receiver >> mes.text;
        return qis;
    }
};

//好友信息
struct FriendInfo
{
    QString account;

    friend QDataStream & operator <<(QDataStream &qos, const FriendInfo &fri)
    {
        qos << fri.account ;
        return qos;
    }

    friend QDataStream & operator >>(QDataStream &qis,  FriendInfo &fri)
    {
        qis >> fri.account ;
        return qis;
    }
};


//指令
struct Order
{
    qint32 requestKind;
    qint32 replyKind;
    qint32 status;
    ClientSocket *clientSocket;

    UserInfo userInfo;
    Message message;
    QString myAccount;
    QString peerAccount;
    LoginInformation logInf;
    QVector<FriendInfo> friends;
};

//loginWindow Request
enum
{
    LOGIN,
    TALK,】
    CONNECT,
    REGISTER
};

//Reply
enum
{
    LOGIN_SUCCESS  = 20,
    LOGIN_NO_ACCOUNT ,
    LOGIN_WORING_PWD ,
    LOGIN_EXISTS,
    CHANGE_STATUE,
    TALK_MESSAGE,
    REGISTER_EXIST,
    REGISTER_SUCCESS
};

//STATUS
enum
{
    ONLINE = 30,
    OFFLINE
};




#endif // CONSTANTS_H
