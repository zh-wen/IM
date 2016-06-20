#ifndef REGISTERWIN_H
#define REGISTERWIN_H
#include"constants.h"
#include"connect.h"
#include <QWidget>

namespace Ui {
class RegisterWin;
}

//账号注册窗口
class RegisterWin : public QWidget
{
    Q_OBJECT
private:
    UserInfo userInfo; //账号
    Connect *conn;
    Ui::RegisterWin *ui;

    bool checkUserInfo() ;
public:
    explicit RegisterWin(Connect *conn,QWidget *parent = 0);
    ~RegisterWin();

public slots:
    //处理接收的数据
    void readResult(qint32 replyKind);
    //点击注册按钮
    void clickedRegiserButton();
    //点击取消按钮
    void clickedQuitButton();
};

#endif // REGISTERWIN_H
