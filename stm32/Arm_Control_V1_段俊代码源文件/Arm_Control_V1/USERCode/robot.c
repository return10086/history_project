
#include "robot.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "motor.h"
#include "tim.h"
#include "main.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

int all_step_1;
int all_step_2;
int all_step_3;
float L1 = 100.0; // 杆臂1长度
float L2 = 100.0; // 杆臂2长度
float sigma3, sigma4, sigma5;
float sigma1 = pi / 4;
float sigma2 = -0.0f;
float T_sigma1;
float T_sigma2;
float last_sigma1;
float last_sigma2;
float L1_sigma1;
float L2_sigma2;
float last_arm = 0.136111 * pi; // 末端执行器水平夹角
float current_sigma;
float current_high = 0;
uint8_t step = 0;
uint8_t step2 = 0;
int flag1 = 0;
int flag2 = 0;
float speed1 = 0;
float speed2 = 0;
float speed3 = 0;
float time = 0.4;
sROBOTIC_ARM scara = {0};

void initScara()
{

	initMotor();
	scara.pStepMotorUpArm = &stepMotor[MOTOR_SLOT_1];	// 杆臂1电机绑定
	scara.pStepMotorForearm = &stepMotor[MOTOR_SLOT_2]; // 杆臂2电机绑定
	scara.pStepMotorZ = &stepMotor[MOTOR_SLOT_4];		// 升降电机绑定
}

void init_arm()
{
	// 步进电机转动步数
	all_step_1 = 0;
	all_step_2 = 0;
	all_step_3 = 0;
	// 杆臂1、2长度
	L1 = 100.0;
	L2 = 100.0;
	//
	sigma3 = 0;
	sigma4 = 0;
	sigma5 = 0;
	// 初始杆臂姿态信息
	sigma1 = -pi / 4;	 // 杆臂1水平夹角
	sigma2 = 3 / 4 * pi; // 杆臂2与杆臂1夹角
	// sigma2 = pi * 0.0f;
	T_sigma1 = 0;
	T_sigma2 = 0;
	// 杆臂1、2姿态缓存区
	last_sigma1 = 0;
	last_sigma2 = 0;
	// 初始化缓存区
	L1_sigma1 = 0;
	L2_sigma2 = 0;
	last_arm = pi / 6;
	current_sigma = 0;
	current_high = 0;
	uint8_t step = 0;
	uint8_t step2 = 0;
	flag1 = 0;
	flag2 = 0;
	speed1 = 0;
	speed2 = 0;
	speed3 = 0;
}

/*
	反正切函数求解
*/
float actan(float x, float y) // 反解目标点相位角
{

	float alpha;
	if (x > 0)
		alpha = atan(y / x);
	else if (x < 0 && y >= 0)
		alpha = atan(y / x) + pi;
	else if (x < 0 && y < 0)
		alpha = atan(y / x) - pi;
	else if (x == 0 && y > 0)
		alpha = pi / 2;
	else if (x == 0 && y < 0)
		alpha = -pi / 2;

	/*C语言库自带函数，不需要考虑输入值正负号以及是否为0--DJ*/
	// alpha = atan2(y, x);

	return alpha;
}

/*
	杆臂1、2姿态解算，不包括Z轴坐标
	@brief:这条语句用于不需要改变Z轴坐标时对目标点的解算，即仅计算杆臂1、2的转动角度。
	@attention:这里使用静态语句，外部无法访问或者调用此语句。为了避免直接调用此语句而未改变相应姿态数据
*/

static void robot_arm(float x, float y)
{

	float c2, s2, r;   // s2、c2--杆臂1、2夹角正余弦值
	float beta, alpha; //
	float sin_b, cos_b;
	r = sqrt(x * x + y * y);									// 目标点到机械臂原点坐标距离
	c2 = ((x * x + y * y - L1 * L1 - L2 * L2) / (2 * L1 * L2)); // 杆臂1、2补角夹角余弦值求解。
	s2 = sqrt(1 - c2 * c2);										// 杆臂夹角正弦值，这里可能存在两个值。
	// sigma2 = actan(c2, s2);										// 反解杆臂1、2夹角
	sigma2 = atan2(c2, s2);
	// alpha = actan(x, y);
	alpha = atan2(x, y);
	sin_b = (L2 * s2 / r);
	cos_b = ((L1 + L2 * c2) / r);
	// beta = actan(cos_b, sin_b);
	beta = atan2(cos_b, sin_b);
	sigma1 = alpha - beta; // 这里解算的是杆臂1中心线在原点与目标点连线下方的情况,一般情况会有两组解。

	/************原始杆臂运动范围限制语句******************/
	if (sigma1 < -pi)
	{
		sigma1 = 2 * pi + sigma1;
	}
	if (sigma1 > -pi && sigma1 <= -pi / 2)
	{
		sigma1 = sigma1 + 2 * pi; // 这里因为现实机械臂无法实现这个角度逆向旋转。
	}
	/**************************************************/

	// /*这里是杆臂1、2的运动范围限制--DJ*/
	// if (sigma1 < pi / 4 /*|| sigma1 > 0.638888 * pi*/) // 现实机械臂逆向运动限位
	// {
	// 	flag1 = 1;
	// 	printf("Joint 1 Over Limitation\n");
	// }
	// else
	// {
	// 	flag1 = 0;
	// }

	// if (sigma2 > 0.772222 * pi || sigma2 < -0.77222 * pi)
	// {
	// 	flag2 = 1;
	// 	printf("Joint 2 Over Limitation\n");
	// }
	// else
	// {
	// 	flag2 = 0;
	// }
	/*待验证*/
}

/*
	机械臂工作台姿态解算，包括Z轴坐标
*/
void motor_result(sROBOTIC_ARM *pScara, float x, float y, float z)
{
	last_sigma1 = sigma1;
	last_sigma2 = sigma2;
	robot_arm(x, y);
	if (sigma1 > last_sigma1) // ①杆臂1目标角度大于当前角度，正向转动
	{
		L1_sigma1 = sigma1 - last_sigma1;							 // 杆臂1角度差值（正值）
		all_step_1 = -((L1_sigma1)*180 / pi) / 360 * 3200 * 21 / 10; // 弧度转换，计算1号电机实际需要转动步数。
		pScara->pStepMotorUpArm->stepTotalSet = (int32_t)all_step_1; // 传入结构体参数。
	}
	else if (sigma1 <= last_sigma1) // ②杆臂1目标角度小于或当前角度，逆向旋转或不转
	{
		L1_sigma1 = sigma1 - last_sigma1;							 // 杆臂1角度差值（负值）
		all_step_1 = -((L1_sigma1)*180 / pi) / 360 * 3200 * 21 / 10; // 弧度转换，计算1号电机实际需要转动步数（负值）
		pScara->pStepMotorUpArm->stepTotalSet = (int32_t)all_step_1; // 、、、、、
	}

	/******************************************这一部分操作原因没懂***************************************/
	current_sigma = L1_sigma1 + last_arm; // 当前末端执行器水平夹角
	if (current_sigma > (pi - sigma2))
	{
		L2_sigma2 = (pi - sigma2) - current_sigma;
		last_arm = (pi - sigma2);
		all_step_2 = -((L2_sigma2)*180 / pi) / 360 * 3200 * 9 / 4;
		pScara->pStepMotorForearm->stepTotalSet = (int32_t)all_step_2;
	}
	else if (current_sigma <= (pi - sigma2))
	{
		L2_sigma2 = (pi - sigma2) - current_sigma;
		last_arm = (pi - sigma2);
		all_step_2 = -((L2_sigma2)*180 / pi) / 360 * 3200 * 9 / 4;
		pScara->pStepMotorForearm->stepTotalSet = (int32_t)all_step_2;
	}
	/*************************************************************************************************/

	// 转轴1解算（测试--DJ）
	// L1_sigma1 = sigma1 - last_sigma1;							 // 杆臂1角度差值（正值）
	// all_step_1 = ((L1_sigma1)*180 / pi) / 360 * 3200 * 21 / 10; // 弧度转换，计算1号电机实际需要转动步数。 这里正负需要考虑到步进电机驱动器上方向引脚的电平高低
	// pScara->pStepMotorUpArm->stepTotalSet = (int32_t)all_step_1; // 传入结构体参数。

	// // 转轴2解算（测试--DJ）
	// L2_sigma2 = sigma2 - last_sigma2;
	// all_step_2 = ((L2_sigma2)*180 / pi) / 360 * 3200 * 9 / 4;
	// pScara->pStepMotorForearm->stepTotalSet = (int32_t)all_step_2;

	speed1 = (L1_sigma1 * 180 / pi) / 360 / time * 2.4;
	speed2 = (L2_sigma2 * 180 / pi) / 360 / time * 2.4;
	if (all_step_1 > 2699)
	{
		all_step_1 = 2659;
	}
}
/*
 */
void move(float x, float y, float z)
{

	motor_result(&scara, x, y, z); //
	// HAL_GPIO_WritePin(GPIOC, Motor4_En_Pin | Motor3_En_Pin, GPIO_PIN_RESET);
	// HAL_GPIO_WritePin(GPIOB, Motor2_En_Pin | Motor1_En_Pin, GPIO_PIN_RESET);

	// movez(z);

	configMotorToPosition(scara.pStepMotorUpArm, all_step_1, 1.5); //
	configMotorToPosition(scara.pStepMotorForearm, all_step_2, 0.9);

	if (!flag1)
	{
		motorStartRun(&stepMotor[MOTOR_SLOT_1]);
	}
	if (!flag2)
	{
		motorStartRun(&stepMotor[MOTOR_SLOT_2]);
	}
	HAL_Delay(4000);
	//
}

void movez(float z)
{
	all_step_3 = (int16_t)(z - current_high) / 4 * 200;
	current_high = z;
	configMotorToPosition(scara.pStepMotorZ, all_step_3, 10);
	motorStartRun(&stepMotor[MOTOR_SLOT_4]);
	if (abs(z - current_high) >= 150)
	{
		HAL_Delay(20000);
	}
	else if (abs(z - current_high) >= 100)
	{
		HAL_Delay(12000);
	}
	else if (abs(z - current_high) && abs(z - current_high) < 100)
	{
		HAL_Delay(5000);
	}
	else
	{
		HAL_Delay(3000);
	}
}

// void moveHome(float x, float y, float z)
//{
//	if (x <= 0)
//	{
//		all_step_1 = 0 - ((fabs(sigma1 + pi / 6) * 180 / pi) / 360 * 1600 * 48 / 20);
//		HAL_GPIO_WritePin(Motor2_Dir_GPIO_Port, Motor2_Dir_Pin, GPIO_PIN_RESET);
//		all_step_2 = 0;
//		HAL_GPIO_WritePin(Motor3_Dir_GPIO_Port, Motor3_En_Pin, GPIO_PIN_SET);
//	}
//	else
//	{
//		all_step_1 = 0 - ((fabs(sigma1 + pi / 6) * 180 / pi) / 360 * 1600 * 48 / 20);
//		HAL_GPIO_WritePin(GPIOB, Motor2_Dir_Pin, GPIO_PIN_RESET);
//		all_step_2 = (fabs(pi - sigma2 - 0.136111 * pi - (sigma1 + pi / 6)) * 180 / pi) / 360 * 1600 * 48 / 20;
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOC, Motor3_En_Pin, GPIO_PIN_RESET);
//	}
//	all_step_3 = 0 - (int16_t)(z) / 4 * 1600;
//	{
//		sigma1 = -pi / 6;
//		sigma2 = 0.86111 * pi;
//		last_arm = 0.136111 * pi;
//		T_sigma1 = 0;
//		T_sigma2 = 0;
//		last_sigma1 = 0;
//		last_sigma2 = 0;
//		L1_sigma1 = 0;
//		L2_sigma2 = 0;
//		last_arm = 0.136111 * pi;
//		current_sigma = 0;
//	}
//	HAL_GPIO_WritePin(GPIOC, Motor4_En_Pin | Motor2_En_Pin, GPIO_PIN_RESET);
//	configMotorToPosition(scara.pStepMotorUpArm, all_step_1, speed1); //?????????????
//	configMotorToPosition(scara.pStepMotorForearm, all_step_2, speed2);
//	configMotorToPosition(scara.pStepMotorZ, all_step_3, 70);

//	motorStartRun(&stepMotor[MOTOR_SLOT_2]);
//	motorStartRun(&stepMotor[MOTOR_SLOT_3]);
//}

// void movehome()
//{
//	steering_gear(htim5, TIM_CHANNEL_1, 15);

//	configMotorToVelocity(&stepMotor[MOTOR_SLOT_2], REV, 1);
//	configMotorToVelocity(&stepMotor[MOTOR_SLOT_4], REV, 10);
//	while (1)
//	{
//		if (HAL_GPIO_ReadPin(GPIOB, LIMIT3_Pin) == 1)
//		{
//			configMotorToBreak(&stepMotor[MOTOR_SLOT_2]);

//			break;
//		}
//		else
//		{
//			switch (step)
//			{
//			case 0:
//				configMotorToVelocity(&stepMotor[MOTOR_SLOT_2], REV, 1);
//				motorStartRun(&stepMotor[MOTOR_SLOT_2]);
//				step++;
//				break;
//			case 1:
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	// z?????
//	while (1)
//	{
//		if (HAL_GPIO_ReadPin(GPIOC, LIMIT4_Pin) == 1)
//		{
//			configMotorToBreak(&stepMotor[MOTOR_SLOT_4]);
//			break;
//		}
//		else
//		{
//			switch (step2)
//			{
//			case 0:
//				configMotorToVelocity(&stepMotor[MOTOR_SLOT_4], REV, 10);
//				motorStartRun(&stepMotor[MOTOR_SLOT_4]);
//				step2++;
//				break;
//			case 1:
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	init_arm();
//	step = 0;
//	step2 = 0;
//	steering_gear(htim5, TIM_CHANNEL_1, 15);
//}
