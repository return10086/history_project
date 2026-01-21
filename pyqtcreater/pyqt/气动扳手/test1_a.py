# 先导入主要的三个模块和各自内部常用的类
import typing
from PyQt5 import QtCore, QtWidgets, QtGui, QtSerialPort
from PyQt5.QtCore import pyqtSlot, QStringListModel, QRegExp, QTimer, Qt
from PyQt5.QtWidgets import QApplication, QWidget, QComboBox
from PyQt5.QtGui import QColor, QIntValidator, QDoubleValidator, QRegExpValidator, QImage, QPixmap
from PyQt5.QtMultimedia import (QCameraInfo, QCameraImageCapture,
                                QImageEncoderSettings, QMultimedia, QVideoFrame, QSound, QCamera)
import os
import glob
import serial.tools.list_ports
import Ui_MainWindow as UI
import test1_b
import string
import Ui_camera
import cv2
import string
from PyQt5.QtWidgets import QFileDialog
from write_excel import write
import time
import serial
import struct
yes_or_no_show = True
define_my_baudrate = True
camera_display = False
write_excel = False
hex_display = False
path = ""


def str_to_hex(s):
    # 文本转16进制
    a = ' '.join([hex(ord(c)).replace('0x', '') for c in s])
    a1 = a[0::3]
    a2 = a[1::3]
    a3 = a[2::3]
    sum = []
    for i in range(len(a1)):
        sum.append(a1[i]+a2[i])
    return sum


class mycamera(QtWidgets.QMainWindow, Ui_camera.Ui_MainWindow):
    def __init__(self, parent=None):
        super(mycamera, self).__init__(parent)
        self.setupUi(self)

        # 清除列表
        self.camera_choose.clear()
        # 获取口列表
        plist = QCameraInfo.availableCameras()
        # 如果有
        if len(plist) > 0:
            # 依次把串口放进列表
            for i in range(0, len(plist)):
                plist_0 = plist[i]
                self.camera_choose.addItem(str(i)+':'+plist_0.deviceName())

        self.CAM_NUM = 0
        self.cap = cv2.VideoCapture()
        # 在label中播放视频
        self.init_timer()

    # 播放视频画面
    def init_timer(self):
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.show_pic)

    # 显示视频图像
    def show_pic(self):
        ret, img = self.cap.read()
        if ret:
            cur_frame = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            # 视频流的长和宽
            height, width = cur_frame.shape[:2]
            pixmap = QImage(cur_frame, width, height, QImage.Format_RGB888)
            pixmap = QPixmap.fromImage(pixmap)
            # 获取是视频流和label窗口的长宽比值的最大值，适应label窗口播放，不然显示不全
            ratio = max(width / self.show_window.width(),
                        height / self.show_window.height())
            pixmap.setDevicePixelRatio(ratio)
            # 视频流置于label中间部分播放
            self.show_window.setAlignment(Qt.AlignCenter)
            self.show_window.setPixmap(pixmap)

    @pyqtSlot()
    # 开关摄像头
    def on_on_or_off_clicked(self):
        global camera_display
        if camera_display == False:
            camera_display = True
            # 获取选择的设备名称
            index = self.camera_choose.currentIndex()
            print(index)
            self.CAM_NUM = index
            # 检测该设备是否能打开
            flag = self.cap.open(self.CAM_NUM)
            print(flag)
            if flag is False:
                print("警告", "该设备未正常连接")
            else:
                # 幕布可以播放
                self.show_window.setEnabled(True)
                self.timer.start()
                print("beginning!")
        elif camera_display == True:
            camera_display = False
            self.show_window.setPixmap(QPixmap(""))
            self.show_window.setEnabled(False)
            self.timer.stop()
            self.cap.release()
            print("stop!")

    @pyqtSlot()
    # 返回主界面
    def on_backbutton_clicked(self):
        self.hide()  # 隐藏此窗口
        ui.show()  # 经第二个窗口显示出来


class mywindow(QtWidgets.QMainWindow, UI.Ui_MainWindow):  # 继承两个类

    serialNamePort = QtSerialPort.QSerialPort()  # 新建一个储存串口的

    def __init__(self):
        super(mywindow, self).__init__()
        self.setupUi(self)

        # 隐藏任意波特率控件
        self.baud_rate.setVisible(False)

        # 串口波特率只能输入数字
        reg = QRegExp('[0-9]+$')
        validator = QRegExpValidator()
        validator.setRegExp(reg)
        self.baud_rate.setValidator(validator)

        # 关联读串口数据的槽函数
        self.serialNamePort.readyRead.connect(self.read_data)
        # 关联串口波特率改变的槽函数
        self.baud_rate_choose.currentIndexChanged.connect(self.change_baudrate)
        # 关联串口停止位改变的槽函数
        self.stop_bit.currentIndexChanged.connect(self.change_stop_bit)
        # 关联串口数据位改变的槽函数
        self.data_bit.currentIndexChanged.connect(self.change_data_bit)
        # 关联任意波特率改变的槽函数
        self.baud_rate.textChanged.connect(self.change_any_baudrate)
        # 扫描串口
        self.scanPorts()

    # 任意波特率改变
    def change_any_baudrate(self):
        buffer = self.baud_rate.text()
        buf = int(buffer)
        self.serialNamePort.baudRate = self.serialNamePort.setBaudRate(buf)

    # 改变常备波特率
    def change_baudrate(self):
        buffer = self.baud_rate_choose.currentText()
        buf = int(buffer)
        self.serialNamePort.baudRate = self.serialNamePort.setBaudRate(buf)
    # 改变常备停止位

    def change_stop_bit(self):
        buffer = self.stop_bit.currentText()
        if (buffer == '1'):
            self.serialNamePort.dataBits = self.serialNamePort.setStopBits(
                self.serialNamePort.StopBits.OneStop)
        elif (buffer == '1.5'):
            self.serialNamePort.dataBits = self.serialNamePort.setStopBits(
                self.serialNamePort.StopBits.OneAndHalfStop)
        elif (buffer == '2'):
            self.serialNamePort.dataBits = self.serialNamePort.setStopBits(
                self.serialNamePort.StopBits.TwoStop)

    # 改变常备数据位
    def change_data_bit(self):
        buffer = self.data_bit.currentText()
        buf = int(buffer)
        self.serialNamePort.dataBits = self.serialNamePort.setDataBits(buf)

    # 扫描串口
    def scanPorts(self):
        # 清除串口列表
        self.com.clear()
        # 获取口列表
        plist = list(serial.tools.list_ports.comports())
        # 如果有串口
        if len(plist) > 0:
            # 依次把串口放进列表
            for i in range(0, len(plist)):
                plist_0 = list(plist[i])
                self.com.addItem(str(plist_0[0]))

    # 读串口内容的槽函数
    def read_data(self):

        global write_excel, write_line, hex_display
        buffer = self.serialNamePort.readAll()

        if (buffer):
            hex_data = str(buffer.data().hex())
            data = str(buffer.data())

        if hex_display:
            if (yes_or_no_show):
                self.show_window.append(hex_data)
        else:
            if (yes_or_no_show):
                self.show_window.append(data)

        if write_excel == True:
            if len(buffer) == 24:
                DataType = "<BBifffhhh"
                Recv_Data = struct.unpack(DataType, buffer.data())
                print(123)
                if (Recv_Data[0] == 0xFF and Recv_Data[1] == 0xF1):
                    write(path=path, value=Recv_Data[2:])

    @pyqtSlot()
    # 发送
    def on_send_clicked(self):
        self.serialNamePort.write(
            self.send_window.text().encode('utf-8'))  # 把句学进行类型设置
        print('发送成功')

    @pyqtSlot()
    # 摄像头
    def on_camera_clicked(self):
        # 打开电脑摄像头
        self.hide()  # 隐藏此窗口
        s.show()  # 经第二个窗口显示出来
        print('打开摄像头')

    @pyqtSlot()
    # 开关
    def on_on_or_off_clicked(self):
        # 初始化数据
        self.serialNamePort.portName = self.serialNamePort.setPortName(
            self.com.currentText())
        self.serialNamePort.baudRate = self.serialNamePort.setBaudRate(
            int(self.baud_rate_choose.currentText()))
        self.serialNamePort.dataBits = self.serialNamePort.setDataBits(
            self.serialNamePort.DataBits.Data8)
        self.serialNamePort.stopBits = self.serialNamePort.setStopBits(
            self.serialNamePort.StopBits.OneStop)
        if (self.serialNamePort.open(QtCore.QIODevice.OpenModeFlag.ReadWrite) == True):
            self.on_or_off.setStyleSheet("background-color:rgb(0,128,0)")
            print('打开成功')
        else:
            self.on_or_off.setStyleSheet("background-color:white")
            self.serialNamePort.close()
            print('打开失败')

    @pyqtSlot()
    # 清接收屏
    def on_clear_clicked(self):
        self.show_window.clear()

    @pyqtSlot()
    # 换串口
    def on_com_currentIndexChanged(self):
        self.serialNamePort.portName = self.serialNamePort.setPortName(
            self.com.currentText())

    @pyqtSlot()
    # 停止/开启显示
    def on_stop_show_clicked(self):
        global yes_or_no_show
        if (yes_or_no_show == True):
            yes_or_no_show = False
        elif (yes_or_no_show == False):
            yes_or_no_show = True

    @pyqtSlot()
    # 任意波特率
    def on_pen_pic_clicked(self):
        global define_my_baudrate
        if define_my_baudrate == True:
            define_my_baudrate = False
            self.baud_rate_choose.hide()
            self.baud_rate.show()
            buffer = self.baud_rate.text()
            print(buffer)
            if len(buffer) != 0:
                buf = int(buffer)
                self.serialNamePort.baudRate = self.serialNamePort.setBaudRate(
                    buf)
        elif define_my_baudrate == False:
            define_my_baudrate = True
            self.baud_rate.hide()
            self.baud_rate_choose.show()
            buffer = self.baud_rate_choose.currentText()
            buf = int(buffer)
            self.serialNamePort.baudRate = self.serialNamePort.setBaudRate(buf)

    @pyqtSlot()
    # 选择excel
    def on_choose_path_clicked(self):
        global write_excel
        filename, fileType = QFileDialog.getOpenFileName(
            None, '选取文件', os.getcwd(), "*.xlsx;")
        print(fileType)
        self.show_path.setText(filename)

        global path
        path = self.show_path.toPlainText()
        if path != '':
            write_excel = True
        send=["Triger","angle","OutSpeed","InputSpeed","ax","ay","az"]
        write(path=path, value=send)
        
    @pyqtSlot()
    # 十六进制显示
    def on_hex_display_clicked(self):
        global hex_display
        if hex_display == False:
            hex_display = True
        else:
            hex_display = False


if __name__ == "__main__":
    # 这里的代码逻辑基本相同
    app = QApplication([])  # 先建立一个app
    ui = mywindow()  # 初始化一个对象，调用init函数，已加载设计的ui文件
    s = mycamera()  # 摄像头界面
    ui.show()  # 显示这个ui
    app.exec_()  # 执行app（运行界面，响应按钮等操作）
