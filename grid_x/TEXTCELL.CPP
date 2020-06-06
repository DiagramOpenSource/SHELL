#include "stdafx.h"
#include "textcell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CText cell is a class used for CCellGrid.  Use it to put
// left justified text in a cell. It is the base class for other text cells.

CTextCell::CTextCell(LPCTSTR text)
 : m_pszText(0)
{
 	if (text)
 	{
 		m_pszText = new TCHAR[strlen(text) + 1];
 		strcpy(m_pszText, text);
 	}
 	else
 	{
 	 	m_pszText = new TCHAR[1];
 	 	m_pszText[0] = '\0';
 	}
}

CTextCell::~CTextCell()
{
	delete [] m_pszText;
}

// copy constructor
CTextCell::CTextCell(const CTextCell& rhs)
 : CCell(rhs), m_pszText(0)
{
 	m_pszText = new TCHAR[strlen(rhs.m_pszText) + 1];
 	strcpy(m_pszText, rhs.m_pszText);
}
 
// assignment operator
const CTextCell& CTextCell::operator=(const CTextCell& rhs)
{
 	if (&rhs == this) return *this;
 	CCell::operator=(rhs);
 	delete m_pszText;
 	m_pszText = new TCHAR[strlen(rhs.m_pszText) + 1];
 	strcpy(m_pszText, rhs.m_pszText);
	return *this; 	
}
 
void CTextCell::DrawUnconstrained(CDC* pDC, int x, int y) const
{
 	pDC->TextOut(x, y, m_pszText, strlen(m_pszText));
} 

void CTextCell::DrawConstrained(CDC* pDC, int x, int y, int cx, int cy) const
{
	CRect R(x, y, x + cx, y + cy);
 	pDC->DrawText(m_pszText, strlen(m_pszText), &R, DT_LEFT);
}                                         

LPCTSTR CTextCell::GetText() const
{
 	return (LPCTSTR) m_pszText;
}

void CTextCell::SetText(const char* text)
{
 	delete m_pszText;
 	m_pszText = 0;
 	if (text)
 	{
 		m_pszText = new TCHAR[strlen(text) + 1];
 		strcpy(m_pszText, text);
 	}
 	else
 	{
 	 	m_pszText = new TCHAR[1];
 	 	m_pszText[0] = '\0';
 	}
}

CSize GetNewlineTextExtent(CDC* pDC, const char* str)
{
	CSize ret(0,0);
	ASSERT(str);
	LPCTSTR strstart = str;
 	LPCTSTR strpos = str;
 	LPCTSTR strend = str + strlen(str);
 	while (strstart != 0)
 	{
 		CSize tempSize;
 	 	strpos = strchr(strstart, '\n');
 	 	if (strpos != 0)
 	 	{
 	 		tempSize = pDC->GetTextExtent(strstart, strpos - strstart);
 	 		strstart = strpos + 1;
 	 	}
 	 	else
 	 	{
 	 	 	tempSize = pDC->GetTextExtent(strstart, strend - strstart);
 	 	 	strstart = strpos;
 	 	}
 	 	ret.cx = max(ret.cx, tempSize.cx);
 	 	ret.cy += tempSize.cy;
 	}
 	return ret;
}

CSize CTextCell::GetExtent(CDC* pDC) const
{
 	return GetNewlineTextExtent(pDC, m_pszText);
}
