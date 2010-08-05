// PrgInterface3.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE PrgInterface3DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("PRGINTERFACE3.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(PrgInterface3DLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call thath
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(PrgInterface3DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PRGINTERFACE3.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(PrgInterface3DLL);
	}
	return 1;   // ok
}


#include "math.h"
#include "tchar.h"
#include "..\\sjinclude\\double2string\\sjdouble2string.h"
#define PRGINTERFACE_API __declspec(dllexport)

PRGINTERFACE_API const char*  PathQuery(int pathid, int clsid)
{
	const char*	pathdef[]={
		/*MDB*/						/*JS*/					/*GROUP*/
		/*REG	*/	"reg",		"*",							"dwcj",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\dwcjjh.htm",	"dwcj",	
		/*REPORT*/	"*",		"*",								"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\dwcjtest.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        	"电位差计",				"电位差计测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}
static CString g_sJinDu1;
static CString g_sJinDu2;
static CString g_sJinDu3;
static CString g_sJinDu0;
static int g_iPanStart;
static int g_iPanEnd;
//Group Definition from 1-7
//Rest  Definition from 0-20
//composite the index to the real value
static double IndexComposition(int iPos){
		double dstd = 0;
		double dFull = pow(10.0,-g_iPanStart);
		if((iPos >= 40) && (iPos < 70) )  dstd = ((iPos-40))*0.1;
		if((iPos >= 70) && (iPos < 100) )  dstd = ((iPos-70))*0.01;
		if((iPos >= 100) && (iPos < 130))  dstd = ((iPos-100))*0.001;
		if((iPos >= 130) && (iPos < 150))  dstd = ((iPos-130))*0.0001;
		if((iPos >= 150) && (iPos < 170))  dstd = ((iPos-150))*0.00001;
		if((iPos >= 170) && (iPos < 190))  dstd = ((iPos-170))*0.000001;
		if((iPos >= 190) && (iPos < 210))  dstd = ((iPos-190))*0.0000001;
		
		if((iPos >= 222) && (iPos <= 224))  dstd = dFull*((iPos-222)*5+10)*0.1;
		if((iPos >= 226) && (iPos <= 228))  dstd = dFull*((iPos-226)*5+10)*0.2;
		if((iPos >= 230) && (iPos <= 232))  dstd = dFull*((iPos-230)*5+10)*5;
		if( (iPos == 221) || (iPos == 225) || (iPos == 229))
			dstd = 0;
	    return dstd*1e6;

}
//decomposite the Index to group and rest value, return value are indexed as group*100+rest
static int IndexDecomposition(int index)
{
	if((index >= 39)&&(index < 69))		return 100+index-40;
	if((index >= 69)&&(index < 99))		return 200+index-70;
	if((index >= 99)&&(index < 129))	return 300+index-100;
	if((index >= 129)&&(index <149))	return 400+index-130;
	if((index >= 149)&&(index <169))	return 500+index-150;
	if((index >= 169)&&(index <189))	return 600+index-150;
	if((index >= 189)&&(index <209))	return 700+index-150;

	if((index >= 222)&&(index <=224))	return 8*100+(index-222)*5+10;
	if((index >= 226)&&(index <=228))	return 9*100+(index-226)*5+10;
	if((index >= 230)&&(index <=232))	return 10*100+(index-230)*5+10;
	if( (index == 221) || (index == 225) || (index == 229))
		return (index-221)*25+800;
	return 0;
}
//get the Precise configuration according to group id and base and precise define stored in mstr
// return value is the digipos*100+manssita depend on digipos's position
typedef struct _PRECBASE{
	int index;
	const char* sLevel;
	int iDigipos;
	int iMantissa;
}PRECBASE;

typedef struct _PRECCONV{
	const char* sLevel;
	int index[7];
}PRECCONV;
PRECCONV g_PrecConv[] = {
	{"0.015",{10,8,8,8,8,7,7}},
	{"0.01",{7,7,5,5,5,5,4}},
	{"0.03",{11,9,9,9,10,10,10}},
	{"0.02",{5,5,5,4,4,4,4}},
	{"0.05",{9,9,9,9,9,9,9}},
	{"0.1", {10,10,10,10,10,10,10}},
};

PRECBASE g_PrecBase[] = {
	// index,slevel, digi, mantissa
		{0, "0.0005", -4 , 5},
		{1, "0.001",  -3 , 1},
		{2, "0.002",  -3 , 2},
		{3, "0.005",  -3 , 5},
		{4, "0.01",   -2 , 1},
		{14,"0.015",  -2,  1},
		{5, "0.02",   -2,  2},
		{6, "0.05",	  -2,  5},
		{7, "0.1",	  -1, 1},
		{8, "0.2",	  -1, 2},
		{9, "0.5",	  -1, 5},
		{10,"1",	  0, 1},
		{11,"2",	  0, 2},
		{12,"5",	  0, 5},
		{13,"10",	  1, 1},
		{15,"20",     1, 2},
		{16,"50",     1, 5},
		{17,"100",    2, 1}
};
static int GetPrecise(int group){
	CString JinDu;
	int cnt;
	JinDu = g_sJinDu0;
	if(group == 8) JinDu = g_sJinDu1;
	if(group == 9) JinDu = g_sJinDu2;
	if(group == 10) JinDu = g_sJinDu3;
	if((group == 8)||(group == 9)||(group == 10)){
		for(cnt=0;cnt < sizeof(g_PrecBase)/sizeof(PRECBASE);cnt++)
			if(JinDu.Compare(g_PrecBase[cnt].sLevel) == 0)
				return g_PrecBase[cnt].index;
	}else
	{
		for(cnt=0; cnt < sizeof(g_PrecConv)/sizeof(PRECCONV);cnt++){
				if(JinDu.Compare(g_PrecConv[cnt].sLevel) == 0)
					return (g_PrecConv[cnt].index[group-1]);
		}
	}
	return 0;
}
//Do Data Integration , round the data to some specified precice,just by index info
static void RoundDataToPrecise(CMapStringToString &mstr,int startPos, int endPos)
{
	while(startPos <= endPos)
	{
		CString sId,sValue;
		sId.Format("Z%d",startPos);
		if(!mstr.Lookup(sId,sValue) || sValue.FindOneOf("0123456789") < 0){
			startPos++;
			continue;
		}
		if((startPos == 69)||
			(startPos == 99)||
			(startPos == 129)||
			(startPos == 149)||
			(startPos == 169)||
			(startPos == 189)||
			(startPos == 209))
		{
			startPos++;
			continue;
	    }
		if(sValue.Find("--") < 0)
		{
			//retrieve iGrp & iRst & iDigipos & iMantissa
			int iGrp,iRst;
			iGrp = IndexDecomposition(startPos);
			iRst = iGrp % 100; iGrp = (iGrp-iRst)/100;
			
			int iDigipos,iMantissa;
			iDigipos = GetPrecise(iGrp);
			for(int i=0;i<sizeof(g_PrecBase)/sizeof(PRECBASE);i++)
			{
				if(g_PrecBase[i].index == iDigipos)
				{
					iDigipos = g_PrecBase[i].iDigipos;
					iMantissa = g_PrecBase[i].iMantissa;
					break;
				}
			}
			
			double value = fabs(atof(sValue));
			char nchar = SJD2SGetchar(value,iDigipos-1);//last char
			char n1char = SJD2SGetchar(value,iDigipos);//next last char
			int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',iMantissa);//get carry char

			value += carry*pow(10.0,iDigipos);//adjust
			if(sValue.GetAt(0) == '-')
				mstr.SetAt(sId,CString("-")+SJD2SByPosition(value,iDigipos));		
			else
				mstr.SetAt(sId,SJD2SByPosition(value,iDigipos));			
		}
		startPos++;
	}
}

//set initial value table depend on the g_iType
static void SetInitTable(CMapStringToString &mstr){
	mstr.Lookup("jhsel0",g_sJinDu0);
	mstr.Lookup("jhsel1",g_sJinDu1);
	mstr.Lookup("jhsel2",g_sJinDu2);
	mstr.Lookup("jhsel3",g_sJinDu3);
}


//Determine Type will fill the necessary parameter to classify different types
//here is just g_iPanStart and g_iPanEnd (pan == group)
static bool DetermineType(CMapStringToString &mstr){
	CString sValue;
	g_iPanStart = 0;
	if(mstr.Lookup("Z200",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 7;
	if(mstr.Lookup("Z180",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 6;
	if(mstr.Lookup("Z160",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 5;
	if(mstr.Lookup("Z140",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 4;
	if(mstr.Lookup("Z110",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 3;
	if(mstr.Lookup("Z80",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 2;
	if(mstr.Lookup("Z50",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanStart = 1;
	g_iPanEnd = 0;
	if(mstr.Lookup("Z50",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 1;
	if(mstr.Lookup("Z80",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 2;
	if(mstr.Lookup("Z110",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 3;
	if(mstr.Lookup("Z140",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 4;
	if(mstr.Lookup("Z160",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 5;
	if(mstr.Lookup("Z180",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 6;
	if(mstr.Lookup("Z200",sValue) && (sValue.FindOneOf("0123456789") >= 0))	g_iPanEnd = 7;
	return g_iPanStart != 0;
}

#define PAN_MAX	7

static void CheckDataForCheck(CMapStringToString &mstr, int startpos, int endpos,bool colormark){

	CString sJdjl;//conclusion
	CString sId,sValue;
	double MaxValue[PAN_MAX],MinValue[PAN_MAX]; //store the max and min value
	double MaxStd[PAN_MAX],MinStd[PAN_MAX];		//store the max and min position

	int cnt;

	//clear to zero
	for(cnt=g_iPanStart;cnt<= g_iPanEnd;cnt++) {
		MaxValue[cnt] = 0;MaxStd[cnt] = 0; MinValue[cnt] = 0;MinStd[cnt] = 0;
	}

	//retrieve max min position and corresponding values in MaxValue MinValue MaxPos MinPos group
	double dM10,dM15,dM20;//value of point 10, 15, 20 in g_iPanStart pan
	for(cnt = startpos; cnt <= endpos;cnt++){
		sId.Format("Z%d",cnt);
		if(!mstr.Lookup(sId,sValue) || sValue.FindOneOf("0123456789") < 0)
			continue;
		if(sValue.FindOneOf("0123456789") < 0)
			continue;
		if((cnt == 69)||
			(cnt == 99)||
			(cnt == 129)||
			(cnt == 149)||
			(cnt == 169)||
			(cnt == 189)||
			(cnt == 209))
		{
			continue;
	    }

		if(sValue.Find("--") < 0)
		{
			int iGrp,iRst;
			iGrp = IndexDecomposition(cnt);  //return iGrp*100+iRst
			iRst = iGrp % 100; iGrp = (iGrp-iRst)/100;
			if(iGrp >= 8) continue;
			double value = atof(sValue);

			//store the dMvalue for later use
			if((iGrp == g_iPanStart) && (iRst == 10)) dM10 = IndexComposition(cnt) + value;
			if((iGrp == g_iPanStart) && (iRst == 15)) dM15 = IndexComposition(cnt) + value;
			if((iGrp == g_iPanStart) && (iRst == 20)) dM20 = IndexComposition(cnt) + value;

			double delta= value;

			if(delta >  MaxValue[iGrp]){
				MaxValue[iGrp] = delta; 
				MaxStd[iGrp] = IndexComposition(cnt);
			}
			if(delta <  MinValue[iGrp]){
				MinValue[iGrp] = delta; 
				MinStd[iGrp] = IndexComposition(cnt);
			}
		}		
	}

	//caculate the sum of the max deltas and standards
	double sumMaxDelta = 0,sumMinDelta = 0,sumMaxStd = 0,sumMinStd = 0;
	for(cnt=g_iPanStart;cnt<= g_iPanEnd;cnt++) {
		sumMaxDelta += MaxValue[cnt];sumMaxStd += MaxStd[cnt]; 
		sumMinDelta += MinValue[cnt];sumMinStd += MinStd[cnt];
	}
	
	//retrieve the shizhi
	double dShiZhi = 1e6*pow(10.0,1-g_iPanStart);

	bool bMaxOk = ((sumMaxStd*atof(g_sJinDu0)/100.0)+(dShiZhi*atof(g_sJinDu0)/1000.0)) > sumMaxDelta;
	bool bMinOk = ((sumMinStd*atof(g_sJinDu0)/100.0)+(dShiZhi*atof(g_sJinDu0)/1000.0)) > abs(sumMinDelta);
	
	if(!bMaxOk) 	sJdjl.Format("%s%f点不合格,",sJdjl,sumMaxStd/1.0e6);
	if(!bMinOk) 	sJdjl.Format("%s%f点不合格,",sJdjl,sumMinStd/1.0e6);

	//检查3个倍率量程
	CString sM(""); //rate range's rate value stored
	CString sV("");
	double dB10,dB15,dB20,Mvalue,Mdelta;
	if(mstr.Lookup("Z222",sValue)  && (sValue.FindOneOf("0123456789") >= 0)){

		double M = 0.1;
								   dB10 = M*IndexComposition(g_iPanStart*30+10+10) + atof(sValue);
		mstr.Lookup("Z223",sValue);dB15 = M*IndexComposition(g_iPanStart*30+10+15) + atof(sValue);
		mstr.Lookup("Z224",sValue);dB20 = M*IndexComposition(g_iPanStart*30+10+20) + atof(sValue);
		
		if((dM10 == 0) || (dM15 == 0) || (dM20 == 0))
		{
			bMaxOk = false;
			bMinOk = false;
			sM = sM + CString("M1=ERROR\n");
		}else
		{	
			Mvalue = (abs(dB10/dM10) + abs(dB15/dM15) + abs(dB20/dM20))/3.0;
			Mdelta = Mvalue/M - 1;

			bMaxOk = bMinOk = true;
			if(abs(abs(dB10/dM10) - abs(dB15/dM15)) > atof(g_sJinDu1)/300.0){
				bMaxOk = bMinOk = false;
			}
			if(abs(abs(dB20/dM20) - abs(dB15/dM15)) > atof(g_sJinDu1)/300.0){
				bMaxOk = bMinOk = false;
			}
			if(abs(abs(dB10/dM10) - abs(dB20/dM20)) > atof(g_sJinDu1)/300.0){
				bMaxOk = bMinOk = false;
			}

			//			bMaxOk = (atof(g_sJinDu1)*sumMaxStd/100.0 + dShiZhi*M*atof(g_sJinDu1)/1000.0) > (M*sumMaxDelta + M*Mdelta*sumMaxStd);
//			bMinOk = (atof(g_sJinDu1)*sumMinStd/100.0 + dShiZhi*M*atof(g_sJinDu1)/1000.0) > (M*abs(sumMinDelta) + M*Mdelta*sumMinStd);

			sV.Format("M1=%.5f\n",Mvalue);
			sM = sM + sV;
		}
		if(!(bMaxOk && bMinOk)) sJdjl += _T("倍率量程X0.1不合格,");

		
	}

	if(mstr.Lookup("Z226",sValue) && (sValue.FindOneOf("0123456789") >= 0)){
		double M = 0.2;
								   dB10 = M*IndexComposition(g_iPanStart*30+10+10) + atof(sValue);
		mstr.Lookup("Z227",sValue);dB15 = M*IndexComposition(g_iPanStart*30+10+15) + atof(sValue);
		mstr.Lookup("Z228",sValue);dB20 = M*IndexComposition(g_iPanStart*30+10+20) + atof(sValue);
		if((dM10 == 0) || (dM15 == 0) || (dM20 == 0))
		{
			bMaxOk = false;
			bMinOk = false;
			sM = sM + CString("M2=ERROR\n");
		}else
		{	
			Mvalue = (abs(dB10/dM10) + abs(dB15/dM15) + abs(dB20/dM20))/3.0;
			Mdelta = Mvalue/M - 1;

			bMaxOk = bMinOk = true;
			if(abs(abs(dB10/dM10) - abs(dB15/dM15)) > atof(g_sJinDu2)/300.0){
				bMaxOk = bMinOk = false;
			}
			if(abs(abs(dB20/dM20) - abs(dB15/dM15)) > atof(g_sJinDu2)/300.0){
				bMaxOk = bMinOk = false;
			}
			if(abs(abs(dB10/dM10) - abs(dB20/dM20)) > atof(g_sJinDu2)/300.0){
				bMaxOk = bMinOk = false;
			}

//			bMaxOk = (atof(g_sJinDu2)*sumMaxStd/100.0 + dShiZhi*M*atof(g_sJinDu2)/1000.0) > (M*sumMaxDelta + M*Mdelta*sumMaxStd);
//			bMinOk = (atof(g_sJinDu2)*sumMinStd/100.0 + dShiZhi*M*atof(g_sJinDu2)/1000.0) > (M*abs(sumMinDelta) + M*Mdelta*sumMinStd);

			sV.Format("M2=%.5f\n",Mvalue);
			sM = sM + sV;
		}
		if(!(bMaxOk && bMinOk)) sJdjl += CString("倍率量程X0.2不合格,");
	}

	if(mstr.Lookup("Z230",sValue) && (sValue.FindOneOf("0123456789") >= 0)){
		double M = 5;
								   dB10 = M*IndexComposition(g_iPanStart*30+10+10) + atof(sValue);
		mstr.Lookup("Z231",sValue);dB15 = M*IndexComposition(g_iPanStart*30+10+15) + atof(sValue);
		mstr.Lookup("Z232",sValue);dB20 = M*IndexComposition(g_iPanStart*30+10+20) + atof(sValue);

		if((dM10 == 0) || (dM15 == 0) || (dM20 == 0))
		{
			bMaxOk = false;
			bMinOk = false;
			sM = sM + CString("M3=ERROR\n");
		}else
		{	
			Mvalue = (abs(dB10/dM10) + abs(dB15/dM15) + abs(dB20/dM20))/3.0;
			Mdelta = Mvalue/M - 1;

			bMaxOk = bMinOk = true;
			if(abs(abs(dB10/dM10) - abs(dB15/dM15)) > atof(g_sJinDu3)/300.0){
				bMaxOk = bMinOk = false;
			}
			if(abs(abs(dB20/dM20) - abs(dB15/dM15)) > atof(g_sJinDu3)/300.0){
				bMaxOk = bMinOk = false;
			}
			if(abs(abs(dB10/dM10) - abs(dB20/dM20)) > atof(g_sJinDu3)/300.0){
				bMaxOk = bMinOk = false;
			}

//			bMaxOk = (atof(g_sJinDu3)*sumMaxStd/100.0 + dShiZhi*M*atof(g_sJinDu3)/1000.0) > (M*sumMaxDelta + M*Mdelta*sumMaxStd);
//			bMinOk = (atof(g_sJinDu3)*sumMinStd/100.0 + dShiZhi*M*atof(g_sJinDu3)/1000.0) > (M*abs(sumMinDelta) + M*Mdelta*sumMinStd);

			sV.Format("M3=%.5f\n",Mvalue);
			sM = sM + sV;

		}
		if(!(bMaxOk && bMinOk)) sJdjl += CString("倍率量程X5不合格,");
	}
	if(!sJdjl.IsEmpty())
		mstr.SetAt("jdjg","<font color='red'>"+sJdjl+"</font>");//检定结果

	mstr.SetAt("mvalues",sM);//倍率量程
}

PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	return;
	if(!DetermineType(mstr)) return;

	SetInitTable(mstr);
	RoundDataToPrecise(mstr,0,indexmax);
	CheckDataForCheck(mstr,0,indexmax,true);

	int cnt;
	for(cnt = 0; cnt <= indexmax;cnt++){
		CString sId,sValue;
		sId.Format("Z%d",cnt);
		if(!mstr.Lookup(sId,sValue) || sValue.FindOneOf("0123456789") < 0)
			continue;
		if((sValue.FindOneOf("0123456789") >= 0) &&(sValue.Find("--") < 0))
		{
			//retrieve iGrp & iRst & iDigipos & iMantissa
			int iGrp,iRst;
			iGrp = IndexDecomposition(cnt);
			iRst = iGrp % 100; iGrp = (iGrp-iRst)/100;

			double delta= (atof(sValue) - IndexComposition(cnt));
			sValue.Format("%.6f",delta);
			mstr.SetAt(sId,sValue);
		}		
	}
	RoundDataToPrecise(mstr,0,indexmax);
}


PRGINTERFACE_API void  PrepareArrayForReport(CMapStringToString &mstr)
{
}

PRGINTERFACE_API void  DataIntegrateForCheck(CMapStringToString &mstr, int indexmax, bool colormark)
{
	if(!DetermineType(mstr)) return;
	SetInitTable(mstr);
	RoundDataToPrecise(mstr,0,indexmax);
	CheckDataForCheck(mstr,0,indexmax,colormark);
}

PRGINTERFACE_API bool CheckPassFail(CMapStringToString &mstr, int pos)
{
	return true;
}
