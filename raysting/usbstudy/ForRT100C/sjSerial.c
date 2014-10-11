#include "sjSerial.h"
#include "sjDefine.h"
//#include "reg52.h"
#include <stdio.h>

BYTE spbuf[SP_BUFFER_MAX];	///< serial port fifo buffer
static BYTE *spHead,*spTail;	///< buffer head and tailst
static BYTE spSFlag;///<serial port flag for send use

BYTE spbuf2[SP_BUFFER_MAX];	///< serial port fifo buffer
static BYTE *spHead2,*spTail2;	///< buffer head and tailst
static BYTE spSFlag2;///<serial port flag for send use

//const for SerialPort Status
#define SPBUSY 	0xd6	
#define SPIDLE 	0xd8


    
void InitCOM1(void)		//9600bps@11.0592MHz
{
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	SCON = 0x50;		//8bit data, no parity
	AUXR |= 0x40;		//1T of Fosc
	AUXR &= 0xFE;		//timer1 to com1
	TMOD &= 0x0F;		//16bit reload
   	TL1 = (65536 - (FOSC/4/BAUD1));   //????????
    TH1 = (65536 - (FOSC/4/BAUD1))>>8;
	//TL1 = 0xE8;	TH1 = 0xFF;		//115200 @11.0592MHz
	//TL1 = 0xD0;	TH1 = 0xFF;	//57600 @11.0592MHz	
	//TL1 = 0xE0;	TH1 = 0xFE;	//9600, @11.0592MHz
	//TL1 = 0x80;	TH1 = 0xFB;	//2400, @11.0592MHz
   	TR1 = 1;                    //start timer1
    ES = 1;                     //enable serial port interrupt
    
	//ET1 = 0;		//disable interrupt
}

void InitCOM2(void)		//57600bps@11.0592MHz
{
	P_SW2 |= S2_S0;             //S2_S0=1 (P4.6/RxD2_2, P4.7/TxD2_2)
	S2CON = 0x50;		//8bit data
	AUXR |= 0x14;		//1T to Fosc
	T2L = (65536 - (FOSC/4/BAUD2));   //????????
    T2H = (65536 - (FOSC/4/BAUD2))>>8;
	//TL2 = 0xE8;	TH2 = 0xFF;	//115200 @11.0592MHz
	//TL2 = 0xD0;	TH2 = 0xFF;	//57600 @11.0592MHz	
	//TL2 = 0xE0;	TH2 = 0xFE;	//9600, @11.0592MHz
	//TL2 = 0x80;	TH2 = 0xFB;	//2400, @11.0592MHz

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

/********************************
*	Add the byte recieved to the 
*	Tail of the buffer
********************************/
void sjSerialHandler2(void) interrupt 8 using 3
{
	//just handle serial interrupt 1
	if(S2CON & S2TI)
	{
		S2CON &= ~S2TI;
		spSFlag2 = SPIDLE;	
	}
	if (S2CON & S2RI)
	{
		*spTail2 = S2BUF;
		spTail2 = spTail2 + 1;
		if(spTail2 == (spbuf2+SP_BUFFER_MAX))
			spTail2 = spbuf2;
		S2CON &= ~S2RI;
	}
}
/**
*	@return TRUE when data availabel ,FALSE while not
*/
BYTE sjSerialIsDataWaiting()
{
	if(spHead == spTail)
		return 0;
	if(spTail > spHead)
		return spTail-spHead;

	return spTail+SP_BUFFER_MAX-spHead;
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
	while(spSFlag == SPBUSY)	
		;
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
	{
	}
	udata = *spHead;
	spHead = spHead+1;
	if(spHead == (spbuf+SP_BUFFER_MAX))
		spHead = spbuf;
	return udata;
}

//=================COM 2 related functions
/**
*	@return TRUE when data availabel ,FALSE while not
*/
BYTE sjSerialIsDataWaiting2()
{
	if(spHead2 == spTail2)
		return 0;
	if(spTail2 > spHead2)
		return spTail2-spHead2;

	return spTail2 + SP_BUFFER_MAX-spHead;
}
/**
*	Send One Byte on Serial port , (under construction)
*	and return with no wait for send finished
*	@param udata the Byte to send
*	@param PortID serial port to use
*/
void sjSerialSendByte2(BYTE udata)
{
	spSFlag2=SPBUSY;
	//Send the udata
	S2BUF = udata;
	while(spSFlag2 == SPBUSY)	
		;
}
/**
*	Wait One Byte until that appeared,
*	it perform read byte function at the same time
*	@param PortID serial port to use
*	@return Byte we received
*/
BYTE sjSerialWaitForOneByte2()
{
	BYTE udata;
	while(spHead2 == spTail2)	
	{
	}
	udata = *spHead2;
	spHead2 = spHead2 + 1;
	if(spHead2 == (spbuf2 + SP_BUFFER_MAX))
		spHead2 = spbuf2;
	return udata;
}

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

	spHead2=spbuf2;
	spTail2=spbuf2;
	spSFlag2=SPIDLE;

	InitCOM1();
	InitCOM2();
}
