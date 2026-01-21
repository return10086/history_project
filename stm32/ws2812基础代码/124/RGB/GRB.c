#define light_number 10
#define WS1 68
#define WS0 22

#include "GRB.h"
#include "main.h"
#include "tim.h"
                  // Device header
uint32_t color[X][Y];
uint8_t bit[3*light_number];//存储红蓝绿
uint16_t a[light_number*24+300];//存储逻辑0和1
int Count=0;
uint32_t bufferx[X];
uint32_t buffery[Y];


void chai32(void)
{
	uint16_t i=0;
	uint16_t j=0;
	for(i=0;i<X;i++)
	{
		for(j=0;j<Y;j++)
		{
		bit[i*24+j*3+0]=color[j][i]>>16;//采集绿色
		bit[i*24+j*3+1]=color[j][i]>>8;//采集红色
		bit[i*24+j*3+2]=color[j][i];//采集蓝色
		}
	}
}

void setpoint(uint8_t x,uint8_t y,uint8_t green,uint8_t red,uint8_t blue)
{
	color[x-1][y-1]=green<<16|red<<8|blue;
}

void full(uint8_t green,uint8_t red,uint8_t blue)
{
	for(uint8_t i=1;i<X+1;i++)
	{
		for(uint8_t j=1;j<Y+1;j++)
		{
			setpoint(i,j,green,red,blue);
		}
	}
}

void Send(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	for(uint32_t count=0;count<3*light_number ;)
	{
		uint8_t data=bit[count];
		for(uint8_t h=0;h<8;h++)
		{
			a[h+count*8]=(data&0x80)?WS1:WS0;
			data=data<<1;
			
		}
		count++;
	}//在这里把所有位存a[]
	for(uint16_t cnt=24*light_number;cnt<light_number*24+300;cnt++)
	{
		a[cnt]=0;
	}//写reset
	HAL_TIM_PWM_Start_DMA(htim,Channel,(uint32_t*)a,24*light_number+300);
}






void move(char dir,_Bool y_or_n)
{
	if(dir=='w')//向上位移一格
	{
		for(uint8_t q=0;q<X;q++)//将最上一层储存起来
		{
			bufferx[q]=color[q][7];
		}
		
		for(int i=Y-2;i>=0;i--)//从y=6向y=7挪，挪到y=0
		{
			for(uint8_t j=0;j<X;j++)//挪一行
			{
				color[j][i+1]=color[j][i];
			}
		}
		
		if(y_or_n)//如果要循环播放
		{
			for(uint8_t q=0;q<X;q++)//把数据放回最后一行
			{
				color[q][0]=bufferx[q];
			}
		}
				
	}
}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
		if(htim->Instance==TIM2)
		{
			//__HAL_TIM_CLEAR_FLAG();
			HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_3);
			//SendFlag=1;  //DAM Pwm finish.			
		}

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}

