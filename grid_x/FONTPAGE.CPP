#include "stdafx.h"
#include "fontpage.h"
#include "wcmfcres.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// WCFontPage dialog

// WCFontPage is a property sheet that allows the selection of a font.
// WCMFCRES.RC must be included into the project resource file for any
// project that uses this class.

WCFontPage::WCFontPage(const LOGFONT* plf /*=0*/)
	: CPropertyPage(IDD_WC_FONTPAGE)
{
	//{{AFX_DATA_INIT(WCFontPage)
	m_StrikeOut = FALSE;
	m_Underline = FALSE;
	m_FaceName = "";
	//}}AFX_DATA_INIT
	if (plf != 0)
	{
		m_lf = *plf;
	}
	else
	{
		memset(&m_lf, 0 , sizeof(m_lf));
	}
	m_StrikeOut = m_lf.lfStrikeOut;
	m_Underline = m_lf.lfUnderline;
	m_FaceName = m_lf.lfFaceName;
	m_DisplayFont.CreateFontIndirect(&m_lf);
}

void WCFontPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WCFontPage)
	DDX_Control(pDX, IDC_FP_SAMPLE, m_SampleText);
	DDX_Control(pDX, IDC_FP_SIZE, m_SizeList);
	DDX_Control(pDX, IDC_FP_STYLELIST, m_StyleList);
	DDX_Control(pDX, IDC_FP_FONTLIST, m_FontList);
	DDX_Check(pDX, IDC_FP_STRIKEOUT, m_StrikeOut);
	DDX_Check(pDX, IDC_FP_UNDERLINE, m_Underline);
	DDX_CBString(pDX, IDC_FP_FONTLIST, m_FaceName);
	DDV_MaxChars(pDX, m_FaceName, 32);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_FP_SIZE, m_lf.lfHeight);
	if (pDX->m_bSaveAndValidate) // playing it safe
	{
		m_lf.lfStrikeOut = (BYTE) m_StrikeOut;
		m_lf.lfUnderline = (BYTE) m_Underline;
		strcpy(m_lf.lfFaceName, m_FaceName);
	}
}

BEGIN_MESSAGE_MAP(WCFontPage, CPropertyPage)
	//{{AFX_MSG_MAP(WCFontPage)
	ON_CBN_EDITCHANGE(IDC_FP_FONTLIST, OnEditChangeFontList)
	ON_CBN_SELCHANGE(IDC_FP_FONTLIST, OnSelchangeFpFontlist)
	ON_CBN_SELCHANGE(IDC_FP_STYLELIST, OnSelchangeFpStylelist)
	ON_BN_CLICKED(IDC_FP_STRIKEOUT, OnFpStrikeout)
	ON_BN_CLICKED(IDC_FP_UNDERLINE, OnFpUnderline)
	ON_CBN_SELCHANGE(IDC_FP_SIZE, OnSelchangeFpSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CALLBACK WCFontPage::FontNameEnumerator(LOGFONT FAR* lf, NEWTEXTMETRIC FAR* /*tm*/, 
		int FontType, WCFontPage* page)
{
	if (FontType & TRUETYPE_FONTTYPE)
	{
	 	page->m_FontList.AddString(lf->lfFaceName);
	}
 	return 1;
}

int CALLBACK WCFontPage::FontStyleEnumerator(LOGFONT FAR* lf, NEWTEXTMETRIC FAR* /*tm*/, 
		int FontType, WCFontPage* page)
{
	if (FontType & TRUETYPE_FONTTYPE)
	{
	 	//page->m_StyleList.AddString(((ENUMLOGFONT*)lf)->elfStyle);
	 	page->m_EnumFont = *lf;
	}
 	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// WCFontPage message handlers

BOOL WCFontPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	LoadFontList();
	if (m_FontList.SelectString(-1, m_lf.lfFaceName) != CB_ERR)
	{
	 	LoadStyleList();
	}
	SetSampleFont();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void WCFontPage::OnEditChangeFontList()
{
	CString editText;
	m_FontList.GetWindowText(editText);
	int index = m_FontList.FindStringExact(-1, editText);
	if (index != CB_ERR)
	{
	 	//m_FontList.SetCurSel(index);
	 	//m_FontList.SetEditSel(-1,-1);
	 	m_FaceName = editText;
	 	m_FontList.SetWindowText(editText);
	 	m_FontList.SelectString(-1, editText);
	 	//UpdateData(FALSE);
	 	OnSelchangeFpFontlist();
	}
}

void WCFontPage::OnSelchangeFpFontlist()
{
	LoadStyleList();
	int index = m_FontList.GetCurSel();
	CString editText;
	m_FontList.GetLBText(index, editText);
	m_FontList.SetWindowText(editText);
	UpdateData(TRUE);
	SetSampleFont();
}

void WCFontPage::LoadFontList()
{
	FONTENUMPROC lpEnumProc = (FONTENUMPROC) MakeProcInstance(
		(FARPROC) FontNameEnumerator, AfxGetInstanceHandle());

	CClientDC dc(GetDesktopWindow());		
	EnumFontFamilies(dc.m_hDC, 0, lpEnumProc, (long) this);
	FreeProcInstance((FARPROC) lpEnumProc);
}

void WCFontPage::LoadStyleList()
{
	m_StyleList.ResetContent();
	int index = m_FontList.GetCurSel();
	CString editText;
	m_FontList.GetLBText(index, editText);
//	m_FontList.GetWindowText(editText);

	FONTENUMPROC lpEnumProc = (FONTENUMPROC) MakeProcInstance(
		(FARPROC) FontStyleEnumerator, AfxGetInstanceHandle());
	CClientDC dc(GetDesktopWindow());		
	EnumFontFamilies(dc.m_hDC, editText, lpEnumProc, (long) this);
	FreeProcInstance((FARPROC) lpEnumProc);
	
	// load style list box
	m_StyleList.ResetContent();
	if (m_EnumFont.lfWeight < FW_SEMIBOLD && !m_EnumFont.lfItalic) // normal
	{
	 	m_StyleList.AddString(_T("Regular"));
	 	m_StyleList.AddString(_T("Italic"));
	 	m_StyleList.AddString(_T("Bold"));
	 	m_StyleList.AddString(_T("Bold Italic"));
	}
	else if (m_EnumFont.lfItalic)
	{
	 	m_StyleList.AddString(_T("Italic"));
	 	m_StyleList.AddString(_T("Bold Italic"));
	}
	else if (m_EnumFont.lfWeight >= FW_SEMIBOLD)
	{
	 	m_StyleList.AddString(_T("Bold"));
	 	m_StyleList.AddString(_T("Bold Italic"));
	}
	else
	{
	 	m_StyleList.AddString(_T("Unknown"));
	}
	
	// select the right style
	if (m_lf.lfWeight < FW_SEMIBOLD && !m_lf.lfItalic)
	{
	 	m_StyleList.SelectString(-1, _T("Regular"));
	}
	else if (m_lf.lfWeight < FW_SEMIBOLD && m_lf.lfItalic)
	{
	 	m_StyleList.SelectString(-1, _T("Italic"));
	}
	else if (!m_lf.lfItalic)
	{
	 	m_StyleList.SelectString(-1, _T("Bold"));
	}
	else
	{
	 	m_StyleList.SelectString(-1,_T("Bold Italic"));
	}
	if (m_StyleList.GetCurSel() == CB_ERR)
	{
	 	m_StyleList.SetCurSel(0);
	 	OnSelchangeFpStylelist();
	}
}
void WCFontPage::OnSelchangeFpStylelist()
{
	int curSel = m_StyleList.GetCurSel();
	CString styleText;
	m_StyleList.GetLBText(curSel, styleText);
	m_StyleList.SetWindowText(styleText);
	if (styleText == _T("Regular"))
	{
	 	m_lf.lfItalic = FALSE;
	 	m_lf.lfWeight = FW_NORMAL;
	}
	else if (styleText == _T("Italic"))
	{
	 	m_lf.lfItalic = TRUE;
	 	m_lf.lfWeight = FW_NORMAL;
	}
	else if (styleText == _T("Bold"))
	{
	 	m_lf.lfItalic = FALSE;
	 	m_lf.lfWeight = FW_BOLD;
	}
	else
	{
	 	m_lf.lfItalic = TRUE;
	 	m_lf.lfWeight = FW_BOLD;
	}
	UpdateData(TRUE);
	SetSampleFont();
}

void WCFontPage::SetSampleFont()
{
	m_DisplayFont.DeleteObject();
	m_DisplayFont.CreateFontIndirect(&m_lf);
 	m_SampleText.SetFont(&m_DisplayFont);	
}

void WCFontPage::OnFpStrikeout()
{
	UpdateData(TRUE);
	SetSampleFont();
}

void WCFontPage::OnFpUnderline()
{
	UpdateData(TRUE);
	SetSampleFont();
}

void WCFontPage::OnSelchangeFpSize()
{
	int curSel = m_SizeList.GetCurSel();
	CString sizeText;
	m_SizeList.GetLBText(curSel, sizeText);
	m_SizeList.SetWindowText(sizeText);
	UpdateData(TRUE);
	SetSampleFont();
}
