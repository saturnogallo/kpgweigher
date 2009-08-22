#if !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
#define AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraph window
#include "Afxtempl.h"
class CGraph : public CStatic
{
// Construction
public:
	CGraph();
	void DrawGraph(CDC* pDC,CRect graphRect);
	static double GetAverage(CList<double,double> *rdata);
	void ClearData(int maxbuf);
	void AssignMainData(CList<double,double> *rdata)
	{	
		mdata = rdata;
		if(mdata != NULL)
			UpdateBaseRange(mdata,mbase,mrange);
	};
	void AssignSideData(CList<double,double> *rdata)
	{	
		sdata = rdata;
		if(sdata != NULL)
			UpdateBaseRange(sdata,sbase,srange);
	};
	void DrawSeries(CDC* pDC,CList<double,double> *data, double base, double range,COLORREF lcol,COLORREF scol);
	
	int iBufMax;
	CString bigtext;
	CString smalltext;
	COLORREF mlinecolor;
	COLORREF mstarcolor;
	COLORREF slinecolor;
	COLORREF sstarcolor;
	int ch;
	int side_ch;
	// Attributes
	double mbase;	//data base of y axis
	
private:
	void RemoveAll(){ 
		sdata = NULL; mdata = NULL;
	};
	CList<double,double> *sdata;
	CList<double,double> *mdata;

	double mrange;	//boundry of y axis
	double sbase;
	double srange;

	int type;
	CRect maxRect;
	CRect axisRect;

	// Operations
	void UpdateBaseRange(CList<double,double> *data,double &base, double &range);
	void DrawAxis(CDC* pDC);
	void DrawSideAxis(CDC* pDC);
	void DrawText(CDC* pDC, CPoint pos, CString text);
	void DrawBigText(CDC *pDC, CPoint pos);
	void DrawSmallText(CDC *pDC, CPoint pos);
	void DrawUpArrow(CDC* pDC, CPoint pos);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraph)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnDblclickRunGraph();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
