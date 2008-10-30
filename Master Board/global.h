// global.h    
#ifndef _GLOBAL_H_
#define _GLOBAL_H_             

#define i32    long int
#define i16    int
#define i8     char
#define u32    unsigned long int
#define u16    unsigned int
#define u8     unsigned char

#define NULL 0x0

//#define RUN	           PORTD.7;
#define LED_ON(addr)       addr=0;
#define LED_OFF(addr)      addr=1;
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
#define CM_OUTBUF_MAX   0x10

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
#define GROUPA       0
#define GROUPB       1
#define GROUPC       2
#define GROUPD       3
#define GROUPPC      4
#define R_NOWAIT     0xff               

#define STATE_BEIDLE            0
#define STATE_DONE_FAIL         0xfe
#define STATE_DONE_OK           0xfd
#define BOARD_TYPE_MASK         0xf0                          
#define BOARD_TYPE_VIBRATE     	0x00
#define BOARD_TYPE_WEIGHT       0x10
#define BOARD_TYPE_INVALID      0xf0 

#define BOARD_GROUP_MASK        0x0f
#define BOARD_GROUP_MASK2       0x07
#define BOARD_GROUP_A           0x00
#define BOARD_GROUP_B           0x01
#define BOARD_GROUP_C           0x02
#define BOARD_GROUP_D           0x03
#define BOARD_MISSING_BIT       0x08
#define BOARD_GROUP_NONE	0x0f
#endif
