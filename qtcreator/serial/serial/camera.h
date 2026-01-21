#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>

//摄像头
#include <QCameraImageCapture>
#include <QCamera>
namespace Ui {
class Camera;
}

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();

private slots:
    void on_shoot_clicked();

    void on_back_clicked();

private:
    Ui::Camera *ui;

    //创建两个摄像头对象
    QCamera *ca;
    QCameraImageCapture *capture;
};

#endif // CAMERA_H
