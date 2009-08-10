//#include "reg51.h" 
#include "stdio.h"
#include "sjSerial.h"

//#define MYDEBUG
#define DBG	sjSerialSendByte
#define tx_byte DBG
void tx_byte2(unsigned char a);
//the length of the switch table,
/*
	row 4 column 8
		P0		

P1      col0    col1     col2    col3    col4   col5   col6    col7   
		----    ----     ----    ----    ----   ----   ----    ----
row0_on | 0|    | 1|     | 2|    | 3|    | 4|   | 5|   | 6|    | 7|
		| A|     | B|    | C|    | D|   | E|   | F|    | G|    | H|
row1_off| 8|    | 9|     |10|    |11|    |12|   |13|   |14|    |15|
		----    ----     ----    ----    ----   ----   ----    ----

		----    ----     ----    ----    ----   ----   ----    ----
row2_on |16|    |17|     |18|    |19|    |20|   |21|   |22|    |23|
		| I|     | J|    | K|    | L|   | M|   | N|    | O|    | P|
row3_off|24|    |25|     |26|    |27|    |28|   |29|   |30|    |31|
		----    ----     ----    ----    ----   ----   ----    ----

		----    ----     ----    ----    ----   ----   ----    ----
row4_on |32|    |33|     |34|    |35|    |36|   |37|   |38|    |39|
		| Q|     | R|    | S|    | T|   | U|   | V|    | W|    | X|
row5_off|40|    |41|     |42|    |43|    |44|   |45|   |46|    |47|
		----    ----     ----    ----    ----   ----   ----    ----
		
		----    ----     ----    ----    ----   ----   ----    ----
row6_on |48|    |49|     |50|    |51|    |52|   |53|   |54|    |55|
		| Y|     | Z|    | a|    | b|   | c|   | d|    | e|    | f|
row7_off|56|    |57|     |58|    |59|    |60|   |61|   |62|    |63|
		----    ----     ----    ----    ----   ----   ----    ----
		
*/

#define  LINIT 32 

#define FALSE	0
#define TRUE	1
/********************
*Parallel keyboard scan
*	NOTE: 
*		: each bit represent a key, when pressed , it will turn to 0
*		: the press key value will be stored in pkey_id,
*		: DELAY_TIME*timer_period = time to erase the disturbance
*********************/
#define PKEY_DELAY_TIME 1

// loop of check the key id
// return none zero if there is a valid key id
// return 0xff if there is no valid keyid
BOOL _pkbd_check();
unsigned char _pkbd_keyid();
unsigned char pkeyboard_update()
{
      /* scan the keyboard */
	  static unsigned char processed=0;
	  static unsigned char pkey_delay=0;
	  if(FALSE == _pkbd_check())      
      {
         pkey_delay = 0;
		 processed = 0;
		 return 0xff;
      }
	  //there is a key press down
      pkey_delay++;
      if( (pkey_delay > PKEY_DELAY_TIME) && !processed) 
      {
	  		//there is really a key press down
            pkey_delay = 0;
			processed = 1;
			return _pkbd_keyid();
   	}
	return 0xff;
}

/*
	Relay board related function
	P1 is row, P0 is column
	void swiAction(unsigned char SwiID) //turn around the switch
	void swiReset()
*/

//for P1 output
//for P0 output
const unsigned char code tbl[8]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};

//for Init use(regaddr of pc)
//0 = off, 1 = on , 0xff = not exist
const BOOL code inittbl[LINIT]={
	0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,//A,B,C,D,E,F,G,H
	0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,//I,J,K,L,M,N,O,P,
	0xff,0xff,0xff,0x00,0x00,0xff,0xff,0x00,//Q,R,S,T,U,V,W,X,
	0xff,0xff,0xff,0x00,0x00,0xff,0xff,0x00,//Y,Z,a,b,c,d,e,f,

};
#define MATCH_NUM	10
const BOOL code matchtbl[2*MATCH_NUM] = {
	'D'-'A','E'-'A','L'-'A','M'-'A','T'-'A','U'-'A','X'-'A','b'-'a'+26,'c'-'a'+26,'f'-'a'+26,
	'G'-'A','F'-'A','O'-'A','N'-'A','W'-'A','V'-'A','P'-'A','e'-'a'+26,'d'-'a'+26,'H'-'A'};

BOOL statustbl[LINIT];

//delay loop
void _swi_delay(unsigned char dl1,unsigned char dl2)
{
	unsigned char i,j;
	for(i=0;i<dl1;i++)
		for(j=0;j<dl2;j++)
			;
}

void swiAction2(unsigned char SwiID);
unsigned char lastID = 0xff;
void swiAction(unsigned char SwiID)
{
	if(lastID == SwiID)
		return;
	if(statustbl[SwiID] == 0xff)
		return;

	swiAction2(lastID); //turn off the lastID
	swiAction2(SwiID);	//turn on the SwiID
}
void swiAction2(unsigned char SwiID)
{
	unsigned char i;
	//SwiID is from 0 to 31
	if(SwiID >= LINIT)
		return;
	
	statustbl[SwiID] = 1 - statustbl[SwiID];

	//low 3 bit represent the row
	//high 3 bit represent the col
	P0 = tbl[(SwiID&0x07)];
	P1 = tbl[((SwiID>>2)&0x0E) + 1 - statustbl[SwiID]];
	
	_swi_delay(0x1f,0xff);
	P1 = 0xff;
	P0 = 0xff;
	_swi_delay(0x03,0xff);

	lastID = SwiID;
	//turn the match point
	
	for(i=0;i<MATCH_NUM;i++)
	{
		if(SwiID == matchtbl[i])
			break;
	}
	if(i == MATCH_NUM)
		return;
	i = matchtbl[i+10];
	P0 = tbl[(i&0x07)];
	P1 = tbl[((i>>2)&0x0E) + 1 - statustbl[SwiID]];
	
	_swi_delay(0x1f,0xff);
	P1 = 0xff;
	P0 = 0xff;
	_swi_delay(0x03,0xff);
	
}

void swiReset()
{
	unsigned char i;
	for(i=0;i<LINIT;i++)
	{
		statustbl[i] = inittbl[i];
		if(statustbl[i] != 0xff)
		{
			statustbl[i] = 1 - statustbl[i]; //turn over the status
			swiAction2(i);
		}
	}
	lastID = 0xff;
}

unsigned char gkeyid;
void timer0(void) interrupt 1
{
	unsigned char m_key;
	m_key = pkeyboard_update();
	if(0xff != m_key)
	{
		gkeyid = m_key;
		return;
	}
}
sbit EKEY10	= P3^7;
//check whether there is any key pressed 
//return true if there is no key pressed
//return false if there is key pressed
BOOL _pkbd_check()
{
    unsigned char port;
	port = P2 & 0xf0;
	if(port != 0xf0)
		return TRUE;
	if(!EKEY10)
		return TRUE;

	return FALSE;
}
//high 4 bit and EKEY10 represent the key
//get the key id
// return none zero if there is a valid key id
// return 0 if there is no valid keyid
unsigned char _pkbd_keyid()
{
	unsigned char pkey_id = 0;
	unsigned char port;
	unsigned char i;

	port = 0x0f -((P2 & 0xf0)>>4);
	if(!EKEY10)
	 	port = 0;

	for(i = 0;i < LINIT; i++)
	{
		if(statustbl[i] == 0xff)
			continue;
		if(port == pkey_id++)
			return i;
	}
	return 0xff;
}
void tx_byte2(unsigned char a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		tx_byte(h+'0');
	else
		tx_byte(h+'A'-0x0a);

	if(l <= 9)
		tx_byte(l+'0');
	else
		tx_byte(l+'A'-0x0a);

}

void initiate_timer(void)
{
   //set serial port parameter (clock 11.0592M)
   //9600 baut rate 8 data non parity and 1 stop.
   SCON = 0x70;
   PCON = 0x00;

   //use timer 1 to be serial
   //timer count
   TH1 = 0xfd;

   //use timer 0 to be heart beat
   TH0 = -4608/256;   // 4608 , 4608/(11.0592M/12) = 0.005s = 5ms
   TL0 = -4608%256;

   TMOD = 0x21;
   TR1 = 1;

   ET0=1;
   EA=1;
   TR0 = 1;
}

void ledUpdate()
{
	unsigned char _p2;
	unsigned char i,j;
	_p2 = P2;
	j = 0;
	for(i = 0;i < LINIT; i++)
	{
		if(statustbl[i] == 0xff)
			continue;
		if(statustbl[i] == 1)		
		{
			//replace the low 4 bit with the id of the key
			_p2 = (_p2 & 0xf0);
			_p2 = _p2+(9-j);
			P2 = _p2;
			return;
		}
		j++;
	}
	//all unset case
	_p2 = (_p2 & 0xf0);
	_p2 = _p2+10;
	P2 = _p2;
	return;
	
}
//the MARK of header ender and error and quest and reset
#define  HMARK 'U'

#define  SID_BUTTON	'B'

void main(void) 
{
	unsigned char bHFound;
  	unsigned char inchar;
	unsigned char sid; //id of function

    IE = 0;//close int
	sjSerialInit(); //init buffer for serial
    /* Initiate timer */
    initiate_timer();
    swiReset();
    ledUpdate();
    IE = 0x92;//enable serial int and timer0 interrupt

    gkeyid = 0xff;


	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/
	

  	/* The major loop */ 
	bHFound = FALSE;
    inchar = 0xff;
	sid = 0xff;
	while(1)
	{
		if(sjSerialIsDataWaiting())
		{
			inchar = sjSerialWaitForOneByte();
			if(bHFound)
			{
				bHFound = FALSE;
				if(inchar != HMARK)
				{
					sid = inchar;
					continue;
				}
			}else{
				if(inchar == HMARK)
				{
					bHFound = TRUE;
					continue;
				}
			}
			if(sid == SID_BUTTON)//click button
			{
				switch(inchar)
				{
				case '*': //reset
						swiReset();
						break;
				case '?': //query
						if(EKEY10)	
							DBG('1'); 
						else
							DBG('0');
						DBG(',');
						tx_byte2(P2);DBG(',');DBG('|');
						for(inchar = 0;inchar < LINIT; inchar++)
						{
							if(statustbl[inchar] == 0xff)
								continue;
							DBG('0'+statustbl[inchar]);DBG(',');
						}
						DBG('\n');
						break;
#ifdef MYDEBUG
				case '0':
						EKEY10 = 0;
						break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
						inchar = ((0x0f-inchar) <<4 ) + (P2 & 0x0f);
						P2 = inchar;
#endif
						break;

				default: //click ('A' - 'Z' , 'a'-'z')
						if(inchar > 'Z')
							inchar = inchar - 'a'+26;
						else
							inchar = inchar - 'A';
						if(inchar < LINIT)
						{
							swiAction(inchar);
							ledUpdate();
						}
				}
			}
		}
	   	if(gkeyid != 0xff)
		{
			//a key is found
			swiAction(gkeyid);
			ledUpdate();

			gkeyid = 0xff;
#ifdef MYDEBUG
			inchar = P2 | 0xf0;
			P2 = inchar;
			EKEY10 = 1;
#endif
		}
   }
/***********************/
} 



  


