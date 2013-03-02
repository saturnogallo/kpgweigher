#ifndef _DEFINE_H_
#define _DEFINE_H_

//My compile switches here: enabled when uncommented
#define _50HZ_FILTER_           //50HZ AC power supply. uncommented if 60HZ.
#define _DISABLE_WATCHDOG_    //uncommented to disable watchdog

                              
#define MAX(a,b)   (a>b?a:b)
#define MIN(a,b)   (a<b?a:b)


#define i32    long int
#define i16    int
#define i8     char
#define u32    unsigned long int
#define u16    unsigned int
#define u8     unsigned char
#define uchar  unsigned char
#define uint   unsigned int
#define ulong  unsigned long
#define bool   unsigned char
#define BOOL   unsigned char


//#define NULL 0x0
#define FALSE 0
#define TRUE  1

#define false      0
#define true      1

#ifndef NULL
#define NULL      0
#endif
#define DEBUG     0
/*****************************************************************************************/
//  Define MCU register bits
/*****************************************************************************************/
// define EEPROM Register bits
#define EERIE 3
#define EEMWE 2
#define EEWE 1
#define EERE 0
#define SPI_SS PORTB.4               /* SPI slave slect pin */                                     
#define P_SS 4                       /* slave_select: PORTB.4 */
#define P_MOSI 5                     /* master_output_slave_input: PORTB.5 */
#define P_MISO 6                     /* master_input_slave_output: PORTB.6 */
#define P_SCK 7                      /* SPI clock: PORTB.7 */ 

#define SPIE 7
#define SPE 6 
#define MSTR 4
#define SPIF 7

// define timer overflow mask bit
#define TOV2 6
#define TOV1 2
#define TOV0 0

// define watch dog register bits
#define WDE 3
#define WDCE 4


// used in debug mode, define delay (ms) between sending 2 
#define UART_DELAY 50    //

void sleepus(u16);
void sleepms(u16);
void sleeps(u16);

   
#define LED_ON(x)         x = 0
#define LED_OFF(x)        x = 1
#define LED_FLASH(x)      x = ~x
                                    
void digi_print(u8 *str,uchar startpos, uchar size);



#endif
