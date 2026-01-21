name='程佳乐'
age=19
print(type(name),type(age))#说明name与age数据类型不相同
#print('我叫'+name+'今年'+age+'岁')#将str类型与int类型进行连接时报错。解决方案：类型转换
print('我叫'+name+'今年'+str(age)+'岁')
print(age,type(age))#数据类型未改变

print("---------str()将其他类型转换成str类型-----")

a=10#int
b=198.8#float
c=False#bool
d='程佳乐'
print(type(a),type(b),type(c))
print(type(a),type(b),type(c),type(str(a)),type(str(b)),type(str(c)))

print("---------int()将其他类型转换成int类型-----")

print(b,int(b))#截取整数部分，舍掉小数
print(c,int(c))#false为0，true为1
#print(d,int(d))#字符串转换为int()类型时必须为数字串（整数）

print("---------float()将其他类型转换成float类型-----")

print(a,float(a))
print(c,float(c))
#print(d,float(d))#非数字字符串不允许转换为float
