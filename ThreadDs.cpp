// ThreadDs.cpp : implementation file
//

#include "stdafx.h"

#ifdef YANOEXISTE

#include "Diagram9.h"
#include "ThreadDs.h"
#include "WindowDs.h"
#include "EntornoDs.h"
#include "MainFrm.h"
#include "Diagram9Doc.h"
#include "Diagram9View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThreadDs

IMPLEMENT_DYNCREATE(CThreadDs, CWinThread)

CThreadDs::CThreadDs()
{
   m_pEDs = NULL;
}

CThreadDs::~CThreadDs()
{
}

BOOL CThreadDs::InitInstance()
{
   // TODO:  perform and per-thread initialization here
   ::OleInitialize(NULL);
   return TRUE;
}

int CThreadDs::ExitInstance()
{
   // TODO:  perform any per-thread cleanup here
   ::OleUninitialize();
   return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadDs, CWinThread)
//{{AFX_MSG_MAP(CThreadDs)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadDs message handlers

int CThreadDs::Run() 
{
   
   if (m_pEDs) 
	  {
      CWindowDs DsWindow(m_pEDs);
      {
         CRect tmp1;
         m_pEDs->GetItemRect(0,&tmp1);
         DsWindow.m_rect.SetRect(2,tmp1.bottom+1,m_pEDs->m_rtab.Size().cx-2,m_pEDs->m_rtab.Size().cy-2);
      }
      if (ClaseLocal == "")
	     {
         ClaseLocal = AfxRegisterWndClass( CS_DBLCLKS );
	     }		 
      if (DsWindow.CreateEx(0,ClaseLocal,"Diagram9_View",WS_CHILD | WS_CLIPCHILDREN,DsWindow.m_rect,m_pEDs,m_pEDs->m_wid))
	     {
         DsWindow.m_visible = TRUE;
         m_pEDs->m_wallowed = &DsWindow;
         BMP_BASE_PAR pars;
         pars.marco.left = 0;		 
         pars.marco.right = 0; 
         pars.marco.top = 1;		 
         pars.marco.bottom = 1;
         pars.campo   = m_pEDs->m_pDoc->m_fontcampo;
         pars.reverse = m_pEDs->m_pDoc->m_fontrever;
         pars.intenso = m_pEDs->m_pDoc->m_fontinten;
         pars.texto   = m_pEDs->m_pDoc->m_fonttexto;
         pars.mensaje = m_pEDs->m_pDoc->m_fontmensa;
         pars.boton   = m_pEDs->m_pDoc->m_fontboton;		 
         theApp.ColoresDefecto(&pars);

         //CToolTipCtrl
         DsWindow.m_tooltip.Create(&DsWindow,TTS_NOPREFIX);

         //LPTOOLINFO lpToolInfo = new TOOLINFO;

         //DsWindow.m_tooltip.SetToolInfo( lpToolInfo );

         DsWindow.EnableToolTips( TRUE );

         CRect rr(0,0,100,100);
         DsWindow.m_tooltip.AddTool( &DsWindow, "Diagram Software"); //,&rr,7312);
         //LPCTSTR lpszText = LPSTR_TEXTCALLBACK
         //LPCRECT lpRectTool = NULL, UINT nIDTool = 0 

         DsWindow.m_tooltip.Activate( TRUE );

         
         llama_fNuevaVentana(m_pEDs->m_fNuevaVentana,m_pEDs->m_dswinsys,DsWindow.m_hWnd,&pars);
         {
            CRect tmp1;
            m_pEDs->GetItemRect(0,&tmp1);            
            DsWindow.CalculaRect();
            CSize s = DsWindow.m_rect.Size();
            s.cx += 4;
            s.cy += (tmp1.bottom+3);
            ::SetWindowPos(m_pEDs->m_hWnd,NULL,0,0,s.cx,s.cy,SWP_NOMOVE | SWP_NOZORDER);
            m_pEDs->CalculaRect();
         }
         m_pEDs->m_pView->SetScrollDs();
         
         m_pEDs->ShowWindow(SW_SHOW);
         
         theApp.m_EntornoActivo = m_pEDs;

         // àra contrarestar la activacion del tool tip
         ::SetActiveWindow(theApp.m_EntornoActivo->m_pMainFrame->m_hWnd);

         {             
             char *ptmp = theApp.m_dirbase.GetBufferSetLength(300);
             llama_fDespachadorInterno(m_pEDs->m_fDespachadorInterno,14,(void *)ptmp);
             theApp.m_dirbase.ReleaseBuffer(-1);
             theApp.m_dirbase.Replace('/','\\');
         }

         llama_fOpcion(m_pEDs->m_fOpcion);
         m_pEDs->m_wallowed = NULL;
         theApp.m_EntornoActivo = NULL;
      }
      m_pEDs->m_EnEjecucion = FALSE;
	  }	  
   
   //m_pEDs->PostMessage(WM_DESTROY);
   theApp.m_pMainWnd->PostMessage(WM_QUIT);	
   return 0;
   //return CWinThread::Run();
}

#endif