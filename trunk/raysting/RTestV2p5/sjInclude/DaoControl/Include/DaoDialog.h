#if !defined(__daodialog_h)
#define __daodialog_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DaoWindow.h"

// DaoDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDaoDialog


class CDaoDialog : public CDialog, public DaoWindow {
	CString caption;
protected:
	bool cont;
		//Attribute for controlling survival of the object after an
		//OnOK event. Useful for continued data entry.
	bool child;
		//This condition is true if the object was created with the child
		//creation constructor.
public:
	CDaoDialog(UINT i, CWnd* p = 0, LPCSTR s = 0, bool a = false);
		//Normal constructor
	CDaoDialog(UINT i, Recordset& r, CWnd* p = 0, bool a = false);
		//Child creation constructor
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoDialog)
	public:
	virtual void OnOK();
	//}}AFX_VIRTUAL

	void SetCaption(LPCTSTR str);
	bool Execute();

	// Generated message map functions
protected:
	void SetModify(int Id, bool mod = true)
     	{SendDlgItemMessage(Id, EM_SETMODIFY, mod);}
		//Helper function for edit controls
	void SetDlgItemText(int nID, LPCTSTR lpszString);
		//Override of the CWnd function. It sets the modify flag in edit controls.
	//{{AFX_MSG(CDaoDialog)
	virtual void OnModificar();		//Manages enabling of controls
	virtual void OnEnter();			//Moves focus among controls
	virtual BOOL OnInitDialog();	
	virtual void DoDataExchange(CDataExchange* pDX) = 0;    // DDX/DDV support
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
