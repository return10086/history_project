/*
 * @Author: 程佳乐 &&cjl2646289864@gmail->com
 * @Date: 2023-08-18 02:42:03
 * @LastEditors: 程佳乐 &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-08-24 04:53:52
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\step_Motor.c
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail->com, All Rights Reserved->
 */

#include "step_Motor.h"

step_Motor Motor[motor_num + 1];
/*
把每个Read函数从1开始标号,执行一个就记录在环形队列
接收数据的时候根据第一个字节判断是哪个电机
             根据环形队列内容判断是哪一个任务
*/
#define lenth_of_queue 100
int queue[lenth_of_queue];
int write_quene = 0;
int read_quene = 0;

/**
 * @description: 解除堵转保护
 * @param {ID} motor:待操作电机
 * @return {*}
 */
void Remove_Lock(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    data[cnt++] = motor->id; // 地址
    data[cnt++] = 0x0E;      //
    data[cnt++] = 0x52;      //
    data[cnt++] = check_bit; // 校验字节
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description: 读电机编码器值
 * @param {ID} motor:待操作电机
 * @return {*}
 */
void Read_Encoder(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id; // 地址
        data[cnt++] = 0x30;      //
        data[cnt++] = check_bit; // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0x30;           //
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          // 校验字节
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xFF);
    queue[(write_quene++) % lenth_of_queue] = 1;
}

/**
 * @description:读输入脉冲数
 * @param {step_Motor} motor:待操作电机
 * @return {*}
 */
void Read_pulse_num(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id; // 地址
        data[cnt++] = 0x33;      //
        data[cnt++] = check_bit; // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0x33;           //
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          // 校验字节
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xFF);
    queue[(write_quene++) % lenth_of_queue] = 2;
}

/**
 * @description: 读取电机实时位置
 * @param {step_Motor} motor:待操作电机
 * @return {*}
 */
void Read_Position(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id; // 地址
        data[cnt++] = 0x36;      //
        data[cnt++] = check_bit; // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        printf("无此功能\r\n");
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xFF);
    queue[(write_quene++) % lenth_of_queue] = 3;
}

/**
 * @description: 读取位置误差
 * @param {step_Motor} motor:待操作电机
 * @return {*}
 */
void Read_Position_Error(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id; // 地址
        data[cnt++] = 0x39;      //
        data[cnt++] = check_bit; // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0x39;           //
        data[cnt++] = check_bit;      // 校验字节
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          //
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 4;
}

/**
 * @description: 读取使能状态
 * @param {step_Motor} motor:待操作电机
 * @return {*}
 */
void Read_Enable_State(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id; // 地址
        data[cnt++] = 0x39;      //
        data[cnt++] = check_bit; // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0x39;           //
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          //
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 5;
}
/**
 * @description: 读取堵转标志
 * @param {step_Motor} motor:待操作电机
 * @return {*}
 */
void Read_Lock_State(step_Motor *motor)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id; // 地址
        data[cnt++] = 0x3E;      //
        data[cnt++] = check_bit; // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0x3E;           //
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          //
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 6;
}

/**
 * @description: 读取单圈上电自动回零状态标志
 * @param {step_Motor} motor:待操作电机
 * @return {*}
 */
void Read_Zero_State(step_Motor *motor)
{
    if (motor->motor_type == 1)
    {
        uint8_t data[20] = {
            motor->id, // 地址
            0x3F,      //
            check_bit  // 校验字节
        };
        HAL_UART_Transmit(&motor->send_usart, data, sizeof(data), 0xff);
        queue[(write_quene++) % lenth_of_queue] = 7;
    }
    else if (motor->motor_type == 0)
    {
        printf("无此功能\r\n");
    }
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description:修改当前细分步数
 * @param {step_Motor} motor:待操作电机
 * @param {uint8_t} subdivision_steps：细分步数（0->255）
 * @return {*}
 */
void Change_Subdivision_Steps(step_Motor *motor, uint8_t subdivision_steps)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id;         // 地址
        data[cnt++] = 0x84;              //
        data[cnt++] = subdivision_steps; // 细分步数
        data[cnt++] = check_bit;         // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;         // 地址
        data[cnt++] = 0x84;              //
        data[cnt++] = subdivision_steps; // 细分步数
        uint8_t check = 0;               //
        for (int i = 0; i < cnt; i++)    //
        {                                //
            check += data[i];            //
        }                                //
        data[cnt++] = check;             //
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xFF);
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description: 修改当前串口通讯地址
 * @param {step_Motor} motor:待操作电机
 * @param {uint8_t} uart_address:串口通讯地址
 * @return {*}
 */
void Change_Uart_Address(step_Motor *motor, uint8_t uart_address)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id;    // 地址
        data[cnt++] = 0xAE;         //
        data[cnt++] = uart_address; //
        data[cnt++] = check_bit;    // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        printf("无此功能\r\n");
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description: 控制闭环电机的使能状态
 * @param {step_Motor} motor:待操作电机
 * @param {uint8_t} enable_state:00不使能 01使能
 * @return {*}
 */
void Control_Enable_State(step_Motor *motor, uint8_t enable_state)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id;    // 地址
        data[cnt++] = 0xf3;         //
        data[cnt++] = enable_state; //
        data[cnt++] = check_bit;    // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0xf3;           //
        data[cnt++] = enable_state;   //
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          //
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xFF);
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description: 控制闭环电机的正反转，即速度模式控制
 * @param {step_Motor} motor:待操作电机
 * @param {DIR} dir：方向
 * @param {uint16_t} speed：速度（0->511)/(0->127)
 * @param {uint8_t} acc:加速度（0->255）
 * @return {*}
 */
void Control_Speed_Mode(step_Motor *motor, DIR dir, uint16_t speed, uint8_t acc)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id;             // 地址
        data[cnt++] = 0xf6;                  //
        data[cnt++] = dir << 4 | speed >> 8; // 方向和速度（共用2个字节）
        data[cnt++] = (uint8_t)speed;        //
        data[cnt++] = acc;                   // 加速度
        data[cnt++] = check_bit;             // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;                 // 地址
        data[cnt++] = 0xf6;                      //
        data[cnt++] = dir << 7 | (speed & 0x7F); // 方向和速度（共用1个字节）
        uint8_t check = 0;                       //
        for (int i = 0; i < cnt; i++)            //
        {                                        //
            check += data[i];                    //
        }                                        //
        data[cnt++] = check;                     //
    }
    motor->run = 1;
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description: 存储/清除闭环电机正反转，即速度模式当前的参数，上电会自动运行
 * @param {step_Motor} motor:待操作电机
 * @param {Mem_Or_Del} mem_Or_del:存储/清除
 * @return {*}
 */
void Control_Mem_Or_Del_State(step_Motor *motor, Mem_Or_Del mem_Or_del)
{
    int cnt = 0;
    uint8_t data[20];
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id;  // 地址
        data[cnt++] = 0xFF;       //
        data[cnt++] = mem_Or_del; // 存储/清除
        data[cnt++] = check_bit;  // 校验字节
    }
    else if (motor->motor_type == 0)
    {
        data[cnt++] = motor->id;      // 地址
        data[cnt++] = 0xFF;           //
        data[cnt++] = mem_Or_del;     // 存储/清除
        uint8_t check = 0;            //
        for (int i = 0; i < cnt; i++) //
        {                             //
            check += data[i];         //
        }                             //
        data[cnt++] = check;          //
    }
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0XFF);
    queue[(write_quene++) % lenth_of_queue] = 9;
}

/**
 * @description:控制闭环电机相对运动的角度，即位置模式控制
 * @param {step_Motor} motor:待操作电机
 * @param {DIR} dir:方向
 * @param {uint16_t} speed：速度（0->1279/(0~127)
 * @param {uint8_t} acc：加速度(0->256)
 * @param {uint32_t} pulse：脉冲数(0->16777215)/（0~65535）
 * @return {*}
 */
void Control_Position_Mode(step_Motor *motor, DIR dir, uint16_t speed, uint8_t acc, float angle)
{
    uint32_t pulse = motor->Mstep * angle * motor->CONVERT / 1.8;
    if (angle < 0)
        dir = CCW;

    uint8_t data[20];
    int cnt = 0;
    if (motor->motor_type == 1)
    {
        data[cnt++] = motor->id;
        data[cnt++] = 0xFD;
        data[cnt++] = dir << 4 | speed >> 8;
        data[cnt++] = (uint8_t)speed;
        data[cnt++] = acc;
        data[cnt++] = (uint8_t)(pulse >> 16);
        data[cnt++] = (uint8_t)((pulse << 16) >> 24);
        data[cnt++] = (uint8_t)pulse;
        data[cnt++] = (uint8_t)check_bit;
    }
    else
    {

        data[cnt++] = motor->id;
        data[cnt++] = 0xFD;
        data[cnt++] = dir << 7 | (speed & 0x7F);
        data[cnt++] = (uint8_t)(pulse >> 24);
        data[cnt++] = (uint8_t)(pulse >> 16);
        data[cnt++] = (uint8_t)(pulse >> 8);
        data[cnt++] = (uint8_t)(pulse);
        uint8_t check = 0;
        for (int i = 0; i < cnt; i++)
        {
            check += data[i];
        }
        data[cnt++] = check;
    }
    motor->run = 1;
    // HAL_UART_Transmit_DMA(&huart1, data, cnt);
    HAL_UART_Transmit(&motor->send_usart, data, cnt, 0xff);
    queue[(write_quene++) % lenth_of_queue] = 10;
    queue[(write_quene++) % lenth_of_queue] = 10;
}

/**
 * @description: 解析收到的数据
 * @param {uint8_t} *UART_RX_BUF：接收到的内容
 * @param {int} rxlen：接收到的长度
 * @return {*}
 */
void Solve_Receive(uint8_t *UART_RX_BUF, int rxlen)
{
    if (UART_RX_BUF[rxlen - 1] != 0x6b)
    {
        return;
    }
    switch (queue[(read_quene++) % lenth_of_queue])
    {
    case 1:
    {
        // 读编码器
        // Read_Encoder(step_Motor *motor);
        printf("1\r\n");
        if (Motor[UART_RX_BUF[0]].motor_type == 1)
            Motor[UART_RX_BUF[0]].encoder = (UART_RX_BUF[1] << 8) | (UART_RX_BUF[2] << 0);
        else if (Motor[UART_RX_BUF[0]].motor_type == 0)
            Motor[UART_RX_BUF[0]].encoder = (UART_RX_BUF[5] << 8) | (UART_RX_BUF[6] << 0);
        break;
    }
    case 2:
    {
        // 读脉冲数
        // Read_Pulse_Num(step_Motor *motor);
        printf("2\r\n");
        Motor[UART_RX_BUF[0]].pulse_num = ((int32_t)UART_RX_BUF[1] << 24) |
                                          ((int32_t)UART_RX_BUF[2] << 16) |
                                          ((int32_t)UART_RX_BUF[3] << 8) |
                                          ((int32_t)UART_RX_BUF[4] << 0);
        break;
    }
    case 3:
    {
        // 读取电机实时位置
        // Read_Position(step_Motor *motor);
        printf("3\r\n");
        Motor[UART_RX_BUF[0]].position = (((int32_t)UART_RX_BUF[1] << 24) |
                                          ((int32_t)UART_RX_BUF[2] << 16) |
                                          ((int32_t)UART_RX_BUF[3] << 8) |
                                          ((int32_t)UART_RX_BUF[4] << 0)) *
                                         360 / 65535 / Motor[UART_RX_BUF[0]].CONVERT;
        break;
    }
    case 4:
    {
        // 读取位置误差
        // Read_Position_Error(step_Motor *motor);
        printf("4\r\n");
        int16_t buf = (UART_RX_BUF[1] << 8) | (UART_RX_BUF[2] << 0);
        Motor[UART_RX_BUF[0]].postion_error = (buf * 360.0f) / 65536;
        break;
    }
    case 5:
    {
        // 读取使能状态
        // Read_Enable_State(step_Motor *motor);
        printf("5\r\n");
        if (Motor[UART_RX_BUF[0]].motor_type == 1)
        {
            if (UART_RX_BUF[1] == 0x00)
                Motor[UART_RX_BUF[0]].enable_state = 0;
            else if (UART_RX_BUF[1] == 0x01)
                Motor[UART_RX_BUF[0]].enable_state = 1;
        }
        else if (Motor[UART_RX_BUF[0]].motor_type == 0)
        {
            if (UART_RX_BUF[1] == 0x02)
                Motor[UART_RX_BUF[0]].enable_state = 0;
            else if (UART_RX_BUF[1] == 0x01)
                Motor[UART_RX_BUF[0]].enable_state = 1;
        }
        break;
    }
    case 6:
    {
        // 读取堵转标志
        // Read_Lock_State(step_Motor *motor);
        printf("6\r\n");
        if (Motor[UART_RX_BUF[0]].motor_type == 1)
        {
            if (UART_RX_BUF[1] == 0x00)
                Motor[UART_RX_BUF[0]].lock_state = 0;
            else if (UART_RX_BUF[1] == 0x01)
                Motor[UART_RX_BUF[0]].lock_state = 1;
        }
        else if (Motor[UART_RX_BUF[0]].motor_type == 0)
        {
            if (UART_RX_BUF[1] == 0x02)
                Motor[UART_RX_BUF[0]].lock_state = 0;
            else if (UART_RX_BUF[1] == 0x01)
                Motor[UART_RX_BUF[0]].lock_state = 1;
        }

        break;
    }
    case 7:
    {
        // 读取单圈上电自动回零状态标志
        // Read_Zero_State(step_Motor *motor);
        printf("7\r\n");
        if (UART_RX_BUF[1] == 0x00)
            Motor[UART_RX_BUF[0]].zero_state = 0;
        else if (UART_RX_BUF[1] == 0x01)
            Motor[UART_RX_BUF[0]].zero_state = 1;
        break;
    }
    case 9:
    {
        printf("9\r\n");
        break;
    }
    case 10:
    {
        // printf("10\r\n");

        if (UART_RX_BUF[1] == 0x9f)
        {
            Motor[UART_RX_BUF[0]].run = 0;
        }
        // printf("run1=%d,run2=%d,run3=%d\r\n", Motor[1].run, Motor[2].run, Motor[3].run);
        break;
    }
    default:
        break;
    }
    queue[(read_quene - 1) % lenth_of_queue] = 0;
}
