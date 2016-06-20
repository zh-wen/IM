#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include "QTcpSocket"
#include <QDataStream>
#include <QMessageBox>
//用户详细信息
struct UserInfo
{
    QString account;
    QString password;
    int statu;
    friend QDataStream & operator <<(QDataStream &qos, const UserInfo &userInfo)
    {
        qos << userInfo.account << userInfo.password;
        return qos;
    }

    friend QDataStream & operator >>(QDataStream &qis, UserInfo &userInfo)
    {
        qis >> userInfo.account >> userInfo.password;
        return qis;
    }
};

//用户登录信息
struct LoginInformation
{
    QString account;
    QString password;
};

//聊天信息
struct Message
{
    QString sender;			//发送者
    QString receiver;		//接收者
    QString text;			//消息内容

    friend QDataStream & operator <<(QDataStream &qos, const Message &mes)
    {
        qos << mes.sender << mes.receiver << mes.text;
        return qos;
    }

    friend QDataStream & operator >>(QDataStream &qis , Message &mes)
    {
        qis >> mes.sender >> mes.receiver >> mes.text;
        return qis;
    }

};



//好友信息
struct FriendInfo
{
    QString account;

    friend QDataStream & operator >>(QDataStream &qis,  FriendInfo &friInfo)
    {
        qis >> friInfo.account;
        return qis;
    }

    friend QDataStream &operator <<(QDataStream &qos, const FriendInfo &friInfo)
    {
        qos << friInfo.account;
        return qos;
    }
};

//loginWindow Request
enum
{
    LOGIN,
    TALK,
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
