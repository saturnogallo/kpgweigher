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
/*********************************************************************************
This file is MEGA16 SPI driver

Chip type           : ATmega16
Program type        : Application
Clock frequency     : 7.372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256

Code by             : fd_lxh@yahoo.com.cn
*********************************************************************************/

#include <mega16.h>
#include "define.h"   

/********************************************************************************/
//   PCB Board configuration:
/********************************************************************************/
sfrb DDR_SPI=0x17;                   /* SPI pins shared with PORT B[7:4] */
                                     /* 0x17 is the addr of DDRB */

/********************************************************************************/
//            MEAG16 SPI Master Initialization
// This subroutine initialize MEGA8 SPI as master.
// SPI interrupt disabled. MSB sent first.
/********************************************************************************/


void SPI_MasterInit(void)
{

/* Set MOSI SCK and SS to outputm Set MISO to input mode */ 
DDR_SPI.P_MOSI = 1;
DDR_SPI.P_SCK  = 1;
DDR_SPI.P_SS   = 1;
DDR_SPI.P_MISO = 0;

//SS# high to disable slave
//PORTB = PORTB | (1<<P_SS);  
SPI_SS = 1;

//clear SPIF flag by reading SPSR first and followed by SPDR later.
#asm
    in   r30,spsr
    in   r30,spdr
#endasm

//Set SPI to master mode, clock rate to fck/8
//SPCR: SPI Control Register
//Bit 7 - SPIE : "1" to enable SPI interrupt.
//Bit 6 - SPE  : "1" to enable.
//Bit 5 - DORD : "1" stands for LSB first to send
//Bit 4 - MSTR : "1" to set SPI as master
//Bit 3 - CPOL : When set to "1", SCK keep high in idle state
//Bit 2 - CPHA : when set to "1", setting data at the rising edge of clock, while sample data at clock falling edge.
//Bit 1,0 - SPR1,SPR0 :  SRR1, SPR0 and SPI2X together define clock freq of SPI master.
//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
SPSR = 0x1;
SPCR = 0b01010001;    
}

/********************************************************************************/
//             MEAG8 SPI Write/Read subroutine
// Function: char SPI_MasterTransmit(char cData)
// when used as writing fuction, cData is the data to be write to 
// SPI slave. When used as read function, set cData equal to 0x0, 
// this subroutine returns data sent from SPI slave.
/********************************************************************************/
void SPI_MasterTransmit_c(u8 cData)
{                            
        u8 dat;
        /* Set SS# pin to low */
        SPI_SS = 0;   
        /* start data transition*/
        SPDR = cData;
        /* waiting for data transition end */
        while(!(SPSR & (1<<SPIF)));
        /* return data and clear SPIF */
        dat = (SPDR); //just discard it
}

/* send a word data MSB first*/
void SPI_MasterTransmit_w(u16 cData)
{
	SPI_MasterTransmit_c((cData>>8) & 0xFF);
	SPI_MasterTransmit_c(cData & 0xFF);	
} 

/* send a long data MSB first*/
void SPI_MasterTransmit_l(u32 cData)
{
	SPI_MasterTransmit_c((cData>>24) & 0xFF);
	SPI_MasterTransmit_c((cData>>16) & 0xFF);
	SPI_MasterTransmit_c((cData>>8) & 0xFF);
	SPI_MasterTransmit_c(cData & 0xFF);	
}

/********************************************************************************/
//             MEAG8 SPI Write/Read subroutine
// Function: char SPI_MasterTransmit(char cData)
// when used as writing fuction, cData is the data to be write to 
// SPI slave. When used as read function, set cData equal to 0x0, 
// this subroutine returns data sent from SPI slave.
/********************************************************************************/
/* receive a char data */
u8 SPI_MasterReceive_c()
{
        /* Set SS# pin to low */
        SPI_SS = 0;   
        /* start data transition*/
        SPDR = 0;
        /* waiting for data transition end */
        while(!(SPSR & (1<<SPIF)));
        /* return data and clear SPIF */
        return(SPDR);
}

/* recieve a long data MSR first*/
u32 SPI_MasterReceive_l()
{
        u32 ret;
	ret = SPI_MasterReceive_c();
	ret = (ret<<8) + SPI_MasterReceive_c();
	ret = (ret<<8) + SPI_MasterReceive_c();
	ret = (ret<<8) + SPI_MasterReceive_c();
	return ret;
}
#include <MEGA16.h>
#include "define.h" 

//delay after close signal is detected
#define SHIFT_PULSE_INCNUM            2
#define SHIFT_PULSE_DECNUM            2              

/*****************************************************************************/
//                       Hardware Related definition
/*****************************************************************************/
#define _HW_REV_3_          /* hardware changes */

#define MOTORS_CLOSED   PIND.3 == 1
#define MOTORW_CLOSED   PIND.2 == 1      
                                                          
#ifdef _HW_REV_2_
#define MAGNET_PULSE_PORT	PORTB.1
#define ENABLE_MOTOR_W          PORTC.0 = 1
#define DISABLE_MOTOR_W		PORTC.0 = 0
#define ENABLE_MOTOR_S		PORTC.1 = 1
#define DISABLE_MOTOR_S		PORTC.1 = 0
#define OUTPUT_ENABLE           PORTC.2 = 1
#define OUTPUT_DISABLE          PORTC.2 = 0
#define MOTOR_CLOCK_PIN         PORTC.4   

#define TRIG_TRIAC()    PORTB.1 = 1 
#define TRIG_CMPLT()    PORTB.1 = 0
#endif

#ifdef _HW_REV_3_
#define MAGNET_PULSE_PORT	PORTB.1
#define ENABLE_MOTOR_W          PORTA.0 = 0         /* optocoupler, inverted logic */
#define DISABLE_MOTOR_W		PORTA.0 = 1
#define ENABLE_MOTOR_S		PORTA.1 = 0
#define DISABLE_MOTOR_S		PORTA.1 = 1
#define MT_MODE_BIT0            PORTA.2
#define MT_MODE_BIT1            PORTA.3 
#define MOTOR_CLOCK_PIN         PORTA.4
#define MOTOR_ROTATE_CLKWISE    PORTA.5 = 1
#define MOTOR_ROTATE_ANTICLKWISE PORTA.5 = 0 

#define TRIG_TRIAC()    PORTB.1 = 0 
#define TRIG_CMPLT()    PORTB.1 = 1
#endif

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
    if(RS485._flash.board == BOARD_TYPE_VIBRATE)
       temp =  (u16)RS485._flash.magnet_amp * 6 + 64166; 
    else
       temp =  (u16)RS485._flash.magnet_amp * 3 + 64166; 
    /* IMPORTANT NOTES: Because TCNT1 is a 16-bit register, writing to TCNT1 will be broken into 2 bytes.
       To */
    //TCNT1 = COUNTER_BASE + temp<<3 ;
    TCNT1 = temp;
    //TCNT1 = 65535 - 6 * (MAX_TRIAC_ANGEL - RS485._flash.magnet_amp); 
    // Enable Timer1 overflow interrupt.  
    TIMSK |= 0x4;
    // flag to indicate timer is set by Analog comparator interruption.
    Timer1_flag = SET_BY_ACINT;  
  }                           
  LED_FLASH(PIN_HWOK);
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
   LED_FLASH(PIN_RxOK);
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
#define MAX_AMP 100
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
#ifdef _HW_REV_3_
   ACSR=0x1B;                    /* both AIN+ and AIN- are driven from external */
#else
   ACSR=0x5B;                    /* AIN+ is driven by internal voltage base */ 
#endif


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
#ifdef _HW_REV_3_
   MOTOR_ROTATE_CLKWISE;        /* motor rotates clockwise */
   MT_MODE_BIT0 = 1;            /* motor mode is 00 (4 phases) */
   MT_MODE_BIT1 = 1; 
#endif   
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
#include <mega16.h> 
#include "uart.h" 
#include "define.h" 
#include "cs5532.h"
#include "motor.h"
#include "drveeprom.h" 
#include "command.h"                                        

/****************************************************************************/
//                 UART Receiver/ Transmiter Variable Definition
/****************************************************************************/
// USART Transmitter buffer
u8 volatile tx_buffer[TX_BUFFER_SIZE];

// define index for transmitter
u8 volatile tx_wr_index;
u8 volatile tx_rd_index;
u8 volatile tx_counter;

//#ifndef _OLD_FASHION_CMD_PROCESS_
//u8 volatile rx_buffer[RX_BUFFER_SIZE];
// define index for Receiver
u8 volatile rx_wr_index;
u8 volatile rx_rd_index;
u8 volatile rx_counter;
//#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow; 

extern u8 debug_mode;
     
/****************************************************************************/
//               UART Receiver Interrupt service routine    
// UART communication frame structure:
// Head1(0xfe) Head2(0x68), addr_from, addr_to, cmd, data_len
// data_to_send_or_receive, check_sum. 
// in normal mode, once a byte is received, function cm_pushc() will be called
// to check if a frame is received completed or not. cm_pushc() will analysize 
// the frame and respond to the frame.
/****************************************************************************/
//#define HW_STATUS_UART_ERROR 0b00010000
interrupt [USART_RXC] void usart_rx_isr(void)
{
   u8 status,data;
   
   //read status and data
   status=UCSRA;
   data=UDR;    
   // check if error happened.
   if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {   
//#ifdef _OLD_FASHION_CMD_PROCESS_
       cm_pushc(data,0);
       //hw_status &=0xEF;
//#else       
      // rx_buffer[rx_wr_index++]= data;
      // if(rx_wr_index >= RX_BUFFER_SIZE)
      //    rx_wr_index=0;
      // rx_counter++;
//#endif    
   }      
   //else
      //hw_status |= HW_STATUS_UART_ERROR;      
}

/****************************************************************************/
//               UART Transmitter Interrupt service routine
/****************************************************************************/
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
      PORTB.0 = 1;         // set RS485 node to transmiter mode
      --tx_counter;
      UDR=tx_buffer[tx_rd_index];
      if (++tx_rd_index == TX_BUFFER_SIZE) 
         tx_rd_index=0;
   }
else 
   PORTB.0 = 0;         // set RS485 node to receiver mode.
}

/****************************************************************************/
//                     UART Set Baud rate.  
// This subroutine sets UART baud rate based on user settings
// Users are allowed to change baudrate of RS485 nodes. 
/****************************************************************************/
void SetBaudrate()
{   
   switch(RS485._flash.baudrate)
   {  case 0x0:                 // 115200bps
        UBRRH=0x00;
        UBRRL=0x03;
        break;
      case 0x1:                 // 57600bps
        UBRRH=0x00;
        UBRRL=0x07;                                 
        break;
      case 0x2:                 // 38400bps
        UBRRH=0x00;
        UBRRL=0x0B;
        break;
      case 0x3:                 // 19200bps
        UBRRH=0x00;
        UBRRL=0x17;
        break;
      case 0x4:                 // 9600bps
        UBRRH=0x00;
        UBRRL=0x2F;
        break;
      default:                  // 115200bps
        UBRRH=0x00;
        UBRRL=0x03;                         
        break;                          
   }     
}
/****************************************************************************/
//                     UART Initialization   
// need to enable global interrupt in main function.
// This module only set UART interrupt bits.
/****************************************************************************/
void UART_Init(void)
{ 
 // Set RS485 node to receiver mode
    PORTB.0 = 0;
 
 // Reset UART buffer and flag;
    rx_buffer_overflow = 0;
    tx_wr_index=0;
    tx_rd_index=0;
    tx_counter=0;
//#ifndef _OLD_FASHION_CMD_PROCESS_
    rx_wr_index=0;
    rx_rd_index=0;
    rx_counter=0;
//#endif    
    
 // USART initialization
 // Communication Parameters: 8 Data, 1 Stop, No Parity
 // USART Receiver: On
 // USART Transmitter: On
 // USART Mode: Asynchronous
    #asm("cli")
    UCSRA=0x00;
 
 // Enable TXC interrupt, RX interrupt, disable UDRE interrupt
    UCSRB=0xD8;
 
 // UCSRC share IO address with reg UBRR, MSB bit for UCSRC must be 1.
 // 8 data, 1 stop, even parity
    UCSRC=0xA6;    // EVEN parity bit
 // UCSRC=0x86;    // No parity bit
 
 // Set Baud rate based on System setting
 // RS485.BaudRate_Index can be read out from EEPROM
 // or set by RS485 master via RS485 communication
 // UCSRC share IO address with reg UBRR, MSB bit for UBRR must be 0.
 // don't enable interrupt here.
 // enable global interrupt in main function after all modules are initialized 
    //RS485._flash.baudrate = 0;
    SetBaudrate();
}                                                                             

/****************************************************************************/
//
//                 Higher Level UART print Subroutines
//
/****************************************************************************/
#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+                         
/****************************************************************************/
// Send a char-Type data via uart.
/****************************************************************************/
void putchar(char c)
{
   // need to be changed to avoid waiting
   while (tx_counter >= TX_BUFFER_SIZE);

   // set RS85-node to transmiter mode
   PORTB.0 = 1;
   
   #asm("cli")
   // if there is data in TX buffer or data being transmitted. 
   // then write data to buffer.
   if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
      {
      tx_buffer[tx_wr_index]=c;
      if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
      ++tx_counter;
      }
   // if no data in buffer and no data being transitted, 
   // write data to UDR to start transimision immediately
   else
      UDR=c; 
   #asm("sei")
}
/****************************************************************************/
// mputs(): UART prints a data array
/****************************************************************************/
void mputs(u8 *buf, u8 size, u8 port)
{
    while(size-- > 0)
      putchar(*buf++);    
} 

/****************************************************************************/
// Read a byte from UART buffer
/****************************************************************************/
/*#ifndef _OLD_FASHION_CMD_PROCESS_
u8 read_uart_db()
{
  u8 uart_rdata;
  uart_rdata = rx_buffer[rx_rd_index++];
  rx_rd_index %= RX_BUFFER_SIZE;
  rx_counter--;
  return uart_rdata;
}
u8 data_available_in_rxbuf()
{
  if (rx_counter > RX_BUFFER_SIZE)
  {  
     rx_rd_index = 0;
     #asm("cli")
     rx_wr_index = 0;
     rx_counter = 0;
     #asm("sei")
  }
  return rx_counter;
}        
#endif//*/

/****************************************************************************/
// d_putchar(): output a character in ASCII code mode.
// For example: 0x9b is converted to 2 characters '9'/'B' before output
/****************************************************************************/
/*void d_putchar(u8 a)
{
    unsigned char h,l;
    h = (a & 0xf0) >> 4;
    l = (a & 0x0f);
    if(h <= 9)		
       putchar(h+'0');
    else
        putchar(h+'A'-0x0a);

    if(l <= 9)
	putchar(l+'0');
    else
	putchar(l+'A'-0x0a);
} //*/ 

/****************************************************************************/
// d_mputs() convert an ASCII code array to HEX and print them via UART
/****************************************************************************/
/*void d_mputs(u8 *buf, u8 size, u8 port)
{   while(size-- > 0)
      d_putchar(*buf++);        
} //*/

/****************************************************************************/
// UART print a string
/****************************************************************************/ 
/*void putstr(flash u8 *ptr)
{
   while(*ptr != 0x00)
      putchar(*ptr++);   
} //*/

#pragma used-
#endif
       
#include "uart.h"
#include <mega16.h>
#include "command.h"

extern u8 myaddr;              //RS485 address of this node.

/****************************************************************************/
// Global buffer to save data received or to be sent out.
// UART Frame Format: 
// Head1(0xfe) Head2(0x68), addr_from, addr_to, cmd, data_len
// data_to_send_or_receive, check_sum.
// Initialize frame buffer.
// The sender had better send an additional byte "Head1" prior to frame
// in case that the first byte is not correctly sent out by UART.
// For example, 0xfe, 0xfe, 0x68, addr_from ....
/****************************************************************************/
CM_INFRAME  infrm[STATE_MACHINE] =  {
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0}
};

CM_OUTFRAME outfrm[STATE_MACHINE] = {
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0}
};
  
static u8 buf_cnt[STATE_MACHINE];   //STATE_MACHINE = 1, only 1 UART.
/****************************************************************************/
// Flag to indicate the process of receiving a frame. 
// Used by function "cm_pushc"
//#define RF_IDLE         0     // idle, no frame being received
//#define RF_HEAD1        1     // got frame head1 byte
//#define RF_HEAD2        2     // got frame head2 byte
//#define RF_ADDRFROM     3     // got addr_from byte
//#define RF_ADDRTO       4     // got addr_to byte
//#define RF_CMD          5     // got command byte
//#define RF_DATALEN      6     // got data length byte
//#define RF_DATABUF      7     // got data
//#define RF_CKSUM        8     // got check sum byte 
//Initialize status to "idle".   
/****************************************************************************/
u8 RFlag[STATE_MACHINE] = {RF_IDLE};

/****************************************************************************/
//                         Time Delay Subroutines
/****************************************************************************/
void sleepus(u16 us)
{               
        u8 j;
       	while(us-- > 0)
        	for (j=10; j>0; j--);
}

void sleepms(u16 ms)
{
  	while(ms-- > 0)
        	sleepus(100);	
}
 
/****************************************************************************/
//              Calculate check_sum data for a group of data
/****************************************************************************/                         
u8 checksum(u8 *buf, u8 size)
{
        u8 sum = 0;
        while(size-- > 0)
                sum = sum + *buf++;
                
        sum = (~sum)+1;
        return sum;
}   
        
/****************************************************************************/
//                         reply to a query 
// prints is an alias of function "m_puts"
//
/****************************************************************************/
void cm_ack(u8 port)
{                
   outfrm[port].cksum = checksum((u8*)(&outfrm[port]),6)+checksum( outfrm[port].databuf, outfrm[port].datalen);
   //print out the first 6 bytes of a frame, head1, ... cmd, data_len
   prints((u8*)(&outfrm[port]),6,port);
   //print out data if there is any to be sent out
   if(outfrm[port].datalen > 0)
        prints(outfrm[port].databuf, outfrm[port].datalen, port);
   //print out check_sum byte
   prints(&outfrm[port].cksum,1,port);
   // LED to indicate command ack.
   LED_FLASH(PIN_TxOK);
}
/****************************************************************************/
//                          Command Receiver
// receive byte by byte from UART and save data into buffer until a completed
// frame is received. 
// Call "cm_process()" to analysis frame information
/****************************************************************************/

void cm_pushc(u8 c,u8 port)
{
      if(RFlag[port] == RF_CKSUM)                       //wait until it is process
                return;
      if(RFlag[port] == RF_DATABUF)               {     //checksum found
	      infrm[port].cksum = c;
    	      if(((checksum((u8*)(&infrm[port]),6)+checksum(infrm[port].databuf,infrm[port].datalen)) & 0xff) == c)
    	      	{
                        RFlag[port] = RF_CKSUM; 
                        cm_process(port);
                        RFlag[port] = RF_IDLE;
                }
              else
              {  RFlag[port] = RF_IDLE;
              }
              return;                                                         
      }
      
      if(RFlag[port] == RF_DATALEN)               {     //databuf found
                infrm[port].databuf[buf_cnt[port]] = c;
                buf_cnt[port]++;
                if(buf_cnt[port] == infrm[port].datalen)
                        RFlag[port] = RF_DATABUF; 
                return;
      }
      if(RFlag[port] == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrm[port].datalen = c;
                        buf_cnt[port] = 0;
                        if(c == 0) 
                                RFlag[port] = RF_DATABUF;
                        else
                                RFlag[port] = RF_DATALEN;
                }
                else
                {
                 RFlag[port] = RF_IDLE;      
                }
                return;                                
      }
      if(RFlag[port] == RF_ADDRTO)                 {     //cmd found
                infrm[port].cmd = c;
                RFlag[port] = RF_CMD; 
                return;
      }      
      if(RFlag[port] == RF_ADDRFROM)                 {     //addr_to found
                infrm[port].addr_to = c;
                RFlag[port] = RF_ADDRTO; 
                return;
      }

      if(RFlag[port] == RF_HEAD2)                 {     //addr_from found
                infrm[port].addr_from = c;
                RFlag[port] = RF_ADDRFROM;
                return;
      }
           
      if((RFlag[port] == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlag[port] = RF_HEAD2;  
                return;
      }
      if((RFlag[port] == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlag[port] = RF_HEAD1;
                return;
      }
      if((RFlag[port] == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlag[port] = RF_HEAD1;
                return;
      }                
      RFlag[port] = RF_IDLE;
}

/****************************************************************************/
//    Analysis a incoming frame/command after it is completed received
/****************************************************************************/
void cm_process(u8 port)   
{       
   if((infrm[port].addr_to == myaddr) || (infrm[port].addr_to == BROADCAST_ADDR))
   {    parse_node_frm((u8*)&RS485,port); //modify the content 
        LED_FLASH(PIN_RxOK);
   }                    
}

/****************************************************************************/
//              Analysis and process incoming frame  
// Frame format: header1, header2, addr_from, addr_to, cmd, data_len, 
// dat_buf[MAX], check_sum.  MAX = 16 here.
// infrm is the frame received. outfrm is the frame to be sent out
// infrm/output.databuf[] format: {reg_id,data, reg_id, data ...}
/****************************************************************************/
void parse_node_frm(u8* ptr, u8 port)
{
        u8 j;                                            
        /*if(infrm[port].cmd == 'C')
        {   // configure, save data to EEPROM
            // I add this command so that I can easily configure my raw boards.
            // infrm.databuf format: rom_start_addr,byte1, byte2, byte3, byte4.... 
            // add 0x80 to distiguish from 'F' command
            RS485._global.NumOfDataToBePgmed = infrm[port].datalen - 1 + 0x80;           
            port_eeprom = port;
        }//*/
        if(infrm[port].cmd == 'F'){   //write to flash
                RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
        }        
        if(infrm[port].cmd == 'W'){   //write bytes to environment
                j = 0;
                while(j < infrm[port].datalen){  
                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                        j = j+2;
                }                              
        }             
        if(infrm[port].cmd == 'X'){   //write words to environment
                j = 0;
                while(j < infrm[port].datalen){
                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                        *(ptr+1+infrm[port].databuf[j]) = infrm[port].databuf[j+2];
                        j = j+3;
                }                              
        }             
        
        if(infrm[port].cmd == 'R'){   //read bytes and feedback to caller
                outfrm[port].addr_from = infrm[port].addr_to;
                outfrm[port].addr_to = infrm[port].addr_from;                
                outfrm[port].cmd = 'W';
                outfrm[port].datalen = infrm[port].datalen << 1;
                j = 0;                       
                while(j < infrm[port].datalen){
                        outfrm[port].databuf[2*j] = infrm[port].databuf[j];
                        outfrm[port].databuf[2*j+1] = *(ptr+infrm[port].databuf[j]);
                        j++;
                }
                cm_ack(port);  
        }
       if(infrm[port].cmd == 'S'){   //read words and feedback to caller           
                outfrm[port].addr_from = infrm[port].addr_to;
                outfrm[port].addr_to = infrm[port].addr_from;                
                outfrm[port].cmd = 'X';
                outfrm[port].datalen = infrm[port].datalen * 3;
                j = 0;                       
                while(j < infrm[port].datalen){
                        outfrm[port].databuf[3*j] = infrm[port].databuf[j];
                        outfrm[port].databuf[3*j+1] = *(ptr+infrm[port].databuf[j]);
                        outfrm[port].databuf[3*j+2] = *(ptr+1+infrm[port].databuf[j]);                        
                        j++;
                }
                cm_ack(port);  
        }        
}                                                                             

/******************************************************************/
// CS5532 driver           
#include "define.h"
#include "spi.h"
#include "cs5532.h"
#include <MEGA16.h> 
#include "uart.h"
#include "drveeprom.h" 
/******************************************************************/

/******************************************************************/
//                      CS5532 Reset      
// Set RS bit in configuration reg to "1" to trigger reset, then 
// change it back to "0" 
// Return 0x10 if reset is successful. Return 0x0 if reset
// failed.
/******************************************************************/ 
u32 CS5532_Reset(void)
{ 
  // Set RS in configuration register to "1" to reset CS5532. 
  // First to write configuration register command
  SPI_MasterTransmit_c(CMD_WRITE_CFG_REG);     
  
  // write data to configuration register, 32bits (4 bytes)
  SPI_MasterTransmit_l(0x20000000); 
  
  sleepms(1);
  
  //change RS in configuration register back to "0"
  // First to write configuration register command
  SPI_MasterTransmit_c(CMD_WRITE_CFG_REG);
  // write data to configuration register, 32bits (4 bytes)
  SPI_MasterTransmit_l(0x00000000);  // Set RS to 0
  
  //check if reset is successful(RV "1") or not.
  //read configuration register, send command first
  SPI_MasterTransmit_c(CMD_READ_CFG_REG);
  // read 4-bytes from configuration register. MSB first.
  return SPI_MasterReceive_l();
}                        

/******************************************************************/
//                      CS5532 Initilization      
// At least 15 SYNC1 followed by SYNC0
// Call subroutine CS5532_Reset() to reset CS5532
/******************************************************************/
u32 CS5532_Init(void)
{
  u8 i;
  for (i=18; i>0; i--)
        SPI_MasterTransmit_c(SYNC1);
  SPI_MasterTransmit_c(SYNC0);
  // reset CS5532
  return CS5532_Reset();
}   

/******************************************************************/
//                      CS5532 Configuration      
// Configuration register is 32-bit long and only D31~D19 are used
// RS(D29): 0, normal operation; 1, reset cycle
// RV(D28): 0, normal operation; 1, reset valid
// IS(D27): 0, normal input; 1, input pairs are short internally
// OGS(D20): 0, cal Regs are used based on CS1-CS0; 1, OG1-OG0
// Filter Rate Select(D19): 0, 60Hz filter; 1, 50Hz filter
// Keep all other bits as default "0"
// Argument examples:
// cfgh: 0x20  --- Reset
// cfgl: 0x8   --- 50 HZ filter
// cfgl: 0x0   --- 60 HZ filter
// cfgh: 0x8,  --- Input short
// For CS5530, Bit D14~D11 are word rate config bits. These bits are
// reserved bits and should be set to "0" if AD is CS5532.
// D10 is the unipolar or bipolar input option bit, only for CS5530
// should be clear to zero if CS5532 is used.
/******************************************************************/
void CS5532_Config(u16 cfg_hw, u16 cfg_lw)
{ 
  // First to write configuration register command
  SPI_MasterTransmit_c(CMD_WRITE_CFG_REG);
  SPI_MasterTransmit_w(cfg_hw); 
  SPI_MasterTransmit_w(cfg_lw);
}

/******************************************************************/
//              CS5532/CS5530 channel setup 
// CS5532 totally has 8 16-bit setup registers
// Decoder: 
// CS[1:0]/Bit[15:14], forced to 00 to select channel 1
// CG[2:0]/Bit[13:11], Gain x1(set to 0)/x2/x4/x8/x16/x32/x64(set to 6)
// WR[3:0]/Bit[10:7], 0000/100sps 0001/50sps 0010/25sps 0011/12.5sps
// 0100/6.25sps 1000/3200sps 1001/1600sps 1010/800sps 1011/400sps
// 1110/200sps
// other bits (Bit[6:0] set to 0x40, see datasheet for details.
// Function introduction:
// This function sets setup register 1 based on word rate and gain settings
// input by user. 
// valid word rate range is "0~4" and "8~12"
// valid gain setting is "0~6". See datasheet for details
// return 0x0 if setup is successful. return 0xff if setup failed. 
// Note: each CSR are 32 bits long and contains 2 setups. 2 setups
// need to be accessed in pairs, even only 1 setup is needed.
/******************************************************************/
/*
u8 CS5532_Setup_CH1(u8 word_rate, u8 gain)
{
   u8 cFlag;
   u8 setup_data[4],i;   

   if(word_rate>12)
        return(0xff);                   //invalid word rate settings
   if((word_rate>4) && (word_rate<8))                               
        return(0xff);                   //invalid word rate settings
   if (gain>6)                          // invalid gain settings
        return (0xff);
   
   setup_data[0] = (gain<<3) + (word_rate/2);
   setup_data[1] = 0x40 + ((word_rate%2)<<7);
   //setup registers needed to be be accessed x32 bits together
   setup_data[2] = setup_data[0];  
   setup_data[3] = setup_data[1];   
   
   // Initalize setup success flag. 0 represents successful setup.
   cFlag = 0x0;           
 
   //write settings into CS5530/CS5532 setup register 1
   SPI_MasterTransmit_c(CMD_WRITE_SETUP_REG1);
   for(i=0;i<4;i++)
        SPI_MasterTransmit_c(setup_data[i]);
 
   //read out all setup registers to check if setup is successful or not.
   SPI_MasterTransmit_c(CMD_READ_SETUP_REG1);
   for(i=0;i<4;i++)
   { //set error flag if data read out doesn't match previous settings
     if(SPI_MasterReceive_c() != setup_data[i])
         cFlag = 0xFF;
   } 
   return(cFlag);      
}
//*/
 
/******************************************************************/
//            CS5532 continous conversion
// Initialize CS5532 to continous conversion mode. 
// SDO of CS5532 falls to indicate completion of a conversion cycle.
// To make code work well on CS5530 which only have setup1, we force
// setup setting to SETUP1.
/******************************************************************/
void CS5532_Cont_Conversion(void)
{ 
  SPI_MasterTransmit_c(CMD_CONTINUE_CONV_SETUP1);  
}

/******************************************************************/
//            CS5532 single conversion
// Initialize CS5532 to single conversion mode. 
// SDO of CS5532 falls to indicate completion of a conversion cycle.
/******************************************************************
void CS5532_Single_Conversion(void)
{
  SPI_MasterTransmit_c(CMD_SINGLE_CONV_SETUP1); 
}        

/******************************************************************/
//            CS5532 conversion result readout
// ADC conversion result readout
// SDO of CS5532 falls to indicate completion of a conversion cycle
// read conversion result when is low.
// return SUCCESSFUL(0x0) when data is read. return TIMEOUTERR(0xff)
// if timeout error occurs. 
// Conversion result is returned via pointer parameter.
/******************************************************************/
u8 CS5532_ReadADC(u8 *ConvDataBuf)
{ 
  //long int timeout = CS5532_READ_DATA_TIMEOUT;
  unsigned char i;
  unsigned char *pConvDataBuf;
  pConvDataBuf = ConvDataBuf; 
   
  // return busy flag: data not available
  if(PORTB.P_MISO == 1)
      return(0xff);
  
  // first 8 clock to clear SDO flag;
   SPI_MasterTransmit_c(NULL);

  // read conversion result
  for(i=4;i>0;i--)
    *pConvDataBuf++ = SPI_MasterReceive_c();
    
  return(0x0);
}

/******************************************************************/
//            CS5532 Manual Offset Calibration
// mannually write a data to offset register to do offset cal. 
// This function calculates data to be written into offset reg based
// on input arg (CS5532 output) so that CS5532 output can be initialized
// to zero. 
// Offset data ~= CS5532_Output * (1/4 + 1/64 + 1/128)
// 1 LSB represents 1.83007 of (-24) order of 2. 
// 1/(1.8300*2) ~ = 1/4 + 1/64 + 1/128
/******************************************************************/
void CS5532_Manual_Offset_Gain_Cal(u8 *AD_Output, u8 gain)
{
  u8 *ptr;
  u32 offset, temp1;   // temp2, temp3;
  
  //convert array data(CS5532 output) into a u32 type data
  ptr = AD_Output;
  offset = 0;
  temp1 = *ptr++;
  offset = temp1 << 24;
  temp1 = *ptr++;
  offset += temp1 << 16; 
  temp1 = *ptr++;
  offset += temp1 << 8; 
  offset += *ptr;
  
  //calculate data to be written into offset register
  //temp1 = offset >> 2; 
  //temp2 = offset >> 6;
  //temp3 = offset >> 7;       
  //offset = temp1 + temp2 + temp3;

  //write offset data to offset register
  SPI_MasterTransmit_c(CMD_WRITE_OFFSET_REG1);
  SPI_MasterTransmit_l(offset); 
  
  //write gain settings to gain register
  SPI_MasterTransmit_c(CMD_WRITE_GAIN_REG1);
  SPI_MasterTransmit_c(1<<gain); 
  SPI_MasterTransmit_c(0); 
  SPI_MasterTransmit_c(0); 
  SPI_MasterTransmit_c(0); 
  
}

/******************************************************************/
// stop CS5532 continous conversion mode so that it can receive
// new command.
/******************************************************************/
/*u8 CS5532_Cont_Conv_Stop()
{ 
   if(PORTB.P_MISO == 1)
      return(0xff);
   //stop continous converson mode
   SPI_MasterTransmit_c(CMD_STOP_CONT_CONV);
   //clock out the 32 bits before exit
   SPI_MasterReceive_l();      
   return(0x0);   
} //*/

/******************************************************************/
//            CS5532 system offset calibration
// Offset calibration on CS5532.
// return TIMEOUTERR (0xff) if timeout error occurs.
// return SUCCESSFUL (0x0) if operation is successful.
/******************************************************************/ 
/*u8 CS5532_SysOffsetCal(void)
{
  u16 timeout;

#ifdef _CS5532_DISPLAY_OFFSET_
  u8 data[4];
#endif  
  
 // perform offset calibration
 SPI_MasterTransmit_c(CMD_SYSTEM_OFFSET_CAL_SETUP1);
 // wait till calibration completes or timeout
 timeout = CAL_TIMEOUT_LIMIT;
 while(PORTB.P_MISO == 1)
 {
    if (timeout-- == 0)
      return(TIMEOUTERR);
 }
 
#ifdef _CS5532_DISPLAY_OFFSET_
  //read back offset register 
  SPI_MasterTransmit_c(CMD_READ_OFFSET_REG1);
  data[0] = SPI_MasterReceive_c(); 
  data[1] = SPI_MasterReceive_c(); 
  data[2] = SPI_MasterReceive_c(); 
  data[3] = SPI_MasterReceive_c();   
  
  //display through RS485 port
  sleepms(UART_DELAY);
  putchar(data[0]);   
  sleepms(UART_DELAY);
  putchar(data[1]);   
  sleepms(UART_DELAY);
  putchar(data[2]);   
  sleepms(UART_DELAY);
  putchar(data[3]);
  sleepms(UART_DELAY);  
#endif   
 
  return(SUCCESSFUL);   
} //*/

/******************************************************************/
//            CS5532 system Gain calibration
// Gain calibration on CS5532 
// return TIMEOUTERR (0xff) if timeout error occurs.
// return SUCCESSFUL (0x0) if operation is successful.
/******************************************************************/
/*u8 CS5532_SysGainCal(void)
{
 u8 timeout;

#ifdef _CS5532_DISPLAY_GAIN_
 u8 data[4];
#endif   

 // gain calibration
 SPI_MasterTransmit_c(CMD_SYSTEM_GAIN_CAL_SETUP1);
 // wait till calibration completes or timeout
 timeout = CAL_TIMEOUT_LIMIT;
 while(PORTB.P_MISO == 1)
  { 
    if (timeout-- == 0)
      return(TIMEOUTERR);
  } 
 
#ifdef _CS5532_DISPLAY_GAIN_
  //read back offset register 
  SPI_MasterTransmit_c(CMD_READ_GAIN_REG1);
  data[0] = SPI_MasterReceive_c(); 
  data[1] = SPI_MasterReceive_c(); 
  data[2] = SPI_MasterReceive_c(); 
  data[3] = SPI_MasterReceive_c(); 
  
  sleepms(UART_DELAY);
  putchar(data[0]);   
  sleepms(UART_DELAY);
  putchar(data[1]);   
  sleepms(UART_DELAY);
  putchar(data[2]);   
  sleepms(UART_DELAY);
  putchar(data[3]);
  sleepms(UART_DELAY);  
#endif 
  
 return(SUCCESSFUL);  
}  //*/

/******************************************************************/
//            Read CS5532 Offset/Gain calibration result
// Read CS5532 offset/gain calibration result and generate checksum.
// Result is read back into a buffer pointed by a pinter parametric.
// Buffer size is at least 11 bytes long. Result will be stored into
// EEPROM later.
/******************************************************************
 //void CS5532_ReadCal(unsigned char *CalBuf) 
 void CS5532_ReadCal(unsigned int CalBuf)
 {
   unsigned char i;
   unsigned char checksum;
   unsigned char *pCalBuf;
   pCalBuf = CalBuf;
   checksum = 0;
   
   //read gain calibration result and save data into buffer
   SPI_MasterTransmit(CMD_READ_GAIN_REG1);
   *pCalBuf++ = 'G';
   for(i=4;i>0;i--)
     *pCalBuf++=SPI_MasterTransmit(NULL);  

   //read offset calibration result and save data into buffer
   SPI_MasterTransmit(CMD_READ_OFFSET_REG1);
   *pCalBuf++= 'O';
   for(i=4;i>0;i--)
      *pCalBuf++=SPI_MasterTransmit(NULL);
  
   //generate checksum byte
   pCalBuf=pCalBuf-10;
   for(i=10;i>0;i--)
       checksum=checksum+*pCalBuf++;
   // checksum byte
   *pCalBuf = ~checksum + 1;      
  } //*/

/******************************************************************/
//             Set CS5532 OFFSET/GAIN Registers 
// This function allows users to set offset cal value manually
// Arg 1: offset_data,  points to a 4-byte data array
// Arg 2: 'O' to set OFFSET register, 'G' to set Gain register.
// return 0x0 if set offset register successfully
// return 0xff if fails to set offset register.
/******************************************************************/
/*u8 CS5532_Set_Offset_Gain(u8 *setting, char reg)
{
  u8 *ptr;
  u8 i, cFlag;

  //Write offset data to OFFSET register
  ptr = setting;
  if ((reg != 'O') && (reg != 'G'))
      return(0xff);  
  if(reg =='O')  
    SPI_MasterTransmit_c(CMD_WRITE_OFFSET_REG1);
  else 
    SPI_MasterTransmit_c(CMD_WRITE_GAIN_REG1); 
  for(i=4;i>0;i--)
     SPI_MasterTransmit_c(*ptr++);
      
  // Verify whether setting is successfully done.
  cFlag = 0x0;
  ptr = setting;
  if(reg =='O')
    SPI_MasterTransmit_c(CMD_READ_OFFSET_REG1);
  else
    SPI_MasterTransmit_c(CMD_READ_GAIN_REG1);
  for(i=4;i>0;i--)
  { if(SPI_MasterReceive_c() != *ptr++)
       cFlag = 0xFF;
  } 
  return(cFlag);           
} //*/

/********************************************************************************/ 
// fill data buffer and get average 
// return AD_BUSY if A/D conversion is not completed yet.
// return AD_OVER_FLOW if A/D conversion overflow happens.
// otherwise return average of multiple AD samples.
/********************************************************************************/ 
#define DATA_NUM 4                 
//fill data buffer and return average.
u16 DataBuf[DATA_NUM];

u16 FillDatBuf()
{
   u32 sum;
   u16 average;
   u8 ConvTempbuf[4], k; 
   k = 0;
   
    //get multiple (DATA_NUM) samples.
   while(k < DATA_NUM)
   {      
      if(CS5532_ReadADC(ConvTempbuf) != SUCCESSFUL)                      
          return(AD_BUSY);
      
      //LSB byte of the 32 bits is monitor byte, 
      if (ConvTempbuf[3] != 0) { 
          RS485._global.cs_status |= 0x8; 
          return(AD_OVER_FLOW);
      }
      else 
      {  
         RS485._global.cs_status &= 0xF7; 
         sum = ConvTempbuf[0];            
         DataBuf[k++] = (sum << 8) + ConvTempbuf[1];
      }
   }
   
   // average of all samples.          
   sum = 0;
   for(k=0; k< DATA_NUM; k++)
      sum += DataBuf[k];
   // long int to int type
   average = (sum/DATA_NUM) & 0xFFFF;              
   return(average);
} 

/********************************************************************************/     
// software filter on material weight 
// Algorithm:  
/********************************************************************************/     
#define AVERAGE_NUM 32  //average buffer size, must be times of 4.
u8 Buf_Is_Full = 0;

void CS5532_PoiseWeight()
{       
   static u16 AverageBuf[AVERAGE_NUM];
   u16 SortBuf[AVERAGE_NUM];     
   u16 temp; 
   u32 sum;
   u8 i, j;
   
   /*************************************************************/         
   //sample more data at the first time to fill the buffer.       
   /*************************************************************/
   if(Buf_Is_Full == 0)
   { 
      for(j=0; j< AVERAGE_NUM; j++)
      {
         temp = FillDatBuf();
         if((temp == AD_BUSY) || (temp == AD_OVER_FLOW)) 
         {
           RS485._global.cs_mtrl = temp; // invalid data 
           return; 
         }
         else
           AverageBuf[j] = temp;     
      }      
  
      // flag to indicate databuf is ready.
      Buf_Is_Full = 1;  
   }     
   /*************************************************************/
   //shift in the buffer to only save latest AVERAGE_NUM average data.
   /*************************************************************/
   temp = FillDatBuf();
   if((temp == AD_BUSY) || (temp == AD_OVER_FLOW)) 
   {
      RS485._global.cs_mtrl = temp; // invalidate "cs_mtrl"
      return;
   }
   
   /*************************************************************/
   // insert new data to data buffer (latest on the right)
   // copy data to sortbuf for further process.
   /*************************************************************/
   for(j=0; j<AVERAGE_NUM-1; j++)
   {  AverageBuf[j] = AverageBuf[j+1];
      SortBuf[j] = AverageBuf[j];   
   }
   AverageBuf[AVERAGE_NUM-1] = temp;
   SortBuf[AVERAGE_NUM-1] = temp; 
   

   // sort data buffer. min at buf[0].
   for(i=0; i<(AVERAGE_NUM/2); i++)
   {  for(j=0; j<AVERAGE_NUM-1-i; j++)
      {  // swap data, max to higher address
         if(SortBuf[j] > SortBuf[j+1])
         {  temp = SortBuf[j+1];
            SortBuf[j+1] = SortBuf[j];
            SortBuf[j] = temp;            
         }                                       
      }      
   } 
   
   /*
   if(temp <= AverageBuf[0])
   {  // insert data into buf[0]
      for(j=AVERAGE_NUM-1; j>0; j--)
        AverageBuf[j] = AverageBuf[j-1];
      AverageBuf[j] = temp;  
   }
   else if(temp >= AverageBuf[AVERAGE_NUM-1])
   {  for(j=0; j<AVERAGE_NUM-1; j++)
         AverageBuf[j] = AverageBuf[j+1];
      AverageBuf[j] = temp;    
   }
   else // insert data in the middle of buffer 
   {  for(j=0; j<AVERAGE_NUM-1;)
      {  if(temp >= AverageBuf[j])
           j++;
         else
           break;          
      } 
      // insert data at buf[j]
      for(i=0; i< j-1; i++)
         AverageBuf[i] = AverageBuf[i+1];
      AverageBuf[i] = temp;       
   } */

   /*********************************************************/
   // Median Filter                         
   /*********************************************************/   
   sum = 0;
   i = AVERAGE_NUM>>2;
   for(j=i; j<AVERAGE_NUM-i; j++)
   {
      sum+= SortBuf[j];
   }
   RS485._global.cs_mtrl = (sum/16) & 0xFFFF;   
   
   //if ((RS485._flash.cs_Filter_option & 0xF0) == 0x10)
   
                    
}                                                                   

/*****************************************************************************/
//              Convert CS5532 output to gram  
// Formula to transfer CS5532 measurement to actual weight£º
//   Mtrl_Weight_gram      (cs_mtrl  - cs_zero)        temp1
// ------------------  = --------------------------- = ------
//   Poise_Weight_gram     (cs_poise - cs_zero)        temp2
// note: to minimize non-lineary of A/D, 5 poises can be used for calibriation
// The one which is most close to cs_Mtrl will be found and used. 
// If Bit 7 of test_mode_reg1 is cleared, multi poises are used.
// "old_cs_zero" is used to monitor change of caliration data.
// if "old_cs_zero" is not equal to "cs_zero", "cs_poise" will be adjusted
// accordingly. 
// There could be such a confliction as below:
// "cs_zero" is changed by user, then node board software detect the change
// and start to adjust "cs_poise" automatically while at the same time
// user starts to change "cs_poise" (calibration) at the same time. Confliction!!!
// However because MCU speed to update cs_poise is much faster than user's 
// operations on PC, this case will not happen.
/***************************************************************************/
void CS5532_Poise2Result()
{  
   u8 min_delta_index;     // index of poise with min delta from material weight.
   u16 temp2;
   u32 temp1,temp3, temp4;
   u16 delta, min_delta;
   u8 i;             
#ifdef _FORCE_CONSTANT_WEIGHT_ 
   RS485._global.Mtrl_Weight_gram = 0xfff1;
   return;   
#endif

   // check if error happened in CS5532_PoiseWeight()
   if(RS485._global.cs_mtrl > MAX_VALID_DATA)
   { // result is invalid. 
     // pass fail code to variable Weight_gram, which will be read by master board.
     RS485._global.Mtrl_Weight_gram =  RS485._global.cs_mtrl;                
     return;      
   }       
  
   if(RS485._global.cs_mtrl < RS485._flash.cs_zero)
      temp1 = 0;
   else
      temp1 = RS485._global.cs_mtrl - RS485._flash.cs_zero;                       

   /***************************************************************/
   //adjust cs_poise based on comparison between cs_zero and old_cs_zero
   if(RS485._flash.cs_zero > RS485._global.old_cs_zero)
   {
      delta = RS485._flash.cs_zero - RS485._global.old_cs_zero; 
      for(i=0;i<5;i++)
         RS485._flash.cs_poise[i] += delta;
   }
   if(RS485._flash.cs_zero < RS485._global.old_cs_zero)
   {
      delta = RS485._global.old_cs_zero - RS485._flash.cs_zero; 
      for(i=0;i<5;i++)
         RS485._flash.cs_poise[i] -= delta;   
   }   
   RS485._global.old_cs_zero = RS485._flash.cs_zero;   
   
   /***************************************************************/
   // Search the poise which is most close to cs_mtrl
   if(!ENABLE_MULTI_POISES) 
      min_delta_index = 0;
   else
   {  min_delta = 0xffff;
      for(i=0;i<5;i++)
      { 
         if(RS485._global.cs_mtrl > RS485._flash.cs_poise[i])
            delta =  RS485._global.cs_mtrl -  RS485._flash.cs_poise[i];
         else
            delta =  RS485._flash.cs_poise[i] - RS485._global.cs_mtrl;      
         if (delta < min_delta)
         {   min_delta = delta ; min_delta_index = i; }      
      }
   }          

   /***************************************************************/        
   if(RS485._flash.cs_poise[min_delta_index] < RS485._flash.cs_zero)
      temp2 = 0;
   else
      temp2 = RS485._flash.cs_poise[min_delta_index] - RS485._flash.cs_zero;  
        
   // compiler default: u16*u16 = u16       
   // so we use u16 * u32 = u32 here
   temp3 = RS485._flash.Poise_Weight_gram[min_delta_index] * temp1;                                                         
   temp3 = temp3 << 6;   // X64. so unity is 1/64g      	
        
   if(temp2 != 0)
   {
      temp4 =  temp3/temp2;
      RS485._global.Mtrl_Weight_gram = (temp4 >> 6);
      if(RS485._global.Mtrl_Weight_gram==0xffff)
          RS485._global.Mtrl_Weight_gram= OVERWEIGHT;
      RS485._global.Mtrl_Weight_decimal = temp4 & 0x3F;
   }   
   else
      RS485._global.Mtrl_Weight_gram =  DIV_ERROR; // indicate bad calibration                     
   // master board won't find this node if Mtrl_Weight_gram is always 0xffff.
   // Bad calibartion data will make Mtrl_Weight_gram always bad.
   // so we set Mtrl_Weight_gram to a different data to indicate calibration error.
   /***************************************************************/                      
   // if bit2 of test_mode_reg1 is set, AD output is sent to master board / PC.
   if(DISPLAY_AD_RAW_DATA)
        RS485._global.Mtrl_Weight_gram =  RS485._global.cs_mtrl;
   return;
}
/************************************************************************/
// MEGA8 EEPROM Read and Write subroutines
// Interrupt mode is used to avoid wait during EEPROM writing
// Header files needed: 
// drveeprom.h:
// mega8.h: MEGA8 related constant
/************************************************************************/
#define _DRVEEPROM_C_ 

#include "drveeprom.h" 
#include <mega16.h>                     
#include "define.h"

/************************************************************************/

// define data buffer queue for EEPROM read/write
static S_EEPROM_POSITION volatile _sEepromBuf[EEPROM_WRITE_BUF_SIZE]; 

// EEPROM buffer write pointer/index(user writes to RAM buffer)
static u8 volatile _bEepromBufWrPtr; 

// EEPROM buffer read pointer(read data from buffer and write to EEPROM)
static u8 volatile _bEepromBufRdPtr; 

// number of data in EEPROM writing buffer
static u8 volatile _bEepromBufNs; 

#define fgEepromBufEmpty()      (_bEepromBufNs == 0) 
#define fgEepromBufFull()       (_bEepromBufNs == EEPROM_WRITE_BUF_SIZE) 


/************************************************************************/
//               Write EEPROM using EEPROM Interrupt
// if buffer is empty, stop EEPROM interrupt
// if buffer is not empty(still some data to be written into EEPROM), 
// write EEPROM.  The buffer here is a queue.
/************************************************************************/

//#pragma interrupt_handler vIvEeReady:iv_EE_READY 
interrupt [EE_RDY] void vIvEeReady(void) 
{ 
    if(!fgEepromBufEmpty()){ 
        while(EECR & (1<<EEWE));  /* wait for last write to complete */
        EEAR = _sEepromBuf[_bEepromBufRdPtr].wAddress; 
        EEDR = _sEepromBuf[_bEepromBufRdPtr].bVal; 
        EECR |= (1<<EEMWE); 
        EECR |= (1<<EEWE); 
        _bEepromBufRdPtr++; 
        if(_bEepromBufRdPtr >= EEPROM_WRITE_BUF_SIZE) 
            _bEepromBufRdPtr = 0; 

        _bEepromBufNs--; 
    }else{   // disable EEPROM interrupt if no more data to be written into EEPROM
        EECR &= ~(1<<EERIE); 
    } 
}    

/************************************************************************/
//                     Write data into EEPROM buffer
// If buffer is not full, write data into buffer, return RET_SUCCESS(0x0)
// flag. If buffer is full, return RET_BUSY(0xff) flag
/************************************************************************/ 

u8 bWriteData2Eeprom_c(u16 wAddress, u8 bVal) 
{ 
    if(fgEepromBufFull()) 
        return RET_BUSY; 
    // add for EEPROM protection
    //if(!EN_EEPROM_WRITE)
    //   return RET_SUCCESS;

    // full file buffer if buffer is not full yet
    #asm("cli");     // disable all interrupt
    _sEepromBuf[_bEepromBufWrPtr].wAddress = wAddress; 
    _sEepromBuf[_bEepromBufWrPtr].bVal = bVal; 
    _bEepromBufWrPtr++; 
    if(_bEepromBufWrPtr >= EEPROM_WRITE_BUF_SIZE) 
        _bEepromBufWrPtr = 0; 

    _bEepromBufNs++; 
    
    EECR |= (1<<EERIE); 
    #asm("sei"); // enable EEPROM ready interrupt and global interrupts 

    return RET_SUCCESS; 
} 
u8 bWriteData2Eeprom(u16 wAddress, u8 *bVal, u8 totalsize){
  u8 uc_eeprom_data;
  while(totalsize > 0){
          if(bReadDataFromEeprom_c(wAddress,&uc_eeprom_data) != RET_SUCCESS){
                return totalsize;
          }
          // skip programming if data to be written is the same as EEPROM data 
          if(uc_eeprom_data != *bVal) {
	         if(RET_SUCCESS != bWriteData2Eeprom_c(wAddress,*bVal))
	                return totalsize;       //wait until the write succeed
	  }
          totalsize--; // already programmed one, number of data left decreases by 1
          wAddress++;
          bVal++;
  }
  return totalsize;  
}


/************************************************************************/
//                         Read data from EEPROM 
// The programer must ensure EEPROM is NOT in writing state before reading
// EEPROM. This subroutine return RET_BUSY(0xff) flag when EEPROM is in 
// writting state. Return RET_SUCCESS(0x0) flag if reading successfully
// Data readout is saved into read buffer.
/************************************************************************/ 
u8 bReadDataFromEeprom(u16 wAddress, u8 *pbVal,u8 size)
{
	while(size-- > 0){
		if(bReadDataFromEeprom_c(wAddress++,pbVal++) != RET_SUCCESS) 
		        return RET_BUSY;
	}
	return RET_SUCCESS;
}	
u8 bReadDataFromEeprom_c(u16 wAddress, u8 *pbVal) 
{ 
    if(EECR & (1<<EEWE)) 
        return RET_BUSY; 

    EEAR = wAddress; 
    EECR |= (1<<EERE); 
    *pbVal = EEDR; 
    return RET_SUCCESS; 
} 

/************************************************************************/
//                        Initialize EEPROM
// Intialize buffer variables. Diable EEPROM interruption
// EEPROM interrupt is enabled after writing data to EEPROM buffer.
/************************************************************************/ 
void vInitEeprom(void) 
{ 
    EECR = 0x00; 
    _bEepromBufWrPtr = 0; 
    _bEepromBufRdPtr = 0; 
    _bEepromBufNs    = 0; 
} 

