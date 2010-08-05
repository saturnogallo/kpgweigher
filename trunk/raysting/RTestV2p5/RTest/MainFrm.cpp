// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TryAgain.h"
#include "TryAgainView.h"
#include "ReportView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_TOGGLELEFT, OnToggleReportView)
	ON_COMMAND(ID_TOGGLERIGHT, OnToggleTestView)
	ON_UPDATE_COMMAND_UI(ID_TOGGLELEFT, OnUpdateToggleleft)
	ON_UPDATE_COMMAND_UI(ID_TOGGLERIGHT, OnUpdateToggleright)
	ON_COMMAND(ID_EXITPROGRAM, OnExitProgram)
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_FULLSCREEN,OnFullScreen)
	ON_COMMAND(ID_TOGGLETEST, OnToggleTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	//bReport = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))//IDR_MAINFRAME
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	PostMessage(WM_FULLSCREEN,0,0);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.style = WS_VISIBLE|WS_DLGFRAME;
	cs.hMenu = NULL;

//	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE| WS_SYSMENU;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CFrameWnd::OnTimer(nIDEvent);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	BOOL bok;
	bok=m_Split.CreateStatic(this,1,2);//WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
	m_Split.CreateView(0,0,RUNTIME_CLASS(CReportView),CSize(0,0),pContext);
	m_Split.CreateView(0,1,RUNTIME_CLASS(CTryAgainView),CSize(0,0),pContext);
	return bok;
	*/
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnToggleReportView() 
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnToggleTestView() 
{
	// TODO: Add your command handler code here

}

void CMainFrame::OnUpdateToggleleft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(!bReport);
}

void CMainFrame::OnUpdateToggleright(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(bReport);	
}

void CMainFrame::OnExitProgram() 
{
	// TODO: Add your command handler code here
//	GetActiveView()->PostMessage(WM_COMMAND,ID_EXITPROGRAM,0);
	this->PostMessage(WM_CLOSE,0,0);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bFullScreen)
	{
		lpMMI->ptMaxSize.x=m_FullScreenRect.Width();
		lpMMI->ptMaxSize.y=m_FullScreenRect.Height();
		lpMMI->ptMaxPosition.x=m_FullScreenRect.Width();
		lpMMI->ptMaxPosition.y=m_FullScreenRect.Height();
		lpMMI->ptMaxTrackSize.x=m_FullScreenRect.Width();
		lpMMI->ptMaxTrackSize.y=m_FullScreenRect.Height();
	}
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnFullScreen()
{
	CRect WindowRect;
	CRect ClientRect;
	GetWindowPlacement(&m_OldWndPlacement);
	GetWindowRect(&WindowRect);
	RepositionBars(0,0xffff,AFX_IDW_PANE_FIRST,reposQuery,&ClientRect);
	ClientToScreen(&ClientRect);

	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	m_FullScreenRect.left=WindowRect.left-ClientRect.left;
	m_FullScreenRect.top=WindowRect.top-ClientRect.top;
	m_FullScreenRect.right=WindowRect.right-ClientRect.right+nFullWidth;
	m_FullScreenRect.bottom=WindowRect.bottom-ClientRect.bottom+nFullHeight;
	m_bFullScreen = TRUE;

	WINDOWPLACEMENT wndpl;
	wndpl.length=sizeof(WINDOWPLACEMENT);
	wndpl.flags=0;
	wndpl.showCmd=SW_SHOWNORMAL;
	wndpl.rcNormalPosition=m_FullScreenRect;
	SetWindowPlacement(&wndpl);
}

void CMainFrame::EndFullScreen()
{
		if(m_bFullScreen)
		{
			ShowWindow(SW_HIDE);
			SetWindowPlacement(&m_OldWndPlacement);
			m_bFullScreen = false;
		}
}

void CMainFrame::ToggleReportView()
{
	/*
	bReport = !bReport;
	if(bReport)
	{
		CReportView *rv;
		CRect rect;
		rv = (CReportView*)m_Split.GetPane(0,0);
		GetWindowRect( &rect );
		m_Split.SetColumnInfo(0, rect.Width()/2, 10);
		m_Split.SetColumnInfo(1, rect.Width()/2, 10);
		m_Split.RecalcLayout();
		rv->Refresh();
	}
	else
	{
		CRect rect;
		GetWindowRect( &rect );
		m_Split.SetColumnInfo(1, rect.Width(), 10);
		m_Split.SetColumnInfo(0, 0, 10);
		m_Split.RecalcLayout();
	}
	*/
}

void CMainFrame::ToggleTestView()
{

}

void CMainFrame::OnToggleTest() 
{
	// TODO: Add your command handler code here

}
