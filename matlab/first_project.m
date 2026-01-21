clear all%清除 Workspace 所有变量
clc
%% matlab数据类型
% 1、数字
2+4

10-7

3*5

8/2
%%
%2、字符与字符串
s='a'    %单引号 字符串
abs(s)   %字符串  ->ascll码
char(65) %ascll码 ->字符串
num2str(65) %数字 ->字符串

str='I Love MATLAB &Machine Learning'
length(str)
%%
%3、矩阵
A=[1 2 3;4 5 6;7 8 9];
B=A'%转置
C=A(:)%竖着转换为向量
D=inv(A)%求逆

E=zeros(10,5,3)
E(:,:,1) =rand(10,5)%10行5列
E(:,:,2)=randi(5,10,5)%最大值是5，10行5列
E(:,:,3)=randn(10,5)%10行5列，均值为0，方差为1
%%
%4、元胞数组
 F=cell(1,6)
 F{2}=eye(3)%3*3的单位 对角矩阵
 F{5}=magic(5)%五阶幻方
 G=F{5}
 %%
 %5、结构体
 books=struct('name',{{'Machine Learning','Data Mining'}},'price',[30,40])
 books.name%属性，取出
 books.name(1)%小括号取出cell
 books.name{1}%中括号取出字符串
%%
%matlab的矩阵操作
H=[1 2 3 4 5 6 7 8]
I=1:2:9%最小值，最大值，步长
J=repmat(I,3,1)%竖着重复B三次，横着重复一次
K=ones(2,4)%两行四列 所有元素全1

%%
%矩阵的四则运算
L=[1 2 3 4;5 6 7 8]
M=[1 1 2 2;2 2 1 1]
N=L+M
O=L-M
P=L*M'
Q=L.*M%对应项相乘
R=L/M%相当于A*B的逆（inv)
S=L./M%对应项相除
%%
%矩阵的下标 
T=magic(5)
U=T(2,3)
V=T(3,:)%取第三行
W=T(:,4)%取第四列
[m,n]=find(T>20)%找到大于20的序号值/矩阵

