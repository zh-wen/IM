#ifndef WINDOW_H
#define WINDOW_H
#include"connect.h"
#include"constants.h"
#include"friendbutton.h"
#include<QMap>
#include <QWidget>
#include<QVBoxLayout>
#include<QGroupBox>


namespace Ui {
class window;
}

//主窗口
class Window : public QWidget
{
    Q_OBJECT
private:
    Ui::window *ui;
    QString hostAddress;
    QString hostPort;
    QString myAccount;
    Connect *conn;
    QVBoxLayout *layout;
    QGroupBox *groupBox;
    QMap<QString, FriendButton*> friendsButtonMap; 
    QVector<FriendInfo> friendsVec; //好友列表

    //接受信息
    void receiveMessage(const Message mes) ;
    //创建目录
    void createFolder() const;
public:
    explicit Window(const QString hostAddress,const QString hostPort,
                    QString acc,QVector<FriendInfo> friendsVec,QWidget *parent = 0);
    ~Window();

public slots:
    //处理接受的数据
    void readResult(qint32 replyKind);

private slots:
    //注册客户端
    void registerSlot();
    //与服务器断开连接
    void serverDisconnected();
};

#endif // WINDOW_H
