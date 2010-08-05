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
		/*DATA	*/	"dwcjdata",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\dwcjjh.htm",		"dwcj",	
		/*REPORT*/	"*",		"*",							"*",	
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
		if((iPos >= 150) && (iPos < 170))  dstd = ((iPos-130))*0.00001;
		if((iPos >= 170) && (iPos < 190))  dstd = ((iPos-150))*0.000001;
		if((iPos >= 190) && (iPos < 210))  dstd = ((iPos-150))*0.0000001;
		
		if((iPos >= 221) && (iPos < 224))  dstd = dFull*((iPos-221)*5+10)*0.1;
		if((iPos >= 224) && (iPos < 227))  dstd = dFull*((iPos-224)*5+10)*0.2;
		if((iPos >= 227) && (iPos < 230))  dstd = dFull*((iPos-227)*5+10)*5;
		
	    return dstd;

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

	if((index >= 221)&&(index <=223))	return 8*100+(index-221)*5+10;
	if((index >= 224)&&(index <=226))	return 9*100+(index-224)*5+10;
	if((index >= 227)&&(index <=229))	return 10*100+(index-227)*5+10;
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
	int iPrec;     //precious setting
	int iPanStart; // start pan
	int iPanId;    //pan id (if = 0, means the default value to all the PanId)
	int Mantissa;  //corresponding mantissa
}PRECCONV;

PRECBASE g_PrecBase[] = {
	// index,slevel, digi, mantissa
		{0, "0.0005", -8 , 5},
		{1, "0.001",  -7 , 1},
		{2, "0.002",  -7 , 2},
		{3, "0.005",  -7 , 5},
		{4, "0.01",   -6 , 1},
		
		{5, "0.02",   -6 , 2},
		{6, "0.05",	  -6 , 5},
		{7, "0.1",	  -5 , 1},
		{8, "0.2",	  -5 , 2},
		{9, "0.5",	  -5 , 5},
		{10,"1",	  -4 , 1},
		{11,"2",	  -4 , 2},
		{12,"5",	  -4 , 5},
		{13,"10",	  -3 , 1},
		{14,"0.015",  -6,  1},
		{15,"20",     -3 , 2},
		{16,"50",     -3 , 5},
		{17,"100",    -2 , 1},
		{18,"200",    -2 , 2}
};
PRECCONV g_PrecConv[] = {
	//prec,panstart, panid ,    约数
	{4, 1, 0, 15},
	{4, 1, 1, 17},
	{4, 1, 6, 13},
	{4, 2, 0, 11},
	{4, 2, 1, 13},
	{4, 2, 7, 10},
	{5, 1, 0, 16},
	{5, 1, 1, 18},
	{5, 1, 5, 17},
	{5, 3, 0,  10},
	{5, 3, 3,  11},
	{6, 2, 0, 16},
	{7, 2, 0, 17}
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
				return (g_PrecBase[cnt].iDigipos+2)*100-g_PrecBase[cnt].iMantissa;
	}

	for(cnt=0; cnt < sizeof(g_PrecConv)/sizeof(PRECCONV);cnt++){
		if(g_PrecConv[cnt].iPanStart == g_iPanStart){
			if((g_PrecConv[cnt].iPanId == group) &&	(JinDu.Compare(g_PrecBase[g_PrecConv[cnt].iPrec].sLevel) == 0))
				return (g_PrecBase[g_PrecConv[cnt].Mantissa].iDigipos+2)*100-g_PrecBase[g_PrecConv[cnt].Mantissa].iMantissa;
		}
	}
	for(cnt=0; cnt < sizeof(g_PrecConv)/sizeof(PRECCONV);cnt++){
		if(g_PrecConv[cnt].iPanStart == g_iPanStart){
			if((g_PrecConv[cnt].iPanId == 0) &&	(JinDu.Compare(g_PrecBase[g_PrecConv[cnt].iPrec].sLevel) == 0))
				return (g_PrecBase[g_PrecConv[cnt].Mantissa].iDigipos+2)*100-g_PrecBase[g_PrecConv[cnt].Mantissa].iMantissa;
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
		if(!mstr.Lookup(sId,sValue)){
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
			iMantissa = (-iDigipos)%100;iDigipos = (iDigipos+iMantissa)/100;
			
		//	iDigipos += 3;
			double value = atof(sValue);
			bool sign = (value > 0);
			if(sign){
				char nchar = SJD2SGetchar(value,iDigipos-1);//last char
				char n1char = SJD2SGetchar(value,iDigipos);//next last char
				int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',iMantissa);//get carry char
				value += carry*pow(10.0,iDigipos);//adjust
			}else{
				char nchar = SJD2SGetchar(-value,iDigipos-1);//last char
				char n1char = SJD2SGetchar(-value,iDigipos);//next last char
				int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',iMantissa);//get carry char
				value -= carry*pow(10.0,iDigipos);//adjust
			}
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
	if(mstr.Lookup("Z200",sValue))	g_iPanStart = 7;
	if(mstr.Lookup("Z180",sValue))	g_iPanStart = 6;
	if(mstr.Lookup("Z160",sValue))	g_iPanStart = 5;
	if(mstr.Lookup("Z140",sValue))	g_iPanStart = 4;
	if(mstr.Lookup("Z110",sValue))	g_iPanStart = 3;
	if(mstr.Lookup("Z80",sValue))	g_iPanStart = 2;
	if(mstr.Lookup("Z50",sValue))	g_iPanStart = 1;
	g_iPanEnd = 0;
	if(mstr.Lookup("Z50",sValue))	g_iPanEnd = 1;
	if(mstr.Lookup("Z80",sValue))	g_iPanEnd = 2;
	if(mstr.Lookup("Z110",sValue))	g_iPanEnd = 3;
	if(mstr.Lookup("Z140",sValue))	g_iPanEnd = 4;
	if(mstr.Lookup("Z160",sValue))	g_iPanEnd = 5;
	if(mstr.Lookup("Z180",sValue))	g_iPanEnd = 6;
	if(mstr.Lookup("Z200",sValue))	g_iPanEnd = 7;
	return g_iPanStart != 0;
}

#define PAN_MAX	7

static void CheckDataForCheck(CMapStringToString &mstr, int startpos, int endpos,bool colormark){

	CString sJdjl;//conclusion
	CString sId,sValue;
	double MaxValue[PAN_MAX+1],MinValue[PAN_MAX+1]; //store the max and min value
	double MaxStd[PAN_MAX+1],MinStd[PAN_MAX+1];		//store the max and min position
	int MaxPos[PAN_MAX+1],MinPos[PAN_MAX+1];
	int cnt;

	//clear to zero
	for(cnt=g_iPanStart;cnt<= g_iPanEnd;cnt++) {
		MaxValue[cnt] = 0;MaxStd[cnt] = 0; MinValue[cnt] = 0;MinStd[cnt] = 0;
		MaxPos[cnt] = 0; MinPos[cnt] = 0;
	}

	//retrieve max min position and corresponding values in MaxValue MinValue MaxPos MinPos group
	double dM10,dM15,dM20;//value of point 10, 15, 20 in g_iPanStart pan
	for(cnt = startpos; cnt <= endpos;cnt++){
		sId.Format("Z%d",cnt);
		if(!mstr.Lookup(sId,sValue))
			continue;
		if(sValue.IsEmpty())
			continue;
		if(sValue.Find("--") < 0)
		{
			int iGrp,iRst;
			iGrp = IndexDecomposition(cnt);  //return iGrp*100+iRst
			iRst = iGrp % 100; iGrp = (iGrp-iRst)/100;
			if(iGrp >= 8) continue;
			double value = atof(sValue);

			//store the dMvalue for later use
			if((iGrp == g_iPanStart) && (iRst == 10)) dM10 = IndexComposition(cnt) - value;
			if((iGrp == g_iPanStart) && (iRst == 15)) dM15 = IndexComposition(cnt) - value;
			if((iGrp == g_iPanStart) && (iRst == 20)) dM20 = IndexComposition(cnt) - value;

			double delta= value;

			if(delta >  MaxValue[iGrp]){
				MaxValue[iGrp] = delta; 
				MaxStd[iGrp] = IndexComposition(cnt);
				MaxPos[iGrp] = cnt;
			}
			if(delta <  MinValue[iGrp]){
				MinValue[iGrp] = delta; 
				MinStd[iGrp] = IndexComposition(cnt);
				MinPos[iGrp] = cnt;
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
	
	if(!bMaxOk) {
		sJdjl.Format("%s%f V点不合格,",sJdjl,sumMaxStd);
		for(cnt=g_iPanStart;cnt<= g_iPanEnd;cnt++) {
			sId.Format("Z%d",MaxPos[cnt]);
			if(!mstr.Lookup(sId,sValue))
				continue;
			if(sValue.IsEmpty())
				continue;
			if(sValue.Find("--") < 0)
				if(colormark)
					mstr.SetAt(sId,"<font color='yellow'>"+sValue+"</font>");
		}
	}
	if(!bMinOk) {
		sJdjl.Format("%s%f V点不合格,",sJdjl,sumMinStd);
		for(cnt=g_iPanStart;cnt<= g_iPanEnd;cnt++) {
			sId.Format("Z%d",MinPos[cnt]);
			if(!mstr.Lookup(sId,sValue))
				continue;
			if(sValue.IsEmpty())
				continue;
			if(sValue.Find("--") < 0)
				if(colormark)
					mstr.SetAt(sId,"<font color='red'>"+sValue+"</font>");
		}

	}

	//检查3个倍率量程
	CString sM; //rate range's rate value stored
	double dB10,dB15,dB20,Mvalue,Mdelta;
	if(mstr.Lookup("Z221",sValue)){

		double M = 0.1;
								   dB10 = IndexComposition(170) - atof(sValue);
		mstr.Lookup("Z222",sValue);dB15 = IndexComposition(171) - atof(sValue);
		mstr.Lookup("Z223",sValue);dB20 = IndexComposition(172) - atof(sValue);
		Mvalue = ((dB10/dM10) + (dB15/dM15) + (dB20/dM20))/3.0;
		Mdelta = Mvalue/M - 1;
		bMaxOk = (atof(g_sJinDu1)*sumMaxStd/100.0 + dShiZhi*M*atof(g_sJinDu1)/1000.0) > (M*sumMaxDelta + M*Mdelta*sumMaxStd);
		bMinOk = (atof(g_sJinDu1)*sumMinStd/100.0 + dShiZhi*M*atof(g_sJinDu1)/1000.0) > (M*abs(sumMinDelta) + M*Mdelta*sumMinStd);
		if(!(bMaxOk && bMinOk)) sJdjl += _T("倍率量程X0.1不合格,");
		sM.Format("%sM1=%.5f<p></p>",sM,Mvalue);
	}

	if(mstr.Lookup("Z224",sValue)){
		double M = 0.2;
								   dB10 = IndexComposition(180) - atof(sValue);
		mstr.Lookup("Z225",sValue);dB15 = IndexComposition(181) - atof(sValue);
		mstr.Lookup("Z226",sValue);dB20 = IndexComposition(182) - atof(sValue);
		Mvalue = ((dB10/dM10) + (dB15/dM15) + (dB20/dM20))/3.0;
		Mdelta = Mvalue/M - 1;
		bMaxOk = (atof(g_sJinDu2)*sumMaxStd/100.0 + dShiZhi*M*atof(g_sJinDu2)/1000.0) > (M*sumMaxDelta + M*Mdelta*sumMaxStd);
		bMinOk = (atof(g_sJinDu1)*sumMinStd/100.0 + dShiZhi*M*atof(g_sJinDu1)/1000.0) > (M*abs(sumMinDelta) + M*Mdelta*sumMinStd);
		if(!(bMaxOk && bMinOk)) sJdjl += CString("倍率量程X0.2不合格,");
		sM.Format("%sM2=%.5f<p></p>",sM,Mvalue);
	}

	if(mstr.Lookup("Z227",sValue)){
		double M = 5;
								   dB10 = IndexComposition(190) - atof(sValue);
		mstr.Lookup("Z228",sValue);dB15 = IndexComposition(191) - atof(sValue);
		mstr.Lookup("Z229",sValue);dB20 = IndexComposition(192) - atof(sValue);
		Mvalue = ((dB10/dM10) + (dB15/dM15) + (dB20/dM20))/3.0;
		Mdelta = Mvalue/M - 1;
		bMaxOk = (atof(g_sJinDu3)*sumMaxStd/100.0 + dShiZhi*M*atof(g_sJinDu3)/1000.0) > (M*sumMaxDelta + M*Mdelta*sumMaxStd);
		bMinOk = (atof(g_sJinDu3)*sumMinStd/100.0 + dShiZhi*M*atof(g_sJinDu3)/1000.0) > (M*abs(sumMinDelta) + M*Mdelta*sumMinStd);
		if(!(bMaxOk && bMinOk)) sJdjl += CString("倍率量程X5不合格,");
		sM.Format("%sM3=%.5f<p></p>",sM,Mvalue);
	}
	if(!sJdjl.IsEmpty())
		if(colormark)
			mstr.SetAt("jdjg","<font color='red'>"+sJdjl+"</font>");//检定结果
		else
			mstr.SetAt("jdjg",sJdjl);

	mstr.SetAt("mvalues",sM);//倍率量程
}

PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	return;

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