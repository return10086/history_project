#range()的三种创建方式
'''第一种创建方式，只有一个参数（小括号中只给了一个数）'''
#range(stop)
r=range(10) #[0,1,2,3,4,5,6,7,8,9] 默认从0开始，默认相差1称为步长
print(r)#range(10)
print(list(r))#用于查看range对象中的整数序列
'''第二种创建方式，给了两个参数'''
#range(start,stop)
a=range(1,10)  #指定了起始值，从1开始，到10结束（不包括10），默认步长为1
print(list(a))
'''第三种创建方式，给了三个参数'''
#range(start,stop,step)
b=range(1,10,2)
print(list(b))#[1, 3, 5, 7, 9]

'''判断指定整数 在序列中是否存在'''
print(10 in r)#False
print(3 in b)#True
print(3 not in b)#False