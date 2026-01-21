

/*

根据大彩屏幕的基本指令
书写的组合扩展功能
*/

#include "main.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"
#include "bigOrLittel.h" //用于大端与小端模式转换的
#include "hmi_userAction.h"

#define UART_DELAY_ONE_CHAR 20000 // 定义串口发送单个字符的时间,过短大彩不能识别

uint8_t uartTxData[64];

// 用于2个帧之间的延时,不检查标志位了,生猛一点
// len--发送的字符个数,根据个数自动调整延时时间
static void delay(uint8_t len)
{
    uint32_t i;
    i = UART_DELAY_ONE_CHAR; // 一个字符大概的时间

    for (; len > 0; len--)
    {
        while (i--)
            ;
        i = UART_DELAY_ONE_CHAR; // 一个字符大概的时间
    }
}

void daCaiSetFrontColor(uint16_t color)
{
    uint16_t *pIntTemp; // 定义字符长度16bit数据的地址累加,这里主要是++运算符,取决于变量类型,对于16bit变量,一个++运算移动16bit
    uint8_t *pCharTemp; // 定义字符长度8bit数据的地址累加,这里主要是++运算符,取决于变量类型
    uint8_t len;

    pCharTemp = uartTxData;
    *(pCharTemp++) = 0xEE; // 帧头
    *(pCharTemp++) = 0x41; // 命令--指定前景颜色

    pIntTemp = (uint16_t *)pCharTemp; // 得到用于整型的地址,应为2个字节表示一个坐标值

    *(pIntTemp++) = BigtoLittle16(color); // 大端数据到小端数据的转换

    pCharTemp = (uint8_t *)pIntTemp; // 得到用于字节的地址

    *(pCharTemp++) = 0xFF;
    *(pCharTemp++) = 0xFC;
    *(pCharTemp++) = 0xFF;
    *(pCharTemp++) = 0xFF;

    /*
    计算字符串长度,
    这里不能使用sizeof,和strlen
    因为sizeof是变量占用系统的空间,
    strlen是"/0"前占用的空间,可以寄理解成实际使用空间
    */
    len = pCharTemp - &uartTxData[0];

    HAL_UART_Transmit_IT(HUART, (uint8_t *)uartTxData, len);
    delay(len);
}

/*
    在屏幕上画线程序
    指令格式：EE【 51  X0     Y0   X1   Y1】FF FC FF FF
            EE   51  005A  00B9 0035 0080 FF FC FF FF
*/
void daCaiDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{

    uint16_t *pIntTemp; // 定义字符长度16bit数据的地址累加,这里主要是++运算符,取决于变量类型,对于16bit变量,一个++运算移动16bit
    uint8_t *pCharTemp; // 定义字符长度8bit数据的地址累加,这里主要是++运算符,取决于变量类型
    uint8_t len;

    daCaiSetFrontColor(color);

    pCharTemp = uartTxData;
    *(pCharTemp++) = 0xEE; // 帧头
    *(pCharTemp++) = 0x51; // 命令--画线

    pIntTemp = (uint16_t *)pCharTemp; // 得到用于整型的地址,应为2个字节表示一个坐标值

    *(pIntTemp++) = BigtoLittle16(x0); // 大端数据到小端数据的转换
    *(pIntTemp++) = BigtoLittle16(y0); // 大端数据到小端数据的转换
    *(pIntTemp++) = BigtoLittle16(x1); // 大端数据到小端数据的转换
    *(pIntTemp++) = BigtoLittle16(y1); // 大端数据到小端数据的转换

    pCharTemp = (uint8_t *)pIntTemp; // 得到用于字节的地址

    *(pCharTemp++) = 0xFF;
    *(pCharTemp++) = 0xFC;
    *(pCharTemp++) = 0xFF;
    *(pCharTemp++) = 0xFF;

    /*
    计算字符串长度,
    这里不能使用sizeof,和strlen
    因为sizeof是变量占用系统的空间,
    strlen是"/0"前占用的空间,可以寄理解成实际使用空间
    */
    len = pCharTemp - &uartTxData[0];

    HAL_UART_Transmit_IT(HUART, (uint8_t *)uartTxData, len);
    delay(len);
}

/*
更新文本控件
destinationScreen:文本控件所在的窗口编号
destinationNumber:文本控件的编号
pString:文本控件的内容,注意要转换成字符送进来
*/
void setTextValue(uint16_t destinationScreen, uint16_t destinationNumber, char *pString)
{
    uint16_t *pIntTemp; // 定义字符长度16bit数据的地址累加,这里主要是++运算符,取决于变量类型,对于16bit变量,一个++运算移动16bit
    uint8_t *pCharTemp; // 定义字符长度8bit数据的地址累加,这里主要是++运算符,取决于变量类型
    uint8_t len;

    pCharTemp = uartTxData;
    *(pCharTemp++) = 0xEE; // 帧头

    *(pCharTemp++) = 0xB1; // 命令--更新控件数值
    *(pCharTemp++) = 0x10; // 命令--更新控件数值

    pIntTemp = (uint16_t *)pCharTemp; // 得到用于整型的地址,

    *(pIntTemp++) = BigtoLittle16(destinationScreen); // 大端数据到小端数据的转换
    *(pIntTemp++) = BigtoLittle16(destinationNumber); // 大端数据到小端数据的转换

    pCharTemp = (uint8_t *)pIntTemp; // 得到用于字节的地址

    // 文本内容
    strcat((char *)pCharTemp, pString); // 将显示的信息追加到待发送的数据后面,把src所指向的字符串（包括“\0”）复制到dest所指向的字符串后面（删除*dest原来末尾的“\0”）

    pCharTemp += strlen(pString);
    // pCharTemp--;//地址回退一个,用于消除'\0'

    // 帧尾
    *(pCharTemp++) = 0xFF;
    *(pCharTemp++) = 0xFC;
    *(pCharTemp++) = 0xFF;
    *(pCharTemp++) = 0xFF;

    /*
    计算字符串长度,
    这里不能使用sizeof,和strlen
    因为sizeof是变量占用系统的空间,
    strlen是"/0"前占用的空间,可以寄理解成实际使用空间
    */
    len = pCharTemp - &uartTxData[0];

    HAL_UART_Transmit_IT(HUART, (uint8_t *)uartTxData, len);
    delay(len);
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t state)
{
    if (screen_id == 0)
    {
        if (control_id == 1)
        {
            if (state == 0)
            {
                printf("1\r\n");
            }
            if (state == 1)
            {
                printf("2\r\n");
            }
        }
    }
    //    if(screen_id == 1)
    //    {
    //        if(control_id == 1)
    //        {
    //
    //        }
    //        else;
    //    }
    //		else if(screen_id == 2)
    //		{
    //
    //		}
    //
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{

    if (screen_id == 7 && control_id == 2) // 滑块控制
    {
        if (value < 100 || value > 0)
        {
        }
    }
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  握手通知
 */
void NOTIFYHandShake()
{
    SetButtonValue(3, 2, 1);
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16_t screen_id)
{
    // TODO: 添加用户代码
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8_t press, uint16_t x, uint16_t y)
{
    
	// TODO: 添加用户代码
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \details  文本控件的内容以字符串形式下发到MCU，如果文本控件内容是浮点值，
 *  \details  则需要在此函数中将下发字符串重新转回浮点值。
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
    //    if(screen_id == 0)                                 //屏幕页面0号
    //    {
    //        long value = 0;
    //        sscanf((char *)str, "%ld", &value);            //把字符串转换为整数
    //
    //			  if(control_id == 1)                            //判断控件号码
    //        {
    //         scara.motionParameterSet.posX=(float)value;
    //        }
    //				else if(control_id == 3)
    //				{
    //				 scara.motionParameterSet.posY=(float)value;
    //				}
    //				else if(control_id == 5)
    //				{
    //				 scara.motionParameterSet.posZ=(float)value;
    //				}
    //				else;
    //    }
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
    if (screen_id == 5)
    {
    }
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
    // TODO: 添加用户代码
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t item, uint8_t state)
{
    // TODO: 添加用户代码
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t item)
{
    if (screen_id == 13 && control_id == 1) // 获取当前选择控件的值
    {
    }

    if (screen_id == 13 && control_id == 2) // 获取当前选择控件的值
    {
    }
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
    if (screen_id == 8 && control_id == 7)
    {
        SetBuzzer(100);
    }
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8_t status, uint8_t *_data, uint16_t length)
{
    // TODO: 添加用户代码
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8_t status)
{
    // TODO: 添加用户代码
}
