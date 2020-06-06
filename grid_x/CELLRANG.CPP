#include "stdafx.h"
#include "cellrang.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellRange

// CCellRange is a utility class to manage selections of grid cells.  It is owned
// by a CBaseGrid.

int CCellRange::operator==(const CCellRange& rhs)
{
 	return ((m_nMinRow == rhs.m_nMinRow) && (m_nMinCol == rhs.m_nMinCol) &&
 		(m_nMaxRow == rhs.m_nMaxRow) && (m_nMaxCol == rhs.m_nMaxCol));
}


int CCellRange::operator!=(const CCellRange& rhs)
{
 	return !operator==(rhs);
}

CCellRange CCellRange::Union(const CCellRange& rhs) const
{
	return CCellRange(min(m_nMinRow,rhs.m_nMinRow), min(m_nMinCol, rhs.m_nMinCol),
				max(m_nMaxRow, rhs.m_nMaxRow), max(m_nMaxCol, rhs.m_nMaxCol));
}
