#ifndef	_SCANNER_H_
#define _SCANNER_H_
#include "utili.h"
#define NAV_INIT        1
#define NAV_30V         2
#define NAV_1V          3
#define NAV_120MV       4
#define NAV_AFLTOFF     5
#define NAV_AFLTON      6
#define NAV_SLOWMODE    7
#define NAV_ZEROON      8
#define NAV_READ        9
void scanner_set_channel(uchar ch);
void scanner_uart_push(uchar data);
void pc_uart_push(uchar data);
void nav_uart_push(uchar data);     
void nav_command(uchar cmd);
#endif
