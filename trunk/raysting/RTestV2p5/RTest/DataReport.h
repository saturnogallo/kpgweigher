#ifndef _AFX_DATAREPORT_H_021106
#define _AFX_DATAREPORT_H_021106

#include "stdafx.h"
#include "afxtempl.h"
#include "RstdSetting.h"
#include "ParamSetting.h"

typedef struct _RESDATA
{
	bool rbValue;
	bool rbResult;
	double rValue;
	double rResult;
	int		rPos;
}RESDATA;



/**
 *	the CDataReport is used to manage the data we measured;
 *	for store and delete and modify ,and return for some 
 *	controls to display ,and generate the Report
 */
class CDataReport 
{
public:
	void ResetRecord(UINT ID=0);
	CDataReport();
	void OperateValue(char vl,char vr,char op,int posl,int posr,char babs,int poss,RSTDTYPE rs,PARAMTYPE ps);
	RESDATA	GetRecord(UINT nID);
	bool	SetRecord(UINT nID,RESDATA result);
	//bool    AddRecord(UINT nID,double result);
private:	
	void AddDefaultRecord(int FinalPos);
	CArray<RESDATA,RESDATA> rset;
	RESDATA	drd;   //defaulted resdata type
};
#endif 