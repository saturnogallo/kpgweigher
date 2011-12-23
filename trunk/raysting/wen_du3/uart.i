/*********************************************************************************/
//                         UART0 and UART1 Subroutines
/*********************************************************************************/
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
// uart.h
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
                                // global.h    
                                                                                                          //extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
//extern u8 x_modem_databuf[];                       /* store data sent from PC */
//extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
//extern u8 x_modem_file_being_transferred;  
//extern u8 x_modem_file_transfer_complete;
/*********************************************************************************/
//                     UART0  Variables & Buffer Definition
/*********************************************************************************/
unsigned char volatile tx_buffer0[128];
unsigned char volatile tx_buffer0[128];
unsigned char volatile tx_wr_index0,tx_rd_index0,tx_counter0;
/*********************************************************************************/
//                     UART1  Variables & Buffer Definition
/*********************************************************************************/
unsigned char volatile tx_buffer1[128];
unsigned char volatile tx_wr_index1,tx_rd_index1,tx_counter1;
/*
 * 0-9,a-f,A-F char to hex conversion
 * otherwise 0xff will be returned.
 */
unsigned char ishexchar(unsigned char dat){
        if( (dat >= '0') && (dat <= '9'))
                return dat-'0';
        if( (dat >= 'a') && (dat <= 'f'))
                return dat-'a'+0x0a;
        if( (dat >= 'A') && (dat <= 'F'))
                return dat-'A'+0x0a;
        return 0xff;                
}                                     
             interrupt [19] void usart0_rx_isr(void)
{  
   unsigned char status,data;
   //read status and data
   status=UCSR0A;
   data=UDR0;            
   kbd_uart_push(data);
}
/*********************************************************************************/
//                 USART0 Transmitter interrupt service routine
/*********************************************************************************/
interrupt [21] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
   tx_rd_index0 =tx_rd_index0 & (128-1);
   };
}
/*********************************************************************************/
//                      UART1 Receiver interrupt service routine
/*********************************************************************************/
interrupt [31] void usart1_rx_isr(void)
{
   unsigned char status,data;
      //read status and data
   status=(*(unsigned char *) 0x9b);
   data=(*(unsigned char *) 0x9c); 
   nav_uart_push(data);
}
/*********************************************************************************/
//                   USART1 Transmitter interrupt service routine
/*********************************************************************************/
interrupt [33] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   (*(unsigned char *) 0x9c)=tx_buffer1[tx_rd_index1++];
   tx_rd_index1=tx_rd_index1 & (128-1);
   };
}
void com0_putc(char c)
{                    
if(0 == 1)
        return;                
while (tx_counter0 == 128);
#asm("cli")
if (tx_counter0 || ((UCSR0A & (1<<5))==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
   tx_wr_index0 = tx_wr_index0 & (128-1);
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}                      
void com1_putc(char c)
{
while (tx_counter1 == 128);
#asm("cli")
if (tx_counter1 || (((*(unsigned char *) 0x9b) & (1<<5))==0))
   {
   tx_buffer1[tx_wr_index1++]=c;
   tx_wr_index1 = tx_wr_index1 & (128-1);
   ++tx_counter1;
   }
else
   (*(unsigned char *) 0x9c)=c;
#asm("sei")
}
/*
void com0_puthex(u8 a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		com0_putc(h+'0');
	else
		com0_putc(h+'A'-0x0a);
	if(l <= 9)
		com0_putc(l+'0');
	else
		com0_putc(l+'A'-0x0a);
} 
*/              
void com0_putstr(unsigned char *ptr){
        while(*ptr != 0x00){
                com0_putc(*ptr++);
        } 
}
void com1_puthex(unsigned char a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		com1_putc(h+'0');
	else
		com1_putc(h+'A'-0x0a);
	if(l <= 9)
		com1_putc(l+'0');
	else
		com1_putc(l+'A'-0x0a);
} 
void com1_putstr(unsigned char *ptr){
        while(*ptr != 0x00){
                com1_putc(*ptr++);
        } 
}                             
/*
void com0_mputs(u8 *buf, u8 size){
        while(size-- > 0){
                com0_puthex(*buf++);
        }
} 
*/
void com1_mputs(unsigned char *buf, unsigned char size){
        while(size-- > 0){
                com1_puthex(*buf++);
        }
}
/*********************************************************************************/
//                              Intialize UART
/*********************************************************************************/
void Init_UART()
{
   // USART0 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART0 Receiver: On
   // USART0 Transmitter: On
   // USART0 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
      UCSR0A=0x00;
      UCSR0B=0xD8;
      (*(unsigned char *) 0x95)=0x06;
      (*(unsigned char *) 0x90)=0x00;      
      UBRR0L=0x0F;      
   // USART1 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART1 Receiver: On
   // USART1 Transmitter: On
   // USART1 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
      (*(unsigned char *) 0x9b)=0x00;
      (*(unsigned char *) 0x9a)=0xD8;
      (*(unsigned char *) 0x9d)=0x06;
      (*(unsigned char *) 0x98)=0x00;
      (*(unsigned char *) 0x99)=0x0F;
         // Initialize buffer variables
      tx_wr_index0 = 0;
      tx_rd_index0 = 0;
      tx_counter0  = 0;
            tx_wr_index1 = 0;
      tx_rd_index1 = 0;
      tx_counter1  = 0; 
} 
