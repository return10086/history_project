
/*

robot.h

*/
#include "motor.h"

#ifndef _SCARA_ROBOT_H_
#define _SCARA_ROBOT_H_

// 机械臂解算相关参数
#define pi 3.1415

// 定义机械臂数据结构--即用数据的方法来描述一个SCARA机械臂
typedef struct
{
    float posX;                    // 当前x坐标
    float posY;                    // 当前y坐标
    float posZ;                    // 当前z坐标
    float angleUpArm;              // 大臂角度
    float angleForearm;            // 小臂角度
    uint16_t angleClamp;           // 手抓角度
    sStepMotor *pStepMotorUpArm;   // 指向大臂电机的数据
    sStepMotor *pStepMotorForearm; // 指向小臂电机的数据
    sStepMotor *pStepMotorZ;       // 指向Z轴电机的数据
} sROBOTIC_ARM;

// 初始化机械臂的数据,在使用机械臂之前,必须最先调用
void initScara(void);

float actan(float x, float y);

/*
解算绝对坐标
x:
y:
*/
static void robot_arm(float x, float y);

/*
解算相对坐标
x:
y:
z:
*/
void motor_result(sROBOTIC_ARM *scara, float x, float y, float z);

// 数学归零
//  当在第二象限时，将小臂使能撤掉。大臂旋转的度数为绝对解算的度数减去初始度数
// 当在第一象限时，大臂与上述方法相同，小臂旋转度数为180-大臂旋转度数-解算度数-初始度数
// 若不对变量初始化则相对解算会出错
//  HAL_GPIO_WritePin(GPIOB, DIR4_Pin, GPIO_PIN_SET);//DIR1 Z轴低电平向无电机一侧转动
//  HAL_GPIO_WritePin(GPIOB, DIR2_Pin, GPIO_PIN_SET);//DIR2 大臂高电平逆时针转动
//  HAL_GPIO_WritePin(GPIOB, DIR3_Pin, GPIO_PIN_SET);//DIR3 小臂高电平逆时针转动
// CalculateSModelLine(fre,period,ACCELERATED_SPEED_LENGTH,FRE_MAX,FRE_MIN,4);
void moveHome(float x, float y, float z);
void move(float x, float y, float z);
void movez(float z);

#endif
