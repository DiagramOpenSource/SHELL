// DsxMainFrm.cpp : implementation of the CDsxFrame class
//

#include "stdafx.h"
#include "diagramx.h"

#include "DsxMainFrm.h"
#include "DsxDoc.h"
#include "DsxLeftView.h"
#include "dsxView.h"
#include "DsTreeMenu.h"
#include "DlgPregunta.h"
#include "DlgKActiva.h"
#include "DlgIndexado.h"
#include "DsStdDlg.h"
#include "Ds9Window.h"
#include "DsTab9.h"
#include "TablaView.h"
#include "EnviaCorreo.h"
#include "DlgOldImpre.h"
#include "DsImagen.h"
#include "DsEdit.h"
#include "DsCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDsStdCtrl *dispara_botones[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

#define WM_MITRAYMESSAGE (WM_USER+7)

#ifdef DSPROFUIS

#define __XP_DSS_MONO DSS_MONO

IMPLEMENT_DYNCREATE( CDsExtButton, CExtButton )

CDsExtButton::CDsExtButton()	
{	
/*
m_clrBackground
m_clrTextDisabled
m_clrTextNormal
m_clrTextHover
m_clrTextPressed
*/
}

IMPLEMENT_DYNCREATE(CDsProfuisCustomOffice2007_R1, CExtPaintManagerOffice2007_R1)

CDsProfuisCustomOffice2007_R1::CDsProfuisCustomOffice2007_R1()
{
    m_colorset = FALSE;

	/*
	m_bmpDCA

	m_bmpNcFrameActive

	m_bmpNcFrameInactive

	m_bmpNcButtonBkHover

	m_bmpNcButtonBkPressed

	m_bmpArrNcButtonsClose

	m_bmpArrNcButtonsMaximize

    m_bmpArrNcButtonsRestore

	m_bmpArrNcButtonsMinimize

	m_bmpStatusBkLight

	m_bmpStatusBkDark

	m_bmpStatusSeparator

	m_arrBmpTabShapeNormal[__ETSOI_TOP]

	m_arrBmpTabSeparator[__ETSOI_TOP]

	m_bmpArrStatesCheckBox

	m_bmpArrStatesRadioButton

	m_bmpMenuItemLargeLeft

	m_bmpMenuItemLargeRight

	m_bmpMenuItemSmallLeft

	m_bmpMenuItemSmallRight

	m_bmpMenuArrow

	m_bmpMenuCheckAreaChecked

	m_bmpMenuCheckAreaUnchecked

	m_bmpArrComboBoxDropDown

	m_arrBmpPushBtnSimpleH

	m_arrBmpPushBtnSDDLeftH

	m_arrBmpPushBtnSDDRightH

	m_bmpRibbonPageBk

	m_bmpRibbonGroupBkExpanded[0]

	m_bmpRibbonGroupBkExpanded[1]

	m_bmpRibbonGroupBkCollapsed[0]

	m_bmpRibbonGroupBkCollapsed[1]

	m_bmpRibbonGroupBkCollapsed[2]

	m_bmpRibbonGroupBkCollapsed[3]

	m_bmpRibbonSeparatorH

	m_bmpRibbonSeparatorV

	m_bmpArrRibbonQACEB

	m_bmpArrRibbonFileButtonR1

	m_bmpRibbonFileButtonArea

	m_bmpRibbonQuickAccessBarBkAtTheBottom

	m_arrBmpRibbonDLB

	m_bmpArrRibbonGalleryScrollButtonDown

	m_bmpArrRibbonGalleryScrollButtonUp

	m_bmpArrRibbonGalleryScrollButtonMenu

	m_bmpRibbonDDA

	m_bmpArrButtonTS[INT(__EBTSA_SOLID)]

	m_bmpArrButtonTS[INT(__EBTSA_LEFT)]

	m_bmpArrButtonTS[INT(__EBTSA_MIDDLE)]

	m_bmpArrButtonTS[INT(__EBTSA_RIGHT)]

	m_bmpArrButtonSS_DD[INT(__EBSSA_LEFT)]

	m_bmpArrButtonSS_DD[INT(__EBSSA_RIGHT)]

	m_bmpArrButtonLSM

	m_bmpArrButtonLSXM

	m_bmpArrButtonLSXB

	m_bmpToolBtnSeparator[INT(__ETBS_HOVER)]

	m_bmpToolBtnSeparator[INT(__ETBS_PRESSED)]

	m_bmpLargeBtnSeparator[INT(__ELBS_HOVER)]

	m_bmpLargeBtnSeparator[INT(__ELBS_PRESSED)]

	m_bmpRibbonModeMenuBorder

	m_bmpMenuCaptionBk

	m_bmpRibbonFileMenuBigBorder

	m_bmpRibbonFileMenuOptionsButtonHot

	m_bmpResizingGripper

	m_arrBmpSpinArrowDown

	m_arrBmpSpinArrowUp

	m_arrBmpSpinDown

	m_arrBmpSpinUp


	//m_bmpNcFrameActive.LoadBMP_Resource( MAKEINTRESOURCE(IDB_EXT_2007_NC_FRAME_ACTIVE_R1) ) 
    m_bmpNcFrameActive.LoadBMP_File("C:\\prueba.bmp",true);// = FrameActive;

	VERIFY( m_bmpNcFrameActive.Make32() );
	m_bmpNcFrameActive.AlphaColor( m_clrTransparentNcFrameActive, RGB(0,0,0), 0 );
	m_bmpNcFrameActive.PreMultiplyRGBChannels();
	m_bmpNcFrameActive.PreMultipliedRGBChannelsSet( true );
    */

};


void CDsProfuisCustomOffice2007_R1::InitTranslatedColors()
{
	CExtPaintManagerOffice2007_R1::InitTranslatedColors();
	/* el codigo de la clase despues de la revision eliminado ... no hace falta ... */
}


bool CDsProfuisCustomOffice2007_R1::DSPaintDockerBkgnd2003(
	bool bClientMapping,
	CDC & dc,
	const CRect &rcDst,
	const CRect &rcWnd,
	COLORREF micolor
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	rcWnd;
	LPARAM lParam = NULL;
	bClientMapping;
COLORREF clrLeft = stat_HLS_Adjust( micolor, 0.02, 0.07, 0.00 );
COLORREF clrRight = stat_HLS_Adjust( micolor, 0.00, 0.43, 0.00 );
CRect rcDst1( rcDst );
CRect rcDst2( 0, 0, 0, 0 );
bool bRTL = ( (dc.GetLayout()&LAYOUT_RTL) != 0 ) ? true : false;
	if( rcDst.Width() >= 686 ) 
	{
		if( bRTL )
		{
			rcDst2 = rcDst;
			rcDst2.right = rcDst2.left + rcDst1.Width() - 686;
			rcDst1 = rcDst;
			rcDst1.left = rcDst2.right;
		} // if( bRTL )
		else
		{
			rcDst1.right = rcDst1.left + 686;
			rcDst2 = rcDst;
			rcDst2.left = rcDst1.right;
		} // else from if( bRTL )
	} // if( rcDst.Width() >= 686 ) 
	stat_PaintGradientRect(
		dc,
		rcDst1,
		bRTL ? clrRight : clrLeft,
		bRTL ? clrLeft : clrRight,
		false,
		64
		);
	if( ! rcDst2.IsRectNull() )
		dc.FillSolidRect( &rcDst2, clrRight );

	return true;
}

bool CDsProfuisCustomOffice2007_R1::DSPaintDockerBkgnd(
	bool bClientMapping,
	CDC & dc,
	const CRect &rcDst,
	const CRect &rcWnd,
	COLORREF micolor
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bClientMapping;
	rcWnd;	
	if( ! m_bUseNcFrameBmpForDockerBkgnd )
	{
		return
			DSPaintDockerBkgnd2003(
				bClientMapping,
				dc,
				rcDst,
				rcWnd,
				micolor
				);
	}

int nOldStretchBltMode = ::GetStretchBltMode( dc.m_hDC );
	::SetStretchBltMode(
		dc.m_hDC,
		( g_PaintManager.m_bIsWinNT )
			? HALFTONE
			: COLORONCOLOR
		);

CSize _size = m_bmpNcFrameActive.GetSize();
CRect rcSrc(
		m_rcNcFrameBordersActive.left,
		m_rcNcFrameBordersActive.top + m_nCaptionHeightActive,
		_size.cx - m_rcNcFrameBordersActive.right,
		_size.cy - m_rcNcFrameBordersActive.bottom
		);
	if( g_PaintManager.m_bIsWin2000orLater )
	{
		if( m_bmpNcFrameActive.Draw(
				dc.m_hDC,
				rcDst,
				rcSrc
				) == GDI_ERROR
			)
			CExtPaintManagerOffice2003::PaintDockerBkgnd(
				bClientMapping,
				dc,
				rcDst,
				rcWnd				
				);
// 			dc.FillSolidRect(
// 				&rcDst,
// 				m_clrSolidDockerBk
// 				);
	}
	else
	{
		if( m_bmpNcFrameActive.AlphaBlend(
				dc.m_hDC,
				rcDst.left,
				rcDst.top,
				rcDst.Width(),
				rcDst.Height(),
				rcSrc.left,
				rcSrc.top,
				rcSrc.Width(),
				rcSrc.Height()
				) == GDI_ERROR
			)
			CExtPaintManagerOffice2003::PaintDockerBkgnd(
				bClientMapping,
				dc,
				rcDst,
				rcWnd				
				);
// 			dc.FillSolidRect(
// 				&rcDst,
// 				m_clrSolidDockerBk
// 				);
	}

	::SetStretchBltMode(
		dc.m_hDC,
		nOldStretchBltMode
		);

//	__EXT_PROFUIS_PAINT_EVALUATION_LOGO( dc, &rcDst, bClientMapping, NULL );

	return true;
}


bool CDsProfuisCustomOffice2007_R1::DSPaintDockerBkgnd(
	bool bClientMapping,
	CDC & dc,
	CWnd * pWnd,
	COLORREF micolor
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL && ::IsWindow(pWnd->GetSafeHwnd()) );


CWnd * pFrame = pWnd;
	for(	;
			pFrame != NULL;
			pFrame = pFrame->GetParent()
			)
	{
		if( (pFrame->GetStyle() & WS_CHILD) == 0 )
		{
// (- 2.25)
//			if(! pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
//				return false;
			break;
		}
		else // frame features - design mode
			if( pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
				break;
		if(		pFrame != pWnd
			&&	pFrame->IsKindOf( RUNTIME_CLASS(CExtToolControlBar) )
			)
		{
			if( ((CExtToolControlBar*)pFrame)->m_pDockSite != NULL )
			{
				if(	!(	pFrame->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
					||	pFrame->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
						)
					)
				{
					if( ((CExtToolControlBar*)pFrame)->m_pDockBar == NULL )
						continue;
				}
			}
			break;
		}
	}
	if( pFrame == NULL )
		return false;


CRect rcPaintGradient;
	pFrame->GetClientRect( &rcPaintGradient );
	pFrame->ClientToScreen( &rcPaintGradient );

	if( bClientMapping )
	{
		pWnd->ScreenToClient( &rcPaintGradient );
	} // if( bClientMapping )
	else
	{
		CRect rc;
		pWnd->GetWindowRect( &rc );
		rcPaintGradient.OffsetRect( -rc.left, -rc.top );
	} // else from if( bClientMapping )

		CRect rcWnd;
		pWnd->GetClientRect( &rcWnd );
		pFrame->ClientToScreen( &rcWnd );
		return DSPaintDockerBkgnd( 
			bClientMapping,
			dc,  
			rcPaintGradient,
			rcWnd,
			micolor
			);
}

#define __ExtMfc_CXTEXTMARGIN (-1) // 1
#define __ExtMfc_CYTEXTMARGIN (0)  // 1

void CDsProfuisCustomOffice2007_R1::PaintPushButton(
	CDC & dc,
	CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
	)
{
	if (_ppbd.m_pHelperSrc == NULL || !_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtButton)))
	{
		CExtPaintManagerOffice2007_R1::PaintPushButton(dc,_ppbd);	
		return;
	}	

	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );


	if( _ppbd.m_clrForceTextDisabled == COLORREF(-1L) )
		_ppbd.m_clrForceTextDisabled = RGB(140,142,140);

	if( _ppbd.m_rcClient.IsRectEmpty() )
		return;
	if(		dc.GetDeviceCaps(BITSPIXEL) <= 8
		||	_ppbd.m_pHelperSrc == NULL
		)
	{
		CExtPaintManagerOffice2003::PaintPushButton( dc, _ppbd );
		return;
	}

	if(		_ppbd.m_pHelperSrc != NULL
		&&	_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) )
		&&	( ((CExtPopupMenuWnd*)_ppbd.m_pHelperSrc)->TrackFlagsGet() & TPMX_RIBBON_MODE ) == 0
		)
	{
		CExtPaintManagerOffice2003::PaintPushButton( dc, _ppbd );
		return;
	}

CExtBarButton * pTBB = DYNAMIC_DOWNCAST( CExtBarButton, _ppbd.m_pHelperSrc );
	if( pTBB != NULL )
	{
#if (!defined __EXT_MFC_NO_RIBBON_BAR)
		if( pTBB->IsRibbonPaintingMode() )
		{
			Ribbon_PaintPushButton( dc, _ppbd );
			return;
		}
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)
		CExtToolControlBar * pBar = pTBB->GetBar();
		if( pBar == NULL )
		{
			CExtPaintManagerOffice2003::PaintPushButton( dc, _ppbd );
			return;
		}
#if (!defined __EXT_MFC_NO_RIBBON_BAR)
		if(		pBar->IsKindOf( RUNTIME_CLASS( CExtRibbonPage ) )
			&&	(! pTBB->IsNoRibbonLayout() )
			&&	(! pTBB->IsKindOf( RUNTIME_CLASS( CExtBarMdiRightButton ) ) )
			)
		{
			Ribbon_PaintPushButton( dc, _ppbd );
			return;
		}
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)
		CExtPaintManagerOffice2003::PaintPushButton( dc, _ppbd );
		return;
	} // if( pTBB != NULL )

enum ButtonDrawingMode
{
	eModeControl,
	eModeToolBar,
	eModeMenuBar,
	eModePopupMenu,
	eModeDatePicker,
	eModeToolBox,
};

bool bHorizontal = true;

ButtonDrawingMode aDrawingMode = eModeControl;
	if( _ppbd.m_pHelperSrc != NULL  )
	{
		if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarButton ) ) )
		{
			CExtBarButton * pBarButton = (CExtBarButton*)_ppbd.m_pHelperSrc;
			CExtToolControlBar * pToolBar = DYNAMIC_DOWNCAST( CExtToolControlBar, pBarButton->GetBar() );
			ASSERT( pToolBar != NULL );
			
			if( pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
				aDrawingMode = eModeMenuBar;
			else
				aDrawingMode = eModeToolBar;
			if( ! pBarButton->GetNoRotateVerticalLayout() )
			{
				if( pToolBar->IsDockedVertically() )
					bHorizontal = false;
			}
		}
		else if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtPopupMenuWnd ) ) )
		{
			aDrawingMode = eModePopupMenu;
		}
#ifndef __EXT_MFC_NO_DATE_PICKER
		else if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtDatePickerWnd ) ) )
		{
			aDrawingMode = eModeDatePicker;
		}
#endif // #ifndef __EXT_MFC_NO_DATE_PICKER
#ifndef __EXT_MFC_NO_TOOLBOX_CTRL
		else if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtToolBoxWnd ) ) )
		{
			aDrawingMode = eModeToolBox;
		}
#endif // #ifndef __EXT_MFC_NO_TOOLBOX_CTRL
		else if(
				_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CWnd ) )
			&&	CExtPopupMenuWnd::g_pCombinedHelper == NULL
			&&	(!_ppbd.m_bTransparentBackground)
			)
		{
			CExtPaintManagerOffice2003::PaintDockerBkgnd( true, dc, (CWnd*)_ppbd.m_pHelperSrc );
		}
	}

	if(		aDrawingMode != eModeControl 
		&&	aDrawingMode != eModeDatePicker
		&&	aDrawingMode != eModeToolBox
		&&	aDrawingMode != eModeToolBar
		)
	{
		CExtPaintManagerOffice2003::PaintPushButton( dc, _ppbd );
		return;
	}

CExtBitmap * pBmpSimple =
		_ppbd.m_bSeparatedDropDown
			? ( bHorizontal ? (&m_arrBmpPushBtnSDDLeftH) : (&m_arrBmpPushBtnSDDLeftV) )
			: ( bHorizontal ? (&m_arrBmpPushBtnSimpleH) : (&m_arrBmpPushBtnSimpleV) )
			;
CExtBitmap * pBmpRP =
		_ppbd.m_bSeparatedDropDown
			? ( bHorizontal ? (&m_arrBmpPushBtnSDDRightH) : (&m_arrBmpPushBtnSDDRightV) )
			: NULL
			;
INT nGlyphHeightSimple =
		_ppbd.m_bSeparatedDropDown
			? ( bHorizontal ? m_nPushBtnSDDLeftGlyphHeightH : m_nPushBtnSDDLeftGlyphHeightV )
			: ( bHorizontal ? m_nPushBtnSimpleGlyphHeightH : m_nPushBtnSimpleGlyphHeightV )
			;
INT nGlyphHeightRP =
		_ppbd.m_bSeparatedDropDown
			? ( bHorizontal ? m_nPushBtnSDDRightGlyphHeightH : m_nPushBtnSDDRightGlyphHeightV )
			: ( 0 )
			;
CRect * pRectPaddingSimple =
		_ppbd.m_bSeparatedDropDown
			? ( bHorizontal ? (&m_rcPushBtnSDDLeftPaddingH) : (&m_rcPushBtnSDDLeftPaddingV) )
			: ( bHorizontal ? (&m_rcPushBtnSimplePaddingH) : (&m_rcPushBtnSimplePaddingV) )
			;
CRect * pRectPaddingRP =
		_ppbd.m_bSeparatedDropDown
			? ( bHorizontal ? (&m_rcPushBtnSDDRightPaddingH) : (&m_rcPushBtnSDDRightPaddingV) )
			: NULL
			;

	if( ! _ppbd.m_bEnabled )
	{
		_ppbd.m_bHover = false;
		_ppbd.m_bDrawFocusRect = false;
		_ppbd.m_bIndeterminate = false;
	}

e_push_button_glyph_type_t ePBGT_Simple = __EPBGT_NORMAL;
	if( ! _ppbd.m_bEnabled )
		ePBGT_Simple = __EPBGT_DISABLED;
	else if( _ppbd.m_bPushed )
		ePBGT_Simple = __EPBGT_PRESSED;
	else if( _ppbd.m_bHover /*|| _ppbd.m_bDrawFocusRect*/ )
		ePBGT_Simple = __EPBGT_HOVER;
	else if( _ppbd.m_bDefaultPushButton )
		ePBGT_Simple = __EPBGT_DEFAULT;
e_push_button_glyph_type_t ePBGT_RP = ePBGT_Simple;
CRect rcSrcSimple( 0, 0, pBmpSimple->GetSize().cx, nGlyphHeightSimple );
CRect rcSrcRP( 0, 0, 0, 0 );
	if( _ppbd.m_bSeparatedDropDown )
	{
		if( ePBGT_Simple == __EPBGT_HOVER || ePBGT_Simple == __EPBGT_PRESSED )
		{
			if( _ppbd.m_bPushedDropDown )
				ePBGT_Simple = __EPBGT_HH;
			else
				ePBGT_RP = __EPBGT_HH;
		}
		ASSERT( pBmpRP != NULL );
		rcSrcRP.right = pBmpRP->GetSize().cx;
		rcSrcRP.bottom = nGlyphHeightRP;
		rcSrcRP.OffsetRect(
			0,
			nGlyphHeightRP * INT(ePBGT_RP)
			);
	}
	rcSrcSimple.OffsetRect(
		0,
		nGlyphHeightSimple * INT(ePBGT_Simple)
		);

bool bRTL = _ppbd.IsRTL();
CRect rectClient( _ppbd.m_rcClient );
CRect rectCaption( &rectClient );

INT nDDW = g_glyph_btn_expand_bottom.Size().cx;
	if( aDrawingMode == eModeControl )
		nDDW += __DROPDOWN_ARROW_GAP*2;
	else
		nDDW += 4;
CRect rcDstSimple = rectClient;
CRect rcPaintSimpleDst = rcDstSimple;
CRect rcDstRP = rectClient;
	if( _ppbd.m_bDropDown )
	{
		if( bHorizontal )
		{
			rcDstRP.left = rcDstRP.right - nDDW;
			rcDstSimple.right = rcDstRP.left;
		} // if( bHorizontal )
		else
		{
			rcDstRP.top = rcDstRP.bottom - nDDW;
			rcDstSimple.bottom = rcDstRP.top;
		} // else from if( bHorizontal )
	} // if( _ppbd.m_bDropDown )
	if( _ppbd.m_bSeparatedDropDown )
	{
		ASSERT( pBmpRP != NULL );
		pBmpRP->AlphaBlendSkinParts(
			dc.m_hDC,
			rcDstRP,
			rcSrcRP,
			*pRectPaddingRP,
			CExtBitmap::__EDM_STRETCH,
			true,
			true
			);
		rcPaintSimpleDst = rcDstSimple;
	} // if( _ppbd.m_bSeparatedDropDown )
	rectCaption = rcDstSimple;
	pBmpSimple->AlphaBlendSkinParts(
		dc.m_hDC,
		rcPaintSimpleDst,
		rcSrcSimple,
		*pRectPaddingSimple,
		CExtBitmap::__EDM_STRETCH,
		true,
		true
		);
	if( _ppbd.m_bDropDown )
	{
		CRect rectGlyph = rcDstRP;
		if( _ppbd.m_bPushedDropDown )
			rectGlyph.OffsetRect( GetPushedOffset() );
		COLORREF ColorValues[2] =
		{
			RGB(0,0,0),
			RGB(0,0,0),
		};
		bool bSetDefaultColorForDD = true;
		if( _ppbd.m_bEnabled )
		{
			if( m_clrCustomDropDownGlyphEnabled != COLORREF(-1L) )
			{
				ColorValues[1] = m_clrCustomDropDownGlyphEnabled;
				bSetDefaultColorForDD = false;
			}
		}
		else
		{
			if( m_clrCustomDropDownGlyphDisabled != COLORREF(-1L) )
			{
				ColorValues[1] = m_clrCustomDropDownGlyphDisabled;
				bSetDefaultColorForDD = false;
			}
		}
		if( bSetDefaultColorForDD )
			ColorValues[1] =
				GetColor(
					_ppbd.m_bEnabled
						? COLOR_BTNTEXT
						: COLOR_3DSHADOW
						,
					_ppbd.m_pHelperSrc,
					_ppbd.m_lParam 
					);
		if( bRTL && (rectGlyph.Width()&1) == 0 )
			rectGlyph.right++;
		PaintGlyphCentered(
			dc,
			rectGlyph,
			_ppbd.m_bHorz ? g_glyph_btn_expand_bottom : g_glyph_btn_expand_right,
			ColorValues
			);
	} // if( _ppbd.m_bDropDown )

CExtSafeString sTitle = _T("");
	if( _ppbd.m_sText != NULL )
		sTitle = _ppbd.m_sText;

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

	rectCaption.DeflateRect( _ppbd.m_rcBorderSizes );
	rectClient = rectCaption;

CRect rcFocus( rectCaption );

bool bRibbonMode = false;
bool bDrawIcon = true;
#if (!defined __EXT_MFC_NO_RIBBON_BAR)
	if(		_ppbd.m_pHelperSrc != NULL
		&&	_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtRibbonButton) )
		)
	{ // ribbon button
		bRibbonMode = true;
		CExtRibbonButton * pTBB = STATIC_DOWNCAST( CExtRibbonButton, _ppbd.m_pHelperSrc );
		INT nILV = pTBB->RibbonILV_Get();
		if( nILV == __EXT_RIBBON_ILV_SIMPLE_LARGE )
		{
			bDrawIcon = false;
			INT nTextToIconDistance = Ribbon_GetTextToIconDistance( dc, nILV, pTBB, _ppbd.m_lParam );
//			CSize _sizeIcon = _ppbd.m_pIcon->GetSize();
			CSize _sizeIcon = Ribbon_GetIconSize( pTBB, nILV, _ppbd.m_lParam );
			CRect rcIconPaint = rectCaption;
			CRect rcCP = pTBB->OnRibbonGetContentPadding();
			rcIconPaint.bottom = rcIconPaint.top + _sizeIcon.cx + rcCP.bottom + nTextToIconDistance;
			rcIconPaint.DeflateRect( rcCP.left, rcCP.top, rcCP.right, 0 );
			rectCaption.top += _sizeIcon.cy + nTextToIconDistance + rcCP.top + rcCP.bottom;
			rectCaption.DeflateRect( rcCP.left, 0, rcCP.right, rcCP.bottom );
			if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
			{
				PaintIcon(
					dc,
					_ppbd.m_bHorz,
					_ppbd.m_pIcon,
					rcIconPaint,
					_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
					_ppbd.m_bEnabled,
					_ppbd.m_bHover,
					_ppbd.m_nIconAlignment,
					_ppbd.m_bForceRtlLayout && _ppbd.m_bHorz,
					NULL,
					NULL,
					&_ppbd.m_rcIconMargins,
					_ppbd.m_clrCustomAccentEffectForIcon
					);
			} // if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		} // if( nILV == __EXT_RIBBON_ILV_SIMPLE_LARGE )
	} // ribbon button
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

	// Draw the icon
	if(		bDrawIcon
		&&	rectClient.bottom > rectClient.top
		&&	rectClient.right > rectClient.left
		)
	{
		if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		{
			PaintIcon(
				dc,
				_ppbd.m_bHorz,
				_ppbd.m_pIcon,
				rectClient,
				_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
				_ppbd.m_bEnabled,
				_ppbd.m_bHover,
				_ppbd.m_nIconAlignment,
				_ppbd.m_bForceRtlLayout && _ppbd.m_bHorz,
				NULL,
				&rectCaption,
				&_ppbd.m_rcIconMargins,
				_ppbd.m_clrCustomAccentEffectForIcon
				);
			
			if( _ppbd.m_nFocusMode == __EBFM_CAPTION_AREA )
				rcFocus = rectCaption;

			if(		_ppbd.m_bHorz
				&&	_ppbd.m_pHelperSrc != NULL
				&&	_ppbd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd))
				)
			{
				// if painting tool button with icon in popup menu
				if( bRTL )
					rectCaption.right -= 3;
				else
					rectCaption.left += 3;
			}

			if( _ppbd.m_bHorz )
			{
				rectCaption.DeflateRect( 
					_ppbd.m_rcTextMargins.left,
					_ppbd.m_rcTextMargins.top,
					_ppbd.m_rcTextMargins.right,
					_ppbd.m_rcTextMargins.bottom
					);
			}
			else
			{
				rectCaption.DeflateRect( 
					_ppbd.m_rcTextMargins.top,
					_ppbd.m_rcTextMargins.right,
					_ppbd.m_rcTextMargins.bottom,
					_ppbd.m_rcTextMargins.left
					);			
			}
		} // if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		else
		{
			if( ! bRibbonMode )
			{
				if(		_ppbd.m_pHelperSrc != NULL
					&&	_ppbd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd))
					)
				{
					if( _ppbd.m_bHorz )
						rectCaption.DeflateRect( 3, 0 );
					else
						rectCaption.DeflateRect( 0, 3 );
				}
			} // if( ! bRibbonMode )
			PaintSysCmdGlyph(
				dc,
				_ppbd.m_nHelperSysCmdID,
				rectClient,
				false,
				_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
				_ppbd.m_bEnabled
				);
		} // else from if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
	}

	if(		rectCaption.bottom > rectCaption.top
		&&	rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text & focus rect
		if( ! sTitle.IsEmpty() )
		{
			if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
//			dc.SetBkColor( bgColor );
			dc.SetBkMode( TRANSPARENT );

			if( _ppbd.m_bEnabled ) 
			{
				if( _ppbd.m_bHover || _ppbd.m_bPushed )
				{
					if(			_ppbd.m_bPushed
							&&	_ppbd.m_clrForceTextPressed != ((COLORREF)-1L)
							)
							dc.SetTextColor( _ppbd.m_clrForceTextPressed );
					else if(	_ppbd.m_bHover
							&&	_ppbd.m_clrForceTextHover != ((COLORREF)-1L)
							)
							dc.SetTextColor( _ppbd.m_clrForceTextHover );
					else if( _ppbd.m_clrForceTextNormal != ((COLORREF)-1L) ) 
							dc.SetTextColor( _ppbd.m_clrForceTextNormal );
					else
							dc.SetTextColor( GetColor( COLOR_BTNTEXT, _ppbd.m_pHelperSrc, _ppbd.m_lParam ) );
				}
				else 
				{
					dc.SetTextColor( 
						( _ppbd.m_clrForceTextNormal == ((COLORREF)-1L) )
							? GetColor( 
								( _ppbd.m_bHover && _ppbd.m_bPushed ) ? XPCLR_PUSHEDHOVERTEXT : COLOR_BTNTEXT, 
								_ppbd.m_pHelperSrc, 
								_ppbd.m_lParam 
								)
							: _ppbd.m_clrForceTextNormal
						);
				}			
			} // if( _ppbd.m_bEnabled ) 
			else 
			{
				dc.SetTextColor(
					( _ppbd.m_clrForceTextDisabled == ((COLORREF)-1L) )
						? GetColor( COLOR_3DSHADOW, _ppbd.m_pHelperSrc, _ppbd.m_lParam )
						: _ppbd.m_clrForceTextDisabled
					);
			} // else from if( _ppbd.m_bEnabled ) 

//			// center text
//			CRect centerRect = rectCaption;

			CFont * pOldBtnFont = NULL;
			CFont * pCurrFont = NULL;
			CFont fontDummy;

			if( _ppbd.m_hFont != NULL )
			{
				if( _ppbd.m_bHorz )
					pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
				else
				{
					LOGFONT lf;
					::memset(&lf,0,sizeof(LOGFONT));
					::GetObject( _ppbd.m_hFont, sizeof(LOGFONT), &lf );
					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
						VERIFY(
							fontDummy.CreateFontIndirect(&lf)
							);
						pCurrFont = &fontDummy;
					}
					else
					{
						// suppose font already prepared
						pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
					}
				}
			} // if( _ppbd.m_hFont != NULL )
			else
			{
				if( _ppbd.m_bHorz )
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBold;
					else
						pCurrFont = &m_FontNormal;
				}
				else
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBoldVert;
					else
						pCurrFont = &m_FontNormalVert;
				}
			} // else from if( _ppbd.m_hFont != NULL )

			ASSERT( pCurrFont != NULL );
			pOldBtnFont = dc.SelectObject( pCurrFont );

			CExtSafeString sBtn( sTitle );
			if( _ppbd.m_bHorz )
			{
				CRect rcTextLocation( 0, 0, 0, 0);
				UINT nDtMeasureFlags =
					DT_LEFT | DT_TOP | DT_CALCRECT | ((bRTL || _ppbd.m_bForceRtlText) ? DT_RTLREADING : 0);
				if( _ppbd.m_bWordBreak )
				{
					rcTextLocation = rectCaption;
					rcTextLocation.OffsetRect( -rcTextLocation.TopLeft() );
					rcTextLocation.bottom = rcTextLocation.top;
					nDtMeasureFlags |= DT_WORDBREAK;
				}
				else
					nDtMeasureFlags |= DT_SINGLELINE;

				dc.DrawText(
					sBtn, 
					-1,
					rcTextLocation,
					nDtMeasureFlags
					);
				rcTextLocation.OffsetRect(
					rectCaption.TopLeft() - rcTextLocation.TopLeft()
					);
				
				UINT nDtDrawFlags = 0;
				if( (_ppbd.m_nTextAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_RIGHT )
				{
					nDtDrawFlags |= DT_RIGHT;
					rcTextLocation.OffsetRect(
						rectCaption.Width() - rcTextLocation.Width(),
						0
						);
				}
				else if( (_ppbd.m_nTextAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
				{
					nDtDrawFlags |= DT_CENTER;
					rcTextLocation.OffsetRect(
						( rectCaption.Width() - rcTextLocation.Width() ) / 2,
						0
						);
				}
				else
					nDtDrawFlags |= DT_LEFT;
				
				if( (_ppbd.m_nTextAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_BOTTOM )
				{
					nDtDrawFlags |= DT_BOTTOM;
					rcTextLocation.OffsetRect(
						0,
						rectCaption.Height() - rcTextLocation.Height()
						);
				}
				else if( (_ppbd.m_nTextAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_TOP )
				{
					nDtDrawFlags |= DT_TOP;
				}
				else
				{
					nDtDrawFlags |= DT_VCENTER;
					rcTextLocation.OffsetRect(
						0,
						( rectCaption.Height() - rcTextLocation.Height() ) / 2
						);
				}

				if( _ppbd.m_bWordBreak )
					nDtDrawFlags |= DT_WORDBREAK;
				else
					nDtDrawFlags |= DT_SINGLELINE;

				dc.DrawText(
					sTitle,
					-1,
					rcTextLocation,
					nDtDrawFlags
					);
			} // if( _ppbd.m_bHorz )
			else
			{
				INT nTextLength = sTitle.GetLength();
				INT nAmpIndex = sTitle.Find( _T('&') );
 				if( nAmpIndex >= 0 )
					sBtn =
						sTitle.Left( nAmpIndex )
						+ sTitle.Right( nTextLength - ( nAmpIndex + 1 ) );

				static TCHAR stat_strDummyAmpSeq[] = _T("\001\001");
				sBtn.Replace( _T("&&"), stat_strDummyAmpSeq );
				sBtn.Remove( _T('&') );
				sBtn.Replace( stat_strDummyAmpSeq, _T("&") );

				CFont * pFont = 
					_ppbd.m_bDefaultPushButton 
						? &m_FontBold 
						: &m_FontNormal;

				CRect rcText =
					stat_CalcTextDimension( dc, *pFont, sBtn );
				CRect rcTextLocation( 
					0, 0, 
					rcText.Height(), rcText.Width()
					);
				rcTextLocation.OffsetRect(
					rectCaption.TopLeft()
					);

				if( (_ppbd.m_nTextAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_RIGHT )
				{
					rcTextLocation.OffsetRect(
						0,
						rectCaption.Height() - rcTextLocation.Height()
						);
				}
				else if( (_ppbd.m_nTextAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
				{
					rcTextLocation.OffsetRect(
						0,
						( rectCaption.Height() - rcTextLocation.Height() ) / 2
						);
				}
				if( (_ppbd.m_nTextAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_TOP )
				{
					rcTextLocation.OffsetRect(
						rectCaption.Width() - rcTextLocation.Width(),
						0
						);
				}
				else if( (_ppbd.m_nTextAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_CENTER )
				{
					rcTextLocation.OffsetRect(
						( rectCaption.Width() - rcTextLocation.Width() ) / 2,
						0
						);
				}
				
				rcTextLocation.OffsetRect( 1, 0 );
				
				if( nAmpIndex >= 0 )
				{
					CPoint ptLineFrom = 
						CPoint(
							rcTextLocation.left,
							rcTextLocation.top + stat_CalcTextWidth( dc, *pFont, sBtn.Left( nAmpIndex ) )
							);
					CPoint ptLineTo =
						CPoint(
							rcTextLocation.left,
							rcTextLocation.top + stat_CalcTextWidth( dc, *pFont, sBtn.Left( nAmpIndex + 1 ) )
							);

					CPen pen( PS_SOLID, 0, dc.GetTextColor() );
					CPen * pOldPen = dc.SelectObject( &pen );

					dc.MoveTo( ptLineFrom );
					dc.LineTo( ptLineTo );
					
					dc.SelectObject( pOldPen );
				}
				
				rcTextLocation.OffsetRect(
					rcTextLocation.Width() + 1,
					0
					);

				dc.DrawText(
					sBtn,
					sBtn.GetLength(),
					rcTextLocation,
					DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX
					);

			} // else from if( _ppbd.m_bHorz )

			dc.SelectObject(pOldBtnFont);

		} // if( ! sTitle.IsEmpty() )

		if(		_ppbd.m_bDrawFocusRect 
			&&	( ( !sTitle.IsEmpty() ) || _ppbd.m_nFocusMode == __EBFM_CLIENT_AREA )
			)
		{
			rcFocus.DeflateRect( 2, 2 );
			dc.DrawFocusRect( &rcFocus );
		}
		
	} // if we have valid area for text & focus rect

	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}


IMPLEMENT_DYNCREATE(CDsProfuisCustomOffice2003, CExtPaintManagerOffice2003)

void CDsProfuisCustomOffice2003::InitTranslatedColors()
{
	CExtPaintManagerOffice2003::InitTranslatedColors();
	/* el codigo de la clase despues de la revision eliminado ... no hace falta ... */
}

bool CDsProfuisCustomOffice2003::PaintDockerBkgnd(
	bool bClientMapping,
	CDC & dc,
	CWnd * pWnd,
	LPARAM lParam // = NULL
	)
{
	if (!theApp.m_colorset3)
	{
		return CExtPaintManagerOffice2003::PaintDockerBkgnd(bClientMapping,dc,pWnd,lParam);		
	}
	if (pWnd->IsKindOf(RUNTIME_CLASS(CExtButton)))
	{
		return DSPaintDockerBkgnd(bClientMapping,dc,pWnd,theApp.m_color3);
	}
	else
		return CExtPaintManagerOffice2003::PaintDockerBkgnd(bClientMapping,dc,pWnd,lParam);		
}

bool CDsProfuisCustomOffice2003::DSPaintDockerBkgnd(
	bool bClientMapping,
	CDC & dc,
	const CRect &rcDst,
	const CRect &rcWnd,
	COLORREF micolor
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	rcWnd;
	LPARAM lParam = NULL;
	bClientMapping;
COLORREF clrLeft = stat_HLS_Adjust( micolor, 0.02, 0.07, 0.00 );
COLORREF clrRight = stat_HLS_Adjust( micolor, 0.00, 0.43, 0.00 );
CRect rcDst1( rcDst );
CRect rcDst2( 0, 0, 0, 0 );
bool bRTL = ( (dc.GetLayout()&LAYOUT_RTL) != 0 ) ? true : false;
	if( rcDst.Width() >= 686 ) 
	{
		if( bRTL )
		{
			rcDst2 = rcDst;
			rcDst2.right = rcDst2.left + rcDst1.Width() - 686;
			rcDst1 = rcDst;
			rcDst1.left = rcDst2.right;
		} // if( bRTL )
		else
		{
			rcDst1.right = rcDst1.left + 686;
			rcDst2 = rcDst;
			rcDst2.left = rcDst1.right;
		} // else from if( bRTL )
	} // if( rcDst.Width() >= 686 ) 
	stat_PaintGradientRect(
		dc,
		rcDst1,
		bRTL ? clrRight : clrLeft,
		bRTL ? clrLeft : clrRight,
		false,
		64
		);
	if( ! rcDst2.IsRectNull() )
		dc.FillSolidRect( &rcDst2, clrRight );

	return true;
}


bool CDsProfuisCustomOffice2003::DSPaintDockerBkgnd(
	bool bClientMapping,
	CDC & dc,
	CWnd * pWnd,
	COLORREF micolor
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL && ::IsWindow(pWnd->GetSafeHwnd()) );

	LPARAM lParam = NULL;

	if(	! GetCb2DbTransparentMode( pWnd ) )
		return false;
COLORREF clrSolidShade0 = ((COLORREF)(-1L));
COLORREF clrSolidShade1 = ((COLORREF)(-1L));
bool bToolbarShade = false;
bool bToolbarHorz = false;
CWnd * pFrame = pWnd;
	for(	;
			pFrame != NULL;
			pFrame = pFrame->GetParent()
			)
	{
		if( (pFrame->GetStyle() & WS_CHILD) == 0 )
		{
// (- 2.25)
//			if(! pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
//				return false;
			break;
		}
		else // frame features - design mode
			if( pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
				break;
		if(		pFrame != pWnd
			&&	pFrame->IsKindOf( RUNTIME_CLASS(CExtToolControlBar) )
			)
		{
			if( ((CExtToolControlBar*)pFrame)->m_pDockSite != NULL )
			{
				if(	!(	pFrame->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
					||	pFrame->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
						)
					)
				{
					if( ((CExtToolControlBar*)pFrame)->m_pDockBar == NULL )
						continue;
					if( ((CExtToolControlBar*)pFrame)->IsFloating() )
					{
						clrSolidShade0 = GetColor( _2003CLR_GRADIENT_DARK, pWnd, lParam );
						clrSolidShade1 = GetColor( _2003CLR_GRADIENT_LIGHT, pWnd, lParam );
					}
					else
					{
						bToolbarShade = true;
						bToolbarHorz = ((CExtToolControlBar*)pFrame)->IsDockedHorizontally();
					}
				}
			}
			break;
		}
	}
	if( pFrame == NULL )
		return false;

CExtToolControlBar::FtLayoutQuery _FtLayoutQuery;
	if( _FtLayoutQuery.Query( pWnd->m_hWnd ) )
	{
		if( bClientMapping )
			_FtLayoutQuery.ReMapRectsToTargetClient();
		else
			_FtLayoutQuery.ReMapRectsToTargetWindow();
		_FtLayoutQuery.DrawRects( dc );
		return true;
	} // if( _FtLayoutQuery.Query( pWnd->m_hWnd ) )

CRect rcPaintGradient;
	pFrame->GetClientRect( &rcPaintGradient );
	pFrame->ClientToScreen( &rcPaintGradient );

	if( bClientMapping )
	{
		pWnd->ScreenToClient( &rcPaintGradient );
	} // if( bClientMapping )
	else
	{
		CRect rc;
		pWnd->GetWindowRect( &rc );
		rcPaintGradient.OffsetRect( -rc.left, -rc.top );
	} // else from if( bClientMapping )

	if(		::IsRectEmpty( &rcPaintGradient )
		||	( ! dc.RectVisible( &rcPaintGradient ) )
		)
		return true;

	if( clrSolidShade0 != ((COLORREF)(-1L)) )
	{
		if( clrSolidShade1 != ((COLORREF)(-1L)) )
		{
			bool bRTL = ( (dc.GetLayout()&LAYOUT_RTL) != 0 ) ? true : false;
			if( bRTL )
			{
				COLORREF clrTmp = clrSolidShade0;
				clrSolidShade0 = clrSolidShade1;
				clrSolidShade1 = clrTmp;
			} // if( bRTL )
			stat_PaintGradientRect(
				dc,
				rcPaintGradient,
				clrSolidShade0,
				clrSolidShade1,
				true
				);
		} // if( clrSolidShade1 != ((COLORREF)(-1L)) )
		else
			dc.FillSolidRect( rcPaintGradient, clrSolidShade0 );
		return true;
	} // if( clrSolidShade0 != ((COLORREF)(-1L)) )

	if( bToolbarShade )
	{
		if( OnQueryUseThemeColors() )
		{
			// if use WinXP themed colors
			COLORREF clrFillHint;
			OnQueryThemeColors( &clrFillHint );
			COLORREF clrLeft	= stat_HLS_Adjust( clrFillHint,  0.00, 0.77, 0.15 );
			COLORREF clrMiddle	= stat_HLS_Adjust( clrFillHint,	 0.00, 0.58, 0.15 );
			COLORREF clrRight	= stat_HLS_Adjust( clrFillHint,	 0.00,-0.25,-0.25 );
			if( OnQuerySystemTheme() == ThemeLunaSilver )
				clrRight = stat_HLS_Adjust( clrRight, 0.05, 0.00, 0.05 );
			stat_PaintGradientRect2steps( 
				dc, 
				rcPaintGradient, 
				clrLeft, 
				clrMiddle, 
				clrRight,
				bToolbarHorz 
 				);
		} // if( OnQueryUseThemeColors() )
		else
		{
			COLORREF clrLeft = 
				bToolbarHorz 
					? GetColor( _2003CLR_MLA_NORM_RIGHT, pWnd, lParam )
					: GetColor( _2003CLR_MLA_NORM_LEFT, pWnd, lParam )
					;
			COLORREF clrRight = 
				bToolbarHorz 
					? GetColor( _2003CLR_MLA_NORM_LEFT, pWnd, lParam )
					: GetColor( _2003CLR_MLA_NORM_RIGHT, pWnd, lParam )
					;
			stat_PaintGradientRect( 
				dc,
				rcPaintGradient,
				clrLeft,
				clrRight,
				bToolbarHorz 
				);
		} // else from if( OnQueryUseThemeColors() )
	} // if( bToolbarShade )
	else
	{
		CRect rcWnd;
		pWnd->GetClientRect( &rcWnd );
		pFrame->ClientToScreen( &rcWnd );
		DSPaintDockerBkgnd( 
			bClientMapping,
			dc,  
			rcPaintGradient,
			rcWnd,
			micolor
			);
	} // else from if(	bToolbarShade )

	return true;
}

void CDsProfuisCustomOffice2003::PintaBotonRedondo(CDC & dc,LPCRECT rectClient, COLORREF clrTopLeft, COLORREF clrBottomRight  )
{   
   clrBottomRight;   

   if (theApp.m_colorset4)
	    clrTopLeft = theApp.m_color4;

	CBrush * pBrush =
		CBrush::FromHandle( (HBRUSH)::GetStockObject(NULL_BRUSH) );
	CBrush * pOldBrush = dc.SelectObject( pBrush );	
	CPen pen;
	pen.CreatePen(
		PS_SOLID,
		1,
		clrTopLeft
		);
	CPen * pOldPen = dc.SelectObject( &pen );	
	dc.RoundRect(
		rectClient,
		CPoint( 10, 10 )
		);
	dc.SelectObject( pOldPen );
	dc.SelectObject( pOldBrush );
}

#define __ExtMfc_CXTEXTMARGIN (-1) // 1
#define __ExtMfc_CYTEXTMARGIN (0)  // 1

void CDsProfuisCustomOffice2003::PaintPushButton(
	CDC & dc,
	CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( _ppbd.m_rcClient.IsRectEmpty() )
		return;

	if (_ppbd.m_pHelperSrc == NULL || !_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtButton)))
	{
		CExtPaintManagerOffice2003::PaintPushButton(dc,_ppbd);	
		return;
	}	

	/*
	CDC dc;

    dc.CreateCompatibleDC(&_dc);
	*/	

bool bRTL = _ppbd.IsRTL();

	if( !_ppbd.m_bEnabled )
	{
		_ppbd.m_bHover = false;
		_ppbd.m_bDrawFocusRect = false;
		_ppbd.m_bIndeterminate = false;
	} // if( ! _ppbd.m_bEnabled )

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

int nIdxMainGradientLeft = -1;
int nIdxMainGradientRight = -1;
COLORREF bgColor, bgColorDD;
	if( _ppbd.m_clrForceBk != ((COLORREF)-1L) )
		bgColor = bgColorDD = _ppbd.m_clrForceBk;
	else if(_ppbd.m_bHover 
		||	_ppbd.m_bPushed 
		||	_ppbd.m_bPushedDropDown 
		)
	{
		bgColor = bgColorDD = 
			GetColor( 
				CLR_3DFACE_IN, 
				_ppbd.m_pHelperSrc, 
				_ppbd.m_lParam 
				);
		if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
		{
			(( _ppbd.m_bPushedDropDown ) ? bgColorDD : bgColor) =
				GetColor(
					_ppbd.m_bHover ? XPCLR_HILIGHT : CLR_3DFACE_IN,
					_ppbd.m_pHelperSrc,
					_ppbd.m_lParam 
					);
		} // if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
		if(		(!_ppbd.m_bHover)
			&&	_ppbd.m_bPushed
			&&	(!_ppbd.m_bPushedDropDown)
			&&	_ppbd.m_bSeparatedDropDown
			)
			bgColorDD =  GetColor( CLR_3DFACE_OUT, _ppbd.m_pHelperSrc, _ppbd.m_lParam );
		if( _ppbd.m_bHover )
		{
			if( _ppbd.m_bPushed )
			{
				nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
				nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
			} // if( _ppbd.m_bPushed )
			else
			{
				nIdxMainGradientLeft = m_nIdxClrBtnHoverLeft;
				nIdxMainGradientRight = m_nIdxClrBtnHoverRight;
			} // else from if( _ppbd.m_bPushed )
		} // if( _ppbd.m_bHover )
		else if( _ppbd.m_bPushed )
		{
			nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
			nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
		} // else if( _ppbd.m_bPushed )
	} // if(	_ppbd.m_bHover || _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
	else
	{
		bgColor = bgColorDD =
			GetColor(
				_ppbd.m_bFlat ? CLR_3DFACE_OUT : COLOR_3DFACE,
				_ppbd.m_pHelperSrc,
				_ppbd.m_lParam 
				);
	}

COLORREF clrDkBorder =
		GetColor(
			(_ppbd.m_bHover || _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
				? XPCLR_HILIGHT_BORDER
				: ( _ppbd.m_bFlat ? XPCLR_HILIGHT_BORDER : COLOR_3DDKSHADOW )
				,
			_ppbd.m_pHelperSrc,
			_ppbd.m_lParam 
			);
bool bPushedDropDown = false;
	if( (_ppbd.m_bPushed && !_ppbd.m_bSeparatedDropDown)
		|| _ppbd.m_bPushedDropDown
		)
		bPushedDropDown = true;

CRect rectClient( _ppbd.m_rcClient );
CRect rectTotal( _ppbd.m_rcClient );
CRect rectCaption( _ppbd.m_rcClient );
CRect rectDropDown( 0, 0, 0, 0 );
CRect rcTexto(_ppbd.m_rcClient);
	if( _ppbd.m_bDropDown )
	{
		rectDropDown = rectClient;
		if( _ppbd.m_bHorz )
		{
			if( bRTL )
			{
				rectCaption.left +=
					__DROPDOWN_ARROW_GAP*2
					+ g_glyph_btn_expand_bottom.Size().cx
					;
				rectDropDown.right = rectCaption.left + 1;
			} // if( bRTL )
			else
			{
				rectCaption.right -=
					__DROPDOWN_ARROW_GAP*2
					+ g_glyph_btn_expand_bottom.Size().cx
					;
				rectDropDown.left = rectCaption.right - 1;
			} // else from if( bRTL )
		} // if( _ppbd.m_bHorz )
		else
		{
			rectCaption.bottom -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_right.Size().cy
				;
			rectDropDown.top = rectCaption.bottom - 1;
		} // if( _ppbd.m_bHorz )

		if(		(! _ppbd.m_bTransparentBackground )
			&&	( _ppbd.m_bHover || _ppbd.m_bPushedDropDown )
			||	_ppbd.m_clrForceBk != ((COLORREF)-1L)
			)
			dc.FillSolidRect( &rectDropDown, bgColorDD );

		if( _ppbd.m_bSeparatedDropDown )
		{
			rectClient = rectCaption;

			if( bPushedDropDown )
			{
				
				if( _ppbd.m_bFlat )
				{
					if( _ppbd.m_bDrawBorder )
						PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
						//dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
				} // if( _ppbd.m_bFlat )
				else    
					PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
					//dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
			} // if( bPushedDropDown )
			else
			{

				if( _ppbd.m_bFlat )
				{
					if( _ppbd.m_bHover && _ppbd.m_bDrawBorder )
						PintaBotonRedondo(dc , rectClient, clrDkBorder, clrDkBorder );
						//dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
				} // if( _ppbd.m_bFlat )
				else
					PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
						//dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
			} // else from if( bPushedDropDown )
		} // if( _ppbd.m_bSeparatedDropDown )
	} // if( _ppbd.m_bDropDown )

	if(		!_ppbd.m_bTransparentBackground 
		||	_ppbd.m_clrForceBk != ((COLORREF)-1L)
		)
	{
		if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
		{
			stat_PaintGradientRect(
				dc,
				rectClient,
				GetColor( _ppbd.m_bHorz ? nIdxMainGradientRight : nIdxMainGradientLeft, _ppbd.m_pHelperSrc, _ppbd.m_lParam ),
				GetColor( _ppbd.m_bHorz ? nIdxMainGradientLeft : nIdxMainGradientRight, _ppbd.m_pHelperSrc, _ppbd.m_lParam ),
				_ppbd.m_bHorz
				);
		} // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
		else
			dc.FillSolidRect( &rectClient, bgColor );
	} // if( ! _ppbd.m_bTransparentBackground )
	// Draw pressed button
	if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
	{

		if( _ppbd.m_bFlat )
		{
			if( _ppbd.m_bDrawBorder )
				PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
				//dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
		}
		else  
			PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
			//dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
	
	} // if( bPushed )
	else // ...else draw non pressed button
	{
		if( _ppbd.m_bFlat )
		{
			if( _ppbd.m_bHover && _ppbd.m_bDrawBorder )				
				PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
				//dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
		} // if( _ppbd.m_bFlat )
		else
		{
			PintaBotonRedondo(dc, rectClient, clrDkBorder, clrDkBorder );
			//dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
			if( _ppbd.m_bHover )
				PintaBotonRedondo(dc,rectClient, clrDkBorder, clrDkBorder );
				//dc.Draw3dRect( rectTotal, clrDkBorder, clrDkBorder );
		}  // else from if( _ppbd.m_bFlat )
	} // else from if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )

	if( _ppbd.m_bDropDown )
	{
		CRect rectGlyph( rectDropDown );
		if( bPushedDropDown )
			rectGlyph.OffsetRect( GetPushedOffset() );
		COLORREF ColorValues[2] =
		{
			RGB(0,0,0),
			GetColor(
				_ppbd.m_bEnabled
					? COLOR_BTNTEXT
					: COLOR_3DSHADOW
					,
				_ppbd.m_pHelperSrc,
				_ppbd.m_lParam 
			)
		};
		if( bRTL && (rectGlyph.Width()&1) == 0 )
			rectGlyph.right++;
		PaintGlyphCentered(
			dc,
			rectGlyph,
			_ppbd.m_bHorz ? g_glyph_btn_expand_bottom : g_glyph_btn_expand_right,
			ColorValues
			);
	} // if( _ppbd.m_bDropDown )

	rectClient = rectCaption;
	rcTexto = rectCaption;

CExtSafeString sTitle( _T("") );
	if( _ppbd.m_sText != NULL )
		sTitle = _ppbd.m_sText;

	// Draw the icon
	if(	rectClient.bottom > rectClient.top
		&& rectClient.right > rectClient.left
		)
	{
		if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		{

			PaintIcon(
				dc,
				_ppbd.m_bHorz,
#ifdef PROFUIS240
				sTitle,
#endif
				_ppbd.m_pIcon,
				rectClient,
#ifdef PROFUIS240
				rectCaption,
#endif
				_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
				_ppbd.m_bEnabled,
#ifdef PROFUIS240
				_ppbd.m_bHover,
				_ppbd.m_eAlign,
				_ppbd.m_bForceRtlLayout && _ppbd.m_bHorz
#else
				_ppbd.m_bHover,
				__ALIGN_VERT_CENTER,
				bRTL,
				NULL,
				&rcTexto
#endif
				);
		} // if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		else
		{
			if( _ppbd.m_bHorz )
				rectCaption.DeflateRect( 3, 0 );
			else
				rectCaption.DeflateRect( 0, 3 );
			PaintSysCmdGlyph(
				dc,
				_ppbd.m_nHelperSysCmdID,
				rectClient,
				false,
				_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
				_ppbd.m_bEnabled
				);
		} // else from if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
	}

	if(		rectCaption.bottom > rectCaption.top
		&&	rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text & focus rect
		if( ! sTitle.IsEmpty() )
		{
			if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
			dc.SetBkColor( bgColor );
			dc.SetBkMode( TRANSPARENT );

			if( _ppbd.m_bEnabled ) 
			{
				int nColorIdx =
					(_ppbd.m_bHover || _ppbd.m_bPushed )
						? (	(_ppbd.m_bHover && _ppbd.m_bPushed)
								? XPCLR_PUSHEDHOVERTEXT
								: COLOR_BTNTEXT 
							)
						: COLOR_BTNTEXT
					;
				if(			nColorIdx == XPCLR_PUSHEDHOVERTEXT
						&&	_ppbd.m_clrForceTextPressed != ((COLORREF)-1L)
						)
						dc.SetTextColor( _ppbd.m_clrForceTextPressed );
				else if(	nColorIdx == CLR_TEXT_IN
						&&	_ppbd.m_clrForceTextHover != ((COLORREF)-1L)
						)
						dc.SetTextColor( _ppbd.m_clrForceTextHover );
				else if( _ppbd.m_clrForceTextNormal != ((COLORREF)-1L) )
						dc.SetTextColor( _ppbd.m_clrForceTextNormal );
				else
						dc.SetTextColor( GetColor( nColorIdx, _ppbd.m_pHelperSrc, _ppbd.m_lParam ) );
			} // if( _ppbd.m_bEnabled ) 
			else 
			{
				dc.SetTextColor(
					( _ppbd.m_clrForceTextDisabled == ((COLORREF)-1L) )
						? GetColor( COLOR_3DSHADOW, _ppbd.m_pHelperSrc, _ppbd.m_lParam )
						: _ppbd.m_clrForceTextDisabled
					);
			} // else from if( _ppbd.m_bEnabled ) 

//			// center text
//			CRect centerRect = rectCaption;

			CFont * pOldBtnFont = NULL;
			CFont * pCurrFont = NULL;
			CFont fontDummy;

			if( _ppbd.m_hFont != NULL )
			{
				if( _ppbd.m_bHorz )
					pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
				else
				{
					LOGFONT lf;
					::memset(&lf,0,sizeof(LOGFONT));
					::GetObject( _ppbd.m_hFont, sizeof(LOGFONT), &lf );
					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
						VERIFY(
							fontDummy.CreateFontIndirect(&lf)
							);
						pCurrFont = &fontDummy;
					}
					else
					{
						// suppose font already prepared
						pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
					}
				}
			} // if( _ppbd.m_hFont != NULL )
			else
			{
				if( _ppbd.m_bHorz )
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBold;
					else
						pCurrFont = &m_FontNormal;
				}
				else
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBoldVert;
					else
						pCurrFont = &m_FontNormalVert;
				}
			} // else from if( _ppbd.m_hFont != NULL )
			ASSERT( pCurrFont != NULL );
			pOldBtnFont = dc.SelectObject( pCurrFont );
			ASSERT( pOldBtnFont != NULL );

			int nTextLength = sTitle.GetLength();
			int nAmpIndex = sTitle.Find( _T('&') );
			CExtSafeString sBtn;
			if( nAmpIndex < 0 )
				sBtn = sTitle;
			else
				sBtn =
					sTitle.Left( nAmpIndex )
					+ sTitle.Right( nTextLength - (nAmpIndex+1) )
					;
			ASSERT( sBtn.Find( _T('&') ) < 0 );

			CRect rcText(0,0,0,0);
			DWORD dwDtAlignFlags = (bRTL || _ppbd.m_bForceRtlText) ? DT_RTLREADING : 0;
			UINT nDrawTextFlags = dwDtAlignFlags;
			if( _ppbd.m_bWordBreak )
			{
				rcText = rcTexto;
				rcText.OffsetRect( -rcText.TopLeft() );
				rcText.bottom = rcText.top;
				nDrawTextFlags |= DT_WORDBREAK;
			}
			else
				nDrawTextFlags |= DT_SINGLELINE;
			dc.DrawText(
				sBtn,
				-1,
				rcText,
				DT_CALCRECT|nDrawTextFlags
				);
			CRect rcCenteredCaption(rcTexto);
			rcCenteredCaption.OffsetRect(
				( ( _ppbd.m_nDrawTextFlagsH & DT_CENTER ) != 0 )
					? ( ( rcTexto.Width() - rcText.Width() ) / 2 )
					: 0
					,
				( ( _ppbd.m_nDrawTextFlagsH & DT_VCENTER ) != 0 )
					? ( ( rcTexto.Height() - rcText.Height() ) / 2 )
					: 0
				);

			if( _ppbd.m_bHorz )
			{
				dc.DrawText(
					sTitle,
					-1,
					rcCenteredCaption,
					nDrawTextFlags
					);
			} // if( _ppbd.m_bHorz )
			else
			{
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rcTexto.Width() - _cyHorzFont)  / 2
					;

				CPoint
					ptLineFrom(0,0),
					ptLineTo(0,0);
				if( nAmpIndex >= 0 )
				{
					ptLineFrom =
						CPoint(
							__ExtMfc_CXTEXTMARGIN
								+ _cyTextMargin,
							__ExtMfc_CYTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									_ppbd.m_bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex)
									)
							);
					ptLineTo =
						CPoint(
							__ExtMfc_CXTEXTMARGIN
								+ _cyTextMargin,
							__ExtMfc_CYTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									_ppbd.m_bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex+1)
									)
							);
					int nXOffsPlus =
						rcTexto.Width()
						- stat_CalcTextDimension(
							dc,
							_ppbd.m_bDefaultPushButton ? m_FontBold : m_FontNormal,
							sBtn
							).Height();
					nXOffsPlus /= 2;
					nXOffsPlus -= 4;
					ptLineFrom.x += nXOffsPlus;
					ptLineTo.x   += nXOffsPlus;
				} // if( nAmpIndex >= 0 )

				CRect rcString = 
					CRect(
						CPoint(
							rcTexto.right - _cyTextMargin + __ExtMfc_CXTEXTMARGIN,
							rcTexto.top + __ExtMfc_CYTEXTMARGIN
							),
						rcTexto.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CSize ptFix(-rcString.Width()-1,0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;
				if( _ppbd.m_bEnabled )
				{
/*VTX*/
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX
						);
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor()
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // if( _ppbd.m_bEnabled )
				else
				{
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor()
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // else from if( _ppbd.m_bEnabled )
			} // else from if( _ppbd.m_bHorz )
			dc.SelectObject(pOldBtnFont);
		} // if( ! sTitle.IsEmpty() )

		if( _ppbd.m_bDrawFocusRect )
		{
			CRect focusRect = rectCaption;
			focusRect.DeflateRect( 3, 3 );
			dc.DrawFocusRect( &focusRect );
		} // if( _ppbd.m_bDrawFocusRect )
	} // if we have valid area for text & focus rect

	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}


/// CDsMenuToolbar y relacionados

CCDsMenuItems::CCDsMenuItems(MenuStruct *menu)
{
	int nitems;
	for (nitems = 0;menu->Items[nitems].NomItem;nitems++);
	m_nitems = nitems;
	m_items.SetSize(nitems);
	m_op.SetSize(nitems);
	m_sub.SetSize(nitems);
	m_nommenu = menu->NomMenu;

	int i;
	for (i = 0;i < m_nitems;i++)
	{	
	   m_items[i] = (const char *)menu->Items[i].NomItem;
       if (menu->Items[i].OpItem[0] == 1 && menu->Items[i].OpItem[1] == 2 && menu->Items[i].OpItem[2] == 3 && menu->Items[i].OpItem[3] == 0)
	   {		   
           MenuStruct *tmp_m = (MenuStruct *)menu->Items[i].OpItem;
		   m_sub[i] = new CCDsMenuItems(tmp_m);
		   m_op[i] = 0;           
       }
       else
	   {
           long mi_par;
           memcpy((void *)&(mi_par),menu->Items[i].OpItem+1,sizeof(long));
		   m_sub[i] = NULL;
           m_op[i] = (UINT)mi_par+1;
       }
	}
}

CCDsMenuItems::~CCDsMenuItems()
{		
	int i;
	for (i = 0;i < m_nitems;i++)
	{
		if (m_sub[i])
			delete m_sub[i];
	}	
}

IMPLEMENT_DYNCREATE(CCDsMenuCombo, CExtComboBox)

CCDsMenuCombo::CCDsMenuCombo()
{
	m_hijo = NULL;
	m_menu = NULL;
}

CCDsMenuCombo::~CCDsMenuCombo()
{
}

void CCDsMenuCombo::Rellena(CCDsMenuItems *menu)
{
	int i;
	ResetContent();
	m_menu = menu;
	if (m_hijo)
		m_hijo->Rellena(NULL);
	if (!menu) return;

	for (i = 0;i < m_menu->m_nitems;i++)
	{
		AddString(m_menu->m_items[i]);
	}
	SetCurSel(-1);
	//OnSelendok();
}

BEGIN_MESSAGE_MAP(CCDsMenuCombo, CExtComboBox)
	//{{AFX_MSG_MAP(CCDsMenuCombo)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboNavega message handlers

void CCDsMenuCombo::OnSelendok() 
{
	int sel = GetCurSel();
	if (!m_menu || sel < 0 || sel >= m_menu->m_nitems)
		return;
	if (m_hijo && m_menu->m_sub[sel])
	{
		m_hijo->Rellena(m_menu->m_sub[sel]);
		return;
	}
	/* activar opcion */
	if (m_menu->m_op[sel])
	{
		((CDsxDoc *)pMFrame->GetActiveDocument())->OpcionDeMenu((int)m_menu->m_op[sel]);
	}
}


CDsMenuToolbar::CDsMenuToolbar()
{
	m_menu = NULL;
	m_menus[0].m_hijo = &(m_menus[1]);
	m_menus[1].m_hijo = &(m_menus[2]);
	m_menus[2].m_hijo = NULL;
}


CDsMenuToolbar::~CDsMenuToolbar()
{
	if (m_menu)
	{
		delete m_menu;
	}
}

void CDsMenuToolbar::Rellena(MenuStruct *mi_menu,int defid)
{
	if (m_menu)
	{
		delete m_menu;
	}	
	m_menu = new CCDsMenuItems(mi_menu);
	m_menus[0].Rellena(m_menu);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsxFrame

// IMPLEMENT_SERIAL( CSimpleControlBar, CExtDynamicControlBar, VERSIONABLE_SCHEMA|1 );

IMPLEMENT_DYNCREATE(CDsxFrame, CFrameWnd)
#ifdef DSXTLIB
BEGIN_MESSAGE_MAP(CDsxFrame, CXTPFrameWnd)
#else
BEGIN_MESSAGE_MAP(CDsxFrame, CFrameWnd)
#endif
	//{{AFX_MSG_MAP(CDsxFrame)
ON_WM_CREATE()
ON_COMMAND(ID_VERTREEMENU, OnVertreemenu)
ON_COMMAND(ID_CENTRAR, OnCentrar)
ON_COMMAND(ID_SINGLEEXPAND, OnSingleexpand)
ON_COMMAND(ID_MULTITAREA, OnMultitarea)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MENUSER, OnMenuser)
	ON_COMMAND(ID_SIEMPREHTML, OnSiemprehtml)
	ON_COMMAND(ID_FONDOHTML, OnFondohtml)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FONDOHTML, OnUpdateTestOtro)
	ON_COMMAND(ID_NEXT_PANE, OnNextPane)
	ON_COMMAND(ID_PREV_PANE, OnPrevPane)
	ON_COMMAND(ID_ALT_DOWN, OnAltDown)
	ON_COMMAND(ID_ALT_UP, OnAltUp)	
	ON_COMMAND(ID_RECORD_LAST, OnRecordLast)
	ON_UPDATE_COMMAND_UI(ID_RECORD_LAST, OnUpdateRecordLast)
	ON_COMMAND(ID_RECORD_FIRST, OnRecordFirst)
	ON_UPDATE_COMMAND_UI(ID_RECORD_FIRST, OnUpdateRecordFirst)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
	ON_UPDATE_COMMAND_UI(ID_RECORD_DELETE, OnUpdateRecordDelete)
	ON_COMMAND(ID_RECORD_NEW, OnRecordNew)
	ON_UPDATE_COMMAND_UI(ID_RECORD_NEW, OnUpdateRecordNew)
	ON_COMMAND(ID_RECORD_NEXT, OnRecordNext)
	ON_UPDATE_COMMAND_UI(ID_RECORD_NEXT, OnUpdateRecordNext)
	ON_COMMAND(ID_RECORD_PREV, OnRecordPrev)
	ON_UPDATE_COMMAND_UI(ID_RECORD_PREV, OnUpdateRecordPrev)
	ON_COMMAND(ID_RECORD_UPDATE, OnRecordUpdate)
	ON_UPDATE_COMMAND_UI(ID_RECORD_UPDATE, OnUpdateRecordUpdate)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_VERCAPTION, OnVercaption)
	ON_COMMAND(ID_EXPORTA_EXCEL, OnExportaExcel)
	ON_UPDATE_COMMAND_UI(ID_EXPORTA_EXCEL, OnUpdateExportaExcel)
	ON_COMMAND(ID_EXPORTA_WORD, OnExportaWord)
	ON_UPDATE_COMMAND_UI(ID_EXPORTA_WORD, OnUpdateExportaWord)
	ON_COMMAND(ID_CONSULTA_FILTRO, OnConsultaFiltro)
	ON_UPDATE_COMMAND_UI(ID_CONSULTA_FILTRO, OnUpdateConsultaFiltro)
	ON_COMMAND(ID_BUSCAMENU, OnBuscamenu)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_TOOLTIPS, OnTooltips)
	ON_COMMAND(ID_ACTIVAR_CRISTAL, OnActivarCristal)
	ON_COMMAND(ID_LISTATAREAS, OnListatareas)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_IMPREDATOS, OnImpredatos)
	ON_COMMAND(ID_CONTROLREMTO, OnControlremto)
	ON_UPDATE_COMMAND_UI(ID_ACTIVAR_ERP, OnUpdateActivarErp)
	ON_COMMAND(ID_ACTIVAR_ERP, OnActivarErp)
	ON_COMMAND(ID_EXPORTA_PLB, OnExportaPlb)
	ON_UPDATE_COMMAND_UI(ID_EXPORTA_PLB, OnUpdateExportaPlb)
	ON_UPDATE_COMMAND_UI(ID_CONTROLREMTO, OnUpdateControlremto)
	ON_COMMAND(ID_EXPORTA_RPT, OnExportaRpt)
	ON_UPDATE_COMMAND_UI(ID_EXPORTA_RPT, OnUpdateExportaRpt)
	ON_COMMAND(ID_ACTIVAR_EVT, OnActivarEvt)
	ON_NOTIFY(MCN_SELECT,6411, OnSelCalendario)
	ON_COMMAND(ID_EXPORTA_XML, OnExportaXml)
	ON_UPDATE_COMMAND_UI(ID_EXPORTA_XML, OnUpdateExportaXml)
	ON_COMMAND(ID_GRIDIMPRIME, OnGridimprime)
    ON_UPDATE_COMMAND_UI(ID_MENUSER, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_SIEMPREHTML, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_IMPRESORAINTERNA, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_VERTREEMENU, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_IMPREDATOS, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_IMPREXCEL, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_CENTRAR, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_SINGLEEXPAND, OnUpdateTestOtro)
    ON_UPDATE_COMMAND_UI(ID_MULTITAREA, OnUpdateTestOtro)
	ON_UPDATE_COMMAND_UI(ID_GRIDIMPRIME, OnUpdateGridimprime)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ACTIVAR_EVTPBX, OnActivarEvtpbx)
	ON_COMMAND(ID_ACTIVAR_EVTCRM, OnActivarEvtcrm)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, DsToolTip )	
#ifdef DSPROFUIS	
	ON_COMMAND_EX(IDR_MAINMENU, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_MAINMENU, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_TOOLMAINFRAME, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_TOOLMAINFRAME, OnUpdateControlBarMenu)	
	ON_REGISTERED_MESSAGE( CExtPopupMenuWnd::g_nMsgPopupDrawItem,OnDrawPopupMenuItem )


	
	/*
	ON_COMMAND_EX(AFX_IDW_TOOLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(AFX_IDW_TOOLBAR, OnUpdateControlBarMenu)	
	*/

	ON_COMMAND_EX(IDR_TOOLMENSA, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_TOOLMENSA, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_NAVEGA, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_NAVEGA, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_TOOLGRID, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_TOOLGRID, OnUpdateControlBarMenu)	
	ON_COMMAND_EX(IDR_TOOLBARMENU, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_TOOLBARMENU, OnUpdateControlBarMenu)	
	/*
	ON_COMMAND_EX(ID_RESIZABLE_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_RESIZABLE_BAR, OnUpdateControlBarMenu)
	*/
	ON_MESSAGE(WM_MITRAYMESSAGE,OnTray)
#endif
END_MESSAGE_MAP()

static UINT indicators[] =
{
      ID_SEPARATOR,   // status line indicator	      
      IDS_STRINGCABECERA,
      IDS_RELACION,
	  IDS_USUARIO,
      //IDS_CLAVES,
      //ID_INDICATOR_CAPS,
      //ID_INDICATOR_NUM,
      //ID_INDICATOR_SCRL
};


#ifdef DSPROFUIS	
void CDsxFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );	
}

BOOL CDsxFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );	
}
#endif

BOOL CDsxFrame::DsToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult) 
{	
	static CString ss;
	if (!theApp.m_tooltips)
		return FALSE;
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
       CDsStdCtrl *pctrl = NULL;
       HWND hwnd = (HWND)nID;
       if (CDsStdDlg::m_hwndctrllst.Lookup(hwnd,pctrl))
       {        
        if (pctrl->m_tooltiptext.IsEmpty())
		{
			/*pTTT->lpszText = "Diagram Software";
			return TRUE; */
            return FALSE;
		}
		
		CWnd *wnd = CWnd::FromHandle(pNMHDR->hwndFrom);				
		wnd->ModifyStyle(0,0x40);		
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 200);
		ss = pctrl->m_pdlg->RecogeAyudaPorPosicion();
		ss = pctrl->m_tooltiptext + ss;
        pTTT->lpszText = (char *)(LPCSTR)ss;
		//m_tooltiptext = "Flecha Arriba cambiar de clave\r\nFlecha Abajo cambiar de clave\r\nAv-Pag cambio clave\r\nRe-Pag cambio clave\r\nEnter continuar\r\nEsc o Ctrl-Q Salir\r\nF11 - consultar relacion\r\nCtrl-Flecha Abajo desplegar grid\r\nShift-F11 consultar claves";		   
        return TRUE;        
        }
    }

    /*pTTT->lpszText = "Diagram Software";
    return TRUE; */

    return(FALSE);
}


class CFileResource : public CFile // (Francesc 6/3/08) -> quick class to read from a resource as a file when a CFile is needed. 
{
public:
	HRSRC m_hrsrc;
	unsigned char *m_p;
	LONGLONG m_pos;
	LONGLONG m_len;

	CFileResource( LPCTSTR resourcetype, UINT resourceid )
	{
		if (!Abre(resourcetype,resourceid))
			::AfxThrowFileException( CFileException::generic );
	}
	~CFileResource()
	{
		Close();
	}
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
		CFileException* pError = NULL)
	{
		return Abre(lpszFileName,nOpenFlags);
	}
	virtual ULONGLONG GetPosition()
	{
		return m_pos;
	}	
	virtual CString GetFileName() const
	{
		return "";
	}
	virtual CString GetFileTitle() const
	{
		return "";
	}
	virtual CString GetFilePath() const
	{
		return "";
	}
	virtual void SetFilePath(LPCTSTR lpszNewName)
	{
		::AfxThrowFileException( CFileException::generic );
	}
	virtual CFile* Duplicate() const
	{
		::AfxThrowFileException( CFileException::generic );
	}
	virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom)
	{
		switch(nFrom)
		{
		case CFile::begin:
			m_pos = lOff;
			break;
		case CFile::end:
			m_pos = m_len + lOff;
			break;
		case CFile::current:
			m_pos += lOff;
		default:
			::AfxThrowFileException( CFileException::generic );
		};
		return m_pos;
	}
	virtual void SetLength(ULONGLONG dwNewLen)
	{
		::AfxThrowFileException( CFileException::generic );
	}
	virtual ULONGLONG GetLength() const
	{
		return m_len;
	}
	virtual UINT Read(void* lpBuf, UINT nCount)	
	{
		UINT n;
		if (!m_p || !lpBuf)
			return 0;
		if (m_pos >= m_len)
			n = 0;
		else if ((m_pos+nCount) > m_len)
		{
			n = (UINT)(m_len - m_pos);
		}
		else
			n = nCount;
		if (n)
		{
			memcpy(lpBuf,m_p+m_pos,n);
			m_pos += n;
		}
		return n;
	}
	virtual void Write(const void* lpBuf, UINT nCount)
	{
		::AfxThrowFileException( CFileException::generic );
	}
	virtual void LockRange(ULONGLONG dwPos, ULONGLONG dwCount)
	{
		::AfxThrowFileException( CFileException::generic );
	}
	virtual void UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount)
	{
		::AfxThrowFileException( CFileException::generic );
	}
	virtual void Abort()
	{
	};
	virtual void Flush()
	{
	};
	virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
		void** ppBufStart = NULL, void** ppBufMax = NULL)
	{
		return 0;
	}
	virtual void Close()
	{
		if (m_p)
			delete m_p;
		/*
		if (m_hrsrc)
		{
			::DeleteObject(m_hrsrc);
		}
		*/
		m_hrsrc = NULL;
		m_hFile = NULL;
		m_p =   NULL;		
	};
	BOOL Abre( LPCTSTR resourcetype, UINT resourceid )
	{
		m_hrsrc = NULL;
		m_hFile = NULL;
		m_p =   NULL;
		m_len = 0;
		m_pos = 0;
		// CFile::modeRead | CFile::typeBinary 				
		m_hrsrc = ::FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(resourceid),resourcetype);
		if (!m_hrsrc)
			return FALSE;

		HGLOBAL h;

		m_len = (LONGLONG)::SizeofResource(AfxGetInstanceHandle(),m_hrsrc);
		h = ::LoadResource(AfxGetInstanceHandle(),m_hrsrc);

		void *pp = NULL;
		if (h)
		{
			pp = ::LockResource(h);
		}
		if (!pp)
		{
			DeleteObject(m_hrsrc);
			m_hrsrc = NULL;
			return FALSE;
		}

		m_p = new unsigned char[(unsigned int)m_len];
		memcpy(m_p,pp,(size_t)m_len);
		::DeleteObject(m_hrsrc);
		m_hrsrc = NULL;
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDsxFrame construction/destruction
#ifdef DSPROFUIS

void CDsxFrame::RecargaSkinBin() {
	if (m_paintmanager) {
		CString s;
		s = theApp.m_dirbase + theApp.m_aparienciafile;
		((CExtPaintManagerSkin *)m_paintmanager)->m_Skin.SerializeFile( LPCTSTR(s), false, false );
	}
}

void CDsxFrame::InstalaLook() {
   CString s;

   if (m_paintmanager) {
	   delete m_paintmanager;
	   m_paintmanager = NULL;
   }
   
   s = theApp.m_dirbase + theApp.m_aparienciafile;
   if (theApp.m_aparienciabase == 4)
   {
	   CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
   	   if( ! pPM->m_Skin.SerializeFile( LPCTSTR(s), false, false ) )
	   {
			::AfxMessageBox( _T("Failed to load skin.") );
			delete pPM;
			theApp.m_aparienciabase = 5;
	   }
	   else
	   {
		  m_paintmanager = (CExtPaintManager *)pPM;
		  g_PaintManager.InstallPaintManager( pPM );
		  theApp.m_aparienciabase = 3;
	   }
   }
   else if (theApp.m_aparienciabase == 3)
   {
  	  CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
	  if( ! pPM->m_Skin.Load( LPCTSTR(s) ) )
	  {
				::AfxMessageBox( _T("Failed to load skin.") );
				delete pPM;
				theApp.m_aparienciabase = 5;
	  }
	  else
	  {
		  s.Replace(".xml",".Skin");
		  pPM->m_Skin.SerializeFile( LPCTSTR(s), true, false );
		  m_paintmanager = (CExtPaintManager *)pPM;
		  g_PaintManager.InstallPaintManager( pPM );
	  }
   }

   if (theApp.m_aparienciabase == 5)
   {
	   CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
	    bool bRetVal = false;
		try
		{			
			CFileResource _file(LPCTSTR( "Skin" ),IDR_SKIN1);
			bRetVal =
				pPM->m_Skin.SerializeFile(
					_file,
					false,
					false
					);			
		} // try
		catch( CException * pException )
		{
			pException->Delete();
		} // catch( CException * pException )
		catch( ... )
		{
		} // catch( ... )
		if (!bRetVal)
		{
			delete pPM;
			theApp.m_aparienciabase = 2;
		}
		else
		{
			theApp.m_aparienciabase = 3;
		    m_paintmanager = (CExtPaintManager *)pPM;
		    g_PaintManager.InstallPaintManager( pPM );
		}
   }  


   if (theApp.m_aparienciabase == 2)
   {
	/* CExtPaintManagerOffice2007_R1 */
	   /*
	   theApp.m_color1 = RGB(0,0,0);
	   theApp.m_color2 = RGB(0,0,0);
	   */

		m_paintmanager = new CDsProfuisCustomOffice2007_R1();
		g_PaintManager.InstallPaintManager( m_paintmanager );		
		if (theApp.m_colorset1 && theApp.m_colorset2)
			((CDsProfuisCustomOffice2007_R1 *)m_paintmanager)->SetThemeColors(theApp.m_color1,theApp.m_color2);		
   }
   else if (theApp.m_aparienciabase == 1)
   {
	    m_paintmanager = new CDsProfuisCustomOffice2003();
	    g_PaintManager.InstallPaintManager( m_paintmanager );
		if (theApp.m_colorset1 && theApp.m_colorset2)
			((CDsProfuisCustomOffice2003 *)m_paintmanager)->SetThemeColors(theApp.m_color1,theApp.m_color2);
   }
}
#endif


#ifdef DSPROFUIS
CDsxFrame::CDsxFrame()// : m_pBarPersistent( NULL )
#else
CDsxFrame::CDsxFrame()
#endif
{
   m_alsoltarcentrar = TRUE;
   m_hayquecentrar = FALSE;

   m_sinbarras = 0;
   m_debugtimer = 0;
   m_signaltimer = 0;	
#ifdef DSPROFUIS

   m_notifyicon = FALSE;

   m_paintmanager = NULL;

   InstalaLook();

   /*
		COLORREF m_clrForceTextDisabled, m_clrForceTextNormal,
			m_clrForceTextHover, m_clrForceTextPressed,
			m_clrCustomAccentEffectForIcon;
		COLORREF m_clrForceBk;
	*/

   m_wndSplitter = new CDSSplitterWnd();
   CExtPopupMenuWnd::g_bMenuExpanding = false;	
   CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
   ::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
   m_dataFrameWP.showCmd = SW_HIDE;
#else
   m_wndSplitter = new CSplitterWnd();
#endif
}

CDsxFrame::~CDsxFrame()
{   
   if (theApp.m_conexion)
      delete theApp.m_conexion;
   delete m_wndSplitter;
}


void CDsxFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
#ifdef DSPROFUIS
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
#endif

	CFrameWnd::ActivateFrame(nCmdShow);
}


int CDsxFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOadIcon
#ifdef DSXTLIB	
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#else
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#endif

	if (getenv("DS_SINBARRAS")) {
		m_sinbarras = atoi(getenv("DS_SINBARRAS"));
		if (m_sinbarras < 0)
			m_sinbarras = 0;
	}


	CMenu* pMenu = GetSystemMenu(FALSE);
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);			
	}

	if (!theApp.m_sicono.IsEmpty())
	{
		CString s;
		s = theApp.m_dirbase;
		s += theApp.m_sicono;		
		LONG image = (LONG)LoadImage(AfxGetInstanceHandle(),s,IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
		if (image)
			SetClassLong(m_hWnd, GCL_HICON,image);
	}

#ifdef DSPROFUIS
	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	VERIFY( g_CmdManager->ProfileSetup(pApp->m_pszProfileName, GetSafeHwnd() )	
	&& g_CmdManager->UpdateFromMenu( pApp->m_pszProfileName, IDR_MAINMENU)
	&& g_CmdManager->UpdateFromToolBar( pApp->m_pszProfileName, IDR_TOOLMAINFRAME )
	&& g_CmdManager->UpdateFromToolBar( pApp->m_pszProfileName, IDR_TOOLMENSA )
	&& g_CmdManager->UpdateFromToolBar( pApp->m_pszProfileName, IDR_TOOLGRID )
	&& g_CmdManager->UpdateFromToolBar( pApp->m_pszProfileName, IDR_NAVEGA )
	&& g_CmdManager->UpdateFromToolBar( pApp->m_pszProfileName, IDR_TOOLBARMENU )
	);

#endif


	if (!theApp.m_tooltip)
		theApp.m_tooltip = new CToolTipCtrl();
	if (theApp.m_tooltip) {
		theApp.m_tooltip->Create(this,TTS_NOPREFIX | TTS_ALWAYSTIP | 0x40);    
		theApp.m_tooltip->Activate( TRUE );
		::SendMessage(theApp.m_tooltip->m_hWnd, TTM_SETMAXTIPWIDTH, 0, 200);
	}

	DWORD visible;
	/*
	if (theApp.m_SuperPadreHwnd)
	{	
		::SetParent(m_hWnd,theApp.m_SuperPadreHwnd);
		visible = 0;
	}
	else
	*/
	{
		visible = WS_VISIBLE;

#ifdef DSPROFUIS
		if(!m_wndMenuBar.Create(
			_T("Menú"),
			this,
			IDR_MAINMENU)|| 
			!m_wndMenuBar.LoadMenuBar(IDR_MAINMENU)
			)
		{
			TRACE0("Failed to create menubar\n");
			return -1; 
		}
#else
		if (!m_wndMenuBar.CreateEx(this, WS_CHILD | visible | CBRS_TOP
			| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_RAISEDBORDER,CRect(0,0,0,0),ID_TTOOLMENU) ||
			!m_wndMenuBar.LoadMenuBar(IDR_MAINMENU))
		{
			TRACE0("Failed to create menubar\n");
			return -1;      // failed to create
		}	
#endif

		theApp.m_MenuBar = &m_wndMenuBar;

#ifdef DSPROFUIS
		if( !m_wndToolBar.Create(
				_T( "Barra Principal" ),
				this,
				IDR_TOOLMAINFRAME)			
			)
		{
			TRACE0( "Failed to create toolbar" );
			return -1;
		}		
		if (!m_wndToolBar.LoadToolBar( IDR_TOOLMAINFRAME))
		{
			TRACE0( "Failed to create toolbar" );
			return -1;
		}
		//m_wndToolBar.SetFont(GetFont());

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING|MF_ENABLED,ID_APP_ABOUT, "Acerca de");
		}
#else
		if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | visible | CBRS_TOP
		  | CBRS_GRIPPER | /*CBRS_TOOLTIPS | */CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(IDR_TOOLMAINFRAME))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}		
#endif

#ifndef DSPROFUIS
		if (theApp.m_tooltip) {
			m_wndToolBar.GetToolBarCtrl().SetToolTips(&theApp.m_tooltip);
		}
		m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
#endif
		
	    CRect rect,rectm;
	    int index = 0;

#ifdef DSPROFUIS
		
		int pepe;
		while( (pepe = m_wndToolBar.GetButtonID(index)) !=ID_TICSCON) 
		{
			index++;		
		}
	    //m_wndToolBar.GetButtonRect(index, &rectm);		
		//rectm.right = rectm.left+53;
		rectm = CRect(0,0,20,20);
#else
	    while(m_wndToolBar.GetItemID(index)!=ID_TICSCON) index++;
	    m_wndToolBar.SetButtonInfo(index, ID_TICSCON, TBBS_SEPARATOR, 20 );
	    m_wndToolBar.GetItemRect(index, &rectm);	   
#endif
		theApp.m_conexion = new CDsImagen();	

		theApp.m_conexion->m_hbmp = ::LoadBitmap(AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDB_TICSCON));

	    if (!((CWnd *)theApp.m_conexion)->Create((LPCTSTR)NULL,(LPCTSTR)NULL,WS_CHILD|WS_VISIBLE,rectm, &m_wndToolBar,ID_TICSCON))
		{
		   TRACE0("Failed to create dsimagen\n");		   
		}
#ifdef DSPROFUIS
		m_wndToolBar.SetButtonCtrl(index,theApp.m_conexion);		
#endif

		//m_CToolDropTarget.Register(&m_wndToolBar);
		m_CToolDropTarget.Register(&m_wndMenuBar);

#ifdef DSPROFUIS
		if( !m_wndToolMensa.Create(
				_T( "Barra de Mensaje" ),
				this,
				IDR_TOOLMENSA
			)
			|| !m_wndToolMensa.LoadToolBar( IDR_TOOLMENSA )
			)
		{
			TRACE0( "Failed to create toolbar" );
			return -1;
		}
#else
	    if (!m_wndToolMensa.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | visible | CBRS_TOP
		  | CBRS_GRIPPER | /*CBRS_TOOLTIPS | */CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_TTOOLMENSA))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
		if (!m_wndToolMensa.LoadToolBar(IDR_TOOLMENSA))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
#endif
#ifndef DSPROFUIS
		if (theApp.m_tooltip)
			m_wndToolMensa.GetToolBarCtrl().SetToolTips(&theApp.m_tooltip);
		m_wndToolMensa.SetBarStyle(m_wndToolMensa.GetBarStyle() & ~CBRS_HIDE_INPLACE);
#endif

		index = 0;
#ifdef DSPROFUIS
	   while(m_wndToolMensa.GetButtonID(index)!=ID_TOOLMENSA) index++;
	   // m_wndToolMensa.GetButtonRect(index, &rectm);
	   // rectm.right = rectm.left+530;
	   rectm = CRect(0,0,535,20);

	   theApp.m_mensa = new CExtEdit();
#else
	   while(m_wndToolMensa.GetItemID(index)!=ID_TOOLMENSA) index++;	   
	   m_wndToolMensa.SetButtonInfo(index, ID_TOOLMENSA, TBBS_SEPARATOR, 510 );
	   m_wndToolMensa.GetItemRect(index, &rectm);
	   rectm.right += 21;

	   theApp.m_mensa = new CEdit();
#endif
	   
	   if (!theApp.m_mensa->Create(WS_CHILD|ES_CENTER|ES_READONLY|WS_VISIBLE,rectm, &m_wndToolMensa, ID_TOOLMENSA))
	   {
		   TRACE0("Failed to create edit-box\n");
		   return -1;
	   }
	   theApp.m_mensa->ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);

#ifdef DSPROFUIS
		m_wndToolMensa.SetButtonCtrl(index,theApp.m_mensa);
#endif

		//m_wndToolMensa.SetFont(GetFont());

#ifdef DSPROFUIS
		if( !m_toolweb.Create(
				_T( "Barra de Web" ),
				this,
				IDR_NAVEGA
			)
			|| !m_toolweb.LoadToolBar( IDR_NAVEGA )
			)
		{
			TRACE0( "Failed to create toolbar" );
			return -1;
		}
#else
		if (!m_toolweb.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | visible | CBRS_TOP
		  | CBRS_GRIPPER | /*CBRS_TOOLTIPS | */CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_TTOOLWEB) ||
			!m_toolweb.LoadToolBar(IDR_NAVEGA))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
#endif

#ifndef DSPROFUIS
		if (theApp.m_tooltip)
			m_toolweb.GetToolBarCtrl().SetToolTips(&theApp.m_tooltip);
		m_toolweb.SetBarStyle(m_toolweb.GetBarStyle() & ~CBRS_HIDE_INPLACE);
#endif

	   index = 0;
#ifdef DSPROFUIS
	   while(m_toolweb.GetButtonID(index)!=IDD_MIUSERWEB) index++;
	   //m_toolweb.GetButtonRect(index, &rectm);
	   //rectm.right = rectm.left+199;
	   rect = CRect(0,0,221,220);
#else
	   while(m_toolweb.GetItemID(index)!=IDD_MIUSERWEB) index++;
	   m_toolweb.SetButtonInfo(index, IDD_MIUSERWEB, TBBS_SEPARATOR, 200 );	   
	   m_toolweb.GetItemRect(index, &rect);
	   rect.right += 21;
	   rect.top +=1;
	   rect.bottom += 200;
#endif

	   if (!m_toolweb.m_navega.Create(WS_CHILD| CBS_AUTOHSCROLL | 
										   CBS_DROPDOWN | CBS_HASSTRINGS | WS_VISIBLE ,
										   rect, &m_toolweb, IDD_MIUSERWEB))
	   {
		   TRACE0("Failed to create combo-box\n");
		   return -1;
	   }
#ifdef DSPROFUIS
		m_toolweb.SetButtonCtrl(index,&m_toolweb.m_navega);
		m_toolweb.m_navega.SetFont(
		CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );
		m_toolweb.m_navega.SetItemHeight(
		-1, m_toolweb.m_navega.GetItemHeight(-1) - 1 );
		g_CmdManager->CmdGetPtr( pApp->m_pszProfileName, m_toolweb.m_navega.GetDlgCtrlID() )->m_sMenuText = _T( "Hyperlinks" );

		if( !m_dsmenubar.Create(
				_T( "Menú de opciones" ),
				this,
				IDR_TOOLBARMENU
			)
			|| !m_dsmenubar.LoadToolBar( IDR_TOOLBARMENU )
			)
		{
			TRACE0( "Failed to create toolbar" );
			return -1;
		}

		rect.right += 20;
	    if (!m_dsmenubar.m_menus[0].Create(WS_CHILD| CBS_AUTOHSCROLL | WS_VSCROLL| WS_HSCROLL|
										   CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VISIBLE ,
										   rect, &m_dsmenubar, IDD_TOOLBARMENU1))
	    {
		   TRACE0("Failed to create combo-box\n");
		   return -1;
	    }		
		::InitializeFlatSB(m_dsmenubar.m_menus[0].m_hWnd);

	    if (!m_dsmenubar.m_menus[1].Create(WS_CHILD| CBS_AUTOHSCROLL | WS_VSCROLL| WS_HSCROLL|
										   CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VISIBLE ,
										   rect, &m_dsmenubar, IDD_TOOLBARMENU2))
	    {
		   TRACE0("Failed to create combo-box\n");
		   return -1;
	    }		
		::InitializeFlatSB(m_dsmenubar.m_menus[1].m_hWnd);
	    if (!m_dsmenubar.m_menus[2].Create(WS_CHILD| CBS_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL| 
										   CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VISIBLE ,
										   rect, &m_dsmenubar, IDD_TOOLBARMENU3))
	    {
		   TRACE0("Failed to create combo-box\n");
		   return -1;
	    }
		::InitializeFlatSB(m_dsmenubar.m_menus[2].m_hWnd);
		

		m_dsmenubar.SetButtonCtrl(0,&m_dsmenubar.m_menus[0]);
		m_dsmenubar.SetButtonCtrl(2,&m_dsmenubar.m_menus[1]);
		m_dsmenubar.SetButtonCtrl(4,&m_dsmenubar.m_menus[2]);		

		//m_dsmenubar.SetFont(CFont::FromHandle((HFONT)GetFont()));

#endif

#ifdef DSPROFUIS
	    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);		
#else
	    m_wndMenuBar.EnableDockingEx(CBRS_ALIGN_ANY);
#endif

	   //m_wndMenuBar.SetFont(GetFont());

	   HFONT hfont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

	   m_toolweb.m_navega.EnableWindow(FALSE);
	   m_toolweb.m_navega.SendMessage(WM_SETFONT,(WPARAM)hfont,MAKELPARAM(TRUE, 0));   

	   m_toolweb.m_navega.ShowWindow(SW_SHOW);
    
	   m_toolweb.m_navega.AddString(theApp.m_url);
	   m_toolweb.m_navega.SetCurSel(0);	   

#ifdef DSPROFUIS
		if( !m_gridtool.Create(
				_T( "Barra de Grid" ),
				this,
				IDR_TOOLGRID
			)
			|| !m_gridtool.LoadToolBar( IDR_TOOLGRID )
			)
		{
			TRACE0( "Failed to create toolbar" );
			return -1;
		}
#else
	    if (!m_gridtool.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | visible | CBRS_TOP
		  | CBRS_GRIPPER | /*CBRS_TOOLTIPS | */CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_TTOOLGRID))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
		if (!m_gridtool.LoadToolBar(IDR_TOOLGRID))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
#endif

		//m_toolweb.SetFont(GetFont());
		//m_gridtool.SetFont(GetFont());

#ifndef DSPROFUIS
		if (theApp.m_tooltip)
			m_gridtool.GetToolBarCtrl().SetToolTips(&theApp.m_tooltip);
		m_gridtool.SetBarStyle(m_gridtool.GetBarStyle() & ~CBRS_HIDE_INPLACE);
#endif

	}

   if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
      sizeof(indicators)/sizeof(UINT)))
   {
      TRACE0("Failed to create status bar\n");
      return -1;      // fail to create
   }

   if (!visible)
   {
	  m_wndStatusBar.ShowWindow(SW_HIDE);
#ifdef DSPROFUIS
		if( !CExtControlBar::FrameEnableDocking(this) )
		{
			ASSERT( FALSE );
			return -1;
		}
#else
		EnableDocking(CBRS_ALIGN_ANY);	
#endif
   }

   if (visible)
   {
		m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
			/*CBRS_TOOLTIPS | */CBRS_FLYBY);

		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

		m_gridtool.SetBarStyle(m_gridtool.GetBarStyle() |
			/*CBRS_TOOLTIPS | */CBRS_FLYBY);
  
        m_gridtool.EnableDocking(CBRS_ALIGN_ANY);

		m_wndToolMensa.SetBarStyle(m_wndToolMensa.GetBarStyle() |
			/*CBRS_TOOLTIPS | */CBRS_FLYBY);
		m_wndToolMensa.EnableDocking(CBRS_ALIGN_ANY);

		m_toolweb.SetBarStyle(m_toolweb.GetBarStyle() |
			/*CBRS_TOOLTIPS | */CBRS_FLYBY);
		m_toolweb.EnableDocking(CBRS_ALIGN_ANY);


#ifdef DSPROFUIS
		m_dsmenubar.SetBarStyle(m_dsmenubar.GetBarStyle() |
			/*CBRS_TOOLTIPS | */CBRS_FLYBY);
		m_dsmenubar.EnableDocking(CBRS_ALIGN_ANY);

		if( !CExtControlBar::FrameEnableDocking(this) )
		{
			ASSERT( FALSE );
			return -1;
		}		
#else
		EnableDocking(CBRS_ALIGN_ANY);	
#endif		
		
		DockControlBar(&m_wndMenuBar);
		DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP,NULL);		

		RecalcLayout();
		
        CRect rcLast;
    
        m_wndToolBar.GetClientRect( &rcLast );
        m_wndToolBar.ClientToScreen( &rcLast );

		CRect rcDock(rcLast.right, rcLast.top, rcLast.right+1, rcLast.top+1 );
		DockControlBar(&m_gridtool,(UINT)AFX_IDW_DOCKBAR_TOP,(LPCRECT)&rcDock);

		RecalcLayout();

		DockControlBar(&m_toolweb,AFX_IDW_DOCKBAR_TOP,NULL);

		RecalcLayout();
		
        m_toolweb.GetClientRect( &rcLast );
        m_toolweb.ClientToScreen( &rcLast );

		rcDock.SetRect(rcLast.right, rcLast.top, rcLast.right+1, rcLast.top+1 );
		

		DockControlBar(&m_wndToolMensa,(UINT)AFX_IDW_DOCKBAR_TOP,(LPCRECT)&rcDock);

#ifdef DSPROFUIS
		DockControlBar(&m_dsmenubar);
#endif

		RecalcLayout();						
		

#ifndef DSPROFUIS		
		
        CDockState state;
		state.LoadState("DockBars");		
		if (state.m_arrBarInfo.GetSize() > 0 && state.GetVersion() > 1)
		{			
			SetDockState( state );
			RecalcLayout();
		}
		
#else
	if( !CExtControlBar::FrameInjectAutoHideAreas(this) )
	{
		ASSERT( FALSE );
		return -1;
	}

//	CExtDynamicBarSite::m_nBarListInMenusCmdID = ID_DYNAMIC_BARS_LIST;
//    CExtDynamicBarSite::Install( this );
/*
HICON hIcon = (HICON)
		::LoadImage(
			::AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,
			16,
			16,
			0
			);
	ASSERT( hIcon != NULL );
CExtCmdIcon icon;
	icon.AssignFromHICON( hIcon, false );
	m_pBarPersistent =
		CExtDynamicBarSite::BarAlloc(
			_T("Persistent Bar"),
			icon,
			0,
			RUNTIME_CLASS(CSimpleControlBar),
			true // this bar is persistent
			);
	ASSERT( m_pBarPersistent != NULL );
//	m_pBarPersistent->m_nMdiMenuResourceID =
//		IDR_DYNAMIC_BAR;
//	m_pBarPersistent->m_nMdiAccelTableResourceID =
//		IDR_MAINFRAME;
UINT nCmdID = (UINT)m_pBarPersistent->GetDlgCtrlID();
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();


CSimpleDynamicEdit * pEdit = new CSimpleDynamicEdit;
	pEdit->m_brush.CreateSolidBrush(
			( STATIC_DOWNCAST(
				CSimpleControlBar,
				m_pBarPersistent
				)
			) -> m_clrEditorBackground
		);
	VERIFY(
		pEdit->Create(
			m_pBarPersistent->m_hWnd,
			_T("\r\n")
			_T("This is persistent bar.\r\n")
			_T("\r\n")
			_T("Persistent bars are not re-created\r\n")
			_T("when the CExtDynamicBarSite::StateLoad()\r\n")
			_T("method is invoked.\r\n")
			_T("\r\n")
			)
		);


static const CSize _sizeDesiredDocked( 100, 100 );
static const CSize _sizeDesiredFloating( 200, 150 );
	m_pBarPersistent->SetInitDesiredSizeHorizontal( _sizeDesiredDocked );
	m_pBarPersistent->SetInitDesiredSizeVertical( _sizeDesiredDocked );
	m_pBarPersistent->SetInitDesiredSizeFloating( _sizeDesiredFloating );
	ShowControlBar( m_pBarPersistent, TRUE, TRUE );

	
	CExtDynamicBarSite::StateLoad(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			);	
	
*/


#endif
   }

#ifdef DSXTLIB	
    // Initialize the command bars
    if (!InitCommandBars())
        return -1;

    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)
    {
        TRACE0("Failed to create command bars object.\n");
        return -1;      // fail to create
    }

    // Add the menu bar
    CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
        _T("Menu Bar"), IDR_MAINMENU);
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }

    // Create ToolBar
    CXTPToolBar* pToolBar = (CXTPToolBar*)
        pCommandBars->Add(_T("Standard"), xtpBarTop);
    if (!pToolBar || !pToolBar->LoadToolBar(IDR_TOOLMAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    // Set Office 2003 Theme
    CXTPPaintManager::SetTheme(xtpThemeOffice2003);
#endif

	//if (getenv("DS_SUBCONTRATA"))
	

	if (m_sinbarras)
	{		
		if (m_sinbarras == 3) {
			m_wndMenuBar.ShowWindow(SW_SHOW);
			m_wndToolBar.ShowWindow(SW_SHOW);
			m_gridtool.ShowWindow(SW_SHOW);	
		}
		else {
			m_wndMenuBar.ShowWindow(SW_HIDE);
			m_wndToolBar.ShowWindow(SW_HIDE);
			m_gridtool.ShowWindow(SW_HIDE);	
		}
		m_wndToolMensa.ShowWindow(SW_HIDE);	    	    
	    m_toolweb.ShowWindow(SW_HIDE);		

		if (m_sinbarras == 3) {
			m_wndToolBar.EnableWindow(TRUE);
			m_gridtool.EnableWindow(TRUE);					
			m_wndMenuBar.EnableWindow(TRUE);
		}
		else {
			m_wndToolBar.EnableWindow(FALSE);
			m_gridtool.EnableWindow(FALSE);					
			m_wndMenuBar.EnableWindow(FALSE);
		}
		m_wndToolMensa.EnableWindow(FALSE);
		m_toolweb.EnableWindow(FALSE);		

		if (m_sinbarras == 2 || m_sinbarras == 3)
		{
			m_dsmenubar.ShowWindow(SW_HIDE);		
			m_dsmenubar.EnableWindow(FALSE);
		}
		else
		{
			m_dsmenubar.ShowWindow(SW_SHOW);		
			m_dsmenubar.EnableWindow(TRUE);		
		}
	    if (m_sinbarras)
	    {
		    m_wndSplitter->SetColumnInfo( 0,0,0);
	    }
	}
	else
	{		
	    m_wndToolBar.ShowWindow(SW_SHOW);
	    m_wndToolMensa.ShowWindow(SW_SHOW);
	    m_gridtool.ShowWindow(SW_SHOW);	
	    m_wndMenuBar.ShowWindow(SW_SHOW);
	    m_toolweb.ShowWindow(SW_SHOW);		
        m_dsmenubar.ShowWindow(SW_HIDE);	
		m_dsmenubar.EnableWindow(FALSE);
	}

#ifdef DSPROFUIS
	if (m_sinbarras != 3) {
		CExtControlBar::ProfileBarStateLoad(
			this,// some kind of CFrameWnd
			"Diagram Software S.L.", // application registry key (usually company name)
			"DockBars", // application profile name (usually product name)  pApp->m_pszProfileName       // Prof-UIS profile name
			"Diagram"
		);
	}
#endif

	RecalcLayout();
    return 0;
}


void CDsxFrame::OnSelCalendario(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (pNMHDR->hwndFrom)
	{
		::SendMessage(pNMHDR->hwndFrom,WM_COMMAND,MCN_SELECT,MCN_SELECT);
	}	
	*pResult = 0;
}


BOOL CDsxFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.hMenu = NULL;
	cs.style &= ~WS_VISIBLE;
	/*
	if (theApp.m_SuperPadreHwnd)
	{	
		cs.hwndParent = theApp.m_SuperPadreHwnd;		
	}
	*/
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

BOOL CDsxFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                                CCreateContext* pContext)
{

	/*
   if (theApp.m_SuperPadreHwnd)
   {
	   return CFrameWnd::OnCreateClient(lpcs,pContext);
   }
   */

   if (!m_wndSplitter->CreateStatic(this, 1, 2))
      return FALSE;   

   if (!m_wndSplitter->CreateView(0, 0, RUNTIME_CLASS(CDsTreeMenu), CSize(170, 100), pContext) ||
       !m_wndSplitter->CreateView(0, 1, RUNTIME_CLASS(CDsxView), CSize(100, 100), pContext))
   {
      m_wndSplitter->DestroyWindow();
      return FALSE;
   }
   
   {
      HDC hdc = ::GetDC(NULL);
      if (::GetDeviceCaps(hdc,HORZRES) < 800)
	  {
         theApp.m_lowres = TRUE;
      }
      ::ReleaseDC(NULL,hdc);      
   }
   
   
   int sideal = theApp.GetProfileInt("Posiciones","TREEMAX",-1);
   int smin = theApp.GetProfileInt("Posiciones","TREEMIN",0);   
   
   if (sideal != -1)
   {
      m_wndSplitter->SetColumnInfo( 0,sideal,smin);
   }
   else if (theApp.m_lowres == TRUE)
   {
      m_wndSplitter->SetColumnInfo( 0,0,0);
   }
   m_wndSplitter->ModifyStyle(SPLS_DYNAMIC_SPLIT,0);
   
   
   return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDsxFrame diagnostics

#ifdef _DEBUG
void CDsxFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CDsxFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDsxFrame message handlers

BOOL CDsxFrame::SetStatusBarText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate)
{
   return m_wndStatusBar.SetPaneText(nIndex,lpszNewText,bUpdate);
}

BOOL CDsxFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
#ifdef PROFUIS
   if( CExtDynamicBarSite::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
            return TRUE;
#endif

    UINT salva = nID;

    nID &= 0xffff;

	if (nID == TECLA_SUPERUSUARIO || nID == PIDO_VEREURO ||
		     nID == PIDO_SUPERBLOQUEO || nID == PIDO_SUPERDESBLOQUEO || nID == PIDO_SUPERCAMBIO
			 || nID == PIDO_SUPERTODO || nID == PIDO_SUPERDESTODO)
	{
		return TRUE;
	}	

   if (nID == TECLA_SUPERUSUARIO && getenv("SOPORTE_DS"))
   {
	   return TRUE;
   }

   if (nID == IDD_TOOLBARMENU1 || nID == IDD_TOOLBARMENU2 || nID == IDD_TOOLBARMENU3 || nID == ID_TOOLMENSA)
	   return TRUE;

   if (nID == IDD_MIUSERWEB)
   {
	   if (theApp.m_phtml)	   
	       return TRUE;
	   else
	       return FALSE;
   }

   
   CDsxDoc *pDoc = (CDsxDoc *)GetActiveDocument();       

   if (!pDoc)
   {
	   nID = salva;
#ifdef DSXTLIB
       return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
#else
       return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
#endif
   }

   if (nID == ID_TICSCON || nID == ID_AVISOGLOBO)
   {
	   return TRUE;
   }

   if (nID == ID_USERLIST)
   {
	  if (theApp.m_9wnd)
	  {
		if (pDsSys && pDsSys->DespachadorInterno9(21,(void *)NULL)) return FALSE;
	  }
	  else
	  {
		if (pDsSys && pDsSys->DespachadorInterno(21,(void *)NULL)) return FALSE;
	  }
	  return TRUE;
   }

   if (nID == ID_BUSCON) {
	   if (pDsSys) 
		   return pDsSys->haybuscon;
	   return FALSE;
   }

   if ((nID ==  ID_MULTITAREA || nID == ID_VERTREEMENU) && theApp.m_Embebido) 
	   return FALSE;

   /*
   if (nID == ID_CENTRAR && theApp.m_9wnd)
	   return FALSE;
	*/

   if (nID == PIDO_EMPRESA)
   {
       if (pDoc->StatusEnWinsysMenu() )
            return TRUE;
       else
            return FALSE;
   }
   
   if (nID == ID_TRASMITIR)
   { 	  
	  if (theApp.m_9wnd)
	  {
		if (pDsSys && pDsSys->DespachadorInterno9(9,(void *)NULL)) return TRUE;
	  }
	  else
	  {
		if (pDsSys && pDsSys->DespachadorInterno(9,(void *)NULL)) return TRUE;
	  }
      return FALSE;
   }
   
   if (nID == ID_FILE_PRINT || nID == ID_FILE_PRINT_PREVIEW)
   {
      return FALSE;
   }
   
  if ((nID == ID_AJUSTASIZE || nID == ID_OTROSFONTS || nID == ID_FONTMENSA || nID == ID_FONTBOTOM ||
     nID ==  ID_FONTTEXTO || nID ==  ID_FONTINTEN || nID == ID_FONTAUTO || nID == ID_FONTDEFAULT || 
     nID == ID_FONTGRID || nID == ID_SETCOLORES || nID == ID_SETCOLORES1 || nID == ID_SETCOLORES2 ||
	 nID == ID_SETCOLORES3 || nID == ID_SETCOLORES4 || nID == ID_SETCOLORES5) ||
	 ((nID == ID_9FONTDEFAULT ||
	   nID == ID_9FONTAUTO    ||
	   nID == ID_9AJUSTASIZE  ||
	   nID == ID_9FONTTEXTO   ||
	   nID == ID_9OTROSFONTS  ||
	   nID == ID_9FONTINTEN   ||
	   nID == ID_9FONTMENSA   ||
	   nID == ID_9FONTBOTOM   ||
	   nID == ID_9FONTEDITA   ||
	   nID == ID_9SETCOLORES ||
	   nID == ID_AJUSTARPANTALLA9_100 ||
	   nID == ID_AJUSTARPANTALLA9_100 ||
	   nID == ID_AJUSTARPANTALLA9_125 ||
	   nID == ID_AJUSTARPANTALLA9_150 ||
	   nID == ID_PANTALLA9_175 ||
	   nID == ID_PANTALLA9_200
	   ) && pDoc->StatusEnWinsysMenu()))
     return TRUE;

  if (nID == ID_RELACION)
  {
     return(LadoActivo(1));
  }
  if (nID == ID_CLAVES)
  {
     return((LadoActivo(1) || LadoActivo(2)));
  }
  
  if (nID == MID_DISENO || nID == ID_TOOLFUNCION || nID == ID_TOOLIR || (nID >= 800 && nID <= 999) || (nID >= MID_XPRIMERA  && nID < MID_ACTUALIZAMENU/*MID_XOPCION1*/) || (nID >=  DISPARO_CONTROL && nID < PREMAX_DISPARO_CONTROL))
  {
     return TRUE;
  }  

  if (nID == ID_ASISTENCIA)
	  return TRUE;

  if (nID == ID_VIEW_TOOLGRID || nID == ID_VIEW_TOOLMENSA || nID == ID_VIEW_TOOLWEB)
  {
	   CMenu *main = GetMenu();	    
	   if (main)
	   {		
		  CMenu *popup = main->GetSubMenu( 2 );
		  if (popup)
		  {
			  CControlBar *pbar;
			  pbar = GetControlBar( nID );
			  if (pbar)
			  {
				 if (pbar->IsWindowVisible())
				 {
					 popup->CheckMenuItem( nID,  MF_BYCOMMAND | MF_CHECKED);
				 }
				 else
				 {
					 popup->CheckMenuItem( nID,  MF_BYCOMMAND | MF_UNCHECKED);
				 }
			  }
		  }
	   }
	   return TRUE;
  }

  nID = salva;

#ifdef DSXTLIB
  return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
#else
  return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
#endif
}

static BOOL flasheando = FALSE;
static long cuando_balon = 0;
static BOOL globo_saliendo = FALSE;
static int singlobitos = 0;

LRESULT CDsxFrame::OnTray(WPARAM wParam, LPARAM lParam)
{
	long tt;
	
	if (lParam == NIN_BALLOONUSERCLICK)
	{			
		if (!m_nid.uTimeout)
		{
			tt = time(NULL);
			if (!globo_saliendo && tt != cuando_balon)
			{
				//::MessageBox(NULL,m_nid.szInfo,m_nid.szInfoTitle,MB_ICONHAND|MB_OK);//m_nid.dwInfoFlags
				cuando_balon = tt;
				if (flasheando)
				{
					//::BringWindowToTop(m_hWnd);
					//::SetForegroundWindow(m_hWnd);
				}
				Shell_NotifyIcon(NIM_MODIFY, &m_nid);
			}
		}
		else
		{
			Shell_NotifyIcon(NIM_DELETE,&m_nid);    	
			m_notifyicon = FALSE;
				if (flasheando)
				{
					FLASHWINFO fwi;
					fwi.cbSize = sizeof(fwi);
					fwi.hwnd = m_hWnd;
					fwi.dwFlags = 0;
					fwi.uCount = 0;
					fwi.dwTimeout = 0;
					::FlashWindowEx(&fwi);
					flasheando = FALSE;
				}
		}
	}
	else if (lParam == NIN_BALLOONTIMEOUT)
	{		
		if (!m_nid.uTimeout)
		{
			tt = time(NULL);
			if (!globo_saliendo && tt != cuando_balon)
			{
				cuando_balon = tt;
				if (flasheando)
				{
					//::BringWindowToTop(m_hWnd);
					//::SetForegroundWindow(m_hWnd);
				}				
				Shell_NotifyIcon(NIM_MODIFY, &m_nid);				
			}
		}
		else
		{
			Shell_NotifyIcon(NIM_DELETE,&m_nid);    	
			m_notifyicon = FALSE;
				if (flasheando)
				{
					FLASHWINFO fwi;
					fwi.cbSize = sizeof(fwi);
					fwi.hwnd = m_hWnd;
					fwi.dwFlags = 0;
					fwi.uCount = 0;
					fwi.dwTimeout = 0;
					::FlashWindowEx(&fwi);
					flasheando = FALSE;
				}
		}
	}
	/*
	else if (lParam == WM_MOUSEMOVE)
	{
		Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	}
	*/
	/*
    else if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP)
    {
		ShowWindow(SW_HIDE);
    }
	else if (lParam == WM_LBUTTONDBLCLK || lParam == WM_RBUTTONDBLCLK)
	{
		ShowWindow(SW_SHOW);
	}
	*/
	return TRUE;
}

BOOL CDsxFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{

   WPARAM nID = wParam & 0xffff;
   static WPARAM LastID = 0;
   

   /*
    if (nID == ID_FONDOWEB)
    {
         CDsxDoc * pDoc = ((CDsxDoc *)GetActiveDocument());
         CDsxView* pView;
         POSITION pos = pDoc->GetFirstViewPosition();
         while (pos != NULL)
         {           
             pView = (CDsxView *)pDoc->GetNextView(pos);
             if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) 
             {
                 pView = NULL;
                 continue;     
             }
             if (pView->m_html)
             {
                 pView->m_html->Navega();
             }             
         }
    }
    */

   if (nID == ID_FLAGCOMMAND)
   {
	   /*
	   switch((int)HIWORD(wParam))
	   {
	   
	   case 0:
		   m_wndSplitter->SetColumnInfo( 0, 0, 0);
		   m_wndSplitter->RecalcLayout();
		   break;
	   case 1:
		   m_wndSplitter->SetColumnInfo( 1, 0, 0);
		   m_wndSplitter->RecalcLayout();
		   break;

	   default:
		   break;
	   }
	   */
   }
   else if (nID == ID_TICSCON)
   {
	   static UINT antes = 0;
	   static long tiempo1 = 0;
	   static int cuantos = 0;
	   static int suma = 0;
	   char tmp[512];
	   CString ss;
	   int i;
	   int val;	   
	   long ahora;
	   ss = m_wndStatusBar.GetPaneText( 2 );
	   if ((i = ss.Find("ms>")) > 0)
	   {
		   ss = ss.Mid(i+3);
	   }
	   val = (int)lParam;
	   sprintf(tmp,"<%ld ms>",val);
	   ss = tmp + ss;
	   m_wndStatusBar.SetPaneText(2,ss, TRUE);

	   cuantos++;
	   suma += val;

	   ahora = (long)time(NULL);

	   if ((ahora-tiempo1) > 30)
	   {
		   tiempo1 = ahora;

		   val = suma/cuantos;

		   suma = 0;
		   cuantos = 0;
		   if (theApp.m_conexion)
		   {
			   UINT res = 0;
			   if (val < 190)
			   {
				   res = IDB_TICSCON;
			   }
			   else if (val < 290)
			   {
				   res = IDB_TICSCON1;
			   }
			   else
			   {
				   res = IDB_TICSCON2;
			   }
			   if (antes != res)
			   {
				   antes = res;
				   if (theApp.m_conexion->m_hbmp)
					   DeleteObject(theApp.m_conexion->m_hbmp);
				   theApp.m_conexion->m_hbmp = ::LoadBitmap(AfxGetInstanceHandle( ),MAKEINTRESOURCE(res));
				   theApp.m_conexion->Invalidate();
			   }
		   }
		   {
			   FILE *f;
			   char tmp[512];
			   sprintf(tmp,"%slogs\\ms%02d%02d%02d.log",theApp.m_dirbase,CTime::GetCurrentTime().GetYear()%100,CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay());
			   f = fopen(tmp,"a");
			   if (f)
			   {
				   fprintf(f,"%-10.10s %6ld %02d%02d%02d %d\r\n",(LPCSTR)pDsSys->m_Usuario,getpid(),
					   CTime::GetCurrentTime().GetHour(),
					   CTime::GetCurrentTime().GetMinute(),
					   CTime::GetCurrentTime().GetSecond(),
					   val);
				   fclose(f);
			   }
		   }
	   }
	   return TRUE;
   }
   else if (nID == ID_AVISOGLOBO)
   {
	   DWORD ni_timeout = 0;
	   DWORD ni_flags = NIIF_NONE;	   
	   char ni_texto[512];
	   char ni_titulo[128];
	   char tmp[1024];
	   char *pp = (char *)lParam;	   

	   cuando_balon = 0;
	   if (!pp || !*pp)
	   {
		   if (m_notifyicon)
		   {
				Shell_NotifyIcon(NIM_DELETE,&m_nid);    	
				m_notifyicon = FALSE;
				if (flasheando)
				{
					FLASHWINFO fwi;
					fwi.cbSize = sizeof(fwi);
					fwi.hwnd = m_hWnd;
					fwi.dwFlags = 0;
					fwi.uCount = 0;
					fwi.dwTimeout = 0;
					::FlashWindowEx(&fwi);
					flasheando = FALSE;
				}
		   }
	   }
	   else
	   {
		    int i;
			int np;
			int sal = 0;
			int flash = 0;
			char *op;
			int oo;

			np = 0;
			op = pp;

			pp = tmp;
			strncpy(tmp,(LPCSTR)lParam,sizeof(tmp)-1);
			tmp[sizeof(tmp)-1] = 0;
			ni_texto[0] = 0;
			ni_titulo[0] = 0;

			for (i = 0;pp[i];i++)
			{
				if (pp[i] == 1)
				{
					pp[i] = 0;
					switch(np)
					{
						case 0:
							ni_timeout = atoi(op);
							break;
						case 1:
							oo = atoi(op);
							if (oo > 4)
							{
								oo -= 5;
								flash = 1;
							}
							switch(oo)
							{
							case 1:
								ni_flags = NIIF_INFO;
								break;
							case 2:
								ni_flags = NIIF_WARNING;
								break;
							case 3:
								ni_flags = NIIF_ERROR;
								break;
							case 4:
								ni_flags = NIIF_ERROR;
								sal = 1;
								break;
							}
							break;
						case 2:
							if (strlen(op) > 127)
								op[127] = 0;
							strcpy(ni_titulo,op);
							break;
						case 3:
							if (strlen(op) > 511)
								op[511] = 0;
							strcpy(ni_texto,op);
							break;
					}
					np++;
					op = pp + i + 1;
				}
			}
			if (m_notifyicon)
			{
					Shell_NotifyIcon(NIM_DELETE,&m_nid);    	
					m_notifyicon = FALSE;
					if (flasheando)
					{
						FLASHWINFO fwi;
						fwi.cbSize = sizeof(fwi);
						fwi.hwnd = m_hWnd;
						fwi.dwFlags = 0;
						fwi.uCount = 0;
						fwi.dwTimeout = 0;
						::FlashWindowEx(&fwi);
						flasheando = FALSE;
					}
			}
 			memset(&m_nid, 0 , sizeof(m_nid));   
			m_nid.cbSize = sizeof(NOTIFYICONDATA);   
			m_nid.uID = 10001;
			m_nid.hWnd = m_hWnd;
			m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			m_nid.uCallbackMessage = WM_MITRAYMESSAGE;
			m_nid.hIcon = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
			strncpy(m_nid.szTip,"Diagramx",sizeof(m_nid.szTip)-1);
			m_nid.szTip[sizeof(m_nid.szTip)-1] = 0;
				
			Shell_NotifyIcon(NIM_ADD,&m_nid);    

			m_notifyicon = TRUE;

			m_nid.cbSize=sizeof(NOTIFYICONDATA);
			m_nid.uFlags = NIF_INFO;
			m_nid.uTimeout = ni_timeout;
			m_nid.dwInfoFlags = ni_flags;
			strncpy(m_nid.szInfo,ni_texto,sizeof(m_nid.szInfo)-1);
			m_nid.szInfo[sizeof(m_nid.szInfo)-1] = 0;
			strncpy(m_nid.szInfoTitle,ni_titulo,sizeof(m_nid.szInfoTitle)-1);
			m_nid.szInfoTitle[sizeof(m_nid.szInfoTitle)-1] = 0;
			Shell_NotifyIcon(NIM_MODIFY, &m_nid);
			if (!singlobitos)
			{
				HKEY hKey = NULL;
				LONG retv;
				DWORD Type;
				DWORD Activa;
				DWORD Size;	

				singlobitos = 1;

				retv = RegOpenKeyEx( HKEY_CURRENT_USER,
					"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",				
					0, 
					KEY_READ, 
					&hKey 
					); 
				if (retv == ERROR_SUCCESS) 
				{ 
					 Size = sizeof(Activa);
					 Activa = 1L;
					 Type = REG_DWORD;
					 retv = RegQueryValueEx(  hKey,"EnableBalloonTips",NULL,&Type,(LPBYTE)&Activa,&Size);
					 if (retv == ERROR_SUCCESS && !Activa)
					 {
						 singlobitos = 2;						 
					 }
					 RegCloseKey( hKey  ); 
				} 
			}
			if (singlobitos == 2)
				::MessageBox(m_hWnd,ni_texto,ni_titulo,MB_ICONHAND|MB_OK);
			if (flash)
			{
				if (!flasheando)
				{
					FLASHWINFO fwi;
					fwi.cbSize = sizeof(fwi);
					fwi.hwnd = m_hWnd;
					fwi.dwFlags = FLASHW_ALL | FLASHW_TIMER; //0x00000001 | 0x00000002 | 0x00000004;
					fwi.uCount = 0;
					fwi.dwTimeout = 0;
					::FlashWindowEx(&fwi);
					flasheando = TRUE;
				}
				::BringWindowToTop(m_hWnd);
				::SetForegroundWindow(m_hWnd);
		    }
			else
			{
				if (flasheando)
				{
					FLASHWINFO fwi;
					fwi.cbSize = sizeof(fwi);
					fwi.hwnd = m_hWnd;
					fwi.dwFlags = 0;
					fwi.uCount = 0;
					fwi.dwTimeout = 0;
					::FlashWindowEx(&fwi);
					flasheando = FALSE;
				}
			}

			if (sal)
			{
				if (theApp.m_ulttecla != 807)
				{
					if (theApp.m_9wnd)
					{
						if (pDsSys)
							pDsSys->DespachadorInterno9(99,(void *)NULL);
					}
					theApp.TeclaAlActivo(807);
					if (theApp.m_ulttecla != 807)
					{
						theApp.m_ulttecla = 807;
					}
					globo_saliendo = TRUE;
				}
				/*
				::Sleep(100);
				::PostQuitMessage(0);
				*/
			}
	   }
	   return TRUE;
   }
                
   if (nID == IDD_ENVIACORREO) // 171
   {
       CEnviaCorreo dlg;

       char *pp = (char *)lParam;

       dlg.m_destino = pp;
       dlg.m_origen = pp + 500;
       dlg.m_asunto = pp + 1000;
       dlg.m_documento = pp + 1500;
       dlg.m_nota = pp + 2000;

       if (dlg.DoModal() != IDOK)
       {
           *pp = 0;
       }
       else
       {
           strcpy(pp,dlg.m_destino);
           strcpy(pp+500,dlg.m_origen);
           strcpy(pp+1000,dlg.m_asunto);
           strcpy(pp+1500,dlg.m_documento);
           strcpy(pp+2000,dlg.m_nota);
       }
       
       return TRUE;
   }


   if (nID == ID_NEXT_PANE)
   {
       OnNextPane();
       return TRUE;
   }
   if (nID == ID_PREV_PANE)
   {
       OnPrevPane();
       return TRUE;
   }
   
   if (nID == ID_ALT_DOWN)
   {
       OnAltDown();
       return TRUE;
   }
  
   if (nID == ID_ALT_UP)
   {
       OnAltUp();
       return TRUE;
   }
   CDsxDoc *pDoc = pActiveDoc;//(CDsxDoc *)GetActiveDocument();

   if (!pDoc)
   {
       return CFrameWnd::OnCommand(wParam, lParam);
   }

   if (nID != ID_STARTMYTIMER)
        LastID = nID;

   if (nID >= MENSAJE_CABECERA_0 && nID <= MENSAJE_CABECERA_3)
   {
      SetStatusBarText(nID - MENSAJE_CABECERA_0, (LPCTSTR) lParam, TRUE);
      return TRUE;
   }
   
   if (nID == ID_USERLIST) 
   {
	  if (theApp.m_9wnd)
	  {
		if (pDsSys && pDsSys->DespachadorInterno9(21,(void *)NULL)) return TRUE;
	  }
	  else
	  {
		if (pDsSys && pDsSys->DespachadorInterno(21,(void *)NULL)) return TRUE;
	  }

      if (theApp.m_ctrlactivo)
      {		  
          theApp.m_ctrlactivo->TeclaStd(PIDO_USUARIOS);          
      }
      else
      {
          pDoc->TeclaStd(PIDO_USUARIOS);          
      }
      
      return TRUE;
   }

   if (nID == ID_TRASMITIR)
   {
      if (theApp.m_9wnd)
	  {		 
	     if (pDsSys->DespachadorInterno9(9,(void *)NULL)) 
		 {
			  if (theApp.m_ctrlactivo)
			  {			      
				 theApp.m_ctrlactivo->TeclaStd(PIDO_TRASMISION);		 
			  }
			  else
			  {
				 pDoc->TeclaStd(PIDO_TRASMISION);
			  }
		  }
	  }
	  else
	  {
	     if (pDsSys->DespachadorInterno(9,(void *)NULL)) 
		 {
			  if (theApp.m_ctrlactivo)
			  {			      
				 theApp.m_ctrlactivo->TeclaStd(PIDO_TRASMISION);		 
			  }
			  else
			  {
				 pDoc->TeclaStd(PIDO_TRASMISION);
			  }
		 }
	  }
      return TRUE;
   }   
      
      if (nID == ID_FONTAUTO)
      {
         //theApp.m_EntornoActivo->FontAuto();
         return TRUE;
      }
      else		  
      if (nID == ID_AJUSTASIZE)
      {
         pDoc->PideFuente(&pDoc->m_lfEdit);
         return TRUE;
      }
      else 
      if (nID == ID_OTROSFONTS)
      {         
         pDoc->PideFuente(&pDoc->m_lfLabelR);
         return TRUE;
      }
      else
      if (nID == ID_FONTMENSA)
      {
         pDoc->PideFuente(&pDoc->m_lfTitulos);
         return TRUE;
      }
      else
      if (nID == ID_FONTBOTOM)
      {
         //pDoc->PideFuente(&pDoc->m_lfX);         
         return TRUE;
      }
      else
      if (nID == ID_FONTTEXTO)
      {
         pDoc->PideFuente(&pDoc->m_lfLabel);
         return TRUE;
      }
      else
      if (nID == ID_FONTINTEN)
      {
         pDoc->PideFuente(&pDoc->m_lfLabelI);
         return TRUE;
      }
      else
      if (nID == ID_FONTDEFAULT)
      {
         pDoc->DefaultFonts();
         return TRUE;
      }
      else
      if (nID == ID_FONTGRID)
      {
         pDoc->PideFuente(&pDoc->m_lfGrid);
         return TRUE;
      }
      else
      if (nID == ID_SETCOLORES)
      {
		  //CColorDialog
         CExtColorDlg cdlg(pDoc->m_cGridBk,0,theApp.m_pMainWnd);

         if (cdlg.DoModal() == IDOK)
         {
            pDoc->m_cGridBk = cdlg.m_clrNew; // GetColor();
            pDoc->UpdateFonts();
         }         
         return TRUE;
      }
	  else if (nID == ID_SETCOLORES1) // seleccion de linea
	  {
         CExtColorDlg cdlg(pDoc->m_cGridSelBk,0,theApp.m_pMainWnd);

         if (cdlg.DoModal() == IDOK)
         {
            pDoc->m_cGridSelBk = cdlg.m_clrNew; // GetColor();
            pDoc->UpdateFonts();
         }         
         return TRUE;
	  }
	  else if (nID == ID_SETCOLORES2) // seleccion de campo
	  {
         CExtColorDlg cdlg(pDoc->m_cGridCamBk,0,theApp.m_pMainWnd);

         if (cdlg.DoModal() == IDOK)
         {
            pDoc->m_cGridCamBk = cdlg.m_clrNew; // GetColor();
            pDoc->UpdateFonts();
         }         
         return TRUE;
	  }
	  else if (nID == ID_SETCOLORES3) // Texto menus grid
	  {
         CExtColorDlg cdlg(pDoc->m_cGridFr,0,theApp.m_pMainWnd);

         if (cdlg.DoModal() == IDOK)
         {
            pDoc->m_cGridFr = cdlg.m_clrNew; // GetColor();
            pDoc->UpdateFonts();
         }         
         return TRUE;
	  }
	  else if (nID == ID_SETCOLORES4) // Texto Seleccion linea
	  {
         CExtColorDlg cdlg(pDoc->m_cGridSelFr,0,theApp.m_pMainWnd);

         if (cdlg.DoModal() == IDOK)
         {
            pDoc->m_cGridSelFr = cdlg.m_clrNew; // GetColor();
            pDoc->UpdateFonts();
         }         
         return TRUE;
	  }
	  else if (nID == ID_SETCOLORES5) // texto seleccion de campo
	  {
         CExtColorDlg cdlg(pDoc->m_cGridCamFr,0,theApp.m_pMainWnd);

         if (cdlg.DoModal() == IDOK)
         {
            pDoc->m_cGridCamFr = cdlg.m_clrNew; // GetColor();
            pDoc->UpdateFonts();
         }         
         return TRUE;
	  }
      else if (pDoc->PideFuente9(nID))
	  {
		  return TRUE;
	  }
	  else if (nID == ID_PREGUCLAVE)
      {       
           int *p1 = (int *)lParam;
           char *p = (char *)(p1+2);
           CDlgPregunta prg;
           prg.m_laclave = p;
           prg.m_clave = "";
           p1[1] = prg.DoModal();
           return TRUE;
      }
      else   
      if (nID == ID_MESSAGEBOX)
      {       
           int *p1 = (int *)lParam;
           char *p = (char *)(p1+2);
           UINT type = MB_ICONEXCLAMATION | MB_OK;		   		   
		   for (char *ipp = p;*ipp;ipp++)
			 oem_a_ansi((unsigned char *)ipp);		   
           MessageBox(p,"ATENCION!",type);
		   for (ipp = p;*ipp;ipp++)
			 ansi_a_oem((unsigned char *)ipp);		   

           return TRUE;
      }
      else
      if (nID == ID_ERRORFICHERO)
      {
           int *p1 = (int *)lParam;
           char *p = (char *)(p1+2);
           if (*p)
           {
              p[40] = 0;
              p[41+40] = 0;
              ((CDsxDoc *)GetActiveDocument())->ErrorFichero(p,p+41,p+41+41,p1[1]);
           }
           else
              ((CDsxDoc *)GetActiveDocument())->FinErrorFichero();
           return TRUE;       
      }
      else
      if (nID == ID_SIONO)
      {
       if (lParam)
       {
           int *p1 = (int *)lParam;
           char *p = (char *)(p1+2);
           UINT type = MB_ICONQUESTION | MB_YESNO;
           if (*p == 'S' || *p == 's')
           {
               type |= MB_DEFBUTTON1;
           }
           else
           {
               type |= MB_DEFBUTTON2;
           }
		   for (char *ipp = p+1;*ipp;ipp++)
			 oem_a_ansi((unsigned char *)ipp);		   
           p1[1] = MessageBox(p+1,"ATENCION!",type);
		   for (ipp = p+1;*ipp;ipp++)
			 ansi_a_oem((unsigned char *)ipp);		   
       }
       return TRUE;
      }
      else
      if (nID == ID_SIONOCANCELAR)
      {
           if (lParam)
           {
               int *p1 = (int *)lParam;
               char *p = (char *)(p1+2);
               UINT type = MB_ICONQUESTION | MB_YESNOCANCEL;
               if (*p == 'S' || *p == 's')
               {
                   type |= MB_DEFBUTTON1;
               }
               else
               if (*p == 'C' || *p == 'c')
               {
                   type |= MB_DEFBUTTON3;
               }
               else
               {
                   type |= MB_DEFBUTTON2;
               }
			   for (char *ipp = p+1;*ipp;ipp++)
					oem_a_ansi((unsigned char *)ipp);		   
               p1[1] = MessageBox(p+1,"ATENCION!",type);            
		       for (ipp = p+1;*ipp;ipp++)
			        ansi_a_oem((unsigned char *)ipp);		   
           }
           return TRUE;   
      }
      else
      if (nID == ID_AVISO_DEFIMPRE)
      {
           theApp.GetPrinterDeviceDefaults((PRINTDLG *)lParam);
           return TRUE;
      }
      else
      if (nID == PIDO_EMPRESA)
      {
          if (theApp.m_ctrlactivo)
          {		  
             theApp.m_ctrlactivo->TeclaStd(PIDO_EMPRESA);                          
          }
         else
         {
             pDoc->TeclaStd(PIDO_EMPRESA);
         }
          return TRUE;
      }
      else
      if (nID == ID_DLGKACTIVA)
      {
          int *p1 = (int *)lParam;
          char *p = (char *)(p1+2);
          p[20] = 0;
          p[21+20] = 0;
          //num1,num2,res
          CDlgKActiva pack(p+21+21,p,p+21,"902100177 / 965337743");
          p1[1] = pack.DoModal();
          ::strcpy(p,pack.m_activacion);
          return TRUE;
      }
      else
      if (nID == ID_DLGINDICES)
      {
          static CDlgIndexado *p_mindex = NULL;

          int *p1 = (int *)lParam;
          char *p = (char *)(p1+2);                    
          if (p_mindex && p1[1] != 1)
          {
              if (::IsWindow(p_mindex->m_hWnd))
              {
                  p_mindex->DestroyWindow();
              }
              delete p_mindex;
              p_mindex = NULL;
          }
          switch (p1[1])
          {
          case 0:
          case 3:
              {
                  p[20] = 0;
                  p_mindex = new CDlgIndexado();
                  p_mindex->m_panel1 = p+21;
                  p_mindex->m_panel2 = p;
                  p_mindex->m_panel2.TrimLeft();
                  p_mindex->m_panel2.TrimRight();
                  p_mindex->Create(IDD_INDEXADO,this);              
                  CRect rr,rr1;
                  GetWindowRect(rr);
                  p_mindex->GetWindowRect(rr1);  
                  rr1.OffsetRect((rr.left+rr.Size().cx/2-rr1.Size().cx/2)-rr1.left,(rr.top+rr.Size().cy/2-rr1.Size().cy/2)-rr1.top);
                  p_mindex->MoveWindow(rr1);
                  p_mindex->BringWindowToTop();
                  p_mindex->ShowWindow(SW_SHOW);
                  if (p1[1] == 3)
                  {
                       CWnd *pwnd = p_mindex->GetDlgItem(IDOK);
                       if (pwnd)
                       {
                           pwnd->EnableWindow();
                           pwnd->SetFocus();
                       }
                       p_mindex->RunModalLoop( 0 );
                       p_mindex->DestroyWindow();                      
                       delete p_mindex;
                       p_mindex = NULL;
                  }
              }
              break;
          case 1:
              if (p_mindex)
              {
                  p_mindex->m_panel2 = p;
                  p_mindex->m_panel2.TrimLeft();
                  p_mindex->m_panel2.TrimRight();
                  p_mindex->UpdateData(FALSE);
              }
              break;
          default:
              break;
          }
          MSG msg;
          while (p_mindex && ::PeekMessage(&msg,p_mindex->m_hWnd,0, 0, PM_NOREMOVE) && ::GetMessage((LPMSG)&msg,p_mindex->m_hWnd,0,0))
          {
              if (!::IsDialogMessage(p_mindex->m_hWnd,&msg))
              {
                  if (!theApp.PreTranslateMessage(&msg))
                  {
                      TranslateMessage((LPMSG)&msg);
                      DispatchMessage((LPMSG)&msg);
                  }
              }
          }

          return TRUE;
      }
      else
      if (nID == MID_DISENO || nID == ID_TOOLFUNCION || nID == ID_TOOLIR || (nID >= 800 && nID <= 999) || (nID >= MID_XPRIMERA  && nID < MID_ACTUALIZAMENU) || (nID >=  DISPARO_CONTROL && nID < PREMAX_DISPARO_CONTROL))
      {
	     if (nID >=  DISPARO_CONTROL && nID < PREMAX_DISPARO_CONTROL)
		 {
			 if (theApp.m_runmode || !theApp.m_enrun || theApp.m_ndisparocontrol || theApp.m_ulttecla != -1)
			 {
				 if (theApp.m_ndisparocontrol < 171)
				 {
					theApp.m_disparocontrol[theApp.m_ndisparocontrol] = nID;
					theApp.m_ndisparocontrol++;
				 }
				 return TRUE;
			 }
		 }
         if (nID == ID_TOOLFUNCION)
         {
            CMenu popmenu;
			int i;
            popmenu.CreatePopupMenu();
            CString texto;
            for (i = 1;i < 10;i++)
            {
               texto.Format("F%d",i+1);
               popmenu.InsertMenu(i-1,MF_STRING | MF_BYPOSITION,823+i,texto);
            }
            texto = "F12";
            popmenu.InsertMenu(11,MF_STRING | MF_BYPOSITION,822,texto);
            for (i = 0;i < 10;i++)
            {				
               texto.Format("PF%d (Ctrl-F%d)",i+1,i+1);
               popmenu.InsertMenu(i+10,MF_STRING | MF_BYPOSITION,833+i,texto);
            }
            POINT pp;
            ::GetCursorPos(&pp);
            popmenu.TrackPopupMenu(TPM_CENTERALIGN,pp.x,pp.y-10,this,NULL);
            return TRUE;
         }
         else
         if (nID == ID_TOOLIR)
         {           
            CMenu popmenu;
            popmenu.CreatePopupMenu();
            int n = 0;
			CString ss,s;
			int i;
            for (i = 0;i < 10;i++)
            {              
              if (!theApp.m_pFunciones[i].IsEmpty())
              {		
                 ss = theApp.m_dFunciones[i];
                 ss.TrimLeft();
                 ss.TrimRight();
                 if (ss.GetLength())
                 {
					 if (!i)
						s.Format(" (Ctrl-F12)");
					 else
						s.Format(" (Ctrl-F%d)",i+1);
					 ss += s;
                     popmenu.InsertMenu(i,MF_STRING | MF_BYPOSITION,833+i,ss);
                     n++;
                 }
              }
            }
            if (!n)
            {
              popmenu.InsertMenu(0,MF_STRING | MF_BYPOSITION,0,"No Hay");
            }	 
           
           
            POINT pp;
            ::GetCursorPos(&pp);
            popmenu.TrackPopupMenu(TPM_CENTERALIGN,pp.x,pp.y-10,this,NULL);			
            return TRUE;
         }
         else if (nID >= MID_XPRIMERA  && nID < MID_XOPCION1)
         {
            ((CDsxDoc *)GetActiveDocument())->OpcionDeMenu(nID-MID_XPRIMERA+1);
            return TRUE;
         }
		 else if (nID >= MID_XOPCION1 && nID < MID_ACTUALIZAMENU)
		 {
            ((CDsxDoc *)GetActiveDocument())->OpcionDeMenu((MID_XOPCION1-nID)-1);
            return TRUE;
		 }
         else
         {            
            if (theApp.m_ctrlactivo)
            {		  
                theApp.m_ctrlactivo->TeclaStd((int)wParam);                
            }
            else
            {
                pDoc->TeclaStd((int)wParam);
            }
            return TRUE;            
         }
      }
    else if (nID == ID_STARTMYTIMER)
    {
        if (m_debugtimer)
        {
            KillTimer(m_debugtimer);
            m_debugtimer = 0;
        }
        else
        {
            if (LastID)
            {
                m_debugtimer = SetTimer(LastID,1000,NULL);
            }
        }
    }
	else if (nID == ID_SIGNAL_PING)
	{     
		/*
		{
			int ndp = 0;
			static int esperar = 0;
			while (dispara_botones[ndp] != NULL) {
				if (esperar < 300) {
					esperar++;
					break;
				}
				dispara_botones[ndp]->m_ctrl->PostMessage(WM_COMMAND,ID_Menu32920,0);
				ndp++;
			}
		}
		*/
        

	   /* adaptado para caso hint */
	   if (theApp.m_conhint && theApp.m_ctrlactivo)
	   {
		   if (theApp.m_ctrlactivo->m_tipo == 2 || theApp.m_ctrlactivo->m_tipo == 11)
		   {
			   if (::GetFocus() == ((CDsCombo *)theApp.m_ctrlactivo->m_ctrl)->m_Edit.m_hWnd)
			   {
				   if (!theApp.m_9wnd && theApp.m_hintcounter > 0)
				   {					   
					   pDsSys->DespachadorInterno(199,(void *)&(theApp.m_hintcounter));
				   }
			   }
			   else
				   theApp.m_hintcounter = 0;
		   }
		   else if (theApp.m_ctrlactivo->m_tipo == 7)
		   {
			   if (::GetFocus() == theApp.m_ctrlactivo->m_ctrl->m_hWnd)
			   {
				   if (!theApp.m_9wnd && theApp.m_hintcounter > 0)
				   {
					   pDsSys->DespachadorInterno(199,(void *)&(theApp.m_hintcounter));					   
				   }
			   }
			   else
				   theApp.m_hintcounter = 0;
		   }
	   }
	   else
		   theApp.m_hintcounter = 0;

	   {
		   static unsigned int mi_tiempo_ping = 0;
		   unsigned int ahora;

		   ahora = (unsigned int)time(NULL)/60;
		   if (mi_tiempo_ping != ahora)
		   {			    
			    if (mi_tiempo_ping)
				{
					if (theApp.m_9wnd)
						pDsSys->DespachadorInterno9(299,NULL);
					pDsSys->DespachadorInterno(299,NULL);
				}
				mi_tiempo_ping = ahora;
		   }
	   }

	   /*
	   if (theApp.m_ctrlactivo)
	   {		  
	 	  theApp.m_ctrlactivo->TeclaStd(TECLA_PING);          
	   }
	   else
	   {
		  pDoc->TeclaStd(TECLA_PING);
	   } 
	   */
	   
  	   return TRUE;
	}
	else if (nID == ID_BUSCON)
    {
		pDoc->TeclaStd(ID_BUSCON);
        return TRUE;        
    }
	else if (nID == TECLA_SUPERUSUARIO || nID == PIDO_VEREURO ||
		     nID == PIDO_SUPERBLOQUEO || nID == PIDO_SUPERDESBLOQUEO || nID == PIDO_SUPERCAMBIO
			 || nID == PIDO_SUPERTODO || nID == PIDO_SUPERDESTODO)
	{
		  if (theApp.m_ctrlactivo)
		  {		  
			  theApp.m_ctrlactivo->TeclaStd(nID);          
		  }
		  else
		  {
			  pDoc->TeclaStd(nID);
		  }
		return TRUE;
	}	
	else if (nID == ID_PESTANA)
	{
        int *p1 = (int *)lParam;
        char *p = (char *)(p1+2);
		CDsStdDlg *pdlg;
		pdlg = theApp.StdView()->LocalizaDlg(atoi(p),atoi(p+500),atoi(p+1000),atoi(p+1500),atoi(p+2000));
		if (pdlg)
		{
			if (pdlg->m_padretab && pdlg->m_padretab->m_dsctrl)
			{
				p1[1] = pdlg->m_padretab->m_dsctrl->m_ctrlid;
			}
		}
		return TRUE;
	}
	else if (nID == ID_ANYTECLA)
	{
		  if (theApp.m_ctrlactivo)
		  {		  
			  theApp.m_ctrlactivo->TeclaStd(lParam);          
		  }
		  else
		  {
			  pDoc->TeclaStd(lParam);
		  }
		return TRUE;
	}

  if (nID == ID_VIEW_TOOLGRID || nID == ID_VIEW_TOOLMENSA || nID == ID_VIEW_TOOLWEB)
  {
	  CControlBar *pbar;
	  pbar = GetControlBar( nID );
	  if (pbar)
	  {
		 if (pbar->IsWindowVisible())
		 {
			 pbar->ShowWindow(SW_HIDE);
		 }
		 else
		 {
			 pbar->ShowWindow(SW_SHOWNOACTIVATE);				 
		 }
		 RecalcLayout();
	  }
      return TRUE;
  }

    if (nID == ID_EDIT_COPY || nID == ID_EDIT_PASTE || nID == ID_EDIT_CUT || nID == ID_EDIT_UNDO)
	{
		if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_ctrl)
		{
			theApp.m_ctrlactivo->m_ctrl->SendMessage(WM_COMMAND,wParam, lParam);
		}
	}

	if (nID == ID_ASISTENCIA)
	{
		DWORD dwSize = MAX_PATH;
		TCHAR tchApplicationPath[ MAX_PATH ] = { 0 };
		HRESULT hr = AssocQueryString( 0,
									ASSOCSTR_EXECUTABLE,
									_T( ".html" ),
									_T( "open" ),
									tchApplicationPath,
									&dwSize );
		char tmp[256];
		char parametros[2048];
		pDsSys->DespachadorInterno9(27,(void *)tmp);
		int i;
		for (i = 0;tmp[i] && tmp[i] != ' ';i++);
		tmp[i] = 0;

		sprintf(parametros,"http://www.geconet.net/at.php?dsini=%06d&dsusu=%s&dsdir=%s",theApp.m_numero_agisys,(LPCSTR)pDsSys->m_Usuario,tmp);

		if( FAILED( hr ))
		{
			::ShellExecute(NULL, "open",parametros,NULL,NULL, SW_SHOWNORMAL);		
		}
		else
		{
			::ShellExecute(NULL, "open",tchApplicationPath,parametros,NULL, SW_SHOWNORMAL);			
		}

		return TRUE;
	}
    
    return CFrameWnd::OnCommand(wParam, lParam);
}


void CDsxFrame::OnVertreemenu() 
{
   CMenu *main = GetMenu();	 
   CMenu *popup = NULL;   
   MENUITEMINFO item;
   ::memset((void *)&item,0,sizeof(item));
   if (main)
   {		
      popup = main->GetSubMenu( 2 );	  

	  item.cbSize = sizeof(item);
	  item.fMask = MIIM_STATE;
	  if (popup)
		::GetMenuItemInfo(popup->m_hMenu,ID_VERTREEMENU,FALSE,&item);
   }
      

	if (theApp.m_LeftView->IsWindowEnabled())
	{		
		theApp.m_LeftView->EnableWindow(FALSE);
		theApp.m_splitmenu->GetParent()->ShowWindow(SW_HIDE);
		theApp.m_splitmenu->GetParent()->EnableWindow(FALSE);
		theApp.m_splitmenu->GetParent()->SetParent(this);
		CRect rr;
		CDsxView *pView = theApp.StdView();
		pView->SetParent(this);	
		m_wndSplitter->GetWindowRect(&rr);
		ScreenToClient(&rr);
		m_wndSplitter->DestroyWindow();
		delete m_wndSplitter;
#ifdef DSPROFUIS
		m_wndSplitter = new CDSSplitterWnd();
#else
		m_wndSplitter = new CSplitterWnd();
#endif
		pView->ModifyStyle(0,WS_BORDER);
		pView->MoveWindow(rr);
		Invalidate();

		item.fMask = MIIM_STATE;
		item.fState &= ~MFS_CHECKED;
	}
	else
	{
		theApp.m_LeftView->EnableWindow(TRUE);
	    m_wndSplitter->CreateStatic(this, 1, 2);
		theApp.m_splitmenu->GetParent()->SetParent(m_wndSplitter);
		theApp.m_splitmenu->GetParent()->EnableWindow(TRUE);		
		CRect rr;	
		CDsxView *pView = theApp.StdView();
		pView->ModifyStyle(WS_BORDER,0);
		pView->SetParent(m_wndSplitter);
		m_wndSplitter->SetColumnInfo( 0,170,170);
		m_wndSplitter->SetRowInfo( 0,100,100);
		m_wndSplitter->SetColumnInfo( 1,100,100);
		m_wndSplitter->EnableWindow(TRUE);	
		theApp.m_splitmenu->GetParent()->ShowWindow(SW_SHOW);
		m_wndSplitter->RecalcLayout();
		m_wndSplitter->ShowWindow(SW_SHOW);
		RecalcLayout();
		Invalidate();

		item.fMask = MIIM_STATE;
		item.fState |= MFS_CHECKED;

	}

	if (popup)
		::SetMenuItemInfo(popup->m_hMenu,ID_VERTREEMENU,FALSE,&item);
}


void CDsxFrame::OnCentrar() 
{    
   CDsxDoc * pDoc = ((CDsxDoc *)GetActiveDocument());   
   CDsxView* pView = NULL; 
   CRect tmp1,tmp2,tmp3;
   int x,y;
   int wid;
   CDsStdDlg *pdlg;
   POSITION pos2;

   POSITION pos = pDoc->GetFirstViewPosition();
   while (pos != NULL)
   {           
       pView = (CDsxView *)pDoc->GetNextView(pos);
       if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) 
       {
           pView = NULL;
           continue;     
       }

       pView->GetWindowRect(&tmp1);
   
       pos2 = pView->m_dlglist.GetStartPosition( );
       while(pos2)
       {
           pView->m_dlglist.GetNextAssoc( pos2, wid, pdlg );
		   if (theApp.m_9wnd && pdlg->m_es9/*pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window))*/)
		   {			   
				 CDsTab9 *ptab9 = ((CDs9Window *)pdlg)->m_tab9;
				 if (ptab9)
					ptab9->CenterWindow(pView);
				 continue;
		   }		   
		   if (!pdlg->m_sid && !pdlg->m_padretab && pdlg->GetParent() == pView)
           {
			   if (pdlg->m_es9)			
				   continue;

			   pdlg->GetWindowRect(&tmp2);

			   if (pdlg->m_referencia)
			   {
				   pdlg->m_referencia->GetWindowRect(tmp3);
				   pView->ScreenToClient(tmp3);
				   x = tmp3.left + (((pdlg->m_lpi%100)-1)*pdlg->m_referencia->m_stdx)/10;
				   y = tmp3.top + (((pdlg->m_lpi/100)-5)*pdlg->m_referencia->m_stdy)/10;				
			   }
			   else
			   {               
				   x = (tmp1.Size().cx - tmp2.Size().cx) / 2;
				   y = (tmp1.Size().cy - tmp2.Size().cy) / 2;
			   }
               if (x < 0) x = 0;
               if (y < 0) y = 0;			   
               pdlg->SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);           
               pView->SetScrollDs();
           }
       }
   }   
}


void CDsxFrame::OnSingleexpand() 
{
   CMenu *main = GetMenu();	 
   int single = 0;
   if (main)
   {		
      CMenu *popup = main->GetSubMenu( 2 );

	  MENUITEMINFO item;
	  ::memset((void *)&item,0,sizeof(item));
	  item.cbSize = sizeof(item);
	  item.fMask = MIIM_STATE;
	  ::GetMenuItemInfo(popup->m_hMenu,ID_SINGLEEXPAND,FALSE,&item);

	  if ((item.fState&MFS_CHECKED) != 0)
	  {		 
		 item.fMask = MIIM_STATE;
		 item.fState &= ~MFS_CHECKED;
		 single = 0;
	  }
	  else
	  {
		 item.fMask = MIIM_STATE;
		 item.fState |= MFS_CHECKED;
		 single = 1;
	  }
	  ::SetMenuItemInfo(popup->m_hMenu,ID_SINGLEEXPAND,FALSE,&item);

	  /*
      if (popup)
      {
         if ((popup->GetMenuState(ID_SINGLEEXPAND,  MF_BYCOMMAND    ) & MF_CHECKED))
         {
            popup->CheckMenuItem( ID_SINGLEEXPAND,  MF_BYCOMMAND | MF_UNCHECKED);
			popup->ModifyMenu( ID_SINGLEEXPAND, MF_BYCOMMAND | MF_STRING, ID_SINGLEEXPAND,"Expandir solo una rama");
            single = 0;
         }
         else
         {
            popup->CheckMenuItem( ID_SINGLEEXPAND,  MF_BYCOMMAND | MF_CHECKED);
			popup->ModifyMenu( ID_SINGLEEXPAND, MF_BYCOMMAND | MF_STRING, ID_SINGLEEXPAND,"Expandir todas las ramas");
            single = 1;
         }
      }
	  */
   
      /*
      CMenu *popup;
	  CMenu *main = m_wndMenuBar.GetMenu(); //((CDsxFrame*)m_pMainWnd)->
      popup = main->GetSubMenu( 2 );	  
	  if (popup)
	  {
		   CMenu *menu;
		   menu = new CMenu();		
		   if (menu->CreatePopupMenu( )) 
		   {
			   int item;
			   UINT iditem;
			   char tmp[1024];
			   item = 0;
			   while(popup->GetMenuString( item, tmp, 1000, MF_BYPOSITION) > 0)
			   {
				   strcpy(tmp,"churro?");
				   iditem = popup->GetMenuItemID(item);
				   if (iditem == (UINT)0xffffffff)
				   {
					   menu->AppendMenu( MF_SEPARATOR,iditem,tmp);
				   }
				   else
				   {
					   menu->AppendMenu( MF_STRING,iditem,tmp);
				   }
				   item++;
			   }
				{
					MENUITEMINFO item;
					item.cbSize = sizeof(item);
					item.fMask = MIIM_SUBMENU;
					item.hSubMenu = menu->m_hMenu;
					::SetMenuItemInfo(main->m_hMenu,2,TRUE,&item);
				}

			   popup->DestroyMenu();

			   m_wndMenuBar.UpdateMenuBar();
			   
			   //main->GetMenuString( 2, tmp, 1000, MF_BYPOSITION);
			   //strcpy(tmp,"churro?");
			   //main->ModifyMenu( 2, MF_BYPOSITION|MF_POPUP|MF_STRING, (UINT)menu->m_hMenu,tmp);
			   
			   menu->Detach();			   
		   }
		   delete menu;
	  }
	  */
   }

   
   DrawMenuBar();
   
   if (!theApp.m_LeftView)
      return;
   
   CTreeCtrl *m_tree = &(theApp.m_LeftView->GetTreeCtrl());
   theApp.m_LeftView->PintaColapso(m_tree->GetRootItem());
   m_tree->SelectItem(NULL);
   m_tree->Expand(m_tree->GetRootItem(), TVE_COLLAPSE );	
   if (single)
      m_tree->ModifyStyle(0,TVS_SINGLEEXPAND);
   else
      m_tree->ModifyStyle(TVS_SINGLEEXPAND,0);
}

void CDsxFrame::OnMultitarea() 
{
   CMenu *main = GetMenu();	 	 
   if (main)
   {		
      CMenu *popup = main->GetSubMenu( 2 );
      if (popup)
      {
         if ((popup->GetMenuState(ID_MULTITAREA	,  MF_BYCOMMAND    ) & MF_CHECKED))
         {
            popup->CheckMenuItem( ID_MULTITAREA	,  MF_BYCOMMAND | MF_UNCHECKED);			  
         }
         else
         {
            popup->CheckMenuItem( ID_MULTITAREA	,  MF_BYCOMMAND | MF_CHECKED);			  
         }
      }
   }
}

void CDsxFrame::OnDestroy() 
{
    if (m_notifyicon)
    {
	     Shell_NotifyIcon(NIM_DELETE,&m_nid);    	
		 m_notifyicon = FALSE;
    }

    if (m_debugtimer)
    {
        KillTimer(m_debugtimer);
        m_debugtimer = 0;
    }
    if (m_signaltimer)
    {
        KillTimer(m_signaltimer);
        m_signaltimer = 0;
    }
	m_toolweb.m_navega.DestroyWindow( ); 
	m_dsmenubar.m_menus[2].DestroyWindow( ); 
	m_dsmenubar.m_menus[1].DestroyWindow( ); 
	m_dsmenubar.m_menus[0].DestroyWindow( ); 
	if (theApp.m_mensa)
	{
		theApp.m_mensa->DestroyWindow( );
		delete theApp.m_mensa;
		theApp.m_mensa = NULL;
	}
	
	CFrameWnd::OnDestroy();

    pMFrame = NULL;	
}

void CDsxFrame::OnMenuser() 
{
  if (theApp.m_splitmenu)
  {
    CMenu *main = GetMenu();
	if (main)
	{
	  int sideal ,smin;
      CMenu *popup = main->GetSubMenu( 2 );

      if (theApp.m_splitmenu->GetRowCount( ) > 1)
      {
         theApp.m_splitmenu->GetRowInfo( 0, sideal, smin  );
		 theApp.WriteProfileInt("Posiciones","RTREEMAX",sideal);
		 theApp.WriteProfileInt("Posiciones","RTREEMIN",smin);
		 if (theApp.m_UserMenu)
		 {
			 m_CMenuDropTarget.Revoke();
		 }
		 theApp.m_splitmenu->ModifyStyle(0,SPLS_DYNAMIC_SPLIT);
         theApp.m_splitmenu->DeleteRow( 1 );
		 theApp.m_splitmenu->ModifyStyle(SPLS_DYNAMIC_SPLIT,0);
         popup->CheckMenuItem( ID_MENUSER ,  MF_BYCOMMAND | MF_UNCHECKED);
      }
      else
      {
		  if (theApp.m_LeftView)
		  {			  
			  ::SetFocus(theApp.m_LeftView->m_hWnd);
		  }  		 
         int cyCur,cyMin;
		 CRect rect;
		 theApp.m_splitmenu->GetWindowRect(rect);         
		 theApp.m_splitmenu->GetRowInfo( 0, cyCur, cyMin );
		 theApp.m_splitmenu->ModifyStyle(0,SPLS_DYNAMIC_SPLIT);
		 sideal = theApp.GetProfileInt("Posiciones","RTREEMAX",rect.Size().cy/2);
		 if (cyCur < rect.Size().cy)
			 theApp.m_splitmenu->SplitRow(cyCur/2);
		 else
		 {
			 theApp.m_splitmenu->SplitRow(sideal);
		 }
         
         theApp.m_splitmenu->ModifyStyle(SPLS_DYNAMIC_SPLIT,0);
         smin = theApp.GetProfileInt("Posiciones","RTREEMIN",40);
		 theApp.m_splitmenu->SetRowInfo( 0, sideal, smin  );
         popup->CheckMenuItem( ID_MENUSER ,  MF_BYCOMMAND | MF_CHECKED);
		  if (theApp.m_UserMenu)
		  {			  
			  m_CMenuDropTarget.Register(theApp.m_UserMenu);
			  ::SetFocus(theApp.m_UserMenu->m_hWnd);
		  }  
      }
	}
  }         
}

void CDsxFrame::OnSiemprehtml() 
{
  CMenu *main = GetMenu();
  CMenu *popup = main->GetSubMenu( 2 );	
  if (popup)
  {
     if ((popup->GetMenuState(ID_SIEMPREHTML	,  MF_BYCOMMAND    ) & MF_CHECKED))
     {
        popup->CheckMenuItem( ID_SIEMPREHTML	,  MF_BYCOMMAND | MF_UNCHECKED);
        theApp.m_hidehtml = TRUE;
     }
     else
     {
        popup->CheckMenuItem( ID_SIEMPREHTML	,  MF_BYCOMMAND | MF_CHECKED);
        theApp.m_hidehtml = FALSE;
     }
     Invalidate();
  }
}

void CDsxFrame::OnFondohtml() 
{
  CMenu *main = GetMenu();
  CMenu *popup = main->GetSubMenu( 2 );	
  
  if (popup)
  {
     BOOL chequear;
     if ((popup->GetMenuState(ID_FONDOHTML	,  MF_BYCOMMAND    ) & MF_CHECKED))
     {
         chequear = FALSE;
     }
     else
     {
         if (!theApp.ThereIsIE4())
         {
             chequear = FALSE;
             AfxMessageBox("Debe Instalar Internet Explorer 4 o superior!");
         }
         else
             chequear = TRUE;
     }
     if (chequear)
     {
        popup->CheckMenuItem( ID_FONDOHTML	,  MF_BYCOMMAND | MF_CHECKED);
        theApp.m_usarweb = TRUE;
     }
     else
     {
        popup->CheckMenuItem( ID_FONDOHTML	,  MF_BYCOMMAND | MF_UNCHECKED);
        theApp.m_usarweb = FALSE;
     }

     CDsxDoc * pDoc = ((CDsxDoc *)GetActiveDocument());
     CDsxView* pView;
     POSITION pos = pDoc->GetFirstViewPosition();
     while (pos != NULL)
     {           
         pView = (CDsxView *)pDoc->GetNextView(pos);
         if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) 
         {
             pView = NULL;
             continue;     
         }
         if (chequear)
         {
             if (pView->m_bmp)
             {
                delete pView->m_bmp;
                pView->m_bmp = NULL;
             }
         }
         else
         {
             if (pView->m_html)
             {
                 pView->m_html->DestroyWindow();
                 //delete pView->m_html; (no se debe ya que el destroy window se encarga de todo)
                 pView->m_html = NULL;
             }
         }
     }     
     Invalidate();
  }	
}

void CDsxFrame::OnTimer(UINT nIDEvent) 
{	
	if (globo_saliendo && theApp.m_ulttecla != 807)
	{
		theApp.m_ulttecla = 807;
	}

	if (m_notifyicon && !m_nid.uTimeout)
	{
		long tt;
		if (cuando_balon)
		{
			tt = time(NULL);
			if (cuando_balon < tt)
			{
				cuando_balon = 0;
				if (flasheando)
				{
					//::BringWindowToTop(m_hWnd);
					//::SetForegroundWindow(m_hWnd);
				}
				Shell_NotifyIcon(NIM_MODIFY, &m_nid);
			}
		}
	}
	if (nIDEvent == IDD_PROGRESO)
	{
		PresentaProgreso(999,NULL);
		return;
	}
	if (theApp.m_Abierto)
	{
		::PostQuitMessage(0);
	}
	else
		OnCommand((WPARAM)nIDEvent,(LPARAM)NULL);
	//CFrameWnd::OnTimer(nIDEvent);
}

void CDsxFrame::OnUpdateTestOtro(CCmdUI* pCmdUI) 
{
    if (!GetActiveDocument())    
        pCmdUI->Enable( FALSE );
    else
        pCmdUI->Enable( TRUE  );
}

void CDsxFrame::OnNextPane() 
{
   theApp.TeclaAlActivo(ID_SIGUIENTEPANE);
}

void CDsxFrame::OnPrevPane() 
{
  theApp.TeclaAlActivo(ID_ANTERIORPANE);
}

void CDsxFrame::OnAltDown() 
{
   theApp.TeclaAlActivo(992);
}

void CDsxFrame::OnAltUp() 
{
  theApp.TeclaAlActivo(994);
}

void CDsxFrame::OnRecordLast() 
{
  theApp.TeclaAlActivo(ID_RECORD_LAST);
}

void CDsxFrame::OnUpdateRecordLast(CCmdUI* pCmdUI) 
{		
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnRecordFirst() 
{
  theApp.TeclaAlActivo(ID_RECORD_FIRST);
}

void CDsxFrame::OnUpdateRecordFirst(CCmdUI* pCmdUI) 
{  
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnRecordDelete() 
{
  theApp.TeclaAlActivo(ID_RECORD_DELETE);
}

void CDsxFrame::OnUpdateRecordDelete(CCmdUI* pCmdUI) 
{
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      CTablaW *pw = (CTablaW *)theApp.m_ctrlactivo->m_ctrl;
      if (pw && (pw->m_gmodo & 1))      
      {
          pCmdUI->Enable(TRUE);
      }
      else
      {
          pCmdUI->Enable(FALSE);
      }
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnRecordNew() 
{
  theApp.TeclaAlActivo(ID_RECORD_NEW);
}

void CDsxFrame::OnUpdateRecordNew(CCmdUI* pCmdUI) 
{
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      CTablaW *pw = (CTablaW *)theApp.m_ctrlactivo->m_ctrl;
      if (pw && (pw->m_gmodo & 1))      
      {
          pCmdUI->Enable(TRUE);
      }
      else
      {
          pCmdUI->Enable(FALSE);
      }
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnRecordNext() 
{
  theApp.TeclaAlActivo(ID_RECORD_NEXT);	
}

void CDsxFrame::OnUpdateRecordNext(CCmdUI* pCmdUI) 
{
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnRecordPrev() 
{
  theApp.TeclaAlActivo(ID_RECORD_PREV);	
}

void CDsxFrame::OnUpdateRecordPrev(CCmdUI* pCmdUI) 
{
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnRecordUpdate() 
{
  theApp.TeclaAlActivo(ID_RECORD_UPDATE);	
}

void CDsxFrame::OnUpdateRecordUpdate(CCmdUI* pCmdUI) 
{	
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      CTablaW *pw = (CTablaW *)theApp.m_ctrlactivo->m_ctrl;
      if (pw && (pw->m_gmodo & 1))      
      {
          pCmdUI->Enable(TRUE);
      }
      else
      {
          pCmdUI->Enable(FALSE);
      }
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);	

	
	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
    {
        if (pMWnd)
        {
            CWnd *pw = (CWnd *)pMWnd;
            CRect r1,r2;
            GetWindowRect(r1);
            pw->GetWindowRect(r2);
            if (r1.top >= r2.top && r1.left >= r2.left && r1.bottom <= r2.bottom && r1.right <= r2.right)
            {
                pw->SetFocus();
            }
        }
    }
	//theApp.ActivacionPorDefecto(NULL);
}

void CDsxFrame::OnVercaption() 
{

   CMenu *main = GetMenu();	 
   BOOL caption = FALSE;
   if (main)
   {		
      CMenu *popup = main->GetSubMenu( 2 );
      if (popup)
      {
         if ((popup->GetMenuState(ID_VERCAPTION,  MF_BYCOMMAND    ) & MF_CHECKED))
         {
            popup->CheckMenuItem( ID_VERCAPTION,  MF_BYCOMMAND | MF_UNCHECKED);
            caption = FALSE;
         }
         else
         {
            popup->CheckMenuItem( ID_VERCAPTION,  MF_BYCOMMAND | MF_CHECKED);
            caption = TRUE;
         }
      }
   }

     theApp.m_concaption = caption;

     CDsxDoc * pDoc = ((CDsxDoc *)GetActiveDocument());
     CDsxView* pView;
     CDsStdDlg *pdlg;
     CRect rect;
     int stdid;
     POSITION pos = pDoc->GetFirstViewPosition(),pos1;
     while (pos != NULL)
     {           
         pView = (CDsxView *)pDoc->GetNextView(pos);
         if (!pView || !pView->IsKindOf(RUNTIME_CLASS(CDsxView))) 
         {
             pView = NULL;
             continue;     
         }
         pos1 = pView->m_dlglist.GetStartPosition( );
         while( pos1 != NULL)
         {
             pView->m_dlglist.GetNextAssoc( pos1, stdid, pdlg );
             if (pdlg && !(pdlg->m_sid))
             {				 
				 if (pdlg->m_noframe)
				 {
					 if (pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window)))
					 {
						 /*						 
						 if (ptab9->GetParent())
						 {
							 ptab9->GetParent()->ScreenToClient(rect);
						 }						 
						 if (caption)
						 {
							 rect.bottom -= ((::GetSystemMetrics(SM_CYDLGFRAME)*2)+1);
							 rect.right  -= ((::GetSystemMetrics(SM_CXDLGFRAME)*2)+1);                     
							 rect.bottom += ::GetSystemMetrics(SM_CYCAPTION);           
							 rect.bottom += (::GetSystemMetrics(SM_CYSIZEFRAME)*2)+1;
							 rect.right  += (::GetSystemMetrics(SM_CXSIZEFRAME)*2)+1;
							 ptab9->MoveWindow(rect,FALSE);
							 ptab9->ModifyStyle(0,WS_THICKFRAME|WS_CAPTION,SWP_DRAWFRAME);
						 }
						 else
						 {
							 rect.bottom -= ::GetSystemMetrics(SM_CYCAPTION);           
							 rect.bottom -= ((::GetSystemMetrics(SM_CYSIZEFRAME)*2)+1);
							 rect.right  -= ((::GetSystemMetrics(SM_CXSIZEFRAME)*2)+1);
							 rect.bottom += (::GetSystemMetrics(SM_CYDLGFRAME)*2)+1;
							 rect.right  += (::GetSystemMetrics(SM_CXDLGFRAME)*2)+1;                     
							 ptab9->MoveWindow(rect,FALSE);
							 ptab9->ModifyStyle(WS_THICKFRAME|WS_CAPTION,0,SWP_DRAWFRAME);
						 }
						 */
					 }					 
					 continue;
				 }                 
				 pdlg->GetWindowRect(rect);
                 if (pdlg->GetParent())
                 {
                     pdlg->GetParent()->ScreenToClient(rect);
                 }
                 if (caption)
                 {
                     rect.bottom -= ((::GetSystemMetrics(SM_CYDLGFRAME)*2)+1);
                     rect.right  -= ((::GetSystemMetrics(SM_CXDLGFRAME)*2)+1);                     
                     rect.bottom += ::GetSystemMetrics(SM_CYCAPTION);           
                     rect.bottom += (::GetSystemMetrics(SM_CYSIZEFRAME)*2)+1;
                     rect.right  += (::GetSystemMetrics(SM_CXSIZEFRAME)*2)+1;
                     pdlg->MoveWindow(rect,FALSE);
                     pdlg->ModifyStyle(theApp.m_dlgframe,WS_THICKFRAME|WS_CAPTION,SWP_DRAWFRAME);
                 }
                 else
                 {
                     rect.bottom -= ::GetSystemMetrics(SM_CYCAPTION);           
                     rect.bottom -= ((::GetSystemMetrics(SM_CYSIZEFRAME)*2)+1);
                     rect.right  -= ((::GetSystemMetrics(SM_CXSIZEFRAME)*2)+1);
                     rect.bottom += (::GetSystemMetrics(SM_CYDLGFRAME)*2)+1;
                     rect.right  += (::GetSystemMetrics(SM_CXDLGFRAME)*2)+1;                     
                     pdlg->MoveWindow(rect,FALSE);
                     pdlg->ModifyStyle(WS_THICKFRAME|WS_CAPTION,theApp.m_dlgframe,SWP_DRAWFRAME);
                 }                
             }
         }
         pView->Invalidate();
     }
}


void CDsxFrame::OnExportaExcel() 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->OnExportaExcel();
  } 
}

void CDsxFrame::OnUpdateExportaExcel(CCmdUI* pCmdUI) 
{  
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnExportaWord() 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->OnExportaWord();
  } 
}

void CDsxFrame::OnUpdateExportaWord(CCmdUI* pCmdUI) 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

void CDsxFrame::OnConsultaFiltro() 
{
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->m_tabla.Filtros(FALSE,TRUE);
  }
}

void CDsxFrame::OnUpdateConsultaFiltro(CCmdUI* pCmdUI) 
{
  if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}

BOOL CDsxFrame::PreTranslateMessage(MSG* pMsg) 
{	
#ifdef DSPROFUIS	
	if (!m_sinbarras)
	{
		if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
			return TRUE;
	}
#endif

	if (m_hayquecentrar)
	{
		if (!GetAsyncKeyState(VK_LBUTTON))
		{
			m_hayquecentrar = FALSE;
			OnCentrar();
		}
	}

	/*
    if (m_wndMenuBar.TranslateFrameMessage(pMsg))
        return TRUE;

	if (theApp.m_9wnd && pMsg->hwnd == theApp.m_9wnd->m_hWnd)
	{
		return FALSE;
	}
    if (pMsg->message == WM_KEYUP)
    {
		int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
        if (comando == 805 || comando == 804)
        {            
            return TRUE;        
        }
	}

    if (pMsg->message == WM_KEYDOWN)
    {
        int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
        if (comando == 805 || comando == 804)
        {
            theApp.TeclaAlActivo(comando);
            return TRUE;        
        }
        return CFrameWnd::PreTranslateMessage(pMsg);
    }
    else
	*/
#ifdef DSXTLIB
	    return CXTPFrameWnd::PreTranslateMessage(pMsg);
#else
	    return CFrameWnd::PreTranslateMessage(pMsg);
#endif		
		
}

CMenu* CDsxFrame::GetMenu()
{  
  if (!theApp.m_MenuBar)
  {
	  return CFrameWnd::GetMenu();
  }
  else
  {
	  return theApp.m_MenuBar->GetMenu();
  }
}

void CDsxFrame::DrawMenuBar()
{
  if (!theApp.m_MenuBar)
	  CFrameWnd::DrawMenuBar();
  else
  {
#ifdef DSPROFUIS
	  theApp.m_MenuBar->UpdateMenuBar();
#else
	  theApp.m_MenuBar->InitItems();
	  theApp.m_MenuBar->RefreshBar();
#endif
  }
}

void CDsxFrame::OnBuscamenu() 
{
    if (theApp.m_splitmenu)
	{
	   CMenu *main = GetMenu();
	   if (main)
	   {
		  CMenu *popup = main->GetSubMenu( 2 );
		  if (popup)
		  {
			 if (popup->GetMenuState(ID_BUSCAMENU,  MF_BYCOMMAND    ) & MF_CHECKED)
			 {
				popup->CheckMenuItem(ID_BUSCAMENU,  MF_BYCOMMAND | MF_UNCHECKED);
				if (theApp.m_splitmenu->m_buscadlg)
				{
					if (::IsWindow(theApp.m_splitmenu->m_buscadlg->m_hWnd))
					{
						theApp.m_splitmenu->m_buscadlg->DestroyWindow();
					}
					delete theApp.m_splitmenu->m_buscadlg;
					theApp.m_splitmenu->m_buscadlg = NULL;
				}
				CRect rect;
				theApp.m_splitmenu->GetParent()->GetClientRect(rect);
				if (theApp.m_splitmenu->m_buscadlg)
				{
					CRect rr;
					theApp.m_splitmenu->m_buscadlg->GetWindowRect(rr);
					rect.top += rr.Size().cy;
				}	
				theApp.m_splitmenu->MoveWindow(rect);
				if (theApp.m_LeftView)
				{
					theApp.m_LeftView->BorraBusqueda();
				}
			 }
			 else
			 {
				theApp.m_splitmenu->m_buscadlg = new CDialogBuscaMenu();
				theApp.m_splitmenu->m_buscadlg->Create(IDD_BUSMENU,theApp.m_splitmenu->GetParent());
				popup->CheckMenuItem(ID_BUSCAMENU,  MF_BYCOMMAND | MF_CHECKED);
				CRect rect;
				theApp.m_splitmenu->GetParent()->GetClientRect(rect);
				if (theApp.m_splitmenu->m_buscadlg)
				{
					CRect rr;
					theApp.m_splitmenu->m_buscadlg->GetWindowRect(rr);
					rect.top += rr.Size().cy;
				}	
				theApp.m_splitmenu->MoveWindow(rect);
			 }
		  }
	   }
	}
}


void CDsxFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
   CFrameWnd::OnShowWindow(bShow, nStatus);	
}

void CDsxFrame::OnTooltips() 
{
   theApp.m_tooltips = !theApp.m_tooltips;
   CMenu *main = GetMenu();
   if (main)
   {
      CMenu *popup = main->GetSubMenu( 2 );
      if (popup)
      {
         if (!theApp.m_tooltips)
            popup->CheckMenuItem( ID_TOOLTIPS,  MF_BYCOMMAND | MF_UNCHECKED);
         else
            popup->CheckMenuItem( ID_TOOLTIPS,  MF_BYCOMMAND | MF_CHECKED);
      }
   }
}

extern void WINAPI DespachaInterface(void *parametro);
void CDsxFrame::OnActivarCristal() 
{
	/* built-in */
    typedef void (WINAPI *_EntryPoint2)(DWORD handle,
					  	          DWORD funcion,
								  DWORD extra
						          ); 

	void WINAPI DscristalEntryPoint2(DWORD handle,DWORD funcion,DWORD extra);

	_EntryPoint2 EntryPoint2;
	EntryPoint2 = (_EntryPoint2)DscristalEntryPoint2;
	/*
	if (theApp.m_9wnd)
  		pDsSys->DespachadorInterno9(801,NULL);
	else
		pDsSys->DespachadorInterno(801,NULL);    
	*/
	EntryPoint2((DWORD)0,(DWORD)DespachaInterface,(DWORD)NULL);
	/*
	if (theApp.m_9wnd)
  		pDsSys->DespachadorInterno9(802,NULL);
	else
		pDsSys->DespachadorInterno(802,NULL);    
	*/


	/*
	HINSTANCE modulo = LoadLibrary("dscristal.dll");
	if (modulo)
	{
        typedef void (WINAPI *_EntryPoint)(DWORD handle,
					  	          DWORD funcion
						          ); 
		_EntryPoint EntryPoint = (_EntryPoint)GetProcAddress(modulo,"DsEntryPoint");
		if (EntryPoint)
		{			
			EntryPoint((DWORD)0,(DWORD)DespachaInterface);			
		}
		else
		{
			AfxMessageBox("Modulo dscristal.dll incorrecto");
		}
		FreeLibrary(modulo);
	}
	else
	{
		AfxMessageBox("Falta el modulo dscristal.dll");
	}
	*/
}

void CDsxFrame::OnListatareas() 
{
  TASK_LIST *ptask = NULL;
  int ntasks = 0;
  if (theApp.m_9wnd)
  {
	if (pDsSys)
	{
		ntasks = pDsSys->DespachadorInterno9(500,(void *)&ptask);
	}
  }
  else
  {
	 if (pDsSys)
	 { 
		 ntasks = pDsSys->DespachadorInterno(500,(void *)&ptask);
	 }
  }

  BOOL ocultar = FALSE;
   CMenu *main = pMFrame->GetMenu();
   if (main)
   {		  
	  CMenu *popup = main->GetSubMenu( 2 );
	  if (popup)
	  {
		 if ((popup->GetMenuState(ID_MULTITAREA,  MF_BYCOMMAND    ) & MF_CHECKED))
		 {
			ocultar = TRUE;
		 }
	  }
   }

  DWORD pid = (DWORD)getpid();
  if (ntasks && ptask)
  {
    CMenu popmenu;
    popmenu.CreatePopupMenu();
    CString texto;
	HWND res = NULL;

	  CString s;
	  int i;
	  for (i = 0;i < ntasks;i++)
	  {
		  s = ptask[i].ProcessName;
		  s.MakeLower();
		  if (s.Find("diagramx") > -1 && ptask[i].hwnd && ptask[i].hwnd != m_hWnd && pid != ptask[i].dwProcessId && ::IsWindowVisible((HWND)ptask[i].hwnd))
		  {
			   texto = ptask[i].WindowTitle;
			   popmenu.InsertMenu(i,MF_STRING | MF_BYPOSITION,(int)ptask[i].hwnd,texto);
		  }
	  }	  
	  POINT pp;
	  ::GetCursorPos(&pp);
	  pp.y += 20;
	  res = (HWND)popmenu.TrackPopupMenu(TPM_CENTERALIGN| TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON,pp.x,pp.y-10,this,NULL);
	  if (::IsWindow(res) && res != m_hWnd)
	  {
		 if (theApp.m_minimizado)
		 {
			::ShowWindow(res,SW_HIDE);
			if (!::IsWindowVisible(res))
			{		 
				::ShowWindow(res,SW_RESTORE);
				::ShowWindow(res,SW_SHOW);
				::BringWindowToTop(res);
				::SetForegroundWindow(res);
				if (ocultar)
				{
					::ShowWindow(m_hWnd,SW_HIDE);
					::ShowWindow(m_hWnd,SW_MINIMIZE);
					::ShowWindow(m_hWnd,SW_SHOW);
				}
			}
			else
			{
				::ShowWindow(res,SW_SHOW);
				AfxMessageBox("No se puede presentar la tarea",MB_OK|MB_ICONEXCLAMATION);
			}
		 }
		 else
		 {
				::BringWindowToTop(res);
				::SetForegroundWindow(res);
		 }
	  }
  }	
}



/* CToolDropTarget */

DROPEFFECT CToolDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
	DWORD dwKeyState, CPoint point 
)
{	
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;

    HGLOBAL handle = pDataObject->GetGlobalData(theApp.m_cMenuFormat);
    if (!handle)
        return DROPEFFECT_NONE;
    
	/*
	if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropeffectRet = DROPEFFECT_MOVE;     
	*/
	return dropeffectRet;
}

/********************************************************************
OnDragOver()
	Called when the user drags the object over Tree control.
********************************************************************/
		
DROPEFFECT CToolDropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
	DWORD dwKeyState, CPoint point )
{	
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;

	/*
	if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropeffectRet = DROPEFFECT_MOVE;     	
	*/
    HGLOBAL handle = pDataObject->GetGlobalData(theApp.m_cMenuFormat);
    if (!handle)
        return DROPEFFECT_NONE;

	return dropeffectRet;
	
	
}

/********************************************************************
OnDragLeave()
	Called when the user drags the object out of Tree control.
********************************************************************/
void CToolDropTarget::OnDragLeave( CWnd* pWnd )
{	
}

/********************************************************************
OnDrop()
	Called when the user drops the object in the  Tree control.
********************************************************************/
BOOL CToolDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point)
{
    char data[1024];

	
    HGLOBAL handle = pDataObject->GetGlobalData(theApp.m_cMenuFormat);
    if (!handle)
        return FALSE;
    char *pdata = (char *)::GlobalLock(handle);
    memcpy(data,pdata,1024);
    ::GlobalUnlock(handle);

    char *p = data;
	if (*p++ != 1)
	{
		return FALSE;
	}
	if (*p++ != 0)
	{
		return FALSE;
	}
	if (*p++ != 0)
	{
		return FALSE;
	}
	if (*p++ != 1)
	{
		return FALSE;
	}

	{
        CMenu popmenu;
        popmenu.CreatePopupMenu();
        int n = 0;
		CString ss,s;
		int i;
        for (i = 0;i < 10;i++)
        {
		  if (!i)
		  {
			  s.Format("Ctrl-F12 = ");
		  }
		  else
		  {
			  s.Format("Ctrl-F%d = ",i+1);
		  }

          if (!theApp.m_pFunciones[i].IsEmpty())
          {		
             ss = theApp.m_dFunciones[i];
             ss.TrimLeft();
             ss.TrimRight();
          }
		  else
			 ss = "Vacio";
		  s += ss;
          popmenu.InsertMenu(i,MF_STRING | MF_BYPOSITION,833+i,s);
          n++;
        }       
       
        POINT pp;
        ::GetCursorPos(&pp);
        int tecla = (int)popmenu.TrackPopupMenu(TPM_CENTERALIGN| TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y-10,pWnd,NULL);


		if (tecla >= 833 && tecla < 843)
		{
			char *exe = p;
			p += (strlen(p)+1);
			char *par = p;
			p += (strlen(p)+1);
			char *titulo = p;
			p += (strlen(p)+1);
			char *opcion = p;
			

			CString funcion,s;

			funcion.Format("%03d",tecla);

			s = "|";
			s += opcion;
			s += (char)10;
			s += titulo;
			theApp.WriteProfileString("Funciones",funcion,s);

			i = tecla - 833;
			theApp.m_pFunciones[i] = "|";
			theApp.m_pFunciones[i] += opcion;
			theApp.m_dFunciones[i] = titulo;
		}
	}
	pActiveDoc->RefrescaIra();

	return TRUE;	
}


/* CMenuDropTarget */

DROPEFFECT CMenuDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
	DWORD dwKeyState, CPoint point 
)
{	
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;

    HGLOBAL handle = pDataObject->GetGlobalData(theApp.m_cMenuFormat);
    if (!handle)
        return DROPEFFECT_NONE;
    
	/*
	if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropeffectRet = DROPEFFECT_MOVE;     
	*/
	return dropeffectRet;
}

/********************************************************************
OnDragOver()
	Called when the user drags the object over Tree control.
********************************************************************/
		
DROPEFFECT CMenuDropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
	DWORD dwKeyState, CPoint point )
{	
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;

	/*
	if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropeffectRet = DROPEFFECT_MOVE;     	
	*/
    HGLOBAL handle = pDataObject->GetGlobalData(theApp.m_cMenuFormat);
    if (!handle)
        return DROPEFFECT_NONE;

	return dropeffectRet;
	
	
}

/********************************************************************
OnDragLeave()
	Called when the user drags the object out of Tree control.
********************************************************************/
void CMenuDropTarget::OnDragLeave( CWnd* pWnd )
{	
}

/********************************************************************
OnDrop()
	Called when the user drops the object in the  Tree control.
********************************************************************/
BOOL CMenuDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point)
{
    char data[1024];

	
    HGLOBAL handle = pDataObject->GetGlobalData(theApp.m_cMenuFormat);
    if (!handle)
        return FALSE;
    char *pdata = (char *)::GlobalLock(handle);
    memcpy(data,pdata,1024);
    ::GlobalUnlock(handle);

    char *p = data;
	if (*p++ != 1)
	{
		return FALSE;
	}
	if (*p++ != 0)
	{
		return FALSE;
	}
	if (*p++ != 0)
	{
		return FALSE;
	}
	if (*p++ != 1)
	{
		return FALSE;
	}

	if (!theApp.m_UserMenu)
	{
		return FALSE;
	}

	if (theApp.m_UserMenu)
	{		
		char tmpx[1024];
		CTreeCtrl *pNew = &(theApp.m_UserMenu->GetTreeCtrl());
		UINT uFlags;
		HTREEITEM primero = NULL,ultimo = TVI_LAST;
		
		primero = pNew->HitTest(point, &uFlags);
		if (!primero)
			primero = pNew->GetRootItem();

		
		if (!pNew->GetParentItem(primero))
		{		
			tmpx[0] = 0;
			while(primero)
			{
			   TVITEM item;		   
			   item.hItem = primero;
			   item.mask = TVIF_TEXT | TVIF_HANDLE;
			   item.pszText = tmpx;
			   item.cchTextMax = sizeof(tmpx)-1;
			   pNew->GetItem(&item);
			   if (strcmp(tmpx,"Busqueda"))
			   {
				   break;
			   }
			   primero = pNew->GetNextItem(primero, TVGN_NEXT);
			}		
		}

		{
			int image = -1,selimage = -1;			
			pNew->GetItemImage( primero, image, selimage);
			if (image > 1)
			{
				ultimo = primero;
				primero = pNew->GetParentItem(primero);
				if (!primero)
					primero = ultimo;
			}
		}		
		
		if (primero)
		{			
			char *exe = p;
			p += (strlen(p)+1);
			char *par = p;
			p += (strlen(p)+1);
			char *titulo = p;
			p += (strlen(p)+1);
			char *opcion = p;

			/* no se permite duplicacion en el mismo submenu ...
			pero si en otros!!! */
			
			/*
		    if (pNew->ItemHasChildren(primero))
			{
			   HTREEITEM hNextItem;
			   HTREEITEM hChildItem = pNew->GetChildItem(primero);
			   int otro;

			   while (hChildItem != NULL)
			   {
				  otro = pNew->GetItemData( hChildItem  );
				  if (otro < 0)
				  {
					  otro = -(otro+1);
					  if (otro < theApp.m_nlistadirecta &&
						  theApp.m_listadirecta[otro] &&
						  !strcmp(theApp.m_listadirecta[otro]+1,opcion)
						  )
					  {
						  return TRUE;
					  }
				  }
				  hNextItem = pNew->GetNextItem(hChildItem, TVGN_NEXT);
				  hChildItem = hNextItem;
			   }
			}
			*/

			int i;
			for (i = 0;i < theApp.m_nlistadirecta;i++)
			{
				if (!theApp.m_listadirecta[i])
					break;
			}
			if (i >= theApp.m_nlistadirecta)
			{
				i = theApp.m_nlistadirecta;
				if (!theApp.m_listadirecta)
				{
					theApp.m_nlistadirecta = 1;
					theApp.m_listadirecta = (char **)malloc(sizeof(char *)*theApp.m_nlistadirecta);
				}
				else
				{
					theApp.m_nlistadirecta ++;
					theApp.m_listadirecta = (char **)realloc((void *)theApp.m_listadirecta,sizeof(char *)*theApp.m_nlistadirecta);
				}
			}

			theApp.m_listadirecta[i] = (char *)malloc(strlen(opcion)+2);
			theApp.m_listadirecta[i][0] = '|';
			strcpy(theApp.m_listadirecta[i]+1,opcion);

			TV_INSERTSTRUCT Nuevo;		

			int anterior = -1;
			if (ultimo != TVI_LAST)
			{
				anterior = (int)pNew->GetItemData(ultimo);
				if (anterior < 0)
				{
					anterior = -(anterior+1);
				}
				else
					anterior = -1;
			}		


			Nuevo.hParent = primero;
			Nuevo.hInsertAfter = ultimo;
			Nuevo.item.pszText = titulo;
			Nuevo.item.cchTextMax = strlen(titulo);
			Nuevo.item.iImage = 2;
			Nuevo.item.iSelectedImage = 2;
			
			Nuevo.item.lParam = -(i+1);
			Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			HTREEITEM nueva = pNew->InsertItem( &Nuevo );
			
            pNew->ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT);
            pNew->Expand(primero, TVE_EXPAND    );
            pNew->SelectItem(nueva);
			
			CString funcion,s;
			funcion.Format("M%04d",i);

			s = "|";
			s += opcion;
			s += (char)10;
			s += titulo;			
			{
				int j;				
				j = pNew->GetItemData(primero);
				if (j < 0)
				{
					char tmp[20];
					s += (char)10;
					sprintf(tmp,"%d",-(j+1));
					s += tmp;
				}				
			}

			theApp.WriteProfileString("MenuPropio",funcion,s);
			s = theApp.GetProfileString("MenuPropio","Indice","");
			s.Replace(funcion,"");
			if (anterior != -1)
			{
				CString ss;
				ss.Format("M%04d",anterior);
				int p1 = s.Find(ss);
				if (p1 >= 0)
				{
					p1 += 5;
					ss = s.Mid(0,p1) + funcion + s.Mid(p1);
					s = ss;
				}
				else
					s+=funcion;
			}
			else
			{
				s+=funcion;				
			}
			theApp.WriteProfileString("MenuPropio","Indice",s);			
		}
	}

	return TRUE;	
}


void CDsxFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDsxFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
		
	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDsxFrame::OnImpredatos() 
{
        CDlgOldImpre dlg;

        dlg.m_impre = "";
        dlg.m_inispool = "";
	    dlg.m_SFont = theApp.m_FontI.lfFaceName;
	    dlg.m_MargenI = theApp.m_MargenI;
	    dlg.m_MargenI2= theApp.m_MargenI2;
	    dlg.m_MargenS = theApp.m_MargenS;  
	    dlg.m_Ajusta  = theApp.m_AjustaImpre;
	    dlg.m_Lineas  = theApp.m_LineasI;
	    dlg.m_Cols  = theApp.m_CarsI;
	    dlg.m_cabeceraexcel = theApp.m_CabeceraExcel;
	    dlg.m_posexcel = theApp.m_PosExcel;
	    if (dlg.DoModal() == IDOK)
		{
		  strcpy(theApp.m_FontI.lfFaceName,dlg.m_SFont);
		  theApp.m_MargenI = dlg.m_MargenI;
		  theApp.m_MargenI2= dlg.m_MargenI2;
		  theApp.m_MargenS = dlg.m_MargenS;
		  theApp.m_AjustaImpre =dlg.m_Ajusta;
		  theApp.m_LineasI = dlg.m_Lineas;
		  theApp.m_CarsI = dlg.m_Cols;
		  theApp.m_CabeceraExcel = dlg.m_cabeceraexcel;
		  theApp.m_PosExcel = dlg.m_posexcel;
		  theApp.m_pedidaimpresora = dlg.m_crselimprecristal;
		}
}

void CDsxFrame::OnSysCommand(UINT nID,LPARAM lParam)
{

	if (nID  == SC_KEYMENU && m_sinbarras < 3)
	{
		return;
	}
	if (nID  == ID_APP_ABOUT)
	{
		theApp.OnAppAbout();
		return;
	}
	CFrameWnd::OnSysCommand(nID, lParam);
}

void CDsxFrame::OnUpdateControlremto(CCmdUI* pCmdUI) 
{
	if ((theApp.m_haycontrolrmto || theApp.m_superusuario) && theApp.m_numero_agisys > 0)
		pCmdUI->Enable( TRUE );		
	else
		pCmdUI->Enable( FALSE );		

}

void CDsxFrame::OnControlremto() 
{
	if (theApp.m_haycontrolrmto || theApp.m_superusuario)
	{
		CString file;
		CString s;
		HWND hwnd;		
		hwnd = ::FindWindow(NULL,"Conexión directa a DSWebcall, Telesoporte Diagram Software S.L.");
		if (hwnd)
		{			
			if (AfxMessageBox("¿Desea abrir otra sesión de telesoporte? (no recomendable)",MB_YESNO|MB_ICONHAND) != IDYES)
			{
				::BringWindowToTop(hwnd);
				return;
			}
		}

		file.Format("%sbin/dstlsprt.exe",theApp.m_dirbase);
		unsigned char tmp[13];
		sprintf((char *)tmp,"123456%06ld",theApp.m_numero_agisys);
		int i;
		for (i = 0;i < 6;i+=2)
		{
			tmp[i] = tmp[11-i]+17+i;
		}
		for (i = 1;i < 6;i+=2)
		{
			tmp[i] = tmp[11-i]+33-i;
		}
		//theApp.m_dirbase
		char tmp1[256];
		pDsSys->DespachadorInterno9(27,(void *)tmp1);
		CString sd;
		sd = tmp1;
		sd.Replace('/','#');
		sd.Replace('\\','#');
		s.Format("%sbin/dstlsprt.exe %s %s",theApp.m_dirbase,(LPCSTR)sd,(char *)tmp);
		s.Replace('/','\\');
		::ShellExecute(NULL, "open",file,s,NULL, SW_SHOWNORMAL);
	}
}
 
void CDsxFrame::OnUpdateActivarErp(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable( TRUE );	
}

void CDsxFrame::OnActivarErp() 
{		
	pActiveDoc->TeclaStd(ID_ACTIVAR_ERP);
}

void CDsxFrame::OnExportaPlb() 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->OnExportaPlb();
  } 
}

void CDsxFrame::OnUpdateExportaPlb(CCmdUI* pCmdUI) 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }	
}


void CDsxFrame::OnExportaRpt() 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->OnExportaRpt();
  } 	
}

void CDsxFrame::OnUpdateExportaRpt(CCmdUI* pCmdUI) 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }		
}

void CDsxFrame::OnActivarEvt() 
{
	pActiveDoc->TeclaStd(ID_ACTIVAR_EVT);	
}

void CDsxFrame::OnActivarEvtpbx() 
{
	pActiveDoc->TeclaStd(ID_ACTIVAR_EVTPBX);	
}

void CDsxFrame::OnActivarEvtcrm() 
{
	pActiveDoc->TeclaStd(ID_ACTIVAR_EVTCRM);	
}


void CDsxFrame::OnExportaXml() 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->OnExportaXml();
  } 	
}

void CDsxFrame::OnUpdateExportaXml(CCmdUI* pCmdUI) 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
}


void CDsxFrame::OnGridimprime() 
{
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      ((CTablaW *)theApp.m_ctrlactivo->m_ctrl)->OnGridimprime();
  }
}

void CDsxFrame::OnUpdateGridimprime(CCmdUI* pCmdUI) 
{

  pCmdUI->Enable(FALSE);
  /*
  if (theApp.m_exports && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
  {
      pCmdUI->Enable(TRUE);
  }
  else
  {
      pCmdUI->Enable(FALSE);
  }
  */
}

void CDsxFrame::OnSize(UINT nType, int cx, int cy)
{	
	int cxCur0, cxCur1, cxMin;
	BOOL setsplit = FALSE;
	if (m_wndSplitter && m_wndSplitter->Activado() && m_wndSplitter->GetColumnCount() > 1)
	{
		setsplit = TRUE;
		m_wndSplitter->GetColumnInfo( 0, cxCur0, cxMin );
		m_wndSplitter->GetColumnInfo( 1, cxCur1, cxMin );
	}	

	CFrameWnd::OnSize(nType, cx, cy);
	
	if (theApp.m_autocentrado)
	{
		if (GetActiveDocument())
		{
			if (m_alsoltarcentrar)
				m_hayquecentrar = TRUE;
			else
				OnCentrar();
		}
	}

	
	if (setsplit)
	{
		if (!cxCur0)
		{			
			m_wndSplitter->SetColumnInfo( 1, 10000, cxMin);			
		}
		else if (!cxCur1)
		{
			m_wndSplitter->SetColumnInfo( 0, 10000, cxMin);			
		}
	}	
	
	/*
	if (GetActiveDocument())
	{
			OnCentrar();
	}
	*/
}

LRESULT CDsxFrame::OnDrawPopupMenuItem(
    WPARAM wParam,
    LPARAM lParam
    )
{
CExtPopupMenuWnd::DRAWITEMDATA * pDrawItemData =
      reinterpret_cast
        < CExtPopupMenuWnd::DRAWITEMDATA * >
            ( lParam );
    ASSERT( pDrawItemData != NULL );
	// m_nItemIndex
	// m_pOwner GetOwner()	
   CMenu *main = GetMenu();	 
   int single = 0;
   if (main)
   {
	  int nitem;
	  UINT cmdid;
	  //CMenu *popup;
	  const CExtPopupMenuWnd::MENUITEMDATA *pItemData = pDrawItemData->m_pItemData;

	  cmdid = pDrawItemData->GetCmdID();
	  nitem = main->GetMenuItemCount();	  
	  for (int i = 0;i < nitem;i++)
	  {
		  CMenu *popup = main->GetSubMenu( i );
		  if (popup)
		  {
			MENUITEMINFO item;
			::memset((void *)&item,0,sizeof(item));
			item.cbSize = sizeof(item);
			item.fMask = MIIM_STATE;
			if (::GetMenuItemInfo(popup->m_hMenu,cmdid,FALSE,&item))
			{
				if ((item.fState&MFS_CHECKED) != 0)
				{
					((CExtPopupMenuWnd::MENUITEMDATA *)pItemData)->Check( true );
				}
				else
				{
					((CExtPopupMenuWnd::MENUITEMDATA *)pItemData)->Check( false );
				}
			}
		 }
	  }
    }      

    pDrawItemData->PaintDefault(
      false, // no paint icon
      false, // no paint text
      false, // no paint check/radio mark
      false, // no paint as enabled
      false // no paint as unselected
      );
    // returns a non-zero value if the item is repainted
    // by this message handler
    return (!0);
}



