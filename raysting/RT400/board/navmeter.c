#include "sjDefine.h"
#include "typedef.h"
#include "stdio.h"
#include "stdlib.h"
#include "sjSerial.h"

static uchar navlen = 0;
static double reading = -1000;
static char navread[20];
static char navread2[20];


static uchar navcmd[12];                                                                  
#define NAV_INVALID     -1000
#define NAV_VALID       -2000
#define RESETNAV       navlen = 0;   reading = NAV_INVALID;  




void nav_command(uchar cmd)
{
//        if(cmd == NAV_30V)
//               sprintf(navcmd,"%%01;12;02\r");
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


unsigned long navtime = 0;
double nav_read()
{                       
//        if(DEBUG == 1)
//                return 1.0; 
		if(navtime == 0)
	        nav_command(NAV_READ);
		navtime++;
        if(navtime++ > 65535)
             navtime = 0;
 
        if(reading < NAV_INVALID) //make sure it is a valid reading
        {              
            reading = atof(navread2);
            return reading;
         }      
//       sleepms(1);
		while(io_hasc())
		{
			nav_uart_push(io_getc());
		}
        return NAV_INVALID;
}            
//incoming data hander of navameter
void nav_uart_push(uchar dat)
{        
		//sjSerialSendByte(dat);                    
        if(navlen >= 19)
        {                          
                RESETNAV;
        }
        if(((dat >= '0') && (dat <= '9')) ||
                (dat == '.') ||
                (dat == '-') ||
                (dat == '+') ||
                (dat == 'e') ||
                (dat == 'E') )
        {                    
                navread[navlen++] = dat;
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