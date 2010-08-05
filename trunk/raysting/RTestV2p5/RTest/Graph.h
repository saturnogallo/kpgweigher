#if !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
#define AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraph window

class CGraph : public CStatic
{
// Construction
public:
	CGraph();

// Attributes
public:
	double data[100];
	int		validcount;
private:
	void ImportValue();
	int tickSpace;	//number of pixels between "y" axis ticks
	int maxHeight;
	int maxWidth;
	int yOrgPoint;
	int xOrgPoint;
	int ytickRange;
	int ytickSpace;
	int xtickRange;
	int xtickSpace;
	CString graphTitle;

//	double mean;	//mean value as the y axis align
//	double imin;
//	double imax;
	double lower;
	double upper;
// Operations
public:
	void DrawGraph(CDC* pDC,CRect graphRect);
	void DrawAxis(CDC* pDC);
	void DrawSeries(CDC* pDC);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	double dbase;
	virtual ~CGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraph)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
