//#define NULL 0x0
// global.h    
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
#pragma used+
sfrb PINF=0;
sfrb PINE=1;
sfrb DDRE=2;
sfrb PORTE=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRR0L=9;
sfrb UCSR0B=0xa;
sfrb UCSR0A=0xb;
sfrb UDR0=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb SFIOR=0x20;
sfrb WDTCR=0x21;
sfrb OCDR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrw ICR1=0x26;   // 16 bit access
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  // 16 bit access
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  // 16 bit access
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  // 16 bit access
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb ASSR=0x30;
sfrb OCR0=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TIFR=0x36;
sfrb TIMSK=0x37;
sfrb EIFR=0x38;
sfrb EIMSK=0x39;
sfrb EICRB=0x3a;
sfrb XDIV=0x3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-
// Interrupt vectors definitions
                                        /*
        Port related definition
*/ 
void kbd_uart_push(unsigned char);
void scanner_uart_push(unsigned char);
void pc_uart_push(unsigned char);
void nav_uart_push(unsigned char);
void testA(unsigned char);
//#define PORT_3          SPORTA
//#define PORT_B          SPORTB
// Hardware related
                            void sleepms(unsigned int ms);
                              double nav_read();
void scanner_set_mode();
void delay (unsigned int us) ;
void delay1 (unsigned int ms);
char lowc(unsigned char x);
char highc(unsigned char x);
//'k'
//'l'
//#define PG_TPRBCONFIG	12
/*
 *	Probe data structure definition
 */
typedef eeprom struct _PRBDATA
{
	double param1[24];
	double param2[24];
	double param3[24];                      
	double rtp[24];
	char  name[24][8];	        //probe serials
	unsigned char type[24];		//probe type
}PRBDATA;
typedef eeprom struct _SYSDATA
{
	double          R0;  //zero offset
	double          V0;  //zero offset
	double          Rs1; //jiao-zheng zhi
	int             ktime;//time for switch
	unsigned char 	        tid[24];	//probe index of each channel for T mode
	unsigned char           rid[24];        //probe index of each channel for R mode
	unsigned char           prbmode;
	unsigned char           kttmode;                    
}SYSDATA;               
typedef struct _RUNDATA
{
	double  reading[24];		  //reading on each channel
	double  temperature[24];	  //temperature result on each channel
	double 	Rx;		  //final result
	double  stdV;		  //voltage on stdV;
}RUNDATA;
extern RUNDATA rdata;
extern SYSDATA eeprom sysdata;
extern PRBDATA eeprom tprbdata;	//probe data for T mode
extern PRBDATA eeprom rprbdata;	//probe data for R mode
void State_Init();
void display_buttons(unsigned char pos,unsigned char val);           
double buf2double();
int buf2byte();
//#define ONESECBIT       14
extern void DBG(unsigned char);
void SwitchWindow(unsigned char page);
char* rname2b(unsigned char i);
char* tname2b(unsigned char i);
// CodeVisionAVR C Compiler
// (C) 1998-2006 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for standard I/O functions
// CodeVisionAVR C Compiler
// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.
// Variable length argument list macros
typedef char *va_list;
#pragma used+
char getchar(void);
void putchar(char c);
void puts(char *str);
void putsf(char flash *str);
char *gets(char *str,unsigned int len);
void printf(char flash *fmtstr,...);
void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);
                                               #pragma used-
#pragma library stdio.lib
  																		void scanner_set_channel(unsigned char ch);
void scanner_uart_push(unsigned char data);
void pc_uart_push(unsigned char data);
void nav_uart_push(unsigned char data);     
void nav_command(unsigned char cmd);
/* CodeVisionAVR C Compiler
   Prototypes for standard library functions

   (C) 1998-2003 Pavel Haiduc, HP InfoTech S.R.L.
*/
#pragma used+
int atoi(char *str);
long int atol(char *str);
float atof(char *str);
void itoa(int n,char *str);
void ltoa(long int n,char *str);
void ftoa(float n,unsigned char decimals,char *str);
void ftoe(float n,unsigned char decimals,char *str);
void srand(int seed);
int rand(void);
void *malloc(unsigned int size);
void *calloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size); 
void free(void *ptr);
#pragma used-
#pragma library stdlib.lib
static unsigned char navlen = 0;
static double reading = -1000;
static char navread[20];
static char navread2[20];
unsigned char scancmd[5];
unsigned char eeprom scanner_type = 1; //1: MI, 2: GUIDLINE
void scanner_set_mode()
{                            
        if (sysdata.prbmode == 1)
        {
            scancmd[0] = '#';
        }else{
            scancmd[0]= '!';
        }
        scancmd[1] = 0x0D;
        scancmd[2] = 0x0A;
        prints(scancmd,3,2);    
}
void scanner_set_channel(unsigned char ch)
{       
        if(scanner_type == 1) //MI
        {        
                if(ch < 10)
                {
                        scancmd[0] = (ch + '0');
                        scancmd[1] = 'A';                
                        scancmd[2] = 0x0D;
                        scancmd[3] = 0x0A;
                        prints(scancmd,4,2);    
                        return;
                }else{              
                        scancmd[0] = (unsigned char)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[1] = ch + '0';
                        scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,2);
                }
        }
        if(scanner_type == 2) //guidline
        {               
                        scancmd[0] = 'A';
                        scancmd[1] = (unsigned char)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[2] = ch + '0';
                        scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,2);
        }
}
                            //incoming data handler of scanner
void scanner_uart_push(unsigned char data)
{
}                                 
//incoming data hander of PC
void pc_uart_push(unsigned char data)
{
}       
static unsigned char navcmd[12];                                                                  
void nav_command(unsigned char cmd)
{
        if(cmd == 2)
               sprintf(navcmd,"%%01;12;02\r");
        if(cmd == 3)
               sprintf(navcmd,"%%01;12;01\r");
        if(cmd == 4)
               sprintf(navcmd,"%%01;12;00\r");
        if(cmd == 5)
               sprintf(navcmd,"%%01;26\r");
        if(cmd == 6)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == 7)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == 8)    
        {
               sprintf(navcmd,"%%01;06\r");
        }
        if(cmd == 1) 
        {           
                navlen = 0;   reading = -1000;  ;
                sprintf(navcmd,"%%01;00\r");        
        }
        if(cmd == 9)
        {       
                navlen = 0;   reading = -1000;  ;
                sprintf(navcmd,"%%01;01\r");
        }                       
//                com1_putc('?');
        prints(navcmd,strlen(navcmd),5                     );
}
unsigned long navtime;
double nav_read()
{                       
        if(0 == 1)
                return 1.0; 
        nav_command(9);
        navtime = 0;        
        while(1)
        {
                if(reading < -1000) //make sure it is a valid reading
                {              
                     reading = atof(navread2);
                     return reading;
                }       
                if(navtime++ > 655350)
                {          
                     nav_command(9);   
                     navtime = 0;
                }                
                sleepms(1);
        }
        return 1.0;
}            
//incoming data hander of navameter
void nav_uart_push(unsigned char data)
{                            
        if(navlen >= 19)
        {                          
                navlen = 0;   reading = -1000;  ;
        }
        if(((data >= '0') && (data <= '9')) ||
                (data == '.') ||
                (data == '-') ||
                (data == '+') ||
                (data == 'e') ||
                (data == 'E') )
        {                    
                navread[navlen++] = data;
                reading = -1000;
                return;
        }
                                                if(navlen < 4) //not enough digits
        {
                navlen = 0;   reading = -1000;  ;
                return;
        }
        navread[navlen] = '\0';        
        do
        {
                navread2[navlen] = navread[navlen];
        }while(navlen-- > 0);
        navlen = 0;         
        reading =  -2000; //valid data
}                                        
