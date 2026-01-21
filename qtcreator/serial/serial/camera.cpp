#include "camera.h"
#include "ui_camera.h"

//调用摄像头
#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>
//弹窗
#include <QMessageBox>

Camera::Camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);

    //获取可用摄像头设备并输出在控制台
    QList<QCameraInfo> infos = QCameraInfo::availableCameras();
    qDebug() << infos.value(0).deviceName() << ":" <<infos.value(0).description();
    QString camera = infos.value(0).deviceName();
    qDebug() << camera;
    //显示摄像头
    ca =new QCamera(camera.toUtf8() ,this );
    ui->camera->show();

    QCameraViewfinder *v2 = new QCameraViewfinder(ui->camera);
    v2->resize(ui->camera->size());
    ca->setViewfinder(v2);
    v2->show();
    ca->start();

}

Camera::~Camera()
{
    delete ui;
}

void Camera::on_shoot_clicked()
{
    //截取用户打卡时的图片
    capture = new QCameraImageCapture(ca);
    capture->capture("E:/sdasfhjksdhfvajkdsl/123.jpg");
    QMessageBox::information(this, "good", "good！");
}

void Camera::on_back_clicked()
{
    this->close();
}
