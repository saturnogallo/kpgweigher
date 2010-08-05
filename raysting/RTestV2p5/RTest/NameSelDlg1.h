#if !defined(AFX_NAMESELDLG1_H__819EF0BB_C3F1_4698_9701_0D7624BEB18D__INCLUDED_)
#define AFX_NAMESELDLG1_H__819EF0BB_C3F1_4698_9701_0D7624BEB18D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NameSelDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNameSelDlg dialog
#include "Resource.h"
#define DT_NAMEDLG  1
#define DT_SELDLG   2

class CNameSelDlg : public CDialog
{
// Construction
public:
	CNameSelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNameSelDlg)
	enum { IDD = IDD_NAMESELDLG };
	CListBox	m_NameSel;
	CString	m_sInput;
	//}}AFX_DATA
public:
	CString  namelist;	//the name list will be displayed in list box (ended with"\n")
	int		 nameid;	//the nameid user selected at last
	int		 nametype;	//the type the dlg will appear on :DT_NAMEDLG ,DT_SELDLG,

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNameSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNameSelDlg)
	afx_msg void OnSelchangeNamesel();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkNamesel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMESELDLG1_H__819EF0BB_C3F1_4698_9701_0D7624BEB18D__INCLUDED_)
