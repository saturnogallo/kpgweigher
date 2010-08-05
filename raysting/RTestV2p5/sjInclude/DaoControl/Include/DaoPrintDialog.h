#if !defined(AFX_DAOPRINTDIALOG_H__D182B117_78BF_11D2_917E_66801C000000__INCLUDED_)
#define AFX_DAOPRINTDIALOG_H__D182B117_78BF_11D2_917E_66801C000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoPrintDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDaoPrintDialog dialog

class CDaoPrintDialog : public CPrintDialog
{
	DECLARE_DYNAMIC(CDaoPrintDialog)

public:
	CDaoPrintDialog(int l, BOOL bPrintSetupOnly,
		// TRUE for Print Setup, FALSE for Print Dialog
		DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES 
			| PD_HIDEPRINTTOFILE | PD_NOSELECTION,
		CWnd* pParentWnd = NULL);
protected:
	//{{AFX_MSG(CDaoPrintDialog)
	afx_msg void OnPrintSetup();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int lines;
	void SetNumPages();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAOPRINTDIALOG_H__D182B117_78BF_11D2_917E_66801C000000__INCLUDED_)
