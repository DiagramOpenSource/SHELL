// ListEdit.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "ListEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListEdit

void ControlListado_(int entero,char *puntero);

CMainLstW *pMWnd = NULL;


CListEdit::CListEdit()
{
    m_Fuente_s = NULL;
}

CListEdit::~CListEdit()
{
    if (m_Fuente_s)
    {
        delete m_Fuente_s;
        m_Fuente_s = NULL;
    }
}


BEGIN_MESSAGE_MAP(CListEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CListEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers



void ControlListado_(int entero,char *puntero)
{    

    if (!pMWnd)
    {

      if (entero == 2) return;

      CRect rect(0,0,640,480);

      theApp.m_pMainWnd->GetWindowRect(rect);

      pMWnd = new CMainLstW();        
      if (!pMWnd->CreateEx(WS_EX_CLIENTEDGE , AfxRegisterWndClass(NULL),
	  "Listado por Pantalla", WS_POPUPWINDOW  | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN,
	   rect,NULL,NULL)) return;

      ::SetClassLong(pMWnd->m_hWnd,GCL_HICON,(long)::LoadIcon(theApp.m_hInstance,"IDR_DS9TYPE"));
      
      pMWnd->m_pMEdt = new CListEdit();
      pMWnd->GetClientRect(rect);
      if (!pMWnd->m_pMEdt->Create( WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VISIBLE,rect,pMWnd,33212 ))
      {
          pMWnd->DestroyWindow();
          return;
      }     


      CDC *cDC = pMWnd->m_pMEdt->GetDC();
      
      int tam_s = cDC->GetDeviceCaps(LOGPIXELSX)/10; 
      int tamy  = cDC->GetDeviceCaps(LOGPIXELSY)/6;      

      pMWnd->m_pMEdt->ReleaseDC(cDC);


      pMWnd->m_pMEdt->m_Fuente_s = new CFont();	   	     
      pMWnd->m_pMEdt->m_Fuente_s->CreateFont(tamy,tam_s, 
         0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_TT_ALWAYS,
         DRAFT_QUALITY ,FIXED_PITCH,theApp.m_FontI.lfFaceName);   
      pMWnd->m_pMEdt->SetFont(pMWnd->m_pMEdt->m_Fuente_s);


      pMWnd->ShowWindow(SW_SHOWMINIMIZED);
      ::SetWindowPos(pMWnd->m_hWnd,(HWND)HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);  
    }   
    if (pMWnd && pMWnd->m_pMEdt)
    {
        if (entero == 2)
        {               
            if (pMWnd->m_comenzado)
            {
                pMWnd->m_inicio  = pMWnd->m_seguido;            
                pMWnd->m_comenzado = FALSE;
                pMWnd->m_pMEdt->SetSel(pMWnd->m_inicio,pMWnd->m_inicio);
                pMWnd->ShowWindow(SW_SHOWNORMAL);
            }
            return;
        }


        for (char *ipp = puntero;*ipp;ipp++)
        {           
           oem_a_ansi((unsigned char *)ipp);           
        }

        if (entero == 1)
        {            
            pMWnd->m_comenzado = FALSE;
        }
        
        if (!pMWnd->m_comenzado)
        {
            pMWnd->m_seguido = pMWnd->m_pMEdt->GetTextLength( );            
            pMWnd->m_pMEdt->SetSel(pMWnd->m_seguido,pMWnd->m_seguido);
            pMWnd->m_comenzado = TRUE;
        }

        pMWnd->m_pMEdt->ReplaceSel( (LPCTSTR) puntero , FALSE );        
    }


}
/////////////////////////////////////////////////////////////////////////////
// CMainLstW

CMainLstW::CMainLstW()
{
    m_pMEdt = NULL;
    m_inicio = 0;
    m_seguido= 0;
    m_comenzado=FALSE;
}

CMainLstW::~CMainLstW()
{
    if (m_pMEdt)
    {
        delete 	m_pMEdt;
        m_pMEdt = NULL;
    }
}


BEGIN_MESSAGE_MAP(CMainLstW, CWnd)
	//{{AFX_MSG_MAP(CMainLstW)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainLstW message handlers

void CMainLstW::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

    if (m_pMEdt)
    {
        CRect rect;
        GetClientRect(rect);
        m_pMEdt->MoveWindow(rect);
    }
	
	
}

void CMainLstW::OnDestroy() 
{
	CWnd::OnDestroy();
    if (pMWnd)
    {
	    delete 	pMWnd;
        pMWnd = NULL;
        
        theApp.m_pMainWnd->SetFocus();
    }
}

void CMainLstW::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
    
    if (m_pMEdt)
    {
        m_pMEdt->SetFocus();
    }	
}
