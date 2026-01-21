#可以输出字符串

print('hello world')
print("hello world")

#含运算符的表达式
print(3+1)

#将数据输出到文件中。
#注意点：
#1、所指定的盘存在
#2、使用file=fp
fp=open('D:/text.txt','a+')#如果文件不存在就创建，存在就在文件内容后面继续追加
print('helloworld',file=fp)
fp.close()

#不进行换行输出（输出在一行中）
print('hello','world','python')