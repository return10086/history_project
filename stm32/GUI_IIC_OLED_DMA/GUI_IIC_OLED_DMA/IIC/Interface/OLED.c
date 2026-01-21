#include "OLED.h"
#include "lcmBase.h"
#include "main.h"
#include "dma.h"
extern uint8_t  gui_disp_buf[GUI_LCM_XMAX][GUI_LCM_YMAX/8];
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;

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

	SCL_HIGH ;
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
	u8 i=0;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(uint8_t n=0;n<128;n++)
		{
			OLED_WR_Byte(gui_disp_buf[n][i],OLED_DATA); 
		}
	}   
}

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(unsigned char dat,u8 cmd)
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
void OLED_Clear(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1)  
{  
	u8 i,n;  
	for(i=x0;i<x1;i++)
        {
          for(n=y0;n<y1;n++)
          {
            gui_disp_buf[i][n]=0X00;  
          }
        }

} 

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空			   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
  u8 i,m,n;
  i=y/8;           //输入的Y坐标要除以8,以定位到要改变的那个点的相应字节,因为是以字节形式来写入的
  m=y%8;                                                          // 该点在该字节中的位置                
  n=1<<m;                                                         //屏坐标和数组是上下相反，要把该点y坐标左移写入n
  if(t)

       {gui_disp_buf[i][x]|=n;}                            //t=1，就把该点写1 
  else
     {
       gui_disp_buf[i][x]=~gui_disp_buf[i][x];
       gui_disp_buf[i][x]|=n;
       gui_disp_buf[i][x]=~gui_disp_buf[i][x];
     }
  
 }

 
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=31	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
                {
                OLED_DrawPoint(x,y,dot);
                }
	}
        OLED_Refresh_Gram();//更新显示
	
}


//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	


//滚动效果配置函数
 
void OLED_Scroll(void)
{
  OLED_WR_Byte(0x2E,OLED_CMD);	//关闭滚动
  OLED_WR_Byte(0x27,OLED_CMD);	//水平向左滚动
  OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0x00,OLED_CMD);	//起始页 0
  OLED_WR_Byte(0x00,OLED_CMD);	//滚动时间间隔
  OLED_WR_Byte(0x07,OLED_CMD);	//终止页 1
  OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0xFF,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0x2F,OLED_CMD);	//开启滚动
}

void OLED_Scroll_speed(void)
{
		for(int i=40;i<128;i++)
	{
			HAL_DMA_Start(&hdma_memtomem_dma1_channel1,(uint32_t)gui_disp_buf[i+1],(uint32_t)gui_disp_buf[i],sizeof(gui_disp_buf[0]));
			MX_DMA_Init();
	}
}


//初始化SSD1306					    
void OLED_Init(void)
{

delay_ms(20);
 	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear(0,0,127,63);

}  

