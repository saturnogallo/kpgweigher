#if !defined(AFX_KEYDIALOG_H__1D574A11_7CBD_401B_9429_F063E243B792__INCLUDED_)
#define AFX_KEYDIALOG_H__1D574A11_7CBD_401B_9429_F063E243B792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyDialog dialog

class CKeyDialog : public CDialog
{
// Construction
public:
	CKeyDialog(CWnd* pParent = NULL);   // standard constructor
	CWnd *target_wnd;
// Dialog Data
	//{{AFX_DATA(CKeyDialog)
	enum { IDD = IDD_KEYDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowNumKbd(UINT style);
	// Generated message map functions
	//{{AFX_MSG(CKeyDialog)
	afx_msg void OnSelchangeKeytab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnum0();
	afx_msg void OnBtnum1();
	afx_msg void OnBtnum2();
	afx_msg void OnBtnum3();
	afx_msg void OnBtnum4();
	afx_msg void OnBtnum5();
	afx_msg void OnBtnum6();
	afx_msg void OnBtnum7();
	afx_msg void OnBtnum8();
	afx_msg void OnBtnumsub();
	afx_msg void OnBtnumpt();
	afx_msg void OnBtnum9();
	afx_msg void OnBtnumdel();
	afx_msg void OnToggleCharKbd();
	afx_msg void OnBtnumbk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYDIALOG_H__1D574A11_7CBD_401B_9429_F063E243B792__INCLUDED_)
