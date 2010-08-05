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
		/*REG	*/	"reg",			"*",						"qj18a",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\qj18ajh.htm",	"qj18a",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\qj18atest.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",							"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        	"QJ18",					    "QJ18a直流测温电桥测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}

PRECBASE2 g_Cfg[] = {
	{PREC_TYPE_RGROUP,41,51,40,"1000","jhsel0","0.01,0.02,0.05","-2,-2,-2","1,2,5","5e-5,1e-4,2e-4",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,61,71,60,"100","jhsel0","0.01,0.02,0.05","-3,-3,-2","1,2,5","5e-5,1e-4,2e-4",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,81,91,80,"10","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","2e-4,5e-4,1e-3",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,101,111,100,"1","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-3,1e-3,1e-2",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,121,131,120,"0.1","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-2,1e-2,1e-1",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,141,151,140,"0.01","jhsel0","0.01,0.02","-4,-3","1,2","1e-1,1e-1",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},
	{PREC_TYPE_RGROUP,161,171,140,"0.001","jhsel0","0.01,","-4,","1,","3e-1,",NULL,"X{val_scope}ohm盘第{rel_pos}点不合格"},

	{PREC_TYPE_RSINGLE,26,26,0,"1000","jhsel0","0.01,0.02,0.05","-2,-2,-2","1,2,5","5e-5,1e-4,2e-4",NULL,"S1000不合格"},
	{PREC_TYPE_RSINGLE,27,27,0,"1000","jhsel0","0.01,0.02,0.05","-2,-2,-2","1,2,5","5e-5,1e-4,2e-4",NULL,"R1000不合格"},
	{PREC_TYPE_RSINGLE,28,28,0,"50","jhsel0","0.01,0.02,0.05","-3,-3,-2","1,2,5","5e-5,1e-4,2e-4",NULL,"R50不合格"},
	{PREC_TYPE_RSINGLE,29,29,0,"10","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","2e-4,5e-4,1e-3",NULL,"R10不合格"},
	{PREC_TYPE_RSINGLE,30,33,0,"0","jhsel0","0.01,0.02,0.05","-4,-4,-4","1,2,5","2e-3,2e-3,2e-3",NULL,"导线电阻不合格"},


	{PREC_TYPE_RSINGLE,25,25,0,"-3.33333e-5,-6.66666e-5,-1.66666e-4","jhsel0","0.01,0.02,0.05","-3,-3,-3","1,2,5","10,10,10",NULL,"倍率变差不合格"},

	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,0.02,0.05","-2,-2,-2","1,2,5","5e-5,1e-4,2e-4","Z41","倍率量程第6点不合格"},
	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,0.02,0.05","-3,-3,-2","1,2,5","5e-5,1e-4,2e-4","Z61","倍率量程第6点不合格"},
	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","2e-4,5e-4,1e-3","Z81","倍率量程第6点不合格"},
	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-3,1e-3,1e-2","Z101","倍率量程第6点不合格"},
	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-2,1e-2,1e-1","Z121","倍率量程第6点不合格"},
	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,0.02","-4,-3","1,2","1e-1,1e-1","Z141","倍率量程第6点不合格"},
	{PREC_TYPE_RIFZ,34,34,0,"5","jhsel0","0.01,","-4,","1,","3e-1,","Z161","倍率量程第6点不合格"},

	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,0.02,0.05","-2,-2,-2","1,2,5","5e-5,1e-4,2e-4","Z41","倍率量程第8点不合格"},
	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,0.02,0.05","-3,-3,-2","1,2,5","5e-5,1e-4,2e-4","Z61","倍率量程第8点不合格"},
	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","2e-4,5e-4,1e-3","Z81","倍率量程第8点不合格"},
	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-3,1e-3,1e-2","Z101","倍率量程第8点不合格"},
	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-2,1e-2,1e-1","Z121","倍率量程第8点不合格"},
	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,0.02","-4,-3","1,2","1e-1,1e-1","Z141","倍率量程第8点不合格"},
	{PREC_TYPE_RIFZ,35,35,0,"5","jhsel0","0.01,","-4,","1,","3e-1,","Z161","倍率量程第8点不合格"},

	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,0.02,0.05","-2,-2,-2","1,2,5","5e-5,1e-4,2e-4","Z41","倍率量程第10点不合格"},
	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,0.02,0.05","-3,-3,-2","1,2,5","5e-5,1e-4,2e-4","Z61","倍率量程第10点不合格"},
	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","2e-4,5e-4,1e-3","Z81","倍率量程第10点不合格"},
	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-3,1e-3,1e-2","Z101","倍率量程第10点不合格"},
	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,0.02,0.05","-4,-3,-2","1,2,5","1e-2,1e-2,1e-1","Z121","倍率量程第10点不合格"},
	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,0.02","-4,-3","1,2","1e-1,1e-1","Z141","倍率量程第10点不合格"},
	{PREC_TYPE_RIFZ,36,36,0,"5","jhsel0","0.01,","-4,","1,","3e-1,","Z161","倍率量程第10点不合格"},

	{PREC_TYPE_RCAN,   120,120,0,"-0.02,0.1","jhsel0","1","-5","1","1","Z121","残余电阻不合格"},
};



bool DetectItems(CMapStringToString &mstr,int i,int j,int k,double &di,double &dj, double &dk)
{
	CString skey,sval;
	skey.Format("Z%d",i);
	if (!mstr.Lookup(skey,sval) || sval.FindOneOf("0123456789") < 0)
		return false;
	di = atof(sval);

	skey.Format("Z%d",j);
	if (!mstr.Lookup(skey,sval) || sval.FindOneOf("0123456789") < 0)
		return false;
	dj = atof(sval);

	skey.Format("Z%d",k);
	if (!mstr.Lookup(skey,sval) || sval.FindOneOf("0123456789") < 0)
		return false;
	dk = atof(sval);
	return true;
}
void CaluResultItems(CMapStringToString &mstr)
{
	double di,dj,dk,di1,dj1,dk1;
	if(DetectItems(mstr,34,35,36,di,dj,dk))
	{
		for(int base = 46;base <= 166; base += 20)
		{
			if(DetectItems(mstr,base,base+2,base+4,di1,dj1,dk1))	
			{
				di = abs(di/di1);dj = abs(dj/dj1);dk = abs(dk/dk1);
				CString rst;
				rst.Format("%f",((di)+(dj)+(dk))/3.0);
				mstr.SetAt("Z39",rst);
				double min,max;
				if(di >= dj)
				{
					min = dj;max = di;
				}else
				{
					max = dj;min = di;
				}
				if(min >= dk)		min = dk;
				if(max <= dk)		max = dk;

				rst.Format("%f",(max-min)/3.0);
				mstr.SetAt("Z25",rst);
				return;
			}
		}
	}

}
PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	g_jherr.Empty();
	CaluResultItems(mstr);
	for(int cnt = 20; cnt <= 200; cnt++){
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
	CaluResultItems(mstr);
	for(int cnt = 20; cnt <= 200; cnt++){
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
