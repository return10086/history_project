#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTime>
#include<QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTime time;
    QTime current_date_time;
    QTimer timer;

private slots:
    void on_start_clicked();

    void timeout_SLOT();
    void on_close_clicked();

    void on_reset_clicked();

    void on_bit_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
