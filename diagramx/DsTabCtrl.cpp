// DsTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsxDoc.h"
#include "DsTabCtrl.h"
#include "DsStdDlg.h"
#include "DsTab9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsTabCtrl
#ifdef DSXTLIB

BEGIN_MESSAGE_MAP(CDsTabCtrl, CXTPTabControl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#else
#ifdef DSPROFUIS

IMPLEMENT_DYNAMIC(CDsTabCtrl, CExtTabPageContainerFlatWnd)

BEGIN_MESSAGE_MAP(CDsTabCtrl, CExtTabPageContainerFlatWnd)
#else
BEGIN_MESSAGE_MAP(CDsTabCtrl, CTabCtrl)
#endif
	//{{AFX_MSG_MAP(CDsTabCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif

CDsTabCtrl::CDsTabCtrl()
{
	m_dsctrl = NULL;
    m_popcion = 0;
    m_pDoc = NULL;

	m_crSelColour     = RGB(0,0,255);
	m_crUnselColour   = RGB(50,50,50);

	m_noevento = FALSE;

	m_estab9 = FALSE;

	m_stdfont = NULL;
}

CDsTabCtrl::~CDsTabCtrl()
{
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
	if (m_stdfont)
		::DeleteObject((HGDIOBJ)m_stdfont);
}

#ifdef DSPROFUIS

IMPLEMENT_DYNAMIC(CDSTabFlatWnd, CExtTabFlatWnd)
IMPLEMENT_DYNAMIC(CDSTabWhidbeyWnd, CExtTabWhidbeyWnd)

void CDSTabFlatWnd::OnFlatTabWndGetItemColors(
		LONG nItemIndex,
		bool bSelected,
#ifndef PROFUIS240
		bool bHover, //**
		bool bEnabled, //**
#endif
		COLORREF &clrLight,
		COLORREF &clrShadow,
		COLORREF &clrDkShadow, 
		COLORREF &clrTabBk, 
		COLORREF &clrText
	)
{ 
	ASSERT_VALID( this );
	nItemIndex;
	bSelected;
	if (!m_colorset)
	{
#ifdef PROFUIS240
		CExtTabFlatWnd::OnFlatTabWndGetItemColors(nItemIndex,bSelected,clrLight,clrShadow,clrDkShadow,clrTabBk,clrText);	
#else
		CExtTabFlatWnd::OnFlatTabWndGetItemColors(nItemIndex,bSelected,bHover,bEnabled,clrLight,clrShadow,clrDkShadow,clrTabBk,clrText);	//**
#endif			
		if (m_colorset9 && theApp.m_colorsettextab9)
			clrText = theApp.m_colortextab9;		
		return;
	}

	clrLight = m_clrLight;			
	clrShadow =  m_clrShadow;			
	clrDkShadow = m_clrDkShadow;			
	clrTabBk = 
			bSelected
				? m_clrTabBkSel
				: m_clrTabBk
				;
	clrText = m_clrText;			
}

void CDSTabFlatWnd::OnTabWndEraseClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode
	)
{

	if (!m_colorset)
	{
		PmBridge_GetPM()->PaintTabClientArea(
			dc,
			rcClient,
			rcTabItemsArea,
			rcTabNearBorderArea,
			dwOrientation,
			bGroupedMode,
			this
			);
		//CExtTabFlatWnd::OnTabWndEraseClientArea(dc,rcClient, rcTabItemsArea,rcTabNearBorderArea,dwOrientation,bGroupedMode);
		return;
	}

	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( !bGroupedMode );

	dc.FillSolidRect(&rcClient,m_clrBkArea/*RGB(192,0,0)*/);
	
	bool bHorz = OrientationIsHorizontal();
	bool bTopLeft = OrientationIsTopLeft();
	CRect rcMargin( rcClient );
	
	if( bHorz )
	{
		if( bTopLeft )
		{
			rcMargin.top = rcMargin.bottom - 1;
		} // if( bTopLeft )
		else
		{
			rcMargin.bottom = rcMargin.top + 1;
		} // else from if( bTopLeft )
	} // if( bHorz )
	else
	{
		if( bTopLeft )
		{
			rcMargin.left = rcMargin.right - 1;
		} // if( bTopLeft )
		else
		{
			rcMargin.right = rcMargin.left + 1;
		} // else from if( bTopLeft )
	} // else from if( bHorz )

	COLORREF clrMargin;
	OnFlatTabWndGetMarginColors(clrMargin);

	dc.FillSolidRect( &rcMargin, clrMargin );
}

#ifdef EJEMPLOS_KK
void CDSTabFlatWnd::OnTabWndEraseClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( !bGroupedMode );

	if( PmBridge_GetPM()->IsKindOf( RUNTIME_CLASS( CExtPaintManagerOffice2003 ) ) )
	{
		PmBridge_GetPM()->PaintTabClientArea(
			dc,
			rcClient,
			rcTabItemsArea,
			rcTabNearBorderArea,
			dwOrientation,
			bGroupedMode,
			this
			);
	}
	else
	{
		dc.FillSolidRect(
			&rcClient,
			PmBridge_GetPM()->GetColor(
				CExtPaintManager::CLR_3DFACE_OUT,
				this
				)
			);
	}
	
bool bHorz = OrientationIsHorizontal();
bool bTopLeft = OrientationIsTopLeft();
CRect rcMargin( rcClient );
	if( bHorz )
	{
		if( bTopLeft )
		{
			rcMargin.top = rcMargin.bottom - 1;
		} // if( bTopLeft )
		else
		{
			rcMargin.bottom = rcMargin.top + 1;
		} // else from if( bTopLeft )
	} // if( bHorz )
	else
	{
		if( bTopLeft )
		{
			rcMargin.left = rcMargin.right - 1;
		} // if( bTopLeft )
		else
		{
			rcMargin.right = rcMargin.left + 1;
		} // else from if( bTopLeft )
	} // else from if( bHorz )
COLORREF clrMargin;
	OnFlatTabWndGetMarginColors( clrMargin );
	dc.FillSolidRect( &rcMargin, clrMargin );
}
#endif


void CDSTabWhidbeyWnd::OnTabWndQueryItemColors(
	LONG nItemIndex,
	bool bSelected,
	bool bHover,
#ifndef PROFUIS240
	bool bEnabled,//**
#endif
	COLORREF * pclrBorder, // = NULL,
	COLORREF * pclrBkLight, // = NULL,
	COLORREF * pclrBkDark, // = NULL,
	COLORREF * pclrText // = NULL
	)
{
	ASSERT_VALID( this );
	bHover;
	nItemIndex;

	if (!m_colorset)
	{	
#ifndef PROFUIS240
		CExtTabWhidbeyWnd::OnTabWndQueryItemColors(nItemIndex,bSelected,bHover,bEnabled,pclrBorder,pclrBkLight,pclrBkDark,pclrText);	//**
#else
		CExtTabWhidbeyWnd::OnTabWndQueryItemColors(nItemIndex,bSelected,bHover,pclrBorder,pclrBkLight,pclrBkDark,pclrText);	
#endif
		if (pclrText != NULL && m_colorset9)
			*pclrText = theApp.m_colortextab9;
		return;
	}

	COLORREF clrBkLight;
	COLORREF clrBkDark;	

	if (bSelected)
	{
		clrBkLight = m_clrBkLightSel;
		clrBkDark  = m_clrBkDarkSel;
	}
	else
	{
		clrBkLight = m_clrBkLight;
		clrBkDark = m_clrBkDark;
	}

	if( pclrBorder != NULL )
		*pclrBorder = m_clrBorder;
	if( pclrBkLight != NULL )
		*pclrBkLight = clrBkLight;
	if( pclrBkDark != NULL )
		*pclrBkDark = clrBkDark;
	if( pclrText != NULL )
		*pclrText = m_clrText;
}

void CDSTabWhidbeyWnd::OnTabWndEraseClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode
	)
{
	if (!m_colorset)
	{
		PmBridge_GetPM()->PaintTabClientArea(
			dc,
			rcClient,
			rcTabItemsArea,
			rcTabNearBorderArea,
			dwOrientation,
			bGroupedMode,
			this
			);
		//CExtTabWhidbeyWnd::OnTabWndEraseClientArea(dc,rcClient, rcTabItemsArea,rcTabNearBorderArea,dwOrientation,bGroupedMode);
		return;
	}
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	rcTabItemsArea;
	bGroupedMode;

	dc.FillSolidRect( 
		&rcClient, 
		m_clrBkArea/*RGB(220,209,215)*/
		);

	if( !rcTabNearBorderArea.IsRectEmpty() )
	{
		CRect rcTabNearMargin( rcTabNearBorderArea ); // prepare tab border margin rect
		switch( dwOrientation )
		{
			case __ETWS_ORIENT_TOP:
				rcTabNearMargin.bottom = rcTabNearMargin.top + 1;
//				rcTabNearMargin.OffsetRect(0,-3);
				rcTabNearMargin.OffsetRect(0,-1);
			break;
			case __ETWS_ORIENT_BOTTOM:
				rcTabNearMargin.top = rcTabNearMargin.bottom - 1;
				rcTabNearMargin.OffsetRect(0,1);
			break;
			case __ETWS_ORIENT_LEFT:
				rcTabNearMargin.right = rcTabNearMargin.left + 1;
//				rcTabNearMargin.OffsetRect(-5,0);
			break;
			case __ETWS_ORIENT_RIGHT:
				rcTabNearMargin.left = rcTabNearMargin.right - 1;
//				rcTabNearMargin.OffsetRect(1,0);
			break;
			default:
				ASSERT( FALSE );
			break;
		} // switch( dwOrientation )

		// paint tab border margin
		dc.FillSolidRect(
			&rcTabNearMargin,
			/*RGB(255,255,255)*/g_PaintManager->GetColor( COLOR_3DSHADOW, this )
			);

	} // if( !rcTabNearBorderArea.IsRectEmpty() )
}


CExtTabWnd* CDsTabCtrl::OnTabWndGetTabImpl()
{
	CDSTabFlatWnd *ptab1 = NULL;
	CDSTabWhidbeyWnd *ptab2 = NULL;
	CDsStdDlg *pdlg;

	if (m_estab9/*IsKindOf(RUNTIME_CLASS(CDsTab9))*/)
	{
			ptab2 = new CExtTWPC < CDSTabWhidbeyWnd >;		
			ptab2->m_colorset9	  = theApp.m_colortextab9;
			ptab2->m_colorset     = theApp.m_colorset_t3;
			ptab2->m_clrBorder    = theApp.m_color_t3[0];
			ptab2->m_clrText      = theApp.m_color_t3[1];
			ptab2->m_clrBkLight   = theApp.m_color_t3[2];
			ptab2->m_clrBkDark    = theApp.m_color_t3[3];
			ptab2->m_clrBkLightSel= theApp.m_color_t3[4];
			ptab2->m_clrBkDarkSel = theApp.m_color_t3[5];
			ptab2->m_clrBkArea    = theApp.m_color_t3[6];
			return ptab2;
	}

	pdlg = (CDsStdDlg *)GetParent();
	if (pdlg->m_sid < 1000)
	{
		if (theApp.m_modotab1 == 0)
		{
			ptab2 = new CExtTWPC < CDSTabWhidbeyWnd >;		
			ptab2->m_colorset     = theApp.m_colorset_t1;
			ptab2->m_clrBorder    = theApp.m_color_t1[0];
			ptab2->m_clrText      = theApp.m_color_t1[1];
			ptab2->m_clrBkLight   = theApp.m_color_t1[2];
			ptab2->m_clrBkDark    = theApp.m_color_t1[3];
			ptab2->m_clrBkLightSel= theApp.m_color_t1[4];
			ptab2->m_clrBkDarkSel = theApp.m_color_t1[5];
			ptab2->m_clrBkArea    = theApp.m_color_t1[6];
			return ptab2;
		}
		else
		{
		    ptab1 = new CExtTWPC < CDSTabFlatWnd >;
			ptab1->m_colorset    = theApp.m_colorset_t1;
			ptab1->m_clrLight    = theApp.m_color_t1[0];
			ptab1->m_clrShadow   = theApp.m_color_t1[1];
			ptab1->m_clrDkShadow = theApp.m_color_t1[2];
			ptab1->m_clrText     = theApp.m_color_t1[3];
			ptab1->m_clrTabBk    = theApp.m_color_t1[4];
			ptab1->m_clrTabBkSel = theApp.m_color_t1[5];
			ptab1->m_clrBkArea   = theApp.m_color_t1[6];
			return ptab1;
		}
	}
	else
	{
		if (theApp.m_modotab2 == 0)
		{
			ptab2 = new CExtTWPC < CDSTabWhidbeyWnd >;		
			ptab2->m_colorset     = theApp.m_colorset_t2;
			ptab2->m_clrBorder    = theApp.m_color_t2[0];
			ptab2->m_clrText      = theApp.m_color_t2[1];
			ptab2->m_clrBkLight   = theApp.m_color_t2[2];
			ptab2->m_clrBkDark    = theApp.m_color_t2[3];
			ptab2->m_clrBkLightSel= theApp.m_color_t2[4];
			ptab2->m_clrBkDarkSel = theApp.m_color_t2[5];
			ptab2->m_clrBkArea    = theApp.m_color_t2[6];
			return ptab2;
		}
		else
		{
		    ptab1 = new CExtTWPC < CDSTabFlatWnd >;						
			ptab1->m_colorset    = theApp.m_colorset_t2;
			ptab1->m_clrLight    = theApp.m_color_t2[0];
			ptab1->m_clrShadow   = theApp.m_color_t2[1];
			ptab1->m_clrDkShadow = theApp.m_color_t2[2];
			ptab1->m_clrText     = theApp.m_color_t2[3];
			ptab1->m_clrTabBk    = theApp.m_color_t2[4];
			ptab1->m_clrTabBkSel = theApp.m_color_t2[5];
			ptab1->m_clrBkArea   = theApp.m_color_t2[6];
			return ptab1;
		}
	}	
}

CExtTabWnd* CDsTabCtrl::DameCExtTabWnd()
{
	return m_pWndTab;
}

#endif

int CDsTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
#ifdef DSPROFUIS
	if (CExtTabPageContainerFlatWnd::OnCreate(lpCreateStruct) == -1) return -1;

	OrientationSet(__ETWS_ORIENT_TOP);	

	CDsStdDlg *pdlg = (CDsStdDlg *)GetParent();
	if (pdlg->IsKindOf(RUNTIME_CLASS(CDsStdDlg)))
	{
		if (pdlg->m_sid < 1000)
		{
			ItemsHasInclineAfterSet(false);
			ItemsHasInclineBeforeSet(false);
		}
	}

#else
#ifdef DSXTLIB
	if (CXTPTabControl::OnCreate(lpCreateStruct) == -1) return -1;
#else
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1) return -1;
	//ModifyStyle(0, TCS_BUTTONS);
	// ModifyStyle(0, TCS_OWNERDRAWFIXED);
#endif	
#endif	
	return 0;
}

void CDsTabCtrl::PreSubclassWindow() 
{	
#ifdef DSPROFUIS
	CExtTabPageContainerFlatWnd::PreSubclassWindow();
#else
#ifdef DSXTLIB	
	CXTPTabControl::PreSubclassWindow();
#else
	CTabCtrl::PreSubclassWindow();
	//ModifyStyle(0, TCS_BUTTONS);
	//ModifyStyle(0, TCS_OWNERDRAWFIXED);
#endif
#endif
}

#ifndef DSPROFUIS
#ifndef DSXTLIB
void CDsTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CRect rect = lpDrawItemStruct->rcItem;
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0) return;
	BOOL bSelected = (nTabIndex == GetCurSel());

	char label[64];
	TC_ITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = label;     
	tci.cchTextMax = 63;    	
	if (!GetItem(nTabIndex, &tci )) return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC) return;
	int nSavedDC = pDC->SaveDC();

	// For some bizarre reason the rcItem you get extends above the actual
	// drawing area. We have to workaround this "feature".
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

	// Draw image
	CImageList* pImageList = GetImageList();
	if (pImageList && tci.iImage >= 0) 
	{

		rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin

		// Get height of image so we 
		IMAGEINFO info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	if (bSelected) 
	{
		pDC->SetTextColor(m_crSelColour);
		pDC->SelectObject(&m_SelFont);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	} else 
	{
		pDC->SetTextColor(m_crUnselColour);
		pDC->SelectObject(&m_UnselFont);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
	}

	pDC->RestoreDC(nSavedDC);
}
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsTabCtrl operations

void CDsTabCtrl::SetColours(COLORREF bSelColour, COLORREF bUnselColour)
{
	m_crSelColour = bSelColour;
	m_crUnselColour = bUnselColour;
	Invalidate();
}

void CDsTabCtrl::SetFonts(CFont* pSelFont, CFont* pUnselFont)
{
	ASSERT(pSelFont && pUnselFont);

	LOGFONT lFont;
	int nSelHeight, nUnselHeight;

	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	pSelFont->GetLogFont(&lFont);
	m_SelFont.CreateFontIndirect(&lFont);
	nSelHeight = lFont.lfHeight;

	pUnselFont->GetLogFont(&lFont);
	m_UnselFont.CreateFontIndirect(&lFont);
	nUnselHeight = lFont.lfHeight;

	SetFont( (nSelHeight > nUnselHeight)? &m_SelFont : &m_UnselFont);
}


void CDsTabCtrl::SetFonts(int nSelWeight,   BOOL bSelItalic,   BOOL bSelUnderline,
						  int nUnselWeight, BOOL bUnselItalic, BOOL bUnselUnderline)
{
	// Free any memory currently used by the fonts.
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	// Get the current font
	LOGFONT lFont;
	CFont *pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	else {
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
		lFont = ncm.lfMessageFont; 
	}

	// Create the "Selected" font
	lFont.lfWeight = nSelWeight;
	lFont.lfItalic = bSelItalic;
	lFont.lfUnderline = bSelUnderline;
	m_SelFont.CreateFontIndirect(&lFont);

	// Create the "Unselected" font
	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bUnselItalic;
	lFont.lfUnderline = bUnselUnderline;
	m_UnselFont.CreateFontIndirect(&lFont);

	SetFont( (nSelWeight > nUnselWeight)? &m_SelFont : &m_UnselFont);
}

/////////////////////////////////////////////////////////////////////////////
// CDsTabCtrl message handlers



int CDsTabCtrl::GetCurSel( )
{
#ifdef DSPROFUIS
	return PageSelectionGet();
#else
	return CTabCtrl::GetCurSel();
#endif
}

int CDsTabCtrl::GetItemCount( )
{
#ifdef DSPROFUIS
	return PageGetCount();
#else
	return CTabCtrl::GetItemCount();
#endif
}

int CDsTabCtrl::SetCurSel(int nItem )
{
#ifdef DSPROFUIS
	PageSelectionSet(nItem);
	return nItem;
#else
	return CTabCtrl::SetCurSel(nItem);
#endif
}

BOOL CDsTabCtrl::DeleteItem(int nItem )
{
#ifdef DSPROFUIS
	 //int sel = -1;
	 //::LockWindowUpdate(m_hWnd);
	 /*
	 if (GetItemCount() > 0)
		sel = GetCurSel();
	*/
	
	 m_noevento = TRUE;
	 m_pWndTab->ItemRemove(nItem,1,true);
     //PageRemove(nItem,1,FALSE);
	 /*
	 if (sel > -1 && sel < GetItemCount() && sel != GetCurSel())
		SetCurSel(sel);
	*/
	
	 m_noevento = FALSE;
	 //::LockWindowUpdate(NULL);
	 return TRUE;
#else
	return CTabCtrl::DeleteItem(nItem);
#endif
}

BOOL CDsTabCtrl::DeleteAllItems( )
{
#ifdef DSPROFUIS	 
     PageRemoveAll(FALSE);	 
	 return TRUE;
#else
	return CTabCtrl::DeleteAllItems( );
#endif
}


LONG CDsTabCtrl::InsertItem(int nItem, TCITEM* pTabCtrlItem)
{
	CString s;
	s = (const char *)pTabCtrlItem->pszText;
	m_textos.SetAtGrow(nItem,s);
#ifdef DSPROFUIS
	return nItem;
#else
#ifdef DSXTLIB
	if (CXTPTabControl::InsertItem(nItem,pTabCtrlItem->pszText))
	{
		RedrawControl();
		return nItem;
	}
	return -1;	
#else
	return CTabCtrl::InsertItem(nItem,pTabCtrlItem);     
#endif
#endif
}

BOOL CDsTabCtrl::SetItemExtra(int nBytes)
{
#ifdef DSPROFUIS
	return FALSE;
#else
#ifdef DSXTLIB
	return FALSE;
#else
	return CTabCtrl::SetItemExtra(nBytes);
#endif
#endif
}

BOOL CDsTabCtrl::SetItem(int nItem,TCITEM* pTabCtrlItem)
{
#ifdef DSPROFUIS
	m_textos[nItem] = (const char *)pTabCtrlItem->pszText;
	PageTextSet(nItem,pTabCtrlItem->pszText,TRUE);
	return TRUE;
#else
#ifdef DSXTLIB
	CXTPTabManagerItem* pi = CXTPTabControl::GetItem(nItem);
	if (pi)
	{
	   pi->SetCaption(pTabCtrlItem->pszText);
	   return TRUE;
	}
	return FALSE;
#else
	return CTabCtrl::SetItem(nItem,pTabCtrlItem);
#endif
#endif
}

void CDsTabCtrl::AdjustRect( BOOL bLarger, LPRECT lpRect)
{
#ifdef DSPROFUIS	
	CRect r;
	m_pWndTab->GetWindowRect(r);
	lpRect->top -= r.Size().cy;	
#else
#ifdef DSXTLIB
	return;
#else
	CTabCtrl::AdjustRect( bLarger, lpRect);
#endif
#endif
}

BOOL CDsTabCtrl::OpcionTab(int opcion, CDsStdDlg *w)
{
   TCITEM Item;   

   opcion -= m_popcion;   
   if (opcion < 0) return FALSE;

   int total = GetItemCount();   

   for (int i = total;i < opcion;i++)
   {
      Item.mask = TCIF_TEXT;
      Item.pszText = "";
      Item.cchTextMax = 0;  
      Item.lParam = NULL;
      InsertItem(i,&Item);
   }   
 
   Item.mask = TCIF_TEXT ;
   Item.pszText = (char *)(LPCSTR)w->m_nombre;
   Item.cchTextMax = strlen(w->m_nombre);   

   m_textos.SetAtGrow(opcion,CString(Item.pszText));
   m_dlgs.SetAtGrow(opcion,w);   
   m_flag.SetAtGrow(opcion,0);   

   if (total <= opcion)
   {      
#ifdef DSPROFUIS
	  PageInsert(w,Item.pszText,NULL,true,opcion,false);
#else
      InsertItem(opcion,&Item);     
#endif
   }
   else   
   {
      SetItem( opcion,&Item);
#ifdef DSPROFUIS
	  PageHwndSet(opcion,w->m_hWnd);
#endif
   }

   w->m_padretab = this;
   w->m_opcionpadre = opcion;   

   return TRUE;
}

#ifdef DSPROFUIS
bool CDsTabCtrl::OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		)
{
	if (nNewItemIndex >= 0 && nNewItemIndex < m_flag.GetCount() && (m_flag[nNewItemIndex]&1))
		return false;

	bool ret = CExtTabPageContainerFlatWnd::OnTabWndSelectionChange(nOldItemIndex,nNewItemIndex,bPreSelectionTest);
	if (ret && !bPreSelectionTest && nOldItemIndex != nNewItemIndex && !m_noevento)
	{
        CDsStdDlg *w = m_dlgs[nNewItemIndex];
        if (w && ::IsWindow(w->m_hWnd))
        {			
			w->DisparaEventoUsuario();       			
        }
	}
	return ret;
}
#endif

void CDsTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
    MuestraOpcion(GetCurSel());
	*pResult = 0;
}

void CDsTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
    
	OcultaOpcion(GetCurSel());
	*pResult = 0;
}

BOOL CDsTabCtrl::SetDlgActivo(CDsStdDlg *pdlg)
{
    int total = GetItemCount();
    for (int i = 0;i < total;i++)
    {
        if (pdlg == m_dlgs[i])
        {
            int op = GetCurSel();
            if (op != i)
            {
                OcultaOpcion(op);
                SetCurSel(i);
                MuestraOpcion(i);
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CDsTabCtrl::OcultaOpcion(int opcion)
{
    if (opcion >= 0)
    {
        CDsStdDlg *w = m_dlgs[opcion];
        if (w && ::IsWindow(w->m_hWnd))
        {
            w->ShowWindow(SW_HIDE);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CDsTabCtrl::MuestraOpcion(int opcion)
{
    if (opcion >= 0)
    {
        CDsStdDlg *w = m_dlgs[opcion];
        if (w && ::IsWindow(w->m_hWnd))
        {			
            w->ShowWindow(SW_SHOW);			
			w->DisparaEventoUsuario();			
            return TRUE;
        }
    }	
    return FALSE;
}

BOOL CDsTabCtrl::EliminaOpcion(CDsStdDlg *pdlg)
{
    if (pdlg->m_padretab != this) return FALSE;

	if (m_dsctrl && m_dsctrl->m_muriendo)
	{
		return FALSE;
	}	

    int total = GetItemCount();
    for (int i = 0;i < total;i++)
    {
        if (pdlg == m_dlgs[i] && pdlg->m_opcionpadre == i)
        {
           total--;
           for (int j = i;j < total;j++)
           {
               m_dlgs[j] = m_dlgs[j+1];
			   m_flag[j] = m_flag[j+1];
               if (m_dlgs[j])
               {
                  m_dlgs[j]->m_opcionpadre = j;
               }
           }
           DeleteItem(i);

           if (!total)
           {
               m_dsctrl->m_pdlg->DestruyeControl(m_dsctrl->m_ctrlid);
           }
           return TRUE;
        }
    }
    return FALSE;
}

void CDsTabCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_dsctrl && m_dsctrl->m_muriendo)
	{
		return;
	}	
#ifdef DSPROFUIS
	CExtTabPageContainerFlatWnd::OnSetFocus(pOldWnd);
#else
#ifdef DSXTLIB	
	CXTPTabControl::OnSetFocus(pOldWnd);
#else
	CTabCtrl::OnSetFocus(pOldWnd);
#endif
#endif
	//m_dsctrl->TeclaStd(999);
	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
	theApp.m_ctrlactivo = m_dsctrl;
	
	if (m_dsctrl)
		m_dsctrl->EnsureVisible();
	
}

void CDsTabCtrl::OnKillFocus(CWnd* pNewWnd) 
{
#ifdef DSPROFUIS
	CExtTabPageContainerFlatWnd::OnKillFocus(pNewWnd);
#else

	/*
    if (theApp.m_ctrlactivo == m_dsctrl)
    {
        theApp.m_antctrlactivo = theApp.m_ctrlactivo;
        theApp.m_ctrlactivo = NULL;
    }
	*/
#ifdef DSXTLIB	
	CXTPTabControl::OnKillFocus(pNewWnd);
#else
	CTabCtrl::OnKillFocus(pNewWnd);
#endif	
#endif
}


BOOL CDsTabCtrl::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->hwnd == m_hWnd && pMsg->message == WM_KEYDOWN)
    {
        int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
		if (comando == 802)
		{
		    CDsStdDlg *w = m_dlgs[GetCurSel()];
			if (w && ::IsWindow(w->m_hWnd))
			{
				int key = 0;
				CDsStdCtrl *ctrl = NULL;
				CDsStdCtrl *elegido = NULL;
				int lpm = 9999;
				POSITION pos = w->m_ctrlst.GetStartPosition( );
				while (pos)
				{
					w->m_ctrlst.GetNextAssoc( pos, key,ctrl);
					if (ctrl && (ctrl->m_tipo == 2 || ctrl->m_tipo == 4 || ctrl->m_tipo == 7))
					{
						DWORD style = ::GetWindowLong(ctrl->m_cthwnd,GWL_STYLE);
						if ( !(style & WS_TABSTOP) || !(style & WS_VISIBLE) )
						{
							continue;
						}
						if (ctrl->m_lpi < lpm)
						{
							elegido = ctrl;
							lpm = ctrl->m_lpi;
						}
					}
				}
				if (elegido)
				{
					::SetFocus(elegido->m_cthwnd);
				}
				return TRUE;
			}			
		}
		else if (comando == 804 || comando == 805 || comando == 807 || comando == 806)
        {
           if (theApp.ActivacionPorDefecto(NULL))
           {
               if (comando != 802)
               {
                    theApp.TeclaAlActivo(comando);
               }
               return TRUE;
           }
           else
           {
               if (comando != 802)
               {
                  m_dsctrl->TeclaStd(comando);
                  return TRUE;
               }
           }
        }
    }
#ifdef DSPROFUIS
	return CExtTabPageContainerFlatWnd::PreTranslateMessage(pMsg);
#else
#ifdef DSXTLIB		
	return CXTPTabControl::PreTranslateMessage(pMsg);
#else
	return CTabCtrl::PreTranslateMessage(pMsg);
#endif		
#endif
}

BOOL CDsTabCtrl::CambiaDeOpcion(int opcion, BOOL relativa)
{
   if (!relativa)
   {
       if (opcion >= 0 && opcion < GetItemCount())
       {
            OcultaOpcion(GetCurSel());
            SetCurSel(opcion);
            MuestraOpcion(GetCurSel());
            return TRUE;
       }
       return FALSE;
   }
   int actual = GetCurSel();
   actual += opcion;
   if (actual < 0)
   {
       actual = GetItemCount()-1;
   }
   else if (actual >= GetItemCount())
   {
       actual = 0;
   }
   if (actual == GetCurSel()) return FALSE;
   OcultaOpcion(GetCurSel());
   SetCurSel(actual);
   MuestraOpcion(GetCurSel());
   return TRUE;
}

BOOL CDsTabCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ((wParam & 0xffff) == ID_COMMANDCONTROL)
	{
		int *p1 = (int *)lParam;
		char *data = (char *)(p1+1);		
		int item = p1[0];
		if (item < 0 || item >= m_flag.GetCount())
		{
			p1[0] = -1;
			return TRUE;
		}
		if (!strcmp(data,"ENABLE"))		
			m_flag[item] &= ~1;		
		else if (!strcmp(data,"DISABLE"))
			m_flag[item] |= 1;
		else
		{
			if ((m_flag[item] & 1))			
			    strcpy(data,"DISABLED");
			else
				strcpy(data,"ENABLED");			
			return TRUE;
		}		
		if ((m_flag[item] & 1))			
			strcpy(data,"DISABLED");
		else
			strcpy(data,"ENABLED");			
		return TRUE;
	}
    else if ((wParam & 0xffff) == ID_CUSTOM_ESTADO)
    {		
		if (lParam == 1001)
		{
			//m_noesconsulta = FALSE;
		} else if (lParam == 1100)
		{
			//m_noesconsulta = TRUE;
		}
		return TRUE;
	}

	return CDsTabCtrl::OnCommand(wParam, lParam);
}
