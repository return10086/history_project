/*
 * @Author: ³Ì¼ÑÀÖ &&cjl2646289864@gmail.com
 * @Date: 2023-08-22 01:54:51
 * @LastEditors: ³Ì¼ÑÀÖ &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-09-06 00:11:04
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\steer.c
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */

#include "steer.h"
#include "tim.h"

void init_my_steer(void)
{
	HAL_TIM_Base_Start(&htim3);
	TIM3->ARR = 20000;
	TIM3->CCR2 = 20000 * 1.0f / 40;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}
void ctrl_steer(float angle)
{ //(0~180)
	TIM3->CCR2 = 20000 * 1.0f * (angle / 45.0 + 1) / 40;
}
