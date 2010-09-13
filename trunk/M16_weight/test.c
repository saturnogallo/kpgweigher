// test files.

#include "cs5532.h"
#include "uart.h"
#include "motor.h"
#include "define.h"   

/***********************************************************************************/
//                        Motor Magnet Test Subroutine
/***********************************************************************************/
#ifdef _TEST_MOTOR_MAGNET_
void Test_Motor_Magnet_Loop()
{
   u8 i;
   // Motor & Magnet settings
   RS485._flash.magnet_amp = 60; 
   RS485._flash.magnet_time = 10; 
   RS485._flash.motor_speed = 3;
   sleepms(1000);
   // Test Motor & Magnet 
   sleepms(5000);                      
   while(1)
   {                         
      while(Motor_Is_Running()) ;
      LED_ON(PIN_RxOK);
      Motor_Driver('W',MOTOR_ONE_CYCLE_NO_SENSOR,POWER_OFF_AFTER_ROTATION);            
      while(Motor_Is_Running()) ;
      LED_OFF(PIN_RxOK);
      sleepms(1000);
      
      LED_ON(PIN_TxOK);
      Motor_Driver('S',MOTOR_ONE_CYCLE_NO_SENSOR,POWER_OFF_AFTER_ROTATION);
      while(Motor_Is_Running()) ;      
      LED_OFF(PIN_TxOK); 
      sleepms(1000);
          
      /************** Test Magnet ****************/
      /*while(Magnet_Is_Running());
      LED_ON(PIN_RUN);  
      E_Magnet_Driver(200);
      while(Magnet_Is_Running());
      LED_OFF(PIN_RUN);
      sleepms(1000);*/ 
   }
} 
#endif
