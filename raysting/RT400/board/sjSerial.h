/**	\file
*	@serial interface function
*
*	Multi Serial management and manage
*/

#ifndef _SJ_SERIAL_H_
#define _SJ_SERIAL_H_
#include "typedef.h"

#define PORT_KBD	1
#define PORT_NAV	2
#define PORT_PC		3
void nav_uart_push(unsigned char dat);


void sjSerialInit();///< init the serial setting  on PortID
BOOL sjSerialIsDataWaiting();///<check if there data waiting on port
void sjSerialSendByte(unsigned char udata);///<byte send on PortID
unsigned char sjSerialWaitForOneByte();///<wait until one BYTE appear on port

BOOL sjSerialIsDataWaiting2();///<check if there data waiting on port
void sjSerialSendByte2(unsigned char udata);///<byte send on PortID
unsigned char sjSerialWaitForOneByte2();///<wait until one BYTE appear on port

void prints(u8 *str, u8 length, char uart_port);

#endif
