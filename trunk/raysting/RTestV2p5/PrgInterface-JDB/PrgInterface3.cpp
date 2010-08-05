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
		/*REG	*/	"reg",		"*",							"jdb",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\jdbjh.htm",		"jdb",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\jdbtest.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        "接地电阻表",							"接地电阻表测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}
//RGROUP,start_pos,end_pos,base_pos,scale,jhsel,jhlist, digipos, mantissa, prec, NULL, errmsg
//RGIFZ,start_pos,end_pos,base_pos,scale,jhsel,jhlist, digipos, mantissa, prec, reference, errmsg
//RSINGLE,start_pos,end_pos,base_pos, scale(no multiple further),jhsel.jhlist,digipos,mantissa,prec,NULL,errmsg
//RIFZ,start_pos,end_pos,base_pos, scale(no multiple further),jhsel.jhlist,digipos,mantissa,prec,reference,errmsg
//RSINGLE,start_pos,end_pos,base_pos,scale,position_in_digi_manti_prec,jhlist, digipos, mantissa, prec, NULL, errmsg

PRECBASE2 g_Cfg[] = {
	{PREC_TYPE_RGROUP,31,49,30,"0.001","jhsel0",JHSEL_0005_50,"-25",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,51,69,50,"0.01","jhsel1",JHSEL_0005_50,"-22",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,71,89,70,"0.1","jhsel2",JHSEL_0005_50,"-19",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,91,109,90,"1","jhsel3",JHSEL_0005_50,"-16",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,111,129,110,"10","jhsel4",JHSEL_0005_50,"-13",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,131,149,130,"100","jhsel5",JHSEL_0005_50,"-10",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,151,169,150,"1000","jhsel6",JHSEL_0005_50,"-7",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,171,189,170,"10000","jhsel7",JHSEL_0005_50,"-4",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},

	{PREC_TYPE_RSINGLE,90,90,0,"0.8","jhsel2",JHSEL_0005_50,MANTI_13_1,PREC_5E5_5E1,NULL,"X1ohm盘第0.8点不合格"},
	{PREC_TYPE_RSINGLE,70,70,0,"0.08","jhsel1",JHSEL_0005_50,MANTI_13_1,PREC_5E5_5E1,NULL,"X0.1ohm盘第0.8点不合格"},

	{PREC_TYPE_RCAN,   19,19,0,"-0.02,0.001,0.01,0.1,1,10,100,1000,10000","jhsel0,jhsel1,jhsel2,jhsel3,jhsel4,jhsel5,jhsel6,jhsel7","1","-4","1","1","Z31,Z51,Z71,Z91,Z111,Z131,Z151,Z171","残余电阻不合格"},
	{PREC_TYPE_RSINGLE,20,20,0,"-0.1","1",NULL,"-2,","1,","1",NULL,"辅助接地电阻 0Ω(Rp)"},
	{PREC_TYPE_RSINGLE,21,21,0,"500","jhsel8",JHSEL_0005_50,"-10,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 500Ω(Rp)"},
	{PREC_TYPE_RSINGLE,22,22,0,"1000","jhsel9",JHSEL_0005_50,"-7,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 1000Ω(Rp)"},
	{PREC_TYPE_RSINGLE,23,23,0,"2000","jhsel18",JHSEL_0005_50,"-7,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 2000Ω(Rp)"},
	{PREC_TYPE_RSINGLE,24,24,0,"5000","jhsel19",JHSEL_0005_50,"-7,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 5000Ω(Rp)"},

	{PREC_TYPE_RSINGLE,25,25,0,"-0.1","1",NULL,"-2,","1,","1",NULL,"辅助接地电阻 0Ω(Rc)"},
	{PREC_TYPE_RSINGLE,26,26,0,"500","jhsel8",NULL,"-10,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 500Ω(Rc)"},
	{PREC_TYPE_RSINGLE,27,27,0,"1000","jhsel9",NULL,"-7,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 1000Ω(Rc)"},
	{PREC_TYPE_RSINGLE,28,28,0,"2000","jhsel18",NULL,"-7,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 2000Ω(Rc)"},
	{PREC_TYPE_RSINGLE,29,29,0,"5000","jhsel19",NULL,"-7,",MANTI_13_1,PREC_5E5_5E1,NULL,"辅助接地电阻 5000Ω(Rc)"},
};



PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	g_jherr.Empty();
	
	for(int cnt = 19; cnt <= 200; cnt++){
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
	
	for(int cnt = 19; cnt <= 200; cnt++){
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
