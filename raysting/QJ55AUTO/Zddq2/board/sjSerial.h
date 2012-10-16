/**	\file
*	@serial interface function
*
*	Multi Serial management and manage
*/

#ifndef _SJ_SERIAL_H_
#define _SJ_SERIAL_H_
#include "sjDefine.h"

void sjSerialInit();///< init the serial setting  on PortID
BOOL sjSerialIsDataWaiting();///<check if there data waiting on port
void sjSerialSendByte(BYTE udata);///<byte send on PortID
BYTE sjSerialWaitForOneByte();///<wait until one BYTE appear on port

#endif
