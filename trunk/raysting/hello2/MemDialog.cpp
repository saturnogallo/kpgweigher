// MemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "hello2Dlg.h"
#include "MemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemDialog dialog

extern CString SjGetAppDirectory();

CMemDialog::CMemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemDialog)
	m_label = _T("");
	m_value = _T("");
	//}}AFX_DATA_INIT
}


void CMemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemDialog)
	DDX_Control(pDX, IDC_LIST1, m_memlist);
	DDX_Text(pDX, IDC_MEMLABEL, m_label);
	DDV_MaxChars(pDX, m_label, MAX_NAME_CHARS);
	DDX_Text(pDX, IDC_MEMVALUE, m_value);
	DDV_MaxChars(pDX, m_value, MAX_VALUE_CHARS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemDialog, CDialog)
	//{{AFX_MSG_MAP(CMemDialog)
	ON_BN_CLICKED(IDC_MEMADD, OnMemadd)
	ON_BN_CLICKED(IDC_MEMDEL, OnMemdel)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickMemCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemDialog message handlers

void CMemDialog::OnMemadd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_label.IsEmpty())
		return;
	MEMITEM m = m_items.SelectParm(m_label);
	swprintf(m.sTitle,_T("%s"),m_label);
	swprintf(m.sValue,_T("%s"),m_value);
	m_items.SaveParm(m);
	m_items.StoreToFile(MEM_DEFINFO);
	CString temp = m_label;
	m_label.Empty();
	OnMemdel();
	m_label = temp;
}

void CMemDialog::OnMemdel() 
{
	// TODO: Add your control notification handler code here
	if(!m_label.IsEmpty()){
		m_items.SelectParm(m_label);
		m_items.DeleteCurParm();
		m_items.StoreToFile(MEM_DEFINFO);
	}
	CString namelist = m_items.GetTitleList('|');	
	m_memlist.DeleteAllItems();
	if(!namelist.IsEmpty()){							
		int id = 0;
		while(namelist.Find('|',0) >= 0){
			CString item = namelist.Left(namelist.Find('|',0));
			MEMITEM cfg = m_items.SelectParm(item);
			namelist.Delete(0,item.GetLength() + 1);
			cfg.sTitle[MAX_NAME_CHARS-1] = '\0';
			cfg.sValue[MAX_VALUE_CHARS-1] = '\0';
			item.Format(_T("%s(%s)"),cfg.sTitle,cfg.sValue);
			m_memlist.InsertItem(id++,item);
		}
		UpdateData(FALSE);
	}	

}

BOOL CMemDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_items.InstallFromFile(MEM_DEFINFO);
	m_label.Empty();
	OnMemdel();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMemDialog::OnClickMemCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int id = m_memlist.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if(id >= 0){
		CString text = m_memlist.GetItemText(id,0);
		m_label = text.Left(text.Find('(',0));
		text.Delete(0,m_label.GetLength() + 1);
		m_value = text.Left(text.Find(')',0));
	}
	UpdateData(FALSE);
	*pResult = 0;
}

