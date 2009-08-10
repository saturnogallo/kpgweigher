#include "lib.h"

//const for SerialPort Status
#define SP_BUSY		 	0xd6	
#define SP_IDLE			0xd8
#define SP_NOINIT		0xda

u8 spbuf[SP_BUFFER_MAX];	///< serial port fifo buffer
u8 *spHead;
u8 *spTail;	///< buffer head and tail
u8 spSFlag;///<serial port flag for send use

/**
*	@brief Set Initially for all serial port(under construction)
*	
*	Serial port format will be set identically
*/
void HW1SerialInit()
{
	spHead=spbuf;
	spTail=spbuf;
	spSFlag=SP_IDLE;
}

/********************************
*	Add the byte recieved to the 
*	Tail of the buffer
********************************/
void HW1SerialInterrupt(void) interrupt 4 using 2
{
	//just handle serial interrupt 4
	if(TI){
		TI = 0;
		spSFlag = SP_IDLE;	
	}
	if(RI){
		*spTail = SBUF;
		spTail = spTail+1;
		if(spTail == (spbuf+SP_BUFFER_MAX))
			spTail = spbuf;
		RI = 0;
	}
}

/**
*	@return TRUE when data available ,FALSE while not
*/
BOOL HW1SerialDataWaiting()
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
void HW1SerialSendByte(BYTE udata)
{
	spSFlag=SP_BUSY;
	SBUF = udata;
	while(spSFlag == SP_BUSY)	
		;
}
/**
*	Wait One Byte until that appeared,
*	it perform read byte function at the same time
*	@param PortID serial port to use
*	@return Byte we received
*/
u8 HW1SerialWaitForOneByte(void)
{
	u8 dat;
	if(spHead == spTail)	{
		while(FALSE == HW1SerialDataWaiting())
			;
	}
	dat = *spHead;
	spHead = spHead + 1;
	if(spHead == (spbuf+SP_BUFFER_MAX))
		spHead = spbuf;
	return dat;
}
