/*
 * @作者: 程佳乐
 * @Date: 2023-03-02 15:43:11
 * @LastEditTime: 2023-03-02 18:46:58
 * @描述:
 * @FilePath: \MDK-ARMe:\project\stm32\dacai\dacai\dacai\inc\processMessage.h
 * @人生目标: 全世界无产者联合起来
 */

#include "hmi_driver.h"
#include "cmd_process.h"
extern uint8_t cmd_buffer[CMD_MAX_SIZE]; // 每次从缓冲区解析的数量
extern uint16_t current_screen_id;       // 当前画面ID

/*!
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage(PCTRL_MSG msg, uint16_t size);
