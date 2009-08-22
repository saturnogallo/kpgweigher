#if !defined(AFX_RIRESIDIALOG_H__80F4B469_C416_4455_9FB2_2FD757A490BA__INCLUDED_)
#define AFX_RIRESIDIALOG_H__80F4B469_C416_4455_9FB2_2FD757A490BA__INCLUDED_
#include "stdafx.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RIResiDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRIResiDialog dialog

class CRIResiDialog : public CDialog
{
// Construction
public:
	CRIResiDialog(CWnd* pParent = NULL);   // standard constructor
	void AddCBItem(int id, CString namelist)
	{
		CComboBox* cb = ((CComboBox*)GetDlgItem(id));
		while(namelist.Find('|',0) >= 0){
			CString item = namelist.Left(namelist.Find('|',0));
			cb->AddString(item);
			namelist.Delete(0,item.GetLength() + 1);
		}
		if(cb->GetCount() > 0){
			cb->SetCurSel(0);
		}
	}
	CString GetCFGSetting();
// Dialog Data
	//{{AFX_DATA(CRIResiDialog)
	enum { IDD = IDD_RIRESI_CFG };
	CString	m_filterid;
	CString	m_probeid;
	CString	m_systemid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRIResiDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRIResiDialog)
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnToNextCfg();
	afx_msg void OnEditchangeProbeId();
	afx_msg void OnEditchangeFilterId();
	afx_msg void OnEditchangeSystemId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIRESIDIALOG_H__80F4B469_C416_4455_9FB2_2FD757A490BA__INCLUDED_)
