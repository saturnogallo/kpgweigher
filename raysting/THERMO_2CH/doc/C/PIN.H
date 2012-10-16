// 硬件相关定义, 请根据实际硬件修改本文件

#include <reg52.h>
#include <intrins.h>
#pragma NOAREGS		// 如果MCS51使用键盘中断功能，那么建议加入此编译选项

//#define		ENABLE_2_CH452		1	// 在2线接口下支持两个CH452芯片并联

/* 延时1uS子程序,主要用于2线接口,与单片机速度有关 */
//#define		DELAY_1US		{}				// MCS51<=10MHz
//#define		DELAY_1US		{_nop_();}		// MCS51<=20MHz
#define		DELAY_1US		{_nop_();_nop_();}	// MCS51<=30MHz

/* 2线接口的连接,与实际电路有关 */
sbit	CH452_SCL=P1^7;
sbit	CH452_SDA=P1^6;   // 如果使用真正的2线接口,那么SDA接中断引脚P3^3/INT1,用SDA直接做中断输出
//sbit	CH452_ADDR=P1^5;  // 实际电路应该硬件接GND或者VCC
sbit	CH452_INT=P3^3;   // 标准的2线接口使用该引脚输出中断

/* 4线接口的连接,与实际电路有关 */
sbit   CH452_DCLK=P1^7;                     // 串行数据时钟上升延激活
sbit   CH452_DIN=P1^6;		                // 串行数据输出，接CH452的数据输入
sbit   CH452_LOAD=P1^5;                     // 串行命令加载，上升延激活
sbit   CH452_DOUT=P3^3;                     // INT1，键盘中断和键值数据输入，接CH452的数据输出

/* 2线接口的位操作,与单片机有关 */
#define		CH452_SCL_SET		{CH452_SCL=1;}
#define		CH452_SCL_CLR		{CH452_SCL=0;}
#define		CH452_SCL_D_OUT		{}				// 设置SCL为输出方向,对于双向I/O需切换为输出
#define		CH452_SDA_SET		{CH452_SDA=1;}
#define		CH452_SDA_CLR		{CH452_SDA=0;}
#define		CH452_SDA_IN		(CH452_SDA)
#define		CH452_SDA_D_OUT		{}				// 设置SDA为输出方向,对于双向I/O需切换为输出
#define		CH452_SDA_D_IN		{CH452_SDA=1;}	// 设置SDA为输入方向,对于双向I/O需切换为输入

/* 4线接口的位操作,与单片机有关 */
#define		CH452_DCLK_SET		{CH452_DCLK=1;}
#define		CH452_DCLK_CLR		{CH452_DCLK=0;}
#define		CH452_DCLK_D_OUT	{}				// 设置DCLK为输出方向,对于双向I/O需切换为输出
#define		CH452_DIN_SET		{CH452_DIN=1;}
#define		CH452_DIN_CLR		{CH452_DIN=0;}
#define		CH452_DIN_D_OUT		{}				// 设置DIN为输出方向,对于双向I/O需切换为输出
#define		CH452_LOAD_SET		{CH452_LOAD=1;}
#define		CH452_LOAD_CLR		{CH452_LOAD=0;}
#define		CH452_LOAD_D_OUT	{}				// 设置LOAD为输出方向,对于双向I/O需切换为输出
#define		CH452_DOUT_D_IN		{CH452_DOUT=1;}	// 设置DOUT为输入方向,对于双向I/O需切换为输入

/* 与单片机有关,与中断连接方式有关 */
//#define		USE_CH452_KEY		1			// 使用了CH452的按键中断
#ifdef USE_CH452_KEY
#define		DISABLE_KEY_INTERRUPT	{EX1=0;}
#define		ENABLE_KEY_INTERRUPT	{EX1=1;}
#define		CLEAR_KEY_INTER_FLAG	{IE1=0;}
#else
#define		DISABLE_KEY_INTERRUPT	{}
#define		ENABLE_KEY_INTERRUPT	{}
#define		CLEAR_KEY_INTER_FLAG	{}
#endif
