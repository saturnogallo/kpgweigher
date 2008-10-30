// header files for spi.c
#include "define.h"
void SPI_MasterInit(void);              // Initialize SPI to master mode, set clock rate etc
void SPI_MasterTransmit_c(u8);    // send data to SPI slave and receive slave feedback
void SPI_MasterTransmit_w(u16);
void SPI_MasterTransmit_l(u32); 

u8 SPI_MasterReceive_c(void);
//u16 SPI_MasterRecieve_w(void);
u32 SPI_MasterReceive_l(void);