#if !defined(AFX_MEMDIALOG_H__BE230146_3D17_4E96_B494_08F4D1C48A52__INCLUDED_)
#define AFX_MEMDIALOG_H__BE230146_3D17_4E96_B494_08F4D1C48A52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemDialog.h : header file
//
#include "Setting.h"
/////////////////////////////////////////////////////////////////////////////
// CMemDialog dialog
#define MEM_DEFINFO	SjGetAppDirectory()+_T("\\MemInfo.def")
class CMemDialog : public CDialog
{
// Construction
public:
	CMemDialog(CWnd* pParent = NULL);   // standard constructor
	CString GetMemList() {return m_items.GetTitleList('|');};
	double GetMemData(CString key){
		MEMITEM m = m_items.SelectParm(key);
		return wcstod(m.sValue,NULL);
	};
	void AddClipboard(CString value){		
		m_value=value;
		m_label.Empty();
		UpdateData(FALSE);
	}
// Dialog Data
	//{{AFX_DATA(CMemDialog)
	enum { IDD = IDD_MEMDIALOG };
	CListCtrl	m_memlist;
	CString	m_label;
	CString	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSetting<MEMITEM> m_items;
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

	// Generated message map functions
	//{{AFX_MSG(CMemDialog)
	afx_msg void OnMemadd();
	afx_msg void OnMemdel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMemCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMDIALOG_H__BE230146_3D17_4E96_B494_08F4D1C48A52__INCLUDED_)
