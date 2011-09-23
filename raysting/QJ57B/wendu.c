#include "global.h"
#include "wendu.h" 
#include "utili.h"
#include "math.h"
#define logl log
#define powl pow
double PT100RToTValue(double r,double r0)
{
        double ac,bc,cc,tlow,tup,rnew,tnew;
        int count;
        ac = 3.908e-3;
        bc = -5.775E-7;
        cc = -4.183E-12;
        tlow = -200;
        tup = 850;
	count = 0;
				
	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
		if(tnew < 0)
		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;
		         
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;

}
double RValueToTValue(double r, u8 prbid)
{
	double ac,bc,cc,tlow,tup,rnew,tnew;
	int count;

	ac = rprbdata.param1[prbid];
	bc = rprbdata.param2[prbid];
	cc = rprbdata.param3[prbid];

	if(rprbdata.type[prbid] == PRBTYPE_PT100)
		return PT100RToTValue(r, cc);
	if(rprbdata.type[prbid] != PRBTYPE_PT25)
		return -9999.999;
        if(rprbdata.rtp[prbid] > 0.1)
                r = r/rprbdata.rtp[prbid];
        else
		r = r/25.0;




	
	//set the search range of T between GetT(r) +/- 1 degree
	tlow = GetT(r) - 1; 
	tup = tlow + 2;

	count = 0;
				
	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetWr(tnew);      
		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
                else
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;

			
//	return r+1;
}

flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};			 
flash u8 sizeAi = sizeof(Ai)/sizeof(double);
flash u8 sizeCi = sizeof(Ci)/sizeof(double);

double GetWr(double t)
{
	double result;
	double pert;
	int i;
	if(t < 0)
	{       //equals to Wr189(t) in mfile
		result = Ai[0];
		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
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
flash u8 sizeDi = sizeof(Di)/sizeof(double);
flash u8 sizeBi = sizeof(Bi)/sizeof(double);

double GetT(double w)
{
	double perw;
	double result;
	int i;

	if(w >= 0)
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
		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
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

flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};

flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};

flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};

flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};

flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};

flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};

flash double BLow[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};

flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};	
flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};	

flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};

flash int TLowLen = sizeof(TLow)/sizeof(double);
flash int THighLen = sizeof(THigh)/sizeof(double);

flash int SLowLen = sizeof(SLow)/sizeof(double);
flash int SMedLen = sizeof(SMed)/sizeof(double);
flash int SHighLen = sizeof(SHigh)/sizeof(double);

flash int RLowLen = sizeof(RLow)/sizeof(double);
flash int RMedLen = sizeof(RMed)/sizeof(double);
flash int RHighLen = sizeof(RHigh)/sizeof(double);

flash int NLowLen = sizeof(NLow)/sizeof(double);
flash int NHighLen = sizeof(NHigh)/sizeof(double);

flash int JLowLen = sizeof(JLow)/sizeof(double);
flash int JHighLen = sizeof(JHigh)/sizeof(double);

flash int BLowLen = sizeof(BLow)/sizeof(double);
flash int BHighLen = sizeof(BHigh)/sizeof(double);

flash int ELowLen = sizeof(ELow)/sizeof(double);
flash int EHighLen = sizeof(EHigh)/sizeof(double);

flash int KLowLen = sizeof(KLow)/sizeof(double);
flash int KHighLen = sizeof(KHigh)/sizeof(double);

flash double *coef;

double GetThmoVolt(double t,char type)
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

		case 'S': 	
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

	result = coef[0];
	if(type == 'K')
	{
		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	}

	pert = t;
	for(i=1;i< len ;i++)
	{
		result = result + coef[i] * t;
		t = t*pert;
	}
	return result;
}

double MValueToTValue(double r,char type)
{
	double tlow;
	double tup;
	double rnew;
	double tnew;
	int count = 0;

	switch(type)
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

	while((tup - tlow > 0.00005) || (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetThmoVolt(tnew,type);
		
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
				
	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
}

