/*********************************************************************************/
//                         UART0 and UART1 Subroutines
/*********************************************************************************/
#include <mega64.h>
#include "uart.h" 
#include "16c554.h" 
#include "global.h"                 

/*********************************************************************************/
//                     UART0  Variables & Buffer Definition
/*********************************************************************************/
u8 volatile tx_buffer0[TX_BUFFER_SIZE0];
u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;

/*********************************************************************************/
//                     UART1  Variables & Buffer Definition
/*********************************************************************************/
u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;

u8 ishexchar(u8 dat){
        if( (dat >= '0') && (dat <= '9'))
                return dat-'0';
        if( (dat >= 'a') && (dat <= 'f'))
                return dat-'a'+0x0a;
        if( (dat >= 'A') && (dat <= 'F'))
                return dat-'A'+0x0a;
        return 0xff;                
}                                     

/*********************************************************************************/
//              USART0 (Debug Port) Receiver interrupt service routine                
/*********************************************************************************/
interrupt [USART0_RXC] void usart0_rx_isr(void)
{  
   u8 status,data;
   
   //read status and data
   status=UCSR0A;
   data=UDR0; 
  
   d_putchar(data);
   debug(data);   
}

/*********************************************************************************/
//                 USART0 Transmitter interrupt service routine
/*********************************************************************************/

interrupt [USART0_TXC] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0];
   if (++tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
   };
}


/*********************************************************************************/
//                      UART1 Receiver interrupt service routine
/*********************************************************************************/
interrupt [USART1_RXC] void usart1_rx_isr(void)
{
   u8 status,data;
   
   //read status and data
   status=UCSR1A;
   data=UDR1; 
  
   cm_pushc(data,GROUPPC);
}

/*********************************************************************************/
//                   USART1 Transmitter interrupt service routine
/*********************************************************************************/

interrupt [USART1_TXC] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   UDR1=tx_buffer1[tx_rd_index1];
   if (++tx_rd_index1 == TX_BUFFER_SIZE1) tx_rd_index1=0;
   };
}

void d_putchar(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0]=c;
   if (++tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}                      

void putchar(char c)
{
while (tx_counter1 == TX_BUFFER_SIZE1);
#asm("cli")
if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer1[tx_wr_index1]=c;
   if (++tx_wr_index1 == TX_BUFFER_SIZE1) tx_wr_index1=0;
   ++tx_counter1;
   }
else
   UDR1=c;
#asm("sei")
}

void d_putchar2(u8 a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		d_putchar(h+'0');
	else
		d_putchar(h+'A'-0x0a);

	if(l <= 9)
		d_putchar(l+'0');
	else
		d_putchar(l+'A'-0x0a);
}
void d_putstr(flash u8 *ptr){
        while(*ptr != 0x00){
                d_putchar(*ptr++);
        } 
}
void putchar2(u8 a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		putchar(h+'0');
	else
		putchar(h+'A'-0x0a);

	if(l <= 9)
		putchar(l+'0');
	else
		putchar(l+'A'-0x0a);
} 
void putstr(flash u8 *ptr){
        while(*ptr != 0x00){
                putchar(*ptr++);
        } 
}                             

void d_mputs(u8 *buf, u8 size){
        while(size-- > 0){
                d_putchar2(*buf++);
        }
}
void mputs(u8 *buf, u8 size){
        while(size-- > 0){
                putchar(*buf++);
        }
}

/*********************************************************************************/
//                              Intialize UART
/*********************************************************************************/
void Init_UART()
{
   // Debug port
   // USART0 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART0 Receiver: On
   // USART0 Transmitter: On
   // USART0 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF) 38400 (0x17)
   // USART0 Baud rate: 19200 (2F) 9600(0x5F)
      UCSR0A=0x00;
      UCSR0B=0xD8;
      UCSR0C=0x06;
      UBRR0H=0x00;
      UBRR0L=0x5F;      //9600    
//      UBRR0L=0x7;       //115200

   // PC data commmuncation. 
   // USART1 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART1 Receiver: On
   // USART1 Transmitter: On
   // USART1 Mode: Asynchronous
   // USART1 Baud rate: 115200
      UCSR1A=0x00;
      UCSR1B=0xD8;
      UCSR1C=0x06;
      UBRR1H=0x00;
      UBRR1L=0x5F;       //9600
      //UBRR1L=0x7;       //115200
      
   // Initialize buffer variables
      tx_wr_index0 = 0;
      tx_rd_index0 = 0;
      tx_counter0  = 0;
      
      tx_wr_index1 = 0;
      tx_rd_index1 = 0;
      tx_counter1  = 0; 
     
} 






