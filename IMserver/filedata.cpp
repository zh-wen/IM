#include "filedata.h"
#include"QDebug"
FileData::FileData(QObject *parent) :
    QObject(parent)
{

}

FileData::~FileData()
{
    saveData();
}

//登录请求
qint32 FileData::loginRequest(const LoginInformation &logInfo,QVector<FriendInfo> &friends)
{
    //若好友列表为空，则读取文件数据
    if(userInfoList.empty())
     {
        UserInfo tmpInfo;

            inFIle = new QFile("AccountInfo");
            inFIle->open(QIODevice::ReadOnly);
            QTextStream in(this->inFIle);
        while(!in.atEnd())
         {
            tmpInfo.account = in.readLine();
            tmpInfo.password = in.readLine();
            tmpInfo.statu = OFFLINE;

            userInfoList.insert(tmpInfo.account,tmpInfo);
          }

            inFIle->close();
            delete inFIle;
    }
    QMap<QString,UserInfo>::iterator iter;

    iter = userInfoList.find(logInfo.account);

    //账号不存在
    if(iter == userInfoList.end())
    {
        return LOGIN_NO_ACCOUNT;
    }
    else if( iter.value().password == logInfo.password)
    {
        //账号已登录
        if(iter.value().statu == ONLINE)
        {
            return LOGIN_EXISTS;
        }
        //登录成功
        else
        {
            iter.value().statu = ONLINE;
            getFriendsAccout(logInfo.account,friends);
            return LOGIN_SUCCESS;
        }
    }
    else
    {
        //密码错误
        return LOGIN_WORING_PWD;
    }
}

//获取好友信息
void FileData::getFriendsAccout(const QString &acc, QVector<FriendInfo> &friends)
{
    QString dir("friendsInfo/");
    QString path = dir + acc;
    inFIle = new QFile(path);

    inFIle->open(QIODevice::ReadOnly);
    QTextStream in(inFIle);
    friends.clear();
    while(!in.atEnd())
    {
        FriendInfo tmpFriInfo;
        tmpFriInfo.account = in.readLine();
        qDebug() << tmpFriInfo.account;
        friends.push_back(tmpFriInfo);
    }
    qDebug("file get friends %d",friends.size());
    inFIle->close();
    delete inFIle;
    inFIle = NULL;
}

//离线请求
void FileData::offlineRequest(const QString acc)
{
    QMap<QString, UserInfo>::iterator iter;
    iter = userInfoList.find(acc);
    iter.value().statu = OFFLINE;
}

//注册请求
qint32 FileData::registerRequest(const UserInfo &userInfo)
{
    //获取用户列表
    if(userInfoList.empty())
     {
        UserInfo tmpInfo;

            inFIle = new QFile("AccountInfo");
            inFIle->open(QIODevice::ReadOnly);
            QTextStream in(this->inFIle);
        while(!in.atEnd())
         {
            tmpInfo.account = in.readLine();
            tmpInfo.password = in.readLine();
            tmpInfo.statu = OFFLINE;

            userInfoList.insert(tmpInfo.account,tmpInfo);
          }

            inFIle->close();
            delete inFIle;
    }
    QMap<QString,UserInfo>::iterator iter;

    iter = userInfoList.find(userInfo.account);

    //若账号不存在，注册成功
    if(iter == userInfoList.end())
    {
        userInfoList.insert(userInfo.account,userInfo);
        return REGISTER_SUCCESS;
    }
    else
    {
        return REGISTER_EXIST;
    }
}

//保存信息
void FileData::saveData() const
{
    QFile file("AccountInfo");
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream out(&file);

    QMap<QString,UserInfo>::const_iterator iter;

    for(iter = userInfoList.begin(); iter !=userInfoList.end(); iter++)
    {
        out << iter->account << endl;
        out << iter->password << endl;
    }
    file.close();
}
