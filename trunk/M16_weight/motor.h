#ifndef _MOTOR_H_ 
#define _MOTOR_H_ 

#include "define.h"

/*************************************************************************************
*                           Stepping Motor Speed Definitions
*************************************************************************************/
#define CYCLE_0_5                 0xB8
#define CYCLE_0_6                 0xC4
#define CYCLE_0_7                 0xCC
#define CYCLE_0_8                 0xD3
#define CYCLE_0_9                 0xD8
#define CYCLE_1_0	          0xDC
#define CYCLE_1_2                 0xE2
#define CYCLE_1_5	          0xE8
#define CYCLE_2_0	          0xEE
#define CYCLE_2_3	          0xF0        
#define CYCLE_2_6	          0xF2
#define CYCLE_3_0	          0xF4
#define CYCLE_3_3	          0xF5
#define CYCLE_3_6	          0xF6
#define CYCLE_4_0	          0xF7
#define CYCLE_4_5	          0xF8
#define CYCLE_5_0	          0xF9
#define CYCLE_6_0	          0xFA
#define CYCLE_7_0	          0xFB  

/*************************************************************************************
*                      Stepping Motor Power Mode Definitions
*************************************************************************************/
#define KEEP_POWER_ON             0x01      // Keep power on after rotation
#define POWER_OFF_AFTER_ROTATION  0x00      // Power off motor driver after rotation


/*****************************************************************************/
//                    Electrical magnet driver
// arguments: intervals, define how many m-seconds Electical magnet will work
// arguments: PWM_Freq, define working frequency of electical magnet.
/*****************************************************************************/
void E_Magnet_Driver(u16 pulse_num, u8 amp); 

/*****************************************************************************/
//                    Stepping Motor driver
// arguments: intervals, define how many m-seconds Electical magnet will work
// arguments: PWM_Freq, define working frequency of electical magnet.
/*****************************************************************************/

void Motor_Driver(u8 Mode,u8 pulsenum, u8 powerflag);

u8 Motor_Is_Running();
u16 Magnet_Is_Running();
u8 read_ana_cmp_isr_counter(); 
void init_ana_isr_counter();
void Motor_One_Step();

#endif
