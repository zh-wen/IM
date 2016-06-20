#include "friendbutton.h"
#include<QIcon>
#include<QPixmap>
FriendButton::FriendButton(Connect *conn,
                           const FriendInfo &friInfo,
                           const QString &acc,
                           QObject *parent )
                         :conn(conn),friendInfo(friInfo),myAccount(acc)
{
    isOpen = false;
    //初始化界面
    setContextMenuPolicy(Qt::CustomContextMenu);
    setIconSize(QSize(50,50));
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setAutoRaise(true);

   setButtonStatus();
    connect(this,SIGNAL(clicked()),this,SLOT(openChatWindow()));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(clickedMenu(QPoint)));
}


//打开聊天窗口
void FriendButton::openChatWindow()
{
    if(isOpen)
        return;
    isOpen = true;
    chatWindow = new ChatWindow(conn,friendInfo,myAccount,NULL);
    connect(chatWindow,SIGNAL(ChatWindowQuit()),this,SLOT(chatWinQuit()));
    chatWindow->show();
}

//退出聊天窗口
void FriendButton::chatWinQuit()
{
    isOpen = false;
}

//删除好友
void FriendButton::removeFriend()
{

}

//右击
void FriendButton::clickedMenu(const QPoint &point)
{
    menu->exec(QCursor::pos());
}

//右击目录
void FriendButton::createMenu()
{
    menu = new QMenu(this);
    QAction *actDelete = new QAction(tr("Remove this friend"),this);

    connect(actDelete,SIGNAL(triggered())
            ,this,SLOT(removeFriend()));

    menu->addAction(actDelete);
}

//设置按钮状态
void FriendButton::setButtonStatus()
{
    QString iconPath("icons/icon.bmp");
    setIcon(QIcon(iconPath));
    QString text = QString("%1\n").arg(friendInfo.account);
    setText(text);
}
