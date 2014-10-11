/* CH374芯片 硬件抽象层 V1.0 */
/* 提供I/O接口子程序,提供寄存器级读写子程序 */

#include	"sjDefine.h"
#include	"CH374INC.H"

#ifndef	__CH374_HAL_H__
#define __CH374_HAL_H__

#ifdef __CX51__
#ifndef __C51__
#define __C51__		1
#endif
#endif

#ifdef __C51__
typedef const unsigned char code		UINT8C;
typedef unsigned char volatile xdata	UINT8XV;
#else
typedef const unsigned char		UINT8C;
typedef unsigned char volatile	UINT8XV;
#endif

#ifdef __C51__
#define	BIG_ENDIAN		1
#endif


//#define CH374_INT_WIRE			INT0	/* 假定CH374的INT#引脚,如果未连接那么也可以通过查询中断标志寄存器实现 */


// 附加的USB操作状态定义
#define		ERR_USB_UNKNOWN		0xFA		// 未知错误,不应该发生的情况,需检查硬件或者程序错误


/* 延时指定微秒时间,根据单片机主频调整,不精确 */
void	mDelayuS( UINT8 us );

/* 延时指定毫秒时间 */
void	mDelaymS( UINT8 ms );

void	CH374_PORT_INIT( );  /* CH374接口初始化 */

UINT8	Read374Byte( UINT8 mAddr );	// 从指定寄存器读取数据

void	Write374Byte( UINT8 mAddr, UINT8 mData );	// 向指定寄存器写入数据

void	Read374Block( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf );  /* 从指定起始地址读出数据块 */

void	Write374Block( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf );  /* 向指定起始地址写入数据块 */

void	Spi374Start( UINT8 addr, UINT8 cmd );  /* SPI开始 */
//void	Spi374OutByte( UINT8 d ); 
void    MySpi374Stop(void);	
// 查询CH374中断(INT#低电平)
BOOL	Query374Interrupt( void );

// 等待CH374中断(INT#低电平)，超时则返回ERR_USB_UNKNOWN
UINT8	Wait374Interrupt( void );

#endif
