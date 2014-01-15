#include "sjSerial.h"
#include "sjDefine.h"
//#include "reg52.h"
#include <stdio.h>

BYTE spbuf[SP_BUFFER_MAX];	///< serial port fifo buffer
static BYTE *spHead,*spTail;	///< buffer head and tailst
static BYTE spSFlag;///<serial port flag for send use

//const for SerialPort Status
#define SPBUSY 	0xd6	
#define SPIDLE 	0xd8


/**
*	@brief Set Initially for all serial port(under construction)
*	
*	Serial port format will be set identically
*/
void sjSerialInit()
{
	spHead=spbuf;
	spTail=spbuf;
	spSFlag=SPIDLE;
}

/********************************
*	Add the byte recieved to the 
*	Tail of the buffer
********************************/
void sjSerialHandler(void) interrupt 4 using 2
{
	//just handle serial interrupt 1
	if(TI)
	{
		TI = 0;
		spSFlag = SPIDLE;	
	}
	if(RI)
	{
		*spTail = SBUF;
		spTail = spTail+1;
		if(spTail == (spbuf+SP_BUFFER_MAX))
			spTail = spbuf;
		RI = 0;
	}
}

/**
*	@return TRUE when data availabel ,FALSE while not
*/
BOOL sjSerialIsDataWaiting()
{
	if(spHead == spTail)
		return FALSE;
	else 
		return TRUE;
}
/**
*	Send One Byte on Serial port , (under construction)
*	and return with no wait for send finished
*	@param udata the Byte to send
*	@param PortID serial port to use
*/
void sjSerialSendByte(BYTE udata)
{
	spSFlag=SPBUSY;
	//Send the udata
	SBUF = udata;
//	tbit_1 = 0;
	while(spSFlag == SPBUSY)	
		;
// 	tbit_1 = 1;
}
/**
*	Wait One Byte until that appeared,
*	it perform read byte function at the same time
*	@param PortID serial port to use
*	@return Byte we received
*/
BYTE sjSerialWaitForOneByte()
{
	BYTE udata;
	while(spHead == spTail)	
		;
	udata = *spHead;
	spHead = spHead+1;
	if(spHead == (spbuf+SP_BUFFER_MAX))
		spHead = spbuf;
	return udata;
}
