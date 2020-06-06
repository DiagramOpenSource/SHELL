#ifndef ccellrange_h
#define ccellrange_h

#include "cellindi.h"

#ifdef WIN32
	#define WCMFC_DEF
	#ifdef WCMFCLIB
	#define WCMFCLIB_CLASS AFX_CLASS_EXPORT
	#else
	#define WCMFCLIB_CLASS AFX_CLASS_IMPORT
	#endif
#else
	#define WCMFCLIB_CLASS
	#ifdef _WCMFC_DEF
	#define WCMFC_DEF EXPORT
	#else
	#define WCMFC_DEF
	#endif
#endif

// CCellRange is a utility class to manage selections of grid cells.  It is owned
// by a CBaseGrid.

class WCMFCLIB_CLASS CCellRange
{ 
	public:
	CCellRange(int minRow = -1, int minCol = -1, int maxRow = -1, int maxCol = -1)
	: m_nMinRow(minRow), m_nMinCol(minCol), m_nMaxRow(maxRow), m_nMaxCol(maxCol)
	{}
	
	CCellRange(CCellIndices min, CCellIndices max)
	: m_nMinRow(min.row), m_nMinCol(min.col), m_nMaxRow(max.row), m_nMaxCol(max.col)
	{}
	
	void WCMFC_DEF Set(int minRow, int minCol, int maxRow, int maxCol);
	
	int WCMFC_DEF IsValid() const;
	int WCMFC_DEF InRange(int row, int col) const;
	int WCMFC_DEF InRange(const CCellIndices& cellIndices) const;
	int WCMFC_DEF IsSingleCell() const;
	
	CCellIndices WCMFC_DEF GetTopLeft() const;
	CCellRange WCMFC_DEF Intersect(const CCellRange& rhs) const;
	CCellRange WCMFC_DEF Union(const CCellRange& rhs) const;
	
	int GetMinRow() const {return m_nMinRow;}
	void SetMinRow(int minRow) {m_nMinRow = minRow;}
	
	int GetMinCol() const {return m_nMinCol;}
	void SetMinCol(int minCol) {m_nMinCol = minCol;}
	
	int GetMaxRow() const {return m_nMaxRow;}
	void SetMaxRow(int maxRow) {m_nMaxRow = maxRow;}
	
	int GetMaxCol() const {return m_nMaxCol;}
	void SetMaxCol(int maxCol) {m_nMaxCol = maxCol;}
	
	int WCMFC_DEF operator==(const CCellRange& rhs);
	int WCMFC_DEF operator!=(const CCellRange& rhs);
	
	protected:
	int m_nMinRow;
	int m_nMinCol;
	int m_nMaxRow;
	int m_nMaxCol;
};

inline void CCellRange::Set(int minRow, int minCol, int maxRow, int maxCol)
{
 	m_nMinRow = minRow;
 	m_nMinCol = minCol;
 	m_nMaxRow = maxRow;
 	m_nMaxCol = maxCol;
}

inline int CCellRange::IsValid() const
{
 	return (m_nMinRow != -1 && m_nMinCol != -1 && m_nMaxRow != -1 && m_nMaxCol != -1 &&
 				m_nMinRow <= m_nMaxRow && m_nMinCol <= m_nMaxCol);
}

inline int CCellRange::InRange(int row, int col) const
{
 	return (row >= m_nMinRow && row <= m_nMaxRow && col >= m_nMinCol && col <= m_nMaxCol);
}

inline int CCellRange::IsSingleCell() const
{
 	return (m_nMinRow == m_nMaxRow && m_nMinCol == m_nMaxCol && IsValid());
}

inline int CCellRange::InRange(const CCellIndices& cellIndices) const
{
 	return InRange(cellIndices.row, cellIndices.col);
}

inline CCellIndices CCellRange::GetTopLeft() const
{
 	return CCellIndices(m_nMinRow, m_nMinCol);
}

inline CCellRange CCellRange::Intersect(const CCellRange& rhs) const
{
 	return CCellRange(max(m_nMinRow,rhs.m_nMinRow), max(m_nMinCol,rhs.m_nMinCol),
 		min(m_nMaxRow,rhs.m_nMaxRow), min(m_nMaxCol,rhs.m_nMaxCol));
}

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
