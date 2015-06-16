#include "sjDefine.h"
#include "sjSerial.h"
//#include "AD5791.h"
//#include "CH374INC.h"
#include "HAL.h"
#include <stdio.h>

//the MARK of header ender and error and quest and reset
#define  HMARK 0x55	//U
#define  EMARK 0x56 //V
#define  QMARK 0x57 //W
#define  RMARK 0x58 //X
  
//the length of the switch table ,
//which must lower than 3fh
#define  LINIT 32 

//for P1 output
//for P0 output
const unsigned char code tbl[8]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};

void nop()
{
}



//delay loop
void swiDelay(unsigned char dl1,unsigned char dl2)
{
	unsigned char i,j;
	for(i=0;i<dl1;i++)
		for(j=0;j<dl2;j++)
			;
}

/*
void swiAction(unsigned char SwiID)
{
	unsigned char exID;

	if(SwiID >= sizeof(lut))
		return;

	exID = lut[SwiID];
	if(exID == 255)
		return;

	P1 = tbl[(exID&7)];
	P0 = tbl[(exID>>3)];
	swiDelay(0x1f,0xff);

	P1 = 0xff;
	P0 = 0xff;
	swiDelay(0x06,0xff);
}
void swiReset()
{
	unsigned char i;
	for(i=0;i<LINIT;i++)
	{
		swiAction(inittbl[i]);
	}
}
*/
unsigned char delayc;

/*
*/
//end interface routine
extern int	test( void );
extern void DBGS(const char* s);
extern void DBG(unsigned char);


//extern void btnencode_loop();
unsigned long count = 0;
BOOL once = 0;
void timer_isr(void) interrupt 1 using 1
{
	asp_handler();
	asp_handler2();
	TF0 = 0; //clear timer
}
extern int	test( void );

#define uchar unsigned char

uchar code led_code[]={0x77, 0x41,0x3b,0x6b,0x4d,0x6e,0x7e,0x43,0x7f,0x6f,0xff}; //LED显示
/*
					  0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6
					  0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0x03,0x62,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0xfd,0x61,0xdB,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,					  
*/
#define LED_HF		0x80//0x02
#define LED_SPACE	0x00//0x00
#define LED_PT		0x80//0x01
#define LEDPT_OP	|

//bits for LED display
//CP P4.4, DP = P4.3, MP = P4.2
sbit dat1 = P4^3;//P2^2;
sbit clk1 = P4^4;//P2^1;
sbit rst1 = P4^2;//P2^0;

void d164(uchar c)   //74164串行移位一个数
{
    uchar i=0;
     do{  
        if(c&0x01)     
           	dat1=1;
        else
			dat1=0;
		c>>=1;        //数组tabel中的第N个数向右移动一位
		 nop(); nop();
        clk1=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
        nop(); nop();
		clk1=1;
		nop(); nop();
        clk1=0;
		nop(); nop();
        i++;
    }
    while(i<8);
}

//bits for row output
/*
sbit dat2 = P2^5;
sbit clk2 = P2^4;
sbit rst2 = P2^3;

void c164(uchar c)   //74164串行移位一个bit
{
        if(c)     
           	dat2=1;
        else
			dat2=0;
		nop(); nop();
        clk2=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
        nop(); nop();
		clk2=1;
		nop(); nop();
        clk2=0;
		nop(); nop();
}
*/
void pause()
{
	while(1)
	{
		if(sjSerialWaitForOneByte() == '!')	
			return;
	}
}


#define MAXROWS	16	//max rows for one pan
#define MAXCOLS	14	// pan number
#define FULLCOLS 16 //max cols for
uchar keypos[FULLCOLS];
void sleepms(unsigned int i)
{
	while(i-- > 0)
		;
}
uchar bUpdate = 1;


char leds[17]; //led code, total 17 LEDS,
char out_10knob[14]; //data buffer for serial output

//translate the real led position to ideal led position.
//ideal led arrangement is:  for 10knob there is led postion 0-----10,
//						  :  for 4 knob there is led postion 11,12 for exp, 13,14,15,16 for val
uchar code disptbls[17] = { 11, 12, 13, 14, 15, 16, 6,7,8,9,10,0,1,2,3,4,5};

#define COL_SCALE		10
#define LEDVAL_POS		13
#define LEDEXP_POS		11

#define COL_10			11
#define COL_1			12
#define COL_P1			13

			            //   N001  N01  N1	 P1   P10    P100   P1K   P10K    P100K   P1M  Z_SCA	Z_10	Z_1		Z_P1
uchar code knobcol[MAXCOLS] = {0,    1,   2,   3,    4,     5,    6,     7,      8,     9,	10,		11,		12,		13};

//translate real col value to ideal col value
uchar code coltbls[16] = {1,    3,   5,   7,    9,   0xff,  13,    11,   12,    10, 0xff,    8,   6,    4,   2,  0};
//translate real row value to ideal row value
uchar code rowtbls[16] = {1,0,5,6,7,2,3,4,9,8,0xff,0xff,0xff,10,11,2};
//uchar code rowtbls[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//parse the column position of 1 in P0 and P1
uchar hitrow[FULLCOLS];
void getcols(uchar row)
{
	uchar pv0,pv1,pos;
	pv0 = P0;
	pv1 = P1;

	pos = 8;
	while(pos < 16)
	{
		if((pv1 & 0x01) == 0)
		{
			if((rowtbls[row] != 0xff) && (coltbls[pos] != 0xff)) 
			{
				hitrow[coltbls[pos]] = 1;
				if(keypos[coltbls[pos]] != rowtbls[row])
				{/*
					sjSerialSendByte(pos);
					sjSerialSendByte(keypos[coltbls[pos]]);
					sjSerialSendByte(row);
					sjSerialSendByte(rowtbls[row]);
					sjSerialSendByte('.');
					pause();
					*/
					bUpdate = 1;
				}
				keypos[coltbls[pos]] = rowtbls[row];
			}
		}
		pos++;
		pv1 = pv1 >> 1;
	}
	pos = 0;
	while(pos < 8)
	{
		if((pv0 & 0x01) == 0)
		{
			if((rowtbls[row] != 0xff) && (coltbls[pos] != 0xff)) 
			{
				hitrow[coltbls[pos]] = 1;
				if(keypos[coltbls[pos]] != rowtbls[row])
				{
				/*
					sjSerialSendByte(pos);
					sjSerialSendByte(keypos[coltbls[pos]]);
					sjSerialSendByte(row);
					sjSerialSendByte(rowtbls[row]);
					sjSerialSendByte('.');
					pause();
				*/
					bUpdate = 1;
				}
				keypos[coltbls[pos]] = rowtbls[row];
			}
		}
		pos++;
		pv0 = pv0 >> 1;
	}
		
}
//PF = P0,P1
//PI = P2, P4.0, P4.1, P3.2 - P3.7
sbit PI9 = P4^0;
sbit PI10 = P4^1;
sbit PI11 = P3^2;
sbit PI12 = P3^3;
sbit PI13 = P3^4;
sbit PI14 = P3^5;
sbit PI15 = P3^6;
sbit PI16 = P3^7;

void clearrows()
{
/*
	uchar i;
	for(i = 0; i < MAXROWS; i++) 
	{
		c164(1); //clear out all the output to 1	
	}
*/
	P2 = 0xff;
	PI9 = 1; 	
	PI10 = 1; 
	PI11 = 1; 
	PI12 = 1; 
	PI13 = 1; 
	PI14 = 1; 
	PI15 = 1; 
	PI16 = 1; 
}
void scanrows()
{
	uchar i = 0;
	for(i = 0; i < FULLCOLS; i++)
		hitrow[i] = 0xff;
	for(i = 0; i < MAXROWS; i++)
	{
	
	P2 = (i==0)?0x7f:0xff;
	P2 = (i==1)?0xBf:0xff;
	P2 = (i==2)?0xDf:0xff;
	P2 = (i==3)?0xEf:0xff;

	P2 = (i==4)?0xF7:0xff;
	P2 = (i==5)?0xFB:0xff;
	P2 = (i==6)?0xFD:0xff;
	P2 = (i==7)?0xFE:0xff;
	PI9 = (i==8)?0:1; 	
	PI10 = (i==9)?0:1; 
	PI11 = (i==10)?0:1; 
	PI12 = (i==11)?0:1; 
	PI13 = (i==12)?0:1; 
	PI14 = (i==13)?0:1; 
	PI15 = (i==14)?0:1; 
	PI16 = (i==15)?0:1; 

		//c164(i); 	//only 1 '0' will be shift out
		sleepms(100);
		getcols(i); //fill the column with current col value
	}
	clearrows();
	for(i = 0; i < FULLCOLS; i++)
	{
		if(hitrow[i] == 0xff)
		{
			if(keypos[i] != 0xff)
				bUpdate=1;
			keypos[i] = 0xff;
		}
	}

/*
	uchar i = 0;
	for(i = 0; i < FULLCOLS; i++)
		hitrow[i] = 0xff;
	for(i = 0; i < MAXROWS; i++)
	{
		c164(i); 	//only 1 '0' will be shift out
		sleepms(100);
		getcols(i); //fill the column with current col value
	}
	clearrows();
	for(i = 0; i < FULLCOLS; i++)
	{
		if(hitrow[i] == 0xff)
		{
			if(keypos[i] != 0xff)
				bUpdate=1;
			keypos[i] = 0xff;
		}
	}
*/
}
void clear_10knob()
{
	uchar p;
	for(p = 0;p < 11;p++)
	{
		leds[p] = LED_SPACE; //delight all the lines
		out_10knob[p] = '0';
	}
}
void update_10knob()
{
	uchar pre = 0; //进位
	uchar p,i;

	for(p = 0;p < 10;p++)
	{
		i = keypos[p] + pre;

		if(i == 0xff)
			break;
		if(p == 2)
			i = i+1; //100mohm start from 1
		if(i >= 10)
		{
			pre = 1;
			i = i-10;
		}else{
			pre = 0;
		}
		out_10knob[10-p] = i+'0';
		if(p != 3) //POINT POSITION
			leds[10-p] = led_code[i];
		else
			leds[10-p] = led_code[i] LEDPT_OP	LED_PT;	//set the point
	}
	if(pre == 1) //has pre
	{
		out_10knob[0] = '1';
		leds[0] = led_code[1];
	}else{
		out_10knob[0] = '0';
		leds[0] = LED_SPACE;
	}
	if(p < 10)	//invalid value
	{
		clear_10knob();
		return;
	}
}
void output_10knob()
{
	uchar p;
	uchar head = 0; //whether leading non-zero value is found
	for(p = 0;p < 11;p++)
	{
		if(out_10knob[p] !='0')
		{
			head = 1;
		}
		if((p < 7) && (head == 0)) //remove leading zero
			continue;
		sjSerialSendByte(out_10knob[p]);
		if(p == 7)
			sjSerialSendByte('.');
	}
}

uchar exp;
uchar val;
void clear_4knob()
{
		leds[LEDEXP_POS]	= LED_SPACE;
		leds[LEDEXP_POS+1]	= LED_SPACE;
		leds[LEDVAL_POS]	= LED_SPACE;
		leds[LEDVAL_POS+1]	= LED_SPACE;
		leds[LEDVAL_POS+2]	= LED_SPACE;
		leds[LEDVAL_POS+3]	= LED_SPACE;
		exp = 0;
		val = 0;

}
uchar out_4knob[10];
void update_4knob()
{
	uchar i;
	i = keypos[COL_SCALE];
	if(i == 11)	//OFF
	{
		val = 0xff;
		clear_4knob();
		return;
	}
	if(i == 0)
	{
		val = keypos[COL_P1];
		if(val >= 20) //invalid value
			return;
		leds[LEDEXP_POS] = LED_SPACE;
		leds[LEDEXP_POS+1] = LED_SPACE;
		out_4knob[0] = '0';
		out_4knob[1] = '0';
		leds[LEDVAL_POS] = LED_SPACE;
		leds[LEDVAL_POS+1] = LED_SPACE;
		if (val >= 10)
		{
			out_4knob[2] = '1';
			out_4knob[3] = '0' + val - 10;
			leds[LEDVAL_POS+2] = led_code[1] LEDPT_OP LED_PT;
			leds[LEDVAL_POS+3] = led_code[val-10] ;
		}else{
			out_4knob[2] = '0';
			out_4knob[3] = '0' + val;
			leds[LEDVAL_POS+2] = led_code[0] LEDPT_OP LED_PT;
			leds[LEDVAL_POS+3] = led_code[val];
		}
		return;
	}

	if(i == 1)
	{
		val = keypos[COL_1];
		if(val >= 20) //invalid value
			return;

		leds[LEDEXP_POS] = LED_SPACE;
		leds[LEDEXP_POS+1] = LED_SPACE;
		out_4knob[0] = '0';
		leds[LEDVAL_POS] = LED_SPACE;
		if (val >= 10)
		{
			out_4knob[1] = '1';
			out_4knob[2] = '0' + val-10;
			leds[LEDVAL_POS+1] = led_code[1];
			leds[LEDVAL_POS+2] = led_code[val-10] LEDPT_OP LED_PT;
		}else{
			out_4knob[1] = '0';
			out_4knob[2] = '0' + val;
			leds[LEDVAL_POS+1] = LED_SPACE;
			leds[LEDVAL_POS+2] = led_code[val] LEDPT_OP LED_PT;
		}
		out_4knob[3] = '0';
		leds[LEDVAL_POS+3] = led_code[0];
		return;
	}
	if(i == 2)
	{

		val = keypos[COL_10];
		if(val >= 20) //invalid value
			return;
		leds[LEDEXP_POS] = LED_SPACE;
		leds[LEDEXP_POS+1] = LED_SPACE;

		if (val >= 10)
		{
			out_4knob[0] = '1';
			out_4knob[1] = '0'+val-10;
			leds[LEDVAL_POS] = led_code[1] ;
			leds[LEDVAL_POS+1] = led_code[val-10] ;
		}else{
			out_4knob[0] = '0';
			out_4knob[1] = '0' + val;
			leds[LEDVAL_POS] = led_code[0];
			leds[LEDVAL_POS+1] = led_code[val];
		}
		out_4knob[2] = '0';
		out_4knob[3] = '0';
		leds[LEDVAL_POS+2] = led_code[0] LEDPT_OP LED_PT;
		leds[LEDVAL_POS+3] = led_code[0];
		return;
	}
	if(i > 10)
		return;
	if( (3 <= i) && (i <= 6) ) //10^-4 ..10^-1
	{
		leds[LEDEXP_POS] = LED_HF; //set to '-';
		exp = 7-i;
	}
	if((7 <= i) && (i <= 10) ) //10^0 ..10^3
	{
		leds[LEDEXP_POS] = LED_SPACE; //set to ' ';
		exp = i - 7;
	}
	leds[LEDVAL_POS]	= LED_SPACE;
	leds[LEDVAL_POS+1]	= LED_SPACE;
	leds[LEDVAL_POS+2]	= LED_SPACE;
	leds[LEDVAL_POS+3]	= LED_SPACE;
	leds[LEDEXP_POS + 1] = led_code[exp];
}
void output_4knob()
{
	uchar p = 0;
	uchar head = 0;
	if(val == 0xff) //off
		return;

	if(leds[LEDEXP_POS+1] == LED_SPACE)
	{
		for(p = 0;p < 4;p++)
		{
			if(out_4knob[p] != '0')
			{
				head = 1;
			}
			if( (p <2) && (head == 0)) //remove leading zero
				continue;
			sjSerialSendByte(out_4knob[p]);
			if(p == 2)
				sjSerialSendByte('.');
		}
		return;
	}else{
		sjSerialSendByte('1');		
		sjSerialSendByte('e');		
		if(leds[LEDEXP_POS] == LED_HF)
			sjSerialSendByte('-');		
		else
			sjSerialSendByte('+');
		sjSerialSendByte(exp+'0');		
	}
}

//display 17 led code in leds based on hardware setting
void led_show()
{
	uchar cnt;
	for(cnt = 0;cnt < 17;cnt++)
	{
		if(cnt == 6)
		{
			d164(LED_SPACE);d164(LED_SPACE);
		}
		if(cnt == 11)
		{
			d164(LED_SPACE);d164(LED_SPACE);d164(LED_SPACE);
		}
		d164(leds[disptbls[cnt]]);
	}
	d164(LED_SPACE);d164(LED_SPACE);
}



void main()
{
	
	unsigned char sid;//id of serial to send 
	uchar type;
	uchar cnt;
//	BYTE temp;
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
	
//	AD5791Init();
	
//	sjRelayInit();

	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/
	
	//DBGS("STARTUP DONE\r\n");	

	//test();
	//mDelaymS(5000);


	//CH374_PORT_INIT( );  /* CH374接口初始化 */


//	mDelaymS(50);
//	CH374_PORT_INIT( );  /* CH374接口初始化 */
//	Init374Device( );  // 初始化USB设备



	TR1 = 1;		//ENABLE COM1
	AUXR |= 0x10;	//Enable COM2

	PT0 = 1; 		//improve timer0 interrupt priority
    ET0 = 1;        //enable timer0 interrupt

	IE = 0x92;//enable serial int and timer0 interrupt//IE=90
	IE2 = 1;
	EA = 1;
//	DBGS("\r\nSTARTUP DONE ");
//	swiReset();
	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/
//	swiDelay(0x0f,0xff);
//	sjSerialSendByte('!');

	for(cnt = 0; cnt < MAXCOLS; cnt++) //reset column values
	{
		keypos[cnt] = 0xff;
//		c164(1); 
	}
	clearrows();	
	bUpdate = 1;

	/*for debug
	for(cnt = 0; cnt < 17; cnt++)
	{
		leds[cnt] = led_code[1];
	}
	led_show();
	while(1)
	{
	}
	*/
	while(1)
	{
		scanrows();

		//TESTING
/*		
		for(cnt = 0;cnt < 10;cnt++)
			d164(led_code[cnt]);

		for(cnt = 0;cnt < 10;cnt++)
			d164(led_code[cnt]  LEDPT_OP LED_PT);
		for(cnt = 0;cnt < 4;cnt++)
			d164(0xff >> cnt);
		pause();

		for(cnt = 0;cnt < MAXCOLS+2;cnt++)
		{
			if(keypos[cnt] >= 10)
				leds[cnt] = led_code[keypos[cnt]-10] LEDPT_OP LED_PT;
			else
				leds[cnt] = led_code[keypos[cnt]];
		}
		led_show();

		sleepms(2000);

		continue;
*/		
						
		type = 0;
		if(keypos[0] != 0xff)
		{
			type = 1;
			update_10knob();
		}else{
			clear_10knob();
		}
		if((keypos[COL_SCALE] != 0xff) && (keypos[COL_SCALE] != 11))
		{
			type = 2;
			update_4knob();
		}else{
			clear_4knob();
		}

		while(sjSerialIsDataWaiting() == TRUE)
		{
			if(sjSerialWaitForOneByte() == '?')
			{
				if(type == 0)
				{
					sjSerialSendByte('0');
				}
				if(type == 1)
				{
					output_10knob();
				}		
				if(type == 2)
				{
					output_4knob();
				}
				sjSerialSendByte(0x0D);
				sjSerialSendByte(0x0A);
			}
		}
		if(bUpdate == 1)
		{
			led_show();
			bUpdate = 0;
//				pause();
		}
	}
}



