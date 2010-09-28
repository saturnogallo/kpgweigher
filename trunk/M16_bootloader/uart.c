
#include <mega16.h> 
#include "uart.h" 
#include "define.h" 
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

//u8 volatile rx_buffer[RX_BUFFER_SIZE];
// define index for Receiver
//u8 volatile rx_wr_index;
//u8 volatile rx_rd_index;
//u8 volatile rx_counter;
     
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
       cm_pushc(data);  
   }          
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
extern u8 baud_rate; 
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
    //rx_buffer_overflow = 0;
    tx_wr_index=0;
    tx_rd_index=0;
    tx_counter=0;
    //rx_wr_index=0;
    //rx_rd_index=0;
    //rx_counter=0;
    
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
 
  /* if baud_rate = 0, 115200bps, else 9600bps */
   UBRRH=0x00; 
   //if(!baud_rate)   
      UBRRL=0x03;   // 115200bps    
   //else
   //   UBRRL=0x2F;   // 9600 bps
  
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
void prints(u8 *buf, u8 size)
{
    while(size-- > 0)
      putchar(*buf++);    
} 
     
#endif

#pragma used-
#endif
       