/** \file
*	@brief Common Const and Command Routine Defines.
*/
#ifndef _SJ_DEFINE_H_
#define _SJ_DEFINE_H_
#include "reg52.h"
#define CHAR_CMDBEGIN	':'
#define CHAR_CMDEND		';'
#define CHAR_LINEEND	0x0A
#ifndef BYTE
#define BYTE unsigned char
#endif
#ifndef BOOL
#define BOOL BYTE
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#define GET_BYTE(pbuf)	(pbuf[0]-'0')*10+(pbuf[1]-'0')///<combine 2char to BYTE 

//BYTE length;		///<indicat the buffer length (no use?)

//sbit tbit_1 = P3^4;
//sbit tbit_2 = P3^5;
//sbit tbit_3 = P3^6;
sbit asp_rx	= P3^3;
sbit asp_tx = P3^2;
/*
PIN ON DA BOARD
1	SDO  = P2^0
2	RESET = P2^1
3	DGND
4	CLR = P2^2
5	IOVCC
6	LDAC = P2^3
7	SDIN = P2^4
8	DGND
9	SCLK = P2^5
10	SYNC = P2^6
*/
sbit SDO = P2^0;
sbit RESET = P2^1;
sbit CLR = P2^2;
sbit LDAC = P2^3;
sbit SDI = P2^4;
sbit SCLK  = P2^5;
sbit SYNC  = P2^6;


#define SERIAL_VERSION_1_0

#define SP_BUFFER_MAX	32 ///<the fifo buffer size for all serial port


#define	SWI_DELAY	10///<wait(SleepMs(1)*SWI_DELAY after switch data sent 

#endif


