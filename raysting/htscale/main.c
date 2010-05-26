#include "define.h"
sbit BEEP = P2^3;
void main()
{
 u8 buf[7] = {0x1,0x1,0x1,0x1,0x1,0x5,0x10};
 u8 prt[7];
 u32 key=0;
 BEEP = 0;
 PT6312_Init();
 ds1302_init();

 prt[6] = '\0';


 while(1)
{
 ds1302_get(buf); //// 秒 分 时 日 月 星期 年
 key = PT6312_ReadKey();
 if(key != 0)
 {
	prt[5] = lowc(buf[0]);
	prt[4] = highc(buf[0]);
	prt[3] = lowc(buf[1]);
	prt[2] = highc(buf[1]);
	prt[1] = lowc(buf[2]);
	prt[0] = highc(buf[2]);
 }else{
	prt[5] = lowc(buf[3]);
	prt[4] = highc(buf[3]);
	prt[3] = lowc(buf[4]);
	prt[2] = highc(buf[4]);
	prt[1] = lowc(buf[6]);
	prt[0] = highc(buf[6]);
 }
 PT6312_Print(prt,6);
 PT6312_Refresh(); 
}

}

