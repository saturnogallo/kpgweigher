#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "stc_new_8051.h"

#include <absacc.h>
#include <intrins.h>

#define	MAX(a,b)	(a>b?a:b)
#define MIN(a,b)	(a<b?a:b)

typedef unsigned char	u8;
typedef unsigned int	u16;
typedef int		i16;
typedef char		i8;
typedef unsigned char	bool;
typedef unsigned long	u32;
typedef	long		i32;
typedef	char		BOOL;
typedef unsigned char  INT8U;
typedef unsigned int   INT16U;
typedef unsigned char BYTE;



#define	false		0
#define	true		1

#ifndef NULL
#define	NULL		0
#endif

/*
	PT related function
*/
#define KEY_A	0x100000
#define KEY_B	0x200000
#define KEY_C	0x400000
#define KEY_D	0x800000

#define KEY_E	0x10000
#define KEY_F	0x20000
#define KEY_G	0x40000
#define KEY_H	0x80000

#define KEY_I	0x1000
#define KEY_J	0x2000
#define KEY_K	0x4000
#define KEY_L	0x8000

#define KEY_M	0x100
#define KEY_N	0x200
#define KEY_O	0x400
#define KEY_P	0x800
// 测试PT6312的程序
// 初始化PT6312
void PT6312_Init();

// 清空缓冲区
void PT6312_Refresh();

void PT6312_Print(u8 *buf, u8 size);

// 写入LED数据
void PT6312_WriteLED( u8 dat );

// 读取键盘
u32 PT6312_ReadKey();

void PT6312_Test();

char highc(u8 x);
char lowc(u8 x);
/*
	ds1302 related constant
*/

sbit T_CLK = P1^1; /*实时时钟时钟线引脚 */
sbit T_IO = P1^2; /*实时时钟数据线引脚 */
sbit T_RST = P1^3; /*实时时钟复位线引脚 */



void ds1302_init();

void ds1302_get(u8 ucCurtime[]);
void ds1302_set(u8 ucCurtime[]);

/*
	CS5532 definition
*/
#define B_GRP1		0x00

#define	B_WRITE		0x00
#define B_READ		0x08

#define B_CH1		0x00
#define B_CH2		0x10
#define B_CH		B_CH2

#define B_OFFSET	1
#define B_GAIN		2
#define B_CONF		3
#define B_SETUP		5

#define B_GRP2		0x80

#define B_SADC		0x00
#define B_MADC		0x40

#define B_SETUP1	0x00
#define B_SETUP2	0x08
#define B_SETUP3	0x10
#define B_SETUP4	0x18
#define B_SETUP5	0x20
#define B_SETUP6	0x28
#define B_SETUP7	0x30
#define B_SETUP8	0x38

#define B_NORMAL_CONV		0x00
#define	B_SELF_OFFSET_CAL	0x01
#define B_SELF_GAIN_CAL		0x02
#define B_SYS_OFFSET_CAL	0x05
#define B_SYS_GAIN_CAL		0x06

#define CMD_WRITE_CFG_REG 0x3
#define CMD_READ_CFG_REG 0xB

#define CMD_READ_ALL_OFFSET_CAL 0x49
#define CMD_WRITE_ALL_OFFSET_CAL 0x41

#define CMD_READ_ALL_GAIN_CAL 0x4A
#define CMD_WRITE_ALL_GAIN_CAL 0x42

#define CMD_READ_ALL_SETUP_REG 0x4D
#define CMD_WRITE_ALL_SETUP_REG 0x45

#define CMD_STOP_CONT_CONV 0xFF


#define TIMEOUTERR 0xff
#define SUCCESSFUL 0x00  
#define CAL_TIMEOUT_LIMIT 4000

#define RESET_VALID 0x10000000	//configure register after reset
//CS5532 Gain settings
#define GAINX1 	0
#define GAINX2 	1
#define GAINX4 	2
#define GAINX8 	3
#define GAINX16 4
#define GAINX32 5
#define GAINX64 6

//CS5532 Word rate settings
#define WORD_RATE_100SPS 	0x0
#define WORD_RATE_50SPS 	0x1
#define WORD_RATE_25SPS 	0x2
#define WORD_RATE_12P5SPS 	0x3
#define WORD_RATE_6P25SPS 	0x4
#define WORD_RATE_3200SPS 	0x8
#define WORD_RATE_1600SPS 	0x9
#define WORD_RATE_800SPS 	0xA
#define WORD_RATE_400SPS 	0xB
#define WORD_RATE_200SPS 	0xC

#define B_UNIPOLAR	1
#define B_BIPOLAR	0

#define B_LATCH		0x00

#define B_NODELAY	0
#define B_DELAY		1	//delay before conversion

#define B_NOOCD		0
#define B_OCD		1	//output 300nA for open circuit detect

#define B_OG_CH1	00	//offset gain register selection
#define B_OG_CH2	01
#define B_OG_CH3	10
#define B_OG_CH4	11

u8 init_cs5532(u16);
u32 read_cs5532(void);
/*
	eeprom constants
*/
//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

#define DATA_FLASH_START_ADDRESS 0x00  //STC5Axx 系列 EEPROM 测试起始地址
void eeprom_write(u16 add, u8 *buf, u8 size);
void eeprom_read(u16 add, u8 *buf, u8 size);
void Sector_Erase(INT16U add);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能
void Delay();

/* 
	void int0_hander(void) interrupt 0
	void timer0_hander(void) interrupt 1
	void int1_hander(void) interrupt 2
	void timer1_hander(void) interrupt 3
	void uart_hander(void) interrupt 4
	void lvd_hander(void) interrupt 6
*/
#endif
