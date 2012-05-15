#ifndef _DEFINE_H_
#define _DEFINE_H_

/*************************************************************************************
*                           Firmware Version History
* -----------------------------------------------------------------------------------
* 0x16:
* 0x17: change TRIAC trigger pulse width from 0.31ms to 0.07ms. 
* 0x18: 1) adjust magnet amplitude in ana_comp_isr to reduce weight at zero amp
*       2) skip magnet drive if current weight exceeds or equal to target.
*       3) dump runtime amplitude to hw_status for monitor purpose.
* 0x1A: 1) resolved a bug in main subroutine:
           if(RS485._global.flag_release){} else{} branch.
* 0x1B: 1)   

* 0x1D: 1) change weight limit to target_weight  
* 0x1E: 1) fix interface timing with packer
* 0x1F  1) fix packer delay issue   
* 0x20: 1) improvement for central magnet
* 0x21: 1) fix for packer interface (interrupt may be disabled when slave mode is disabled)
*       2) fix a bug that when reset weight, updated cs_poise[] may not be programmed
*          into EEPROM and only cs_zero is updated.
*       3) target weight per group instead of unique target weight.
*       4) reposition motor before release if motor is not in the right position. 
* 0x22: 1) remove "target-weight-per-group" logic. This is implemented in software 
* 0X24: 1) Add more calibration data point. 
*************************************************************************************/
#define FIRMWARE_REV 0x03  //0x25 

/*************************************************************************************
*                               Compile Switches
*************************************************************************************/
#define _DISABLE_WATCHDOG_                     //uncommented to disable watchdog
//#define _BOARD_TYPE_IS_VIBRATE_15            
//#define _BOARD_TYPE_IS_VIBRATE_11
//#define _MOTOR_2WAY_


#define NUM_OF_CAL_POINT 10

/*************************************************************************************
*                               Alias of Basic Types
*************************************************************************************/
#define i32 long int
#define i16 int
#define i8  char
#define u32 unsigned long int
#define u16 unsigned int
#define u8  unsigned char

/*************************************************************************************
*                               Alias of Basic Results
*************************************************************************************/
#define NULL              0x0
#define PASS              0x0        
#define FAIL              0xff
#define TRUE              1
#define FALSE             0

/*************************************************************************************
*                               Hardware Abstract
*************************************************************************************/
#define PIN_RUN           PORTD.4        
#define PIN_RxOK          PORTD.5
#define PIN_TxOK          PORTD.6
#define PIN_HWOK          PORTD.7
    
#define LED_ON(x)         x = 0
#define LED_OFF(x)        x = 1
#define LED_FLASH(x)      x = ~x

/*************************************************************************************
*                               System Definitions
*************************************************************************************/
#define MAX_RS485_ADDR                  36   // Default addr of un-configured boards
#define EEPROM_MFG_ADDR               0xC0   // Starting addr of MFG calibration data
#define EEPROM_RS485ADDR              0x80   // Starting addr of board configuration 
                                             // data in EEPROM.
#define NUM_OF_DATA_TO_BE_PGMED       0xFF   // data to be programmed into EERPOM.

/*------------------------------define motor pulse numbers---------------------------*/
#define MOTOR_ONE_CYCLE_WITH_SENSOR    230 
#define HALF_CYCLE_CLOSE_WITH_SENSOR   130

#ifdef _MOTOR_2WAY_
#define MOTOR_SHIFT_CYCLE_OPEN         80
#else
#define MOTOR_SHIFT_CYCLE_OPEN        100
#endif

/*-----------------------------------Board Property----------------------------------*/
#define BOARD_TYPE_MASK               0xf0                          
#define BOARD_TYPE_VIBRATE            0x00
#define BOARD_TYPE_WEIGHT             0x10
#define BOARD_TYPE_MOTOR              0x20
#define BOARD_TYPE_INVALID            0xf0    

#define HW_ID_WEIGHT                  0x03   // Don't change, H/W dependant
#define HW_ID_INTERFACE               0x02   // Don't change, H/W dependant 

#define BOARD_GROUP_MASK              0x0f
#define BOARD_GROUP_A                 0x00
#define BOARD_GROUP_B                 0x01
#define BOARD_GROUP_C                 0x02
#define BOARD_GROUP_D                 0x03

/*************************************************************************************
*                           Bootloader Related Definitions
*************************************************************************************/
#define EEPROM_BOOTFLAG_ADDR          0x40   // Address of boot flag in EEPROM
#define UPGRADE_FIRMWARE              0x0A   // Firmware upgrade command word

/*************************************************************************************
*                              Weight Query Returns
*************************************************************************************/
#define INVALID_DATA                0xfffe   // 65534 
#define AD_BUSY                     0xfffd   // 65533
#define AD_OVER_FLOW                0xfffc   // 65532, most times Mechanical errors
#define FILTER_ONGOING              0xfffb   // 65531, 
#define BAD_CALIBRATION             0xfffa   // 65530, calibration data error
#define OVERWEIGHT                  0xfff9   // 65529,
#define NEGATIVE_WEIGHT             0xfff8   // 65528, weight value is negative 
#define MAX_VALID_DATA              0xfff0

/*************************************************************************************
*                       RS485 Communication Protocol Definition
*************************************************************************************/
// bit 0:  "1" -> First Head Byte(0xfe) Found
// bit 1:  "1" -> Frame header (0xfe/0x68) Found
// bit 2:  "1" -> First Address Byte Received
// bit 3:  "1" -> Second Address Byte Received
// bit 4:  "1" -> Command Byte Received
// bit 5:  "1" -> Data Length Byte Received
// bit 6:  "1" -> Data and CheckSum Byte Received  "the sum of whole frame will be 0"
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

// Define RS485 node parametrics(Registers)
// initialized in main.c as {0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0}, //flash data
// {0,0,0,100,0x1ABC,0xBC,200,50,0,3,0,9,50,1,0,0,0,0,0,0,0}
// Data in struct "S_FLASH" will be programmed into EEPROM
// Registers will be initialized as data saved in EEPROM after power up.
// If data readout from EEPROM is invalid, registers will be initialized by code.
// Registers can be overwritten or inquired by master board.
// Regsiter settings can be written into EEPROM after receiving writing Flash cmd 
// from PC. Both group and individual saving options are available on PC software.
// TEST MODE: To Be Developped (by forcing a pin to low/high)
typedef struct {
        // RS485 address, 0x0 is broadcasting address.
        // valid address range 1~36, wherein address 1~3 are reserved for spare board. 
        volatile u8      addr;           
        // Board Function: WEIGHT_MOTOR_VIBRATION or WEIGHT_VIBRATION (the top one)
        // Valid type, 0x0~0x15. Default to "0" (WEIGHT_MOTOR_VIBRATION).
        // Bit 0 of "hw_status" will be set if type invalid.
        volatile u8      board;           
        // RS485 Baudrate between nodes and master board,0x0(115200bps) ~ 0x4(9600bps)
        // default to "0" 
        volatile u8      baudrate;       
        
        /**************************************************************/
        
        // Weight of poise. unity: gram.        
        // Initialized as data saved in EEPROM, 
        // To get better linearity and higher accuracy, we need to have more than 1
        // poise data (saved in EEPROM). 5 at most. Data will be used in CS5532_Poise2Result(). 
        //volatile u16     Poise_Weight_gram[5];     
        // CS5530 conversion of poise.  to get higher accuracy, we need to 
        // have more than 1 
        volatile u16     cs_poise[10];
        // CS5530 conversion when no material is in.
        volatile u16     cs_zero;                    
        // target weight(unity: gram) of each node. pre-set by master board
        // user can set it with PC software.  
        volatile u16     target_weight;
        // max offset allowed from target(unity: g), pre-set by master board   
        //volatile u16     offset_up;     changed to 2 u8 words (addr_copy1, board_copy1)
        volatile u8      addr_copy1;
        volatile u8      board_copy1;
        
        /**************************************************************/             

        // AD filter options: bit[3:0] filter modes, bit[6:4], buf length.
        // bit 7 = 0, Reserved for Future Use. Default to 0. 
        volatile u8      cs_Filter_option;         
        // Bit[6:4]: gain opt, bit[3:0]: word_rate opt, Bit7=0, RFU.
        // Default to 0x3
        volatile u8      cs_gain_wordrate;
        
        /**************************************************************/
        
        // motor speed options: 0~9, default to 0.
        volatile u8      motor_speed;
        // magnet frequency 0~10, default to 9.   
        volatile u8      magnet_freq;
        // magnet amp: 0~75, default to 50.
        volatile u8      magnet_amp;
        // magnet working time, default to 1. unity: 100ms. 
        volatile u16     magnet_time;

        /**************************************************************/
        
        // Viberation fill delay
        volatile u8      delay_f;	
        // Weight bucket delay
        volatile u8      delay_w;	
        // Suspend bucket delay
        volatile u8      delay_s;	
	  // Delay for feeding pool (top bucket) motor
	  volatile u8	 open_s;		 
	  // Delay for weighing pool (bottom bucket) motor
	  volatile u8	 open_w;	   
	
	  // Reserved for Future Use
	  volatile u8      check_sum;
	  volatile u8      rom_para_valid; 
	  volatile u8      addr_copy2;
	  volatile u8      board_copy2;

} S_FLASH;

typedef struct {
        
        /**************************************************************/
        // the followings are read only to master board.
        // Formula to transfer CS5532 measurement to actual weight£º
        //   Mtrl_Weight_gram             (cs_Mtrl - cs_zero)
        //   --------------------   =  --------------------------
        //   Poise_Weight_gram            (cs_poise - cs_zero)
        
        // CS5530 output with material in bucket.
        volatile u16     cs_mtrl;
        
        // polled by master board, 
        // Please display 2 decimal digts on PC software.
        volatile u16     Mtrl_Weight_gram;       // integar part        
        volatile u8      Mtrl_Weight_decimal;    // decimal part: LSB: (1/64)g
                       
        // Diagnostic resultm byte 1, polled by master
        volatile u8      diag_status1;
        volatile u8      diag_status2;         
        volatile u8      hw_status; 
        
        /**************************************************************/
        // CS5530 system calbrition data. Originally saved into EEPROM during 
        // factory calibriation. Data will be download to CS5530 everytime after 
        // system powerup. Master board won't overwrite them.
        // master board need to do nothing regarding these 2 registers. 
        volatile u32     cs_sys_gain_cal_data;
        //volatile u32     cs_sys_offset_cal_data;
        volatile u8      ac_freq;
        volatile u8      packer_intr_cnt;
        volatile u8      flag_cmd_cnt;
        volatile u8      packer_release_cnt;
        
        volatile u16     new_cs_zero;  // initially it is variable "reserved3"; 
        
        /**************************************************************/
        // Control bytes from Master Board. only write is needed
        volatile u8      NumOfDataToBePgmed;
        volatile u8      flag_reset;        
        volatile u8      flag_enable;
        volatile u8      flag_disable;
        volatile u8      flag_release; 
        volatile u8      flag_goon;        
        // please provide me with 2 input boxes in PC software so that I can change test_mode_reg1/reg2 value.
        // my software with check data of these registers to switch ON/OFF a certain functions accordingly.        
        // it is useful especially when debugging raw boards.
        // I haven't got the code up in my software. Will do so later.
        // For example, I would like to use these registers this way: 
        // test_mode_reg1: when bit 0 set, use default data instead of EEPROM data.
        volatile u8      test_mode_reg1;         
        volatile u8      test_mode_reg2;        
       /**************************************************************/
}S_GLOBAL;                      


typedef struct { 
      //status of motor
      // Motor working mode, 
      // 'N': Motor 1 and 2 stop. 'B': Both Motor 1 and 2 work,'W': Weighing Motor(1) works.'S': Storing Motor works.
      volatile i8 mode;
      volatile u8 phase;       // Motor phase index --- cycling: Motor[Motor_Phase]
      volatile u8 pulse_num;   // Number of interrupted raised during the time   
}S_MOTOR;                 

typedef struct  { //status of magnet
      // define pulse numbers to drive the electrical magnet.
      volatile u16 pulse_num;  //u16 changed to u32
      volatile u8  rsm_state;  /* debug information */
      volatile u8  wsm_state;  /* debug information */
      volatile u16 half_period;
}S_MAGNET;

/**************************************************************/
//  When
/**************************************************************/
typedef struct { 
  u16 reserved1;                  // reserved for future use. 
  u16 mfg_cs[NUM_OF_CAL_POINT];   // factory calibration data -- AD readings
  u16 mfg_cs_zero;                // factory calibration data -- AD reading at zero gram
  u16 mfg_checksum;               // checksum + mfg_gram + mfg_cs + mfg_cs_zero = 0
  
  u16 reserved2;
  u16 new_cs_zero;                // AD readings corresponding to user set zero-gram.
} S_ROM;

typedef struct {
        S_FLASH _flash;
        S_GLOBAL _global;  
        S_MOTOR _motor;
        S_MAGNET _magnet;
        S_ROM _rom;
}NODE_CONFIG;       



/*************************************************************************************
*                                  Timer Definitions
*************************************************************************************/
typedef struct {
   volatile u16 timerlen[8];     /* Time to go before task will be scheduled */
   volatile u8  status;          /* boolean type variable, 1: in progress, 0: task completed */
} OS_SCHEDULER;  

#define MAX_NUM_OF_TIMERS               0x08
#define MAX_TMR_ID     (MAX_NUM_OF_TIMERS-1)
#define AC_FREQ_TIMER                   0x07          
#define PACKER_RESP_TIMER               0x06
#define ERROR_SIGNAL_TIMER              0x05  
#define SIGNAL_PLS_WIDTH_TIMER          0x04
#define READY_SIGNAL_TIMER              0x03
#define FEED_DELAY_TIMER                0x02
#define WSM_TIMER                       0x01
#define RSM_TIMER                       0x00

#define MASK_MEAS_AC_FREQ               0x80
#define MSAK_PACKER_TIMER               0x60

#define TIMER_AC_FREQ_ONGOING          (os_sched.status & MASK_MEAS_AC_FREQ)
#define TIMER_PACKER_RESP_ONGIONG      (os_sched.status & MSAK_PACKER_TIMER)

#define RSM_TMR_DLY_END !(os_sched.status & 0x1)   // used in release_material()
#define WSM_TMR_DLY_END !(os_sched.status & 0x2)   // used in magnet_add_material()
                                                   // and motor_magnet_action()
                                              
/*************************************************************************************
*                              Global Variables
*************************************************************************************/
extern void kick_off_timer(u8 task_id, u16 timer_len);
extern OS_SCHEDULER os_sched; 
extern NODE_CONFIG RS485;
extern u8 debug_mode;
/*************************************************************************************
*                                  Test Switches
*************************************************************************************/
// test mode register 1:
#define TEST_BIT0               0b00000001
#define TEST_BIT1               0b00000010
#define TEST_BIT2               0b00000100
#define TEST_BIT3               0b00001000
#define TEST_BIT4               0b00010000
#define TEST_BIT5               0b00100000
#define TEST_BIT6               0b01000000
#define TEST_BIT7               0b10000000
#define TEST_BITS_76            0b11000000
#define TEST_BITS_765           0b11100000

#define ENABLE_MULTI_POISES    (RS485._global.test_mode_reg1 & TEST_BIT7) == 0
#define TURN_OFF_WATCHDOG      (RS485._global.test_mode_reg1 & TEST_BIT5) == 1  
#define DISPLAY_AD_RAW_DATA    (RS485._global.test_mode_reg1 & TEST_BIT4) == 1
#define EN_EEPROM_WRITE        (RS485._global.test_mode_reg1 & TEST_BIT3) == 1 

#define CHANGE_RS485_ADDR      (RS485._global.test_mode_reg2 & TEST_BITS_76) == 0xC0
#define CHANGE_BOARD_TYPE      (RS485._global.test_mode_reg2 & TEST_BITS_76) == 0x80
#define READ_AUTO_AMP_REC      (RS485._global.test_mode_reg2 & TEST_BITS_76) == 0x40 

#endif
