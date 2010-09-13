#ifndef  _COMMAND_H_
#define _COMMAND_H_
                  
#include "global.h"      



#define STATE_MACHINE   5

extern CM_INFRAME ginfrm[STATE_MACHINE];
extern CM_INFRAME ginfrm2[STATE_MACHINE];
extern CM_OUTFRAME outfrm[STATE_MACHINE];         
void sleepms(u16 ms);

void cm_pushA(u8 data);
void cm_pushB(u8 data);
void cm_pushC(u8 data);
void cm_pushD(u8 data);
void cm_pushPC(u8 data);
void cm_ack(u8 port);
void cm_forward(u8* frm,u8 oport);
void parse_node_frm(u8* base,u8* frm,u8 port);
void cm_process();
void cm_query(u8 addr, u8 regid, u8 size, u8 port);
void pc_query(u8 addr, u8 regid, u8 size, u8 port);
void cm_write(u8 addr, u8 regid, u8 size, u8 *dat, u8 port); 
void cm_block_write(u8 addr, u8 start_addr, u8 size, u8 *dat, u8 port);
void cm_report_b(u8 addr, u8 regid, u8 size, u8 *dat, u8 port);
void cm_report_w(u8 addr, u8 regid, u8 size, u8 *dat, u8 port);   

extern volatile u8  *infrmA;
extern volatile u8  *infrmB;
extern volatile u8  *infrmC;
extern volatile u8  *infrmD;
extern volatile u8  *infrmPC;    

#define use_infrmBin(port)       infrm##port  = (u8*)&ginfrm[SPORT##port]
#define use_infrm2Bin(port)      infrm##port =  (u8*)&ginfrm2[SPORT##port]


#endif
