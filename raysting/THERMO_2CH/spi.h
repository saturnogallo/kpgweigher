// header files for spi.c
void SPI_MasterInit(void);              // Initialize SPI to master mode, set clock rate etc
void SPI_MasterTransmit_c(unsigned char);    // send data to SPI slave and receive slave feedback
void SPI_MasterTransmit_w(unsigned int);
void SPI_MasterTransmit_l(unsigned long int); 

unsigned char SPI_MasterReceive_c(void);
//u16 SPI_MasterRecieve_w(void);
unsigned long int SPI_MasterReceive_l(void);