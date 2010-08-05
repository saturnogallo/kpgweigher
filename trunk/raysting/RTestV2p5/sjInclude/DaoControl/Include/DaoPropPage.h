#if !defined(daoprpag_h)      
#define daoprpag_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDaoPropPage

class CDaoPropSheet;
class DaoControl;

class CDaoPropPage : public CPropertyPage {
	CDaoPropSheet* padre;
protected:
	bool onsetactive;
public:
	CDaoPropPage(UINT resId);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoPropPage)
	public:
	virtual BOOL OnApply();
    virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
    virtual BOOL OnWizardFinish();    
	virtual void OnOK();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	bool GetModif() const;
	void SetModify(int Id,bool mod = true)
      	{SendDlgItemMessage(Id,EM_SETMODIFY,mod);}
	bool GetAlta() const;
	//{{AFX_MSG(CDaoPropPage)
	virtual void OnModificar();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX) = 0;    // DDX/DDV support
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
