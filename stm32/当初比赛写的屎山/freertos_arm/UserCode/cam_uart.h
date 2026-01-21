/*
 * @Author: 程佳乐 &&cjl2646289864@gmail.com
 * @Date: 2023-08-31 16:07:04
 * @LastEditors: 程佳乐 &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-09-06 00:11:42
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\cam_uart.h
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#ifndef __CAM_UART_H
#define __CAM_UART_H

#include "MyProject.h"
// 数据类型只能确定为一下几种,传输负数只能用short类型
// short
// unsigned short

// 每一个地址对应两字节16位，因此只能使用short和unsigned short类型

#define Rx_num 20 // 定义接收字节数
#define Tx_num 18 // 定义发送字节数

typedef struct
{
	float32_t x, y;
} CAM_TxDATA;

extern CAM_TxDATA CAM_TxData;
/**
 * @description: 发送内容给摄像头
 * @param {CAM_TxDATA} CAM_TxData：发送数组
 * @return {*}
 */
void CAMTransmit(CAM_TxDATA CAM_TxData);
#endif
