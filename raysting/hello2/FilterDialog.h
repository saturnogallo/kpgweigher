#if !defined(AFX_FILTERDIALOG_H__F420219A_6C97_41EE_B374_A6A083A6B8EE__INCLUDED_)
#define AFX_FILTERDIALOG_H__F420219A_6C97_41EE_B374_A6A083A6B8EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDialog.h : header file
//
#include "Setting.h"
/////////////////////////////////////////////////////////////////////////////
// CFilterDialog dialog
#define FLT_DEFINFO	SjGetAppDirectory()+_T("\\FilterInfo.def")


class CFilterDialog : public CDialog
{
// Construction
public:
	CFilterDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterDialog)
	enum { IDD = IDD_FILTER2_CFG };
	int		m_integral;
	int		m_interval;
	CString	m_name;
	int		m_response;
	double	m_thresh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSetting<FILTERCFG> m_setting;
	FILTERCFG	m_cfg;
	void SettingToCFG(){
		m_cfg.iIntegral = m_integral;
		m_cfg.iInterval = m_interval;
		m_cfg.iResponse = m_response;
		m_cfg.iAverage = ((CComboBox*)GetDlgItem(IDC_FLT_CBAVG))->GetCurSel();
		m_cfg.iSpeed = ((CComboBox*)GetDlgItem(IDC_FLT_SPEED))->GetCurSel();;
		m_cfg.dThresh = m_thresh;
		swprintf(m_cfg.sTitle,_T("%s"),m_name);
  };
	void CFGToSetting(){
		m_integral =	m_cfg.iIntegral;
		m_interval =	m_cfg.iInterval;
		m_response =	m_cfg.iResponse;
		((CComboBox*)GetDlgItem(IDC_FLT_CBAVG))->SetCurSel(m_cfg.iAverage);
		((CComboBox*)GetDlgItem(IDC_FLT_SPEED))->SetCurSel(m_cfg.iSpeed);
		m_thresh = m_cfg.dThresh;
		m_name.Format(_T("%s"),m_cfg.sTitle);
	};
	// Generated message map functions
	//{{AFX_MSG(CFilterDialog)
	afx_msg void OnModify();
	afx_msg void OnAddScheme();
	afx_msg void OnSelchangeFltList();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDIALOG_H__F420219A_6C97_41EE_B374_A6A083A6B8EE__INCLUDED_)
