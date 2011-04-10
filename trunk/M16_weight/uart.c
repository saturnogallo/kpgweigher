#include <mega16.h> 
#include "uart.h" 
#include "define.h" 
#include "cs5532.h"
#include "motor.h"
#include "drveeprom.h" 
#include "command.h"                                        

/****************************************************************************/
//                 UART Receiver/ Transmiter Variable Definition
/****************************************************************************/
// USART Transmitter buffer
u8 volatile tx_buffer[TX_BUFFER_SIZE];

// define index for transmitter
u8 volatile tx_wr_index;
u8 volatile tx_rd_index;
u8 volatile tx_counter;

//#ifndef _OLD_FASHION_CMD_PROCESS_
//u8 volatile rx_buffer[RX_BUFFER_SIZE];
// define index for Receiver
u8 volatile rx_wr_index;
u8 volatile rx_rd_index;
u8 volatile rx_counter;
//#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow; 

extern u8 debug_mode;
     
/****************************************************************************/
//               UART Receiver Interrupt service routine    
// UART communication frame structure:
// Head1(0xfe) Head2(0x68), addr_from, addr_to, cmd, data_len
// data_to_send_or_receive, check_sum. 
// in normal mode, once a byte is received, function cm_pushc() will be called
// to check if a frame is received completed or not. cm_pushc() will analysize 
// the frame and respond to the frame.
/****************************************************************************/
//#define HW_STATUS_UART_ERROR 0b00010000
interrupt [USART_RXC] void usart_rx_isr(void)
{
   u8 status,data;
   //read status and data
   status=UCSRA;
   data=UDR;    
   // check if error happened.
   if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {   
//#ifdef _OLD_FASHION_CMD_PROCESS_
       cm_pushc(data,0);
       //hw_status &=0xEF;
//#else       
      // rx_buffer[rx_wr_index++]= data;
      // if(rx_wr_index >= RX_BUFFER_SIZE)
      //    rx_wr_index=0;
      // rx_counter++;
//#endif    
   }      
   //else
      //hw_status |= HW_STATUS_UART_ERROR;      
}

/****************************************************************************/
//               UART Transmitter Interrupt service routine
/****************************************************************************/
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
      PORTB.0 = 1;         // set RS485 node to transmiter mode
      --tx_counter;
      UDR=tx_buffer[tx_rd_index];
      if (++tx_rd_index == TX_BUFFER_SIZE) 
         tx_rd_index=0;
   }
else 
   PORTB.0 = 0;         // set RS485 node to receiver mode.
}

/****************************************************************************/
//                     UART Set Baud rate.  
// This subroutine sets UART baud rate based on user settings
// Users are allowed to change baudrate of RS485 nodes. 
/****************************************************************************/
void SetBaudrate()
{   
   switch(RS485._flash.baudrate)
   {  case 0x0:                 // 115200bps
        UBRRH=0x00;
        UBRRL=0x03;
        break;
      case 0x1:                 // 57600bps
        UBRRH=0x00;
        UBRRL=0x07;                                 
        break;
      case 0x2:                 // 38400bps
        UBRRH=0x00;
        UBRRL=0x0B;
        break;
      case 0x3:                 // 19200bps
        UBRRH=0x00;
        UBRRL=0x17;
        break;
      case 0x4:                 // 9600bps
        UBRRH=0x00;
        UBRRL=0x2F;
        break;
      default:                  // 115200bps
        UBRRH=0x00;
        UBRRL=0x03;                         
        break;                          
   }     
}
/****************************************************************************/
//                     UART Initialization   
// need to enable global interrupt in main function.
// This module only set UART interrupt bits.
/****************************************************************************/
void UART_Init(void)
{ 
 // Set RS485 node to receiver mode
    PORTB.0 = 0;
 
 // Reset UART buffer and flag;
    rx_buffer_overflow = 0;
    tx_wr_index=0;
    tx_rd_index=0;
    tx_counter=0;
//#ifndef _OLD_FASHION_CMD_PROCESS_
    rx_wr_index=0;
    rx_rd_index=0;
    rx_counter=0;
//#endif    
    
 // USART initialization
 // Communication Parameters: 8 Data, 1 Stop, No Parity
 // USART Receiver: On
 // USART Transmitter: On
 // USART Mode: Asynchronous
    #asm("cli")
    UCSRA=0x00;
 
 // Enable TXC interrupt, RX interrupt, disable UDRE interrupt
    UCSRB=0xD8;
 
 // UCSRC share IO address with reg UBRR, MSB bit for UCSRC must be 1.
 // 8 data, 1 stop, even parity
    UCSRC=0xA6;    // EVEN parity bit
 // UCSRC=0x86;    // No parity bit
 
 // Set Baud rate based on System setting
 // RS485.BaudRate_Index can be read out from EEPROM
 // or set by RS485 master via RS485 communication
 // UCSRC share IO address with reg UBRR, MSB bit for UBRR must be 0.
 // don't enable interrupt here.
 // enable global interrupt in main function after all modules are initialized 
    //RS485._flash.baudrate = 0;
    SetBaudrate();
}                                                                             

/****************************************************************************/
//
//                 Higher Level UART print Subroutines
//
/****************************************************************************/
#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+                         
/****************************************************************************/
// Send a char-Type data via uart.
/****************************************************************************/
void putchar(char c)
{
   // need to be changed to avoid waiting
   while (tx_counter >= TX_BUFFER_SIZE);

   // set RS85-node to transmiter mode
   PORTB.0 = 1;
   
   #asm("cli")
   // if there is data in TX buffer or data being transmitted. 
   // then write data to buffer.
   if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
      {
      tx_buffer[tx_wr_index]=c;
      if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
      ++tx_counter;
      }
   // if no data in buffer and no data being transitted, 
   // write data to UDR to start transimision immediately
   else
      UDR=c; 
   #asm("sei")
}
/****************************************************************************/
// mputs(): UART prints a data array
/****************************************************************************/
void mputs(u8 *buf, u8 size, u8 port)
{
    while(size-- > 0)
      putchar(*buf++);    
} 

/****************************************************************************/
// Read a byte from UART buffer
/****************************************************************************/
/*#ifndef _OLD_FASHION_CMD_PROCESS_
u8 read_uart_db()
{
  u8 uart_rdata;
  uart_rdata = rx_buffer[rx_rd_index++];
  rx_rd_index %= RX_BUFFER_SIZE;
  rx_counter--;
  return uart_rdata;
}
u8 data_available_in_rxbuf()
{
  if (rx_counter > RX_BUFFER_SIZE)
  {  
     rx_rd_index = 0;
     #asm("cli")
     rx_wr_index = 0;
     rx_counter = 0;
     #asm("sei")
  }
  return rx_counter;
}        
#endif//*/

/****************************************************************************/
// d_putchar(): output a character in ASCII code mode.
// For example: 0x9b is converted to 2 characters '9'/'B' before output
/****************************************************************************/
/*void d_putchar(u8 a)
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
} //*/ 

/****************************************************************************/
// d_mputs() convert an ASCII code array to HEX and print them via UART
/****************************************************************************/
/*void d_mputs(u8 *buf, u8 size, u8 port)
{   while(size-- > 0)
      d_putchar(*buf++);        
} //*/

/****************************************************************************/
// UART print a string
/****************************************************************************/ 
/*void putstr(flash u8 *ptr)
{
   while(*ptr != 0x00)
      putchar(*ptr++);   
} //*/

#pragma used-
#endif
       