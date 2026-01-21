'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-10-23 00:13:09
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-11-01 03:40:04
FilePath: \设计与建造\match.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''
import cv2
import numpy as np
import serial
from mousroi import DrawRects, onmouse_draw_rect
import write_excel

# 读取模板图像
imga = cv2.imread('img1.jpg')
imga=cv2.cvtColor(imga,cv2.COLOR_BGR2GRAY)
imgb=cv2.imread('img2.jpg')
imgb=cv2.cvtColor(imgb,cv2.COLOR_BGR2GRAY)
imgc=cv2.imread('img3.jpg')
imgc=cv2.cvtColor(imgc,cv2.COLOR_BGR2GRAY)

# 使用SIFT算法检测模板图像中的关键点（kp1）和描述符（des1）
sift = cv2.SIFT_create(contrastThreshold=0.04)
kp1, des1 = sift.detectAndCompute(imga, None)
kp2, des2 = sift.detectAndCompute(imgb, None)

# 定义最小匹配点数
MIN_MATCH_COUNT = 100
MAX_MATCH_COUNT = 100
def match_template(draw_rects,img,source_img):
    global sift, kp1, des1, MIN_MATCH_COUNT,kp2,des2,imga,imgb
    
    catch=False
    if source_img==1:
        source_img=imga
        MIN_MATCH_COUNT=100
        MAX_MATCH_COUNT=200
    elif source_img==2:
        source_img=imgb
        MIN_MATCH_COUNT=0
        MAX_MATCH_COUNT=100
    else:
        source_img=imgc
    draw_rects.original_image = img
    draw_rects.reset_image()
    draw_rects.draw()
    draw_rects.draw_current_rect()

    img2 = img.copy()
    if len(draw_rects.rects) > 0:
        img2 = img2[draw_rects.rects[-1].tl[1]:draw_rects.rects[-1].br[1], draw_rects.rects[-1].tl[0]:draw_rects.rects[-1].br[0]]
    # 使用SIFT算法检测“img2”中的关键点（kp2）和描述符（des2）。如果没有检测到任何特征点，则描述符将设置为“None”。
    kp2, des2 = sift.detectAndCompute(img2, None)

    # 创建FLANN匹配器对象
    FLANN_INDEX_KDTREE = 0
    indexParams = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
    searchParams = dict(checks=80)
    flann = cv2.FlannBasedMatcher(indexParams, searchParams)

    # 使用FLANN匹配器进行特征点匹配
    matches = flann.knnMatch(des1, des2, k=2)

    # 选取最佳匹配点
    good = []
    for m, n in matches:
        if m.distance < 0.7 * n.distance:
            good.append(m)

    '''
    write_excel.write(path="Z:\img2-3.xlsx", value=len(good))
    '''
    # 如果匹配点数大于最小匹配点数，则进行透视变换
   
    if len(good) > MIN_MATCH_COUNT and len(good) < MAX_MATCH_COUNT:
        try:
            src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
            dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)

            M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)

            matchesMask = mask.ravel().tolist()

            size = source_img.shape
            print(size)
            
            h=size[0]
            w=size[1]
            
            
            pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
        
            dst = cv2.perspectiveTransform(pts, M)
            imm = np.zeros(img2.shape[:2], dtype='uint8')
            polygon_mask = cv2.fillPoly(imm, [np.int32(dst)], 255)
            a=np.sum(np.greater(polygon_mask, 0))
            # 如果匹配成功，则在图像中绘制矩形框
            if a > 2000:
                img2 = cv2.polylines(img2, [np.int32(dst)], True, (0, 0, 255), 2, cv2.LINE_AA)
            catch=True

            print("检测到既定目标，机器开始工作")
        except:
            print("")

    else:
        print("Not Enough " + str(len(good)))
        matchesMask = None
        
    return img2,catch,len(good)

'''
cap = cv2.VideoCapture(0)
ret, r_img = cap.read()
draw_rects = DrawRects(r_img, (0, 255, 0), 2)
#img1 130~200
#img2 10~70
#img3 20~80
while True:

    ret, r_img = cap.read()
    r_img,catch1=match_template(draw_rects,r_img,2)
    
    #读像素
    h,w=r_img.shape[:2]
    print(h,w)
    r_img=r_img[160:380,180:460]
    
    cv2.imshow('frame', r_img)
    cv2.waitKey(1)
    
'''