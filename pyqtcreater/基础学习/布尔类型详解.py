#对象的布尔值，使用内置函数bool()获得
#以下对象的布尔值为False
'''
False
数值0
None
空字符串
空列表
空元组
空字典
空集合
'''
print(bool(False))

print(bool(0))
print(bool(0.0))

print(bool(None))

print(bool(''))
print(bool(""))

print(bool([]))#空列表
print(bool(list()))

print(bool(()))#空元组
print(bool(tuple()))

print(bool({}))#空字典
print(bool(dict()))

print(bool(set()))#空集
print("以上对象布尔值均为false，其他对象布尔值均为true")
print(bool(18))
print(bool(True))
print(bool("hello world"))