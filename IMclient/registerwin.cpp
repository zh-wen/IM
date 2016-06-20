#include "registerwin.h"
#include "ui_registerwin.h"
#include<QRegExp>
RegisterWin::RegisterWin(Connect *conn,QWidget *parent) :
    QWidget(parent),
    conn(conn),
    ui(new Ui::RegisterWin)
{
    //初始化界面
    ui->setupUi(this);
    setWindowTitle(tr("Register new user"));
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmPwdLineEdit->setEchoMode(QLineEdit::Password);

    connect(conn,SIGNAL(newReply(qint32)),this,SLOT(readResult(qint32)));
    connect(ui->registerButton,SIGNAL(clicked()),this,SLOT(clickedRegiserButton()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(clickedQuitButton()));
}

RegisterWin::~RegisterWin()
{
    delete ui;
}

//点击取消按钮
void RegisterWin::clickedQuitButton()
{
    close();
}

//点击注册按钮
void RegisterWin::clickedRegiserButton()
{
    if(checkUserInfo())
    {
        userInfo.account = ui->accountLineEdit->text();
        userInfo.password = ui->passwordLineEdit->text();
        conn->registerRequest(userInfo);
    }
    else
    {
        return ;
    }
}

//处理接受的数据
void RegisterWin::readResult(qint32 replyKind)
{
    if(REGISTER_EXIST == replyKind)
        QMessageBox::critical(this, tr("Error"), tr("This user name has been registered"));
    else if(REGISTER_SUCCESS == replyKind)
    {
        QMessageBox::information(this, tr("Success"),tr("registration successful"));
        close();
    }
}

//检查输入的数据
bool RegisterWin::checkUserInfo()
{
    QRegExp regExp;
    regExp.setPattern("^[A-Za-z0-9]{6,20}$");
    if(!regExp.exactMatch(ui->passwordLineEdit->text()))
    {

        QMessageBox::critical(this, tr("Error"),
            tr("password's Format is wrong. Please try again"));
        return false;
    }

    if(ui->passwordLineEdit->text() != ui->confirmPwdLineEdit->text())
    {

        QMessageBox::critical(this, tr("Error"),
            tr("Two  input passwords are not the same. Please try again"));
        return false;
    }
    return true;
}
