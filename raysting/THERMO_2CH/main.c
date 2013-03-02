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
#include "define.h"  
//#include "spi.h"
#include "drveeprom.h" 
#include "uart.h"    
#include "utili.h"
#include "scanner.h"
#include "wendu.h"
//#include "stdlib.h"
/*********************************************************************************************************************/
// Boot vetctors: This structure is used to share boot configuration info between primary firmware and boot firmware.
/*********************************************************************************************************************/
#define BOOT_VECT_ADDR 0x60
//BOOT_CONFIG boot_vector @ BOOT_VECT_ADDR;  /* store boot vector at a specified address in SRAM for sharing with primary */
                                           /* firmware. The first 96 (0x0~0x5F) addresses are reserved for I/O and registers */
/*
	double          R0;  	 //zero offset
	double          V0;  	 //zero offset
	double          Rs1; 	 //jiao-zheng zhi for PT100
	double          Rs2; 	 //for PT1000              
	u8             	ktime;	 //time for switch
	u8		type_bore;  //probe type of channel 1 and channel 2 for bore case
	u8              type_therm; //probe type of channel 1 and channel 2 for therm case
	u8		mode;	 //mode
	u8		kttmode; //ktt mode
	u8		fcmode;	 //F or C
	u8		chan;	 //channel mode
*/                
SYSDATA eeprom sysdata = { 0,0,100,1000,4,1,1,1,1,0,0};

RUNDATA rundata;
int ch_to_search = 0;    
extern u8 databuf[12];

extern double GetThmoVolt(double t,char type);   
extern void HandleKey(u8);
extern void CH453_I2C_Init();
extern void init_digi_Led();
extern void digi_cprint(u8 flash *str,u8 startpos, u8 size);
extern void timer_tick(); 
extern u8 CH453_ReadKey();

/*
typedef eeprom struct _PRBDATA
{
	char param1[8]; //+0.000E+0
	char param2[8]; //+0.000E+0
	char param3[8]; //+0.000E+0                        
	char rtp[8];	//0000.0000
}PRBDATA;
*/                
PRBDATA   eeprom rprbdata = {
        {"", ""},
        {"", ""},
        {"", ""},
        {"", ""}
 };
               
u8 start_of_buf()
{
        u8 i = 0;
        while(i< 16)
        {          
                if(databuf[i] == 0x00)
                        return i;
                i++;
        }          
        return i;
}
void myftoa(double v)
{             
        u8 pos;
        int dat;
        sprintf(databuf,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");                                       
        dat = (int)v;
        sprintf(databuf,"%d",dat);
        pos = start_of_buf();
        databuf[pos] = '.';
        v = v - dat;
        if(v < 0)
                v = -v;
        pos = pos+1;
        while(1)
        {
                v = v * 10000;         
                dat = (int)v;
                sprintf(databuf+pos,"%d",dat);
                v = v - dat;
                pos = start_of_buf();
                if(pos > 11)
                        return;
        }
}                                            
void sleepms(unsigned int i)
{
   while(i-- > 0)
      ;
}                       
u8 keycode = KEY_INVALID;
u8 refresh_cnt = 0;
u8 busy = 0;
extern u8 flash_cnt;
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{           
        if(busy == 1)
                return;
        busy = 1;       
	flash_cnt++;   
	if(flash_cnt > 10)
	        flash_cnt = 0;

        if(keycode == KEY_INVALID)
        {
                keycode = CH453_ReadKey();
        }        
        busy = 0;
}
/*****************************************************************************/
//                      Initialize Timers
// Timer 0 is used to generate pulses to drive electrical magnet
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
   TCNT0=0x92;            //60hz
      
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
   TCCR1B=0x00;
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
   TCCR2=0x00;
   TCNT2=0x00;
   OCR2=0x00;
   
   // Timer(s)/Counter(s) Interrupt(s) initialization
   // disable Timer 0, 1 and 2 interrupt
   TIMSK=0x01;     
}   

/*******************************************************************************/
//                          MEGA8 PORT initialization  
/*******************************************************************************/
void Init_PORT(void)
{
   //PORT,DDR: 0,0, Hi-Z, 		1,0, Hi-Z pullup
   //          0,1, output low, 1,1, output high
   // Input/Output Ports initialization
   // Port A initialization
   // bit6,bit5,bit4: OUTPUT, others are No use , all set to Hi-Z
   PORTA=0x70;
   DDRA=0x70;

   // Port B initialization
   // bit7: OUT, bit6: OUT, bit 5: OUT bit4-bit1 no use bit0 : 485DIR, bit1
   // 11100000
   // 11100001
   PORTB=0xE0;
   DDRB=0xE1;

   // Port C initialization
   // bit7-bit2 no use bit1 = SDA, bit0,SCL
   PORTC=0x03;
   DDRC=0x03;

   // Port D initialization
   // bit7-bit2 out, bit1 :txd, bit0:rxd
   // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=In 
   // State7=1 State6=1 State5=1 State4=1 State3=T State2=T State1=1 State0=T 
   PORTD=0xFE;
   DDRD=0xFE;

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
//                    Initialize System Global Variable 
// Read data from EEPROM and set vaule for system global variables
/*******************************************************************************/
extern void ValidatePrbType();
void Init_Vars(void)
{                    
        if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))      
                sysdata.Rs1 = 100;                         
        if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))                      
                sysdata.Rs2 = 1000;                                         
    if(IS_BORE_MODE)
    {
        if(IS_THERM_MODE)
                sysdata.mode = MODE_TEMP | MODE_BORE;
    }                                       
    ValidatePrbType();                
                                   
}

uchar nav1v = 1;
void navto120mv()
{               
   nav1v = 0;  
   nav_command(NAV_120MV);
   sleepms(200*ONEMS);            
}           

void navto1v()
{            
    nav1v = 1;
    nav_command(NAV_1V);
    sleepms(200*ONEMS);
} 
double mabs(double val)
{
        if(val < 0)
                return -val;
        else
                return val;
}
u8 ToByte(char l, char h)                                                 
{
	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
	return (h<<4)|l;
}             
void update_display()
{
        double v;
        if(sysdata.mode & MODE_TEMP)
        {                      
                v = rundata.temperature[ch_to_search];
                if((sysdata.chan == CHAN_MINUS))
                {
                        if(ch_to_search==0)
                                return;
                        v = v - rundata.temperature[0];
                }
                if(sysdata.fcmode == 0) //F case
                        v = (v*9.0+32*5.0)/5.0;
                myftoa(v);
                digi_print(databuf,0,11);
                return;
        }else
        {    
                v = rundata.reading[ch_to_search];
                if((sysdata.chan == CHAN_MINUS))
                {
                        if(ch_to_search==0)
                            return;

                        v = v - rundata.reading[0];
                }
                myftoa(v);
                digi_print(databuf,0,11);                
                return;
        }
}


double pickfrom5()
{
        double v,sum,max,min; //use the average of middle 3
        uchar i;
        max = -9999999;
        min  = 9999999;
        sum = 0;
        for(i = 0; i < 5; i++)
        {
                v = nav_read();
                if(max < v)
                        max = v;
                if(min > v)
                        min = v;
                sum = sum + v;
        }                     
        sum = sum - max - min;
        return sum/3.0;

}
u8 phase=0;
u16 dlg_cnt = 0;
u16 onesec_cnt = 0;
//state machine of therm type     
//phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
//phase 1:get the reading and update the display 
//phase 2: move to next channel
uchar therm_state()
{                  
        uchar i;
	if(phase > 2)
	        phase = 0;

	if(phase == 0)
	{                         
	        update_led(1);
		if( (sysdata.type_therm >= PRBTYPE_K) && (sysdata.type_therm <= PRBTYPE_R))
		{              
			scanner_set_channel(ch_to_search+1);	
			dlg_cnt = 6*ONESEC;
			onesec_cnt = 0;
			phase = 1;
			return 0;
		}else{
			rundata.temperature[ch_to_search] = -9999; //invalid data
			phase = 2;
		}
	}
	if(phase == 1)
	{
		rundata.reading[ch_to_search] = pickfrom5();//nav_read();//-sysdata.V0;
                //sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]); //no work
                //prints(strbuf,strlen(strbuf),PORT_PC);
		
		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], sysdata.type_therm);

                //sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                //prints(strbuf,strlen(strbuf),PORT_PC);
		phase = 2;
	}
	if(phase == 2)
	{
		update_display();
		phase = 0;
	}
	return 1;       //one reading is updated
}
//state machine of bore measure
//phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
//phase1 get reading of rs+, set to nktt, add delay 
//phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
//phase3 get reading of rx-, set to pktt, add delay
//phase4 get reading of rx+, switch to rx, caculate
//phase5 update display and move to next channel
void safesys()
{
SET_PKTT;
SET_TORX;
SET_TO1MA;
SET_TOCH1;
}
double valuep;         
u8 databuf[12];
uchar bore_state()
{
        uchar i;
	if(phase > 5)
	        phase = 0;
        i = sysdata.type_bore;
	if(phase == 0)
	{                               
	        update_led(1);
		if((i <= PRBTYPE_PT1000) && (i >= PRBTYPE_PT100))
		{
			scanner_set_channel(ch_to_search+1);	
			SET_PKTT;
			SET_TORS;                            
                        
			if(i == PRBTYPE_PT100)
			{                       
			        SET_TOPT100;
			        SET_TO1MA;			     
			}
			if(i == PRBTYPE_PT25)
			{
			        SET_TOPT100;
			        SET_TO1MA;
			}
			if(i == PRBTYPE_PT1000)
			{
			        SET_TOP1MA;
       			        SET_TOPT1000;
			}
			dlg_cnt =  ONESEC * sysdata.ktime;
			onesec_cnt = 0;
			if(IS_MODE_KTT)
				phase = 1;
			else
				phase = 2;
       			return 0;
		}else{                           
			rundata.temperature[ch_to_search] = -9999;
			phase = 5;
		}

	}
	if(phase == 1)  //-stdV in ktt mode
	{                                                
		rundata.stdV = mabs(pickfrom5());
		SET_NKTT;
		dlg_cnt =  ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		phase = 2;
		return 0;
	}
	if(phase == 2) //final stdV 
	{
		if(IS_MODE_KTT)
		{          
			rundata.stdV = (rundata.stdV + mabs(pickfrom5()));
			phase = 3;
		}else{                       
			rundata.stdV = mabs(pickfrom5());
			phase = 4;
		}                              
		SET_TORX;     
		
		dlg_cnt = ONESEC * sysdata.ktime;      
		onesec_cnt = 0;
		return 0;	
	}
	if(phase == 3)  //-V on rx in ktt mode
	{                             
		valuep = mabs(pickfrom5());
		SET_PKTT;
		dlg_cnt = ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		phase = 4;
		return 0;	
	}
	if(phase == 4) // final V on rx
	{
		if(IS_MODE_KTT){       
			valuep = (valuep + mabs(pickfrom5()));
/* auto switch voltage range			
			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
			{
        		        navto1v();     
			}            
			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
			{
			        navto120mv();
			}
*/			
		}else{               
			valuep = mabs(nav_read());                      
/* auto switch voltage range			
			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
			{
        		        navto1v();     
			}            
			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
			{
			        navto120mv();
			}
*/			
		}
		if(rundata.stdV != 0)
		{                           
                        if(i == PRBTYPE_PT1000)                       
              			rundata.reading[ch_to_search] = valuep*sysdata.Rs2/rundata.stdV - sysdata.R0;
                        else
        			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
			if(rundata.reading[ch_to_search] > 0)
			{
                                //sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
                                //prints(strbuf,strlen(strbuf),PORT_PC);

			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i,sysdata.type_bore);
                                rundata.Rx = rundata.reading[ch_to_search];
                                //sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                                //prints(strbuf,strlen(strbuf),PORT_PC);
			}
		}else{  
		        rundata.temperature[ch_to_search] = -9999;
		}
		phase = 5;
	}
	if(phase == 5) //got one reading
	{
		update_display();//pgmain_handler(MSG_REFRESH);
		phase = 0;
	}                 
	return 1;
}
        
void reset_counter()
{
	dlg_cnt = 0;					
       	onesec_cnt = 0;
        phase = 0;      //reset the state machine
}
/*****************************************************************************/
//                      Main function entry here
/*****************************************************************************/
void main(void)
{                         
   /********************************************************************/
   //               System Initialization
   /********************************************************************/          
   // Initialize MEGA8 PORT
   Init_PORT();   
      // Initialize SPI to communicate with CH453
   CH453_I2C_Init();

   // Initialize Global Variable: 
   // "Init_Vars()" has to be placed after "bReadDataFromEeprom()" because
   Init_Vars();
      
   // Initialize UART, 8 data, 1 stop, no parity. Boardrate is 2400.                   
   UART_Init(); 
      // Timer (Interrupt) initialization
   safesys();
   digi_cprint("88888888",0,8);

                
   init_digi_Led();
   Init_Timers();
   // initialize golbal interrupt.
   #asm("sei")  

    //init the DMM
    nav_command(NAV_INIT);              
    sleepms(200*ONEMS);                                
    navto1v();
    nav_command(NAV_SLOWMODE);
    sleepms(200*ONEMS);
    nav_command(NAV_AFLTON);
    sleepms(200*ONEMS);
                     
    update_led(0);
    
    
    sysdata.R0 = 0;
	 while(1)
	 {
	        if(keycode != KEY_INVALID)
	        {
                        HandleKey(keycode);
                        keycode = KEY_INVALID;
                        continue;
                }
         
		if(dlg_cnt > 1)
		{         
       			dlg_cnt--;
			continue;
		}
                timer_tick();		
		if(IS_THERM_MODE)                    
		{
        	        if(therm_state() == 0)
		                continue;     
                }
		if(IS_BORE_MODE)
		{
	        	if(bore_state() == 0)
		                continue;
		}     
			
                //shift to next channel 
                       
                if(sysdata.chan == CHAN_1)
                       ch_to_search = 0;
                else if(sysdata.chan == CHAN_2)
                       ch_to_search = 1;                                
                else 
                     ch_to_search = 1 - ch_to_search&0x01;                                
        }                      
}
