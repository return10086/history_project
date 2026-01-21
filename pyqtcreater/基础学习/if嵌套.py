#if嵌套
answer=input('您是会员吗：')
money=int(input('请输入您的购物金额：'))
#判断是否是会员
if answer=='y':
    print('会员')
    if money>=200:
        print('打八折，付款金额为：',money*0.8)
    elif 100<=money<=199:
        print('打九折，付款金额为：',money*0.9)
    else:
        print('不打折，付款金额为',money)
else:
    print('非会员')
    if money>=200:
        print('打九五折，付款金额为：',money*0.95)
    else:
        print('不打折，付款金额为',money)