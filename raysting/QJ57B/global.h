// global.h    
#ifndef _GLOBAL_H_
#define _GLOBAL_H_             
#include <mega128.h>
#include "typedef.h"

#define LED_ON(addr)       addr=0;
#define LED_OFF(addr)      addr=1;  
#define LED_FLASH(addr)    addr=~addr;
                     

#define SPORTA       0
#define SPORTB       1
#define SPORTC       2
#define SPORTD       3
#define SPORT0       4
#define SPORT1       5                     
            

#define DEBUG        0
/*
        Port related definition
*/ 

void testA(uchar); //dummy handler
#define cm0_push(data)  testA(data)
#define PORT_KBD        SPORT0

void scanner_uart_push(uchar);
#define cm_pushB(data)  scanner_uart_push(data)
#define PORT_SCANNER    SPORTC

void pc_uart_push(uchar);
#define cm_pushA(data)  pc_uart_push(data)
#define PORT_PC         SPORTD              


void nav_uart_push(uchar);
#define cm1_push(data)  nav_uart_push(data)
#define PORT_NAV        SPORT1

void relay_uart_push(uchar);
#define cm_pushC(data)  relay_uart_push(data)
#define PORT_RELAY          SPORTA

#define cm_pushD(data)  testA(data)
//#define PORT_POWER          SPORTB
// Hardware related

#define MASK_TMR0_INT()  TIMSK &= 0xFE  
#define ENABLE_TMR0_INT() TIMSK |= 0x1
#define CLR_TOV0()  TIFR |= 0x1                   /* write "1" to clear TOV0 bit */
#define Set_10ms_Tick() TCNT0 = 122               /* 10ms interrupt */
#define TMR0_Is_Enabled() (TIMSK & 0x1)  // bit 0 TOIE0
                            
void sleepms(u16 ms);

double nav_read();

void scanner_set_mode();
#endif
