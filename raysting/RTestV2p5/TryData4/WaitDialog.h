#pragma once


// CWaitDialog 对话框

class CWaitDialog : public CDialog
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnCancel();

	void ShowOkButton(bool show)
	{
		if(show)
			GetDlgItem(IDOK)->ShowWindow(SW_SHOWNORMAL);
		else
			GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	}
	void SetWaitingText(CString text)
	{
		m_sWaiting = text;
	//	if(this->IsWindowVisible()){
			UpdateData(FALSE);
	//		this->Invalidate();
	//	}
	}
	bool bQuit;
private:

		// //waiting message
	CString m_sWaiting;

};
