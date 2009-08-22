#if !defined(AFX_RUNCFGDIALOG_H__613DFDAB_58DD_4332_BA51_BBE322BA0A61__INCLUDED_)
#define AFX_RUNCFGDIALOG_H__613DFDAB_58DD_4332_BA51_BBE322BA0A61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunCfgDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunCfgDialog dialog

class CRunCfgDialog : public CDialog
{
// Construction
public:
	CRunCfgDialog(CWnd* pParent = NULL);   // standard constructor
	CString GetCFGSetting();
// Dialog Data
	//{{AFX_DATA(CRunCfgDialog)
	enum { IDD = IDD_RUN_CFG };
	CTabCtrl	m_timestab;
	int		m_times;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunCfgDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunCfgDialog)
	afx_msg void OnSelchangeTimeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNCFGDIALOG_H__613DFDAB_58DD_4332_BA51_BBE322BA0A61__INCLUDED_)
