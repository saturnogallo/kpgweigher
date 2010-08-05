#ifndef _AFX_DATAREPORT_H_021106
#define _AFX_DATAREPORT_H_021106

#include "stdafx.h"
#include "afxtempl.h"

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
class CTesterData
{
public:
	void ResetRecord(UINT ID=0);
	CTesterData();
	void OperateValue(char vl,char vr,char op,int posl,int posr,char babs,int poss);
	RESDATA	GetRecord(UINT nID);
	bool	SetRecord(UINT nID,RESDATA result);
	//bool    AddRecord(UINT nID,double result);
	int m_iBase;			//current base of record
private:	
	void AddDefaultRecord(int FinalPos);
	CArray<RESDATA,RESDATA> rset;
	RESDATA	drd;   //defaulted resdata type
};
#endif 




















