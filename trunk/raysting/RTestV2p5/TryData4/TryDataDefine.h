#ifndef _TRYDATA3_DEFINE_H_
#define _TRYDATA3_DEFINE_H_

/*	Parameter Setting */
typedef struct _PARAMTYPE
{
	CString sLowest;
	int		iLowest;
	CString sRange;
	int		iRange;
	double	iDelay;
	double	dTemper;
	int		iTimes;
	int		bSingle;
	CString sTitle;
	CString	sOther;
}PARAMTYPE;
/*	Resister Standard Setting */
typedef struct _RSTDTYPE
{
	CString	sTitle;
	int	bUse[9];
	double	dAlpha[9];
	double	dBeta[9];
	double	dValue[9];
	int		iScaledR0;
}RSTDTYPE;


/*	Test Configuration (Dynamic) Setting */
typedef struct _TESTERCONF
{
	RSTDTYPE m_RsDefault;
	RSTDTYPE m_Rs;
	PARAMTYPE m_Ps;
	
	/* Retest setting */
	CString m_RetestType;  
	CString m_Xhgg;		    //model id (ex, zx54)
	int		m_iRetestID;
	
	int m_iBase;			//current base of record
	int m_RangeV;
	int m_iDelayKT;
	int m_iDelayKI;
}TESTERCONF;

#endif