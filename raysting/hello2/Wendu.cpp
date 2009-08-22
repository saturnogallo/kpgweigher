#include "math.h"
#include "Wendu.h"

const int acoef_pos[] = {0, 3, 5, 7, 8, 9,11};	
const int bcoef_pos[] = {1, 4, 6,-1,-1,10,12};	
const int ccoef_pos[] = {2,-1,-1,-1,-1,-1,-1};	

const int coef_pos[] = {0,1,2,3,4,-1,5,6,-1,7,-1,-1,8,-1,-1,9,10,-1,11,12,-1};

void SaveABCcoef(int range,double w_tp, double acoef, double bcoef, double ccoef, double *coef)
{
	if((range < 0) || (range >= (sizeof(acoef_pos)/sizeof(int))))
		return;
	coef[coef_pos[range*3]] = acoef;
	if((coef_pos[range*3+1] >= 0))
		coef[coef_pos[range*3+1]] = bcoef;

	if((coef_pos[range*3+2] >= 0))
		coef[coef_pos[range*3+2]] = ccoef;
	coef[COEF_RANGE_POS] = range + 0.01;
	coef[COEF_VALUE_RTP] = w_tp;
}
void GetABCcoef(int range, double &w_tp, double &acoef, double &bcoef, double &ccoef, double *coef)
{
	if(!coef){
		return;
	}
	acoef = bcoef = ccoef = 0.0;
	if(acoef_pos[range] >= 0)
		acoef = coef[acoef_pos[range]];
	
	if(bcoef_pos[range] >= 0)
		bcoef = coef[bcoef_pos[range]];
	
	if(ccoef_pos[range] >= 0)
		ccoef = coef[ccoef_pos[range]];
	
	w_tp = coef[COEF_VALUE_RTP];

}
double RValueToTValue(double r,const double coef[])
{
	int range = (int)floor(coef[COEF_RANGE_POS]);

	if((range < 0) || (range >= (sizeof(acoef_pos)/sizeof(int))))
		return 0.0;
	r = r/coef[COEF_VALUE_RTP];
	double ac,bc,cc;
	ac = coef[acoef_pos[range]];
	bc = (bcoef_pos[range] >= 0) ? coef[bcoef_pos[range]] : 0;
	cc = (ccoef_pos[range] >= 0) ? coef[ccoef_pos[range]] : 0;

	
	//set the search range of T between GetT(r) +/- 1 degree
	double tlow = GetT(r) - 1; 
	double tup = tlow + 2;

	double rnew;
	double tnew;
	int count = 0;
				
	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetWr(tnew);
		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
			
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	
}


const double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
const double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};

const double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
const double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
const double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};

const double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};

const double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
const double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};

const double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
const double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};

const double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
const double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};

const double BLow[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
const double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};

const double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};	
const double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};	

const double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
const double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};

double GetThmoVolt(double t,char type)
{

	int TLowLen = sizeof(TLow)/sizeof(double);
	int THighLen = sizeof(THigh)/sizeof(double);

	int SLowLen = sizeof(SLow)/sizeof(double);
	int SMedLen = sizeof(SMed)/sizeof(double);
	int SHighLen = sizeof(SHigh)/sizeof(double);

	int RLowLen = sizeof(RLow)/sizeof(double);
	int RMedLen = sizeof(RMed)/sizeof(double);
	int RHighLen = sizeof(RHigh)/sizeof(double);

	int NLowLen = sizeof(NLow)/sizeof(double);
	int NHighLen = sizeof(NHigh)/sizeof(double);

	int JLowLen = sizeof(JLow)/sizeof(double);
	int JHighLen = sizeof(JHigh)/sizeof(double);

	int BLowLen = sizeof(BLow)/sizeof(double);
	int BHighLen = sizeof(BHigh)/sizeof(double);

	int ELowLen = sizeof(ELow)/sizeof(double);
	int EHighLen = sizeof(EHigh)/sizeof(double);

	int KLowLen = sizeof(KLow)/sizeof(double);
	int KHighLen = sizeof(KHigh)/sizeof(double);

	const double *coef;
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
			coef = (t < 630.615) ? NLow : NHigh;
			len = (t < 630.615) ? NLowLen : NHighLen;
			break;
		case 'J': 	
			coef = (t < 760) ? NLow : NHigh;
			len = (t < 760) ? NLowLen : NHighLen;
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

	double result = coef[0];
	if(type == 'K')
	{
		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	}

	double pert = t;
	for(int i=1;i< len ;i++)
	{
		result = result + coef[i] * t;
		t = t*pert;
	}
	return result;
}
void WrtoCoef(double w_sn,double w_zn,double w_al, double w_in, double w_ga, double w_hg,double w_ar, double w_tp,int range, double &acoef, double &bcoef, double &ccoef,double coef[])
{
	double delta_sn,delta_zn,delta_in,delta_ga,delta_hg,delta_al,DA,DZ;
	acoef = bcoef = ccoef = 0.0;
	if(range == 0) //a7
	{
		delta_sn = w_sn - GetWr(231.928);
		delta_zn = w_zn - GetWr(419.527);
		delta_al = w_al - GetWr(660.323);
		DA = (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1) + (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1);
		DA = DA +  (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_al-1) * (w_al-1) + (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) * (w_al-1);
		DA = DA -  (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1) + (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_al-1);
		
		acoef = delta_sn*((w_zn-1) * (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1) - (w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) * (w_al-1));
		acoef = acoef +  delta_zn*((w_sn-1) * (w_sn-1) * (w_sn-1) * (w_al-1) * (w_al-1) - (w_sn-1) * (w_sn-1) * (w_al-1) * (w_al-1) * (w_al-1));
		acoef = (acoef +  delta_al*((w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1) - (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1)))/DA;
		
		bcoef = delta_sn*((w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) - (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1));
		bcoef = bcoef +  delta_zn*((w_sn-1) * (w_al-1) * (w_al-1) * (w_al-1) - (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_al-1));
		bcoef = (bcoef +  delta_al*((w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) - (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1)))/DA;
		
		ccoef = delta_sn*((w_zn-1) * (w_al-1) * (w_al-1) - (w_zn-1) * (w_zn-1) * (w_al-1));
		ccoef = ccoef +  delta_zn*((w_sn-1) * (w_sn-1) * (w_al-1) - (w_sn-1) * (w_al-1) * (w_al-1));
		ccoef = (ccoef +  delta_al*((w_sn-1) * (w_zn-1) * (w_zn-1) - (w_sn-1) * (w_sn-1) * (w_zn-1)))/DA;
	}
	if(range == 1) //a8
	{
		
		delta_in = w_in - GetWr(156.5985);
		delta_sn = w_sn - GetWr(231.928);
		DZ = (w_sn-1)*(w_in-1) * (w_in-1) - (w_sn-1) * (w_sn-1) * (w_in-1);
		acoef = ((w_in-1) * (w_in-1) *  delta_sn - (w_sn-1) * (w_sn-1) * delta_in)/DZ;
		bcoef = ((w_sn-1) *  delta_in - (w_in-1) * delta_sn)/DZ;
	}
	if(range == 2) //a9
	{
		
		delta_zn = w_zn - GetWr(419.527);
		delta_sn = w_sn - GetWr(231.928);
		DZ = (w_sn-1)*(w_zn-1) * (w_zn-1) - (w_sn-1) * (w_sn-1) * (w_zn-1);
		acoef = ((w_zn-1) * (w_zn-1) *  delta_sn - (w_sn-1) * (w_sn-1) * delta_zn)/DZ;
		bcoef = ((w_sn-1) *  delta_zn - (w_zn-1) * delta_sn)/DZ;
	}
	
	if(range == 3) //a10
	{
		delta_in = w_in - GetWr(156.598);
		acoef = delta_in/(w_in-1);
	}
	
	if(range == 4) //a11
	{
		delta_ga = w_ga - GetWr(29.7646);
		acoef = delta_ga/(w_ga-1);
	}
	
	if(range == 5) //a5
	{
		delta_ga = w_ga - GetWr(29.7646);
		delta_hg = w_hg - GetWr(-38.8344);
		DZ = (w_ga-1)*(w_hg-1) * (w_hg-1) - (w_ga-1) * (w_ga-1) * (w_hg-1);
		acoef = ((w_hg-1) * (w_hg-1) *  delta_ga - (w_ga-1) * (w_ga-1) * delta_hg)/DZ;
		bcoef = ((w_ga-1) *  delta_hg - (w_hg-1) * delta_ga)/DZ;
	}
	
	if(range == 6)// a4
	{
		double w_rhg,w_rar;
		w_rhg = GetWr(-38.8344);
		w_rar = GetWr(-189.3442);
		acoef = (w_hg-w_rhg)*(w_ar-1)*log(w_ar) - (w_ar - w_rar)*(w_hg-1)*log(w_hg);
		acoef = acoef / ((w_hg-1)*(w_ar-1)*(log(w_ar)-log(w_hg)));
		
		bcoef = (w_ar-w_rar)*(w_hg-1) - (w_hg - w_rhg)*(w_ar-1);
		bcoef = bcoef / ((w_hg-1)*(w_ar-1)*(logl(w_ar)-logl(w_hg)));
	}

	if(coef){
			if(acoef_pos[range] >= 0)
				coef[acoef_pos[range]] = acoef;
	
			if(bcoef_pos[range] >= 0)
				coef[bcoef_pos[range]] =  bcoef;
	
			if(ccoef_pos[range] >= 0)
				coef[ccoef_pos[range]] =  ccoef;

			coef[COEF_VALUE_RTP] = w_tp;
	}
}
//mV to T(degree) convert
double MValueToTValue(double r,char type)
{
	double tlow;
	double tup;
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
	double rnew;
	double tnew;
	int count = 0;

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

double GetWr(double t)
{
	double result;
	double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
	double pert;
    double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};			 
	if(t < 0)
	{
		result = Ai[0];
		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
		t = pert;
		for(int i=1;i<sizeof(Ai)/sizeof(double);i++)
		{
			result = result + Ai[i] * t;
			t = t*pert;
		}
		result = exp(result);
	}else{
			 result = Ci[0];
			 pert = t/481.0 - 1;
			 t = pert;
			 for(int i=1;i<sizeof(Ci)/sizeof(double);i++)
			 {
				 result = result + Ci[i] * t;
				 t = t*pert;
			 }
	}
	return result;
}

double GetdWrdT(double t)
{
	double result;
	double Ci[] = {1.64650916, -0.13714390*2, -0.00649767*3, -0.00234444*4, 0.00511868*5, 0.00187982*6, -0.00204472*7, -0.00046122*8, 0.00045724*9};
	double pert;
    double Ai[] = {3.18324720, -1.80143597*2, 0.71727204*3, 0.50344027*4, -0.61899395*5, -0.05332322*6, 0.28021362*7, 0.10715224*8, -0.29302865*9, 0.04459872*10, 0.11868632*11, -0.05248134*12}; 
	if(t < 0)
	{
		result = Ai[0];
		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
		t = pert;
		for(int i=1;i<sizeof(Ai)/sizeof(double);i++)
		{
			result = result + Ai[i] * t;
			t = t*pert;
		}
		result = GetWr(t)*result/(1.5*(t+273.15));
	}else{
		result = Ci[0];
		pert = t/481.0 - 1;
		t = pert;
		for(int i=1;i<sizeof(Ci)/sizeof(double);i++)
		{
			result = result + Ci[i] * t;
			t = t*pert;
		}
		result = result / 481.0;
	}
	return result;
	
}

double GetT(double w)
{
	double perw;
	double result;
	double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
	double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
	if(w >= 0)
	{
		perw = (w-2.64)/1.64;
		w = perw;
		result = Di[0];
		for(int i=1;i<sizeof(Di)/sizeof(double);i++)
		{
			result = result + Di[i] * w;
			w = w*perw;
		}
	}else{
		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
		w = perw;
		result = Bi[0];
		for(int i=1;i<sizeof(Bi)/sizeof(double);i++)
		{
			result = result + Bi[i] * w;
			w = w*perw;
		}		
		result = 273.15*result - 273.5;
	}
	return result;
}
