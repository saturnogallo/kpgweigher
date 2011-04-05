#ifndef __UTILI_H
#define __UTILI_H


#include "typedef.h"
/*
	Command Functions
 */
void delay (uint us) ;
void delay1 (uint ms);
uchar strlen(uchar *s);
char lowc(uchar x);
char highc(uchar x);


#define PI	3.1415926
/* 	
	Key Definitions
 */
#define KEY_INVALID '-'													  
#define KEY_NUM0	'0'
#define KEY_NUM1	'1'
#define KEY_NUM2	'2'
#define KEY_NUM3	'3'
#define KEY_NUM4	'4'
#define KEY_NUM5	'5'
#define KEY_NUM6	'6'
#define KEY_NUM7	'7'
#define KEY_NUM8	'8'
#define KEY_NUM9	'9'
#define KEY_DOT		'.'
#define KEY_UP		'U'
#define KEY_DN		'D'
#define KEY_CE		'C'
#define KEY_OK		'O'
#define KEY_TAB		'T'
#define KEY_BTN1	'a'
#define KEY_BTN2	'b'
#define KEY_BTN3	'c'
#define KEY_BTN4  	'd'

#define CLR_BTN1	'e'
#define CLR_BTN2	'f'
#define CLR_BTN3	'g'
#define CLR_BTN4	'h'

#define KEY_KTT		'i'
#define KEY_RS		'j'
//'k'
//'l'
#define CLR_KTT		'm'
#define CLR_RS		'n'
//'o'
//'p'
#endif
