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
      LED_ON(LED_D6);
      Motor_Driver('W',MOTOR_ONE_CYCLE_NO_SENSOR,POWER_OFF_AFTER_ROTATION);            
      while(Motor_Is_Running()) ;
      LED_OFF(LED_D6);
      sleepms(1000);
      
      LED_ON(LED_D8);
      Motor_Driver('S',MOTOR_ONE_CYCLE_NO_SENSOR,POWER_OFF_AFTER_ROTATION);
      while(Motor_Is_Running()) ;      
      LED_OFF(LED_D8); 
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

#define WAIT_TIME 8000
#define CAL_TIME 2000
#ifdef _AD_CALIBRATION_
void Factory_Calibration()
{ 
   u16 j;
   u8 Databuf[2];
   
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
   //cs_zero
   sleepms(WAIT_TIME);
   LED_OFF(LED_D6);
   LED_OFF(LED_D7);
   LED_OFF(LED_D8);
   while(1) { 
     CS5532_ReadADC(Databuf); 
     if ((Databuf[0] != 0) || (Databuf[1] != 0)) { 
       j = Databuf[0];
       RS485._flash.cs_zero = j<<8 + Databuf[1];
       break;
     }   
   }
   sleepms(CAL_TIME);
   
   //poise0
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
   sleepms(WAIT_TIME);
   LED_ON(LED_D6);
   LED_OFF(LED_D7);
   LED_OFF(LED_D8);
   while(1) { 
     CS5532_ReadADC(Databuf); 
     if ((Databuf[0] != 0) || (Databuf[1] != 0)) { 
       j = Databuf[0];
       RS485._flash.cs_poise[0] = j<<8 + Databuf[1];
       break;
     }   
   }
   sleepms(CAL_TIME);
   
   //poise1
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
   sleepms(WAIT_TIME);
   LED_OFF(LED_D6);
   LED_ON(LED_D7);
   LED_OFF(LED_D8);
   while(1) { 
     CS5532_ReadADC(Databuf); 
     if ((Databuf[0] != 0) || (Databuf[1] != 0)) { 
       j = Databuf[0];
       RS485._flash.cs_poise[1] = j<<8 + Databuf[1];
       break;
     }   
   }
   sleepms(CAL_TIME);
  
   //poise2
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
   sleepms(WAIT_TIME);
   LED_ON(LED_D6);
   LED_ON(LED_D7);
   LED_OFF(LED_D8);
   while(1) { 
     CS5532_ReadADC(Databuf); 
     if ((Databuf[0] != 0) || (Databuf[1] != 0)) { 
       j = Databuf[0];
       RS485._flash.cs_poise[2] = j<<8 + Databuf[1];
       break;
     }   
   }
   sleepms(CAL_TIME); 
   
   //poise3
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
   sleepms(WAIT_TIME);
   LED_OFF(LED_D6);
   LED_OFF(LED_D7);
   LED_ON(LED_D8);
   while(1) { 
     CS5532_ReadADC(Databuf); 
     if ((Databuf[0] != 0) || (Databuf[1] != 0)) { 
       j = Databuf[0];
       RS485._flash.cs_poise[3] = j<<8 + Databuf[1];
       break;
     }   
   }
   sleepms(CAL_TIME);   
   
   //poise4
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
   sleepms(WAIT_TIME);
   LED_ON(LED_D6);
   LED_OFF(LED_D7);
   LED_ON(LED_D8);
   while(1) { 
     CS5532_ReadADC(Databuf); 
     if ((Databuf[0] != 0) || (Databuf[1] != 0)) { 
       j = Databuf[0];
       RS485._flash.cs_poise[4] = j<<8 + Databuf[1];
       break;
     }   
   }
   sleepms(CAL_TIME); 
   LED_ON(LED_D6);
   LED_ON(LED_D7);      
   LED_ON(LED_D8);
}                 

#endif

