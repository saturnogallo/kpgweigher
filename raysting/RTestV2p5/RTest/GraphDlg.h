#if !defined(AFX_GRAPHDLG_H__12C0FBE1_3434_11D7_BAF9_8A528E6C2C46__INCLUDED_)
#define AFX_GRAPHDLG_H__12C0FBE1_3434_11D7_BAF9_8A528E6C2C46__INCLUDED_
#include "Graph.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg dialog

class CGraphDlg : public CDialog
{
// Construction
public:
	CGraphDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGraphDlg)
	enum { IDD = IDD_GRAPHDLG };
	double	m_dbase;
	double	m_dAverage;
	//}}AFX_DATA
	
	double data[100];
	int validcount;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGraphDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double DataAverage(double input[],int length);
	double DataExp(double input);
	CGraph *testGraph;
	void OnDrawGraph();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDLG_H__12C0FBE1_3434_11D7_BAF9_8A528E6C2C46__INCLUDED_)
