/*
*/
#include "stc51.h"
#include "utili.h"
#include "lcd.h"
#include "key.h"
#include "window.h"
#include "math.h"
#include "font.h"
uchar key;
sbit sktt = P3^7;
sbit srs1 = P3^5;
sbit srs2 = P3^4;
void analog_timer()	interrupt 1
{
	Key_Scan();
}
void sleepms(unsigned int i)
{
	while(i-- > 0)
		;
		
}
void initiate_timer(void)
{
   //set serial port parameter (clock 11.0592M)
   //9600 baut rate 8 data non parity and 1 stop.
   SCON = 0x70;
   PCON = 0x80;//0x00 = 9600, 80 = 57600;

   //use timer 1 to be serial
   //timer count
   TH1 = 0xFF;//0xfD = 9600,0xff = 57600;

   //use timer 0 to be heart beat
   TH0 = -4608/256;   // 4608 , 4608/(11.0592M/12) = 0.005s = 5ms
   TL0 = -4608%256;


   TMOD = 0x21;
   TR1 = 1;

   ET0=1;
   EA=1;
   TR0 = 1;
}

/*
	Command protocal related definition
 */
#define SPIDLE	0
#define SPBUSY	1
uchar 	spSFlag=SPIDLE;
#define write(A)	SBUF=A; while(spSFlag==SPBUSY);

#define LOOP_BUF_MAX	80
uchar 	inbuf[LOOP_BUF_MAX];
uchar 	inwrite_pos = 0;  //pointer for storing SBUF
uchar	in_leds = KEY_INVALID;

#define MARK_HEAD        0xAA
#define MARK_TAIL        0x55
#define MARK_IDLE        0x66

#define STATUS_IDLE      0xF0
#define STATUS_ERR       0xF1
#define STATUS_DRAW      0xF2
#define STATUS_DONE      0xF3
       
#define CMDO_DISPLAYBTN  0x02
#define CMDO_LCD_INIT    0x03
#define CMDO_LCD_CLS     0x04
#define CMDO_LCD_REVERSE 0x05
#define CMDO_LCD_CLSBLK  0x06
#define CMDO_LCD_RECT    0x07
#define CMDO_LCD_HZ12    0x08
#define CMDO_LCD_HZ16    0x09
#define CMDO_LCD_68      0x10
#define CMDO_LCD_816     0x11
#define CMDO_TOIDLE      0x12

uchar 	state = STATUS_IDLE;
uchar 	lastc;
void SerialHandler(void) interrupt 4 using 2
{
	if(TI)	//just handle serial interrupt 1
	{
		TI = 0;
		spSFlag = SPIDLE;
	}
	if(RI)
	{
		lastc = SBUF;
		if(lastc == MARK_HEAD)
		{
			inwrite_pos = 0;
			state = STATUS_IDLE;
		}else{
	 		if(lastc == MARK_TAIL)
			{
				if(state == STATUS_IDLE)
				{
				 	state = STATUS_DRAW;
				}
			}else{
				if(inwrite_pos < LOOP_BUF_MAX)
					inbuf[inwrite_pos++] = lastc;
			}
		}
		RI = 0;
	}
}
unsigned long timeout;
void parse_cmd()
{
	uchar len;
	uchar i;
	uchar sum;
	uchar l,h;

	if(state == STATUS_DRAW)	//completion of a command
	{
		write(STATUS_DRAW);
		len = inwrite_pos;
		sum = 0;
		for(i=0;i<(len);i = i+2)
		{
			if(inbuf[i] <= '9')
				h = inbuf[i] - '0';
			else
				h = inbuf[i] - 'A' + 0x0A;			

			if(inbuf[i+1] <= '9')
				l = inbuf[i+1] - '0';
			else
				l = inbuf[i+1] - 'A' + 0x0A;			

			inbuf[i>>1] = ((h<<4) | l);
			sum = sum + inbuf[i>>1];
		}
		if(sum != 0xff)	//check sum failure
		{
			state = STATUS_ERR;
			write(STATUS_ERR);
			return;
		}

		switch(inbuf[0])
		{
		case CMDO_TOIDLE:
			state = STATUS_IDLE;
			write(STATUS_IDLE);
			return;
		case CMDO_DISPLAYBTN:
			in_leds = inbuf[1];
			break;
		case CMDO_LCD_INIT:
			LCD_Init();
			break;
		case CMDO_LCD_CLS:
			LCD_Cls();
			break;
		case CMDO_LCD_REVERSE:
			LCD_ReverseRect(inbuf[1],inbuf[2],inbuf[3],inbuf[4]);
			break;
		case CMDO_LCD_CLSBLK:
			LCD_ClsBlock(inbuf[1],inbuf[2],inbuf[3],inbuf[4]);
			break;
		case CMDO_LCD_RECT:
			LCD_Rectange(inbuf[1],inbuf[2],inbuf[3],inbuf[4]);
			break;
		case CMDO_LCD_HZ12:
			LCD_PrintHz12(inbuf[1],inbuf[2],inbuf+4);
			break;
		case CMDO_LCD_HZ16:
			LCD_PrintHz16(inbuf[1],inbuf[2],inbuf+4);
			break;
		case CMDO_LCD_68:
			LCD_Print6X8(inbuf[1],inbuf[2],inbuf+4);
			break;
		case CMDO_LCD_816:
			LCD_Print8X16(inbuf[1],inbuf[2],inbuf+4);
			break;
		default:
			break;
		}
		state = STATUS_IDLE;
		write(STATUS_IDLE);
		timeout = 0;
		return;
	}
}
void main()
{

	 IE = 0;//close int
	 /* Initiate timer */
	 initiate_timer();

 	 Key_Init();

	 inwrite_pos = 0;
	 state = STATUS_IDLE;
	 IE = 0x92;//enable serial int and timer0 interrupt

	 key = KEY_INVALID;
	 timeout = 0;

	 while(1)
	 {
	 	parse_cmd();

		if(timeout++ > 20000)
		{
			write(state);
			timeout = 0;
		}
		if(key != KEY_INVALID)
		{
			write(key);
			key = KEY_INVALID;
	  	}
		if(in_leds != KEY_INVALID)
		{
			if(in_leds == KEY_BTN1)
				display_buttons(KEY_BTN1,1);
			if(in_leds == KEY_BTN2)
				display_buttons(KEY_BTN2,1);
			if(in_leds == KEY_BTN3)
				display_buttons(KEY_BTN3,1);
			if(in_leds == KEY_BTN4)
				display_buttons(KEY_BTN4,1);

			if(in_leds == KEY_KTT)//PKTT
			{
				sktt = 1;				
			}
			if(in_leds == CLR_KTT)//NKTT
			{
				sktt = 0;
			}
			if(in_leds == KEY_RS)//RS
			{
				srs1 = 0;
				srs2 = 0;
				srs1 = 1;
				sleepms(10000);
				srs1 = 0;
			}
			if(in_leds == CLR_RS)//RX
			{
				srs1 = 0;
				srs2 = 0;
				srs2 = 1;
				sleepms(10000);
				srs2 = 0;
			}
						
			if(in_leds == CLR_BTN1)
				display_buttons(KEY_BTN1,0);
			if(in_leds == CLR_BTN2)
				display_buttons(KEY_BTN2,0);
			if(in_leds == CLR_BTN3)
				display_buttons(KEY_BTN3,0);
			if(in_leds == CLR_BTN4)
				display_buttons(KEY_BTN4,0);

			in_leds = KEY_INVALID;
		}
	}	
}




