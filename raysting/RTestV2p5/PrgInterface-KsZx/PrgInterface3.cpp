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
		/*REG	*/	"reg",		"*",							"zx",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\zxjh.htm",		"zx",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\zxtest.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        "电阻箱",							"电阻箱快速测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}


PRECBASE2 g_Cfg[] = {
	{PREC_TYPE_RGROUP,31,41,30,"0.001","jhsel0",JHSEL_0005_50,"-25",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,51,61,50,"0.01","jhsel1",JHSEL_0005_50,"-22",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,71,81,70,"0.1","jhsel2",JHSEL_0005_50,"-19",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,91,101,90,"1","jhsel3",JHSEL_0005_50,"-16",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,111,121,110,"10","jhsel4",JHSEL_0005_50,"-13",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,131,141,130,"100","jhsel5",JHSEL_0005_50,"-10",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,151,161,150,"1000","jhsel6",JHSEL_0005_50,"-7",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,171,181,170,"10000","jhsel7",JHSEL_0005_50,"-4",MANTI_13_1,PREC_5E5_5E1,NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	
	{PREC_TYPE_RCAN,   19,19,0,"-0.02,0.001,0.01,0.1,1,10,100,1000,10000","jhsel0,jhsel1,jhsel2,jhsel3,jhsel4,jhsel5,jhsel6,jhsel7","1","-4","1","1","Z31,Z51,Z71,Z91,Z111,Z131,Z151,Z171","残余电阻不合格"},
	{PREC_TYPE_BIANCHA,17,17,0,"-0.02,0.001,0.01,0.1,1,10,100,1000,10000","jhsel0,jhsel1,jhsel2,jhsel3,jhsel4,jhsel5,jhsel6,jhsel7","1","-4","1","1","Z31,Z51,Z71,Z91,Z111,Z131,Z151,Z171","开关变差不合格"},

	{PREC_TYPE_FORMAT,42,42,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X0.001ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,62,62,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X0.01ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,82,82,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X0.1ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,102,102,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X1ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,122,122,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X10ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,142,142,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X100ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,162,162,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X1000ohm盘变差不合格"},
	{PREC_TYPE_FORMAT,182,182,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"X10000ohm盘变差不合格"},

	{PREC_TYPE_FORMAT,26,26,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"第一次残余电阻"},
	{PREC_TYPE_FORMAT,27,27,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"第二次残余电阻"},
	{PREC_TYPE_FORMAT,28,28,0,"-0.01","1",NULL,"-4,","1,","1,",NULL,"第三次残余电阻"},
};
PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	g_jherr.Empty();
	
	for(int cnt = 1; cnt <= 400; cnt++){
		CString err;
		if(CHECK_REC_FAIL == sjCheckOneRecordFull(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,cnt,true,true,err,false,true))
			g_jherr += err;
	}
	mstr.SetAt("jdjg",g_jherr);//检定结果
}


PRGINTERFACE_API void  PrepareArrayForReport(CMapStringToString &mstr)
{
	const static char * Panhao[] ={
	_T("0.001Ω"),_T("0.01Ω"),_T("0.1Ω"),_T("1Ω"),_T("10Ω"),_T("100Ω"),_T("1000Ω"),_T("10000Ω")};

	//check the start position of the resister group
	int grpoffset=0;
	CString skey,sval;
	do{
		skey.Format("Z%d",31+grpoffset++*20);
		if(mstr.Lookup(skey,sval) && sval.FindOneOf("0123456789") >= 0)
			break;
	}while(grpoffset<9);
	
	--grpoffset;
	
				
	for (int grpindex = 0;grpindex < 8;grpindex++){
		skey.Format("Z%d",31+grpindex*20);
		if (mstr.Lookup(skey,sval) && (sval.FindOneOf("0123456789") >= 0) )
		{
			if(sval.FindOneOf("0123456789") != -1)
			{
				skey.Format("Z%d",31+grpindex*20+12);
				mstr.SetAt(skey,CString(Panhao[grpindex]));
				skey.Format("jhsel%d",grpindex);
				if(mstr.Lookup(skey,sval) && (sval.FindOneOf("0123456789") >= 0))
				{
					sval = _T("±")+sval;
					skey.Format("Z%d",31+grpindex*20+13);
					mstr.SetAt(skey,sval);
				}
			}
		}
	}
	return;
	//move the table and insert "/" at corresponding place
	for ( grpindex = 0;grpindex < 8;grpindex++)
		for (int subindex = 0;subindex< 14;subindex++){
			skey.Format("Z%d",31+grpindex*20+subindex);
			if (!mstr.Lookup(skey,sval) || sval.FindOneOf("0123456789") < 0)
				mstr.SetAt(skey,_T("/"));
			else	if(sval.FindOneOf("0123456789") == -1)
				mstr.SetAt(skey,_T("/"));
			else {
				mstr.RemoveKey(skey);
				skey.Format("Z%d",31+(grpindex-grpoffset)*20+subindex);
				mstr.SetAt(skey,sval);
			}
			
		}
}

PRGINTERFACE_API void  DataIntegrateForCheck(CMapStringToString &mstr, int indexmax, bool colormark)
{
	g_jherr.Empty();
	
	for(int cnt = 1; cnt <= 400; cnt++){
		CString err;
		if(CHECK_REC_FAIL == sjCheckOneRecordFull(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,cnt,colormark,false,err,false,true))
			g_jherr += err;
	}
	mstr.SetAt("jdjg",g_jherr);//检定结果
}

PRGINTERFACE_API bool CheckPassFail(CMapStringToString &mstr, int pos)
{
	CString err;
	return (CHECK_REC_FAIL != sjCheckOneRecordFull(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,pos,false,false,err,true,true));
}

