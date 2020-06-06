#include "stdafx.h"
#include "rtxtcell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CRightText cell is a class used for CCellGrid.  Use it to put
// right justified text in a cell.

void CRightTextCell::DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const
{
 	CRect R(x, y, x+cx, y+cy);
   pDC->DrawText(m_pszText, -1, &R, DT_RIGHT);
}

