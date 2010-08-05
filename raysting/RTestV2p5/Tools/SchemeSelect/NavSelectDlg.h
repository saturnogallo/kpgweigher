// NavSelectDlg.h : header file
//

#if !defined(AFX_NAVSELECTDLG_H__9562A9EF_F62D_4FC9_BFB8_C1155CDD3568__INCLUDED_)
#define AFX_NAVSELECTDLG_H__9562A9EF_F62D_4FC9_BFB8_C1155CDD3568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNavSelectDlg dialog

class CNavSelectDlg : public CDialog
{
// Construction
public:
	CNavSelectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNavSelectDlg)
	enum { IDD = IDD_NAVSELECT_DIALOG };
	CListBox	m_NavList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNavSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SearchDir(LPSTR pszDir);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVSELECTDLG_H__9562A9EF_F62D_4FC9_BFB8_C1155CDD3568__INCLUDED_)
