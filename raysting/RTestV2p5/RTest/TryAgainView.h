// TryAgainView.h : interface of the CTryAgainView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "webbrowser2.h"
//}}AFX_INCLUDES

#if !defined(AFX_TRYAGAINVIEW_H__66DF4782_8349_4091_A800_C734672879B9__INCLUDED_)
#define AFX_TRYAGAINVIEW_H__66DF4782_8349_4091_A800_C734672879B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TryAgainDoc.h"
#include "RstdSetting.h"
#include "ParamSetting.h"
#include "SplashDlg.h"
#include "..\\CommonH&Libs\\Switch.h"
#include "..\\CommonH&Libs\\Navmeter.h"
#include "DataReport.h"
#ifndef  NOPRINTER
	#include "WHPrinter.h"
#endif

#define BEIDLE   0
#define BETEST	 1
#define BEERRO   2
#define BEFINI   3
#define BEPAUSE  4
#define BEOVER   5

class CTryAgainView : public CFormView
{
protected: // create from serialization only
	CTryAgainView();
	DECLARE_DYNCREATE(CTryAgainView)
public:
	//{{AFX_DATA(CTryAgainView)
	enum { IDD = IDD_TRYAGAIN_FORM };
	CTabCtrl	m_Tab;
	CProgressCtrl	m_Progress;
	CComboBox	m_Scheme;
	CComboBox	m_iRange;
	CComboBox	m_iLowest;
	BOOL	m_bUse1;
	BOOL	m_bUse2;
	BOOL	m_bUse3;
	BOOL	m_bUse4;
	BOOL	m_bUse5;
	BOOL	m_bUse6;
	BOOL	m_bUse7;
	double	m_dValue1;
	double	m_dValue2;
	double	m_dValue3;
	double	m_dValue4;
	double	m_dValue5;
	double	m_dValue6;
	double	m_dValue7;
	double	m_dAlpha1;
	double	m_dAlpha2;
	double	m_dAlpha3;
	double	m_dAlpha4;
	double	m_dAlpha5;
	double	m_dAlpha6;
	double	m_dAlpha7;
	double	m_dBeta1;
	double	m_dBeta7;
	double	m_dBeta2;
	double	m_dBeta3;
	double	m_dBeta6;
	double	m_dBeta5;
	double	m_dBeta4;
	BOOL	m_bSingle;
	double	m_dTemper;
	int		m_iTimes;
	CMSFlexGrid	m_Grid;
	CWebBrowser2	m_Web;
	CString	m_sRunOn;
	CString	m_sDisplay;
	double	m_iDelay;
	//}}AFX_DATA

// Attributes
public:
	CTryAgainDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTryAgainView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTryAgainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTryAgainView)
	afx_msg void OnRstAdd();
	afx_msg void OnRstDelete();
	afx_msg void OnRstSelect();
	afx_msg void OnParmAdd();
	afx_msg void OnParmDelete();
	afx_msg void OnParmSelect();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBeginTest();
	afx_msg void OnPause();
	afx_msg void OnRetestQuest();
	afx_msg void OnSelChangeMsflexgrid1();
	afx_msg void OnExitprogram();
	afx_msg void OnUpdateExitprogram(CCmdUI* pCmdUI);
	afx_msg void OnQuitProgram();
	afx_msg void OnViewReport();
	afx_msg void OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReplace();
	afx_msg void OnSaveReport();
	afx_msg void OnDrawData();
	afx_msg void OnOutofmemoryProgress1(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateParamSetting(PARAMTYPE pt);//load ParamSetting to display
	PARAMTYPE RetrieveParmSetting(); //get ParamSetting from input
	RSTDTYPE RetrieveRstSetting();   //get standard resistant setting from input
	void UpdateRstSetting(RSTDTYPE m_rs);//load standard resistant to display
private:
	double ConvertDataExp(double input);
	bool FunEnter(CString &cmdLine,bool bUse);
	bool ProgramAnalyse(CString &cmdLine,bool bUse=true);
	bool CheckDriveID();
	CString DoublePrecision(double value,int prec);
	double GetAverage(double *data,int count);
	void DoEvents();
	void OnEndTest(UINT flag);    //Handle the end of the test
	void RstdSettingCombination();//combine the standard resistant of inner and outer ones
	bool HandleFlexGridData(CString &line);//Handle FlexGridData Manuplate
	void IntallScheme();//Load TestScheme 
	bool ReplaceReportDefine(CString sType);//Replace data in type.htm
	bool InternalRoutines(char type);  //Rountines internal
	bool HandleCommand(char type,CString Parm); //deliver the command
	bool HandleProgram(CString line); //analyse the condition
//	bool GenerateReport(CString sType); //no use now
	void OverLoadHandle();	//handle overload case
	void CommandFinish();	//
	int CommandBegin(CString sType);
	bool CommandStep();
	bool RuleCheck(CString sType); //rules check of setting

	//for function analyse use
	CStringArray m_fun;	
	int 		 m_iStackDepth;
	int			 m_iFunpos[11];
	CRstdSetting m_rset;	//management of RstSetting
	CParamSetting m_pset;	//management of ParamSetting
	PARAMTYPE m_ps;	
	RSTDTYPE  m_rs;
	CSwitch   m_Switch;
	CNavmeter m_Navmeter;
	CSplashDlg *splash;
	CStdioFile	csf;
#ifndef  NOPRINTER
	CWHPrinter m_WHPrinter;
#endif
	int m_RangeV;		//<define the max size of Grid
	int m_iGridLine;	//<the current lines of Grid
	int m_TotalLines;   //<the total command lines to be handled
	int	m_iCmdPos;		//<the current command progress 
	int m_iRetestID;	//<the ValueID index for retest
	int	m_iBase;		//<the Base variable for caculate use
	bool	bOpen;      //indicate the use of csf
	int m_RetestType;   //test type for retest use
	int m_iScaledR0;    //indicate the scale level of RO resistant
	CDataReport m_Data;
	UINT m_State;		//indicate the current statuse
	bool bReport;
	long m_WebWidth;
	int	 m_CheckCount;	//count for overload check
	int  m_iReplace;    //the retest id of the item for replace in Grid
	int  m_iDelayKT;	//the delay variable for NKTT &PKTT
	int  m_iDelayKI;	//the delay variable for KI change
};

#ifndef _DEBUG  // debug version in TryAgainView.cpp
inline CTryAgainDoc* CTryAgainView::GetDocument()
   { return (CTryAgainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRYAGAINVIEW_H__66DF4782_8349_4091_A800_C734672879B9__INCLUDED_)
