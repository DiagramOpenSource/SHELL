#ifndef fontpage_h
#define fontpage_h

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

/////////////////////////////////////////////////////////////////////////////
// WCFontPage dialog

// WCFontPage is a property sheet that allows the selection of a font.
// WCMFCRES.RC must be included into the project resource file for any
// project that uses this class.

class WCMFCLIB_CLASS WCFontPage : public CPropertyPage
{
// Construction
public:
	WCMFC_DEF WCFontPage(const LOGFONT* plf = 0);	// standard constructor

// Dialog Data
	//{{AFX_DATA(WCFontPage)
	CStatic	m_SampleText;
	CComboBox	m_SizeList;
	CComboBox	m_StyleList;
	CComboBox	m_FontList;
	BOOL	m_StrikeOut;
	BOOL	m_Underline;
	CString	m_FaceName;
	//}}AFX_DATA
   LOGFONT m_lf;
   
// Implementation
protected:
	virtual void WCMFC_DEF DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(WCFontPage)
	virtual BOOL WCMFC_DEF OnInitDialog();
	afx_msg void WCMFC_DEF OnEditChangeFontList();
	afx_msg void WCMFC_DEF OnSelchangeFpFontlist();
	afx_msg void WCMFC_DEF OnSelchangeFpStylelist();
	afx_msg void WCMFC_DEF OnFpStrikeout();
	afx_msg void WCMFC_DEF OnFpUnderline();
	afx_msg void WCMFC_DEF OnSelchangeFpSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	static int CALLBACK EXPORT FontNameEnumerator(LOGFONT FAR* lf, NEWTEXTMETRIC FAR* tm, 
		int FontType, WCFontPage* page);
	static int CALLBACK EXPORT FontStyleEnumerator(LOGFONT FAR* lf, NEWTEXTMETRIC FAR* tm, 
		int FontType, WCFontPage* page);
		
	void WCMFC_DEF LoadFontList();
	void WCMFC_DEF LoadStyleList();
	void WCMFC_DEF SetSampleFont();
	
	LOGFONT m_EnumFont;
   CFont m_DisplayFont;
};

#undef WCMFC_DEF
#undef WCMFCLIB_CLASS

#endif
