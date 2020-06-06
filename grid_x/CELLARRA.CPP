#include "stdafx.h"
#include "cellarra.h"
#include "cell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CCellArray is a class to manage cell arrays for cells which you put 
// into a CCellGrid cell.  It is owned by a CCellMatrix

CCellArray::CCellArray(int cellcount)
{
	m_Cells.SetSize(cellcount);
	
	// initialize all cell to NULL
	for (int i = 0; i < cellcount; i++)
	{
		m_Cells[i] = 0;
	}
}

CCellArray::~CCellArray()
{                                
	// delete all cells
	int size = m_Cells.GetSize();
	for (int i = 0; i < size; i++)
	{
	 	delete (CCell*) m_Cells[i];
	}
}

void CCellArray::SetCell(int index, CCell* aCell)
{
 	delete (CCell*) m_Cells[index];
 	m_Cells[index] = aCell;
}

const CCell* CCellArray::GetCell(int index)
{
 	return (CCell*) m_Cells[index];
}

void CCellArray::DeleteAt(int index)
{
	delete (CCell*) m_Cells[index];
 	m_Cells.RemoveAt(index);
}

void CCellArray::InsertAt(int index, CCell* aCell)
{
 	m_Cells.InsertAt(index, aCell);
}

void CCellArray::Add(CCell* aCell)
{
 	m_Cells.Add(aCell);
}
