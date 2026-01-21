#include "chart.h"
#include "ui_chart.h"

#define MAX_X 50
#define MAX_Y 70

chart::chart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chart)
{
    ui->setupUi(this);
    Chart_Init();
    tim = new QTimer();
    tim->start(100);
    connect(tim,SIGNAL(timeout()),this,SLOT(DrawLine()));
    tim->start();

}

chart::~chart()
{
    delete ui;
}



void chart::Chart_Init()
{
    //初始化QChart的实例
    Chart=new QChart;
    //初始化QSplineSeries的实例
    lineSeries = new QSplineSeries();
    //设置曲线的名称
    lineSeries->setName("随机数测试曲线");
    //把曲线添加到QChart的实力chart中
    Chart->addSeries(lineSeries);
    //声明并初始化x轴、两个y轴
    QValueAxis *axisX=new QValueAxis();
    QValueAxis *axisY=new QValueAxis();
    //设置坐标轴的显示范围
    axisX->setMin(0);
    axisX->setMax(MAX_X);
    axisY->setMin(0);
    axisY->setMax(MAX_Y);
    //设置坐标轴上的格点
    axisX->setTickCount(10);
    axisY->setTickCount(10);
    //设置坐标轴显示的名称
    QFont font("Microsoft YaHei",8,QFont::Normal);//微软雅黑，字体大小8
    axisX->setTitleFont(font);
    axisY->setTitleFont(font);
    axisX->setTitleText("X");
    axisY->setTitleText("Y");
    //设置网格不显示
    axisY->setGridLineVisible(false);
    //下方：QT::AllbgBottom,左边：QT::AlignLeft
    //右边：QT::AlihnRight, 上方：QT::AlignTop
    Chart->addAxis(axisX,Qt::AlignBottom);
    Chart->addAxis(axisY,Qt::AlignLeft);
    //把曲线关联到坐标轴
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
    //把chart显示到窗口上
    ui->graphicsView->setChart(Chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);//设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
}

void chart::DrawLine()
{
    static int count = 0;
    if(count>MAX_X)
    {
        //当曲线上最早的点超出X轴的范围时，提出最早的点
        lineSeries->removePoints(0,lineSeries->count()-MAX_X);
        //更新X轴的范围
        //Y轴用的是Vertical,X轴用的是Horizontal
        Chart->axes(Qt::Horizontal).back()->setMin(count-MAX_X);
        Chart->axes(Qt::Horizontal).back()->setMax(count);
    }
    //增加新的点到曲线末端
    lineSeries->append(count,rand()%65);//随机生成0到65的随机数
    count++;
}


void chart::on_back_clicked()
{
    this->close();
}
