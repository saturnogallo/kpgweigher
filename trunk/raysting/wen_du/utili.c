#include "utili.h"
#include "stc51.h"
sbit SPI_CK = P3^5;
sbit SPI_DI1 = P3^4;
sbit SPI_DI2 = P0^5;
sbit SPI_DO = P3^3;

/*---------------延时子程序----------------*/
void delay (uint us) 
{
  	while(us--);
}
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;
		
}
char lowc(uchar x)
{
	x = x&0x0f;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
char highc(uchar x)
{

	x = x>>4;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
//计算字符串长度
uchar strlen(uchar *s)
{
	uchar len = 0;
	while(*s++) len ++;
	
	return len;
}
double buf2double()		
{
	double tmp = 0.0;
	uchar i = 0;
	uchar pos = 0;
	for(i=0;i<pos_databuf;i++) {
		if(databuf[i] != KEY_DOT)
			tmp = tmp * 10.0+(databuf[i] - '0');
		else
			pos = pos_databuf - i - 1;
	}
	while(pos > 0) {
		tmp = tmp / 10.0;
		pos--;
	}
	return tmp;
}
int buf2byte()	    //convert rdata.tempbuf to byte (00-99)
{
	int tmp = 0;
	uchar i;
	for(i=0;i<pos_databuf;i++) {
		tmp = tmp * 10+(databuf[i] - '0');
	}
	return tmp;
}

//MY SPI Related function 
uchar xdata ch1buf[8];
//uchar xdata ch2buf[8];
#define SM_WDELAY	100
#define SM_RDELAY	100
#define SM_RDELAY2	10
#define HEAD_MARK	0xcc	//header mark
void sm_init()
{	
	uchar i;
	i = SPI_DI1; //set to input
	i = SPI_DI2; //set to input
}
void   sm_write(uchar   value)  
{  
	uchar   no;  
  	for(no=0;no<8;no++) {  
	       SPI_CK = 1;      
		 
   	  	if   ((value &0x80)==0x80)  
       		  SPI_DO = 1;  
		else  
		      SPI_DO = 0;  
		 delay(SM_WDELAY);   
	     SPI_CK = 0;    
   		 value   =   (value <<1);  
		delay(SM_WDELAY);
  	}
	SPI_CK = 1;
}  
void sm_read(uchar pos,uchar ch)
{  
   	uchar   no,value1,value2;  
   
 	for (no=0;no<8;no++) 	{ 
		  SPI_CK = 1;  
		  delay(SM_RDELAY);		
		  if( ch & HASCH1)
		          value1   =   (value1   <<1);  
		  if( ch & HASCH2)
			  value2   =   (value2   <<1);  
         	  SPI_CK = 0;  
		  delay(SM_RDELAY2);
		  if( ch & HASCH1){
	 		  if (SPI_DI1 == 1)  
		        	value1  |=0x01;  
  			  else  
				value1  &=~0x01;  
		  }
		  if( ch & HASCH2) {
	 		  if (SPI_DI2 == 1)  
			        value2  |=0x01;  
  			  else  
				value2  &=~0x01;  
		  }
	}
	SPI_CK = 1;
	if( ch & HASCH1)
		ch1buf[pos] = value1;
 }  


/*********************************************************************/
/*                                                                   */
/*   Copyright Agilent Technology  1996-2000                         */
/*   all rights reserved                                             */
/*                                                                   */
/*********************************************************************/

#include	"stdarg.h"

#define   fSP  0x01
#define   fPL  0x02
#define   fMI  0x04
#define   fNO  0x08
#define   fZE  0x10
#define   WMAX   99

static
int sjstrlen(const char *s1)
{
	char *s2 = (char *)s1;
	while (*s2) s2++;
	return (s2 - s1);
}


static
int _sjPrintf(char *p, const char *fmt, va_list ap)
{
static const char code ldig[] = "0123456789abcdef";
static const char code udig[] = "0123456789ABCDEF";

	char ac[24];
	char  *s, *s1;
	const char *dig;
	short flags, width, prec, nchar;
	char  qual;
	short n0, n1, nz0;
	int   vi;
	unsigned int vu;

        nchar = 0;
	while (*fmt != '\0') {
          if (*fmt == '%') {
	    fmt++;
	    for (flags = 0; ;fmt++)
	      if (*fmt == ' ') flags |= fSP;
	      else if (*fmt == '+') flags |= fPL;
	      else if (*fmt == '-') flags |= fMI;
	      else if (*fmt == '#') flags |= fNO;
	      else if (*fmt == '0') flags |= fZE;
	      else break;
	    for (width = 0; (*fmt >= '0') && (*fmt <= '9'); fmt++)
	      if (width < WMAX) width = (width * 10) + *fmt - '0';
	    if (*fmt == '.') {
	      fmt++;
	      for (prec = 0; (*fmt >= '0') && (*fmt <= '9'); fmt++)
	        if (prec < WMAX) prec = (prec * 10) + *fmt - '0';
	    } else prec = -1;
	    if ((*fmt == 'h') || (*fmt == 'l') || (*fmt == 'L')) qual = *fmt++;
	    else qual = '\0';
	    s = ac;  s1 = s + 24;  *--s1 = '\0';  n0 = n1 = nz0 = 0;
	    if (*fmt == 'c') {
	      *s++ = va_arg(ap, int);  n0 = 1; }
	    else if ((*fmt == 'i') || (*fmt == 'd')) {
	      vi = va_arg(ap, int); 
	      if (vi < 0) {*s++ = '-';  n0 = 1; vi = -vi; }
	      else if ((flags & fPL) != 0) {*s++ = '+';  n0 = 1; }
	      else if ((flags & fSP) != 0) {*s++ = ' ';  n0 = 1; };
	      do {
		*--s1 = ldig[vi % 10];  vi = vi / 10;  n1++; } while (vi > 0);
	      goto prn1; }
	    else if (*fmt == 'u') {
	      vu = va_arg(ap, int); 
	      if (qual == 'h') vu = vu & 0x0ffff;
	      do {
		*--s1 = ldig[vu % 10];  vu = vu / 10;  n1++; } while (vu > 0);
	      goto prn1; }
	    else if ((*fmt == 'x') || (*fmt == 'X')) {
	      vu = va_arg(ap, int);  if (*fmt == 'x') dig = ldig; else dig = udig;
	      if (qual == 'h') vu = vu & 0x0ffff;
	      if ((flags & fNO) != 0) {*s++ = '0'; *s++ = *fmt; n0 = 2; };
	      do {
		*--s1 = dig[vu & 0x0f];  vu = vu >> 4;  n1++; } while (vu > 0);
   prn1:      if (n1 < prec) nz0 = prec - n1;
	      if (prec < 0 && flags & fZE && width > (n0 + nz0 + n1))
		 nz0 = width - (n0 + nz0 + n1); }
	    else if (*fmt == 's') {
	      s1 = va_arg(ap, char *);  n1 = sjstrlen(s1);
	      if (0 <= prec && prec < n1) n1 = prec; }
	    else {*s++ = *fmt;  n0 = 1;};
	    width -= n0 + nz0 + n1;
	    if ((flags & fMI) == 0) for (; width>0; --width) {*p++ = ' '; nchar++; };
	    for (s = ac; n0 > 0; --n0) {*p++ = *s++; nchar++; };
	    for (; nz0 > 0; --nz0) {*p++ = '0'; nchar++; };
	    for (; n1 > 0; --n1) {*p++ = *s1++; nchar++; };
	    if ((flags & fMI) != 0) for (; width>0; --width) {*p++ = ' '; nchar++; };
	  }
	  else {*p++ = *fmt; nchar++; };
	  fmt++;
	};
	*p = '\0';
	return nchar;
}

int sjprintf(char *s, const char *fmt, ...)
{
        va_list ap; int i;
        va_start(ap, fmt);
        i = _sjPrintf(s, fmt, ap);
        va_end(ap);
	return i;
}

