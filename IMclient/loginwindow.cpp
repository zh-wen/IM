#include "loginwindow.h"
#include"registerwin.h"
#include"window.h"
#include "ui_loginwindow.h"
#include<QMessageBox>
#include"QDebug"
LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    //ui->groupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->ipLineEdit->setText(QString("127.0.0.1"));
    ui->portLineEdit->setText(QString("8888"));

    hostAddress = ui->ipLineEdit->text();
    hostPort = ui->portLineEdit->text();
    conn = new Connect(hostAddress,hostPort ,this);
    connect(ui->settingButton,SIGNAL(toggled(bool)),this,SLOT(setHostAddress()));
    connect(ui->loginPushButton, SIGNAL(clicked()),this, SLOT(clickedLogBtn()));
    connect(ui->registerPushButton,SIGNAL(clicked()),this,SLOT(clickedRegisterBtn()));
    connect(conn,SIGNAL(disconnectedSignal()),
            this,SLOT(serverDisconnected()));
    connect(conn,SIGNAL(newReply(qint32)),this,SLOT(readReply(qint32)));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

//点击登录按钮
void LoginWindow::clickedLogBtn()
{
        if(ui->accountLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"),
            tr("The account line can not be empty"));
        return;
    }
    //若服务器地址为空
    if(ui->portLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"),
            tr("The port line can not be empty"));
        return;
    }

    logInfo.account =  ui->accountLineEdit->text();
    logInfo.password = ui->passwordLineEdit->text();

    //发送请求
    if(conn->getConnectStatu())
    {
        conn->loginRequest(logInfo);
    }
    else
    {
        hostAddress = ui->ipLineEdit->text();
        hostPort = ui->portLineEdit->text();
        conn->reConnectToServer(hostAddress,hostPort);
        QMessageBox::critical(this, tr("Error"),
            tr("Connect Server Failed"));
    }
}

//与服务器断开连接
void LoginWindow::serverDisconnected()
{
//    conn->deleteLater();
//    conn = NULL;
    QMessageBox::information(NULL,tr("Error")
                ,tr("Can not connect to server"));
           return ;
}

//处理接受的数据
void LoginWindow::readReply(qint32 replyKind)
{

    switch (replyKind)
    {
    //账号不存在
    case LOGIN_NO_ACCOUNT:
        QMessageBox::information(NULL,tr("Login failed"),
                                 tr("Account not exisit"));
        qDebug("Recive the Message1");
       break;
    //密码错误
    case LOGIN_WORING_PWD:
        QMessageBox::information(NULL,tr("Login failed"),
                                 tr("Password Error"));
        break;
    //账号已登录
    case LOGIN_EXISTS:
        QMessageBox::information(NULL,tr("Login failed"),
                                 tr("Account is logged in"));
        break;
    //登录成功
    case LOGIN_SUCCESS:
        QVector<FriendInfo> friendsVec;
        conn->getFriendsVec(friendsVec);
        qDebug() << friendsVec.size();/*
        for(int i = 0; i < friendsVec.size(); i++)
            qDebug() << friendsVec.at(i).account;*/
        qDebug() << hostAddress << hostPort;
        Window *win = new Window(hostAddress,hostPort,
                                 ui->accountLineEdit->text(),friendsVec,NULL);
        win->show();
        close();
        break;
    }
}

//设置服务器地址
void LoginWindow::setHostAddress()
{
    hostAddress = ui->ipLineEdit->text();
    hostPort = ui->portLineEdit->text();
}

//点击注册按钮
void LoginWindow::clickedRegisterBtn()
{
    RegisterWin *reg = new RegisterWin(conn,NULL);
    reg->show();
}
