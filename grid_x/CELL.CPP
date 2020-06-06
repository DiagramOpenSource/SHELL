#include "stdafx.h"
#include "cell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// CCell is a base class for all other cells which allows you to put a 
// cell into a CCellGrid cell.

CCell::CCell()
{
}

CCell::~CCell()
{
}

CCell::CCell(const CCell& /*rhs*/)
{
}

const CCell& CCell::operator=(const CCell& rhs)
{
 	if (&rhs == this) return *this;
 	return *this;
}

void CCell::DrawUnconstrained(CDC* /* pDC */, int /* x */, int /* y */) const
{
}

void CCell::DrawConstrained(CDC* /* pDC */, int /*x*/, int /*y*/, int /*cx*/, int /*cy*/) const
{
}

CSize CCell::GetExtent(CDC* /*pDC*/) const
{
 	return CSize(0,0);
}
