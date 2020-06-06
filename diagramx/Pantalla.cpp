// Pantalla.cpp : implementation file
//

#include "stdafx.h"

#ifdef YANOEXISTE

#include "Diagramx.h"
#include "Pantalla.h"
#include "EntornoDs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPantalla

CPantalla::CPantalla(CWnd *Hijo)
{
   m_hijo = Hijo;
}

CPantalla::~CPantalla()
{
}


BEGIN_MESSAGE_MAP(CPantalla, CWnd)
//{{AFX_MSG_MAP(CPantalla)
ON_WM_SETFOCUS()
ON_WM_ACTIVATEAPP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPantalla message handlers

CString CPantalla::Clase()
{
   if (m_clase == "")
   {
      m_clase = AfxRegisterWndClass( CS_DBLCLKS | CS_NOCLOSE);
   }
   return (m_clase);
}

void CPantalla::OnSetFocus(CWnd* pOldWnd) 
{
   CWnd::OnSetFocus(pOldWnd);
   
   m_hijo->SetFocus();
   
}

void CPantalla::AjustaMinimize()
{
   
   return;
   
   CRect  mTr;// = m_rtab;  
   CPoint mpt;
   CWnd *parent = GetParent();
   
   parent->GetWindowRect(&mTr);
   mpt.x = mTr.left+5;
   mpt.y = mTr.bottom;
   
   WINDOWPLACEMENT wndpl;
   wndpl.length = sizeof(wndpl);
   wndpl.flags = WPF_SETMINPOSITION ;// | WPF_RESTORETOMAXIMIZED;
   wndpl.showCmd = SW_SHOWNA;
   wndpl.ptMinPosition = mpt;
   //wndpl.ptMaxPosition;
   GetWindowRect(&mTr);
   wndpl.rcNormalPosition = mTr;
   SetWindowPlacement( &wndpl );
}

void CPantalla::OnActivateApp(BOOL bActive, HTASK hTask) 
{
   CWnd::OnActivateApp(bActive, hTask);
   
   if (bActive)
   {
      if (m_hijo == (CWnd *)theApp.m_EntornoActivo)
	     {
         BringWindowToTop();
         ShowWindow(SW_SHOWNORMAL);
      }
   }	
}


#endif
