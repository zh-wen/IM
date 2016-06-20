#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDir>
#include <QDate>
#include <QTime>
#include <QCloseEvent>
#include <QShortcut>

//构造类
ChatWindow::ChatWindow(Connect *conn, const FriendInfo &peer,
                       const QString &acc, QWidget *parent) :
    QWidget(parent),
    conn(conn),peer(peer),
    myAccount(acc),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    QString title = QString("chat with %1").arg(peer.account);
    setWindowTitle(title);
    //设置显示信息
    ui->aboutTextEdit->append(tr("Friend Account :\n") + peer.account);
    ui->aboutTextEdit->append(tr("My Account :\n") + myAccount);
    ui->outputTextEdit->setReadOnly(true);

    //设置头像
    ui->iconFrame->setObjectName("avatar");
    QString iconPath = QString("QFrame#avatar{border-image:url(icons/icon.bmp)}");
    ui->iconFrame->setStyleSheet(iconPath);

    connect(ui->sendPushButton,SIGNAL(clicked()),
            this,SLOT(clickedSendButton()));
    connect(ui->recordPushButton,SIGNAL(clicked()),
            this,SLOT(clickedRecordButton()));
    connect(ui->closePushButton,SIGNAL(clicked()),
            this,SLOT(close()));

}

ChatWindow::~ChatWindow()
{
    delete ui;
}

//发送信息
void ChatWindow::clickedSendButton()
{
    if(ui->inputTextEdit->toPlainText().isEmpty())
        return;
    //获取输入框信息
    sendMessage = ui->inputTextEdit->toPlainText();
    ui->inputTextEdit->clear();
    QString dateTime = getCurrentDateTime();
    QString temp = QString("%1  %2\n%3\n").arg(myAccount).arg(dateTime).arg(sendMessage);
    ui->outputTextEdit->append(temp);

    //发送信息
    Message message;
    message.sender = myAccount;
    message.receiver = peer.account;
    message.text = sendMessage;
    conn->messageRequest(message);
}

//显示聊天信息
void ChatWindow::setOutputLine(const Message &mes)
{
    QString dateTime = getCurrentDateTime();
    QString temp = QString("%1 %2\n%3\n").arg(peer.account).arg(dateTime).arg(mes.text);
    ui->outputTextEdit->append(temp);
}

//打开聊天记录
void ChatWindow::clickedRecordButton()
{
    QString record = readChatRecord();
    QTextEdit *textEdit = new QTextEdit(record, this);
    textEdit->resize(300,500);
    Qt::WindowFlags flag = Qt::Window
            |Qt::WindowCloseButtonHint
            |Qt::WindowMinimizeButtonHint;

    textEdit->setWindowFlags(flag);
    textEdit->setReadOnly(true);
    textEdit->show();
}

//保存聊天记录
void ChatWindow::saveChatRecord() const
{
    //打开文件  
    QString filePath = QString("./records/%1/%2.txt")
            .arg(myAccount).arg(peer.account);
    filePath = QDir::toNativeSeparators(filePath);
    QFile file(filePath);
    bool isOpen = file.open(QIODevice::WriteOnly|QIODevice::Append);

    if(!isOpen)
    {
        file.close();
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    if(ui->outputTextEdit->toPlainText().isEmpty())
        return;

    //保存信息
    QString chatRecord = ui->outputTextEdit->toPlainText();
    out << chatRecord;
    file.close();
}

//读取聊天记录
QString ChatWindow::readChatRecord()
{
    //打开文件
    QString allRecord;
    QString filePath = QString("./records/%1/%2.txt")
       .arg(myAccount).arg(peer.account);
    filePath = QDir::toNativeSeparators(filePath);
    QFile file(filePath);
    bool isOpen = file.open(QIODevice::ReadOnly);

    //如果读取聊天记录失败，就直接返回输入栏的字符
    if(!isOpen)
        return ui->outputTextEdit->toPlainText();

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);

    //读取信息
    QString tempString;
    while(!file.atEnd())
    {
        in >> tempString;
        allRecord += tempString;
    }
    file.close();
    allRecord += ui->outputTextEdit->toPlainText();
    return allRecord;
}

//获取当前时间
QString ChatWindow::getCurrentDateTime()
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();

    return QString("%1  %2").arg(date.toString(Qt::ISODate))
        .arg(time.toString(Qt::ISODate));
}

//重载关闭事件
void ChatWindow::closeEvent(QCloseEvent *event)
{
    //关闭聊天窗口时，保存聊天记录并发出退出信号
    emit ChatWindowQuit();
    saveChatRecord();

    //    disconnect(conn, SIGNAL(newReply(qint32)),this,SLOT());
    event->accept();
}
