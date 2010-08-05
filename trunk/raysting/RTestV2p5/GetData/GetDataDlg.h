// GetDataDlg.h : header file
//

#if !defined(AFX_GETDATADLG_H__B3AB9645_829A_4F44_B1B6_F4F6C4D319F8__INCLUDED_)
#define AFX_GETDATADLG_H__B3AB9645_829A_4F44_B1B6_F4F6C4D319F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGetDataDlg dialog

class CGetDataDlg : public CDialog
{
// Construction
public:
	CString m_sDataPath;
	CGetDataDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGetDataDlg)
	enum { IDD = IDD_GETDATA_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGetDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetAllData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETDATADLG_H__B3AB9645_829A_4F44_B1B6_F4F6C4D319F8__INCLUDED_)
