// global.h    
#ifndef _GLOBAL_H_
#define _GLOBAL_H_             

#define i32    long int
#define i16    int
#define i8     char
#define u32    unsigned long int
#define u16    unsigned int
#define u8     unsigned char

//#define TEST_SERIAL_PORTS //test if com0/com1 is in good condition

#define NULL 0x0
#define FALSE 0
#define TRUE  1

//#define RUN	           PORTD.7;

#define LED_ON(addr)       addr=0;
#define LED_OFF(addr)      addr=1;  
#define LED_FLASH(addr)    addr=~addr;
#define LED_RUN            PORTB.7
#define LED_PC             PORTB.6
#define LED_NODE           PORTB.5
#define LED_RST554         PORTB.4
                     
#define RF_IDLE         0
#define RF_HEAD1        1
#define RF_HEAD2        2
#define RF_ADDRFROM     3
#define RF_ADDRTO       4
#define RF_CMD          5
#define RF_DATALEN      6
#define RF_DATABUF      7
#define RF_CKSUM        8

#define USE_COM1
// USART Baud rate: 115200 (0x7), 57600 (0xF) 38400 (0x17)
// USART Baud rate: 19200 (2F) 9600(0x5F)
#define BAUD_TO_USE     0x7

//define RS485 node parameter structure 
#define MAX_NODE_NUM       36               
#define MAX_VIBR_NUM       4

typedef struct 
{                    
  u8  addr;           //0 RS485 node address
  u8  board_property; //1 RS485 node board function identification, 

  u16 Mtrl_Weight_gram;//2 Goods weight of this node  
  u8  Mtrl_Weight_decimal; //4
  u8  status;         //5 status of current node.    
  u8  cs_status;      //6
  u8  hw_status;      //7    
  u8  uart_port;     // node connected to which port of 16C554(A/B/C/D)
  u8  fail_counter;   // how many times this node hasn't been selected
} NODE;

#define NREG_BOARD       1
#define NREG_STATUS	 50
#define NREG_CS_STATUS	 51
#define NREG_HW_STATUS	 52
#define NREG_WEIGHT      47	
#define NREG_RESET       64
#define NREG_ENABLE      65         
#define NREG_DISABLE     66                                   
#define NREG_RELEASE     67
#define NREG_GOON        68
                                  
#define NREG_SIZE_TOTAL	 80 //sizeof(all the regs in node)

#define MREG_INVALID     0xff
extern flash u8 n2m_map[NREG_SIZE_TOTAL];
//define system struct
typedef struct
{ //status from pc
  u16 target_weight[MAX_VIBR_NUM];          //0 	// system target weight of goods  
  u16 offset_up_limit[MAX_VIBR_NUM];        //8 	// offset upper limit
  u16 offset_lo_limit[MAX_VIBR_NUM];        //16 	// offset lower limit
  u8 flag_goon[MAX_VIBR_NUM];		    //24	
  //summary of system
  u8 node_num;                 		    //28	// how many weight node (head) in this system
  u8 vibrator_num;             		    //29	// number of Vibrator detected in the system
  u8 running[MAX_VIBR_NUM];		    //30	
  //command register
  u8 flag_start_machine[MAX_VIBR_NUM];       //34 // Set when starting machine command is received.
  u8 flag_stop_machine[MAX_VIBR_NUM];        //38
  u8 flag_search[MAX_VIBR_NUM];		     //42
  u8 flag_report;			     //46
  u8  reserved1;                             //47  -- aligned
  u16 packer_config1;                        //48  -- word aligned
  u16 packer_config2;                        //50      
  u16 reserved2;                             //52
  u16 reserved3;                             //54
}SYSTEM;

extern u8 true_val;
extern u8 false_val;
#define set_cmd(cmd,id)          cm_write(RS485_Node[##id].addr,cmd,1,&true_val,RS485_Node[##id].uart_port)
#define unset_cmd(cmd,id)          cm_write(RS485_Node[##id].addr,cmd,1,&false_val,RS485_Node[##id].uart_port)
//v means vibrator bucket
#define vset_cmd(cmd,id)         cm_write(vibrator[##id].addr,cmd,1,&true_val,vibrator[##id].uart_port)
#define vunset_cmd(cmd,id)         cm_write(vibrator[##id].addr,cmd,1,&false_val,vibrator[##id].uart_port)
// bit 0:  "1" -> First Head Byte(0xfe) Found
// bit 1:  "1" -> Frame header (0xfe/0x68) Found
// bit 2:  "1" -> First Address Byte Received
// bit 3:  "1" -> Second Address Byte Received
// bit 4:  "1" -> Command Byte Received
// bit 5:  "1" -> Data Length Byte Received
// bit 6:  "1" -> Data and CheckSum Byte Received   (the sum of the total frame will be zero)
// bit 7:  "1" -> Complete Frame Received.

#define CM_INBUF_MAX    0x10          
#define CM_OUTBUF_MAX   0x84   /* max size used by burst write: 1-byte regid,128-byte data buf, 2-byte CRC */

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
#define FRM_POS_HEAD1   0
#define FRM_POS_HEAD2   1
#define FRM_POS_FROM    2
#define FRM_POS_TO      3
#define FRM_POS_CMD     4
#define FRM_POS_DLEN    5
#define FRM_POS_DBUF    6
#define FRM_POS_CKSUM   22

#define NULL_OUTBUF {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

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
#define NULL_INBUF   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define MY_ADDR      0xff
#define PC_ADDR	     0x00
#define SPORTA       0
#define SPORTB       1
#define SPORTC       2
#define SPORTD       3
#define SPORT_TOTAL  4
#define SPORTPC      4
#define R_NOWAIT     0xff               

#define STATE_BEIDLE            0
#define STATE_DONE_FAIL         0xfe
#define STATE_DONE_OK           0xfd
#define STATE_BUSY              0xfc
#define CMD_START_MACHINE	0x01
#define CMD_START_SEARCH	0x02
#define CMD_RESET_16C554        0x03            
#define CMD_REBOOT_ME           0x04  
#define CMD_PACKER_BUSY         0x05    //check until the packer is busy or not, IDLE means busy, PASS means free
#define CMD_PACKER_INIT         0x06    //init the packer
#define CMD_PACKER_AVAILABLE    0x07    //combination available
#define CMD_PACKER_DONE         0x08    //release is done

#define BOARD_TYPE_MASK         0xf0                          
#define BOARD_TYPE_VIBRATE     	0x00
#define BOARD_TYPE_WEIGHT       0x10
#define BOARD_TYPE_INVALID      0xf0 

#define MY_INVALID_DATA         0xfff1                                                      
#define AD_INVALID_DATA         0xfffe
#define AD_BUSY                 0xfffd
#define AD_FILTER               0xfffb
#define BOARD_GROUP_MASK        0x0f
#define BOARD_GROUP_MASK_SHORT  0x07
#define BOARD_GROUP_A           0x00
#define BOARD_GROUP_B           0x01
#define BOARD_GROUP_C           0x02
#define BOARD_GROUP_D           0x03
#define BOARD_MISSING_BIT       0x00	//disable the missing bit now
#define BOARD_GROUP_NONE	0x0f          
               

#define INTF_PLS_WIDTH 30 
#define MAX_SVS_NUM 8 
#define SVS_CMPLT 0xff 
#define PACKER_OF_MASK      0x0018
#define PACKER_IF_MASK      0x0060
#define NEED_HANDSHAKE      (packer_config & 0x0080)
#define ASYN_IF_MASK        0x0002
#define INIT_MASK           0x0001 
#define SIGNAL_DELAY_MASK   0x0004
#define INTF_MODE_SHAKEHANDS  (CONFIG_REG & 0x80)

#define CONFIG_REG      packer_config
// Hardware related
#define PORT_OR1 0                                /* PORTF.0 */
#define PORT_OF1 1                                /* PORTF.1 */
#define PORT_FR  2                                /* PORTF.2 */

#define TOGGLE_OR1 PORTF ^= 0x1                   /* PORTF.0 */
#define TOGGLE_OF1 PORTF ^= 0x2                   /* PORTF.1 */
#define TOGGLE_OFR1 PORTF ^= 0x4                  /* PORTF.2 */
#define MASK_TMR0_INT()  TIMSK &= 0xFE  
#define ENABLE_TMR0_INT() TIMSK |= 0x1
#define CLR_TOV0()  TIFR |= 0x1                   /* write "1" to clear TOV0 bit */
#define Set_10ms_Tick() TCNT0 = 122               /* 10ms interrupt */
#define TMR0_Is_Enabled() (TIMSK & 0x1)  // bit 0 TOIE0

// Xmodem control characters
#define XMODEM_NUL 0x00 
#define XMODEM_SOH 0x01 
#define XMODEM_STX 0x02 
#define XMODEM_EOT 0x04 
#define XMODEM_ACK 0x06 
#define XMODEM_NAK 0x15 
#define XMODEM_CAN 0x18 
#define XMODEM_EOF 0x1A 
#define XMODEM_RECIEVING_WAIT_CHAR 'C' 

typedef struct {
    u8 upgrade_cmd;                     /* command sent from master to control firmware upgrade in node board */
    u8 status;                          /* firmware upgrade status, polled by master board */
    
    u16 page_addr;                      /* specify which page to be written */
    u8 page_buffer;                     /* databuf, actually this is an array in node.*/
} BOOT_COMM_INTERFACE;                  /* boot communication interface with master board */
              
//packer related function     
void Init_interface();
u8 Packer_Is_Busy();
void Tell_Packer_Weigher_Rdy();
u8 Tell_Packer_Release_Done();
extern u16 packer_config;
void nfu_process_node_feedback();
#endif
