#ifndef FASHION_H
#define FASHION_H

#include "main.h"

//´ı·¢ËÍÊı×é
extern uint8_t Send_buf[200];


int init_my_arm(void);
void angle_ctrl(int num_of_steer,double angle,uint16_t reach_ms,uint16_t power);
int solve_receive(uint8_t* re_buf);

#endif



