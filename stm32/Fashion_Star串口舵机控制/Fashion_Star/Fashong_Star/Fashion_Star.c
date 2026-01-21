#include "Fashion_Star.h"
#include "usart.h"
#include "math.h"
#include "stdio.h"

uint8_t Send_buf[200];

#define steering_num 2
uint16_t check_my_arm=0;


int init_my_arm(void)
{
	
	for(int j=0;j<steering_num;j++)
	{
		int ch=0;//用作累增
		//识别字节
		Send_buf[ch++]=0x12;
		Send_buf[ch++]=0x4c;
	
		//封包编号
		Send_buf[ch++]=0x01;
	
		//内容长度
		Send_buf[ch++]=0x01;
	
		//内容
		Send_buf[ch++]=j;
	
		//检查码
		uint8_t check=0;
		for(int i=0;i<ch;i++)
		{
				check+=Send_buf[i];
		}
		Send_buf[ch++]=check;
		HAL_UART_Transmit_DMA(&huart2,Send_buf,ch);
		HAL_Delay(20);
	}
	memset(Send_buf,0,200);
	if(check_my_arm==0xC)
	{
		printf("校验成功\r\n");
		return 1;
	}
	else
	{
		printf("%d\r\n",check_my_arm);
		printf("校验失败\r\n");
		return 0;
	}
}

void angle_ctrl(int num_of_steer,double angle,uint16_t reach_ms,uint16_t power)
{
	int ch=0;//用作累增
	//识别字节
	Send_buf[ch++]=0x12;
	Send_buf[ch++]=0x4c;

	//封包编号
	Send_buf[ch++]=0x08;

	//内容长度
	Send_buf[ch++]=0x07;

	//内容
	//id
	Send_buf[ch++]=num_of_steer;
	//angle
	Send_buf[ch++]=(int16_t)(angle*10);
	Send_buf[ch++]=((int16_t)(angle*10)>>8);
	//time
	Send_buf[ch++]=reach_ms;
	Send_buf[ch++]=reach_ms>>8;
	//功率
	Send_buf[ch++]=power;
	Send_buf[ch++]=power>>8;

	//检查码
	uint8_t check=0;
	for(int i=0;i<ch;i++)
	{
			check+=Send_buf[i];
	}
	Send_buf[ch++]=check;
	HAL_UART_Transmit_DMA(&huart2,Send_buf,ch);
}


int solve_receive(uint8_t* re_buf)
{//判断舵机工作正常
	uint8_t ch=0;
	uint8_t check=0;
	if(re_buf[0]!=0x05||re_buf[1]!=0x1c)
	{
		return 0;
	}
	check+=(0x05+0x1c);
	switch(re_buf[2])
	{
		case 0x01://PING相应包
			ch++;
			check+=(0x01+0x01);//PING指令的响应数据包+响应数据包内容的长度是１
			check+=re_buf[4];//舵机数
			check_my_arm = check_my_arm + pow(2,re_buf[4]);
			break;
	}
	if(check==re_buf[5])
	{//校验成功
		return 1;
	}
	else
	{//校验失败
		return 0;
	}
}

