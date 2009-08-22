#if !defined(AFX_RICACUDIALOG_H__3B4F2C0F_1114_4FC2_AB2E_4B281902E498__INCLUDED_)
#define AFX_RICACUDIALOG_H__3B4F2C0F_1114_4FC2_AB2E_4B281902E498__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RICacuDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRICacuDialog dialog

class CRICacuDialog : public CDialog
{
// Construction
public:
	CRICacuDialog(CWnd* pParent = NULL);   // standard constructor
	void FillOperator(CString memlist);
	void AddCBItem(int id, CString namelist)
	{
		CComboBox* cb = ((CComboBox*)GetDlgItem(id));

		while(cb->GetCount() > 0)
			cb->DeleteString(0);
		while(namelist.Find('|',0) >= 0){
			CString item = namelist.Left(namelist.Find('|',0));
			cb->AddString(item);
			namelist.Delete(0,item.GetLength() + 1);
		}
		if(cb->GetCount() > 0){
			cb->SetCurSel(0);
		}		
	}
// Dialog Data
	//{{AFX_DATA(CRICacuDialog)
	enum { IDD = IDD_RICACU_CFG };
	CString	m_op;
	CString	m_op1;
	CString	m_op2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRICacuDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRICacuDialog)
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnToNextCfg();
	afx_msg void OnSelchangeOp1();
	afx_msg void OnEditchangeOp();
	afx_msg void OnSelchangeOp2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICACUDIALOG_H__3B4F2C0F_1114_4FC2_AB2E_4B281902E498__INCLUDED_)
