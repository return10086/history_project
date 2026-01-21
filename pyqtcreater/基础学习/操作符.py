print('hello\nworld')#\n  表示换行
print('hello\tworld')#\t  表示退字节
print('helloooo\tworld')
print('hello\rworld')#\r  world将helllo覆盖
print('hello\bworld')#\b是退一个格，将b退没了

print('http:\\\\www.baidu.com')#\\被输出为\
print('老师说：\'大家好\'')#在字符串中用单双引号

#原字符，不希望字符终端转义字符起作用，就使用原字符，就是在字符串之前加上r，或R
print(r'hello\nworld')
print(r'hello\nworld\\')#注意：最后不能是\，可以是\\
