#ifndef _DRVEEPROM_H_ 
#define _DRVEEPROM_H_ 
#include "define.h"

#ifdef  _DRVEEPROM_C_ 
#define EXTERN 
#else 
#define EXTERN  extern 
#endif 

typedef struct 
{ 
    u16 wAddress; 
    u8 bVal; 
} S_EEPROM_POSITION; 

#define EEPROM_WRITE_BUF_SIZE   64 
#define RET_BUSY 0xff
#define RET_SUCCESS 0x0  

u8 bWriteData2Eeprom(u16 wAddress, u8 *bVal, u8 totalsize);
u8 bWriteData2Eeprom_c(u16 wAddress, u8 bVal);     

u8 bReadDataFromEeprom(u16 wAddress, u8 *pbVal, u8 size); 
u8 bReadDataFromEeprom_c(u16 wAddress, u8 *pbVal); 
void vInitEeprom(void); 

#undef  EXTERN 
#endif 

