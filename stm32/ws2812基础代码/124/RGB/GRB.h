#ifndef _GRB_H
#define _GRB_H

#define X 10
#define Y 10

#include "main.h"
void chai32(void);//将32位颜色拆为空+绿+红+蓝
void setpoint(uint8_t x,uint8_t y,uint8_t green,uint8_t red,uint8_t blue);
void full(uint8_t green,uint8_t red,uint8_t blue);
void Send(TIM_HandleTypeDef *htim, uint32_t Channel);
void move(char dir,_Bool y_or_n);//方向（wasd），消失或重复播放
#endif
