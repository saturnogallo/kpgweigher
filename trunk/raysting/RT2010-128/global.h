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

void testA(uchar);
#define cm_pushC(data)  testA(data)
//#define PORT_3          SPORTA

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

//PORTB.7 RX, PORTB.6 RS, PORTB.5 1MA, PORTB.4. 0.1MA,
//PORTB.3 PT100, PORTB.2 PT1000, PORTB.1 CH1,  PORB.0 CH2    
#define RLYDELAY     60*ONEMS
#define SET_TORS     {PORTB.7 = 0; sleepms(RLYDELAY);PORTB.7 = 1;PORTB = 0xff;}
#define SET_TORX     {PORTB.6 = 0; sleepms(RLYDELAY);PORTB.6 = 1;PORTB = 0xff;}   
#define SET_TOCH2    {PORTB.5 = 0; sleepms(RLYDELAY);PORTB.5 = 1;PORTB = 0xff;}
#define SET_TOCH1    {PORTB.4 = 0; sleepms(RLYDELAY);PORTB.4 = 1;PORTB = 0xff;}      
#define SET_TOPT1000 {PORTB.3 = 0; sleepms(RLYDELAY);PORTB.3 = 1;PORTB = 0xff;}
#define SET_TOPT100  {PORTB.2 = 0; sleepms(RLYDELAY);PORTB.2 = 1;PORTB = 0xff;}
#define SET_TO1MA    {PORTB.1 = 0; sleepms(RLYDELAY);PORTB.1 = 1;PORTB = 0xff;}
#define SET_TOP1MA   {PORTB.0 = 0; sleepms(RLYDELAY);PORTB.0 = 1;PORTB = 0xff;}
//#define SET_TORX     display_buttons(KEY_RS,1)
//#define SET_TORS     display_buttons(KEY_RS,0)
                              
double nav_read();

void scanner_set_mode();
#endif
