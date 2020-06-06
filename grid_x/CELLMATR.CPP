#include "stdafx.h"
#include "cellmatr.h"
#include "cellarra.h"
#include "cell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CCellMatrix is a utility class to manage CCell instances.  It is owned
// by a CBaseGrid.

CCellMatrix::CCellMatrix(int rows, int cols)
 : m_nRows(rows), m_nCols(cols)
{
	m_nCellColumns.SetSize(m_nCols);
	
	// initialize each column to a cell array of row length
	for (int i = 0; i < m_nCols; i++)
	{
	 	m_nCellColumns[i] = new CCellArray(m_nRows);
	 	ASSERT(m_nCellColumns[i]);
	}
}

CCellMatrix::~CCellMatrix()
{
	// delete all columns
	for (int i = 0; i < m_nCols; i++)
	{
	 	delete (CCellArray*) m_nCellColumns[i];
	}
}

void CCellMatrix::AddRow()
{
 	for (int i = 0; i < m_nCols; i++)
 	{
 	 	((CCellArray*) m_nCellColumns[i])->Add(0);
 	}
 	m_nRows++;
}

void CCellMatrix::AddColumn()
{
	// note - ARRAY_P.CPP fills this with zeroes
 	CCellArray* newColumn = new CCellArray(m_nRows);
 	ASSERT(newColumn);
 	m_nCellColumns.Add(newColumn);
 	m_nCols++;
}

void CCellMatrix::DeleteRow(int row)
{
 	ASSERT( (row >= 0) && (row < m_nRows) );
 	for (int i = 0; i < m_nCols; i++)
 	{
 	 	((CCellArray*) m_nCellColumns[i])->DeleteAt(row);
 	}
 	m_nRows--;
}

void CCellMatrix::DeleteColumn(int col)
{
 	ASSERT( (col >= 0) && (col < m_nCols) );
 	delete (CCellArray*) m_nCellColumns[col];
 	m_nCellColumns.RemoveAt(col);
 	m_nCols--;
}

void CCellMatrix::InsertRowAt(int row)
{
 	ASSERT( (row >= 0) && (row < m_nRows) );
 	for (int i = 0; i < m_nCols; i++)
 	{
 	 	((CCellArray*) m_nCellColumns[i])->InsertAt(row, 0);
 	}
 	m_nRows++;
}

void CCellMatrix::InsertColumnAt(int col)
{
 	ASSERT( (col >= 0) && (col < m_nCols) );
 	CCellArray* newColumn = new CCellArray(m_nRows);
 	ASSERT(newColumn);
 	m_nCellColumns.InsertAt(col, newColumn);
 	m_nCols++;
}

void CCellMatrix::SetCell(int row, int col, CCell* cell)
{
	((CCellArray*) m_nCellColumns[col])->DeleteAt(row);
	((CCellArray*) m_nCellColumns[col])->InsertAt(row, cell);
}

const CCell* CCellMatrix::GetCell(int row, int col)
{
 	return ((CCellArray*) m_nCellColumns[col])->GetCell(row);
}

void CCellMatrix::SetRowCount(int rows)
{
	ASSERT(rows > 0);
	if (rows == GetRowCount())
	{
	 	return;
	}
	else if (rows < GetRowCount())
	{
		// delete rows from the end
		int iterations = GetRowCount() - rows;
		for (int i = 0; i < iterations; i++)
		{
		 	DeleteRow(GetRowCount() - 1);
		}
	}
	else //(rows > GetRowCount())
	{
		// add rows to the end
		int iterations = rows - GetRowCount();
		for (int i = 0; i < iterations; i++)
		{
		 	AddRow();
		}
	}
}

void CCellMatrix::SetColCount(int cols)
{
	ASSERT(cols > 0);
	if (cols == GetColCount())
	{
	 	return;
	}
	else if (cols < GetColCount())
	{
		// delete cols from the end
	 	int iterations = GetColCount() - cols;
	 	for (int i = 0; i < iterations; i++)
	 	{
	 	 	DeleteColumn(GetColCount() - 1);
	 	}
	}
	else // (cols > GetColCount())
	{
	 	// add cols to the end
	 	int iterations = cols - GetColCount();
	 	for (int i = 0; i < iterations; i++)
	 	{
	 	 	AddColumn();
	 	}
	}
}
