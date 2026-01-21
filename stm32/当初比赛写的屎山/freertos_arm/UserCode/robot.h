/*
 * @Author: 程佳乐 &&cjl2646289864@gmail.com
 * @Date: 2023-08-18 06:31:56
 * @LastEditors: 程佳乐 &&cjl2646289864@gmail.com
 * @LastEditTime: 2023-09-06 00:10:45
 * @FilePath: \MDK-ARMe:\project\stm32\freertos_arm\UserCode\robot.h
 * @Description:
 *
 * Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved.
 */
#ifndef _ROBOT_H
#define _ROBOT_H

#include "MyProject.h"

#define LENTH1 180     // 延伸臂长
#define LENTH2 180     // 小臂长
#define CONVERT_Z 0.0  // z轴传动比
#define CONVERT_R1 3.0 // 大臂传动比
#define CONVERT_R2 3.0 // 小臂传动比

typedef struct
{
    float tar_x, tar_y, tar_z;  // 目标位置
    float cur_x, cur_y, cur_z;  // 当前位置
    float tar_beta1, tar_beta2; // 目标两轴角度
    float cur_beta1, cur_beta2; // 当前两轴角度
    // float position_store[100][3]; // 拆分后的位置
    // float angle_store[100][3];    // 拆分后的角度
} _ARM;

extern _ARM ARM;
void Init_Arm(float x, float y, float z);
void Set_Target_Point(float x, float y, float z);
void position_to_angle(int adjust);
#endif
