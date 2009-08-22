#pragma once


// CThermoDlg 对话框

class CThermoDlg : public CDialog
{
	DECLARE_DYNAMIC(CThermoDlg)

public:
	CThermoDlg(CWnd* pParent = NULL);   // 标准构造函数
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
	virtual ~CThermoDlg();
	CString GetCFGSetting();
// 对话框数据
	//{{AFX_DATA(CThermoDlg)
	enum { IDD = IDD_RITHMO_CFG };
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// Generated message map functions
	//{{AFX_MSG(CThermoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnToNextCfg();
	afx_msg void OnExit();
};
