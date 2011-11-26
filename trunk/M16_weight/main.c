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

/*********************************************************************************************
*                                     GLOBAL VARIABLES
*********************************************************************************************/
NODE_CONFIG RS485;                                /* structure of node */
OS_SCHEDULER os_sched;

u8 myaddr;                                        /* rs485 addr of current node */
u8 hw_id;                                         /* hardwired board id (PORTC[7:6]) */
bit freq_is_50HZ;                                 /* freq of AC power supply */
u16 release_times;                                /* how many times node has been released */
u8 flag_turn_off_watchdog;

/* dynamic magnet amplitude adjustment */
#define MAX_AMP_WEIGHT_RECORDS 25                  /* don't exceed 32 */
bit run_time_adj_done; 
u16 amp_weight[MAX_AMP_WEIGHT_RECORDS];
u16 last_weight, current_weight, weight_gap;     /* initial weight before adding material*/  
u8  local_magnet_amp;

#define NOT_AVAILABLE 0xFF
                                                   
/* packer interface prototypes */    
extern u8 Packer_Is_Busy();
extern void Init_interface(); 
extern void Tell_Packer_Weigher_Rdy();
extern u8 Tell_Packer_Release_Done();   
extern void Tell_Packer_Force_Release();

/*****************************************************************************/                                            
#define ENABLE_OFF              0    // stop machine.
#define ENABLE_START_MACHINE    1    // for mannual manipulation. 
#define ENABLE_INIT_AD          2    // initialize AD
#define ENABLE_TURNS            3    // rotate upper motor
#define ENABLE_TURNW            4    // rotate lower motor
#define ENABLE_VIBRATE          5    // start vibrator  
#define ENABLE_RESET_WEIGHT     7    // reset current weight to 0     
#define ENABLE_INIT_PACKER      8    // initialize packer interface
#define ENABLE_INIT_PACKER_DEFAULT 9 // default setting, for debug only
#define ENABLE_PACKER_RELEASE   10   // send release signal to packer 
#define ENABLE_WEIGHER_READY    11   // send weigher ready signal to packer
#define ENABLE_FORCE_RELEASE    12   // send signal to indicate "force release"
#define ENABLE_ON               80   // machine is fully running now
/*****************************************************************************/

/*****************************************************************************/
//                     Timer 2 interrupt service routine
// Timer 2 overflow interrupt service routine.
// Generate 10ms interrupt.
// Support up to 8 (0-7) timer services. timerlen[] store counters for different 
// timer tasks.  Timer task 7 is reserved for OS timer tick. 
/*****************************************************************************/
#define OS_TIMER_TICK 7 
#define PACKER_TIMER 6
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
  u8 i; 
  // generate 10ms timer interrupt periodically. 
  TIFR |= (1<<TOV2);                            // clear overflow flag
  TCNT2=0xB8;                                   // 7.2KHZ input,interrupt frequency is set to 100HZ (10ms timer)
    
  // handle tasks  
  if(os_sched.status)                           // if status is not zero, there is at least one task running
  {   
      for(i=0; i<8; i++)                        // check task 0-7
      {
         if(os_sched.status & ((u8)1<<i))       // tasks not handled yet
         {
            if(os_sched.timerlen[i])            // Timer is ongoing
              os_sched.timerlen[i]--;
            else
            {  
               switch(i)
               {
                 case 3:                        // toggle OR PORTB.4, weigher ready signal
                   PORTB.4 = ~PORTB.4;
                   break;
                 case 4:                        // toggle OF PORTB.5, release done signal
                   PORTB.5 = ~PORTB.5; 
                   PORTB.7 = ~PORTB.7;
                   break;
                 case 5:                        // toggle OE PORTB.6, force release signal
                   PORTB.6 = ~PORTB.6;
                   break;
                 default:
                   break;            
               } 
               os_sched.status &= ~((u8)1<<i);  // clear status bit to indicate task completion
            } 
         }
         else
            continue;
      } /*for*/    
  } 
  else
     TIMSK &= 0xBF;                          /* disable TMR2 overflow interrupt.*/ 
        
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
   if((task_id > 7)||(!timer_len))             /* support up to 8 timer tasks, 0~7 */
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

void kill_timer(u8 task_id)
{
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
       static u16 tick = 0; 
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
#define ERR_DATA_FAIL 1
#define STATUS_OK 0
u8 reset_weight()
{    
   u8 status;
   
   status = STATUS_OK;
   
   /* wait max 0.5 sec for valid data */      
   kill_timer(0);
   kick_off_timer(0, 50);
   /* get current A/D readings */   
   while(CS5532_PoiseWeight() != DATA_VALID)
   {
      if(DELAY1_END) /* time out */ 
      {
         status = ERR_DATA_FAIL;
         break;
      }
   }   
   kill_timer(0);

   if(status == STATUS_OK)
      RS485._flash.cs_zero = RS485._global.cs_mtrl;
            
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
    RS485._magnet.rsm_state = RSM_Flag; 
    
    // initialize: weight is zero after release.
    last_weight = 0; 
    current_weight = 0;
    weight_gap = RS485._flash.target_weight; 
    
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
             kick_off_timer(0, RS485._flash.open_w);         
          }
          RSM_Flag = RM_WAIT_OPEN_W;     
       }
       return;
    }
    /************************************************************/      
    //wait till delay eclapse (material released to pool)
    //then start motor to close the bucket.
    /************************************************************/      
    if(RSM_Flag == RM_WAIT_OPEN_W){      
       if(DELAY1_END){                        
          Motor_Driver('W',HALF_CYCLE_CLOSE_WITH_SENSOR,POWER_OFF_AFTER_ROTATION);
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
             kick_off_timer(0, RS485._flash.delay_w);
          }
          RSM_Flag = RM_WAIT_DELAY_W;     
       }
       return;
    }
    /************************************************************/      
    // After delay, state machine goes back to initial state. 
    // Clear release flag.
    // RS485._flash.magnet_freq is now used to set weight reset freq  
    /************************************************************/      
    if(RSM_Flag == RM_WAIT_DELAY_W){
       if(DELAY1_END)
       {
          RS485._magnet.half_period = release_times;  
          if(RS485._flash.magnet_freq)        
          {             
             if ((release_times) >= RS485._flash.magnet_freq)       /* time to reset weight */
             { 
                release_times = 0;                                  /* reset counter */ 
                retry_cnt = 0;
                kick_off_timer(0, 100);                             /* start timer service to wait for weight to be stable */
                RSM_Flag = RM_DELAY_BE4_RESET;                      /* move to next state */
                return;
             }
             else                                                   /* not a reset cycle */
                release_times++; 
          } 
          
          /* autoclear disabled or not this cycle */
          RS485._global.flag_release = 0;                           /* release completes */
          RSM_Flag = RM_READY_RELEASE;
          if(RS485._global.flag_enable == ENABLE_ON)
             RS485._global.flag_goon = 1;                           /* automatically add material again after release*/ 
       }
       
       return;
    }
    /************************************************************/      
    // If weight needs to be reset, wait 2s before reset weight
    /************************************************************/ 
    if(RSM_Flag == RM_DELAY_BE4_RESET){
       if(DELAY1_END)
       {
         retry_cnt++; 
         if((reset_weight() == STATUS_OK) || (retry_cnt > 6))       /* weight was reset successfully */
         {                    
            RS485._global.flag_release = 0;                         /* release completes */
            if(RS485._global.flag_enable == ENABLE_ON)
                RS485._global.flag_goon = 1;                        /* automatically add material again after release*/            
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
void auto_amp_adj(u16 weight_delta)
{
    u8 new_amp, index, i, max_amp;
    
    index = local_magnet_amp >> 2;  
    for(new_amp=0,i=1; i<MAX_AMP_WEIGHT_RECORDS; i++)
    {    
        if(amp_weight[i] == NOT_AVAILABLE)
            continue;
        else if(amp_weight[i] < weight_delta)     
            new_amp = i;                      
    }
    if(amp_weight[new_amp+1] == NOT_AVAILABLE)
        new_amp++;

    max_amp = RS485._flash.magnet_amp+20;
    
    if(new_amp*4 > max_amp)
        local_magnet_amp = max_amp; 
    else
       local_magnet_amp=new_amp<<2;

} 

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

//Initialize state of Motor/Magnet/Weight State Machine.
static u8  WSM_Flag = MM_ADD_MATERIAL; 
u8 magnet_add_material()
{    
    u16 temp;
    /**************************************************************/
    //  start Magnet
    /**************************************************************/          
     // wait from magnet to feed material
     if(WSM_Flag == MM_ADD_MATERIAL){
        local_magnet_amp = RS485._flash.magnet_amp;   			              
        if(RS485._flash.target_weight)
            auto_amp_adj(weight_gap); 
        /* log actual amp to hw_status register for monitor */
        RS485._global.hw_status = local_magnet_amp;  

        temp = RS485._flash.target_weight;
        /* stop adding material when current weight exceeds target weight */
        if((temp==0)||(temp > current_weight))
            E_Magnet_Driver(RS485._flash.magnet_time, local_magnet_amp); 
            
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
            kick_off_timer(1, RS485._flash.delay_f);   
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
   RS485._magnet.wsm_state = WSM_Flag;
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
    RS485._magnet.wsm_state = WSM_Flag;
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
     if(WSM_Flag == MM_PASS_MATERIAL)
     {  
        //need to check whether lower bucket is open
        //don't open upper bucket until lower bucket is closed
        if(!Motor_Is_Running())
        {                                   
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
     if(WSM_Flag == MM_WAIT_OPEN)
     {   
        if(!Motor_Is_Running())
        {        
           if(RS485._flash.open_s > 0)
           {
              kick_off_timer(1, RS485._flash.open_s);  /* start timer service */
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
     if(WSM_Flag == MM_WAIT_OPEN_S)
     {  
        if(DELAY2_END)
        {
           Motor_Driver('S',HALF_CYCLE_CLOSE_WITH_SENSOR, POWER_OFF_AFTER_ROTATION); 
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
              kick_off_timer(1, RS485._flash.delay_s);    /* start timer service */
           }
           WSM_Flag = MM_WAIT_DELAYEND;
        }
        return;
     }                          
    /**************************************************************/      
    //OK, wait till delay ends                                      
    /**************************************************************/      
     if(WSM_Flag ==  MM_WAIT_DELAYEND){        
        if(DELAY2_END)
           WSM_Flag = MM_CHECK_WEIGHT;                 
        return;
     }  
    /**************************************************************/           
    // Ready to measure weight or do something else. 		                          
    /**************************************************************/           
     if(WSM_Flag ==  MM_CHECK_WEIGHT)
     {     
        // Set flag to inform master board new weight is being calculated
        RS485._global.flag_goon = 0;
        RS485._global.Mtrl_Weight_gram = INVALID_DATA ;  
        WSM_Flag = MM_ADD_MATERIAL;
                        
        // Idle here if no command from master board asking me to go on.
        // Combination algorithm will decide whether magnet should 
        // give more material. 2 conditions to break out of this loop:  
        // flag_goon = 1 or flag_enable not equal to ENABLE_ON any more.
        // These 2 registers may be changed by RS485 masters through UART. 
        // Add a third branch so that flag release command can also break the infinite loop. 11/13/2009
        run_time_adj_done = FALSE;
        while(RS485._global.flag_goon == 0)
        {
           MYDEAD(0x2000);            
           
           // test conditions of breaking out of this "while" loop.
           if((RS485._global.flag_enable != ENABLE_ON) || (RS485._global.flag_release))
           {   
               weight_gap = RS485._flash.target_weight; 
               // re-add material. last amp forecast is out-of-date
               if((WSM_Flag == MM_PASS_MATERIAL) && (RS485._flash.target_weight))
               {   WSM_Flag = MM_ADD_MATERIAL;
               }
               return; 
           }     
           if(hw_id == HW_ID_WEIGHT)                                      // if board has A/D on it, update weight
           {              
               /* polling when no valid weight is available */
               if(RS485._global.Mtrl_Weight_gram > MAX_VALID_DATA)
               {    
                    CS5532_PoiseWeight();  
                    CS5532_Poise2Result();
               }
              //############################################################
              //  Run time adjust magnet amplitude and working time
              //############################################################              
              if((RS485._flash.target_weight > 0) && (!run_time_adj_done))
              { 
                 if(RS485._global.cs_mtrl < MAX_VALID_DATA)
                 {                                                                  
                    /* Valid weight, */
                    /* log last weight_vs_amp record */
                    last_weight = current_weight;
                    current_weight = RS485._global.Mtrl_Weight_gram;
                    index = local_magnet_amp>>2;
                    if(current_weight>last_weight) 
                        amp_weight[index] = current_weight - last_weight; 
                    else
                        amp_weight[index] = 0;
                    
                    /* calculate gap between current weight and target weight */    
                    if(RS485._flash.target_weight > current_weight)
                        weight_gap = RS485._flash.target_weight - current_weight;
                    else
                        weight_gap = 0;                    

                    run_time_adj_done = TRUE; 
                 } /* RS485._global.cs_mtrl < MAX_VALID_DATA*/   
              }
              //############################################################
              //  Run time adjust magnet amplitude and working time
              //############################################################                 
           }           
           // and we can start magnet to feed material to upper bucket.
           if((run_time_adj_done) || (!RS485._flash.target_weight))
               magnet_add_material();
           Feed_Watchdog(); // feed watch dog in case that we exceed the timeout limit. 
        } /* while */    
        
        // Get command from masterboard/PC to go on (add material)  
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
     u8 i;
     
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
     // Backup cs_zero, user may reset weight to zero if there is zero point shift.
     // In that case, cs_poise needs to be adjusted accordingly.
     // We will use the delta between cs_zero and old_cs_zero to do the adjustment.
     RS485._global.old_cs_zero = RS485._flash.cs_zero; 
 
     flag_turn_off_watchdog = 0; //clear flag 
     os_sched.status = 0;  
     freq_is_50HZ = TRUE; 
     last_weight = 0; 
     current_weight = 0;
     for(i=0; i<MAX_AMP_WEIGHT_RECORDS;i++)
         amp_weight[i] = NOT_AVAILABLE;      
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
   RS485._flash.addr = 0xFF; 
   // define tolerance for filtering.  
   RS485._flash.cs_gain_wordrate = 8;      
   // Intialize RS485 baudrate to 115200 (0x0)
   RS485._flash.baudrate = 0x0;
   // Intialize filter option: 0x11: 0x10-> option 1, average + forecast
   // 0x01-> skip next 1*8 samples when data is dected unstable.
   RS485._flash.cs_Filter_option = 0x12;    
   // Board type
   //RS485._flash.board = BOARD_TYPE_VIBRATE;
   RS485._flash.board = BOARD_TYPE_WEIGHT;
   // Weight Calibration settings
   RS485._flash.target_weight = 0;
   RS485._flash.cs_zero = 0x3EDA;
   RS485._flash.cs_poise[0] = 0x43DB;
   RS485._flash.Poise_Weight_gram[0] = 50;
   RS485._flash.cs_poise[1] = 0x48F9;
   RS485._flash.Poise_Weight_gram[1] = 100;
   RS485._flash.cs_poise[2] = 0x531E;
   RS485._flash.Poise_Weight_gram[2] = 200;
   RS485._flash.cs_poise[3] = 0x5D4C;
   RS485._flash.Poise_Weight_gram[3] = 300;
   RS485._flash.cs_poise[4] = 0x719F;
   RS485._flash.Poise_Weight_gram[4] = 500;
   // Motor & Magnet settings
   //RS485._flash.magnet_freq = 9;
   RS485._flash.magnet_amp = 50; 
   RS485._flash.magnet_time = 10; 
   RS485._flash.motor_speed = 1;
   RS485._flash.delay_f = 10;
   RS485._flash.delay_w = 10;
   RS485._flash.delay_s = 10;
   RS485._flash.open_s  = 10;
   RS485._flash.open_w  = 10; 

#ifdef _BOARD_TYPE_IS_VIBRATE_15
   RS485._flash.addr = 0xf;
   RS485._flash.board = BOARD_TYPE_VIBRATE;
#endif

#ifdef _BOARD_TYPE_IS_VIBRATE_11
   RS485._flash.addr = 0xB;
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
         temp = 3;                                          // 4: 6.25 Sps, 3: 12.5 sps
         temp = (temp << 11) | 0x400;                       // unipolar
         if(freq_is_50HZ)
            ad_config_reg_msw = 0x08;                        // xianghua 0x8, 0x0
         else
            ad_config_reg_msw = 0x00;         
         CS5532_Config(ad_config_reg_msw,temp);
         
         offset_data[0] = 0x08;                             // mannually set offset/gain regs.
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
    
    if(os_sched.status & 0x80)                               // timer hasn't got timeout yet
       return freq_measured;
    else if(!timer_enabled)                                  // There was no timer task enabled
    {
       if(!Magnet_Is_Running())                              // if Magnet is not running
       {
          kick_off_timer(OS_TIMER_TICK, 50);                // kick off 50*10ms timer service
          E_Magnet_Driver(0,0);                                // measure AC power frequency. 
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
   u8 i, AC_freq_measured; 
   u16 loopcnt; 
   bit ad_initialized_to_50HZ;    
   bit packer_interface_initialized;  
   bit packer_timer_kicked;     
   u8  release_signal_not_sent; 
    
   packer_interface_initialized = FALSE;
   release_signal_not_sent = FALSE; 
   packer_timer_kicked = FALSE; 
   /********************************************************************/
   //               System Initialization
   /********************************************************************/             
   Init_PORT();                                              // Initialize MEGA16 PORT
   Init_Timers();                                            // Timer (Interrupt) initialization
   vInitEeprom();                                            // Initialize EEPROM   
                                                             // Initialize System global variables (read from EEPROM)
   bReadDataFromEeprom(EEPROM_RS485ADDR,(u8*)&RS485._flash, sizeof(S_FLASH));   
   Init_Vars();                                              // Must be placed after "bReadDataFromEeprom()"
                                                             // because of RS485._flash.cs_zero.   
   query_fw_version();                                       // FW rev, LSB of "cs_sys_gain_cal_data".    
   bReadDataFromEeprom_c(EEPROM_BOOTFLAG_ADDR, &i);
   if(i != 0xff)                                             // Clear bootloader firmware upgrade flag in EEPROM.
   {  bWriteData2Eeprom_c(EEPROM_BOOTFLAG_ADDR, 0xff);
      sleepms(10);
   }      

   if(RS485._flash.rom_para_valid != ROM_DATA_VALID_FLAG)    // If raw EEPROM (raw board with no EEPROM data programmed) 
       Init_EEPROM_Para();                                   // initialize EEPROM data
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
   //if RS485 addr is invalid(new board), Use default addr: 0x30
   if((RS485._flash.addr == 0) || (RS485._flash.addr > MAX_RS485_ADDR))
        RS485._flash.addr = MAX_RS485_ADDR;        
   myaddr = RS485._flash.addr;         
   
   kick_off_timer(OS_TIMER_TICK, 1); // kick off 10ms OS timer tick,
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
       if(RS485._global.flag_reset >0 )
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
            {  kick_off_timer(PACKER_TIMER,1000);
               packer_timer_kicked = TRUE;
            }
            else 
            {   if(os_sched.status &((u8)1<<PACKER_TIMER))
                    RS485._global.diag_status2 &=0xFD;        /* clear packer diagostic flag */
                else                                          /* timeout */
                    RS485._global.diag_status2 |=0x2;         /* set packer diagostic flag */
            } 
         }
         else  /* packer response available */
         {
             kill_timer(PACKER_TIMER);
             packer_timer_kicked = FALSE;
             RS485._global.diag_status2 &=0xFD;        /* clear packer diagostic flag */             
         }
         
         /* if release signal not sent yet */
         if(release_signal_not_sent)
         {  release_signal_not_sent = Tell_Packer_Release_Done(); 
            /* if signal has been sent, tell "flag enable" not to do again */
            if(!release_signal_not_sent)
               RS485._global.flag_enable = ENABLE_OFF;                      
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
       // or bad mechanism. 
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
       // Test Mode(test_mode_reg2): when set:
       // Bit[7:6] = 11, Bit[5:0]as new RS485 addr.
       // Bit[7:6] = 10, Bit[5:0]as new Board Type.
       // See "define.h" for details.                                 
       /************************************************************/       
       if(CHANGE_RS485_ADDR)
       {   
            RS485._flash.addr = RS485._global.test_mode_reg2 & 0x1f;        
            RS485._flash.addr_copy1 = RS485._flash.addr;
            RS485._flash.addr_copy2 = RS485._flash.addr;            
            myaddr = RS485._flash.addr;       
            if(hw_id == HW_ID_WEIGHT)
            {   RS485._flash.board = BOARD_TYPE_WEIGHT;
            }
            else
            {   RS485._flash.board = BOARD_TYPE_VIBRATE;
            }
                                                       
            RS485._flash.board_copy1 = RS485._flash.board;
            RS485._flash.board_copy2 = RS485._flash.board;              
            RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
            RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;            
            RS485._global.test_mode_reg2=0;
            continue; 
       }
       if(CHANGE_BOARD_TYPE)
       { 
         RS485._global.test_mode_reg2 &= 0x3F; 
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
       /************************************************************/
       // Invalidate last weight if master asks me to add material. 
       // "Mtrl_Weight_gram" should be invalided before
       // new weight is available.
       /************************************************************/       
       if(RS485._global.flag_goon)
           RS485._global.Mtrl_Weight_gram = INVALID_DATA;
       
       /************************************************************/
       // Magnet/Motor actions. 
       // State range ENABLE_OFF< WSM_Flag <ENABLE_ON: mannual operation. 
       // Turn off watchdog in manual operation.
       /************************************************************/       
       if((RS485._global.flag_enable > ENABLE_OFF)&&(RS485._global.flag_enable < ENABLE_ON))
       {  
          Turn_Off_Watchdog();
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
                         /* adjust poise because cs_zero has changed */
                         CS5532_Poise2Result();
                         if(RS485._global.Mtrl_Weight_gram < MAX_VALID_DATA)
                         {
                            /* save calibration data to EEPROM */
                            RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);   
                            RS485._global.test_mode_reg1 |= EN_EEPROM_PROG_BIT;
                         }
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
                   {   release_signal_not_sent = Tell_Packer_Release_Done(); 
                       if(!release_signal_not_sent)
                           RS485._global.flag_enable = ENABLE_OFF;                   
                   }
                }
                else  
                   RS485._global.flag_enable = ENABLE_OFF;             
                break;                
             //###########################################
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
             //###########################################
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
          //RS485._global.flag_goon = 0; //clear goon symbol                
          if(hw_id == HW_ID_WEIGHT)    // board has A/D on it.
          {   
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
       /************************************************************/       
       if(hw_id != HW_ID_WEIGHT) 
          continue; /* skip the operation below if board is not type_weight*/
          
       if(RS485._global.flag_release)
       { 
          //invalidate weight when doing material release actions.
          RS485._global.Mtrl_Weight_gram = INVALID_DATA; 
          release_material(); 	  

          //add material during idle/wait period of release state machine
          // to improve system speed. The 2 subrountines are state machines and 
          // they can run in parallel.
          magnet_add_material();  	  
          continue;
       }else if(RS485._global.flag_enable == ENABLE_ON){
          // originally there was a bug, when master sends "flag_enable" commands 
          // less than ENABLE_ON very fast, after executing 
          // if((RS485._global.flag_enable > ENABLE_OFF)&&(RS485._global.flag_enable < ENABLE_ON)) 
          // and before if(RS485._global.flag_enable == ENABLE_OFF), RS485._global.flag_enable may 
          // be set again by interrupt subroutine. It may go here. So we add a test here. 
          
    	  // weight measurement is embedded at the end of motor_magnet_action().
    	  // if no go on command received, code won't break out of motor_magnet_action().
    	  // motor_magnet_action() will continuously output weight values.
    	  motor_magnet_action();
    	  continue;
       }              
   }
}
                                 