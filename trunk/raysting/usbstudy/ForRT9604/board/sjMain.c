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
//	asp_handler();
//	asp_handler2();
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
//#define MAXCOLS	16	// pan number
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
uchar code disptbls[17] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

#define COL_SCALE		10
#define LEDVAL_POS		13
#define LEDEXP_POS		11

#define COL_100			5
#define COL_10			4
#define COL_1			3
#define COL_P1			2
#define COL_P01			1
#define COL_P001		0

			            //   N001  N01  N1	 P1   P10    P100   P1K   P10K    P100K   P1M  Z_SCA	Z_10	Z_1		Z_P1
uchar code knobcol[MAXCOLS] = {0,    1,   2,   3,    4,     5,    6,     7,      8,     9,	10,		11,		12,		13};

//translate real col value to ideal col value
//uchar code coltbls[16] = {1,    3,   5,   7,    9,   0xff,  13,    11,   12,    10, 0xff,    8,   6,    4,   2,  0};
uchar code coltbls[16] =   {1,    3,   5,   7,    9,    11,   13,  0xff,     0xff, 12, 10,   8,    6, 4,   2,  0};
//uchar code coltbls[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//translate real row value to ideal row value
//uchar code rowtbls[16] = {1,0,5,6,7,2,3,4,9,8,0xff,0xff,0xff,10,11,2};
uchar code rowtbls[16] = {0xff,0xff,11,9,7,5,3,1,0xff,12,10,8,6,4,2,0};
//uchar code rowtbls[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//parse the column position of 1 in P0 and P1
uchar hitrow[FULLCOLS];
//key value (0-11 are row values)
void getcols(uchar row)
{
	uchar pv0,pv1,pos;
	pv0 = P0;
	pv1 = P1;

	/*
	sjSerialSendByte(rowtbls[row]);
	sjSerialSendByte(pv0);
	sjSerialSendByte(pv1);
	sjSerialSendByte('.');
	pause();
	*/
	pos = 8;
	while(pos < 16)
	{
		if((pv1 & 0x01) == 0)
		{
			if((rowtbls[row] != 0xff) && (coltbls[pos] != 0xff)) //row is valid and col is valid
			{
				hitrow[coltbls[pos]] = 1;
				if(keypos[coltbls[pos]] != rowtbls[row]) //rowtbls[row] is the actual key number
				{
/*
					sjSerialSendByte(pos);
					sjSerialSendByte(coltbls[pos]);
					sjSerialSendByte(keypos[coltbls[pos]]);
					sjSerialSendByte(row);
					sjSerialSendByte(rowtbls[row]);
					sjSerialSendByte('.');
*/
//					pause();
					
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
					sjSerialSendByte(coltbls[pos]);
					sjSerialSendByte(keypos[coltbls[pos]]);
					sjSerialSendByte(row);
					sjSerialSendByte(rowtbls[row]);
					sjSerialSendByte('.');
*/
//					pause();
				
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

sbit PI1 = P2^0;
sbit PI2 = P2^1;
sbit PI3 = P2^2;
sbit PI4 = P2^3;
sbit PI5 = P2^4;
sbit PI6 = P2^5;
sbit PI7 = P2^6;
sbit PI8 = P2^7;


void clearrows()
{
/*
	uchar i;
	for(i = 0; i < MAXROWS; i++) 
	{
		c164(1); //clear out all the output to 1	
	}
*/
	PI1 = 1; 	
	PI2 = 1;
	PI3 = 1;
	PI4 = 1;
	PI5 = 1;
	PI6 = 1;
	PI7 = 1; 	 	 	 	 	 	
	PI8 = 1; 	
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
	
	PI1 = (i==0)?0:1;
	PI2 = (i==1)?0:1;
	PI3 = (i==2)?0:1;
	PI4 = (i==3)?0:1;

	PI5 = (i==4)?0:1;
	PI6 = (i==5)?0:1;
	PI7 = (i==6)?0:1;
	PI8 = (i==7)?0:1;
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
		getcols(i); //fill the column with current row value
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
	for(p = 0;p < 17;p++)
	{
		leds[p] = LED_SPACE; //delight all the lines
		out_10knob[p] = '0';
	}
}
void update_10knob()
{
	uchar pre = 0; //进位
	uchar head;
	uchar p,i;

	leds[11] = led_code[0];// LED_SPACE;
	for(p = 0;p < 10;p++)
	{
		i = keypos[p] + pre;

		if(i == 0xff)
			break;
//		if(p == 2)		i = i+1; //100mohm start from 1
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
		leds[0] = led_code[0];//LED_SPACE;
	}
	if(p < 10)	//invalid value
	{
		clear_10knob();
		return;
	}

	/*check head
	head = 0;
	for(p = 0;p < 7;p++)
	{
		if((leds[p] != led_code[0])&&(leds[p] != LED_SPACE))
		{
			break;
		}
		leds[p] = LED_SPACE;
	}
	*/
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

//uchar exp;
//uchar val;
void clear_4knob()
{
	uchar j;
	for(j = 0; j < 17; j++)
	{
		leds[j] = LED_SPACE;
	}		

//	exp = 0;
//	val = 0;

}
uchar out_4knob[10];
uchar code2key(uchar c)
{
	uchar m;
	for(m=0;m<10;m++)
	{
		if(led_code[m] == c)
			return m;
	}
	return 0xff;
}

void update_4knob()
{
	uchar i,j,val;
	i = keypos[COL_SCALE];

	if(i == 6)	//OFF
		return;
	clear_4knob();
	if( i >= 7 ) //10^0 ..10^-4
	{
		leds[7] = (i==7)?  led_code[1]:led_code[0];
		leds[8] = (i==8)?  led_code[1]:led_code[0];
		leds[9] = (i==9)?  led_code[1]:led_code[0];
		leds[10]= (i==10)? led_code[1]:led_code[0];
		leds[11]= (i==11)? led_code[1]:led_code[0];
	}else{
		leds[7] = led_code[0];
		leds[8] = led_code[0];
		leds[9] = led_code[0];
		leds[10] = led_code[0];
		leds[11] = led_code[0];
		if((0 <= i) && (i <= 5) ) //11^-3 to 11x10^2
		{
			for(j = (11-i); j < 12; j++)
			{
				leds[j] = led_code[0];
			}		
			val = keypos[i]; //0-3,1-
			if(i < 4)
				leds[9-i] = (val >= 10) ? led_code[1]:led_code[0];
			else
				leds[9-i] = (val >= 10) ? led_code[1]:LED_SPACE;
			if(val >= 10)
			{
				val = val - 10;
			}
			if(val < 10)
				leds[10-i] = led_code[val];
		}
	}
	leds[7] = leds[7] LEDPT_OP	LED_PT;
}
void output_4knob()
{
	uchar p,v;
	uchar head = 0; //whether leading non-zero value is found
	for(p = 0;p < 12;p++)
	{
		v = code2key(leds[p] & 0x7f);
		if(v !=0 && v != 0xff)
		{
			head = 1;
		}
		if((p < 7) && (head == 0)) //remove leading zero
			continue;

		if(v != 0xff)
			sjSerialSendByte('0'+v);
		if(p == 7)
			sjSerialSendByte('.');
	}
}
/*
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
*/
//display 17 led code in leds based on hardware setting
void led_show()
{
	uchar cnt;
	/*
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

	*/
	for(cnt = 0;cnt < 12;cnt++)
	{
			d164(leds[cnt]);
	}
	//d164(LED_SPACE);//d164(LED_SPACE);
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
//	P_SW1 = 0x08  ; // 0 0  0 0  1 0   0  0
	P_SW1 = 0x3C ;  // 0 0  1 1  1 1   0  0
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
//	init_uart();  //fake serial port 3
//	init_uart2(); //fake serial port 4

//	asp_set_timer();


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
						
		type = 0; //type = 0
		
		if((keypos[0] != 0xff) && (keypos[COL_SCALE] == 6))
		{
			type = 1;
			update_10knob();
		}else{
			if((keypos[COL_SCALE] != 0xff) && (keypos[COL_SCALE] != 6))
			{
				type = 2;
				update_4knob();
			}else{
				clear_10knob();
			}
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



