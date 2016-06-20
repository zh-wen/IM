#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include"constants.h"
#include"connect.h"
#include <QWidget>

namespace Ui {
class LoginWindow;
}

//登录窗口
class LoginWindow : public QWidget
{
    Q_OBJECT
private:
    LoginInformation logInfo;
    QString hostAddress;
    QString hostPort;
    Connect *conn;
public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;

private slots:
    //点击登录按钮
    void clickedLogBtn();
    //点击注册按钮
    void clickedRegisterBtn();
    //与服务器断开连接
    void serverDisconnected();
    //处理接收的数据
    void readReply(qint32 replyKind) ;
    //设置服务器地址
    void setHostAddress();
};

#endif // LOGINWINDOW_H
