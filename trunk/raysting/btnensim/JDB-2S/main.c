#include "stc51.h"
#include "stdio.h"
#include "intrins.h"
#define uchar unsigned char
#define BYTE unsigned char

/*
For JDB-2S use
2014-12-04 initial version
*/
//the MARK of header ender and error and quest and reset
#define  HMARK 0x55	//U
#define  EMARK 0x56 //V
#define  QMARK 0x57 //W
#define  RMARK 0x58 //X


#define SP_BUFFER_MAX	12
#define BOOL	unsigned char
#define TRUE	1
#define FALSE	0

BYTE spbuf[SP_BUFFER_MAX];	///< serial port fifo buffer
static BYTE *spHead,*spTail;	///< buffer head and tailst
static BYTE spSFlag;///<serial port flag for send use

//const for SerialPort Status
#define SPBUSY 	0xd6	
#define SPIDLE 	0xd8


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

/**
*	@return TRUE when data availabel ,FALSE while not
*/
BOOL sjSerialIsDataWaiting()
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
		;
	udata = *spHead;
	spHead = spHead+1;
	if(spHead == (spbuf+SP_BUFFER_MAX))
		spHead = spbuf;
	return udata;
}
//0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xff
uchar code led_code[]={0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0xFF};//LED显示
					  
					 //0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0xFF,0x7F

/*
					  0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0x03,0x62,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0xfd,0x61,0xdB,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
*/
#define LED_HF		0x08
#define LED_SPACE	0x00
#define LED_PT		0x80
#define LEDPT_OP	|

//bits for LED display
sbit dat1 = P2^2;
sbit clk1 = P2^1;
sbit rst1 = P2^0;

void d164(uchar c)   //74164串行移位一个数
{
    uchar i=0;
     do{  
        if(c&0x01)     
           	dat1=1;
        else
			dat1=0;
		c>>=1;        //数组tabel中的第N个数向右移动一位
		_nop_(); _nop_();
        clk1=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
        _nop_(); _nop_();
		clk1=1;
		_nop_(); _nop_();
        clk1=0;
		_nop_(); _nop_();
        i++;
    }
    while(i<8);
}

//bits for row output
sbit dat2 = P2^5;
sbit clk2 = P2^4;
sbit rst2 = P2^3;

void c164(uchar c)   //74164串行移位一个bit
{
        if(c)     
           	dat2=1;
        else
			dat2=0;
		_nop_(); _nop_();
        clk2=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
        _nop_(); _nop_();
		clk2=1;
		_nop_(); _nop_();
        clk2=0;
		_nop_(); _nop_();
}
void pause()
{
	while(1)
	{
		if(sjSerialWaitForOneByte() == '!')	
			return;
	}
}


#define MAXROWS		16	//total rows for one pan

#define POINTPOS	4	//position of point
#define FULLCOLS 	11 	//total pans
#define SCALECOLS	10	//total scale pan number
#define TYPECOL		0	//index of type pan 11th

#define TOTALLEDS	12	//total leds
#define MAXRETRY	10  //max retry times
uchar keypos[FULLCOLS];
uchar wait;	//wait times count
void sleepms(unsigned int i)
{
	while(i-- > 0)
		;
}
uchar bUpdate = 1;



char leds[TOTALLEDS]; //led code, total 12 LEDS, 0 is for highest bit
char out_12knob[TOTALLEDS]; //data buffer for serial output
char cmp_leds[TOTALLEDS]; //buffer to compare the new data to old data

			            //   N001  N01  N1	 P1   P10    P100   P1K   P10K    P100K   P1M  Z_SCA	Z_10	Z_1		Z_P1
//uchar code knobcol[MAXCOLS] = {0,    1,   2,   3,    4,     5,    6,     7,      8,     9,	10,		11,		12,		13};

//translate real col value to ideal col value //real col index to pan index
uchar code coltbls[16] = {1,3,5,7,9,0xff,0xff,0xff,0,0xff,0xff,10,8,6,4,2};
//uchar code coltbls[16] = {1,    3,   5,   7,    9,   0xff,  13,    11,   12,    10, 0xff,    8,   6,    4,   2,  0};
//translate real row value to ideal row value //real row index to num index
//uchar code rowtbls[16] = {1,0,5,6,7,2,3,4,9,8,0xff,0xff,0xff,10,11,2};
//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
uchar code rowtbls[16] = {9,10,5,4,3,8,7,6,1,2,0xff,0xff,0xff,0,11};
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
				if((keypos[coltbls[pos]] != rowtbls[row]) || (wait < MAXRETRY))
				{
/*debug only
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
				if((keypos[coltbls[pos]] != rowtbls[row]) || (wait < MAXRETRY))
				{
/*debug only
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
void clearrows()
{
	uchar i;
	for(i = 0; i < MAXROWS; i++) 
	{
		c164(1); //clear out all the output to 1	
	}
}

void scanrows()
{
	uchar i = 0;

	for(i = 0; i < FULLCOLS; i++)
		hitrow[i] = 0xff;
//		sjSerialSendByte(0xaa);
	for(i = 0; i < MAXROWS; i++)
	{
		c164(i); 	//only 1 '0' will be shift out
		sleepms(100);

		getcols(i); //fill the column with current col value
//		sjSerialSendByte(P0);
//		sjSerialSendByte(P1);
//		if((P0 != 0xff) && (P1 != 0xff))
//			pause();

	}
//			sjSerialSendByte(0x55);
//	pause();
	clearrows();
	
	for(i = 0; i < FULLCOLS; i++)
	{
		if(hitrow[i] == 0xff) //no number found for one col
		{
//			if(keypos[i] != 0xff)
//				bUpdate = 1;	
			keypos[i] = 0xff;
		}
	}
}
//clear leds and out
void clear_12knob()
{
	uchar p;
	for(p = 0;p < TOTALLEDS;p++)
	{
		leds[p] = LED_SPACE; //delight all the lines
		out_12knob[p] = '0';
	}
}
//set le
void update_12knob()
{
	uchar pre = 0; //进位
	uchar p,i;

    leds[FULLCOLS] = led_code[0];
	out_12knob[FULLCOLS] = '0';
	for(p = 1;p < SCALECOLS;p++)
	{
		i = keypos[p] + pre;
		if(i == 0xff)
			break;
		if (p == 4) //0.01 ohm starting from 1
			i = i+1;
		if(i >= 10)
		{
			if (p == 9)
			{
				if(keypos[p] == 11)
					i = i+8;
			}
			pre = i / 10;
			i = i % 10;
		}else{
			pre = 0;
		}
		out_12knob[SCALECOLS-p] = i+'0';
		leds[SCALECOLS-p] = led_code[i];
	}
	leds[POINTPOS] = leds[POINTPOS] LEDPT_OP LED_PT;	//set the point
	if(pre > 0 ) //has pre
	{
		out_12knob[0] = pre+'0';
		leds[0] = led_code[pre];
	}else{
		out_12knob[0] = '0';
		leds[0] = led_code[0];
	}
	if(p < SCALECOLS)	//invalid value
	{
		clear_12knob();
		return;
	}
}
void output_12knob()
{
	uchar p;
	uchar head = 0; //whether leading non-zero value is found
	
	for(p = 0;p < (TOTALLEDS);p++)
	{
		if(out_12knob[p] !='0')
		{
			head = 1;
		}
		if((p < POINTPOS) && (head == 0)) //remove leading zero
			continue;
		sjSerialSendByte(out_12knob[p]);
		if(p == POINTPOS)
			sjSerialSendByte('.');
	}
}


//display 17 led code in leds based on hardware setting
void led_show()
{
	uchar cnt;
	for(cnt = 0;cnt < TOTALLEDS;cnt++)
	{
		d164(LED_SPACE);
	}
	for(cnt = 0;cnt < TOTALLEDS;cnt++)
	{
		d164(leds[cnt]);
	}
}
#define ILEDVAL_POS  8	//where fixed value display starts
void update_fixknob(char i)
{
	leds[ILEDVAL_POS] = LED_SPACE;
	leds[ILEDVAL_POS+1] = LED_SPACE;
	leds[ILEDVAL_POS+2] = LED_SPACE;
	leds[ILEDVAL_POS+3] = LED_SPACE;
	if(i == 0)
		{
			leds[ILEDVAL_POS+2] = led_code[0] LEDPT_OP LED_PT;
			leds[ILEDVAL_POS+3] = led_code[0];
		}
		if(i == 1)
		{
			leds[ILEDVAL_POS+2] = led_code[0] LEDPT_OP LED_PT;
			leds[ILEDVAL_POS+3] = led_code[5];
		}
		if(i == 2)
		{
			leds[ILEDVAL_POS+2] = led_code[1] LEDPT_OP LED_PT;
			leds[ILEDVAL_POS+3] = led_code[0];
		}
		if(i == 3)
		{
			leds[ILEDVAL_POS+2] = led_code[2] LEDPT_OP LED_PT;
			leds[ILEDVAL_POS+3] = led_code[0];
		}
		if(i == 4)
		{
			leds[ILEDVAL_POS+2] = led_code[5] LEDPT_OP LED_PT;
			leds[ILEDVAL_POS+3] = led_code[0];
		}
}
void main()
{
	uchar type;
	uchar cnt;
	uchar temp;
	uchar sid;//id of serial to send 

	//serial port related init operation
	IE = 0;//close int
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
	sjSerialInit();
	IE = 0x90;//enable serial int and timer0 interrupt//IE=90

	for(cnt = 0; cnt < FULLCOLS; cnt++) //reset column values
	{
		keypos[cnt] = 0xff;
		c164(1); 
	}
	clearrows();	
	bUpdate = 1;
	wait = 0;
	//debug only
	//update_fixknob(6);
	//led_show();
	//while(1){}
	
	while(1)
	{
		scanrows();


		update_12knob();

		type = keypos[TYPECOL];
		update_fixknob(type);

		if(bUpdate == 1)
		{
		/*
			if(wait == 0) 
			{	//copy leds to cmp_leds
				for(cnt = 0;cnt < TOTALLEDS; cnt++)
					cmp_leds[cnt] = leds[cnt];
				wait = 1;
				continue;
			}else{
				for(cnt = 0;cnt < TOTALLEDS; cnt++)
				{
					if(cmp_leds[cnt] != leds[cnt])
						break;
				}				
				if(cnt < TOTALLEDS)
				{
					wait = 0;
					continue;
				}
				if(wait == MAXRETRY)
				{
				}
				if(wait < MAXRETRY)	//retry 10 time before update
				{
					wait++;
					continue;
				}
			}*/
			wait = MAXRETRY;
			led_show();

			bUpdate = 0;
		}


		while(sjSerialIsDataWaiting() == TRUE)
		{
				temp = sjSerialWaitForOneByte();
				if(temp == ':')  //incase :fetch?
				{
					sid = sjSerialWaitForOneByte();
					continue;
				}
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
								continue;
							if(temp == QMARK)
								continue;
						}
						sjSerialSendByte(HMARK);
						temp = 0xff;
						sjSerialSendByte(temp);
						temp = 0xff;
						sjSerialSendByte(temp);
						sjSerialSendByte(EMARK);
					}
				}
				if(sid == 'n')//send to navmeter
				{
					if(temp == '?')
					{
						output_12knob();
//						sjSerialSendByte(0x0D);
						sjSerialSendByte(0x0A);
					}
				}
				if(sid == 'f') //incase of :fetch?
				{
					if(temp == '?')
					{
						output_12knob();
						sjSerialSendByte(0x0D);
						sjSerialSendByte(0x0A);
						sid = ' ';
					}
				}
		}		
	}
}
