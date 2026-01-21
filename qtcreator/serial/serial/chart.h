#ifndef CHART_H
#define CHART_H

#include <QDialog>
#include "QtCharts/QChartView"
#include "QtCharts/QSplineSeries"
#include "QValueAxis"
#include "widget.h"
#include "QTimer"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class chart;
}

class chart : public QDialog
{
    Q_OBJECT

public:
    explicit chart(QWidget *parent = nullptr);
    ~chart();

    QChart *Chart;
    QSplineSeries *lineSeries;

private slots:

    void Chart_Init();

    void DrawLine();

    void on_back_clicked();

private:
    Ui::chart *ui;
    QTimer *tim;
};

#endif // CHART_H
