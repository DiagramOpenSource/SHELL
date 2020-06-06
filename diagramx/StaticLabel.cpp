// StaticLabel.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsxMainFrm.h"
#include "DsxDoc.h"
#include "DsStdDlg.h"
#include "StaticLabel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticLabel

CStaticLabel::CStaticLabel()
{
	m_Atrib = -1;
	m_font = NULL;
    m_pDoc = NULL;
    m_pBkBrush = NULL;
    m_maparaya = NULL;
    m_dsctrl   = NULL;
    m_group    = FALSE;
	m_maparaya_size = 0;
}

CStaticLabel::~CStaticLabel()
{
	if (m_font)
		delete m_font;
    if (m_maparaya)
    {
        ::free((void *)m_maparaya);
		m_maparaya_size = 0;
    }
    if (m_pBkBrush)
        delete m_pBkBrush;
}


BEGIN_MESSAGE_MAP(CStaticLabel, DSBASECStatic)
	//{{AFX_MSG_MAP(CStaticLabel)  
    ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticLabel message handlers

static CSize stat_GetIconSize( HICON hIcon )
{
	// Get the icon sizes	
CSize szIcon = CSize(0,0);
	ASSERT( hIcon != NULL );
	if( hIcon == NULL )
		return szIcon;
ICONINFO ii;
	ZeroMemory( &ii, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
	szIcon.cx = (BYTE)(ii.xHotspot * 2);
	szIcon.cy = (BYTE)(ii.yHotspot * 2);
	if( szIcon.cx == 0 || szIcon.cy == 0 )
	{
		BITMAP bi;
		::memset( &bi, 0, sizeof(BITMAP) );
		::GetObject(
			(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
			sizeof(BITMAP),
			&bi
			);
		ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
		szIcon.cx = bi.bmWidth;
		szIcon.cy = bi.bmHeight;
	} // if( szIcon.cx == 0 || szIcon.cy == 0 )
	ASSERT( szIcon.cx > 0 && szIcon.cy > 0 );
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
	return szIcon;
}


HBRUSH CStaticLabel::CtlColor(CDC* pDC, UINT nCtlColor)
{
    if (m_pBkBrush)
    {
        pDC->SetBkColor( m_BkColor );
        pDC->SetTextColor( m_FrColor );
	    return (HBRUSH)m_pBkBrush->m_hObject;
    }
    pDC->SetBkColor( m_dsctrl->m_pdlg->m_cLabelBk );
    pDC->SetTextColor( m_dsctrl->m_pdlg->m_cLabelFr );       
	return (HBRUSH)m_dsctrl->m_pdlg->m_bLabelBk->m_hObject;
}

#ifdef DSPROFUIS
void CStaticLabel::OnPaint() 
{

DWORD dwWndStyle = GetStyle();
DWORD dwWndType = (dwWndStyle&SS_TYPEMASK);
/*
	if(		dwWndType == SS_BLACKRECT 
		||	dwWndType == SS_GRAYRECT 
		||	dwWndType == SS_WHITERECT 
		||	dwWndType == SS_BLACKFRAME 
		||	dwWndType == SS_GRAYFRAME 
		||	dwWndType == SS_WHITEFRAME 
		||	dwWndType == SS_USERITEM 
		||	dwWndType == SS_OWNERDRAW 
		||	dwWndType == SS_BITMAP 
		||	dwWndType == SS_ENHMETAFILE 
		||	dwWndType == SS_ETCHEDHORZ 
		||	dwWndType == SS_ETCHEDVERT 
		||	dwWndType == SS_ETCHEDFRAME 
		)
	{
		CStatic::OnPaint();
		return;
	}
*/
COLORREF cbk;
CBrush *pb;
if (m_pBkBrush)
{
    cbk = m_BkColor;
    pb  = m_pBkBrush;
	m_clrTextNormal = m_FrColor;
}
else
{
    cbk = m_pDoc->m_cLabelBk;
    pb  = m_pDoc->m_bLabelBk;
	m_clrTextNormal = m_dsctrl->m_pdlg->m_cLabelFr;
}

CPaintDC dcPaint( this );
CRect rcClient;
	GetClientRect( &rcClient );
	if( ! dcPaint.RectVisible(&rcClient) )
		return;
CExtMemoryDC dc( &dcPaint, &rcClient );
CRgn rgnClient;
	if( rgnClient.CreateRectRgnIndirect( &rcClient ) )
		dc.SelectClipRgn( &rgnClient );

	if (theApp.m_fondosolido1)
	{
		dc.FillRect(&dcPaint.m_ps.rcPaint,pb);
	}
	else if (theApp.m_aparienciabase == 3)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CExtPaintManagerSkin *)pMFrame->m_paintmanager)->PaintDockerBkgnd( true, dc, this , cbk))		
			)
		    dc.FillRect(&dcPaint.m_ps.rcPaint,pb);
	}
	else if (theApp.m_aparienciabase == 2)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CDsProfuisCustomOffice2007_R1 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, dc, this , cbk))		
			)
		    dc.FillRect(&dcPaint.m_ps.rcPaint,pb);
	}
	else if (theApp.m_aparienciabase == 1)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CDsProfuisCustomOffice2003 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, dc, this, cbk))		
			)
			dc.FillRect(&dcPaint.m_ps.rcPaint,pb);
	}
   

/*
COLORREF clrBackground = GetBkColor();

bool bTransparent = false;

	if(		(! bTransparent )
		&&	g_PaintManager->GetCb2DbTransparentMode(this)
		&&	( clrBackground == COLORREF(-1L) )
		)
	{
		CExtPaintManager::stat_ExcludeChildAreas(
			dc,
			GetSafeHwnd(),
			CExtPaintManager::stat_DefExcludeChildAreaCallback
			);
		//if( g_PaintManager->PaintDockerBkgnd( true, dc, this ) )
		if( pMFrame->m_paintmanager->DSPaintDockerBkgnd( true, dc , this , cbk))		
			bTransparent = true;
	}

	if( ! bTransparent )
		dc.FillSolidRect(
			&rcClient,
			(clrBackground != COLORREF(-1L)) 
				? clrBackground 
				: g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) 
			);

	*/
    if (m_maparaya && m_dsctrl)
    {
		CDsStdDlg::PintaRayas(dcPaint,m_maparaya,cbk,m_dsctrl->m_alto,m_dsctrl->m_ancho,m_dsctrl->m_stdx,m_dsctrl->m_stdy,m_dsctrl->m_offx,m_dsctrl->m_offy,CRect(0,0,0,0),CPoint(0,0),NULL,m_maparaya_size);
		return;
    }

	if( dwWndType == SS_ICON )
	{
		HICON hIcon = GetIcon();
		if( hIcon != NULL )
		{
			CSize szIcon = stat_GetIconSize( hIcon );
			bool bCenterImage = ( (dwWndStyle&SS_CENTERIMAGE) != 0 );
			int nOffsetX = bCenterImage ? (rcClient.Width() - szIcon.cx) / 2 : 0;
			int nOffsetY = bCenterImage ? (rcClient.Height() - szIcon.cy) / 2 : 0;
			VERIFY(
				::DrawIconEx(
					dc.GetSafeHdc(),
					rcClient.left + nOffsetX,
					rcClient.top + nOffsetY,
					hIcon,
					szIcon.cx,
					szIcon.cy,
					0,
					(HBRUSH)NULL,
					DI_NORMAL
					)
				);
		}	
	}
	else
	{
		CExtSafeString strText;
		int nTextLen = GetWindowTextLength();
		if( nTextLen > 0 )
		{
			GetWindowText( strText.GetBuffer( nTextLen + 2 ), nTextLen + 1 );
			strText.ReleaseBuffer();
		} // if( nTextLen > 0 )
		if( strText.GetLength() > 0 )
		{
			DWORD dwDrawTextFlags = 0;
				switch( dwWndType )
			{
			case SS_RIGHT: 
				dwDrawTextFlags = DT_RIGHT|DT_WORDBREAK; 
				break; 
			case SS_CENTER: 
				dwDrawTextFlags = DT_CENTER|DT_WORDBREAK;
				break;
			case SS_LEFTNOWORDWRAP: 
				dwDrawTextFlags = DT_LEFT; 
				break;
			default: // all the other types assumed as left
			case SS_LEFT: 
				dwDrawTextFlags = DT_LEFT|DT_WORDBREAK; 
				break;
				} // switch( dwWndType )

			if( strText.Find( _T('\t') ) != -1 ) // do tabs expanding
				dwDrawTextFlags |= DT_EXPANDTABS;

			if( (dwWndStyle&SS_CENTERIMAGE) != 0 )
			{ // center vertically
				// dwDrawTextFlags = DT_CENTER; // - 2.28
				// DT_VCENTER is for DT_SINGLELINE only
				if( strText.Find( _T("\r\n") ) == -1 )
					dwDrawTextFlags |= DT_VCENTER|DT_SINGLELINE;
			} // center vertically

			dwDrawTextFlags |= (DT_VCENTER|DT_END_ELLIPSIS); 

			bool bEnabled = IsWindowEnabled() ? true : false;
			OnDrawLabelText(
				dc,
				rcClient,
				strText,
				dwDrawTextFlags,
				bEnabled
				);
		} // if( strText.GetLength() > 0 )
	}
	g_PaintManager->OnPaintSessionComplete( this );
	if( rgnClient.GetSafeHandle() != NULL )
		dc.SelectClipRgn( &rgnClient );	
}
#else
void CStaticLabel::OnPaint() 
{
    if (m_maparaya && m_dsctrl)
    {
        COLORREF cbk;
        CBrush *pb;
        if (m_pBkBrush)
        {
            cbk = m_BkColor;
            pb  = m_pBkBrush;
        }
        else
        {
            cbk = m_pDoc->m_cLabelBk;
            pb  = m_pDoc->m_bLabelBk;
        }
        CPaintDC dc(this);
        if (!m_group)
        {
            dc.FillRect(&dc.m_ps.rcPaint,pb);
            CDsStdDlg::PintaRayas(dc,m_maparaya,cbk,m_dsctrl->m_alto,m_dsctrl->m_ancho,m_dsctrl->m_stdx,m_dsctrl->m_stdy,m_dsctrl->m_offx,m_dsctrl->m_offy,CRect(0,0,0,0),CPoint(0,0),NULL,m_maparaya_size);
        }
        else
        {
            CDsStdDlg::PintaRayas(dc,m_maparaya,cbk,m_dsctrl->m_alto,m_dsctrl->m_ancho,m_dsctrl->m_stdx,m_dsctrl->m_stdy,m_dsctrl->m_offx,m_dsctrl->m_offy,CRect(0,0,0,0),CPoint(0,0),pb,m_maparaya_size);
        }
    }
    else
    {
        DSBASECStatic::OnPaint();
    }
}
#endif

/*
     CString text; GetWindowText(text);
     int l = text.GetLength();
     CRect rectClient = rect;

     // get font from control
     CFont* pFont = GetFont();

     // ensure we have a valid height and width and select the font
     LOGFONT logfont;
     pFont->GetObject(sizeof(LOGFONT),&logfont);
     if (logfont.lfHeight == 0) logfont.lfHeight = 20;
     logfont.lfWidth = 0;     // 0 so it will be calculated
     logfont.lfWeight = 1000;
     logfont.lfEscapement = logfont.lfOrientation = 0;
     CFont tryfont; VERIFY(tryfont.CreateFontIndirect(&logfont));
     CFont* pFontOld = pDC->SelectObject(&tryfont);

     // get the control size and adjust font width & height accordingly
     if (m_bUse3D) rectClient.DeflateRect(3,3);
     CSize textSizeClient = pDC->GetTextExtent(text,l);
     if (rectClient.Width()*textSizeClient.cy >
rectClient.Height()*textSizeClient.cx) {
          logfont.lfHeight = ::MulDiv(logfont.
lfHeight,rectClient.Height(),textSizeClient.cy);
     } else {
          logfont.lfHeight = ::MulDiv(logfont.
lfHeight,rectClient.Width(),textSizeClient.cx);
     }
     logfont.lfHeight--; // fudge factor
     if (m_bUse3D) rectClient.InflateRect(3,3);

     // create adjusted font and select
     CFont font; font.CreateFontIndirect(&logfont);
     pDC->SelectObject(&font);
     textSizeClient = pDC->GetTextExtent(text,l);

     int minx = rectClient.left+(rectClient.Width()-textSizeClient.cx)/2;
     int miny = rectClient.top+(rectClient.Height()-textSizeClient.cy)/2;

     int oldBkMode = pDC->SetBkMode(TRANSPARENT);
     COLORREF textcol = ::GetSysColor((state & ODS_FOCUS) ? COLOR_GRAYTEXT
: COLOR_BTNTEXT);
     COLORREF oldTextColor = pDC->SetTextColor(textcol);

     int cx = minx;
     int cy = miny;
     if (m_bUse3D) {
          int s = (state & ODS_SELECTED) ? -1 : +1;
          cx += 3; cy += 3;

          // draw 3D highlights
          pDC->SetTextColor(::GetSysColor(COLOR_3DDKSHADOW));
          pDC->TextOut(cx-s*2,cy+s*2,text);
          pDC->TextOut(cx+s*2,cy-s*2,text);
          pDC->TextOut(cx+s*2,cy+s*2,text);
          pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
          pDC->TextOut(cx+s*1,cy-s*2,text);
          pDC->TextOut(cx-s*2,cy+s*1,text);
          pDC->TextOut(cx-s*2,cy-s*2,text);
          pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
          pDC->TextOut(cx-s*1,cy+s*1,text);
          pDC->TextOut(cx+s*1,cy-s*1,text);
          pDC->TextOut(cx+s*1,cy+s*1,text);
          pDC->SetTextColor(::GetSysColor(COLOR_3DLIGHT));
          pDC->TextOut(cx,cy-s*1,text);
          pDC->TextOut(cx-s*1,cy,text);
          pDC->TextOut(cx-s*1,cy-s*1,text);
          pDC->SetTextColor(textcol);
     }
     // draw the text
     pDC->TextOut(cx,cy,text);

     // restore DC
     pDC->SetTextColor(oldTextColor);
     pDC->SetBkMode(oldBkMode);
     pDC->SelectObject(pFontOld);
*/

BOOL CStaticLabel::Atributo(int Atrib, int Cc, int Cf)
{
	CDsStdDlg *pDlg = m_dsctrl->m_pdlg;
	if (m_Atrib != Atrib)
	{
 	   if (m_font)
	  	  delete m_font;
	   m_font = NULL;
       if (m_pBkBrush)
          delete m_pBkBrush;
	   m_pBkBrush = NULL;
	   if ( (Atrib & 0x8000) )
	   {
		   int idx;
		   idx = (Atrib >> 8) & ~128;
		   if (idx >= 0 && idx < 128)
		   {
			   COLORREF cb,cf;
			   if (Cc == -1)
			   {
				   Cc = theApp.m_cf[idx];
			   }
			   if (Cf == -1)
			   {
				   Cf = theApp.m_cb[idx];
			   }
			   cf = (COLORREF)Cc;
			   cb = (COLORREF)Cf;
			   m_pBkBrush = new CBrush();
			   m_pBkBrush->CreateSolidBrush(cb);
               m_BkColor = cb;
               m_FrColor = cf;
			   m_font = new CFont();
			   m_font->CreateFontIndirect(&(theApp.m_dfont[idx]));
		   }
	   }
       else switch((Atrib&~128))
       {
           case 'R':
             m_pBkBrush = new CBrush();
             m_pBkBrush->CreateSolidBrush(pDlg->m_pDoc->m_cLabelRBk);
             m_BkColor = pDlg->m_pDoc->m_cLabelRBk;
             m_FrColor = pDlg->m_pDoc->m_cLabelRFr;
             break;
           case 'I':
             m_pBkBrush = new CBrush();
             m_pBkBrush->CreateSolidBrush(pDlg->m_pDoc->m_cLabelIBk);
             m_BkColor = pDlg->m_pDoc->m_cLabelIBk;
             m_FrColor = pDlg->m_pDoc->m_cLabelIFr;
             break;             
       }
	   m_Atrib = Atrib;
#ifdef DSPROFUIS
	   //SetBkColor( m_BkColor );
	   SetTextColor( true, m_FrColor );
#endif
	   return TRUE;
	}
	return FALSE;
}

void CStaticLabel::PonFont()
{	
   if (m_font)
   {
	   SetFont(m_font);
   }
   else if (!m_group)
   {
	  CDsStdDlg *pDlg = m_dsctrl->m_pdlg;
      if (m_BkColor == pDlg->m_pDoc->m_cLabelRBk && m_FrColor == pDlg->m_pDoc->m_cLabelRFr)
      {
          SetFont(pDlg->m_pDoc->m_cfLabelR);
      }
      else
      if (m_BkColor == pDlg->m_pDoc->m_cLabelIBk && m_FrColor == pDlg->m_pDoc->m_cLabelIFr)
      {
          SetFont(pDlg->m_pDoc->m_cfLabelI);
      }
      else
      {
          SetFont(pDlg->m_pDoc->m_cfLabel);
      }              
   }
#ifdef DSPROFUIS
    HFONT hFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0L, 0L );
	if( hFont == NULL )
	{
		HWND hWndParent = ::GetParent( m_hWnd );
		if( hWndParent != NULL )
			hFont = (HFONT)
				::SendMessage( hWndParent, WM_GETFONT, 0L, 0L );
	} // if( hFont == NULL )
	if( hFont == NULL )
	{
		hFont = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
		if( hFont == NULL )
			hFont = (HFONT)::GetStockObject( SYSTEM_FONT );
	} // if( hFont == NULL )
	LOGFONT lf;
	::GetObject( 
		(HGDIOBJ)hFont, 
        sizeof(lf), 
        (LPVOID) &lf 
		);
	hFont = NULL;
	
	if (lf.lfWeight == FW_BOLD)
		m_bFontBold = true;
	else
		m_bFontBold = false;	
	if (lf.lfItalic)
		m_bFontItalic = true;
	else
		m_bFontItalic = false;
	if (lf.lfUnderline)
		m_bFontUnderline = true;
	else
		m_bFontUnderline = false;
	if (lf.lfStrikeOut)
		m_bFontStrikeOut = true;
	else
		m_bFontStrikeOut = false;
#endif
}
