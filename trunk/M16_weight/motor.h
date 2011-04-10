//motor.h 
#ifndef _MOTOR_H_ 
#define _MOTOR_H_ 
#include "define.h"
/*****************************************************************************/
//                    Electrical magnet driver
// arguments: intervals, define how many m-seconds Electical magnet will work
// arguments: PWM_Freq, define working frequency of electical magnet.
/*****************************************************************************/
void E_Magnet_Driver(u16); 

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
