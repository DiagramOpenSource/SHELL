#include "stdafx.h"
#include "colorbut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

// CColorButton is an owner drawn button class that is used by the 
// WCColorPage class.  It simply fills its client area with a color.

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton(COLORREF color)
 : m_Color(color)
{
}

CColorButton::~CColorButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDI)
{
	CDC* pDC = CDC::FromHandle(lpDI->hDC);
	CBrush fillBrush(m_Color);
	CBrush* oldBrush = pDC->SelectObject(&fillBrush);
	if (lpDI->itemState & ODS_FOCUS || lpDI->itemState & ODS_SELECTED)
	{
		CPen thickPen(PS_SOLID, 3, (~m_Color) & 0x00FFFFFF);
		CPen* oldPen = pDC->SelectObject(&thickPen);
	 	pDC->Rectangle(&lpDI->rcItem);
	 	pDC->SelectObject(oldPen);
	}
	else
	{
		pDC->Rectangle(&lpDI->rcItem);
	}
	pDC->SelectObject(oldBrush);
}

void CColorButton::SetColor(COLORREF color)
{
   m_Color = color;
}

COLORREF CColorButton::GetColor()
{
 	return m_Color;
}
