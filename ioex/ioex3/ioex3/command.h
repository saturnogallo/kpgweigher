
#include "ioex.h"

u16 checksum(u8 *buf, u8 size);
void parse_frame(u8* ptr);
void cm_process();
void cm_pushc(u8 c);
void cm_ack();
void cm_query(u8 addr, u8 regid, u8 size);
void cm_write(u8 addr, u8 regid, u8 size, u8 *dat);

#define read_sysreg_b(name)	cm_query(0xff,(u8)((u8*)&Sysboard.##name-(u8*)&Sysboard),sizeof(u8))
#define read_sysreg_w(name)	cm_query(0xff,(u8)((u8*)&Sysboard.##name-(u8*)&Sysboard),sizeof(u16))

#define read_nodereg_b(id,name)	cm_query(RS485_Node[id].addr,(u8)((u8*)&RS485_Node[id].##name-(u8*)&RS485_Node[id]),sizeof(u8))
#define read_nodereg_w(id,name)	cm_query(RS485_Node[id].addr,(u8)((u8*)&RS485_Node[id].##name-(u8*)&RS485_Node[id]),sizeof(u16))

#define write_sysreg_b(name)	cm_write(0xff,(u8)((u8*)&Sysboard.##name-(u8*)&Sysboard),sizeof(u8),(u8*)&Sysboard.##name)
#define write_sysreg_w(name)	cm_write(0xff,(u8)((u8*)&Sysboard.##name-(u8*)&Sysboard),sizeof(u16),(u8*)&Sysboard.##name)

#define write_nodereg_b(id,name)	cm_write(RS485_Node[id].addr,(u8)((u8*)&RS485_Node[id].##name-(u8*)&RS485_Node[id]),sizeof(u8),(u8*)&RS485_Node[id].##name)
#define write_nodereg_w(id,name)	cm_write(RS485_Node[id].addr,(u8)((u8*)&RS485_Node[id].##name-(u8*)&RS485_Node[id]),sizeof(u16),(u8*)&RS485_Node[id].##name)
