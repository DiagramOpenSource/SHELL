// Basegrid.cpp

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

#include "stdafx.h"
#include "basegrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WCMFC_GRID_EDIT_ID	101

class CGridEditWnd : public CEdit
{
public:
	CGridEditWnd(CBaseGrid* pParent, const CRect& r, LPCTSTR lpszText, UINT nChar, UINT nFlags);
	CString m_strEdit;
protected:
	void OnDestroy();
	int OnCreate(LPCREATESTRUCT pCS);
	void OnChar(UINT nChar, UINT nRepCount, UINT nFlags);
	void OnKeyDown(UINT nChar, UINT nRepCount, UINT nFlags);
	void OnMove(int x, int y);
	CBaseGrid* m_pGrid;
	CFont m_Font;
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CGridEditWnd, CEdit)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_MOVE()
END_MESSAGE_MAP()

CGridEditWnd::CGridEditWnd(CBaseGrid* pParent, const CRect& r, LPCTSTR lpszText, UINT nChar, UINT nFlags)
: m_pGrid(pParent), m_strEdit(lpszText)
{
	ASSERT_VALID(m_pGrid);
	Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, r, pParent, WCMFC_GRID_EDIT_ID);
	SetWindowText(lpszText);
	SetFocus();
	switch (nChar){
	case VK_BACK:
		SetSel((int)strlen(lpszText), -1);
		break;
	case VK_RETURN:
		SetSel((int)strlen(lpszText), -1);
		return;
		break;
	default:
		SetSel(0, -1);
	}
	SendMessage(WM_CHAR, nChar);
}

void CGridEditWnd::OnDestroy()
{
	CEdit::OnDestroy();
}

int CGridEditWnd::OnCreate(LPCREATESTRUCT lpCS)
{
	if (CEdit::OnCreate(lpCS) == -1)
		return -1;
	SetFont(&m_pGrid->m_Font);
	return 0;
}

void CGridEditWnd::OnChar(UINT nChar, UINT nRepCount, UINT nFlags)
{
	switch (nChar){
	case VK_RETURN:
	case VK_ESCAPE:
	case VK_TAB:
		m_pGrid->StopEdit(nChar);
		break;
	default:
		CEdit::OnChar(nChar, nRepCount, nFlags);
	}
}

void CGridEditWnd::OnKeyDown(UINT nChar, UINT nRepCount, UINT nFlags)
{
	switch (nChar){
	case VK_DOWN:
	case VK_UP:
	case VK_NEXT:
	case VK_PRIOR:
	case VK_HOME:
	case VK_END:
		m_pGrid->StopEdit(nChar);
		break;
	default:
		CEdit::OnKeyDown(nChar, nRepCount, nFlags);
	}
}

void CGridEditWnd::OnMove(int x, int y)
{
	CRect r;
	m_pGrid->GetClientRect(&r);
	if (x < m_pGrid->GetFixedColWidth() || y < m_pGrid->GetFixedRowHeight() || x > r.right || y > r.bottom){
		ShowWindow(SW_HIDE);
	} else {
		if (!IsWindowVisible()){
			ShowWindow(SW_SHOW);
			SetFocus();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBaseGrid

static const int widthFactor = 12; // width of cell = width * character width
static const int slop = 3; // pixels around sizing region
ATOM CBaseGrid::m_atomClassName = 0;
static TCHAR szClassName[] = _T("WORLDCOMGRID");

CBaseGrid::CBaseGrid(int rows, int cols, int fixedRows, int fixedCols)
 : m_nRows(rows), m_nCols(cols), m_nFixedRows(fixedRows), m_nFixedCols(fixedCols), m_MouseMode(Nothing),
	m_bConstrained(TRUE), m_TextColor(0), m_nCellBorderThickness(2), m_bEditEnabled(FALSE), 
	m_bEditMode(FALSE), m_bListMode(FALSE), m_pEdit(0), m_nTimerID(0)
{
	RegisterClass();
	CWnd* desktop = GetDesktopWindow();
	CDC* pDC = desktop->GetDC();

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(10, pDC->GetDeviceCaps(LOGPIXELSY), 72); // 10 point font
	strcpy(lf.lfFaceName,_T("Courier New"));
	m_Font.CreateFontIndirect(&lf);

	TEXTMETRIC tm;
	CFont* pOldFont = pDC->SelectObject(&m_Font);
	pDC->GetTextMetrics(&tm);
	m_nDefCellHeight = tm.tmHeight + 2 * m_nCellBorderThickness;
	m_nDefCellWidth = tm.tmAveCharWidth * widthFactor + 2 * m_nCellBorderThickness;
	pDC->SelectObject(pOldFont);
	desktop->ReleaseDC(pDC);
	

	// initialize row heights
	m_auRowHeights.SetSize(rows);
	for (int i = 0; i < rows; i++)
	{
		m_auRowHeights[i] = m_nDefCellHeight;
	}
	
	// initialize col widths
	m_auColWidths.SetSize(cols);
	for (i = 0; i < cols; i++)
	{
		m_auColWidths[i] = m_nDefCellWidth;
	}
	
	// set selection range
	m_SelectedCellRange.Set(m_nFixedRows, m_nFixedCols, m_nFixedRows, m_nFixedCols);
	
	// note - did not initialize cells, they are already zeroed
}

CBaseGrid::~CBaseGrid()
{
	DestroyWindow();
}

BOOL CBaseGrid::Create(const RECT& rect, CWnd* parent, UINT nID, CCreateContext* /*pCC*/ , DWORD style)
{
	ASSERT(parent->GetSafeHwnd());
	HWND hwnd = ::CreateWindow(szClassName, 0, style, 
						rect.left, 
						rect.top,
						rect.right - rect.left,
						rect.bottom - rect.top,
						parent->GetSafeHwnd(), 
						(HMENU) nID, 
						AfxGetInstanceHandle(),
						0);
	ASSERT(hwnd);
	if (hwnd)
	{
		SubclassWindow(hwnd);
		ResetScrollBars();
		return TRUE;
	}
	return FALSE;
}


BOOL CBaseGrid::SubclassDlgItem(UINT nID, CWnd* parent)
{
	if (!CWnd::SubclassDlgItem(nID, parent)) return FALSE;
	ResetScrollBars();   
	return TRUE;
}

BEGIN_MESSAGE_MAP(CBaseGrid, CWnd)
	//{{AFX_MSG_MAP(CBaseGrid)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
#ifdef WIN32
	ON_WM_CAPTURECHANGED()
#endif
END_MESSAGE_MAP()

int CBaseGrid::GetRowCount()
{
	return m_nRows;
}

int CBaseGrid::GetColCount()
{
	return m_nCols;
}

int CBaseGrid::GetFixedRowCount()
{
	return m_nFixedRows;
}

int CBaseGrid::GetFixedColCount()
{
	return m_nFixedCols;
}

long CBaseGrid::GetVirtualWidth()
{
	long lVirtualWidth = 0;
	int iColCount = GetColCount();
	for (int i = 0; i < iColCount; i++)
	{
		lVirtualWidth += m_auColWidths[i];
	}
	return lVirtualWidth;
}

long CBaseGrid::GetVirtualHeight()
{
	long lVirtualHeight = 0;
	int iRowCount = GetRowCount();
	for (int i = 0; i < iRowCount; i++)
	{
		lVirtualHeight += m_auRowHeights[i];
	}
	return lVirtualHeight;
}

void CBaseGrid::SetRowCount(int rows)
{
	if (rows == GetRowCount()) return;
	int addedRows = rows - GetRowCount();
	m_nRows = rows;
	m_auRowHeights.SetSize(rows);
	int startRow = rows - addedRows;
	if (addedRows < 0)
	{
		ResetSelectedRange();
	}
	
	// initialize row heights
	for (int j= startRow; j < GetRowCount(); j++)
	{
		m_auRowHeights[j] = m_nDefCellHeight;
	}
	
	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
}

void CBaseGrid::SetColCount(int cols)
{
	if (cols == GetColCount()) return;
	int addedCols = cols - GetColCount();
	m_nCols = cols;
	m_auColWidths.SetSize(cols);
	if (addedCols > 0)
	{
		// initialized all added cols to NULL
		int startCol = cols - addedCols;
		for (int i = startCol; i < GetColCount(); i++)
		{
			m_auColWidths[i] = m_nDefCellWidth;
		}
	}
	else
	{
		// check for selected cell ranges
		ResetSelectedRange();
	}
	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
}

void CBaseGrid::SetFixedRowCount(int fixedRows)
{
	ASSERT((fixedRows >= 0) && (fixedRows < GetRowCount()));
	m_nFixedRows = fixedRows;
	ResetScrollBars();
}

void CBaseGrid::SetFixedColCount(int fixedCols)
{
	ASSERT((fixedCols >= 0) && (fixedCols < GetColCount()));
	m_nFixedCols = fixedCols;
	ResetScrollBars();
}

int CBaseGrid::GetRowHeight(int row)
{
	return m_auRowHeights[row];
}

int CBaseGrid::GetColWidth(int col)
{
	return m_auColWidths[col];
}

int CBaseGrid::GetFixedRowHeight()
{
	int height = 0;
	for (int i = 0; i < m_nFixedRows; i++)
	{
		height += GetRowHeight(i);
	}
	return height;
}

int CBaseGrid::GetFixedColWidth()
{
	int width = 0;
	for (int i = 0; i < m_nFixedCols; i++)
	{
		width += GetColWidth(i);
	}                          
	return width;
}

int CBaseGrid::GetColStartX(int col, int colOffset)
{
	int xpos = 0;
	if (col < m_nFixedCols) // is a fixed column
	{
		for (int i = 0; i < col; i++)
		{
			xpos += GetColWidth(i);
		}
	}
	else // is a scrollable data column
	{
		xpos = GetFixedColWidth();
		for (int i = m_nFixedCols + colOffset; i < col; i++)
		{
			xpos += GetColWidth(i);
		}
	}
	return xpos;
}

int CBaseGrid::GetColStartX(int col)
{   
	return GetColStartX(col, GetScrollPos(SB_HORZ));
}

int CBaseGrid::GetRowStartY(int row, int rowOffset)
{
	int ypos = 0;
	if (row < m_nFixedRows) // is a fixed row
	{
		for (int i = 0; i < row; i++)
		{
			ypos += GetRowHeight(i);
		}
	}
	else // is a scrollable data row
	{
		ypos = GetFixedRowHeight();
		for (int i = m_nFixedRows + rowOffset; i < row; i++)
		{
			ypos += GetRowHeight(i);
		}
	}
	return ypos;
}

int CBaseGrid::GetRowStartY(int row)
{
	return GetRowStartY(row, GetScrollPos(SB_VERT));
}

int CBaseGrid::GetColStartXAbsolute(int col)
{
	int xpos = 0;
	if (col < m_nFixedCols) // is a fixed column
	{
		for (int i = 0; i < col; i++)
		{
			xpos += GetColWidth(i);
		}
	}
	else // is a scrollable data column
	{
		xpos = GetFixedColWidth();
		for (int i = m_nFixedCols; i < col; i++)
		{
			xpos += GetColWidth(i);
		}
	}
	return xpos;
}

int CBaseGrid::GetRowStartYAbsolute(int row)
{
	int ypos = 0;
	if (row < m_nFixedRows) // is a fixed row
	{
		for (int i = 0; i < row; i++)
		{
			ypos += GetRowHeight(i);
		}
	}
	else // is a scrollable data row
	{
		ypos = GetFixedRowHeight();
		for (int i = m_nFixedRows; i < row; i++)
		{
			ypos += GetRowHeight(i);
		}
	}
	return ypos;
}

BOOL CBaseGrid::GetConstrainedFlag()
{
	return m_bConstrained;
}

void CBaseGrid::SetConstrainedFlag(BOOL bConstrain)
{
	m_bConstrained = bConstrain;
}

const CCellRange& CBaseGrid::GetSelectedRange()
{
	return m_SelectedCellRange;
}

void CBaseGrid::SetSelectedRange(const CCellRange& newSelectedRange, BOOL ForceRepaint)
{
	m_SelectedCellRange = newSelectedRange;
	if (ForceRepaint)
	{
		Invalidate();
	}
}

void CBaseGrid::ResetScrollBars()
{                             
	CRect R;
	GetWindowRect(&R);
	
	// fixes for borders
	
	HWND hwnd = GetSafeHwnd();
	if (hwnd)
	{
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		int cxoffset = 0;
		int cyoffset = 0;
		if (style && WS_THICKFRAME)
		{
			cxoffset = - GetSystemMetrics(SM_CXFRAME);
			cyoffset = - GetSystemMetrics(SM_CYFRAME);
		}
		else if (style && WS_BORDER)
		{
			cxoffset = - GetSystemMetrics(SM_CXBORDER);
			cyoffset = - GetSystemMetrics(SM_CYBORDER);
		}
		else
		{
			TRACE(_T("Grid does not have acceptable border\n"));
			ASSERT(0);
		}
		R.InflateRect(cxoffset, cyoffset);
	}
	
	CCellRange visibleCells = GetUnobstructedCellsInRect(R);
	
	if ((visibleCells.GetMaxRow() == GetRowCount() - 1) &&
			visibleCells.GetMaxCol() == GetColCount() - 1)
	{
		// all cells visible
		m_nVScrollMax = 0;
		m_nHScrollMax = 0;
	}
	else if ((visibleCells.GetMaxRow() < GetRowCount() - 1) &&
				visibleCells.GetMaxCol() == GetColCount() -1)
	{
		// rows obstructed
		m_nVScrollMax = GetRowCount() - m_nFixedRows - 1;
		// check to see it vertical scroll bar obstructs columns
		int cx = -GetSystemMetrics(SM_CXVSCROLL);
		R.left -= cx;
		visibleCells = GetUnobstructedCellsInRect(R);
		if (visibleCells.GetMaxCol() < GetColCount() - 1)
		{
			m_nHScrollMax = GetColCount() - m_nFixedCols - 1;
		}
		else
		{
			m_nHScrollMax = 0;
		}
	}
	else if ((visibleCells.GetMaxRow() == GetRowCount() - 1) &&
				visibleCells.GetMaxCol() < GetColCount() - 1)
	{
		// columns obstructed
		m_nHScrollMax = GetColCount() - m_nFixedCols - 1;
		// check to see if horizontal scroll bar obstructs rows
		int cy = -GetSystemMetrics(SM_CYHSCROLL);
		R.top -= cy;
		visibleCells = GetUnobstructedCellsInRect(R);
		if (visibleCells.GetMaxRow() < GetRowCount() - 1)
		{
			m_nVScrollMax = GetRowCount() - m_nFixedRows - 1;
		}
		else
		{
			m_nVScrollMax = 0;
		}
	}
	else
	{
		// both rows and cols are obstructed
		m_nVScrollMax = GetRowCount() - m_nFixedRows - 1;
		m_nHScrollMax = GetColCount() - m_nFixedCols - 1;
	}
	
	SetScrollRange(SB_VERT, 0, m_nVScrollMax, FALSE);
	SetScrollRange(SB_HORZ, 0, m_nHScrollMax, FALSE);  
}


// this gets even partially visible cells?
CCellRange CBaseGrid::GetVisibleNonFixedCellRange()
{
	int horzScrollPos = GetScrollPos(SB_HORZ);
	int vertScrollPos = GetScrollPos(SB_VERT);
	CRect R;
	GetClientRect(&R);

	// calc bottom
	int bottom = 0;
	int fixedRowHeight = bottom = GetFixedRowHeight();
	for (int i = m_nFixedRows + vertScrollPos; i < GetRowCount(); i++)
	{
		bottom += GetRowHeight(i);
		if (bottom >= R.bottom) break;
	}                                
	int maxVisibleRow = min(i, GetRowCount() - 1);
	
	// calc right
	int right = 0;
	int fixedColWidth = right = GetFixedColWidth();
	for (i = m_nFixedCols + horzScrollPos; i < GetColCount(); i++)
	{
		right += GetColWidth(i);
		if (right >= R.right) break;
	}
	int maxVisibleCol = min(i, GetColCount() - 1);
	return CCellRange(vertScrollPos + m_nFixedRows, horzScrollPos + m_nFixedCols, 
		maxVisibleRow, maxVisibleCol);
}

// used by ResetScrollBars()
CCellRange CBaseGrid::GetUnobstructedCellsInRect(CRect R)
{
	R.OffsetRect(-R.TopLeft());
	// calc bottom
	int bottom = 0;
	for (int i = 0; i < GetRowCount(); i++)
	{
		bottom += GetRowHeight(i);
		if (bottom >= R.bottom) break;
	}                                
	int maxVisibleRow = min(i - 1, GetRowCount() - 1);
	if (bottom > R.bottom)
	{
		maxVisibleRow--;
	}
	
	// calc right
	int right = 0;
	for (i = 0; i < GetColCount(); i++)
	{
		right += GetColWidth(i);
		if (right >= R.right) break;
	}
	int maxVisibleCol = min(i - 1, GetColCount() - 1);
	if (right > R.right)
	{
		maxVisibleCol--;
	}
	return CCellRange(0, 0, maxVisibleRow, maxVisibleCol);
}


/////////////////////////////////////////////////////////////////////////////
// CBaseGrid message handlers


void CBaseGrid::PrepareDC(CDC* pDC)
{
	if (pDC->IsPrinting()){
		CDC screenDC;
		screenDC.CreateIC(_T("DISPLAY"), 0, 0, 0);
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(screenDC.GetDeviceCaps(LOGPIXELSX), screenDC.GetDeviceCaps(LOGPIXELSY));
		pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	}

	m_pOrigFont = pDC->SelectObject(&m_Font);
	m_OrigTextColor = pDC->SetTextColor(m_TextColor);
}

void CBaseGrid::ResetDC(CDC* pDC)
{
	pDC->SelectObject(m_pOrigFont);
	pDC->SetTextColor(m_OrigTextColor);
}

void CBaseGrid::Draw(CDC& origDC, int rowOffset, int columnOffset)
{
	CDC* pDC;
	CBitmap blitMap, *pOrigBitmap;
	
	CRect R;
	GetClientRect(&R);
	int nClientWidth = R.Width(), nClientHeight = R.Height();
	int nClipWidth, nClipHeight;
	CRect clipRect;
	
	if (origDC.IsPrinting()){
		pDC = &origDC;
	} else {
		origDC.GetClipBox(&clipRect);
		nClipWidth = clipRect.Width();
		nClipHeight = clipRect.Height();

		pDC = new CDC;
		pDC->CreateCompatibleDC(&origDC);	
		blitMap.CreateCompatibleBitmap(&origDC, nClipWidth, nClipHeight);
		pOrigBitmap = pDC->SelectObject(&blitMap);
		pDC->SetWindowOrg(clipRect.left, clipRect.top);
		CBrush buttonFaceBrush(GetSysColor(COLOR_BTNFACE));
		pDC->FillRect(&clipRect, &buttonFaceBrush);
	}

	PrepareDC(pDC);

	if (pDC->IsPrinting())
	{
		// for printing
		R.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&R);
	}

	int nPixWidth = 1;
	int nPixHeight = 1;

	int nPix2xWidth = 2 * nPixWidth;
	int nPix2xHeight = 2 * nPixHeight;

	// calc bottom
	int bottom = 0;
	int fixedRowHeight = bottom = GetFixedRowHeight();
	for (int i = m_nFixedRows + rowOffset; i < GetRowCount(); i++)
	{
		bottom += GetRowHeight(i);
		if (bottom >= R.bottom) break;
	}                                
	int maxVisibleRow = min(i, GetRowCount() - 1);
	
	// calc right
	int right = 0;
	int fixedColWidth = right = GetFixedColWidth();
	for (i = m_nFixedCols + columnOffset; i < GetColCount(); i++)
	{
		right += GetColWidth(i);
		if (right >= R.right) break;
	}
	int maxVisibleCol = min(i, GetColCount() - 1);
	// create range of visible cells
	CCellRange visibleRange(rowOffset + m_nFixedRows, columnOffset + m_nFixedCols, 
		maxVisibleRow, maxVisibleCol);

	int maxPrintableRow = maxVisibleRow;
	int maxPrintableCol = maxVisibleCol;
	if (pDC->IsPrinting())
	{
		int x = GetColStartX(maxPrintableCol, columnOffset);
		if (x + GetColWidth(maxPrintableCol) > R.right)
			maxPrintableCol--;
		x = GetColStartX(maxPrintableCol, columnOffset) + GetColWidth(maxPrintableCol);

		int y = GetRowStartY(maxPrintableRow, rowOffset);
		if (y + GetRowHeight(maxPrintableRow) > R.right)
			maxPrintableRow--;
		y = GetRowStartY(maxPrintableRow, rowOffset) + GetRowHeight(maxPrintableRow);



		// fill in buttons
		CBrush brush;
		brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		CRect r(0, 0, x, GetFixedRowHeight());
		pDC->FillRect(&r, &brush);
		r.right = GetFixedColWidth();
		r.bottom = y;
		pDC->FillRect(&r, &brush);

		maxVisibleCol = maxPrintableCol;
		maxVisibleRow = maxPrintableRow;

		right = x;
		bottom = y;
	}

	R = GetCellRangeRect(visibleRange, rowOffset, columnOffset);
	CBrush whiteBrush;
	whiteBrush.CreateStockObject(WHITE_BRUSH);
	pDC->FillRect(&R, &whiteBrush);
	
	
	// brush fill hilighted cells
	CCellRange visibleSelectedRange = m_SelectedCellRange.Intersect(visibleRange);
	/*
	if (visibleSelectedRange.IsValid())
	{  
		CRect vsrRect = GetCellRangeRect(visibleSelectedRange, rowOffset, columnOffset); // visible selected rect
		CBrush cyanBrush(RGB(0,255,255));
		pDC->FillRect(&vsrRect, &cyanBrush);
	}
	*/
	
	// draw top and left lines
	pDC->MoveTo(0,0);
	pDC->LineTo(right, 0);
	pDC->MoveTo(0,0);
	pDC->LineTo(0,bottom);
	
	// draw vertical lines           
	// fixed followed by variable
	
	// fixed
	int x = 0;
	for(i = 0; i < m_nFixedCols; i++)
	{
		x += GetColWidth(i);
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, bottom);   
	}                       
	// variable
	for (i = m_nFixedCols + columnOffset; i <= maxVisibleCol; i++)
	{
		x += GetColWidth(i);
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, bottom);   
	}
	
	
	// draw horizontal lines
	// fixed followed by variable
	
	//fixed
	int y = 0; 
	for (i = 0; i < m_nFixedRows; i++)
	{
		y += GetRowHeight(i);
		pDC->MoveTo(0,y);
		pDC->LineTo(right, y);
	}
	// variable
	for ( i = m_nFixedRows + rowOffset; i <= maxVisibleRow; i++)
	{
		y += GetRowHeight(i);
		pDC->MoveTo(0,y);
		pDC->LineTo(right, y);
	}
	
	// draw 3D button highlights
	
	CPen* oldPen = (CPen*) pDC->SelectStockObject(WHITE_PEN);
	
	// draw upper-left corner
	x = nPixWidth;
	for(i= 0; i < m_nFixedCols; i++)
	{   
		y = nPixHeight;
		for(int j = 0; j < m_nFixedRows; j++)
		{
			pDC->MoveTo(x,y);
			pDC->LineTo(x + GetColWidth(i) - nPix2xWidth, y);
			pDC->MoveTo(x,y);
			pDC->LineTo(x, y + GetRowHeight(j) - nPix2xHeight);
			y += GetRowHeight(j);
		}
		x += GetColWidth(i);
	}
	
	
	// vertical buttons
	x = nPixWidth;
	for (i = 0; i < m_nFixedCols; i++)
	{
		y = fixedRowHeight + nPixHeight;
		for (int j = m_nFixedRows + rowOffset; j <= maxVisibleRow; j++)
		{
			pDC->MoveTo(x,y);
			pDC->LineTo(x + GetColWidth(i) - nPix2xWidth, y);
			pDC->MoveTo(x,y);
			pDC->LineTo(x, y + GetRowHeight(j) - nPix2xHeight);
			y += GetRowHeight(j);
		}
		x += GetColWidth(i);
	}
			
	// highlight horizontal buttons        
	y = nPixHeight;
	for (i = 0; i < m_nFixedRows; i++)
	{
		x = fixedColWidth + nPixWidth;
		for (int j = m_nFixedCols + columnOffset; j <= maxVisibleCol; j++)
		{
			pDC->MoveTo(x,y);
			pDC->LineTo(x + GetColWidth(j) - nPix2xWidth, y);
			pDC->MoveTo(x,y);
			pDC->LineTo(x, y + GetRowHeight(i) - nPix2xHeight);
			x += GetColWidth(j);
		}
		y += GetRowHeight(i);
	}
	
	pDC->SelectObject(oldPen);
	
	// draw individual cells
	pDC->SetBkMode(TRANSPARENT);

	// draw top-left cells (0,0)
	x = m_nCellBorderThickness * nPixWidth;
	for (i = 0; i < m_nFixedCols; i++)
	{                         
		y = m_nCellBorderThickness * nPixHeight;
		for (int j = 0; j < m_nFixedRows; j++)
		{
			DrawCell(j, i, pDC, x, y);
			y += GetRowHeight(j);
		}
		x += GetColWidth(i);
	}
	 
	// draw fixed column cells  1..n
	x = m_nCellBorderThickness * nPixWidth;;
	for (i = 0; i < m_nFixedCols; i++)
	{
		y = GetFixedRowHeight() + m_nCellBorderThickness * nPixHeight;
		for (int j = rowOffset + m_nFixedRows; j <= maxVisibleRow; j++)
		{
			DrawCell(j,i, pDC, x, y);
			y += GetRowHeight(j);
		}
		x += GetColWidth(i);
	}
	
	// draw fixed row cells  1..n
	//x = GetColWidth(0) + m_nCellBorderThickness;
	x = GetFixedColWidth() + m_nCellBorderThickness * nPixWidth;
	for (i = columnOffset + m_nFixedCols; i <= maxVisibleCol; i++)
	{                                       
		y = m_nCellBorderThickness * nPixHeight;
		for (int j = 0; j < m_nFixedRows; j++)
		{
			DrawCell(j,i, pDC, x, y);
			y += GetRowHeight(j);
		}
		x += GetColWidth(i);
	}
	
	
	// draw rest of non-fixed cells
	/*
	x = fixedColWidth + m_nCellBorderThickness * nPixWidth;
	for (i = columnOffset + m_nFixedCols; i <= maxVisibleCol; i++)
	{
		y = fixedRowHeight + m_nCellBorderThickness * nPixHeight;
		for (int j = rowOffset + m_nFixedRows; j <= maxVisibleRow; j++)
		{
			DrawCell(j,i, pDC, x, y);
			y += GetRowHeight(j);
		}
		x += GetColWidth(i);
	}
	*/

	y = fixedRowHeight + m_nCellBorderThickness * nPixHeight;
	for (int j = rowOffset + m_nFixedRows; j <= maxVisibleRow; j++)
	{
		x = fixedColWidth + m_nCellBorderThickness * nPixWidth;
		for (i = columnOffset + m_nFixedCols; i <= maxVisibleCol; i++)
		{
			DrawCell(j,i, pDC, x, y);
			x += GetColWidth(i);
		}
		
		y += GetRowHeight(j);
	}
	
	if (m_bEditMode && IsSelectedCellVisible())
	{
		DrawEditText();
	}
	
	if (visibleSelectedRange.IsValid() && !pDC->IsPrinting())
	{  
		CRect vsrRect = GetCellRangeRect(visibleSelectedRange, rowOffset, columnOffset); // visible selected rect
		//CBrush cyanBrush(RGB(0,255,255));
		//pDC->FillRect(&vsrRect, &cyanBrush);
		pDC->InvertRect(&vsrRect);
	}

	ResetDC(pDC);
	if (!pDC->IsPrinting()){
		origDC.BitBlt(clipRect.left, clipRect.top, nClipWidth, nClipHeight, pDC, clipRect.left, clipRect.top, SRCCOPY);
		pDC->SelectObject(pOrigBitmap);
		delete pDC;
	}
	
}

void CBaseGrid::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	int columnOffset = GetScrollPos(SB_HORZ);
	int rowOffset = GetScrollPos(SB_VERT);
	Draw(dc, rowOffset, columnOffset);
}

int CBaseGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CBaseGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
	int scrollPos = GetScrollPos(SB_HORZ);
	switch (nSBCode)
	{
		case SB_LINERIGHT:
			if (scrollPos != m_nHScrollMax)
			{
				SetScrollPos(SB_HORZ, scrollPos + 1);
				int xScroll = -GetColWidth(scrollPos + m_nFixedCols);
				CRect R;
				GetClientRect(&R);
				
				R.left = GetFixedColWidth() - xScroll;
				ScrollWindow(xScroll, 0, &R);
				R.left = R.right + xScroll;
				InvalidateRect(&R);
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(xScroll, 0);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_PAGERIGHT:
			if (scrollPos != m_nHScrollMax)
			{
				CRect R;
				GetClientRect(&R);
				CCellRange visCells = GetVisibleNonFixedCellRange();
				int offset = visCells.GetMaxCol() - visCells.GetMinCol();
				int pos = min(m_nHScrollMax, scrollPos + offset);
				int xOffset = -(GetColStartX(pos + m_nFixedCols) - GetColStartX(scrollPos + m_nFixedCols));
				SetScrollPos(SB_HORZ, pos);
				R.left = GetFixedColWidth();
				InvalidateRect(&R);
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(xOffset, 0);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_LINELEFT:
			if (scrollPos != 0)
			{
				SetScrollPos(SB_HORZ, scrollPos - 1);
				int xScroll = GetColWidth(scrollPos - 1 + m_nFixedCols);
				CRect R;
				GetClientRect(&R);

				R.left = GetFixedColWidth();
				BOOL bEditVisible = GetEditControl() && GetEditControl()->IsWindowVisible();
				ScrollWindowEx(xScroll, 0, &R, 0, 0, 0, SW_ERASE|SW_INVALIDATE|(bEditVisible ? SW_SCROLLCHILDREN : 0));
				UpdateWindow();
				if (GetEditControl() && !bEditVisible){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(xScroll, 0);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_PAGELEFT:
			if (scrollPos != 0)
			{
				CRect R;
				GetClientRect(&R);
				int scrollableWidth = R.right - GetFixedColWidth();
				CCellRange visCells = GetVisibleNonFixedCellRange();
				int i, j;
				for (i = scrollPos, j = visCells.GetMinCol() - 1; i >= 0 && j >= 0; i--, j--)
				{
					scrollableWidth -= GetColWidth(j);
					if (scrollableWidth <= 0) break;
				}
				int pos = max(0, i);
				SetScrollPos(SB_HORZ, pos);
				R.left = GetFixedColWidth();
				InvalidateRect(&R);
				int xOffset = -(GetColStartX(pos + m_nFixedCols) - GetColStartX(scrollPos + m_nFixedCols));
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(xOffset, 0);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				CRect R;
				GetClientRect(&R);
				SetScrollPos(SB_HORZ, nPos);
				R.left = GetFixedColWidth();
				InvalidateRect(&R);
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					int xPos = GetColStartXAbsolute(m_SelectedCellRange.GetMinCol() + m_nFixedCols) - GetColStartXAbsolute(nPos + m_nFixedCols);
					GetEditControl()->SetWindowPos(0, xPos, r.top, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
				}
			}
			break;
		case SB_LEFT:
			if (scrollPos != 0)
			{
				SetScrollPos(SB_HORZ, 0);
				Invalidate();
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					int xPos = GetColStartXAbsolute(m_SelectedCellRange.GetMinCol() + m_nFixedCols) - GetColStartXAbsolute(0 + m_nFixedCols);
					GetEditControl()->SetWindowPos(0, xPos, r.top, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
				}
			}
			break;
		default:
			{
				//TRACE("Horizontal scroll case not handled\n");
			}
	}
}

void CBaseGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
	int scrollPos = GetScrollPos(SB_VERT);
	int scrollMin, scrollMax;
	GetScrollRange(SB_VERT, &scrollMin, &scrollMax);
	switch (nSBCode)
	{
		case SB_LINEDOWN:
			if (scrollPos != m_nVScrollMax)
			{
				SetScrollPos(SB_VERT, scrollPos + 1);
				int yScroll = -GetRowHeight(scrollPos + m_nFixedRows);
				CRect R;
				GetClientRect(&R);
				
				R.top = GetFixedRowHeight() + GetRowHeight(scrollPos + m_nFixedRows);
				ScrollWindow( 0, yScroll, &R);
				R.top = R.bottom + yScroll;
				InvalidateRect(&R);
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(0, yScroll);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_PAGEDOWN:
			if (scrollPos != m_nVScrollMax)
			{
				CRect R;
				GetClientRect(&R);
				CCellRange visCells = GetVisibleNonFixedCellRange();
				int offset = visCells.GetMaxRow() - visCells.GetMinRow();
				int origPos = scrollPos;
				if (offset == 0)
				{
					offset++;
				}
				scrollPos += offset;        
				if (scrollPos > m_nVScrollMax)
				{
					scrollPos = m_nVScrollMax;
				}
				int yOffset = - (GetRowStartY(scrollPos + m_nFixedRows) - GetRowStartY(origPos + m_nFixedRows));
				SetScrollPos(SB_VERT, scrollPos);
				R.top = GetFixedRowHeight();
				InvalidateRect(&R);
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(0, yOffset);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_LINEUP:
			if (scrollPos != 0)
			{
				SetScrollPos(SB_VERT, scrollPos - 1);
				int yScroll = GetRowHeight(scrollPos - 1 + m_nFixedRows);
				CRect R;
				GetClientRect(&R);
				
				R.top = GetFixedRowHeight();
				BOOL bEditVisible = GetEditControl() && GetEditControl()->IsWindowVisible();
				ScrollWindowEx(0, yScroll, &R, 0, 0, 0, SW_ERASE|SW_INVALIDATE|(bEditVisible ? SW_SCROLLCHILDREN : 0));
				UpdateWindow();

				if (GetEditControl() && !bEditVisible){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(0, yScroll);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_PAGEUP:
			if (scrollPos != 0)
			{
				CRect R;
				GetClientRect(&R);
				int scrollableHeight = R.bottom - GetFixedRowHeight();
				CCellRange visCells = GetVisibleNonFixedCellRange();
				int i, j;
				for (i = scrollPos, j = visCells.GetMinRow() - 1; i >= 0 && j >= 0; i--, j--)
				{
					scrollableHeight -= GetRowHeight(j);
					if (scrollableHeight <= 0) break;
				}
				int pos = max(0, i);
				SetScrollPos(SB_VERT, pos);
				R.top = GetFixedRowHeight();
				InvalidateRect(&R);
				int yOffset =  - (GetRowStartY(pos + m_nFixedRows) - GetRowStartY(scrollPos + m_nFixedRows));
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					r.OffsetRect(0, yOffset);
					GetEditControl()->MoveWindow(&r, FALSE);
				}
			}
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				CRect R;
				GetClientRect(&R);
				SetScrollPos(SB_VERT, nPos);
				R.top = GetFixedRowHeight();
				InvalidateRect(&R);
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					int yPos = GetRowStartYAbsolute(m_SelectedCellRange.GetMinRow() + m_nFixedRows) - GetRowStartYAbsolute(nPos + m_nFixedRows);
					GetEditControl()->SetWindowPos(0, r.left, yPos, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
				}
			}
			break;
		case SB_TOP:
			if (scrollPos != 0)
			{
				SetScrollPos(SB_VERT, 0);
				Invalidate();
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					int yPos = GetRowStartYAbsolute(m_SelectedCellRange.GetMinRow() + m_nFixedRows) - GetRowStartYAbsolute(0 + m_nFixedRows);
					GetEditControl()->SetWindowPos(0, r.left, yPos, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
				}
			}
			break;
		case SB_BOTTOM:
			if (scrollPos != scrollMax)
			{
				SetScrollPos(SB_VERT, scrollMax);
				Invalidate();
				if (GetEditControl()){
					CRect r;
					GetEditControl()->GetWindowRect(&r);
					ScreenToClient(&r);
					int yPos = GetRowStartYAbsolute(m_SelectedCellRange.GetMinRow() + m_nFixedRows) - GetRowStartYAbsolute(scrollMax + m_nFixedRows);
					GetEditControl()->SetWindowPos(0, r.left, yPos, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
				}
			}
		default:
			{
				//TRACE("Vertical scroll case not handled\n");
			}
	}
}

void CBaseGrid::OnSelectingCols(CCellIndices currentCell)
{
	int col = currentCell.col;
	if (col == m_MouseMoveCell.col) return;
	if (col < m_nFixedCols) return;
	InvalidateCellRect(m_SelectedCellRange);
	if (col > m_LeftClickDownCell.col)
	{
		m_SelectedCellRange.SetMinCol(m_LeftClickDownCell.col);
		m_SelectedCellRange.SetMaxCol(col);
	}
	else
	{
		m_SelectedCellRange.SetMinCol(col);
		m_SelectedCellRange.SetMaxCol(m_LeftClickDownCell.col);
	}
	InvalidateCellRect(m_SelectedCellRange);
}


void CBaseGrid::OnSelectingRows(CCellIndices currentCell)
{  
	int row = currentCell.row;
	if (row == m_MouseMoveCell.row) return;
	if (row < m_nFixedRows) return;
	InvalidateCellRect(m_SelectedCellRange);
	if (row > m_LeftClickDownCell.row)
	{
		m_SelectedCellRange.SetMinRow(m_LeftClickDownCell.row);
		m_SelectedCellRange.SetMaxRow(row);
	}
	else
	{
		m_SelectedCellRange.SetMinRow(row);
		m_SelectedCellRange.SetMaxRow(m_LeftClickDownCell.row);
	}
	InvalidateCellRect(m_SelectedCellRange);
}

void CBaseGrid::OnSelectingCells(CCellIndices currentCell)
{
	int row = currentCell.row;
	int col = currentCell.col;
	if (row < m_nFixedRows || col < m_nFixedCols) return;
	if (currentCell == m_MouseMoveCell) return;
	InvalidateCellRect(m_SelectedCellRange);
	if (currentCell.IsDownRightOf(m_LeftClickDownCell))
	{
		m_SelectedCellRange.SetMinRow(m_LeftClickDownCell.row);
		m_SelectedCellRange.SetMaxRow(row);
		m_SelectedCellRange.SetMinCol(m_LeftClickDownCell.col);
		m_SelectedCellRange.SetMaxCol(col);
	}
	else if (currentCell.IsDownLeftOf(m_LeftClickDownCell))
	{
		m_SelectedCellRange.SetMinRow(m_LeftClickDownCell.row);
		m_SelectedCellRange.SetMaxRow(row);
		m_SelectedCellRange.SetMinCol(col);
		m_SelectedCellRange.SetMaxCol(m_LeftClickDownCell.col);
	}
	else if (currentCell.IsUpRightOf(m_LeftClickDownCell))
	{
		m_SelectedCellRange.SetMinRow(row);
		m_SelectedCellRange.SetMaxRow(m_LeftClickDownCell.row);
		m_SelectedCellRange.SetMinCol(m_LeftClickDownCell.col);
		m_SelectedCellRange.SetMaxCol(col);
	}
	else if (currentCell.IsUpLeftOf(m_LeftClickDownCell))
	{
		m_SelectedCellRange.SetMinRow(row);
		m_SelectedCellRange.SetMaxRow(m_LeftClickDownCell.row);
		m_SelectedCellRange.SetMinCol(col);
		m_SelectedCellRange.SetMaxCol(m_LeftClickDownCell.col);
	}
	InvalidateCellRect(m_SelectedCellRange);
}

void CBaseGrid::OnSelecting(const CCellIndices & currentCell)
{
	switch(m_MouseMode)
	{
	case SelectingCol:
		OnSelectingCols(currentCell);
		break;
	case SelectingRow:
		OnSelectingRows(currentCell);
		break;
	case SelectingCells:
		OnSelectingCells(currentCell);
		break;
	}
}


void CBaseGrid::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	CCellIndices currentCell = GetCellIndices(point);
	CRect clientRect;
	GetClientRect(&clientRect);
	if (!clientRect.PtInRect(point))
		return;
	if (m_MouseMode == Nothing)
	{
		int xLogSlop = slop;
		int yLogSlop = slop;
		if (currentCell.row < m_nFixedRows && currentCell.IsValid())
		{
			int startx = GetColStartX(currentCell.col);
			int endx = startx + GetColWidth(currentCell.col);
			if ((point.x - startx <= slop && currentCell.col != 0) || endx - point.x <= slop)
			{
				HCURSOR cursor = ::LoadCursor(0,IDC_SIZEWE);
				SetCursor(cursor);
			}
			else
			{
				SetCursor(::LoadCursor(0, IDC_ARROW));
			}
		}
		else if (currentCell.col < m_nFixedCols && currentCell.IsValid())
		{
			int starty = GetRowStartY(currentCell.row);
			int endy = starty + GetRowHeight(currentCell.row);
			if ((point.y - starty <= slop && currentCell.row != 0) || endy - point.y <= slop)
			{
				SetCursor(::LoadCursor(0, IDC_SIZENS));
			}
			else
			{
				SetCursor(::LoadCursor(0, IDC_ARROW));
			}
		}
		else
		{
			SetCursor(::LoadCursor(0, IDC_ARROW));
		}
		m_LastMousePoint = point;
		return;
	}
	if (!m_LeftClickDownCell.IsValid())
	{
		m_LastMousePoint = point;
		return;
	}
	switch(m_MouseMode)
	{
		case SelectingAll:
			break;
		case SelectingCol:
		case SelectingRow:
		case SelectingCells:
			OnSelecting(currentCell);
			break;
		case SizingColumn:
			{
				CRect R;
				GetClientRect(&R);
				CDC* pDC = GetDC();
				CRect oldInvertedRect(m_LastMousePoint.x, R.top, m_LastMousePoint.x + 2, R.bottom);
				pDC->InvertRect(&oldInvertedRect);
				CRect newInvertedRect(point.x, R.top, point.x + 2, R.bottom);
				pDC->InvertRect(&newInvertedRect);
				ReleaseDC(pDC);
			}
			break;
		case SizingRow:
			{
				CRect R;
				GetClientRect(&R);
				CDC* pDC = GetDC();
				CRect oldInvertedRect(R.left, m_LastMousePoint.y, R.right, m_LastMousePoint.y + 2);
				pDC->InvertRect(&oldInvertedRect);
				CRect newInvertedRect(R.left, point.y, R.right, point.y + 2);
				pDC->InvertRect(&newInvertedRect);
				ReleaseDC(pDC);
			}
			break;
	}
	m_MouseMoveCell = currentCell;
	m_LastMousePoint = point;
}

void CBaseGrid::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (!StopEdit()) return; // could not validate data
	SetFocus();
	SetCapture();
	m_LeftClickDownCell = GetCellIndices(point);
	if (GetCursor() == LoadCursor(0, IDC_SIZEWE)) // sizing column
	{
		int xLogSlop = slop;
		m_MouseMode = SizingColumn;
		int startx = GetColStartX(m_LeftClickDownCell.col);
		CRect R;
		GetClientRect(&R);
		CRect invertedRect(point.x, R.top, point.x + 2, R.bottom);
		CDC* pDC = GetDC();
		pDC->InvertRect(&invertedRect);
		ReleaseDC(pDC);
		if (point.x - startx <= slop) // clicked right of border
		{
			R.left = GetColStartX(--m_LeftClickDownCell.col);
		}
		else
		{
			R.left = startx;
		}
		ClientToScreen(&R);
		ClipCursor(&R);
	}
	else if (GetCursor() == LoadCursor(0, IDC_SIZENS)) // sizing row
	{
		m_MouseMode = SizingRow;
		int starty = GetRowStartY(m_LeftClickDownCell.row);
		CRect R;
		GetClientRect(&R);
		CRect invertedRect(R.left, point.y, R.right, point.y + 2);
		CDC* pDC = GetDC();
		pDC->InvertRect(&invertedRect);
		ReleaseDC(pDC);
		if (point.y - starty <= slop) // clicked below border
		{
			R.top = GetRowStartY(--m_LeftClickDownCell.row);
		}
		else
		{
			R.top = starty;
		}
		ClientToScreen(&R);
		ClipCursor(&R);
	}
	else // not sizing -- selecting
	{
		if (!m_LeftClickDownCell.IsValid()) return;
	
		if (m_SelectedCellRange.IsValid())
		{
				InvalidateCellRect(m_SelectedCellRange);
		}
		if (IsTopLeftCell(m_LeftClickDownCell) && m_nFixedRows && m_nFixedCols)
		{
			m_SelectedCellRange.Set(m_nFixedRows, m_nFixedCols, GetRowCount() - 1, GetColCount() - 1);
			m_MouseMode = SelectingAll;
		}
		else if (IsTopFixedCell(m_LeftClickDownCell))
		{
			m_SelectedCellRange.Set(m_nFixedRows, m_LeftClickDownCell.col, GetRowCount() - 1, m_LeftClickDownCell.col);
			m_MouseMode = SelectingCol;
		}
		else if (IsLeftFixedCell(m_LeftClickDownCell))
		{
			m_SelectedCellRange.Set(m_LeftClickDownCell.row, m_nFixedCols, m_LeftClickDownCell.row, GetColCount() - 1);
			m_MouseMode = SelectingRow;
		}
		else
		{
			m_MouseMode = SelectingCells;
			if (m_bListMode)
			{
				m_SelectedCellRange.Set(m_LeftClickDownCell.row, m_nFixedCols,
					m_LeftClickDownCell.row, m_nCols - 1);
				m_MouseMode = SelectingRow;
			}
			else
			{
				m_SelectedCellRange = CCellRange(m_LeftClickDownCell, m_LeftClickDownCell);
			}
		}
		InvalidateCellRect(m_SelectedCellRange);
		m_nTimerID = SetTimer(WM_LBUTTONDOWN, 200, 0);
	}   
	m_LastMousePoint = point;
}

void CBaseGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
	if (GetCapture() == this){
		ReleaseCapture();
#ifndef WIN32
		OnCaptureChanged(0);// WM_CAPTURECHANGED is only Win32
#endif
	}
	ClipCursor(0);
	if (m_MouseMode == SizingColumn)
	{
		CRect R;
		GetClientRect(&R);
		CRect invertedRect(m_LastMousePoint.x, R.top, m_LastMousePoint.x + 2, R.bottom);
		CDC* pDC = GetDC();
		pDC->InvertRect(&invertedRect);
		ReleaseDC(pDC);
		int startx = GetColStartX(m_LeftClickDownCell.col);
		SetColWidth(m_LeftClickDownCell.col, point.x - startx);
		ResetScrollBars();
		Invalidate();
	}
	else if (m_MouseMode == SizingRow)
	{
		CRect R;
		GetClientRect(&R);
		CRect invertedRect(R.left, m_LastMousePoint.y, R.right, m_LastMousePoint.y + 2);
		CDC* pDC = GetDC();
		pDC->InvertRect(&invertedRect);
		ReleaseDC(pDC);
		int starty = GetRowStartY(m_LeftClickDownCell.row);
		SetRowHeight(m_LeftClickDownCell.row, point.y - starty);
		ResetScrollBars();
		Invalidate();
	}
	m_MouseMode = Nothing;
	if (!m_LeftClickDownCell.IsValid()) return;
	m_LeftClickUpCell = GetCellIndices(point);
	#ifdef WIN32
		GetParent()->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM) m_hWnd);
	#else
		GetParent()->PostMessage(WM_COMMAND, GetDlgCtrlID(), MAKELONG(m_hWnd, BN_CLICKED));
	#endif
}

void CBaseGrid::OnCaptureChanged(CWnd* pWnd)
{
	if (pWnd != this && m_nTimerID != 0){
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
}

void CBaseGrid::OnTimer(UINT nIDEvent)
{
	ASSERT(nIDEvent == WM_LBUTTONDOWN);
	CPoint origPt;
	GetCursorPos(&origPt);
	ScreenToClient(&origPt);

	CRect r;
	GetClientRect(&r);

	CPoint pt = origPt;

	int nFixedRowHeight = GetFixedRowHeight();
	int nFixedColWidth = GetFixedColWidth();
	if (pt.y > r.bottom){
		SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		if (pt.x < r.left)
			pt.x = r.left;
		if (pt.x > r.right)
			pt.x = r.right;
		pt.y = r.bottom;
		OnSelecting(GetCellIndices(pt));
	} else if (pt.y < nFixedRowHeight){
		SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
		if (pt.x < r.left)
			pt.x = r.left;
		if (pt.x > r.right)
			pt.x = r.right;
		pt.y = nFixedRowHeight + 1;
		OnSelecting(GetCellIndices(pt));
	}

	pt = origPt;
	if (pt.x > r.right){
		SendMessage(WM_HSCROLL, SB_PAGEDOWN, 0);
		if (pt.y < r.top)
			pt.y = r.top;
		if (pt.y > r.bottom)
			pt.y = r.bottom;
		pt.x = r.right;
		OnSelecting(GetCellIndices(pt));
	} else if (pt.x < nFixedColWidth){
		SendMessage(WM_HSCROLL, SB_PAGEUP, 0);
		if (pt.y < r.top)
			pt.y = r.top;
		if (pt.y > r.bottom)
			pt.y = r.bottom;
		pt.x = nFixedColWidth + 1;
		OnSelecting(GetCellIndices(pt));
	}
}

void CBaseGrid::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (!GetCellIndices(point).IsValid()) return;
	#ifdef WIN32
		GetParent()->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_DOUBLECLICKED), (LPARAM) m_hWnd);
	#else
		GetParent()->PostMessage(WM_COMMAND, GetDlgCtrlID(), MAKELONG(m_hWnd, BN_DOUBLECLICKED));
	#endif
}


void CBaseGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	ResetScrollBars();
}
 
CRect CBaseGrid::GetCellRangeRect(const CCellRange& cellRange, int rowOffset, int colOffset)
{
	int left = GetColStartX(cellRange.GetMinCol(), colOffset);
	int top = GetRowStartY(cellRange.GetMinRow(), rowOffset);
	int right = GetColStartX(cellRange.GetMaxCol(), colOffset) + GetColWidth(cellRange.GetMaxCol());
	int bottom = GetRowStartY(cellRange.GetMaxRow(), rowOffset) + GetRowHeight(cellRange.GetMaxRow());
	return CRect(left, top, right, bottom);
}

CRect CBaseGrid::GetCellRangeRect(const CCellRange& cellRange)
{  
	return GetCellRangeRect(cellRange, GetScrollPos(SB_VERT), GetScrollPos(SB_HORZ));
} 
 
void CBaseGrid::InvalidateCellRect(const CCellRange& cellRange)
{
	ASSERT(cellRange.IsValid());
	CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);
	CRect visibleRect = GetCellRangeRect(visibleCellRange);
	InvalidateRect(&visibleRect);
}

int CBaseGrid::IsTopLeftCell(const CCellIndices& cellIndices)
{
	return (cellIndices.row == 0 && cellIndices.col == 0);
}

int CBaseGrid::IsTopFixedCell(const CCellIndices& cellIndices)
{
	return ((cellIndices.IsValid())  && (cellIndices.row < m_nFixedRows));
}

int CBaseGrid::IsLeftFixedCell(const CCellIndices& cellIndices)
{
	return ((cellIndices.IsValid()) && (cellIndices.col < m_nFixedCols));
}        


WNDPROC* CBaseGrid::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper = NULL;
	return &pfnSuper;
}

LONG FAR PASCAL CBaseGrid::DummyWndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg){
	case WM_PAINT:
		{
#ifdef _DEBUG
			PAINTSTRUCT ps;
			HDC hDC = ::BeginPaint(hwnd, &ps);
			CString s(_T("Subclass!, Subclass!, Subclass!"));
			::TextOut(hDC, 0, 0, s, s.GetLength());
			::EndPaint(hwnd, &ps);
			TRACE(_T("You need to subclass this window\n"));
#endif
		}
		return 0;
		break;
	}
	return ::DefWindowProc(hwnd, msg, w, l);
}

void CBaseGrid::RegisterClass()
{
	if (m_atomClassName == 0){
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		
		wc.style = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW|CS_GLOBALCLASS;
		wc.lpfnWndProc = DummyWndProc;
		wc.hInstance = AfxGetInstanceHandle();
		wc.hCursor = 0;
		wc.lpszClassName = szClassName;
		wc.hbrBackground = 0;//(HBRUSH) (COLOR_BTNFACE + 1);
		
		m_atomClassName = ::RegisterClass(&wc);
		ASSERT(m_atomClassName);
	}
}

void CBaseGrid::AutoSizeCol(int col)
{
	CDC* pDC = GetDC();
	PrepareDC(pDC);
	int width = 0;
	CSize size;
	for (int i = 0; i < GetRowCount(); i++)
	{
		size = GetCellExtent(i, col, pDC);
		if (size.cx > width)
		{
			width = size.cx;
		}
	}  
	m_auColWidths[col] = width + 2 * m_nCellBorderThickness;
	ResetDC(pDC);
	ReleaseDC(pDC);
	ResetScrollBars();
}

void CBaseGrid::AutoSizeRow(int row)
{
	CDC* pDC = GetDC();
	PrepareDC(pDC);
	int height = 0;
	CSize size;
	for (int i = 0; i < GetColCount(); i++)
	{  
		size = GetCellExtent(row, i, pDC);
		if (size.cy > height)
		{
			height = size.cy;
		}
	}  
	m_auRowHeights[row] = height + 2 * m_nCellBorderThickness;
	ResetDC(pDC);
	ReleaseDC(pDC);
	ResetScrollBars();
}

void CBaseGrid::AutoSizeAllCols()
{
	for (int col = 0; col < GetColCount(); col++)
	{
		AutoSizeCol(col);
	}
}

void CBaseGrid::AutoSizeAllRows()
{
	for (int row = 0; row < GetRowCount(); row++)
	{
		AutoSizeRow(row);
	}
}
					
// sizes all rows and columns
// faster than calling both AutoSizeAllCols() and AutoSizeAllRows()
void CBaseGrid::AutoSize()
{
	CDC* pDC = GetDC();
	PrepareDC(pDC);
	int colCount = GetColCount();
	int rowCount = GetRowCount();
	
	// initialize col widths to zero
	for (int i = 0; i < colCount; i++)
	{
		m_auColWidths[i] = 0;
	}
	
	// initialize row heights to zero
	for (int j = 0; j < rowCount; j++)
	{
		m_auRowHeights[j] = 0;
	}
	
	CSize size;
	for (i = 0; i < colCount; i++)
	{
		for (j = 0; j < rowCount; j++)
		{
			size = GetCellExtent(j, i, pDC);
			if (size.cx > (int) m_auColWidths[i])
			{
				m_auColWidths[i] = size.cx;
			}
			if (size.cy > (int) m_auRowHeights[j])
			{
				m_auRowHeights[j] = size.cy;
			}
		}
	}
	
	for (i = 0; i < colCount; i++)
	{
		m_auColWidths[i] += 2 * m_nCellBorderThickness;
	}
	
	for (j = 0; j< rowCount; j++)
	{
		m_auRowHeights[j] += 2 * m_nCellBorderThickness;
	}
	ResetDC(pDC);
	ReleaseDC(pDC);
	ResetScrollBars();
	if (GetSafeHwnd())
	{
		Invalidate();
	}
}
					
void CBaseGrid::DrawCell(int row, int col, CDC* pDC, int x, int y)
{
	if (m_bConstrained)
	{
		CRect R(x, y, x + m_auColWidths[col] - 2 * m_nCellBorderThickness,
			y + m_auRowHeights[row] - 2 * m_nCellBorderThickness);
		pDC->DrawText(_T("base"), -1, &R, DT_CENTER);
	}
	else
	{  
		pDC->TextOut(x, y, _T("base"), 4);
	}
}

CCellRange CBaseGrid::GetCellRange()
{
	return CCellRange(0, 0, GetRowCount() - 1, GetColCount() - 1);
}

void CBaseGrid::ResetSelectedRange()
{
	m_SelectedCellRange = m_SelectedCellRange.Intersect(GetCellRange());
	if (!m_SelectedCellRange.IsValid())
	{
		m_SelectedCellRange = CCellRange(m_nFixedRows, m_nFixedCols, m_nFixedRows, m_nFixedCols);
	}
}

void CBaseGrid::GetFont(LOGFONT& logFont)
{
	m_Font.GetObject(sizeof(LOGFONT), &logFont);
}

void CBaseGrid::SetFont(const LOGFONT& logFont)
{
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&logFont);
}

void CBaseGrid::SetFont(const char* facename)
{
	LOGFONT lf;
	GetFont(lf);
	strcpy(lf.lfFaceName, facename);
	SetFont(lf);
}

COLORREF CBaseGrid::GetTextColor()
{
	return m_TextColor;
}

void CBaseGrid::SetTextColor(COLORREF textColor)
{
	m_TextColor = textColor;
}

void CBaseGrid::ExpandToFit()
{
	CRect R;
	GetClientRect(&R);
	long virtualWidth = GetVirtualWidth();
	if (virtualWidth < R.Width())
	{
		int difference = R.Width() - (int) virtualWidth -1  ; // -1 for padding
		int additionalColWidth = difference / GetColCount();
		for (int i = 0; i < GetColCount(); i++)
		{
			m_auColWidths[i] += additionalColWidth;
		}
		int leftOver = difference % GetColCount();
		for (i = 0; i < leftOver; i++)
		{
			m_auColWidths[i] += 1;
		}
	}
	long virtualHeight = GetVirtualHeight();
	if (virtualHeight < R.Height())
	{
		int difference = R.Height() - (int) virtualHeight;
		int additionalRowHeight = difference / GetRowCount();
		for (int i = 0; i < GetRowCount(); i++)
		{
			m_auRowHeights[i] += additionalRowHeight;
		}
		int leftOver = difference % GetRowCount();
		for (i = 0; i < leftOver; i++)
		{
			m_auRowHeights[i] += 1;
		}
	}
	if (IsWindowVisible())
	{  
		Invalidate();
	}
}

UINT CBaseGrid::OnGetDlgCode()
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}

void CBaseGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{                        
	CCellIndices topLeft(m_SelectedCellRange.GetTopLeft());
	CCellIndices next(topLeft);
	switch (nChar)
	{
		case VK_DOWN:
			if (topLeft.row < (GetRowCount() - 1))
			{
				next.row += 1;
			}
			break;
		case VK_UP:
			if (topLeft.row > m_nFixedRows)
			{
				next.row -= 1;
			}
			break;
		case VK_RIGHT:
			if (topLeft.col < (GetColCount() - 1))
			{
				next.col += 1;
			}
			break;
		case VK_LEFT:
			if (topLeft.col > m_nFixedCols)
			{
				next.col -= 1;
			}
			break;
		case VK_NEXT: // same as page down
			{
				SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
				int hscroll = GetScrollPos(SB_HORZ);
				int vscroll = GetScrollPos(SB_VERT);
				m_SelectedCellRange.Set(m_nFixedRows + vscroll, m_SelectedCellRange.GetMinCol(), 
					m_nFixedRows + vscroll, m_SelectedCellRange.GetMinCol());
				Invalidate();
				return;
			}
			break;
		case VK_PRIOR:
			{
				SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
				int hscroll = GetScrollPos(SB_HORZ);
				int vscroll = GetScrollPos(SB_VERT);
				m_SelectedCellRange.Set(m_nFixedRows + vscroll, m_SelectedCellRange.GetMinCol(), 
					m_nFixedRows + vscroll, m_SelectedCellRange.GetMinCol());
				Invalidate();
				return;
			}
			break;
		case VK_HOME:
			{
				SendMessage(WM_VSCROLL, SB_TOP, 0);
				SendMessage(WM_HSCROLL, SB_TOP, 0);
				m_SelectedCellRange.Set(m_nFixedRows, m_nFixedCols, m_nFixedRows, m_nFixedCols);
				Invalidate();
				return;
			}
			break;
		case VK_END:
			{
				SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
				SendMessage(WM_HSCROLL, SB_TOP, 0);
				int vscroll = GetScrollPos(SB_VERT);
				m_SelectedCellRange.Set(m_nFixedRows + vscroll, m_nFixedCols,m_nFixedRows + vscroll, m_nFixedCols);
				Invalidate();
				return;
			}
			break;
		default:
			CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	if (next != topLeft)
	{
		CCellRange newSelectedRange(next,next);
		InvalidateCellRect(newSelectedRange.Union(m_SelectedCellRange));
		m_SelectedCellRange = newSelectedRange;
		if (!IsCellVisible(next))
		{
			switch (nChar)
			{
				case VK_RIGHT:
					PostMessage(WM_HSCROLL, SB_LINERIGHT, 0);
					break;
				case VK_LEFT:
					PostMessage(WM_HSCROLL, SB_LINELEFT, 0);
					break;
				case VK_DOWN:
					PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
					break;
				case VK_UP:
					PostMessage(WM_VSCROLL, SB_LINEUP, 0);
					break;
			}
		}
	}
}
				
int CBaseGrid::IsSelectedCellVisible()
{
	return IsCellVisible(m_SelectedCellRange.GetTopLeft());
}            
				
int CBaseGrid::IsCellVisible(CCellIndices cell)
{  
	int horzScrollPos = GetScrollPos(SB_HORZ);
	int vertScrollPos = GetScrollPos(SB_VERT);
	if (cell.col < m_nFixedCols + horzScrollPos)
	{
		return 0;
	} 
	else if (cell.row < m_nFixedRows + vertScrollPos)
	{
		return 0;
	}
	CRect R;
	GetClientPix(&R);
	int x = GetFixedColWidth();
	for (int i = m_nFixedCols + horzScrollPos; i <= cell.col; i++)
	{
		x += GetColWidth(i);
		if (x > R.right) break;
	}
	if (x > R.right)
	{
		return 0;
	}
	int y = GetFixedRowHeight();
	for (i = m_nFixedRows + vertScrollPos; i <= cell.row; i++)
	{
		y += GetRowHeight(i);
		if (y > R.bottom) break;
	}
	if (y > R.bottom)
	{
		return 0;
	}
	return 1;
}

void CBaseGrid::SetRowHeight(int row, int height)
{
	m_auRowHeights[row] = height;
}

void CBaseGrid::SetColWidth(int col, int width)
{
	m_auColWidths[col] = width;
}

CSize CBaseGrid::GetCellExtent(int /*row*/, int /*col*/, CDC* /*pDC*/)
{
	return CSize(m_nDefCellWidth, m_nDefCellHeight);
}

void CBaseGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL startedEdit = FALSE; // set this to TRUE if we are beginning to edit
	
	if (m_bEditEnabled && !m_bEditMode && (m_MouseMode == Nothing) && 
		m_SelectedCellRange.IsSingleCell() && IsCellVisible(m_SelectedCellRange.GetTopLeft()))
	{
		if (!m_bEditMode)
		{
			switch (nChar){
			case VK_ESCAPE:
				break;
			default:
				StartEdit(nChar, nFlags);
				startedEdit = TRUE;
				if (nChar != VK_BACK && nChar != VK_RETURN)
				{
					m_strCurrentEditData.Empty();
				}
			}
		}
	}
	
	if (m_bEditMode && IsSelectedCellVisible())
	{
		int length = max(0, m_strCurrentEditData.GetLength());
		switch (nChar)
		{
			case VK_TAB:
			case VK_RETURN:
				if (!startedEdit) 
					StopEdit();
				break;
			default: 
			{
				char* s = m_strCurrentEditData.GetBufferSetLength(++length);
				s[length - 1] = char(nChar);
				m_strCurrentEditData.ReleaseBuffer(length);
			} break;
		}
		DrawEditText();
	}
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CBaseGrid::EnableEdit(BOOL bEnableEdit /* = TRUE */)
{
	// can't edit files, when you can't select single cells
	ASSERT(!m_bListMode);
	m_bEditEnabled = bEnableEdit;
}

BOOL CBaseGrid::GetEditText(int /*row*/, int /*col*/, const CString& /*text*/)
{
	TRACE(_T("You must override this in your derived class\n"));
	ASSERT(0);
	return TRUE;
}

BOOL CBaseGrid::SetEditText(int /*row*/, int /*col*/, CString& text)
{
	TRACE(_T("You must override this in your derived class\n"));
	ASSERT(0);
	text = (_T("grid"));
	return TRUE;
}

void CBaseGrid::DrawEditText()
{
	return;
	ASSERT(m_SelectedCellRange.IsSingleCell());
	ASSERT(IsCellVisible(m_SelectedCellRange.GetTopLeft()));
	int row = m_SelectedCellRange.GetMinRow();
	int col = m_SelectedCellRange.GetMinCol();
	int x = GetColStartX(col);
	int y = GetRowStartY(row);
	int cx = GetColWidth(col);
	int cy = GetRowHeight(row);
	
	CRect R(x ,y, x + cx, y + cy);
	R.InflateRect(-m_nCellBorderThickness, -m_nCellBorderThickness);
	CDC* pDC = GetDC();
	PrepareDC(pDC);
	CBrush whiteBrush;
	whiteBrush.CreateStockObject(WHITE_BRUSH);

	
	//pDC->TextOut(x, y, m_strCurrentEditData, m_strCurrentEditData.GetLength());
	pDC->FillRect(&R, &whiteBrush);
	pDC->DrawText(m_strCurrentEditData, -1, &R, DT_CENTER);
	ResetDC(pDC);
	
	ReleaseDC(pDC);
}

CEdit* CBaseGrid::CreateEditWnd(const CRect& r, LPCTSTR lpszText, int nChar, int nFlags)
{
	return new CGridEditWnd(this, r, lpszText, nChar, nFlags);
}

CEdit* CBaseGrid::GetEditControl()
{
	return m_pEdit;
}

BOOL CBaseGrid::StartEdit(UINT nChar, UINT nFlags)
//-- return TRUE to allow editing
//-- return FALSE to prohibit editing
{
	ASSERT(m_SelectedCellRange.IsSingleCell());
	ASSERT(IsCellVisible(m_SelectedCellRange.GetTopLeft()));
	int col = m_SelectedCellRange.GetMinCol();
	int row = m_SelectedCellRange.GetMinRow();
	CString text;
	if (SetEditText(row, col, text)){
		m_bEditMode = TRUE;
		m_strOldEditData = m_strCurrentEditData = text;
	} else {
		return FALSE;
	}
	int x = GetColStartX(col);
	int y = GetRowStartY(row);
	CRect r(x, y, x + GetColWidth(col) + 1, y + GetRowHeight(row) + 1);
	m_pEdit = CreateEditWnd(r, m_strOldEditData, nChar, nFlags);
	ASSERT(m_pEdit);
	ASSERT(m_pEdit->GetSafeHwnd());
	return TRUE;
}

BOOL CBaseGrid::StopEdit(int nChar)
//-- return TRUE to specify the data is valid
//-- return FALSE to specify the data is invalid
{               
	if (m_bEditMode)
	{
		ASSERT(m_SelectedCellRange.IsSingleCell());
		ASSERT(IsCellVisible(m_SelectedCellRange.GetTopLeft()));
		ASSERT(m_pEdit);
		CString text;
		m_pEdit->GetWindowText(text);
		switch (nChar){
		case VK_RETURN:
		case VK_TAB:
			if (!GetEditText(m_SelectedCellRange.GetMinRow(), m_SelectedCellRange.GetMinCol(), text))
				return FALSE;
			break;
		case VK_UP:
		case VK_DOWN:
		case VK_NEXT:
		case VK_PRIOR:
		case VK_HOME:
		case VK_END:
			if (!GetEditText(m_SelectedCellRange.GetMinRow(), m_SelectedCellRange.GetMinCol(), text))
				return FALSE;
			OnKeyDown(nChar, 1, 0);
			break;
		default:
			return FALSE;
			break;
		case VK_ESCAPE:
			break;
		}
		
		CancelEdit();
	}
	return TRUE;
}

BOOL CBaseGrid::CancelEdit()
{
	if (m_bEditMode){
		m_bEditMode = FALSE;
		delete m_pEdit;
		m_pEdit = 0;
	}
	return TRUE; // this has no meaning
}

int CBaseGrid::GetFirstSelectedRow()
{
	return m_SelectedCellRange.GetMinRow();
}

int CBaseGrid::GetLastSelectedRow()
{
	return m_SelectedCellRange.GetMaxRow();
}

int CBaseGrid::GetFirstSelectedCol()
{
	return m_SelectedCellRange.GetMinCol();
}

int CBaseGrid::GetLastSelectedCol()
{
	return m_SelectedCellRange.GetMaxCol();
}

// if set to TRUE, makes single cell selection impossible
void CBaseGrid::SetListMode(BOOL bEnableListMode)
{
	// can't edit, when you can't select single cells
	ASSERT(!m_bEditEnabled);
	m_bListMode = bEnableListMode;
}

int CBaseGrid::HitTest(CPoint pt, int& xCell, int& yCell)
{
	
	return 0;
}

void CBaseGrid::GetClientPix(CRect* pRect)
{
	GetClientRect(pRect);
}

int CBaseGrid::HitTest(CPoint pt, CCellIndices& cellIndices)
{
	int hitValue = 0;
	CRect r;
	GetClientRect(&r);
	CPoint logPoint(pt.x, pt.y);

	int fixedColWidth = GetFixedColWidth();
	int fixedRowHeight = GetFixedRowHeight();

	// test x-values
	if (pt.x < 0){
		hitValue |= GHT_LEFT;
		cellIndices.col = -1;
	} else if (pt.x > r.right) {
		hitValue |= GHT_RIGHT;
		cellIndices.col = -1;
	} else {
		int col;
		if (logPoint.x < fixedColWidth){
			int xpos = 0;
			for (col = 0; col < m_nFixedCols; ++col){
				xpos += GetColWidth(col);
				if (xpos > logPoint.x) break;
			}
			cellIndices.col = col;
		} else {
			int xpos = fixedColWidth;
			int horzScrollPos = GetScrollPos(SB_HORZ);
			for (col = m_nFixedCols + horzScrollPos; col < GetColCount(); ++col){
				xpos += GetColWidth(col);
				if (xpos > logPoint.x) break;
			}
			if (col >= GetColCount()){
				cellIndices.col = -1;
			} else {
				cellIndices.col = col;
			}
		}
		// do sizing header test
		if (logPoint.y < fixedRowHeight && col > 0 && col+1 < GetColCount()){
			int colx = GetColStartX(col);
			if (pt.x - GetColStartX(col) <= slop){
				// barely to right of sizing line
				++cellIndices.col;
				hitValue = GHT_COLSIZER;
			} else if (GetColStartX(col+1) - pt.x <= slop){
				// barely to left of sizing line
				hitValue = GHT_ROWSIZER;
			}
		}
	}


	if (pt.y < 0){
		hitValue |= GHT_ABOVE;
		cellIndices.row = -1;
	} else if (pt.y > r.bottom){
		hitValue |= GHT_BELOW;
		cellIndices.row = -1;
	} else {
		int row;
		if (logPoint.y < fixedRowHeight){
			int ypos = 0;
			for (row = 0; row < m_nFixedRows; ++row){
				ypos += GetRowHeight(row);
				if (ypos > logPoint.y) break;
			}
			cellIndices.row = row;
		} else {
			int ypos = fixedRowHeight;
			int vertScrollPos = GetScrollPos(SB_VERT);
			for (row = m_nFixedRows + vertScrollPos; row < GetRowCount(); ++row){
				ypos += GetRowHeight(row);
				if (ypos > logPoint.y) break;
			}
			if (row >= GetRowCount()){
				cellIndices.row = -1;
			} else {
				cellIndices.row = row;
			}
		}
		if (logPoint.x < fixedColWidth && row > 0 && row+1 < GetRowCount()){
			int coly = GetRowStartY(row);
			if (pt.y - GetRowStartY(row) <= slop){
				++cellIndices.row;
				hitValue = GHT_ROWSIZER;
			} else if (GetRowStartY(row+1) - pt.y <= slop){
				hitValue = GHT_ROWSIZER;
			}
		}
	}

	// do sizing header test

	return hitValue;
}

CCellIndices CBaseGrid::GetCellIndices(CPoint clickPoint)
{  
	CCellIndices cellIndices; // return value

	// calculate column index
	int fixedColWidth = GetFixedColWidth();
	if (clickPoint.x < 0) // not in window
	{
		cellIndices.col = -1;
	}
	else if (clickPoint.x < fixedColWidth) // in fixed col
	{
		int xpos = 0;
		for (int col = 0; col < m_nFixedCols; col++)
		{
			xpos += GetColWidth(col);
			if (xpos > clickPoint.x) break;
		}
		cellIndices.col = col;
	}
	else
	{
		int xpos = fixedColWidth;
		int horzScrollPos = GetScrollPos(SB_HORZ);
		for (int col = m_nFixedCols + horzScrollPos; col < GetColCount(); col++)
		{
			xpos += GetColWidth(col);
			if (xpos > clickPoint.x) break;
		}
		if (col >= GetColCount())
		{
			//cellIndices.col = -1;
			cellIndices.col = GetColCount() - 1;
		}
		else
		{
			cellIndices.col = col;
		}
	}
	
	// calculate row index
	int fixedRowHeight = GetFixedRowHeight();
	if (clickPoint.y < 0) // not in window
	{
		cellIndices.row = -1;
	}
	else if (clickPoint.y < fixedRowHeight) // in fixed col
	{
		int ypos = 0;
		for (int row = 0; row < m_nFixedRows; row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > clickPoint.y) break;
		}
		cellIndices.row = row;
	}
	else
	{
		int ypos = fixedRowHeight;
		int vertScrollPos = GetScrollPos(SB_VERT);
		for (int row = m_nFixedRows + vertScrollPos; row < GetRowCount(); row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > clickPoint.y) break;
		}
		if (row >= GetRowCount())
		{
			cellIndices.row = -1;
		}
		else
		{
			cellIndices.row = row;
		}
	}

	return cellIndices;
}

int CBaseGrid::FillPrintIndices(CPrintIndicesArray& printIndices, const CSize& extent)
{
	int pages = 1;
	printIndices.RemoveAll();

	int xExtent = extent.cx;
	int yExtent = extent.cy;

#ifdef WIN32
	CArray<int, int> nColOffsets;
#else
   CUIntArray nColOffsets;
#endif
	nColOffsets.Add(0);
	int nColPages = 1;
	int xoffset = 0;//GetFixedColWidth();
	for (int c = 0; c < GetColCount(); ++c){
		xoffset += GetColWidth(c);
		if (xoffset > xExtent){
			nColOffsets.Add(c - 1);
			xoffset = GetFixedColWidth() + GetColWidth(c);
			++nColPages;
		}
	}

	for (int i = 0; i < nColPages; ++i){
		printIndices.Add(CCellIndices( 0, nColOffsets[i]));
	}
	int nRowPages = 1;
	
	int yoffset = 0;
	int rstart = 0;
	for (int r = 0; r < GetRowCount(); ++r){
		yoffset += GetRowHeight(r);
		if (yoffset > yExtent){
			for (int i = 0; i < nColPages; ++i){
				printIndices.Add(CCellIndices(r - 1, nColOffsets[i]));
			}
			yoffset = GetFixedRowHeight() + GetRowHeight(r);
			++nRowPages;
			rstart = r - 1;
		}
	}
	pages = nRowPages * nColPages;
	ASSERT(pages == printIndices.GetSize());
	return pages;
}

int CBaseGrid::PrepareForPrinting(CDC* pDC)
{
	CDC screenDC;
	screenDC.CreateIC(_T("DISPLAY"), 0, 0, 0);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(screenDC.GetDeviceCaps(LOGPIXELSX), screenDC.GetDeviceCaps(LOGPIXELSY));
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	CSize extent(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	pDC->DPtoLP(&extent);

	int pages = FillPrintIndices(m_PrintIndices, extent);
#ifdef _DEBUG
	for (int i = 0; i < pages; ++i)
		TRACE(_T("Col Offset: %i\tRowOffset %i\n"), m_PrintIndices[i].col,
			m_PrintIndices[i].row);
#endif
	return pages;
}

void CBaseGrid::PrintPage(CDC* pDC, int page)
{
	ASSERT(page <= m_PrintIndices.GetSize());
	int nColOffset = m_PrintIndices[page].col;
	int nRowOffset = m_PrintIndices[page].row;
	Draw(*pDC, nRowOffset, nColOffset);
}

void CBaseGrid::PrintPage(CDC* pDC, CPrintInfo* pInfo)
{
	PrintPage(pDC, pInfo->m_nCurPage - 1);
}

// Copyright (c) 1997 by Joe Willcoxson and WorldCom.  All rights reserved.
// E-mail:  chinajoe@aol.com
// URL:  http://users.aol.com/chinajoe
// For great long distance service see http://www.wcom.com

