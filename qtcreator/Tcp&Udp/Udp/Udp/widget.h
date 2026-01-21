#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QUdpSocket"
#include "QHostAddress"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QUdpSocket *qUdpSocket;
private slots:
    void on_open_button_clicked();
    void readyRead_SLOT();
    void on_send_button_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
