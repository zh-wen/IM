#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"server.h"
namespace Ui {
class Widget;
}

//主窗口
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
	//显示时间
    void displayTime();

private:
    Ui::Widget *ui;
    Server *server;
    //创建文件夹
    void createFolder() const;
};

#endif // WIDGET_H
