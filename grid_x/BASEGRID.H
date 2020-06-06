// Basegrid.h

/*****************************************************************
These source files represent significant changes to the original
WorldCom MFC library.  Therefore, these source files may not be
copied except by permission of the author, Joe Willcoxson.  These
source files may not be included in source or compiled form as any
part of a commercial distribution except by permission of the
author.  These files may be used freely for internal corporate use
such as software produced by an IS departement.  These files may
also may be used freely by students for educational purposes.

No warranty or guarrantee of fitness can be made to these files.
*****************************************************************/

#ifndef cbasegrid_h
#define cbasegrid_h

/////////////////////////////////////////////////////////////////////////////
// CBaseGrid window

#include "cellindi.h"
#include "cellrang.h"

//////  USAGE NOTES

/*
  	If using this class from within a CDialog/CFormView, please take note:
  	
  	1.  In your .RC file, layout a user-defined control with the name "WORLDCOMGRID".
  	2.  In your OnCreate() or OnInitDlg() handler, subclass the user defined control with
  			the grid control:
  				
  				m_grid.SubclassDlgItem(IDC_USER1, this);
	
	3.  You will have to customize the grid. Overload the virtual member functions DrawCell() 
		and GetCellExtent().
		
	4.  If you want to enable editting in your derived Grid, overload the GetEditText()
		and SetEditText() member functions, and also remember to call EnableEdit().

*/

#ifdef WIN32
	#define WCMFC_DEF
	#ifdef WCMFCLIB
	#define WCMFCLIB_CLASS AFX_CLASS_EXPORT
	#else
	#define WCMFCLIB_CLASS AFX_CLASS_IMPORT
	#endif
	#define __export
#else
	#define WCMFCLIB_CLASS
	#ifdef _WCMFC_DEF
	#define WCMFC_DEF EXPORT
	#else
	#define WCMFC_DEF
	#endif
#endif


#define GHT_DATA 0x0000
#define GHT_TOPLEFT 0x0001
#define GHT_COLHDR 0x0002
#define GHT_ROWHDR 0x0004
#define GHT_COLSIZER 0x0008
#define GHT_ROWSIZER 0x0010
#define GHT_LEFT 0x0020
#define GHT_RIGHT 0x0040
#define GHT_ABOVE 0x0080
#define GHT_BELOW 0x0100

class WCMFCLIB_CLASS CBaseGrid : public CWnd
{
// Construction
public:
	WCMFC_DEF CBaseGrid(int rows, int cols, int fixedRows = 1, int fixedCols = 1);
	
	BOOL WCMFC_DEF SubclassDlgItem(UINT nID, CWnd* parent); // use in Dialog/FormView 
	
	BOOL WCMFC_DEF Create(const RECT& rect, CWnd* parent, UINT nID, CCreateContext* pCC = 0, 
	   DWORD style = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	static void WCMFC_DEF RegisterClass();

// Attributes
public:
	int WCMFC_DEF HitTest(CPoint pt, int& xCell, int& yCell);
	void WCMFC_DEF GetClientPix(CRect* pRect);
	
	int WCMFC_DEF GetRowCount();
	int WCMFC_DEF GetColCount();
	
	long WCMFC_DEF GetVirtualWidth(); // in pixels
	long WCMFC_DEF GetVirtualHeight(); // in pixels
	
	enum MouseMode{Nothing, 
						SelectingCells, 
						SelectingRow, 
						SelectingCol, 
						SelectingAll, 
						SizingColumn, // not implemented .... yet
						SizingRow, // ditto
						Righty  // anticipation for right mouse button activity
						};
	
// Operations
public:
	void WCMFC_DEF SetRowCount(int rows);
	void WCMFC_DEF SetColCount(int cols);

	void WCMFC_DEF SetRowHeight(int row, int height);
	void WCMFC_DEF SetColWidth(int col, int width);

	int WCMFC_DEF GetRowHeight(int row);
	int WCMFC_DEF GetRowStartY(int row);
	int WCMFC_DEF GetColWidth(int col);
	int WCMFC_DEF GetColStartX(int col);

	int WCMFC_DEF GetFixedRowCount();
	int WCMFC_DEF GetFixedColCount();
	void WCMFC_DEF SetFixedRowCount(int fixedRows);
	void WCMFC_DEF SetFixedColCount(int fixedCols);
	
	int WCMFC_DEF GetFirstSelectedRow();
	int WCMFC_DEF GetLastSelectedRow();
	int WCMFC_DEF GetFirstSelectedCol();
	int WCMFC_DEF GetLastSelectedCol();
	
	// list mode selects cells as whole rows
	// individual cell selection is not possible
	void WCMFC_DEF SetListMode(BOOL bEnableListMode = TRUE);
	BOOL WCMFC_DEF IsListMode() {return m_bListMode;}
	
	const CCellRange& WCMFC_DEF GetSelectedRange();
	void WCMFC_DEF SetSelectedRange(const CCellRange& newSelectedRange, BOOL ForceRepaint = TRUE);
	CCellRange WCMFC_DEF GetVisibleNonFixedCellRange();

	void WCMFC_DEF AutoSize(); // sizes all rows and cols
	void WCMFC_DEF AutoSizeAllCols();
	void WCMFC_DEF AutoSizeAllRows();
	void WCMFC_DEF AutoSizeCol(int col);
	void WCMFC_DEF AutoSizeRow(int row);
	
	void WCMFC_DEF ExpandToFit(); // expands row heights and col widths to fit dead space

	BOOL WCMFC_DEF GetConstrainedFlag();
	void WCMFC_DEF SetConstrainedFlag(BOOL bConstrain = TRUE);

	void WCMFC_DEF GetFont(LOGFONT& logFont);
	void WCMFC_DEF SetFont(const LOGFONT& logFont);
	void WCMFC_DEF SetFont(const char* facename);
	
	COLORREF WCMFC_DEF GetTextColor();
	void WCMFC_DEF SetTextColor(COLORREF textColor);

	virtual void WCMFC_DEF Draw(CDC& dc, int rowOffset, int columnOffset);
	virtual int WCMFC_DEF HitTest(CPoint pt, CCellIndices& indices);

	int FillPrintIndices(CPrintIndicesArray& printIndices, const CSize& extent);

	int WCMFC_DEF PrepareForPrinting(CDC* pDC);
	void WCMFC_DEF PrintPage(CDC* pDC, CPrintInfo* pInfo);
	void WCMFC_DEF PrintPage(CDC* pDC, int page);
	
// Implementation
public:
	virtual WCMFC_DEF ~CBaseGrid();

	protected:
	virtual void WCMFC_DEF DrawCell(int row, int col, CDC* pDC, int x, int y);
	virtual CSize WCMFC_DEF GetCellExtent(int row, int col, CDC* pDC);
	void WCMFC_DEF PrepareDC(CDC* pDC);
	void WCMFC_DEF ResetDC(CDC* pDC);
	void WCMFC_DEF ResetScrollBars();


	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseGrid)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	// Attributes
	int WCMFC_DEF GetFixedRowHeight();
	int WCMFC_DEF GetFixedColWidth();
	
	int WCMFC_DEF GetColStartX(int col, int colOffset);
	int WCMFC_DEF GetRowStartY(int row, int rowOffset);

	int WCMFC_DEF GetColStartXAbsolute(int col);
	int WCMFC_DEF GetRowStartYAbsolute(int row);

	int WCMFC_DEF IsTopLeftCell(const CCellIndices& cellIndices);
	int WCMFC_DEF IsTopFixedCell(const CCellIndices& cellIndices);
	int WCMFC_DEF IsLeftFixedCell(const CCellIndices& cellIndices);
	
	CCellRange WCMFC_DEF GetUnobstructedCellsInRect(CRect R);
	int WCMFC_DEF IsCellVisible(CCellIndices cell);
	int WCMFC_DEF IsSelectedCellVisible();
	
	int m_nDefCellHeight;
	int m_nDefCellWidth;
	
	int m_nFixedRows;
	int m_nFixedCols;
	
	int m_nRows;
	int m_nCols;
	
	int m_nVScrollMax;
	int m_nHScrollMax;
	
	CUIntArray m_auRowHeights;
	CUIntArray m_auColWidths;
	
	CCellRange m_SelectedCellRange;
	                   
	MouseMode m_MouseMode;	                   
	CCellIndices m_LeftClickDownCell;
	CCellIndices m_LeftClickUpCell;
	CCellIndices m_MouseMoveCell;
	CPoint m_LastMousePoint;
	
	BOOL m_bConstrained;

	int m_nTimerID;

	CFont m_Font;
	CFont* m_pOrigFont; // font before m_Font selected in
	COLORREF m_TextColor;
	COLORREF m_OrigTextColor; // color before m_TextColor selected in
	
	int m_nCellBorderThickness;
	
	BOOL m_bListMode;
	
	// in place editting stuff!
	BOOL m_bEditEnabled;
	BOOL m_bEditMode;
	
	CString m_strOldEditData; // stores old cell data 
	CString m_strCurrentEditData; // current edit cell data
	static ATOM m_atomClassName;

	CPrintIndicesArray m_PrintIndices;
	
	void WCMFC_DEF EnableEdit(BOOL bEnableEdit = TRUE); // can only call from base/derived class
	
	// called during StopEdit to return the editted text
	virtual BOOL WCMFC_DEF GetEditText(int row, int col, const CString& text);
	
	// called during StartEdit to set the edittable text
	virtual BOOL WCMFC_DEF SetEditText(int row, int col, CString& text);
	virtual void WCMFC_DEF DrawEditText();
	virtual BOOL WCMFC_DEF StartEdit(UINT nChar, UINT nFlags);
	virtual BOOL WCMFC_DEF StopEdit(int nChar = VK_RETURN);
	virtual BOOL WCMFC_DEF CancelEdit();
	virtual CEdit* WCMFC_DEF CreateEditWnd(const CRect& r, LPCTSTR lpszText, int nChar, int nFlags);

	friend class CGridEditWnd;
	CEdit* WCMFC_DEF GetEditControl();
	CEdit* m_pEdit;
	
	protected:
		void OnSelecting(const CCellIndices& currentCell);
		void OnSelectingCells(CCellIndices currentCell);
		void OnSelectingRows(CCellIndices currentCell);
		void OnSelectingCols(CCellIndices currentCell);

	CCellIndices WCMFC_DEF GetCellIndices(CPoint clickPoint);
	void WCMFC_DEF InvalidateCellRect(const CCellRange& cellRange);
	CRect WCMFC_DEF GetCellRangeRect(const CCellRange& cellRange);
	CRect WCMFC_DEF GetCellRangeRect(const CCellRange& cellRange, int rowOffset, int colOffset);
	CCellRange WCMFC_DEF GetCellRange();
	void WCMFC_DEF ResetSelectedRange();
	
	private:
	static LONG FAR PASCAL __export DummyWndProc(HWND, UINT, WPARAM, LPARAM);
	virtual WNDPROC* WCMFC_DEF GetSuperWndProcAddr();
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS
/////////////////////////////////////////////////////////////////////////////

#endif // end of sentinel code

// Copyright (c) 1997 by Joe Willcoxson and WorldCom.  All rights reserved.
// E-mail:  chinajoe@aol.com
// URL:  http://users.aol.com/chinajoe
// For great long distance service see http://www.wcom.com

