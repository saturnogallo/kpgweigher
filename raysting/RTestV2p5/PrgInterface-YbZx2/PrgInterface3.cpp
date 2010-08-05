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
#define PRGINTERFACE_API __declspec(dllexport)

PRGINTERFACE_API const char*  PathQuery(int pathid, int clsid)
{
	const char*	pathdef[]={
		/*MDB*/						/*JS*/					/*GROUP*/
		/*REG	*/	"reg",		"*",							"zx",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\zxjh.htm",		"zx",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\zxtest2.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        "电阻箱",							"电阻箱一般测量2"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}

//convert the zindex to group 0=0.001,1=0.01 ....7=10000
#define INDEX2GROUP(x)	int(floor((x-30)/20.0))
#define INDEX2REST(x)	(x - INDEX2GROUP(x)*20-30)

const static char * Panhao[] ={
	_T("0.001Ω"),_T("0.01Ω"),_T("0.1Ω"),_T("1Ω"),_T("10Ω"),_T("100Ω"),_T("1000Ω"),_T("10000Ω")};
	
//check whether is value is in the range
static double GetDiffData(int zindex,double value,double accuracy)
{
	int iGrp = INDEX2GROUP(zindex);
	
	if((iGrp > 7) || (iGrp < 0)) 	return value;//is not a resister value,should be handle later
	
	if ( INDEX2REST(zindex) > 11)	return value;//is not a resister value,should be handle later
	
	double stdval = pow(10.0,iGrp-3)*INDEX2REST(zindex);//multiple the coefficient and get the std
	
	return (value-stdval);
}
static bool CheckData(int zindex,double value,double accuracy)
{
	int iGrp = INDEX2GROUP(zindex);

	if((iGrp > 7) || (iGrp < 0)) 	return true;//is not a resister value,should be handle later

	if ( INDEX2REST(zindex) > 11)	return true;//is not a resister value,should be handle later

	double stdval = pow(10.0,iGrp-3)*INDEX2REST(zindex);//multiple the coefficient and get the std
	

	return ((fabs(value-stdval)) < accuracy * fabs(stdval));
}
//determine whether need to cut zero , added by sojo 07-07-14
//convert the double data to some specified precision and mantissa
double zero;
int z14;

static double getzero(int zindex){
	CString perrecord;
	CString sid;

	for(int i=3;i < 17;i=i+2)
	{
		if((zindex >= (10*i+1)) && (zindex <= (10*i+11))){
			if(i == z14){
				return 0.0;
			}else{
				return zero;
			}
		}
	}
	return 0.0;
}


//convert the double data to some specified precision and mantissa

CString DataIntergrate(int zindex,double value,int digipos,int mantissa)
{
	int iGrp = INDEX2GROUP(zindex);//0-0.001,1-0.01 ....7-10000
	value = value - getzero(zindex);
	if((iGrp > 7) || (iGrp < 0)) //not a resister value, case should be handled here later
	{
		CString tmp;
		tmp.Format("%.4f",value);
		return tmp;
	}
	char nchar = SJD2SGetchar(value,iGrp+digipos-1);//last char
	char n1char = SJD2SGetchar(value,iGrp+digipos);//next last char
	mantissa = 1; //force using natural rule
	int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',mantissa);//get carry char

	value += carry*pow(10.0,iGrp+digipos);//adjust

	return SJD2SByPosition(value,iGrp+digipos);
}
//get the Precise configuration according to group id and base and precise define stored in mstr
// return value is the digipos*100+manssita depend on digipos's position
typedef struct _PRECBASE{
	int index;
	const char* sLevel;
	int iDigipos;
	int iMantissa;
}PRECBASE;

PRECBASE g_PrecBase[] = {
	// index,slevel, digi, mantissa
		{0, "0.0005", -8 , 1},
		{1, "0.001",  -7 , 1},
		{2, "0.002",  -7 , 1},
		{3, "0.005",  -7 , 1},
		{4, "0.01",   -6 , 1},
		{5, "0.02",   -6 , 1},
		{6, "0.05",	  -6 , 1},
		{7, "0.1",	  -5 , 1},
		{8, "0.2",	  -5 , 1},
		{9, "0.5",	  -5 , 1},
		{10,"1",	  -4 , 1},
		{11,"2",	  -4 , 1},
		{12,"5",	  -4 , 1},
		{13,"10",	  -3 , 1},
		{14,"20",	  -3 , 1},
		{13,"30",	  -3 , 1},
};
static int CheckPrecious(CString strPrec)
{
		for(int cnt=0; cnt < sizeof(g_PrecBase)/sizeof(PRECBASE);cnt++){
			if(strPrec.Compare(g_PrecBase[cnt].sLevel) == 0)
				return cnt;
		}
		return -1;
}
PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	CString perrecord;
	int ipos = indexmax;
	zero = 0.0;
	if(mstr.Lookup(_T("Z19"),perrecord) && (perrecord.FindOneOf("0123456789") >= 0)){
		zero = atof(perrecord);
	}
	z14 = 0;
	if(mstr.Lookup(_T("Z14"),perrecord) && (perrecord.FindOneOf("0123456789") >= 0)){
		z14 = atoi(perrecord);
	}
	
	while(ipos-- > 0)
	{
		CString sid;
		double accuracy;
		sid.Format("Z%d",ipos);
		if(mstr.Lookup(sid,perrecord) && (perrecord.FindOneOf("0123456789") >= 0))
		{
			if(perrecord.Find("--") >= 0)
				continue;
			sid.Format("jhsel%d",INDEX2GROUP(ipos));
			if(!mstr.Lookup(sid,sid) || sid.FindOneOf("0123456789") < 0)		continue;//retrieve the jhsel
			
			accuracy = atof(sid)/100.0;
			
			int iid = CheckPrecious(sid);
			if(iid < 0)		continue;//translate the jhsel
			perrecord = DataIntergrate(ipos,atof(perrecord),g_PrecBase[iid].iDigipos-2,g_PrecBase[iid].iMantissa);
			sid=SJD2SByPosition(GetDiffData(ipos,atof(perrecord),accuracy),INDEX2GROUP(ipos)+g_PrecBase[iid].iDigipos-3);
			if(!CheckData(ipos,atof(perrecord),accuracy))
			{
				sid = "<font color='red'>"+sid+"</font>";
			}
			perrecord = sid;
			sid.Format("Z%d",ipos);
			mstr.SetAt(sid,perrecord);
		}
	}
}


PRGINTERFACE_API void  PrepareArrayForReport(CMapStringToString &mstr)
{
	//check the start position of the resister group
	int grpoffset=0;
	CString skey,sval,perrecord;
	zero = 0.0;
	if(mstr.Lookup(_T("Z19"),perrecord) && (perrecord.FindOneOf("0123456789") >= 0)){
		zero = atof(perrecord);
	}
	z14 = 0;
	if(mstr.Lookup(_T("Z14"),perrecord) && (perrecord.FindOneOf("0123456789") >= 0)){
		z14 = atoi(perrecord);
	}

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
	CString perrecord;
	CString jherr("");
	int ipos = indexmax;

	zero = 0.0;
	if(mstr.Lookup(_T("Z19"),perrecord) && (perrecord.FindOneOf("0123456789") >= 0)){
		zero = atof(perrecord);
	}
	z14 = 0;
	if(mstr.Lookup(_T("Z14"),perrecord) && (perrecord.FindOneOf("0123456789") >= 0)){
		z14 = atoi(perrecord);
	}
	while(ipos-- > 0)
	{
		CString sid;
		double accuracy;
		sid.Format("Z%d",ipos);
		if(mstr.Lookup(sid,perrecord) && (perrecord.FindOneOf("0123456789") >= 0))
		{
			if(perrecord.Find("--") >= 0)
				continue;
			sid.Format("jhsel%d",INDEX2GROUP(ipos));
			if(!mstr.Lookup(sid,sid) || (sid.FindOneOf("0123456789") < 0) )		continue;//retrieve the jhsel
						
			accuracy = atof(sid)/100.0;
			
			int iid = CheckPrecious(sid);
			if(iid < 0)		continue;//translate the jhsel//translate the jhsel
			perrecord = DataIntergrate(ipos,atof(perrecord),g_PrecBase[iid].iDigipos-2,g_PrecBase[iid].iMantissa);
			if((!CheckData(ipos,atof(perrecord),accuracy)))
			{
				//add the a message to the error
				CString stmp;
				stmp.Format("%s盘第%d点不合格;\n",Panhao[INDEX2GROUP(ipos)],INDEX2REST(ipos));
				jherr += stmp;
				if(colormark)
					perrecord = "<font color='red'>"+perrecord+"</font>";
			}
			sid.Format("Z%d",ipos);
			mstr.SetAt(sid,perrecord);
		}

	}
	mstr.SetAt("jdjg",jherr);//检定结果
}

PRGINTERFACE_API bool CheckPassFail(CMapStringToString &mstr, int pos)
{
	return true;
}
