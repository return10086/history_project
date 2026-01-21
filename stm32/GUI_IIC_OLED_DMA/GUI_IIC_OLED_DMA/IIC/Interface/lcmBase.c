#include "config.h"
#include "lcmBase.h"
#include "stm32f1xx_hal_i2c.h"

#include <math.h>
uint8_t a=0;
uint8_t  bak;
u8 pos=0,bx=0,temp=0;
/* 定义前景色及背景色变量，用于ASCII码、汉字、窗口、单色位图显示 */
TCOLOR  disp_color=0xFF;
TCOLOR	back_color=0x00;


/* 定义显示缓冲区 */
//缓冲区的定义决定了,图库或者字库的取模方式
TCOLOR  gui_disp_buf[GUI_LCM_XMAX][GUI_LCM_YMAX/8];//行-->Y  列-->XLevelH



/***********************************************************************
* 名称：LCM_WriteByte()
* 功能：向指定点写数据(一字节)使用MCU的显存。
* 入口参数：x 		x坐标值(0-127)
*	    	y       y坐标值(0-63)
*           wrdata	所要写的数据
* 说明：会重新设置CS1/CS2，及其内部指针
***********************************************************************/
void LCM_WriteByte(uint8_t x, uint8_t y, uint8_t wrdata)
{ //x = x&0x7f;				// 参数过滤
  //y = y&0x3f;


  uint8_t tempY;


  //更新显示缓冲区
  //写入缓冲区的模式需要和硬件相同
  //这里需要区分两种屏幕模式
  //1.硬件为--横向写入BYTE
  //2.硬件为--纵向写入BYTE



  //在更新缓存前,需要处理好数据存储模式
  /*■■■■纵向写入■■■■■*/
  tempY = y>>3;//--用于纵向取模


  //更新显存
  gui_disp_buf[x][tempY] = wrdata;

  //向硬件中写入一个字节的数据
  LCM_WriteByteToHardware(x,y,wrdata);
}

void LCM_WriteByteToHardware(uint8_t X,uint8_t Y,uint8_t Data)
{
	gui_disp_buf[Y][X]=Data; 
}

/***********************************************************************
* 名称：LCM_ReadByte()
* 功能：读取指定点上的数据。
* 入口参数：x 	  x坐标值(0-127)
*	   	    y     y坐标值(0-63)
* 出口参数：返回该点上的字节数据。
***********************************************************************/
uint8_t  LCM_ReadByte(uint8_t x, uint8_t y)
{ //x = x&0x7f;				// 参数过滤
  //y = y&0x3f;



  //读取显示缓冲区
  //读取缓冲区的模式需要和硬件相同
  //这里需要区分两种屏幕模式
  //1.硬件为--横向写入BYTE
  //2.硬件为--纵向写入BYTE


  /*■■■■纵向写入,纵向读取模式■■■■■*/
  y = y>>3;
  return(gui_disp_buf[x][y]);//对于硬件是纵向写入还是横向写入,可以通过更改这个顺序决定
}




/****************************************************************************
* 名称：GUI_FillSCR()
* 功能：全屏填充。直接使用数据填充显示缓冲区。
* 入口参数：dat		填充的数据
* 出口参数：无
* 说明：用户根据LCM的实际情况编写此函数。
****************************************************************************/
void  GUI_FillSCR(TCOLOR dat)
{  uint32_t i,j;

   // 填充缓冲区
   for(i=0; i<(GUI_LCM_YMAX/8); i++)
   {  for(j=0; j<GUI_LCM_XMAX; j++)
      {  gui_disp_buf[j][i] = dat;
      }
   }

   // 填充LCM
  // LCM_DispFill(dat);
}








/****************************************************************************
* 名称：GUI_Initialize()
* 功能：初始化GUI，包括初始化显示缓冲区，初始化LCM并清屏。
* 入口参数：无
* 出口参数：无
* 说明：用户根据LCM的实际情况编写此函数。
****************************************************************************/
void  GUI_Initialize(void)
{
   LCM_DispIni();			        // 初始化LCM模块工作模式，纯图形模式
   GUI_FillSCR(0x00);				// 初始化缓冲区为0x00，并输出屏幕(清屏)
}


uint8_t const  DEC_HEX_TAB[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
/****************************************************************************
* 名称：GUI_Point()

* 功能：在指定位置上画点。
* 入口参数：x		指定点所在列的位置
*           y		指定点所在行的位置
*           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：返回值为1时表示操作成功，为0时表示操作失败。
* 说明：操作失败原因是指定地址超出缓冲区范围。
****************************************************************************/
uint8_t  GUI_Point(uint8_t x, uint8_t y, TCOLOR color)
{  //uint8_t   bak;

	
	if(x>127||y>63)return 0;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=pow(2,bx);
	//temp=1<<(bx);
	if(color)gui_disp_buf[x][pos]=temp|gui_disp_buf[x][pos];
	else gui_disp_buf[pos][x]&=~temp;	   
	
	
   // 参数过滤
//   if(x>=GUI_LCM_XMAX) return(0);
//   if(y>=GUI_LCM_YMAX) return(0);

//   // 设置相应的点为1或0
//   bak = LCM_ReadByte(x,y);
//   if(0==color)
//   {  bak &= (~DEC_HEX_TAB[y&0x07]);
//   }
//   else
//   {  bak |= DEC_HEX_TAB[y&0x07];//y&0x07作用是取余数,好处是小于0x07的也能保留结果,
//   }

//   // 刷新显示
//   LCM_WriteByte(x, y, bak);
   return(1);
}


/****************************************************************************
* 名称：GUI_ReadPoint()
* 功能：读取指定点的颜色。
* 入口参数：x		指定点所在列的位置
*           y		指定点所在行的位置
*           ret     保存颜色值的指针
* 出口参数：返回0表示指定地址超出缓冲区范围
* 说明：对于单色，设置ret的d0位为1或0，4级灰度则为d0、d1有效，8位RGB则d0--d7有效，
*      RGB结构则R、G、B变量有效。
****************************************************************************/
uint8_t  GUI_ReadPoint(uint8_t x, uint8_t y, TCOLOR *ret)
{  uint8_t  bak;

   // 参数过滤
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);

   bak = LCM_ReadByte(x,y);
   if( (bak & (DEC_HEX_TAB[y&0x07])) == 0 ) *ret = 0x00;
     else  *ret = 0x01;

   return(1);
}


/****************************************************************************
* 名称：GUI_HLine()
* 功能：画水平线。
* 入口参数：x0		水平线起点所在列的位置
*           y0		水平线起点所在行的位置
*           x1      水平线终点所在列的位置
*           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：无
* 说明：操作失败原因是指定地址超出缓冲区范围。
****************************************************************************/
void  GUI_HLine(uint8_t x0, uint8_t y0, uint8_t x1, TCOLOR color)
{  

   if(x0>x1) 						// 对x0、x1大小进行排列，以便画图
   {  bak = x1;
      x1 = x0;
      x0 = bak;
   }

   do
   {  GUI_Point(x0, y0, color);		// 逐点显示，描出水平直线
      x0++;
   }while(x1>=x0);
}

//由右到左画直线
void  GUI_XLine(uint8_t x0, uint8_t y0, uint8_t x1, TCOLOR color)
{  uint8_t  bak;

   if(x0>x1) 						// 对x0、x1大小进行排列，以便画图
   {  bak = x1;
      x1 = x0;
      x0 = bak;
   }

   do
   {  GUI_Point(x1, y0, color);		// 逐点显示，描出水平直线
      x1--;
   }while(x1>=x0);
}

/***********************************************************************
* 名称：GUI_RLine()
* 功能：画竖直线。根据硬件特点，实现加速。
* 入口参数：x0		垂直线起点所在列的位置
*           y0		垂直线起点所在行的位置
*           y1      垂直线终点所在行的位置
*           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：	无
* 说明：操作失败原因是指定地址超出缓冲区范围。
***********************************************************************/
void  GUI_RLine(uint8_t x0, uint8_t y0, uint8_t y1, TCOLOR color)
{  uint8_t  bak;
   //uint8_t  wr_dat;

   if(y0>y1) 		// 对y0、y1大小进行排列，以便画图
   {  bak = y1;
      y1 = y0;
      y0 = bak;
   }

   do
   {  
		 
			GUI_Point(x0, y0, color);
		  y0++;
   }while(y1>=y0);

}
