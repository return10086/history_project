lit=['hello','world',98]
print(id(lit))
print(type(lit))
print(lit)


#列表的创建
liebiao1=[123,'shit',123,'123']
liebiao2=list([123,1234])

#列表的引用
print(liebiao1[0])
print(liebiao1[-1])#倒数第一

#列表的查询
print(liebiao1.index('123'))
liebiao3=[10,20,30,40,50,60,70,80]

print('原列表',id(liebiao3))
liebiao4=liebiao3[0:6:1]#左闭右开#start(默认为1） end（默认到底）    step（默认为1）
print('切的列表',id(liebiao4))


liebiao5=liebiao1[::-1]#步长为负数，切出列表第一个元素是列表的最后一个元素
print(liebiao5)
liebiao5=liebiao3[8:0:-1]
print(liebiao5)

#判断对象在列表中是否存在
print(123 in liebiao2)#True
for item in liebiao2:
    print(item)

#向列表添加元素
lst=[1,2,3]
lst.append(100)#在列表尾添加元素
lst2=['123','456']
lst.append(lst2)#将lst2做一个元素添加至列表尾
print(lst)