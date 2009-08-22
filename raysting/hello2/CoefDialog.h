#if !defined(AFX_COEFDIALOG_H__D0864CAB_22BC_4A37_9F60_14A00520899F__INCLUDED_)
#define AFX_COEFDIALOG_H__D0864CAB_22BC_4A37_9F60_14A00520899F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoefDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoefDialog dialog

class CCoefDialog : public CDialog
{
// Construction
public:
	void LoadABCCoef(double val[]);
	CCoefDialog(CWnd* pParent = NULL);   // standard constructor
	double abc_coef[16];
// Dialog Data
	//{{AFX_DATA(CCoefDialog)
	enum { IDD = IDD_COEF_DIALOG };
	double	m_tp;
	double	m_zn;
	double	m_al;
	double	m_ar;
	double	m_ga;
	double	m_hg;
	double	m_in;
	double	m_sn;
	double	m_acoef;
	double	m_bcoef;
	double	m_ccoef;
	CString	m_atitle;
	CString	m_btitle;
	CString	m_ctitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoefDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCoefDialog)
	afx_msg void OnWtocoef();
	afx_msg void OnCoefmodify();
	afx_msg void OnCoefquit();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateControlStatus();
	int m_trange;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COEFDIALOG_H__D0864CAB_22BC_4A37_9F60_14A00520899F__INCLUDED_)
