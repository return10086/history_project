/*
 * @作者: 程佳乐
 * @Date: 2023-03-02 15:43:11
 * @LastEditTime: 2023-03-02 18:48:10
 * @描述:
 * @FilePath: \MDK-ARMe:\project\stm32\dacai\dacai\dacai\inc\cmd_process.h
 * @人生目标: 全世界无产者联合起来
 */
/*!
 *  \file hmi_driver.h
 *  \brief 指令分类处理
 *  \version 1.0
 *  \date 2012-2018
 *  \copyright 广州大彩光电科技有限公司
 */

#ifndef _CMD_PROCESS_H
#define _CMD_PROCESS_H
#include "hmi_driver.h"

/*
事件命令列表
*/
#define NOTIFY_TOUCH_PRESS 0X01       // 触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE 0X03     // 触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK 0X0C    // 写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD 0X0D // 写FLASH失败
#define NOTIFY_READ_FLASH_OK 0X0B     // 读FLASH成功
#define NOTIFY_READ_FLASH_FAILD 0X0F  // 读FLASH失败
#define NOTIFY_MENU 0X14              // 菜单事件通知
#define NOTIFY_TIMER 0X43             // 定时器超时通知
#define NOTIFY_CONTROL 0XB1           // 控件更新通知
#define NOTIFY_READ_RTC 0XF7          // 读取RTC时间
#define MSG_GET_CURRENT_SCREEN 0X01   // 画面ID变化通知
#define MSG_GET_DATA 0X11             // 控件数据通知
#define NOTIFY_HandShake 0X55         // 握手通知

#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0] << 8) | ((uint8_t *)(PTR))[1])                                                                 // 从缓冲区取16位数据
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0] << 24) | (((uint8_t *)(PTR))[1] << 16) | (((uint8_t *)(PTR))[2] << 8) | ((uint8_t *)(PTR))[3]) // 从缓冲区取32位数据

typedef enum
{
    kCtrlUnknown = 0x0,
    kCtrlButton = 0x10, // 按钮
    kCtrlText,          // 文本
    kCtrlProgress,      // 进度条
    kCtrlSlider,        // 滑动条
    kCtrlMeter,         // 仪表
    kCtrlDropList,      // 下拉列表
    kCtrlAnimation,     // 动画
    kCtrlRTC,           // 时间显示
    kCtrlGraph,         // 曲线图控件
    kCtrlTable,         // 表格控件
    kCtrlMenu,          // 菜单控件
    kCtrlSelector,      // 选择控件
    kCtrlQRCode,        // 二维码
} CtrlType;

#pragma pack(push)
#pragma pack(1) // 按字节对齐

typedef struct
{
    uint8_t cmd_head; // 帧头

    uint8_t cmd_type;     // 命令类型(UPDATE_CONTROL)
    uint8_t ctrl_msg;     // CtrlMsgType-指示消息的类型
    uint16_t screen_id;   // 产生消息的画面ID
    uint16_t control_id;  // 产生消息的控件ID
    uint8_t control_type; // 控件类型

    uint8_t param[256]; // 可变长度参数，最多256个字节

    uint8_t cmd_tail[4]; // 帧尾
} CTRL_MSG, *PCTRL_MSG;

#pragma pack(pop)

#endif
