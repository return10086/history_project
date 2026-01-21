/*
 * @Author: ≥Ãº—¿÷ &&cjl2646289864@gmail.com
 * @Date: 2023-08-31 16:07:04
 * @LastEditors: ≥Ãº—¿÷ &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-09-06 00:11:32
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\cam_uart.c
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#include "cam_uart.h"
#include "string.h"

CAM_TxDATA CAM_TxData;

void CAMTransmit(CAM_TxDATA CAM_TxData)
{
	uint8_t HEAD1 = 0xAA;
	uint8_t HEAD2 = 0x55;
	HAL_UART_Transmit(&huart2, &HEAD1, 1, 100);
	HAL_UART_Transmit(&huart2, &HEAD2, 1, 100);
	HAL_UART_Transmit(&huart2, (uint8_t *)&CAM_TxData, sizeof(CAM_TxData), 100);
}
