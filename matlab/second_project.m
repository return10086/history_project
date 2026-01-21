clear all%清除 Workspace 所有变量
clc
%%
%for循环结构
sum=0
for i=0:1:100%初值 步长 尾值（步长是1可省略）
    sum=sum+i
end
sum2=0
for i=1:1:5
    p=1
    for j=1:1:i
        p=p*i
    end
    sum2=sum2+p
end 
%%
%while循环结构
s=0
n=1
while n<=10
    s=s+n
    n=n+1
end
%%
%分支结构 
a=1
b=0
if a<b
    'B大'
else
    'A大'
end
%%switch…case…end
c=0
switch c
    case 0
        '0'
    case 1
        '1'
    case 2
        '2'
end