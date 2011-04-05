#include "stc51.h"
#include "stdio.h"
#include "intrins.h"
#define uchar unsigned char
#define BYTE unsigned char



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

uchar code led_code[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xff}; //LED显示

/*
					  0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0x03,0x62,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                      0xfd,0x61,0xdB,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
*/
#define LED_HF		0x02
#define LED_SPACE	0x00
#define LED_PT		0x01
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
	uchar type;
	uchar cnt;
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

	for(cnt = 0; cnt < MAXCOLS; cnt++) //reset column values
	{
		keypos[cnt] = 0xff;
		c164(1); 
	}
	clearrows();	
	bUpdate = 1;
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
