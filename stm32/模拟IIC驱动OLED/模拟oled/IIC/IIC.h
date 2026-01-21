#ifndef IIC_H
#define IIC_H
#include "main.h"
#include "gpio.h"			  	    

//PB5做时钟
//PB6做数据
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/****************时钟*********************/
#define OLED_SCL_PORT  GPIOB
#define OLED_SCL_PINS  GPIO_PIN_5

/****************数据*********************/
#define OLED_SDIN_PORT  GPIOB
#define OLED_SDIN_PINS  GPIO_PIN_6


#define SCL_LOW   HAL_GPIO_WritePin(OLED_SCL_PORT,OLED_SCL_PINS,GPIO_PIN_RESET)
#define SCL_HIGH  HAL_GPIO_WritePin(OLED_SCL_PORT,OLED_SCL_PINS,GPIO_PIN_SET)

#define SDA_LOW   HAL_GPIO_WritePin(OLED_SDIN_PORT,OLED_SDIN_PINS,GPIO_PIN_RESET)
#define SDA_HIGH  HAL_GPIO_WritePin(OLED_SDIN_PORT,OLED_SDIN_PINS,GPIO_PIN_SET)

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		32
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	32	    						  
//-----------------OLED端口定义----------------  					   



void delay_ms(unsigned int ms);
		     
//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   				   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);
void OLED_Scroll(void);	 

#endif


