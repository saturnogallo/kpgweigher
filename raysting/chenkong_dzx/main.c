#include "stdio.h"
#include "math.h"
#include "lib.h"


//u8 code led_code[]={0xdd,0x50,0x9b,0xd3,0x56,0xc7,0xcf,0xd0,0xdf,0xd7,0x00,0x02}; 
//u8 code led_code[]={0xdd,0x50,0x9b,0xd3,0x56,0xc7,0xcf,0xd0,0xdf,0xd7,0x00,0x02}; 
//u8 code led_code[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0x00,0x02}; 
u8 code led_code[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0xFF,0x7F}; 

u8 led_buffer[4];
u8 led_flash;
LED_INFO code ck_led = {
		4, 						//number of leds
		&led_flash,
		led_buffer,
		30,						//number of millisecond for flash
		8,						//stop the led flash after  flashed FLASH_COUNT times
		led_code,				//code list for the code translation
		0x7F,					//code for digit point
};

///////////////////////////////////////////////////
// settings of different leds - TYPEA
///////////////////////////////////////////////////
#define LEDA_DATA_PORT	P0
sbit ledA_0 = P2^3;
sbit ledA_1 = P2^2;
sbit ledA_2 = P2^1;
sbit ledA_3 = P2^0;
#define LED_HIGH	0
#define LED_LOW		1
void led_outputA(u8 pos,u8 dat)
{
	LEDA_DATA_PORT = led_code[LED_NOSHOW_INDEX];
	switch(pos)
	{
	case 0: ledA_3 = LED_HIGH;break;
	case 1: ledA_0 = LED_HIGH;break;
	case 2: ledA_1 = LED_HIGH;break;
	case 3: ledA_2 = LED_HIGH;break;
	default: break;
	}
	LEDA_DATA_PORT = dat;	
	switch(pos)
	{
	case 0: ledA_0 = LED_LOW;return;
	case 1: ledA_1 = LED_LOW;return;
	case 2: ledA_2 = LED_LOW;return;
	case 3: ledA_3 = LED_LOW;return;
	}
}
extern u16 pkbd_timer;

/*PWM1 related definition*/
#define PWM1_HIGH	1	//define 0 first or 1 first
#define PWM1_LOW	0
u8  pwm1_period = 6;	//period length of pwm
u8  pwm1_up     = 3;		//1 part length in period
u8  pwm1_tick   = 0;
sbit pwm1_pin 	= 	P2^7;

/*PWM2 related definition*/
#define PWM2_HIGH	1	//define 0 first or 1 first
#define PWM2_LOW	0
u8  pwm2_period = 6;	//period length of pwm
u8  pwm2_up     = 3;		//1 part length in period
u16 pwm2_tick   = 0;
sbit pwm2_pin = 	P2^6;

u16 motor1_degree = 0;
u16 motor2_degree = 0;
u16 motor1_target = 0;
u16 motor2_target = 0;

u8 gs =0;
sbit g_ou1	= P3^7;
sbit g_ou2  = P3^6;
#define PDEG	200
void timer_tick()	interrupt 1 using 1
{
	if(pkbd_timer > 0)
		pkbd_timer--;

	if(motor1_degree != motor1_target)
	{
		pwm1_tick++;
//		if(0x0080 == (motor1_degree & 0x0fff))
//			ledout_refresh(&ck_led);	
		if(pwm1_tick >= pwm1_period) //one cycle is done
		{
			motor1_degree++;
			pwm1_tick = 0;
		}else{
			if(pwm1_tick > pwm1_up)
				pwm1_pin = PWM1_LOW;
			else
				pwm1_pin = PWM1_HIGH;
		}
	}
	if(motor2_degree != motor2_target)
	{
		pwm2_tick++;
//		if(0x00ff == (motor2_degree & 0x0fff))
//			ledout_refresh(&ck_led);	

		if(pwm2_tick >= pwm2_period) //one cycle is done
		{
			motor2_degree++;
			pwm2_tick = 0;
		}else{
			if(pwm2_tick > pwm2_up)
				pwm2_pin = PWM2_LOW;
			else
				pwm2_pin = PWM2_HIGH;
		}
	}

	if((0 == g_ou1) && (motor1_degree > (10*PDEG)))
	{
		motor1_degree = 0;
		pwm1_tick =0;
	}

	if((0 == g_ou2) && (motor2_degree > (10*PDEG)))
	{
		motor2_degree = 0;
		pwm2_tick = 0;
	}

}

code char code *str[] = {"0015","0025","0035","0045","0055","0065","0075","0085","0095", //cu 0-8
						 "0100","0110","0120","0130","0140","0150","0160","0170","0180", //pt100 a 9-17
						 "0190","0200","0210","0220","0230","0240","0250",				 //pt100 b 18-24
						 "0800","0900","1000","1100","1200","1300","1400","1500","1600", //pt1k a 25-33
						 "1700","1800","1900","2000","2100","2200","2300","2400","2500"};//pt1k b 34-42

code u8 code m1_pos[] = {3,4,5,6,7,9,10,11,12,
						  12,1,3,4,5,6,7,9,10,11,12,1,3,4,5,6,
						  14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15};
code u8 code m2_pos[] = {1,1,1,1,1,1,1,1,1,
						  2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,
						  4,5,6,7,8,9,10,11,12,3,4,5,6,7,8,9,10,11};
code u8 code up_next[] = {1,2,3,4,5,6,7,8,0,	//cu
						  10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,9, //pt100
						  26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,25}; //pt1k

code u8 code dn_next[] = {8,0,1,2,3,4,5,6,7,	//cu
						  24,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23, //pt 100
						  42,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41}; //pt1k

extern u8 pkey_id;
sbit LCu = P1^6;
sbit LPt = P1^7;
sbit L1k = P3^4;
void main(void)
{
	u8 key;
	u8 cpos;
	//init variables
	HW1SerialInit();

	//set serial port parameter (clock 11.0592M)
	//9600 baut rate 8 data non parity and 1 stop.
	SCON = 0x70;
	PCON = 0x00;

	//timer count
	TH1 = 0xfd;
	TH0 = 0xad;
//	TL0 = 0xa0;
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
	

	if(g_ou1 == 1)
		key = 0;
	if(g_ou2 == 1)
		key = 0;

	//reset position of motor
	motor1_target = 0;
	motor1_degree = 1;
	while(motor1_degree != 0)
		;
	motor2_target = 0;
	motor2_degree = 1;
	while(motor2_degree != 0)
		;


	//move to reset position Cu15
	cpos = 0;

	motor1_target = ((u16)m1_pos[cpos])*PDEG;
	while(motor1_degree != motor1_target)
		;

	motor2_target = ((u16)m2_pos[cpos])*PDEG;
	while(motor2_degree != motor2_target)
		;
	LCu = 1;
	LPt = 0;
	L1k = 0;
	led_print(&ck_led,str[cpos],4);

	key = 0;
	while(1){
		key = PKeyboard_Update();
		if(key != 0)
		{
			switch(key)
			{
			case PKBD_CH_UP:
				key = 'u';break;
			case PKBD_CH_DN:
				key = 'd';break;
			case PKBD_CH_CU:
				key = 'c';break;
			case PKBD_CH_PT:
				key = 'p';break;
			case PKBD_CH_1K:
				key = 'k';break;
			default:
				key = 0; break;
			}
		}else{
			if(HW1SerialDataWaiting())
			{
				key = HW1SerialWaitForOneByte();
			}
		}
		
		if(key == 0)
		{
			ledout_refresh(&ck_led);
			continue;
		}else{
			switch(key)
			{
			case 'u':
				cpos = up_next[cpos];break;
			case 'd':
				cpos = dn_next[cpos];;break;
			case 'c':
				cpos = 0;break;
			case 'p':
				cpos = 9;break;
			case 'k':
				cpos = 25;break;
			default:
				key = 0;break;
			}
			if(key == 0)
				continue;
			motor1_target = m1_pos[cpos]*PDEG;
			motor2_target = m2_pos[cpos]*PDEG;
			led_print(&ck_led,str[cpos],4);
			LCu = 0;	LPt = 0;	L1k = 0;
			if(cpos < 9)
				LCu = 1;
			else if (cpos < 25)
				LPt = 1;
			else
				L1k = 1;
			key = 0;
		}

	}
}