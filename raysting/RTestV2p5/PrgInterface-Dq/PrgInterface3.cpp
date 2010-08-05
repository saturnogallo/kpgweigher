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
		//  that uses this Extension DLL should then explicitly call that
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
#include "..\\sjinclude\\Maths\\sjCalib.h"
#define PRGINTERFACE_API __declspec(dllexport)

CString g_jherr;

PRGINTERFACE_API const char*  PathQuery(int pathid, int clsid)
{
	const char*	pathdef[]={
		/*MDB*/						/*JS*/					/*GROUP*/
		/*REG	*/	"reg",		"*",							"dq",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\dqjh.htm",		"dq",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\dqtest.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        	"电桥",						"电桥测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}


PRECBASE2 g_Cfg[] = {
	//NEIBI 6 PAN CASE
	{PREC_TYPE_RGROUP,151,161,150,"1000","jhsel7",JHSEL_0005_50,"-7",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂内臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,131,141,130,"100","jhsel6",JHSEL_0005_50,"-10",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂内臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,111,121,110,"10","jhsel5",JHSEL_0005_50,"-13",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂内臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,91,101,90,"1","jhsel4",JHSEL_0005_50,"-16",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂内臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,71,81,70,"0.1","jhsel3",JHSEL_0005_50,"-19",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂内臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,51,61,50,"0.01","jhsel2",JHSEL_0005_50,"-22",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂内臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	
	//WAIBI 6 PAN CASE
	{PREC_TYPE_RGROUP,271,281,270,"1000","jhsel7",JHSEL_0005_50,"-7",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂外臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,251,261,250,"100","jhsel6",JHSEL_0005_50,"-10",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂外臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,231,241,230,"10","jhsel5",JHSEL_0005_50,"-13",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂外臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,211,221,210,"1","jhsel4",JHSEL_0005_50,"-16",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂外臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,191,201,190,"0.1","jhsel3",JHSEL_0005_50,"-19",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂外臂X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,171,181,170,"0.01","jhsel2",JHSEL_0005_50,"-22",MANTI_13_512,PREC_5E5_5E1,NULL,"比较臂外臂X{val_scope}ohm盘第{rel_pos}点不合格"},

	{PREC_TYPE_RSINGLE,39,39,0,"-0.02","1",NULL,"-3","1,","1",NULL,"比较臂内臂0电阻不合格"},
	{PREC_TYPE_RSINGLE,49,49,0,"-0.02","1",NULL,"-3","1,","1",NULL,"比较臂外臂0电阻不合格"},

	{PREC_TYPE_RSINGLE,28,28,0,"10000","jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-2,-2,-2,-1,-1,-1","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R2比例臂10000ohm点不合格"},
	{PREC_TYPE_RSINGLE,27,27,0,"1000" ,"jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-3,-3,-3,-2,-2,-2","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R2比例臂1000ohm点不合格"},
	{PREC_TYPE_RSINGLE,26,26,0,"100",  "jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-4,-4,-4,-3,-3,-3","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R2比例臂100ohm点不合格"},
	{PREC_TYPE_RSINGLE,25,25,0,"10",   "jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-5,-5,-5,-4,-4,-4","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R2比例臂10ohm点不合格"},

	{PREC_TYPE_RSINGLE,24,24,0,"10000","jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-2,-2,-2,-1,-1,-1","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R1比例臂10000ohm点不合格"},
	{PREC_TYPE_RSINGLE,23,23,0,"1000" ,"jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-3,-3,-3,-2,-2,-2","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R1比例臂1000ohm点不合格"},
	{PREC_TYPE_RSINGLE,22,22,0,"100",  "jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-4,-4,-4,-3,-3,-3","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R1比例臂100ohm点不合格"},
	{PREC_TYPE_RSINGLE,21,21,0,"10",   "jhsel0","0.0015,0.0025,0.005,0.01,0.015,0.02","-5,-5,-5,-4,-4,-4","1,2,5,1,1,2","1.5e-5,2.5e-5,5e-5,1e-4,1.5e-4,2e-4",NULL,"R1比例臂10ohm点不合格"},

};
static int g_iPanStart;
static int g_iPanEnd;


//Group Definition from 1-7
//Rest  Definition from 0-20
//composite the index to the real value
static double IndexComposition(int index){
	if((index >= 21)&&(index <= 24))		return pow(10.0,index-20);
	if((index >= 25)&&(index <= 28))		return pow(10.0,index-24);
	if((index == 39)||(index == 49))		return 0;
	if((index >= 51)&&(index <= 161))		return ((index+10)%20)*pow(10.0,((index+10)/20-5));
	if((index >= 171)&&(index <= 281))		return ((index+10)%20)*pow(10.0,((index+10)/20-11));
	return -1;
}
//decomposite the Index to group and rest value, return value are indexed as group*100+rest
static int IndexDecomposition(int index)
{
	if((index >= 21)&&(index <= 28))		return 0+index-20;
	if((index == 39))						return 100;
	if((index == 49))						return 1100;
	if((index >= 51)&&(index <= 161))		return 100*((index+10)/20-2)+((index+10)%20);
	if((index >= 171)&&(index <= 281))		return 100*((index+10)/20+2)+((index+10)%20);
	return -1;
}

//Determine Type will fill the necessary parameter to classify different types
//here is just g_iPanStart and g_iPanEnd (pan == group)
static bool DetermineType(CMapStringToString &mstr){
	CString sValue;
	g_iPanStart = 1;
	g_iPanEnd = (mstr.Lookup("Z160",sValue) && (sValue.FindOneOf("0123456789") >= 0)) ? 6:5;
	return (mstr.Lookup("Z60",sValue) && (sValue.FindOneOf("0123456789") >= 0)) == TRUE;
}
#define PAN_MAX	7
#define REST_MAX  13
static CString CheckDataForCheck(CMapStringToString &mstr, int startpos, int endpos,bool colormark){
	CString sJdjl;//conclusion
	CString sId, sValue; 

	//clear to zero
//	bool bOK[PAN_MAX][REST_MAX];
	bool bAbort = false;

	double R1max,R1min,R2max,R2min;
	R1max = 0; R1min = 0; R2max = 0;R2min = 0;

	double R0n,R0w;	//n mean 内,w means 外
	mstr.Lookup("Z39",sValue);R0n = atof(sValue);
	if(R0n < 0.02) R0n = 0;

	mstr.Lookup("Z49",sValue);R0w = atof(sValue);
	if(R0w < 0.02) R0w = 0;

	for(int cnt = startpos; cnt <= endpos;cnt++){
		sId.Format("Z%d",cnt);
		if(!mstr.Lookup(sId,sValue) || sValue.FindOneOf("0123456789") < 0)
			continue;
		if((sValue.FindOneOf("0123456789") >= 0) &&(sValue.Find("--") < 0))
		{
			//retrieve iGrp & iRst
			int iGrp,iRst;
			iGrp = IndexDecomposition(cnt);
			if(iGrp < 0) continue;

			iRst = iGrp % 100; iGrp = (iGrp-iRst)/100;
			
			double dvalue = atof(sValue);
			double delta= abs(dvalue- IndexComposition(cnt));
			
			

			if(iGrp == 0) //collect R1,R2 max & min
			{
				if(iRst > 5)  {
					//R2 case
					if((delta > 0) && (delta > R1max)) R1max = delta;
					if((delta < 0) && (delta < R1min)) R1min = delta;
				}else{
					//R1 case
					if((delta > 0) && (delta > R2max)) R2max = delta;
					if((delta < 0) && (delta < R2min)) R2min = delta;
				}
			}
		}		
	}

	int group = 1;
	if(!bAbort){
		double Delta12n[11][11],Delta12w[11][11];
		double dr1,dr2;

#define P1BASE	151
#define P2BASE  131
#define DIFFNW  120
		
		for(int P1=0;P1 < 10; P1++) {
			sId.Format("Z%d",P1+P1BASE);mstr.Lookup(sId,sValue);dr1 = atof(sValue);
			for(int P2 = 0; P2 < 10; P2++){
				double dRange;
				//nei pan
				sId.Format("Z%d",P2+P2BASE);mstr.Lookup(sId,sValue);dr2 = atof(sValue);

				Delta12n[P1][P2] = (dr1+R0n+dr2)/(IndexComposition(P1+P1BASE)+IndexComposition(P2+P2BASE)) -1;
				dRange = (g_iPanEnd == 5) ? (1.0e-1):(1.0e-1);
				if((Delta12n[P1][P2] >=  0)	&& (R1max-R2min+Delta12n[P1][P2]) > dRange)
					sJdjl.Format("%s,内臂第I盘第%d点与第II盘第%d点组合不合格,",sJdjl,P1+1,P2+1);
				if((Delta12n[P1][P2] <=  0)	&& (R1min-R2max+Delta12n[P1][P2]) < -dRange)
					sJdjl.Format("%s,内臂第I盘第%d点与第II盘第%d点组合不合格,",sJdjl,P1+1,P2+1);
				
				//wai pan
				sId.Format("Z%d",P1+P1BASE+DIFFNW);mstr.Lookup(sId,sValue);dr1 = atof(sValue);
				sId.Format("Z%d",P2+P2BASE+DIFFNW);mstr.Lookup(sId,sValue);dr2 = atof(sValue);

				Delta12w[P1][P2] = (dr1+R0w+dr2)/(IndexComposition(P1+P1BASE)+IndexComposition(P2+P2BASE)) -1;
				dRange = (g_iPanEnd == 5) ? (1.0e-2):(1.0e-2);
				if((Delta12w[P1][P2] >=  0) && ((1.2*Delta12w[P1][P2] - 0.2*Delta12n[P1][P2] - 1.2*R1min + 0.2*R2min) > dRange))
					sJdjl.Format("%s,外臂第I盘第%d点与第II盘第%d点组合不合格,",sJdjl,P1,P2);
				if((Delta12w[P1][P2] <=  0) && ((1.2*Delta12w[P1][P2] - 0.2*Delta12n[P1][P2] - 1.2*R1max + 0.2*R2max) < -dRange))
					sJdjl.Format("%s,外臂第I盘第%d点与第II盘第%d点组合不合格,",sJdjl,P1,P2);
			}
		}
	}
	return sJdjl;
}

PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	if(!DetermineType(mstr)) return;
	g_jherr = CheckDataForCheck(mstr,0,indexmax,true);	//special check to dianqiao
	for(int cnt = 1; cnt <= 400; cnt++){
		CString err;
		if(CHECK_REC_FAIL == sjCheckOneRecord(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,cnt,true,true,err,false))
			g_jherr += err;
	}
	mstr.SetAt("jdjg",g_jherr);//检定结果
}


PRGINTERFACE_API void  PrepareArrayForReport(CMapStringToString &mstr)
{
	return;
}

PRGINTERFACE_API void  DataIntegrateForCheck(CMapStringToString &mstr, int indexmax, bool colormark)
{
	if(!DetermineType(mstr)) return;
	g_jherr = CheckDataForCheck(mstr,0,indexmax,true);	//special check to dianqiao
	for(int cnt = 1; cnt <= 400; cnt++){
		CString err;
		if(CHECK_REC_FAIL == sjCheckOneRecord(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,cnt,colormark,false,err,false))
			g_jherr += err;
	}
	mstr.SetAt("jdjg",g_jherr);//检定结果
}

PRGINTERFACE_API bool CheckPassFail(CMapStringToString &mstr, int pos)
{
	CString err;
	return (CHECK_REC_FAIL != sjCheckOneRecord(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,pos,false,false,err,true));
}
