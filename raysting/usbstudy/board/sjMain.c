#include "sjDefine.h"
#include "sjSerial.h"
#include "AD5791.h"
#include "CH374INC.h"
#include "HAL.h"
#include <stdio.h>



unsigned char delayc;

/*
*/
//end interface routine
extern int	test( void );
extern void DBGS(const char* s);
extern void DBG(unsigned char);

extern void	FlushToEndp2(unsigned char);
extern void	FlushToEndp1();
extern void	UsbSendByte(UINT8);


void CommandProcessor()
{
	UINT8 temp,temp1,temp2;
	UINT8 sid;
	USBGetChar();
	if(temp != HMARK)
		return;
	
	USBGetChar();
	sid = temp;
	switch(sid)
	{
	case 'd'://send to DAC //5 byte data , 1st byte is for sum check = 0x00, (2nd-4th byte is data), 5th byte is function code,
			cm_ad5791(temp);
			sid = "";
			break;
	case 's'://send to switch
			cm_relay(temp);
			sid = "";
			break;
	case 'n'://send to navmeter
			io_putc(temp);
			break;
	case 't':
			{
				temp1=sjSerialWaitForOneByte();
				temp2=sjSerialWaitForOneByte(); //temp2 is check sum

				if( (temp1 + temp2 + temp) & 0xff == 0xff)
				{
					P0 = temp;
					P1 = temp1;
				}
				sid = "";
				sjSerialSendByte(EMARK);
			}
			break;
	default:
			break;
	}
}
extern void btnencode_loop();
unsigned long count = 0;
BOOL once = 0;
void timer_isr(void) interrupt 1 using 1
{
	asp_handler();
	asp_handler2();
	TF0 = 0; //clear timer
}
extern int	test( void );
void main()
{
	
	unsigned char sid;//id of serial to send 
	BYTE temp;
	long i=1000;


	IE = 0;//close int
				  // S1   CCP  SPI
	P_SW1 = 0x08  ; // 0 0  0 0  1 0   0  0
				  // X X PWM6  PWM2345 X  S4_S S3_S S2_S
    P_SW2 = 0x07  ; // 0 0  0      0     0    1    1     1

	AUXR = 0x10 ;// t0x12  t1x12  m0x6 t2r t2ct t2x12 extram s1st2
				 // 0       0      0    1   0     0      0     0
	T4T3M = 0x88;// t4r  t4ct  t4x12 t4clko t3r t3ct t3x12 t3clk
				 //	1     0     0      0      1  0    0     0
	INT_CLKO = 0x00;  //x ex4 ex3 ex2 x t2 t1 t0


	sjSerialInit();	//init COM1 and COM2 buffer

	//CLK_DIV = 0x40; //MCKO_S1 MCKO_S0 ADRJ TX_RX MCLKO_2 CLKS2 S1 S0 //24Mhz
					//	0      1        0    0     0      0       0 0  
	CLK_DIV = 0x80; //MCKO_S1 MCKO_S0 ADRJ TX_RX MCLKO_2 CLKS2 S1 S0  //12Mhz
					//	1      0        0    0     0      0       0 0  
	init_uart();  //fake serial port 3
	init_uart2(); //fake serial port 4

	asp_set_timer();


	sid = 0;
	
	AD5791Init();
	
	sjRelayInit();

	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/
	
	//DBGS("STARTUP DONE\r\n");	

	//test();
	mDelaymS(5000);


	CH374_PORT_INIT( );  /* CH374接口初始化 */

	temp = 0;
	while(temp != 0x01)
	{
		mDelaymS(50);
//		DBGS("\r\nSYSINFO = ");
		temp = Read374Byte(REG_SYS_INFO) & 0x03;
//		DBG(temp);
//		temp = temp & 0x03;
	}
	mDelaymS(50);
	CH374_PORT_INIT( );  /* CH374接口初始化 */
	Init374Device( );  // 初始化USB设备



	TR1 = 1;		//ENABLE COM1
	AUXR |= 0x10;	//Enable COM2

	PT0 = 1; 		//improve timer0 interrupt priority
    ET0 = 1;        //enable timer0 interrupt

	IE = 0x92;//enable serial int and timer0 interrupt//IE=90
	IE2 = 1;
	EA = 1;
	DBGS("\r\nSTARTUP DONE ");

	while(1)
	{
//		test();
		if ( Query374Interrupt( ) )
 	    {
//			sjSerialSendByte('<');
			USB_DeviceInterrupt( );  // 等待USB设备中断，然后处理USB设备中断
//			sjSerialSendByte('>');
		}
//		if (sjSerialIsDataWaiting())
//		{
//			temp = sjSerialWaitForOneByte();
//			FlushToEndp2(BIT_EP2_TRAN_TOG);
//		}
		if (sjSerialIsDataWaiting())
		{
			temp = sjSerialWaitForOneByte();
			sjSerialSendByte(temp);
		}	
		if (sjSerialIsDataWaiting2())
		{
			temp = sjSerialWaitForOneByte2();
			sjSerialSendByte2(temp);
		}	
		if (io_hasc())
		{
			temp = io_getc();
			io_putc(temp);
		}
		if (io_hasc2())
		{
			temp = io_getc2();
			io_putc2(temp);
		}	

		continue;
	}
//	btnencode_loop();
}



