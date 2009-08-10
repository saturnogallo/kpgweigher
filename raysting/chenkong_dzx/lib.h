#include "reg52.h"

#define BYTE unsigned char
#define u8	BYTE
#define u32    unsigned long
#define u16    unsigned int
#define i32	   long
#define BOOL unsigned char
#define FALSE	0
#define TRUE	1

#define LED_NOSHOW_INDEX 0x0a
/*********************************
 *	Led related definitions
 ********************************/
typedef struct	_led_info {
	u8 number; 	//number of leds
	/*
	 *	Arry store the flash and sel info
	 *	high 4 bits : the index of currently selected led index, 
	 *	low 4 bits  : flash is the flash count
	 */
	u8* m_flash;
	/* 
	 *   Array represent each word. Size should be the sum of numbers of each led type 
	 *	 [typeA.number+typeB.number+...]
	 *	 bit 7: means whether it has point
	 *	 bit 6: means whether it flashes
	 *	 bit 5 to bit 0: index in m_code
	 */
	u8* m_leddata;

	u16 m_flash_msec; //number of millisecond for flash
	u8 	m_flash_count;	//stop the led flash after  flashed FLASH_COUNT times
	u8* m_code;			//code list for the code translation
									//define led_code index: 0-9 (ascii '0'->'9') 10 (no show)
	u8  m_point_code;	//code for digit point
}LED_INFO;

extern void led_outputA(u8 pos,u8 dat);
#define led_output	led_outputA

extern void ledout_refresh(LED_INFO code *info) reentrant;
extern void led_print(LED_INFO code *info,u8 *str,u8 size);


sbit	pkbd_up = P1^3; 
sbit	pkbd_dn = P1^4;
sbit	pkbd_cu = P1^0;
sbit	pkbd_pt = P1^1;
sbit	pkbd_1k = P1^2;

#define PKBD_CH_UP	0x01
#define PKBD_CH_DN	0x02
#define PKBD_CH_CU	0x08
#define PKBD_CH_PT	0x10
#define PKBD_CH_1K	0x20

#define pkbd_has_key(A)	(pkey_id & A)

extern u8 PKeyboard_Update();

#define SP_BUFFER_MAX	6

extern 	void HW1SerialInit();
extern BOOL HW1SerialDataWaiting();
extern void HW1SerialSendByte(BYTE udata);
extern u8 HW1SerialWaitForOneByte(void);
