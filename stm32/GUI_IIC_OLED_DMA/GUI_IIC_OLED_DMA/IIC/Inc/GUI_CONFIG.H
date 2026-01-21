
#include "config.h"





/****************************************************************************************
* 文件名：GUI_CONFIG.H
* 功能：用于裁剪GUI代码。
* 作者：黄绍斌
* 日期：2004.03.11
* 备注：将宏设置为1时使能相应功能函数，为0或其它值时禁止。
****************************************************************************************/
#ifndef  GUI_CONFIG_H
#define  GUI_CONFIG_H









/* 有宽度的直线函数 */
#define   GUI_LineWith_EN		1
/* 画圆函数 */
#define   GUI_CircleX_EN		1
/* 椭圆函数 */
#define   GUI_EllipseX_EN		1
/* 圆弧函数 */
#define   GUI_ArcX_EN			1
/* 扇形函数 */
#define   GUI_Pieslice_EN		1





/*
填充函数
填充函数 GUI_FloodFill()使能控制 设置为 1 时函数有效 为 0 或其它值时函数禁止
当使用填充函数时 可以定义 DOWNP_N UPP_N 宏来设置向上及向下折点个数 这两个
宏用于定义保存向上及向下折点数据的数组大小
*/
#define   GUI_FloodFill_EN		1
/* 向上及向下节点个数(填充函数) */
#define   DOWNP_N			20
#define   UPP_N				20






/*
窗口管理
如 GUI_WindowsDraw() GUI_WindowsHide() GUI_WindowsClr()等 由于窗口管理函数
使用到 5×7 ASCII 字体显示 所以必需同时设置 FONT5x7_EN 为 1
*/
#define   GUI_WINDOW_EN			1



/*
图标菜单
如 GUI_MenuIcoDraw() GUI_Button49x14() GUI_Button_OK()
GUI_Button_Cancle() GUI_Button_OK1() GUI_Button_Cancle1()等
*/
#define  GUI_MenuIco_EN			1


/*
下拉菜单
如       GUI_MMenuDraw()
         GUI_MMenuSelect()
         GUI_MMenuNSelect()
         GUI_SMenuDraw ()
         GUI_SMenuSelect ()
         GUI_SMenuHide ()等
*/
#define  GUI_MenuDown_EN		1



/* 5*7字体 */
#define  FONT5x7_EN				1


/* 8*8字体 */
#define  FONT8x8_EN				1


/* 24*32字体 */
#define  FONT24x32_EN			1


/* 汉字显示函数 */
#define  GUI_PutHZ_EN			1


/*
单色图形显示函数
                GUI_LoadPic()
                GUI_LoadPic1()
*/
#define	 GUI_LoadPic_EN			1


/*
颜色转换
        GUI_Color2Index_565()
        GUI_Color2Index_555()
        GUI_Color2Index_444()
        GUI_Color2Index_332
*/
#define  CONVERTCOLOR_EN		1


#endif
