#include "utili.h"
#include "scanner.h"
#include "stdlib.h"
/*************************** 
        Scanner interface
****************************/
u8 scancmd[5];
u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE, 3: RAYSTING
void scanner_set_mode()
{      
        if(scanner_type == 3)                      
        {
                scancmd[0] = '#';
                //scancmd[0]= '!';
                scancmd[1] = 0x0D;
                scancmd[2] = 0x0A;
                prints(scancmd,3,PORT_SCANNER);    
        }
}
void scanner_set_channel(uchar ch)
{       
        if((scanner_type == 1)||(scanner_type == 3)) //MI OR RAYSTING
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
void scanner_uart_push(uchar dat)
{
}                        
/**********************************
        Navameter interface
**********************************/
static uchar navlen = 0;
static double reading = -1000;
static char navread[20];
static char navread2[20];
static uchar navcmd[12];                                                                  
#define NAV_INVALID     -1000
#define NAV_VALID       -2000
#define RESETNAV       navlen = 0;   reading = NAV_INVALID;  
//get stable reading from navmeter , remove max and min                                  
double nav_dbs[5];
double nav_read_stable()
{               
        double dmin,dmax,sum;
        uchar i;
        dmin = 1000;
        dmax = -1000;       
        sum = 0;
        for(i = 0;i < 5;i++)
        {
                sleepms(1000); //sleep100ms        
                nav_dbs[i] = nav_read();
                if(nav_dbs[i] < dmin)
                      dmin = nav_dbs[i];
                if(nav_dbs[i] > dmax)
                      dmax = nav_dbs[i];
                sum = sum + nav_dbs[i];
        }                              
        return (sum - dmin - dmax)/(5.0-2.0);
}
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

uchar nav1v = 1;
void navto120mv()
{               
   nav1v = 0;  
   nav_command(NAV_120MV);
   sleepms(20000);            
}           

void navto1v()
{            
    nav1v = 1;
    nav_command(NAV_INIT);
    sleepms(20000);
    nav_command(NAV_1V);
    sleepms(20000);
    nav_command(NAV_SLOWMODE);
    sleepms(20000);
    nav_command(NAV_AFLTON);
    sleepms(20000);
} 
         
/**********************************
        Relay board interface
**********************************/     
static uchar rlybuf[4] = {'U','s','U','U'};
#define SEND_RLYHEAD         prints(rlybuf,6,PORT_RELAY);
#define SEND_RLYTAIL         turn_relay('V')
void relay_uart_push(uchar dat)
{
}
void turn_relay(uchar reg)
{           
     prints(&reg,1,PORT_RELAY);
     sleepms(2000);
}                                

/**********************************
        Relay control logic
**********************************/

static uchar rlystate = 0x00; //current relay state
void relaystate(uchar mode)
{                                                            
        uchar  s = 0x00;               
        SEND_RLYHEAD;
//      if((mode & 0xE0) != (rlystate & 0xE0))
        {
                //turn off relay first
                turn_relay(0xf1);turn_relay(0xf2);turn_relay(0xf3);
                if(mode & RELAYSTATE_K1)         turn_relay(0x01);
                if(mode & RELAYSTATE_K2)         turn_relay(0x02);
                if(mode & RELAYSTATE_K3)         turn_relay(0x03);                
        }
//      if((mode & RELAYSTATE_KTT) != (rlystate & RELAYSTATE_KTT))                
        {
                s = (mode & RELAYSTATE_KTT)? 0x25: 0x2C;        
                turn_relay(s);
        }
        if((mode & 0x07)) //new current setting        
        {               
//              if(((mode & 0x07) != (rlystate & 0x07)))
                {                               
                        s = mode & 0x07;        
                        //turn on the new current relay;
                        if(s == RELAYCURR_P01)     turn_relay(0x42);
                        if(s == RELAYCURR_P02)     turn_relay(0x13);                
                        if(s == RELAYCURR_P05)     turn_relay(0x40);                
                        if(s == RELAYCURR_P1)      turn_relay(0x22);                
                        if(s == RELAYCURR_1)       turn_relay(0x30);                
                        if(s == RELAYCURR_3)       turn_relay(0x11);                
                        if(s == RELAYCURR_10)      turn_relay(0x31);
                        //turn off the old current relay;
                        if(s != RELAYCURR_P01)     turn_relay(0x43);
                        if(s != RELAYCURR_P02)     turn_relay(0x1A);                
                        if(s != RELAYCURR_P05)     turn_relay(0x41);                
                        if(s != RELAYCURR_P1)      turn_relay(0x2B);                
                        if(s != RELAYCURR_1)       turn_relay(0x39);                
                        if(s != RELAYCURR_3)       turn_relay(0x18);                
                        if(s != RELAYCURR_10)      turn_relay(0x38);                
                }
//                if(((mode & RELAYSTATE_SQR) != (rlystate & RELAYSTATE_SQR))
                {
                        s = (mode & RELAYSTATE_SQR)? 0x1E: 0x17;        
                        turn_relay(s);
                }        
                rlystate = mode;        
        }else{                  
                rlystate = (mode & 0xf8) | (rlystate & 0x07);        
        }            
        SEND_RLYTAIL;
}

//get the correct k based on vrs and vrx input           
extern double vrs,vrx,vcross_p,vcross_n;
extern double isrc;             //current of i src based on initial vrs reading
extern double rs;                 //rs value
extern double rx;                 //calculated rx value
extern uchar irx;                     //index of irx
extern unsigned int k_pos;        //default k value
/**********************************
        Capture and Track mode update
**********************************/   
static int track_weight[5]; //weights in tracking mode
static uchar ptr_tw = 0;    //pointer in track weight
                                                     
//Get a new K value in non tracking mode
//return the difference between new K and current k value
unsigned int calc_capture_nextk()
{               
        unsigned int N;    
        {
                N = (floor(isrc*rs*800*2/vrs)+1)/2; //get new k
                for(ptr_tw = 0; ptr_tw < sizeof(track_weight)/sizeof(int);ptr_tw++)
                        track_weight[ptr_tw] = 0;
                ptr_tw = 0;  
                if(N < 1)     N = 1;
                if(N > 8191)  N = 8191;                              
                return N-k_pos;
        }                 
}                                       
//Get a new K value in tracking mode                     
//return the difference between new k and old k based on weight algorithm
//if delta > 4, no weight algorithm
//else use the weight algorithm
unsigned int calc_track_nextk()
{               
        unsigned int N;    
        int sum;
      
        //tracking mode 
        rx = (800*rs/k_pos)-(vcross_p-vcross_n)/(2*isrc);//get new rx first
        N = (floor(800*rs*2/rx)+1) /2;                 //get new k
                
        if(N < 1)      N = 1;
        if(N > 8191)   N = 8191;                              
               
        track_weight[ptr_tw] = N - k_pos;             
        if(N-k_pos > 4)
               return  (N-k_pos);
                
        if(++ptr_tw >= sizeof(track_weight)/sizeof(int))
               ptr_tw = 0;
        //calculate the weight
        sum = 0;
        for(ptr_tw = 0; ptr_tw < sizeof(track_weight)/sizeof(int);ptr_tw++)                
               sum = sum + track_weight[ptr_tw]; 
        sum = sum >> 2; //since size of track_weight is 5
        return sum;
}
//turn winding to k                                  
void turn_k(unsigned int k)
{                  
        unsigned char s;    
        k_pos = k;
        SEND_RLYHEAD;
        s = (k & 0x0001)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0002)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0004)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0008)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0010)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0020)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0040)? 0x00: 0xff; turn_relay(s);
        s = (k & 0x0080)? 0x00: 0xff; turn_relay(s);                
        s = (k & 0x0100)? 0x00: 0xff; turn_relay(s);        
        s = (k & 0x0200)? 0x00: 0xff; turn_relay(s);        
        s = (k & 0x0400)? 0x00: 0xff; turn_relay(s);                        
        s = (k & 0x0800)? 0x00: 0xff; turn_relay(s);                
        s = (k & 0x1000)? 0x00: 0xff; turn_relay(s);                        
        SEND_RLYTAIL;        
}
//turn into capture mode.
void capture_prepare()
{
       navto1v();                                     
       turn_k(800); //1:1 mode                       
       relaystate(RLYMODE_VRS);       
       sleepms(10000); //sleep 1s       
}
//update reading in capture mode
//return 1: invalid vrs
//       2: invalid vrx
//       0: valid reading
uchar capture_once()
{
        //update vrs
        relaystate(RLYMODE_VRS + irx);
        sleepms(10000); //sleep 1s
        vrs = nav_read_stable();
        if((vrs > 10.0)||(vrs < -10.0))
                return 1;
        //update vrx
        relaystate(RLYMODE_VRX + irx);        
        sleepms(10000); //sleep 1s                
        vrx = nav_read_stable();
        if((vrx > 10.0)||(vrx < -10.0))
                return 2;
        //update k
        turn_k(k_pos + calc_capture_nextk());
        return 0;
}                
//turn into track mode               
void track_prepare()  
{
       navto120mv();                                     
       turn_k(800); //1:1 mode                       
       relaystate(RLYMODE_TRACKP + irx);       
       sleepms(10000); //sleep 1s       
}                        
//update k in track mode
//return 1: invalid vcross_p 
//return 2: invalid vcross_n
//return 3: delta k >64 
//return 4: delta k > 0
//return 0: delta k = 0
uchar track_once()
{       
        unsigned int newN;                                       
        relaystate(RLYMODE_TRACKP + irx);       
        sleepms(5000); //sleep 0.5s        
        vcross_p = nav_read_stable();
        if((vcross_p > 10.0)||(vcross_p < -10.0))
                return 1;
        
        relaystate(RLYMODE_TRACKN + irx);  
        sleepms(5000); //sleep 0.5s                     
        vcross_n = nav_read_stable();
        if((vcross_n > 10.0)||(vcross_n < -10.0))
                return 2;
        newN = calc_track_nextk();
        if(newN > 0)                 
        {
                if(newN >= 64)   //too big changes
                      return 3;
                turn_k(k_pos + newN);                                
                return 4;
        }        
        return 0;
}
/********************************* 
        Debug interface
**********************************/
//incoming data hander of PC
//four char or 1 char command     end with 0x0D or 0x0A
//example : 'x' or 'x' '1''2''3'4'                     
//R xx: turn relay to xx (ex: R 3E)
//M [RX,RS,KP,KN][1-7][Q] ( Q means sqr of current, RX is VRX, RS is VRS, KP is TRACKP, KN is TRACKN , [1-7] is current index)
//K nnnn : switch the relay to K nnnn is number
//N : reading navmeter
//t: Track prepare
//r: track once
//c: Capture prepare
//a: capture once
//0-9.UDCOT simulate keyboards
static uchar pccmd[7];
static uchar ptr_pccmd = 0;
void pc_uart_push(uchar data)
{           
        if(ptr_pccmd >= sizeof(pccmd)/sizeof(uchar))//no place to hold
        {
                ptr_pccmd = 0;          
                return;
        }              
        pccmd[ptr_pccmd++] = data;
        prints(&data,1,PORT_PC);
        if((data == 0x0D) || (data == 0x0A)) //end of command line
        {               
                if(ptr_pccmd == 0)
                        return;
                ptr_pccmd = 0;                      
                if(pccmd[0] == 'R')     //turn switch directly, R 3E
                {
                        SEND_RLYHEAD;
                        turn_relay(ToByte(pccmd[3],pccmd[2]));
                        SEND_RLYTAIL;                     
                        return;
                }
                if(pccmd[0] == 'M')     //turn to mode , M (RX,RS,KP,KN)[1-9]Q
                {                       
                        if(pccmd[3] == 'X')  data = RLYMODE_VRX;
                        if(pccmd[3] == 'S')  data = RLYMODE_VRS;
                        if(pccmd[3] == 'P')  data = RLYMODE_TRACKP;
                        if(pccmd[3] == 'N')  data = RLYMODE_TRACKN;                                                                        
                        if(pccmd[5] == 'Q')  data = data + RELAYSTATE_SQR;
                        data = data + pccmd[4] - '0';
                        irx = pccmd[4] - '0';                                
                        relaystate(data);
                        return;
                }
                if(pccmd[0] == 'K')     //set K, K 2345
                {              
                        turn_k( ToByte(pccmd[3],pccmd[2])*256+ToByte(pccmd[5],pccmd[4]));
                        return;
                }                
                
                if(pccmd[0] == 'N')     //read navmeter
                {
                        dbgout(nav_read());
                        dbgout(rx);
                        dbgout(k_pos);
                        return;
                }                
                if(pccmd[0] == 't')     //track prepare
                {
                        track_prepare();
                        return;
                }                
                if(pccmd[0] == 'r')     //track once
                {
                        dbgout(track_once());
                        return;
                }                

                if(pccmd[0] == 'c')     //capture prepare
                {
                        capture_prepare();
                        return;
                }                
                if(pccmd[0] == 'a')     //capture once
                {
                        dbgout(capture_once());
                        return;
                }                
                if((pccmd[0] >= '') && (pccmd[0] <= '9'))
                {              
                        key = pccmd[0];                          
                        return;
                }
                if((pccmd[0] == '.') ||
                   (pccmd[0] == 'U') ||
                   (pccmd[0] == 'D') ||
                   (pccmd[0] == 'C') ||
                   (pccmd[0] == 'O') ||
                   (pccmd[0] == 'T') )
                {              
                        key = pccmd[0];                          
                        return;
                }
                
        }
}                
