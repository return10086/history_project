/*
 * @Author: 程佳乐 &&cjl2646289864@gmail.com
 * @Date: 2023-08-18 06:31:56
 * @LastEditors: 程佳乐 &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-08-29 09:18:34
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\robot.c
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#include "robot.h"

_ARM ARM;

/**
 * @description: 初始化机械臂
 * @param {float} x
 * @param {float} y：三轴位置
 * @param {float} z
 * @return {*}
 */
void Init_Arm(float x, float y, float z)
{
    ARM.cur_x = x;
    ARM.cur_y = y;
    ARM.cur_z = z;
    position_to_angle(1);
    for (int i = 0; i <= motor_num; i++)
    {
        Motor[i].id = (ID)i;
        Motor[i].enable_state = 1;
        Motor[i].encoder = 0;
        Motor[i].lock_state = 0;
        Motor[i].postion_error = 0;
        Motor[i].position = 0;
        Motor[i].pulse_num = 0;
        Motor[i].zero_state = 0;
        Motor[i].Mstep = 256;
        Motor[i].run = 0;
    }
    Motor[1].motor_type = 1;
    Motor[2].motor_type = 1;
    Motor[3].motor_type = 1;
    Motor[0].send_usart = huart2;
    Motor[1].send_usart = huart2;
    Motor[2].send_usart = huart2;
    Motor[3].send_usart = huart2;
    Motor[1].CONVERT = 3.0f;
    Motor[2].CONVERT = 3.0f;
    Motor[3].CONVERT = 40.0f; //(360/8)
}

/**
 * @description: 重设目标点
 * @param {float} x
 * @param {float} y：三轴位置
 * @param {float} z
 * @return {*}
 */
void Set_Target_Point(float x, float y, float z)
{
    ARM.tar_x = x;
    ARM.tar_y = y;
    ARM.tar_z = z;
}

/**
 * @description: 三轴位置->三个电机角度(0:目标点->目标角度  1：当前点->当前角度)
 * @return {*}
 */
void position_to_angle(int adjust)
{
    if (adjust == 1)
    {
        float angle1 = atan2(ARM.cur_y, ARM.cur_x);
        float lenth = sqrt(ARM.cur_x * ARM.cur_x + ARM.cur_y * ARM.cur_y);
        float angle2 = acos(pow(lenth, 2) / (2 * lenth * 180));
        float angle3;
        if (ARM.cur_x * ARM.cur_y > 0)
        {
            angle3 = angle1 - angle2;
            ARM.cur_beta2 = 2 * angle2 / PI * 180;
        }
        else
        {
            angle3 = angle1 + angle2;
            ARM.cur_beta2 = -(2 * angle2 / PI * 180);
        }
        ARM.cur_beta1 = angle3 / PI * 180;
    }
    else if (adjust == 0)
    {
        float angle1 = atan2(ARM.tar_y, ARM.tar_x);
        float lenth = sqrt(pow(ARM.tar_x, 2) + pow(ARM.tar_y, 2));
        float angle2 = acos(pow(lenth, 2) / (2 * lenth * 180));
        float angle3;
        if (ARM.tar_x * ARM.tar_y > 0)
        {
            angle3 = angle1 - angle2;
            ARM.tar_beta2 = 2 * angle2 / PI * 180;
        }
        else
        {
            angle3 = angle1 + angle2;
            ARM.tar_beta2 = -(2 * angle2 / PI * 180);
        }
        ARM.tar_beta1 = angle3 / PI * 180;
    }
}