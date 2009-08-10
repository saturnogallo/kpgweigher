#include "lib.h"
/*
void sleepus(BYTE j){
 while(j-- > 0);
}

void sleepms(BYTE j){
 BYTE i;
 while(j-- > 0){
 	i = 255;
	while(i-- > 0) ;
 }
}
void sleeps(BYTE j){
 while(j-- > 0){
 	sleepms(255);
 }
}
*/

void ledout_refresh(LED_INFO code *info) reentrant
{
	u8 dat,i,j,tick;
	tick = 0;
	do{
		for(i = 0;i < (*info).number;i++)
		{
			dat = (*info).m_leddata[i];
			j = dat & 0xC0;
			dat = (*info).m_code[0x3f & dat];

			if( ( ((*((*info).m_flash))>>4) == i) && ( ((*((*info).m_flash))&0x0f) > 0) && (dat & 0x40))
			{
				if(tick < (*info).m_flash_msec)
					dat = (*info).m_code[LED_NOSHOW_INDEX];

				if(tick > ((*info).m_flash_msec << 1)){
					(*((*info).m_flash))--;
					tick = 0;
				}
				tick++;
			}
			if(j & 0x80)	//handle point case
				dat &= (*info).m_point_code;
			else
				dat |= ~(*info).m_point_code;
			led_output(i,dat);
		}
	}while(0);
}

void led_print(LED_INFO code *info,u8 *str,u8 size) 
{
	if(size > (*info).number)
		return;
	size=(*info).number-size;
	while(size < (*info).number){
		if(((*str) >= '0') && ((*str) <= '9')){
			(*info).m_leddata[size] = ((*info).m_leddata[size] & 0x40) | (*str)-'0'; //reserve flash flag
		}else{
			 if((*str) == '-'){
					(*info).m_leddata[size] = ((*info).m_leddata[size] & 0x40) | 0x0b; //reserve flash flag;
			 }else{
		 		    if((*str) == '.'){
							if(size > 0){
								(*info).m_leddata[size-1] |= 0x80; //set point flag to 1
								size--;
							}
					}else{
						(*info).m_leddata[size] = LED_NOSHOW_INDEX; //no show
					}
			}
		}
		str++;
		size++;
	}
}

/**********************Main starts here******************/
/*********************************************************
*Parallel keyboard scan
*	NOTE: 
*		: each bit represent a key, when pressed , it will return value 
*		: 
**********************************************************/
u8 _pkbd_check()
{
	u8 i;
	i = 0;
	if(0 == pkbd_up)	i |= PKBD_CH_UP;
	if(0 == pkbd_dn)	i |= PKBD_CH_DN;
	if(0 == pkbd_cu)	i |= PKBD_CH_CU;
	if(0 == pkbd_pt)	i |= PKBD_CH_PT;
	if(0 == pkbd_1k)	i |= PKBD_CH_1K;

	return i;
}

u8 pkey_id = 0;
u8 p_processed=0;
u16 pkbd_timer = 0;
#define PKBD_REPEAT	24000	//300 ms to ensure a repeat button
#define PKBD_WAIT	20	//30 ms to remove noise
#define PKBD_PROCESS_IDLE	0
#define PKBD_PROCESS_DONE	1
#define PKBD_PROCESS_WAIT	2
u8 PKeyboard_Update()
{
    /* scan the keyboard */
	pkey_id = _pkbd_check();
	if(pkey_id == 0)  {
			p_processed = PKBD_PROCESS_IDLE;
			pkbd_timer = 0;
			return 0;
  	}
  	if(p_processed == PKBD_PROCESS_DONE)
  	{
		if(pkbd_timer > 0)
			return 0;
		pkbd_timer = PKBD_REPEAT;
  		p_processed = PKBD_PROCESS_IDLE;
		return 0;
  	}
    //there is a key press down
    if((pkbd_timer == 0) && (p_processed == PKBD_PROCESS_IDLE)){
		pkbd_timer = PKBD_WAIT;
		p_processed = PKBD_PROCESS_WAIT;
   		return 0;
	}
	if(pkbd_timer > 0)
		return 0;
	if(p_processed != PKBD_PROCESS_WAIT)
		return 0;

    if(pkey_id == _pkbd_check()) 
    {		//there is really a key press down
		p_processed = PKBD_PROCESS_DONE;
		pkbd_timer = 0;
		return pkey_id;
   	}
}




