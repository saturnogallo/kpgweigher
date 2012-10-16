#include "define.h"



void State_Init()
{
}
void ValidatePrbType()
{
	if(sysdata.mode == MODE_BORE)
	{
		if((sysdata.prbtype > PRBTYPE_PT1000))
			sysdata.prbtype = PRBTYPE_PT100;
		return;
	}
	if(sysdata.mode == MODE_THERMO) 
	{
		if((sysdata.prbtype < PRBTYPE_K) || (sysdata.prbtype > R))
			sysdata.prbtype = PRBTYPE_K;
		return;
	}
}
u8 menu_index = 0xff; //not in menu mode
void HandleKey()
{
	u8 key = CH453_ReadKey();
	if(key == 0xff)
		return;
	if(key == KEY_MODE)
	{
		switch(sysdata.mode)
		{
			case MODE_TEMP:	 		sysdata.mode = MODE_BORE; break;
			case MODE_THERMO:   	sysdata.mode = MODE_TEMP; break;
			case MODE_BORE: 		sysdata.mode = MODE_THERMO; break;
			default:				sysdata.mode = MODE_THERMO; break;
		}
		ValidatePrbType();
		UpdateLeds();
		return;
	}
	if(key == KEY_PROBE)
	{	//change probe type
		sysdata.prbtype++;
		if(sysdata.prbtype > PRBTYPE_R) 
			sysdata.prbtype = PRBTYPE_PT100;
		ValidatePrbType();
		UpdateLeds();
		return;
	}
	if(key == KEY_ZERO)
	{
		if(sysdata.prbtype > PRBTYPE_PT1000)
			sysdata.V0 = display;
		UpdateValues();
		return;
	}
	if(key == KEY_DIGI)
	{
		return;
	}
	if(key == KEY_CHAN)
	{
		sysdata.chan++;
		if(sysdata.chan > CHAN_MINUS)
			sysdata.chan = CHAN_1;
		UpdateLeds();
		return;
	}
	if(key == KEY_KTT)
	{
		sysdata.kttmode = sysdata.kttmode ^ 0x01;
		return;
	}
	if(key == KEY_OK)
	{
		in_menu = 1;
	}
}
#define MAX_MENU_INDEX	8	//total menu rows index
extern u8 databuf[12];
void EnterMenu()
{	//mode, probe, zero(left), digi(right), chan(up), ktt(down), key_ok
	//up, 	 down is shift the menu index, and modify the digital
	//right, left is change the position
	//ok :	enter the menu, exit the menu, and start to modify
	u8 key;
	u8 inflash = 0xff; //flash position
	if(ReadKey() != KEY_OK)
		return;
	menu_index = 0;
	key = 0xff;
	while(1)
	{
		if(menu_index == 0)	//dUAL	ON|OFF
		{
			inflash = ((inflash == 0xff) && (key == KEY_OK)) ? 1 : 0xff;
			if((inflash == 1) && ((key == KEY_UP) || (key == KEY_DN)))
					sysdata.kttmode	= 1 - sysdata.kttmode;

			if(sysdata.kttmode == 0)
				digi_cprint("dUAL OFF",0,8);
			else
				digi_cprint("dUAL  ON");
			if(inflash == 1)
				digi_flash(5,7,1);
			else
				digi_flash(0,7,0);
			key = 0xff;
		}
		if(menu_index == 1) //UN1t   F|C
		{
			inflash = ((inflash == 0xff) && (key == KEY_OK)) ? 1 : 0xff;
			if((inflash == 1) && ((key == KEY_UP) || (key == KEY_DN)))
					sysdata.fcmode	= 1 - sysdata.fcmode;

			if(sysdata.fcmode == 0)
				digi_cprint("UN1t   F");
			else
				digi_cprint("UN1t   C");
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
					inflash = 5;
				else
					inflash = 0xff;
			}
			if((inflash == 5) && (key == KEY_RIGHT))
				inflash = 6;
			if((inflash == 6) && (key == KEY_LEFT))
				inflash = 5;
			if((inflash == 5) && (key == KEY_DN))
			{
				if(sysdata.ktime >= 10)
					sysdata.ktime -= 10;
				else
					sysdata.ktime += 90;
			}
			if((inflash == 5) && (key == KEY_UP))
			{
				if(sysdata.ktime < 90)
					sysdata.ktime += 10;
				else
					sysdata.ktime -= 90;
			}
			if((inflash == 6) && (key == KEY_DN))
			{
				if((sysdata.ktime % 10) != 9)
					sysdata.ktime++;
				else
					sysdata.ktime -= 9;
			}
			if((inflash == 6) && (key == KEY_UP))
			{
				if((sysdata.ktime % 10) != 0)
					sysdata.ktime--;
				else
					sysdata.ktime += 9;
			}
			sprintf(databuf,"dLY   %c%c", (u8)(sysdata.ktime/10)+'0',(u8)(sysdata.ktime%10)+'0');
			digi_print(databuf,0,8);	
			digi_flash(0,7,0);
			if(inflash != 0xff)
				digi_flash(inflash,inflash,1);
			key = 0xff;
		}
		if((menu_index == 3) || (menu_index == 7))//1r, 2r  ....
		{
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
				{
					inflash = (menu_index-3)/4;
					memcpy(prbdata.rtp[inflash],databuf,4);
					memcpy(4+prbdata.rtp[inflash],databuf,4);
					databuf[4] = '.';
					databuf[8] = 0x00;
					inflash = 0;
				}else{
					inflash = (menu_index-3)/4;
					memcpy(databuf,prbdata.rtp[inflash],4);
					memcpy(databuf+5,prbdata.rtp[inflash]+4,4);
					rundata.rtp[inflash] = buf2double(databuf);
					inflash = 0xff;
				}
			}
			if((key == KEY_DN) && (inflash != 0xff))
			{
				if(databuf[inflash] == '0')
					databuf[inflash] == '9';
				else
					databuf[inflash] = databuf[inflash]-1;
			}
			if((key == KEY_UP) && (inflash != 0xff))
			{
				if(databuf[inflash] == '9')
					databuf[inflash] == '0';
				else
					databuf[inflash] = databuf[inflash]+1;
			}
			if((key == KEY_LEFT) && (inflash != 0xff))
			{
				if(inflash == 0)
					inflash = 7;
				else
					inflash--;
				if(inflash == 4)
					inflash = 3;
			}
			if((key == KEY_RIGHT) && (inflash != 0xff))
			{
				if(inflash == 7)
					inflash = 0;
				else
					inflash++;
				if(inflash == 4)
					inflash = 5;
			}
			digi_flash(0,7,0);
			if(inflash != 0xff)
			{
				digi_print(databuf,0,8);
				digi_flash(inflash,inflash,1);
			}else{
				sprintf(databuf, "%cr    . . .",'1' + (menu_index-3)/4);
				digi_print(databuf,0,8);
			}
			key = 0xff;
		}
		if((menu_index >= 4) && (menu_index  != 7))//1AbC  ....
		{
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
				{
					inflash = (menu_index-3)/4;
					memcpy(prbdata.rtp[inflash],databuf,2);
					memcpy(2+prbdata.rtp[inflash],databuf,6);
					databuf[2] = '.';
					databuf[8] = 0x00;
					inflash = 0;
				}else{
					inflash = (menu_index-3)/4;
					memcpy(databuf,prbdata.rtp[inflash],2);
					memcpy(databuf+3,prbdata.rtp[inflash]+2,6);
					inflash = 0xff;
					if((menu_index % 4) == 0)
						rundata.param1[inflash] = buf2double(databuf);
					if((menu_index % 4) == 1)
						rundata.param2[inflash] = buf2double(databuf);
					if((menu_index % 4) == 2)
						rundata.param3[inflash] = buf2double(databuf);
				}
			}
			if((key == KEY_DN) && (inflash != 0xff))
			{
				if(databuf[inflash] == '0')
					databuf[inflash] = '9';
				else if(databuf[inflash] == '-')
					databuf[inflash] = '+';
				else if(databuf[inflash] == '+')
					databuf[inflash] = '-';
				else 
					databuf[inflash] = databuf[inflash]-1;
			}
			if((key == KEY_UP) && (inflash != 0xff))
			{
				if(databuf[inflash] == '9')
					databuf[inflash] == '0';
				else if(databuf[inflash] == '-')
					databuf[inflash] == '+';
				else if(databuf[inflash] == '+')
					databuf[inflash] == '-';
				else
					databuf[inflash] = databuf[inflash]+1;
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
			{
				if(inflash == 7)
					inflash = 0;
				else
					inflash++;
				if(inflash == 5)
					inflash = 6;
			}
			digi_flash(0,7,0);
			if(inflash != 0xff)
			{
				digi_print(databuf,0,8);
				digi_flash(inflash,inflash,1);
			}else{
				if((menu_index % 4) == 0)
					sprintf(databuf, "%cA    . . .",'1' + (menu_index-3)/4);
				if((menu_index % 4) == 1)
					sprintf(databuf, "%cb    . . .",'1' + (menu_index-3)/4);
				if((menu_index % 4) == 2)
					sprintf(databuf, "%cc    . . .",'1' + (menu_index-3)/4);
				digi_print(databuf,0,8);
			}
			key = 0xff;
		}
		while(key == 0xff)
			key = ReadKey();
	
		if((key == KEY_MODE) || (key == KEY_PROBE))
			break;
		if(key == KEY_OK)
			continue; //let sub handle it
		if((key == KEY_UP) && (inflash == 0xff))
		{
			if(menu_index == 0)
				menu_index = MAX_MENU_INDEX;
			else
				menu_index--;
		}
		if((key == KEY_DN) && (inflash == 0xff))
		{
			if(menu_index == MAX_MENU_INDEX)
				menu_index = 0;
			else
				menu_index++;
		}
		key = 0xff;
		continue;
	}
	menu_index = 0xff;
}
