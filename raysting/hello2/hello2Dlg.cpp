// hello2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxwin.h"
#include "hello2.h"
#include "hello2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CMD_PAUSE	'p'
#define CMD_ABORT	'b'
#define CMD_GOON	'g'
#define CMD_QUIT	'q'
#define CMD_IDLE	'i'


// CHello2Dlg 对话框
DWORD dwThreadId;
HANDLE hThread;

DWORD  WINAPI  OneRunThread(LPVOID  lparam){
	CHello2Dlg *mach = (CHello2Dlg*)lparam;
	static can_run = false;
	static wait_cnt = 0;
	while(1)
	{
		if(mach->thread_cmd != CMD_IDLE)
		{
			if(mach->thread_cmd == CMD_PAUSE)
				can_run = false;
			if(mach->thread_cmd == CMD_GOON)
				can_run = true;
			if(mach->thread_cmd == CMD_QUIT)
				break;
			if(mach->thread_cmd == CMD_ABORT)
			{
				if(mach->cur_prg)
					mach->cur_prg->Abort();
				can_run = false;
			}
			mach->thread_cmd = CMD_IDLE;
		}
		if(can_run && mach->cur_prg){
			if(wait_cnt > 0)
			{
				wait_cnt--;
				Sleep(10);
				continue;
			}
			if(mach->cur_prg->DuringPause())
			{
				wait_cnt = 100;
				continue;
			}

			mach->cur_prg->StepProgram();
			
						continue;
		}
		Sleep(1);
	}
	mach->thread_cmd = CMD_IDLE;
	return 0;
}

CHello2Dlg::CHello2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHello2Dlg::IDD, pParent), curview(0),m_pause(false)
	, m_runcount(1)
	, m_cfgtype(0)
	, lastid(0)
{
	//{{AFX_DATA_INIT(CHello2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
		m_ridlgs[i] = NULL;
	}
	m_sysdlg = NULL;
	m_prbdlg = NULL;
	m_fltdlg = NULL;
	m_keydlg = NULL;
	m_ccfg = NULL;
	m_running = false;
	
	m_cfgtype = CFGTYPE_BORE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHello2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_cfgtab);
	DDX_Control(pDX, IDC_LIST3, m_chlist);
	DDX_Control(pDX, IDC_RUN_GRAPH, m_graph);
	DDX_Control(pDX, IDC_LIST4, m_history);
	DDX_Text(pDX, IDC_RUNCOUNT, m_runcount);
}


BEGIN_MESSAGE_MAP(CHello2Dlg, CDialog)
	//{{AFX_MSG_MAP(CHello2Dlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, OnExit)
	ON_BN_CLICKED(IDC_BTN_RUN, OnBtnRun)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_LBN_SELCHANGE(IDC_LIST3, OnLbnSelchangeList3)
	ON_LBN_DBLCLK(IDC_LIST3, OnLbnDblclkList3)
	ON_STN_CLICKED(IDC_RUN_GRAPH, OnStnDblclickRunGraph)
	ON_EN_SETFOCUS(IDC_RUNCOUNT, OnEnSetfocusRuncount)
	ON_EN_KILLFOCUS(IDC_RUNCOUNT, OnEnKillfocusRuncount)
	ON_LBN_DBLCLK(IDC_LIST4, OnLbnDblclkList4)
	ON_COMMAND(ID_32771, OnProcessSel1)
	ON_COMMAND(ID_32772, OnProcessSel2)
	ON_COMMAND(ID_32773, OnProcessSel3)
	ON_COMMAND(ID_32774, OnProcessSel4)
	ON_COMMAND(ID_32775, OnProcessSel5)
	ON_COMMAND(ID_32776, OnProcessSel6)
	ON_COMMAND(ID_32777, OnProcessSel7)
	ON_COMMAND(ID_32778, OnProcessSel8)
	ON_COMMAND(ID_32779, OnProcessSel9)
	ON_COMMAND(ID_32780, OnProcessSel10)
	ON_COMMAND(ID_32781, OnProcessSel11)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHello2Dlg message handlers
#define SET_RUN_TEXT 	((CWnd*)GetDlgItem(IDC_BTN_RUN))->SetWindowText(m_running ? _T("停止") :_T("运行"))
#define MOVETOTOP		28
#define MOVETOBOTTOM	1
void CHello2Dlg::MoveWindowTo(CWnd* wnd, int xoffset, int yoffset, UINT style)
{
	CRect mrect,rect;
	
	GetWindowRect(&mrect);
	wnd->GetWindowRect(&rect);
	if(yoffset == MOVETOBOTTOM)
	{
		yoffset = mrect.Height()-rect.Height()+13;
	}
//rect.OffsetRect(mrect.TopLeft().x + xoffset, mrect.TopLeft().y + yoffset);
	wnd->MoveWindow(xoffset+1,yoffset,mrect.Width(),rect.Height());
	wnd->ShowWindow(style);
}
BOOL CHello2Dlg::OnInitDialog()
{

	SHELLEXECUTEINFO   execInf;  
	ZeroMemory   (&execInf,   sizeof   (execInf));    
	execInf.cbSize   =   sizeof   (SHELLEXECUTEINFO);    
	execInf.fMask   =   NULL;
	execInf.nShow = SW_SHOWNORMAL;
	execInf.lpFile   =   _T("\\windows\\explorer.exe");    
	execInf.lpVerb   =   NULL;  
	execInf.lpParameters = _T("\\windows");
	execInf.hInstApp = NULL;
	execInf.hwnd = NULL;
	ShellExecuteEx (&execInf);

	CDialog::OnInitDialog();
	CTabCtrl *ct = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	ct->SetMinTabWidth(50);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CenterWindow(GetDesktopWindow());	// center to the hpc screen
	CRect rect;
	GetWindowRect(&rect);
	this->MoveWindow(0,0,rect.Width(),rect.Height());
	m_prbdlg = new CProbeDialog();
	m_prbdlg->Create(IDD_PROBE2_CFG,this);
	MoveWindowTo(m_prbdlg,0,MOVETOTOP,SW_HIDE);

	m_sysdlg = new CSystemDialog();
	m_sysdlg->Create(IDD_SYSTEM2_CFG,this);
	MoveWindowTo(m_sysdlg,0,MOVETOTOP,SW_HIDE);

	m_fltdlg = new CFilterDialog();
	m_fltdlg->Create(IDD_FILTER2_CFG,this);
	MoveWindowTo(m_fltdlg,0,MOVETOTOP,SW_HIDE);

	m_ccfg = new CCoefDialog();
	m_ccfg->Create(IDD_COEF_DIALOG,this);
	MoveWindowTo(m_ccfg,0,MOVETOTOP,SW_HIDE);

	m_keydlg = new CKeyDialog();
	m_keydlg->Create(IDD_KEYDIALOG,this);
	MoveWindowTo(m_keydlg,1,MOVETOBOTTOM,SW_HIDE);

	for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
		m_ridlgs[i] = new CRunitemDialog();
		m_ridlgs[i]->m_ch = i+1;
		m_ridlgs[i]->cfg_now = CFGTYPE_NONE;
		m_ridlgs[i]->Create(IDD_RUNITEM,this);
		m_ridlgs[i]->LoadLastConfig();
		/*
		m_runtab.InsertItem(i,name);
		m_ridlgs[i]->DisplayBar(true);
		MoveWindowTo(m_ridlgs[i],0,MOVETOTOP,SW_HIDE);
		*/
	}
	CSpinButtonCtrl *spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	spin->SetBuddy(GetDlgItem(IDC_RUNCOUNT));
	spin->SetRange(-1,10000);


	m_cfgtab.InsertItem(0,_T("   铂电阻测温   "));
	m_cfgtab.InsertItem(1,_T("   热电偶测温   "));
	m_cfgtab.InsertItem(2,_T("    探头设置    "));
	m_cfgtab.InsertItem(3,_T("    标准设置    "));
	m_cfgtab.InsertItem(4,_T("    滤波设置    "));
	m_cfgtab.SetCurSel(0);
	curview = 0;

	
	
	LoadCurrentView();
	m_chlist.SetCurSel(curview);
	thread_cmd = CMD_ABORT;
	cur_prg = NULL;
	hThread = CreateThread(NULL,NULL,OneRunThread,this ,0,&dwThreadId);
	// TODO: Add extra initialization here
	SetTimer(1,TIMER_LEN,NULL);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}


void CHello2Dlg::OnDestroy() 
{
	if(m_prbdlg)	{m_prbdlg->DestroyWindow(); delete m_prbdlg;}
	if(m_sysdlg)	{m_sysdlg->DestroyWindow(); delete m_sysdlg;}
	if(m_fltdlg)	{m_fltdlg->DestroyWindow(); delete m_fltdlg;}
	if(m_keydlg)	{m_keydlg->DestroyWindow(); delete m_keydlg;}
	if(m_ccfg)		{m_ccfg->DestroyWindow();	delete m_ccfg;}
	for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
		if(m_ridlgs[i]){
			m_ridlgs[i]->DestroyWindow();
			delete m_ridlgs[i];
		}
	}
	// TODO: Add your message handler code here
	CDialog::OnDestroy();	
}

void CHello2Dlg::OnTimer(UINT nIDEvent) 
{
	static bool beidle=true; //avoid reentry the function


	if(!beidle)
		return;
	beidle = false;

	if(!m_running || !cur_prg) {//waiting for the answer of the whether to continue question, or not running
		beidle = true;
		return;
	}
	
	SendCmd(CMD_PAUSE);
	if(!cur_prg->IsDone())
	{
		((CWnd*)GetDlgItem(IDC_MAIN_STATUS))->SetWindowText(cur_prg->mystate);
		SendCmd(CMD_GOON);
		beidle = true;
		return;
	}
	m_ridlgs[lastid]->PostRun(); //store the result of current run
	//one run is done
	//check all done
	bool alldone = true;
	for(int n = 0; n < sizeof(m_ridlgs)/sizeof(CRunitemDialog*) ;n++){//update the cacu result after a round a test
		if((m_ridlgs[n]->cfg_now != m_cfgtype))
			continue;
		if(!m_ridlgs[n]->IsAllDone()){ //finish all the runs required
			alldone = false;
		}
	}
		
	if(alldone){
		SendCmd(CMD_ABORT);
		m_running = false;
		Refresh();
		SET_RUN_TEXT;		
		GetDlgItem(IDC_MAIN_STATUS)->SetWindowText(_T("测试已完成"));
	}else{
		//current frame is done and move to next valid frame
		lastid = (lastid+1) % (sizeof(m_ridlgs)/sizeof(CRunitemDialog*));
		
		while((m_ridlgs[lastid]->cfg_now != m_cfgtype))
			lastid = (lastid+1) % (sizeof(m_ridlgs)/sizeof(CRunitemDialog*));
					

		cur_prg = &(m_ridlgs[lastid]->m_prg);
		m_ridlgs[lastid]->PrepareRun(m_runcount);
		if(m_graph.ch != curview)
		{
			m_graph.ch = curview;

		}
		m_graph.Invalidate();				
		Refresh();
		SendCmd(CMD_GOON);
	}
	beidle = true;
}

void CHello2Dlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	SendCmd(CMD_QUIT);
	OnCancel();
}

void CHello2Dlg::SendCmd(char cmd)
{
	thread_cmd = cmd;
	while(thread_cmd == cmd)
	{
		Sleep(10);
	}
}
void CHello2Dlg::OnBtnDisplay() 
{
	OnStnDblclickRunGraph();
}
void CHello2Dlg::OnBtnRun() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_running){ //stop the running program
		m_running = false;
		SendCmd(CMD_PAUSE);
		if(IDYES == ::AfxMessageBox(_T("是否停止测量?"),MB_YESNO)){
				SendCmd(CMD_ABORT);
				for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
					m_ridlgs[i]->m_prg.Abort();
				}
				CTesterProgram::m_swinav.reset_swi();
				m_running = false;
		}else{
			SendCmd(CMD_GOON);
			m_running = true;
		}
	}else{	//start the running program
		
		SendCmd(CMD_ABORT);
	SendCmd(CMD_PAUSE);
		
	m_graph.iBufMax = m_runcount;
		if(m_graph.iBufMax > BUF_MAX_DEFAULT)
			m_graph.iBufMax = BUF_MAX_DEFAULT;
	if(m_runcount <= 0)
		m_graph.iBufMax = BUF_MAX_DEFAULT;
		
		LoadCurrentView();

		CString def = CRunitemDialog::GetSingleSetting(SYS_DEFINFO,DEFAULT_RES);
		def.Replace(_T("u_"),_T("d_"));
		CTesterProgram::SetGlobalEnv(def);
		int start = -1;
		for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
			m_ridlgs[i]->Reset(m_runcount);
			if(m_ridlgs[i]->IsValid() && (m_ridlgs[i]->cfg_now == m_cfgtype)){
				if(start < 0)
					start = i;
				m_ridlgs[i]->PrepareRun(m_runcount);
				m_ridlgs[i]->m_prg.SetEnv(CRunitemDialog::GetSingleSetting(SYS_DEFINFO,m_sysdlg->m_name)+\
						CRunitemDialog::GetSingleSetting(FLT_DEFINFO,m_fltdlg->m_name)+\
						CRunitemDialog::GetSingleSetting(PRB_DEFINFO,m_ridlgs[i]->m_prbid)+\
						m_ridlgs[i]->GetCFGSetting());
			}
		}
		lastid = 0;
		cur_prg = &(m_ridlgs[start]->m_prg);

		m_running = true;
	}
	SET_RUN_TEXT;	
}

//show the coef dialog
void CHello2Dlg::SwitchToCcfg(double *cc)
{
	m_ccfg->ShowWindow(SW_SHOWNORMAL);
	m_ccfg->LoadABCCoef(cc);
	m_keydlg->target_wnd = m_ccfg;
	m_keydlg->ShowWindow(SW_SHOWNORMAL);
}
void CHello2Dlg::SwitchFromCcfg(double *abc_coef)
{
	for(int i=0;i<16;i++)
		m_prbdlg->m_cfg.sCoef[i] = abc_coef[i];

	m_prbdlg->m_coef.Format(_T("%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f"),
		abc_coef[0],abc_coef[1],abc_coef[2],abc_coef[3],abc_coef[4],\
		abc_coef[5],abc_coef[6],abc_coef[7],abc_coef[8],abc_coef[9],\
		abc_coef[10],abc_coef[11],abc_coef[12],abc_coef[13],abc_coef[14],\
		abc_coef[15]);
	m_prbdlg->CFGToSetting();

}
void CHello2Dlg::QuitCfgDialog()
{
	m_fltdlg->ShowWindow(SW_HIDE);
	m_sysdlg->ShowWindow(SW_HIDE);
	m_prbdlg->ShowWindow(SW_HIDE);
	m_keydlg->ShowWindow(SW_HIDE);
	m_ccfg->ShowWindow(SW_HIDE);
}

void CHello2Dlg::ShowCFG(CWnd* target)
{
	//hide all the temporary dialog		
	QuitCfgDialog();

	//show the desired dialog
	if(target)
	{
		target->ShowWindow(SW_SHOWNORMAL);
		m_keydlg->target_wnd = target;
		m_keydlg->ShowWindow(SW_SHOWNORMAL);
	}
}
void CHello2Dlg::LoadCurrentView()
{
	//display the current view base on the selection
	int curch = m_chlist.GetCurSel();
	
	int curcfg = m_cfgtype;

	//clear the channel list first
	while(m_chlist.GetCount() > 0)
		m_chlist.DeleteString(0);

	//fill in the channel list
	for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
		CString name;
		name.Format(_T("CH%i"),i+1);
		if (curcfg == CFGTYPE_BORE)
		{
			if(m_ridlgs[i]->IsValid() && m_ridlgs[i]->cfg_now == CFGTYPE_BORE)
			{
				name.Format(_T("CH%i:%s▼"),i+1,m_ridlgs[i]->m_prbid);
			}else{
				name.Format(_T("CH%i:关闭 ▼"),i+1);
			}
		}
		if (curcfg == CFGTYPE_THMO)
		{
			if(m_ridlgs[i]->IsValid() && m_ridlgs[i]->cfg_now == CFGTYPE_THMO)
			{
				name.Format(_T("CH%i:%s型▼"),i+1,m_ridlgs[i]->m_prbid);
			}else{
				name.Format(_T("CH%i:关闭▼"),i+1);
			}
		}
		m_chlist.AddString(name);
		m_chlist.SetCurSel(max(0,curch));
	}
	curview = max(0,curch);
	GetDlgItem(IDC_MAIN_STATUS)->SetWindowText(m_ridlgs[curview]->m_prg.mystate);
	Refresh();
}
void CHello2Dlg::Refresh()
{
	int curch = m_chlist.GetCurSel();
	if(curch < 0)
		return;
	int curcfg = m_cfgtype;

	//clear the history and graph
	while(m_history.GetCount() > 0)
		m_history.DeleteString(0);
	if(m_ridlgs[curch]->cfg_now != m_cfgtype)
	{
		m_graph.AssignMainData(NULL);
		m_graph.AssignSideData(NULL);
		m_graph.Invalidate();
		return;
	}

	//update data shown in graph, (data and text)
	m_graph.AssignMainData(&m_ridlgs[curch]->m_data);
	m_graph.ch = curch;
	m_graph.AssignSideData(m_ridlgs[curch]->sidedata);
	m_graph.side_ch = m_ridlgs[curch]->side_ch;
	CString dat;
	if(m_ridlgs[curch]->cfg_now == CFGTYPE_THMO)
	{
		m_graph.smalltext.Format(_T("%.4f mV"),CGraph::GetAverage(&m_ridlgs[curch]->m_output));
		m_graph.bigtext.Format(_T("%.4f ℃"),CGraph::GetAverage(&m_ridlgs[curch]->m_data));
	}else{
		if(m_ridlgs[curch]->cfg_now == CFGTYPE_RESI)
		{
			m_graph.smalltext.Format(_T("%.4f Ω"),CGraph::GetAverage(&m_ridlgs[curch]->m_output));
			m_graph.bigtext.Format(_T("%.4f ℃"),CGraph::GetAverage(&m_ridlgs[curch]->m_data));
		}else{
			m_graph.smalltext = m_ridlgs[curch]->m_smalltxt;
			m_graph.bigtext = m_ridlgs[curch]->m_bigtxt;
				
		}
	}
	m_graph.ch = curview;
	m_graph.Invalidate();

	//add data to history listbox
	CList<double,double> *data = &m_ridlgs[curch]->m_data;
	if(m_ridlgs[curch]->show_type == SHOW_ORGDATA)
		data = &m_ridlgs[curch]->m_output; 

	if(data && (data->GetCount() > 0))
	{
		POSITION pos = data->GetHeadPosition();
		do{
			double v =  data->GetAt(pos);
			CString r;
			r.Format(_T("%8f"),v);
			m_history.AddString(r);
			if(pos == data->GetTailPosition())
				break;
			data->GetNext(pos);
		}while(1);
		if(m_history.GetCount() > 0)
			m_history.SetCaretIndex(m_history.GetCount()-1,0);
	}
}
void CHello2Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_cfgtab.GetCurSel())
	{
	case 0:		//bo resi
		m_cfgtype = CFGTYPE_BORE;
		ShowCFG(NULL);
		LoadCurrentView();
		break;
	case 1:		//thermo 
		m_cfgtype = CFGTYPE_THMO;
		ShowCFG(NULL);
		LoadCurrentView();
		break;
	case 2:		//probe
		ShowCFG(m_prbdlg);
		break;
	case 3:		//sys
		ShowCFG(m_sysdlg);
		break;
	case 4:
		ShowCFG(m_fltdlg);
	default:
		break;
	}
	*pResult = 0;	
}

void CHello2Dlg::OnLbnSelchangeList3()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadCurrentView();
}

void CHello2Dlg::OnLbnDblclkList3()
{
	// TODO: 在此添加控件通知处理程序代码
  CMenu   mainmenu;    
  mainmenu.LoadMenu(IDR_SELPROBE);

  CMenu *pM = mainmenu.GetSubMenu(0);
  //remove the existing menu
  CString   str   ;  
  
  for(int   i=pM->GetMenuItemCount()-1;i>=0;i--)   //取得菜单的项数。  
  {    
          pM->GetMenuString(i,str,MF_BYPOSITION);    
          //将指定菜单项的标签拷贝到指定的缓冲区。MF_BYPOSITION的解释见上。    
		  pM->DeleteMenu(i,MF_BYPOSITION);   
  }
  
  CSetting<PROBECFG> prbset;
  prbset.InstallFromFile(PRB_DEFINFO);			
  CString namelist = prbset.GetTitleList('|');	

  int curch = m_chlist.GetCurSel();
  if(curch < 0)
	  return;

  if(m_cfgtype == CFGTYPE_BORE)
  {
	int j = 1;
	pM->AppendMenu(MF_STRING,ID_32771,_T("关闭"));

	while(namelist.Find('|',0) >= 0){
		CString item = namelist.Left(namelist.Find('|',0));
		pM->AppendMenu(MF_STRING,ID_32771+j,item);   //添加新的菜单项		
		namelist.Delete(0,item.GetLength() + 1);
		j = j+1;
	}
	m_cmdbase = ID_32771;
  }
  if(m_cfgtype == CFGTYPE_THMO)
  {
	  //TBEJKNRS
	  m_cmdbase = ID_32771;;
	  pM->AppendMenu(MF_STRING,ID_32771,_T("关闭"));
	  pM->AppendMenu(MF_STRING,ID_32772,_T("T型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32773,_T("B型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32774,_T("E型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32775,_T("J型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32776,_T("K型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32777,_T("N型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32778,_T("R型热电偶"));
	  pM->AppendMenu(MF_STRING,ID_32779,_T("S型热电偶"));
  }
  CRect rect;
  m_chlist.GetClientRect(&rect);
  
  CPoint pt;
  GetCursorPos(&pt);
  SetForegroundWindow();
  pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
  return;

}
#define DATAOUT_NAME	SjGetAppDirectory()+_T("\\data.txt")
void CHello2Dlg::OnProcessSel(int id)
{
	int curch = m_chlist.GetCurSel();

	if(m_cmdbase == ID_32773) //data operation
	{
		CList<double,double> *data = &m_ridlgs[curch]->m_data;
		if(m_ridlgs[curch]->show_type == SHOW_ORGDATA)
			data = &m_ridlgs[curch]->m_output;

		if(id == 1) //show converted result
		{
			m_ridlgs[curch]->show_type = SHOW_MAINDATA;
			
		}
		if(id == 2) //show raw result
		{
			m_ridlgs[curch]->show_type = SHOW_ORGDATA;
		}
		
		if(id == 3)	//delete the current record
		{
			int pos = m_history.GetCurSel();
			if((pos >= 0) && (data))
			{
				POSITION p = data->GetHeadPosition();
				while(pos-- > 0 )
					data->GetNext(p);
				data->RemoveAt(p);
			}
		}
		if(id == 4)	//delete all the record
		{
			data->RemoveAll();
		}
		if(id == 5)	//export all the record
		{
			CFile cf;
		
			if(!cf.Open(DATAOUT_NAME,CFile::modeCreate|CFile::modeWrite))
				return;

			//write to txt and open with notepad
			if(data && (data->GetCount() > 0))
			{
				POSITION pos = data->GetHeadPosition();
				char r[50];
				do{
					double v =  data->GetAt(pos);
					
					sprintf(r,"%8f\r\n",v);
					cf.Write(r,strlen(r));
					if(pos == data->GetTailPosition())
						break;
					data->GetNext(pos);
				}while(1);
			}
			cf.Close();
			SHELLEXECUTEINFO   execInf;  
			ZeroMemory   (&execInf,   sizeof   (execInf));    
			execInf.cbSize   =   sizeof   (SHELLEXECUTEINFO);    
			execInf.fMask   =   SEE_MASK_NOCLOSEPROCESS;
			execInf.nShow = SW_SHOWNORMAL;
			execInf.lpFile   =   _T("\\windows\\pword.exe");    
			execInf.lpVerb   =   _T("open");  
			execInf.lpParameters = DATAOUT_NAME;
			ShellExecuteEx (&execInf);   
		}

	}
	if(m_cmdbase == ID_32772)
	{
		if(id == 1)
		{
			m_ridlgs[curview]->sidedata = NULL;
		}else{
			m_ridlgs[curview]->sidedata = &m_ridlgs[id-2]->m_data;
			m_ridlgs[curview]->side_ch = id-2;
		}
		m_ridlgs[curview]->SaveLastConfig();
	}
	if(m_cmdbase == ID_32771) //channel config
	{
		if(m_cfgtype == CFGTYPE_BORE)
		{
			CSetting<PROBECFG> prbset;
			prbset.InstallFromFile(PRB_DEFINFO);			
		
			if(id == 1)//just close it
			{
				m_ridlgs[curch]->cfg_now = CFGTYPE_NONE;
				for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
					if((m_ridlgs[i]->cfg_now == CFGTYPE_BORE) && (i != curch) && (m_ridlgs[i]->side_ch == curch))
					{
						m_ridlgs[i]->sidedata = NULL;
					}
				}
			}else{
				m_ridlgs[curch]->m_prbid = prbset.GetNameAt(id-2);
				m_ridlgs[curch]->cfg_now = CFGTYPE_BORE;
			}
			m_ridlgs[curch]->SaveLastConfig();
		}
		if(m_cfgtype == CFGTYPE_THMO)
		{
			if(m_ridlgs[curch]->IsValid() && (id == 1)) //just close it
			{
				m_ridlgs[curch]->cfg_now = CFGTYPE_NONE;
				for(int i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
					if((m_ridlgs[i]->cfg_now == CFGTYPE_THMO) && (i != curch) && (m_ridlgs[i]->side_ch == curch))
					{
						m_ridlgs[i]->sidedata = NULL;
					}
				}
			}else{
				if(id != 1){
					m_ridlgs[curch]->cfg_now = CFGTYPE_THMO;
					CString types(_T("TBEJKNRS"));
					m_ridlgs[curch]->m_prbid = types.GetAt(id-2);
				}
			}
			m_ridlgs[curch]->SaveLastConfig();
		}
	}
	LoadCurrentView();
}
void CHello2Dlg::OnProcessSel1()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(1);
}
void CHello2Dlg::OnProcessSel2()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(2);
}
void CHello2Dlg::OnProcessSel3()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(3);
}
void CHello2Dlg::OnProcessSel4()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(4);
}
void CHello2Dlg::OnProcessSel5()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(5);
}
void CHello2Dlg::OnProcessSel6()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(6);
}
void CHello2Dlg::OnProcessSel7()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(7);
}
void CHello2Dlg::OnProcessSel8()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(8);
}
void CHello2Dlg::OnProcessSel9()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(9);
}
void CHello2Dlg::OnProcessSel10()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(10);
}
void CHello2Dlg::OnProcessSel11()
{
	// TODO: 在此添加命令处理程序代码
	OnProcessSel(11);
}

void CHello2Dlg::OnStnClickedRunGraph()
{
	
}

void CHello2Dlg::OnStnDblclickRunGraph()
{
	// TODO: 在此添加控件通知处理程序代码
  CMenu   mainmenu;    
  mainmenu.LoadMenu(IDR_SELPROBE);
  m_graph.Invalidate();
  CMenu *pM = mainmenu.GetSubMenu(0);
  //remove the existing menu
  CString   str ;  
  int i;
  for(i=pM->GetMenuItemCount()-1;i>=0;i--)   //取得菜单的项数。  
  {    
          pM->GetMenuString(i,str,MF_BYPOSITION);    
          //将指定菜单项的标签拷贝到指定的缓冲区。MF_BYPOSITION的解释见上。    
		  pM->DeleteMenu(i,MF_BYPOSITION);   
  }
  pM->AppendMenu(MF_STRING,ID_32771,_T("关闭辅助显示"));
  for(i=0;i<sizeof(m_ridlgs)/sizeof(CRunitemDialog*);i++){
		CString name;
		name.Format(_T("同时显示通道%i"),i+1);
  		pM->AppendMenu(MF_STRING,ID_32771+1+i,name);
  }
  m_cmdbase = ID_32772;

  
  CPoint pt;
  GetCursorPos(&pt);
	SetForegroundWindow();
    pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	return;
/*  CString tt;
  tt.Format(_T("clicked:%i,%i"),pt.x,pt.y);
  GetDlgItem(IDC_MAIN_STATUS)->SetWindowText(tt);
*/
}

void CHello2Dlg::OnEnSetfocusRuncount()
{
	// TODO: 在此添加控件通知处理程序代码
	m_keydlg->target_wnd = GetDlgItem(IDC_RUNCOUNT);
//	m_keydlg->ShowWindow(SW_SHOWNORMAL);
}

void CHello2Dlg::OnEnKillfocusRuncount()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_keydlg->ShowWindow(SW_HIDE);
}

void CHello2Dlg::OnLbnDblclkList4()
{
	// TODO: 在此添加控件通知处理程序代码
  CMenu   mainmenu;    
  mainmenu.LoadMenu(IDR_SELPROBE);

  CMenu *pM = mainmenu.GetSubMenu(0);
  //remove the existing menu
  CString   str ;  
  int i;
  for(i=pM->GetMenuItemCount()-1;i>=0;i--)   //取得菜单的项数。  
  {    
          pM->GetMenuString(i,str,MF_BYPOSITION);    
          //将指定菜单项的标签拷贝到指定的缓冲区。MF_BYPOSITION的解释见上。    
		  pM->DeleteMenu(i,MF_BYPOSITION);   
  }
  pM->AppendMenu(MF_STRING,ID_32771,_T("显示转换结果"));
  pM->AppendMenu(MF_STRING,ID_32771+1,_T("显示原始数据"));
  pM->AppendMenu(MF_STRING,ID_32771+2,_T("删除选中数据"));
  pM->AppendMenu(MF_STRING,ID_32771+3,_T("清空所有数据"));
  pM->AppendMenu(MF_STRING,ID_32771+4,_T("导出所有数据"));
  m_cmdbase = ID_32773;

  CRect rect;
  m_chlist.GetClientRect(&rect);
  
  CPoint pt;
  GetCursorPos(&pt);

    pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	return;

}
