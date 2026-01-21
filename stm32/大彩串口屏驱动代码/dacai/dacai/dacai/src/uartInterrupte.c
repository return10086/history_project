

/*
串口发送中断的处理函数,可以直接在这里处理,但是我为了保持程序的可读性,我将这里直接处理

使用说明:
■在需要的地方,调用一次下面的函数,开启串口中断接收,(仅调用一次即可)
■之后程序会在收到数据时,持续进入中断

HAL_UART_Receive_IT(&huart2,uart2RececieData,1);
■在调用上述文件的地方增加 #include "uartInterrupte.h"

*/



#include "main.h"
#include "usart.h"
#include "cmd_queue.h"

uint8_t uart2RececieData[20];






void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	
 if(huart==&huart1)
 {
	 //uart2TxOperition();
 }
}

//串口接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {

     //串口2的接收中断处理
     if(huart==&huart1)                            //收到来自PC机或者主机的数据
      {
				queue_push(uart2RececieData[0]);						//将收到数据放入指定的缓存,等待分析
        HAL_UART_Receive_IT(&huart1,uart2RececieData,1);
      }

 }

 
 
 
 
 
 
 