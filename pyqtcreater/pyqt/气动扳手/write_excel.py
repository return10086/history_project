'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-10-04 14:21:16
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-10-06 21:10:40
FilePath: \pyqt\气动扳手\write_excel.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''
import os
import openpyxl

path = r""


def write(path, value):
    workbook = openpyxl.load_workbook(path)  # 返回一个workbook数据类型的值
    sheet = workbook['Sheet1']  # 获取指定sheet表

    sheet.append(value)  # 使用append插入数据
    workbook.save(path)  # 保存文件
