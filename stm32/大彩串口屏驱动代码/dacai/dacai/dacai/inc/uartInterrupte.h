/*
 * @作者: 程佳乐
 * @Date: 2023-03-02 15:43:11
 * @LastEditTime: 2023-03-02 18:47:08
 * @描述:
 * @FilePath: \MDK-ARMe:\project\stm32\dacai\dacai\dacai\inc\uartInterrupte.h
 * @人生目标: 全世界无产者联合起来
 */

/*
串口发送中断的处理函数,可以直接在这里处理,但是我为了保持程序的可读性,我将这里直接处理

使用说明:
在需要的地方,调用一次下面的函数,开启串口中断接收,(仅调用一次即可)
之后程序会在收到数据时,持续进入中断

HAL_UART_Receive_IT(&huart2,uart2RececieData,1);

*/

#include "main.h"
#include "usart.h"
#include "cmd_queue.h"

extern uint8_t uart2RececieData[];
