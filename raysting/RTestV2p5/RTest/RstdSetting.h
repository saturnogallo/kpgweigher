#ifndef _AFX_RSTDSETTING_H_021106
#define _AFX_RSTDSETTING_H_021106

#include "stdafx.h"
#include "afxtempl.h"
/**
 *	RSTDTYPE is a collection of the 
 *	resistance criterion used in test and stored in file;
 */

typedef struct _RSTDTYPE
{
	CString	sTitle;
	BOOL	bUse[7];
	double	dAlpha[7];
	double	dBeta[7];
	double	dValue[7];
}RSTDTYPE;

/**
 *	CParamSetting class is used to manage the resistance criterion list
 *	for example,store or install from to file,and delete,add 
 *	operations . though somewhat similiar to CParamSetting, we use copy
 *	of the default setting to manange the use of default after check
 */
class CRstdSetting
{
public:
	CRstdSetting();
	RSTDTYPE GetDefault(); //<return the default setting
	bool InstallFromFile(); //<Load list from file
	bool StoreToFile();	//<Store the list to file
	RSTDTYPE GetCurRstd();//get the current setting
	void DeleteCurRstd();   //delete the current setting
	void AddRstd(RSTDTYPE rt); //add the rt to the list
	void SelectRstd();	     //select another RstdSetting
	void ModifyDefaultRst(RSTDTYPE rt);
private:
	CArray<RSTDTYPE,RSTDTYPE> m_list;
	int  CurIndex;		//< the current selection's position
	CStdioFile csf;
};
#endif	
	







