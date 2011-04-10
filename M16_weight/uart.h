/************************************************************************/
//                            MEGA8 UART Head File
/************************************************************************/
#ifndef _UART_H_
#define _UART_H_
#include "define.h"     

/************************************************************************************/
//                                 define
/************************************************************************************/
#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC) 

// USART Transmit buffer
#define TX_BUFFER_SIZE 0x20  
// USART Receiver buffer 
#define RX_BUFFER_SIZE 0x20  

#define BROADCAST_BASE_ADDR 0x80    // RS485 Boardcast address, group A~D 0x80-0x83

// subroutines that maybe called by subroutines in other *.c files. 
void UART_Init(void);
void putchar(u8);    
//void d_putchar(u8); //byte to char format;
void SetBaudrate(void);  
void mputs(u8 *buf, u8 size, u8 port);  
#ifndef _OLD_FASHION_CMD_PROCESS_
//u8 read_uart_db();
//u8 data_available_in_rxbuf();
#endif
//void d_mputs(u8 *buf, u8 size, u8 port); 
//void putstr(flash u8 *ptr);    
//u8 ishexchar(u8 dat);                       
//extern char getchar(void);
#endif