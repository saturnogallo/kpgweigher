// Graph.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AXIS_COLOR		RGB(255,255,255)
#define BK_COLOR		RGB(0,0,0)
#define LINE_COLOR		RGB(255,127,255)
#define STAR_COLOR		RGB(127,255,255)
#define MARK_COLOR		RGB(255,255,127)
#define CURSOR_COLOR	RGB(127,127,255)
#define X_SHIFT		40
#define Y_SHIFT		40

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
	data.RemoveAll();
	upper = 1.0+(6e-6);
	lower = 1.0-(6e-6);
	dbase = 1;
	dcursor = 4e-6;
	criteria = 4e-6;
	for(int i=1;i < 25;i++){
		data.Add(1+0.00000001*rand());
	}
}


CGraph::~CGraph()
{
}

BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers
void CGraph::DrawGraph(CDC *pDC,CRect graphRect)
{
	CRect rc;
	CBrush brush;

	brush.CreateSolidBrush(BK_COLOR);	//black background

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(&graphRect,&brush);

	maxHeight=graphRect.Height();
	maxWidth=graphRect.Width();
	xOrgPoint=graphRect.left+xshift; //mark 10 pixel to show value
	yOrgPoint=graphRect.top+yshift;  //mark 10 pixel to show title

	//mark title in the center
	CFont titleFont;
	titleFont.CreateFont(12, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,_T("Arial"));
	CFont* pOldFont = (CFont*) pDC->SelectObject(&titleFont);
	pDC->SetTextColor(AXIS_COLOR);

	xshift = X_SHIFT;
	yshift = Y_SHIFT;

	if(maxHeight < (2*yshift))
		yshift = 5 ;
	
	ytickRange=maxHeight-(2*yshift);	// 5 pixel as space & 10 pixel as label

	ytickSpace=ytickRange/8;		// pixel per step (9 level in all)
	if(ytickSpace <= 0)
		ytickSpace = 1;
	
	if(maxWidth < (2*xshift))
		xshift = 5 ;
	
	xtickRange=maxWidth-(2*xshift)-20;	// 5 pixel as space & 10 pixel as label
	
	if(data.GetSize() > 0){
		xtickSpace = xtickRange/10.0;          //pixel per step 
		DrawAxis(pDC);
		DrawSeries(pDC);
	}
	pDC->SelectObject(pOldFont);
}
void CGraph::DrawAxis(CDC* pDC)
{
	CBrush br(AXIS_COLOR);
	CString tickLabel;
	CRgn arrowy,arrowx;
	CPoint pt[3];

	pDC->SetTextColor(AXIS_COLOR);
	//draw y axis and the triangle
	pt[0].x = xOrgPoint;pt[0].y = yOrgPoint-3;
	pt[1].x = xOrgPoint+2;pt[1].y = yOrgPoint;
	pt[2].x = xOrgPoint-2;pt[2].y = yOrgPoint;
	arrowy.CreatePolygonRgn(pt,3,ALTERNATE);
	pDC->FillRgn(&arrowy,&br);
	pDC->MoveTo(xOrgPoint, yOrgPoint);  
	pDC->LineTo(xOrgPoint, yOrgPoint + ytickRange);

	pt[0].x = xOrgPoint+xtickRange+3;pt[0].y = yOrgPoint+ytickRange;
	pt[1].x = xOrgPoint+xtickRange;pt[1].y = yOrgPoint+ytickRange+2;
	pt[2].x = xOrgPoint+xtickRange;pt[2].y = yOrgPoint+ytickRange-2;
	arrowx.CreatePolygonRgn(pt,3,ALTERNATE);
	pDC->FillRgn(&arrowx,&br);
	//draw x axis
	pDC->MoveTo(xOrgPoint, yOrgPoint+ytickRange);  
	pDC->LineTo(xOrgPoint + xtickRange, yOrgPoint+ytickRange);

	//draw labels
	CPen axispen(PS_DOT,1,AXIS_COLOR);
	CPen *oldpen;
	oldpen = pDC->SelectObject(&axispen);
	//draw y axis ticks
	for(int y = yOrgPoint; y < yOrgPoint+ytickRange; y+= ytickSpace)  //four ticks
	{
		tickLabel.Format(_T("%.1fppm"),(POS2VAL(upper,yOrgPoint,upper-lower,-ytickRange,y)-dbase)*1000000.0/dbase);
		//draw tick label
		pDC->TextOut(xOrgPoint-xshift+2,y-4, tickLabel);
		pDC->MoveTo(xOrgPoint,y);
		pDC->LineTo(xOrgPoint+xtickRange,y);
	}
	//draw X axis ticks
	int cnt = 0;
	int step = (data.GetSize()/10);
	if(step == 0) step = 1;
	for(int x = 0; x < data.GetSize(); x += step )
	{
		int xpos;
		if(data.GetSize() > 10)
		{
			tickLabel.Format(_T("%d"),x+1);
			xpos = VAL2POS(1,xOrgPoint,data.GetSize()-1,xtickRange,x+1);
		}else{
			xpos = xOrgPoint + x*xtickSpace;
			tickLabel.Format(_T("%d"),x);
		}
		
		//draw tick label
		pDC->TextOut(xpos-4,yOrgPoint+ytickRange+2, tickLabel);
		pDC->MoveTo(xpos,yOrgPoint);
		pDC->LineTo(xpos,yOrgPoint+ytickRange);
	}
	//draw mark line
	if((dbase > lower) && (dbase < upper)){
		CPen markpen(PS_SOLID,1,MARK_COLOR);
		pDC->SelectObject(&markpen);
		tickLabel.Format(_T("%3.3e"),dbase);
		y = VAL2POS(upper,yOrgPoint,upper-lower,-ytickRange,dbase);
		pDC->TextOut(xOrgPoint+xtickRange+2,y-4, tickLabel);
		pDC->MoveTo(xOrgPoint,y);
		pDC->LineTo(xOrgPoint+xtickRange,y);
	}
	//draw cursor line
	if((dcursor > lower) && (dcursor < upper)){
		CPen cursorpen(PS_SOLID,1,CURSOR_COLOR);
		pDC->SelectObject(&cursorpen);
		tickLabel.Format(_T("%.1fppm(%3.3e)"),((dcursor-dbase)*1000000.0/dbase),dcursor);
		y = VAL2POS(upper,yOrgPoint,upper-lower,-ytickRange,dcursor);
		pDC->TextOut(xOrgPoint+xtickRange-xshift,y-4, tickLabel);
		pDC->MoveTo(xOrgPoint,y);
		pDC->LineTo(xOrgPoint+xtickRange,y);
	}

	pDC->SelectObject(oldpen);
}

void CGraph::DrawSeries(CDC* pDC)
{
	CBrush starbrush (STAR_COLOR);
	CBrush markbrush (MARK_COLOR);

	CBrush* pOldBrush;
	pOldBrush = pDC->SelectObject(&starbrush);

	CPen linePen(PS_SOLID, 1, LINE_COLOR);
	CPen* pOldPen;
	pOldPen=pDC->SelectObject(&linePen);

	double	lastx = xOrgPoint;
	int lasty;
	for(int x=0; x < data.GetSize(); x++)
	{

				if(x != 0)
					pDC->MoveTo(int(lastx + 1), lasty - 1);

				//update y position
				if(data[x] > upper)
					lasty = yOrgPoint;
				else if(data[x] < lower)
						lasty = yOrgPoint+ytickRange;
					 else
						lasty = VAL2POS(upper,yOrgPoint,upper-lower,-ytickRange,data[x]);;

			    //update x position
				if(data.GetSize() <= 10)
					lastx += xtickSpace;
				else
					lastx = VAL2POS(1,xOrgPoint,data.GetSize()-1,xtickRange,x+1);
					 
				if(x != 0)										
					pDC->LineTo(int(lastx-1),lasty-1);

				//now draw ellipse...
				if((data[x] < (dbase+criteria)) && (data[x] > (dbase-criteria)))
					pDC->SelectObject(&starbrush);
				else
					pDC->SelectObject(&markbrush);

				pDC->Ellipse(int(lastx - 3), lasty-3,
					int(lastx+ 3), lasty + 3);
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


void CGraph::MouseUp(short button, short shift, long x, long y)
{
	if(button == LEFT_BUTTON){
		dcursor = POS2VAL(upper,yOrgPoint,upper-lower,-ytickRange,y);
	}
	if(button == RIGHT_BUTTON){
		dbase = POS2VAL(upper,yOrgPoint,upper-lower,-ytickRange,y);
		upper = dbase + 5*criteria;
		lower = dbase - 5*criteria;
	}
}
