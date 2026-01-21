#include "ANO.h"
#include "usart.h"

extern uint8_t TestBuffer[100];
uint8_t DataToSend[100];

void SendString(uint8_t *Data, uint8_t len);

void ANO_SendF1(uint16_t m, uint16_t n, uint16_t p, uint16_t q)
{
    uint8_t cnt = 0;

    DataToSend[cnt++] = 0xAA;
    DataToSend[cnt++] = 0xFF;
    DataToSend[cnt++] = 0xF1;
    DataToSend[cnt++] = 8;

    DataToSend[cnt++] = BYTE0(m);
    DataToSend[cnt++] = BYTE1(m);

    DataToSend[cnt++] = BYTE0(n);
    DataToSend[cnt++] = BYTE1(n);

    DataToSend[cnt++] = BYTE0(p);
    DataToSend[cnt++] = BYTE1(p);

    DataToSend[cnt++] = BYTE0(q);
    DataToSend[cnt++] = BYTE1(q);

    uint8_t sc = 0;
    uint8_t ac = 0;

    for (uint8_t i = 0; i < DataToSend[3] + 4; i++)
    {
        sc += DataToSend[i];
        ac += sc;
    }
    DataToSend[cnt++] = sc;
    DataToSend[cnt++] = ac;

    // SendString(DataToSend,cnt);
    HAL_UART_Transmit(&huart2, (uint8_t *)&DataToSend, cnt, 0xFFFF);
}

void ANO_SendCheck(uint8_t ID_GET, uint8_t SC_GET, uint8_t AC_GET)
{
    uint8_t cnt = 0;
    DataToSend[cnt++] = 0xAA;
    DataToSend[cnt++] = 0xFF;
    DataToSend[cnt++] = 0x00;
    DataToSend[cnt++] = 0x03;
    DataToSend[cnt++] = ID_GET;
    DataToSend[cnt++] = SC_GET;
    DataToSend[cnt++] = AC_GET;

    uint8_t sc = 0;
    uint8_t ac = 0;

    for (uint8_t i = 0; i < DataToSend[3] + 4; i++)
    {
        sc += DataToSend[i];
        ac += sc;
    }
    DataToSend[cnt++] = sc;
    DataToSend[cnt++] = ac;

    HAL_UART_Transmit(&huart2, DataToSend, cnt, 0xFFFF);
}

int32_t DATA_GET = 0;
uint16_t ID_GET;

int32_t value1 = 0, value2 = 0, value3 = 0, value4 = 0, value5= 0;  //根据需要自行删减

void ANO_PC_COMMAND(uint8_t *pBuffer)
{

    if (pBuffer[2] == 0xE1) // 读取命令
    {
        // ANO_SendCheck(0xE1,pBuffer[10],pBuffer[11]);  //返回验证帧

        ID_GET = pBuffer[5] << 8 | pBuffer[4];
        switch (ID_GET)
        {
        case 10:
            /*Code*/
            ANO_SendPC(10, value1);
            break;

        case 11:
            /*Code*/
            ANO_SendPC(11, value2);
            break;

        case 12:
            /*Code*/
            ANO_SendPC(12, value3);
            break;

        case 13:
            /*Code*/
            ANO_SendPC(13, value4);
            break;

        case 14:
            /*Code*/
            ANO_SendPC(14, value5);
            break;
        }
    }
    else if (pBuffer[2] == 0xE2) // 写入命令
    {

				ANO_SendCheck(0xE2, pBuffer[10], pBuffer[11]); // 返回验证帧
			
        ID_GET = pBuffer[5] << 8 | pBuffer[4];
        DATA_GET = pBuffer[9] << 24 | pBuffer[8] << 16 | pBuffer[7] << 8 | pBuffer[6];  

        switch (ID_GET)
        {
        case 10:
            /*Code*/
            value1 = DATA_GET;

            break;

        case 11:
            /*Code*/
            value2 = DATA_GET;

            break;

        case 12:
            /*Code*/
            value3 = DATA_GET;

            break;

        case 13:
            /*Code*/
            value4 = DATA_GET;

            break;

        case 14:
            /*Code*/
            value5 = DATA_GET;

            break;
        }
    }
}

void ANO_SendPC(uint16_t ID_SEND, int32_t DATA_SEND)
{
    uint8_t cnt = 0;
    DataToSend[cnt++] = 0xAA;
    DataToSend[cnt++] = 0xAF;
    DataToSend[cnt++] = 0xE2;
    DataToSend[cnt++] = 0x06;

    // ID_SEND
    DataToSend[cnt++] = BYTE0(ID_SEND);
    DataToSend[cnt++] = BYTE1(ID_SEND);
    // DATA_SEND
    DataToSend[cnt++] = BYTE0(DATA_SEND);
    DataToSend[cnt++] = BYTE1(DATA_SEND);
    DataToSend[cnt++] = BYTE2(DATA_SEND);
    DataToSend[cnt++] = BYTE3(DATA_SEND);

    uint8_t sc = 0;
    uint8_t ac = 0;

    for (uint8_t i = 0; i < DataToSend[3] + 4; i++)
    {
        sc += DataToSend[i];
        ac += sc;
    }
    DataToSend[cnt++] = sc;
    DataToSend[cnt++] = ac;

    HAL_UART_Transmit(&huart2, DataToSend, cnt, 0xFFFF);
}
