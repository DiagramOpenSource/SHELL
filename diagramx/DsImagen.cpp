// DsImagen.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsxDoc.h"
#include "DsStdDlg.h"
#include "DsxView.h"
#include "DsImagen.h"

#include "anydec.h"
#include "winbmp.h"
#include "except.h"
#include "streamsink.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsImagen

CDsImagen::CDsImagen()
{
	m_hbmp = NULL;
	m_abmp = NULL;
	m_path = "";
    m_dentro = FALSE;
    m_pstdctrl = NULL;
	m_totalsize = CSize(0,0);
	m_conscroll = FALSE;
	m_posicion.x = 0;
	m_posicion.y = 0;
}

CDsImagen::~CDsImagen()
{
	if (m_hbmp)
	{
		DeleteObject(m_hbmp);
	}
}


BEGIN_MESSAGE_MAP(CDsImagen, CExtScrollWnd)
	//{{AFX_MSG_MAP(CDsImagen)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDsImagen::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_posicion = point;
	if (m_dentro)
		return;
	m_dentro = TRUE;
    if (m_pstdctrl)
    {
		char tmp[512];
		sprintf(tmp,"%d,%d",point.x,point.y);
		(m_pstdctrl+pmask)->DisparaEventoUsuario(0,tmp);        
    }    
	m_dentro = FALSE;
}

void CDsImagen::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_dentro)
		return;
	m_dentro = TRUE;
    if (m_pstdctrl)
    {
		char tmp[512];
		sprintf(tmp,"%d,%d",point.x,point.y);
		(m_pstdctrl+pmask)->DisparaEventoUsuario(1,tmp);        
    }    
	m_dentro = FALSE;
}

BOOL CDsImagen::OnCommand(WPARAM wParam, LPARAM lParam)
{ 
	if (!LOWORD(wParam) || !lParam || ::IsWindow((HWND)lParam))
		return CExtScrollWnd::OnCommand(wParam,lParam);
	return TRUE;
}

void CDsImagen::MueveScroll(int x,int y)
{
       LONG lPosHorz = ScrollPos32Get( SB_HORZ );
       LONG lPosVert = ScrollPos32Get( SB_VERT );
	   lPosHorz += x;
	   lPosVert += y;	   	   
	   if (lPosHorz < 0)
		   lPosHorz = 0;
	   if (lPosVert < 0)
		   lPosVert = 0;	   
	   OnSwSetScrollPos( CPoint(lPosHorz,lPosVert) );
}

void CDsImagen::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ((nFlags & MK_LBUTTON) && m_conscroll)
	{
	   int x,y;
	   x = m_posicion.x - point.x;
	   y = m_posicion.y - point.y;
	   MueveScroll(x,y);
	}
	m_posicion = point;
	CExtScrollWnd::OnMouseMove(nFlags, point);
}

void CDsImagen::OnSetFocus(CWnd* pOldWnd) // Este control no debe tener el foco
{
	if (pOldWnd)
		::SetFocus(pOldWnd->m_hWnd);
	//pOldWnd
}

CSize CDsImagen::OnSwGetTotalSize() const
{
	return m_totalsize;
}

bool CDsImagen::OnSwDoScrollBy(
					CSize sizeScroll,
					bool bDoScroll // = true
					)
{
	return CExtScrollWnd::OnSwDoScrollBy(sizeScroll,bDoScroll);	
}

/////////////////////////////////////////////////////////////////////////////
// CDsImagen message handlers

BOOL CDsImagen::LoadImage(LPCSTR path, int scale,int ipx,int ipy)
{
	BOOL noh = FALSE;
	BOOL nov = FALSE;
	if (scale == 3)
	{
		nov = TRUE;
		scale = 0;
	}
	else if (scale == 4)
	{
		noh = TRUE;
		scale = 0;
	}

	if (m_hbmp)
	{
		DeleteObject(m_hbmp);
		m_hbmp = NULL;
	}
	m_conscroll = FALSE;
	HBITMAP hBmp = NULL;
	CRect rr;
	GetClientRect(&rr);	
	int sx = 0,sy = 0;
	try
	{
		{
			CAnyPicDecoder Decoder;												
			CWinBmp Bmp;

			
			if (scale == 2)
			{
				Decoder.MakeBmpFromFile(path,&Bmp,rr.Size().cx,rr.Size().cy, 32);
			}
			else						
			{
				Decoder.MakeBmpFromFile(path,&Bmp, 32);
			}
			sx = Bmp.GetWidth();
			sy = Bmp.GetHeight();						

			BITMAPINFOHEADER& bmiHeader=*Bmp.GetBMI();
			BITMAPINFO& bmInfo=*(BITMAPINFO*)Bmp.GetBMI();
			LPVOID lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
					((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
			CDC *pdc = GetDC();
			hBmp=CreateDIBitmap(pdc->m_hDC, &bmiHeader, CBM_INIT, lpDIBBits, &bmInfo, DIB_RGB_COLORS);
			ReleaseDC(pdc);
		}
	}
	catch(...)
	{
	}
	if (hBmp)
	{
		CBitmap bmp;		
		CDC *pdc = GetDC();
		int px,dx;
		int py,dy;
		double factor;

		sx -= ipx;
		sy -= ipy;
		
		if (rr.Size().cx == sx && rr.Size().cy == sy && scale == 0)
			scale = 2;
		

		if (scale == 0)
		{
			if (noh && sy > rr.Size().cy)
			{
				m_totalsize = CSize(rr.Size().cx-::GetSystemMetrics(SM_CXVSCROLL)-1,sy);
				m_conscroll = TRUE;
			}
			else if (nov && sx > rr.Size().cx)
			{
				m_totalsize = CSize(sx,rr.Size().cy-::GetSystemMetrics(SM_CYHSCROLL)-1);
				m_conscroll = TRUE;
			}
			else
			{
				m_totalsize = CSize(rr.Size().cx,rr.Size().cy);
			}
			px = (m_totalsize.cx - sx);
			py = (m_totalsize.cy - sy);
			if (px < py)
			{
				factor = (double)(m_totalsize.cx) / (double)sx;
				dx = m_totalsize.cx;
				dy = (int)(double)((double)sy * factor);				
				if (noh)				
					m_totalsize.cy = dy;				
				px = 0;
				py = (m_totalsize.cy - dy) / 2;
			}
			else
			{
				factor = (double)(m_totalsize.cy) / (double)sy;
				dy = m_totalsize.cy;				
				dx = (int)(double)((double)sx * factor);				
				if (nov)
					m_totalsize.cx = dx;
				px = (m_totalsize.cx - dx) / 2;
				py = 0;
			}			
		}
		else
		{
			if (scale == 1)
			{
				m_totalsize = CSize(sx,sy);
			}
			else
			    m_totalsize = CSize(rr.Size().cx,rr.Size().cy);

			if (sx <= rr.Size().cx)
				m_totalsize.cx = rr.Size().cx;
			else
				m_conscroll = TRUE;
			if (sy <= rr.Size().cy)				
				m_totalsize.cy = rr.Size().cy;				
			else
				m_conscroll = TRUE;

			if (m_conscroll)
			{
				if (sx < (rr.Size().cx-::GetSystemMetrics(SM_CXVSCROLL)))
				{
					m_totalsize.cx = rr.Size().cx-::GetSystemMetrics(SM_CXVSCROLL)-1;
				}
				if (sy < (rr.Size().cy-::GetSystemMetrics(SM_CYHSCROLL)))
				{
					m_totalsize.cy = rr.Size().cy-::GetSystemMetrics(SM_CYHSCROLL)-1;
				}
			}

			px = (m_totalsize.cx - sx) / 2;
			py = (m_totalsize.cy - sy) / 2;
		}



		::SetStretchBltMode(pdc->m_hDC,HALFTONE);
		CDC pdc1;
		pdc1.CreateCompatibleDC(pdc);
		::SetStretchBltMode(pdc1.m_hDC,HALFTONE);
		HDC hdc2 = ::CreateCompatibleDC(pdc->m_hDC);		
		::SetStretchBltMode(hdc2,HALFTONE);

		bmp.CreateCompatibleBitmap(pdc,m_totalsize.cx,m_totalsize.cy);		
		HBITMAP hbmp2 = (HBITMAP)::SelectObject(pdc1.m_hDC,bmp.m_hObject);

		FillRect(pdc1.m_hDC,CRect(0,0,m_totalsize.cx,m_totalsize.cy),(HBRUSH)::GetStockObject(WHITE_BRUSH));

		HBITMAP hbmp3 = (HBITMAP)::SelectObject(hdc2,hBmp);
		if (scale == 0)
		{
			::StretchBlt(pdc1.m_hDC,px,py,dx,dy,hdc2,ipx,ipy,sx,sy,SRCCOPY);
		}
		else
		{
			::BitBlt(pdc1.m_hDC,px,py,sx,sy,hdc2,ipx,ipy,SRCCOPY);
		}		
		rr.top += 2;

		//pdc1.Draw3dRect( rr ,RGB(255,255,255),RGB(0,0,0));

		rr.top -= 2;
		::SelectObject(hdc2,hbmp3);
		::DeleteDC(hdc2);
		::DeleteObject(hBmp);		
		
		m_hbmp = (HBITMAP)bmp.m_hObject;
		bmp.Detach();

		if (m_conscroll)
		{
	       LONG lPosHorz = ScrollPos32Get( SB_HORZ );
	       LONG lPosVert = ScrollPos32Get( SB_VERT );
		   ::BitBlt(pdc->m_hDC,rr.left,rr.top,rr.Size().cx,rr.Size().cy,pdc1.m_hDC,lPosHorz,lPosVert,SRCCOPY);
		}
		else
		{
		   ::BitBlt(pdc->m_hDC,rr.left,rr.top,rr.Size().cx,rr.Size().cy,pdc1.m_hDC,rr.left,rr.top,SRCCOPY);
		}

		::SelectObject(pdc1.m_hDC,hbmp2);
		pdc1.DeleteDC();
		ReleaseDC(pdc);

		m_path = path;

		OnSwUpdateScrollBars();

		return TRUE;
	}

	return FALSE;
}

char *CDsImagen::SacaCoordenadas(char *p,int &px,int &py)
{
	char *pp = p;
	while(*pp && *pp != ',') pp++;
	if (*pp == ',')
	{
		*pp = 0;
		px = atoi(p);
		*pp = ',';
		p = pp+1;
		while(*pp && *pp != ':') pp++;
		if (*pp == ':')
		{
			*pp = 0;
			py = atoi(p);
			*pp = ':';
			return (pp+1);				
		}
		else
			py = atoi(p);
	}
	return NULL;
}

BOOL CDsImagen::LoadImage(LPCSTR path)
{
	int scale = 0;	
	int px = 0,py = 0;
	if (strlen(path) > 3 && !memcmp(path,"PI:",3))
	{
		char *p = (char *)path + 3;		
		p = SacaCoordenadas(p,px,py);
		if (p)
		{
			path = (LPCSTR)p;
		}
	}

	if (strlen(path) > 6 && !memcmp(path,"SCROLL:",7))
	{
		scale = 1;
		path += 7;
	}
	else if (strlen(path) > 7 && !memcmp(path,"SCROLLH:",8))
	{
		scale = 3;
		path += 8;
	}
	else if (strlen(path) > 7 && !memcmp(path,"SCROLLV:",8))
	{
		scale = 4;
		path += 8;
	}
	return LoadImage(path,scale,px,py);
}

void CDsImagen::OnDestroy() 
{
	/*
	if (m_hbmp)
	{
		CDC *pdc = GetDC();
		::SelectObject(pdc->m_hDC,m_abmp);
		ReleaseDC(pdc);
	}
	*/
	CExtScrollWnd::OnDestroy();
}

void CDsImagen::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	
	CRect r;
	//dc.RectVisible(&r);
	GetClientRect(&r);
	if (m_hbmp)
	{		
		CDC cdc;
		cdc.CreateCompatibleDC(&dc);		
		HBITMAP abmp = (HBITMAP)::SelectObject(cdc.m_hDC,m_hbmp);
		if (m_conscroll)
		{
	       LONG lPosHorz = ScrollPos32Get( SB_HORZ );
	       LONG lPosVert = ScrollPos32Get( SB_VERT );
		   dc.BitBlt(r.left,r.top,r.Size().cx,r.Size().cy,&cdc,lPosHorz,lPosVert,SRCCOPY);
		}
		else
		   dc.BitBlt(r.left,r.top,r.Size().cx,r.Size().cy,&cdc,r.left,r.top,SRCCOPY);

		::SelectObject(cdc.m_hDC,abmp);
		cdc.DeleteDC();
	}
	
}
