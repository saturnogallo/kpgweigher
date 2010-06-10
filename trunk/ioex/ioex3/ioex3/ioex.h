#pragma once

#include "resource.h"
#include "serial.h"
#include "Winsock2.h"
#define u8	unsigned char
#define i8  char
#define u16 unsigned short
#define u32 unsigned int


#define dbgprint	printf
#define MAX_NODE_NUM 17 //the first one is ignored.               
#define MAX_VIBR_NUM 4
#define MASTER_VIBR_ADDR 11  //RS485 addr of vibrator board is 11.

#define BOARD_TYPE_MASK         0xf0                          
#define BOARD_TYPE_VIBRATE     	0x00
#define BOARD_TYPE_WEIGHT       0x10
#define BOARD_TYPE_COLLECT      0x20
#define BOARD_TYPE_INVALID      0xf0 


#define BOARD_GROUP_MASK        0x07
#define BOARD_GROUP_A           0x00
#define BOARD_GROUP_B           0x01
#define BOARD_GROUP_C           0x02
#define BOARD_GROUP_D           0x03
#define BOARD_GROUP_NONE	0x07

/*************************************************************************************************************/
// RS485._global.flag_enable
#define ENABLE_OFF              0     // stop machine.
#define ENABLE_START_MACHINE    1     // for mannual manipulation.
#define ENABLE_INIT_AD          2
#define ENABLE_TURNS            3
#define ENABLE_TURNW            4
#define ENABLE_VIBRATE          5
#define ENABLE_RESET_WEIGHT		7     // calibrate weight to zero.
#define ENABLE_ON               80    // machine is fully running now.

#define NREG_ENABLE 65
/*************************************************************************************************************/

void answer(CString str);
void query_serial();

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


typedef struct {
           // RS485 address, 0x0 is broadcasting address.
        // valid address range 1~36, wherein address 1~3 are reserved for spare board. 
        u8      addr;//0 
        // Board Function: WEIGHT_MOTOR_VIBRATION or WEIGHT_VIBRATION (the top one)
        // Valid type, 0x0~0x15. Default to "0" (WEIGHT_MOTOR_VIBRATION).
        // Bit 0 of "hw_status" will be set if type invalid.
        u8      board;//1           
        // RS485 Baudrate between nodes and master board,0x0(115200bps) ~ 0x4(9600bps)
        // default to "0" 
        u8      baudrate;//2       
        
        /**************************************************************/
        
        // Weight of poise. unity: gram.        
        // Initialized as data saved in EEPROM, 
        // invalid if data > 4000 and bit 4 of "cs_status" will be set.
        u16     Poise_Weight_gram[5];//4
        // CS5530 conversion of poise.
        u16     cs_poise[5];//14
        // CS5530 conversion when no material is in.
        u16     cs_zero;//24                    
        // target weight(unity: gram) of each node. pre-set by master board
        // user can set it with PC software.  
        u16     target_weight;//26
        // max offset allowed from target(unity: g), pre-set by master board   
        u16     offset_up;//28    
        
        /**************************************************************/             

        // AD filter options: bit[3:0] filter modes, bit[6:4], buf length.
        // bit 7 = 0, Reserved for Future Use. Default to 0. 
        u8      cs_Filter_option;//30
        // Bit[6:4]: gain opt, bit[3:0]: word_rate opt, Bit7=0, RFU.
        // Default to 0x3
        u8      cs_gain_wordrate;//31
        
        /**************************************************************/
        
        // motor speed options: 0~9, default to 0.
        u8      motor_speed;//32
        // magnet frequency 0~10, default to 9.   
        u8      magnet_freq;//33
        // magnet amp: 0~75, default to 50.
        u8      magnet_amp;//34
        // magnet working time, default to 1. unity: 100ms. 
        u16     magnet_time;//35

        /**************************************************************/
        
        // Viberation fill delay
        u8      delay_f;//36	
        // Weight bucket delay
        u8      delay_w;//37	
        // Suspend bucket delay
        u8      delay_s;//38	
		// Delay for feeding pool (top bucket) motor
		u8		open_s;//39	 
		// Delay for weighing pool (bottom bucket) motor
		u8		open_w;//40	   
	
		// Reserved for Future Use
		u8  check_sum;//41
		u8  rom_para_valid;//42
		u8  backup_addr;//43
		u8  backup_board;//44
        u16 cs_mtrl;//45
        /**************************************************************/
        // the followings are read only to master board.
        // Formula to transfer CS5532 measurement to actual weight£º
        //   Mtrl_Weight_gram             (cs_Mtrl - cs_zero)
        //   --------------------   =  --------------------------
        //   Poise_Weight_gram            (cs_poise - cs_zero)

        // polled by master board, 3-byte-Integar + 1-byte-decimal.
        // for example, 0x123456 = 4660.336
        // Please display 2 decimal digts on PC software.
        u16     Mtrl_Weight_gram;//47
        u8      Mtrl_Weight_decimal;//49  
        // status of state machine. polled by master board.
        // Originally defined by sojo, no changes.
        u8      status;//50
        
        // AD status polled by master board for error check, when set:
        // bit 0 -> cs5530 offset cal fail
        // bit 1 -> cs5530 gain cal fail
        // bit 2 -> cs5530 setup fail
        // bit 3 -> cs5530 conversion overflow (invalid readout)  
        // bit 4 -> cs5530 output-to-weight calculaton invalid. 
        // bit 7:5 -> reserved for future use. keep 0.
        u8      cs_status; //51
        
        // hardware status, when set:
        // bit 0: -> motor_s action fail
        // bit 1: -> motor_w action fail
        // bit 2: -> magnet action fail. reserved bit.
        // bit 3: -> watchdog reset happened. Settings may be different from master board...
        // bit 4: -> UART/Frame error.
        // bit 7:5-> reserved for future use.   keep 0.
        u8      hw_status; //52
        
        // CS5530 system calbrition data. Originally saved into EEPROM during 
        // factory calibriation. Data will be download to CS5530 everytime after 
        // system powerup. Master board won't overwrite them.
        u32     cs_sys_gain_cal_data; //53
        u32     cs_sys_offset_cal_data;//57
        
        // RFU
        u16     old_cs_zero;   //61
        
        /**************************************************************/
        // Control bytes from Master Board.
        u8      NumOfDataToBePgmed;//63
        u8      flag_reset;        //64
        u8      flag_enable;	   //65
        u8      flag_disable;      //66
        u8      flag_release;      //67
        u8      flag_goon;         //68
        u8      test_mode_reg1;    //69      // when set, enter test mode.
        u8		test_mode_reg2;	   //70

		//status of motor
		// Motor working mode, 
		// 'N': Motor 1 and 2 stop. 'B': Both Motor 1 and 2 work,'W': Weighing Motor(1) works.'S': Storing Motor works.
		i8 mode;//71
		u8 phase;//72       // Motor phase index --- cycling: Motor[Motor_Phase]
		u8 motor_pulse_num;//73   // Number of interrupted raised during the time   

		//status of magnet
		// define pulse numbers to drive the electrical magnet.
		u32 mag_pulse_num;//74  //u16 changed to u32
		u16 half_period;//78

}NODE_CONFIG;


typedef struct {
	  u8 fail_counter;
}APP_NODE_CONFIG;

#define check_sysflag(addr,expect) (*addr == expect)

//check whether the address matches to some RS485_Node's name
#define ADDR_TO_NAME(name)	if(( id ==(u8)((u8*)&(RS485_Node[0].##name)-(u8*)&RS485_Node[0].addr))) return _T(#name)
#define NODE_ADDR_TO_NAME	ADDR_TO_NAME(addr);\
							ADDR_TO_NAME(board);\
							ADDR_TO_NAME(baudrate);\
							ADDR_TO_NAME(Poise_Weight_gram[0]);\
							ADDR_TO_NAME(Poise_Weight_gram[1]);\
							ADDR_TO_NAME(Poise_Weight_gram[2]);\
							ADDR_TO_NAME(Poise_Weight_gram[3]);\
							ADDR_TO_NAME(Poise_Weight_gram[4]);\
							ADDR_TO_NAME(cs_poise[0]);\
							ADDR_TO_NAME(cs_poise[1]);\
							ADDR_TO_NAME(cs_poise[2]);\
							ADDR_TO_NAME(cs_poise[3]);\
							ADDR_TO_NAME(cs_poise[4]);\
							ADDR_TO_NAME(cs_zero);\
							ADDR_TO_NAME(target_weight);\
							ADDR_TO_NAME(offset_up);\
							ADDR_TO_NAME(cs_Filter_option);\
							ADDR_TO_NAME(cs_gain_wordrate);\
							ADDR_TO_NAME(motor_speed);\
							ADDR_TO_NAME(magnet_freq);\
							ADDR_TO_NAME(magnet_amp);\
							ADDR_TO_NAME(magnet_time);\
							ADDR_TO_NAME(delay_f);\
							ADDR_TO_NAME(delay_w);\
							ADDR_TO_NAME(delay_s);\
							ADDR_TO_NAME(open_s);\
							ADDR_TO_NAME(open_w);\
							ADDR_TO_NAME(check_sum);\
							ADDR_TO_NAME(rom_para_valid);\
							ADDR_TO_NAME(backup_addr);\
							ADDR_TO_NAME(backup_board);\
							ADDR_TO_NAME(cs_mtrl);\
							ADDR_TO_NAME(Mtrl_Weight_gram);\
							ADDR_TO_NAME(Mtrl_Weight_decimal);\
							ADDR_TO_NAME(status);\
							ADDR_TO_NAME(cs_status);\
							ADDR_TO_NAME(hw_status);\
							ADDR_TO_NAME(cs_sys_gain_cal_data);\
							ADDR_TO_NAME(cs_sys_offset_cal_data);\
							ADDR_TO_NAME(old_cs_zero);\
							ADDR_TO_NAME(NumOfDataToBePgmed);\
							ADDR_TO_NAME(flag_reset);\
							ADDR_TO_NAME(flag_enable);\
							ADDR_TO_NAME(flag_disable);\
							ADDR_TO_NAME(flag_release);\
							ADDR_TO_NAME(flag_goon);\
							ADDR_TO_NAME(test_mode_reg1);\
							ADDR_TO_NAME(test_mode_reg2);\
							ADDR_TO_NAME(mode);\
							ADDR_TO_NAME(phase);\
							ADDR_TO_NAME(motor_pulse_num);\
							ADDR_TO_NAME(mag_pulse_num);\
							ADDR_TO_NAME(half_period);
/* SYSTEM_BOARD should be consistent with array "sys_reg_size[]" defined in action.cpp */
typedef struct
{ //status from pc
	u16 gw_target[MAX_VIBR_NUM];//0
	u16 gw_uvar[MAX_VIBR_NUM];  //8
	u16 gw_dvar[MAX_VIBR_NUM];  //16
	u8  flag_goon[MAX_VIBR_NUM];//24
	u8  node_num;               //28  // how many weight node in this system
	u8  vibrator_num;           //29  // number of Vibrator detected in the system
	u8  running[MAX_VIBR_NUM];  //30
	u8  flag_start_machine[MAX_VIBR_NUM]; //34   Set when starting machine command is received.
	u8  flag_stop_machine[MAX_VIBR_NUM];  //38
	u8  flag_search[MAX_VIBR_NUM];        //42
	u8  flag_report;					  //46
	u8  reserved1;                        //47  -- aligned
	u16 packer_config1;                   //48  -- word aligned
    u16 packer_config2;                   //50      
    u16 reserved2;                        //52
    u16 reserved3;                        //54
}SYSTEM_BOARD;

//check whether the address matches to some Sysboard's name
#define A2N_SET(name)	if( id ==(u8)((u8*)&Sysboard.##name-(u8*)&Sysboard)) return #name
#define GRP_A2N_SET(a)	A2N_SET(a[0]);A2N_SET(a[1]);A2N_SET(a[2]);A2N_SET(a[3]);
#define	SYS_ADDR_TO_NAME	GRP_A2N_SET(gw_target);\
							GRP_A2N_SET(gw_uvar);\
							GRP_A2N_SET(gw_dvar);\
							GRP_A2N_SET(flag_goon);\
							GRP_A2N_SET(running);\
							GRP_A2N_SET(flag_start_machine);\
							GRP_A2N_SET(flag_stop_machine);\
							GRP_A2N_SET(flag_search);\
							A2N_SET(node_num);\
							A2N_SET(vibrator_num);\
							A2N_SET(flag_report);\
							A2N_SET(reserved1);\
							A2N_SET(packer_config1);\
							A2N_SET(packer_config2);\
							A2N_SET(reserved2);\
							A2N_SET(reserved3);

extern NODE_CONFIG RS485_Node[MAX_NODE_NUM];
extern APP_NODE_CONFIG App_Node[MAX_NODE_NUM];
extern SYSTEM_BOARD Sysboard;
extern CSerial serial;
const char* addr2name(u8 addr,u8 id); //convert local address to name
void answer_flash(CString);
void check_serial();
int flash_cmd_handler();

#define PROFILE_NAME	sAppPath+_T("\\profile.ini")
#define PRODUCT_NAME	sAppPath+_T("\\product.xml")
#define HISTORY_NAME	sAppPath+_T("\\history.xml")
#define RESULT_NAME	sAppPath+_T("\\result.xml")
#define TWOFRM_DURATION	2
#define TICK_DURATION 2
BOOL WriteProfile(CString sect,CString name,CString value);
CString ReadProfile(CString sect,CString name);
