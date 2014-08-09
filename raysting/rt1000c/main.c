//#include <t89c51rd2.h>
#include "stc51.h"
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"
#include "stdio.h"
#include "mnuart.h"
#include "eeprom.h"
RUNDATA rdata;
char buf[20];
uchar key;

#define CMD_READ	  0xc1	//read data
#define CMD_ZERO	  0xc2	//remove offset of navmeter
//const for SerialPort Status
#define SPBUSY 	0xd6	
#define SPIDLE 	0xd8

#define SP_BUFFER_MAX	32
uchar spbuf[SP_BUFFER_MAX];	///< serial port fifo buffer
static uchar *spHead = spbuf;
static uchar *spTail = spbuf;	///< buffer head and tailst
static uchar spSFlag = SPIDLE;///<serial port flag for send use
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

uchar sjSerialIsDataWaiting()
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
void sjSerialSendByte(uchar udata)
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
uchar sjSerialWaitForOneByte()
{
	uchar udata;
	while(spHead == spTail)	
	{
	}
	udata = *spHead;
	spHead = spHead+1;
	if(spHead == (spbuf+SP_BUFFER_MAX))
		spHead = spbuf;

	return udata;
}

double lastrx = -1;
void analog_timer()	interrupt 1 using 1
{
	Key_Scan();
}

void 		State_Update();
void State_Display()
{
	EA = 0;
	if(rdata.StateId == PG_ZERO){
		LCD_Cls();
		LCD_PrintHz16(60, 18, "扣零...");
		delay(2000);
		EA = 1;
		sjSerialSendByte(CMD_ZERO);
		EA = 0;
		LCD_Print8X16(40, 40, "OK.");
		delay(10000);
		rdata.pos_len = 0;
		rdata.StateId = PG_MENU1;
		
		/*
		if(rdata.pos_len != 0)
			LCD_PrintHz16(2,   2, " 1. 关闭");
		else
			LCD_PrintHz16(2,   2, "*1. 关闭");
		if(rdata.pos_len != 1)
			LCD_Print8X16(120,  2, " 2. 2400");
		else
			LCD_Print8X16(120,  2, "*2. 2400");

		if(rdata.pos_len != 2)
			LCD_Print8X16(2,    24," 3. 4800");
		else
			LCD_Print8X16(2,    24,"*3. 4800");
		if(rdata.pos_len != 3)
			LCD_Print8X16(120,  24," 4. 9600");
		else
			LCD_Print8X16(120,  24,"*4. 9600");
		
		if(rdata.pos_len != 4)
			LCD_Print8X16(2,    48," 5. 12800");
		else
			LCD_Print8X16(2,    48,"*5. 12800");
		*/
	}

	if(rdata.StateId == PG_CALIBRATE){
		LCD_Cls();
		LCD_PrintHz16(60, 18, "校准...");
		delay(2000);
		if((rdata.Rx < 1.2) && (rdata.Rx > 0.8))
		{
			LCD_Print8X16(40, 40, "OK.");
			rdata.cali_ratio = rdata.Rx * rdata.cali_ratio;  //raw reading
			SaveToEEPROM();
		}else{
			LCD_Print8X16(40, 40, "FAIL.");
		}
		delay(10000);
		rdata.pos_len = 0;
		rdata.StateId = PG_MENU1;

	}

	if(rdata.StateId == PG_MAIN){
		LCD_Cls();
		State_Update();
	}
	if(rdata.StateId == PG_MENU1){
		LCD_Cls();
		LCD_PrintHz16(2,  2, "1.设定标准电阻");
		LCD_PrintHz16(120,2, "2.设定换向时间");
		LCD_PrintHz16(2,  23,"3.电压表扣零");
		LCD_PrintHz16(120,23,"4.设定采样速度");
		LCD_PrintHz16(2,  46,"5.1：1校准");
		LCD_PrintHz16(120,  46,"6.退出");
	}
	if(rdata.StateId == PG_SETRS){
		LCD_Cls();
		LCD_PrintHz16(2,  2, "当前标准电阻值:");
		LCD_PrintHz16(2,22,  "   输入新阻值:");

		sprintf(buf,"%.4f",rdata.Rs);
		LCD_Print6X8(120,10,buf);
		State_Update();
	}
	if(rdata.StateId == PG_SETDELAY){
		LCD_Cls();
		LCD_PrintHz16(2,  12, "当前换向时间(秒):");
		sprintf(buf,"%i",(int)0 + rdata.Delay);
		LCD_Print6X8(140,17,buf);
		LCD_PrintHz16(2,32,  "  输入新设定(秒):");
		State_Update();
	}
	if(rdata.StateId == PG_SETRATE){
		LCD_Cls();
		LCD_PrintHz16(12,  12, "当前采样速度:");
		sprintf(buf,"%i",(int)0 + rdata.Samplerate);
		LCD_Print6X8(120,20,buf);
		LCD_PrintHz16(12,32,  "  输入新速度:");
		State_Update();
	}
	EA = 1;
}
void State_Update()
{
	EA = 0;
	if(rdata.StateId == PG_MAIN){
		sprintf(buf,"Is= ");
		sprintf(buf+4,"%.3f",rdata.Is);
		sprintf(buf+9," A");
		LCD_Print6X8(11,1,buf);

		sprintf(buf,"Ix= ");
		sprintf(buf+4,"%.3f",rdata.Ix);
		sprintf(buf+9," A");
		LCD_Print6X8(130,1,buf);

		sprintf(buf,"Rs= ");
		sprintf(buf+4,"%.6f",rdata.Rs);
		sprintf(buf+12," ");
		LCD_Print6X8(11,54,buf);

		sprintf(buf,"Rx= ");
		sprintf(buf+4,"%.6f",lastrx*1000);
		sprintf(buf+12," ");
		LCD_Print6X8(130,54,buf);

		LCD_ClsBlock(10,20,10+24*8,52);
		sprintf(buf,"%.6f",rdata.Rx*1000.0);
		sprintf(buf+8," ");
		LCD_Print24X32(10,18,buf);
		LCD_Print8X16(205,34,"m");
		LCD_PrintHz16(216,34,"Ω");
	}
	if(rdata.StateId == PG_SETRS){
		LCD_Print8X16(120,24,"        ");
		rdata.tempbuf[rdata.pos_len] = 0;
		LCD_Print8X16(120,24,rdata.tempbuf);
	}
	if(rdata.StateId == PG_SETDELAY){
		LCD_Print8X16(140,34,"        ");
		rdata.tempbuf[rdata.pos_len] = 0;
		LCD_Print8X16(140,34,rdata.tempbuf);
	}
	if(rdata.StateId == PG_SETRATE){
		LCD_Print8X16(120,34,"        ");
		rdata.tempbuf[rdata.pos_len] = 0;
		LCD_Print8X16(120,34,rdata.tempbuf);
	}
	if(rdata.StateId == PG_ZERO){
//		State_Display();
	}
	if(rdata.StateId == PG_CALIBRATE){
	}
	if(rdata.StateId == PG_MENU1){
	}

	EA = 1;
}
sbit SPI_CK = P3^5;
sbit SPI_DI1 = P3^4;
sbit SPI_DI2 = P0^5;
sbit SPI_DO = P3^3;
#define SM_WDELAY	100
#define SM_RDELAY	100
#define SM_RDELAY2	10
void   sm_write(uchar   value)  
{  
	uchar   no;  
       
  	for(no=0;no<8;no++) {  
	       SPI_CK = 1;      
		 
   	  	if   ((value &0x80)==0x80)  
       		  SPI_DO = 1;  
		else  
		      SPI_DO = 0;  
		 delay(SM_WDELAY);   
	     SPI_CK = 0;    
   		 value   =   (value <<1);  
		delay(SM_WDELAY);
  	}
	SPI_CK = 1;
}  


sbit sca_clk = P0^7;
sbit sca_dat = P0^6;
void display_scale()   //74164串行移位一个数
{
         uchar i=0;
		 uchar c= 0;
		 switch(rdata.Ratio)
		 {
 			case KEY_SCA10:  c = 0x11;break;
 			case KEY_SCA100: c = 0x22;break;
 			case KEY_SCA1K:  c = 0x44;break;
 			case KEY_SCA10K: c = 0x88;break;
			default:
				return;
		 }

         do{  
             if(c&0x01)     
                 sca_dat=1;
             else
				 sca_dat=0;
		     c>>=1;        //数组tabel中的第N个数向右移动一位
			 _nop_(); _nop_();
             sca_clk=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
             _nop_(); _nop_();
			 sca_clk=1;
			  _nop_(); _nop_();
             sca_clk=0;
			  _nop_(); _nop_();
             i++;
            }
        while(i<8);
}

uchar xdata ch1buf[5];
uchar xdata ch2buf[5];


ulong ch1val = 0;
ulong ch2val = 0;
double ch1valr, ch2valr;
uchar collect_data()
{
		int try;
		int pos;
		while(1)
		{
			while(sjSerialIsDataWaiting() > 0) //clean existing buffer
				ch1buf[0] = sjSerialWaitForOneByte();

			sjSerialSendByte(CMD_READ); //return total 8 bytes.
/*
			try = 0;	
			while(try++ < 200)
			{
				delay(10);
			}	
*/
			try = 0;
			pos = 0;
			while((try++ < 200) && (pos < 10))
			{
				if(sjSerialIsDataWaiting() > 0)
				{
					if(pos < 5)
						ch1buf[pos] = sjSerialWaitForOneByte();
					else
						ch2buf[(pos) - 5] = sjSerialWaitForOneByte();
					pos++;
					continue;
				}
				delay(10);
			}
//			if((ch1buf[4] == (ch1buf[0]+ch1buf[1]+ch1buf[2]+ch1buf[3])))
//			if(1)
			if( (pos == 10) &&
				(~ch1buf[4] == (0xff &(ch1buf[0]+ch1buf[1]+ch1buf[2]+ch1buf[3]))) &&
				(~ch2buf[4] == (0xff &(ch2buf[0]+ch2buf[1]+ch2buf[2]+ch2buf[3]))))
			{
//				sjSerialSendByte(0xff);
				ch1val = 0;
				ch1val = ch1val + ch1buf[0];	ch1val <<= 8;
				ch1val = ch1val + ch1buf[1];	ch1val <<= 8;
				ch1val = ch1val + ch1buf[2];	ch1val <<= 8;
				ch1val = ch1val + ch1buf[3];
				if(ch1val == 0)
					ch1val = 9.9;
				ch1valr = *((double*)(&ch1val));
				ch2val = 0;
				ch2val = ch2val + ch2buf[0];	ch2val <<= 8;
				ch2val = ch2val + ch2buf[1];	ch2val <<= 8;
				ch2val = ch2val + ch2buf[2];	ch2val <<= 8;
				ch2val = ch2val + ch2buf[3];	
				if(ch2val == 0)
					ch2val = 0;
				ch2valr = *((double*)(&ch2val));
				return 1;
			}else{
				return 0;
			}
		}
}
#define ONESEC	1282
sbit KTT=P3^7;
void main()
{
	uchar i,pos;
	uchar signhit;
	double chs,chx;
	int j;
	KTT = 0;
	signhit = 0;
	LCD_Init();
	Key_Init();
	State_Init();	
	
	//set serial port parameter (clock 11.0592M)
	//9600 baut rate 8 data non parity and 1 stop.
	SCON = 0x70;
	PCON = 0x00;

	//timer count
	TH1 = 0xfd;
	TH0 = 0xd8;
	TL0 = 0xf0;
	//use timer 1 to be serial
	//use timer 0 too
	TMOD = 0x22;
	TR1 = 1; //start timer 1
	TR0 = 1; //start timer 0
	ET1 = 0;
	ES = 1;
	PS = 1;
	EA = 1;
	ET0 = 1;
	
	i = SPI_DI1; //set to input
	i = SPI_DI2; //set to input

	key = KEY_INVALID;
	if(rdata.Rs < 0.001)
		rdata.Rs = 1;
	if ((rdata.cali_ratio < 0.8) || (rdata.cali_ratio > 1.2))
		rdata.cali_ratio = 1;
	 while(1)
	 { 
		if(key != KEY_INVALID)
		{
			if((key == KEY_SCA10)||(key == KEY_SCA100)||(key == KEY_SCA1K)||(key == KEY_SCA10K))
			{
				rdata.Ratio = key;
				display_scale();
				SaveToEEPROM();
			}else{

		 		i = rdata.StateId;
				pos = rdata.pos_len;
				State_Change(key);
				
				if(i != rdata.StateId){ 
					State_Display();
				}else{
				    if((pos != rdata.pos_len) || (key == KEY_UP) | (key == KEY_DN))
						State_Update();
				}
			}
			key = KEY_INVALID;
	  	}else{
			if(rdata.StateId != PG_MAIN) //配置页不处理数据
				continue;
			if(j > 0)
			{
				j--;
				continue;
			}else{
				KTT = 0;
			}
			if(rdata.delay_cnt != 0)
			{
				rdata.delay_cnt = rdata.delay_cnt-1;
				sprintf(buf,"%is",(int)0+rdata.delay_cnt);
				LCD_Print6X8(208, 22, "    ");
				LCD_Print6X8(208, 22, buf);

				j = ONESEC;
				continue;
			}
			//发送帧头
			//读串口数据
			if(0 == collect_data())
			{
					rdata.delay_cnt = 1;
					j = ONESEC;
					continue;
			}
//			sjSerialSendByte(0xfe);
			if((ch1valr < -10|| ch1valr > 10) ||
			(ch2valr < -10|| ch2valr > 10))
			{
				rdata.delay_cnt = 1;
				j = ONESEC;
				continue;
			}
//			sjSerialSendByte(0xfd);
			chs = ch1valr; ///2560000000.0;
			chx = ch2valr; ///2560000000.0;

			rdata.Is = chs / rdata.Rs;
			switch(rdata.Ratio)
			{
			case KEY_SCA10:
				rdata.Rx = chx  / (rdata.Is * 1.0);
				rdata.Ix = rdata.Is * 1.0;
				break;
			case KEY_SCA100:
				rdata.Rx = chx  / (rdata.Is * 10.0); 
				rdata.Ix = rdata.Is * 10.0;
				break;
			case KEY_SCA1K:
				rdata.Rx = chx  / (rdata.Is * 100.0);
				rdata.Ix = rdata.Is * 100.0;
				break;
			case KEY_SCA10K:
				rdata.Rx = chx  / (rdata.Is * 1000.0);
				rdata.Ix = rdata.Is * 1000.0;
				break;
			default:
				break;
			}
			if((chx < 0.00005) && (chx > - 0.00005))
			{
				rdata.Rx = 0;
			}
			rdata.Rx = rdata.Rx / rdata.cali_ratio;
			
			if(rdata.Rx > 0)
			{
				if(signhit == 1)
				{
					rdata.Rx = (rdata.Rx + lastrx)/2;
					signhit = 0;
				}
				lastrx = rdata.Rx; // rdata.Rx*2 - lastrx;
			}else{
				lastrx = -rdata.Rx;
				signhit = 1;
			}
			
			State_Update();
			//turn switch and set delay
			KTT = 1;
			rdata.delay_cnt = rdata.Delay;
			j = ONESEC;

		}
	}	
}
