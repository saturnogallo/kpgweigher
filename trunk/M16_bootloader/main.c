/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.8b Professional
Automatic Program Generator
?Copyright 1998-2006 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2010-4-7
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega16L
Program type        : Boot Loader - Size:1024words
Clock frequency     : 7.372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/

#include <mega16.h>
#include "define.h"
#include "uart.h" 

#define LAST_APP_PAGE 0x3780    /* byte addr, bootloader code starts from 0x3800 (byte address) */   
#define SPMCSR SPMCR

//Flash self program command
#define FILL_PAGE_CMD 0x01
#define ERASE_CMD 0x03
#define PGM_CMD 0x05
#define ENABLE_READ_CMD 0x11

// bootloader returned flags:
#define CRC_ERR 0x20                       /* bit 5 */
#define PGM_ERR 0x10                       /* bit 4 */

u8 boot_cmd;                               /* boot command from primary firmware*/
#define UPGRADE_FIRMWARE 0xA
#define NORMAL_BOOT      0x0	           /* Default data after powerup is 0 (normal boot) */
//u8 baud_rate;
                                            
BOOT_COMM_INTERFACE boot_comm;             /* communications interface between bootloader with master boards */     
#define BE_IDLE            0x0             /* firmware upgrade request is served successfully, or failed */
//#define INVALIDATE_APP   0x1             /* Clear last application page to indicate code is disturbed */
#define PGM_CURRENT_PAGE   0x2             /* command from master board asking node bootloader to program current flash page */
#define FW_UPGRADE_CMPT    0x3             /* Bootloader calculates CRC of whole application code and writes it to last app page */
#define RUN_APP            0x4             /* firmware upgrade completes, run application */

/*********************************************************************************************************************/
//                              NOTES: Calling Assembly language from C
//
// The following example shows how to access functions written in assembly language from a C program:
//
//  int sum_abc(int a, int b, unsigned char c) {
//  #asm
//      ldd   r30,y+3 ;R30=LSB a
//      ldd   r31,y+4 ;R31=MSB a
//      ldd   r26,y+1 ;R26=LSB b
//      ldd   r27,y+2 ;R27=MSB b
//      add   r30,r26 ;(R31,R30)=a+b
//      adc   r31,r27
//      ld    r26,y   ;R26=c
//      clr   r27     ;promote unsigned char c to int
//
//      add   r30,r26 ;(R31,R30)=(R31,R30)+c
//      adc   r31,r27
//  #endasm
//  }
//
//  void main(void) {
//  int r;
//  r=sum_abc(2,4,6);
//  }
//
// The compiler passes function parameters using the Data Stack.
// First it pushes the integer parameter a, then b, and finally the unsigned char parameter c.
// On every push the Y register pair decrements by the size of the parameter (4 for long int, 2 for int, 1 for char).
//
// For multiple byte parameters the MSB is pushed first.
// As it is seen the Data Stack grows downward.
// After all the functions parameters were pushed on the Data Stack, the Y register points to the last parameter c, 
// so the function can read its value in R26 using the instruction: ld r26,y.The b parameter was pushed before c, 
// so it is at a higher address in the Data Stack.
// The function will read it using: ldd r27,y+2 (MSB) and ldd r26,y+1 (LSB).
// The MSB was pushed first, so it is at a higher address.
//
// The a parameter was pushed before b, so it is at a higher address in the Data Stack.
// The function will read it using: ldd r31,y+4 (MSB) and ldd r30,y+3 (LSB).
//
// The functions return their values in the registers (from LSB to MSB):
//    R30 for char and unsigned char
//    R30, R31 for int and unsigned int
//    R30, R31, R22, R23 for long and unsigned long.

// So our function must return its result in the R30, R31 registers.

// After the return from the function the compiler automatically generates code to reclaim the Data Stack space used by
// the function parameters.
//
/*********************************************************************************************************************/
// Erase (code=0x03)and program (code=0x05)a flash page
// Z pointer (ZH： R31， ZL：R30) used as address register of Flash SPM.
/*********************************************************************************************************************/
void boot_page_ew(u16 p_address,u8 code) 
{ 
#asm
    cli; 
    ldd r30, y+1;
    ldd r31, y+2; 
    ld  r26, y;
    STS 0x57, r26;   SPMCR: SRAM addr 0x57
    SPM;
    sei;
#endasm  
}         
/*********************************************************************************************************************/
// Write one word (code = 0x01) to flash page buffer. R1R0 is the word to be written, R31R30 contains internal address
// within a page.
/*********************************************************************************************************************/
void boot_page_fill(u16 ui_address,u16 uc_data) 
{ 
#asm
    cli; 
    LDD R30,Y+2  ; R30 LSB 
    LDD R31,Y+3  ; R31 MSB 
    LD  R0, Y
    LDD R1, Y+1
    LDI R26,0x1 ;  FILL_PAGE_CMD = 0x1
    STS 0x57, R26; SPMCR: SRAM addr 0x57
    SPM
    sei;
#endasm     
}
/*********************************************************************************************************************/
// This subroutine reads a word from application code area.
/*********************************************************************************************************************/
u16 boot_read_program_memory (u16 adr)
{
#asm
    cli;
    LD R30, Y               ; load address to Z pointer, load LSB of "adr" to R30
    LDD R31, Y+1            ; load address to Z pointer, load MSB of "adr" to R31
    LPM R0, Z+              ; read lower byte addressed by Z pointer
    LPM R1, Z               ; read upper byte addressed by Z pointer
    MOV R30, R0             ; R31/R31 are registers containing returned data.
    MOV R31, R1             ;
    sei;
#endasm
}
/*********************************************************************************************************************/
// read programmed data from flash memory and compare it with original data in page buffer.
/*********************************************************************************************************************/
char boot_validate_flash(u16 page_addr)
{
    unsigned char i;							
    unsigned int TempInt;
    unsigned int TempInt2;

    for (i=0;i<SPM_PAGESIZE;i+=2)
      {
	TempInt = boot_read_program_memory(page_addr + i);   /*read data programmed*/

	TempInt2 = ((unsigned int)boot_comm.page_buffer[i+1]) << 8; 
	TempInt2 += boot_comm.page_buffer[i]; 

	if (TempInt != TempInt2) return 0;   /* if any words mismatch, error out */
      }
    return 1;
}

/*********************************************************************************************************************/
//wait for one page to complete.
//SRMCSR[6]: RWW busy flag。RWWSB is set to "1" by hardware when page is being programmed or erased.
//SRMCSR[0]: SPMEN, this bit is set until page program/erase completes.
// if SRMCSR[0] and SRMSCR[4] are both set to 1, SPM instruction within the 4 next clock cycles will reenable RWW section.
/*********************************************************************************************************************/
void wait_page_rw_ok(void) 
{ 
     while(SPMCSR & 0x40) 
     { 
         while(SPMCSR & 0x01); 
         SPMCSR = 0x11;       //re-enable RWW section
         #asm("spm") 
     } 
} 
/*********************************************************************************************************************/
//Write one flash page                                               
/*********************************************************************************************************************/
void write_one_page(u16 address) 
{ 
    u16 i,temp; 
    boot_page_ew(address,ERASE_CMD);              /* erase a flash page */
    wait_page_rw_ok();                            /* wait for page erase completion */
    for(i=0;i<SPM_PAGESIZE;i+=2)                  /* fill data into flash internal page buffer */
    { 
        temp = boot_comm.page_buffer[i+1]; 
        boot_page_fill(i,(u16)boot_comm.page_buffer[i]+(temp<<8));         
    } 
    boot_page_ew(address,PGM_CMD);                /* program a flash page */
    wait_page_rw_ok();                            /* waiting for program completion */
} 

/*********************************************************************************************************************/
// Calculate CRC                                                                       
/*********************************************************************************************************************/
int calcrc(char *ptr, int count) 
{ 
    int crc = 0; 
    char i; 
     
    while (--count >= 0) 
    { 
        crc = crc ^ (int) *ptr++ << 8; 
        i = 8; 
        do 
        { 
        if (crc & 0x8000) 
            crc = crc << 1 ^ 0x1021; 
        else 
            crc = crc << 1; 
        } while(--i); 
    } 
    return (crc); 
} 
/*********************************************************************************************************************/
//退出Bootloader程序，从0x0000处执行应用程序 
/*********************************************************************************************************************/
void execute_app_code(void)      
{ 
    MCUCR = 0x01; 
    MCUCR = 0x00;                    //将中断向量表迁移到应用程序区头部 
    #asm("jmp 0x0000")               //跳转到Flash的0x0000处，执行用户的应用程序 
} 
/*********************************************************************************************************************/
// MEGA16 IO PORT INITIALIZATION
/*********************************************************************************************************************/
void Init_PORT(void)
{

   // Input/Output Ports initialization
   // Port A initialization
   // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
   // State7=T State6=T State5=1 State4=1 State3=1 State2=1 State1=1 State0=1
   // PA0: MT1_PS, PA1: MT2_PS, PA2: MT_MD0, PA3: MT_MD1, PA4: MT_CLK, PA5: MT_DIR
   // PA6: PWM_FB, PA7: Test1 (force bootloader) 
   PORTA=0xBF;  // PORTA.7 set to IN with pull-up resister enabled.
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
     
}
/*********************************************************************************************************************/
// Main Entry
/*********************************************************************************************************************/
u8 myaddr;
#define FORCE_FW_UPGRADE (PINA.7 == 0)
#define UPPER_MOTOR_RESET  PIND.3         /* upper motor-S */
#define LOWER_MOTOR_RESET  PIND.2         /* lower motor-W */

void main(void) 
{ 
    u16 i, check_sum, tick; 
    bit app_firmware_invalidated; 
    
    // Initialize ports
    Init_PORT();

    /* read RS485 address from EEPROM */
    EEAR = EEPROM_RS485ADDR; 
    EECR |= (1<<EERE); 
    myaddr = EEDR; 
    /* The following code is to set RS485 address manually through U-type optocouplers */
    if(myaddr > MAX_RS485_ADDR)
    {   myaddr = 0; 
        while(UPPER_MOTOR_RESET) /* upper motor position resets, U-Type opto is blocked */
        { 
           while(LOWER_MOTOR_RESET && UPPER_MOTOR_RESET);
           myaddr++;
           myaddr &= 0x1F;       /* max 31*/
           PORTD = (~myaddr)<<4;      /* led display PORTD[7:4], LSB on "RUN" LED, MSB on "HWOK" LED */
           while((!LOWER_MOTOR_RESET) && UPPER_MOTOR_RESET);
        }
    }
    
    /* check if firmware upgrade is needed */
    EEAR = EEPROM_BOOTFLAG_ADDR; 
    EECR |= (1<<EERE);
    if(EEDR == FIRMWARE_UPGRADE_CMD)            /* Primary firmware writes 0xA0 in EEPROM to */
       boot_cmd = UPGRADE_FIRMWARE;
    else
       boot_cmd = NORMAL_BOOT;      

    /* Initialize USART0, 115200bps,8 data, 1 stop, even parity */
    UART_Init(); 
         
    // Initialize variables (used by bootloader and PC)
    boot_comm.upgrade_cmd = BE_IDLE;
    boot_comm.status = 'B';	       /* This will be polled by master to make sure board is in boot mode */
    boot_comm.page_addr = 0x0;    
    app_firmware_invalidated = FALSE; 
    
    // Global enable interrupts
    #asm("sei")       
    
   /*******************************************************************************/
   // Stay in boot loader and waiting for firmware upgrade command if: 
   // (1) System recovers from WDT reset and "boot_cmd" is set to UPGRADE_FIRMWARE, or
   // (2) Application flash space is blank/corrupted, or
   // (3) Hardware manual force reset (PINA.7 = 0)
   /*******************************************************************************/   
    if((boot_read_program_memory(LAST_APP_PAGE) != 0x55AA) || FORCE_FW_UPGRADE)
        boot_cmd = UPGRADE_FIRMWARE; 
    
    while(1){
       /******************************************************************************************************/    
       /* if command received is to update firmware, waiting for data from master board.*/
       /******************************************************************************************************/    
       if((boot_cmd == UPGRADE_FIRMWARE) ){
          if(tick++ > 25000)
          { LED_FLASH(PIN_HWOK);     /* HWOK LED flashing indicates we are currently in bootloader mode */
            tick = 0;
          }
          /********************************************************************************************/       
          // To be able to recover from power loss during firmware upgrade, 
          // Erase last application flash page to indicate application code is disturbed.
          // After all application code is re-installed (FW_UPGRADE_CMPT), the flag will be programmed 
          // again. 
          /********************************************************************************************/                 
          /*if(boot_comm.upgrade_cmd == INVALIDATE_APP)
          {
             boot_page_ew(LAST_APP_PAGE, ERASE_CMD);        /* erase last application flash page */ 
          /*   wait_page_rw_ok();                             /* wait for page erase completion */
          /*   boot_comm.upgrade_cmd = BE_IDLE;
          }*/
          /********************************************************************************************/       
          // Master requests to program current page 
          /********************************************************************************************/              
          if(boot_comm.upgrade_cmd == PGM_CURRENT_PAGE)
          {
             /* check CRC over received data in the buffer before programming */
             check_sum = boot_comm.page_buffer[SPM_PAGESIZE];
             check_sum = check_sum << 8;
             check_sum += boot_comm.page_buffer[SPM_PAGESIZE+1];

             if(calcrc(boot_comm.page_buffer, SPM_PAGESIZE)!= check_sum )
             {               
                boot_comm.upgrade_cmd = CRC_ERR;                  /* Bit 5: set CRC error bit*/
                continue;
             }
             // Invalidate application firmware before changing it. 
             // This flag will be reprogrammed after application firmware is upgraded.
             if(!app_firmware_invalidated)
             { 
                boot_page_ew(LAST_APP_PAGE, ERASE_CMD);        /* erase last application flash page to 
                                                                  invalidate application firmware */ 
                wait_page_rw_ok();                             /* wait for page erase completion */
                app_firmware_invalidated = TRUE;                                      
             }
             write_one_page(boot_comm.page_addr);              /* program one flash page */
             if(boot_validate_flash(boot_comm.page_addr))      /* if data is correctly programmmed*/
                  boot_comm.upgrade_cmd = BE_IDLE;
             else
                 boot_comm.upgrade_cmd = PGM_ERR;              /* Bit 4: set flash program error bit */             
             continue;
          }
          /********************************************************************************************/       
          // firmware update completes. No further data will be sent. bootloader programs a flag in 
          // last application page to indicate application firmware is good to use 
          // It is possible that power is interrupted during firmware upgrade and firmware is corrupted. 
          // By checking this flag, bootloader can tell whether last upgrade is successful or not.
          /********************************************************************************************/       
          if(boot_comm.upgrade_cmd == FW_UPGRADE_CMPT){
              for(i=0; i<SPM_PAGESIZE; i++) 
                boot_comm.page_buffer[i] = 0xFF;
              boot_comm.page_buffer[0] = 0xAA;            /* program word 0x55AA (firmware valid) */   
              boot_comm.page_buffer[1] = 0x55;              
              write_one_page(LAST_APP_PAGE);              /* program check sum to last application page */ 
              if(boot_validate_flash(LAST_APP_PAGE))      /* if data is correctly programmmed*/
                boot_comm.upgrade_cmd = BE_IDLE;              
              else
                boot_comm.upgrade_cmd = PGM_ERR;          /* Bit 4: set flash program error bit */               
              continue;
          } 
          /********************************************************************************************/       
          // Firmware upgrade completes, so break and start normal boot.
          /********************************************************************************************/       
          if(boot_comm.upgrade_cmd == RUN_APP){
             boot_cmd = NORMAL_BOOT;                      /* clear command on completion*/
             //execute_app_code();           
          }                                    
       } /* END boot_cmd == UPGRADE_FIRMWARE */
       
       /******************************************************************************************************/    
       // if normal boot, run into application code
       /******************************************************************************************************/            
       if(boot_cmd == NORMAL_BOOT)            
           execute_app_code();
              
    }    /*end of while(1) */
} 
