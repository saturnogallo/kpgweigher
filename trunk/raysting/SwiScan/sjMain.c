#include "sjDefine.h"
#include "sjSerial.h"
#include <stdio.h>

/**************************************************
*函数功能：读74ls165的端口                        *
*入口：无                                         *
*出口：read_data                                  *
**************************************************/
#define uint	unsigned int
sbit SH_LD = P1^1;
sbit CLK = P1^2;
sbit QH = P1^3;
uint read_int165(void)
{
	uchar i=0;
	uint read_data=0;
	SH_LD=0;    //置入控制有效，锁存端口值
	_NOP();
	SH_LD=1;
	for(i=0;i<16;i++)
	{
   	   read_data<<=1;
	   if(QH)
	   {
	   		read_data|=QH;
	   }
   	   CLK=0;             
	   _NOP();
	   CLK=1;          //上升沿
	   _NOP();

	}
	return read_data;
}




//end interface routine
void main()
{
	unsigned char temp;


	IE = 0;//close int

	sjSerialInit();
	
	//set serial port parameter (clock 11.0592M)
	//9600 baut rate 8 data non parity and 1 stop.
	SCON = 0x70;
	PCON = 0x00;
	//timer count
	TH1 = 0xfd;
	//use timer 1 to be serial
	//use timer 0 to be analog I/O
	TMOD = 0x22;
	TR1 = 1;

	IE = 0x90;//enable serial int and timer0 interrupt//IE=90

	/*
	*	Protocal routine: 
	*	1.	if ?0x0d is read, mean :fetch? command is recieved.
	*   2.  then send out the data  result will be parsed by round(x*10000000)/100000000.0
	*/
	while(1)
	{
		if(sjSerialIsDataWaiting())
		{
			temp = sjSerialWaitForOneByte();
			if(temp == HMARK)
			{
				temp = sjSerialWaitForOneByte();		
				if(temp != HMARK)
				{
					sid = temp;
					continue;
				}
			}
			if(sid == 's')//send to switch
			{
				if(temp == HMARK)
				{
					while(1)
					{
						temp = sjSerialWaitForOneByte();
						if(temp == EMARK)
							break;
						if(temp == RMARK)
						{
							swiReset();
							continue;
						}
						if(temp == QMARK)
							continue;
						swiAction(temp);
					}
					sjSerialSendByte(HMARK);
					temp = P2;
					sjSerialSendByte(temp);
					temp = P3;
/*
					temp = 0;
					if(P3^4)
						temp |= (0x10);
					if(P3^5)
						temp |= (0x20);
*/
					sjSerialSendByte(temp);
					sjSerialSendByte(EMARK);
				}
			}
			if(sid == 'n')//send to navmeter
			{
				ioputchar(temp);
			}
			if(sid == 't')
			{
				P0=sjSerialWaitForOneByte();
				P1=sjSerialWaitForOneByte();
				sjSerialSendByte(EMARK);
			}
		}
		if(kbhit())
		{
			temp = iogetchar();
			sjSerialSendByte(temp);
		}
	}
}



