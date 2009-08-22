#if !defined(AFX_SYSTEMDIALOG_H__EA4DB017_0BC8_4286_8A19_22CE28B6B900__INCLUDED_)
#define AFX_SYSTEMDIALOG_H__EA4DB017_0BC8_4286_8A19_22CE28B6B900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemDialog dialog
#include "Setting.h"
#include "TesterProgram.h"
#define SYS_DEFINFO	SjGetAppDirectory()+_T("\\SystemInfo.def")

#define DEFAULT_RES _T("内标准设置")

class CSystemDialog : public CDialog
{
// Construction
public:
	CSystemDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemDialog)
	enum { IDD = IDD_SYSTEM2_CFG };
	double	m_100ohm;
	double	m_10kohm;
	double	m_10ohm;
	double	m_1kohm;
	double	m_1ohm;
	CString	m_name;
	double	m_alpha1ohm;
	double	m_alpha10ohm;
	double	m_alpha100ohm;
	double	m_alpha10kohm;
	double	m_alpha1kohm;
	double	m_beta1ohm;
	double	m_beta10ohm;
	double	m_beta100ohm;
	double	m_beta10kohm;
	double	m_beta1kohm;
	float	m_tempnow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSetting<SYSTEMCFG> m_setting;
	SYSTEMCFG	m_cfg;
	void SettingToCFG(){
		m_cfg.d_1ohm = m_1ohm;
		m_cfg.d_10ohm = m_10ohm;
		m_cfg.d_100ohm = m_100ohm;
		m_cfg.d_1kohm = m_1kohm;
		m_cfg.d_10kohm = m_10kohm;
		m_cfg.d_alpha10 = m_alpha10ohm;
		m_cfg.d_alpha100 = m_alpha100ohm;
		m_cfg.d_alpha1k = m_alpha1kohm;
		m_cfg.d_alpha10k = m_alpha10kohm;
		m_cfg.d_beta1 = m_beta1ohm;
		m_cfg.d_beta10 = m_beta10ohm;
		m_cfg.d_beta100 = m_beta100ohm;
		m_cfg.d_beta1k = m_beta1kohm;
		m_cfg.d_beta10k = m_beta10kohm;
		m_cfg.d_tempnow = m_tempnow;
		swprintf(m_cfg.sTitle,_T("%s"),m_name);
  };
	void CFGToSetting(){
		m_1ohm = m_cfg.d_1ohm;
		m_10ohm = m_cfg.d_10ohm;
		m_100ohm = m_cfg.d_100ohm;
		m_1kohm = m_cfg.d_1kohm;
		m_10kohm = m_cfg.d_10kohm;
		m_alpha1ohm = m_cfg.d_alpha1;
		m_alpha10ohm = m_cfg.d_alpha10;
		m_alpha100ohm = m_cfg.d_alpha100;
		m_alpha1kohm = m_cfg.d_alpha1k;
		m_alpha10kohm = m_cfg.d_alpha10k;
		m_beta1ohm = m_cfg.d_beta1;
		m_beta10ohm = m_cfg.d_beta10;
		m_beta100ohm = m_cfg.d_beta100;
		m_beta1kohm = m_cfg.d_beta1k;
		m_beta10kohm = m_cfg.d_beta10k;
		m_tempnow = m_cfg.d_tempnow;
		m_name.Format(_T("%s"),m_cfg.sTitle);
	};
	CTesterProgram m_prg;
	void DoSysCalib(CString func,UINT bt);
	void UpdateCalibButton();
	// Generated message map functions
	//{{AFX_MSG(CSystemDialog)
	afx_msg void OnAddScheme();
	afx_msg void OnModify();
	afx_msg void OnSelchangeSysList();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSYSCALIB1ohm();
	afx_msg void OnSYSCALIB10ohm();
	afx_msg void OnSYSCALIB100ohm();
	afx_msg void OnSYSCALIB1k();
	afx_msg void OnSYSCALIB10kohm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMDIALOG_H__EA4DB017_0BC8_4286_8A19_22CE28B6B900__INCLUDED_)
