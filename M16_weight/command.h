#ifndef _COMMAND_H_
#define _COMMAND_H_
                  
#include "define.h"
//Flags to indicate process of receiving a frame.
#define RF_IDLE         0
#define RF_HEAD1        1
#define RF_HEAD2        2
#define RF_ADDRFROM     3
#define RF_ADDRTO       4
#define RF_CMD          5
#define RF_DATALEN      6
#define RF_DATABUF      7
#define RF_CKSUM        8

// number of UART ports
#define STATE_MACHINE   1 

extern CM_INFRAME infrm[STATE_MACHINE];
extern CM_OUTFRAME outfrm[STATE_MACHINE];         
extern u8 RFlag[STATE_MACHINE];
void cm_pushc(u8 data,u8 port);
void cm_ack(u8 port);
void sleepms(u16 ms);
void cm_send(u8 addr, u8 cmdid, u8 port, u8 wait);  
void cm_forward(u8 iport,u8 oport);
void parse_node_frm(u8* base,u8 port);
void cm_process(u8 port);
void cm_query(u8 addr, u8 regid, u8 size, u8 port);
void cm_write(u8 addr, u8 regid, u8 size, u8 *dat, u8 port);

#endif
