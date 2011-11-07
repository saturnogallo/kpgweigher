#include "utili.h"
#include "scanner.h"
#include "stdlib.h"

static uchar navlen = 0;
static double reading = -1000;
static char navread[20];
static char navread2[20];
u8 scancmd[5];
u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE
void scanner_set_mode()
{                            
        if (IS_BORE_MODE)
        {
            scancmd[0] = '#';
        }else{
            scancmd[0]= '!';
        }
        scancmd[1] = 0x0D;
        scancmd[2] = 0x0A;
        prints(scancmd,3,PORT_SCANNER);    
}
void scanner_set_channel(uchar ch)
{       
        if(scanner_type == 1) //MI
        {        
                if(ch < 10)
                {
                        scancmd[0] = (ch + '0');
                        scancmd[1] = 'A';                
                        scancmd[2] = 0x0D;
                        scancmd[3] = 0x0A;
                        prints(scancmd,4,PORT_SCANNER);    
                        return;
                }else{              
                        scancmd[0] = (u8)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[1] = ch + '0';
                        scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,PORT_SCANNER);
                }
        }
        if(scanner_type == 2) //guidline
        {               
                        scancmd[0] = 'A';
                        scancmd[1] = (u8)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[2] = ch + '0';
                        scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,PORT_SCANNER);
        }
}

                            
//incoming data handler of scanner
void scanner_uart_push(uchar data)
{
}                                 
//incoming data hander of PC
void pc_uart_push(uchar data)
{
}       
static uchar navcmd[12];                                                                  
#define NAV_INVALID     -1000
#define NAV_VALID       -2000
#define RESETNAV       navlen = 0;   reading = NAV_INVALID;  

void nav_command(uchar cmd)
{
        if(cmd == NAV_30V)
               sprintf(navcmd,"%%01;12;02\r");
        if(cmd == NAV_1V)
               sprintf(navcmd,"%%01;12;01\r");
        if(cmd == NAV_120MV)
               sprintf(navcmd,"%%01;12;00\r");
        if(cmd == NAV_AFLTOFF)
               sprintf(navcmd,"%%01;26\r");
        if(cmd == NAV_AFLTON)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_SLOWMODE)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_ZEROON)    
        {
               sprintf(navcmd,"%%01;06\r");
        }
        if(cmd == NAV_INIT) 
        {           
                RESETNAV;
                sprintf(navcmd,"%%01;00\r");        
        }
        if(cmd == NAV_READ)
        {       
                RESETNAV;
                sprintf(navcmd,"%%01;01\r");
        }                       
//                com1_putc('?');
        prints(navcmd,strlen(navcmd),PORT_NAV);
}


unsigned long navtime;
double nav_read()
{                       
        if(DEBUG == 1)
                return 1.0; 
        nav_command(NAV_READ);
        navtime = 0;        
        while(1)
        {
                if(reading < NAV_INVALID) //make sure it is a valid reading
                {              
                     reading = atof(navread2);
                     return reading;
                }       
                if(navtime++ > 655350)
                {          
                     nav_command(NAV_READ);   
                     navtime = 0;
                }                
                sleepms(1);
        }
        return 1.0;
}            
//incoming data hander of navameter
void nav_uart_push(uchar data)
{                            
        if(navlen >= 19)
        {                          
                RESETNAV;
        }
        if(((data >= '0') && (data <= '9')) ||
                (data == '.') ||
                (data == '-') ||
                (data == '+') ||
                (data == 'e') ||
                (data == 'E') )
        {                    
                navread[navlen++] = data;
                reading = NAV_INVALID;
                return;
        }
                                        
        if(navlen < 4) //not enough digits
        {
                RESETNAV;
                return;
        }
        navread[navlen] = '\0';        
        do
        {
                navread2[navlen] = navread[navlen];
        }while(navlen-- > 0);
        navlen = 0;         
        reading =  NAV_VALID; //valid data
}                                        
