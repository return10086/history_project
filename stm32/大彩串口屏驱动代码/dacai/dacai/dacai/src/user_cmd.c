/*
 * @作者: 程佳乐
 * @Date: 2023-03-02 15:43:11
 * @LastEditTime: 2023-03-02 18:47:36
 * @描述:
 * @FilePath: \MDK-ARMe:\project\stm32\dacai\dacai\dacai\src\user_cmd.c
 * @人生目标: 全世界无产者联合起来
 */

// 触摸屏交互程序

#include "main.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "string.h"
#include "uartInterrupte.h"

// 大彩屏幕相关
#include "hmi_userAction.h"
#include "processMessage.h"

/*
   特别注意
   MCU不要频繁向串口屏发送数据，否则串口屏的内部缓存区会满，从而导致数据丢失(缓冲区大小：标准型8K，基本型4.7K)
   1) 一般情况下，控制MCU向串口屏发送数据的周期大于100ms，就可以避免数据丢失的问题；
   2) 如果仍然有数据丢失的问题，请判断串口屏的BUSY引脚，为高时不能发送数据给串口屏。
   TODO: 添加用户代码
   数据有更新时标志位发生改变，定时100毫秒刷新屏幕
*/
void dacai_cmd()
{
    qsize size = 0;

    HAL_UART_Receive_IT(&huart1, uart2RececieData, 1); // 开启中断接收函数

    while (1)
    {
//        size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE); // 从串口中解析指令

        if (size > 0) // 接收到指令 ，及判断是否为开机提示
        {
            ProcessMessage((PCTRL_MSG)cmd_buffer, size); // 指令处理
        }

        HAL_Delay(200); // 每20ms解析一次数据
    }
}
