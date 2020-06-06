#ifndef cellarray_h
#define cellarray_h


// CCellArray is a class to manage cell arrays for cells which you put 
// into a CCellGrid cell.  It is owned by a CCellMatrix

class CCell;

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

class WCMFCLIB_CLASS CCellArray
{
  	public:
  	WCMFC_DEF CCellArray( int cellcount = 0);
  	WCMFC_DEF ~CCellArray();
  	
  	void WCMFC_DEF SetCell(int index, CCell* aCell); // deletes old cell
  	const CCell* WCMFC_DEF GetCell(int index);
  	
  	void WCMFC_DEF DeleteAt(int index);
  	void WCMFC_DEF InsertAt(int index, CCell* aCell);
  	
  	void WCMFC_DEF Add(CCell* aCell);
  	
  	protected:
  	
  	CPtrArray m_Cells;
  	
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
