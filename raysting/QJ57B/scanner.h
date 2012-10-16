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
void navto1v();        
void navto120mv();
void turn_k(unsigned int k);
void relaystate(uchar mode);    
double nav_read_stable();
unsigned int calc_capture_nextk();
unsigned int calc_track_nextk();
//Relay position const
//the high 5 bits of mode is for relay control
//the low 3 bit is for 7 current index, 0 means no change
//there are 4 relay, K1/K2/K3/KTT 
#define RELAYSTATE_K1   0x10
#define RELAYSTATE_K2   0x20
#define RELAYSTATE_K3   0x40
#define RELAYSTATE_KTT  0x80
#define RELAYSTATE_SQR  0x08
#define RELAYCURR_P01   0x01
#define RELAYCURR_P02    0x02
#define RELAYCURR_P05     0x03
#define RELAYCURR_P1    0x04
#define RELAYCURR_1   0x05
#define RELAYCURR_3    0x06
#define RELAYCURR_10   0x07

#define RLYMODE_VRS     (RELAYSTATE_K1 | RELAYSTATE_KTT)
#define RLYMODE_VRX     (RELAYSTATE_K2 | RELAYSTATE_KTT)
#define RLYMODE_TRACKP  (RELAYSTATE_K3 | RELAYSTATE_KTT)
#define RLYMODE_TRACKN  (RELAYSTATE_K3 )

#endif
