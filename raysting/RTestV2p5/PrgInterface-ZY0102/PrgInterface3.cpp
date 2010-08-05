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

PRGINTERFACE_API const char*  PathQuery(int pathid, int clsid)
{
	const char*	pathdef[]={
		/*MDB*/						/*JS*/					/*GROUP*/
		/*REG	*/	"reg",		"*",							"ZY0102",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\zy0102jh.htm",	"ZY0102",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\zy0102test.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",							"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        "ZY0102",							"ZY0102直流低值电阻表校验器测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}

CString g_jherr;

PRECBASE2 g_Cfg[] = {
		{PREC_TYPE_RSINGLE,41,41,0,"19000","jhsel11",JHSEL_0001_50,"0",MANTI_15_1,PREC_1E5_5E1,NULL,"19KΩ不合格"},
		{PREC_TYPE_RSINGLE,42,42,0,"10000","jhsel10",JHSEL_0001_50,"-3",MANTI_15_1,PREC_1E5_5E1,NULL,"10KΩ不合格"},
		{PREC_TYPE_RSINGLE,43,43,0,"1900","jhsel10",JHSEL_0001_50,"-3",MANTI_15_1,PREC_1E5_5E1,NULL,"1.9KΩ不合格"},
		{PREC_TYPE_RSINGLE,44,44,0,"1000","jhsel9",JHSEL_0001_50,"-6",MANTI_15_1,PREC_1E5_5E1,NULL,"1KΩ不合格"},
		{PREC_TYPE_RSINGLE,45,45,0,"190","jhsel9",JHSEL_0001_50,"-6",MANTI_15_1,PREC_1E5_5E1,NULL,"190Ω不合格"},
		{PREC_TYPE_RSINGLE,46,46,0,"100","jhsel8",JHSEL_0001_50,"-9",MANTI_15_1,PREC_1E5_5E1,NULL,"100Ω不合格"},
		{PREC_TYPE_RSINGLE,47,47,0,"19","jhsel8",JHSEL_0001_50,"-9",MANTI_15_1,PREC_1E5_5E1,NULL,"19Ω不合格"},
		{PREC_TYPE_RSINGLE,48,48,0,"10","jhsel7",JHSEL_0001_50,"-12",MANTI_15_1,PREC_1E5_5E1,NULL,"10Ω不合格"},
		{PREC_TYPE_RSINGLE,49,49,0,"1.9","jhsel7",JHSEL_0001_50,"-12",MANTI_15_1,PREC_1E5_5E1,NULL,"1.9Ω不合格"},

		{PREC_TYPE_RSINGLE,50,50,0,"1","jhsel6",JHSEL_0001_50,"-15",MANTI_15_1,PREC_1E5_5E1,NULL,"1Ω不合格"},
		{PREC_TYPE_RSINGLE,51,51,0,"0.19","jhsel6",JHSEL_0001_50,"-15",MANTI_15_1,PREC_1E5_5E1,NULL,"190mΩ不合格"},
		{PREC_TYPE_RSINGLE,52,52,0,"0.1","jhsel5",JHSEL_0001_50,"-18",MANTI_15_1,PREC_1E5_5E1,NULL,"100mΩ不合格"},
		{PREC_TYPE_RSINGLE,53,53,0,"0.019","jhsel5",JHSEL_0001_50,"-18",MANTI_15_1,PREC_1E5_5E1,NULL,"19mΩ不合格"},
		{PREC_TYPE_RSINGLE,54,54,0,"0.01","jhsel4",JHSEL_0001_50,"-21",MANTI_15_1,PREC_1E5_5E1,NULL,"10mΩ不合格"},
		{PREC_TYPE_RSINGLE,55,55,0,"0.0019","jhsel4",JHSEL_0001_50,"-21",MANTI_15_1,PREC_1E5_5E1,NULL,"1.9mΩ不合格"},
		{PREC_TYPE_RSINGLE,56,56,0,"0.001","jhsel3",JHSEL_0001_50,"-24",MANTI_15_1,PREC_1E5_5E1,NULL,"1mΩ不合格"},
		{PREC_TYPE_RSINGLE,57,57,0,"0.00019","jhsel3",JHSEL_0001_50,"-24",MANTI_15_1,PREC_1E5_5E1,NULL,"190uΩ不合格"},
		{PREC_TYPE_RSINGLE,58,58,0,"0.0001","jhsel2",JHSEL_0001_50,"-27",MANTI_15_1,PREC_1E5_5E1,NULL,"100uΩ不合格"},
		{PREC_TYPE_RSINGLE,59,59,0,"0.000019","jhsel2",JHSEL_0001_50,"-27",MANTI_15_1,PREC_1E5_5E1,NULL,"19uΩ不合格"},
		{PREC_TYPE_RSINGLE,60,60,0,"0.00001","jhsel1",JHSEL_0001_50,"-30",MANTI_15_1,PREC_1E5_5E1,NULL,"10uΩ不合格"}

};

PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	g_jherr.Empty();
	for(int cnt = 31; cnt < 100; cnt++){
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
	g_jherr.Empty();
	for(int cnt = 31; cnt < 100; cnt++)
	{
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
