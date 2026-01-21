#include "motor.h"
#include "usart.h"
#include "stdio.h"
extern CAN_HandleTypeDef hcan;


_CANMESSAGE CAN_Message;
void CAN1_Transmit(signed short int i1, signed short int i2, signed short int i3, signed short int i4)
{

    CAN_Message.TxHeader.DLC = 0x08; // 0x02对应一个电机  0x08 对应4个电机
    CAN_Message.TxHeader.RTR = CAN_RTR_DATA;//数据帧和远程帧的标志，CAN_RTR_REMOTE为远程帧
    CAN_Message.TxHeader.IDE = CAN_ID_STD;//IDE位为显性，表示数据帧为标准格式；IDE位为隐性，表示数据帧为扩展帧格式
    CAN_Message.TxHeader.StdId = 0x200;//ID
    

		CAN_Message.TxData[0] = i1 >> 8;
	 CAN_Message.TxData[1] = i1;
	 CAN_Message.TxData[2] = i2 >> 8;
	 CAN_Message.TxData[3] = i2;
	 CAN_Message.TxData[4] = i3 >> 8;
	 CAN_Message.TxData[5] = i3;
	 CAN_Message.TxData[6] = i4 >> 8;
	 CAN_Message.TxData[7] = i4;
		
    HAL_CAN_AddTxMessage(&hcan,&CAN_Message.TxHeader, CAN_Message.TxData, &CAN_Message.TxMailBox);
}


void can_filter_init(void)
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 1;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan, &can_filter_st);
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

};


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)

{
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_Message.RxHeader, CAN_Message.RxData);
	CAN_Message.angleH=CAN_Message.RxData[0];
	CAN_Message.angleL=CAN_Message.RxData[1];
	CAN_Message.speedH=CAN_Message.RxData[2];
	CAN_Message.speedL=CAN_Message.RxData[3];
	CAN_Message.currentH=CAN_Message.RxData[4];
	CAN_Message.currentL=CAN_Message.RxData[5];
	CAN_Message.angleH=CAN_Message.RxData[6];
	printf("%d",CAN_Message.angleH|CAN_Message.angleL);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_Stop(hcan);
}
