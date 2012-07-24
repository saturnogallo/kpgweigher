/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.8b Professional
Automatic Program Generator
?Copyright 1998-2006 Pavel Haiduc, HP InfoTech s.r.l.
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
// include header files
/*************************************************************************************************************/
#include <mega16.h>
#include "spi.h"
#include "cs5532.h"
#include "drveeprom.h" 
#include "uart.h"
#include "motor.h"
#include "define.h"  
#include "command.h" 

/*********************************************************************************************
*                                     GLOBAL VARIABLES
*********************************************************************************************/
NODE_CONFIG RS485;                                /* structure of node */
OS_SCHEDULER os_sched;                            /* timer service */
u8 myaddr;                                        /* rs485 addr of current node */
u8 hw_id;                                         /* hard-wired board type (PORTC[7:6]), weight or interface */
bit freq_is_50HZ;                                 /* freq of AC power supply */
u16 release_times;                                /* How many times node has been released */
u8 flag_turn_off_watchdog;
u8 numOfMfgDataToBePgm;

/* dynamic magnet amplitude adjustment */
#define MAX_AMP_WEIGHT_RECORDS        50          
#define NOT_AVAILABLE             0xFFFF
bit run_time_adj_done;
bit record_weight_flag;  
u16 amp_weight[MAX_AMP_WEIGHT_RECORDS];
u16 last_weight, current_weight, weight_gap;      /* initial weight before adding material*/  
u8  local_magnet_amp;
u16 local_target;
//u8  goon_cnt;
                                                   
/* packer interface prototypes */    
extern u8 Packer_Is_Busy();
extern void Init_interface(); 
extern u8 Tell_Packer_Release_Done(); 
#if 0  
extern void Tell_Packer_Force_Release();
extern void Tell_Packer_Weigher_Rdy();
#endif

/*****************************************************************************/
// Flag_enable command:                                            
#define ENABLE_OFF                  0    // stop machine.
#define ENABLE_START_MACHINE        1    // for mannual manipulation. 
#define ENABLE_INIT_AD              2    // initialize AD
#define ENABLE_TURNS                3    // rotate upper motor
#define ENABLE_TURNW                4    // rotate lower motor
#define ENABLE_VIBRATE              5    // start vibrator  
#define ENABLE_RESET_WEIGHT         7    // reset current weight to 0     
#define ENABLE_INIT_PACKER          8    // initialize packer interface
#define ENABLE_INIT_PACKER_DEFAULT  9    // default setting, for debug only
#define ENABLE_PACKER_RELEASE      10    // send release signal to packer 
#define ENABLE_WEIGHER_READY       11    // send weigher ready signal to packer
#define ENABLE_FORCE_RELEASE       12    // send signal to indicate "force release"
#define ENABLE_LOAD_MFG_CAL        13    // use factory calibration data
#define ENABLE_SAVE_MFG_CAL        14    // save user calibration data as MFG CAL data

#define ENABLE_ON                  80   // machine is fully running now
/*****************************************************************************/

/*****************************************************************************/
//                     Timer 2 interrupt service routine
// Timer 2 overflow interrupt service routine.
// Generate 10ms interrupt.
// Support up to 8 (0-7) timer services. timerlen[] store counters for different 
// timer tasks.  Timer task 7 is reserved for OS timer tick. 
// 
// #define AC_FREQ_TIMER                   0x07          
// #define PACKER_RESP_TIMER               0x06
// #define ERROR_SIGNAL_TIMER              0x05  
// #define SIGNAL_PLS_WIDTH_TIMER          0x04
// #define READY_SIGNAL_TIMER              0x03
// #define FEED_DELAY_TIMER                0x02
// #define WSM_TIMER                       0x01
// #define RSM_TIMER                       0x00
//
/*****************************************************************************/
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
  u8 tmr; 
  /* Generate 10ms timer interrupt periodically. */ 
  TIFR |= (1<<TOV2);                             // Clear overflow flag
  TCNT2=0xB8;                                    // 7.2KHZ input,10ms timer.
      
  if(os_sched.status)                            // At least one timer is active   
  {   
     for(tmr=0; tmr<MAX_NUM_OF_TIMERS; tmr++)    // Loop to find all active timers.
     {
        if(os_sched.status & ((u8)1<<tmr))       // If coresponding timer is active, 
        {
            if(os_sched.timerlen[tmr])           // If timer not expired, adjust 
               os_sched.timerlen[tmr]--;         // timer ticks to go 
            else                                 // Else timer expired.   
            {   switch(tmr)                              
                { 
                 case READY_SIGNAL_TIMER:        // toggle OR PORTB.4, weigher ready signal
                   PORTB.4 = ~PORTB.4;
                   break;                   
                 case SIGNAL_PLS_WIDTH_TIMER:    // toggle OF PORTB.5, release done signal
                   PORTB.5 = ~PORTB.5; 
                   PORTB.7 = ~PORTB.7;
                   break;                   
                 case ERROR_SIGNAL_TIMER:        // toggle OE PORTB.6, force release signal
                   PORTB.6 = ~PORTB.6;
                   break;                   
                 default:
                   break;            
                } 
                os_sched.status &= ~((u8)1<<tmr); // clear status bit to indicate task completion
            } /* end of else */  
        }
     } /*for*/    
  } 
  else
     TIMSK &= 0xBF;                              // disable TMR2 overflow interrupt.       
}                          

/*****************************************************************************/
// kick off a timer task, for timer_len * 10 ms
// task0: delay1
// task1: delay2
// task2: feed delay (delay between releasing material and informing packer)
// task3: Output_Ready pulse width
// task4: Output_Feed pulse width
// task5: Output_Force_Release pulse width
// task6: magnet (magnet_is running) 
// task7: OS timer tick: 10ms
/*****************************************************************************/
void kick_off_timer(u8 task_id, u16 timer_len)
{
   if((task_id > MAX_TMR_ID)||(!timer_len))
     return;

   #asm("cli");  
   os_sched.timerlen[task_id] = timer_len;   /* set up timer */
   os_sched.status |= (u8)1<<task_id; 
   #asm("sei"); 
   
   if(!(TIMSK & 0x40))                       /* if timer not kicked off yet, kick it off */
   {
     #asm("cli");
     TCNT2=0xB8;                             /* 7.2KHZ input,interrupt frequency is set to 100HZ (10ms timer) */
     TIFR |= (1<<TOV2);                      /* clear residual overflow flag */
     TIMSK |= 0x40;                          /* enable TMR2 overflow interrupt.*/  
     #asm("sei");  
   }
}
/*****************************************************************************/
//  Kill an active timer
/*****************************************************************************/
void kill_timer(u8 task_id)
{
   if(task_id > MAX_TMR_ID)
      return;
   #asm("cli"); 
   os_sched.status &= ~((u8)1<<task_id); 
   os_sched.timerlen[task_id] = 0;
   #asm("sei");     
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
   // State7=1 State6=T State5=1 State4=1 State3=1 State2=1 State1=1 State0=1
   // PA0: MT1_PS, PA1: MT2_PS, PA2: MT_MD0, PA3: MT_MD1, PA4: MT_CLK, PA5: MT_DIR
   // PA6: PWM_FB, PA7: Test1 
   PORTA=0x3F;
   DDRA=0xBF;

   // Port B initialization
   // Func7=Out Func6=In Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=Out 
   // State7=0 State6=T State5=0 State4=1 State3=T State2=T State1=1 State0=0 
   PORTB=0x12;
   DDRB=0xB3;


   // Port C initialization
   // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
   // State7=PU State6=PU State5=T State4=T State3=T State2=T State1=T State0=T 
   // PC[7:6], board id, input 
   PORTC=0xC0;  // PINC[7:6] internal pull up. 
   DDRC=0x00;  // all input 

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
   
   // Test H/W ID PC[7:6]: 11 -- Weight Board, 10 -- Interface Board
   hw_id = (PINC & 0xC0) >> 6; 
   PORTC=0x00;  // PINC[7:6], turn off internal pull up.  
   
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
#define MYDEAD(a) dead_loop(a)
void dead_loop(u16 refresh_cycle)
{ 
    static u16 tick; 
    tick++;
    if(tick == refresh_cycle)
        LED_ON(PIN_RUN);
    if(tick >= (refresh_cycle<<1)){
        LED_OFF(PIN_RUN);
        tick = 0;
    }
}    

/*****************************************************************************/
//                     reset current weight to zero
/*****************************************************************************/
#define ERR_DATA_FAIL 1
#define STATUS_OK 0
u8 reset_weight()
{    
   u8 status = STATUS_OK;
   
   /* wait max 0.5 sec for valid data */      
   kill_timer(RSM_TIMER);
   kick_off_timer(RSM_TIMER, 50);
   /* get current A/D readings */   
   while(CS5532_PoiseWeight() != DATA_VALID){
      if(RSM_TMR_DLY_END){   /* time out */ 
         status = ERR_DATA_FAIL;
         break;
      }
   }   
   kill_timer(RSM_TIMER);

   if(status == STATUS_OK){   
      // update user defined "cs_zero"
      RS485._rom.new_cs_zero = RS485._global.cs_mtrl;
      RS485._global.new_cs_zero = RS485._rom.new_cs_zero;       
      //update weight
      CS5532_Poise2Result();
   }         
   return status;
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
    static u8 retry_cnt; 
    
    // initialize: weight is zero after release.
    last_weight = 0; 
    current_weight = 0;
    //weight_gap = local_target; 
    
    switch(RSM_Flag)
    {
    /************************************************************/   
    // Start to open the lower bucket.
    /************************************************************/
    case RM_READY_RELEASE:
       if(!Motor_Is_Running()){
          RS485._global.Mtrl_Weight_gram = INVALID_DATA ; 
          flush_data_buffer();           
          Motor_Driver('W',MOTOR_SHIFT_CYCLE_OPEN, KEEP_POWER_ON);                   
          RSM_Flag = RM_WAIT_OPEN;
       }
       break;                              
    /************************************************************/      
    //wait for motor to stop(bucket open) 
    //Once it stops, start timer to delay so that material have 
    //enough time to pass from the lower bucket to material pool.
    /************************************************************/   
    case RM_WAIT_OPEN:       
       if(!Motor_Is_Running()){            
          if(RS485._flash.open_w){
             kick_off_timer(RSM_TIMER, RS485._flash.open_w);         
          }
          RSM_Flag = RM_WAIT_OPEN_W;     
       }
       break;
    /************************************************************/      
    //wait till delay eclapse (material released to pool)
    //then start motor to close the bucket.
    /************************************************************/      
    case RM_WAIT_OPEN_W:      
       if(RSM_TMR_DLY_END){                        
          Motor_Driver('W',HALF_CYCLE_CLOSE_WITH_SENSOR,POWER_OFF_AFTER_ROTATION);
          RSM_Flag = RM_WAIT_SHIFT;     
       }
       break;                                     
    /************************************************************/      
    // if motor stops (bucket closed), set timer to delay before
    // next action.
    /************************************************************/      
    case RM_WAIT_SHIFT:       
       if(!Motor_Is_Running()){            
          if(RS485._flash.delay_w)  
             kick_off_timer(RSM_TIMER, RS485._flash.delay_w);
          RSM_Flag = RM_WAIT_DELAY_W;     
       }
       break;
    /************************************************************/      
    // After delay, state machine goes back to initial state. 
    // Clear release flag.
    // RS485._flash.magnet_freq is now used to set weight reset freq  
    /************************************************************/      
    case RM_WAIT_DELAY_W:
       if(RSM_TMR_DLY_END){
          RS485._magnet.half_period = release_times;                /* debug monitor */
          if(RS485._flash.magnet_freq){             
             if ((release_times) >= RS485._flash.magnet_freq){      /* time to reset weight */ 
                release_times = 0;                                  /* reset counter */ 
                retry_cnt = 0;
                flush_data_buffer();                                /* flush AD buffer */
                kick_off_timer(RSM_TIMER, 100);                     /* start timer service to wait for weight to be stable */
                RSM_Flag = RM_DELAY_BE4_RESET;                      /* move to next state */
                return;
             }
             else                                                   /* not a reset cycle */
                release_times++; 
          } 
          
          /* autoclear disabled or not this cycle */
          RS485._global.flag_release = 0;                           /* release completes */
          RSM_Flag = RM_READY_RELEASE;
       }
       break;
    /************************************************************/      
    // If weight needs to be reset, wait 1 second before reset weight
    /************************************************************/ 
    case RM_DELAY_BE4_RESET:
       if(RSM_TMR_DLY_END){
         retry_cnt++; 
         if((reset_weight() == STATUS_OK) || (retry_cnt > 3)){      /* weight was reset successfully */                    
            RS485._global.flag_release = 0;                         /* release completes */

            RSM_Flag = RM_READY_RELEASE;
         }
       }
       break;
    /************************************************************/      
    // default entry:
    /************************************************************/       	
    default:
       RSM_Flag = RM_READY_RELEASE;
       break;
    } /* end of switch */
   
    return;  
}
/*******************************************************************************/
//
/*******************************************************************************/
void clear_weight_record()
{  
   u8 i;   
   for(i=0; i<MAX_AMP_WEIGHT_RECORDS;i++)
       amp_weight[i] = NOT_AVAILABLE;
}
/*******************************************************************************/
//                    Feeder(Magnet) Auto Adjustment Algorithm
// This subroutine searches through a "Amp vs. Weight" table to find an optimum
// amplitude value for magnet so that it can produce appoximate target weight
// into bucket. 
//
// Argument 1: weight_delta, Weight of material to be added.
//
// Output: This subroutine updates variable "local_magnet_amp" for magnet driver.
//         "local_magnet_amp" is the amplitude of next magnet cycle.
/*******************************************************************************/
void auto_amp_adj(u16 weight_delta)
{
    u8 new_amp, i, max_amp;
    /*********************************************************
     * Search through all amp_weight records. 
     * IF current "amp" has no weight value attached, 
     *    Move to next record. 
     * ELSE if a weight value is found close to weight gap, 
     *    Use its index to determine new magnet amplitude.
     *********************************************************/
    for(new_amp=0,i=1; i<MAX_AMP_WEIGHT_RECORDS; i++)
    {    
        if(amp_weight[i] == NOT_AVAILABLE)
            continue;
        else if(amp_weight[i] < weight_delta + (local_target>>3))     
            new_amp = i;                      
        else if(!record_weight_flag)
            break;                     
    }
    
    /***********************************************************
    * If the amplitude found is not strong enough to generate 
    * target weight, AND next amp_weight record doesn't have 
    * a valid weight value, increase amplitude by one step to 
    * make a risky trial.
    ************************************************************/
    if(new_amp < MAX_AMP_WEIGHT_RECORDS-1)
    {   
        if((amp_weight[new_amp+1] == NOT_AVAILABLE) && (amp_weight[new_amp] < weight_delta))
            new_amp++;
    }
    else
       new_amp = MAX_AMP_WEIGHT_RECORDS-1;

    /****************************************************************
    * Convert amp_weight index (new_amp) to new magnet amplitude. 
    * To be safe, we set a max limit for internal amplitude adjustment
    * based on user setting. 
    ******************************************************************/
    local_magnet_amp = new_amp<<1;
    max_amp = RS485._flash.magnet_amp + 20;
    if(local_magnet_amp> max_amp)
        local_magnet_amp = max_amp;
} 

/*******************************************************************************/
//                  Magnet State Machine
// magnet_add_material()is called during the delay period of motor_magnet_action()
// or in parallel with release_material() call to save time. 
// State Flag:  WSM_Flag  
// We need to divide the state of MM_WAIT_ADDEND and MM_DELAY_BEFORE_OPEN_MOTOR_S
// into more sub-states so that we can parallelly embeded Magnet State Machine
// into other state machine. 
/*******************************************************************************/
#define MM_ADD_MATERIAL               0
#define MM_WAIT_ADDEND                1
#define MM_DELAY_BEFORE_OPEN_MOTOR_S  2
#define MM_PASS_MATERIAL              3 
#define MM_WAIT_OPEN                  4
#define MM_WAIT_OPEN_S                5 
#define MM_WAIT_PASSEND               6 
#define MM_WAIT_DELAYEND              7
#define MM_CHECK_WEIGHT               8  

//Initialize state of Motor/Magnet/Weight State Machine.
static u8  WSM_Flag = MM_ADD_MATERIAL; 
u8 magnet_add_material()
{    
    switch(WSM_Flag)
    {
    /**************************************************************/
    // Start Magnet to add material
    // Get Magnet amplitude first, IF auto-amp is enabled, call 
    // auto_amp_adj() to adjust amp.
    /**************************************************************/
     case MM_ADD_MATERIAL:
        if(Magnet_Is_Running())
           break;
        /* log actual amp to hw_status register for monitor */
        RS485._global.hw_status = local_magnet_amp; 
                			              
        if(local_target)
           auto_amp_adj(weight_gap);
        else
           local_magnet_amp = RS485._flash.magnet_amp;
        /* Stop adding material when current weight exceeds target weight */
        if((!local_target)||(local_target > current_weight))
        {   E_Magnet_Driver(RS485._flash.magnet_time, local_magnet_amp);     
            WSM_Flag = MM_WAIT_ADDEND;
        }
        else /* don't bother to perform "pass material" action */
        {            
             WSM_Flag = MM_CHECK_WEIGHT; 
             run_time_adj_done = TRUE;   /* don't bother to update record */
        }  
        break;         
    /**************************************************************/
    // wait for magnet to stop, then delay before motor action.
    /**************************************************************/    
     case MM_WAIT_ADDEND:  
        // delay before motor action if magnet stops
        if((!Magnet_Is_Running()) || (Magnet_Is_Running()== 0xffff)){
           if(RS485._flash.delay_f){
              kick_off_timer(WSM_TIMER, RS485._flash.delay_f);   
           }                           
           // move to next state.
           WSM_Flag = MM_DELAY_BEFORE_OPEN_MOTOR_S;
        }	   
        break;
    /**************************************************************/                           
    // Hold in this state for a while so that material have enough
    // time to fall into upper bucket.
    /**************************************************************/
     case MM_DELAY_BEFORE_OPEN_MOTOR_S:
        if(WSM_TMR_DLY_END){ //if delay ends, move to next state.
           WSM_Flag = MM_PASS_MATERIAL;
        }
        break;     
    /**************************************************************/                           
    // Default states.
    /**************************************************************/
     default:  
        break;
    }
    
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
    u8 index;  
    u8 i;
    
    run_time_adj_done = FALSE;  /* permit to update record by default*/ 
    /**************************************************************/ 
    // Start magnet to add material, if we haven't reach state
    // "MM_PASS_MATERIAL" yet, call magnet_add_material() to add
    // material.  
    /**************************************************************/           
     if(magnet_add_material() < MM_PASS_MATERIAL) 
          return;  
    
    switch(WSM_Flag)
    {             
    /**************************************************************/      
    // Open upper bucket to pass material to the lower bucket.
    // Stay in this state if bottom bucket is not closed yet.
    // otherwise, rotate motor for half cycle to open upper bucket.      
    /**************************************************************/ 
     case MM_PASS_MATERIAL:       
        if(!Motor_Is_Running())
        {                                   
           RS485._global.Mtrl_Weight_gram = INVALID_DATA ; 
           flush_data_buffer(); 
           Motor_Driver('S',MOTOR_SHIFT_CYCLE_OPEN, KEEP_POWER_ON);          
           WSM_Flag = MM_WAIT_OPEN;
        }
        break;   
    /**************************************************************/      
    // Wait for the upper motor to stop. Then delay for a while
    // so that material has enough time to be passed from upper bucket
    // to the lower bucket. delay is important especially when material 
    // falls very slowly. A brief delay is needed to avoid material 
    // from getting stuck in bucket. 
    /**************************************************************/     
     case MM_WAIT_OPEN:   
        if(!Motor_Is_Running())
        {        
           if(RS485._flash.open_s)
              kick_off_timer(WSM_TIMER, RS485._flash.open_s);                            
           WSM_Flag = MM_WAIT_OPEN_S;             
        }
        break;
    /**************************************************************/      
    // Waiting (motor stops/bucket open) to give material enough
    // time to drop to the lower bucket, then start motor to close 
    // upper bucket.
    /**************************************************************/     
     case MM_WAIT_OPEN_S:   
        if(WSM_TMR_DLY_END)
        {
           Motor_Driver('S',HALF_CYCLE_CLOSE_WITH_SENSOR, POWER_OFF_AFTER_ROTATION); 
           WSM_Flag = MM_WAIT_PASSEND;
        }
        break;
    /**************************************************************/           
    // Delay for a while after upper motor stops to make sure upper 
    // bucket is closed completely. This prevents magnet from feeding 
    // upper bucket is not closed. This also provide a delay for
    // weight of material in lower bucket to stablize.
    /**************************************************************/          
     case MM_WAIT_PASSEND:  
        if(!Motor_Is_Running())
        {
           if(RS485._flash.delay_s)
           {
              if(RS485._flash.delay_s < 8)
                 RS485._flash.delay_s = 8;   
              kick_off_timer(WSM_TIMER, RS485._flash.delay_s);
           }
           WSM_Flag = MM_WAIT_DELAYEND;
        }
        break;                          
    /**************************************************************/      
    //OK, wait till delay ends                                      
    /**************************************************************/      
     case MM_WAIT_DELAYEND:        
        if(WSM_TMR_DLY_END)
           WSM_Flag = MM_CHECK_WEIGHT;                 
        break; 
    /**************************************************************/           
    // Ready to measure weight or do something else. 		                          
    /**************************************************************/           
     case MM_CHECK_WEIGHT:     
        /*********************************************************** 
        * Clear goon flag to inform master end of "add material" cycle. 
        * New weight is being measured. Node is ready to receive next
        * command: goon or release. 
        *************************************************************/
        RS485._global.flag_goon = 0;
        
        /* Initialize state machine for a new cycle. */
        WSM_Flag = MM_ADD_MATERIAL;      
        /*********************************************************** 
        * Stay here and wait for next instruction from master. Break
        * out from current state in any of the following conditions:
        * 1. Master sends goon command asking me to re-add material. 
        * 2. Master sends release command to release lower bucket.
        * 3. Master sends command to bring me back to manual/stop mode. 
        ************************************************************/ 
        while(RS485._global.flag_goon == 0)
        {
           MYDEAD(0x2000); /* flash LED slower to indicate current state */                     
           /**********************************************************
           *  Debug monitor, update amp_vs_weight record in register 
           **********************************************************/
           if(READ_AUTO_AMP_REC)
           {
              i = RS485._global.test_mode_reg2 & 0x1F;
              RS485._global.new_cs_zero = amp_weight[i];
           }      
           /***********************************************************
           * Polling weight when no valid weight is available. Once
           * weight is available, don't bother to check weight from 
           * time to time. This is to make machine less vulnerable to 
           * mechanical vibration during runtime.  
           ************************************************************/              
           if(RS485._global.Mtrl_Weight_gram > MAX_VALID_DATA)
           {    
                CS5532_PoiseWeight();  
                CS5532_Poise2Result();
           }   
           /************************************************************* 
           * Run time update "amplitude vs. weight" record. 
           * Run time adjustment is only available when we have an 
           * explicit target weight. A "zero" target means no target 
           * weight is provided by user configuration.
           **************************************************************/              
           if((local_target) && (!run_time_adj_done))
           { 
              if(RS485._global.cs_mtrl < MAX_VALID_DATA)
              {                                                                  
                 /* Valid weight, log last "weight_vs_amp" record */
                 last_weight = current_weight;
                 current_weight = RS485._global.Mtrl_Weight_gram;
                 index = local_magnet_amp>>1; 
                 
                 /* If we were asked not to record weight (material 
                  * is re-added), skip amp_weight update.*/
                 if(record_weight_flag == TRUE)
                 {  
                    if(current_weight>last_weight) 
                       amp_weight[index] = current_weight - last_weight; 
                    else
                       amp_weight[index] = 0;
                 
                 /* If we set a record greater than target, its corresponding
                  * amplitude will never be eligible again. So we set it to  
                  * NOT_AVAILABLE, giving it chances to be selected again.*/   
                 if((amp_weight[index] > (local_target-(local_target>>3))) &&
                    (amp_weight[index] < (local_target+(local_target>>2)))) 
                       amp_weight[index] = NOT_AVAILABLE; 
                     /* Last amptidue selected is not strong enough. We didn't
                      * go to a higher amplitude probably because its weight is 
                      * too high to be selected. We need to adjust it. */    
                     if((local_target+(local_target>>3)) > current_weight)
                     {  
                        index++;
                        if(amp_weight[index] != NOT_AVAILABLE)
                        {    
                           if(amp_weight[index] > amp_weight[index-1])
                               amp_weight[index] -= 2;
                        }
                        else
                           amp_weight[index] = amp_weight[index-1];
                     }
                 }
                 else
                    record_weight_flag = TRUE; 
                 /* calculate gap between current weight and target weight */    
                 if(local_target > current_weight)
                 {     
                     weight_gap = local_target - current_weight;                     
                 }
                 else
                     weight_gap = 0;                    

                 run_time_adj_done = TRUE;
              } /* RS485._global.cs_mtrl < MAX_VALID_DATA*/   
           }                               
           /*********************************************************** 
            * While waiting for master command, we can start magnet to
            * add material to upper bucket. Add material only when 
            * flag_enable is ENABLE_ON (bug fix). 
            ***********************************************************/
           else if(RS485._global.flag_enable == ENABLE_ON)
               magnet_add_material(); 
                                  
           /************************************************************** 
           * Conditions of breaking out of this "while" loop: 
           * 1. flag_goon = 1:  
           * IF machine is stopped or in manual mode, break out from the loop.
           * IF release command is received, break out the loop to start 
           * a new cycle.
           ****************************************************************/
           if(RS485._global.flag_enable != ENABLE_ON)
               return;
           else if((RS485._global.flag_release))
           {   
               if(RS485._global.cs_mtrl < MAX_VALID_DATA)
                   weight_gap = RS485._global.Mtrl_Weight_gram;    
               else
                   weight_gap = local_target>>2;   
               // re-add material. last amp forecast is out-of-date
               if(local_target)   
               {   WSM_Flag = MM_ADD_MATERIAL;
                   record_weight_flag = FALSE;
               }
               return; 
           } 
           /************************************************************** 
           * kick watch dog to avoid reset. 
           ****************************************************************/                      
           Feed_Watchdog();  
        } /* end of while */    
        
        break;
    /**************************************************************/           
    // default. 		                          
    /**************************************************************/         
     default: 
        break;
    } /* end of switch */     
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
     RS485._global.diag_status1 = 0;               // Clear all faults
     RS485._global.diag_status2 = 0x80;            // Set bit 7 to indicate board has been reset.
     RS485._global.hw_status = 0; 
     RS485._global.test_mode_reg1 = 0;            
     RS485._global.test_mode_reg2 = 0;                 
     
     RS485._flash.board &= (BOARD_GROUP_MASK | BOARD_TYPE_MASK);  
 
     flag_turn_off_watchdog = 0; //clear flag  
     numOfMfgDataToBePgm = 0;
     os_sched.status = 0;  
     freq_is_50HZ = TRUE; 
     last_weight = 0; 
     current_weight = 0; 
     record_weight_flag = TRUE;
     clear_weight_record();
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
   // Set to default address. 
   RS485._flash.addr = MAX_RS485_ADDR; 
   RS485._flash.addr_copy1 = MAX_RS485_ADDR; 
   RS485._flash.addr_copy2 = MAX_RS485_ADDR;   
   // Board type
   if(hw_id == HW_ID_WEIGHT)
   {   RS485._flash.board = BOARD_TYPE_WEIGHT;
       RS485._flash.board_copy1 = BOARD_TYPE_WEIGHT;
       RS485._flash.board_copy2 = BOARD_TYPE_WEIGHT;
   }
   else if(HW_ID_INTERFACE)
   {   RS485._flash.board = BOARD_TYPE_VIBRATE; 
       RS485._flash.board_copy1 = BOARD_TYPE_VIBRATE;
       RS485._flash.board_copy2 = BOARD_TYPE_VIBRATE;
   }
   // Intialize RS485 baudrate to 115200 (0x0)
   RS485._flash.baudrate = 0x0;
         
   // define tolerance for filtering.  
   RS485._flash.cs_gain_wordrate = 8;      
   RS485._flash.cs_Filter_option = 0x0;    

   // Weight Calibration settings
   RS485._flash.target_weight = 0;   
#if 0
   RS485._flash.cs_zero = 0x4DB1;                              
   RS485._flash.cs_poise[0] = 0x4FB8;
   RS485._flash.cs_poise[1] = 0x52C1;
   RS485._flash.cs_poise[2] = 0x57DD;
   RS485._flash.cs_poise[3] = 0x6209;
   RS485._flash.cs_poise[4] = 0x6C35; 
   RS485._flash.cs_poise[5] = 0x7662;
   RS485._flash.cs_poise[6] = 0x8091;
   RS485._flash.cs_poise[7] = 0x94E8;
   RS485._flash.cs_poise[8] = 0xA945;
   RS485._flash.cs_poise[9] = 0xB373;
#endif
   // Motor & Magnet settings
   RS485._flash.magnet_amp = 50; 
   RS485._flash.magnet_time = 10; 
   RS485._flash.motor_speed = 1;
   RS485._flash.delay_f = 10;
   RS485._flash.delay_w = 10;
   RS485._flash.delay_s = 10;
   RS485._flash.open_s  = 10;
   RS485._flash.open_w  = 10; 
            
   RS485._flash.rom_para_valid = ROM_DATA_VALID_FLAG; 
   RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
   
   //RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;
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
   else if((RS485._flash.addr_copy1 == RS485._flash.addr) && (RS485._flash.addr_copy2 != RS485._flash.addr_copy1))
   {   RS485._flash.addr_copy2 = RS485._flash.addr_copy1; invalid_flag = 0xff;}
   else if((RS485._flash.addr_copy2 == RS485._flash.addr) && (RS485._flash.addr_copy1 != RS485._flash.addr_copy2))
   {   RS485._flash.addr_copy1 = RS485._flash.addr_copy2; invalid_flag = 0xff;}
   
   if((RS485._flash.board_copy1 == RS485._flash.board_copy2) && (RS485._flash.board != RS485._flash.board_copy1))
   {   RS485._flash.board = RS485._flash.board_copy1;  invalid_flag = 0xff; }
   else if((RS485._flash.board_copy1 == RS485._flash.board) && (RS485._flash.board_copy2 != RS485._flash.board_copy1))
   {   RS485._flash.board_copy2 = RS485._flash.board_copy1;  invalid_flag = 0xff;}
   else if((RS485._flash.board_copy2 == RS485._flash.board) && (RS485._flash.board_copy1 != RS485._flash.board_copy2))
   {   RS485._flash.board_copy1 = RS485._flash.board_copy2;  invalid_flag = 0xff;}
                
    if(invalid_flag) 
       RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH); 

}
/*******************************************************************************/
//                    Initialize CS5530
/*******************************************************************************/
#define RESET_VALID 0x10000000 
void init_cs5532(void)
{            
   u8 i; 
   u16 ad_config_reg_msw, temp;
   u8 offset_data[4];

   if(hw_id != HW_ID_WEIGHT)                                // board has no A/D on it.
     return;

   /**********************************************************************************/       
   // To begin, a 4.9152 MHz or 4.096 MHz crystal takes approximately 20 ms 
   // to start. To accommodate for this, it is recommended that a software delay 
   // of approximately 20 ms be inserted before the start of the processor¡¯s ADC 
   // initialization code. Actually this is not necessary in this system because
   // MEGA16 is configured to stay in reset state for 64ms (fuse bit) on power up.
   /**********************************************************************************/   
   for(i=3;i>0;i--)                                         //Initialize AD. Try 3 times max. 
      if(CS5532_Init() == RESET_VALID)
      {          
         temp = 3;                                          // 4: 6.25 Sps, 3: 12.5 sps  2: 25sps
         temp = (temp << 11) | 0x400;                       // unipolar
         if(freq_is_50HZ)
            ad_config_reg_msw = 0x08;                        // xianghua 0x8, 0x0
         else
            ad_config_reg_msw = 0x00;         
         CS5532_Config(ad_config_reg_msw,temp);
         // 0x08: 0B: 0x6DF1, 0x10: 0x2489 
         offset_data[0] = 0x0B;                             // mannually set offset/gain regs.08
         offset_data[1] = 0x00;                             // offset_data[0] = 0x08, gain = 8
         offset_data[2] = 0x00;
         offset_data[3] = 0x00;         
         CS5532_Manual_Offset_Gain_Cal(offset_data, 2);     // gain = 2^3 = 8  xianghua
                                                     
         CS5532_Cont_Conversion();                          //Start CS5532 A/D conversion 
         /**********************************************************************/
         // IMPORTANT NOTE: from now on, CS5532/CS5530 won't accept commands
         // you need to stop cont converison before issue command to CS5532.
         /**********************************************************************/
         break;
      }
            
   if(i)                                                  // initialized successfully,
       RS485._global.diag_status1 &= 0xEF;                // clear bit 4. 
   else
       RS485._global.diag_status1 |= 0x10;                // set bit 4.     
}                                                                        

/*****************************************************************************/
//  master to query firmware revision. 
//****************************************************************************/   
void query_fw_version()
{
   RS485._global.cs_sys_gain_cal_data &= 0xFFFFFF00; 
   RS485._global.cs_sys_gain_cal_data |= FIRMWARE_REV;
}

/*****************************************************************************/
//  Measure AC power supply frequency (50HZ or 60Hz, or error)
//****************************************************************************/ 
u8 measure_AC_freq()
{
    static u8 last_ana_comp_isr_count, timer_enabled;
    u8 cur_ana_comp_isr_count, ana_isr_count; 
    u8 freq_measured; 
    
    freq_measured = FALSE;
                                              
    if(TIMER_AC_FREQ_ONGOING)                                // timer hasn't got timeout yet
       return freq_measured;
    else if(!timer_enabled)                                  // There was no timer task enabled
    {
       if(!Magnet_Is_Running())                              // if Magnet is not running
       {
          kick_off_timer(AC_FREQ_TIMER, 50);                 // kick off 50*10ms timer service
          E_Magnet_Driver(0,0);                              // measure AC power frequency. 
       }
       else
         return freq_measured;  

       if(Motor_Is_Running())
         return freq_measured;
       
       last_ana_comp_isr_count = read_ana_cmp_isr_counter();
       timer_enabled = TRUE; 
       return freq_measured;                                          
    }
    
    //################################################################
    // We triggered a timer task which now gets timeout. 
    // Calculate how many isr (AC pulses) has occured within one second. 
    // if ISR number is within 46~55, AC freq is 50HZ
    // if ISR numberis within 56~65, AC freq is 60HZ.
    // default to 50HZ. 
    //################################################################  
    cur_ana_comp_isr_count = read_ana_cmp_isr_counter();
    if(cur_ana_comp_isr_count > last_ana_comp_isr_count)
       ana_isr_count = cur_ana_comp_isr_count - last_ana_comp_isr_count;
    else
       ana_isr_count = last_ana_comp_isr_count - cur_ana_comp_isr_count;
    //RS485._global.cs_sys_offset_cal_data = ana_isr_count << 1;  // copy the data to cal reg for debug purpose
    RS485._global.ac_freq = ana_isr_count << 1; 
    
    if((ana_isr_count>22) && (ana_isr_count<29))      // AC power supply freq is 50HZ
    {
       freq_is_50HZ = TRUE;
       RS485._global.diag_status1 &= 0xFE;            // AC signal has been detected 
       RS485._global.diag_status2 &= 0xBF;            // freq is 50HZ 
       freq_measured = TRUE;                  
    }
    else if((ana_isr_count>28) && (ana_isr_count<35)) // AC power supply freq is 60HZ
    {   
       freq_is_50HZ = FALSE;       
       RS485._global.diag_status1 &= 0xFE;            // AC signal has been detected
       RS485._global.diag_status2 |= 0x40;            // freq is 60HZ. 
       freq_measured = TRUE;   
    } 
    else                                             
    {                                                 // incorrect freq. 
       freq_is_50HZ = TRUE;                           // default to 50HZ. 
       RS485._global.diag_status1 |= 0x1;             // fail to detect AC cross zero
       RS485._global.diag_status2 &= 0xBF;            // set freq is 50HZ default
       freq_measured = FALSE;       
    }
    
    timer_enabled = FALSE;                            // clear flag (this cycle completes). 
    return(freq_measured); 
}


/*****************************************************************************/
//                      Main function entry here
//
// TASK list of state machine used in main function
// This can be understand as different commands    
// Master board sets NREG_ENABLE to "1" to start machine. Procedure main()here 
// will set "flag_enable" to ENABLE_ON if flag_enable == ENABLE_START_MACHINE. 
/*****************************************************************************/
#define MEASURE_AC_FREQ_INTERVAL 10000     
void main(void)
{           
   u8 i, AC_freq_measured, board_type; 
   u16 loopcnt; 
   bit ad_initialized_to_50HZ;    
   bit packer_interface_initialized;  
   bit packer_timer_kicked;

   u8  release_signal_not_sent = 0;     
   packer_interface_initialized = FALSE;
   
   packer_timer_kicked = FALSE; 
   /********************************************************************/
   //               System Initialization
   /********************************************************************/             
   Init_PORT();                                              // Initialize MEGA16 PORT
   Init_Timers();                                            // Timer (Interrupt) initialization
   vInitEeprom();                                            // Initialize EEPROM   
                                                             // Initialize System global variables (read from EEPROM)
   bReadDataFromEeprom(EEPROM_RS485ADDR,(u8*)&RS485._flash, sizeof(S_FLASH));      
   bReadDataFromEeprom(EEPROM_MFG_ADDR,(u8*)&RS485._rom, sizeof(S_ROM));
   Init_Vars();                                                
   query_fw_version();                                       // FW rev, LSB of "cs_sys_gain_cal_data".    
   /*********************************************************************/
   // Clear firmware upgrade flag on primary firmware power up
   /*********************************************************************/
   bReadDataFromEeprom_c(EEPROM_BOOTFLAG_ADDR, &i);
   if(i != 0xff)                                             // Clear bootloader firmware upgrade flag in EEPROM.
   {  bWriteData2Eeprom_c(EEPROM_BOOTFLAG_ADDR, 0xff);
      sleepms(10);
   }      

   if(RS485._flash.rom_para_valid != ROM_DATA_VALID_FLAG)    // If raw EEPROM (raw board with no EEPROM data programmed) 
       Init_EEPROM_Para();                                   // initialize EEPROM data, must be called after Init_PORT();
   else
       Validate_EEPROM_Data();                               // validate key EEPROM data include rs485 addr, board type etc. 

   UART_Init();                                              // 8 data, 1 stop, even parity, 115200.
   
   if(hw_id == HW_ID_WEIGHT)                                 // Board has A/D on it, board type is tested in Init_PORT()
   { 
      SPI_MasterInit();                                      // Initialize SPI interface (with A/D)
      init_cs5532();                                         // Initialize A/D CS5530 
      ad_initialized_to_50HZ = freq_is_50HZ;
      RS485._global.diag_status2 &= 0xDF;                    // board type is weight       
   }
   else                                                      // This is interface board: packer interface, magnet. 
   {     
      DDRB |= 0xF0;                                           // PORTB[7:4]  -- interface output signals, output.
      PORTB |= 0xF0;                                          // Initialize interface to OFF.
      DDRD.2 = 0;                                             // DDRD.2 = 0, input.
      RS485._global.diag_status2 |= 0x20;                     // board type is weight      
   }
   
   #asm("sei")                                               // initialize golbal interrupt.

   /*************************************************************************/ 
   // Raw board check
   /*************************************************************************/       
   //if RS485 addr is invalid(new board), Use default addr.
   if((!RS485._flash.addr) || (RS485._flash.addr > MAX_RS485_ADDR))
        RS485._flash.addr = MAX_RS485_ADDR;        
   myaddr = RS485._flash.addr;                                                 
   
   /*************************************************************************/ 
   // if user calibration data is invalid, load factory calibration data.
   /*************************************************************************/ 
   if(valid_user_calibration() == FAIL)
       load_mfg_calibration();
       
   kick_off_timer(AC_FREQ_TIMER, 1); // kick off 10ms OS timer tick,
   
   /*************************************************************************/ 
   //                          Task State Machine
   /*************************************************************************/          
   loopcnt = 0; 
   AC_freq_measured = FALSE;  
          
   while(1) 
   {                       
       /************************************************************/
       // Reset node on reset command from master board.
       // flag_reset[7:4] --- reset type
       //   #define UPGRADE_FIRMWARE 0xA
       //   #define NORMAL_BOOT      0x0 Default data after powerup
       // flag_reset[2:0] --- baud rate
       //   0 - 115200bps   1 - 57600bps  2 - 38400bps 
       //   3 - 19200bps    4 - 9600bps   
       /************************************************************/ 
// DO NOT COMMENT THIS CODE SEGMENT OTHERWISE FIRMWARE UPGRADE WON'T WORK.!!!!!!
       if(RS485._global.flag_reset)
       { 
          // If MSB 4 bits refer to a boot for firmware upgrade, write 
          // boot flag into EEPROM (addr 0x40), this will be checked by 
          //   bootloader.
          if( (RS485._global.flag_reset >> 4) == UPGRADE_FIRMWARE)
          {    bWriteData2Eeprom_c(EEPROM_BOOTFLAG_ADDR, RS485._global.flag_reset);           
          } 
          // housekeeping (motor/magnet) before reset
          // we should make sure magnet/motor action completes.
                             
          #asm("cli")
          #asm("wdr")                           // reset watchdog timer to avoid timeout reset
          WDTCR |= (1<<WDCE) | (1<<WDE);        // set WDCE and WDE bit
          WDTCR = 0x09;                         // turn on watch dog timer: WDE = 1, WDP = 001, 32ms.
          #asm("sei")                           // Enable global interrupt.
          RS485._global.flag_reset = 0;          
          while(1);                             // Trapped for Watch-Dog Reset.
       } 
       
       /************************************************************/          
       // Heart beat, can be accessed in ioex reg gain_cal_data
       /************************************************************/          
        MYDEAD(0x400);   /* Flash LED RUN to indicate system is alive*/
        
        if(loopcnt > MEASURE_AC_FREQ_INTERVAL)   
           loopcnt = 0;
        else  
           loopcnt++;
        RS485._global.cs_sys_gain_cal_data &= 0xFFFF; 
        RS485._global.cs_sys_gain_cal_data |= ((u32)loopcnt)<<16; 
        
       /************************************************************/
       // Feed watchdog to avoid reset.Timeout limit is set to 1 sec
       /************************************************************/                                            
        Feed_Watchdog();          
       /************************************************************/
       // polled by PC: packer interface
       /************************************************************/
       if(hw_id == HW_ID_INTERFACE)
       { 
          /* intialize packer interface if not yet */
          if(!packer_interface_initialized)
          {
             RS485._flash.target_weight = 0xD2; // use default, positive output pulse, high level input. 
             RS485._flash.cs_Filter_option = 30; /*300 ms pulse width*/
             Init_interface();
             packer_interface_initialized = TRUE;
          }        
          RS485._global.hw_status = Packer_Is_Busy();          
          if(Packer_Is_Busy())
          {  /* kick off timer for diagnostic */
             if(!packer_timer_kicked)
             {  kick_off_timer(PACKER_RESP_TIMER,1000);
                packer_timer_kicked = TRUE;
             }
             else 
             {   if(os_sched.status &((u8)1<<PACKER_RESP_TIMER))
                     RS485._global.diag_status2 &=0xFD;        /* clear packer diagostic flag */
                 else                                          /* timeout */
                     RS485._global.diag_status2 |=0x2;         /* set packer diagostic flag */
             } 
          }
          else  /* packer response available */
          {
              kill_timer(PACKER_RESP_TIMER);
              packer_timer_kicked = FALSE;
              RS485._global.diag_status2 &=0xFD;        /* clear packer diagostic flag */             
          }         
          /* if release signal not sent yet */
          if(release_signal_not_sent)
          {   release_signal_not_sent = Tell_Packer_Release_Done(); 
             /* if signal has been sent, tell "flag enable" not to do again */
             //if(!release_signal_not_sent)
             //   RS485._global.flag_enable = ENABLE_OFF;                      
		  }                                        
       }
       /************************************************************/
       // Self Diagnose: 
       // Diagnostic result is saved in diag_status1 and diag_status2:       
       // diag_status1[0] = 1: AC cross zero detection failure
       // diag_status1[1] = 1: Magnet control signal wire open failure
       // diag_status1[2] = 1: Lower motor sensor detection failure
       // diag_status1[3] = 1: Upper motor sensor detection failure
       // diag_status1[4] = 1: A/D initialization failure
       // diag_status1[5] = 1: A/D overflow
       // diag_status1[6] = 1: Bad calibration data.
       // diag_status1[7] = 1: Load cell sensor open wire failure
                                   
       // diag_status2[0] = 1: EEPROM update failure
       // diag_status2[1] = 1: Packer response timeout failure
       // diag_status2[2] = 1: 
       // diag_status2[3] = 1: 
       // diag_status2[4] = 1: 
       // diag_status2[5] = 1: 
       // diag_status2[6] = 1: 
       // diag_status2[7] = 1: board reset occured.
       /************************************************************/       
       if((loopcnt < 200) && (!AC_freq_measured))
       {   
           AC_freq_measured = measure_AC_freq();   // result is in bool variable "freq_is_50HZ"
           LED_FLASH(PIN_HWOK);       
           // If AD was not initialized to detected AC frequency, stop A/D and 
           // reinitialize AD. 
           if(hw_id==HW_ID_WEIGHT)
           {
             if(freq_is_50HZ != ad_initialized_to_50HZ)
             {  CS5532_Cont_Conv_Stop();
                init_cs5532();
                ad_initialized_to_50HZ = freq_is_50HZ; 
             } 
           }           
       }
 
       // test if AD reading is normal, too high or too low implies bad sensor
       // or bad mechanical installation. 
       // add a ">0" test because cs_mtrl was intialized to 0. 
       if((RS485._global.cs_mtrl<MAX_VALID_DATA) && RS485._global.cs_mtrl)
       { if((RS485._global.cs_mtrl>0x800) && (RS485._global.cs_mtrl<0xC000))
            RS485._global.diag_status1 &= 0x7F;
         else
            RS485._global.diag_status1 |= 0x80; 
       }
       /************************************************************/
       // Runtime Diagnose on addr, board and baudrate
       /************************************************************/       
        Validate_EEPROM_Data(); 
        myaddr = RS485._flash.addr; 
       /************************************************************/
       // Set local target weight per group.  
       /************************************************************/       
       local_target = RS485._flash.target_weight;
        
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
       //else
          //RS485._global.test_mode_reg1 &= 0xF7;   /* disable EEPROM programming*/
       
       /* program factory calibration data */
       if((numOfMfgDataToBePgm > 0) && (numOfMfgDataToBePgm <= sizeof(S_ROM)))
       {  
          numOfMfgDataToBePgm = bWriteData2Eeprom(
              EEPROM_MFG_ADDR + sizeof(S_ROM) - numOfMfgDataToBePgm,
              (u8*)&RS485._rom + sizeof(S_ROM) - numOfMfgDataToBePgm, 
              numOfMfgDataToBePgm
          );
       }  
          
       /************************************************************/
       // Test Mode(test_mode_reg2): when set:
       // Bit[7:6] = 11, Bit[5:0]as new RS485 addr.
       // Bit[7:6] = 10, Bit[5:0]as new Board Type.
       // Bit[7:6] = 01, Bit[4:0] as index to array "amp_weight". 
       // See "define.h" for details.                                 
       /************************************************************/             
       if(CHANGE_RS485_ADDR)
       {   
            RS485._flash.addr = RS485._global.test_mode_reg2 & 0x1f;        
            myaddr = RS485._flash.addr; 
            RS485._flash.addr_copy1 = myaddr;
            RS485._flash.addr_copy2 = myaddr;            
                  
            if(hw_id == HW_ID_WEIGHT)
               board_type = BOARD_TYPE_WEIGHT;
            else
               board_type = BOARD_TYPE_VIBRATE;
            RS485._flash.board = board_type;
            RS485._flash.board_copy1 = board_type;
            RS485._flash.board_copy2 = board_type; 
                          
            RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
            //RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;            
            RS485._global.test_mode_reg2=0;
            continue; 
       }
#if 0
       if(CHANGE_BOARD_TYPE)
       { 
         RS485._global.test_mode_reg2 &= 0x3F; 
         if((RS485._global.test_mode_reg2 == BOARD_TYPE_WEIGHT) || (RS485._global.test_mode_reg2 == BOARD_TYPE_VIBRATE) || 
            (RS485._global.test_mode_reg2 == BOARD_TYPE_MOTOR))
         {  RS485._flash.board = RS485._global.test_mode_reg2; 
            RS485._flash.board_copy1 = RS485._flash.board;
            RS485._flash.board_copy2 = RS485._flash.board;       
            RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
            //RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;  
            RS485._global.test_mode_reg2 = 0;  
         }
         continue;
       } 
#endif         
       /************************************************************/
       // Magnet/Motor actions. 
       // State range ENABLE_OFF< WSM_Flag <ENABLE_ON: mannual operation. 
       // Turn off watchdog in manual operation.
       /************************************************************/       
       if((RS485._global.flag_enable > ENABLE_OFF)&&(RS485._global.flag_enable < ENABLE_ON))
       {  
          Turn_Off_Watchdog(); 
#if 0          
          if(hw_id == HW_ID_WEIGHT)
          {  
             //if there is ongoing motor_magnet action, allow it to complete this cycle.
             while(WSM_Flag != MM_ADD_MATERIAL)
     	        motor_magnet_action();                
             RS485._global.Mtrl_Weight_gram = INVALID_DATA;
             //if there is release request pending, release material. 
             while((RS485._global.flag_release) || (RSM_Flag != RM_READY_RELEASE))
                release_material();       
          }            
#endif
          // flag_enable register represents different command 
          // need to take care of watch dog.
          switch(RS485._global.flag_enable)
          {             
             case ENABLE_START_MACHINE:
                RS485._global.flag_enable = ENABLE_ON;
                break;
             //###########################################
             case ENABLE_INIT_AD:  
                if(hw_id == HW_ID_WEIGHT)
                   init_cs5532();
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             //###########################################
             case ENABLE_TURNS:      //upper bucket/motor                          
                while(Motor_Is_Running());                
                Motor_Driver('S',MOTOR_SHIFT_CYCLE_OPEN,KEEP_POWER_ON);                         
                while(Motor_Is_Running()) ;
                sleepms(RS485._flash.open_s);              		                                        
                Motor_Driver('S',HALF_CYCLE_CLOSE_WITH_SENSOR,POWER_OFF_AFTER_ROTATION);  
                while(Motor_Is_Running()) ;	                                       
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             //###########################################
             case ENABLE_TURNW:
                RS485._global.flag_release = 1;
                while(RS485._global.flag_release == 1)
                    release_material();             
                RS485._global.flag_enable = ENABLE_OFF;                                             
                break;
             //###########################################
             case ENABLE_VIBRATE:
                while(Magnet_Is_Running())
                {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
                       break; 
                }
                E_Magnet_Driver(RS485._flash.magnet_time, RS485._flash.magnet_amp);  
                while(Magnet_Is_Running())
                {   if(Magnet_Is_Running() == 0xffff)            /* something wrong with magnet driver */
                       break; 
                }                                       
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             //###########################################
             case ENABLE_RESET_WEIGHT:
                if(hw_id == HW_ID_WEIGHT)
                {  for(i=20;i>0;i--)
                   {  if(reset_weight() == STATUS_OK)
                      {
                         if(RS485._global.cs_mtrl < MAX_VALID_DATA)
                            numOfMfgDataToBePgm = sizeof(RS485._rom.new_cs_zero);
                         break;
                      }
                   }
                }
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             //###########################################
             case ENABLE_INIT_PACKER:
                if(hw_id == HW_ID_INTERFACE)
                {  
                   Init_interface();
                }
                RS485._global.flag_enable = ENABLE_OFF;
                break;    
             //###########################################
             case ENABLE_INIT_PACKER_DEFAULT:
                if(hw_id == HW_ID_INTERFACE)
                {  
                   RS485._flash.cs_Filter_option = 30; /*300 ms pulse width*/
                   // 242= 00F2 = 0000 0000 1111 0010
                   // shakehands, high level input trigger. 
                   // positive pulse output. 
                   // no delay between releasing material and sending signals
                   // ignore feed request if weighter is not ready
                   // wait for packer to initialize handshake
                   // no multi-feed.
                   // no delay (0 *200ms)                    
                   RS485._flash.target_weight = 0xD2;  // negative input pulse, positive output pulse
                   Init_interface();
                }
                RS485._global.flag_enable = ENABLE_OFF;
                break; 
             //###########################################
             case ENABLE_PACKER_RELEASE:
                if(hw_id == HW_ID_INTERFACE)                   /* interface board */
                {
                   if(!Packer_Is_Busy())
                   {   
                       release_signal_not_sent = Tell_Packer_Release_Done(); 
                       //if(!release_signal_not_sent)
                       RS485._global.flag_enable = ENABLE_OFF;
                   }
                }
                else  
                   RS485._global.flag_enable = ENABLE_OFF;             
                break;                
             //###########################################
#if 0
             case ENABLE_WEIGHER_READY:
                if(hw_id == HW_ID_INTERFACE)                   /* interface board */
                {
                      Tell_Packer_Weigher_Rdy();
                }
                RS485._global.flag_enable = ENABLE_OFF;             
                break; 
             //###########################################
             case ENABLE_FORCE_RELEASE:
                if(hw_id == HW_ID_INTERFACE)                   /* interface board */
                {
                      Tell_Packer_Force_Release();
                }
                RS485._global.flag_enable = ENABLE_OFF;             
                break;
#endif    
             //########################################### 
             case ENABLE_LOAD_MFG_CAL:
                if(hw_id == HW_ID_WEIGHT) 
                {  
                   if(load_mfg_calibration() == PASS)
                      RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
                }    
                RS485._global.flag_enable = ENABLE_OFF;
                break;
             //########################################### 
             case ENABLE_SAVE_MFG_CAL:
                if(hw_id == HW_ID_WEIGHT) 
                {   if(save_mfg_calibration()==PASS)
                        numOfMfgDataToBePgm = sizeof(S_ROM);                  
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
       // MM_ADD_MATERIAL, let machine finish this cycle before
       // shutting it down.
       // When machine(motor/magnet action is shut down, weight is 
       // repeatedly checked.
       /************************************************************/              
       if(RS485._global.flag_enable == ENABLE_OFF){        
          RS485._global.flag_goon = 0; //clear goon symbol                
          if(hw_id == HW_ID_WEIGHT)    // board has A/D on it.
          {   
             clear_weight_record();
             while(WSM_Flag != MM_ADD_MATERIAL)
             {
                Feed_Watchdog();
                motor_magnet_action();    // Complete this cycle before shutting down machine
             }
             /* if we received a release command, or material is being released, complete it! */
             while((RS485._global.flag_release) || (RSM_Flag != RM_READY_RELEASE))
             {
                Feed_Watchdog();
                release_material();      // Complete this cycle before shutting down machine            
             }
             
             CS5532_PoiseWeight();        
             CS5532_Poise2Result();
          } 
          RS485._global.flag_goon = 0; //clear goon symbol                                                                     
          continue;      
       }
                                                                 
       /************************************************************/
       // If you can reach here, let's say machine is running in 
       // production automatic mode. Condition:flag_enable=ENABLE_ON.
       // Check release command or other actions from 
       // flag will be cleared in release_material() subroutine.
       // Skip add/release actions if board type is not WEIGHT
       /************************************************************/       
       if(hw_id != HW_ID_WEIGHT) 
          continue; 
          
       if(RS485._global.flag_release)
       { 
          /********************************************************/
          // Release material in lower bucket while in parallel with  
          // adding material to upper bucket. Weight is invalidated 
          // in release_material(). 
          /********************************************************/          
          release_material(); 	  
          magnet_add_material();  	  
          continue;
       }
       /***********************************************************/
       // Test if flag_enable is still on (may be reset by ISR)  
       /***********************************************************/
       else if(RS485._global.flag_enable == ENABLE_ON)
       {
    	    motor_magnet_action();
    	    continue;
       }              
   }
}
                                 