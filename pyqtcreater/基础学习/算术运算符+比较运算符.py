print('-----------算术运算符-----------')
print(1+1)#加法运算
print(1-1)#减法运算
print(2*2)#乘法运算
print(2/2)#除法运算
print(11//5)#整除运算
print(11%2)#取余数运算
print(2**3)#模运算，2^3

print(11//5)#2
print(-11//-5)#2
print(-11//5)#-3
print(11//-5)#-3
#一正一负整除向下取整
print(-11%5)#被除数（-11）/除数（5）=商（-3）+余数（4）

#赋值运算符
#执行顺序：从右至左
i=3+4
print(i)
print('-----------支持链式赋值-----------')
a=b=c=20
print(a,id(a))
print(b,id(b))
print(c,id(c))#id指示相同
print('-----------支持参数赋值-----------')
a=20
a+=30  #a=a+30
print(a,type(a))
a*=2   #a=a*2
print(a,type(a))
a/=3   #a=a/3
print(a,type(a))
a//=3  #a=a//3
print(a,type(a))

print('--------支持系列解包赋值赋值--------')
a,b,c=20,30,40
print(a,b,c)
print('--------交换两个变量的值--------')
a,b=b,a
print(a,b,c)

print('--------比较运算符--------')
print("a<b吗",a<b)#false
print("a>b吗",a>b)#ture
print("a>=b吗",a>=b)#false
print("a<=b吗",a<=b)#ture
print("a=b吗",a==b)#false
print("a!=b吗",a!=b)#ture
#比较对象的值用==
#比较对象的标识用is
a=10
b=10
print(a==b)   #ture  a与b的值相同
print(a is b) #ture  a与b的id相同

list1=[11,22,33,44]
list2=[11,22,33,44]
print(list1==list2)#ture  储值相同
print(list1 is list2)#false  id不相同
print(list1 is not list2)#ture
print('--------布尔运算符--------')
a,b=1,2
print('--------and--------')
print(a==1 and b==2)#ture
print(a==1 and b<2)#false
print(a!=1 and b==2)#false
print(a!=1 and b!=2)#false
print('--------or--------')
print(a==1 or b==2)#ture
print(a==1 or b<2)#ture
print(a!=1 or b==2)#ture
print(a!=1 or b!=2)#false
print('--------not--------')
f1=True
f2=False
print(not f1)
print(not f2)
print('--------in与not in--------')
w='hello world'
print('w' in w)#ture
print('k' in w)#false
print('w' not in w)#false
print('k' not in w)#ture
