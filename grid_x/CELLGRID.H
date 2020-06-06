#ifndef ccellgrid_h
#define ccellgrid_h

#include "basegrid.h"
#include "cellmatr.h"
#include "cellindi.h"
#include "cellrang.h"

/////////////////////////////////////////////////////////////////////////////
// CCellGrid window

// CCellGrid is a grid derived from CBaseGrid which owns "cells".  These cells 
// are derived from CCell and have a defined interface.  If you derive your
// own classes from CCell, you can put many different types of objects into
// this grid.


//////  USAGE NOTES

/*
  	If using this class from within a CDialog/CFormView, please take note:
  	
  	1.  In your .RC file, layout a user-defined control with the name "WORLDCOMGRID".
  	2.  In your OnCreate() or OnInitDlg() handler, subclass the user defined control with
  			the grid control:
  				
  				m_grid.SubclassDlgItem(IDC_USER1, this);
	
	3.  If you want to customize the grid, overload the DrawCell() member function, or
			derive a new class from CCell and put those CCells in the grid.
	4.  If you want to enable editting in the cell, call EnableEdit(), however, be warned
			that you will GPF if you don't have all text cells in editable areas.
*/

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

class WCMFCLIB_CLASS CCellGrid : public CBaseGrid
{
// Construction
public:
	WCMFC_DEF CCellGrid(int rows, int cols, int fixedRows = 1, int fixedCols = 1);
	

// Operations
public:
	void WCMFC_DEF SetRowCount(int rows);
	void WCMFC_DEF SetColCount(int cols);
	
	void WCMFC_DEF EmptyDataCells(); // deletes data in non-control cells

	const CCell* WCMFC_DEF GetCell(int row, int col);
	
	void WCMFC_DEF SetCell(int row, int col, CCell* aCell, BOOL bRepaint = TRUE);
	void WCMFC_DEF SetText(int row, int col, LPCTSTR text, BOOL bRepaint = TRUE);
	void WCMFC_DEF SetRightText(int row, int col, LPCTSTR text, BOOL bRepaint = TRUE);
	void WCMFC_DEF SetCenteredText(int row, int col, LPCTSTR text, BOOL bRepaint = TRUE);
	
	void WCMFC_DEF EnableEdit(BOOL bEnable = TRUE);
	
// Implementation
public:
	virtual WCMFC_DEF ~CCellGrid();

	protected:
	virtual void WCMFC_DEF DrawCell(int row, int col, CDC* pDC, int x, int y);
	virtual CSize WCMFC_DEF GetCellExtent(int row, int col, CDC* pDC);


	// Generated message map functions
protected:
	//{{AFX_MSG(CCellGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	// Edit functionality
	virtual BOOL WCMFC_DEF GetEditText(int row, int col, const CString& text);
	virtual BOOL WCMFC_DEF SetEditText(int row, int col, CString& text);

protected:
	// Attributes
	
	CCellMatrix m_CellMatrix;
};

/////////////////////////////////////////////////////////////////////////////

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif // end of sentinel code
