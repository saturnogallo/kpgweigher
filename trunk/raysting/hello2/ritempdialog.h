#if !defined(AFX_RITEMP_H__17FB47C6_F5F2_4693_912E_0DDFD3A213A7__INCLUDED_)
#define AFX_RITEMP_H__17FB47C6_F5F2_4693_912E_0DDFD3A213A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RITemp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRITempDialog dialog

class CRITempDialog : public CDialog
{
// Construction
public:
	CRITempDialog(CWnd* pParent = NULL);   // standard constructor
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
	//{{AFX_DATA(CRITempDialog)
	enum { IDD = IDD_RITEMP_CFG };
	CString	m_filterid;
	CString	m_probeid;
	CString	m_systemid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRITempDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRITempDialog)
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

#endif // !defined(AFX_RITEMP_H__17FB47C6_F5F2_4693_912E_0DDFD3A213A7__INCLUDED_)
