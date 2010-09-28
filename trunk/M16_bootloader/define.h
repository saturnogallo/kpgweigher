#ifndef _DEFINE_H_
#define _DEFINE_H_

#define i32 	long int
#define i16 	int
#define i8      char
#define u32 	unsigned long int
#define u16 	unsigned int
#define u8      unsigned char

#define MAX_RS485_ADDR 32
#define BOOT_FIRMWARE_REV 0x01
#define NULL 0x0
#define PASS 0x0 
#define TRUE 1
#define FALSE 0       
#define FAIL 0xff
#define BROADCAST_ADDR 0x80
#define EEPROM_RS485ADDR 0x80 
#define EEPROM_BOOTFLAG_ADDR 0x40                  /* The value must be set to the same as application firmware */
#define FIRMWARE_UPGRADE_CMD 0xA0                  /* The value must be set to the same as application firmware */

// define EEPROM Register bits
#define EERIE 3
#define EEMWE 2
#define EEWE 1
#define EERE 0

/*void sleepus(u16);  
void sleepms(u16);
void sleeps(u16);*/

// 定义硬件管脚名称
#define PIN_RUN           PORTD.4        
#define PIN_RxOK          PORTD.5
#define PIN_TxOK          PORTD.6
#define PIN_HWOK          PORTD.7
    
#define LED_ON(x)         x = 0
#define LED_OFF(x)        x = 1
#define LED_FLASH(x)      x = ~x
                                    
#define SPM_PAGESIZE 128                     /* M16's page size is 128 bytes */
#define DATA_BUFFER_SIZE (SPM_PAGESIZE + 2)  /* 2 CRC bytes at highest address */  
// Allow 
#define CM_INBUF_MAX    (DATA_BUFFER_SIZE + 1) // buf address (1 byte) + data buffer (128 bytes + 2 bytes CRC)
#define CM_OUTBUF_MAX   0x10

// bit 0:  "1" -> First Head Byte(0xfe) Found
// bit 1:  "1" -> Frame header (0xfe/0x68) Found
// bit 2:  "1" -> First Address Byte Received
// bit 3:  "1" -> Second Address Byte Received
// bit 4:  "1" -> Command Byte Received
// bit 5:  "1" -> Data Length Byte Received
// bit 6:  "1" -> Data and CheckSum Byte Received  "the sum of whole frame will be 0"
// bit 7:  "1" -> Complete Frame Received.
// 定义RS485 通讯命令帧格式：
typedef struct {
        u8      head_1;
        u8      head_2;
        u8      addr_from;
        u8      addr_to;
        u8      cmd;
        u8      datalen;
        u8      databuf[CM_OUTBUF_MAX];
        u8      cksum;
}CM_OUTFRAME;
#define NULL_OUTBUF      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

typedef struct  {
        u8      head_1;
        u8      head_2;
        u8      addr_from;
        u8      addr_to;
        u8      cmd;
        u8      datalen;
        u8      databuf[CM_INBUF_MAX];
        u8      cksum;
}CM_INFRAME;             
#define NULL_INBUF      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}


typedef struct {
    u8 upgrade_cmd;                     /* command sent from master to control firmware upgrade in node board */    
    u8 status;                          /* The second byte. This byte is initialized to 'B' when code is in bootloader */
                                        /* PC polls this byte to know whether MEGA16 is executing bootloader or application firmware */
                                        /* 'B' --- bootloader, Valid board id --- application firmware */
    u16 page_addr;                      /* specify which page to be written */
    u8 page_buffer[DATA_BUFFER_SIZE];   /* data sent from master board. to be programmed to flash, last 2 bytes are CRC bytes */
} BOOT_COMM_INTERFACE;                  /* boot communication interface with master board */

//typedef struct {
// u8 addr;                  /* node addr, set by primary firmware before switching to boot firmware, default 0x0:broadcasting */
 //u8 baud_rate;             /* serial baud rate, set by primary firmware, default: 9600 bps */
// u8 boot_cmd;              /* command to tell boot firmware to upgrade primary firmware or do other tasks */
// u8 reserved;
//} BOOT_CONFIG;             

#define CMD_IDLE 0


#endif
