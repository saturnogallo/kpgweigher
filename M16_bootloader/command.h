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

extern CM_INFRAME infrm;
extern CM_OUTFRAME outfrm;         
extern u8 RFlag;
void cm_pushc(u8 data);
void cm_ack(void);
void parse_node_frm(u8* base);
void cm_process(void);
#endif
