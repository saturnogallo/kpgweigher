#include <MATH.H>
#include "typedef.h"       
#include "global.h"
#include "utili.h"
#include "key.h"
extern uchar key;         
extern u8 lcd_buffer;    
/*
        Communication with LCD/KBD board,
        Incoming command format:
                CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
                key:            key pressed
        Out command format:
                CMDO_DISPLAYBTN,LED_STATUS      update the led status                
*/
/**************************Ö÷³ÌÐò********************************/
void Key_Init()
{
	key = KEY_INVALID;
}

