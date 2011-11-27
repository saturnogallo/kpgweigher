// global.h    
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.


sfrb PINF=0;
sfrb PINE=1;
sfrb DDRE=2;
sfrb PORTE=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRR0L=9;
sfrb UCSR0B=0xa;
sfrb UCSR0A=0xb;
sfrb UDR0=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb SFIOR=0x20;
sfrb WDTCR=0x21;
sfrb OCDR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrw ICR1=0x26;   // 16 bit access
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  // 16 bit access
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  // 16 bit access
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  // 16 bit access
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb ASSR=0x30;
sfrb OCR0=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TIFR=0x36;
sfrb TIMSK=0x37;
sfrb EIFR=0x38;
sfrb EIMSK=0x39;
sfrb EICRB=0x3a;
sfrb XDIV=0x3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-



        Port related definition
*/ 
void kbd_uart_push(unsigned char);




//#define PORT_3          SPORTA

// Hardware related



// global.h    

// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.



double MValueToTValue(double r,char type);
double GetWr(double t);
double GetT(double w);


void delay (unsigned int us) ;
void delay (unsigned int us) ;
void delay1 (unsigned int ms);
char lowc(unsigned char x);
char highc(unsigned char x);
//'k'
//'l'


 *	Probe data structure definition
 */

{
	double param1[24];
	double param2[24];
	double param3[24];                      
	double rtp[24];
	char  name[24][8];	        //probe serials
	unsigned char type[24];		//probe type
}PRBDATA;

{
	double          R0;  //zero offset
	double          V0;  //zero offset
	double          Rs1; //jiao-zheng zhi
	int             ktime;//time for switch
	unsigned char 	        tid[24];	//probe index of each channel for T mode
	unsigned char           rid[24];        //probe index of each channel for R mode
	unsigned char           prbmode;
	unsigned char           kttmode;                    
}SYSDATA;               

{
	double  reading[24];		  //reading on each channel
	double  temperature[24];	  //temperature result on each channel

	double  stdV;		  //voltage on stdV;
}RUNDATA;

extern SYSDATA eeprom sysdata;
extern PRBDATA eeprom tprbdata;	//probe data for T mode
extern PRBDATA eeprom rprbdata;	//probe data for R mode

void display_buttons(unsigned char pos,unsigned char val);           

int buf2byte();




char* tname2b(unsigned char i);
// CodeVisionAVR C Compiler
// (C) 1998-2006 Pavel Haiduc, HP InfoTech S.R.L.


// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.



void putchar(char c);
void puts(char *str);
void putsf(char flash *str);


void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);



CodeVisionAVR C Compiler
Prototypes for mathematical functions

Portions (C) 1998-2001 Pavel Haiduc, HP InfoTech S.R.L.
Portions (C) 2000-2001 Yuri G. Salov
*/


unsigned int abs(int x);
unsigned long labs(long x);
float fabs(float x);
signed char cmax(signed char a,signed char b);
int max(int a,int b);
long lmax(long a,long b);
float fmax(float a,float b);
signed char cmin(signed char a,signed char b);
int min(int a,int b);
long lmin(long a,long b);
float fmin(float a,float b);
signed char csign(signed char x);
signed char sign(int x);
signed char lsign(long x);
signed char fsign(float x);
unsigned char isqrt(unsigned int x);
unsigned int lsqrt(unsigned long x);
float sqrt(float x);
float floor(float x);
float ceil(float x);
float fmod(float x,float y);
float modf(float x,float *ipart);
float ldexp(float x,int expon);
float frexp(float x,int *expon);
float exp(float x);
float log(float x);
float log10(float x);
float pow(float x,float y);
float sin(float x);
float cos(float x);
float tan(float x);
float sinh(float x);
float cosh(float x);
float tanh(float x);
float asin(float x);
float acos(float x);
float atan(float x);
float atan2(float y,float x);

#pragma library math.lib

{
        double ac,bc,cc,tlow,tup,rnew,tnew;
        int count;
        ac = 3.908e-3;
        bc = -5.775E-7;
        cc = -4.183E-12;
        tlow = -200;
        tup = 850;
	count = 0;
				
	{
		tnew = (tlow+tup)/2.0;
		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
		if(tnew < 0)
		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;
		         
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;

double RValueToTValue(double r, unsigned char prbid)
{
	double ac,bc,cc,tlow,tup,rnew,tnew;
	int count;

	bc = rprbdata.param2[prbid];
	cc = rprbdata.param3[prbid];

		return PT100RToTValue(r, cc);
	if(rprbdata.type[prbid] != 0xf2)
		return -9999.999;
        if(rprbdata.rtp[prbid] > 0.1)
                r = r/rprbdata.rtp[prbid];
        else
		r = r/25.0;

	tlow = GetT(r) - 1; 
	tup = tlow + 2;

				
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetWr(tnew);      
		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*log(rnew);
                else
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;

}

flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};			 
flash unsigned char sizeAi = sizeof(Ai)/sizeof(double);
flash unsigned char sizeCi = sizeof(Ci)/sizeof(double);

{
	double result;
	double pert;
	int i;
	if(t < 0)
	{       //equals to Wr189(t) in mfile
		result = Ai[0];
		pert = (log((t+273.15)/273.16)+1.5)/1.5;
		t = pert;
		for(i=1;i<sizeAi;i++)
		{
			result = result + Ai[i] * t;
			t = t*pert;
		}
		result = exp(result);
	}else{
			 result = Ci[0];
			 pert = t/481.0 - 1;
			 t = pert;
			 for(i=1;i<sizeCi;i++)
			 {
				 result = result + Ci[i] * t;
				 t = t*pert;
			 }
	}
	return result;
}
flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
flash unsigned char sizeDi = sizeof(Di)/sizeof(double);
flash unsigned char sizeBi = sizeof(Bi)/sizeof(double);

{
	double perw;
	double result;
	int i;

	{       //t660r
		perw = (w-2.64)/1.64;
		w = perw;
		result = Di[0];
		for(i=1;i<sizeDi;i++)
		{
			result = result + Di[i] * w;
			w = w*perw;
		}
	}else{   //t189(r)
		perw = (pow(w,1.0/6.0) - 0.65)/0.35;
		w = perw;
		result = Bi[0];
		for(i=1;i<sizeBi;i++)
		{
			result = result + Bi[i] * w;
			w = w*perw;
		}		
		result = 273.15*result - 273.15;
	}
	return result;
}

flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};

flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};


flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};

flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};

flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};

flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};

flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};	

flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};

flash int THighLen = sizeof(THigh)/sizeof(double);

flash int SMedLen = sizeof(SMed)/sizeof(double);
flash int SHighLen = sizeof(SHigh)/sizeof(double);

flash int RMedLen = sizeof(RMed)/sizeof(double);
flash int RHighLen = sizeof(RHigh)/sizeof(double);

flash int NHighLen = sizeof(NHigh)/sizeof(double);

flash int JHighLen = sizeof(JHigh)/sizeof(double);

flash int BHighLen = sizeof(BHigh)/sizeof(double);

flash int EHighLen = sizeof(EHigh)/sizeof(double);

flash int KHighLen = sizeof(KHigh)/sizeof(double);


{
	double result,pert;
	int i;
	int len = 0;
	switch(type)
	{
		case 'T': 	
			coef = (t < 0) ? TLow : THigh;
			len = (t < 0) ? TLowLen : THighLen;
			break;
		case 'K': 	
			coef = (t < 0) ? KLow : KHigh;
			len = (t < 0) ? KLowLen : KHighLen;
			break;
		case 'N': 	
			coef = (t < 0) ? NLow : NHigh;
			len = (t < 0) ? NLowLen : NHighLen;
			break;
		case 'E': 	
			coef = (t < 0) ? ELow : EHigh;
			len = (t < 0) ? ELowLen : EHighLen;
			break;
		case 'B': 	
			coef = (t < 630.615) ? BLow : BHigh;
			len = (t < 630.615) ? BLowLen : BHighLen;
			break;
		case 'J': 	
			coef = (t < 760) ? JLow : JHigh;
			len = (t < 760) ? JLowLen : JHighLen;
			break;

			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
			break;
		case 'R': 	
			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
			break;
		default:
			return 0.0;
	}
	if (len == 0)
		return 0.0;

	if(type == 'K')
	{
		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	}

	for(i=1;i< len ;i++)
	{
		result = result + coef[i] * t;
		t = t*pert;
	}
	return result;
}

{
	double tlow;
	double tup;
	double rnew;
	double tnew;
	int count = 0;

	{
		case 'T': 	
			tlow =  -270;	tup = 400;
			break;
		case 'K': 	
			tlow =  -270;	tup = 1372;
			break;
		case 'N': 	
			tlow =  -270;	tup = 1300;
			break;
		case 'E': 	
			tlow =  -270;	tup = 1000;
			break;
		case 'B': 	
			tlow =  0;	tup = 1280;
			break;
		case 'J': 	
			tlow =  -210;	tup = 1200;
			break;
		case 'S': 	
			tlow =  -50;	tup = 1768;
			break;
		case 'R': 	
			tlow =  -50;	tup = 1768;
			break;
		default:
			return -9999.9999;
	}	

	{
		tnew = (tlow+tup)/2.0;
		rnew = GetThmoVolt(tnew,type);
		
			tup = tnew;
		else
			tlow = tnew;
	}
				
}
