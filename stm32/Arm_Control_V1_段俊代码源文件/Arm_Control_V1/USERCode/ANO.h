#include "main.h"

#define BYTE0(dwTemp)    (*(char *)(&dwTemp))
#define BYTE1(dwTemp)    (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)    (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)    (*((char *)(&dwTemp) + 3))

void ANO_SendF1(uint16_t m,uint16_t n,uint16_t p,uint16_t q);
void ANO_SendCheck(uint8_t ID_GET, uint8_t SC_GET, uint8_t AC_GET);
void ANO_PC_COMMAND(uint8_t *pBuffer);
void ANO_SendPC(uint16_t ID_SEND, int32_t DATA_SEND);
