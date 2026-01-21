'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-10-17 23:36:31
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-11-20 20:00:51
FilePath: \设计与建造\Main_Window.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''
'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-10-17 23:36:31
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-10-23 05:21:38
FilePath: \设计与建造\Main_Window.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''

import typing
from PyQt5 import QtCore, QtWidgets, QtGui, QtSerialPort
from PyQt5.QtCore import pyqtSlot, QStringListModel, QRegExp, QTimer, Qt
from PyQt5.QtWidgets import QApplication, QWidget, QComboBox
from PyQt5.QtGui import QColor, QIntValidator, QDoubleValidator, QRegExpValidator, QImage, QPixmap
from PyQt5.QtMultimedia import (QCameraInfo, QCameraImageCapture,
                                QImageEncoderSettings, QMultimedia, QVideoFrame, QSound, QCamera)

import Ui_designed_and_build as UI
import socket
import threading
import time
import numpy as np
import cv2
from mousroi import DrawRects, onmouse_draw_rect
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

from threading import Thread
from time import sleep, ctime
from tkinter import filedialog
from PIL import Image, ImageEnhance
import struct
import match



begin_data = b'Frame Begin'
end_data = b'Frame Over'


# 接收端用的
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('172.20.10.6', 1347))
server.listen(5)
temp_data = b''
receive_data = []
r_img = cv2.imread('robot.jpg')
matrix = [0 for i in range(100)]
matrix_num = 0

x1 = y1 = -1
flag = True

server2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server2.bind(('172.20.10.6', 163))
server2.listen(5)

message = 'stop0'
scope=50
receive_fine = False
Tomove = False
allow_to_send=False

has_reach=True
class mywindow(QtWidgets.QMainWindow, UI.Ui_MainWindow):  # 继承两个类

    
    frame=[]
    #使用DrawRects类在摄像头捕捉的画面中绘制矩形框，框选特定目标图像区域
    draw_rects = DrawRects(frame, (0, 255, 0), 2)
    cv2.namedWindow('m', 0)
    cv2.setMouseCallback('m', onmouse_draw_rect, draw_rects)
    img_count=1
    color_count=1
    def __init__(self):
        super(mywindow, self).__init__()
        self.setupUi(self)
        size = self.img.size()
        self.img.setIconSize(size)
        self.img.setIcon(QIcon('img1.jpg'))
        #设置按键背景颜色
        self.color_choose.setStyleSheet("background-color: rgb(128,0,0)")
        color_count=1
        self.img_count=1
        
    def mouse_drawing(self,event, x, y, flags, param):
        global x1, y1, flag
        if event == cv2.EVENT_LBUTTONDOWN:  # 左键按下
            x1 = x
            y1 = y
            print("("+str(x1)+","+str(y1)+")")
            flag = False
     
    def nothing(self,x):
        print(x)  # x是拖动条的值
    
    # 图片解析函数
    def rec(self):
        global temp_data, receive_data, r_img,message,allow_to_send,scope,matrix_num,matrix,flag,has_reach
        #打开摄像头
        cap = cv2.VideoCapture(0)
        ret, frame = cap.read()
        success=5
        
        #选择圈的颜色
        while (flag):
            ret, frame = cap.read()
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            cv2.namedWindow("blue", cv2.WINDOW_AUTOSIZE)
            cv2.setMouseCallback("blue", self.mouse_drawing)
            cv2.imshow("blue", frame)
            cv2.waitKey(1)
        flag=True
        bh, bs, bv = hsv[y1, x1]
        #删除窗口blue
        cv2.destroyWindow("blue")
        
        while (flag):
            ret, frame = cap.read()
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            cv2.namedWindow("red", cv2.WINDOW_AUTOSIZE)
            cv2.setMouseCallback("red", self.mouse_drawing)
            cv2.imshow("red", frame)
            cv2.waitKey(1)
        flag=True
        rh, rs, rv = hsv[y1, x1]
        #删除窗口red
        cv2.destroyWindow("red")

        t1 = int(round(time.time() * 1000))
        while has_reach:
            #做图像基础处理
            ret, r_img = cap.read()
            t2 = int(round(time.time() * 1000))
            fps = 1000//(t2-t1)
            cv2.putText(r_img, "FPS" + str(fps), (300, 50),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
            t1 = t2

            #捕捉图像
            r_img,catch1,mold=match.match_template(self.draw_rects,r_img,self.img_count)
            
            if self.color_count%2==0:#蓝色
                h, s, v = bh,bs,bv
            elif self.color_count%2==1:#红色
                h, s, v = rh,rs,rv
            
            
            #画圆
            img2 = cv2.cvtColor(r_img, cv2.COLOR_BGR2HSV)  # 转化为hsv的的图
            lower = np.array([h-scope, s-scope, v-scope])  # 红色下限
            upper = np.array([h+scope, s+scope, v+scope])  # 红色上限
            mask = cv2.inRange(img2, lower, upper)  # 二值化 
            mask2 = cv2.medianBlur(mask, 7)  # 高斯滤波
            cv2.imshow('mask', mask2)
            cv2.waitKey(1)
            circles = cv2.HoughCircles(  # 霍夫变换
                mask2, cv2.HOUGH_GRADIENT, 1, 30, param1=100, param2=10, minRadius=70, maxRadius=120)
            if circles is not None:
                circles = np.uint16(np.around(circles))  # 取整
                for i in circles[0, :]:
                    matrix[matrix_num % 10] = i[2]
                    matrix_num += 1
                    # 在原图上画圆,圆心,半径,颜色,线框
                    cv2.circle(r_img, (i[0], i[1]), i[2],
                            (0, 255, 0), 2)
                    # 拼字符串
                    info = "r="+str(sum(matrix)/10)
                    # 写文字：坐标，字型，字大小，线宽
                    cv2.putText(r_img, info, (50, 50),
                                cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 255, 0), 2, 1)
                    # 画圆心
                    cv2.circle(r_img, (i[0], i[1]), 2, (0, 0, 255), 3)
            
            #给单片机发消息
            print("success=",str(success))
            if catch1 and circles is not None:
                success-=1
                if success<=0:
                    message = 'move0'
                    allow_to_send=True
            else :
                message='NULL'
            allow_to_send=True
            
            #改上位机上的字
            if mold<100:
                self.mold.setText('%d' %2)
            elif mold>100 and mold<200:
                self.mold.setText('%d' %1)
            else:
                self.mold.setText('%s' %"NULL")
                
            #检测蓝色检测到了
            if self.color_count%2==0 and circles is not None:#蓝色
                self.color.setText('%s' %"蓝色")
            #检测红色检测到了
            elif self.color_count%2==1 and circles is not None:#红色
                self.color.setText('%s' %"红色")
            #检测蓝色没监测到
            elif self.color_count%2==0 and circles is None:#红色
                self.color.setText('%s' %"红色")
            #检测红色没监测到
            elif self.color_count%2==1 and circles is None:#蓝色
                self.color.setText('%s' %"蓝色")
            else:
                self.color.setText('%s' %"NULL")
        
        
            #显示在上位机
            cur_frame = cv2.cvtColor(r_img, cv2.COLOR_BGR2RGB)
            # 视频流的长和宽
            height, width = cur_frame.shape[:2]
            pixmap = QImage(cur_frame, width, height, QImage.Format_RGB888)
            pixmap = QPixmap.fromImage(pixmap)
            # 获取是视频流和label窗口的长宽比值的最大值，适应label窗口播放，不然显示不全
            ratio = max(width / self.label.width(),
                        height / self.label.height())
            pixmap.setDevicePixelRatio(ratio)
            # 视频流置于label中间部分播放
            self.label.setAlignment(Qt.AlignCenter)
            self.label.setPixmap(pixmap)
            


    # 给单片机发数据
    def sen(self, sock2, addr2):
        global message,allow_to_send
        while True:
            if allow_to_send:
                try:
                    allow_to_send=False
                    sock2.sendall(message.encode())
                except:
                    pass
    # 接收单片机数据
    def rec_controler(self, sock2, addr2):
        global has_reach
        while True:
            try:
                data = sock2.recv(1)
                if data is not None:
                    DataType = "<B"
                    Recv_Data = struct.unpack(DataType, data)
                    if Recv_Data[0]==1:
                        has_reach=True
            except:
                pass

    # 接收线程

    def handle_sock_rec(self):
        receive = threading.Thread(
            target=self.rec)
        self.camera.setStyleSheet("background-color: rgb(0,128,0)")
        receive.start()

    # 发送线程
    def handle_sock_sen(self):
        global send
        sock2, addr2 = server2.accept()
        print('Connect--{}'.format(addr2))
        
        sock2.setblocking(False)
            
        # 连接成功后开一个线程用于处理客户端
        send = threading.Thread(target=self.sen, args=(sock2, addr2))
        self.controler.setStyleSheet("background-color: rgb(0,128,0)")
        send.start()
        
        # 开启一个线程用来接收客户端信息
        receive = threading.Thread(target=self.rec_controler, args=(sock2, addr2))
        receive.start()

    # 槽函数
    @pyqtSlot()
    def on_Tobegin_clicked(self):
        t1 = Thread(target=self.handle_sock_rec)
        t2 = Thread(target=self.handle_sock_sen)

        t1.start()
        t2.start()

    # 槽函数
    @pyqtSlot()
    def on_Tomove_clicked(self):
        global message, send,allow_to_send
        message = 'move0'
        allow_to_send=True

    @pyqtSlot()
    def on_img_clicked(self):
        if self.img_count==1:
            size = self.img.size()
            self.img.setIconSize(size)
            self.img.setIcon(QIcon('img2.jpg'))
            self.img_count=2
        elif self.img_count==2:
            size = self.img.size()
            self.img.setIconSize(size)
            self.img.setIcon(QIcon('img1.jpg'))
            self.img_count=1
        elif self.img_count==3:
            size = self.img.size()
            self.img.setIconSize(size)
            self.img.setIcon(QIcon('img1.jpg'))
            self.img_count=1
    @pyqtSlot()
    def on_color_choose_clicked(self):
        self.color_count+=1
        if self.color_count%2==0:
            self.color_choose.setStyleSheet("background-color: rgb(0,0,128)")
        elif self.color_count%2==1:
            self.color_choose.setStyleSheet("background-color: rgb(128,0,0)")

if __name__ == "__main__":
    # 这里的代码逻辑基本相同
    app = QApplication([])  # 先建立一个app
    ui = mywindow()  # 初始化一个对象，调用init函数，已加载设计的ui文件
    ui.show()  # 显示这个ui
    app.exec_()  # 执行app（运行界面，响应按钮等操作）
