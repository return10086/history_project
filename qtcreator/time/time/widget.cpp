#include "widget.h"
#include "./ui_widget.h"
#include "QLabel"
static int cnt=0;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout_SLOT()));

    time.setHMS(0,0,0,0);
    ui->showtime->setText("00:00:00:000");

    current_date_time=current_date_time.currentTime();
    ui->showtime_2->setText(current_date_time.toString("hh:mm:ss.zzz"));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::timeout_SLOT()
{
    //qDebug("123");
    time=time.addMSecs(40);
    ui->showtime->setText((time.toString("hh:mm:ss.zzz")));

    current_date_time=current_date_time.currentTime();
    ui->showtime_2->setText(current_date_time.toString("hh:mm:ss.zzz"));

}

void Widget::on_start_clicked()
{
    timer.start(40);//单位毫秒 50ms时会触发timeout信号
}


void Widget::on_close_clicked()
{
    timer.stop();
}


void Widget::on_reset_clicked()
{
    time.setHMS(0,0,0,0);
    ui->showtime->setText("00:00:00:000");

    ui->bit_time->clear();
}


void Widget::on_bit_clicked()
{
    cnt++;
    QString temp;
    temp.asprintf("%d:",cnt);
    ui->bit_time->append(temp);

    ui->bit_time->append((time.toString("hh:mm:ss.zzz")));
}

