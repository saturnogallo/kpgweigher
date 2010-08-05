// ResiDefaultSetDlg.h : header file
//

#if !defined(AFX_RESIDEFAULTSETDLG_H__322BEB7B_F33F_415A_9C8F_12DF1875DDCD__INCLUDED_)
#define AFX_RESIDEFAULTSETDLG_H__322BEB7B_F33F_415A_9C8F_12DF1875DDCD__INCLUDED_
#include "RstdSetting.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CResiDefaultSetDlg dialog

class CResiDefaultSetDlg : public CDialog
{
// Construction
public:
	void UpdateList(CString namelist);
	CResiDefaultSetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CResiDefaultSetDlg)
	enum { IDD = IDD_RESIDEFAULTSET_DIALOG };
	CListBox	m_Name;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResiDefaultSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CResiDefaultSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetdefault();
	afx_msg void OnDeleteCur();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRstdSetting m_rst;
	RSTDTYPE m_rs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIDEFAULTSETDLG_H__322BEB7B_F33F_415A_9C8F_12DF1875DDCD__INCLUDED_)
