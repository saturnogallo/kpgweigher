/************************************************************************/
//                            MEGA8 UART Head File
/************************************************************************/
#ifndef _UART_H_
#define _UART_H_  

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

// subroutines that maybe called by subroutines in other *.c files. 
void UART_Init(void);
void putchar(char);    
//void d_putchar(u8); //byte to char format;
void SetBaudrate(void);  

