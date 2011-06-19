#include <MEGA16.h>
#include "motor.h"


/*************************************************************************************
*                               Global Variables used
*************************************************************************************/
extern bit freq_is_50HZ;                   // AC frequency, 1: 50Hz, 0:60Hz
static u8 magnet_amp;                      // magnet amplitude, global within this file
static u8 Timer1_flag; 
static u8 ana_cmp_isr_counter;             //delay after close signal is detected

/*************************************************************************************
*                            Definitions Used
*************************************************************************************/
#define SHIFT_PULSE_INCNUM            0
#define SHIFT_PULSE_DECNUM            0              
#define SET_BY_ACINT                  1
#define SET_BY_TMR1                   2
#define ONE_PLS_CMPLT                 3

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
#define TRIG_CMPLT()             PORTB.1 = 1

flash u8 MOTOR_FREQ[]={CYCLE_1_5,CYCLE_2_0,CYCLE_2_3,CYCLE_2_6,CYCLE_3_0,CYCLE_3_3,CYCLE_3_6,CYCLE_4_0,CYCLE_4_5,CYCLE_5_0};        
 
/*************************************************************************************
*  @ Proc:  Motor_One_Step()
*  @ Brief: Generate a clock pulse to drive stepping motor a step forward.
*************************************************************************************/
void Motor_One_Step()
{
  u8 i;
  for(i=20;i>0;i--)
     MOTOR_CLOCK_PIN = 0;
  for(i=20;i>0;i--)
     MOTOR_CLOCK_PIN = 1; 
}
 
/*************************************************************************************
*              Analog Comparator interrupt service routine 
* Interrupt when AC power supply cross zero (voltage)
* As a method to control magnet magtitude (power supply), timer is started 
* to delay for a certain before turns on Triac to drive magnet.
* Triac automatically turns off when 220V AC power supply 
*
*
*       AC Peak Detection Interrupt
*                  ^
*     *  *         |
*   *      *       |
*  *---------*-----------*
*      |     | *        *|
*      |     |    *  *   |
*   Det(Int) |ZC     |   | 
*      |             |   |
*      |------------>|   |        
*      | Timer Delay |   |
*      |             |   |
*      |  Triac ON-->|   |<-- Triac ON
*      |             |
*                    |<---- Trigger Triac on   
*
*  ZC: Zero Cross
*************************************************************************************/
// 176: Max triac ON range: 0 ~ 165 degree. 0.87ms Triac ON time.
// 181: Max triac ON range: 0 ~ 170 degree. 0.55ms Triac ON time 
// 186: Max triac ON range: 0 ~ 175 degree. 0.3ms Triac ON time 
interrupt [ANA_COMP] void ana_comp_isr(void)
{
  u16 temp;                                
  
  if(os_sched.status & 0x80)                         // continue to count if timer is
     ana_cmp_isr_counter++;                          // not timeout. 
  
  if(RS485._magnet.pulse_num) 
  {    RS485._magnet.pulse_num--;         
  }
  else 
  {  
     if(!(os_sched.status & 0x80))                // Timer task 7 (measure power AC frequency) ongoing, 
     {  ACSR &= 0xF7;                             // disable AC interrupt
        TIMSK &= 0xFB;                            // disable timer1 OV interrupt.
     } 
     Timer1_flag = ONE_PLS_CMPLT;  
     TRIG_CMPLT();                                //  shut down triac.
     return; 
  }       
  
  if(Timer1_flag == ONE_PLS_CMPLT)
  {  
    // Timers starts on system power up. Timer 1 is started now. 
    // So OV1 flag has already been set. 
    // Clear Timer1 over-flow flag residuals: TOV1, bit 2.
    // There was once a bug: TOV1 is not cleared, therefore after quit from 
    // "ana_comp_isr", "timer1_ovf_isr" is quickly serviced.  
    TIFR |= 0x4;
    // Start Timer1 to delay a time before turning on Triac. 
    if(RS485._flash.board == BOARD_TYPE_VIBRATE)
    {   
        if(freq_is_50HZ)
        {   if(RS485._flash.addr == 11)
                temp =  (u16)magnet_amp * 6 + 64166; 
            else if (RS485._flash.addr == 15)
                temp =  (u16)magnet_amp * 2 + 64000;
        }
        else /* 60HZ */
        {   if(RS485._flash.addr == 11)
                temp =  (u16)magnet_amp * 6 + 64186; 
            else if (RS485._flash.addr == 15)
                temp =  (u16)magnet_amp * 2 + 64186;        
        }     
    }
    else /* weight board */
    {   if(freq_is_50HZ)
            temp =  (u16)magnet_amp * 3 + 64166;      
        else /* 60HZ */
            temp =  (u16)magnet_amp * 3 + 64186;            
    
    }
    /* IMPORTANT NOTES: Because TCNT1 is a 16-bit register, writing to TCNT1 will be broken into 2 bytes.*/
    //TCNT1 = COUNTER_BASE + temp<<3 ;
    TCNT1 = temp;
    //TCNT1 = 65535 - 6 * (MAX_TRIAC_ANGEL - RS485._flash.magnet_amp); 
    // Enable Timer1 overflow interrupt.  
    TIMSK |= 0x4;
    // flag to indicate timer is set by Analog comparator interruption.
    Timer1_flag = SET_BY_ACINT;  
  }
                              
}

/*****************************************************************************/
//                     Timer 1 interrupt service routine
// Timer 1 overflow interrupt service routine.
// Timer 1 is used to when triac should be triggered.
/*****************************************************************************/
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{         
   if(Timer1_flag == SET_BY_ACINT)
   { TRIG_TRIAC();
     TCNT1 = 65500; // 0.31ms. Triac trigger pulse width is 0.31ms 
     Timer1_flag = SET_BY_TMR1;
   }
   else
   {  
     //######################################################
     // PORTB.1 is magnet output control logic output, 
     // PORTA.6 is the feedback from field. If they are not 
     // consistent, report loop wire-open fault. 
     //######################################################
     if(!PINB.1)                
     {
        if(PINA.6)
           RS485._global.diag_status1 |= 0x2;   // set err bit 1
        else
           RS485._global.diag_status1 &= 0xFD;  // clear bit 1
     }
     //#######################################################   
      TRIG_CMPLT();            
      Timer1_flag = ONE_PLS_CMPLT;
      TIMSK &= 0xFB; // disable timer1 OV interrupt.
   }       
}

/*****************************************************************************/
//                     Timer 0 interrupt service routine
// Timer 0 overflow interrupt service routine.
// Timer 0 is used to generate pulse to drive stepping motors.
// Register counter "TCNT0" is used to set clock frequency. freq=7.2K/(0xFF-TCNT0)
// Global variable "Motor_pulse_num" is the number of pulses still needed before
// stepping motor completes a cycle. step length: 1.8 degree per step.
// Global variable "Motor_Speed" modulates motor speed (Timer0 interrupt frequency)
// Motor_Speed is set to a value defined in array of "Motor_Pulse_Freq".
// This interrupt subroutine is called to control 2 motor. totally 3 working modes:
// 'N': Motor 1(top) and 2 (bottom) stop. 'B': Both Motor 1 and 2 work
// 'W': Weighing Motor(1) works.  'S': Storing Motor(2)works.
// To avoid mis-stepping at high speed, motor is forced to start at a low speed
// during the first several pulse (for example 25 pulses).
// Test result: motor speed range 0~7.
// 
/*****************************************************************************/
#define CLOSURE_DETECTED 0x80
#define CLEAR   0x0
#define POWER_FLAG 0x1
#define POWERON 0b01000000
#define POWEROFF 00    

#define NUM_OF_STARTUP_PULSE 25

#define RETURN_TO_SENSOR ((RS485._motor.pulse_num>SHIFT_PULSE_INCNUM) && (RS485._motor.pulse_num < 50))

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
  // Does the motor still need more pulse (1 cycle completes?)? 
  // Will use a TP807 sensor to detect whether more pulses are needed or not
  if (RS485._motor.pulse_num)
  {                    
      RS485._motor.pulse_num--;      
      
      //check how many low-frequency driving pulses need to be issued before motor
      // is ready to run at high speed. Bit 5~0 of "phase" contains pulse numbers.
      if(RS485._motor.phase & 0x3F)
      {  RS485._motor.phase--;
         TCNT0 = MOTOR_FREQ[0]; // start at a relatively lower speed
      }
      else // motor is ready to run at high speed now. 
         TCNT0 = MOTOR_FREQ[RS485._flash.motor_speed];
      
      // Motor to Move one step
      Motor_One_Step(); 
      // If Bit4 of test_mode_reg1 is unset, motor sensors are enabled. see "define.h".
      //if(ENABLE_MOTOR_SENSORS)
      {           
          // Adjust number of pulses to go.
          // if sensor detects closure action, motor is forced to run "SHIFT_PULSE_INCNUM" more pulses
          // disregard of acutal pulse numbers left.
          // Generally, before first half period of rotation(open action), intial state of sensor is "CLOSED"
          // In this case we will not adjust pulse number.
          // Bit 6 of "phase" indicates power-on (open half) or power-off (close half) options, which can 
          // be used here determine whether we need to adjust pulse number or not.
          // open half cycle: keep power on = leaving sensor = no need to adjust pulse number
          // close half cycle: keep power off = moving toward sensor = need to adjust pulse number. 
          if((RS485._motor.phase & 0x40 ) == 0)
          {   if(RS485._motor.mode == 'W')
              {  
                 if(RETURN_TO_SENSOR)
                 {   if((BOARD_IS_INTERFACE) && (INTF_MOTORW_CLOSED)) 
                        RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;
                     if((BOARD_IS_WEIGHT) && (WGHT_MOTORW_CLOSED))
                        RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;                     
                 }
              }
              if(RS485._motor.mode == 'S')
              {  if(RETURN_TO_SENSOR)
                 {   if((BOARD_IS_INTERFACE) && (INTF_MOTORS_CLOSED))
                        RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;
                     if((BOARD_IS_WEIGHT) && (WGHT_MOTORS_CLOSED))                        
                        RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;                     
                 }
              }
  
              if(BOARD_IS_WEIGHT)
              { 
                 if((WGHT_MOTORW_CLOSED)||(WGHT_MOTORS_CLOSED))
                    RS485._motor.phase |= CLOSURE_DETECTED;   //bit 7 as a flag.   
              }
              else if(BOARD_IS_INTERFACE)
              { 
                 if((INTF_MOTORW_CLOSED)||(INTF_MOTORS_CLOSED))
                    RS485._motor.phase |= CLOSURE_DETECTED;   //bit 7 as a flag.   
              }
                            
          }
      }        
  }   
  // NO MORE PULSES.
  else // DEBUG INFORMATION IN STATUS REGISTER
  {  
      //if(ENABLE_MOTOR_SENSORS)
      {
          if(RS485._motor.phase && CLOSURE_DETECTED)
          {  if(RS485._motor.mode == 'S')
                RS485._global.diag_status1 &= 0xF7;
             if(RS485._motor.mode == 'W')        
                RS485._global.diag_status1 &= 0xFB;
          }
          else
          {  if(RS485._motor.mode == 'S')
                RS485._global.diag_status1 |= 0x8;
             if(RS485._motor.mode == 'W')
                RS485._global.diag_status1 |= 0x4;      
          } 
      }           
      // No more pulse needed. If bit 6 of phase (power on/off flag) 
      // is not set, power off motor driver after rotation completes.
      if((RS485._motor.phase & 0x40 ) == 0)
      { DISABLE_MOTOR_W; DISABLE_MOTOR_S;}      
      // TIMSK Bit 0 is cleared to disable Timer 0 interrupt. Stop pulses.
      TIMSK &= 0xFE; 
  }
  
}

/*****************************************************************************/
// Read ana_cmp_isr_counter. enquired by main loop for diagnostic. 
/*****************************************************************************/
u8 read_ana_cmp_isr_counter()
{
   return ana_cmp_isr_counter; 
}                              

/*****************************************************************************/
//                    Electrical magnet driver
// Define E_magnet working time, unity is 20ms
// Usage: E_Magnet_Driver(u16 pulse_num, u8 amp)
/*****************************************************************************/
#define MAX_AMP 100 
#define MIN_VIBRATING_TIME 4

void E_Magnet_Driver(u16 pls_num, u8 amp)
{                                            
   // Max amplitude is 85% duty based on experiment.OCR1A/ICR1 is pulse duty   
   magnet_amp = amp; 
   if(amp > MAX_AMP)
      magnet_amp = MAX_AMP; 

   // Initialize isr counter. 
   ana_cmp_isr_counter = 0;  
   // Magnet will work for pls_num * 20ms.   
   RS485._magnet.pulse_num = pls_num; 
      
   // initialize Timer flag.
   Timer1_flag = ONE_PLS_CMPLT;   
   // Clear AC interrupt flag, ACSR4 (ACI), either cleared by HW when int is serviced
   // or by writing "1" to ACI. There was once a bug. Also AC int is disabled, ACI
   // is set every 20ms (50HZ). so when interrupt is enabled again. interrupt is quickly
   // serviced. Actually interrupt should be serviced only when AC power supply crosses zero.
   // Enable Analog Comparator interrupt.     
   ACSR=0x1B;                    /* both AIN+ and AIN- are driven from external */
}

/*****************************************************************************/
//                    Stepping Motor driver
// set stepping motor speed and running mode. 
// Usage: Motor_Driver(arg1, arg2) 
// totally 6 available speed, from 1.5 cycle/s to 4 cycle/s. selected by arg1
// totally 4 working mode: selected by arg2
// 'N': Motor 1(top) and 2 (bottom) stop. 'B': Both Motor 1 and 2 work
// 'W': Weighing Motor(1) works.  'S': Storing Motor(2)works.
//  Arg poweron: 
//  0: power down motor driver after rotation. (back to optocoupler) 
//  >0 keep driver active.(departure, first half)
//            *   *
//         *        *
//       *            *
//       *            *
//         *        *
//            *   *
//       <          <
//   poweron>0      poweron = 0 
//    1st half      2nd half
/*****************************************************************************/
void Motor_Driver(u8 MotorMode,u8 pulsenum,u8 powerdown)
{
   // Motor driving frequency table, set to Timer register TCNT.
   // 0xDC: 1 cycle/s; 0xE8: 1.5 cycle/s; 0xEE: 2 cycles/s; 
   // 0xF0: 2.3 cycle/s; 0xF2: 2.6; 0xF4: 3; 0xF5: 3.3; 0xF6: 3.6; 0xF7: 4 cycle/s
   // 0xF8: 4.5 cycle/s; 0xF9: 5 cycle/s; 0xFA: 6 cycle/s; 0xFB: 7 cycle/s
   // Motor_Pulse_Freq[10]={0xF0,0xF2,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB}; 
   // totally 10 speed options       
   u8 i;
   i = sizeof(MOTOR_FREQ)/sizeof(u8); 
   if(RS485._flash.motor_speed >= i)
     RS485._flash.motor_speed = i-1;                            
       
   RS485._motor.pulse_num = pulsenum; // 1.8 degree per step. 200 pulses to run one cycle
   // To avoid motor mis-step at high speed, force motor to start at low speed 
   // during first several driving pulses. 
   // bit 5~0 of phase indicates how many pulses are given at low frequency.
   RS485._motor.phase = NUM_OF_STARTUP_PULSE; 
   // bit 6 of phase is motor power on/off flag after rotation.
   if (powerdown)
        RS485._motor.phase |= 0x40;  // set bit 6, keep motor driver active after rotation.  
   else
        RS485._motor.phase &= 0xBF; // clear bit 6, power off motor driver after rotation.
   RS485._motor.mode = MotorMode;  
   
   // Activate Motor back from Reset mode
   MOTOR_ROTATE_CLKWISE;        /* motor rotates clockwise */
   MT_MODE_BIT0 = 1;            /* motor mode is 00 (4 phases) */
   MT_MODE_BIT1 = 1; 

   switch(RS485._motor.mode)
   {
       case 'W':             	 
	 DISABLE_MOTOR_S; 
	 ENABLE_MOTOR_W;
         break;    
       case 'S':
	 DISABLE_MOTOR_W;	
	 ENABLE_MOTOR_S; 
	 break;      
       default:
         DISABLE_MOTOR_W; 
         DISABLE_MOTOR_S;
         break;
   }   
   // Stepping motor driver needs 40us max to wake up from power saving mode.
   for(i=128;i>0;i--);
             
   // Define Timer0 interrupt frequency           
   TCNT0 = MOTOR_FREQ[RS485._flash.motor_speed];    
   // Enable Timer0 overflow interrupt.
   TIMSK |= 0x1;            
}
 
/*****************************************************************************/
// Get motor status, return pules number left.
// if data returned is zero, motor already stopped.
/*****************************************************************************/
u8 Motor_Is_Running()
{
  return(RS485._motor.pulse_num);
}
 
/*****************************************************************************/
// Get magnet status, return pules number left.
// if data returned is zero, magnet already stopped.
//
// All the nodes share the same AC_cross_zero signal as the input to their embeded 
// Analog comparator. The Analog compartor interrupts when cross_zero is detected.
// Every analog compartor interrupt, magnet pulse number "RS485._magnet.pulse_num"
// decrease by one. If board has no AC_cross_zero signal connected to it's analog 
// comparator, no interrupt will happen. Therefore "RS485._magnet.pulse_num" never
// returns to zero. Subroutine Magnet_Is_Running()return TRUE when 
// "RS485._magnet.pulse_num" >0, So in the case mentioned above, Magnet_Is_Running()
// always returns TRUE. That will be a problem.
// We need to set a timeout limit (>20ms for 50HZ AC). If "RS485._magnet.pulse_num"
// >0 and doesn't decrease when timeout, something wrong happened with magnet driver,
// we should set flag to indicate "magnet_dead".
/*******************************************************************************/
#define SET_MAGNET_ERROR_BIT RS485._global.diag_status1 |= 0x1
#define CLEAR_MAGNET_ERROR_BIT  RS485._global.diag_status1 &= 0xFE
u16 Magnet_Is_Running()
{ 
  static u8 timer_set_already;
  static u16 last_pulse_num;  
  /* if pulse_num goes to zero, magnet stops running */
  if(RS485._magnet.pulse_num == 0)
  {  timer_set_already = 0;           /* clear flag anyway*/
     last_pulse_num = 0;
     return 0; 
  } 
  
  /* magnet is running, we have to set a limit to avoid infinite waiting */ 
  if(!timer_set_already)               /* timeout not set yet, set it*/
  {  
     kick_off_timer(6, 4);             /* kick off Timer service 6 for 40 ms timer */
     timer_set_already = 0xff;         /* set flag */
     last_pulse_num = RS485._magnet.pulse_num; 
     CLEAR_MAGNET_ERROR_BIT;
     return(RS485._magnet.pulse_num); 
  }
  
  /* magnet is running, and we have kicked off a timer */  
  if(!(os_sched.status & 0x40))                  /* time expires */
  { if(last_pulse_num == RS485._magnet.pulse_num)
    {  
       RS485._magnet.pulse_num = 0;    /* error happened, stop magnet pulses */ 
       SET_MAGNET_ERROR_BIT;
       return 0xffff;                  /* pulse num didn't decrease within 40ms, error*/
    }
    else
       return RS485._magnet.pulse_num; /* looks normal*/
  }
  
  /* magnet is running, and timer haven't expired yet, too early to make a conclusion */
  return(RS485._magnet.pulse_num);
}
