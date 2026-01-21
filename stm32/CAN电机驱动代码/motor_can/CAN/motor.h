#include "main.h"
#include "stm32f1xx_hal_can.h"
typedef struct 
{
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8];
    uint32_t TxMailBox;
    
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];
    uint32_t RxMailBox; 
		uint8_t angleH;
		uint8_t angleL;
		uint8_t speedH;
		uint8_t speedL;
		uint8_t currentH;
		uint8_t currentL;
		uint8_t Tem;
}_CANMESSAGE;



void CAN1_Transmit(signed short int i1, signed short int i2, signed short int i3, signed short int i4);

void can_filter_init(void);

