#include "window.h"
#include "ui_window.h"
#include"QDebug"
#include<QDir>

Window::Window(const QString hostAddress,const QString hostPort,
               QString acc, QVector<FriendInfo>friendsVec, QWidget *parent) :
    QWidget(parent),hostAddress(hostAddress),hostPort(hostPort),myAccount(acc),
    friendsVec(friendsVec),
    ui(new Ui::window)
{
    //初始化界面
    ui->setupUi(this);
    qDebug() << hostAddress << hostPort;
    conn = new Connect(hostAddress,hostPort,this);
    ui->label->setText(acc);
    createFolder();
    groupBox = new QGroupBox(this);
    layout = new QVBoxLayout(groupBox);
    layout->setMargin(10);
    layout->setAlignment(Qt::AlignTop);

    qDebug("list Friends num %d",friendsVec.size());
    //创建好友按钮
    for(int i = 0; i< friendsVec.size(); ++i)
    {
        addFriendButton(friendsVec[i]);
    }
    ui->friendToolBox->removeItem(0);
    ui->friendToolBox->addItem(groupBox,tr("My friends"));
    setWindowTitle(myAccount);

    QString iconPath("icons/icon.bmp");
    setWindowIcon(QIcon(iconPath));

    connect(conn,SIGNAL(newReply(qint32)),this,SLOT(readResult(qint32)));
    connect(conn, SIGNAL(socketConnectedSignal()),this,SLOT(registerSlot()));
    connect(conn,SIGNAL(disconnectedSignal()),this,SLOT(serverDisconnected()));
}

Window::~Window()
{
    delete ui;
}

//添加好友按钮
void Window::addFriendButton(const FriendInfo &friInfo)
{
    FriendButton *button  = new FriendButton(conn,friInfo,myAccount,this);
    layout->addWidget(button);
    friendsButtonMap.insert(friInfo.account,button);
}

//处理接收的数据
void Window::readResult(qint32 replyKind)
{
    switch(replyKind)
    {
    case TALK_MESSAGE:
      {
        Message message;
        message = conn->getMessage();
        receiveMessage(message);
        break;
      }
    }
}

//接受信息
void Window::receiveMessage(const Message mes)
{
    QMap<QString, FriendButton*>::iterator iter;
    iter = friendsButtonMap.find(mes.sender);
    qDebug("Find Friend ");
    qDebug() << mes.sender << mes.receiver << mes.text;
    if(friendsButtonMap.end() == iter)
        return ;

    iter.value()->openChatWindow();
    iter.value()->getChatWindow()->setOutputLine(mes);
}

//注册客户端
void Window::registerSlot()
{
    conn->registerClientRequest(myAccount);
    qDebug("register");
}

//创建目录
void Window::createFolder() const
{
    QDir tempDir;
    if(!tempDir.exists("records"))
        tempDir.mkdir("records");
    tempDir.cd("records");
    if(!tempDir.exists(myAccount))
        tempDir.mkdir(myAccount);
}

//与服务器断开连接
void Window::serverDisconnected()
{
    QMessageBox::critical(this, tr("error"),
        tr( "The client is disconnected with the server\n"
        " Quit now"));
    qApp->quit();

}
