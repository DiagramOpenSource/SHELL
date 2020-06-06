
// Ds9Window.cpp: implementation of the CDs9Window class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "diagramx.h"
#include "DsxMainFrm.h"
#include "DsxDoc.h"
#include "DsStdDlg.h"
#include "Ds9Window.h"
#include "DsTab9.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CDs9Window, CDsStdDlg)

BEGIN_MESSAGE_MAP(CDs9Window, CDsStdDlg)        
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, Ds9DlgToolTip )
	//{{AFX_MSG_MAP(CDs9Window)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW( )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDs9Window::CDs9Window()
{
   m_tab9 = NULL;
   m_noframe = TRUE;
   m_es9 = TRUE;
   m_focoprevio = NULL;
}

CDs9Window::~CDs9Window()
{
   m_yo.m_ctrl = NULL;
   m_yo.m_cthwnd = NULL;
   m_yo.m_pdlg = NULL;

   if (m_tab9)
   {
	   if (::IsWindow(m_tab9->m_hWnd))
	   {
		   m_tab9->DestroyWindow();
	   }
	   delete m_tab9;
	   m_tab9 = NULL;
   }
}

void CDs9Window::OnShowWindow( BOOL bShow, UINT nStatus )
{
	return;
}

BOOL CDs9Window::Ds9DlgToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{	
	if (!theApp.m_tooltips)
		return FALSE;

    /* caso especial tool diagram 9 */
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
		return CDsStdDlg::DsStdDlgToolTip(id,pNMHDR,pResult);
	}

	{
		CToolTipCtrl *wnd = (CToolTipCtrl *)CToolTipCtrl::FromHandle(pNMHDR->hwndFrom);
		wnd->SetDelayTime(TTDT_AUTOPOP,100000);
	}	

	char *p = RecogeAyudaPorPosicion();
	
	if (strlen(p))
	{
		pTTT->lpszText = p; /* tmp es estatica */
		return TRUE;        
	}
	return FALSE;

/*
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
       CDsStdCtrl *pctrl = NULL;
       HWND hwnd = (HWND)nID;
       if (CDsStdDlg::m_hwndctrllst.Lookup(hwnd,pctrl))
       {        
        if (pctrl->m_tooltiptext.IsEmpty())
		{
			pTTT->lpszText = "Diagram Software";
			return TRUE;                    
		}

        pTTT->lpszText = (char *)(LPCSTR)pctrl->m_tooltiptext;
        return TRUE;        
        }
    }

    pTTT->lpszText = "Diagram Software";
    return TRUE;        
*/
}


BOOL CDs9Window::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	{
		int tecla = wParam & 0xffff;
		if (tecla >= 833 && tecla < 843)
		{
			tecla -= 833;
			if (!theApp.m_pFunciones[tecla].IsEmpty())
			{
				((CDsxDoc *)pMFrame->GetActiveDocument())->TeclaStd(tecla+833);
				return TRUE;
			}
		}
	}

    if (pDsSys->MensajeCommand9(wParam & 0xffff,lParam)) return TRUE;

	return CDsStdDlg::OnCommand(wParam, lParam);
}

BOOL CDs9Window::PreTranslateMessage(MSG* pMsg) 
{
	/*
    if (m_tooltip.m_hWnd && ::IsWindow(m_tooltip.m_hWnd))
	{
		if (pMsg->message == WM_LBUTTONDOWN || 
			pMsg->message == WM_MOUSEMOVE   ||
			pMsg->message == WM_LBUTTONUP   ||
			pMsg->message == WM_RBUTTONDOWN ||
			pMsg->message == WM_MBUTTONDOWN ||
			pMsg->message == WM_RBUTTONUP   ||
			pMsg->message == WM_MBUTTONUP)
		{
			m_tooltip.RelayEvent( pMsg );	
		}
	}
	*/

	return CDsStdDlg::PreTranslateMessage(pMsg);
}

void CDs9Window::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	pDsSys->MensajeChar9(nChar,nRepCnt,nFlags);	
	//CDsStdDlg::OnChar(nChar, nRepCnt, nFlags);
}

void CDs9Window::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	pDsSys->MensajeKeyDown9(nChar, nRepCnt, nFlags);
	//CDsStdDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDs9Window::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	pDsSys->MensajeKeyUp9(nChar, nRepCnt, nFlags);
	//CDsStdDlg::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDs9Window::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pDsSys->MensajeLButtonDblClk9(nFlags, point);	
	//CDsStdDlg::OnLButtonDblClk(nFlags, point);
}

void CDs9Window::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pDsSys->MensajeLButtonDown9(nFlags, point);	
	//CDsStdDlg::OnLButtonDown(nFlags, point);
}

void CDs9Window::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pDsSys->MensajeLButtonUp9(nFlags, point);	
	//CDsStdDlg::OnLButtonUp(nFlags, point);
}

void CDs9Window::OnMouseMove(UINT nFlags, CPoint point) 
{	
	pDsSys->MensajeMouseMove9(nFlags, point);	
	CDsStdDlg::OnMouseMove(nFlags, point);
}

void CDs9Window::OnPaint() 
{	
	/* prueba ...
	if (IsWindowVisible( ))
	{
		ShowWindow(SW_HIDE);
		return;
	}
	*/
	/*
	CDsStdDlg::OnPaint();
	return;
	*/

	/*
    MSG msg;
    if (::PeekMessage(&msg,this->m_hWnd,WM_PAINT,WM_PAINT,PM_NOREMOVE))  
    {
        ::Sleep(15);
    }
	*/

   CPaintDC dc(this); // device context for painting
  
#ifdef DSPROFUIS9
   //pMFrame->m_paintmanager->PaintDockerBkgnd( true, dc, this);
#else
   
   CBrush *hh = CBrush().FromHandle((HBRUSH)(::GetStockObject(LTGRAY_BRUSH)));
   
   dc.FillRect(&dc.m_ps.rcPaint,hh);
#endif
   
   pDsSys->MensajePaint9(dc);   
}

void CDs9Window::OnSize(UINT nType, int cx, int cy) 
{
	CDsStdDlg::OnSize(nType, cx, cy);

	/*
	if (::IsWindow(m_tooltip.m_hWnd))
	{
		CRect r;
		GetClientRect(r);
		m_tooltip.SetToolRect( this , IDW_STDDLG+m_wid, &r);
	}
	*/
}

void CDs9Window::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd); // bypass CStdDlg

	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
	theApp.m_ctrlactivo = &m_yo;
}

void CDs9Window::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDsStdDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDs9Window::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDsStdDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDs9Window::OnKillFocus(CWnd* pNewWnd) 
{
/*
  if (theApp.m_antctrlactivo == &m_yo)
  {
     theApp.m_antctrlactivo = NULL;
  }
  if (theApp.m_ctrlactivo == &m_yo)
  {
     theApp.m_ctrlactivo = theApp.m_antctrlactivo;
     theApp.m_antctrlactivo = NULL;
  }
*/
  CWnd::OnKillFocus(pNewWnd); // bypass CStdDlg
}

BOOL CDs9Window::Create(void *datos,CDsxView *pView, int& stdid)
{	
	stdid = -1;
	BOOL r = CDsStdDlg::Create(datos,pView,stdid);
	if (r)
	{
		//CRect r;
		//GetClientRect(r);
		m_yo.m_tipo = 9990999;
		m_yo.m_ctrl = (CWnd *)this;
		m_yo.m_cthwnd = m_hWnd;
		m_yo.m_pdlg = (CDsStdDlg *)this;
		m_yo.m_ctrlid = -1;
	}
	return r;
}

BOOL CDs9Window::OnEraseBkgnd(CDC* pDC)
{
	CRect  rect;
	GetClientRect(rect);
	pDC->FillSolidRect(
				&rect,
				theApp.m_9fondo);
	return TRUE;
}