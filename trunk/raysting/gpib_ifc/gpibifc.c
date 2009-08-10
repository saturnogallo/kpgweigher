#include "gpibifc.h"

//GLOBAL variable
//for serial use
#define MAXBUFFER 0x30
BYTE buffer[MAXBUFFER];
BYTE *pHead,*pTail;


BYTE mla;	//MLA , MTA=MLA+0x20;
BYTE eoimode; //the end mode of I/O string  
			  //omode=eoimode(bit7-4) imode=eoimode(bit3-0)
			  //mode: 0 <CR><LF>,1 <LF><CR> ,2 <LF> 3,<CR>
BYTE uFlgSP; //the flag used to ensure that data has been sent on serial

//the buffer for control line status (send and read)
BYTE bdata sctl;
BYTE bdata rctl;

sbit SIFC = 	sctl^0;
sbit RIFC = 	rctl^0;
sbit SREN = 	sctl^1;
sbit RREN =	rctl^1;
sbit SEOI = 	sctl^2;
sbit REOI = 	rctl^2;
#define SDAV 	sctl^3
#define RDAV 	rctl^3
#define SNRFD 	sctl^4
#define RNRFD	rctl^4
#define SNDAC 	sctl^5
#define RNDAC	rctl^5
#define SSRQ	sctl^6
#define RSRQ 	rctl^6
#define SATN 	sctl^7
#define RATN 	rctl^7

/********************************
*	Sleep mscout millisecond
********************************/
void SleepMs(int mscount)
{
	int i,j;
	while(mscount > 0)
	{
		for(i=1;i < 0x100;i++)
				;
		mscount--;
	}
}
/********************************
*	Sleep uscout Mircosecond
********************************/
void SleepUs(int uscount)
{
	while(uscount-- > 0)
		;
}

/********************************
*	Add the byte recieved to the 
*	Tail of the buffer
********************************/

void SerialHandler(void) interrupt 1 using 2
{
	if(TI == 1)
	{
		uFlgSP = SPIDLE;
		TI = 0;
		return;
	}
	RI = 0;
	*pTail = SBUF;
	pTail = pTail+1;
	if(pTail == (buffer+MAXBUFFER))
		pTail = buffer;
}

/********************************
*	Get the Byte from the head
*	of the Array(wait if not)
********************************/

BYTE WaitForOneByte()
{
	BYTE udata;
	while(pHead == pTail);
	udata = *pHead;
	pHead = pHead+1;
	if(pHead == (buffer+MAXBUFFER))
		pHead = buffer;
	return udata;
}

/********************************
*	send one byte at a time
********************************/
void SendOneByte(BYTE udata)
{
	uFlgSP = SPBUSY;
	SBUF = udata;
	while(uFlgSP == SPBUSY)	
		;
}

/**************************************
*	Write Data to the DIO Bus 
*	using 3 line hook(Pullup)
*************************************/
void GpibWriteData(BYTE uData)//BOOL bPullup,
{
	if((BNRFD == HIGH)&&(BNDAC == HIGH))
		return;
	BDATA = uData;
	CTLPE = TRUE;	//	set bPullup TRUE;
	SleepMs(1);//wait 1ms
	while(BNRFD != HIGH) ;
	BDAV = LOW;
	while(BNDAC != HIGH) ;
	BDAV = HIGH;	
	return;
}
/***********************************
*	Read data(Data or Command) from 
*	DIO Gpib bus using 3 line hook
***********************************/
BYTE GpibReadData()
{
	BYTE uData;
	BNRFD = HIGH;
	while(BDAV != LOW) ;
	BNRFD = LOW;
	uData = BDATA;
	REOI = BEOI; //save the state of EOI
	BNDAC = HIGH;
	while(BDAV != HIGH) ;
	BNDAC = LOW;
	return uData;
}

//do prepares before data transfer
//mode:
//	DSEND:	data-send
//	DREAD:	data-read 
//  CSEND:	command-send
//  MSEND:  enter controller mode
void GpibPrepare(int mode)
{
	if(mode == MSEND)
	{
		BIFC = LOW;
		BREN = LOW;
		BATN = 1;
		CTLDC = 0;
	}
	if(mode == DSEND)
	{	
		BDAV = HIGH;
		BEOI = LOW;
		CTLTE = 1;   //talk mode
		BATN = 0;	//data mode
	}
	if(mode == DREAD)
	{
		BNDAC = LOW;
		BNRFD = LOW;
		CTLTE = 0;	//listen mode
		BATN = 0;	//data mode
	}	
	if(mode == CSEND)
	{	
		BDAV = HIGH;
		BEOI = LOW;
		CTLTE = 1;	//talk mode
		BATN = 1;	//command mode
	}
}

/************************************
*	init the device using gribaddr and eoimode
*	data sequence: [addr,eoimode]
************************************/
void GpInit(void)
{
	mla = WaitForOneByte();
	mla = mla&0x3f;
	eoimode = WaitForOneByte();
}

/****************************************
*	send the string to the gpibaddr defined device
*	data sequence: [raddr length data1 data2 ... datan](no eol)
*****************************************/
void GpWrt(void)
{
	BYTE len;
	BYTE raddr;
	BYTE udata;

	raddr = WaitForOneByte();
	raddr = raddr&0x3f;

	GpibPrepare(CSEND);
	GpibWriteData(MSG_UNL);
	GpibWriteData(MSG_UNT);
	GpibWriteData(raddr); //send listener's address
	raddr = mla+0x20;
	GpibWriteData(raddr);	//send talker's address

	len = WaitForOneByte();
	GpibPrepare(DSEND);
	while(len > 0)
	{
		udata = WaitForOneByte();
		GpibWriteData(udata);
		len--;
	}
	//send oel
	raddr = eoimode&0xf0;
	if(raddr == 0x00)
	{
		GpibWriteData(MSG_CR);
		raddr = 0x20;
	}
	if(raddr == 0x10)
	{
		GpibWriteData(MSG_LF);
		raddr = 0x30;
	}
	BEOI = HIGH;SEOI = HIGH;
	if(raddr == 0x20)
	{
		GpibWriteData(MSG_LF);
	}
	if(raddr == 0x30)
	{
		GpibWriteData(MSG_CR);
	}
}
/**********************************
*	get the str from the grip device
*	data sequence: [raddr]
*	send sequence: [data1 data2 .. datan] 
*	if(datai == EMARK) an EMARK appended
**********************************/
void GpRd(void)
{
	BYTE raddr;
	BYTE udata;

	raddr = WaitForOneByte();
	raddr = raddr&0x3f;
	raddr = raddr+0x20;
	GpibPrepare(CSEND);
	GpibWriteData(MSG_UNL);
	GpibWriteData(MSG_UNT);
	GpibWriteData(mla);  	//send listener address
	GpibWriteData(raddr);	//send talker address

	GpibPrepare(DREAD);
	while(1)
	{
		udata=GpibReadData();
		if((udata == MSG_CR)||(udata == MSG_LF))
			break;
		SendOneByte(udata);
		if(udata == EMARK)
			SendOneByte(udata);
	}
	raddr = eoimode&0x0f;
	if((raddr == 0)||(raddr == 1))
	{
		GpibReadData();
	}
}
/********************************
*	clear the GPIB interface bus
********************************/
void GpIfc(void)
{
	BIFC = 1;SIFC = 1;
	SleepMs(100);
	BIFC = 0;SIFC = 0;
}

/*********************************
*	clear the device
*********************************/
void GpDclr(void)
{
	GpibPrepare(CSEND);
	GpibWriteData(MSG_DCL);
}
/**********************************
*Local Lock out device front panel of all
***********************************/
void GpLlo(void)
{
	GpibPrepare(CSEND);
	GpibWriteData(MSG_LLO);
}
/********************************
*	make device go local
*********************************/
void GpLocal(void)
{
	GpibPrepare(CSEND);
	GpibWriteData(MSG_GTL);
	BREN = 0;SREN = 0;
}

/**********************************
*	make the device on gpibaddr under remote control 
**********************************/
void GpRen(void)
{
	BREN = 1;SREN = 1;
}

/***********************************
*	send GET command to device
*	data sequence: [raddr]
************************************/
void GpTrig(void)
{
	BYTE raddr;
	raddr = WaitForOneByte();
	GpibPrepare(CSEND);
	GpibWriteData(MSG_UNL);
	GpibWriteData(raddr);
	GpibWriteData(MSG_GET);
}

void main()
{
	IE = 0x00;   	//close all the interrupt
	pHead = buffer;   //buffer to store is start at 0x7f(?)
    pTail = buffer;   //and max length is 0x30

	//set serial port parameter (clock 11.0592M)
	//9600 baut rate 8 data non parity and 1 stop.
	SCON = 0x70;
	PCON = 0x00;
	TH1 = 0xfd;
	TMOD = 0x20;
	TR1 = 1;
	EX1 = 1;

	//to be added :set timer 0 as a watch dog

	CTLDC = FALSE;		
	GpibPrepare(MSEND);
	mla = 0x23;

	//handle the data and deliver the command
	//the format of the command is
	//BMARK CMDTYPE parameter EMARK
	while(TRUE)
	{
		if(WaitForOneByte() != BMARK)
			continue;

		SendOneByte(BMARK);
		switch(WaitForOneByte())
		{
		case GPINIT	:	GpInit();	break;
		case GPWRT 	:	GpWrt();	break;
		case GPRD  	:	GpRd();		break;
		case GPIFC	:	GpIfc();	break;
//		case GPEOL	:	GpEol();	break;
		case GPDCLR	:	GpDclr();	break;
		case GPLLO	:	GpLlo();	break;
		case GPLOCAL:	GpLocal();	break;
		case GPREN	:	GpRen();	break;
		case GPTRIG :	GpTrig();	break;
		default		:	SendOneByte(EMARK);break;
		}
		SendOneByte(EMARK);
		SendOneByte(BMARK);
	}
}
