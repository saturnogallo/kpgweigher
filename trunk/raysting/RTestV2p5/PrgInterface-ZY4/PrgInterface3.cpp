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
		/*REG	*/	"reg",		"*",							"zy4",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\zy4jh.htm",		"zy4",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\zy4test.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        	"ZY4双桥",					"ZY4双桥标准电阻箱测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}

PRECBASE2 g_Cfg[] = {
	{PREC_TYPE_RGROUP,81,91,80,"10","jhsel10",JHSEL_0005_10,"-13",MANTI_11_1,PREC_5E5_1E1,NULL,"比较臂X{val_scope}盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,61,71,60,"1","jhsel9",JHSEL_0005_10,"-16",MANTI_11_1,PREC_5E5_1E1,NULL,"比较臂X{val_scope}盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,41,51,40,"0.1","jhsel8",JHSEL_0005_10,"-19",MANTI_11_1,PREC_5E5_1E1,NULL,"比较臂X{val_scope}盘第{rel_pos}点不合格"},

	{PREC_TYPE_RSINGLE,31,31,0,"1000","jhsel0",JHSEL_0005_10,"-7",MANTI_11_1,PREC_5E5_1E1,NULL,"比例臂1000ohm点不合格"},	
	{PREC_TYPE_RSINGLE,32,32,0,"100","jhsel1",JHSEL_0005_10,"-10",MANTI_11_1,PREC_5E5_1E1,NULL,"比例臂100ohm点不合格"},	
	{PREC_TYPE_RSINGLE,33,33,0,"10","jhsel2",JHSEL_0005_10,"-13",MANTI_11_1,PREC_5E5_1E1,NULL,"比例臂10ohm点不合格"},	
	{PREC_TYPE_RSINGLE,34,34,0,"1","jhsel3",JHSEL_0005_10,"-16",MANTI_11_1,PREC_5E5_1E1,NULL,"比例臂1ohm点不合格"},	
	{PREC_TYPE_RSINGLE,35,35,0,"0.1","jhsel4",JHSEL_0005_10,"-19",MANTI_11_1,PREC_5E5_1E1,NULL,"比例臂0.1ohm点不合格"},
	
	{PREC_TYPE_RSINGLE,36,36,0,"0.01","jhsel5",JHSEL_0005_50,"-22",MANTI_13_1,PREC_5E5_5E1,NULL,"比例臂0.01ohm点不合格"},
	{PREC_TYPE_RSINGLE,37,37,0,"0.001","jhsel6",JHSEL_0005_50,"-25",MANTI_13_1,PREC_5E5_5E1,NULL,"比例臂0.001ohm点不合格"},
	{PREC_TYPE_RSINGLE,38,38,0,"0.0001","jhsel7",JHSEL_0005_50,"-28",MANTI_13_1,PREC_5E5_5E1,NULL,"比例臂0.0001ohm点不合格"}
};

PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	g_jherr.Empty();
	
	for(int cnt = 31; cnt <= 99; cnt++){
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
	
	for(int cnt = 31; cnt <= 99; cnt++){
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

