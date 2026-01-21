#include "motor.h"
#include "math.h"
#include "tim.h"
#include "stdio.h"
/*
定义4个步进电机
*/
sStepMotor stepMotor[4];
// 加速步数
int acc_step1 = 0;
int acc_step2 = 0;
int acc_step3 = 0;
int acc_step4 = 0;

// 电机S曲线表格

float fre_X[1000]; // 数组存储加速过程中每一步的频率
float fre_Y[1000]; // 数组存储加速过程中每一步的频率
float fre_Z[1000]; // 数组存储加速过程中每一步的频率
float fre_M[1000];
// 电机S曲线表格
uint16_t period_X[1000]; // 数组储存加速过程中每一步定时器的自动装载值,一个脉冲的周期,注意使用时要/2
uint16_t period_Y[1000]; // 数组储存加速过程中每一步定时器的自动装载值
uint16_t period_Z[1000]; // 数组储存加速过程中每一步定时器的自动装载值
uint16_t period_M[1000];

/*
用户函数
*/
void initMotor()
{
    // 填写插槽4电机的相关参数
    stepMotor[MOTOR_SLOT_4].DIR_GPIOx = GPIOC;
    stepMotor[MOTOR_SLOT_4].DIR_GPIO_Pin = Motor4_Dir_Pin;
    stepMotor[MOTOR_SLOT_4].htimx = htim4;
    stepMotor[MOTOR_SLOT_4].pFrcTable = fre_M;
    stepMotor[MOTOR_SLOT_4].pPeriodTable = period_M;
    stepMotor[MOTOR_SLOT_4].pPeriodTableSizeof = acc_step4;
    stepMotor[MOTOR_SLOT_4].TIM_CHANNEL_x = TIM_CHANNEL_1;

    // 填写插槽3电机的相关参数
    stepMotor[MOTOR_SLOT_3].DIR_GPIOx = GPIOC;
    stepMotor[MOTOR_SLOT_3].DIR_GPIO_Pin = Motor3_Dir_Pin;
    stepMotor[MOTOR_SLOT_3].htimx = htim3;
    stepMotor[MOTOR_SLOT_3].pFrcTable = fre_Z;
    stepMotor[MOTOR_SLOT_3].pPeriodTable = period_Z;
    stepMotor[MOTOR_SLOT_3].pPeriodTableSizeof = acc_step3;
    stepMotor[MOTOR_SLOT_3].TIM_CHANNEL_x = TIM_CHANNEL_1;

    // 填写插槽2电机的相关参数
    stepMotor[MOTOR_SLOT_2].DIR_GPIOx = GPIOC;
    stepMotor[MOTOR_SLOT_2].DIR_GPIO_Pin = Motor2_Dir_Pin;
    stepMotor[MOTOR_SLOT_2].htimx = htim2;
    stepMotor[MOTOR_SLOT_2].pFrcTable = fre_Y;
    stepMotor[MOTOR_SLOT_2].pPeriodTable = period_Y;
    stepMotor[MOTOR_SLOT_2].pPeriodTableSizeof = acc_step2;
    stepMotor[MOTOR_SLOT_2].TIM_CHANNEL_x = TIM_CHANNEL_1;
    // 填写插槽1电机的相关参数
    stepMotor[MOTOR_SLOT_1].DIR_GPIOx = GPIOC;
    stepMotor[MOTOR_SLOT_1].DIR_GPIO_Pin = Motor1_Dir_Pin;
    stepMotor[MOTOR_SLOT_1].htimx = htim1;
    stepMotor[MOTOR_SLOT_1].pFrcTable = fre_X;
    stepMotor[MOTOR_SLOT_1].pPeriodTable = period_X;
    stepMotor[MOTOR_SLOT_1].pPeriodTableSizeof = acc_step1;
    stepMotor[MOTOR_SLOT_1].TIM_CHANNEL_x = TIM_CHANNEL_1;
}

/*
将机械臂配置为速度模式
sStepMotor:指定电机
frequency:指定运行频率 1000Hz时,脉冲周期2ms,电机不细分,200P/圈,电机速度为150转/分钟
                       4000Hz时,                                         600转/分钟
uMotorDir dir:电机运转方向
*/
void configMotorToVelocity(sStepMotor *stepMotor, uMotorDir dir, float rpm) // 初始化电机运动参数
{
    // 对于匀速运动

    uint32_t frequency;
    frequency = 3200 * rpm;

    uint32_t acceleratedSpeedLength = ACCELERATED_SPEED_LENGTH1; // 指定加速步数
    uint32_t frequencyMax = frequency;                           // 指定匀速运动时的频率,亦即最高值
    uint32_t frequencyMin = FRE_MIN;                             // 指定加速曲线的起始频率

    // 指定加速阶段的步数
    stepMotor->stepAccelerationSet = acceleratedSpeedLength;
    // 根据加速段的步数,计算每一步的S曲线表
    CalculateSModelLine(stepMotor->pPeriodTable, stepMotor->pFrcTable, acceleratedSpeedLength, frequencyMax, frequencyMin, 4);

    // 计算匀速运动的步数--对于匀速运动,步数直接指定为32bit能接受的最大值4294967295
    stepMotor->stepConstantSpeedSet = 0xFFFFFFFF; // 加速的步数+减速的步数

    // 初始化数据
    stepMotor->num_callback = 0;           // 初始化脉冲1/2周期控制值
    stepMotor->stepCurrent = 0;            // 初始化电机脉冲计数值
    stepMotor->state = ACCELERATION;       // 设定电机的初始状态
    stepMotor->mode = AT_A_CONSTANT_SPEED; // 指定电机为速度模式
		
    // 设定定时器数值
    __HAL_TIM_SET_AUTORELOAD(&stepMotor->htimx, *(stepMotor->pPeriodTable));                            // 用加速表中的第一个元素初始化定时器溢出值
    __HAL_TIM_SET_COMPARE(&stepMotor->htimx, stepMotor->TIM_CHANNEL_x, *(stepMotor->pPeriodTable) / 2); // 用加速表中的第一个元素初始化定时器比较值,占空比50%,所以正好是1/2

    // 更新电机方向
    stepMotor->dir = dir;

    if (stepMotor->dir == FOR)
    {
        HAL_GPIO_WritePin(stepMotor->DIR_GPIOx, stepMotor->DIR_GPIO_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(stepMotor->DIR_GPIOx, stepMotor->DIR_GPIO_Pin, GPIO_PIN_SET);
    }
}

void configMotorToBreak(sStepMotor * stepMotor)
{
//对于匀速
    //初始化数据
		
		
    stepMotor->state = STOPPING	;						//设定电机的状态
    stepMotor->mode = AT_A_CONSTANT_SPEED;			//指定电机为速度模式
}

/*
启动对应的定时器命令
*/
void motorStartRun(sStepMotor *stepMotor)
{
    HAL_TIM_Base_Start(&stepMotor->htimx);                            // 启动定时器
    HAL_TIM_OC_Start_IT(&stepMotor->htimx, stepMotor->TIM_CHANNEL_x); // 启动定时器8通道1比较输出中断
}

/*
根据提供的电机运行总步数,将电机配置成定位模式
sStepMotor * stepMotor:指定电机
int32_t totalStepSet:电机运行的步数 正数+转 负数反转
float  rpm:运行速度 单位:转/分钟---如果为了简便,这里直接给脉冲频率也行
*/
void configMotorToPosition(sStepMotor *stepMotor, int32_t totalStepSet, float rpm)
{
    int max_fre=2000;
    int min_fre = 800;
    // 计算电机的加速曲线需要的总步数

    if (totalStepSet >= 0)
    {
        stepMotor->stepTotalSet = totalStepSet;
        stepMotor->dir = FOR; // 更新电机方向
        HAL_GPIO_WritePin(stepMotor->DIR_GPIOx, stepMotor->DIR_GPIO_Pin, GPIO_PIN_RESET);
    }
    else
    {
        stepMotor->stepTotalSet = 0 - totalStepSet;
        stepMotor->dir = REV; // 更新电机方向
        HAL_GPIO_WritePin(stepMotor->DIR_GPIOx, stepMotor->DIR_GPIO_Pin, GPIO_PIN_SET);
    }

    // 根据速度计算最高频率
//    max_fre = 1600 * rpm;
    // 这里要验证就一个脉冲怎么处理

    if (stepMotor->stepTotalSet >= 1500)
    {
        acc_step1 =300;
    }
    else if (stepMotor->stepTotalSet <= 1500 && stepMotor->stepTotalSet > 1300)
    {
        acc_step1 = 250;
    }
    else if (stepMotor->stepTotalSet <= 1300 && stepMotor->stepTotalSet > 1100)
    {
        acc_step1 = 200;
    }
    else if (stepMotor->stepTotalSet <= 900 && stepMotor->stepTotalSet > 700)
    {
        acc_step1 = 150;
    }
    else if (stepMotor->stepTotalSet <= 700 && stepMotor->stepTotalSet > 500)
    {
        acc_step1 = 100;
    }
    else if (stepMotor->stepTotalSet <500 && stepMotor->stepTotalSet >200)
    {
        acc_step1 = 50;
    }
    // 计算电机的加速曲线最高速度--这个最高速度就是匀速运动的速度
    // 要小心,还没到匀速就需要减速的情况
    // 要参考机械臂的运动差,计算宏观速度,之后算出微观频率及周期

    // 计算电机的加速曲线的最低速度

    // 计算出S曲线中加速段每一步的表格
    CalculateSModelLine(stepMotor->pPeriodTable, stepMotor->pFrcTable, acc_step1, max_fre, min_fre, 4);

    // 计算加速阶段的步数
    stepMotor->stepAccelerationSet = acc_step1;

    // 计算匀速运动的步数
    stepMotor->stepConstantSpeedSet = stepMotor->stepTotalSet - acc_step1 * 2; // 加速的步数+减速的步数

    // 初始化数据
    stepMotor->num_callback = 0;     // 初始化脉冲1/2周期控制值
    stepMotor->stepCurrent = 0;      // 初始化电机脉冲计数值
    stepMotor->state = ACCELERATION; // 设定电机的初始状态
    stepMotor->mode = POSTION_ONCE;  // 指定电机为定位模式

    // 设定定时器数值
    __HAL_TIM_SET_AUTORELOAD(&stepMotor->htimx, *(stepMotor->pPeriodTable));                            // 用加速表中的第一个元素初始化定时器溢出值
    __HAL_TIM_SET_COMPARE(&stepMotor->htimx, stepMotor->TIM_CHANNEL_x, *(stepMotor->pPeriodTable) / 2); // 用加速表中的第一个元素初始化定时器比较值,占空比50%,所以正好是1/2
}

/*
S型加减速计算
函数说明：计算出每一部所需的计数值,产生S型曲线表格
生成每一个细分步定时器的自动重装载值（实际就是改变脉冲的周期
calculate the Period and Freq array value, fill the Period value into the Period register during the timer interrupt.
*		calculate the acceleration procedure , a totally 1000 elements array.
* 	parameter    fre[]: point to the array that keeps the freq value.--使用数组,估计是不是仅仅用来调试用???
*   uint16_t *pPeriod[]: point to the array that keeps the timer period value.
*   len: the procedure of acceleration length.it is best thing to set the float number, some compile software maybe transfer error if set it as a int
*   fre_max: maximum speed, frequency vale.
*   fre_min: start minimum speed, frequency vale.  mind : 10000000/65535 = 152, so fre_min can't less than 152.
*   flexible:  flexible value. adjust the S curves
*/
void CalculateSModelLine(uint16_t *pPeriod, float *pFre, float len, float fre_max, float fre_min, float flexible)
{
    int i = 0;
    float deno;
    float melo;
    float delt = fre_max - fre_min;

    for (; i < len; i++)
    {
        /*
        -flexible*(i - num)/num是对S型曲线进行拉伸变化，
        其中flexible代表S曲线区间（越大代表压缩的最厉害，中间（x坐标0点周围）加速度越大；越小越接近匀加速。理想的S曲线的取值为4-6），
        i是在循环计算过程中的索引，从0开始，
        num为 length/2 大小(这样可以使得S曲线对称)。
        在项目中i的区间[0,1000), num=1000/2=500。这些参数均可以修改。
        */
        melo = flexible * (i - len / 2) / (len / 2); // x值 对S曲线进行拉伸变化
        deno = 1.0f / (1 + expf(-melo));             // expf is a library function of exponential(e)?	 y值
        *(pFre + i) = delt * deno + fre_min;
        *(pPeriod + i) = (unsigned short)(4500000.0f / *(pFre + i)); // 10000000 is the timer driver frequency//这里我改动了
    }
}

/*
捕获比较中断调用的函数,用来产生和控制脉冲
stepMotor:电机数据,例如可以写&stepMotor[0]
*htim:定时器指针 例如可以写:&htim1
Channel:定时器通道 例如可以写:TIM_CHANNEL_1
TIM_FLAG_CCx:捕获比较的通道号码 例如 TIM_FL

AG_CC1
*/
void stepMotorProcess(sStepMotor *stepMotor)
{
    uint32_t TIM_FLAG_CCx; // 定义捕获通道
    /*
    根据定时器的通道信息,计算捕获通道寄存器数值,用来清除寄存器标志位
    */
    stepMotor->num_callback++;

    /*
    产生电平的过程:
    开始中断前,引脚为低电平,之后指定比较值和溢出值
                      比较
                 溢出  |
        比较   |     |
    起始   |     |     |
    |      |     |     |
    0      1     2     3
           ___________
    _______|     .    |________

    */

    if (stepMotor->num_callback % 2 == 0) // 这个是不是用来判断奇数/偶数呢,应该是产生一个高电平接一个低电平之后,才更新溢出值吗?似乎不用????
    {
        // 记录电机的绝对位置
        if (stepMotor->dir == FOR)
        {
            stepMotor->stepCurrentABS++;
        }
        else
        {
            stepMotor->stepCurrentABS--;
        }

        // 记录走过的相对总步数---这个数字在定位完成后,会被清零(也就是StepTotalSet完成后)
        stepMotor->stepCurrent++;

        switch (stepMotor->state)
        {
        case ACCELERATION:     
                                                                                                            // 加速
						printf("加速%d\r\n",stepMotor->stepCurrent);
            __HAL_TIM_SET_AUTORELOAD(&(stepMotor->htimx), *(stepMotor->pPeriodTable + stepMotor->stepCurrent));                            // 定时器溢出后,计数器会清零,重新指定溢出值
            __HAL_TIM_SET_COMPARE(&(stepMotor->htimx), stepMotor->TIM_CHANNEL_x, *(stepMotor->pPeriodTable + stepMotor->stepCurrent) / 2); // 更新计数器的比较值,正好是溢出值的一半 50%的占空比

            if (stepMotor->stepCurrent >= (stepMotor->stepAccelerationSet) - 1) // 判断是否到达匀速条件,这里-1,是因为起始从0开始的
            {
                stepMotor->state = CONSTANT_SPEED;
            }

            break;

        case CONSTANT_SPEED:                   // 匀速
						printf("匀速%d\r\n",stepMotor->stepConstantSpeedSet);
            stepMotor->stepConstantSpeedSet--; // 这里最好再显式的计算一下溢出值和比较值,后期可能需要电机直接跳转匀速状态

            if (stepMotor->stepConstantSpeedSet < 1)
                stepMotor->state = DECELERATION;

            break;

        case DECELERATION: // 减速
						printf("减速%d\r\n",stepMotor->stepAccelerationSet);
            stepMotor->stepAccelerationSet--;
            __HAL_TIM_SET_AUTORELOAD(&(stepMotor->htimx), *(stepMotor->pPeriodTable + stepMotor->stepAccelerationSet));
            __HAL_TIM_SET_COMPARE(&(stepMotor->htimx), stepMotor->TIM_CHANNEL_x, *(stepMotor->pPeriodTable + stepMotor->stepAccelerationSet) / 2);

            if (stepMotor->stepAccelerationSet < 1)
                stepMotor->state = STOPPING;

            break;

        case STOPPING: // 停止
            switch (stepMotor->TIM_CHANNEL_x)
            {
            case TIM_CHANNEL_1:
                TIM_FLAG_CCx = TIM_FLAG_CC1;
                break;

            case TIM_CHANNEL_2:
                TIM_FLAG_CCx = TIM_FLAG_CC2;
                break;

            case TIM_CHANNEL_3:
                TIM_FLAG_CCx = TIM_FLAG_CC3;
                break;

            case TIM_CHANNEL_4:
                TIM_FLAG_CCx = TIM_FLAG_CC4;
                break;

            default:
                break;
            }

            // 关闭通道--同时复位数据,以用于下次使用
            stepMotor->stepCurrent = 0;
            TIM_CCxChannelCmd((stepMotor->htimx).Instance, stepMotor->TIM_CHANNEL_x, TIM_CCx_DISABLE); // 原有你的程序是TIM8????
            __HAL_TIM_CLEAR_FLAG(&(stepMotor->htimx), TIM_FLAG_CCx);
            HAL_TIM_Base_Stop(&(stepMotor->htimx));
            HAL_TIM_OC_Stop_IT(&(stepMotor->htimx), stepMotor->TIM_CHANNEL_x);
            __HAL_TIM_SET_COUNTER(&(stepMotor->htimx), 0); // 计数值清零

            // 降低电流
            // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
            // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
            // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

            break;

        default:
            break; // switch 标准
        }
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 电机物理接口从上至下
    // 电机物理接口4
    // 电机物理接口3
    // 电机物理接口2
    // 电机物理接口1
    // TIM4--电机物理接口4
    // TIM2--电机物理接口3
    // TIM1--电机物理接口2
    // TIM3--电机物理接口1

    if (htim->Instance == stepMotor[MOTOR_SLOT_4].htimx.Instance) // 如果是4号插槽的定时器动作TIM4
    {
        stepMotorProcess(&stepMotor[MOTOR_SLOT_4]);
			
    }
    else if (htim->Instance == stepMotor[MOTOR_SLOT_3].htimx.Instance) // 如果是3号插槽的定时器动作TIM2
    {
        stepMotorProcess(&stepMotor[MOTOR_SLOT_3]);
			
    }
    else if (htim->Instance == stepMotor[MOTOR_SLOT_2].htimx.Instance) // 如果是2号插槽的定时器动作TIM1
    {
        stepMotorProcess(&stepMotor[MOTOR_SLOT_2]);
	
    }
    else if (htim->Instance == stepMotor[MOTOR_SLOT_1].htimx.Instance) // 如果是1号插槽的定时器动作TIM3
    {
        stepMotorProcess(&stepMotor[MOTOR_SLOT_1]);
				
    }
    else
        ;
}

// void steering_gear(TIM_HandleTypeDef htimx, int TIM_CHANNEL, int y)
//{
//     HAL_TIM_PWM_Start(&htimx, TIM_CHANNEL);
//     __HAL_TIM_SET_COMPARE(&htimx, TIM_CHANNEL, y);
//		HAL_Delay(2000);
//

//}
