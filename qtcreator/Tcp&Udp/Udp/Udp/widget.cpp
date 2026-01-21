#include "widget.h"
#include "ui_widget.h"
#include "QString"
#include "QMessageBox"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qUdpSocket = new QUdpSocket(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::readyRead_SLOT()
{
    while(qUdpSocket->hasPendingDatagrams())
    {
        QByteArray array;
        array.resize(qUdpSocket->pendingDatagramSize());
        qUdpSocket->readDatagram(array.data(),array.size());

        QString buf;
        buf=array.data();
        ui->recvEdit->appendPlainText(buf);
    }
}
void Widget::on_open_button_clicked()
{
    if(qUdpSocket->bind(ui->localPort->text().toUInt()))
    {
        QMessageBox::information(this,"提示","成功");
    }
    else
    {
        QMessageBox::information(this,"提示","失败");
    }
    connect(qUdpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_SLOT()));
}

void Widget::on_send_button_clicked()
{
    quint16 port;
    QString sendbuff;
    QHostAddress address;
    address.setAddress(ui->tar_IP->text());
    sendbuff = ui->sendEidt->text();
    port=ui->aimPort->text().toUInt();
    qUdpSocket->writeDatagram(sendbuff.toLocal8Bit().data(),sendbuff.length(),address,port);
}

void Widget::on_close_button_clicked()
{
    qUdpSocket->close();
}
