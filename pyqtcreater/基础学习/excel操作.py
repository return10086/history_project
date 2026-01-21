'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-09-28 19:28:24
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-10-03 23:34:14
FilePath: \python2\基础学习\excel操作.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''
import os
import openpyxl

path = r"E:\project\python2\基础学习"
os.chdir(path)  # 修改工作路径

workbook = openpyxl.load_workbook('test_excel.xlsx')  # 返回一个workbook数据类型的值
print(workbook.sheetnames)  # 打印Excel表中的所有表


# 结果：
# ['Sheet1', 'Sheet2']

sheet = workbook['Sheet1']  # 获取指定sheet表
print(sheet)
# 结果：
# ['Sheet1', 'Sheet2']
# <Worksheet "Sheet1">

cell1 = sheet['A1']         # 获取A1单元格的数据
cell2 = sheet['B7']         # 获取B7单元格的数据
# cell2 = sheet['B7'].value		# 另一种写法

# 正确示范
# cell1.value获取单元格A1中的值
# cell2.value获取单元格B7中的值
print(cell1.value,cell2.value)  # 姓名 18

# 错误示范
print(cell1,cell2)  # <Cell 'Sheet1'.A1> <Cell 'Sheet1'.B7>
