// global.h    
#ifndef _GLOBAL_H_
#define _GLOBAL_H_             
#include <mega64.h>
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
            
#define MARK_HEAD        0xAA
#define MARK_TAIL        0x55

#define STATUS_IDLE      0xF0
#define STATUS_ERR       0xF1
#define STATUS_DRAW      0xF2
#define STATUS_DONE      0xF3
       
#define CMDO_DISPLAYBTN  0x02
#define CMDO_LCD_INIT    0x03
#define CMDO_LCD_CLS     0x04
#define CMDO_LCD_REVERSE 0x05
#define CMDO_LCD_CLSBLK  0x06
#define CMDO_LCD_RECT    0x07
#define CMDO_LCD_HZ12    0x08
#define CMDO_LCD_HZ16    0x09
#define CMDO_LCD_68      0x10
#define CMDO_LCD_816     0x11
#define CMDO_TOIDLE      0x12

#define DEBUG        0
/*
        Port related definition
*/ 
void kbd_uart_push(uchar);

#define cm0_push(data)  kbd_uart_push(data)
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
#define PORT_RELAY      SPORTA

void testA(uchar);
#define cm_pushD(data)  testA(data)
//#define PORT_B          SPORTB
// Hardware related

#define MASK_TMR0_INT()  TIMSK &= 0xFE  
#define ENABLE_TMR0_INT() TIMSK |= 0x1
#define CLR_TOV0()  TIFR |= 0x1                   /* write "1" to clear TOV0 bit */
#define Set_10ms_Tick() TCNT0 = 122               /* 10ms interrupt */
#define TMR0_Is_Enabled() (TIMSK & 0x1)  // bit 0 TOIE0
                            
void sleepms(u16 ms);

#define SET_PKTT     display_buttons(KEY_KTT,1)
#define SET_NKTT     display_buttons(KEY_KTT,0)
#define SET_TORX     display_buttons(KEY_RS,1)
#define SET_TORS     display_buttons(KEY_RS,0)
                              
double nav_read();

void scanner_set_mode();
#endif
