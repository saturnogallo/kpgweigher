#ifndef _MOTOR_H_ 
#define _MOTOR_H_ 

#include "define.h"

/*************************************************************************************
*                           Stepping Motor Speed Definitions
*************************************************************************************/
#define CYCLE_0_5           0xB8
#define CYCLE_0_6           0xC4
#define CYCLE_0_7           0xCC
#define CYCLE_0_8           0xD3
#define CYCLE_0_9           0xD8
#define CYCLE_1_0	          0xDC
#define CYCLE_1_2           0xE2
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
*                           Hardware Abstract definition
*************************************************************************************/
#define WGHT_MOTORS_CLOSED       PIND.3 == 1    // sensor is read at PORTD[3:2]
#define WGHT_MOTORW_CLOSED       PIND.2 == 1 
#define INTF_MOTORS_CLOSED       PINC.1 == 1    // sensor is read at PORTC[1:0]
#define INTF_MOTORW_CLOSED       PINC.0 == 1     
#define BOARD_IS_INTERFACE       (hw_id == HW_ID_INTERFACE)
#define BOARD_IS_WEIGHT          (hw_id == HW_ID_WEIGHT)

#define MAGNET_PULSE_PORT        PORTB.1
#define ENABLE_MOTOR_W	         PORTA.0 = 0    // optocoupler, inverted logic
#define DISABLE_MOTOR_W	         PORTA.0 = 1
#define ENABLE_MOTOR_S	         PORTA.1 = 0
#define DISABLE_MOTOR_S	         PORTA.1 = 1
#define MT_MODE_BIT0             PORTA.2
#define MT_MODE_BIT1             PORTA.3 
#define MOTOR_CLOCK_PIN          PORTA.4
#define MOTOR_ROTATE_CLKWISE     PORTA.5 = 1
#define MOTOR_ROTATE_ANTICLKWISE PORTA.5 = 0 

#define TRIG_TRIAC()             PORTB.1 = 0 
#define SHUTDOWN_TRIAC()         PORTB.1 = 1
#define DISABLE_TIMER1_INT()     TIMSK &= 0xFB
#define CLR_TIMER1_INT_FLG()     TIFR |= (1<<TOV1)
#define ENABLE_TIMER1_INT()      TIMSK |= 0x4
#define STOP_TIMER1_CNT()        TCCR1B = 0x0
#define START_TIMER1_CNT()       TCCR1B = 0x3
#define DISABLE_TIMER0_INT()     TIMSK &= 0xFE
#define ENABLE_TIMER0_INT()      TIMSK |= 0x1 
#define STOP_TIMER0_CNT()        TCCR0=0x00
#define START_TIMER0_CNT()       TCCR0=0x05


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
//void init_ana_isr_counter();
void Motor_One_Step();

#endif
