// TryData3Dlg.h : header file
//

#if !defined(AFX_TRYDATA3DLG_H__FEC4485B_9673_4A25_BAC6_7988424986C3__INCLUDED_)
#define AFX_TRYDATA3DLG_H__FEC4485B_9673_4A25_BAC6_7988424986C3__INCLUDED_

#include "..\\sjInclude\\HtmlCtrl\\Include\\HtmlCtrl.h"
#include "..\\sjInclude\\Report\\Include\\Replace.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_HTMLCTRL	WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CTryData3Dlg dialog
#include "HtmlHandler.h"
#include "Tester.h"
#include "WaitDlg.h"

class CTryData3Dlg : public CDialog
{
// Construction
public:
	CTryData3Dlg(CWnd* pParent = NULL);	// standard constructor
public:
	CDialog *splash;
// Dialog Data
	//{{AFX_DATA(CTryData3Dlg)
	enum { IDD = IDD_TRYDATA3_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTryData3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CTryData3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg	LRESULT OnHtmlCtrlNotify(WPARAM wParam,LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCancelMode();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//htm event handler
	CHtmlHandler m_Handler;
private:
	//
	CHtmlCtrl m_hc;

public:
	
	// //overwrite the onOK function
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRYDATA3DLG_H__FEC4485B_9673_4A25_BAC6_7988424986C3__INCLUDED_)
