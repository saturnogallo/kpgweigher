#include "sjDefine.h"
#include "sjSerial.h"
//#include "AD5791.h"
//#include "CH374INC.h"
#include "HAL.h"
#include "stdio.h"
#include "stdlib.h"

#define uchar	unsigned char
#define u8	unsigned char
#define ulong	unsigned long
#define uint	unsigned int
unsigned char delayc;

//#define CMD_DBG		0xc0
//#define CMD_DBG2		0xc3
#define CMD_READ	0xc1
#define CMD_ZERO	0xc2 //zero offset of navmeter
/*
*/
//end interface routine
extern int	test( void );
extern void DBGS(const char* s);
extern void DBG(unsigned char);

void delay (uint us) 
{
  	while(us--);
}
/*
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;
		
}
*/
char lowc(uchar x)
{
	x = x&0x0f;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
char highc(uchar x)
{

	x = x>>4;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}

/*计算字符串长度*/
uchar strlen(uchar *s)
{
	uchar len = 0;
	while(*s++) len ++;
	
	return len;
}



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

uchar  xdata da[4];
void ComOutStr(ulong dT)
{
  uchar xyw;
  da[3] =  dT&0xff; 
  da[2] =  dT>>8&0xff;
  da[1] =  dT>>16&0xff;
  da[0] =  dT>>24&0xff; 
  xyw=(da[0]+da[1]+da[2]+da[3]) & 0xff;	
  xyw = ~xyw;
  
  sjSerialSendByte(da[0]);
  sjSerialSendByte(da[1]);
  sjSerialSendByte(da[2]);
  sjSerialSendByte(da[3]);
  xyw=da[0]+da[1]+da[2]+da[3];	
  xyw = ~xyw;
  sjSerialSendByte(xyw);
}

static uchar navlen1 = 0;
static uchar navlen2 = 0;
static double reading1 = -1000;
static double reading2 = -1000;
static double reading1r = -1000;
static double reading2r = -1000;

static char navread1[20];
static char navread1b[20];
static char navread2[20];
static char navread2b[20];

static uchar navcmd[12];                                                                  
#define NAV_INVALID     -1000
#define NAV_VALID       -2000

#define NAV_INIT        1
#define NAV_30V         2
#define NAV_1V          3
#define NAV_120MV       4
#define NAV_AFLTOFF     5
#define NAV_AFLTON      6
#define NAV_SLOWMODE    7
#define NAV_ZEROON      8
#define NAV_READ        9

void ResetNav(char port)
{
	if(port == '1')
	{
		navlen1 = 0;   reading1 = NAV_INVALID;  
	}
	if(port == '2')
	{
		navlen2 = 0;   reading2 = NAV_INVALID;  
	}
}
void prints(u8 *str, u8 length, char uart_port)
{
	if(uart_port == '0')
	{
	           while(length-- > 0){  
//  		   		sjSerialSendByte(*str);
                sjSerialSendByte(*str++);            
				}
	}
	if(uart_port == '1')
	{
//	  		   		sjSerialSendByte('1');
           while(length-- > 0){  
//  		   		sjSerialSendByte(*str);
                io_putc(*str++);            
           }
	}
	if(uart_port == '2')
	{
//		   		    sjSerialSendByte('2');
           while(length-- > 0){

//   		   		sjSerialSendByte(*str);
                io_putc2(*str++);            
           }
	}
}
void nav_command(uchar cmd, char port)
{
//        if(cmd == NAV_30V)
//             sprintf(navcmd,"%%01;12;02\r");
        if(cmd == NAV_1V)
               sprintf(navcmd,"%%01;12;02\r\n");
        if(cmd == NAV_120MV)
               sprintf(navcmd,"%%01;12;01\r\n");
        if(cmd == NAV_AFLTOFF)
               sprintf(navcmd,"%%01;26\r");
        if(cmd == NAV_AFLTON)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_SLOWMODE)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_ZEROON)    
        {
               sprintf(navcmd,"%%01;06\r\n");
        }
        if(cmd == NAV_INIT) 
        {           
                ResetNav(port);
                sprintf(navcmd,"%%01;00\r\n");        
        }
        if(cmd == NAV_READ)
        {       
                ResetNav(port);
                sprintf(navcmd,"%%01;01\r");
        }                       
        prints(navcmd,strlen(navcmd),port);
}

unsigned long navtime;
double nav_read1()
{                       
        nav_command(NAV_READ,'1');
        navtime = 0;        
        while(1)
        {
                if(reading1 < NAV_INVALID) //make sure it is a valid reading
                {              
                     reading1 = atof(navread1b);
                     return reading1;
                }       
                if(navtime++ > 655350)
                {          
                     nav_command(NAV_READ,'1');   
                     navtime = 0;
                }                
                delay(10);
        }
        return 1.0;
}
double nav_read2()
{                       
        nav_command(NAV_READ,'2');
        navtime = 0;        
        while(1)
        {
                if(reading2 < NAV_INVALID) //make sure it is a valid reading
                {              
                     reading2 = atof(navread2b);
                     return reading2;
                }       
                if(navtime++ > 655350)
                {          
                     nav_command(NAV_READ,'2');   
                     navtime = 0;
                }                
                delay(10);
        }
        return 1.0;
}
//incoming data hander of navameter
void nav_uart_push1(uchar dat)
{
//		sjSerialSendByte(dat);                            
//		return;
        if(navlen1 >= 19)
        {                          
                ResetNav('1');
        }
        if(((dat >= '0') && (dat <= '9')) ||
                (dat == '.') ||
                (dat == '-') ||
                (dat == '+') ||
                (dat == 'e') ||
                (dat == 'E') )
        {                    
                navread1[navlen1++] = dat;
                reading1 = NAV_INVALID;
                return;
        }
                                        
        if(navlen1 < 4) //not enough digits
        {
                ResetNav('1');
                return;
        }
        navread1[navlen1] = '\0';        
        navlen1 = 0;         
        reading1 =  NAV_VALID; //valid data
        reading1r = atof(navread1);
		ResetNav('1');

/*
        navread1b[navlen1] = '\0';        
        do
        {
                navread1b[navlen1] = navread1[navlen1];
        }while(navlen1-- > 0);
*/
}
void nav_uart_push2(uchar dat)
{
//		sjSerialSendByte(dat);                            
//		return;
        if(navlen2 >= 19)
        {                          
                ResetNav('2');
        }
        if(((dat >= '0') && (dat <= '9')) ||
                (dat == '.') ||
                (dat == '-') ||
                (dat == '+') ||
                (dat == 'e') ||
                (dat == 'E') )
        {                    
                navread2[navlen2++] = dat;
                reading2 = NAV_INVALID;
                return;
        }
                                        
        if(navlen2 < 4) //not enough digits
        {
                ResetNav('2');
                return;
        }
        navread2[navlen2] = '\0';        
        navlen2 = 0;         
        reading2 =  NAV_VALID; //valid data
        reading2r = atof(navread2);
		ResetNav('2');			
/*
        navread2b[navlen2] = '\0';        
        do
        {

                navread2b[navlen2] = navread2[navlen2];
								sjSerialSendByte(navread2b[navlen2]);
        }while(navlen2-- > 0);
		sprintf(navcmd,"%5f",reading2r);
        prints(navcmd,strlen(navcmd),'0');
*/
}
void main()
{
	
	unsigned char sid;//id of serial to send 
	BYTE temp;
	BYTE fail_cnt;
	BYTE curr_rng;
	double nn;
	ulong c;
	long i=1000;
	nn = 1.234;

	c = 0x42C8077F;
	nn = *(double*)(&c);
//	ComOutStr(*(ulong*)(&nn));


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

	reading1r = -1000;
	reading2r = -1000;
	fail_cnt = 5;
	while(fail_cnt > 0)
	{
		delay(50000);
		fail_cnt--;
	}
	curr_rng = -1;

	while(1)
	{
		if(io_hasc())
		{
			nav_uart_push1(io_getc());
		}
		if(io_hasc2())
		{
			nav_uart_push2(io_getc2());
		}

		if(sjSerialIsDataWaiting())
		{
			temp = sjSerialWaitForOneByte();
			if (temp == CMD_ZERO)
			{
				reading1r = -1000;
				reading2r = -1000;
				nav_command(NAV_ZEROON,'1');
				nav_command(NAV_ZEROON,'2');
				delay(10000);
				nav_command(NAV_ZEROON,'1');
				nav_command(NAV_ZEROON,'2');
			}
			if(temp == CMD_READ)
			{
				if(curr_rng == -1)
				{
					nav_command(NAV_1V,'1');
					nav_command(NAV_1V,'2');
					curr_rng = 1;
					delay(10000);
					nav_command(NAV_1V,'1');
					nav_command(NAV_1V,'2');
					delay(10000);
					continue;
				}
				reading1r = -1000;
				reading2r = -1000;
				if(fail_cnt > 2)
				{
					if(curr_rng != 1)
					{
						nav_command(NAV_1V,'1');
						nav_command(NAV_1V,'2');
						curr_rng = 1;
						fail_cnt = 0;
						delay(10000);
						nav_command(NAV_1V,'1');
						nav_command(NAV_1V,'2');
						delay(10000);
						continue;
					}
				}
				nav_command(NAV_READ,'1');
				nav_command(NAV_READ,'2');
				if(fail_cnt < 10)
					fail_cnt++;

			}
		}
		if((reading1r > -999) && (reading2r > -999)) //make sure it is a valid reading
        {
			if(curr_rng == 0)
			{
				reading1r = reading1r/1000.0;
				reading2r = reading2r/1000.0;
			}
			if((curr_rng != 0) && (reading1r < 0.12) && (reading1r > -0.12) &&
			(reading2r < 0.12) && (reading2r > -0.12))
			{
				nav_command(NAV_120MV,'1');
				nav_command(NAV_120MV,'2');
				curr_rng = 0;
				fail_cnt = 0;
				delay(10000);
				nav_command(NAV_120MV,'1');
				nav_command(NAV_120MV,'2');
				delay(10000);
			}
			if(((reading1r > 10) || (reading2r > 10)))
			{
				nav_command(NAV_1V,'1');
				nav_command(NAV_1V,'2');
				curr_rng = 1;
				fail_cnt = 0;
				delay(10000);
				nav_command(NAV_1V,'1');
				nav_command(NAV_1V,'2');
				delay(10000);
			}
			ComOutStr(*(ulong*)(&reading1r));
			reading1r = -1000;
			ComOutStr(*(ulong*)(&reading2r));
			reading2r = -1000;
			fail_cnt = 0;
        }       
	}
}



