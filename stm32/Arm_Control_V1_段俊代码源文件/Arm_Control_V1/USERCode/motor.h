

#include "main.h"

#ifndef _STEP_MOTOR_H_
#define _STEP_MOTOR_H_




/* S型加速参数 */
#define ACCELERATED_SPEED_LENGTH1 10  //定义加速时,电机走的脉冲数量---电机1?
#define ACCELERATED_SPEED_LENGTH2 10  //定义加速度的点数（其实也是3000个细分步的意思）----电机2?
#define ACCELERATED_SPEED_LENGTH3 10  //定义加速时,电机走的脉冲数量---电机1?
#define ACCELERATED_SPEED_LENGTH4 10  //定义加速度的点数（其实也是3000个细分步的意思）----电机2?

#define FRE_MIN 0  	 //最低的运行频率，调这个参数调节最低运行速度
#define FRE_MAX 3500 //最高的运行频率，调这个参数调节匀速时的最高速度3500





//定义硬件的插槽顺序---自下而上
#define MOTOR_SLOT_4 3  //其中的数字为电机stuct对应的数字
#define MOTOR_SLOT_3 2
#define MOTOR_SLOT_2 1
#define MOTOR_SLOT_1 0


//正视机械臂，从左往右，从上到下编号（1-4）

//大臂电机方向引脚宏定义
#define MOTOR_UPARM_DIR_FOR 		HAL_GPIO_WritePin(Motor1_Dir_GPIO_Port, Motor1_Dir_Pin, GPIO_PIN_SET)
#define MOTOR_UPARM_DIR_REV 		HAL_GPIO_WritePin(Motor1_Dir_GPIO_Port, Motor1_Dir_Pin, GPIO_PIN_RESET)
//小臂电机方向引脚宏定义
#define MOTOR_FOREARM_DIR_FOR 	HAL_GPIO_WritePin(Motor2_Dir_GPIO_Port, Motor2_Dir_Pin, GPIO_PIN_SET)
#define MOTOR_FOREARM_DIR_REV 	HAL_GPIO_WritePin(Motor2_Dir_GPIO_Port, Motor2_Dir_Pin, GPIO_PIN_RESET)
//升降电机方向引脚宏定义
#define MOTOR_Z_DIR_FOR 				HAL_GPIO_WritePin(Motor4_Dir_GPIO_Port, Motor4_DIR_Pin, GPIO_PIN_SET)
#define MOTOR_Z_DIR_REV 				HAL_GPIO_WritePin(Motor4_Dir_GPIO_Port, Motor4_DIR_Pin, GPIO_PIN_RESET)

/*
定义电机的运行状态
*/
typedef enum
{
    STOPPING = 0,			/*停止状态,=0为显式定义*/
    ACCELERATION,		/*加速状态*/
    CONSTANT_SPEED,	/*匀速状态*/
    DECELERATION 		/*减速状态*/
} uMotorState;

/*
定义电机运行模式
*/
typedef enum
{
    POSTION_ONCE,						/*单次定位模式,=0为显式定义*/
	  POSITION_CONTINUE,			/*连续定位模式,只是在第一次运动的时候,有加速运动,之后,等待新的命令,用于插补定位的情况*/
    AT_A_CONSTANT_SPEED,		/*连续转动模式,到了匀速段后,将永远不会进入减速度段*/
} uMotorMode;

/*
定义电机运行方向
*/
typedef enum
{
    FOR = 0,								/*正向,=0为显式定义*/
    REV,									/*反向*/
} uMotorDir;



/*
定义步进电机数据类型
可以理解成,如何用数据的方法抽象表达一个现实中的步进电机,
也就是用数字的方法描述一台电机的属性
*/
typedef struct
{
    uMotorState state;						//电机的状态---加速,减速,匀速,停止
    uMotorMode mode;							//电机的运行模式---速度模式,定位模式.速度模式时,中断函数运行加速,匀速,不进入减速状态
    int32_t stepTotalSet;				//总步数设定
    uint32_t stepAccelerationSet;	//加速段步数设定
    uint32_t stepDecelerationSet;	//减速段步数设定(减速段和加速段公用)
    uint32_t stepConstantSpeedSet;	//匀速段的步数设定---该数值会在匀速运动阶段,自减1

    uint32_t stepCurrent;							//当前步数(仅在一个定位周期有效)
    int32_t stepCurrentABS;			//步进电机的绝对步数,正转计数值+1,反转计数值-1(注意该值有正负)


    uint8_t num_callback;				//用奇偶的方法,控制脉冲的高低的算法,应为一个50%的方波,需要使用2次相同的比较和溢出值
    uMotorDir dir;								//电机运转方向
    float *pFrcTable;							//S型加速频率表格(即每一步的频率)
    uint16_t *pPeriodTable;						//电机运转的S型加速周期表格
    uint16_t pPeriodTableSizeof;				//S型加速表格的长度(即步数)

    TIM_HandleTypeDef htimx;		//电机脉冲所以使用的定时器
    uint32_t TIM_CHANNEL_x;		//脉冲对应的定时器通道TIM_CHANNEL_3

    GPIO_TypeDef *DIR_GPIOx;	//方向引脚的端口组号
    uint16_t DIR_GPIO_Pin;		//方向引脚的序号
} sStepMotor;


/*
定义4个步进电机
*/
extern sStepMotor stepMotor[4];

extern float fre_X[]; //数组存储加速过程中每一步的频率
extern float fre_Y[]; //数组存储加速过程中每一步的频率
extern float fre_Z[]; //数组存储加速过程中每一步的频率
extern float fre_M[]; //数组存储加速过程中每一步的频率

extern uint16_t period_X[]; //数组储存加速过程中每一步定时器的自动装载值
extern uint16_t period_Y[]; //数组储存加速过程中每一步定时器的自动装载值
extern uint16_t period_Z[]; //数组储存加速过程中每一步定时器的自动装载值
extern uint16_t period_M[]; //数组储存加速过程中每一步定时器的自动

//初始化电机相关参数
void initMotor(void);
void init_arm(void);
void configMotorToVelocity(sStepMotor * stepMotor, uMotorDir dir,float rpm);
void configMotorToBreak   (sStepMotor * stepMotor);
void motorStartRun(sStepMotor * stepMotor);
void configMotorToPosition(sStepMotor * stepMotor, int32_t totalStepSet, float  rpm);
void movehome(void);

void steering_gear(TIM_HandleTypeDef htimx, int TIM_CHANNEL, int y);
void CalculateSModelLine(uint16_t *pPeriod, float *pFre, float len, float fre_max, float fre_min, float flexible);

#endif
