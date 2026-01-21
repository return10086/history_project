#if else的简写
a=int(input('请输入一个整数'))
b=int(input('请输入另一个整数'))
'''if a>=b:
    print(a,'大于等于',b)
else:
    print(a,'小于',b)
'''
print('使用条件表达式进入比较')
print(str(a)+'小于'+str(b) if a>=b else str(a)+'小于'+str(b))
#如果为true执行左段，如果为false执行右端