/******************************************************************************************************/
//  Interface with packing machines
//  OR1(PORTB.4): OUT_READY, output to indicate at least one combination is available.
//  OF1(PORTB.5): OUT_Feed_Done, output to request packing machine to pack
//  OE1(PORTB.6): force release output to indicate current release is force release. 
//  IF1(PORTD.2): IN_Feed_Request. Request weigher to feed 
//  Note: PORTD.2 is external interrupt input 0               
//  

// ***************************** USAGE ******************************
// Init_interface() should be re-called everytime when interface settings are changed in GUI. 

// *********Shakehands mode:*************
// step 1: On finding available combinations, master board output OR1 pulse 
// step 2: Packer request weigher to feed
// step 3: master board releases material and output OF1 pulse. 
                                                                
// IF (combinations available)
// {    Tell_Packer_Weigher_Rdy();
//   While(Packer_Is_Busy()); 
//   Release_Material();
//   Tell_Packer_Release_Done();
// }
// CHECK NEW COMBINATIONS
//
                           
// ********Non shakehands mode:**********
// step 1: On finding available combinations, master board output OR1 pulse 
// step 2: master board releases material and output OF1 pulse. 
// IF (combinations available)
// {    Tell_Packer_Weigher_Rdy();
//   Release_Material();
//   Tell_Packer_Release_Done();
// }
// CHECK NEW COMBINATIONS.

   
// *************************************************************************
// 1 10 10 0 1 0
// 1 11 10 0 1 0 
// 
// PC interface (use command start reg and offset_low_limit only): 
//    Mode:     BIT[7] 1: with shakehands    0: No shakehands 
//
//    IF1/IF2:  BIT[6:5] 00: low level trigger
//                       01: rising edge trigger 
//                       10: falling edge trigger
//                       11: high level trigger
//
//    OR1/OF1   BIT[4:3] 00: low level true
//                       01: negative pulse true
//                       10: positvie pulse true
//    FR/OF2             11: high level true
//
//    DLY:      BIT[2]   1:  add delay for material to drop into packer 
//                           before sending release_done signal to packer.
//                       0:  no delay
//
//    Asyn_IF:  BIT[1]   1:  Save feed request if weigher is not ready. 
//                           Packer doesn't need to send IF request again.
//                       0:  Ingore feed request if weigher is not ready.
//                           Packer has to request again later.
//
//    Init:     BIT[0]   0:  wait for packer to intiate handshake. 
//                       1:  weigher intiates handshake (first send)
//
//    MULTI_FD: BIT[10:8]:   continuously feed multi (1~8) times every request from packer.
//    OFDLY:    BIT[15:11]:  delay, unity 200ms
//  
//    
/******************************************************************************************************/

#include <mega16.h>
#include "define.h"

#define MAX_SVS_NUM           8 
  
#define NEED_HANDSHAKE        (packer_config & 0x0080)
#define ASYN_IF_MASK          0x0002
#define INIT_MASK             0x0001 
#define SIGNAL_DELAY_MASK     0x0004
#define INTF_MODE_SHAKEHANDS  (CONFIG_REG & 0x80)
#define CONFIG_REG            packer_config
#define CFG_MEM_PACK_REQ      (CONFIG_REG & ASYN_IF_MASK)

#define PACKER_IF_MASK        0x0060
#define IF_CFG_LOW            0x0000
#define IF_CFG_RISING_EDGE    0x0020
#define IF_CFG_FALLING_EDGE   0x0040
#define IF_CFG_HIGH           0x0060
#define IF_CFG                (CONFIG_REG & PACKER_IF_MASK)

#define PACKER_OF_MASK        0x0018
#define OF_CFG_LOW            0x0000
#define OF_CFG_NEG_PULSE      0x0008
#define OF_CFG_POS_PULSE      0x0010
#define OF_CFG_HIGH           0x0018
#define OF_CFG                (CONFIG_REG & PACKER_OF_MASK)

#define PIN_FEED_PACKER       PORTB.5
#define PIN_RFU               PORTB.7
#define PIN_PACKER_REQ        PIND.2

#define IF_PIN_HIGH           (PIN_PACKER_REQ)
#define IF_PIN_LOW            (!PIN_PACKER_REQ)

#define TASK_OF               4
 
#define PACKER_BUSY           0xFF
#define PACKER_READY          0x0

#define LOW_LEVEL             0
#define HIGH_LEVEL            1

#define PENDING_REQ           3
#define INT_RISING_EDGE       2
#define INT_FALLING_EDGE      1
#define NO_PENDING_REQ        0

#define RISING_EDGE_OCCURED   (Intf.pack_reqs_pending & INT_RISING_EDGE)
#define FALLING_EDGE_OCCURED  (Intf.pack_reqs_pending & INT_FALLING_EDGE)

#define LEVEL_TEST_WAIT_TIME  150   /* 150 ms */

/******************************************************************************************************/
//                                  Global variables
/******************************************************************************************************/
extern void kick_off_timer(u8 task_id, u16 timer_len);
typedef struct {
   volatile u8 pack_reqs_pending;              /* flag to indicate whether there is any 
                                                   pending reuqests from packer */                              
   volatile u8 weigher_is_ready;               /* weigher is available */
   volatile u8 multi_feed_inprogress; 
   volatile u8 max_feed_counter;               /* recorde how many times feed has been done */
   volatile u8 feed_counter;                   /* recorde how many times feed has been done */
   
}INTERFACE;
INTERFACE Intf;                                /* packer interface global variables */
u16 packer_config;                             /* configuration of packer interface, passed from PC */ 
u16 interface_pulse_width;                    
     
/******************************************************************************************************/
//                 de-assert OF (output signal -- feeding is done) signal 
/******************************************************************************************************/
void deassert_of()
{
   switch(OF_CFG)
   {
      case OF_CFG_LOW: 
          PIN_FEED_PACKER = HIGH_LEVEL; 
          PIN_RFU = HIGH_LEVEL;                  
          break;    
      case OF_CFG_NEG_PULSE:
          kill_timer(TASK_OF);
          PIN_FEED_PACKER = HIGH_LEVEL; 
          PIN_RFU = HIGH_LEVEL; 
          break;
      case OF_CFG_POS_PULSE:                                         
          kill_timer(TASK_OF);
          PIN_FEED_PACKER = LOW_LEVEL;
          PIN_RFU = LOW_LEVEL;
          break;                      
      case OF_CFG_HIGH:
          PIN_FEED_PACKER = LOW_LEVEL; 
          PIN_RFU = LOW_LEVEL; 
          break;
      default: 
          break;         
   }
}

/******************************************************************************************************/
// External Interrupt 0 service routine: feed request from packer
// Interrupt trigger type: Edge trigger.  
// Flag "Intf.pack_reqs_pending" to indicate whether packer is requesting feeding.
//      bit 1 = 1: rising edge detected; bit 0 = 1 falling edge detected; 
// Counter "RS485._global.packer_intr_cnt" is used to monitor occurance of packer request signal.
/******************************************************************************************************/
interrupt [EXT_INT0] void ext_int0_isr(void)
{  
   RS485._global.packer_intr_cnt++;                /* interrupt counter for debug monitor */
   GIFR |= 0x40;                                   /* clear interrupt flag */
   if(hw_id != HW_ID_INTERFACE)                    /* board is not interface board */
   {
     GICR &= 0xBF;                                 /* clear bit 6 to disable int 0 */
     return; 
   } 
   /***********************************************************************/
   /* If packer request has been de-asserted, then last OF signal has been 
      acknowledged by packer, then we should deassert OF signal. */
   if(IF_PIN_HIGH)                                 /* risking edge interrupt */
   {   if((IF_CFG == IF_CFG_LOW) || 
          (IF_CFG == IF_CFG_RISING_EDGE))
           deassert_of();
   }
   else                                            /* falling edge interrupt */
   {   if((IF_CFG == IF_CFG_HIGH) || 
          (IF_CFG == IF_CFG_FALLING_EDGE) )
           deassert_of();
   }
   /***********************************************************************/
   /* If weigher is configured to memorized any packer requests when not 
      ready, or it is ready, set flag to indicate a request from packer 
      is pending. Flag is cleared outside of this ISR */
   if((CFG_MEM_PACK_REQ)||(Intf.weigher_is_ready))
   {          
       if(IF_PIN_HIGH)                               /* Current input level is high */
         Intf.pack_reqs_pending |= INT_RISING_EDGE;  /* Set bit 1 to indicate rising edge */ 
       else
         Intf.pack_reqs_pending |= INT_FALLING_EDGE; /* Set bit 0 to indicate falling edge */ 
   }
   /***********************************************************************/   
   Intf.feed_counter = 0;                         /* reset feed counter */
}

/******************************************************************************************************/
// Initialize interface pins
// This procedure should be called after PC download interface settings to masterboard 
/******************************************************************************************************/
void Init_interface()
{      
   /* Create a flag to indicate initialization is poweron initialization,
      or user initialization (configuration from user interface) */
   static u8 user_initialization;
   
   /* Get configuration data from IO registers */    
   packer_config = RS485._flash.target_weight; 
   /* max pulse width 255*10 ms = 2.55s, minimum pulse width 10ms */
   interface_pulse_width = (u16)RS485._flash.cs_Filter_option ;     
   if(interface_pulse_width < 1)
      interface_pulse_width = 1;
  
   if(!INTF_MODE_SHAKEHANDS)
   {
      DDRD.2 = 0;                   /* PORTD.2 as input (int0)*/   
      DDRB |= 0xF0;                 /* set PORTB[7:4] as output*/
      PORTB |= 0xF0;                /* output high to shut down interface */          
      return;
   }
   /***********************************************************************/     
   /* configure input (IF), enable interrupt to monitor state change */
   if(!user_initialization)   /* first time (poweron) initialization */
   {  /* Settings for IF1, use External interrupt 0*/
      DDRD.2 = 0;             /* set PORTD.2 (INT0)  as input */
      MCUCR &=0xFB;           /* Interrupt triggered when PORTD.2 toggles */
      MCUCR |=0x1; 
      GIFR &= 0xBF;           /* Clear interupt flag (INTF0: bit 6) */
      GICR |= 0x40;           /* Enable INT0 interrupt */ 
      
      /* Initialize packer flag for the first time, for non-powerup 
         initializations, use last state */
      Intf.pack_reqs_pending = NO_PENDING_REQ;      
      user_initialization = TRUE;                            
   }
   /***********************************************************************/        
   /* Configure output signals */
   DDRB |= 0xF0;                    /*configure pins as output */             
   switch(OF_CFG)            
   {
      case OF_CFG_LOW:
      case OF_CFG_NEG_PULSE:
           PORTB |= 0xF0;           /* output high */
           break;
      case OF_CFG_POS_PULSE: 
      case OF_CFG_HIGH:
           PORTB &= 0x0F;           /* output low */
           break;
      default: 
          break;                                                            
   }
   /***********************************************************************/   
   /* Initialize interface variables */
   Intf.max_feed_counter = 1 + ((CONFIG_REG & 0x0700) >> 8); 
   Intf.feed_counter = 0;

   if(CONFIG_REG & INIT_MASK)                  /* weigher is to initialize the handshake */
      Intf.pack_reqs_pending = PENDING_REQ;    /* set pending flag so that weigher won't wait */
   
   Intf.weigher_is_ready = FALSE;              /* initialize weigher status, not ready yet */      
   Intf.multi_feed_inprogress = FALSE;         /* not a multi_feed*/
}

/******************************************************************************************************/
// return 0x0 if packing machine is ready to receive another feeding. return 0xff if busy    
// "new_request"
//    TRUE:  request from packer is level valid and weigher has not responded yet
//    FALSE: request from packer is level valid and weigher has responded already
/******************************************************************************************************/
u8 Packer_Is_Busy()
{  
    u8 ret_status;
    
    ret_status = PACKER_READY;
    
    /********************************************************************************/
    /* if in shakehands mode and no requests from packer is pending.*/
    /* variable "pack_reqs_pending" is set by ISR, it is declared as volatile type.*/
    if((INTF_MODE_SHAKEHANDS) && (!Intf.multi_feed_inprogress)) 
    {  
       ret_status = PACKER_BUSY;
       switch(IF_CFG)   
       {
       /******************************************************************/       
       case IF_CFG_LOW:
          /* Packer request is configured as low valid */           
          /* Release material only when falling edge detected (new reqeust). */
          /* edge flag is cleared when responding to packer (Tell_Packer_Release_Done) */
          if(IF_PIN_LOW)
          {  
             if(FALLING_EDGE_OCCURED)  ret_status = PACKER_READY;
             else
             {
                sleepms(LEVEL_TEST_WAIT_TIME);
                if(IF_PIN_LOW)   ret_status = PACKER_READY;
             }
          }
       break;          
       /******************************************************************/
       case IF_CFG_HIGH:
          /* Packer request is configured as high valid */             
          /* Release material only when input valid and rising edge detected (new reqeust). */
          /* edge flag is cleared when responding to packer (Tell_Packer_Release_Done) */
          if(IF_PIN_HIGH)
          {  
             if((RISING_EDGE_OCCURED))   ret_status = PACKER_READY;
             else
             {  sleepms(LEVEL_TEST_WAIT_TIME);
                if(IF_PIN_HIGH)   ret_status = PACKER_READY;           
             }
          }
          break;       
       /******************************************************************/
       case IF_CFG_RISING_EDGE:
          /* packer request is configured as rising edge valid */           
          if(RISING_EDGE_OCCURED)  ret_status = PACKER_READY; 
          break;
       /******************************************************************/
       case IF_CFG_FALLING_EDGE:
          /* packer request is configured as falling edge valid */           
          if(FALLING_EDGE_OCCURED)  ret_status = PACKER_READY; 
          break;
       /******************************************************************/
       default:
          break;
       } /* end of switch */
       /*****************************************************************************/
    } /* end of if */
           
    return ret_status;
}
 
/******************************************************************************************************/
// When combination is available, call this function to inform packer that weigher is ready to release
// material 
// Output: negative or positive pulse, 
// this signal is to connect collector bucket
/******************************************************************************************************/
#define PIN_WEIGHER_READY PORTB.4
#define TASK_OR 3
void Tell_Packer_Weigher_Rdy()
{  
   switch(OF_CFG)
   {
      case OF_CFG_LOW:                                   /* low to enable */
      case OF_CFG_NEG_PULSE:
          PIN_WEIGHER_READY = LOW_LEVEL;
          break;
      case OF_CFG_POS_PULSE:
      case OF_CFG_HIGH:                                  /* high to enable */
          PIN_WEIGHER_READY = HIGH_LEVEL;
          break;
      default: 
          break;         
   }
   Intf.weigher_is_ready = TRUE;                    /* flag to inform interrupt 0 */
   kick_off_timer(TASK_OR, interface_pulse_width);  /* start timer to generate pulse OR1. */   
}

/******************************************************************************************************/
// When material in a bucket is force released due to some reasons (for example overweight), master 
// issues command to call this subroutine to send out signal indicating current activity is a force 
// release. 
/******************************************************************************************************/
#define PIN_FORCE_RELEASE PORTB.6
#define TASK_OE 5     
void Tell_Packer_Force_Release()
{  
   switch(OF_CFG)
   {
      case OF_CFG_LOW: 
      case OF_CFG_NEG_PULSE:
          PIN_FORCE_RELEASE = LOW_LEVEL;
          break;
      case OF_CFG_POS_PULSE:
      case OF_CFG_HIGH:
          PIN_FORCE_RELEASE = HIGH_LEVEL;
          break;
      default: 
          break;         
   }

   kick_off_timer(TASK_OE, interface_pulse_width);  /* start timer to generate pulse OR1. */   
}

/******************************************************************************************************/
// Inform packer that weigher has completed release of material       
// In some applications, material is packed after multiple feeds. Therefore packer will acknowledge
// to weigher only when weigher has sent release_done signals for n times. 
// Variable "feed_counter" is used to track whether weigher needs to await acknowledge signal from 
// packer.
/******************************************************************************************************/
//  send out release_done signal to packer
/**************************************************/
void send_packer_release_signal()
{   
   switch(OF_CFG)
   {
      case OF_CFG_LOW:                            /* low to enable */      
          PIN_FEED_PACKER = LOW_LEVEL;            /* assert low until packer response */ 
          PIN_RFU = LOW_LEVEL;             
          break;
      case OF_CFG_NEG_PULSE:                      /* negative pulse to enable */
          PIN_FEED_PACKER = LOW_LEVEL;            /* assert low first */
          PIN_RFU = LOW_LEVEL;
          kick_off_timer(TASK_OF, interface_pulse_width);/* back to high when timeout*/
          break;
      case OF_CFG_POS_PULSE:                      /* high to enable */
          PIN_FEED_PACKER = HIGH_LEVEL;           /* assert high until packer response */
          PIN_RFU = HIGH_LEVEL;
          kick_off_timer(TASK_OF, interface_pulse_width);/* back to low when timeout*/
          break;
      case OF_CFG_HIGH:
          PIN_FEED_PACKER = HIGH_LEVEL;            /* assert high first */
          //PIN_RFU = LOW_LEVEL; 
          break;
      default: 
          break;         
   } 
}
/***************************************************************/
// Tell packer material is release, packer can start to pack now.
// There is a time between multi-head-weigher release material and
// material drops into packer. So we allow user to set a delay here.
// In the case of single-release-single-pack and multi-release-single-pack
// mode, it is good enough to add the delay after last release and
// before sending out release_done signal to packer.
// To avoid waiting in this subroutine,  TMR0_SVS2 status bit is
// checked to decide whether we should go into this subroutine. 
// Accordingly main.c periodically call this subroutine
// and complete remaining actions in it if delay expires.
/***************************************************************/
#define TASK_DELAY 2
#define TASK_NOT_FINISHED 0xff
#define TASK_DONE 0x0

u8 Tell_Packer_Release_Done()
{
   u16 signal_delay; 
   static u8 release_signal_not_sent_yet; 
   static u8 wait_for_signal_complete; 
      
   /************************************************************************/
   /* Check if delay (releasing material <-> sending signal) is ongoing */   
   if(os_sched.status & 0x4)                       /* Task 2(feed delay) is ongoing */
       return TASK_NOT_FINISHED;                   /* return and check again later*/           
   /************************************************************************/
   /* if OF signal has not been sent yet, send it and wait for it to complete */
   else if(release_signal_not_sent_yet)            /* delay expires, and we do have signal to sent*/
   {
       send_packer_release_signal();               /* send packer release done signal*/   
       release_signal_not_sent_yet = FALSE;        /* clear flag */
       wait_for_signal_complete = TRUE; 
       return TASK_NOT_FINISHED;                   /* signal sent to packer, return (task done!) */
   }
   
   /************************************************************************/   
   /* wait for release signal to complete */   
   if (wait_for_signal_complete)                        
   {
      if (os_sched.status & 0x10)                   /* wait for pulse width to complete */
          return TASK_NOT_FINISHED;
      else
      {
          wait_for_signal_complete = FALSE;
          RS485._global.packer_release_cnt++;               
          return TASK_DONE;     
      }
   }
     
   /************************************************************************/      
   /* send out release_done signal or kick off timer to send signal later */  
   Intf.weigher_is_ready = FALSE;                  /* we are responding and completing this cycle, so clear flag for next cycle */      
   Intf.feed_counter++;                            /* how many times weigher has feed packer in this cycle */
   if(Intf.feed_counter < Intf.max_feed_counter)   /* packer needs more before it starts to pack */
   {  
      Intf.pack_reqs_pending = PENDING_REQ;        /* so we don't have to wait for acknowledge signal from packer to proceed */
      Intf.multi_feed_inprogress = TRUE;              
      return TASK_DONE;                            /* task done, return */
   }
   else                                            /* last feed, already gave packer what it claims */
   {  Intf.pack_reqs_pending = NO_PENDING_REQ;     /* Now need to await packer's feedback to continue. */   
      Intf.multi_feed_inprogress = FALSE;
      Intf.feed_counter = 0;                       /* reset counter */

      if(CONFIG_REG & 0xF800)                        /* need delay before sending out signal */
      {   
         signal_delay = (CONFIG_REG & 0xF800)>>11;  /* 5 most significant bits:  get delay time */         
         signal_delay = signal_delay * 20;          /* X 20 */
         kick_off_timer(TASK_DELAY,signal_delay);   /* kickoff timer service 2 to start a delay, unit: 20*10ms */ 
         release_signal_not_sent_yet = TRUE;        /* we haven't sent release_done signal to packer yet */         
         return(TASK_NOT_FINISHED);                 /* return 0xff to tell caller we have something to do in the future.*/
      }
      else                                          /* send out signal immediately */
      {   
          if(os_sched.status & 0x4)
              kill_timer(TASK_DELAY);               /* kill timer in case any prior task is in progress */
          send_packer_release_signal();             /* send packer release done signal*/
          wait_for_signal_complete = TRUE; 
          return TASK_NOT_FINISHED;         
      }
   }
   
   return TASK_DONE;
}
                       
