#if !defined(AFX_DSBUTTON_H__1D9FCCB3_A0F7_11D3_B2B2_004F4905CBCF__INCLUDED_)
#define AFX_DSBUTTON_H__1D9FCCB3_A0F7_11D3_B2B2_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsButton window

#include "DrawButton.h"

class CDsStdCtrl;

#ifdef DSPROFUIS
#define BASECDsButton CDsExtButton
#else
#define BASECDsButton CDrawButton
#endif

#define IRGB(r,g,b)			((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define INVERSECOLOR(x)		((COLORREF)(x&0xFF00)|((x&0xFF)<<16)|((x&0xFF0000)>>16))

class CCloneBitmap : public CBitmap  
{
public:
	COLORREF *m_lpData;
	HICON MakeIcon(COLORREF clrTransparent);
	int ChangeColor(COLORREF clrOld, COLORREF clrNew);
	int Clone(HBITMAP hSrc);
	CCloneBitmap();
	virtual ~CCloneBitmap();
};


class CDsButton : public BASECDsButton 
{
// Construction
public:
	CDsButton();

// Attributes
public:
	int m_idimagen;
    CDsStdCtrl *m_pstdctrl;

    CBrush    *m_pBkBrush;
    COLORREF  m_BkColor;
    COLORREF  m_FrColor;

    CDsxDoc *m_pDoc;    

    BOOL m_group;

	int m_teclarespuesta;
	BOOL m_escampo;
	BOOL m_noesconsulta;

	BOOL m_dentro;
	BOOL m_pulsado;

	CFont *m_Font;
	//BOOL m_izquierda;
	int m_tecladirecta;

// Operations
public:
	void SetWindowText( LPCTSTR lpszString );
#ifdef PRUEBAS
    virtual void _RenderImpl(
        CDC & dc,
        bool bTransparent = false,
        bool bCombinedContent = false
        )
    {
        ASSERT_VALID( this );
        CRect rectClient;
        GetClientRect( &rectClient );
        CExtSafeString sWindowText( _T("") );
        OnQueryWindowText( sWindowText );
        bool bPB = true;
        bool bDrawFocusRect = (GetFocus() == this && m_bDrawFocusRect) ? true : false;
        bool bDefault = GetDefault() ? true : false;
        bool bPushed = m_bPushed ? true : false;
        
        bool bEnabled = OnQueryWindowEnabledState();
        
        bool bFlat = GetFlat() ? true : false;
        bool bHover =
            ( m_bMouseOver
            && !CExtPopupMenuWnd::IsMenuTracking() )
            ? true : false;
        HICON hIcon = (bHover) ? m_hIconIn : m_hIconOut;
        CExtCmdIcon icon( hIcon, false );
        bool bDetachIcon = true;
        bool bSupportsCheck = SupportsCheck() ? true : false;
        bool bChecked = false;
        if( bSupportsCheck )
        {
            bChecked = (GetCheck() == 0) ? false : true;
            if( (GetStyle()&BS_PUSHLIKE) == 0 )
                bPB = false;
            if( ! bPB )
            {
                bDrawFocusRect = false;
                if( icon.IsEmpty() || bDetachIcon )
                {
                    if( bDetachIcon )
                        icon.DetachAsHICON();
                    if( bChecked )
                    {
                        if(        m_nButtonType == BS_RADIOBUTTON
                            ||    m_nButtonType == BS_AUTORADIOBUTTON 
                            )
                        {
                            VERIFY( icon.CreateRadioMarkIcon( COLORREF(-1), COLORREF(-1), CSize(24,16) ) );
                            bDetachIcon = false;
                        }
                        else
                        {
                            VERIFY( icon.CreateCheckMarkIcon( COLORREF(-1), COLORREF(-1), CSize(24,16) ) );
                            bDetachIcon = false;
                        }
                    } // if( bChecked )
                    else
                    {
                        VERIFY( icon.CreateEmptyIcon( COLORREF(-1), CSize(24,16) ) );
                        bDetachIcon = false;
                    } // else from if( bChecked )
                } // if( icon.IsEmpty() || bDetachIcon )
                else if( bChecked )
                    bPushed = true;
                bFlat = true;
            } // if( ! bPB )
            else
            {
                if( bChecked )
                    bPushed = true;
            } // if( ! bPB )
        } // if( bSupportsCheck )
        if( GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)) )
        {
            bDrawFocusRect = false;
            bFlat = true;
        }
        
        COLORREF clrBackground = GetBkColor();
        
        if(        (! bTransparent )
            &&    (! (bHover || bPushed) )
            &&    g_PaintManager->GetCb2DbTransparentMode(this)
            &&    ( clrBackground == COLORREF(-1L) )
            )
        {
            CExtPaintManager::stat_ExcludeChildAreas(
                dc,
                GetSafeHwnd(),
                CExtPaintManager::stat_DefExcludeChildAreaCallback
                );
            if( g_PaintManager->PaintDockerBkgnd( true, dc, this ) )
                bTransparent = true;
        }
        if( ! bTransparent )
            dc.FillSolidRect( 
                &rectClient, 
                (clrBackground != COLORREF(-1L)) 
                    ? clrBackground 
                    : g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) 
            );
        
        bool bDefaultDrawing = true;		

		
        if(        g_PaintManager.m_UxTheme.IsAppThemed()
            &&    g_PaintManager.m_UxTheme.OpenThemeData( m_hWnd, L"BUTTON" ) != NULL
            )
        {
            INT nState = 
                (!bEnabled) 
                    ? PBS_DISABLED 
                    : bPushed 
                        ? PBS_PRESSED 
                        : bHover 
                            ? PBS_HOT 
                            : PBS_NORMAL;

			
            if( g_PaintManager.m_UxTheme.DrawThemeBackground(
                        dc.GetSafeHdc(), 
                        BP_PUSHBUTTON, 
                        nState, 
                        &rectClient, 
                        0
                    ) == S_OK
                )
				
                bDefaultDrawing = false;
            g_PaintManager.m_UxTheme.CloseThemeData();
        }
		
		
        if( bDefaultDrawing )
        {
            INT nState = 
                (!bEnabled) 
                    ? DFCS_INACTIVE 
                    : bPushed 
                        ? (DFCS_PUSHED|DFCS_FLAT)
                        : 0;
			//dc.RoundRect(rectClient, CPoint(10, 10));
			
            dc.DrawFrameControl(
                &rectClient, 
                DFC_BUTTON,
                DFCS_BUTTONPUSH|nState
                );
			
        } // if( bDefaultDrawing )

        CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
            this,                                           
            true, 
            rectClient, 
            (LPCTSTR)sWindowText,
            &icon, 
            true,
            bHover, 
            bPushed,
            false, 
            bEnabled,
            false,
            bDrawFocusRect, 
            bDefault, 
            m_nAlign,
            OnQueryFont(), 
            _IsMenuAvail(), 
            0, 
            true
            );
        _ppbd.m_bSeparatedDropDown = 
            m_bSeparatedDropDown ? true : false;
        if(        _ppbd.m_bSeparatedDropDown
            &&    m_nTrackingHT == 2
            &&    _ppbd.m_bPushed
            )
        {
            _ppbd.m_bPushed = false;
            _ppbd.m_bPushedDropDown = true;
        }
        
        _ppbd.m_clrForceBk = GetBkColor();
        _ppbd.m_clrForceTextDisabled = GetTextColorDisabled();
        _ppbd.m_clrForceTextNormal = GetTextColorNormal();
        _ppbd.m_clrForceTextHover = GetTextColorHover();
        _ppbd.m_clrForceTextPressed = GetTextColorPressed();
        
        if( bCombinedContent && (GetExStyle()&WS_EX_LAYOUTRTL) != 0 )
            _ppbd.m_bForceRtlLayout = true;
        
        if( (GetStyle()&BS_MULTILINE) != 0 )
            _ppbd.m_bWordBreak = true;
        
        g_PaintManager->PaintPushButton( dc, _ppbd );

        if( bDetachIcon )
            icon.DetachAsHICON();
    }
#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsButton)    
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL TeclaStd(int tecla);
	virtual ~CDsButton();
	void Izquierdo();
	void PerformClick();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsButton)
	afx_msg void OnClicked();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnDoubleclicked();
	afx_msg void ParentNotify(UINT message, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSBUTTON_H__1D9FCCB3_A0F7_11D3_B2B2_004F4905CBCF__INCLUDED_)
