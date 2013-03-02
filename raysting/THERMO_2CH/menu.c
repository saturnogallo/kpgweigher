#include "define.h"
#include "scanner.h"
#include "utili.h"
#include "stdlib.h"
extern PRBDATA eeprom  rprbdata;
void memcpy1(eeprom u8 *src, u8 *dst, u8 size)
{
        while(size-- > 0)
        {
                *dst++ = *src++;
        }
}         
void memcpy2( u8 *src, eeprom u8 *dst, u8 size)
{
        while(size-- > 0)
        {
                *dst++ = *src++;
        }
}
double buf2double(u8 *buf)
{
        return 0.1;
}
void ValidatePrbType()
{
	if((sysdata.type_bore > PRBTYPE_PT1000))
		sysdata.type_bore = PRBTYPE_PT100;
        if((sysdata.type_therm < PRBTYPE_K) || (sysdata.type_therm > PRBTYPE_R))
		sysdata.type_therm = PRBTYPE_K;
	
	
}
u8 menu_index = 0xff; //not in menu mode
extern void reset_counter();              
extern void EnterMenu();         

void HandleKey(u8 key)
{
	if(key == 0xff)
		return;
	if(key == KEY_MODE) //display temperature of direct reading
	{       
	        if(IS_BORE_MODE)
	        {
        	        sysdata.mode = (sysdata.mode & MODE_TEMP) | MODE_THERM;
	        }else{
               	        sysdata.mode = (sysdata.mode & MODE_TEMP) | MODE_BORE;
	        }       
	        digi_cprint("   . . . . ",0,8);                       
	        update_led(0);
	        phase = 0;
                safesys();
		return;
	}                                                          
	if(key == KEY_PROBE) //change to next probe type
	{
	        if(IS_BORE_MODE)
	        {
	                sysdata.type_bore += 1;
	        }
	        if(IS_THERM_MODE)
	        {
	                sysdata.type_therm += 1;
	        }            
	        digi_cprint("   . . . . ",0,8);                       
	        ValidatePrbType();
	        update_led(0);
	        return;
	}

	if(key == KEY_ZERO)
	{
		if(IS_BORE_MODE){
		        sysdata.R0 = 0;// rundata.Rx;
		}else{             
			sysdata.V0 = nav_read();
		        nav_command(NAV_ZEROON);
		        sleepms(1000*ONEMS);
		}
		digi_cprint("  doNE   ",0,8);
		update_display();
		return;
	}
	if(key == KEY_DIGI) //show temp or not
	{                 
	        sysdata.mode = sysdata.mode ^ MODE_TEMP;
		ValidatePrbType();
		update_led(0);   
		update_display();
		return;
	}
	if(key == KEY_CHAN)
	{
		sysdata.chan++;
		if(sysdata.chan > CHAN_MINUS)
			sysdata.chan = CHAN_1;
		if(sysdata.chan == CHAN_1)
		        digi_cprint("  chAn 1",0,8);
		else if(sysdata.chan == CHAN_2)
		        digi_cprint("  chAn 2",0,8);
		else if(sysdata.chan == CHAN_MINUS)
		        digi_cprint("  chAn -",0,8);
		else
       		        digi_cprint(" chAn 1.2",0,8);
		update_led(0);
		return;
	}
	if(key == KEY_KTT)
	{
		if(IS_BORE_MODE)
		{
			sysdata.kttmode = sysdata.kttmode ^ 0x01; 
                        SET_TOP1MA;
	                SET_TOPT1000;                				
			SET_PKTT;	   
			if(sysdata.kttmode & 0x01)
				digi_cprint("dUAL  0N",0,8);
			else
				digi_cprint("dUAL oFF",0,8);			
                        reset_counter();
		}		
		return;
	}
	if(key == KEY_OK)
	{
		EnterMenu();
		digi_flash(0,7,0);
	}
}                                                    
void ValidateDataBuf()
{//need format 1234.5678
        u8 j=0;
        for(j=0;j<9;j++)
        {
                if(j == 4)
                        continue;
                if(databuf[j] <= '9' && databuf[j] >= '0')
                        continue;
                databuf[j] = '0';
        }        
	databuf[4] = '.';
	databuf[9] = 0x00;

}                     
void ValidateDataBuf2()
{//need format +1.234E-3   
        u8 j=0;
        for(j=0;j<9;j++)                    
        {
                if((j==0) || (j==7))
                {
                     if((databuf[j] != '-') && (databuf[j] != ' '))  
                        databuf[j] = ' ';
                     continue;
                }
                if(databuf[j] > '9' || databuf[j] < '0')
                        databuf[j] = '0';
        }
     databuf[2] = '.';
     databuf[6] = 'E';
     databuf[9] = 0x00;
}
#define MAX_MENU_INDEX	10	//total menu rows index     0:dual, 1:unit, 2:dly, 3,4,5,6:r1,a1,b1,c1, 7,8,9,10:r2,a2,b2,c2
extern u8 databuf[12];
extern u8 keycode;
void EnterMenu()
{	//mode, probe, zero(left), digi(right), chan(up), ktt(down), key_ok
	//up, 	 down is shift the menu index, and modify the digital
	//right, left is change the position
	//ok :	enter the menu, exit the menu, and start to modify
	u8 key;
	u8 inflash = 0xff; //flash position 
	double v;
	if(keycode != KEY_OK)
		return;
	menu_index = 1;
	key = KEY_INVALID;
	keycode = KEY_INVALID;
	while(1)
	{                                       
/*
		if(menu_index == 0)	//dUAL	ON|OFF
		{                 
		        if(key == KEY_OK)
		        {
		                if(inflash == 0xff)
		                        inflash = 1;
		                else
		                        inflash = 0xff;
			}
			if((inflash == 1) && ((key == KEY_UP) || (key == KEY_DN)))
					sysdata.kttmode	= 1 - sysdata.kttmode & 0x01;


			if(inflash == 1)
				digi_flash(5,7,1);
			else
				digi_flash(0,7,0);
			key = 0xff;
		}
*/
		if(menu_index == 1) //UN1t   F|C
		{
		        if(key == KEY_OK)
		        {
		                if(inflash == 0xff)
		                        inflash = 1;
		                else
		                        inflash = 0xff;
			}

			if((inflash == 1) && ((key == KEY_UP) || (key == KEY_DN)))
					sysdata.fcmode	= 1 - sysdata.fcmode&0x01;

			if(sysdata.fcmode == 0)
				digi_cprint("UN1t   F",0,8);
			else
				digi_cprint("UN1t   C",0,8);
			if(inflash == 1)
				digi_flash(7,7,1);
			else
				digi_flash(7,7,0);
			key = 0xff;
		}
		if(menu_index == 2) //dLY   00
		{
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
					inflash = 6;
				else
					inflash = 0xff;
			}
			if((inflash == 6) && (key == KEY_RIGHT))
				inflash = 7;
			if((inflash == 7) && (key == KEY_LEFT))
				inflash = 6;
			if((inflash == 6) && (key == KEY_DN))
			{
				if(sysdata.ktime >= 10)
					sysdata.ktime -= 10;
				else
					sysdata.ktime += 90;
			}
			if((inflash == 6) && (key == KEY_UP))
			{
				if(sysdata.ktime < 90)
					sysdata.ktime += 10;
				else
					sysdata.ktime -= 90;
			}
			if((inflash == 7) && (key == KEY_DN))
			{
				if((sysdata.ktime % 10) != 9)
					sysdata.ktime += 1;
				else
					sysdata.ktime -= 9;
			}
			if((inflash == 7) && (key == KEY_UP))
			{
				if((sysdata.ktime % 10) != 0)
					sysdata.ktime -= 1;
				else
					sysdata.ktime += 9;
			}        
			if(sysdata.ktime >= 100)
			        sysdata.ktime = 0;
			sprintf(databuf,"dLY   %d%d", (sysdata.ktime/10),(sysdata.ktime%10));
			digi_print(databuf,0,8);	
			
			if(inflash != 0xff)
				digi_flash(inflash,inflash,1);
			else
			        digi_flash(0,7,0);
			key = 0xff;
		}
		if((menu_index == 3) || (menu_index == 7))//1r, 2r  ....   rvalue for channel 1 and channel 2
		{       //1234.5678
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
				{
					inflash = (menu_index-3)/4;
					memcpy1(rprbdata.rtp[inflash],databuf,4);
					memcpy1(4+rprbdata.rtp[inflash],databuf+5,4);
					ValidateDataBuf();
					inflash = 0;
				}else{
					ValidateDataBuf();
					inflash = (menu_index-3)/4;  
					memcpy2(databuf,rprbdata.rtp[inflash],4);
					memcpy2(databuf+5,rprbdata.rtp[inflash]+4,4);
					rundata.rtp[inflash] = atof(databuf);//buf2double(databuf); //
					inflash = 0xff;
				}
			}
			if((key == KEY_DN) && (inflash != 0xff))
			{                             
			        if(inflash >= 4)
			                inflash += 1;
				if(databuf[inflash] == '0')
					databuf[inflash] = '9';
				else
					databuf[inflash] = databuf[inflash]-1;
		                if(inflash >= 4)
			                inflash -= 1;

			}
			if((key == KEY_UP) && (inflash != 0xff))
			{
			        if(inflash >= 4)
			                inflash += 1;
				if(databuf[inflash] == '9')
					databuf[inflash] = '0';
				else
					databuf[inflash] = databuf[inflash]+1;
		                if(inflash >= 4)
			                inflash -= 1;
			}
			if((key == KEY_LEFT) && (inflash != 0xff))
			{
				if(inflash <= 0)
					inflash = 7;
				else
					inflash--;
				if(inflash == 4)
					inflash = 3;
			}
			if((key == KEY_RIGHT) && (inflash != 0xff))
			{
				if(inflash >= 7)
					inflash = 0;
				else
					inflash++;
			}
			
			if(inflash != 0xff)
			{
				digi_print(databuf,0,8);
				digi_flash(inflash,inflash,1);
			}else{
				sprintf(databuf, "%cr    . . .",'1' + (menu_index-3)/4);
				digi_print(databuf,0,8);
				digi_flash(0,7,0);
			}
			key = 0xff;
			timer_tick();
		}
		if((menu_index >= 4) && (menu_index  != 7) && (menu_index  < 11))//1AbC  ....     coeffient a,b,c for channel 1 and channel 2, row 4,5,6, 8, 9, 10
		{ //+1.234E-3
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
				{
					inflash = (menu_index-3)/4;
					memcpy1(rprbdata.rtp[inflash],databuf,2);
					memcpy1(2+rprbdata.rtp[inflash],databuf+3,6);
					ValidateDataBuf2();
					inflash = 0;
				}else{
					inflash = (menu_index-3)/4;
					ValidateDataBuf2();
					memcpy2(databuf,rprbdata.rtp[inflash],2);
					memcpy2(databuf+3,rprbdata.rtp[inflash]+2,6);
					v = atof(databuf);//buf2double(databuf); //
					if((menu_index % 4) == 0)
						rundata.param1[inflash] = v;
					if((menu_index % 4) == 1)
						rundata.param2[inflash] = v;
					if((menu_index % 4) == 2)
						rundata.param3[inflash] = v;
					inflash = 0xff;
				}
			}
			if((key == KEY_DN) && (inflash != 0xff))
			{                          
			        if(inflash >= 2)
			                inflash += 1;
				if(databuf[inflash] == '0')
					databuf[inflash] = '9';
				else if(databuf[inflash] == '-')
					databuf[inflash] = ' ';
				else if(databuf[inflash] == ' ')
					databuf[inflash] = '-';
				else if((databuf[inflash] >='1') && (databuf[inflash] <='9'))
					databuf[inflash] = databuf[inflash]-1;
			        if(inflash >= 2)
			                inflash -= 1;

			}
			if((key == KEY_UP) && (inflash != 0xff))
			{                               
			        if(inflash >= 2)
			                inflash += 1;
			        
				if(databuf[inflash] == '9')
					databuf[inflash] = '0';
				else if(databuf[inflash] == '-')
					databuf[inflash] = ' ';
				else if(databuf[inflash] == ' ')
					databuf[inflash] = '-';
				else if((databuf[inflash] >='0') && (databuf[inflash] <='8'))
					databuf[inflash] = databuf[inflash]+1;
			        if(inflash >= 2)
			                inflash -= 1;

			}
			if((key == KEY_LEFT) && (inflash != 0xff))
			{
				if(inflash == 0)
					inflash = 7;
				else
					inflash--;
				if(inflash == 5)
					inflash = 4;
			}
			if((key == KEY_RIGHT) && (inflash != 0xff))
			{       inflash++;
				if(inflash == 8)
					inflash = 0;
				
				if(inflash == 5)
					inflash = 6;
			}
			
			if(inflash != 0xff)
			{
				digi_print(databuf,0,8);
				digi_flash(inflash,inflash,1);
			}else{                
			        key = '1' + (menu_index-3)/4;
				if((menu_index % 4) == 0)
					sprintf(databuf, "%cA    . . .",key);
				if((menu_index % 4) == 1)
					sprintf(databuf, "%cb    . . .",key);
				if((menu_index % 4) == 2)
					sprintf(databuf, "%cc    . . .",key);
				digi_print(databuf,0,8);
				digi_flash(0,7,0);
			}
			key = 0xff;
		}
		while(keycode == 0xff) 
		{
		        timer_tick();
			sleepms(1);
		}
	        key = keycode;
	        keycode = KEY_INVALID;
		if((key == KEY_MODE) || (key == KEY_PROBE))
			break;
		if(key == KEY_OK || key == KEY_LEFT || key == KEY_RIGHT)
			continue; //let sub handle it
		if((key == KEY_UP))
		{                 
        		if(inflash == 0xff)
        		{
		        	if(menu_index == 1)
	        			menu_index = MAX_MENU_INDEX;
        			else
			        	menu_index--;
        		}else{
        		        continue; //let sub handle it
        		}
		}
		if((key == KEY_DN) )
		{
        		if(inflash == 0xff)
                	{
		        	if(menu_index == MAX_MENU_INDEX)
		        	{
		        	        digi_cprint("  doNE  ",0,8);
			        	break;//menu_index = 0;
        			}
        			menu_index++;
	        	}else{
        		        continue; //let sub handle it
        		}
		}
		key = 0xff;
		continue;
	}
	menu_index = 0xff;
}
