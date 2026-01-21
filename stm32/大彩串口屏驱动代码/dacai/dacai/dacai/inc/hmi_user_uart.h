/*
 * @作者: 程佳乐
 * @Date: 2023-03-02 15:43:11
 * @LastEditTime: 2023-03-02 18:46:38
 * @描述:
 * @FilePath: \MDK-ARMe:\project\stm32\dacai\dacai\dacai\inc\hmi_user_uart.h
 * @人生目标: 全世界无产者联合起来
 */
/*!
 *  \file hmi_driver.h
 *  \brief 串口初始化
 *  \version 1.0
 *  \date 2012-2018
 *  \copyright 广州大彩光电科技有限公司
 */

#include "main.h"

#ifndef _USER_UART__
#define _USER_UART__

typedef unsigned char uchar;

/****************************************************************************
 * 名    称： UartInit()
 * 功    能： 串口初始化
 * 入口参数： 无
 * 出口参数： 无
 ****************************************************************************/
void UartInit(uint32_t Baudrate);

/*****************************************************************
 * 名    称： SendChar()
 * 功    能： 发送1个字节
 * 入口参数： t  发送的字节
 * 出口参数： 无
 *****************************************************************/
void SendChar(uchar t);

#endif
