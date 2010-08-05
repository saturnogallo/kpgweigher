#ifndef _TRYDATA3_DEFINE_H_
#define _TRYDATA3_DEFINE_H_

/*	Parameter Setting */
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
/*	Resister Standard Setting */
typedef struct _RSTDTYPE
{
	CString	sTitle;
	BOOL	bUse[9];
	double	dAlpha[9];
	double	dBeta[9];
	double	dValue[9];
	int		iScaledR0;
}RSTDTYPE;


/*	Test Configuration (Dynamic) Setting */
struct TESTERCONF
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
};

struct RTESTGROUP
{
	CString sRegSel;
	CString sRegLst;
	CString sRegUrl;
	
	CString sConfSel;
	CString sConfLst;
	

	CString sTestUrl;
	CString sTestLst;
	
	CString sJhSel1;
	CString sJhLst1;
	CString sJhSel2;
	CString sJhLst2;
	CString sJhUrl;
	
	CString sDataSel;
	CString sDataLst;
	CString sDataUrl;
	
	CString sReportSel;
	CString sReportLst;
	CString sReportUrl;
};
#endif