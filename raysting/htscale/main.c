#include "define.h"
#include "stdio.h"

sbit BEEP = P2^3;
extern u32 CS5532_Init();
extern void sleepms(u8);

#define CH2_SETUP 	((u16)B_CH2<<10) |\
					((u16)GAINX1<<11) |\
				 	((u16)WORD_RATE_12P5SPS<<7) |\
					((u16)B_BIPOLAR<<6) |\
					((u16)B_LATCH<<4) |\
					((u16)B_NODELAY<<4) |\
					B_OG_CH2
void main()
{
 u8 buf[7] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7};
 u8 prt[7];
 u32 key=0;

/*
 u8 r;
 r = P4SW;
 P4SW = r | 0x70;
*/
 BEEP = 0;

 //ds1302_init();
 PT6312_Init();

 //verify eeprom
 Sector_Erase(DATA_FLASH_START_ADDRESS);
 eeprom_write(DATA_FLASH_START_ADDRESS,buf,7);
 eeprom_read(DATA_FLASH_START_ADDRESS,prt,7);
 if((prt[0] == 0x01) && (prt[6] == 0x07))
 {
 	PT6312_Print("OK   ",5);
	while(1)
	{
	 	PT6312_Refresh(); 
	}
 }



 if(init_cs5532(CH2_SETUP) == 0xff)
 {
 	PT6312_Print("AD-ERR",6);
	while(1)
	{
	 	PT6312_Refresh(); 
	}
 }

 prt[6] = '\0';
 while(1)
 {
 	key = read_cs5532();
	key = key>>8;

 	prt[5] = lowc(key & 0x000000ff);
 	prt[4] = highc(key & 0x000000ff);
    key = key>>8;
 	prt[3] = lowc(key & 0x000000ff);
 	prt[2] = highc(key & 0x000000ff);
	key = key >> 8;
 	prt[1] = lowc(key & 0x000000ff);
 	prt[0] = highc(key & 0x000000ff);

 	PT6312_Print(prt,6);
 	PT6312_Refresh(); 
	sleepms(20);
 }

 
 while(1)
{
// ds1302_get(buf); //// 秒 分 时 日 月 星期 年 in BCD code
 PT6312_Print(prt,6);
 PT6312_Refresh(); 
}

}

