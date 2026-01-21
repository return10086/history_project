#include "IIC.h"
#include "main.h"
#include "oledfont.h"
unsigned char OLED_GRAM[128][8];

	 

void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}
void IIC_Start()
{
	SCL_HIGH;
	SDA_HIGH;
	SDA_LOW;
	SCL_LOW;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
  SCL_HIGH ;
//	SCL_LOW;
	SDA_LOW;
	SDA_HIGH;
	
}

void IIC_Wait_Ack()
{


	SCL_HIGH ;
	SCL_LOW;
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	SCL_LOW;
	for(i=0;i<8;i++)		
	{
			m=da;
		m=m&0x80;
		if(m==0x80)
		{SDA_HIGH;}
		else SDA_LOW;
			da=da<<1;
		SCL_HIGH;
		SCL_LOW;
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(unsigned char dat,uint8_t cmd)
{	
	if(cmd)
			{
   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	} 	  
} 
 	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)
        {
          for(n=0;n<128;n++)
          {
            OLED_GRAM[n][i]=0X00;  
          }
        }
	OLED_Refresh_Gram();//更新显示
} 

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空			   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
  uint8_t i,m,n;
  i=y/8;           //输入的Y坐标要除以8,以定位到要改变的那个点的相应字节,因为是以字节形式来写入的
  m=y%8;                                                          // 该点在该字节中的位置                
  n=1<<m;                                                         //屏坐标和数组是上下相反，要把该点y坐标左移写入n
  if(t)

       {OLED_GRAM[x][i]|=n;}                            //t=1，就把该点写1 
  else
     {
       OLED_GRAM[x][i]=~OLED_GRAM[x][i];
       OLED_GRAM[x][i]|=n;
       OLED_GRAM[x][i]=~OLED_GRAM[x][i];
     }
  
 }

 
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=31	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
                {
                OLED_DrawPoint(x,y,dot);
                }
	}
        OLED_Refresh_Gram();//更新显示
	
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~31
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }
    OLED_Refresh_Gram();//更新显示
}

//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	

//显示数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 

//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	         
}	

//滚动效果配置函数
 
void OLED_Scroll(void)
{
  OLED_WR_Byte(0x2E,OLED_CMD);	//关闭滚动
  OLED_WR_Byte(0x27,OLED_CMD);	//水平向左滚动
  OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0x00,OLED_CMD);	//起始页 0
  OLED_WR_Byte(0x00,OLED_CMD);	//滚动时间间隔
  OLED_WR_Byte(0x01,OLED_CMD);	//终止页 1
  OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0xFF,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0x2F,OLED_CMD);	//开启滚动
}

//初始化SSD1306					    
void OLED_Init(void)
{

delay_ms(20);
  OLED_WR_Byte(0xA8,OLED_CMD);
  OLED_WR_Byte(0x1F,OLED_CMD);
  OLED_WR_Byte(0xDA,OLED_CMD);
  OLED_WR_Byte(0x02,OLED_CMD);
  OLED_WR_Byte(0xD3,OLED_CMD);
  OLED_WR_Byte(0x00,OLED_CMD);
  OLED_WR_Byte(0x40,OLED_CMD);
  OLED_WR_Byte(0xA1,OLED_CMD);
  OLED_WR_Byte(0x66,OLED_CMD);//81
  OLED_WR_Byte(0xFF,OLED_CMD);
  OLED_WR_Byte(0xA4,OLED_CMD);
  OLED_WR_Byte(0xA6,OLED_CMD);
  OLED_WR_Byte(0xD5,OLED_CMD);
  OLED_WR_Byte(0xF0,OLED_CMD);
  OLED_WR_Byte(0x8D,OLED_CMD);
  OLED_WR_Byte(0x14,OLED_CMD);
  OLED_WR_Byte(0xAE,OLED_CMD);
  OLED_WR_Byte(0x20,OLED_CMD);
  OLED_WR_Byte(0x02,OLED_CMD);
  OLED_WR_Byte(0xB0,OLED_CMD);
  OLED_WR_Byte(0xC8,OLED_CMD);
  OLED_WR_Byte(0x00,OLED_CMD);
  OLED_WR_Byte(0x10,OLED_CMD);
  OLED_WR_Byte(0x40,OLED_CMD);
  OLED_WR_Byte(0xD9,OLED_CMD);
  OLED_WR_Byte(0x22,OLED_CMD);
  OLED_WR_Byte(0xDB,OLED_CMD);
  OLED_WR_Byte(0x20,OLED_CMD);
  OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();

}  

