// Graph.cpp : implementation file
//

#include "stdafx.h"
//#include "testdraw.h"
#include "math.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraph

CGraph::CGraph()
{
	graphTitle.Format("图示结果");
	for(int i=0;i<32;i++)
	{
		data[i]=i*0.1+0.5;
	}
	validcount=30;
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
	brush.CreateSysColorBrush(COLOR_BTNFACE);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(&graphRect,&brush);

	maxHeight=graphRect.Height();
	maxWidth=graphRect.Width();
	xOrgPoint=graphRect.left+10; //10 pixel to show value
	yOrgPoint=graphRect.top+10;  //10 pixel to show title

	CFont titleFont;
	titleFont.CreateFont(28, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
	CFont* pOldFont = (CFont*) pDC->SelectObject(&titleFont);
//	pDC->TextOut(maxWidth/2+xOrgPoint-5,yOrgPoint-40,graphTitle);
	pDC->SelectObject(pOldFont);
	ytickRange=maxHeight-25;	//5 pixel as space & 10 pixel as label
	ytickSpace=ytickRange/50;   // pixel per step
	xtickRange=maxWidth-20;		//5 pixel as space & 10 pixel as label
	xtickSpace=xtickRange/validcount; //pixel per step
	if(validcount <= 0)
		return;
	DrawAxis(pDC);
	ImportValue();
	DrawSeries(pDC);
}
void CGraph::DrawAxis(CDC* pDC)
{
	CString tickLabel;
	pDC->SetTextColor(RGB(0,0,0));

	//draw y axis
	pDC->MoveTo(xOrgPoint, yOrgPoint);  
	pDC->LineTo(xOrgPoint, yOrgPoint + ytickRange);

	//draw x axis
	//pDC->MoveTo(xOrgPoint, yOrgPoint+ytickRange);  
	pDC->MoveTo(xOrgPoint, yOrgPoint+ytickSpace*25+2*ytickSpace);  
	pDC->LineTo(xOrgPoint + xtickRange, yOrgPoint+ytickSpace*25+2*ytickSpace);

	//draw labels
	//draw y axis ticks
	pDC->MoveTo(xOrgPoint-4,yOrgPoint+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+2*ytickSpace);
	tickLabel.Format("+超出");
	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint, tickLabel);

	pDC->MoveTo(xOrgPoint-4,yOrgPoint+5*ytickSpace+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+5*ytickSpace+2*ytickSpace);
	tickLabel.Format(" 20");
	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+5*ytickSpace, _T(" 20"));

	pDC->MoveTo(xOrgPoint-4,yOrgPoint+15*ytickSpace+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+15*ytickSpace+2*ytickSpace);

	tickLabel.Format(" 10");
	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+15*ytickSpace, _T(" 10"));
	
	pDC->MoveTo(xOrgPoint-4,yOrgPoint+25*ytickSpace+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+25*ytickSpace+2*ytickSpace);
	tickLabel.Format(" 0 ");
	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+25*ytickSpace, _T(" 0 "));

	pDC->MoveTo(xOrgPoint-4,yOrgPoint+35*ytickSpace+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+35*ytickSpace+2*ytickSpace);

	tickLabel.Format("-10");
	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+35*ytickSpace, _T("-10"));

	pDC->MoveTo(xOrgPoint-4,yOrgPoint+45*ytickSpace+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+45*ytickSpace+2*ytickSpace);

	tickLabel.Format("-20");

	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+45*ytickSpace, _T("-20"));

	pDC->MoveTo(xOrgPoint-4,yOrgPoint+50*ytickSpace+2*ytickSpace);
	pDC->LineTo(xOrgPoint,yOrgPoint+50*ytickSpace+2*ytickSpace);
	tickLabel.Format("-超出");
	pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+50*ytickSpace, tickLabel);

/*
	for(int y = 0; y <= 4; y++)  //four ticks
	{
		pDC->MoveTo(xOrgPoint-4,yOrgPoint+ytickRange-y*ytickSpace);
		pDC->LineTo(xOrgPoint,yOrgPoint+ytickRange-y*ytickSpace);
		//draw tick label
		if(imin==imax)
		{
			if(y==0)
				tickLabel.Format("0");
			else
				tickLabel.Format("");
		}
		tickLabel.Format("%6.2f", imin+(imax-imin)*y/4);
		pDC->TextOut(xOrgPoint -4 - (tickLabel.GetLength() * 8),yOrgPoint+ytickRange-y*ytickSpace, tickLabel);
	}
*/
	//draw X axis tick marks
	for(int x = 0; x <= validcount/5; x++)
	{
		pDC->MoveTo(xOrgPoint+x*5*xtickSpace,yOrgPoint+ytickRange+14);
		pDC->LineTo(xOrgPoint+x*5*xtickSpace,yOrgPoint+ytickRange+10);
		//draw tick label
		tickLabel.Format("%d",x*5);
//		pDC->SetBkColor(RGB(0,255,255));

		pDC->TextOut(xOrgPoint+x*5*xtickSpace,yOrgPoint+ytickRange+22, tickLabel);
	}
}

void CGraph::DrawSeries(CDC* pDC)
{
	COLORREF barColor1,barColor2;
	barColor1 = RGB(0,255,0);
	barColor2 = RGB(255,0,255);  //for overdata use
	int lastx,lasty;
	int ydata;
//	int ySpace;
//	ySpace=ytickRange/50;

	CPen linePen1(PS_SOLID, 1, RGB(127,127,127));
	CPen linePen2(PS_SOLID, 1, barColor2);
	CBrush brush1 (barColor1);
	CBrush brush2 (barColor2);
	CBrush* pOldBrush;
	pOldBrush = pDC->SelectObject(&brush1);
	CPen* pOldPen;
	pOldPen=pDC->SelectObject(&linePen1);
	lastx = xOrgPoint;
	lasty = (yOrgPoint+27*ytickSpace);
	ydata = lasty;
	for(int x=0;x<validcount;x++)
	{
	   		   //now have x and y location of center of ellipse

				//draw line back to last data member
				ydata=(int)((yOrgPoint+27*ytickSpace)-((data[x]-1)*ytickSpace*(1.0e+6)));

				if(ydata <= (yOrgPoint+7*ytickSpace+2))
				{
					ydata = (int)(yOrgPoint+2*ytickSpace);
				}
				if(ydata >= (yOrgPoint+47*ytickSpace-2))
					ydata = (int)(yOrgPoint+52*ytickSpace);

				if(x > 0)
				{

					pDC->MoveTo(lastx + 1, lasty - 1);
					pDC->LineTo(xOrgPoint+x*xtickSpace-1,ydata-1);
				}
				//now draw ellipse...
				if((ydata != (yOrgPoint+2*ytickSpace))&&(ydata != (yOrgPoint+52*ytickSpace)))
					pDC->SelectObject(&brush2);
				else
					pDC->SelectObject(&brush1);
				pDC->Ellipse(xOrgPoint+x*xtickSpace - 2, ydata-2,
					xOrgPoint+x*xtickSpace+ 2, ydata+ 2);
				lastx = xOrgPoint+x*xtickSpace;
				lasty = ydata;
	}
	pDC->SelectObject(&pOldBrush);
	pDC->SelectObject(&pOldPen);
}

void CGraph::ImportValue()
{
	int index;
	
	//check the lower
	upper = (dbase >= 0)?1.00002:0.99998;
	lower = (dbase >= 0)?0.99998:1.00002;
	
	upper = dbase*upper;
	lower = dbase*lower;

	index=0;
	while(index < validcount)
	{
		if(data[index] < lower)
			data[index]=lower;
		if(data[index] > upper)
			data[index]= upper;

		if(dbase != 0)
			data[index]=(data[index]/dbase);
		else
			data[index]=1;
		index++;
	}
}
