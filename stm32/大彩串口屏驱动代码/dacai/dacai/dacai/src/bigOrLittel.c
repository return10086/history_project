


#include "stdio.h"

/*
判断机器存储的形式是大端还是小端
*/
void bigOrLittle()
{
int i=1;   
char *p=(char *)&i;   
if(*p == 1)     
    printf("小端模式"); 
else // (*p == 0)
    printf("大端模式");
}

