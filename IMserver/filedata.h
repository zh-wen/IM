#ifndef FILEDATA_H
#define FILEDATA_H
#include"constants.h"
#include<QMap>
#include<QFile>
#include <QObject>
#include<QTextStream>
class FileData : public QObject
{
    Q_OBJECT

private:
    QFile *inFIle;
    QMap<QString,UserInfo>  userInfoList;//好友信息
public:
    explicit FileData(QObject *parent = 0);
    ~FileData();
    //登录请求
    qint32 loginRequest(const LoginInformation &logInfo,QVector<FriendInfo> &friends);
    //注册请求
    qint32 registerRequest(const UserInfo &userInfo);
    //获取好友信息
    void getFriendsAccout(const QString &acc, QVector<FriendInfo> &friends);
    //离线请求
    void offlineRequest(const QString acc);
    //保存数据
    void saveData() const;
signals:

public slots:

};

#endif // FILEDATA_H
