#if !defined(AFX_RUNITEMDIALOG_H__AC4FC7C1_B6F7_4CC5_AC87_0C17A2C627BD__INCLUDED_)
#define AFX_RUNITEMDIALOG_H__AC4FC7C1_B6F7_4CC5_AC87_0C17A2C627BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunitemDialog.h : header file
#include "RITempDialog.h"
#include "RIResiDialog.h"
#include "RICacuDialog.h"
#include "ThermoDlg.h"
#include "RunCfgDialog.h"
#include "TesterProgram.h"
#include "IconButton.h"

#include "Graph.h"

#define PROFILE_NAME	SjGetAppDirectory()+_T("\\Profile.txt")
extern CString GetProfileString(const CString strSection, const CString strEntry, const CString strDefault);
extern BOOL  MyWriteProfileString(const CString strSection, const CString strEntry, const CString strValue);
#define SHOW_MAINDATA	0
#define SHOW_ORGDATA	1
/////////////////////////////////////////////////////////////////////////////
// CRunitemDialog dialog
class CRunitemDialog : public CDialog
{
// Construction
public:
	CRunitemDialog(CWnd* pParent = NULL);   // standard constructor
	int cfg_now; //cfg_type that currently used.

	void PrepareRun(int times);
	void AbortRun();
	void DoCacu();
	bool DoRun();
	bool PostRun();
	void Reset(int times);
	bool IsValid();
	bool IsInOneRun(bool &bOneDone);
	static CString GetSingleSetting(CString file,CString id);
	bool IsCacu(); 
	CString GetCFGSetting(){
		CString ret;
		ret.Format(_T("chid=%i|"),m_ch);
		return ret;
	};
	void LoadLastConfig(){
		CString ch;
		CString lastcfg;
		ch.Format(_T("ch%i"),m_ch);
		
		lastcfg = GetProfileString(ch,_T("cfg_now"),_T(""));
		swscanf(lastcfg,_T("%i"),&cfg_now);

		m_prbid = GetProfileString(ch,_T("prbid"),_T(""));

		lastcfg = GetProfileString(ch,_T("sidech"),_T(""));
		swscanf(lastcfg,_T("%i"),&side_ch);

		lastcfg = GetProfileString(ch,_T("shdata"),_T(""));
		swscanf(lastcfg,_T("%i"),&show_type);
		
	};
	void SaveLastConfig(){
		CString ch,lastcfg;
		ch.Format(_T("ch%i"),m_ch);
		lastcfg.Format(_T("%i"),cfg_now);
		MyWriteProfileString(ch,_T("cfg_now"),lastcfg);

		MyWriteProfileString(ch,_T("prbid"),m_prbid);

		lastcfg.Format(_T("%i"),side_ch);
		MyWriteProfileString(ch,_T("sidech"),lastcfg);

		lastcfg.Format(_T("%i"),show_type);
		MyWriteProfileString(ch,_T("shdata"),lastcfg);
		
	};
	CString m_prbid;
	CString status;
	CString m_bigtxt;
	CString m_smalltxt;
	bool IsAllDone();
	CList<double,double> m_data;		//converted data result array
	CList<double,double> m_output;		//original data array
	CList<double,double> *sidedata;		//side data pointer for display usage
	int side_ch;							//side data channel
	int show_type;							//display main data or converted data
	CTesterProgram m_prg;
// Dialog Data
	//{{AFX_DATA(CRunitemDialog)
	enum { IDD = IDD_RUNITEM };
	CStatic m_led;
	int		m_ch;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunitemDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int runcnt;
	char status_now;
	int _times;

	
	// Generated message map functions
	//{{AFX_MSG(CRunitemDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNITEMDIALOG_H__AC4FC7C1_B6F7_4CC5_AC87_0C17A2C627BD__INCLUDED_)
