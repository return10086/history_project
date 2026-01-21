#include "widget.h"
#include "ui_widget.h"
#include "QString"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpServer=new QTcpServer;
    tcpSocket=new QTcpSocket;


    //一旦tcpServer产生新的连接就执行newConnection_Slot函数
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection_Slot()));
}

void Widget::newConnection_Slot()
{
    //获得已经连接的客户端的Socket
    tcpSocket = tcpServer->nextPendingConnection();
    //客户端的Socket有可读的东西，那么服务端就执行readyRead_Slot
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
}

void Widget::readyRead_Slot()
{
    QString buf;
    buf=tcpSocket->readAll();
    ui->recvEdit->appendPlainText(buf);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_open_button_clicked()
{
    //tcpServer监听端口
    tcpServer->listen(QHostAddress::Any,ui->portEdit->text().toUInt());
}

void Widget::on_close_button_clicked()
{
    tcpServer->close();
}

void Widget::on_send_button_clicked()
{
    tcpSocket->write(ui->sendEdit->text().toLocal8Bit().data());
}
