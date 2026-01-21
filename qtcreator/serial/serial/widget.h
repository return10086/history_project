#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include "qserialport.h"
#include "QSerialPortInfo"
#include "QMessageBox"
#include "QString"
#include "stdlib.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QSerialPort *serialport;
private slots:
    void on_on_or_off_clicked();

    void serialport_readyRead_Slot();
    void on_push_button_clicked();

    void on_clear_clicked();

    void on_com_currentIndexChanged(const QString &arg1);

    void on_stop_show_clicked();



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_define_buadrate_textChanged(const QString &arg1);

    void on_databits_currentIndexChanged(int index);

    void on_checkbit_currentIndexChanged(int index);

    void on_stopbit_currentIndexChanged(int index);

    void on_baudrate_currentIndexChanged(int index);

    void on_baudrate_currentIndexChanged(const QString &arg1);

    void on_pushButton_3_clicked();


    void on_camera_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
