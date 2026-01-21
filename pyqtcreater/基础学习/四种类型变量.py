#整数类型       int
#有二/八/十/十六进制，默认十进制
#可以表示正数负数和0
n1=90
n2=-70
n3=0
print(n1,type(n1))
print(n2,type(n2))
print(n3,type(n3))
print('十进制',118)
print('二进制',0b1101011)
print('八进制',0o273)
print('十六进制',0xFF)

#浮点数类型     float
a=3.14159
print(a,type(a))

f1=1.1
f2=2.2
f3=2.1
print(f1+f2)

from decimal import Decimal
print(Decimal('1.1')+Decimal('2.2'))

print(f1+f3)

#布尔类型       bool=True/False
b1=True
b2=False
print(b1,type(b1))
print(b2,type(b2))
print(b2+1)#False=0
print(b1+1)#True=1

#字符串类型     str,加单/双引号/三引号
str1='道可道，非常道'
print(str1,type(str1))
str2="民不畏死"
print(str2,type(str2))
str3='''和其光
同其尘'''
print(str3,type(str3))
str4="""夫唯不争
故无尤"""
print(str4,type(str4))
