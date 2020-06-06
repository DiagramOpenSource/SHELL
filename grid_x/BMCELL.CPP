#include "stdafx.h"
#include "bmcell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CBitmapCell is a class which allows you to put a bitmap into a CCellGrid cell.

CBitmapCell::CBitmapCell(UINT nIDResource)
{
 	VERIFY(m_Bitmap.LoadBitmap(nIDResource));
	BITMAP bm;
	m_Bitmap.GetObject(sizeof(bm), &bm);
	m_Bitmap.SetBitmapDimension(bm.bmWidth,bm.bmHeight);
}

CBitmapCell::~CBitmapCell()
{
}

void CBitmapCell::DrawUnconstrained(CDC* pDC, int x, int y) const
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* oldBM = memDC.SelectObject((CBitmap*) &m_Bitmap);
	CSize size = GetExtent(pDC);
	pDC->BitBlt(x,y,size.cx,size.cy,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(oldBM);
}

void CBitmapCell::DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* oldBM = memDC.SelectObject((CBitmap*) &m_Bitmap);
	pDC->BitBlt(x, y, cx, cy, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldBM);
}

CSize CBitmapCell::GetExtent(CDC* /*pDC*/) const
{
 	return m_Bitmap.GetBitmapDimension();
}
