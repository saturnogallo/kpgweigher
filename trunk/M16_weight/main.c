/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.8b Professional
Automatic Program Generator
© Copyright 1998-2006 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2010-5-2
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega16L
Program type        : Application
Clock frequency     : 7.372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/

/*************************************************************************************************************/

/*************************************************************************************************************/
#include <mega16.h>
#include "spi.h"
#include "cs5532.h"
#include "drveeprom.h" 
#include "uart.h"
#include "motor.h"
#include "define.h"  
#include "command.h" 

#define CS5532_WORD_RATE WORD_RATE_12P5SPS        /* CS5530 conversion speed */
#define ZERO_ADJUSTMENT_INTREVAL 200              /* reset weight every 200 releases */

NODE_CONFIG RS485;                                /* structure of node */
u8 myaddr;                                        /* rs485 addr of current node */
u16 pre_feed_weight;                              /* initial weight before adding material*/  
u16 release_times = ZERO_ADJUSTMENT_INTREVAL;     /* how many times node has been released */
u8 flag_turn_off_watchdog;    
/*********************************************************************************************************************/
// Boot vetctors: This structure is used to share boot configuration info between primary firmware and boot firmware.
/*********************************************************************************************************************/
#define BOOT_VECT_ADDR 0x60
BOOT_CONFIG boot_vector @ BOOT_VECT_ADDR;  /* store boot vector at a specified address in SRAM for sharing with primary */
                                           /* firmware. The first 96 (0x0~0x5F) addresses are reserved for I/O and registers */
                                            
/*****************************************************************************/
// TASK list of state machine used in main function
// This can be understand as different commands    
// Master board sets NREG_ENABLE to "1" to start machine. Procedure main()here 
// will set "flag_enable" to ENABLE_ON if flag_enable == ENABLE_START_MACHINE. 
/*****************************************************************************/
#define ENABLE_OFF              0    // stop machine.
#define ENABLE_START_MACHINE    1    // for mannual manipulation. 
#define ENABLE_INIT_AD          2    // initialize AD
#define ENABLE_TURNS            3    // rotate upper motor
#define ENABLE_TURNW            4    // rotate lower motor
#define ENABLE_VIBRATE          5    // start vibrator  
#define ENABLE_RESET_WEIGHT     7    // reset current weight to 0     
#define ENABLE_ON               80   // machine is fully running now.

/*****************************************************************************/
//                     Timer 2 interrupt service routine
// Timer 2 overflow interrupt service routine.
// Generate 10ms interrupt.
// Support up to 3 timer services. timerlen[3] store counters for different 
// timers.
/*****************************************************************************/
//bit timeoutflag;
#define DELAY1_END timerlen[0]==0  
#define DELAY2_END timerlen[1]==0 
#define DELAY3_END timerlen[2]==0
u16 timerlen[4] = {0,0,0,0};

interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
  TCNT2=0xB8;            // 7.2KHZ input,interrupt frequency is set to 100HZ (10ms timer)
  // if timerlen != 0, at least 1 timer service is ongoing.
  if(timerlen[0] || timerlen[1] || timerlen[2] || timerlen[3]){
    if(timerlen[0]>0)   timerlen[0]--;
    if(timerlen[1]>0)   timerlen[1]--;
    if(timerlen[2]>0)   timerlen[2]--; 
    if(timerlen[3]>0)   timerlen[3]--;     
  }else{
    TIMSK &= 0xbf;       // mask Timer2 overflow interrupt
  } 
}
  
/*****************************************************************************/
//                      Initialize Timers
// Timer 0 is used to generate pulses to drive electrical magnet
// Timer 1 is used to generate pulses to drive motors  
// Timer 2 is used to measure RS485 frame intervals. 
//
/*****************************************************************************/
void Init_Timers()
{
   // For Stepping Motor
   // Timer/Counter 0 initialization
   // Clock source: System Clock
   // Clock value: 7.200 kHz. Configured in TCCR0
   TCCR0=0x05;
   // define Timer0 interrupt frequency: 7.2K/(FF-TCNT) = 514HZ
   // 200 pulses to run one cycle (1.8/step)
   // Motor Speed: 514/200 ~= 2.5 cycles/sec
   TCNT0=0xF2;  
      
   // Timer/Counter 1 initialization 
   // Used for delay from 220V AC cross zero point to Triac ON.
   // Clock source: System Clock
   // Clock value: 115.200 kHz
   // Mode: Normal top=FFFFh
   // OC1A output: Discon.
   // OC1B output: Discon.
   // Noise Canceler: Off
   // Input Capture on Falling Edge
   // Timer 1 Overflow Interrupt: On
   // Input Capture Interrupt: Off
   // Compare A Match Interrupt: Off
   // Compare B Match Interrupt: Off
   TCCR1A=0x00;
   TCCR1B=0x03;
   TCNT1H=0x00;
   TCNT1L=0x00;
   ICR1H=0x00;
   ICR1L=0x00;
   OCR1AH=0x00;
   OCR1AL=0x00;
   OCR1BH=0x00;
   OCR1BL=0x00;
   
   // For Time Delay
   // Timer/Counter 2 initialization
   // Clock source: System Clock
   // Clock value: 7.200 kHz
   // Mode: Normal top=FFh
   // OC2 output: Disconnected
   ASSR=0x00;
   TCCR2=0x07;
   TCNT2=0x00;
   OCR2=0x00;
   
   // Timer(s)/Counter(s) Interrupt(s) initialization
   // disable Timer 0, 1 and 2 interrupt
   TIMSK=0x00;     
}   

/*******************************************************************************/
//                          MEGA8 PORT initialization  
/*******************************************************************************/
void Init_PORT(void)
{

   // Input/Output Ports initialization
   // Port A initialization
   // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
   // State7=T State6=T State5=1 State4=1 State3=1 State2=1 State1=1 State0=1
   // PA0: MT1_PS, PA1: MT2_PS, PA2: MT_MD0, PA3: MT_MD1, PA4: MT_CLK, PA5: MT_DIR
   // PA6: PWM_FB, PA7: Test1 
   PORTA=0x3F;
   DDRA=0x3F;

   // Port B initialization
   // Func7=Out Func6=In Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=Out 
   // State7=0 State6=T State5=0 State4=1 State3=T State2=T State1=1 State0=0 
   PORTB=0x12;
   DDRB=0xB3;

   // Port C initialization
   // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
   // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
   PORTC=0x00;
   DDRC=0x00;

   // Port D initialization
   // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=In 
   // State7=1 State6=1 State5=1 State4=1 State3=T State2=T State1=1 State0=T 
   PORTD=0xF2;
   DDRD=0xF2;

   // External Interrupt(s) initialization
   // INT0: Off
   // INT1: Off
   // INT2: Off
   MCUCR=0x00;
   MCUCSR=0x00;

   // Timer(s)/Counter(s) Interrupt(s) initialization
   TIMSK=0x00;

   // Analog Comparator initialization
   // Analog Comparator: Off
   // Analog Comparator Input Capture by Timer/Counter 1: Off
   ACSR=0x03;   // interrupt on rising edge, disable AC interrupt when intialized. To be enabled later.
   SFIOR=0x00;
   
   LED_OFF(PIN_RUN);
   LED_OFF(PIN_HWOK);
   LED_OFF(PIN_RxOK);
   LED_OFF(PIN_TxOK);      
}

/*******************************************************************************/
// Enable watchdog timer. 
// Timeout limit WDTCR[2:0]: 
// 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
// 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s 
/*******************************************************************************/
#ifdef _DISABLE_WATCHDOG_
//dummy subroutine
void Feed_Watchdog(){;;}
void Turn_Off_Watchdog(){;;}
#else
void Feed_Watchdog()
{ 
  #asm("cli")
  #asm("wdr") // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
  if(flag_turn_off_watchdog)
     WDTCR = 0x00; // turn off watch dog timer.
  else
     WDTCR = 0x0F; // turn on watch dog timer: WDE = 1, WDP = 110, 1 sec.
  #asm("sei") // Enable global interrupt.
}
void Turn_Off_Watchdog()
{ 
  #asm("cli")
  #asm("wdr") // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
  WDTCR = 0x00; // turn off watch dog timer
  #asm("sei") // Enable global interrupt.
}
#endif

/*******************************************************************************/
// This subroutine flashes LED in an idle period
/*******************************************************************************/
static u16 tick = 0;                              
#define MYDEAD(a) dead_loop(a)
void dead_loop(u16 refresh_cycle)
{ 
       tick++;
       if(tick == refresh_cycle)
       {
            LED_ON(PIN_RUN);
       }
       if(tick >= (refresh_cycle<<1))
       {
            LED_OFF(PIN_RUN);
            tick = 0;
       }
}    

/*****************************************************************************/
//                     reset current weight to zero
/*****************************************************************************/
u8 reset_weight()
{    
   CS5532_PoiseWeight();
   if(RS485._global.cs_mtrl < MAX_VALID_DATA) {
      RS485._flash.cs_zero = RS485._global.cs_mtrl;
      return 0;
   }
   return 1; 
}

/*******************************************************************************/
//                      Release material State machine       
// Lower bucket/motor affected only. State flag: RSM_Flag 
/*******************************************************************************/
#define RM_READY_RELEASE        0              //wait to start the empty action
#define RM_WAIT_OPEN            1              //wait until the open action is done
#define RM_WAIT_OPEN_W          2              //wait until the openw timeout
#define RM_WAIT_SHIFT           3              //wait until the shift action is done 
#define RM_WAIT_DELAY_W         4              //wait until the shift move is done  
#define RM_DELAY_BE4_RESET      5              //wait for lower bucket to be stable

static u8 RSM_Flag = RM_READY_RELEASE;

void release_material()
{
    //status to indicate the state system is in
    //>200 means release material status
    //100~200 means motor_magnet status
    RS485._global.status = 200 + RSM_Flag;   
    /************************************************************/   
    //start to open the lower bucket.
    /************************************************************/
    if(RSM_Flag == RM_READY_RELEASE){
       if(!Motor_Is_Running()){
          Motor_Driver('W',MOTOR_SHIFT_CYCLE_OPEN, KEEP_POWER_ON);                   
          RSM_Flag = RM_WAIT_OPEN;
       }
       return;
    }                              
    /************************************************************/      
    //wait for motor to stop(bucket open) 
    //Once it stops, start timer to delay so that material have 
    //enough time to pass from the lower bucket to material pool.
    /************************************************************/   
    if(RSM_Flag == RM_WAIT_OPEN){       
       if(!Motor_Is_Running()){            
          if(RS485._flash.open_w > 0){
             // start timer2, use timerlen[0] as counter.
             timerlen[0] = RS485._flash.open_w;
             TCNT2=0xB8;
             TIFR |= (1<<TOV2);                   /* clear overflow flag */
             TIMSK |= 0x40;                       /* enable overflow interrupt */
          }
          RSM_Flag = RM_WAIT_OPEN_W;     
       }
       return;
    }
    /************************************************************/      
    //wait till delay eclips (material released to pool)
    //then start motor to close the bucket.
    /************************************************************/      
    if(RSM_Flag == RM_WAIT_OPEN_W){      
       if(DELAY1_END){                        
          if(ENABLE_MOTOR_SENSORS)
             Motor_Driver('W',HALF_CYCLE_CLOSE_WITH_SENSOR,POWER_OFF_AFTER_ROTATION);
          else
             Motor_Driver('W',HALF_CYCLE_CLOSE_NO_SENSOR,POWER_OFF_AFTER_ROTATION);
          
          RSM_Flag = RM_WAIT_SHIFT;     
       }
       return;
    }                                     
    /************************************************************/      
    // if motor stops (bucket closed), set timer to delay before
    // next action.
    /************************************************************/      
    if(RSM_Flag == RM_WAIT_SHIFT){       
       if(!Motor_Is_Running()){            
          if(RS485._flash.delay_w > 0){
             timerlen[0] = RS485._flash.delay_w;
             TCNT2=0xB8;
             TIFR |= (1<<TOV2);                  /* clear overflow flag */
             TIMSK |= 0x40;//set TOIE2 (bit 6 of TIMSK) to enable Timer 2 overflow interrupt.
          }
          RSM_Flag = RM_WAIT_DELAY_W;     
       }
       return;
    }
    /************************************************************/      
    // After delay, state machine goes back to inital state. 
    // Clear release flag. 
    /************************************************************/      
    if(RSM_Flag == RM_WAIT_DELAY_W){
       if(DELAY1_END){
          if(EN_RUNTIME_ZERO_ADJUST){             /* Runtime reset weight feature enabled */
             if (release_times > ZERO_ADJUSTMENT_INTREVAL){ /* weight needs to be reset again */
                release_times = 0;                /* reset counter */
                timerlen[0] = 200;                /* delay 200*10ms */
                TCNT2=0xB8;                       /* 10ms timer */
                TIFR |= (1<<TOV2);                /* clear overflow flag */
                TIMSK |= 0x40;                    /* enable TMR2 overflow interrupt.*/
                RSM_Flag = RM_DELAY_BE4_RESET;    /* move to next state */
             }
             else                                /* not a reset cycle */
                release_times++; 
          }  /* EN_RUNTIME_ZERO_ADJUST */
          else {                                 /* feature not enabled */
             RS485._global.flag_release = 0;      /* set release complete flag */
             RSM_Flag = RM_READY_RELEASE;
          }
       } /* DELAY1_END */
       return;
    }
    /************************************************************/      
    // If weight needs to be reset, wait 2s before reset weight
    /************************************************************/ 
    if(RSM_Flag == RM_DELAY_BE4_RESET){
       if(DELAY1_END){
         if(!reset_weight()) {                    /* weight was reset successfully */
            RS485._global.flag_release = 0;       /* set release complete flag */
            RSM_Flag = RM_READY_RELEASE;
         }
       }
       return;	
    }

}

/*******************************************************************************/
//                    Feeder(Magnet) Auto Adjustment Algorithm
// This subroutine automatically adjust amplitude and working time of magnet
// to produce approximately 1/4 of total target weight in lower bucket.
// Current weight is compared with node target weight. Parameter "magnet_amp" 
// and "magnet_time"are adjusted accordingly.
// This subroutine is enabled by Bit 1 of test_mode_reg1 (EN_RUNTIME_MAGNET_ADJ)
/*******************************************************************************/

/*******************************************************************************
//                  Magnet State Machine
// magnet_add_material()is called during the idle/delay period
// of motor_magnet_action()and release_material(). 
// this sub-state-machine quit only when one state completes. 
// So don't worry about that its state may be disturbed. 
// State Flag:  WSM_Flag  
// we need to divide the state of MM_WAIT_ADDEND and MM_DELAY_BEFORE_OPEN_MOTOR_S
// into more sub-states so that we can parallelly embeded Magnet State Machine
// into other state machine. 
/*******************************************************************************/
#define MM_PASS_MATERIAL              0 
#define MM_WAIT_OPEN                  1
#define MM_WAIT_OPEN_S                2 
#define MM_WAIT_PASSEND               4 
#define MM_DELAY_BEFORE_OPEN_MOTOR_S  5
#define MM_ADD_MATERIAL               6
#define MM_WAIT_ADDEND                7
#define MM_WAIT_DELAYEND              9
#define MM_CHECK_WEIGHT               10  
#define MM_GET_PRE_FEED_WEIGHT        11
#define MM_GET_PRE_FEED_WEIGHT_2ND    12
#define MM_SET_MAGNET_AMP             13


//Initialize state of Motor/Magnet/Weight State Machine.
static u8  WSM_Flag = MM_ADD_MATERIAL; 
u8 magnet_add_material()
{   
    static u16 prev_weight;  
    /**************************************************************/
    // Get weight before feeding for calucation later
    /**************************************************************
     if(WSM_Flag == MM_GET_PRE_FEED_WEIGHT) {     
        // if release ongoing (1st feed action), start to add material
        if(RS485._global.flag_release) {
           pre_feed_weight = 0;
           WSM_Flag = MM_SET_MAGNET_AMP; // skip data stability check
        }else { // complemental feed
           pre_feed_weight = 0xffff;
           // if weight is valid, record it and go to next state to 
           // check if data is stable.
           if(RS485._global.Mtrl_Weight_gram < MAX_VALID_DATA) {
              prev_weight = RS485._global.Mtrl_Weight_gram;
              WSM_Flag = MM_GET_PRE_FEED_WEIGHT_2ND; 
           }          
        }
        return WSM_Flag;     
     } */
             
    /**************************************************************/
    // Get weight before feeding for calucation later
    /**************************************************************
     if(WSM_Flag == MM_GET_PRE_FEED_WEIGHT_2ND) {    
        // if current weight value matches previous one, pre_feed_weight
        // is got.Move to next state, otherwise, go to previous state
        if((RS485._global.Mtrl_Weight_gram < prev_weight+2) && 
           (prev_weight < RS485._global.Mtrl_Weight_gram+2)) {
           pre_feed_weight = RS485._global.Mtrl_Weight_gram;
           WSM_Flag = MM_SET_MAGNET_AMP;
        } else 
           WSM_Flag = MM_GET_PRE_FEED_WEIGHT_1ST; 
        return WSM_Flag;
     } */
    /**************************************************************/
    // Runtime adjust amplitude/time of magnet
    /**************************************************************/
     if(WSM_Flag == MM_SET_MAGNET_AMP) {    
        
        WSM_Flag = MM_ADD_MATERIAL; 
        return WSM_Flag;     
     }
    /**************************************************************/
    //  start Magnet
    /**************************************************************/      
     // wait from magnet to feed material
     if(WSM_Flag == MM_ADD_MATERIAL){ 			      
        E_Magnet_Driver(RS485._flash.magnet_time); 
        WSM_Flag = MM_WAIT_ADDEND;
	return WSM_Flag;
     }           
    /**************************************************************/
    // wait for magnet to stop, then delay before motor action.
    /**************************************************************/    
     if(WSM_Flag == MM_WAIT_ADDEND){  
       // delay before motor action if magnet stops
       if((!Magnet_Is_Running()) || (Magnet_Is_Running()== 0xffff)){
         if(RS485._flash.delay_f > 0){
            timerlen[1] = RS485._flash.delay_f; /* start timer2, timerlen[1] as the counter.*/
            TCNT2=0xB8;                         /* 10ms interrupt */
            TIFR |= (1<<TOV2);                  /* clear overflow flag */
            TIMSK |= 0x40;                      /* set TOIE2 (bit 6 of TIMSK) to enable Timer2 interrupt */
         }                           
         // move to next state.
         WSM_Flag = MM_DELAY_BEFORE_OPEN_MOTOR_S;
       }	   
	 return WSM_Flag;
     }
    /**************************************************************/                           
    // Hold in this state for a while so that material have enough
    // time to fall into upper bucket.
    /**************************************************************/
     if(WSM_Flag == MM_DELAY_BEFORE_OPEN_MOTOR_S){
         if(DELAY2_END){ //if delay ends, move to next state.
           WSM_Flag = MM_PASS_MATERIAL;
         }
         return WSM_Flag;     
     }
   
   // other states.
    return WSM_Flag;
}

/*******************************************************************************/
//                     Motor/Weight State Machine
// This state machine uses several states to coordate 2 motors and 1 magnet to action
// properly. Basic flow is: 
// (1) magnet to feed material to upper bucket
// (2) delay to ensure material drops to the upper bucket
// (3) get weight before passing material to lower bucket
// (4) upper motor running to open upper bucket, passing material to lower bucket
// (5) delay to make sure the upper bucket is closed completed
// (6) measure weight of material
// (7) lower motor running to open lower bucket, pass material to the pool
// (8) delay to make sure lower bucket is completely closed
// Guideline: 
// Check whether lower bucket is closed before open the upper bucket
// Check whehter upper bucket is closed before open the lower bucket
// Every time only 1 bucket is allowed to be opened.       
// State Flag: WSM_Flag
/*******************************************************************************/          
void motor_magnet_action()
{                                                
    //status to indicate the state system is in
    //>200 means release material status
    //100~200 means motor_magnet status
     RS485._global.status = 100 + WSM_Flag;
    /**************************************************************/ 
    // Start magnet to add material, if returned status is not
    // MM_PASS_MATERIAL, operation of adding mtrl not finished yet.
    /**************************************************************/           
     if(magnet_add_material() == MM_WAIT_ADDEND) 
          return;                               
     if(magnet_add_material() == MM_DELAY_BEFORE_OPEN_MOTOR_S) 
          return;                                                                                  
    /**************************************************************/      
    // upper bucket opens to pass material to the lower bucket      
    /**************************************************************/ 
     if(WSM_Flag == MM_PASS_MATERIAL){  
        //need to check whether lower bucket is open
        //don't open upper bucket until lower bucket is closed
        if(!Motor_Is_Running()){           
           Motor_Driver('S',MOTOR_SHIFT_CYCLE_OPEN, KEEP_POWER_ON);
           // upper motor is running now... enter next state,           
           WSM_Flag = MM_WAIT_OPEN;
        }
        return;                        
     }
    /**************************************************************/      
    // wait for the upper motor to stop(bucket open action stops),
    // then delay for "RS485._flash.open_s" so that material can be 
    // passed from upper bucket to the lower bucket. delay is important
    // especially when material falls very slowly. Need to enlong the
    // delay for such kind of material.
    /**************************************************************/     
     if(WSM_Flag == MM_WAIT_OPEN){   
        if(!Motor_Is_Running()){        
           if(RS485._flash.open_s > 0){
              //start timer2,timerlen[2] as the counter 
              timerlen[2] = RS485._flash.open_s;
              TCNT2=0xB8;                         /* 10ms interrupt */  
              TIFR |= (1<<TOV2);                  /* clear overflow flag */
              TIMSK |= 0x40;                      /* enable TMR2 overflow interrupt */
           }                             
           WSM_Flag = MM_WAIT_OPEN_S;             
        }
        return;
     }
    /**************************************************************/      
    // now waiting (motor stops/bucket open) to give material enough
    // time to drop to the lower bucket, then start motor to close 
    // upper bucket.
    /**************************************************************/     
     if(WSM_Flag == MM_WAIT_OPEN_S){  
        if(DELAY3_END){
           if(ENABLE_MOTOR_SENSORS)
                Motor_Driver('S',HALF_CYCLE_CLOSE_WITH_SENSOR, POWER_OFF_AFTER_ROTATION);                     
           else
                Motor_Driver('S',HALF_CYCLE_CLOSE_NO_SENSOR, POWER_OFF_AFTER_ROTATION);
           WSM_Flag = MM_WAIT_PASSEND;
        }
        return;                        
     }
    /**************************************************************/           
    // wait after upper motor stops to make sure upper bucket is closed
    // completely. This prevents magnet from feeding material when
    // upper bucket is not closed. This also provide a delay for
    // weight to stablize.
    /**************************************************************/          
     if(WSM_Flag == MM_WAIT_PASSEND){  
        if(!Motor_Is_Running()){
           if(RS485._flash.delay_s > 0){  
              timerlen[2] = RS485._flash.delay_s;
              TCNT2=0xB8;
              TIFR |= (1<<TOV2);                  /* clear overflow flag */
              TIMSK |= 0x40;
           }
           WSM_Flag = MM_WAIT_DELAYEND;
        }
        return;
     }                          
    /**************************************************************/      
    //OK, wait till delay ends                                      
    /**************************************************************/      
     if(WSM_Flag ==  MM_WAIT_DELAYEND){        
        if(DELAY3_END)
           WSM_Flag = MM_CHECK_WEIGHT;                 
        return;
     }  
    /**************************************************************/           
    // Ready to measure weight or do something else. 		                          
    /**************************************************************/           
     if(WSM_Flag ==  MM_CHECK_WEIGHT){  
        // flag "ENABLE_ON" indicates that system is running
        // (production mode vs. mannual operation mode).
        if(RS485._global.flag_enable != ENABLE_ON){
           WSM_Flag = MM_ADD_MATERIAL;
           return;
        }      

        // Code below is for condition "ENABLE_ON", when machine is working 
        // in automatic mode.
        // Set flag to inform master board new weight is being calculated
        RS485._global.flag_goon = 0;
        RS485._global.Mtrl_Weight_gram = INVALID_DATA ;  
        // Read Weight: valid and invalid weights are both possible.
        CS5532_PoiseWeight();  
        CS5532_Poise2Result();         
                
        // Idle here if no command from master board asking me to go on.
        // Combination algorithm will decide whether magnet should 
        // give more material. 2 conditions to break out of this loop:  
        // flag_goon = 1 or flag_enable not equal to ENABLE_ON any more.
        // These 2 registers may be changed by RS485 masters through UART. 
        // Add a third branch so that flag release command can also break the infinite loop. 11/13/2009
        WSM_Flag = MM_ADD_MATERIAL;
        while(RS485._global.flag_goon == 0){
           // 
           MYDEAD(0x2000);            
           // test conditions of breaking out of this "while" loop.
           if(RS485._global.flag_enable != ENABLE_ON){
              WSM_Flag = MM_ADD_MATERIAL; //break if extra command is coming 
              return;
           }
           if(RS485._global.flag_release) return; // release command pending for service
                
           // Update weight periodically
           CS5532_PoiseWeight();  
           CS5532_Poise2Result(); 
           /*
           if((RS485._global.Mtrl_Weight_gram < MAX_VALID_DATA) && 
              (RS485._global.Mtrl_Weight_gram > pre_feed_weight+TOLERANCE)) {
               weight_increment = RS485._global.Mtrl_Weight_gram - pre_feed_weight;
               pre_feed_weight = RS485._global.Mtrl_Weight_gram;
           } */
                                                  
           // and we can start magnet to feed material to upper bucket.          
           magnet_add_material();
           Feed_Watchdog(); // feed watch dog in case that we exceed the timeout limit.                    
        }    
        
        // Get command from masterboard/PC to go on (add material)
        //WSM_Flag = MM_ADD_MATERIAL;                  
        //invalidate weight result.
        RS485._global.Mtrl_Weight_gram = INVALID_DATA;                 
        return;
     }              
}                     
      
/*******************************************************************************/
//                    Initialize System Global Variable 
// Read data from EEPROM and set vaule for system global variables
/*******************************************************************************/
void Init_Vars(void)
{     
     RS485._global.flag_enable = ENABLE_OFF;
     RS485._global.flag_disable = 0;        
     RS485._global.flag_reset = 0;  
     RS485._global.flag_goon = 0;  
     RS485._global.flag_release = 0;          
     RS485._global.Mtrl_Weight_gram = INVALID_DATA;                    
     RS485._global.NumOfDataToBePgmed = 0;
     RS485._global.status = 0; 
     RS485._global.cs_status = 0;
     RS485._global.hw_status = 0; 
     RS485._global.test_mode_reg1 = 0;            
     RS485._global.test_mode_reg2 = 0;                 
     RS485._flash.board &= (BOARD_GROUP_MASK | BOARD_TYPE_MASK);    
     // Backup cs_zero, user may reset weight to zero if there is zero point shift.
     // In that case, cs_poise needs to be adjusted accordingly.
     // We will use the delta between cs_zero and old_cs_zero to do the adjustment.
     RS485._global.old_cs_zero = RS485._flash.cs_zero; 
 
     flag_turn_off_watchdog = 0; //clear flag 
}

/*******************************************************************************/
//                      Initialize EEPROM Parameters 
// This subroutine initializes and writes default system parameters into EEPROM 
// when software detects that parameters in EEPROM is invalid(RAW BOARD).
/*******************************************************************************/
#define ROM_DATA_VALID_FLAG 0xAA   
#define EN_EEPROM_PROG_BIT 0x8

void Init_EEPROM_Para()
 {  
   RS485._flash.addr = 15;
   // mannual initializing Flash variables before EEPROM data is ready.
   // only wordrate is valid for CS5530: 0x3 -> 12.5bps
   RS485._flash.cs_gain_wordrate = 0x3;   
   // Intialize RS485 baudrate to 115200 (0x0)
   RS485._flash.baudrate = 0x0;
   // Intialize filter option: 0x11: 0x10-> option 1, average + forecast
   // 0x01-> skip next 1*8 samples when data is dected unstable.
   RS485._flash.cs_Filter_option = 0x12;    
   // Board type
   //RS485._flash.board = BOARD_TYPE_VIBRATE;
   RS485._flash.board = BOARD_TYPE_WEIGHT;
   // Weight Calibration settings
   RS485._flash.target_weight = 100;
   //RS485._flash.offset_up = 25; // changed to addr_copy1/board_copy1
   RS485._flash.cs_zero = 0xBCE4;
   RS485._flash.cs_poise[0] = 0xC1B8;
   RS485._flash.Poise_Weight_gram[0] = 50;
   RS485._flash.cs_poise[1] = 0xC68F;
   RS485._flash.Poise_Weight_gram[1] = 100;
   RS485._flash.cs_poise[2] = 0xD03B;
   RS485._flash.Poise_Weight_gram[2] = 200;
   RS485._flash.cs_poise[3] = 0xD9E9;
   RS485._flash.Poise_Weight_gram[3] = 300;
   RS485._flash.cs_poise[4] = 0xED47;
   RS485._flash.Poise_Weight_gram[4] = 500;
   // Motor & Magnet settings
   //RS485._flash.magnet_freq = 9;
   RS485._flash.magnet_amp = 55; 
   RS485._flash.magnet_time = 10; 
   RS485._flash.motor_speed = 2;
   RS485._flash.delay_f = 10;
   RS485._flash.delay_w = 10;
   RS485._flash.delay_s = 10;
   RS485._flash.open_s  = 10;
   RS485._flash.open_w  = 10; 

#ifdef _BOARD_TYPE_IS_VIBRATE_
   RS485._flash.addr = 0xf;
   RS485._flash.board = BOARD_TYPE_VIBRATE;
#endif
   
   //backup key parameters
   RS485._flash.addr_copy1 = RS485._flash.addr;
   RS485._flash.addr_copy2 = RS485._flash.addr;
   
   RS485._flash.board_copy1 = RS485._flash.board;
   RS485._flash.board_copy2 = RS485._flash.board;
            
   RS485._flash.rom_para_valid = ROM_DATA_VALID_FLAG; 
   RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
   
   RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;
 }
/*******************************************************************************/
//
/*******************************************************************************/
void Validate_EEPROM_Data()
{
   u8 invalid_flag = 0;
   // check whether "addr" "board" "RS485" data in EEPROM is valid   
   if(RS485._flash.baudrate > 4)
   { RS485._flash.baudrate = 0; invalid_flag = 0xff;}
   
   // There is an issue that first 2 bytes (addr/board) written to EEPROM are liable to be alternated. 
   // As a workaround, we save 2 copies in other locations. If there are 2 copies equal, the other one has to be 
   // equal to them. 
   if((RS485._flash.addr_copy1 == RS485._flash.addr_copy2) && (RS485._flash.addr != RS485._flash.addr_copy1))
   {  RS485._flash.addr = RS485._flash.addr_copy1; invalid_flag = 0xff;}
   if((RS485._flash.addr_copy1 == RS485._flash.addr) && (RS485._flash.addr_copy2 != RS485._flash.addr_copy1))
   {   RS485._flash.addr_copy2 = RS485._flash.addr_copy1; invalid_flag = 0xff;}
   if((RS485._flash.addr_copy2 == RS485._flash.addr) && (RS485._flash.addr_copy1 != RS485._flash.addr_copy2))
   {   RS485._flash.addr_copy1 = RS485._flash.addr_copy2; invalid_flag = 0xff;}
   
   if((RS485._flash.board_copy1 == RS485._flash.board_copy2) && (RS485._flash.board != RS485._flash.board_copy1))
   {   RS485._flash.board = RS485._flash.board_copy1;  invalid_flag = 0xff; }
   if((RS485._flash.board_copy1 == RS485._flash.board) && (RS485._flash.board_copy2 != RS485._flash.board_copy1))
   {   RS485._flash.board_copy2 = RS485._flash.board_copy1;  invalid_flag = 0xff;}
   if((RS485._flash.board_copy2 == RS485._flash.board) && (RS485._flash.board_copy1 != RS485._flash.board_copy2))
   {   RS485._flash.board_copy1 = RS485._flash.board_copy2;  invalid_flag = 0xff;}
                
    if(invalid_flag) 
    {  RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
       RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;
    } 

}
/*******************************************************************************/
//                    Initialize CS5530
// CS5530 is not 100% code compatible with CS5532 in configuration.
// It has no setup registers. Gain of fixed to X64. 
// Bit definition in configuration register is different from that of CS5532.
/*******************************************************************************/
#ifdef _50HZ_FILTER_
#define AD_CONFIG_UPPER_16BITS 0x8
#else
#define AD_CONFIG_UPPER_16BITS 0x0
#endif 
#define RESET_VALID 0x10000000

void init_cs5532(void)
{            
   u8 i; 
   u16 temp;
   u8 offset_data[4];
       
   // To begin, a 4.9152 MHz or 4.096 MHz crystal takes approximately 20 ms 
   // to start. To accommodate for this, it is recommended that a software delay 
   // of approximately 20 ms be inserted before the start of the processor¡¯s ADC 
   // initialization code. Actually this is not necessary in this system because
   // MEGA8 is configured to stay in reset state for 64ms (fuse bit) after power up.
   // sleepms(20);  
   //Initialize CS5532. Try 3 times max. 

   for(i=3;i>0;i--)
      if(CS5532_Init() == RESET_VALID)
      {  //Bit[3:0] are word rate.         
         temp = RS485._flash.cs_gain_wordrate & 0xF;
         // There was once an issue that CS5530 conversion data is always zero.
         // Root cause was figured out as "RS485._flash.cs_gain_wordrate" was not 
         // properly initialized. Data in EEPROM is 0xFF after erase. Before parameters 
         // are programmed into EEPROM again, "RS485._flash.cs_gain_wordrate" is 0xFF.
         // Therefore WR[3:0] of CS5530's confirgration register is set to "1111", which 
         // is an invalid setting. So here a check is added. if setting is invalid, word rate
         // setting will be forced to 3 (15sps/12.5sps). 
         if(((temp>4)&&(temp<8))||(temp>12))
            temp = 3;
         temp = (temp << 11) | 0x400;
         CS5532_Config(AD_CONFIG_UPPER_16BITS,temp);

         //CS5532_SysOffsetCal();
         // mannually set CS5530 offset/gain register.
         // offset positive: MSB = 0, gain
         offset_data[0] = 0x08;   //18
         offset_data[1] = 0x00;
         offset_data[2] = 0x00;
         offset_data[3] = 0x00;         
         // gain = 2^3 = 8
         CS5532_Manual_Offset_Gain_Cal(offset_data, 2);

         //CS5530 has no setup registers.                                
         //Start CS5532 A/D conversion    
         CS5532_Cont_Conversion();
         //Important note: from now on, CS5532/CS5530 won't accept commands
         //you need to stop cont converison before issue command to CS5532.         
         break;
      }
      
     if(i) // initialized successfully,
     {   RS485._global.cs_status = 0;
         LED_ON(PIN_HWOK);
     }
     else
        RS485._global.cs_status |= 0x4 ; // flag bit2 
     
}
/*****************************************************************************/
//
//*****************************************************************************/
#ifdef _TEST_MOTOR_MAGNET_  
    void Test_Motor_Magnet_Loop(); 
#endif   

void query_fw_version()
{
   RS485._global.cs_sys_gain_cal_data = FIRMWARE_REV;
}

/*****************************************************************************/
//                      Main function entry here
/*****************************************************************************/
void main(void)
{           
   u8 flag_turn_off_watchdog; 
   u8 i;
   u8 Convtemp[4]; 
   /********************************************************************/
   //               System Initialization
   /********************************************************************/          
   // Initialize MEGA8 PORT
   Init_PORT();        
   // Timer (Interrupt) initialization
   Init_Timers();             
   // Initialize EEPROM
   vInitEeprom();        
   // Initialize System global variables (read from EEPROM)
   bReadDataFromEeprom(EEPROM_RS485ADDR,(u8*)&RS485._flash, sizeof(S_FLASH));
   // Initialize Global Variable: 
   // "Init_Vars()" has to be placed after "bReadDataFromEeprom()" because
   // RS485._flash.cs_zero is involved. 
   Init_Vars();
   query_fw_version();   /* the LSB byte of variable "cs_sys_gain_cal_data" */
      
   // Write default system parameters into EEPROM if EEPROM data is invalid.
#ifdef _FORCE_INIT_EEPROM_  
   RS485._flash.rom_para_valid = ~ROM_DATA_VALID_FLAG;
#endif
   // Raw EEPROM
   if(RS485._flash.rom_para_valid != ROM_DATA_VALID_FLAG)  
       Init_EEPROM_Para(); 
   else
       Validate_EEPROM_Data(); 

   // Initialize UART, 8 data, 1 stop, even parity. Boardrate is set based on
   // EEPROM parameters.                   
   UART_Init(); 
   // Initialize SPI to communicate with CS5532/CS5530
   SPI_MasterInit();                           
   init_cs5532(); 

   // initialize golbal interrupt.
   #asm("sei")                        

   /*************************************************************************/ 
   // Raw board check
   /*************************************************************************/       
   //if RS485 addr is invalid(new board), Use default addr: 0x30
   if((RS485._flash.addr == 0) || (RS485._flash.addr > MAX_RS485_ADDR))
        RS485._flash.addr = MAX_RS485_ADDR;        
   myaddr = RS485._flash.addr;         

   /*************************************************************************/ 
   // Test motor and magnet
   /*************************************************************************/

#ifdef _DISP_AD_OUT_ 

   while(1)
   {
      Convtemp[0]=0;
      Convtemp[1]=0;
      Convtemp[2]=0;
      Convtemp[3]=0;
      putchar(RS485._flash.addr);
      sleepms(UART_DELAY);
      putchar(RS485._flash.board);
      sleepms(UART_DELAY); 
      CS5532_ReadADC(Convtemp);   
     // CS5532_PoiseWeight();
      putchar('A');
      
      //if ((Convtemp[0] != 0) || (Convtemp[1] != 0))
      {  
         sleepms(UART_DELAY);
         putchar(Convtemp[0]);   
         sleepms(UART_DELAY);
         putchar(Convtemp[1]);   
         sleepms(UART_DELAY);
         putchar(Convtemp[2]);   
         sleepms(UART_DELAY);
         putchar(Convtemp[3]);
         sleepms(UART_DELAY);  
         LED_FLASH(PIN_HWOK);       
      }                  
   }
#endif
    
#ifdef _TEST_MOTOR_MAGNET_
   Test_Motor_Magnet_Loop();
#endif

#ifdef _TEST_MAGNET_
  sleepms(5000);
   LED_OFF(PIN_RxOK);
   LED_OFF(PIN_TxOK);
   LED_OFF(PIN_HWOK);
  while(1)
  {
   while(Magnet_Is_Running())
   {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
          break; 
   }
   //LED_ON(PIN_RxOK);
   RS485._flash.magnet_time = 20;
   RS485._flash.magnet_amp = 45;
   E_Magnet_Driver(RS485._flash.magnet_time); 
   while(Magnet_Is_Running())
   {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
          break; 
   }
   //LED_OFF(PIN_RxOK); 
   
   sleepms(1000);
   
   LED_ON(PIN_TxOK);
   RS485._flash.magnet_time = 20;
   RS485._flash.magnet_amp = 55;
   E_Magnet_Driver(RS485._flash.magnet_time); 
   while(Magnet_Is_Running())
   {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
          break; 
   }
   LED_OFF(PIN_TxOK);
   
   sleepms(1000);

   //LED_ON(PIN_HWOK);
   RS485._flash.magnet_time = 20;
   RS485._flash.magnet_amp = 65;
   E_Magnet_Driver(RS485._flash.magnet_time); 
   while(Magnet_Is_Running())
   {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
          break; 
   }
   //LED_OFF(PIN_HWOK);
   
   sleepms(1000);
      
  }
#endif
   /*************************************************************************/ 
   //                          Task State Machine
   /*************************************************************************/          
   while(1) 
   {                       
       /************************************************************/
       
       MYDEAD(0x400);  // Run
       /************************************************************/
       // Feed watch dog to avoid reset.
       // Timeout limit is set to 1 sec
       /************************************************************/                                            
        Feed_Watchdog();                         
       /************************************************************/
       // Reset node on reset command from master board.
       // flag_reset[7:4] --- reset type
       //   #define UPGRADE_FIRMWARE 0xA
       //   #define NORMAL_BOOT      0x0 Default data after powerup
       // flag_reset[2:0] --- baud rate
       //   0 - 115200bps   1 - 57600bps  2 - 38400bps 
       //   3 - 19200bps    4 - 9600bps   
       /************************************************************/
       if(RS485._global.flag_reset >0 )
       { 
          boot_vector.baud_rate = RS485._global.flag_reset & 0x7;
          boot_vector.boot_cmd  = RS485._global.flag_reset >> 4;
          // housekeeping (motor/magnet) before reset
          // we should make sure magnet/motor action completes.
                   
          #asm("cli")
          #asm("wdr") // reset watchdog timer to avoid timeout reset
          WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
          WDTCR = 0x09; // turn on watch dog timer: WDE = 1, WDP = 001, 32ms.
          #asm("sei") // Enable global interrupt.
          RS485._global.flag_reset = 0;          
          /*Trapped for Watch-Dog Reset*/
          while(1);
       }
       /************************************************************/
       // Runtime Diagnose on addr, board and baudrate
       /************************************************************/       
        Validate_EEPROM_Data(); 
        myaddr = RS485._flash.addr;         
       /************************************************************/
       // program all configuration settings into EEPROM  
       // program will not go down unless program job done.
       // Be aware of the "continue" statement below.           
       /************************************************************/
       if(RS485._global.NumOfDataToBePgmed > 0){  
          if(RS485._global.NumOfDataToBePgmed > sizeof(S_FLASH))
                RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
          RS485._global.NumOfDataToBePgmed = bWriteData2Eeprom(EEPROM_RS485ADDR + sizeof(S_FLASH) - RS485._global.NumOfDataToBePgmed,
                                                               (u8*)&RS485._flash + sizeof(S_FLASH) - RS485._global.NumOfDataToBePgmed, 
                                                               RS485._global.NumOfDataToBePgmed);        
          continue;
       }
       else
          RS485._global.test_mode_reg1 &= 0xF7;   /* disable EEPROM programming*/
       /************************************************************/
       // Test Mode(test_mode_reg1): when set,
       // Bit 7: force to use the first poise value.
       // Bit 6: don't use motor sensors. 
       // Bit 5: turn off Watchdog timer.
       // Bit 4: display AD output (cs_material) in PC.       
       //
       // Test Mode(test_mode_reg2): when set:
       // Bit[7:6] = 10, Bit[5:0]as new RS485 addr.
       // Bit[7:6] = 01, Bit[5:0]as new Board Type.
       // See "define.h" for details.                                 
       /************************************************************/       
      if((CHANGE_RS485_ADDR) && (RS485._global.test_mode_reg2>0) && (RS485._global.test_mode_reg2 < MAX_RS485_ADDR))
       {
            RS485._flash.addr = RS485._global.test_mode_reg2 & 0x3f;        
            RS485._flash.addr_copy1 = RS485._flash.addr;
            RS485._flash.addr_copy2 = RS485._flash.addr;            
            myaddr = RS485._flash.addr;
            RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
            RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;            
            RS485._global.test_mode_reg2=0;
            continue; 
       }
       if(CHANGE_BOARD_TYPE)
       {
         if((RS485._global.test_mode_reg2 == BOARD_TYPE_WEIGHT) || (RS485._global.test_mode_reg2 == BOARD_TYPE_VIBRATE) || 
            (RS485._global.test_mode_reg2 == BOARD_TYPE_MOTOR))
         {  RS485._flash.board = RS485._global.test_mode_reg2; 
            RS485._flash.board_copy1 = RS485._flash.board;
            RS485._flash.board_copy2 = RS485._flash.board;       
            RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
            RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;  
            RS485._global.test_mode_reg2 = 0;  
         }
         continue;
       }  
       /*
       if(TURN_OFF_WATCHDOG)
       {
           flag_turn_off_watchdog = 1;        
           continue;
       } //*/  // some boards flash LED_D6, bug here???                                    
       /************************************************************/
       // Magnet/Motor actions. 
       // State range ENABLE_OFF< WSM_Flag <ENABLE_ON: mannual operation. 
       // Turn off watchdog in manual operation.
       /************************************************************/       
       if((RS485._global.flag_enable > ENABLE_OFF)&&(RS485._global.flag_enable < ENABLE_ON))
       {  
          Turn_Off_Watchdog();
          //if there is ongoing motor_magnet action, allow it to complete this cycle.
          while(WSM_Flag != MM_ADD_MATERIAL)
     	     motor_magnet_action();                
          RS485._global.Mtrl_Weight_gram = INVALID_DATA;
          //if there is release request pending, release material. 
          while(RS485._global.flag_release)	  
             release_material();                    
          // flag_enable register represents different command 
          // need to take care of watch dog.
          switch(RS485._global.flag_enable)
          {             
             case ENABLE_START_MACHINE:
                RS485._global.flag_enable = ENABLE_ON;
                break;
             case ENABLE_INIT_AD:  
                init_cs5532();
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             case ENABLE_TURNS:      //upper bucket/motor                          
                while(Motor_Is_Running());                
                Motor_Driver('S',MOTOR_SHIFT_CYCLE_OPEN,KEEP_POWER_ON);                         
                while(Motor_Is_Running()) ;
                sleepms(RS485._flash.open_s);
                if(ENABLE_MOTOR_SENSORS)               		                                        
                  Motor_Driver('S',HALF_CYCLE_CLOSE_WITH_SENSOR,POWER_OFF_AFTER_ROTATION); 
                else
                  Motor_Driver('S',HALF_CYCLE_CLOSE_NO_SENSOR,POWER_OFF_AFTER_ROTATION); 
                while(Motor_Is_Running()) ;	                                       
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             case ENABLE_TURNW:
                RS485._global.flag_release = 1;
                while(RS485._global.flag_release == 1)
                    release_material();             
                RS485._global.flag_enable = ENABLE_OFF;                                             
                break;
             case ENABLE_VIBRATE:
                while(Magnet_Is_Running())
                {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
                       break; 
                }
                E_Magnet_Driver(RS485._flash.magnet_time); 
                while(Magnet_Is_Running())
                {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
                       break; 
                }		                                        
                RS485._global.flag_enable = ENABLE_OFF; 
                break;
             case ENABLE_RESET_WEIGHT:
                for(i=20;i>0;i--){
                   if(reset_weight() == 0)
                      break;
                }
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             default:
                break;                                                      
          }
          //Feed_Watchdog(); // turn on WDT again.
          continue;
       }	                                        
       /************************************************************/
       // shut down machine. if weight state machine state is not 
       // MM_ADD_MATERIAL, let machine to finish this cycle before
       // shutting it down.
       // When machine(motor/magnet action is shut down, weight is 
       // repeatedly checked.
       /************************************************************/       
       if(RS485._global.flag_enable == ENABLE_OFF){        
          RS485._global.flag_goon = 0; //clear goon symbol                
          while(WSM_Flag != MM_ADD_MATERIAL)
          {
             Feed_Watchdog();
             motor_magnet_action();    //reset the motor status
          }
          CS5532_PoiseWeight();        
          CS5532_Poise2Result();           
          RS485._global.flag_goon = 0; //clear goon symbol                                                                     
          continue;      
       }                                                             
       /************************************************************/
       // If you can reach here, let's say machine is running in 
       // production automatic mode. Condition:flag_enable=ENABLE_ON.
       // Check release command or other actions from 
       // flag will be cleared in release_material() subroutine.
       /************************************************************/       
       if(RS485._global.flag_release){ 
          //invalidate weight when doing material release actions.
          RS485._global.Mtrl_Weight_gram = INVALID_DATA; 
          release_material(); 	  
          // initialize pre_feed_weight: weight is zero after release.
          pre_feed_weight = 0; 
          //add material during idle/wait period of release state machine
          // to improve system speed. The 2 subrountines are state machines and 
          // they can run in parallel.
          magnet_add_material();  	  
          continue;
       }else{            
    	  // weight measurement is embedded at the end of motor_magnet_action().
    	  // if no go on command received, code won't break out of motor_magnet_action().
    	  // motor_magnet_action() will continuously output weight values.
    	  motor_magnet_action();
       	  continue;
       }              
   }
}
