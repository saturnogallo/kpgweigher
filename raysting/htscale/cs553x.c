//The program for CS5532-ASZ
//This is a 24bit ADC and PGIA
//Made by OurWay and 2006/03/21

#include "define.h"
#include <intrins.h>


//根据实际情况定义
sbit SDI5532 = P2^1;
sbit SDO5532 = P2^2;
sbit CLK5532 = P2^3;
sbit CS5532 = P2^0;
#define CS_SET	
#define CS_CLR	

#define Adjust5532Run 0
#define ReadSADC5532Run 1
#define ReadMADC5532Run 1

//The ADC results varibles define
struct{
          u8 top;
          u8 high;
          u8 mid;
          u8 low;
         }
RegDat;

//The CS5532-ASZ comm define
#define RegRead  0x08
#define RegWrite 0x00

//=== Offset Register ===
#define OffsetRS 0x09

//=== Gain Register ===
#define GainRS 0x0a

//=== Configuration Register ===
#define ConfigWrite 0x03  //write config
#define ConfigRead  0x0b  //read config

#define PSS 	0x80       //Power Save Select
#define PDW 	0x40       //Power Down Mode
#define RS     	0x20       //Reset System
#define RV     	0x10       //Reset Valid
#define IS     	0x08       //Input Short
#define GB     	0x04       //Guard Signal Bit
#define VRS 	0x02       //Voltage Reference Select(Ref>2.5V,VRS=0)
#define A1     	0x01
#define A0     	0x80
#define OLS 	0x40
#define OGS 	0x10
#define FRS 	0x08


//=== Channel Setup Register ===
#define SetupCH1 0x05
#define SetupCH2 0x15

//Channel Select Bits
#define CH1 0x00   //CS1=0,CS0=0
#define CH2 0x40   //CS1=0,CS0=1
//Gain Bits
#define Gain1     0x00
#define Gain2     0x08
#define Gain4     0x10
#define Gain8     0x18
#define Gain16 	  0x20
#define Gain32 	  0x28
#define Gain64 	  0x30

//=== Converter mode ===
#define SingleC 0x80
#define ContinC 0xC0
#define Setup1 0x00
#define Setup2 0x08
#define Setup3 0x10
#define Setup4 0x18
#define Setup5 0x20
#define Setup6 0x28
#define Setup7 0x30
#define Setup8 0x38

//The data(8bit) form MCU to CS5532
static void _SendByte(u8 Dat)
{
	u8 i;
    CLK5532 = 0;
    for(i=8;i>0;i--)
    {
        SDI5532=(bit)(Dat & 0x80);
        CLK5532=1;
        _nop_();_nop_();
        _nop_();_nop_();
        CLK5532=0;
        _nop_();_nop_();
        _nop_();_nop_();
        Dat = Dat<<1;
    }
    SDI5532 = 1;
}

//The Setup CS5532's register
static void _WriteReg(u8 command,u8 low,u8 mid,u8 high,u8 top)
{
    CS_SET;
    _SendByte(command);
    _SendByte(low);
    _SendByte(mid);
    _SendByte(high);
    _SendByte(top);
    CS_CLR;
}

//The data(8bit) form CS5532 to MCU
static u8 _ReceiveByte(void)
{
    u8 i;
	u8 dat;
    dat=0;
    for(i=8;i>0;i--)
    {
       dat=dat<<1;
       if(SDO5532)
	   		dat |= 0x01;
       CLK5532=1;
       _nop_();_nop_();
       _nop_();_nop_();
       CLK5532=0;
       _nop_();_nop_();
       _nop_();_nop_();
    }
    return(dat);
}

//Receive ADC signal data from CS5532 to MCU
//single convert
void ReadSADC5532(u8 command)
{
    CS_SET;
    _SendByte(command);
    do{
		_nop_();
		CLK5532=0;
		SDI5532=0;
	}while(SDO5532!=0);			  //delay until SDO drops
    _SendByte(0x00);              //generate 8 bit SCLK only with SDI = 0;
    RegDat.top = _ReceiveByte();
    RegDat.high = _ReceiveByte();
    RegDat.mid = _ReceiveByte();
    RegDat.low = _ReceiveByte();
    CS_CLR;
}
void ReadMADC5532(u8 command)
{
    CS_SET;
    do{
		_nop_();
	}while(SDO5532!=0);
    //SDO5532 = 1;
    _SendByte(command);            //8bit SCLK and SDI=command;
    RegDat.top = _ReceiveByte();
    RegDat.high = _ReceiveByte();
    RegDat.mid = _ReceiveByte();
    RegDat.low = _ReceiveByte();
    CS_CLR;
}

//Receive CS5532's Register from CS5532 to MCU
void _ReadReg(u8 command)
{
    CS_SET;
    _SendByte(command);
    RegDat.top = _ReceiveByte();
    RegDat.high = _ReceiveByte();
    RegDat.mid = _ReceiveByte();
    RegDat.low = _ReceiveByte();
    CS_CLR;
}

void Adjust5532(u8 command)
{
    CS_SET;
    _SendByte(command);
    do{
		_nop_();
	}while(SDO5532!=0);
    _SendByte(0x0a);
    RegDat.top = _ReceiveByte();
    RegDat.high = _ReceiveByte();
    RegDat.mid = _ReceiveByte();
    RegDat.low = _ReceiveByte();
    CS_CLR;
}

//The initialization CS5532
void Init5532(void)
{
	//send SYNC1 command
    _WriteReg(0xff,0xff,0xff,0xff,0xff);
    _WriteReg(0xff,0xff,0xff,0xff,0xff);
    _WriteReg(0xff,0xff,0xff,0xff,0xff);
	//send SYNC0 command
    _WriteReg(0xff,0xff,0xff,0xff,0xfe);
}
//The CS5532-ASZ subpram end

