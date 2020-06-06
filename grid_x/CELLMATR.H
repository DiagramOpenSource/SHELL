#ifndef cellmatrix_h
#define cellmatrix_h

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

class CCell;
class CCellArray;

// CCellMatrix is a utility class to manage CCell instances.  It is owned
// by a CBaseGrid.

class WCMFCLIB_CLASS CCellMatrix
{
	public:
	WCMFC_DEF CCellMatrix(int rows, int cols);
	WCMFC_DEF ~CCellMatrix();
	
	void WCMFC_DEF AddRow();
	void WCMFC_DEF AddColumn();
	
	void WCMFC_DEF DeleteRow(int row);
	void WCMFC_DEF DeleteColumn(int col);
	
	void WCMFC_DEF InsertRowAt(int row);
	void WCMFC_DEF InsertColumnAt(int col);
	
	void WCMFC_DEF SetCell(int row, int col, CCell* cell);
	const CCell* WCMFC_DEF GetCell(int row, int col);
	
	int WCMFC_DEF GetRowCount() {return m_nRows;}
	int WCMFC_DEF GetColCount() {return m_nCols;}
	
	void WCMFC_DEF SetRowCount(int rows);
	void WCMFC_DEF SetColCount(int cols);
	
	protected:
	int m_nRows;
	int m_nCols;
	
	CPtrArray m_nCellColumns;
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
