#include "stdafx.h"
#include "Tester.h"
#include "NbzHandler.h"
#include "math.h"

extern char SJD2SGetchar(double value,int posn);
extern CString SJD2SByPosition(double value,int lastpos);
extern int SJD2SGetcarryC(char vchar,char ochar,int sel);
extern int SJD2SGetcarryI(int vchar,int ochar,int sel);


//mdb related

const char* CNbzHandler::PathQuery(int clsid,int pathid)
{
	const char*	pathdef[][3]={
				/*MDB*/		/*JS*/							/*GROUP*/
	/*REG	*/	"reg",		"\\Html\\imgorg\\reg.js",		"*",	
	/*DATA	*/	"nbzdata",	"\\Html\\imgorg\\nbzdata.js",	"*",	
	/*JH	*/	"jh",		"\\Html\\imgorg\\nbzjh.js",		"nbz",	
	/*REPORT*/	"*",		"\\Html\\imgorg\\report.js",	"*",	
	/*TEST	*/	"*",		"\\Html\\imgorg\\nbztest.js",	"*",	
	/*CONF1	*/	"conf",		"\\Html\\imgorg\\zxconf1.js",	"nbzconf1",	
	/*CONF2	*/	"conf",		"\\Html\\imgorg\\zxconf2.js",	"nbzconf2",	
	/*TBR	*/	"*",		"\\Html\\imgorg\\tbr.js",		"*",	
	/*WAIT	*/	"*",		"\\Html\\wait.htm",				"*",	
	};
	return pathdef[pathid-1][clsid-1];
}

extern CString g_Path;
extern CString g_TestType;
extern CMapStringToString g_msTemp;
extern CTester *g_Tester;
extern TESTERCONF g_TesterConf;




void DataIntegrateForCompensate(CMapStringToString& mstr,int indexmax);


bool CNbzHandler::Handler(CString spage,CString op,CString scmd)
{
	return CHtmlHandler::Handler(spage,op,scmd);
}

bool CNbzHandler::ReportHandler(CString op,CString scmd)
{	
	return CHtmlHandler::ReportHandler(op,scmd);
}
bool CNbzHandler::DataHandler(CString op,CString scmd)
{
	if(op.Compare("compensate") == 0)
	{
		SqlSelect(PathQuery(CLSMDB,IDDATA),m_RGroup.sDataSel,m_RGroup.sRegSel);
		SqlSelect(PathQuery(CLSMDB,IDJH),m_RGroup.sJhSel1,PathQuery(CLSGROUP,IDJH));
		DataIntegrateForCompensate(g_msTemp,ZINDEX_MAX);
	}
	return CHtmlHandler::DataHandler(op,scmd);
}
bool CNbzHandler::JhHandler(CString op,CString scmd)
{
	return CHtmlHandler::JhHandler(op,scmd);
}
bool CNbzHandler::TestHandler(CString op,CString scmd)
{
	return CHtmlHandler::TestHandler(op,scmd);
}

bool CNbzHandler::Configure1Handler(CString op,CString scmd)
{
	return CHtmlHandler::Configure1Handler(op,scmd);
}
bool CNbzHandler::Configure2Handler(CString op,CString scmd)
{
	return CHtmlHandler::Configure2Handler(op,scmd);
}

bool CNbzHandler::RegHandler(CString op,CString scmd)
{
	return CHtmlHandler::RegHandler(op,scmd);
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
	
	double stdval = pow(10,iGrp-3)*INDEX2REST(zindex);//multiple the coefficient and get the std
	
	return (value-stdval);
}
static bool CheckData(int zindex,double value,double accuracy)
{
	int iGrp = INDEX2GROUP(zindex);

	if((iGrp > 7) || (iGrp < 0)) 	return true;//is not a resister value,should be handle later

	if ( INDEX2REST(zindex) > 11)	return true;//is not a resister value,should be handle later

	double stdval = pow(10,iGrp-3)*INDEX2REST(zindex);//multiple the coefficient and get the std
	

	return ((fabs(value-stdval)) < accuracy * fabs(stdval));
}

//convert the double data to some specified precision and mantissa
static CString DataIntergrate(int zindex,double value,int digipos,int mantissa)
{
	int iGrp = INDEX2GROUP(zindex);//0-0.001,1-0.01 ....7-10000
	
	if((iGrp > 7) || (iGrp < 0)) //not a resister value, case should be handled here later
	{
		CString tmp;
		tmp.Format("%.4f",value);
		return tmp;
	}
	char nchar = SJD2SGetchar(value,iGrp+digipos-1);//last char
	char n1char = SJD2SGetchar(value,iGrp+digipos);//next last char
	int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',mantissa);//get carry char

	value += carry*pow(10.0,iGrp+digipos);//adjust

	return SJD2SByPosition(value,iGrp+digipos);
}

void CNbzHandler::DataIntegrateForCheck(CMapStringToString& mstr,int indexmax,bool colormark)
{
	mstr.SetAt("0.005","0");	mstr.SetAt("0.01","1");	mstr.SetAt("0.02","2");
	mstr.SetAt("0.05","3");		mstr.SetAt("0.1","4");	mstr.SetAt("0.2","5");
	mstr.SetAt("0.5","6");		mstr.SetAt("1","7");	mstr.SetAt("2","8");
	mstr.SetAt("5","9");
	static int g_Digipos[] = {-7,-6,-6,-6,-5,-5,-5,-4,-4,-4,-3};
	static int g_Mantissa[]= { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	CString perrecord;
	CString jherr("");
	int ipos = indexmax;
	
	while(ipos-- > 0)
	{
		CString sid;
		double accuracy;
		sid.Format("Z%d",ipos);
		if(mstr.Lookup(sid,perrecord))
		{
			if(perrecord.Find("--") >= 0)
				continue;
			sid.Format("jhsel%d",INDEX2GROUP(ipos));
			if(!mstr.Lookup(sid,sid))		continue;//retrieve the jhsel

			accuracy = atof(sid)/100.0;

			if(!mstr.Lookup(sid,sid))		continue;//translate the jhsel
			perrecord = DataIntergrate(ipos,atof(perrecord),g_Digipos[atoi(sid)]-2,g_Mantissa[atoi(sid)]);
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
void CNbzHandler::DataIntegrateForCompensate(CMapStringToString& mstr,int indexmax)
{
	mstr.SetAt("0.005","0");	mstr.SetAt("0.01","1");	mstr.SetAt("0.02","2");
	mstr.SetAt("0.05","3");		mstr.SetAt("0.1","4");	mstr.SetAt("0.2","5");
	mstr.SetAt("0.5","6");		mstr.SetAt("1","7");	mstr.SetAt("2","8");
	mstr.SetAt("5","9");		mstr.SetAt("5","10");
	static int g_Digipos[] = {-7,-6,-6,-6,-5,-5,-5,-4,-4,-4,-3};
	static int g_Mantissa[]= { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	CString perrecord;
	int ipos = indexmax;
	
	while(ipos-- > 0)
	{
		CString sid;
		double accuracy;
		sid.Format("Z%d",ipos);
		if(mstr.Lookup(sid,perrecord))
		{
			if(perrecord.Find("--") >= 0)
				continue;
			sid.Format("jhsel%d",INDEX2GROUP(ipos));
			if(!mstr.Lookup(sid,sid))		continue;//retrieve the jhsel
			
			accuracy = atof(sid)/100.0;
			
			if(!mstr.Lookup(sid,sid))		continue;//translate the jhsel
			perrecord = DataIntergrate(ipos,atof(perrecord),g_Digipos[atoi(sid)]-2,g_Mantissa[atoi(sid)]);
			sid=SJD2SByPosition(GetDiffData(ipos,atof(perrecord),accuracy),INDEX2GROUP(ipos)+g_Digipos[atoi(sid)]-3);
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

void CNbzHandler::PrepareArrayForReport(CMapStringToString& mstr)
{
	//check the start position of the resister group
	int grpoffset=0;
	CString skey,sval;
	do{
		skey.Format("Z%d",31+grpoffset++*20);
		if(mstr.Lookup(skey,sval))
			break;
	}while(grpoffset<9);
	
	--grpoffset;
	
				
		for (int grpindex = 0;grpindex < 8;grpindex++){
			skey.Format("Z%d",31+grpindex*20);
			if (mstr.Lookup(skey,sval))
			{
				if(sval.FindOneOf("0123456789") != -1)
				{
					skey.Format("Z%d",31+grpindex*20+12);
					mstr.SetAt(skey,CString(Panhao[grpindex]));
					skey.Format("jhsel%d",grpindex);
					if(mstr.Lookup(skey,sval))
					{
						sval = _T("±")+sval;
						skey.Format("Z%d",31+grpindex*20+13);
						mstr.SetAt(skey,sval);
					}
				}
			}
		}
		
		//move the table and insert "/" at corresponding place
		for ( grpindex = 0;grpindex < 8;grpindex++)
			for (int subindex = 0;subindex< 14;subindex++){
				skey.Format("Z%d",31+grpindex*20+subindex);
				if (!mstr.Lookup(skey,sval))
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








