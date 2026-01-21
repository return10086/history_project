#单分支结构
money=1000#余额
s=int(input('请输入取款金额'))#取款金额
#判断余额是否充足
if money>=s:
    money-=s
    print("取款成功，余额为：",money)


#双分支结构
a=int(input("请输入一个整数"))
if a%2==0:#如果a为偶数
    print(a,"是偶数")
else:
    print(a,"是奇数")


#多分支结构
score = int(input("请输入一个成绩："))

if score>=90 and score<=100:
 print('A级')
elif score>=80 and score<=89:
 print('B级')
elif score>=70 and score<=79:
 print('C级')
elif score>=60 and score<=69:
 print('D级')
elif score >=0 and score<=59:
 print('E级')
else:
     print("对不起，成绩有误，不在有效成绩范围内")

     '''
if 90<=score<=100:
 print('A级')
elif 80<=score<=89:
 print('B级')
elif 70<=score<=79:
 print('C级')
elif 60<=score<=69:
 print('D级')
elif 0<=score<=59:
 print('E级')
else:
     print("对不起，成绩有误，不在有效成绩范围内")
     '''#同样可以