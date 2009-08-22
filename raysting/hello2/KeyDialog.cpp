// KeyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "KeyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyDialog dialog


CKeyDialog::CKeyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyDialog::IDD, pParent),target_wnd(NULL)
{
	//{{AFX_DATA_INIT(CKeyDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyDialog, CDialog)
	//{{AFX_MSG_MAP(CKeyDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_KEYTAB, OnSelchangeKeytab)
	ON_BN_CLICKED(IDC_BTNUM0, OnBtnum0)
	ON_BN_CLICKED(IDC_BUTTON7, OnBtnum1)
	ON_BN_CLICKED(IDC_BUTTON8, OnBtnum2)
	ON_BN_CLICKED(IDC_BUTTON9, OnBtnum3)
	ON_BN_CLICKED(IDC_BUTTON10, OnBtnum4)
	ON_BN_CLICKED(IDC_BUTTON11, OnBtnum5)
	ON_BN_CLICKED(IDC_BUTTON12, OnBtnum6)
	ON_BN_CLICKED(IDC_BUTTON13, OnBtnum7)
	ON_BN_CLICKED(IDC_BUTTON14, OnBtnum8)
	ON_BN_CLICKED(IDC_BUTTON16, OnBtnumsub)
	ON_BN_CLICKED(IDC_BUTTON17, OnBtnumpt)
	ON_BN_CLICKED(IDC_BUTTON15, OnBtnum9)
	ON_BN_CLICKED(IDC_BUTTON18, OnBtnumdel)
	ON_BN_CLICKED(IDC_BUTTON2, OnToggleCharKbd)
	ON_BN_CLICKED(IDC_BUTTON19, OnBtnumbk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyDialog message handlers
const CString keylist("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
void CKeyDialog::OnSelchangeKeytab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTabCtrl *ctab = (CTabCtrl*)GetDlgItem(IDC_KEYTAB);
	int id;
	
	id = ctab->GetCurSel();
	if(id == 1)
	{
		ctab->SetCurSel(0);
		ctab->ShowWindow(SW_HIDE);
		ShowNumKbd(SW_SHOWNORMAL);
		*pResult = 0; 
		if(target_wnd)
			target_wnd->BringWindowToTop();
		return;
	}
	switch(id){
	case 0: return;
	case 2: id = VK_DELETE;break;
	case 3: id = VK_BACK;break;
	case 4: id = VK_LEFT;break;
	case 5: id = VK_RIGHT;break;
	case 6: id = VK_ADD;break;
	case 7: id = VK_SUBTRACT;break;
	case 8: id = VK_MULTIPLY;break;
	case 9: id = VK_DIVIDE;break;
	case 10: id = VK_DECIMAL;break;
	case 11: id = VK_SPACE;break;
	default:
		id = keylist[id-12];
	}
	//TODO: send the key value;	
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event((unsigned char)id,0,0,0);
	ctab->SetCurSel(0);
	UpdateData(FALSE);
	*pResult = 0;
}

BOOL CKeyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTabCtrl *ctab = (CTabCtrl*)GetDlgItem(IDC_KEYTAB);
	int id = 0;
	ctab->SetMinTabWidth(1);
	ctab->InsertItem(id++,_T(""));
	ctab->InsertItem(id++,_T("Êý×Ö¼üÅÌ"));
	ctab->InsertItem(id++,_T("Delete"));
	ctab->InsertItem(id++,_T("Backspace"));
	ctab->InsertItem(id++,_T("<-"));
	ctab->InsertItem(id++,_T("->"));
	ctab->InsertItem(id++,_T("+"));
	ctab->InsertItem(id++,_T("-"));
	ctab->InsertItem(id++,_T("*"));
	ctab->InsertItem(id++,_T("/"));
	ctab->InsertItem(id++,_T("."));
	ctab->InsertItem(id++,_T(" "));
	for(int a=0;a < keylist.GetLength();a++){
		ctab->InsertItem(id++,keylist.Mid(a,1));
	}
	// TODO: Add extra initialization here
	//switch to Number board on startup
	ctab->SetCurSel(0);
	ctab->ShowWindow(SW_HIDE);
	ShowNumKbd(SW_SHOWNORMAL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyDialog::OnBtnum0() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('0',0,0,0);
}

void CKeyDialog::OnBtnum1() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('1',0,0,0);
	
}

void CKeyDialog::OnBtnum2() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('2',0,0,0);
	
}

void CKeyDialog::OnBtnum3() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('3',0,0,0);
	
}

void CKeyDialog::OnBtnum4() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('4',0,0,0);
	
}

void CKeyDialog::OnBtnum5() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('5',0,0,0);
	
}

void CKeyDialog::OnBtnum6() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('6',0,0,0);
	
}

void CKeyDialog::OnBtnum7() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('7',0,0,0);
	
}

void CKeyDialog::OnBtnum8() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('8',0,0,0);
	
}

void CKeyDialog::OnBtnum9() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event('9',0,0,0);
	
}

void CKeyDialog::OnBtnumsub() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event(VK_SUBTRACT,0,0,0);
	
}

void CKeyDialog::OnBtnumpt() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event(VK_DECIMAL,0,0,0);
	
}

void CKeyDialog::OnBtnumdel() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event(VK_DELETE,0,0,0);	
}
void CKeyDialog::ShowNumKbd(UINT style)
{
	UINT bts[] = {IDC_BUTTON7,IDC_BUTTON8,IDC_BUTTON9,IDC_BUTTON10,IDC_BUTTON11,IDC_BUTTON12,\
				IDC_BUTTON13,IDC_BUTTON14,IDC_BUTTON15,IDC_BUTTON16,IDC_BUTTON17,IDC_BUTTON18,\
				IDC_BUTTON19,IDC_BUTTON2,IDC_BTNUM0};
	for(int i = 0;i<sizeof(bts)/sizeof(UINT);i++)
	{
		GetDlgItem(bts[i])->ShowWindow(style);		
	}

}
void CKeyDialog::OnToggleCharKbd() 
{
	// TODO: Add your control notification handler code here
	CTabCtrl *ctab = (CTabCtrl*)GetDlgItem(IDC_KEYTAB);	
	ctab->ShowWindow(SW_SHOWNORMAL);
	ShowNumKbd(SW_HIDE);
	if(target_wnd)
		target_wnd->BringWindowToTop();
}

void CKeyDialog::OnBtnumbk() 
{
	// TODO: Add your control notification handler code here
	if(target_wnd)
		target_wnd->BringWindowToTop();
	keybd_event(VK_BACK,0,0,0);	
}
