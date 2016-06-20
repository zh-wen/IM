#ifndef CHATWINDOW_H
#define CHATWINDOW_H
#include "constants.h"
#include "connect.h"
#include <QWidget>

//聊天窗口
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(Connect *conn, const FriendInfo &peer,
                        const QString &acc, QWidget *parent = 0);
    ~ChatWindow();
    //显示聊天信息
    void setOutputLine(const Message &mes);
signals:
    //退出聊天窗口信号
    void ChatWindowQuit();
    //发送信息信号
    void messageSingnal(const Message &mes);
public slots:
    //点击发送信息按钮
    void clickedSendButton();
    //点击聊天记录按钮
    void clickedRecordButton();
protected:
    void closeEvent(QCloseEvent * event);

private:
    Ui::ChatWindow *ui;
    Connect *conn;       //通信类
    FriendInfo peer;     //聊天对方信息
    QString myAccount;   //账号
    QString sendMessage; //信息

    void saveChatRecord ()const; //保存聊天记录
    QString readChatRecord();    //读取聊天记录

    QString getCurrentDateTime(); //获取当前时间
};

#endif // CHATWINDOW_H
