#include "stdafx.h"
#include "cellgrid.h"
#include "textcell.h" // standard text cell
#include "rtxtcell.h" // right justified text cell
#include "ctxtcell.h" // center justified text cell

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CCellGrid

// CCellGrid is a grid derived from CBaseGrid which owns "cells".  These cells 
// are derived from CCell and have a defined interface.  If you derive your
// own classes from CCell, you can put many different types of objects into
// this grid.


CCellGrid::CCellGrid(int rows, int cols, int fixedRows, int fixedCols)
 : CBaseGrid(rows, cols, fixedRows, fixedCols), m_CellMatrix(rows, cols)
{
	// note - did not initialize cells, they are already zeroed
}

CCellGrid::~CCellGrid()
{
}


BEGIN_MESSAGE_MAP(CCellGrid, CBaseGrid)
	//{{AFX_MSG_MAP(CCellGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


const CCell* CCellGrid::GetCell(int row, int col)
{
 	return m_CellMatrix.GetCell(row, col);
}

void CCellGrid::SetRowCount(int rows)
{
	if (rows == GetRowCount()) return;
	int addedRows = rows - GetRowCount();
	CBaseGrid::SetRowCount(rows);
	m_CellMatrix.SetRowCount(rows);
	int startRow = rows - addedRows;
	if (addedRows > 0)
	{
		// initialize all added rows to NULL
	 	for (int i = 0; i < GetColCount(); i++)
	 	{
	 	 	for (int j = startRow; j < GetRowCount(); j++)
	 	 	{
	 	 	 	SetCell(j, i, 0, FALSE);
	 	 	}
	 	}
	}
}

void CCellGrid::SetColCount(int cols)
{
	if (cols == GetColCount()) return;
	int addedCols = cols - GetColCount();
	CBaseGrid::SetColCount(cols);
	m_CellMatrix.SetColCount(cols);
	if (addedCols > 0)
	{
	 	// initialized all added cols to NULL
	 	int startCol = cols - addedCols;
	 	for (int i = startCol; i < GetColCount(); i++)
	 	{
	 	 	for (int j= 0; j < GetRowCount(); j++)
	 	 	{
	 	 	 	SetCell(j, i, 0, FALSE);
	 	 	}
	 	}
	}
}

void CCellGrid::SetCell(int row, int col, CCell* cell, BOOL bRepaint)
{
	m_CellMatrix.SetCell(row, col, cell);
	if (bRepaint && GetSafeHwnd())
	{
		Invalidate();
	}
}

void CCellGrid::SetText(int row, int col, LPCTSTR text, BOOL bRepaint)
{
	SetCell(row, col, new CTextCell(text), bRepaint);
}

void CCellGrid::SetRightText(int row, int col, LPCTSTR text, BOOL bRepaint)
{
	SetCell(row, col, new CRightTextCell(text), bRepaint);
}

void CCellGrid::SetCenteredText(int row, int col, LPCTSTR text, BOOL bRepaint)
{
	SetCell(row, col, new CCenteredTextCell(text), bRepaint);
}

void CCellGrid::DrawCell(int row, int col, CDC* pDC, int x, int y)
{
	const CCell* cell = GetCell(row,col);
	if (cell != 0)
	{
 		if (m_bConstrained)
 		{
 			cell->DrawConstrained(pDC, x, y, m_auColWidths[col] - 2 *m_nCellBorderThickness, 
 				m_auRowHeights[row] - 2 * m_nCellBorderThickness);
 		}
 		else
 		{
 			cell->DrawUnconstrained(pDC, x, y);
 		}
	}
}

CSize CCellGrid::GetCellExtent(int row, int col, CDC* pDC)
{
 	const CCell* cell = m_CellMatrix.GetCell(row, col);
 	if (!cell)
 	{
 	 	return CSize(0, 0);
 	}
 	return cell->GetExtent(pDC);
}

void CCellGrid::EnableEdit(BOOL bEnable /* = TRUE */)
{
 	CBaseGrid::EnableEdit(bEnable);
}

BOOL CCellGrid::SetEditText(int row, int col, CString& text)
{                   
	const CCell* cell = m_CellMatrix.GetCell(row, col);
	if (cell)
	{
		text = ((CTextCell*) cell)->GetText();
		return TRUE;
	}
	return FALSE;
}


BOOL CCellGrid::GetEditText(int row, int col, const CString& text)
{
 	SetRightText(row, col, text);
	return TRUE;
}

void CCellGrid::EmptyDataCells()
{
	for (int i = m_nFixedCols; i< m_nCols; i++)
		for (int j = m_nFixedRows; j < m_nRows; j++)
		{
			m_CellMatrix.SetCell(j, i, 0);
		}
}
