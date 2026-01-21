#ifndef __OLED_H
#define __OLED_H
#define u8 uint8_t
#define u32 uint32_t
#include "main.h"			  	    
 

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/****************时钟*********************/
#define OLED_SCLK_PORT  (GPIOE)
#define OLED_SCLK_PINS  (GPIO_PIN_1)

/****************数据*********************/

#define SCL_LOW   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
#define SCL_HIGH   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);

#define SDA_LOW   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define SDA_HIGH   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   



void delay_ms(unsigned int ms);
		     
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   				   		    
void OLED_Init(void);
void OLED_Clear(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_Scroll_speed(void);	 


void OLED_Scroll(void);	 
#endif  
	 



