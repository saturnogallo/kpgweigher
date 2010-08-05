#include "stdafx.h"
#include "TesterData.h"
#include "TryDataDefine.h"
#include "math.h"

extern TESTERCONF g_TesterConf;
CTesterData g_Data;

CTesterData::CTesterData()
{
	drd.rbValue = false;
	drd.rbResult = false;
	drd.rValue = -1000;
	drd.rResult = -1000;
	drd.rPos = -1;
	rset.RemoveAll();
	rset.Add(drd);
}

RESDATA  CTesterData::GetRecord(UINT nID)
{
	AddDefaultRecord(nID);
	return rset.GetAt(nID);
}

bool  CTesterData::SetRecord(UINT nID,RESDATA result)
{
	bool bok;
	bok = true;
	AddDefaultRecord(nID);		
	rset.SetAt(nID,result);
	return bok;
}

void CTesterData::AddDefaultRecord(int FinalPos)
{
	int dsize;
	dsize = rset.GetSize()-1;

	if(FinalPos > dsize)
	{
		while(dsize++ < FinalPos)
		{
			rset.Add(drd);
		}
	}
}

void CTesterData::OperateValue(char vl, char vr, char op, int posl, int posr,char babs,int poss)
{

	double dl,dr;
	int dsize;
	RESDATA rd;
	if(vl != 'C')		AddDefaultRecord(posl);
	if(vr != 'C')		AddDefaultRecord(posr);
	RSTDTYPE rs = g_TesterConf.m_Rs;
	PARAMTYPE ps = g_TesterConf.m_Ps;
	if(vl == 'V')
	{
		rd=rset.GetAt(posl);
		dl=rd.rValue;
	}
	if(vl == 'Z')
	{
		rd=rset.GetAt(posl);
		dl=rd.rResult;
	}
	if(vl == 'R')
	{
		dl = rs.dValue[posl];//some equation should be added here;
		dl = dl*(1+(1e-6)*rs.dAlpha[posl]*(ps.dTemper-20)+(1e-6)*rs.dBeta[posl]*(ps.dTemper-20)*(ps.dTemper-20));
	}
	if(vl == 'C')
	{
		dl = posl;
	}
	if(vr == 'V')
	{
		rd=rset.GetAt(posr);
		dr=rd.rValue;
	}
	if(vr == 'Z')
	{
		rd=rset.GetAt(posr);
		dr=rd.rResult;
	}
	if(vr == 'R')
	{
		dr = rs.dValue[posr];
		dr = dr*(1+(1e-6)*rs.dAlpha[posr]*(ps.dTemper-20)+(1e-6)*rs.dBeta[posr]*(ps.dTemper-20)*(ps.dTemper-20));
	}
	if(vr == 'C')
	{
		dr = posr;
	}
	if(babs == '1')
	{
		if(dl <0) dl = -dl;
		if(dr <0) dr = -dr;
	}
	dsize = rset.GetSize()-1;

	if(poss > dsize)
	{
		AddDefaultRecord(poss);
	}
	rd = rset.GetAt(poss);
	switch(op)
	{
	case '+':
		rd.rResult = dl+dr;break;
	case '-':
		rd.rResult = dl-dr;break;
	case '*':
		rd.rResult = dl*dr;break;
	case '/':
		if(dr == 0) dr = 1;
		rd.rResult = dl/dr;break;
	case '>':
		if(dl > dr) 
			rd.rResult = dl;
		else
			rd.rResult = dr;
		break;
	case '<':
		if(dl < dr) 
			rd.rResult = dl;
		else
			rd.rResult = dr;
		break;
	default:
		break;
	}
	rd.rbResult = true;
	rset.SetAt(poss,rd);
}

void CTesterData::ResetRecord(UINT ID)
{
	int dsize;
	
	if(ID == 0)
	{
		drd.rbValue = false;
		drd.rbResult = false;
		drd.rValue = -1000;
		drd.rResult = -1000;
		drd.rPos = -1;
		rset.RemoveAll();
		rset.Add(drd);
	}
	else
	{
		dsize = rset.GetSize();
		while(dsize > int(ID))
		{
			rset.RemoveAt(dsize-1);
			dsize--;
		}
	}
}




















