#pragma once


// CInputDlg dialog

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// Message show in the box
	CString m_Message;
	// inputted value
	CString m_Value;
};
