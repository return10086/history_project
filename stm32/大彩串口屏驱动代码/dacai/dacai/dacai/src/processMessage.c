
/*
大彩触摸屏指令解析及处理函数
用户可以根据需要自己更改
*/



#include "main.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "hmi_userAction.h"



uint8_t  cmd_buffer[CMD_MAX_SIZE];                                                     //指令缓存





/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
*  \brief  消息处理流程
*  \param msg 待处理消息
*  \param size 消息长度
*/
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
    volatile uint8_t cmd_type = msg->cmd_type;                                                  //指令类型
    volatile uint8_t ctrl_msg = msg->ctrl_msg;                                                  //消息的类型
    volatile CtrlType control_type = (CtrlType)msg->control_type;                               //控件类型
    volatile uint16_t screen_id = PTR2U16(&msg->screen_id);                                     //画面ID
    volatile uint16_t control_id = PTR2U16(&msg->control_id);                                   //控件ID
    volatile uint32_t value = PTR2U32(msg->param);                                              //数值


    switch(cmd_type)
    {
        case NOTIFY_TOUCH_PRESS:                                                        //触摸屏按下
					if(screen_id==0&&control_id==1)
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
						while(1);
						break;
        case NOTIFY_TOUCH_RELEASE:                                                      //触摸屏松开
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
            break;

        case NOTIFY_WRITE_FLASH_OK:                                                     //写FLASH成功
            NotifyWriteFlash(1);
            break;

        case NOTIFY_WRITE_FLASH_FAILD:                                                  //写FLASH失败
            NotifyWriteFlash(0);
            break;

        case NOTIFY_READ_FLASH_OK:                                                      //读取FLASH成功
            NotifyReadFlash(1, cmd_buffer + 2, size - 6);                               //去除帧头帧尾
            break;

        case NOTIFY_READ_FLASH_FAILD:                                                   //读取FLASH失败
            NotifyReadFlash(0, 0, 0);
            break;

        case NOTIFY_READ_RTC:                                                           //读取RTC时间         
            break;

        case NOTIFY_CONTROL://控件更新通知 0xB1
        {
            if(ctrl_msg == MSG_GET_CURRENT_SCREEN)   //0x01                         //画面ID变化通知
            {
                NotifyScreen(screen_id);                                            //画面切换调动的函数
            }
            else
            {
                switch(control_type)//控件类型(按钮,文本,进度条,滑动条,仪表....)
                {
                    case kCtrlButton:                                                   //按钮控件
                        NotifyButton(screen_id, control_id, msg->param[1]);
                        break;

                    case kCtrlText:                                                     //文本控件
                        NotifyText(screen_id, control_id, msg->param);
                        break;

                    case kCtrlProgress:                                                 //进度条控件
                        NotifyProgress(screen_id, control_id, value);
                        break;

                    case kCtrlSlider:                                                   //滑动条控件
                        NotifySlider(screen_id, control_id, value);
                        break;

                    case kCtrlMeter:                                                    //仪表控件
                        NotifyMeter(screen_id, control_id, value);
                        break;

                    case kCtrlMenu:                                                     //菜单控件
                        NotifyMenu(screen_id, control_id, msg->param[0], msg->param[1]);
                        break;

                    case kCtrlSelector:                                                 //选择控件
                        NotifySelector(screen_id, control_id, msg->param[0]);
                        break;

                    case kCtrlRTC:                                                      //倒计时控件
                        NotifyTimer(screen_id, control_id);
                        break;

                    default:
                        break;
                }
            }

            break;
        }

        case NOTIFY_HandShake:                                                          //握手通知
            NOTIFYHandShake();
            break;

        default:
            break;
    }
}




