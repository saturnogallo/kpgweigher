#include <MEGA8.h>  
#include "define.h"

//delay after close signal is detected
#define SHIFT_PULSE_INCNUM            10
#define SHIFT_PULSE_DECNUM            10              

/*****************************************************************************/
//                       Hardware Related definition
/*****************************************************************************/
#define MOTORS_CLOSED   PIND.3 == 1
#define MOTORW_CLOSED   PIND.2 == 1      
                                                          
#define MAGNET_PULSE_PORT	PORTB.1
#define ENABLE_MOTOR_W          PORTC.0 = 1
#define DISABLE_MOTOR_W		PORTC.0 = 0
#define ENABLE_MOTOR_S		PORTC.1 = 1
#define DISABLE_MOTOR_S		PORTC.1 = 0
#define OUTPUT_ENABLE           PORTC.2 = 1
#define OUTPUT_DISABLE          PORTC.2 = 0
#define MOTOR_CLOCK_PIN         PORTC.4

flash u8 MOTOR_FREQ[]={CYCLE_2_3,CYCLE_2_6,CYCLE_3_0,CYCLE_3_3,CYCLE_3_6,CYCLE_4_0,CYCLE_4_5,CYCLE_5_0,CYCLE_6_0,CYCLE_7_0};        

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
//                     Timer 1 interrupt service routine
// Timer 1 overflow interrupt service routine.
// Timer 1 is used to generate pulse to drive electrical magnet.
// In every cycle, MAGNET_PULSE_PORT (PB.1) outputs 9.6K PWM pulses in first
// half cycle and keep low in the second half cycle. 
/*****************************************************************************/
#define MIN_AMP 10
#define MAX_AMP 85
#define HIGH_PULSE_NUM 3
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{  
  //static u16 n=0; 
  static u8 full_power_pulse_num;     
  if(RS485._magnet.pulse_num)
  {  
     RS485._magnet.pulse_num--;       
     full_power_pulse_num++;
     if(full_power_pulse_num < HIGH_PULSE_NUM)
        OCR1AL = MAX_AMP;
     else
        OCR1AL=(RS485._flash.magnet_amp + MIN_AMP);
     /*n++;
     if ((n >= RS485._magnet.half_period) && (n < 2*RS485._magnet.half_period))
     {  
        //Disconnect PWM output from OC1A pin
        TCCR1A = 0x00;
        MAGNET_PULSE_PORT = 0;                          
     }
     if(n >= 2*RS485._magnet.half_period)
     {  TCCR1A = 0x80;
        n=0;
     } //*/ 
  }
  else
  {  
     // Disable Timer 1 interrupt
     TIMSK &= 0xFB;               
     // Disconnect PWM output from OC1A pin
     // Power off electrical magnet to save power     
     TCCR1A = 0x00;
     MAGNET_PULSE_PORT = 0;    
     // reset pulse 
     full_power_pulse_num = 0;
     //n = 0;
  } //*/
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

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
  static u8 motor_close_happened;
  // Does the motor still needs more pulse (1 cycle completes?)? 
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
              {  if((RS485._motor.pulse_num > SHIFT_PULSE_INCNUM) && (MOTORW_CLOSED))
                     RS485._motor.pulse_num = SHIFT_PULSE_INCNUM;
              }
              if(RS485._motor.mode == 'S')
              {  if((RS485._motor.pulse_num > SHIFT_PULSE_INCNUM) && (MOTORS_CLOSED))
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
//                    Electrical magnet driver
// Define E_magnet working time, unity is 100ms
// Usage: E_Magnet_Driver(u16 arg)
//  
/*****************************************************************************/
#define PWM_BASE_FREQ 7200           // source frequency is 7200HZ, MCLK/1024
#define PULSE_NUM_IN_100MS 5         // 36HZ pulse, 3.6 pulses in 100ms
#define PWM_BASE_FREQ_DIV 118        // 36HZ = 7200/(2*PWM_BASE_FREQ_DIV)
#define MAGNET_LOW_END_FREQ 1
#define MAGNET_HIGH_END_FREQ 80
/*****************************************************************************/
void E_Magnet_Driver(u16 m_hundred_ms)
{                                            
   // Filter invalid magnet frequency settings, valid range is 1HZ~60HZ
   if(RS485._flash.magnet_freq < MAGNET_LOW_END_FREQ)
        return;
   if(RS485._flash.magnet_freq > MAGNET_HIGH_END_FREQ)
        return; 
   // Max amplitude is 85% duty based on experiment.OCR1A/ICR1 is pulse duty   
   if(RS485._flash.magnet_amp > (MAX_AMP - MIN_AMP))
      RS485._flash.magnet_amp = (MAX_AMP - MIN_AMP);      
  
   // calculate total pulse needed based on 9600HZ interupt freq.
   RS485._magnet.pulse_num = (u32)m_hundred_ms * PULSE_NUM_IN_100MS + HIGH_PULSE_NUM;   
   // Set Magnet frequency
   //RS485._magnet.half_period = (PWM_BASE_FREQ/2) / RS485._flash.magnet_freq;

   //////////////////////////////////////////////////////////////////////////
   // BUILD POWER SUPPLY (PWM Switching Regulator Tech.) and SETUP Amplitude
   // ICR is used to set PWM base frequency: 921600/(2*48)=9600HZ.
   // 921600 is the Timer1 clock freq defined in Init_Timers()
   // This PWM pulse is used to build power suply for Magnet.
   // Set OC1A PWM pulse duty. Power supply amp can be changed by adjusting
   // PWM pulse duty. Note that OCR1A should not exceed ICR setting,
   // "OCR1A = ICR1" means 100% duty, or max power supply amplitude.
   //////////////////////////////////////////////////////////////////////////
   ICR1H=0x00;
   ICR1L= PWM_BASE_FREQ_DIV - RS485._flash.magnet_freq *2;
   // Set power supply amplitude
   OCR1AH=0x00;
   OCR1AL=(RS485._flash.magnet_amp + MIN_AMP);   
   //connect OC1A to PB1 pin.
   TCCR1A = 0x80;
   // Enable Timer1 overflow interrupt.  
   TIMSK |= 0x4;    
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
       case 'B':
         ENABLE_MOTOR_W;
         ENABLE_MOTOR_S;
         break;       
       default:
         DISABLE_MOTOR_W; 
         DISABLE_MOTOR_S;
         break;
   }   
   // Stepping motor driver needs 40us max to wake up from power saving mode.
   for(i=255;i>0;i--);
             
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
/*****************************************************************************/ 
u16 Magnet_Is_Running()
{
  return(RS485._magnet.pulse_num);
}
