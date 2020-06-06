#include "stdafx.h"
#include "cellindi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellIndices

// CCellIndices is a utility class to help with CBaseGrid and CCellGrid.
// It provides operations to manage cell indices.

int CCellIndices::IsDownRightOf(const CCellIndices& rhs)
{
 	return (row >= rhs.row && col >= rhs.col);
}

int CCellIndices::IsDownLeftOf(const CCellIndices& rhs)
{
 	return (row >= rhs.row && col <= rhs.col);
}

int CCellIndices::IsUpRightOf(const CCellIndices& rhs)
{
 	return (row <= rhs.row && col >= rhs.col);
}

int CCellIndices::IsUpLeftOf(const CCellIndices& rhs)
{
 	return (row <= rhs.row && col <= rhs.col);
}

#ifdef WIN32
CPrintIndicesArray::CPrintIndicesArray()
{
}

#else

CPrintIndicesArray::CPrintIndicesArray()
: m_pArray(0), m_nSize(0), m_nTotalSize(0), m_nStep(10)
{
   m_pArray = 0;
}

CPrintIndicesArray::~CPrintIndicesArray()
{
	RemoveAll();
}

CCellIndices CPrintIndicesArray::operator[](int index)
{
   ASSERT(index < m_nSize);
   return m_pArray[index];
}

int CPrintIndicesArray::Add(const CCellIndices& ci)
{
   if (m_nSize == m_nTotalSize){
      CCellIndices* pNewCI = new CCellIndices[m_nTotalSize + m_nStep];
      memcpy(pNewCI, m_pArray, m_nSize * sizeof(CCellIndices));
      delete m_pArray;
      m_pArray = pNewCI;
      m_nTotalSize += m_nStep;
   }
   m_pArray[m_nSize] = ci;
   return m_nSize++;
}

void CPrintIndicesArray::RemoveAll()
{
   if (m_nTotalSize != 0){
	   m_nTotalSize = m_nSize = 0;
	   delete [] m_pArray;
	   m_pArray = 0;
   }
}

#endif
