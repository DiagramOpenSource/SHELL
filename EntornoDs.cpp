
// EntornoDs.cpp : implementation file
//
#include "stdafx.h"


#ifdef YANOEXISTE

#include "Diagram9.h"
#include "MainFrm.h"
#include "Diagram9Doc.h"
#include "Diagram9View.h"
#include "LeftView.h"
#include "EntornoDs.h"
#include "WindowDs.h"
#include "LoginDialog.h"
#include "ThreadDs.h"
#include "Pantalla.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AfxLoadLibrary LoadLibrary
#define AfxFreeLibrary FreeLibrary


//static int flag_de_espera = 0;

#define AfxThrowUserException() /* */

/////////////////////////////////////////////////////////////////////////////
// CEntornoDs



CEntornoDs::CEntornoDs(CDiagram9Doc *pDoc,CMainFrame *pMainFrame)
{
   
   m_LocalParent   = NULL;
   m_avisocontrol  = 0;
   m_cursel        = 0;
   m_EnEjecucion   = FALSE;
   m_Autentificado = FALSE;
   m_wallowed      = NULL;
   m_pDoc          = pDoc;       // Documento
   m_pMainFrame    = pMainFrame; // Mainframe para barra de estado y menu principal
   m_Exe           = pDsSys->m_Exe;
   m_Args          = pDsSys->m_Args;
   m_pLeftView     = NULL;
   //m_pMainFrame    = NULL;
   m_pView         = NULL;
   m_Ejecucion     = NULL;
   m_pEntornoPrevio = NULL;
   m_tabid = MID_MITAB;
   m_wid   = MID_MIDSWIN;
   
   int sexe = m_Exe.GetLength()+1;
   char *pexe = (char *)m_Exe.GetBuffer(sexe);
   
   int sarg = m_Args.GetLength()+1;
   char *parg = (char *)m_Args.GetBuffer(sarg);
   
   ApuntaLineaDeComando(&m_argc,m_argv,100,pexe,parg);
   if (pDsSys->m_Usuario == "")
   {
      int i;
      char *p1;
      char tmp[512];
      char *p;
      char *lpszParam;
      
      for (i = 1;i < m_argc;i++)
      {
         lpszParam = m_argv[i];   
         if (lpszParam[0] == '$' && lpszParam[1] == '$' && lpszParam[2] == '$')
         {		  
            strcpy(tmp,lpszParam+3);
            p = tmp;
            while(*p && *p != ',' && *p != '#') p++;
            if (*p == ',')
            {
               *p = 0;
               p++;
               p1 = p;		 
               while(*p && *p != '#') p++;
               if (*p) *p = 0;
               pDsSys->m_Password = p1;
            }
            else		  
               if (*p) *p = 0;
         pDsSys->m_Usuario = tmp;
         }
      }
   }
   

#ifdef _DEBUG
   m_dswinsys = AfxLoadLibrary( "c:\\fgv\\proyectos\\ds_server\\ds_winsys\\debug\\ds_winsys.dll" );
   if (!m_dswinsys) 
   {   
      AfxThrowUserException( );
      exit(2);
   }
#else
   if (!(m_dswinsys = AfxLoadLibrary( theApp.m_midll )) && !(m_dswinsys = AfxLoadLibrary( "ds_winsys.dll" )))
   {  
      char tmpxx[512];
      
      tmpxx[0] = 0;
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),NULL,tmpxx,500,NULL);     
      AfxMessageBox(tmpxx);
      
      AfxThrowUserException( );
      exit(2);
   }
#endif
   else
   {
      m_fInicializa             = GetProcAddress(m_dswinsys,"fInicializa");        if (!m_fInicializa) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
      m_fFinaliza               = GetProcAddress(m_dswinsys,"fFinaliza");          if (!m_fFinaliza) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fLogin                  = GetProcAddress(m_dswinsys,"fLogin");             if (!m_fLogin) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fOpcion                 = GetProcAddress(m_dswinsys,"fOpcion");            if (!m_fOpcion) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fLogout                 = GetProcAddress(m_dswinsys,"fLogout");            if (!m_fLogout) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
      m_fDevuelveVentana        = GetProcAddress(m_dswinsys,"fDevuelveVentana");   if (!m_fDevuelveVentana) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fNuevaVentana           = GetProcAddress(m_dswinsys,"fNuevaVentana");      if (!m_fNuevaVentana) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fPreparaEjecucion       = GetProcAddress(m_dswinsys,"fPreparaEjecucion");  if (!m_fPreparaEjecucion) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fpinta_zona             = GetProcAddress(m_dswinsys,"pinta_zona");         if (!m_fpinta_zona) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_char           = GetProcAddress(m_dswinsys,"mensaje_char");       if (!m_fmensaje_char) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_keydown        = GetProcAddress(m_dswinsys,"mensaje_keydown");    if (!m_fmensaje_keydown) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_keyup          = GetProcAddress(m_dswinsys,"mensaje_keyup");      if (!m_fmensaje_keyup) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_ldblclk        = GetProcAddress(m_dswinsys,"mensaje_ldblclk");    if (!m_fmensaje_ldblclk) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_lbuttondown    = GetProcAddress(m_dswinsys,"mensaje_lbuttondown");if (!m_fmensaje_lbuttondown) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_mousemove      = GetProcAddress(m_dswinsys,"mensaje_mousemove");  if (!m_fmensaje_mousemove) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_lbuttonup      = GetProcAddress(m_dswinsys,"mensaje_lbuttonup");  if (!m_fmensaje_lbuttonup) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fmensaje_command        = GetProcAddress(m_dswinsys,"mensaje_command");    if (!m_fmensaje_command) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fDespachadorInterno     = GetProcAddress(m_dswinsys,"fDespachadorInterno");if (!m_fDespachadorInterno) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      
      
      FARPROC m_fAsignaVentana  = GetProcAddress(m_dswinsys,"fAsignaVentana");
      if (m_fAsignaVentana)
      {
         AfxMessageBox(ID_ERROR2);
         AfxThrowUserException( );
      }  
      
      
      int mi_version = (theApp.m_version & 0xffffff);  
      int version = llama_fDespachadorInterno(m_fDespachadorInterno,0,(void *)&mi_version);
      if (!version)
      {
         AfxMessageBox("Versión de DLL incompatible");
         AfxThrowUserException( );
      }
      if ((version&0xffffff) < (theApp.m_version&0xffffff))
      {
         AfxMessageBox("Versión de DLL inferior");
         AfxThrowUserException( );
      }  
      theApp.m_version = version;    
   }
   
   theApp.m_PrimerEntorno = this;
}

CEntornoDs::CEntornoDs(CEntornoDs *Previo)
{
   m_LocalParent   = NULL;
   m_rtab          = Previo->m_rtab;
   m_avisocontrol  = 0;
   m_cursel        = 0;
   m_pEntornoPrevio = Previo;
   m_EnEjecucion   = Previo->m_EnEjecucion;
   m_Autentificado = Previo->m_Autentificado;
   m_pDoc          = Previo->m_pDoc;
   m_pMainFrame    = Previo->m_pMainFrame;
   m_Exe           = Previo->m_Exe;
   m_Args          = Previo->m_Args;
   m_pLeftView     = Previo->m_pLeftView;
   m_pView         = Previo->m_pView;
   m_Ejecucion     = Previo->m_Ejecucion;
   
   m_wallowed      = NULL; // la ventana a de ser nueva
   m_tabid = Previo->m_tabid + 1;
   m_wid   = Previo->m_wid + 1;
   
   ApuntaLineaDeComando(&m_argc,m_argv,100,(char *)m_Exe.GetBuffer(m_Exe.GetLength()+1),(char *)m_Args.GetBuffer(m_Args.GetLength()+1));
   m_dswinsys      = Previo->m_dswinsys;
   m_fInicializa             = Previo->m_fInicializa;
   m_fFinaliza               = Previo->m_fFinaliza;
   m_fLogin                  = Previo->m_fLogin;
   m_fOpcion                 = Previo->m_fOpcion;
   m_fLogout                 = Previo->m_fLogout;
   m_fDevuelveVentana        = Previo->m_fDevuelveVentana;
   m_fNuevaVentana           = Previo->m_fNuevaVentana;
   m_fPreparaEjecucion       = Previo->m_fPreparaEjecucion;
   m_fpinta_zona             = Previo->m_fpinta_zona;
   m_fmensaje_char           = Previo->m_fmensaje_char;
   m_fmensaje_keydown        = Previo->m_fmensaje_keydown;
   m_fmensaje_keyup          = Previo->m_fmensaje_keyup;
   m_fmensaje_ldblclk        = Previo->m_fmensaje_ldblclk;
   m_fmensaje_lbuttondown    = Previo->m_fmensaje_lbuttondown;
   m_fmensaje_mousemove      = Previo->m_fmensaje_mousemove;
   m_fmensaje_lbuttonup      = Previo->m_fmensaje_lbuttonup;
   m_fmensaje_command        = Previo->m_fmensaje_command;
   m_fDespachadorInterno     = Previo->m_fDespachadorInterno;
}

CEntornoDs::~CEntornoDs()
{
   if (!m_pEntornoPrevio)
   {
      if (m_Ejecucion)
      {
         delete m_Ejecucion;
         m_Ejecucion = NULL;
      }
      if (m_dswinsys)
      {
         if (m_Autentificado)
         {
            LiberaUsuario();
         }
         AfxFreeLibrary( m_dswinsys );
      }
   }
   m_Exe.ReleaseBuffer();
   m_Args.ReleaseBuffer();
   if (m_LocalParent)
   {
      delete m_LocalParent;
      m_LocalParent = NULL;
   }
}


BEGIN_MESSAGE_MAP(CEntornoDs, PADRECEntornoDs)
//{{AFX_MSG_MAP(CEntornoDs)
ON_WM_MOUSEMOVE()
ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntornoDs message handlers

BOOL CEntornoDs::GetItemRect( int nItem, LPRECT lpRect )
{
    //**NOTAB**
    // return CTabCtrl::GetItemRect( nItem, lpRect );
     lpRect->top = 0;
     lpRect->left = 0;
     lpRect->bottom = 10;
     lpRect->right = 20;         
     return FALSE;
}


void CEntornoDs::ApuntaLineaDeComando(int *dsargc, char *dsargv[], int maximo, char *exe, char *args)
{
   if (maximo < 1) return;
   
   char *inic = exe;
   while (*inic && *inic == ' ') inic++;
   char *tmp;
   if (*inic == '"')
   {
      inic++;
      while (*inic && *inic == ' ') inic++;
      tmp = inic;
      while (*tmp && *tmp != '"') tmp++;
   }
   else
   {
      tmp = inic;
      while (*tmp && *tmp != ' ') tmp++;
   }
   tmp--;
   while(tmp > inic && *tmp == ' ') tmp--;
   tmp++;
   *tmp = 0;
   
   *dsargc = 1;
   dsargv[0] = inic;
   
   if (!args || maximo < 2) return;
   
   while(*args && *args == ' ') args++;
   if (*args == '"') args++;
   while(*args && *args == ' ') args++;
   if (*args)
   {
      // AL MENOS HAY UN PARAMETRO	     
      tmp = args;
      dsargv[*dsargc] = tmp;
      (*dsargc)++;
      do 
      {
         while(*tmp && *tmp != ' ' && *tmp != '"') tmp++;
         if (!*tmp || *tmp == '"') {*tmp = 0;break;} // unas comillas marcan el final!
         if (*tmp == ' ')
         {		   
            *tmp = 0;
            if (*dsargc >= maximo) break;
            
            tmp++;
            while(*tmp && *tmp == ' ') tmp++;
            if (!*tmp || *tmp == '"') {*tmp = 0;break;} // unas comillas marcan el final!
            dsargv[*dsargc] = tmp;
            (*dsargc)++;               
         }
      } while (*tmp);
   }
}


/*
   if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed && StatusEnWinsysMenu() && theApp.m_EntornoActivo->m_pLeftView)
   {
       ::Sleep(100);
       if (StatusEnWinsysMenu())
       {
           ::SetFocus(theApp.m_EntornoActivo->m_pLeftView->m_hWnd);       
           return;
       }
   }   
*/



static int p_MiContador = 0;

static int p_npan = 0;
static CString p_titulo;
static CString p_titulos[100];
static CString p_b_titulo;
static CString b_titulo;

static int a_p_npan = 0;
static CString a_p_titulo;
static CString a_p_titulos[100];
static CString a_p_b_titulo;
static CString a_b_titulo;






BOOL CEntornoDs::Autentifica()
{    
   if (m_Autentificado)
   {
      LiberaUsuario();
   }
   
   CWindowDs Winicio(this);
   
   HDC hdc = ::GetDC(NULL);
   int ancho = ::GetDeviceCaps(hdc,HORZRES) - 2;
   int alto =  ::GetDeviceCaps(hdc,VERTRES) - 2;
   ::ReleaseDC(NULL,hdc);
   if (ClaseLocal == "")
   {
      ClaseLocal = AfxRegisterWndClass( CS_DBLCLKS );
   }
   
   if (!Winicio.CreateEx(0,ClaseLocal,"INICIO DIAGRAM9",WS_OVERLAPPED,0,0,ancho,alto,NULL,NULL))
   {
      m_wallowed = NULL;
      return FALSE;
   }  
   
   m_wallowed = &Winicio;
   
   BMP_BASE_PAR pars;  
   pars.campo   = m_pDoc->m_fontcampo;
   pars.reverse = m_pDoc->m_fontrever;
   pars.intenso = m_pDoc->m_fontinten;
   pars.texto   = m_pDoc->m_fonttexto;
   pars.mensaje = m_pDoc->m_fontmensa;
   pars.boton   = m_pDoc->m_fontboton;		  
   pars.marco.top = 1;
   pars.marco.left = 1;
   pars.marco.bottom = alto-2;
   pars.marco.right = ancho -2;    
   theApp.ColoresDefecto(&pars);
   
   /*llama_fInicializa(void *fff,HINSTANCE hInst,HWND Ventana,int argc,char **argv,BMP_BASE_PAR *marco)*/
   
   int mira;
   if (!(mira = llama_fInicializa(m_fInicializa,m_dswinsys,Winicio.m_hWnd,m_argc,m_argv,&pars)))
   {
      m_wallowed = NULL;
      return FALSE;	
   }
   
   
   
   if (mira == -10)
   {       
      m_wallowed = NULL;
      return FALSE;
   }
   
   
   do
	  {
        if (pDsSys->m_Usuario == "")
        {
           CLoginDialog dlg;
           dlg.m_usuario = "ds";
           dlg.m_password = "";
           if (dlg.DoModal() != IDOK)
           {
              m_wallowed = NULL;
              llama_fFinaliza(m_fFinaliza);
              return FALSE;
           }
           pDsSys->m_Usuario = dlg.m_usuario;
           pDsSys->m_Password= dlg.m_password;                 
        }
        if (!llama_fLogin(m_fLogin,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password))
        {	   
           pDsSys->m_Usuario = "";
           AfxMessageBox("PASSWORD INCORRECTO",MB_OK | MB_ICONHAND);
        }
     } while (pDsSys->m_Usuario == "");
     
     m_wallowed = NULL;
     llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,NULL,NULL); // anula la ventana
     m_Autentificado = TRUE;
     return TRUE;
}

BOOL CEntornoDs::LiberaUsuario()
{
   if (!m_Autentificado) return FALSE;
   
   CWindowDs Winicio(this);
   if (!llama_fDevuelveVentana(m_fDevuelveVentana)) // si se Libera el usuario sin tener pantalla hay que asignar una temporal
   {    
      Winicio.CreateEx(0,ClaseLocal,"FIN DIAGRAM9",WS_OVERLAPPED | WS_DISABLED,0,0,640,480,NULL,NULL);
      BMP_BASE_PAR pars;  
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		  
      pars.marco.top = pars.marco.bottom = pars.marco.left = pars.marco.right = 0;
      theApp.ColoresDefecto(&pars);
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,Winicio.m_hWnd,&pars);
   }
   llama_fLogout(m_fLogout);
   llama_fFinaliza(m_fFinaliza);
   m_Autentificado = FALSE;
   return TRUE;
}

int SinHebra(CEntornoDs *m_pEDs);

int SinHebra(CEntornoDs *m_pEDs)
{
   if (m_pEDs) 
	  {
      CWindowDs DsWindow(m_pEDs);
      {
         //CRect tmp1;
         //m_pEDs->GetItemRect(0,&tmp1);
         //DsWindow.m_rect.SetRect(2,tmp1.bottom+1,m_pEDs->m_rtab.Size().cx-2,m_pEDs->m_rtab.Size().cy-2);
          //**NOTAB**
         m_pEDs->GetClientRect(DsWindow.m_rect);
      }
      if (ClaseLocal == "")
	     {
         ClaseLocal = AfxRegisterWndClass( CS_DBLCLKS );
	     }		 
      //if (DsWindow.CreateEx(0,ClaseLocal,"Diagram10_tab",WS_CHILD | WS_CLIPCHILDREN,DsWindow.m_rect,m_pEDs,m_pEDs->m_wid))      
      DsWindow.m_rect.top += /*(DsWindow.m_rect.Size().cy/21)*5 +*/ 5;
      DsWindow.m_rect.bottom -= 5;
      DsWindow.m_rect.left += 5;
      DsWindow.m_rect.right -= 5;
      if (DsWindow.Create(/*TCS_BOTTOM | TCS_VERTICAL |*/ WS_CLIPSIBLINGS | TCS_TABS | WS_CHILD | WS_CLIPCHILDREN,DsWindow.m_rect,m_pEDs,m_pEDs->m_wid))
	     {

         DsWindow.DeleteAllItems( );
         DsWindow.SetItemExtra( 0 );
         {
              TCITEM Item;
              Item.mask = TCIF_TEXT | TCIF_PARAM;
              Item.pszText = "TEST-Diagram10";
              Item.cchTextMax = 1;
              Item.lParam = 1;
              DsWindow.InsertItem(0,&Item);
              Item.mask = TCIF_TEXT | TCIF_PARAM;
              Item.pszText = "TEST-2";
              Item.cchTextMax = 1;
              Item.lParam = 2;
              DsWindow.InsertItem(1,&Item);
         }


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
            //CRect tmp1;
            //m_pEDs->GetItemRect(0,&tmp1);
            //DsWindow.CalculaRect();
            //CSize s = DsWindow.m_rect.Size();
            //s.cx += 4;
            //s.cy += (tmp1.bottom+3);
            //::SetWindowPos(m_pEDs->m_hWnd,NULL,0,0,s.cx,s.cy,SWP_NOMOVE | SWP_NOZORDER);
            //**NOTAB**
            m_pEDs->CalculaRect();
         }
         m_pEDs->m_pView->SetScrollDs();
         
         DsWindow.ShowWindow(SW_HIDE);
         m_pEDs->ShowWindow(SW_SHOW);
         DsWindow.ShowWindow(SW_SHOW);
         
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
}

void CEntornoDs::Ejecuta(CDiagram9View *pView, CLeftView *pLeftView, CMainFrame *pMainFrame)
{
   m_EnEjecucion = TRUE;
   
   if (!m_pEntornoPrevio)
   {
      // solo para la primera vez!!
      
      ASSERT( pView );
      //ASSERT( pLeftView );
      ASSERT( pMainFrame );
      ASSERT( (CDiagram9Doc *)pView->GetDocument() == m_pDoc ); 
      
      m_pView = pView;
      m_pLeftView = pLeftView;
      m_pMainFrame = pMainFrame;
      
      CRect m_area;
      m_pView->GetClientRect(&m_area);
     
      // Inicialmente no estan incluidas la toolbar ni la status bar que se incluiran luego
      // ocuparan (a pelo ) 42 pixels
      // se suman a pelo 20 pixels para ajustar
      m_rtab.SetRect(m_area.left+5,m_area.top+5,m_area.right,m_area.bottom-42+20);
      
      if (m_rtab.Size().cy < 361 || m_rtab.Size().cx < 565)
      {
         m_rtab.SetRect(m_area.left+5,m_area.top+5,m_area.left+569,m_area.top+365);
      }
   }
   
   m_cursel = 0;
   
   if (!m_pEntornoPrevio)
   {
     // solo para la primera vez!!  
     //**NOTAB**       
     //if (!Create(WS_CLIPSIBLINGS | TCS_TABS | WS_CHILD | WS_CLIPCHILDREN | TCS_FOCUSNEVER /*| WS_THICKFRAME*/,m_rtab,m_pView,m_tabid))
     if ( !CreateEx(WS_EX_TOOLWINDOW|WS_EX_CLIENTEDGE,NULL,"Diagram10",WS_CLIPSIBLINGS | WS_CHILD | WS_CLIPCHILDREN | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU,m_rtab,m_pView,m_tabid) )
     {
         m_pMainFrame->PostMessage(WM_QUIT);
         m_EnEjecucion = FALSE;
         return;
     }
     /*
      LONG ll = ::GetWindowLong(m_hWnd,GWL_EXSTYLE);
      ll |= WS_EX_TRANSPARENT;
      ::SetWindowLong(m_hWnd,GWL_EXSTYLE,ll);
      */
   }
   else
   {
      //**NOTAB**
      /*
      CRect mLr,mTr,tmp1;// = m_rtab;  
      m_pView->GetWindowRect(&mLr);
      m_pEntornoPrevio->GetWindowRect(&mTr); 
      m_pEntornoPrevio->GetItemRect(0,&tmp1);
      
      mTr.OffsetRect(-mLr.left-3,-mLr.top+tmp1.Size().cy);
      mTr.right += 5;
      mTr.bottom += 5;
      
      m_LocalParent = new CPantalla((CWnd *)this);
      m_LocalParent->CreateEx(WS_EX_DLGMODALFRAME,m_LocalParent->Clase(),"",WS_CHILD | WS_DLGFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,mTr,m_pView,m_tabid+50);
      
      m_rtab.bottom = m_rtab.Size().cy;
      m_rtab.right = m_rtab.Size().cx;
      m_rtab.left = 0;
      m_rtab.top = 0;
      if (!Create(TCS_TABS | WS_CHILD | WS_CLIPCHILDREN| WS_CLIPSIBLINGS,m_rtab,m_LocalParent,m_tabid))
      {
         delete m_LocalParent;
         m_LocalParent = NULL;
         return;
      }
      */
      if ( !CreateEx(WS_EX_DLGMODALFRAME,NULL,"Diagram10",WS_CLIPSIBLINGS | WS_CHILD | WS_CLIPCHILDREN | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU,m_rtab,m_pView,m_tabid) )
      {
         return;
      }
   } 
   
   //**NOTAB**
   /*
   DeleteAllItems( );
   SetItemExtra( 0 );
   {
      TCITEM Item;
      Item.mask = TCIF_TEXT | TCIF_PARAM;
      Item.pszText = "TEST-Diagram10";
      Item.cchTextMax = 1;
      Item.lParam = 1;
      if (InsertItem(0,&Item) < 0)
      {
         m_pMainFrame->PostMessage(WM_QUIT);
         m_EnEjecucion = FALSE;
         return;     
      }
   }
   */
   
   
   if (!m_pEntornoPrevio)
   {
       /*
      PostMessage(WM_COMMAND,0x3333);
      //SinHebra
      */
      theApp.m_EntornoActivo = this;
      return;
      

      // solo para la primera vez!!
      m_Ejecucion = new CThreadDs();
      m_Ejecucion->m_pEDs = this;
      m_Ejecucion->m_bAutoDelete = FALSE; // importante
      if (!m_Ejecucion->CreateThread())
      {
         m_pMainFrame->PostMessage(WM_QUIT);	
         m_EnEjecucion = FALSE;
         return;
      }
      m_Ejecucion->SetThreadPriority( THREAD_PRIORITY_NORMAL );
      //::AfxBeginThread( RUNTIME_CLASS(CExecDs),THREAD_PRIORITY_NORMAL/*THREAD_PRIORITY_TIME_CRITICAL*/);
   }
   else
   {
      //m_pEntornoPrevio->m_wallowed->PostMessage(WM_COMMAND,(WPARAM)ACTIVA_NUEVO_ENTORNO,(LPARAM)this);      
       ::SendMessage(m_pEntornoPrevio->m_wallowed->m_hWnd,WM_COMMAND,(WPARAM)ACTIVA_NUEVO_ENTORNO,(LPARAM)this);
       ::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
       BringWindowToTop();
   }
}

BOOL CEntornoDs::EnEjecucion()
{
   return(m_EnEjecucion);
}

BOOL CEntornoDs::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    /*
   if (wParam == 0x3333)
   {
       SinHebra(this);
       return TRUE;
   }
   */

         if ( (wParam & 0xffff) == SOBREPONE_PANTALLA)
         {
            // crear nuevo entorno
            CEntornoDs *ne = new CEntornoDs(this);
            ne->Ejecuta();
            //flag_de_espera = 1;
            return TRUE;       
         }
         else
            if ( (wParam & 0xffff) == REPON_PANTALLA)
            {
               delete this;
               return TRUE;
            }
            else
               return PADRECEntornoDs::OnCommand(wParam, lParam);
}

void CEntornoDs::PintaOpcion(int old_op, int op)
{		  
   //**NOTAB**
    /*
   CString tmp;  
   TCITEM Item;
   if (p_npan < 101)
   {
      tmp.Format("   -%d-",old_op+1);             
      Item.mask = TCIF_TEXT | TCIF_PARAM;
      Item.pszText = (char *)(LPCSTR)tmp;
      Item.cchTextMax = tmp.GetLength();
   }
   else
   {
      Item.mask = TCIF_PARAM;
   }             
   Item.lParam = 1;
   Item.iImage = -1;
   if (old_op >= 0)
   {
      SetItem(old_op,&Item);
   }
   if (p_npan < 101)
   {
      tmp.Format("%s -%d-",(LPCSTR)p_titulo,op+1);
      Item.pszText = (char *)(LPCSTR)tmp;
      Item.cchTextMax = tmp.GetLength();
   }
   SetItem(op,&Item);			 
   */
}

void CEntornoDs::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (!m_hWnd) return;

   if ((nFlags & MK_LBUTTON))
   {				
      CRectTracker tracker(&m_rtab,CRectTracker::hatchedBorder);
      CPoint p(point.x+m_rtab.left,point.y+m_rtab.top);
      if (tracker.Track(this,p,FALSE,(CWnd *)m_pView))
      { 		   
         if (m_pEntornoPrevio)
         {
            CRect tmp1,tmp2;
            m_LocalParent->GetWindowRect(&tmp1);
            m_pView->GetWindowRect(&tmp2);
            int x = (tmp1.left-tmp2.left)+tracker.m_rect.left;
            int y = (tmp1.top-tmp2.top)+tracker.m_rect.top;
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            m_LocalParent->SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
         }
         else
         {
            int x = m_rtab.left+tracker.m_rect.left;
            int y = m_rtab.top+tracker.m_rect.top;
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
         }
         CalculaRect();
         m_pView->SetScrollDs();
         // m_pView->PostMessage( WM_COMMAND, MID_ACTUALIZA_AREA,NULL);
         return;
      }		
   }
   PADRECEntornoDs::OnMouseMove(nFlags, point);
}

void CEntornoDs::CalculaRect()
{
   CRect tmp1,tmp2;
   ::GetWindowRect(GetParent()->m_hWnd,&tmp2);
   GetWindowRect(&tmp1);
   m_rtab.SetRect(tmp1.left-tmp2.left,
      tmp1.top-tmp2.top,
      tmp1.right-tmp2.left,
      tmp1.bottom-tmp2.top);
   m_rtab.NormalizeRect();  
}


int CEntornoDs::SetCurSel(int nItem)
{
    //**NOTAB**
    /*
   int prev = m_cursel;
   m_cursel = nItem;   // esto evita enviar el mensaje de posicionamiento por mi causa 
   int r;
   if ((r = PADRECEntornoDs::SetCurSel(nItem)) < 0)
   {
      m_cursel = prev;
   }
   return(r);
   */
   return 0;//notab
}

void CEntornoDs::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
   //**NOTAB**
    /*
   int cur = GetCurSel();
   if (cur != m_cursel)
   {
      int cuantos;
      short mensaje;
      int ant_sel = m_cursel;
      if (cur < m_cursel)
      {
         mensaje = SOLICITA_RETROCESO;
         cuantos = m_cursel - cur - 1;
      }
      else
      {
         mensaje = SOLICITA_AVANCE;
         cuantos = cur - m_cursel - 1;
      }
      int mensa = (int)(unsigned short)mensaje;	   
      m_cursel = cur;
      if (m_wallowed)
      {		  
         m_wallowed->PostMessage(WM_COMMAND,(WPARAM)mensa,(LPARAM)cuantos);		  
      }
      PintaOpcion(ant_sel,cur);	   
      *pResult = 1;
   }
   else
      *pResult = 0;
    */
}


static int CALLBACK EnumFontFamExProc(
                                      ENUMLOGFONTEX *lpelfe,    // pointer to logical-font data
                                      NEWTEXTMETRICEX *lpntme,  // pointer to physical-font data
                                      int FontType,             // type of font
                                      LPARAM lParam             // application-defined data
                                      )
{
   if ( (lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH) )
   {
      int *numerador = (int *)lParam;        
      if (*numerador >= 50) return (0);
      
      char *puntero = ((char *)lParam)+sizeof(int);
      
      strcpy(puntero+((*numerador) * 33),lpelfe->elfLogFont.lfFaceName);
      (*numerador)++;
   }
   return(1);
}


int CALLBACK SelEnumFontFamExProc(
                                  ENUMLOGFONTEX *lpelfe,    // pointer to logical-font data
                                  NEWTEXTMETRICEX *lpntme,  // pointer to physical-font data
                                  int FontType,             // type of font
                                  LPARAM lParam             // application-defined data
                                  )
{
   if ( (lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH) )
   {
      int *tamx = (int *)lParam;        
      lParam += sizeof(int);
      LOGFONT *plf = *((LOGFONT **)lParam);
      if (!*tamx)
      {
         if (lpelfe->elfLogFont.lfWidth == plf->lfWidth && strcmp(lpelfe->elfLogFont.lfFaceName,plf->lfFaceName))
         {
            *plf = lpelfe->elfLogFont;
            return(0);
         }
      }
      else
         if (lpelfe->elfLogFont.lfWidth <= *tamx && (plf->lfWidth > *tamx || lpelfe->elfLogFont.lfWidth > plf->lfWidth))
         {
            *plf = lpelfe->elfLogFont;
         }
         else
         {
            if (!plf->lfWidth || lpelfe->elfLogFont.lfWidth < plf->lfWidth)
            {
               *plf = lpelfe->elfLogFont;
            }
         }
   }
   return(1);
}

int StatusEnWinsysMenu()
{
    return 0;
}

void CEntornoDs::FontDefault()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      
      m_pDoc->DefaultFonts();

      theApp.m_cfondillo = RGB(225,255,255);
      theApp.m_ctextillo = RGB(0,0,0);
      theApp.m_cfondo_seleccion = RGB(0,0,128);
      theApp.m_ctexto_seleccion = RGB(255,255,255);
      theApp.m_cfondo_campo_seleccion = RGB(64,64,64);

      
      CRect m_area;
      m_pView->GetClientRect(&m_area);	   
      CRect tmp1;       
      GetItemRect(0,&tmp1);
      CSize s = m_wallowed->m_rect.Size();
      
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      m_wallowed->GetWindowRect(&m_wallowed->m_rect);
      s = m_wallowed->m_rect.Size();
      s.cx += 4;
      s.cy += (tmp1.bottom+3);
      ::SetWindowPos(m_hWnd,NULL,0,0,s.cx,s.cy,SWP_NOMOVE | SWP_NOZORDER);
      CalculaRect();
      m_wallowed->CalculaRect();
      m_pView->SetScrollDs();
      //GuardaFonts();
   }   
   
}

void CEntornoDs::FontAuto()
{
   int EXTRAIZQUIERDA = 0; // HOJO HABRIA QUE SACARLO DEL API INTERNO!!!
   int EXTRADERECHA = 17; // reservado para posible scroll-bar
   int LINEAS = 22;
   int COLUMNAS = 80;
   
   
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      
      CRect m_area;
      m_pView->GetClientRect(&m_area);
      
      // del modo automatico ...
      m_rtab.SetRect(m_area.left+5,m_area.top+5,m_area.right,m_area.bottom+20);
      if (m_rtab.Size().cy < 361 || m_rtab.Size().cx < 565)
      {
         m_rtab.SetRect(m_area.left+5,m_area.top+5,m_area.left+569,m_area.top+365);
      }
      CRect tmp1;       
      GetItemRect(0,&tmp1);
      
      // del modo automatico ...
      m_wallowed->m_rect.SetRect(2,tmp1.bottom+1,m_rtab.Size().cx-2,m_rtab.Size().cy-2);
      CSize s = m_wallowed->m_rect.Size();
      ::SetWindowPos(m_wallowed->m_hWnd,NULL,0,0,s.cx,s.cy,SWP_NOMOVE | SWP_NOZORDER);
      
      
      
      //HFONT agi_fcampo = ::CreateFontIndirect(&MainFontRec);
      HDC hDC = ::GetDC(m_pView->m_hWnd);
      ::SetMapMode(hDC,MM_TEXT);
      ::SetBkMode(hDC,TRANSPARENT);
      ::SetTextAlign(hDC,TA_NOUPDATECP | TA_LEFT );
      
      LOGFONT tmplf;
      tmplf.lfCharSet = DEFAULT_CHARSET;
      tmplf.lfFaceName[0] = 0;
      tmplf.lfPitchAndFamily = 0;
      
      char enumeracion[50*33+sizeof(int)];	   
      int *numerador = (int *)enumeracion;
      *numerador = 0;	   	    
      ::EnumFontFamiliesEx(hDC , &tmplf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)enumeracion,0);
      
      char *puntero = ((char *)enumeracion)+sizeof(int);
      char datos[sizeof(int)+sizeof(LOGFONT *)];
      int cx = (s.cx-EXTRADERECHA-EXTRAIZQUIERDA)/COLUMNAS;
      LOGFONT lf,*plf;
      memset((void *)&lf,0,sizeof(lf));
      if (cx >= 8)
      {
         strcpy(lf.lfFaceName,"Fixedsys");
         lf.lfHeight = -MulDiv(9,::GetDeviceCaps(hDC,LOGPIXELSY),72);
         lf.lfWidth = 8;
         lf.lfWeight = FW_MEDIUM;
         lf.lfCharSet = DEFAULT_CHARSET;
         lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
         lf.lfClipPrecision = CLIP_MASK;
         lf.lfQuality = DRAFT_QUALITY;
         lf.lfPitchAndFamily = FIXED_PITCH;
      }
      
      plf = &lf;
      memcpy(datos,(void *)&cx,sizeof(int));
      memcpy(datos+sizeof(int),(void *)&plf,sizeof(LOGFONT *));
      
      for (int i = 0;i < *numerador;i++)
      {
         tmplf.lfCharSet = DEFAULT_CHARSET;
         strcpy(tmplf.lfFaceName,puntero);
         puntero += 33;
         tmplf.lfPitchAndFamily = 0;
         ::EnumFontFamiliesEx(hDC , &tmplf,(FONTENUMPROC)SelEnumFontFamExProc,(LPARAM)datos,0);
      }
      
      m_pDoc->m_fontcampo = lf;
      m_pDoc->m_fontrever = lf;
      
      puntero = ((char *)enumeracion)+sizeof(int);
      cx = 0; // coge uno de igual cx pero diferente
      memcpy(datos,(void *)&cx,sizeof(int));
      for (i = 0;i < *numerador;i++)
      {
         tmplf.lfCharSet = DEFAULT_CHARSET;
         strcpy(tmplf.lfFaceName,puntero);
         puntero += 33;
         tmplf.lfPitchAndFamily = 0;
         ::EnumFontFamiliesEx(hDC , &tmplf,(FONTENUMPROC)SelEnumFontFamExProc,(LPARAM)datos,0);
         if (strcmp(m_pDoc->m_fontcampo.lfFaceName,lf.lfFaceName)) break;
      }
      
      m_pDoc->m_fonttexto = lf;
      m_pDoc->m_fontinten = lf;
      
      ::ReleaseDC(m_pView->m_hWnd,hDC);       
      
      
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      m_wallowed->GetWindowRect(&m_wallowed->m_rect);
      s = m_wallowed->m_rect.Size();
      s.cx += 4;
      s.cy += (tmp1.bottom+3);
      ::SetWindowPos(m_hWnd,NULL,0,0,s.cx,s.cy,SWP_NOMOVE | SWP_NOZORDER);
      CalculaRect();
      m_wallowed->CalculaRect();
      m_pView->SetScrollDs();    
      //GuardaFonts();
      }
}

void CEntornoDs::ReSize()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fontcampo;
      CFontDialog fuente(&MainFontRec,CF_FIXEDPITCHONLY | CF_FORCEFONTEXIST | CF_SCREENFONTS);   
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fontcampo = MainFontRec;
      m_pDoc->m_fontrever = MainFontRec;
      m_pDoc->m_fontinten = MainFontRec;
      m_pDoc->m_fonttexto = MainFontRec;
      
      CRect m_area;
      m_pView->GetClientRect(&m_area);	   
      CRect tmp1;       
      GetItemRect(0,&tmp1);
      CSize s = m_wallowed->m_rect.Size();
      
      
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      
      m_wallowed->GetWindowRect(&m_wallowed->m_rect);
      s = m_wallowed->m_rect.Size();
      s.cx += 4;
      s.cy += (tmp1.bottom+3);
      ::SetWindowPos(m_hWnd,NULL,0,0,s.cx,s.cy,SWP_NOMOVE | SWP_NOZORDER);
      
      CalculaRect();
      m_wallowed->CalculaRect();
      m_pView->SetScrollDs();
      
      //GuardaFonts();
   }
}

void CEntornoDs::OtrosFonts()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fontrever;
      CFontDialog fuente(&MainFontRec,CF_FIXEDPITCHONLY | CF_FORCEFONTEXIST | CF_SCREENFONTS);
      
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fontrever = MainFontRec;
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      //GuardaFonts();
   }
}

void CEntornoDs::FontTexto()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fonttexto;
      CFontDialog fuente(&MainFontRec,CF_FIXEDPITCHONLY | CF_FORCEFONTEXIST | CF_SCREENFONTS);
      
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fonttexto = MainFontRec;
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      //GuardaFonts();
   }
}

void CEntornoDs::FontInten()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fontinten;
      CFontDialog fuente(&MainFontRec,CF_FIXEDPITCHONLY | CF_FORCEFONTEXIST | CF_SCREENFONTS);
      
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fontinten = MainFontRec;
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      //GuardaFonts();
   }
}

void CEntornoDs::FontMensa()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fontmensa;
      CFontDialog fuente(&MainFontRec,CF_FORCEFONTEXIST | CF_SCREENFONTS);   
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fontmensa = MainFontRec;
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      //GuardaFonts();
   }
}

void CEntornoDs::FontBoton()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fontboton;
      CFontDialog fuente(&MainFontRec,CF_FORCEFONTEXIST | CF_SCREENFONTS);   
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fontboton = MainFontRec;
      BMP_BASE_PAR pars;	   
      pars.marco.left = 0;
      pars.marco.right = 0; 
      pars.marco.top = 1;
      pars.marco.bottom = 1;	   
      pars.campo   = m_pDoc->m_fontcampo;
      pars.reverse = m_pDoc->m_fontrever;
      pars.intenso = m_pDoc->m_fontinten;
      pars.texto   = m_pDoc->m_fonttexto;
      pars.mensaje = m_pDoc->m_fontmensa;
      pars.boton   = m_pDoc->m_fontboton;		 
      theApp.ColoresDefecto(&pars);
      llama_fNuevaVentana(m_fNuevaVentana,m_dswinsys,m_wallowed->m_hWnd,&pars);
      //GuardaFonts();
   }
}


void CEntornoDs::FontGrid()
{
   if (!m_pEntornoPrevio && m_wallowed && StatusEnWinsysMenu())
   {
      LOGFONT MainFontRec = m_pDoc->m_fontgrid;
      CFontDialog fuente(&MainFontRec,CF_FORCEFONTEXIST | CF_SCREENFONTS);   
      if (fuente.DoModal() != IDOK)
      {
         return;
      }
      m_pDoc->m_fontgrid = MainFontRec;
      //GuardaFonts();
   }
}


void CEntornoDs::SetColores()
{
    CColorDialog cdlg(theApp.m_cfondillo,0,theApp.m_pMainWnd);

    if (cdlg.DoModal() == IDOK)
    {
        theApp.m_cfondillo = cdlg.GetColor();
    }
}


void CEntornoDs::GuardaFonts()
{
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fontcampo),"Primario");
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fonttexto),"Texto");
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fontrever),"Reverse");
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fontinten),"Intenso");
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fontmensa),"Mensaje");
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fontboton),"Botones");
   m_pDoc->GrabaProfileFont(&(m_pDoc->m_fontgrid),"Grid");
}


void CEntornoDs::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	PADRECEntornoDs::OnLButtonDblClk(nFlags, point);
}


void DespachoEnLineas(int modo,RECT *r)
{
   if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed)
   {
      if ((!modo || modo == 100) && r)
	  {  
         BOOL vertical = TRUE;
         CRect rect = r;
         if (modo)
         {
             vertical = FALSE;
             rect.bottom += 17;
         }
         else
            rect.right += 17;
         theApp.m_EntornoActivo->m_wallowed->ActivaScrollBar(&rect,vertical);
      }
      else
         theApp.m_EntornoActivo->m_wallowed->DesactivaScrollBar();
	  }
}

void CEntornoDs::OnSetFocus(CWnd* pOldWnd) 
{
   PADRECEntornoDs::OnSetFocus(pOldWnd);
   if (m_wallowed)
      m_wallowed->PonFoco();	
}

int CEntornoDs::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (PADRECEntornoDs::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)::GetStockObject(LTGRAY_BRUSH));
	
	return 0;
}

void CEntornoDs::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    CBrush *hh = CBrush().FromHandle((HBRUSH)(::GetStockObject(LTGRAY_BRUSH)));
    
    dc.FillRect(&dc.m_ps.rcPaint,hh);
}

void CreaMiControl(int entero,char *puntero)
{
   if (!theApp.m_EntornoActivo || !theApp.m_EntornoActivo->m_wallowed) 
   {
      memset(puntero,0,sizeof(int));
      return;
   }
   if (strcmp(puntero,"TAB")) // solo el tab por ahora
   {
      memset(puntero,0,sizeof(int));
      return;
   }
   
   char *old_p = puntero;
   
   puntero += 257;
   int pos;
   memcpy((char *)&pos,puntero,sizeof(int));
   puntero += sizeof(int);
   pos = pos/100;
   char tmp[256];
   memcpy(tmp,(char *)&pos,sizeof(int));
   llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,2,(void *)tmp);
   RECT r;
   memcpy((char *)&r,tmp,sizeof(RECT));
   int defecto = atoi(puntero);
   puntero += 257;
   CRect rr = r;
   theApp.m_EntornoActivo->m_wallowed->ActivaTabMenu(puntero,&rr,DISPARO_CONTROL+entero*100,PREDISPARO_CONTROL+entero*100,defecto);
   entero++;
   memcpy(old_p,(char *)&entero,sizeof(int));
}

void EliminaMiControl(int entero)
{
   if (!theApp.m_EntornoActivo || !theApp.m_EntornoActivo->m_wallowed) 
   {
      return;
   }
   theApp.m_EntornoActivo->m_wallowed->DesactivaTabMenu(DISPARO_CONTROL+entero*100);
}

void AvisoAControl(int entero)
{
   if (!theApp.m_EntornoActivo || !theApp.m_EntornoActivo->m_wallowed) 
   {
      return;
   }
   
   theApp.m_EntornoActivo->m_avisocontrol = entero;
}

void SobreponePantalla()
{
   // La activacion pasa primero por crear el entorno en la hebra principal,
   // luego la ventana en la hebra de trabajo y asignarla al winsys
   if (!theApp.m_EntornoActivo || !theApp.m_EntornoActivo->m_wallowed) 
   {
      return;
   }
   //flag_de_espera = 0; // ATENCION: sincronizador de colas entre hebras
   int timeout = 0;
   //theApp.m_EntornoActivo->PostMessage(WM_COMMAND,(WPARAM)SOBREPONE_PANTALLA);
   ::SendMessage(theApp.m_EntornoActivo->m_hWnd,WM_COMMAND,(WPARAM)SOBREPONE_PANTALLA,(LPARAM)0);
   /*
   while(!flag_de_espera && timeout < 1000)
   {
      Sleep(2); 
      timeout++;
   }
   flag_de_espera = 0;
   */
   llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,5,NULL); // procesa mensajes de esta hebra
}

void ReponPantalla()
{
   // la desactivacion para por reasignar primero la stdpan y la ventana original
   if (!theApp.m_EntornoActivo || !theApp.m_EntornoActivo->m_wallowed || !theApp.m_EntornoActivo->m_pEntornoPrevio) 
   {
      return;
   }
   CEntornoDs *no = theApp.m_EntornoActivo;
   CEntornoDs *ne = theApp.m_EntornoActivo->m_pEntornoPrevio;
   
   char tmp[sizeof(void *)+sizeof(HWND)];
   char *p = tmp;
   memset(p,0,sizeof(void *)+sizeof(HWND));		  
   memcpy(p,(void *)&(ne->m_wallowed->m_hWnd),sizeof(HWND));
   p += sizeof(HWND);	
   memcpy(p,(void *)&(ne->m_wallowed->m_salvastdpan),sizeof(void *));
   llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,4,(void *)tmp);
   theApp.m_EntornoActivo = ne;
   //theApp.m_pActiveWnd = ne->m_LocalParent;
   //if (!theApp.m_pActiveWnd) theApp.m_pActiveWnd = theApp.m_pMainWnd;
   no->ShowWindow(SW_HIDE);
   delete no->m_wallowed;
   no->m_wallowed = NULL;   
   theApp.m_EntornoActivo->EnableWindow( TRUE );
   if (theApp.m_EntornoActivo->m_LocalParent)
   {
      theApp.m_EntornoActivo->m_LocalParent->EnableWindow( TRUE );
      theApp.m_EntornoActivo->m_LocalParent->BringWindowToTop();
      theApp.m_EntornoActivo->m_LocalParent->ShowWindow(SW_SHOWNORMAL);
   }   
   theApp.m_EntornoActivo->BringWindowToTop();
   theApp.m_EntornoActivo->m_wallowed->PonFoco();   
   theApp.m_EntornoActivo->m_wallowed->ActivaControles();
   //no->PostMessage(WM_COMMAND,(WPARAM)REPON_PANTALLA); // destruye el entorno actual en su hebra
   ::SendMessage(no->m_hWnd,WM_COMMAND,(WPARAM)REPON_PANTALLA,(LPARAM)0);
   ::SendMessage(theApp.m_EntornoActivo->m_pView->m_hWnd,WM_COMMAND,ID_REAJUSTASCROLL,0);
   
}






void LlamaPretranslate(MSG *msg)
{
    
    if (theApp.m_EntornoActivo)
    {
        if (theApp.m_EntornoActivo->m_Ejecucion)
        {
            if (theApp.m_EntornoActivo->m_Ejecucion->PreTranslateMessage( msg ))
            {
                msg->hwnd = NULL;
            }
        }
        else
        {
            if (msg->hwnd == theApp.m_EntornoActivo->m_wallowed->m_hWnd && msg->message == WM_KEYDOWN)
            {
                return;
            }
            if (theApp.PreTranslateMessage( msg ))
            {
                msg->hwnd = NULL;
            }
            MSG msg2;
            if (!::PeekMessage(&msg2,NULL,0, 0, PM_NOREMOVE))
            {
                theApp.OnIdle(0);
            }
        }
    }
}



void HazVisible()
{
   if (!theApp.m_EntornoActivo || !theApp.m_EntornoActivo->m_wallowed) 
   {
      return;
   }
   theApp.m_EntornoActivo->m_wallowed->HazVisible();
}


int ActivarEdita(char *txt,int area)
{
   if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed)
   {
      
      return theApp.m_EntornoActivo->m_wallowed->ActivaEdita(txt,area);
   }
   return -1;
}

#endif