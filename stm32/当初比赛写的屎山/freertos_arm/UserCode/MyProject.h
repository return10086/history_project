/*
 * @Author: ?????? &&cjl2646289864@gmail.com
 * @Date: 2023-08-18 01:17:01
 * @LastEditors: ≥Ãº—¿÷ &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-09-06 00:10:26
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\MyProject.h
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#ifndef _My_Project_H
#define _My_Project_H

#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "step_Motor.h"
#include "robot.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "arm_math.h"
#include "steer.h"
#include "tim.h"
#include "cam_uart.h"
extern osThreadId_t task1Handle;
extern osThreadId_t task2Handle;
extern osThreadId_t task3Handle;
extern osThreadId_t first_taskHandle;

void USAR_UART_IDLECallback(UART_HandleTypeDef *huart, uint8_t rxlen);
void init_my_project(void);
void solve_message(void);
void task1(void);
void task2(void);
void task3(void);
void task4(void);
void rotation(float32_t *x, float32_t *y);
#endif
