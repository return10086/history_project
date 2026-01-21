/*
 * @Author: 程佳乐 &&cjl2646289864@gmail.com
 * @Date: 2023-08-18 02:42:03
 * @LastEditors: 程佳乐 &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-08-22 07:23:28
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\step_Motor.h
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H

#include "MyProject.h"
// 使用电机数量
#define motor_num 3
// 校验字节
#define check_bit 0x6B
/*
https://blog.csdn.net/zhangdatou666/article/details/128320222?spm=1001.2014.3001.5501
https://blog.csdn.net/gjy_skyblue/article/dnetails/120568809?spm=1001.2014.3001.5501
*/

typedef enum
{
    MY = 0,
    R1 = 1,  // 旋转电机（大臂）
    R2 = 2,  // 小臂电机
    Z = 3 // 升降电机
} ID;

typedef enum
{
    CW = 0, // 顺时针
    CCW     // 逆时针
} DIR;

typedef enum
{
    anathing = 0,
    Mem = 0xC8, // 储存
    Del = 0xCA  // 清除
} Mem_Or_Del;

typedef struct
{
    // 电机编号
    ID id;
    // 编码器数值
    uint16_t encoder;
    // 输入脉冲数
    int32_t pulse_num;
    // 电机实时位置
    float position;
    // 位置误差
    float postion_error;
    // 使能状态(1 enable 0 disable)
    int enable_state;
    // 堵转状态(1 lock 0 unlock)
    int lock_state;
    // 上电回0状态(1 不正常 0 正常)
    int zero_state;
    // 细分
    uint16_t Mstep;
    // 电机类型(0:1.1  1:4.2)
    int motor_type;
    // 发送串口
    UART_HandleTypeDef send_usart;
    // 传动比
    float CONVERT;
    // 是否运动（1运动，0静止）
    int run;
} step_Motor;

// 定义motor_num个电机
extern step_Motor Motor[];

void Remove_Lock(step_Motor *motor);

void Read_Encoder(step_Motor *motor);        // 1
void Read_pulse_num(step_Motor *motor);      // 2
void Read_Position(step_Motor *motor);       // 3
void Read_Position_Error(step_Motor *motor); // 4
void Read_Enable_State(step_Motor *motor);   // 5
void Read_Lock_State(step_Motor *motor);     // 6
void Read_Zero_State(step_Motor *motor);     // 7

void Change_Subdivision_Steps(step_Motor *motor, uint8_t subdivision_steps);
void Change_Uart_Address(step_Motor *motor, uint8_t uart_address);

void Control_Enable_State(step_Motor *motor, uint8_t enable_state);
void Control_Speed_Mode(step_Motor *motor, DIR dir, uint16_t speed, uint8_t acc);
void Control_Mem_Or_Del_State(step_Motor *motor, Mem_Or_Del mem_Or_del);
void Control_Position_Mode(step_Motor *motor, DIR dir, uint16_t speed, uint8_t acc, float angle);

void Solve_Receive(uint8_t *UART_RX_BUF, int rxlen);
#endif
