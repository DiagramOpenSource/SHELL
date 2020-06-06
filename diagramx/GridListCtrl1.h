#if !defined(AFX_GRIDLISTCTRL1_H__790F2E56_02FB_11D3_B165_004F4905CBCF__INCLUDED_)
#define AFX_GRIDLISTCTRL1_H__790F2E56_02FB_11D3_B165_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridListCtrl1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridListCtrl window

class CGridListCtrl : public CListCtrl
{
// Construction
public:
	CGridListCtrl();

// Attributes
public:
	// The current subitem or column number which is order based.	
	int m_CurSubItem;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridListCtrl)
	public:
	//}}AFX_VIRTUAL
  
// Implementation
public:
	void MakeColumnVisible(int nCol);
	BOOL PositionControl( CWnd * pWnd, int iItem, int iSubItem );
	BOOL PrepareControl(WORD wParseStyle);
	virtual ~CGridListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridListCtrl)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	WORD m_wStyle;
	DECLARE_MESSAGE_MAP()
private:
	int IndexToOrder( int iIndex );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDLISTCTRL1_H__790F2E56_02FB_11D3_B165_004F4905CBCF__INCLUDED_)
