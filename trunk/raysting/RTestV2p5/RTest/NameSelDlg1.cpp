// NameSelDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "TryAgain.h"
#include "NameSelDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNameSelDlg dialog


CNameSelDlg::CNameSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNameSelDlg::IDD, pParent)
{
	nametype = 0;
	//{{AFX_DATA_INIT(CNameSelDlg)
	m_sInput = _T("");
	//}}AFX_DATA_INIT
}


void CNameSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNameSelDlg)
	DDX_Control(pDX, IDC_NAMESEL, m_NameSel);
	DDX_Text(pDX, IDC_NAMEINPUT, m_sInput);
	DDV_MaxChars(pDX, m_sInput, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNameSelDlg, CDialog)
	//{{AFX_MSG_MAP(CNameSelDlg)
	ON_LBN_SELCHANGE(IDC_NAMESEL, OnSelchangeNamesel)
	ON_LBN_DBLCLK(IDC_NAMESEL, OnDblclkNamesel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNameSelDlg message handlers

void CNameSelDlg::OnSelchangeNamesel() 
{
	// TODO: Add your control notification handler code here
	m_NameSel.GetText(m_NameSel.GetCurSel(),m_sInput);
	UpdateData(FALSE);
}

void CNameSelDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(nametype == DT_NAMEDLG)
	{
		if(m_sInput.GetLength() <= 0)
		{
			::AfxMessageBox("名字不能为空");
			return;
		}
		if(m_NameSel.FindString(0,(LPCSTR)m_sInput) >= 0)
		{
			::AfxMessageBox("请不要选择重复名字");
			return;
		}
	}
	if(nametype == DT_SELDLG)
	{
		if(m_NameSel.GetCurSel() >= 0)
		{
			m_NameSel.GetText(m_NameSel.GetCurSel(),m_sInput);
			nameid = m_NameSel.GetItemData(m_NameSel.GetCurSel());
		}
		else
		{
			CDialog::OnCancel();
			return;
		}
	}
	CDialog::OnOK();	
	CDialog::OnOK();
}

void CNameSelDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CNameSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int pos,opos,count,spos;
	count = 0;
	pos = 0;
	opos = 0;
	while(pos != -1)
	{
		pos = namelist.Find(_T("\n"),opos);
		if(pos == -1)
			break;
		spos = m_NameSel.AddString(namelist.Mid(opos,pos-opos));
		m_NameSel.SetItemData(spos,count++);
		opos = pos+1;
	}
	if(m_NameSel.GetCount() > 0)
	{
		m_NameSel.SetCurSel(0);
		m_NameSel.GetText(0,m_sInput);
	}
	if(nameid == DT_SELDLG)
		SetWindowText("请选择一个名字");
	if(nameid == DT_NAMEDLG)
		SetWindowText("请输入一个名字");	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNameSelDlg::OnDblclkNamesel() 
{
	// TODO: Add your control notification handler code here
	if(nametype == DT_SELDLG)
		OnOK();
}
