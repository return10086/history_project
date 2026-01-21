clear all%清除 Workspace 所有变量
clc
%%基本绘图操作
%1、二维平面绘图
x=0:0.01:2*pi
y=sin(x)
figure%建立一个幕布
plot(x,y)%画图
title('y=sin(x)')
xlabel('x')
ylabel('sin(x)')
xlim([0 2*pi])%贴边

x=0:0.01:20
y1=200*exp(-0.05*x).*sin(x)
y2=0.8*exp(-0.5*x).*sin(10*x)
figure
[AX,H1,H2]=plotyy(x,y1,x,y2,'plot')
set(get(AX(1),'Ylabel'),'String','Slow Decay')
set(get(AX(2),'Ylabel'),'String','Fast Decay')
xlabel('Time(\musec)')
title('Multuple Decay Rates')
set(H1,'LineStyle','--')
set(H2,'LineStyle',':')

%%
%3、三维立体绘图
t=0:pi/50:10*pi
plot3(sin(t),cos(t),t)
xlabel('sin(t)')
ylabel('cos(t)')
zlabel('t')
grid on%加网格线
axis square%弄成方形



