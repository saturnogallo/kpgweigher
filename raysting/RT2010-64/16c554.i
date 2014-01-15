/********************************************************************************/
//                             TL16C554 Driver
// Controller:  MEGA64
// 16C554 clk:  7.3728MHZ
//
// Schematic Description: 
// 16C554 <---> MEGA64
// INTA   <---> INT4
// INTB   <---> INT5
// INTC   <---> INT6
// INTD   <---> INT7
// CSA#   <---> A15
// CSB#   <---> A14
// CSC#   <---> A13
// CSD#   <---> A12
//   A0   <---> A8
//   A1   <---> A9
//   A2   <---> A10
// RESET  <---> PB0
// INTN#  <---> PE2
// Coded by: fd_lxh@yahoo.com.cn
/********************************************************************************/
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
// global.h    
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
//#define NULL 0x0
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
//PORTB.7 RX, PORTB.6 RS, PORTB.5 1MA, PORTB.4. 0.1MA,
//PORTB.3 PT100, PORTB.2 PT1000, PORTB.1 CH1,  PORB.0 CH2    
//#define SET_TORX     display_buttons(KEY_RS,1)
//#define SET_TORS     display_buttons(KEY_RS,0)
                              double nav_read();
void scanner_set_mode();
/*********************************************************************************/
//                              16C554 Header file
/*********************************************************************************/
// global.h    
                                                                                                          // Registers of 16C554
// Registers for UART A
// Registers for UART B
// Registers for UART C
// Registers for UART D
extern void Reset_554(void);
extern void Init_554(void);
void prints(unsigned char*, unsigned char, char);
                                // uart.h
// global.h    
                                                                                                          void d_mputs(unsigned char *buf, unsigned char size);
void d_putchar2(char);    
void d_putchar(char);
void d_putstr(flash unsigned char *ptr);
void Init_UART(void);
void com0_putc(unsigned char);      
void com0_putstr(unsigned char *ptr);
void com1_putc(unsigned char);
void putchar(char);  
void putchar2(char);
void putstr(flash unsigned char *ptr);    
void mputs(unsigned char *buf, unsigned char size);
unsigned char ishexchar(unsigned char dat);
// global.h    
                                                                                                          void delay (unsigned int us) ;
void delay (unsigned int us) ;
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
	double          Rs1; //jiao-zheng zhi for PT100
	int             ktime;//time for switch
	unsigned char 	        tid[24];	//probe index of each channel for T mode
	unsigned char           rid[24];        //probe index of each channel for R mode
	unsigned char           prbmode;
	unsigned char           kttmode;      
	double          Rs2; //for PT1000              
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
extern void navto120mv();
extern void navto1v();
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
// LCR.7 must be cleared to 0 when accessing RBR/THR/IER
// LCR.7 must be set to 1 when accessing divisor latch
/********************************************************************************/
//               TLC16C554 UART PORT A Interrupt Subroutine
/********************************************************************************/
interrupt [6] void ext_int4_isr(void)
{
   unsigned char rundata;     
   switch((*(volatile unsigned char *)(0x7000+0x200))  & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = (*(volatile unsigned char *)(0xE000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = (*(volatile unsigned char *)(0x7000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          pc_uart_push(rundata);
          return;
      default:
          break;
   }           
      rundata = (*(volatile unsigned char *)(0x7000+0x500)) ;        // read LSR to clear interrupt. dummy read     
}
/********************************************************************************/
//               TLC16C554 UART PORT B Interrupt Subroutine
/********************************************************************************/
// External Interrupt 5 service routine
interrupt [7] void ext_int5_isr(void)
{
   unsigned char rundata;           
   switch((*(volatile unsigned char *)(0xB000+0x200))  & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = (*(volatile unsigned char *)(0xE000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = (*(volatile unsigned char *)(0xB000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          scanner_uart_push(rundata);
          return;
      default:
          break;
   }                   
   rundata = (*(volatile unsigned char *)(0xB000+0x500)) ;        // read LSR to clear interrupt. dummy read     
}
/********************************************************************************/
//               TLC16C554 UART PORT C Interrupt Subroutine
/********************************************************************************/
// External Interrupt 6 service routine
interrupt [8] void ext_int6_isr(void)
{
   unsigned char rundata;
   switch((*(volatile unsigned char *)(0xD000+0x200))  & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = (*(volatile unsigned char *)(0xE000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = (*(volatile unsigned char *)(0xD000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          testA(rundata);
          return;
      default:
          break;
   }                   
   rundata = (*(volatile unsigned char *)(0xD000+0x500)) ;        // read LSR to clear interrupt. dummy read     
}
/********************************************************************************/
//               TLC16C554 UART PORT D Interrupt Subroutine
/********************************************************************************/
// External Interrupt 7 service routine
interrupt [9] void ext_int7_isr(void)
{
   unsigned char rundata;
   switch((*(volatile unsigned char *)(0xE000+0x200))  & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt            
          rundata = (*(volatile unsigned char *)(0xE000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = (*(volatile unsigned char *)(0xE000+0x000)) ;        // read data from receiver FIFO. RBR read to clear int          
          testA(rundata);
          return;
      default:
          break;
   }                   
   rundata = (*(volatile unsigned char *)(0xE000+0x500)) ;        // read LSR to clear interrupt. dummy read     
}
/********************************************************************************/
//                             Reset 16C554
/********************************************************************************/
void Reset_554(void)
{
   // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
      PORTE.3 = 1;                                                         
      sleepms(5*(unsigned int)10000);    // Delay
      PORTE.3 = 0;          
      sleepms(5*(unsigned int)10000);    // Delay
}
/********************************************************************************/
//                             Initialize 16C554 
/********************************************************************************/
void Init_554(void)
{
      Reset_554();  // Reset 554
      /********************************************************************/      
   //                      Initialize UART A
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      (*(volatile unsigned char *)(0x7000+0x300))  = 0x80;
      (*(volatile unsigned char *)(0x7000+0x000)) = 0x30          ;
      (*(volatile unsigned char *)(0x7000+0x100)) = 0x0;     
      // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      (*(volatile unsigned char *)(0x7000+0x300))  = 0b00011011    ;
   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      (*(volatile unsigned char *)(0x7000+0x200))  = 0b00000111   ;
               // Set Modem Control Register:
   // Enable enternal interrupt
      (*(volatile unsigned char *)(0x7000+0x400))  = 0b00001000;
         // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      (*(volatile unsigned char *)(0x7000+0x100))  = 0b00000001;                                                  
   /********************************************************************/      
   //                      Initialize UART B
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
   // LCR.7 must be set to "1" before setting baud rate
      (*(volatile unsigned char *)(0xB000+0x300))  = 0x80;
      (*(volatile unsigned char *)(0xB000+0x000)) = 0x30          ;
      (*(volatile unsigned char *)(0xB000+0x100)) = 0x0;     
      // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      (*(volatile unsigned char *)(0xB000+0x300))  = 0b00011011    ;
   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      (*(volatile unsigned char *)(0xB000+0x200))  = 0b00000111   ;
               // Set Modem Control Register:
   // Enable enternal interrupt
      (*(volatile unsigned char *)(0xB000+0x400))  = 0b00001000;
         // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      (*(volatile unsigned char *)(0xB000+0x100))  = 0b00000001;
   /********************************************************************/      
   //                      Initialize UART C
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      (*(volatile unsigned char *)(0xD000+0x300))  = 0x80;
      (*(volatile unsigned char *)(0xD000+0x000)) = 0x30          ;
      (*(volatile unsigned char *)(0xD000+0x100)) = 0x0;     
      // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      (*(volatile unsigned char *)(0xD000+0x300))  = 0b00011011    ;
   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      (*(volatile unsigned char *)(0xD000+0x200))  = 0b00000111   ;
               // Set Modem Control Register:
   // Enable enternal interrupt
      (*(volatile unsigned char *)(0xD000+0x400))  = 0b00001000;
         // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      (*(volatile unsigned char *)(0xD000+0x100))  = 0b00000001;      
      /********************************************************************/      
   //                      Initialize UART D
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      (*(volatile unsigned char *)(0xE000+0x300))  = 0x80;
      (*(volatile unsigned char *)(0xE000+0x000)) = 0x30          ;
      (*(volatile unsigned char *)(0xE000+0x100)) = 0x0;     
      // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      (*(volatile unsigned char *)(0xE000+0x300))  = 0b00011011    ;
   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      (*(volatile unsigned char *)(0xE000+0x200))  = 0b00000111   ;
               // Set Modem Control Register:
   // Enable enternal interrupt
      (*(volatile unsigned char *)(0xE000+0x400))  = 0b00001000;
         // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      (*(volatile unsigned char *)(0xE000+0x100))  = 0b00000001; 
}                                            
/********************************************************************************/
//                 Send multi-bytes (less than 16 bytes) via UART
// Function: 
// Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
// format:
// print_554(u8 *str, char uart_port)
// arg1: string to be sent. arg2: port (A B,C,D)
/********************************************************************************/
void prints(unsigned char *str, unsigned char length, char uart_port)
{           
    unsigned char len, i;
    //len = length & 0x0F;        //15bytes at most 
    len = length; 
    switch(uart_port)
    {
       case 0:
           // wait if data still being transmitted in UART
           // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
           // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
           // check if FIFO is empty by LSR5. 
           // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because 
           // UC_554A_LSR is not a register name, 
           // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
           // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
           // here in 16c554.h, a bracket is added
           while(len)
           {
              while( !((*(volatile unsigned char *)(0x7000+0x500))  & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < ((len<16)?(len):(16)); i++)   /* fill FIFO with 16 bytes  */              
                  (*(volatile unsigned char *)(0x7000+0x000))  = *str++;           
              len = len - ((len<16)?(len):(16));
           }               
           break;
       case 1:
           while(len)
           {
              while( !((*(volatile unsigned char *)(0xB000+0x500))  & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < ((len<16)?(len):(16)); i++)   /* fill FIFO with 16 bytes  */              
                  (*(volatile unsigned char *)(0xB000+0x000))  = *str++;           
              len = len - ((len<16)?(len):(16));
           }               
           break;
       case 2:
           while(len)
           {
              while( !((*(volatile unsigned char *)(0xD000+0x500))  & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < ((len<16)?(len):(16)); i++)   /* fill FIFO with 16 bytes  */              
                  (*(volatile unsigned char *)(0xD000+0x000))  = *str++;           
              len = len - ((len<16)?(len):(16));
           }               
                         break;
       case 3:
           while(len)
           {
              while( !((*(volatile unsigned char *)(0xE000+0x500))  & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < ((len<16)?(len):(16)); i++)   /* fill FIFO with 16 bytes  */              
                  (*(volatile unsigned char *)(0xE000+0x000))  = *str++;           
              len = len - ((len<16)?(len):(16));
           }               
           break;
       case 4:
           while(len-- > 0){    
                com0_putc(*str++);            
           }
           break;
       case 5                     :
           while(len-- > 0){
                com1_putc(*str++);            
           }
           break;
       default:
           PORTC = 0xF0;
           break;
    }
}
           