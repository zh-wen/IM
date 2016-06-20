#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QDateTime>
#include<QHostAddress>
#include<QNetworkInterface>
#include<QDir>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

   //Display System Time
    QTimer *timer = new QTimer(this);
    displayTime();
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    connect(ui->quitButton,SIGNAL(clicked()),this, SLOT(close()));
    timer->start(1000);

    //Display ipAddress
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    ui->localIpLabel->setText( ipAddress);
    server = new Server(this);
    setWindowTitle("Server");
}

Widget::~Widget()
{
    delete ui;
}

//显示时间
void Widget::displayTime()
{
    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("yyyy:MM:dd:hh:mm:ss");
    ui->timeLabel->setText(currentDate);
}

//创建目录
void Widget::createFolder() const
{
    QDir tempDir;
    if(!tempDir.exists("friendsInfo"))
        tempDir.mkdir("friendsInfo");
}


