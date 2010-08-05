#ifndef _AFX_PARAMSETTING_H_021106
#define _AFX_PARAMSETTING_H_021106

/**
 *	PARAMTYPE is a collection of the 
 *	parameters used in test and stored in file;
 *
 */
#include "Resource.h"
#include "stdafx.h"
#include "afxtempl.h"

typedef struct _PARAMTYPE
{
	int	iLowest;
	int	iRange;
	double	iDelay;
	double	dTemper;
	int		iTimes;
	BOOL	bSingle;
	CString sTitle;
	CString	sOther;
}PARAMTYPE;

/**
 *	CParamSetting class is used to manage the PARAMTYPE list
 *	for example,store or install from to file,and delete,add 
 *	operations
 */
class CParamSetting
{
public:
	CParamSetting();
	bool InstallFromFile(); //<Load list from file
	bool StoreToFile();	//<Store the list to file
	PARAMTYPE GetCurParam();//<get the current setting
	void DeleteCurParm();   //<delete the current setting
	void AddParam(PARAMTYPE pt); //<add the pt to the list
	void SelectParam();	     //<select another ParamSetting
private:
	CArray<PARAMTYPE,PARAMTYPE> m_list; //<the setting list for store
	int  CurIndex;		 //<store the index of the current selection	
	CStdioFile csf;
};
#endif	
	







