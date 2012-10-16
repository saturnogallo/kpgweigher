/*********************************************************************************/
//                         UART0 and UART1 Subroutines
/*********************************************************************************/
#include <mega128.h>
#include "uart.h" 
#include "16c554.h" 
#include "global.h"                 

//extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
//extern u8 x_modem_databuf[];                       /* store data sent from PC */
//extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
//extern u8 x_modem_file_being_transferred;  
//extern u8 x_modem_file_transfer_complete;

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

/*
 * 0-9,a-f,A-F char to hex conversion
 * otherwise 0xff will be returned.
 */
u8 ishexchar(u8 dat){
        if( (dat >= '0') && (dat <= '9'))
                return dat-'0';
        if( (dat >= 'a') && (dat <= 'f'))
                return dat-'a'+0x0a;
        if( (dat >= 'A') && (dat <= 'F'))
                return dat-'A'+0x0a;
        return 0xff;                
}                                     


             
interrupt [USART0_RXC] void usart0_rx_isr(void)
{  
   u8 status,data;
   //read status and data
   status=UCSR0A;
   data=UDR0;            
   cm0_push(data);
}

/*********************************************************************************/
//                 USART0 Transmitter interrupt service routine
/*********************************************************************************/
interrupt [USART0_TXC] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
   tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
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
   cm1_push(data);
}

/*********************************************************************************/
//                   USART1 Transmitter interrupt service routine
/*********************************************************************************/
interrupt [USART1_TXC] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   UDR1=tx_buffer1[tx_rd_index1++];
   tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
   };
}

void com0_putc(char c)
{                    
if(DEBUG == 1)
        return;                
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
   tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}                      


void com1_putc(char c)
{
while (tx_counter1 == TX_BUFFER_SIZE1);
#asm("cli")
if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer1[tx_wr_index1++]=c;
   tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
   ++tx_counter1;
   }
else
   UDR1=c;
#asm("sei")
}

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
              

void com0_mputs(u8 *buf, u8 size){
        while(size-- > 0){
                com0_puthex(*buf++);
        }
} 


void com0_putstr(u8 *ptr){
        while(*ptr != 0x00){
                com0_putc(*ptr++);
        } 
}

void com1_puthex(u8 a)
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
void com1_putstr(u8 *ptr){
        while(*ptr != 0x00){
                com1_putc(*ptr++);
        } 
}                             
void com1_mputs(u8 *buf, u8 size){
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
      UCSR0C=0x06;
      UBRR0H=0x00;      
      UBRR0L=0x0F;      

   // USART1 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART1 Receiver: On
   // USART1 Transmitter: On
   // USART1 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
      UCSR1A=0x00;
      UCSR1B=0xD8;
      UCSR1C=0x06;
      UBRR1H=0x00;
      UBRR1L=0x0F;
      
   // Initialize buffer variables
      tx_wr_index0 = 0;
      tx_rd_index0 = 0;
      tx_counter0  = 0;
      
      tx_wr_index1 = 0;
      tx_rd_index1 = 0;
      tx_counter1  = 0; 
} 
