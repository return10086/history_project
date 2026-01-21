/*
 * @作者: 程佳乐
 * @Date: 2023-03-10 13:08:51
 * @LastEditTime: 2023-04-11 14:28:14
 * @描述:
 * @FilePath: \vscodec:\Users\程佳乐\Desktop\PID.h
 * @人生目标: 全世界无产者联合起来
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_
#include "main.h"
// PID相关
typedef struct
{
    float SetTarget;  // 目标值
    float Proportion; // 比例系数
    float Integral;   // 积分系数
    float Derivate;   // 微分系数
    float error[3];   // 误差项 0当前 1 上次 2上上次

    float Pout; // 比例输出
    float Iout; // 积分输出
    float Dout; // 微分输出

    float max_output;     // 限幅最大输出
    float max_iout;       // 积分项限幅
    float integral_start; // 积分分离，偏差较大时去掉积分作用

    int mode; // 选择增量式（0），位置式（1）
} PID;
extern PID pid_bottom,pid_top;
/*限幅*/
void LimitMax(float input, float max);
//*初始化*/
void incPIDInit(PID *sPID, float Proportion, float Integral, float Derivate, float SetTarget, int mode);
/*改设定目标值*/
void changeTarget(PID *sPID, float SetTarget);
/*PID更新*/
float PIDCalc(PID *sPID, float Value);

#endif

