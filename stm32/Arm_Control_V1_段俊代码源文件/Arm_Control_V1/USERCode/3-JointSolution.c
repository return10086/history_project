#include "math.h"
#include "arm_math.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "3-JointSolution.h"
#include "motor.h"

ThreeJointsARM _ARM =
    {
        .Length1 = _ARM_LENGHT1_,
        .Length2 = _ARM_LENGTH2_,
        .Length3 = _ARM_Length3_,
        // .psi_Set = Pi / 6,
        // .theta1_Current = 0,
        // .theta2_Current = -Pi / 4,
        // .theta3_Current = 3 / 4 * Pi,

        .theta1_LimitationN = _THETA1_LIMITATIONN_,
        .theta1_LimitationP = _THETA1_LIMITATIONP_,
        .theta2_LimitationN = _THETA2_LIMITATIONN_,
        .theta2_LimitationP = _THETA2_LIMITATIONP_,
        .theta3_LimitationN = _THETA3_LIMITATIONN_,
        .theta3_LimitationP = _THETA3_LIMITATIONP_,
        .Height_LimitationN = _HEIGHT_LIMITATIONN_,
        .Height_LimitationP = _HEIGHT_LIMITATIONP_,
        .J3PositionN = _J3POSTTION_LIMITATIONN_,
        .J3PositionP = _J3POSITION_LIMITATIONP_,
        .IsReachable = 0};

/*初始化机械臂姿态信息*/
void InitializeArmPosture(ThreeJointsARM *_MYARM, float Init_theta1, float Init_theta2, float Init_theta3, float Init_H)
{
    /*关节当前转角*/
    _MYARM->theta1_Current = Init_theta1;
    _MYARM->theta2_Current = Init_theta2;
    _MYARM->theta3_Current = Init_theta3;
    _MYARM->Height_Current = Init_H;

    /*关节转动差量*/
    _MYARM->theta1_Delta = 0;
    _MYARM->theta2_Delta = 0;
    _MYARM->theta3_Delta = 0;
    _MYARM->Height_Delta = 0;

    _MYARM->Steps1 = 0;
    _MYARM->Steps2 = 0;
    _MYARM->Steps3 = 0;
    _MYARM->Steps4 = 0;

    _MYARM->theta1_Target = 0;
    _MYARM->theta2_Target = 0;
    _MYARM->theta3_Target = 0;
    _MYARM->Height_Target = 0;

    initMotor();
    _MYARM->pStepMotor1 = &stepMotor[MOTOR_SLOT_1]; // 杆臂1电机绑定	 旋转
    _MYARM->pStepMotor2 = &stepMotor[MOTOR_SLOT_2]; // 杆臂2电机绑定 大臂
    _MYARM->pStepMotor3 = &stepMotor[MOTOR_SLOT_3]; // 杆臂3电机绑定 小臂
    _MYARM->pStepMotor4 = &stepMotor[MOTOR_SLOT_4]; // 升降电机绑定  升降
}

/*
机械臂复位
*/
void reset_position(void)
{
    // 大臂2小臂3
    configMotorToVelocity(&stepMotor[MOTOR_SLOT_2], REV, 1);
    motorStartRun(_ARM.pStepMotor2);

    configMotorToVelocity(&stepMotor[MOTOR_SLOT_3], FOR, 1);
    motorStartRun(_ARM.pStepMotor3);
}

/*
余弦定理解算
*/
//                          /|
//                         / |
//                        /  |
//                    C  /   |b
//                      /    |
//               (0,0) /_ _ _|
//                      a
void XY_Solution(ThreeJointsARM *_MYARM, float X, float Y)
{
    float c = sqrt(((X * X) + (Y * Y)));
    float a = _MYARM->Length2;
    float b = _MYARM->Length3;
		printf("%f,%f,%f\r\n",a,b,c);
    if (c > (a + b) && c < abs(a - b))
    {
        _MYARM->IsReachable = 0;
        printf("Unreachable Position\n");
    }
    else
    {
        _MYARM->IsReachable = 1;
        float cx_angle = atan(Y/X)/Pi*180;
				printf("cx=%f\r\n",cx_angle);
			
        float ca_angle = acos((a * a + c * c - b * b) /( 2 * a * c))/Pi*180;
				if(Y>0)
					;
				else
					ca_angle=-ca_angle;
				printf("ca=%f\r\n",ca_angle);
			
        // 大臂与x轴夹角
        float ax_angle = cx_angle - ca_angle;
				printf("ax=%f\r\n",ax_angle);
			
        float cb_angle = acos((c * c + b * b - a * a) /( 2 * b * c))/Pi*180;
				if(Y>0)
					;
				else
					cb_angle=-cb_angle;
				printf("cb=%f\r\n",cb_angle);
			
        // 小臂与x轴夹角
        float bx_angle = cb_angle + cx_angle;
				printf("bx=%f\r\n",bx_angle);

        _MYARM->theta2_Delta = ax_angle - _MYARM->theta2_Current;
        _MYARM->theta3_Delta = bx_angle - _MYARM->theta3_Current;
			
				_MYARM->theta2_Target=ax_angle;
				_MYARM->theta3_Target=bx_angle;
			printf("angle2=%f,angle3=%f\r\n",_MYARM->theta2_Delta,_MYARM->theta3_Delta);
    }
}

/*
    几何逆解--迭代算法

*/
void ThreeJointsInverseSolution(ThreeJointsARM *_MYARM, float X, float Y)
{
    float Xj3 = 0, Yj3 = 0, beta = 0, r /*r_LimitationP*/;
    double alpha = 0, c2, s2;
    int16_t delta_psi, Start_Psi;

    if (sqrt(X * X + Y * Y) > _MYARM->Length1 + _MYARM->Length2 + _MYARM->Length3)
    {
        _MYARM->IsReachable = 0;
        printf("Unreachable Position");
        return;
    }

    // 设置起始迭代角度
    if (Y < 0)
    {
        Start_Psi = -180;
    }
    else
    {
        Start_Psi = 0;
    }

    for (delta_psi = Start_Psi; delta_psi <= 180; delta_psi++)
    {
        Xj3 = X - _MYARM->Length3 * cos(delta_psi * 1.0 / 180 * Pi);
        Yj3 = Y - _MYARM->Length3 * sin(delta_psi * 1.0 / 180 * Pi);
        alpha = atan2(Yj3, Xj3);
        r = sqrt(Xj3 * Xj3 + Yj3 * Yj3);

        printf("alpha:%f\n", alpha / 3.14 * 180);
        printf("\n\n");
        printf(" %f<Xj3<%f\n", -cos(_MYARM->theta2_LimitationN) * _MYARM->Length2, cos(_MYARM->theta2_LimitationP) * _MYARM->Length2);
        printf(" %f<Yj3<%f || %f<Yj3<%f\n", sin(_MYARM->theta2_LimitationP) * _MYARM->Length2 + _MYARM->Length1, _MYARM->Length1 + _MYARM->Length2, -(_MYARM->Length1 + _MYARM->Length2), -(sin(_MYARM->theta2_LimitationP) * _MYARM->Length2 + _MYARM->Length1));
        printf(" %f<r<%f\n", _MYARM->J3PositionN, _MYARM->J3PositionP);
        printf("(Xj3,Yj3):(%f,%f) \t r:%f \t NUM:%d:\n", Xj3, Yj3, r, delta_psi);

        if (r > _MYARM->J3PositionP || r < _MYARM->J3PositionN)
        {
            _MYARM->IsReachable = 0;
            printf("Unreachable Position\n");
            continue;
        }
        
                // 获取关节2实际运动最大外径
                // r_LimitationP = _MYARM->J3PositionP < _MYARM->Length1 + _MYARM->Length2 ? _MYARM->J3PositionP : _MYARM->Length1 + _MYARM->Length2;
                if(Yj3>=0) //这里需要判断J3所在正负象限
                {
                    if (Xj3 < -cos(_MYARM->theta2_LimitationN) * _MYARM->Length2
                    || Xj3 > cos(_MYARM->theta2_LimitationP) * _MYARM->Length2
                    || Yj3 < sin(_MYARM->theta2_LimitationP) * _MYARM->Length2 + _MYARM->Length1 //这里因为正向运动范围较大选用正向限位角
                    || Yj3 > _MYARM->Length1 + _MYARM->Length2)
                    {
                        printf("Unreachable Position\n");
                        _MYARM->IsReachable = 0;
                        continue;
                    }
                }

                if(Yj3<0)
                {
                    if(Xj3 < -cos(_MYARM->theta2_LimitationN) * _MYARM->Length2
                    || Xj3 > cos(_MYARM->theta2_LimitationP) * _MYARM->Length2
                    || Yj3 > -(sin(_MYARM->theta2_LimitationP) * _MYARM->Length2 + _MYARM->Length1)//这里因为正向运动范围较大选用正向限位角
                    || Yj3 < -(_MYARM->Length1 + _MYARM->Length2))
                    {
                        printf("Unreachable Position\n");
                        _MYARM->IsReachable = 0;
                        continue;
                    }
                }

                c2 = (r * r - _MYARM->Length1 * _MYARM->Length1 - _MYARM->Length2 * _MYARM->Length2) / (2 * _MYARM->Length1 * _MYARM->Length2);
                s2 = sqrt(1 - c2 * c2); // 这里是正值
                printf("c2:%f\ts2:%f\n", c2/3.14*180, s2/3.14*180);

                _MYARM->theta2_Target = atan2(s2, c2);

                beta = atan2(_MYARM->Length2 * s2 / r, (_MYARM->Length1 + _MYARM->Length2 * c2) / r);
                printf("beta:%f\n", beta/3.14*180);

                _MYARM->theta1_Target = alpha - beta;

                _MYARM->theta3_Target = delta_psi * 1.0 / 180 * Pi - _MYARM->theta1_Target - _MYARM->theta2_Target;
                //_MYARM->theta3_Target = _MYARM->psi_Set - _MYARM->theta1_Target - _MYARM->theta2_Target;

                printf("theta1_T:%f\n", _MYARM->theta1_Target/3.14*180);
                printf("theta2_T:%f\n", _MYARM->theta2_Target/3.14*180);
                printf("theta3_T:%f\n", _MYARM->theta3_Target/3.14*180);

                //运动范围限制
                if (_MYARM->theta1_Target >= _MYARM->theta1_LimitationN && _MYARM->theta1_Target <= _MYARM->theta1_LimitationP)
                {

                    _MYARM->theta1_Delta = _MYARM->theta1_Target - _MYARM->theta1_Current;
                }
                else
                {
                    printf(">>>>>>>theta1_T:%f\n",_MYARM->theta1_Target/3.14*180);
                    printf("%f<theta1<%f\n",_MYARM->theta1_LimitationN/3.14*180,_MYARM->theta1_LimitationP/3.14*180);
                    printf(">>>>>>>theta1 Over Limitation!\n");
                    continue;
                }

                if (_MYARM->theta2_Target >= _MYARM->theta2_LimitationN && _MYARM->theta2_Target <= _MYARM->theta2_LimitationP)
                {
                    _MYARM->theta2_Delta = _MYARM->theta2_Target - _MYARM->theta2_Current;
                }
                else
                {
                    printf(">>>>>>>theta2_T:%f\n",_MYARM->theta2_Target/3.14*180);
                    printf("%f<theta2<%f\n",_MYARM->theta2_LimitationN/3.14*180,_MYARM->theta2_LimitationP/3.14*180);
                    printf(">>>>>>>theta2 Over Limitation!\n");
                    continue;
                }

                if (_MYARM->theta3_Target >= _MYARM->theta3_LimitationN && _MYARM->theta3_Target <= _MYARM->theta3_LimitationP)
                {
                    _MYARM->theta3_Delta = _MYARM->theta3_Target - _MYARM->theta3_Current;
                }
                else
                {
                    printf(">>>>>>>theta3_T:%f\n",_MYARM->theta3_Target/3.14*180);
                    printf("%f<theta3<%f\n",_MYARM->theta3_LimitationN/3.14*180,_MYARM->theta3_LimitationP/3.14*180);
                    printf(">>>>>>>theta3 Over Limitation!\n");
                    continue;
                }

                //如果满足上述所有条件则退出循环
                _MYARM->IsReachable = 1;
                _MYARM->psi_Set = delta_psi*1.0/Pi;

                printf("\n>>>>>>>Find Inverse Solution !<<<<<<<<< \tNUM:%d\n", delta_psi);
                printf("\n\n\n");

                break;
                        
    }
    // 两个关节转动差量
    _MYARM->Steps2 = alpha - _MYARM->theta2_Current;
    _MYARM->Steps3 = delta_psi - _MYARM->theta3_Current;

    // Xj3 = X - _MYARM->Length3 * cos(_MYARM->psi_Set);
    // Yj3= Y - _MYARM->Length3 * sin(_MYARM->psi_Set);
    // r = sqrt(Xj3 * Xj3 + Yj3* Yj2);

    // printf("(Xj3,Yj2) (%f,%f)\tr:%f\n", Xj3, Yj2, r);

    // if (r < _MYARM->PositionN || r > _MYARM->PositionP)
    // {
    //     printf("Unreachable Position\n");
    //     _MYARM->IsReachable = 0;
    //     return;
    // }
    // else
    // {
    //     _MYARM->IsReachable = 1;
    // }
}

/*
    几何解算--实际解算时，先调用此语句对输入数据进行解算。使用此语句是机械臂不移动,这里是对机械臂的缓存区操作
*/

void OperatePostureData(ThreeJointsARM *_MYARM, float X, float Y, float Z)
{

    // if (X == _MYARM->Last_X && Y == _MYARM->Last_Y && Z == _MYARM->Last_Z)
    // {
    //     printf(">>>>>>>>No need to Move.<<<<<<<<\n");
    //     return;
    // }

    XY_Solution(_MYARM, X, Y);
    // // 不需要移动
    // if (_MYARM->theta1_Target == _MYARM->theta1_Current && _MYARM->theta2_Target == _MYARM->theta2_Current && _MYARM->theta3_Target == _MYARM->theta3_Current && _MYARM->Height_Current == _MYARM->Height_Target)
    // {
    //     printf(">>>>>>>>No need to Move.<<<<<<<<\n");
    //     return;
    // }

    if (_MYARM->IsReachable == 1)
    {
        _MYARM->Height_Target = Z;
        if (_MYARM->Height_Target >= _MYARM->Height_LimitationN && _MYARM->Height_Target <= _MYARM->Height_LimitationP)
        {
            _MYARM->Height_Delta = _MYARM->Height_Target - _MYARM->Height_Current;
        }
        else
        {
            _MYARM->Height_Delta = 0;
        }

        _MYARM->Steps1 = (int32_t)(_MYARM->theta1_Delta  * _Joint1TransmissionRatio_); // 弧度换算，考虑减速比
        _MYARM->Steps2 = (int32_t)(_MYARM->theta2_Delta  * _Joint2TransmissionRatio_); // 弧度换算，考虑减速比
        _MYARM->Steps3 = (int32_t)(_MYARM->theta3_Delta  * _Joint3TransmissionRatio_); // 弧度换算，考虑减速比
        _MYARM->Steps4 = (int32_t)(_MYARM->Height_Delta * _Joint4TransmissionRatio_);            // 同步带齿距
				
//				_MYARM->pStepMotor1->stepTotalSet=_MYARM->Steps1;
//				_MYARM->pStepMotor2->stepTotalSet=_MYARM->Steps2;
//				_MYARM->pStepMotor3->stepTotalSet=_MYARM->Steps3;
//				_MYARM->pStepMotor4->stepTotalSet=_MYARM->Steps4;
				
        printf("\nStep1:%d\n", _MYARM->Steps1);
        printf("\nStep2:%d\n", _MYARM->Steps2);
        printf("\nStep3:%d\n", _MYARM->Steps3);
        printf("\nStep4:%d\n\n\n", _MYARM->Steps4);

        printf("theta1_C:\t%f D\n", _MYARM->theta1_Current );
        printf("theta1_T:\t%f D\n", _MYARM->theta1_Target );
        printf("theta1_D:\t%f D\n\n", _MYARM->theta1_Delta );

        printf("theta2_C:\t%f D\n", _MYARM->theta2_Current );
        printf("theta2_T:\t%f D\n", _MYARM->theta2_Target );
        printf("theta2_D:\t%f D\n\n", _MYARM->theta2_Delta );

        printf("theta3_C:\t%f D\n", _MYARM->theta3_Current );
        printf("theta3_T:\t%f D\n", _MYARM->theta3_Target );
        printf("theta3_D:\t%f D\n\n", _MYARM->theta3_Delta );
    }
    else
    {
        return;
    }

    _MYARM->Last_X = X;
    _MYARM->Last_Y = Y;
    _MYARM->Last_Z = Z;
}

/*>>>>>>>调用此词语移动机械臂，并更新机械臂姿态信息<<<<<<<<*/
void UpdateArmPosture(ThreeJointsARM *_MYARM)
{

    //    if (_MYARM->theta1_Target == _MYARM->theta1_Current && _MYARM->theta2_Target == _MYARM->theta2_Current && _MYARM->theta3_Target == _MYARM->theta3_Current)
    //    {
    //        printf(">>>>>>>>No need to Move.<<<<<<<<\n\n\n\n");
    //        return;
    //    }

    if (_MYARM->IsReachable == 1)
    {
				printf("move\r\n");
        configMotorToPosition(_MYARM->pStepMotor1, _MYARM->Steps1, 1.5);
        configMotorToPosition(_MYARM->pStepMotor2, _MYARM->Steps2, 1.5);
				printf("大臂加速%d步\r\n",_MYARM->pStepMotor2->stepAccelerationSet);
			
        configMotorToPosition(_MYARM->pStepMotor3, _MYARM->Steps3, 1.0);
				printf("小臂加速%d步\r\n",_MYARM->pStepMotor3->stepAccelerationSet);
			
        configMotorToPosition(_MYARM->pStepMotor4, _MYARM->Steps4, 0.9);

        motorStartRun(_MYARM->pStepMotor1);
        motorStartRun(_MYARM->pStepMotor2);
        motorStartRun(_MYARM->pStepMotor3);
        motorStartRun(_MYARM->pStepMotor4);

        /*更新状态*/
        _MYARM->theta1_Current = _MYARM->theta1_Target;
        _MYARM->theta2_Current = _MYARM->theta2_Target;
        _MYARM->theta3_Current = _MYARM->theta3_Target;
        _MYARM->Height_Current = _MYARM->Height_Target;

        /*清空差量*/
        _MYARM->theta1_Delta = 0;
        _MYARM->theta2_Delta = 0;
        _MYARM->theta3_Delta = 0;
        _MYARM->Height_Delta = 0;

        _MYARM->Steps1 = 0;
        _MYARM->Steps2 = 0;
        _MYARM->Steps3 = 0;
        _MYARM->Steps4 = 0;

        // 清空标志位
        _MYARM->IsReachable = 0;
    }
}

/***********************************************************************************************************/
/***********************************************************************************************************/

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>               代数法             <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/

/************************************************************************************************************/
/************************************************************************************************************/

/*初始化变换矩阵*/
void InitializeDFTM(DFT_MATRIX *pM, Matirx_Sort Sort)
{
    // for(uint8_t cnt=0;cnt<16;cnt++)
    // {
    //     pM->Dft_One[cnt]=0;
    // }

    pM->M_Sort = Sort;

    memset(pM->Dft_One, 0, sizeof(pM->Dft_One));

    pM->Matrix_ins.numRows = 4;         // 设置矩阵行数
    pM->Matrix_ins.numCols = 4;         // 设置矩阵列数
    pM->Matrix_ins.pData = pM->Dft_One; // 传递矩阵缓存区地址

    for (uint8_t row = 0; row < 4; row++) // 生成单位矩阵
    {
        pM->Dft_One[row * 4 + row] = 1;
    }
}

/*生成变化矩阵(单变量)*/
void GenerateSingalDFTM(DFT_MATRIX *pM, float data)
{

    /*

    相关旋转产生的变换矩阵

Rotate_X    |   1       0       0     0   |
            |   0     cosα    sinα    0   |
            |   0     -sinα   cosα    0   |
            |   0       0       0     1   |

Rotate_Y    |   cosα    0      -sinα  0   |
            |   0       1       0     0   |
            |   sinα    0       cosα  0   |
            |   0       0       0     1   |

Rotate_Z    |   cosα    sinα    0     0   |
            |   -sinα   cosα    0     0   |
            |   0       0       1     0   |
            |   0       0       0     1   |


    */

    switch (pM->M_Sort)
    {
    case R_X:
        pM->Dft_One[5] = ((float)cos(data));
        pM->Dft_One[6] = ((float)sin(data));
        pM->Dft_One[9] = ((float)-sin(data));
        pM->Dft_One[10] = ((float)cos(data));
        break;

    case R_Y:
        pM->Dft_One[0] = ((float)cos(data));
        pM->Dft_One[2] = ((float)-sin(data));
        pM->Dft_One[8] = ((float)sin(data));
        pM->Dft_One[10] = ((float)cos(data));
        break;

    case R_Z:
        pM->Dft_One[0] = ((float)cos(data));
        pM->Dft_One[1] = ((float)sin(data));
        pM->Dft_One[4] = ((float)-sin(data));
        pM->Dft_One[5] = ((float)cos(data));
        break;

    case P_X:
        pM->Dft_One[3] = data;
        break;

    case P_Y:
        pM->Dft_One[7] = data;
        break;

    case P_Z:
        pM->Dft_One[11] = data;
        break;

    default:
        break;
    }
}
/*
    计算单个关节变换矩阵
*/
void GenerateJointTransFormMatrix(JOINT_MATRIX *pJoint)
{
    /*获取关节变换矩阵（包括移动和转动）*/

    /*
        这一步是单个关节的四个变化矩阵相乘（α-*->a-*->d-*->theta）结果存入关节变换矩阵缓存区中
         注意!>>>>>>>：这里在计算时需要先将单个机械臂关节最终的变化矩阵单位化
    */
    arm_mat_mult_f32(&pJoint->TFM_Mul[0].Matrix_ins, &pJoint->TFM_Mul[1].Matrix_ins, &pJoint->JTFM.Matrix_ins);
    arm_mat_mult_f32(&pJoint->JTFM.Matrix_ins, &pJoint->TFM_Mul[2].Matrix_ins, &pJoint->JTFM.Matrix_ins);
    arm_mat_mult_f32(&pJoint->JTFM.Matrix_ins, &pJoint->TFM_Mul[3].Matrix_ins, &pJoint->JTFM.Matrix_ins);
    /***********************************************************************************************************/
}

/*
    计算机械臂变换矩阵
*/
void GenerateARMTransFormMatrix(DH_TABLE_JOINTS *pARM)
{
    /*存储上一次机械臂最终转换矩阵*/
    memcpy(pARM->LastTM_Ins.pData, pARM->FinalTM_Ins.pData, pARM->FinalTM_Ins.numCols * pARM->FinalTM_Ins.numRows);

    /*
        将每个关节的变换矩阵相乘，结果存入机械臂最终的变换矩阵缓存区中
        注意!>>>>>>>：这里在计算时需要先将机械臂最终的变化矩阵单位化
    */

    memset(pARM->FinalTM_Ins.pData, 0, pARM->FinalTM_Ins.numCols * pARM->FinalTM_Ins.numRows);
    for (uint8_t row = 0; row < 4; row++)
    {
        pARM->FinalTM_Ins.pData[row * 4 + row] = 1; // 单位化
    }

    for (uint8_t num = 0; num < _DOF_NUM_; num++)
    {
        arm_mat_mult_f32(&pARM->FinalTM_Ins, &pARM->ARM_JTFMS[num].JTFM.Matrix_ins, &pARM->FinalTM_Ins); // 这里注意矩阵的先后顺序（右乘） FinalMatrix = [1]*[J1]*[J2]*[J3]*[J4]*.....[Jn]
    }
    arm_mat_mult_f32(&pARM->FinalTM_Ins, &pARM->EndDeltaPositon_Ins, &pARM->FinalTM_Ins); // 乘上末端关节与末端执行器之间的变换矩阵
}

/*
    初始化机械臂变换矩阵--使用DH-TABLE矩阵
*/
void InitializeTransformMatrixBy_DH_TABLE(DH_TABLE_JOINTS *pARM, float *DH_TABLE, Joint_Sort *JointFun)
{
    memcpy(pARM->DH_Matrix, DH_TABLE, sizeof(pARM->DH_Matrix)); // 存入DH-TABLE信息

    for (uint8_t row; row < _DOF_NUM_; row++)
    {
        pARM->ARM_Type[row] = JointFun[row];
    }

    /*绑定DH－Table与Matrix实体*/
    pARM->DH_Matrix_Ins.numRows = 4;
    pARM->DH_Matrix_Ins.numCols = 4;
    pARM->DH_Matrix_Ins.pData = pARM->DH_Matrix;

    /*相关矩阵初始化*/
    pARM->DH_MAtrix_T_Ins.numRows = 4;
    pARM->DH_MAtrix_T_Ins.numCols = 4;
    pARM->DH_MAtrix_T_Ins.pData = pARM->DH_Matrix_T;

    pARM->FinalTM_Ins.numRows = 4;
    pARM->FinalTM_Ins.numCols = 4;
    pARM->FinalTM_Ins.pData = pARM->FianlTransFormMatrix;

    pARM->FinalTM_LimitationN_Ins.numRows = 4;
    pARM->FinalTM_LimitationN_Ins.numCols = 4;
    pARM->FinalTM_LimitationN_Ins.pData = pARM->FinalTM_LimitN;

    pARM->FinalTM_LimitationP_Ins.numRows = 4;
    pARM->FinalTM_LimitationP_Ins.numCols = 4;
    pARM->FinalTM_LimitationP_Ins.pData = pARM->FinalTM_LimitP;

    pARM->LastTM_Ins.numRows = 4;
    pARM->LastTM_Ins.numCols = 4;
    pARM->LastTM_Ins.pData = pARM->LastTransFormMatrix;

    /*设计末端关节与末端执行器之间的变换矩阵*/
    pARM->EndDeltaPositon_Ins.numRows = 4;
    pARM->EndDeltaPositon_Ins.numCols = 4;
    pARM->EndDeltaPositon_Ins.pData = pARM->EndEffortorMatrix;

    /*设置末端关节与末端执行器间距离*/
    memset(pARM->EndDeltaPositon_Ins.pData, 0, sizeof(pARM->EndDeltaPositon_Ins.pData));
    for (uint8_t row = 0; row < 4; row++) // 生成单位矩阵
    {
        pARM->EndDeltaPositon_Ins.pData[row * 4 + row] = 1;
    }
    pARM->EndEffortorMatrix[4 * 1 - 1] = _FINAL_DELTA_X;

    // for(uint8_t cnt=0;cnt<16;cnt++)
    // {
    //     pARM->FianlDeltaPositon.pData[cnt]=0;
    // }

    /*初始化一次机械臂最终的变换矩阵*/
    memset(pARM->FinalTM_Ins.pData, 0, pARM->FinalTM_Ins.numCols * pARM->FinalTM_Ins.numRows);
    for (uint8_t row = 0; row < 4; row++)
    {
        pARM->FinalTM_Ins.pData[row * 4 + row] = 1; // 单位化
    }

    /*初始化各个关节变换矩阵*/
    for (uint8_t row = 0; row < _DOF_NUM_; row++)
    {
        pARM->alpha[row] = DH_TABLE[row * 4];
        pARM->a[row] = DH_TABLE[row * 4 + 1];
        pARM->d[row] = DH_TABLE[row * 4 + 2];
        pARM->theta[row] = DH_TABLE[row * 4 + 3];

        /*设置关节功能：转动/移动*/
        pARM->ARM_JTFMS[row].Joint_Fun = JointFun[row];

        /*绑定DH-TABLE参数与变换矩阵关系*/
        InitializeDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[0], R_X);
        InitializeDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[1], P_X);
        InitializeDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[2], P_Z);
        InitializeDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[3], R_Z);

        /*生成单变量变换矩阵*/
        GenerateSingalDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[0], /*DH:alpha*/ pARM->alpha[row]); // alpha 对应R_X
        GenerateSingalDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[1], /*DH:a*/ pARM->a[row]);         // a对应P_X
        GenerateSingalDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[2], /*DH:d*/ pARM->d[row]);         // d对应P_Z
        GenerateSingalDFTM(&pARM->ARM_JTFMS[row].TFM_Mul[3], /*DH:theta*/ pARM->theta[row]); // theta对应R_Z

        /*生成关节变换矩阵*/
        GenerateJointTransFormMatrix(&pARM->ARM_JTFMS[row]);
    }
    /*计算机械臂最终变换矩阵*/
    GenerateARMTransFormMatrix(pARM);
}

/*
    更新关节变换矩阵
*/
void UpdataJointTransFormMatrix(JOINT_MATRIX *pJoint, float Data)
{
    /*
        ————————————————————————————————————————————————————————
        |          |          |          |          |          |
        |    i     |    α     |     a    |     d    |  theta   |
        ————————————————————————————————————————————————————————
        |          |          |          |          |          |
        |    1     |          |          |          |          |
        ________________________________________________________
        |          |          |          |          |          |
        |    2     |          |          |          |          |
        ————————————————————————————————————————————————————————
        |          |          |          |          |          |
        |    3     |          |          |          |          |
        ————————————————————————————————————————————————————————
        |          |          |          |          |          |
        |    4     |          |          |          |          |
        ————————————————————————————————————————————————————————
        |                                                      |
        |                                                      |
        、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
    */

    /*更新DH-TABLE参数*/
    switch (pJoint->Joint_Fun)
    {
    case Position:
        GenerateSingalDFTM(&pJoint->TFM_Mul[2], Data); // 更新单关节第三个变换矩阵

        break;

    case Rotation:
        GenerateSingalDFTM(&pJoint->TFM_Mul[3], Data); // 更新单关节第四个变换矩阵

        break;

    default:
        break;
    }

    /*更新机械臂变换矩阵*/
    GenerateJointTransFormMatrix(pJoint);
    // arm_mat_trans_f32(pTable->DH_Matrix,pTable->DH_Matrix_T);  //获取DH_TABLE 转置矩阵（备用）
}

/*移动单关节*/
void MoveOneJointPos(DH_TABLE_JOINTS *pARM, JOINT_MATRIX *pJoint, float TargetPos)
{
    if (TargetPos < pJoint->JointPos.Joint_LimitationN || TargetPos > pJoint->JointPos.Joint_LimitationP)
    {
        IndicateError("Joint movement over Limitaton");
        return;
    }

    pJoint->JointPos.JointLastPos = pJoint->JointPos.JointCurrentPos; // 存入上次姿态信息

    UpdataJointTransFormMatrix(pJoint, TargetPos); // 更新变换矩阵
    GenerateARMTransFormMatrix(pARM);              // 更新机械臂变换矩阵

    pJoint->JointPos.JointDeltaPos = TargetPos - pJoint->JointPos.JointCurrentPos;
    pJoint->Motor.MotorSteps = (int32_t)(pJoint->JointPos.JointDeltaPos * pJoint->Motor.TransPro);

    configMotorToPosition(pJoint->Motor.pStepMotor, pJoint->Motor.MotorSteps, pJoint->Motor.MAX_Speed);
    motorStartRun(pJoint->Motor.pStepMotor);

    pJoint->JointPos.JointCurrentPos = TargetPos; // 更新姿态信息
}

/*
    最对位置解算
    @param:(X,Y,Z)目标点绝对坐标，R_X,R_Y,R_Z目标点绝对姿态
*/
void InverseSolution_DH_AbsolutePos(DH_TABLE_JOINTS *pARM, float X, float Y, float Z, float R_X, float R_Y, float R_Z)
{
    switch (_DOF_NUM_)
    {
    case 1:

        break;

    case 2:
        break;

    case 3:
        break;

    case 4:
        if (pARM->ARM_Type[0] == Rotation && pARM->ARM_Type[1] == Position && pARM->ARM_Type[2] == Rotation && pARM->ARM_Type[3] == Rotation)
        {
        }
        break;

    case 5:
        break;

    case 6:
        break;

    default:
        break;
    }
}

/*获取某关节状态信息*/
void GetJointStateInfo(JOINT_MATRIX *pJoint, float *MessageBuffer)
{
    memcpy(MessageBuffer, pJoint->JTFM.Dft_One, sizeof(pJoint->JTFM.Dft_One));
}

void IndicateError(char *ErrorType)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>An error has occured!,---->Error type:%s\n\n", ErrorType);
}
