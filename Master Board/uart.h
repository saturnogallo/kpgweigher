// uart.h
#ifndef _UART_H_
#define _UART_H_
#include "global.h"
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

#define RX_BUFFER_SIZE0 16
#define TX_BUFFER_SIZE0 128
#define RX_BUFFER_SIZE1 16
#define TX_BUFFER_SIZE1 128

void d_mputs(u8 *buf, u8 size);
void d_putchar2(char);    
void d_putchar(char);
void d_putstr(flash u8 *ptr);
void Init_UART(void);

void putchar(char);  
void putchar2(char);
void putstr(flash u8 *ptr);    
void mputs(u8 *buf, u8 size);
u8 ishexchar(u8 dat);

#endif
