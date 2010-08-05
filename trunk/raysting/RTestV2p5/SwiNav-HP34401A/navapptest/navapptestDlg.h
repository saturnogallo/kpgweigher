// navapptestDlg.h : header file
//

#if !defined(AFX_NAVAPPTESTDLG_H__B9A7DEA7_4069_45B8_8BD9_8F6500F25D75__INCLUDED_)
#define AFX_NAVAPPTESTDLG_H__B9A7DEA7_4069_45B8_8BD9_8F6500F25D75__INCLUDED_

#include "Navmeter.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNavapptestDlg dialog

class CNavapptestDlg : public CDialog
{
// Construction
public:
	CNavapptestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNavapptestDlg)
	enum { IDD = IDD_NAVAPPTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavapptestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNavapptestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CNavmeter m_nav;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVAPPTESTDLG_H__B9A7DEA7_4069_45B8_8BD9_8F6500F25D75__INCLUDED_)
