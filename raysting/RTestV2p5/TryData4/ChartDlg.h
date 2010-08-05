#pragma once
#include "afxwin.h"
#include "rtestchartctrl1.h"


// CChartDlg dialog

class CChartDlg : public CDialog
{

public:
	CChartDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChartDlg();
	void AddData(CString newValue);
	void DeleteAll();
	bool bQuit;

	
// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CChartDlg)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedOk();

	void UpdateChart();
	CListBox m_Datasel;
	double m_dBase;
	double m_dVary;
	CRtestchartctrl1 m_Chart;
	
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedAutoupdate();
};
