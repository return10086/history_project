#include "widget.h"
#include "ui_widget.h"
#include "chart.h"
#include "camera.h"

bool yes_or_no_show=true;
bool define_my_baudrate=true;
Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //只能输入数字
    ui->define_buadrate->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    //隐藏控件
    ui->define_buadrate->setVisible(0);
    QStringList serialNamePort;

    serialport = new QSerialPort(this);
    connect(serialport,SIGNAL(readyRead()),this,SLOT(serialport_readyRead_Slot()));
    foreach(const QSerialPortInfo &inf0,QSerialPortInfo::availablePorts())
    {//自动搜索当前串口并返回到inf0数组
        serialNamePort<<inf0.portName();
    }
    ui->com->addItems(serialNamePort);
}

Widget::~Widget()
{
    delete ui;
}

void Widget:: serialport_readyRead_Slot()
{//显示接收到的数据
    QString buffer;
    buffer=QString(serialport->readAll());
    if(yes_or_no_show)
        ui->plainTextEdit->appendPlainText((buffer));
}
void Widget::on_on_or_off_clicked()
{//按键开关
   QSerialPort::BaudRate baudRate=QSerialPort::Baud115200;
   QSerialPort::DataBits dataBits=QSerialPort::Data8;
   QSerialPort::StopBits stopBits=QSerialPort::OneStop;
   QSerialPort::Parity checkBits=QSerialPort::NoParity;

   if(ui->com->currentText()=="4800")
   {
       baudRate=QSerialPort::Baud4800;
   }
   else if(ui->com->currentText()=="9600")
   {
       baudRate=QSerialPort::Baud9600;
   }
   else if(ui->com->currentText()=="115200")
   {
       baudRate=QSerialPort::Baud115200;
   }

   if(ui->databits->currentData()=="5")
   {
       dataBits=QSerialPort::Data5;
   }
   else if(ui->databits->currentData()=="6")
   {
       dataBits=QSerialPort::Data6;
   }
   else if(ui->databits->currentData()=="7")
   {
       dataBits=QSerialPort::Data7;
   }
   else if(ui->databits->currentData()=="8")
   {
       dataBits=QSerialPort::Data8;
   }

   if(ui->stopbit->currentData()=="1")
   {
       stopBits=QSerialPort::OneStop;
   }
   else if(ui->stopbit->currentData()=="1.5")
   {
       stopBits=QSerialPort::OneAndHalfStop;
   }
   else if(ui->stopbit->currentData()=="2")
   {
       stopBits=QSerialPort::TwoStop;
   }

   if(ui->checkbit->currentData()=="None")
   {
       checkBits=QSerialPort::NoParity;
   }
   else if(ui->checkbit->currentData()=="Even")
   {
       checkBits=QSerialPort::EvenParity;
   }
   else if(ui->checkbit->currentData()=="Odd")
   {
       checkBits=QSerialPort::OddParity;
   }
   else if(ui->checkbit->currentData()=="Mark")
   {
       checkBits=QSerialPort::MarkParity;
   }
   serialport->setPortName(ui->com->currentText());
   serialport->setBaudRate(baudRate);
   serialport->setDataBits(dataBits);
   serialport->setStopBits(stopBits);
   serialport->setParity(checkBits);

   if(serialport->open(QIODevice::ReadWrite)==true)
   {//如果检测到设备了
       QMessageBox::information(this,"提示","打开成功");
   }
   else
   {
       serialport->close();
       QMessageBox::information(this,"提示","关闭成功");
   }

}

void Widget::on_push_button_clicked()
{//发送按钮回调
    serialport->write((ui->push->text().toLocal8Bit().data()));
}

void Widget::on_clear_clicked()
{//清接收屏
    ui->plainTextEdit->clear();
}

void Widget::on_com_currentIndexChanged(const QString &arg1)
{//换串口
    serialport->close();
    serialport->setPortName(arg1);
}

void Widget::on_stop_show_clicked()
{//停止显示
    yes_or_no_show=!yes_or_no_show;
}

void Widget::on_pushButton_clicked()
{//更新串口
    ui->com->clear();
    QStringList serialNamePort;
    foreach(const QSerialPortInfo &inf0,QSerialPortInfo::availablePorts())
    {//自动搜索当前串口并返回到inf0数组
        serialNamePort<<inf0.portName();
    }
    ui->com->addItems(serialNamePort);
}

void Widget::on_pushButton_2_clicked()
{//自定义波特率
    if(define_my_baudrate)
    {
        define_my_baudrate=!define_my_baudrate;
        ui->baudrate->setVisible(0);//隐藏控件
        ui->define_buadrate->setVisible(1);//显示新控件
        QString buffer;
        buffer=QString(ui->define_buadrate->text());

        int buf;
        buf=buffer.toUInt();
        qDebug("%d",buf);
        if(buffer!=0)
            serialport->setBaudRate(buf);
    }
    else
    {
        define_my_baudrate=!define_my_baudrate;
        ui->baudrate->setVisible(1);//隐藏控件
        ui->define_buadrate->setVisible(0);//显示新控件
        int buf;
        buf=ui->baudrate->currentText().toUInt();
        serialport->setBaudRate(buf);
    }
}

void Widget::on_define_buadrate_textChanged(const QString &arg1)
{//更新自定义波特率
    int buf;
    buf=arg1.toUInt();
    serialport->setBaudRate(buf);
}

void Widget::on_databits_currentIndexChanged(int index)
{//换数据位
    serialport->setDataBits((QSerialPort::DataBits)(index+5));
}

void Widget::on_checkbit_currentIndexChanged(int index)
{//换校验位
   int ii=0;
   if(index==0)
       ii=2;
   else if(index==1)
       ii=5;
   else if(index==2)
       ii=0;
   else if(index==3)
       ii=3;
   serialport->setParity((QSerialPort::Parity)ii);
}



void Widget::on_stopbit_currentIndexChanged(int index)
{//换停止位
    int ii=0;
    if(index==0)
        ii=1;
    else if(index==1)
        ii=3;
    else if(index==2)
        ii=2;
    serialport->setStopBits((QSerialPort::StopBits)ii);
}

void Widget::on_baudrate_currentIndexChanged(int index)
{//换波特率


    int ii=0;
    if(index==0)
        ii=1;
    else if(index==1)
        ii=3;
    else if(index==2)
        ii=2;
    serialport->setBaudRate((QSerialPort::BaudRate)ii);
}

void Widget::on_baudrate_currentIndexChanged(const QString &arg1)
{//换波特率
    serialport->setBaudRate(arg1.toInt());
}

void Widget::on_pushButton_3_clicked()
{//换曲线界面
    chart *cha=new chart;
    cha->setGeometry(this->geometry());
    cha->show();
}


void Widget::on_camera_clicked()
{//换摄像头界面
    Camera *cam=new Camera;
    cam->setGeometry(this->geometry());
    cam->show();
}
