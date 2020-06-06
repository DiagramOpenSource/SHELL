// DsButton.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsxDoc.h"
#include "DsButton.h"
#include "DsStdDlg.h"
#include "DsxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CCloneBitmap::CCloneBitmap()
{
	m_hObject=NULL;
	m_lpData=NULL;
}

CCloneBitmap::~CCloneBitmap()
{
}

int CCloneBitmap::Clone(HBITMAP hSrc)
{
	CBitmap bmpSrc;
	BITMAP bmp;
	if(!bmpSrc.Attach(hSrc))
		return 0;
	if(!bmpSrc.GetBitmap(&bmp))
		return 0;
	HGDIOBJ hOldSrc, hOldDest;
	if(m_hObject)
		DeleteObject();
	BITMAPINFOHEADER bmi;
	CDC dcSrc, dcDest, dcScr;
	HDC hdc;
	hdc=::GetDC(NULL);
	dcScr.Attach(hdc);
	dcDest.CreateCompatibleDC(&dcScr);
	dcSrc.CreateCompatibleDC(&dcScr);
	bmi.biBitCount=32;
	bmi.biClrImportant=0;
	bmi.biClrUsed=0;
	bmi.biCompression=BI_RGB;
	bmi.biHeight=bmp.bmHeight;
	bmi.biPlanes=1;
	bmi.biSize=sizeof(bmi);
	bmi.biSizeImage=0;
	bmi.biWidth=bmp.bmWidth;
	bmi.biXPelsPerMeter=0;
	bmi.biYPelsPerMeter=0;
	m_hObject=CreateDIBSection(dcScr, (LPBITMAPINFO)&bmi, 0, (void**)&m_lpData, NULL, DIB_RGB_COLORS);
	hOldDest=dcDest.SelectObject(*this);
	hOldSrc=dcSrc.SelectObject(bmpSrc);
	if(!dcDest.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &dcSrc, 0, 0, SRCCOPY))
		return 0;
	dcSrc.SelectObject(hOldSrc);
	dcDest.SelectObject(hOldDest);
	dcSrc.DeleteDC();
	dcDest.DeleteDC();
	dcScr.Detach();
	ReleaseDC(NULL, hdc);
	bmpSrc.Detach();
	return 1;
}

int CCloneBitmap::ChangeColor(COLORREF clrOld, COLORREF clrNew)
{
	if(m_lpData==NULL)
		return 0;
	BITMAP bmp;
	if(!GetBitmap(&bmp))
		return 0;
	if(bmp.bmHeight==0 || bmp.bmWidth==0)
		return 0;
	DWORD dwSize;
	dwSize=bmp.bmHeight*bmp.bmWidth;

	COLORREF *pData;
	pData=m_lpData;

	for(DWORD d=0; d<dwSize; d++)
	{
		if(*pData==clrOld)
			*pData=clrNew;
		pData++;
	}
	
	return 1;
}

HICON CCloneBitmap::MakeIcon(COLORREF clrTransparent)
{
	HICON hIcon;
	BITMAP bmp;
	DWORD dwSize;
	COLORREF *lpData;

	if(m_lpData==NULL)
		return NULL;
	if(!GetBitmap(&bmp))
		return NULL;
	if(bmp.bmHeight==0 || bmp.bmWidth==0)
		return NULL;

	HBITMAP hMask, hColor;
	LPCOLORREF lpColorData, lpMaskData;

	BITMAPINFOHEADER bmi;
	bmi.biBitCount=32;
	bmi.biClrImportant=0;
	bmi.biClrUsed=0;
	bmi.biCompression=BI_RGB;
	bmi.biHeight=bmp.bmHeight;
	bmi.biPlanes=1;
	bmi.biSize=sizeof(bmi);
	bmi.biSizeImage=0;
	bmi.biWidth=bmp.bmWidth;
	bmi.biXPelsPerMeter=0;
	bmi.biYPelsPerMeter=0;
	hColor=CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, 0, (void**)&lpColorData, NULL, DIB_RGB_COLORS);
	if(hColor==NULL)
		return NULL;
	hMask=CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, 0, (void**)&lpMaskData, NULL, DIB_RGB_COLORS);
	if(hMask==NULL)
	{
		::DeleteObject(hColor);
		return NULL;
	}

	dwSize=16*16;

	lpData=m_lpData;

	for(DWORD d=0; d<dwSize; d++)
	{
		if(*lpData==clrTransparent)
		{
			lpMaskData[d]=0xFFFFFF;
			lpColorData[d]=0;
		}
		else
		{
			lpMaskData[d]=0;
			lpColorData[d]=*lpData;
		}
		lpData++;
	}


	ICONINFO ii;
	ii.fIcon=1;
	ii.hbmColor=hColor;
	ii.hbmMask=hMask;
	ii.xHotspot=0;
	ii.yHotspot=0;
	hIcon=CreateIconIndirect(&ii);

	::DeleteObject(hMask);
	::DeleteObject(hColor);

	return hIcon;
}


/////////////////////////////////////////////////////////////////////////////
// CDsButton

CDsButton::CDsButton()
{
m_Font = NULL;
m_idimagen = -1;
m_pstdctrl = NULL;    
m_pDoc = NULL;
m_group = FALSE;
m_pBkBrush = NULL;
m_teclarespuesta = -1;
m_escampo = FALSE;
m_noesconsulta = FALSE;
m_dentro = FALSE;
m_pulsado = FALSE;
m_tecladirecta = -1;

#ifdef DSPROFUIS
m_rcIconMargins = CRect( 0, 0, 0, 0 );
m_rcTextMargins = CRect( 0, 0, 0, 0 );
#endif

}

CDsButton::~CDsButton()
{
 if (theApp.m_ctrlactivo == m_pstdctrl)
 {
     theApp.m_ctrlactivo = NULL;
 }
 else
 if (theApp.m_antctrlactivo == m_pstdctrl)
 {
     theApp.m_antctrlactivo = NULL;
 }
 if (m_tecladirecta != -1) {
	 theApp.m_teclashortcuts.RemoveKey(m_tecladirecta);
 }

 if (m_Font)
	 delete m_Font;
 if (m_pBkBrush)
     delete m_pBkBrush;
}


BEGIN_MESSAGE_MAP(CDsButton, BASECDsButton)
	//{{AFX_MSG_MAP(CDsButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(BN_DOUBLECLICKED, OnDoubleclicked)
	ON_WM_PARENTNOTIFY_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsButton message handlers

void CDsButton::OnClicked() 
{        
	if (!m_pulsado)
		return;
	m_pulsado = FALSE;
	if (m_dentro)
		return;
	m_dentro = TRUE;
    if (!m_group && m_pstdctrl && !(m_pstdctrl+pmask)->DisparaEventoUsuario())
    {
        TeclaStd(m_teclarespuesta);
    }    
	m_dentro = FALSE;
}

void CDsButton::PerformClick() {
    m_pulsado = TRUE;
	OnClicked();
	m_pulsado = FALSE;
}

void CDsButton::Izquierdo()
{
#ifdef DSPROFUIS
#ifdef PROFUIS240
	SetAlign(CExtPaintManager::__ALIGN_HORIZ_RIGHT);
#else
	SetTextAlignment(CExtPaintManager::__ALIGN_HORIZ_RIGHT);
#endif
#else
	m_izquierda = TRUE;
#endif
}

BOOL CDsButton::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		m_pulsado = TRUE;
	}
	else if (!m_group && pMsg->message == WM_KEYDOWN)
    {
        WPARAM comando = 0;
        WPARAM nChar = pMsg->wParam;
        
        comando = CDsStdDlg::DeterminaTecla(nChar);

        if (!comando)
        {
			if (nChar == ' ')
			{
				m_pulsado = TRUE;
			}
			else if (nChar >= 'A' && nChar <= 'Z' && !(::GetKeyState( VK_CONTROL ) & 0x8000))
            {
                comando = nChar;
            }
        }
        if (comando)
        {
		   if (comando == 802)
		   {
			   m_pulsado = TRUE;
			   OnClicked();
			   return TRUE;
		   }

           TeclaStd(comando);
           return TRUE;
        }
    }
	return BASECDsButton::PreTranslateMessage(pMsg);
}


BOOL CDsButton::TeclaStd(int tecla)
{
    if (m_pstdctrl)
    {        
        return m_pstdctrl->TeclaStd(tecla);        
    }
    return FALSE;
}

void CDsButton::OnKillFocus(CWnd* pNewWnd) 
{
	BASECDsButton::OnKillFocus(pNewWnd);

    if (m_group) return;

#ifndef DSPROFUIS	
	if (!m_izquierda)
	{
		//UINT style = GetButtonStyle( ) &  ~BS_DEFPUSHBUTTON;
		//SetButtonStyle( style );
		SetDefaultButton( FALSE );
	}
#endif

	/*
    if (theApp.m_ctrlactivo == m_pstdctrl)
    {
        theApp.m_antctrlactivo = theApp.m_ctrlactivo;
        theApp.m_ctrlactivo = NULL;
    }
	*/
}

void CDsButton::OnSetFocus(CWnd* pOldWnd) 
{
	BASECDsButton::OnSetFocus(pOldWnd);		

    if (m_group) return;


#ifndef DSPROFUIS	
	if (!m_izquierda)
	{
		//UINT style = GetButtonStyle( ) | BS_DEFPUSHBUTTON;
		//SetButtonStyle( style );
		SetDefaultButton( TRUE );
	}
#endif	

	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
    theApp.m_ctrlactivo = m_pstdctrl;
	if (m_pstdctrl)
		m_pstdctrl->EnsureVisible(); 

	if (m_pstdctrl)
	{
		/*
		if (m_escampo && m_noesconsulta)
		{
			HWND oldw = NULL;
			if (pOldWnd)
				oldw = pOldWnd->m_hWnd;
			if (!theApp.m_hwnd_999)
				theApp.m_hwnd_999 = (int)oldw;
			TeclaStd(999);
		}
		*/
	}	
}

BOOL CDsButton::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ((wParam & 0xffff) == ID_Menu32920) {
		m_pulsado = TRUE;
		OnClicked();
		return TRUE;
	}
    if ((wParam & 0xffff) == ID_CUSTOM_ESTADO)
    {		
		if (lParam == 1001)
		{
			m_noesconsulta = FALSE;
		} else if (lParam == 1100)
		{
			m_noesconsulta = TRUE;
		}
		return TRUE;
	} else if (!m_group && (wParam&0xffff) >= 800 && (wParam&0xffff) <= 999)
    {
        TeclaStd((wParam&0xffff));
        return TRUE;
    }
    else
	{
		return BASECDsButton::OnCommand(wParam, lParam);
	}
}

void CDsButton::SetWindowText( LPCTSTR lpszString )
{	
	BASECDsButton::SetWindowText( lpszString );
}

void CDsButton::OnDestroy() 
{
    if (!m_group && theApp.m_antctrlactivo == m_pstdctrl)
    {
       theApp.m_antctrlactivo = NULL;
    }
	BASECDsButton::OnDestroy();
}

HBRUSH CDsButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    if (!m_group) return NULL;
    CRect r;
    GetClientRect(r);
    if (m_pBkBrush)
    {
        pDC->SetBkColor( m_BkColor );
        pDC->SetTextColor( m_FrColor );        
	    return (HBRUSH)m_pBkBrush->m_hObject;
    }
    pDC->SetBkColor( m_pstdctrl->m_pdlg->m_cLabelBk );
    pDC->SetTextColor( m_pstdctrl->m_pdlg->m_cLabelFr );

	return (HBRUSH)m_pstdctrl->m_pdlg->m_bLabelBk->m_hObject;
}


void CDsButton::OnDoubleclicked() 
{
	OnClicked();	
}

void CDsButton::ParentNotify(UINT message, LPARAM lParam) 
{
	// TODO: Add your message handler code here
	
}

HBRUSH CDsButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = BASECDsButton::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDsButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	return BASECDsButton::PreCreateWindow(cs);
}

void CDsButton::PreSubclassWindow() 
{
		
	BASECDsButton::PreSubclassWindow();
}



int CDsButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
#ifndef DSPROFUIS
	if (m_idimagen > -1 || m_izquierda)
	{
		lpCreateStruct->style |= BS_OWNERDRAW;
	}
#else
	if (m_idimagen > -1)
	{
		HBITMAP hBmp;
#ifndef DSPROFUIS // PROFUIS240
		COLORREF transparente = RGB(192,192,192);
		int i;
		for (i = 0;i < theApp.m_nbotones;i++)
		{
			if (theApp.m_botones[i].id == m_idimagen)
			{				
				transparente = theApp.m_botones[i].transparente;
				hBmp=(HBITMAP)::LoadImage(theApp.m_hInstance,theApp.m_botones[i].path,IMAGE_BITMAP,0,0,/*LR_DEFAULTSIZE |*/ LR_LOADFROMFILE );
				break;
			}
		}
		if (i >= theApp.m_nbotones)
		{
			hBmp=LoadBitmap(AfxGetResourceHandle(),
				MAKEINTRESOURCE(m_idimagen));
		}
		if(hBmp!=NULL)
		{
			m_icon.AssignFromHBITMAP(hBmp,transparente,NULL,false);
		}
#else		
		CCloneBitmap bmpClone;
		HICON hIcon;
		COLORREF transparente = RGB(192,192,192);
		int i;
		for (i = 0;i < theApp.m_nbotones;i++)
		{
			if (theApp.m_botones[i].id == m_idimagen)
			{				
				transparente = theApp.m_botones[i].transparente;
				hBmp=(HBITMAP)::LoadImage(theApp.m_hInstance,theApp.m_botones[i].path,IMAGE_BITMAP,0,0,/*LR_DEFAULTSIZE |*/ LR_LOADFROMFILE );
				break;
			}
		}
		if (i >= theApp.m_nbotones)
		{
			hBmp=LoadBitmap(AfxGetResourceHandle(),
				MAKEINTRESOURCE(m_idimagen));
		}
		if(hBmp!=NULL)
		{
			bmpClone.Clone(hBmp);
			DeleteObject(hBmp);
			hIcon=bmpClone.MakeIcon(transparente);
			//**
#ifndef PROFUIS240
			m_icon.AssignFromHICON(hIcon,false );
#else
			m_hIconIn = hIcon;
			m_hIconOut =hIcon;			
#endif
			//**
			DeleteObject(hBmp);
		}	
#endif
	}

	if (m_pBkBrush && m_BkColor != m_FrColor) {
	  SetBkColor( m_BkColor );
      SetTextColorNormal( m_FrColor );        
	  SetFlat(TRUE);
	}

#endif
	if (BASECDsButton::OnCreate(lpCreateStruct) == -1)
		return -1;	
#ifndef DSPROFUIS
	if (m_idimagen > -1)
	{
		SetMaskedBitmap( m_idimagen,-1, /*lpCreateStruct->cy-3*/-1, RGB(192,192,192));	
	}
#endif
	return 0;
}

void CDsButton::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	BASECDsButton::OnLButtonDown(nFlags, point);
}
