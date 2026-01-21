#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qtcpSocktet=new QTcpSocket(this);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::readyRead_Slot()
{
    qDebug("接收到数据");
    ui->recvEdit->appendPlainText((qtcpSocktet->readAll()));
}
void Widget:: connected_Slot()
{
    connect(qtcpSocktet,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
    qDebug("打开成功");
}
void Widget::on_open_button_clicked()
{
    qtcpSocktet->connectToHost(ui->IP->text(),ui->port->text().toUInt());
    connect(qtcpSocktet,SIGNAL(connected()),this,SLOT(connected_Slot()));
    qDebug("打开");
}

void Widget::on_send_button_clicked()
{
    qtcpSocktet->write(ui->sendEidt->text().toLocal8Bit().data());
}
