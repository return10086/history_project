/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.h
 * @brief   This file contains all the function prototypes for
 *          the usart.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define UART1_RX_SIZE 100
  extern uint8_t UART1_RX_BUF[];  // 串口接收缓存
  extern uint8_t UART1_RX_LEN;    // 接收到的数据量
  extern uint8_t UART1_RX_Data[]; // 数据缓存

#define UART2_RX_SIZE 100
  extern uint8_t UART2_RX_BUF[];  // 串口接收缓存
  extern uint8_t UART2_RX_LEN;    // 接收到的数据量
  extern uint8_t UART2_RX_Data[]; // 数据缓存

#define UART3_RX_SIZE 100
  extern uint8_t UART3_RX_BUF[];  // 串口接收缓存
  extern uint8_t UART3_RX_LEN;    // 接收到的数据量
  extern uint8_t UART3_RX_Data[]; // 数据缓存

#define UART4_RX_SIZE 100
  extern uint8_t UART4_RX_BUF[];  // 串口接收缓存
  extern uint8_t UART4_RX_LEN;    // 接收到的数据量
  extern uint8_t UART4_RX_Data[]; // 数据缓存
/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart6;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

