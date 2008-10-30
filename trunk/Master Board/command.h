#ifndef  _COMMAND_H_
#define _COMMAND_H_
                  
#include "global.h"      

#define RF_IDLE         0
#define RF_HEAD1        1
#define RF_HEAD2        2
#define RF_ADDRFROM     3
#define RF_ADDRTO       4
#define RF_CMD          5
#define RF_DATALEN      6
#define RF_DATABUF      7
#define RF_CKSUM        8

#define STATE_MACHINE   5

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
void pc_query(u8 addr, u8 regid, u8 size, u8 port);
void cm_write(u8 addr, u8 regid, u8 size, u8 *dat, u8 port);     
void cm_report_b(u8 addr, u8 regid, u8 size, u8 *dat, u8 port);
void cm_report_w(u8 addr, u8 regid, u8 size, u8 *dat, u8 port);   
void handle_pending_read();
extern u8 FB_in_up;
extern u8 FB_out_up;
extern u8 FB_in_dn;
extern u8 FB_out_dn;
#define checkforward_up()        if(FB_in_up != FB_out_up)      { cm_forward(FB_in_up, FB_out_up);FB_in_up = FB_out_up;}
#define checkforward_dn()        if(FB_in_dn != FB_out_dn)      { cm_forward(FB_in_dn, FB_out_dn);FB_in_dn = FB_out_dn;}

#endif
