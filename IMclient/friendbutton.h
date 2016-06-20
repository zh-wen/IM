  #ifndef FRIENDBUTTON_H
#define FRIENDBUTTON_H
#include"constants.h"
#include"connect.h"
#include"chatwindow.h"
#include <QToolButton>
#include<QMenu>
#include<QAction>
//好友按钮
class FriendButton : public QToolButton
{
    Q_OBJECT
    bool isOpen; //是否打开
    QString myAccount; //账号
    FriendInfo friendInfo; //好友信息
    Connect  *conn; //通信类
    QMenu   *menu;  
    ChatWindow *chatWindow; //聊天窗口

    //create右击目录
    void createMenu();
    //设置按钮状态
    void setButtonStatus();
public:
    explicit FriendButton(Connect *conn,
                          const FriendInfo &friInfo,
                          const QString &acc,
                          QObject *parent = 0);
    //关闭聊天窗口
    void closeChatWindow();
    //获取聊天窗口
    ChatWindow *getChatWindow()const
    {return chatWindow;}
signals:

public slots:
    //删除好友
    void removeFriend();
    //打开聊天窗口
    void openChatWindow();
    //右击
    void clickedMenu(const QPoint &point);
    //退出聊天窗口
    void chatWinQuit();
};

#endif // FRIENDBUTTON_H
