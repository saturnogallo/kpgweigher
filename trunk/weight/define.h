#ifndef _DEFINE_H_
#define _DEFINE_H_

//My compile switches here: enabled when uncommented
#define _50HZ_FILTER_           //50HZ AC power supply. uncommented if 60HZ.
//#define _SENSOR_ENABLED_        //uncommented to enable stepping motor sensors
#define _DISABLE_WATCHDOG_    //uncommented to disable watchdog
//#define _FORCE_INIT_EEPROM_  // force to initialize EEPROM.
//#define _FORCE_CONSTANT_WEIGHT_    
//#define _BOARD_TYPE_IS_VIBRATE_

#define i32 	long int
#define i16 	int
#define i8	char
#define u32 	unsigned long int
#define u16 	unsigned int
#define u8	unsigned char

#define FIRMWARE_REV 0x00
#define NULL 0x0
#define PASS 0x0        
#define FAIL 0xff
#define GOT_DATA 0x0
#define CS5532_BUSY 0xFF  
#define SYS_GAIN_CAL_PREWORK_NOT_READY 1
#define CS5532_SETUP_ERR 0x1
#define CS5532_OFFSET_CAL_ERR 0x2
#define CS5532_GAIN_CAL_ERR 0x3

// AD conversion indicators
#define INVALID_DATA 0xfffe   // Adding material to lower bucket or releasing material.
#define AD_BUSY 0xfffd
#define AD_OVER_FLOW 0xfffc                     //65532
#define FILTER_ONGOING 0xfffb 
#define DIV_ERROR 0xfffa                         //65530
#define OVERWEIGHT 0xfff9 
#define MAX_VALID_DATA 0xfff0

// 定义还有多少数据需要被写入EEPROM。
#define NUM_OF_DATA_TO_BE_PGMED 0xFF

// used in debug mode, define delay (ms) between sending 2 
#define UART_DELAY 5    //

void sleepus(u16);
void sleepms(u16);
void sleeps(u16);

// 定义硬件管脚名称
#define PIN_UART          PORTB.7        // LED D7
#define PIN_EEPROM        PORTB.7        // LED D7
#define PIN_AD            PORTD.6        // LED D8
#define PIN_RUN           PORTD.5        // LED D6

#define LED_D7            PORTB.7        // LED D7
#define LED_D8            PORTD.6        // LED D8
#define LED_D6            PORTD.5        // LED D6

    
#define LED_ON(x)         x = 0
#define LED_OFF(x)        x = 1
#define LED_FLASH(x)      x = ~x
                                    
#define TARGET_WEIGHT_500G 500

// 马达/线振机参数最大可选项个数
#define MAX_MOTOR_SPEED_INDEX 10
#define MAX_MAGNET_AMP_INDEX 75
#define MAX_MAGNET_FREQ_INDEX 5

// 定义步进电机的驱动频率选项: 这些数值将被赋给定时器寄存器 TCNT
// 0xF0: 2.3 cycle/s; 0xF2: 2.6; 0xF4: 3; 0xF5: 3.3; 0xF6: 3.6; 0xF7: 4 cycle/s 
#define CYCLE_2_3	0xF0  //0xF0->0xFB
#define CYCLE_2_6	0xF2
#define CYCLE_3_0	0xF4
#define CYCLE_3_3	0xF5
#define CYCLE_3_6	0xF6
#define CYCLE_4_0	0xF7
#define CYCLE_4_5	0xF8
#define CYCLE_5_0	0xF9
#define CYCLE_6_0	0xFA
#define CYCLE_7_0	0xFB   

//define motor pulse numbers
#define MOTOR_ONE_CYCLE_WITH_SENSOR 230 
#define MOTOR_ONE_CYCLE_NO_SENSOR 200         
#define MOTOR_SHIFT_CYCLE_OPEN 100
#define HALF_CYCLE_CLOSE_WITH_SENSOR 130
#define HALF_CYCLE_CLOSE_NO_SENSOR 100

// define motor's power modes. 
#define KEEP_POWER_ON 0x1       // bit0  = 1
#define POWER_OFF_AFTER_ROTATION 0x0  //bit0 = 0

// 定义步进电机驱动相位: 2 phase---4 steps
// L298 only. Not used anymore. 
//#define PHASE_1		0x32
//#define PHASE_2		0x38
//#define PHASE_3		0x31
//#define PHASE_4		0x34


// RS485节点信息/参数在EEPROM中的存放地址
#define EEPROM_RS485ADDR 0x80
#define CAL_DATA_START_ADDR_IN_EEPROM 0xA0   

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
        u8      addr;           
        // Board Function: WEIGHT_MOTOR_VIBRATION or WEIGHT_VIBRATION (the top one)
        // Valid type, 0x0~0x15. Default to "0" (WEIGHT_MOTOR_VIBRATION).
        // Bit 0 of "hw_status" will be set if type invalid.
        u8      board;           
        // RS485 Baudrate between nodes and master board,0x0(115200bps) ~ 0x4(9600bps)
        // default to "0" 
        u8      baudrate;       
        
        /**************************************************************/
        
        // Weight of poise. unity: gram.        
        // Initialized as data saved in EEPROM, 
        // To get better linearity and higher accuracy, we need to have more than 1
        // poise data (saved in EEPROM). 5 at most. Data will be used in CS5532_Poise2Result(). 
        // code is 
        // not ready yet. 
        u16     Poise_Weight_gram[5];     
        // CS5530 conversion of poise.  to get higher accuracy, we need to 
        // have more than 1 
        u16     cs_poise[5];
        // CS5530 conversion when no material is in.
        u16     cs_zero;                    
        // target weight(unity: gram) of each node. pre-set by master board
        // user can set it with PC software.  
        u16     target_weight;
        // max offset allowed from target(unity: g), pre-set by master board   
        u16     offset_up;    
        
        /**************************************************************/             

        // AD filter options: bit[3:0] filter modes, bit[6:4], buf length.
        // bit 7 = 0, Reserved for Future Use. Default to 0. 
        u8      cs_Filter_option;         
        // Bit[6:4]: gain opt, bit[3:0]: word_rate opt, Bit7=0, RFU.
        // Default to 0x3
        u8      cs_gain_wordrate;
        
        /**************************************************************/
        
        // motor speed options: 0~9, default to 0.
        u8      motor_speed;
        // magnet frequency 0~10, default to 9.   
        u8      magnet_freq;
        // magnet amp: 0~75, default to 50.
        u8      magnet_amp;
        // magnet working time, default to 1. unity: 100ms. 
        u16     magnet_time;

        /**************************************************************/
        
        // Viberation fill delay
        u8      delay_f;	
        // Weight bucket delay
        u8      delay_w;	
        // Suspend bucket delay
        u8      delay_s;	
	// Delay for feeding pool (top bucket) motor
	u8	open_s;		 
	// Delay for weighing pool (bottom bucket) motor
	u8	open_w;	   
	
	// Reserved for Future Use
	u8      check_sum;
	u8      rom_para_valid; 
	u8      addr_backup;
	u8      board_backup;

} S_FLASH;                

typedef struct {
        
        /**************************************************************/
        // the followings are read only to master board.
        // Formula to transfer CS5532 measurement to actual weight：
        //   Mtrl_Weight_gram             (cs_Mtrl - cs_zero)
        //   --------------------   =  --------------------------
        //   Poise_Weight_gram            (cs_poise - cs_zero)
        
        // CS5530 output with material in bucket.
        u16     cs_mtrl;
        
        // polled by master board, 
        // Please display 2 decimal digts on PC software.
        u16     Mtrl_Weight_gram;       // integar part        
        u8      Mtrl_Weight_decimal;    // decimal part: LSB: (1/64)g
                
        
        // status of state machine. polled by master board.
        // Originally defined by sojo, no changes. May be polled by master board as needed
        u8      status;
        
        // AD status polled by master board for error check, when set:
        // bit 0 -> cs5530 offset cal fail. This bit won't be used in production(keep 0).
        // bit 1 -> cs5530 gain cal fail. This bit won't be used in production(keep 0). 
        // bit 2 -> cs5532 initialization fail.
        // bit 3 -> cs5530 conversion overflow (invalid readout)  
        // bit 4 -> cs5530 output-to-gram calculaton error, "Mtrl_Weight_gram" invalid. 
        // bit 7:5 -> reserved for future use. keep 0.
        // maybe polled by master board as needed.
        u8      cs_status; 
        
        // hardware status, when set:
        // bit 0: -> motor_s action fail
        // bit 1: -> motor_w action fail
        // bit 2: -> magnet action fail. reserved bit.
        // bit 3: -> watchdog reset happened. Settings may be different from master board...
        // bit 4: -> UART/Frame error.
        // bit 7:5-> reserved for future use.   keep 0.
        // maybe polled by master board as needed.        
        u8      hw_status; 
        
        /**************************************************************/
        // CS5530 system calbrition data. Originally saved into EEPROM during 
        // factory calibriation. Data will be download to CS5530 everytime after 
        // system powerup. Master board won't overwrite them.
        // master board need to do nothing regarding these 2 registers. 
        u32     cs_sys_gain_cal_data;
        u32     cs_sys_offset_cal_data;
        
        // Reserved for Future Use.
        // This variable is first initalized from EEPROM (equal to cs_zero)
        // sometimes "zero" point shifts and cs_zero/cs_poise needs to be re-adjusted.
        // In PC software, user can click "Reset to Zero" to force actual weight(unity: gram) to zero.
        // Software in RS-485 nodes calculates delta between "cs_zero" and "old_cs_zero"
        // and adjust cs_poise[5] accordingly. After that "old_cs_zero" is set equal to "cs_zero".
        // Users don't need to do weight calibration again.
        // Clicking button "Write_to_EEPROM" will save new data into EEPROM.
        // After node powerup, "old_cs_zero" will be initialzed equal to cs_zero value in EEPROM.
        
        u16     old_cs_zero;  // initially it is variable "reserved3"; 
        
        /**************************************************************/
        // Control bytes from Master Board. only write is needed
        u8      NumOfDataToBePgmed;
        u8      flag_reset;        
        u8      flag_enable;
        u8      flag_disable;
        u8      flag_release; 
        u8      flag_goon;        
        // please provide me with 2 input boxes in PC software so that I can change test_mode_reg1/reg2 value.
        // my software with check data of these registers to switch ON/OFF a certain functions accordingly.        
        // it is useful especially when debugging raw boards.
        // I haven't got the code up in my software. Will do so later.
        // For example, I would like to use these registers this way: 
        // test_mode_reg1: when bit 0 set, use default data instead of EEPROM data.
        u8      test_mode_reg1;         
        u8      test_mode_reg2;        
       /**************************************************************/
}S_GLOBAL;                      


typedef struct { 
      //status of motor
      // Motor working mode, 
      // 'N': Motor 1 and 2 stop. 'B': Both Motor 1 and 2 work,'W': Weighing Motor(1) works.'S': Storing Motor works.
      i8 mode;
      u8 phase;       // Motor phase index --- cycling: Motor[Motor_Phase]
      u8 pulse_num;   // Number of interrupted raised during the time   
}S_MOTOR;                 

typedef struct  { //status of magnet
      // define pulse numbers to drive the electrical magnet.
      u16 pulse_num;  //u16 changed to u32
      u16 reserved; 
      u16 half_period;
}S_MAGNET;

typedef struct {
        S_FLASH _flash;
        S_GLOBAL _global;  
        S_MOTOR _motor;
        S_MAGNET _magnet;
}NODE_CONFIG;       

extern NODE_CONFIG RS485;
extern u8 debug_mode;  

#define prints  mputs

/******************************************************************/
// OK... let's still use your definition.


// I don't know what it means, let's align here, 
// 0000b is for weigh_motor_viberate
// 1000  without motor_s
// 0100  without motor_w
// 1100  without motor_sw
// 1110  weight only
// others....
// your settings. 
/******************************************************************/

#define BOARD_TYPE_MASK         0xf0                          
#define BOARD_TYPE_VIBRATE      0x00
#define BOARD_TYPE_WEIGHT       0x10
#define BOARD_TYPE_INVALID      0xf0    

#define BOARD_GROUP_MASK        0x0f
#define BOARD_GROUP_A           0x00
#define BOARD_GROUP_B           0x01
#define BOARD_GROUP_C           0x02
#define BOARD_GROUP_D           0x03

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

#define ENABLE_MULTI_POISES  (RS485._global.test_mode_reg1 & TEST_BIT7) == 0
#define ENABLE_MOTOR_SENSORS (RS485._global.test_mode_reg1 & TEST_BIT6) == 0
#define TURN_OFF_WATCHDOG    (RS485._global.test_mode_reg1 & TEST_BIT5) == 1  
#define DISPLAY_AD_RAW_DATA  (RS485._global.test_mode_reg1 & TEST_BIT4) == 1
#define EN_EEPROM_WRITE      (RS485._global.test_mode_reg1 & TEST_BIT3) == 1

#define CHANGE_RS485_ADDR    (RS485._global.test_mode_reg2 & TEST_BITS_76) == 0x80
#define CHANGE_BOARD_TYPE    (RS485._global.test_mode_reg2 & TEST_BITS_76) == 0x40

#endif
