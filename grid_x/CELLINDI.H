#ifndef ccellindices_h
#define ccellindices_h

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

// CCellIndices is a utility class to help with CBaseGrid and CCellGrid.
// It provides operations to manage cell indices.

class WCMFCLIB_CLASS CCellIndices
{    
	public:
	int row; // range < 0 is not valid
	int col;
	CCellIndices(int aRow = -1, int aCol = -1) : row(aRow), col(aCol){}
	int IsValid() const {return (row != -1 && col != -1);}
	int operator==(const CCellIndices& rhs) {return (row == rhs.row && col == rhs.col);}
	int operator!=(const CCellIndices& rhs) {return !operator==(rhs);}
	int WCMFC_DEF IsDownRightOf(const CCellIndices& rhs);
	int WCMFC_DEF IsDownLeftOf(const CCellIndices& rhs);
	int WCMFC_DEF IsUpRightOf(const CCellIndices& rhs);
	int WCMFC_DEF IsUpLeftOf(const CCellIndices& rhs);
};

#ifdef WIN32
class WCMFCLIB_CLASS CPrintIndicesArray : public CArray<CCellIndices, CCellIndices>
{
public:
	CPrintIndicesArray();
};
#else

class CPrintIndicesArray
{                                     
public:
	CPrintIndicesArray();
	~CPrintIndicesArray();
	CCellIndices operator[](int index);
	int Add(const CCellIndices& ci);
	int GetSize() {return m_nSize;}
	void RemoveAll();
protected:
	CCellIndices* m_pArray;
	int m_nSize;
	int m_nTotalSize;
	int m_nStep;
private:
   CPrintIndicesArray(const CPrintIndicesArray&);
   CPrintIndicesArray& operator=(const CPrintIndicesArray&);
};

#endif



#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
