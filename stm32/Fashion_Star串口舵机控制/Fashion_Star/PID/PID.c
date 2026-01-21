/*
 * @作者: 程佳乐
 * @Date: 2023-03-10 13:08:50
 * @LastEditTime: 2023-04-11 14:28:20
 * @描述:
 * @FilePath: \vscodec:\Users\程佳乐\Desktop\PID.c
 * @人生目标: 全世界无产者联合起来
 */

#include <PID.h>
#include "math.h"
PID pid_bottom,pid_top;
/**
 * @description: 控制单次调整值
 * @param {float} input：输入
 * @param {float} max：限幅绝对值
 * @return {*}
 */
void LimitMax(float input, float max)
{
    if (input > max)
    {
        input = max;
    }
    else if (input < -max)
    {
        input = -max;
    }
}
/**
 * @description:
 * @param {PID} *sPID：PID指针
 * @param {float} Proportion:积分系数
 * @param {float} Integral：积分系数
 * @param {float} Derivate：微分系数
 * @param {float} SetTarget：设定目标值
 * @param {int} mode：0:增量式/1：位置式
 * @return {*}
 */
void incPIDInit(PID *sPID, float Proportion, float Integral, float Derivate, float SetTarget, int mode)
{
    sPID->error[0] = 0.0f;
    sPID->error[1] = 0.0f;
    sPID->error[2] = 0.0f;

    sPID->mode = mode;
    sPID->Proportion = Proportion;
    sPID->Integral = Integral;
    sPID->Derivate = Derivate;
    sPID->SetTarget = SetTarget;
}
/**
 * @description: 更改PID目标值
 * @param {PID} *sPID：PID指针
 * @param {float} SetTarget：更改的目标值
 * @return {*}
 */
void changeTarget(PID *sPID, float SetTarget)
{
    sPID->SetTarget = SetTarget;
}
/**
 * @description: 增量式/位置式PID
 * @param {PID} *sPID：PID指针
 * @param {float} Value：传感器回传值
 * @return {*}：PID返回值
 */
float PIDCalc(PID *sPID, float Value)
{
    if (sPID->mode == 1)
    { // 位置式
        float iIncpid;
        // 更新误差值
        sPID->error[2] = sPID->error[1];
        sPID->error[1] = sPID->error[0];
        sPID->error[0] = sPID->SetTarget - Value;
        // 比例项输出
        sPID->Pout = sPID->Proportion * sPID->error[0];
        // 积分项输出
        if ((sPID->error[0] > (-sPID->integral_start)) && (sPID->error[0] < sPID->integral_start))
        { // 如果当前误差值没有过于离谱(误差值还算正常)
            sPID->Iout += sPID->Integral * sPID->error[0];
        }
        else
        { // 误差值不正常，停止积分项作用
            sPID->Iout = 0.0f;
        }
        // 微分项输出
        sPID->Dout = sPID->Derivate * (sPID->error[0] - sPID->error[1]);
        // 积分项限幅
        float max_iout = 10; /*自改*/
        LimitMax(sPID->Iout, max_iout);
        // 最终输出
        iIncpid = sPID->Pout + sPID->Iout + sPID->Dout;
        // 输出限幅
        float max_out = 1000; /*自改*/
        LimitMax(iIncpid, max_out);
        return iIncpid;
    }
    else
    { // 增量式
        float iIncpid;
        // 更新误差值
        sPID->error[2] = sPID->error[1];
        sPID->error[1] = sPID->error[0];
        sPID->error[0] = sPID->SetTarget - Value;
        // 比例项输出
        sPID->Pout = sPID->Proportion * (sPID->error[0] - sPID->error[1]);
        // 积分项输出
        sPID->Iout = sPID->Integral * sPID->error[0];
        // 微分项输出
        sPID->Dout = sPID->Derivate * (sPID->error[0] - 2 * sPID->error[1] + sPID->error[2]);
        // 最终输出
        iIncpid = sPID->Pout + sPID->Iout + sPID->Dout;
        // 输出限幅
        float max_out = 1000; /*自改*/
        LimitMax(iIncpid, max_out);
        return iIncpid;
    }
}
