#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QTcpServer"
#include "QTcpSocket"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

private slots:
    void on_open_button_clicked();
    void newConnection_Slot();
    void readyRead_Slot();
    void on_close_button_clicked();

    void on_send_button_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
