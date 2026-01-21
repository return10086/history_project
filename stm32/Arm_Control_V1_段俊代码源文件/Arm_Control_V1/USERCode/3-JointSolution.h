#include "main.h"
#include "motor.h"
#include "arm_math.h"
#ifndef Pi
#define Pi 3.1415926
#endif

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>几何法相关参数<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#define _ARM_LENGHT1_ (80.0f)                                         // 连杆1长度
#define _ARM_LENGTH2_ (100.0f)                                        // 连杆2长度
#define _ARM_Length3_ (100.0f)                                        // 连杆3长度
#define _THETA1_LIMITATIONP_ ((float)(Pi))                            // 关节1正向运动限位
#define _THETA1_LIMITATIONN_ ((float)(-Pi))                           // 关节2逆向运动限位
#define _THETA2_LIMITATIONP_ ((float)(25 * 1.0 / 180 * Pi))           // 关节2正向运动限位
#define _THETA2_LIMITATIONN_ ((float)(-Pi / 4))                       // 关节2逆向运动限位
#define _THETA3_LIMITATIONP_ ((float)(3 * 1.0 / 4 * Pi))              // 关节3正向运动限位
#define _THETA3_LIMITATIONN_ ((float)(-3 * 1.0 / 4 * Pi))             // 关节3逆向运动限位
#define _HEIGHT_LIMITATIONP_ (280.0f)                                 // Z轴上升运动限位
#define _HEIGHT_LIMITATIONN_ (0.0f)                                   // Z轴下降运动限位
#define _J3POSITION_LIMITATIONP_ (180.0f)                             // 关节3坐标逆解（Xj3,Yj3）空间限制
#define _J3POSTTION_LIMITATIONN_ (135.0f)                             // 关节3坐标逆解（Xj3,Yj3）空间限制
																											//1.8度 细分 减速器 齿轮
#define _Joint1TransmissionRatio_ ((float)(1.0 / 1.8 * 16 * 6))       // 关节1传动比
#define _Joint2TransmissionRatio_ ((float)(1.0 / 1.8 * 8 * 16 *21 / 10)) // 关节2传动比
#define _Joint3TransmissionRatio_ ((float)(1.0 / 1.8 * 8 * 5* 9 / 4))   // 关节3传动比
	#define _Joint4TransmissionRatio_ ((float)(1.0 / 1.8 * 8 * 4))            // 关节4传动比mm为单位
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>代数法相关参数<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#define _DOF_NUM_ (4u)        // 机械臂自由度（Degree of Freedom）
#define _FINAL_DELTA_X (100u) // 末端执行器中心点到末端关节的距离
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>机械臂姿态相关数据（几何）<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
typedef struct
{
  /*关节目标*/
  double theta1_Target;
  double theta2_Target;
  double theta3_Target;

  /*末端杆臂水平夹角*/
  float psi_Set;

  /*关节当前转角*/
  double theta1_Current;
  double theta2_Current;
  double theta3_Current;

  /*关节转动差量*/
  float theta1_Delta;
  float theta2_Delta;
  float theta3_Delta;

  /*关节移动权重比*/
  float theta1_Proportion;
  float theta2_Proportion;
  float theta3_Proportion;

  /*关节运动限位*/
  float theta1_LimitationP;
  float theta1_LimitationN;
  float theta2_LimitationP;
  float theta2_LimitationN;
  float theta3_LimitationP;
  float theta3_LimitationN;

  /*杆臂长度*/
  float Length1;
  float Length2;
  float Length3;

  /*机械臂Z轴位置信息*/
  float Height_Target;
  float Height_Current;
  float Height_Delta;
  float Height_LimitationN;
  float Height_LimitationP;

  /*步进电机移动步数*/
  int32_t Steps1;
  int32_t Steps2;
  int32_t Steps3;
  int32_t Steps4;

  /*步进电机绑定电机*/
  sStepMotor *pStepMotor1; // 指向转盘电机的数据
  sStepMotor *pStepMotor2; // 指向大臂电机的数据
  sStepMotor *pStepMotor3; // 指向小臂电机的数据
  sStepMotor *pStepMotor4; // 指向升降电机的数据

  /*运动限位*/

  float J3PositionN; // 关节3最大运动外径
  float J3PositionP; // 关节3最小运动内径

  /*记录机械臂上次的坐标信息*/
  float Last_X;
  float Last_Y;
  float Last_Z;

  /*运动限位*/
  uint8_t IsReachable;
} ThreeJointsARM;
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>机械臂姿态相关数据（代数）<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*变换矩阵种类*/
typedef enum
{
  R_X = 0xA0, // 绕X轴旋转矩阵
  R_Y,        // 绕y轴旋转矩阵
  R_Z,        // 绕z轴旋转矩阵
  P_X,        // 绕X轴移动矩阵
  P_Y,        // 绕y轴移动矩阵
  P_Z,        // 绕z轴移动矩阵
  T_M         // 坐标系变换矩阵
} Matirx_Sort;

typedef enum
{
  Rotation = 0xFF,
  Position = 0xFE

} Joint_Sort;

/*单个变换矩阵（旋转或者移动）*/
typedef struct
{
  float Dft_One[4 * 4]; // 单个参数变化产生的变化矩阵
  arm_matrix_instance_f32 Matrix_ins;
  Matirx_Sort M_Sort;
} DFT_MATRIX;

/*机械臂关节姿态信息*/
typedef struct
{
  float JointLastPos;
  float JointDeltaPos;
  float JointTargetPos;
  float JointCurrentPos;
  float Joint_LimitationN;
  float Joint_LimitationP;

} ARM_PosInfo;

/*关节电机参数*/
typedef struct
{
  int32_t MotorSteps;     // 步进电机移动步数
  sStepMotor *pStepMotor; // 步进电机硬件参数
  float TransPro;         // 相关传动比
  float MAX_Speed;        // 电机最大速度
} Joint_Motor;

/*单个关节矩阵：包括旋转和移动两部分*/
typedef struct
{
  DFT_MATRIX TFM_Mul[4]; // 单个关节四个变量α a d theta的变化矩阵组
  DFT_MATRIX JTFM;       // 关节（坐标系）变化矩阵
  Joint_Sort Joint_Fun;  // 关节功能，移动或是转动。

  ARM_PosInfo JointPos; // 关节姿态信息
  Joint_Motor Motor;    // 关节电机参数
} JOINT_MATRIX;

/*DH-TABLE*/
typedef struct
{
  float alpha[_DOF_NUM_];
  float a[_DOF_NUM_];
  float d[_DOF_NUM_];
  float theta[_DOF_NUM_];

  float DH_Matrix[_DOF_NUM_ * 4];   // DH_TABLE抽象二维矩阵
  float DH_Matrix_T[4 * _DOF_NUM_]; // DH_TABLE抽象二维转置矩阵
  arm_matrix_instance_f32 DH_Matrix_Ins;
  arm_matrix_instance_f32 DH_MAtrix_T_Ins;

  JOINT_MATRIX ARM_JTFMS[_DOF_NUM_]; // 每个机械臂关节的变化矩阵

  float EndEffortorMatrix[16];                 // 目标点相对末端关节变换矩阵
  arm_matrix_instance_f32 EndDeltaPositon_Ins; // 目标点相对末端关节变换矩阵_Ins

  arm_matrix_instance_f32 FinalTM_Ins;             // 机械臂最终的变换矩阵（这里最终的意思是指将所有的关节变换矩阵按顺序相乘得出的最终变换矩阵，这个矩阵确定了末端关节的位姿<Position-Rotaion>）
  arm_matrix_instance_f32 LastTM_Ins;              // 上一次变换最终矩阵
  arm_matrix_instance_f32 FinalTM_LimitationN_Ins; // 最终变换矩阵限制（最小值）
  arm_matrix_instance_f32 FinalTM_LimitationP_Ins; // 最终变换矩阵限制（最大值）

  float FianlTransFormMatrix[16]; // 最终变换矩阵缓存数组
  float LastTransFormMatrix[16];  // 上一次最终变换矩阵缓存数组
  float FinalTM_LimitN[16];       // 最小可达点限制矩阵
  float FinalTM_LimitP[16];       // 最大可达点限制矩阵

  Joint_Sort ARM_Type[_DOF_NUM_]; // 机械臂类型
} DH_TABLE_JOINTS;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

void InitializeArmPosture(ThreeJointsARM *_MYARM, float Init_theta1, float Init_theta2, float Init_theta3, float Init_H);
void ThreeJointsInverseSolution(ThreeJointsARM *_MYARM, float X, float Y);
void OperatePostureData(ThreeJointsARM *_MYARM, float X, float Y, float Z);
void UpdateArmPosture(ThreeJointsARM *_MYARM);
void IndicateError(char *ErrorType);
void GetJointStateInfo(JOINT_MATRIX *pJoint, float *MessageBuffer);
void reset_position(void);
void XY_Solution(ThreeJointsARM *_MYARM, float X, float Y);

