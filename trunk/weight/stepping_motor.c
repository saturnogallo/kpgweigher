#include "MEGA8.h"
#include "define.h" 

//delay after close signal is detected
#define SHIFT_PULSE_INCNUM            2
#define SHIFT_PULSE_DECNUM            2              

/*****************************************************************************/
//                       Hardware Related definition
/*****************************************************************************/
#define MOTORS_CLOSED   PIND.3 == 1
#define MOTORW_CLOSED   PIND.2 == 1      
#define TRIG_TRIAC()    PORTB.1 = 1 
#define TRIG_CMPLT()    PORTB.1 = 0
                                                          
#define MAGNET_PULSE_PORT	PORTB.1
#define ENABLE_MOTOR_W          PORTC.0 = 1
#define DISABLE_MOTOR_W		PORTC.0 = 0
#define ENABLE_MOTOR_S		PORTC.1 = 1
#define DISABLE_MOTOR_S		PORTC.1 = 0
#define OUTPUT_ENABLE           PORTC.2 = 1
#define OUTPUT_DISABLE          PORTC.2 = 0
#define MOTOR_CLOCK_PIN         PORTC.4

flash u8 MOTOR_FREQ[]={CYCLE_2_3,CYCLE_2_6,CYCLE_3_0,CYCLE_3_3,CYCLE_3_6,CYCLE_4_0,CYCLE_4_5,CYCLE_5_0,CYCLE_6_0,CYCLE_7_0};        


#define SET_BY_ACINT 1
#define SET_BY_TMR1 2
#define ONE_PLS_CMPLT 3

u8 Timer1_flag;  
/*****************************************************************************/
//  Generate a clock pulse to move stepping motor forward a step
/*****************************************************************************/
void Motor_One_Step()
{
  u8 i;
  for(i=20;i>0;i--)
     MOTOR_CLOCK_PIN = 0;
  for(i=20;i>0;i--)
     MOTOR_CLOCK_PIN = 1; 
}
 
/*****************************************************************************/
//              Analog Comparator interrupt service routine 
// Interrupt when AC power supply cross zero (voltage)
// As a method to control magnet magtitude (power supply), timer is started 
// to delay for a certain before turns on Triac to drive magnet.
// Triac automatically turns off when 220V AC power supply 
//
//     *  *
//   *      *
//  *-------|-*-----------*
//  |       | | *        *|
//  |       | |    *  *   |
// ZC(Int)  | |ZC         |ZC(interrupt)
//  |       | |
//  |------>| |           
//  | Delay | |
//  |       | |
//       -->| |<-- Triac On
//  |       | |
//          |<---- Trigger Triac on   
//
//  ZC: Zero Cross
/*****************************************************************************/
//#define MAX_TRIAC_ANGEL  (65535-(181-13)*6) //181-13  // minus 13 based on actual offset
#define COUNTER_BASE (65535-(181-13)*6) 
#define AMP_ADJUST 25
// 176: Max triac ON range: 0 ~ 165 degree. 0.87ms Triac ON time.
// 181: Max triac ON range: 0 ~ 170 degree. 0.55ms Triac ON time 
// 186: Max triac ON range: 0 ~ 175 degree. 0.3ms Triac ON time 
interrupt [ANA_COMP] void ana_comp_isr(void)
{
  u16 temp; 
  if(RS485._magnet.pulse_num) 
      RS485._magnet.pulse_num--; 
  else
  {  ACSR &= 0xF7;  // disable AC interrupt
     TIMSK &= 0xFB; // disable timer1 OV interrupt.
     Timer1_flag = ONE_PLS_CMPLT;  
     TRIG_CMPLT(); //  shut down triac.
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
    temp =  RS485._flash.magnet_amp + AMP_ADJUST; 
    TCNT1 = COUNTER_BASE + temp<<3;
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
     //return;
   }
   else
   //if(Timer1_flag == SET_BY_TMR1)
   {  TRIG_CMPLT();            
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

#define RETURN_TO_SENSOR ((RS485._motor.pulse_num>SHIFT_PULSE_INCNUM) && (RS485._motor.pulse_num < 50))

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
  static u8 motor_close_happened;
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
      {   TCNT0 = MOTOR_FREQ[RS485._flash.motor_speed];
      }
      // Motor to Move one step
      Motor_One_Step(); 
      // If Bit4 of test_mode_reg1 is unset, motor sensors are enabled. see "define.h".
      if(ENABLE_MOTOR_SENSORS)
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
                 if((RETURN_TO_SENSOR) && (MOTORW_CLOSED))
                     RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;
              }
              if(RS485._motor.mode == 'S')
              {  if((RETURN_TO_SENSOR) && (MOTORS_CLOSED))
                     RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;
              }
              if((MOTORW_CLOSED)||(MOTORS_CLOSED))
                 RS485._motor.phase |= CLOSURE_DETECTED;   //bit 7 as a flag.   
          }
      }        
  }   
  // NO MORE PULSES.
  else // DEBUG INFORMATION IN STATUS REGISTER
  {  
      if(ENABLE_MOTOR_SENSORS)
      {
          if(RS485._motor.phase && CLOSURE_DETECTED) // if bit 7 is set. 
          {  if(RS485._motor.mode == 'S')
                RS485._global.hw_status &= 0xFE;
             if(RS485._motor.mode == 'W')
                RS485._global.hw_status &= 0xFD;
          }
          else
          {  if(RS485._motor.mode == 'S')
                RS485._global.hw_status |= 0x1;
             if(RS485._motor.mode == 'W')
                RS485._global.hw_status |= 0x2;      
          } 
      } //*/          
      // No more pulse needed. If bit 6 of phase (power on/off flag) 
      // is not set, power off motor driver after rotation completes.
      if((RS485._motor.phase & 0x40 ) == 0)
      { DISABLE_MOTOR_W; DISABLE_MOTOR_S;}      
      // TIMSK Bit 0 is cleared to disable Timer 0 interrupt. Stop pulses.
      TIMSK &= 0xFE; 
  } 
}

/*****************************************************************************/
//                    Electrical magnet driver
// Define E_magnet working time, unity is 20ms
// Usage: E_Magnet_Driver(u16 arg)
/*****************************************************************************/
#define MAX_AMP 75
void E_Magnet_Driver(u16 pls_num)
{                                            
   // Max amplitude is 85% duty based on experiment.OCR1A/ICR1 is pulse duty   
   if(RS485._flash.magnet_amp > MAX_AMP)
      RS485._flash.magnet_amp = MAX_AMP; 
        
   // Magnet will work for pls_num * 20ms.
   RS485._magnet.pulse_num = pls_num;   
   
   // initialize Timer flag.
   Timer1_flag = ONE_PLS_CMPLT;   
   // Clear AC interrupt flag, ACSR4 (ACI), either cleared by HW when int is serviced
   // or by writing "1" to ACI. There was once a bug. Also AC int is disabled, ACI
   // is set every 20ms (50HZ). so when interrupt is enabled again. interrupt is quickly
   // serviced. Actually interrupt should be serviced only when AC power supply crosses zero.
   // Enable Analog Comparator interrupt.     
   ACSR=0x5B;  
}

/*****************************************************************************/
//                    Stepping Motor driver
// set stepping motor speed and running mode. 
// Usage: Motor_Driver(arg1, arg2) 
// totally 6 available speed, from 2.3 cycle/s to 4 cycle/s. selected by arg1
// totally 4 working mode: selected by arg2
// 'N': Motor 1(top) and 2 (bottom) stop. 'B': Both Motor 1 and 2 work
// 'W': Weighing Motor(1) works.  'S': Storing Motor(2)works.
//  Arg powerdown, 0->power down motor driver after rotation. >0 keep driver active.
/*****************************************************************************/
#define NUM_OF_STARTUP_PULSE 25
void Motor_Driver(u8 MotorMode,u8 pulsenum,u8 powerdown)
{
   // Motor driving frequency table, set to Timer register TCNT.
   // 0xF0: 2.3 cycle/s; 0xF2: 2.6; 0xF4: 3; 0xF5: 3.3; 0xF6: 3.6; 0xF7: 4 cycle/s
   // 0xF8: 4.5 cycle/s; 0xF9: 5 cycle/s; 0xFA: 6 cycle/s; 0xFB: 7 cycle/s
   // Motor_Pulse_Freq[10]={0xF0,0xF2,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB}; 
   // totally 10 speed options       
   u8 i;
   if(RS485._flash.motor_speed >= (sizeof(MOTOR_FREQ)/sizeof(u8)))
        return;
   RS485._motor.pulse_num = pulsenum; // 1.8 degree per step. 200 pulses to run one cycle
   // To avoid motor mis-step at high speed, force motor to start at low speed 
   // during first several driving pulses. 
   // bit 5~0 of phase indicates how many pulses are given at low frequency.
   RS485._motor.phase = NUM_OF_STARTUP_PULSE; 
   // bit 6 of phase is motor power on/off flag after rotation.
   if (powerdown > 0)
        RS485._motor.phase |= 0x40;  // set bit 6, keep motor driver active after rotation.
   else
        RS485._motor.phase &= 0xBF; // clear bit 6, power off motor driver after rotation.
   RS485._motor.mode = MotorMode;  
   
   // Activate Motor back from Reset mode
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
extern u16 timerlen[4]; 
#define SET_MAGNET_ERROR_BIT RS485._global.hw_status |= 0x4
#define CLEAR_MAGNET_ERROR_BIT  RS485._global.hw_status &= 0xFB
u16 Magnet_Is_Running()
{ 
  static u8 timer_set_already;
  static u16 last_pulse_num;  
  /* if pulse_num goes to zero, magnet stops running */
  if(RS485._magnet.pulse_num == 0)
  {  timer_set_already = 0;           /* clear flag anyway*/
     last_pulse_num = 0;
     CLEAR_MAGNET_ERROR_BIT;  
     return 0; 
  } 

  /* magnet is running, we have to set a limit to avoid infinite waiting */ 
  if(!timer_set_already)               /* timeout not set yet, set it*/
  {  
     timerlen[3] = 4;                  /* set timer2 to 4*10ms */
     TCNT2=0xB8;                       /* 10ms timer tick */

     TIFR |= (1<<TOV2);                /* clear overflow flag */

     TIMSK |= 0x40;                    /* enable TMR2 overflow interrupt.*/
     timer_set_already = 0xff;         /* set flag */
     last_pulse_num = RS485._magnet.pulse_num; 
     CLEAR_MAGNET_ERROR_BIT;
     return(RS485._magnet.pulse_num); 
  }

  /* magnet is running, and we have kicked off a timer */  
  if(timerlen[3] ==0)                  /* time expires */
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
