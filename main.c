/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.6 Professional
Automatic Program Generator
© Copyright 1998-2005 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
e-mail:office@hpinfotech.com

Project : M64_Bootloader
Version : 0.0
Date    : 2008-11-6
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega64
Program type        : Boot Loader - Size:4096words
Clock frequency     : 14.745600 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*****************************************************/

#include <mega64.h>
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
/******************************************************************************/
//ÒÔÏÂ²ÎÊı¿É¸ù¾İÏµÍ³ÉèÖÃ¿ÉÒÔ×Ô¼º¸Ä¶¯¡£ÆäÖĞUART_FORMAT¶¨ÒåÈçÏÂ£º
//0xE: ÎŞĞ£Ñé 2Î»Í£Ö¹Î»         0x6: ÎŞĞ£Ñé 1Î»Í£Ö¹Î»
//0x2E:Å¼Ğ£Ñé 2Î»Í£Ö¹Î»         0x26:Å¼Ğ£Ñé 1Î»Í£Ö¹Î»
//0x3E:ÆæĞ£Ñé 2Î»Í£Ö¹Î»         0x36:ÆæĞ£Ñé 1Î»Í£Ö¹Î»
/******************************************************************************/
#define SPM_PAGESIZE 256                     //M64µÄÒ»¸öFlashÒ³Îª256×Ö½Ú(128×Ö) 
#define BAUD 9600                            //²¨ÌØÂÊ²ÉÓÃ9600bps 
#define UART_FORMAT 0xE                      //UART¸ñÊ½ÉèÖÃ
#define CRYSTAL 14745600                     //ÏµÍ³Ê±ÖÓ14.745600 MHz£¬ÓÃÓÚ¼ÆËã
#define DATA_BUFFER_SIZE SPM_PAGESIZE        //¶¨Òå½ÓÊÕ»º³åÇø³¤¶È 

/******************************************************************************/
//ÒÔÏÂ¶¨ÒåÇëÎğÇáÒ×¸Ä¶¯
/******************************************************************************/
//¼ÆËãºÍ¶¨ÒåM64µÄ²¨ÌØÂÊÉèÖÃ²ÎÊı 
#define BAUD_SETTING (u8)((u32)CRYSTAL/(16*(u32)BAUD)-1) 
#define BAUD_H (u8)(BAUD_SETTING>>8) 
#define BAUD_L (u8)(BAUD_SETTING)   

//¶¨ÒåXmoden¿ØÖÆ×Ö·û 
#define XMODEM_NUL 0x00 
#define XMODEM_SOH 0x01 
#define XMODEM_STX 0x02 
#define XMODEM_EOT 0x04 
#define XMODEM_ACK 0x06 
#define XMODEM_NAK 0x15 
#define XMODEM_CAN 0x18 
#define XMODEM_EOF 0x1A 
#define XMODEM_RECIEVING_WAIT_CHAR 'C' 

//¶¨ÒåFlash×Ô±à³ÌÃüÁî×Ö
#define FILL_PAGE_CMD 0x01
#define ERASE_CMD 0x03
#define PGM_CMD 0x05 

u8 data[DATA_BUFFER_SIZE]; 
u16 address = 0;           //Ó¦ÓÃ³ÌĞòÇø´æ·ÅÔÚ´ÓµØÖ·0x0¿ªÊ¼µÄÇøÓò¡£
/******************************************************************************/
//ÌáÊ¾ĞÅÏ¢
/******************************************************************************/
//¶¨ÒåÈ«¾Ö±äÁ¿ 
const char startupString[]="Bootloader now. Type 'd' within 3 seconds to upgrade.\n\r\0"; 
const char AppString[]="Run application now.\n\r\0";

/******************************************************************************/
//²Á³ı(code=0x03)ºÍĞ´Èë(code=0x05)Ò»¸öFlashÒ³ 
//ZÖ¸Õë£¨ZH£º R31£¬ ZL£ºR30£©ÓÃÓÚFlash SPMµÄÑ°Ö·¼Ä´æÆ÷¡£
/******************************************************************************/ 
void boot_page_ew(u16 p_address,u8 code) 
{ 
#asm 
    ldd r30, y+1;
    ldd r31, y+2;
#endasm                    
    SPMCSR = code;          //¼Ä´æÆ÷SPMCSRÖĞÎª²Ù×÷Âë 
    #asm("spm")
}         
/******************************************************************************/
//Ìî³äFlash»º³åÒ³ÖĞµÄÒ»¸ö×Ö£¨code=0x01): R1R0Îª´ıĞ´ÈëµÄ×Ö£¬R31R30ÎªÒ³ÄÚµØÖ·                                                 
/******************************************************************************/
void boot_page_fill(u16 ui_address,u16 uc_data) 
{ 
#asm 
    LDD R30,Y+2  ; R30 LSB 
    LDD R31,Y+3  ; R31 MSB 
    LD  R26,Y 
    LDD R27,Y+1 
    MOV R0,R26 
    MOV R1,R27 
    MOV R26,R30 
    MOV R27,R31 
#endasm 
    SPMCSR =FILL_PAGE_CMD;      //¼Ä´æÆ÷SPMCSRÖĞÎª²Ù×÷Âë  
#asm    
    mov r30,r26 
    mov r31,r27 
#endasm 
    #asm("spm");                //¶ÔÖ¸¶¨FlashÒ³½øĞĞ²Á²Ù×÷   
}
         
/******************************************************************************/
//µÈ´ıÒ»¸öFlashÒ³µÄĞ´Íê³É
//SRMCSR[6]: RWWÇøÃ¦±êÖ¾¡£Æô¶¯Ò³²Á³ı»òÒ³Ğ´Èë²Ù×÷Ê±£¬ RWWSB ±»Ó²¼şÖÃ1¡£
//SRMCSR[0]: ÔÚÒ³²Á³ıºÍÒ³Ğ´¹ı³ÌÖĞSPMEN ±£³ÖÎª1Ö±µ½²Ù×÷Íê³É¡£
//Èç¹ûSRMCSR[0]ºÍSRMCSR[4]Í¬Ê±±»Ğ´Èë1£¬ÔòÔÚ½ô½Ó×ÅµÄËÄ¸öÊ±ÖÓÖÜÆÚÄÚµÄSPMÖ¸Áî½«ÔÙ
//´ÎÊ¹ÄÜRWWÇø¡£
/******************************************************************************/
void wait_page_rw_ok(void) 
{ 
     while(SPMCSR & 0x40) 
     { 
         while(SPMCSR & 0x01); 
         SPMCSR = 0x11;       //ÔÙ´ÎÊ¹ÄÜRWWÇø¡£
         #asm("spm") 
     } 
} 
/******************************************************************************/
//¸üĞÂÒ»¸öFlashÒ³µÄÍêÕû´¦Àí                                                     
/******************************************************************************/
void write_one_page(void) 
{ 
    u16 i,temp; 
    boot_page_ew(address,ERASE_CMD);            //²Á³ıÒ»¸öFlashÒ³ 
    wait_page_rw_ok();                          //µÈ´ı²Á³ıÍê³É 
    for(i=0;i<SPM_PAGESIZE;i+=2)                //½«Êı¾İÌîÈëFlash»º³åÒ³ÖĞ 
    { 
        temp = data[i+1]; 
        boot_page_fill(i,data[i]+(temp<<8));         
    } 
    boot_page_ew(address,PGM_CMD);              //½«»º³åÒ³Êı¾İĞ´ÈëÒ»¸öFlashÒ³ 
    wait_page_rw_ok();                          //µÈ´ıĞ´ÈëÍê³É 
}         
/******************************************************************************/
//´ÓRS232·¢ËÍÒ»¸ö×Ö½Ú                                                           
/******************************************************************************/
void uart_putchar(u8 c) 
{ 
    while(!(UCSR0A & 0x20)); 
    UDR0 = c; 
} 
/******************************************************************************/
//´ÓRS232½ÓÊÕÒ»¸ö×Ö½Ú                                                           
/******************************************************************************/
int uart_getchar(void) 
{ 
    u8 status,res; 
    if(!(UCSR0A & 0x80)) return -1;        //no data to be received 
    status = UCSR0A; 
    res = UDR0; 
    if (status & 0x1c) return -1;        // If error, return -1 
    return res; 
} 
/******************************************************************************/
//µÈ´ı´ÓRS232½ÓÊÕÒ»¸öÓĞĞ§µÄ×Ö½Ú                                                 
/******************************************************************************/
char uart_waitchar(void) 
{ 
    int c; 
    while((c=uart_getchar())==-1); 
    return (char)c; 
} 
/******************************************************************************/
//¼ÆËãCRC                                                                       
/******************************************************************************/
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
/******************************************************************************/
//ÍË³öBootloader³ÌĞò£¬´Ó0x0000´¦Ö´ĞĞÓ¦ÓÃ³ÌĞò 
/******************************************************************************/
void quit(void)      
{ 
    u8 i;
    while(AppString[i]!='\0')       //»ØËÍÍË³öbootloaderµÄÌáÊ¾ĞÅÏ¢
    { 
        uart_putchar(startupString[i]); 
        i++; 
    } 
    while(!(UCSR0A & 0x20));        //µÈ´ı½áÊøÌáÊ¾ĞÅÏ¢»ØËÍÍê³É 
    MCUCR = 0x01; 
    MCUCR = 0x00;                    //½«ÖĞ¶ÏÏòÁ¿±íÇ¨ÒÆµ½Ó¦ÓÃ³ÌĞòÇøÍ·²¿ 
    #asm("jmp 0x0000")               //Ìø×ªµ½FlashµÄ0x0000´¦£¬Ö´ĞĞÓÃ»§µÄÓ¦ÓÃ³ÌĞò 
} 
/******************************************************************************/
//Ö÷³ÌĞò                                                                        
/******************************************************************************/
void main(void) 
{ 
    int i = 0; 
    u8 timercount = 0; 
    u8 packNO = 1; 
    int bufferPoint = 0; 
    u16 crc; 
    //³õÊ¼»¯M64µÄUSART0 
    UBRR0H = BAUD_H;     
    UBRR0L = BAUD_L;          //Set baud rate 
    UCSR0B = 0x18;            //Enable Receiver and Transmitter 
    UCSR0C = UART_FORMAT;            //Set frame format: 8data, 2stop bit 
    //³õÊ¼»¯M64µÄT/C0£¬15ms×Ô¶¯ÖØÔØ 
    OCR0 = 0xEA; 
    TCCR0 = 0x0F;     
    
    //ÏòPC»ú·¢ËÍ¿ªÊ¼ÌáÊ¾ĞÅÏ¢ 
    while(startupString[i]!='\0') 
    { 
        uart_putchar(startupString[i]); 
        i++; 
    } 
        
   //3ÃëÖÖµÈ´ıPCÏÂ·¢"d"£¬·ñÔòÍË³öBootloader³ÌĞò£¬´Ó0x0000´¦Ö´ĞĞÓ¦ÓÃ³ÌĞò 
    while(1) 
    { 
        if(uart_getchar()== 'd') break; 
        if (TIFR & 0x02)                        //timer0 over flow 
        { 
               if (++timercount > 200) quit();  //200*15ms = 3s 
            TIFR = TIFR|0x02; 
        } 
    }             
    
    //Ã¿ÃëÏòPC»ú·¢ËÍÒ»¸ö¿ØÖÆ×Ö·û"C"£¬µÈ´ı¿ØÖÆ×Ö¡´soh¡µ 
    while(uart_getchar()!=XMODEM_SOH)           //receive the start of Xmodem 
    { 
         if(TIFR & 0x02)                        //timer0 over flow 
        { 
            if(++timercount > 67)               //wait about 1 second 
            { 
                uart_putchar(XMODEM_RECIEVING_WAIT_CHAR);    //send a "C" 
                timercount=0; 
            } 
            TIFR=TIFR | 0x02; 
        } 
    } 
 
/*************************************************************************************/
// XmodemĞ­ÒéÊÇÒ»ÖÖÊ¹ÓÃ²¦ºÅµ÷ÖÆ½âµ÷Æ÷µÄ¸öÈË¼ÆËã»úÍ¨ĞÅÖĞ¹ã·ºÊ¹ÓÃµÄÒì²½ÎÄ¼şÔËÊäĞ­Òé¡£
// ÕâÖÖĞ­ÒéÒÔ128×Ö½Ú¿éµÄĞÎÊ½´«ÊäÊı¾İ£¬²¢ÇÒÃ¿¸ö¿é¶¼Ê¹ÓÃÒ»¸öĞ£ÑéºÍ¹ı³ÌÀ´½øĞĞ´íÎó¼ì²â¡£
// Èç¹û½ÓÊÕ·½¹ØÓÚÒ»¸ö¿éµÄĞ£ÑéºÍÓëËüÔÚ·¢ËÍ·½µÄĞ£ÑéºÍÏàÍ¬Ê±£¬½ÓÊÕ·½¾ÍÏò·¢ËÍ·½·¢ËÍÒ»¸ö
// ÈÏ¿É×Ö½Ú¡£
// XmodemµÄ¿ØÖÆ×Ö·û£º<soh> 01H¡¢<eot> 04H¡¢<ack> 06H¡¢<nak> 15H¡¢<can> 18H¡¢<eof> 1AH¡£
// Xmodem´«ÊäÊı¾İ¿é¸ñÊ½£º"<soh> <packNO> <255-packNO> <128¸ö×Ö½ÚµÄÊı¾İ¿é> <cksum>"¡£
// ÆäÖĞ<soh>ÎªÆğÊ¼×Ö½Ú£»<packNO>ÎªÊı¾İ¿é±àºÅ×Ö½Ú£¬Ã¿´Î¼ÓÒ»£»<255-packNO>ÊÇÇ°Ò»×Ö½ÚµÄ
// ·´Âë£»½ÓÏÂÀ´ÊÇ³¤¶ÈÎª128×Ö½ÚµÄÊı¾İ¿é£»×îºóµÄ<cksum>ÊÇ128×Ö½ÚÊı¾İµÄCRCĞ£ÑéÂë£¬³¤¶È
// Îª2¸ö×Ö½Ú¡£½ÓÊÕ¶ËÊÕµ½Ò»¸öÊı¾İ¿é²¢Ğ£ÑéÕıÈ·Ê±£¬»ØËÍ<ack>£»½ÓÊÕ´íÎó»ØËÍ<nak>£»¶ø»Ø
// ËÍ<can>±íÊ¾Òª·¢ËÍ¶ËÍ£Ö¹·¢ËÍ¡£ ·¢ËÍ¶ËÊÕµ½<ack>ºó£¬¿É¼ÌĞø·¢ËÍÏÂÒ»¸öÊı¾İ¿é£¨packNO+1£©£»
// ¶øÊÕµ½<nak>Ôò¿ÉÔÙ´ÎÖØ·¢ÉÏÒ»¸öÊı¾İ¿é¡£ ·¢ËÍ¶Ë·¢ËÍ<eot>±íÊ¾È«²¿Êı¾İ·¢ËÍÍê³É¡£Èç¹û×î
// ºóĞèÒª·¢ËÍµÄÊı¾İ²»×ã128¸ö×Ö½Ú£¬ÓÃ<eof>ÌîÂúÒ»¸öÊı¾İ¿é¡£ ¿ØÖÆ×Ö·û"C"ÓĞÌØÊâµÄ×÷ÓÃ£¬
// µ±·¢ËÍ¶ËÊÕµ½"C"¿ØÖÆ×Ö·ûÊ±£¬Ëü»ØÖØĞÂ¿ªÊ¼ÒÔCRCĞ£Ñé·½Ê½·¢ËÍÊı¾İ¿é£¨packNO = 1£©¡£Ã¿
// ·¢ËÍÒ»¸öĞÂµÄÊı¾İ¿é<packNO>¼Ó1£¬¼Óµ½OxFFºóÏÂÒ»¸öÊı¾İ¿éµÄ<packNO>ÎªÁã¡£Ğ£Ñé·½Ê½²É
// ÓÃ16Î»CRCĞ£Ñé(X^16 + X^12 + X^5 + 1)¡£ 
/************************************************************************************/  
    //¿ªÊ¼½ÓÊÕÊı¾İ¿é 
    do{ 
        if ((packNO == uart_waitchar()) && (packNO ==(~uart_waitchar()))) {    //ºË¶ÔÊı¾İ¿é±àºÅÕıÈ· 
            for(i=0;i<128;i++) {                         //½ÓÊÕ128¸ö×Ö½ÚÊı¾İ              
                data[bufferPoint]= uart_waitchar(); 
                bufferPoint++;     
            } 
            crc = uart_waitchar();
            crc = crc << 8;
            crc += uart_waitchar();                     //½ÓÊÕ2¸ö×Ö½ÚµÄCRCĞ§Ñé×Ö 
            if(calcrc(&data[bufferPoint-128],128)==crc){//CRCĞ£ÑéÑéÖ¤             
                while(bufferPoint >= SPM_PAGESIZE)  {   //ÕıÈ·½ÓÊÜ256¸ö×Ö½ÚµÄÊı¾İ 
                    write_one_page();                   //ÊÕµ½256×Ö½ÚĞ´ÈëÒ»Ò³FlashÖĞ 
                    address += SPM_PAGESIZE;            //FlashÒ³¼Ó1 
                    bufferPoint = 0; 
                }     
                uart_putchar(XMODEM_ACK);               //ÕıÈ·ÊÕµ½Ò»¸öÊı¾İ¿é 
                packNO++;                               //Êı¾İ¿é±àºÅ¼Ó1 
            } 
            else { 
                uart_putchar(XMODEM_NAK);               //ÒªÇóÖØ·¢Êı¾İ¿é 
            } 
        } 
        else { 
            uart_putchar(XMODEM_NAK);                   //ÒªÇóÖØ·¢Êı¾İ¿é 
        } 
    }while(uart_waitchar()!=XMODEM_EOT);               //Ñ­»·½ÓÊÕ£¬Ö±µ½È«²¿·¢Íê 
    
    uart_putchar(XMODEM_ACK);                           //Í¨ÖªPC»úÈ«²¿ÊÕµ½      
    if(bufferPoint) write_one_page();                   //°ÑÊ£ÓàµÄÊı¾İĞ´ÈëFlashÖĞ 
    quit();                                             //ÍË³öBootloader³ÌĞò£¬´Ó0x0000´¦Ö´ĞĞÓ¦ÓÃ³ÌĞò 
} 
