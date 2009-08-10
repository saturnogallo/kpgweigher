#ifndef GPIBIFC_H
#define GPIBIFC_H
#include "reg51.h"

//define const on ioport
#define BDATA P1

sbit CTLPE= 	P0^1;
sbit CTLTE= 	P0^2;
sbit CTLDC= 	P0^3;

sbit BREN = 	P2^0;
sbit BIFC =	P2^1;
sbit BNDAC=	P2^2;
sbit BNRFD=	P2^3;
sbit BDAV =	P2^4;
sbit BEOI =	P2^5;
sbit BATN =	P2^6;
sbit BSRQ =	P2^7;

//define the const
#define BYTE 	unsigned char
#define BOOL 	BYTE
#define TRUE 	1
#define FALSE 	0
#define HIGH	0
#define LOW	1

#define SPBUSY 	0xd6
#define SPIDLE 	0xd8


//the format of the command is
//send sequence:BMARK CMDTYPE DATALENGTH DATA1 DATA2 .... EMARK
//reply sequence:BMARK STATUS DATA1 DATA2 .....EMARK BMARK 
//				(EMARK appeared in DATAi will be replaced with EMARK EMARK)

//defines for SP command format
//recieve part
#define BMARK 	0x5A
#define EMARK   0xA5
#define GPINIT 	0x11
#define GPWRT	0x22
#define GPRD	0x33
#define GPIFC	0x44
#define GPEOL   0x55
#define GPDCLR  0x66
#define GPLLO   0x77
#define GPLOCAL 0x88
#define GPREN	0x99
#define GPTRIG	0xaa
//send part
#define ESTATUS 0x11	//error occur
#define OSTATUS 0x22    //finished ok

//define prepare mode
#define DSEND	0
#define DREAD	1
#define CSEND	2
#define CREAD	3
#define MSEND	4
#define	MREAD	5

//define GPIB command Msg
#define MSG_LLO	0x11	//local lockout
#define MSG_DCL	0x14	//device clear
#define MSG_PPU 0x15	//pararell quest unconfig
#define MSG_SPE	0x18	//serial quest enable
#define MSG_SPD	0x19	//serial quest disable
#define MSG_SDC 0x04	//selected device clear
#define MSG_GTL	0x01	//go to local
#define MSG_GET 0x08	//group execute trigger
#define MSG_PPC 0x05	//pararell quest configure
#define MSG_TCT 0x39	//transfer controller
#define MSG_UNL 0x3F	//un listen
#define MSG_UNT 0x5F	//un talk
#define MSG_PPD 0x70	//pararell quest disable

#define MSG_CR  0x0A
#define MSG_LF  0x0d
#endif
