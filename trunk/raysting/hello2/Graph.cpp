// Graph.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "Graph.h"
#include "afx.h"
#include "afxwin.h"
#include ".\graph.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AXIS_COLOR		RGB(0,0,0)
#define AXISR_COLOR		RGB(128,128,128)
#define BK_COLOR		RGB(231,231,231)

#define X_SHIFT		40
#define Y_SHIFT		40

#define SHIFTTICK   35
static int VAL2POS(double vup,int pup,double vrange,int prange,double val){
	int dd = 	int((double)pup-(vup-val)*(double)prange/vrange);
	return dd;
}
static double POS2VAL(double vup,int pup,double vrange,int prange,int val){
	double dd =  (vup-(pup-val)*vrange/(double)prange);
	return dd;
}

/////////////////////////////////////////////////////////////////////////////
// CGraph
CGraph::CGraph()
{
	iBufMax = BUF_MAX_DEFAULT;
	mdata = NULL;
	sdata = NULL;
	ch = 0;
	side_ch = 0;
}

CGraph::~CGraph()
{
}
BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_RUN_GRAPH, OnStnDblclickRunGraph)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers
#define COLOR_STEP	32
const unsigned int lcolors[] = { 0x00ff0000,0x0000ff00,0x000000ff,0x00808000,0x00800080,0x00008080};

BOOL CGraph::OnEraseBkgnd(CDC* pDC){
	return FALSE;
}
void CGraph::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);



	if(sdata != NULL){
			UpdateBaseRange(sdata,sbase,srange);
			slinecolor = (COLORREF)lcolors[side_ch];
			sstarcolor = (COLORREF)(lcolors[side_ch]&0x00f7f7f7);

	}
	if(mdata != NULL){
			UpdateBaseRange(mdata,mbase,mrange);
			mlinecolor = (COLORREF)lcolors[ch];
			mstarcolor = (COLORREF)(lcolors[ch]&0x00f7f7f7);
	}
	DrawGraph(&dc,rect);
}
void CGraph::ClearData(int times)
{
	mdata = NULL;	
	sdata = NULL;	
	mbase = mrange = sbase = srange = 0;
	this->Invalidate();
	if(times == 0)
		iBufMax = BUF_MAX_DEFAULT;
	if(times > BUF_MAX_DEFAULT)
		iBufMax = BUF_MAX_DEFAULT;
}

void CGraph::UpdateBaseRange(CList<double,double> *data, double &base,double &range)
{
	int num;
	num = data->GetCount();
	base = 0;
	range = 0;
	if(num == 0)
		return;
	
	POSITION pos;
	pos = data->GetHeadPosition();

	int j=data->GetCount();
	while(j > BUF_MAX_DEFAULT)
	{
		data->GetNext(pos);
		j--;
	}
	base = 0;
	num = 0;
	do{
		base = base + data->GetAt(pos);
		num++;
		if(pos == data->GetTailPosition())
			break;
		data->GetNext(pos);
	}while(1);

	base = base/num;
	
	pos = data->GetHeadPosition();

	j=data->GetCount();
	while(j > BUF_MAX_DEFAULT)
	{
		data->GetNext(pos);
		j--;
	}

	range = 0;

	do{
		range = range + (base - data->GetAt(pos))*(base - data->GetAt(pos));
		if(pos == data->GetTailPosition())
			break;
		data->GetNext(pos);
	}while(1);

	range = 5*sqrt(range/num);
	if(range < 1e-10)
		range = 1;

}
double CGraph::GetAverage(CList<double,double> *rdata)
{
	double mybase;
	int num;
	num = rdata->GetCount();
	

	if(num == 0)
		return 0.0;
	
	POSITION pos;
	pos = rdata->GetHeadPosition();
	mybase = 0;

	do{
		mybase = mybase + rdata->GetAt(pos);
		if(pos == rdata->GetTailPosition())
			break;
		rdata->GetNext(pos);
	}while(1);

	return  mybase/num;
}
void CGraph::DrawGraph(CDC *pDC,CRect graphRect)
{
	CBrush brush;
	brush.CreateSolidBrush(BK_COLOR);	//black background

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(&graphRect,&brush);
	
	maxRect = graphRect;
	maxRect.DeflateRect(0,15,0,15);
	axisRect = maxRect;
	axisRect.InflateRect(-10,-5);
	axisRect.OffsetRect(5,0);

	if(mdata && (mdata->GetCount() > 0))
	{
		DrawAxis(pDC);
		DrawSeries(pDC,mdata,mbase,mrange,mlinecolor,mstarcolor);
	}
	if(sdata && (sdata->GetCount() > 0))
	{
		DrawSideAxis(pDC);
		DrawSeries(pDC,sdata,sbase,srange,slinecolor,sstarcolor);
	}
}
void CGraph::DrawUpArrow(CDC* pDC, CPoint pos)
{
	CPoint pt[3];

	pDC->SetTextColor(AXIS_COLOR);

	pDC->MoveTo(pos+CPoint(-2,3));  
	pDC->LineTo(pos);

	pDC->MoveTo(pos+CPoint(2,3));  
	pDC->LineTo(pos);
}
void CGraph::DrawBigText(CDC *pDC, CPoint pos)
{
	LOGFONT plf;
	pDC->GetCurrentFont()->GetLogFont(&plf);
	
	CFont font;
	plf.lfHeight *= 2;
	plf.lfWidth *= 2;
	font.CreateFontIndirect(&plf);

	// Do something with the font just created...
	CFont* def_font = pDC->SelectObject(&font);
	COLORREF oldc = pDC->SetTextColor(mstarcolor);
	pDC->DrawText(bigtext,CRect(pos+CPoint(-10,0),pos+CPoint(-1,5)), DT_CENTER|DT_NOCLIP);
	pDC->SelectObject(def_font);
	// Done with the font.  Delete the font object.
	font.DeleteObject(); 
	pDC->SetTextColor(oldc);
}
void CGraph::DrawSmallText(CDC *pDC, CPoint pos)
{
	LOGFONT plf;
	pDC->GetCurrentFont()->GetLogFont(&plf);
	
	CFont font;
	plf.lfHeight *= 1;
	plf.lfWidth *= 1;
	font.CreateFontIndirect(&plf);
	
	// Do something with the font just created...
	CFont* def_font = pDC->SelectObject(&font);
	COLORREF oldc = pDC->SetTextColor(mstarcolor);
	pDC->DrawText(smalltext,CRect(pos+CPoint(-10,0),pos+CPoint(-1,9)), DT_LEFT|DT_NOCLIP);
	pDC->SelectObject(def_font);
	// Done with the font.  Delete the font object.
	font.DeleteObject(); 
	pDC->SetTextColor(oldc);
}

void CGraph::DrawText(CDC* pDC, CPoint pos, CString text)
{
	pDC->DrawText(text,CRect(pos+CPoint(-10,0),pos+CPoint(-1,5)),DT_LEFT|DT_NOCLIP);
}
void CGraph::DrawSideAxis(CDC* pDC)
{
	CPen axispen(PS_SOLID,1,AXISR_COLOR);
	CPen *oldpen;
	oldpen = pDC->SelectObject(&axispen);
	
	//draw y axis
	pDC->MoveTo(CPoint(axisRect.right,axisRect.top));  
	pDC->LineTo(CPoint(axisRect.right,axisRect.bottom));
	
	//draw y tick
	pDC->MoveTo(CPoint(axisRect.right, axisRect.top + axisRect.Height()/2));  
	pDC->LineTo(CPoint(axisRect.right - 2, axisRect.top + axisRect.Height()/2));
	
	pDC->MoveTo(CPoint(axisRect.right, axisRect.top + axisRect.Height()/4));  
	pDC->LineTo(CPoint(axisRect.right - 2, axisRect.top + axisRect.Height()/4));
	
	pDC->MoveTo(CPoint(axisRect.right, axisRect.top + axisRect.Height()*3/4));  
	pDC->LineTo(CPoint(axisRect.right - 2, axisRect.top + axisRect.Height()*3/4));
	

	//draw labels
	CString tickLabel;
	
	//draw y axis ticks
	tickLabel.Format(_T("%.4f"),sbase);
	DrawText(pDC,CPoint(axisRect.right-40, axisRect.top + axisRect.Height()/2), tickLabel);
	
	tickLabel.Format(_T("%.4f"),sbase + srange);
	DrawText(pDC,CPoint(axisRect.right-40,axisRect.top) + CPoint(0,-2), tickLabel);

	tickLabel.Format(_T("%.4f"),sbase + srange/2);
	DrawText(pDC,CPoint(axisRect.right-40,axisRect.top) + CPoint(0,axisRect.Height()/4-2), tickLabel);
	
	tickLabel.Format(_T("%.4f"),sbase - srange);
	DrawText(pDC,CPoint(axisRect.right-40,axisRect.top) + CPoint(0, axisRect.Height()-2), tickLabel);

	tickLabel.Format(_T("%.4f"),sbase - srange/2);
	DrawText(pDC,CPoint(axisRect.right-40,axisRect.top)+CPoint(0, axisRect.Height()*3/4-2), tickLabel);
	
	pDC->SelectObject(oldpen);
}
void CGraph::DrawAxis(CDC* pDC)
{
	CPen axispen(PS_SOLID,1,AXIS_COLOR);
	CPen *oldpen;
	oldpen = pDC->SelectObject(&axispen);

	DrawUpArrow(pDC, axisRect.TopLeft()+CPoint(SHIFTTICK, 0));
	//draw y axis
	pDC->MoveTo(axisRect.TopLeft()+CPoint(SHIFTTICK, 0));  
	pDC->LineTo(axisRect.TopLeft() + CPoint(SHIFTTICK,axisRect.Height()));

	//draw y tick
	pDC->MoveTo(axisRect.TopLeft()+CPoint(SHIFTTICK, axisRect.Height()/2));  
	pDC->LineTo(axisRect.TopLeft()+CPoint(SHIFTTICK+2,axisRect.Height()/2));

	pDC->MoveTo(axisRect.TopLeft()+CPoint(SHIFTTICK, axisRect.Height()*3/4));  
	pDC->LineTo(axisRect.TopLeft()+CPoint(SHIFTTICK+2,axisRect.Height()*3/4));

	pDC->MoveTo(axisRect.TopLeft()+CPoint(SHIFTTICK, axisRect.Height()/4));  
	pDC->LineTo(axisRect.TopLeft()+CPoint(SHIFTTICK+2,axisRect.Height()/4));
	
	//draw x axis
	pDC->MoveTo(axisRect.TopLeft()+CPoint(SHIFTTICK, axisRect.Height()));  
	pDC->LineTo(axisRect.TopLeft()+CPoint(axisRect.Width(),axisRect.Height()));

	//draw labels
	CString tickLabel;
	

	//draw y axis ticks
	tickLabel.Format(_T("%.4f"),mbase);
	DrawText(pDC,axisRect.TopLeft()+CPoint(0, axisRect.Height()/2), tickLabel);
		
	tickLabel.Format(_T("%.4f"),mbase + mrange);
	DrawText(pDC,axisRect.TopLeft()+CPoint(0,-2), tickLabel);

	tickLabel.Format(_T("%.4f"),mbase + mrange/2);
	DrawText(pDC,axisRect.TopLeft()+CPoint(0,axisRect.Height()/4-2), tickLabel);
	
	tickLabel.Format(_T("%.4f"),mbase - mrange);
	DrawText(pDC,axisRect.TopLeft()+CPoint(0, axisRect.Height()-2), tickLabel);
		
	tickLabel.Format(_T("%.4f"),mbase - mrange/2);
	DrawText(pDC,axisRect.TopLeft()+CPoint(0, axisRect.Height()*3/4-2), tickLabel);
	
	DrawBigText(pDC,axisRect.TopLeft()+CPoint(axisRect.Width()/2+SHIFTTICK,-2));
	DrawSmallText(pDC,axisRect.TopLeft()+CPoint(axisRect.Width()/4+SHIFTTICK,axisRect.Height()-22));
	
	pDC->SelectObject(oldpen);
}

void CGraph::DrawSeries(CDC* pDC,CList<double,double> *data,double base,double range,COLORREF lcol,COLORREF scol)
{
	CPen linePen(PS_SOLID, 1, lcol);
	CPen starPen(PS_SOLID, 1, scol);

	CPen* pOldPen;
	CPoint orgpt; 
	CPoint lastpt;

	POSITION pos,startpos;
	if(data->GetCount() <= 0)
		return;
	//draw line
	pOldPen = pDC->SelectObject(&linePen);
	orgpt= axisRect.TopLeft()+CPoint(SHIFTTICK, axisRect.Height()/2);  
	
	pos = data->GetHeadPosition();
	int j=data->GetCount();
	while(j > BUF_MAX_DEFAULT)
	{
		data->GetNext(pos);
		j--;
	}
	int x = 0;
	startpos = pos;
	do{
		lastpt = orgpt + CPoint((axisRect.Width())*x/iBufMax,\
			static_cast<int>((base - data->GetAt(pos))*maxRect.Height()/(2*range)));
		if(pos != startpos){
			pDC->LineTo(lastpt);
		}else{
			pDC->MoveTo(lastpt);
		}
//		pDC->Ellipse(lastpt.x-1,lastpt.y-1,lastpt.x+1,lastpt.y+1);
		if(pos == data->GetTailPosition())
			break;
		data->GetNext(pos);
		x++;
	}while(1);

	pDC->SelectObject(pOldPen);

	//draw mark
	pOldPen = pDC->SelectObject(&starPen);
	orgpt= axisRect.TopLeft()+CPoint(SHIFTTICK, axisRect.Height()/2);  
	
	pos = data->GetHeadPosition();
	j=data->GetCount();
	while(j > BUF_MAX_DEFAULT)
	{
		data->GetNext(pos);
		j--;
	}
	x = 0;
	startpos = pos;
	do{
		lastpt = orgpt + CPoint((axisRect.Width())*x/iBufMax,\
			static_cast<int>((base - data->GetAt(pos))*maxRect.Height()/(2*range)));
		if(pos != startpos){
//			pDC->LineTo(lastpt);
		}else{
			pDC->MoveTo(lastpt);
		}
		pDC->Ellipse(lastpt.x-2,lastpt.y-2,lastpt.x+2,lastpt.y+2);
		if(pos == data->GetTailPosition())
			break;
		data->GetNext(pos);
		x++;
	}while(1);

	pDC->SelectObject(pOldPen);

}


void CGraph::OnStnDblclickRunGraph()
{
	// TODO: 在此添加控件通知处理程序代码
	//is this true?
	bool a = true;
//	AfxMessageBox(_T("hit"));
}
