// ModifySerialDlg.h : 头文件
//

#pragma once


// CModifySerialDlg 对话框
class CModifySerialDlg : public CDialog
{
// 构造
public:
	CModifySerialDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MODIFYSERIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	CString m_strKey;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
