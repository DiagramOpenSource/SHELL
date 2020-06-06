#if !defined(AFX_DSTABCTRL_H__C44D18B6_9DD3_11D3_B2AC_004F4905CBCF__INCLUDED_)
#define AFX_DSTABCTRL_H__C44D18B6_9DD3_11D3_B2AC_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsTabCtrl window

class CDsStdDlg;
class CDsStdCtrl;

#ifdef DSXTLIB
class CDsTabCtrl : public CXTPTabControl
#else
#ifdef DSPROFUIS

class CDSTabFlatWnd : public CExtTabFlatWnd
{
	DECLARE_DYNAMIC(CDSTabFlatWnd)
public:
	CDSTabFlatWnd::CDSTabFlatWnd()
	{
		m_colorset = FALSE;
		m_colorset9 = FALSE;
	};
	virtual void OnTabWndEraseClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode
		);
	virtual void OnFlatTabWndGetItemColors(
		LONG nItemIndex,
		bool bSelected,
#ifndef PROFUIS240
		//**
		bool bHover,
		bool bEnabled,
		//**
#endif
		COLORREF &clrLight,
		COLORREF &clrShadow,
		COLORREF &clrDkShadow, 
		COLORREF &clrTabBk, 
		COLORREF &clrText
		);
	COLORREF m_clrLight;
	COLORREF m_clrShadow;
	COLORREF m_clrDkShadow;	
	COLORREF m_clrText;
	COLORREF m_clrTabBk;
	COLORREF m_clrTabBkSel;	
	COLORREF m_clrBkArea;	
	BOOL m_colorset;
	BOOL m_colorset9;
};

class CDSTabWhidbeyWnd : public CExtTabWhidbeyWnd
{
	DECLARE_DYNAMIC(CDSTabWhidbeyWnd)
public:
	CDSTabWhidbeyWnd::CDSTabWhidbeyWnd()
	{
		m_colorset = FALSE;
		m_colorset9 = FALSE;
	};
	virtual void OnTabWndEraseClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode
		);
	virtual void OnTabWndQueryItemColors(
		LONG nItemIndex,
		bool bSelected,
		bool bHover,
#ifndef PROFUIS240
		bool bEnabled, //**
#endif
		COLORREF * pclrBorder = NULL,
		COLORREF * pclrBkLight = NULL,
		COLORREF * pclrBkDark = NULL,
		COLORREF * pclrText = NULL
		);
	COLORREF m_clrBorder;
	COLORREF m_clrText;
	COLORREF m_clrBkLight;
	COLORREF m_clrBkDark;
	COLORREF m_clrBkLightSel;
	COLORREF m_clrBkDarkSel;
	COLORREF m_clrBkArea;	
	BOOL m_colorset;
	BOOL m_colorset9;
};


class CDsTabCtrl : public CExtTabPageContainerFlatWnd
#else
class CDsTabCtrl : public CTabCtrl
#endif
#endif
{
#ifdef DSPROFUIS
	DECLARE_DYNAMIC(CDsTabCtrl)
#endif
// Construction
public:
	CDsTabCtrl();

// Attributes
public:
	BOOL m_noevento;
	BOOL m_estab9;

// Operations
public:
	void SetFonts(CFont* pSelFont, CFont* pUnselFont);
	void SetFonts(int nSelWeight=FW_SEMIBOLD, BOOL bSelItalic=FALSE,   BOOL bSelUnderline=FALSE,
				  int nUnselWeight=FW_MEDIUM, BOOL bUnselItalic=FALSE, BOOL bUnselUnderline=FALSE);

	void SetColours(COLORREF bSelColour, COLORREF bUnselColour);
	
    LONG InsertItem(int nItem, TCITEM* pTabCtrlItem);
	BOOL SetItemExtra(int nBytes);
	BOOL SetItem(int nItem,TCITEM* pTabCtrlItem);
	
	int GetCurSel( );
	int GetItemCount( );
	int SetCurSel(int nItem );
	BOOL DeleteItem( int nItem );
	BOOL DeleteAllItems( );
	void AdjustRect( BOOL bLarger, LPRECT lpRect);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

#ifdef DSPROFUIS
	virtual CExtTabWnd* OnTabWndGetTabImpl();	
	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		);
	CExtTabWnd* DameCExtTabWnd();
#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsTabCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#ifndef DSPROFUIS
#ifndef DSXTLIB
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
#endif
#endif
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_crSelColour, m_crUnselColour;
	CFont    m_SelFont,	m_UnselFont;
public:
	HFONT m_stdfont;
	BOOL CambiaDeOpcion(int opcion,BOOL relativa = FALSE);
	BOOL EliminaOpcion(CDsStdDlg *pdlg);
	BOOL MuestraOpcion(int opcion);
	BOOL OcultaOpcion(int opcion);
	BOOL SetDlgActivo(CDsStdDlg *pdlg);
	BOOL OpcionTab(int opcion,CDsStdDlg *w);
	virtual ~CDsTabCtrl();

    CArray< CDsStdDlg*, CDsStdDlg* > m_dlgs;
	CArray< CString, CString > m_textos;
	CArray< int, int > m_flag;

    int m_popcion;

    CDsStdCtrl *m_dsctrl;

    CDsxDoc *m_pDoc;
    

	// Generated message map functions
protected:    
	//{{AFX_MSG(CDsTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSTABCTRL_H__C44D18B6_9DD3_11D3_B2AC_004F4905CBCF__INCLUDED_)
