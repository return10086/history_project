/*
 * @Author: 程佳乐 &&cjl2646289864@gmail.com
 * @Date: 2023-08-18 01:22:48
 * @LastEditors: 程佳乐 &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-08-31 17:18:48
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\MyProject.c
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#include "MyProject.h"
uint32_t SendBuffer[100];
float32_t rotation_angle = 0.0;
float x = 180.0, y = 180.0, z = 0.0;
uint8_t Uart16_Rx_Cnt = 0; // 接收缓冲计数

/**
 * @description:四个串口回调，处理串口接收到的不定长数据
 * @param {UART_HandleTypeDef} *huart
 * @param {uint8_t} rxlen
 * @return {*}
 */

// x y z 舵机（0合1开） 电机（0停1动）
#define move_num 3
float target[move_num][5] =
    {
        {360, 0, 50, 0, 0},
        {180, 180, 50, 0, 0},
        {180, 0, 60, 0, 0}};
int move_cnt = 0;
void USAR_UART_IDLECallback(UART_HandleTypeDef *huart, uint8_t rxlen)
{
    if (huart == &huart1) // 判断是否为串口1产生中断
    {
        memcpy(UART1_RX_Data, UART1_RX_BUF, rxlen); // 将UART1_RX_BUF的数据复制到UART1_RX_Data中，最后两位是标志不是数据
        switch (UART1_RX_BUF[0])
        {
        case 'S':
            // 大臂转固定角度
            Control_Position_Mode(&Motor[1], CW, (uint16_t)100, (uint8_t)0, atof((const char *)(UART1_RX_Data + 1)));
            break;
        case 'B':
            // 小臂转固定角度
            Control_Position_Mode(&Motor[2], CW, (uint16_t)100, (uint8_t)0, atof((const char *)(UART1_RX_Data + 1)));
            break;
        case 'D':
            // Z转固定角度
            Control_Position_Mode(&Motor[3], CW, (uint16_t)100, (uint8_t)255, atof((const char *)(UART1_RX_Data + 1)));
            break;
        case 'X': // 记录串口发来的三轴位置
            x = atof((const char *)(UART1_RX_Data + 1));
            break;
        case 'Y':
            y = atof((const char *)(UART1_RX_Data + 1));
            break;
        case 'Z':
            z = atof((const char *)(UART1_RX_Data + 1));
            break;
        case 'R':
            printf("开始运行\r\n");
            // 解悬任务1
            xTaskResumeFromISR(task1Handle);
            break;
        default:
            // 发来的是相应命令
            Solve_Receive(UART1_RX_BUF, rxlen);
            break;
        }
        HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE); // 重新打开DMA接收
    }
    else if (huart == &huart2) // 判断是否为串口2产生中断
    {
        memcpy(UART2_RX_Data, UART2_RX_BUF, rxlen);                                          // 将UART2_RX_BUF的数据复制到UART2_RX_Data中，最后两位是标志不是数据
        solve_message();                                                                     // 处理摄像头回传信息
        rotation(&CAM_TxData.x, &CAM_TxData.y);                                              // 把回传的信息解到车身坐标系
        Set_Target_Point((ARM.tar_x + CAM_TxData.x), (ARM.tar_x + CAM_TxData.y), ARM.tar_z); // z轴不动调整xy
        xTaskResumeFromISR(task1Handle);                                                     // 解悬任务1
        HAL_UART_Receive_DMA(&huart2, UART2_RX_BUF, UART2_RX_SIZE);                          // 重新打开DMA接收
    }
    else if (huart == &huart3) // 判断是否为串口3产生中断
    {
        memcpy(UART3_RX_Data, UART3_RX_BUF, rxlen); // 将UART3_RX_BUF的数据复制到UART3_RX_Data中，最后两位是标志不是数据
        Solve_Receive(UART3_RX_Data, rxlen);
        HAL_UART_Receive_DMA(&huart3, UART3_RX_BUF, UART3_RX_SIZE); // 重新打开DMA接收
    }
    else if (huart == &huart4) // 判断是否为串口6产生中断
    {
        memcpy(UART4_RX_Data, UART4_RX_BUF, rxlen); // 将UART4_RX_BUF的数据复制到UART4_RX_Data中，最后两位是标志不是数据
        Solve_Receive(UART4_RX_Data, rxlen);
        HAL_UART_Receive_DMA(&huart4, UART4_RX_BUF, UART4_RX_SIZE); // 重新打开DMA接收
    }
}

/**
 * @description: 初始化：1、基础外设 2、机械臂初始化 3、电机ID相应 4、归零
 * @return {*}
 */
void init_my_project(void)
{
    // 1、基础外设
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);                // 使能空闲中断
    HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE); // 启动DMA接收，UART1_RX_BUF：数据接收缓冲
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);                // 使能空闲中断
    HAL_UART_Receive_DMA(&huart2, UART2_RX_BUF, UART2_RX_SIZE); // 启动DMA接收，UART2_RX_BUF：数据接收缓冲
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);                // 使能空闲中断
    HAL_UART_Receive_DMA(&huart3, UART3_RX_BUF, UART3_RX_SIZE); // 启动DMA接收，UART3_RX_BUF：数据接收缓冲
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);                // 使能空闲中断
    HAL_UART_Receive_DMA(&huart4, UART4_RX_BUF, UART4_RX_SIZE); // 启动DMA接收，UART3_RX_BUF：数据接收缓冲
    init_my_steer();                                            // 初始化舵机
    ctrl_steer(180);

    // 2、机械臂初始化
    Init_Arm(360.0, 0, 0);
    // 3、电机ID响应
    while (1)
    {
        Read_Position_Error(&Motor[1]);
        Read_Position_Error(&Motor[2]);
        HAL_Delay(200);
        printf("error1=%f,error2=%f\r\n", Motor[1].postion_error, Motor[2].postion_error);
        // 4、角度归0
        if (Motor[1].postion_error < 0.1 && Motor[2].postion_error < 0.1)
            break;
    }
}
/**
 * @description: 读坐标，解算，发运动命令
 * @return {*}
 */
void task1(void)
{
    printf("任务一\r\n");
    // 读坐标
    // Set_Target_Point(target[move_cnt][0], target[move_cnt][1], target[move_cnt][2]);
    // 目标点->目标角度（解算）
    position_to_angle(0);
    // 发运动命令
    //  大臂 小臂 z轴
    if ((ARM.tar_beta1 - ARM.cur_beta1) != 0)
        Control_Position_Mode(&Motor[1], CW, (uint16_t)1279, (uint8_t)0, -(ARM.tar_beta1 - ARM.cur_beta1));
    if ((ARM.tar_beta2 - ARM.cur_beta2) != 0)
        Control_Position_Mode(&Motor[2], CW, (uint16_t)1279, (uint8_t)0, -(ARM.tar_beta2 - ARM.cur_beta2));
    if ((ARM.tar_z - ARM.cur_z) != 0)
        Control_Position_Mode(&Motor[3], CW, (uint16_t)100, (uint8_t)255, (ARM.tar_z - ARM.cur_z));
    vTaskResume(task2Handle);
    printf("任务二\r\n");
    vTaskSuspend(NULL);
}
/**
 * @description: 等待运动完成（运动过程）
 * @return {*}
 */
void task2(void)
{

    if (Motor[1].run == 0 && Motor[2].run == 0 && Motor[3].run == 0)
    {
        ARM.cur_beta1 = ARM.tar_beta1;
        ARM.cur_beta2 = ARM.tar_beta2;
        ARM.cur_x = ARM.tar_x;
        ARM.cur_y = ARM.tar_y;
        ARM.cur_z = ARM.tar_z;
        printf("ang1=%f,ang2=%f,z=%f\r\n", ARM.tar_beta1, ARM.tar_beta2, ARM.cur_z);
        vTaskResume(task3Handle);
        vTaskSuspend(task2Handle);
    }
}

/**
 * @description: 操作舵机，底盘电机
 * @return {*}
 */
void task3(void)
{
    printf("任务三\r\n");
    if (target[move_cnt][3] == 1)
    { // 控制舵机开
        ;
    }
    else if (target[move_cnt][3] == 0)
    { // 控制舵机合
        ;
    }
    if (target[move_cnt][4] == 1)
    { // 控制电机转
        memset(SendBuffer, 500, 90);
        HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t *)SendBuffer, 90);
    }
    else if (target[move_cnt][4] == 0)
    { // 控制电机不转
        ;
    }
    move_cnt += 1;

    if (move_cnt == move_num)
    {
        printf("任务完成\r\n");
        vTaskResume(first_taskHandle);
    }
    else
    {
        vTaskResume(task1Handle);
    }
}

/**
 * @description: 读取摄像头回传坐标，并解算到新目标位置，返回task1
 * @return {*}
 */
void task4(void)
{
}

void solve_message(void)
{
}

/**
 * @description: 坐标点旋转
 * @param {float32_t} *x
 * @param {float32_t} *y
 * @return {*}
 */
void rotation(float32_t *x, float32_t *y)
{
    float32_t temp1, temp2;
    temp1 = arm_cos_f32(rotation_angle) * *x + arm_sin_f32(rotation_angle) * *y;
    temp2 = -arm_sin_f32(rotation_angle) * *x + arm_cos_f32(rotation_angle) * *y;
    *x = temp1;
    *y = temp2;
}