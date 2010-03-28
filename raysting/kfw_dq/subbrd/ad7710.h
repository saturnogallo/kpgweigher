#ifndef __ad7710_H
#define __ad7710_H
#include "Typedef.h"
    

void delay (uint us)  ;
void delay1 (uint ms) ;
void ad_write(uchar lc);	 
void ad_main( void )   ;
void  UART_SENDBIT(uchar UART_data);
void uart_5bit();

extern uchar g_iAdc;
extern uchar g_iCurr;
extern uchar g_iPwr;
#endif