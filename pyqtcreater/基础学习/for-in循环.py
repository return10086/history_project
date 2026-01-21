#for-in用于可迭代对象（字符串/整数序列

for item in "Python":#第一次取出来的是P，赋值给item，将item输出
    print(item)

#range()产生一个整数序列，也是一个可迭代对象
for i in range(10):
    print(i)

#如果在循环体中不需要使用到自定义变量，可将自定义变量写为"_"
for _ in range(5):
    print("老子要做赵匡胤")#循环五次

#计算1到100的偶数和
sum=0
for item in range(2,101,2):
    sum+=item;

print(sum)

#输出100到999之间的水仙花数

for item in range(100,1000):
    ge=item%10
    shi=item//10%10
    bai=item//100
    if ge**3+shi**3+bai**3==item:
        print(item)