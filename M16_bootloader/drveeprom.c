/************************************************************************/
// MEGA8 EEPROM Read and Write subroutines
// Interrupt mode is used to avoid wait during EEPROM writing
// Header files needed: 
// drveeprom.h:
// mega8.h: MEGA8 related constant
/************************************************************************/
#define _DRVEEPROM_C_ 

#include "drveeprom.h" 
#include <mega16.h>                     
#include "define.h"

/************************************************************************/

// define data buffer queue for EEPROM read/write
static S_EEPROM_POSITION volatile _sEepromBuf[EEPROM_WRITE_BUF_SIZE]; 

// EEPROM buffer write pointer/index(user writes to RAM buffer)
static u8 volatile _bEepromBufWrPtr; 

// EEPROM buffer read pointer(read data from buffer and write to EEPROM)
static u8 volatile _bEepromBufRdPtr; 

// number of data in EEPROM writing buffer
static u8 volatile _bEepromBufNs; 

#define fgEepromBufEmpty()      (_bEepromBufNs == 0) 
#define fgEepromBufFull()       (_bEepromBufNs == EEPROM_WRITE_BUF_SIZE) 


/************************************************************************/
//               Write EEPROM using EEPROM Interrupt
// if buffer is empty, stop EEPROM interrupt
// if buffer is not empty(still some data to be written into EEPROM), 
// write EEPROM.  The buffer here is a queue.
/************************************************************************/

//#pragma interrupt_handler vIvEeReady:iv_EE_READY 
interrupt [EE_RDY] void vIvEeReady(void) 
{ 
    if(!fgEepromBufEmpty()){ 
        while(EECR & (1<<EEWE));  /* wait for last write to complete */
        EEAR = _sEepromBuf[_bEepromBufRdPtr].wAddress; 
        EEDR = _sEepromBuf[_bEepromBufRdPtr].bVal; 
        EECR |= (1<<EEMWE); 
        EECR |= (1<<EEWE); 
        _bEepromBufRdPtr++; 
        if(_bEepromBufRdPtr >= EEPROM_WRITE_BUF_SIZE) 
            _bEepromBufRdPtr = 0; 

        _bEepromBufNs--; 
    }else{   // disable EEPROM interrupt if no more data to be written into EEPROM
        EECR &= ~(1<<EERIE); 
    } 
}    

/************************************************************************/
//                     Write data into EEPROM buffer
// If buffer is not full, write data into buffer, return RET_SUCCESS(0x0)
// flag. If buffer is full, return RET_BUSY(0xff) flag
/************************************************************************/ 

u8 bWriteData2Eeprom_c(u16 wAddress, u8 bVal) 
{ 
    if(fgEepromBufFull()) 
        return RET_BUSY; 
    // add for EEPROM protection
    //if(!EN_EEPROM_WRITE)
    //   return RET_SUCCESS;

    // full file buffer if buffer is not full yet
    #asm("cli");     // disable all interrupt
    _sEepromBuf[_bEepromBufWrPtr].wAddress = wAddress; 
    _sEepromBuf[_bEepromBufWrPtr].bVal = bVal; 
    _bEepromBufWrPtr++; 
    if(_bEepromBufWrPtr >= EEPROM_WRITE_BUF_SIZE) 
        _bEepromBufWrPtr = 0; 

    _bEepromBufNs++; 
    
    EECR |= (1<<EERIE); 
    #asm("sei"); // enable EEPROM ready interrupt and global interrupts 

    return RET_SUCCESS; 
} 
u8 bWriteData2Eeprom(u16 wAddress, u8 *bVal, u8 totalsize){
  u8 uc_eeprom_data;
  while(totalsize > 0){
          if(bReadDataFromEeprom_c(wAddress,&uc_eeprom_data) != RET_SUCCESS){
                return totalsize;
          }
          // skip programming if data to be written is the same as EEPROM data 
          if(uc_eeprom_data != *bVal) {
	         if(RET_SUCCESS != bWriteData2Eeprom_c(wAddress,*bVal))
	                return totalsize;       //wait until the write succeed
	  }
          totalsize--; // already programmed one, number of data left decreases by 1
          wAddress++;
          bVal++;
  }
  return totalsize;  
}


/************************************************************************/
//                         Read data from EEPROM 
// The programer must ensure EEPROM is NOT in writing state before reading
// EEPROM. This subroutine return RET_BUSY(0xff) flag when EEPROM is in 
// writting state. Return RET_SUCCESS(0x0) flag if reading successfully
// Data readout is saved into read buffer.
/************************************************************************/ 
u8 bReadDataFromEeprom(u16 wAddress, u8 *pbVal,u8 size)
{
	while(size-- > 0){
		if(bReadDataFromEeprom_c(wAddress++,pbVal++) != RET_SUCCESS) 
		        return RET_BUSY;
	}
	return RET_SUCCESS;
}	
u8 bReadDataFromEeprom_c(u16 wAddress, u8 *pbVal) 
{ 
    if(EECR & (1<<EEWE)) 
        return RET_BUSY; 

    EEAR = wAddress; 
    EECR |= (1<<EERE); 
    *pbVal = EEDR; 
    return RET_SUCCESS; 
} 

/************************************************************************/
//                        Initialize EEPROM
// Intialize buffer variables. Diable EEPROM interruption
// EEPROM interrupt is enabled after writing data to EEPROM buffer.
/************************************************************************/ 
void vInitEeprom(void) 
{ 
    EECR = 0x00; 
    _bEepromBufWrPtr = 0; 
    _bEepromBufRdPtr = 0; 
    _bEepromBufNs    = 0; 
} 

