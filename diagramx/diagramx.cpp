
//
// diagramx.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <direct.h>

#include "afxpriv.h" // cuando dssys sea estandar habra que quitar esto
#ifdef _DEBUG
	void AFXAPI _AfxTraceMsg(LPCTSTR lpszPrefix, const MSG* pMsg);
#endif

#include "diagramx.h"

#include "DsxMainFrm.h"
#include "DsxIpFrame.h"
#include "DsxDoc.h"
#include "DsxLeftView.h"
#include "DsxView.h"
#include "LoginDialog.h"
#include "DlgCPasswd.h"
#include "DlgPregunta.h"
#include "DlgCambiaClave.h"
#include "DsTreeMenu.h"
#include "DsStdDlg.h"
#include "Ds9Window.h"
#include "DsTab9.h"

#include "Trasmite.h"
#include "DlgUsuarios.h"
#include "WTraza.h"
#include "DlgOldImpre.h"
#include "TablaView.h"

#include "DsEdit.h"
#include "DsCombo.h"
#include "DsTextEdit.h"
#include "DlgProgreso.h"
#include "DlgBrowse.h"

#include "DsPopup.h"

#include "DsImagen.h"

#include "DsButton.h"

#include <process.h>

#include "../../../Librairie_Excel-Win_Lin/headers/xlsFile.h"
#include "../../../Librairie_Excel-Win_Lin/headers/xls.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
#define AfxLoadLibrary LoadLibrary
#define AfxFreeLibrary FreeLibrary
*/

const char *quita_path(const char *ff);


extern "C"
{
	void *AccesoPython(int f,void *p,int *flag);
    typedef void *(*_AccesoPython)(int f,void *p,int *flag);
	typedef int (*_fnpythonpre)(MSG *msg);
	extern _fnpythonpre fnpythonpre;

	int linea_a_pdf(unsigned char *linea);
	int fin_de_pdf(char *salida);
};

_fnpythonpre fnpythonpre = NULL;


class CAboutDlg : public CExtNCW < CBASEDLG >
{
public:
   CAboutDlg();
   
   // Dialog Data
   //{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOX };
   //}}AFX_DATA
   
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CAboutDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL
   CDLGBUTTON m_boton1;
   CDLGLABEL m_label1;
   CDLGLABEL m_label2;
   CDLGLABEL m_label3;
   CDLGLABEL m_label4;
   CDLGLABEL m_label5;
   CDLGLABEL m_label6;
   CDLGLABEL m_label7;


   // Implementation
protected:
   //{{AFX_MSG(CAboutDlg)
   virtual BOOL OnInitDialog();
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDsxApp

BEGIN_MESSAGE_MAP(CDsxApp, CWinApp)
	//{{AFX_MSG_MAP(CDsxApp)
		ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


CString ClaseLocal;

CDsSys * pDsSys = NULL;

// el puntero general al documento activo
CDsxFrame * pMFrame = NULL;
/////////////////////////////////////////////////////////////////////////////
// The one and only CDsxApp object

CDsxApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {712EA093-6417-4176-BB69-D2A70681A7D4}
static const CLSID clsid1 =
{ 0x712ea093, 0x6417, 0x4176, { 0xbb, 0x69, 0xd2, 0xa7, 0x6, 0x81, 0xa7, 0xd4 } };

// {812EA093-6417-4176-BB69-D2A70681A7D4} // PARA WEB CLIENT
static const CLSID clsid2 =
{ 0x812ea093, 0x6417, 0x4176, { 0xbb, 0x69, 0xd2, 0xa7, 0x6, 0x81, 0xa7, 0xd4 } };

/* CID Ids */
const Cid NULO_CID = {-1,-1,-1};
static unsigned long _c = 0x03101010;
static unsigned long _t = 0x03141414;
static unsigned long _r = 0x03161616;
static unsigned long _p = 0x03121212;
/* */

CDsxDoc *pActiveDoc = NULL;

void ControlWord_(int entero,char *puntero);

void ControlListado_(int entero,char *puntero);


CDsSys::CDsSys()
{
   m_dswinsys             = NULL;
   m_arg2                 = 0;

   m_fPreInicializa       = NULL;
   m_fInicializa          = NULL;
   m_fFinaliza            = NULL;
   m_fLogin               = NULL;
   m_fOpcion              = NULL;
   m_fLogout              = NULL;
   m_fDevuelveVentana     = NULL;
   m_fNuevaVentana        = NULL;
   m_fPreparaEjecucion    = NULL;

   m_fDespachadorInterno  = NULL;
   m_Autentificado = FALSE;
   haybuscon = FALSE;


   if (1/*theApp.m_bsolo10*/)
   {

#ifdef _DEBUG
   m_dswinsys = AfxLoadLibrary( "c:\\fgv_10\\proyectos\\ds_server\\ds_winsys\\debug\\diagramx.dll" );   
   if (!m_dswinsys)
   {
	  if (theApp.m_bsolo10)
	  {
		  AfxThrowUserException( );
		  exit(2);
	  }
   }
#else
   
   if (!(m_dswinsys = AfxLoadLibrary( theApp.m_midll )) && !(m_dswinsys = AfxLoadLibrary( "diagramx.dll" )))
   {  
	  if (theApp.m_bsolo10)
	  {
		  char tmpxx[512];
      
		  tmpxx[0] = 0;
		  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),NULL,tmpxx,500,NULL);     
		  AfxMessageBox(tmpxx);
      
		  AfxThrowUserException( );
		  exit(2);
	  }	  
   }
#endif
   else
   {
	  

      m_fPreInicializa          = GetProcAddress(m_dswinsys,"fPreInicializa");     if (!m_fPreInicializa) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
      m_fInicializa             = GetProcAddress(m_dswinsys,"fInicializa");        if (!m_fInicializa) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
      m_fFinaliza               = GetProcAddress(m_dswinsys,"fFinaliza");          if (!m_fFinaliza) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fLogin                  = GetProcAddress(m_dswinsys,"fLogin");             if (!m_fLogin) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fOpcion                 = GetProcAddress(m_dswinsys,"fOpcion");            if (!m_fOpcion) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fLogout                 = GetProcAddress(m_dswinsys,"fLogout");            if (!m_fLogout) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
      m_fDevuelveVentana        = GetProcAddress(m_dswinsys,"fDevuelveVentana");   if (!m_fDevuelveVentana) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fNuevaVentana           = GetProcAddress(m_dswinsys,"fNuevaVentana");      if (!m_fNuevaVentana) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fPreparaEjecucion       = GetProcAddress(m_dswinsys,"fPreparaEjecucion");  if (!m_fPreparaEjecucion) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      m_fDespachadorInterno     = GetProcAddress(m_dswinsys,"fDespachadorInterno");if (!m_fDespachadorInterno) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
      
      
      FARPROC m_fAsignaVentana  = GetProcAddress(m_dswinsys,"fAsignaVentana");
      if (m_fAsignaVentana)
      {
         AfxMessageBox(ID_ERROR2);
         AfxThrowUserException( );
      }  
      
      
      int mi_version = (theApp.m_version & 0xffffff);  
      int version = DespachadorInterno(0,(void *)&mi_version);
      if (!version)
      {
         AfxMessageBox("Versión de DLL incompatible");
         AfxThrowUserException( );
      }
	  /*
      if ((version&0xffffff) < (theApp.m_version&0xffffff))
      {
         AfxMessageBox("Versión de DLL inferior");
         AfxThrowUserException( );
      } 
	  */
      theApp.m_version = version;    

	  m_Exe = theApp.m_LineaDeComando.m_exe;
	  m_Args = theApp.m_LineaDeComando.m_args;
	  ApuntaLineaDeComando(&m_argc,m_argv,100,(char *)m_Exe.GetBuffer(m_Exe.GetLength()+1),(char *)m_Args.GetBuffer(m_Args.GetLength()+1));


	  llama_fPreInicializa(m_fPreInicializa,&m_argc,(char **)m_argv);

	  char *ptmp = theApp.m_dirbase.GetBufferSetLength(300);
	  DespachadorInterno(14,(void *)ptmp);
	  theApp.m_dirbase.ReleaseBuffer(-1);
	  theApp.m_dirbase.Replace('/','\\');

	  
   }




   } /* fin de sinrecursos */


   /**** CARGA DEL Diagram9.DLL ******/
   if (!theApp.m_bsolo10)
   {

	   
#ifdef _DEBUG
	   m_ds9winsys = AfxLoadLibrary( "c:\\fgv\\proyectos\\ds_server\\ds_winsys\\debug\\ds_winsysnet.dll" );
	   if (!m_ds9winsys) 
	   {   
		  AfxThrowUserException( );
		  exit(2);
	   }
#else   
	   
	   if (!(m_ds9winsys = AfxLoadLibrary( theApp.m_midll9 )) && !(m_ds9winsys = AfxLoadLibrary( "ds_winsysnet.dll" )))
	   {  
		  char tmpxx[512];
      
		  AfxMessageBox(CString("No se puede cargar ")+theApp.m_midll9,MB_OK);

		  tmpxx[0] = 0;
		  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),NULL,tmpxx,500,NULL);     
		  AfxMessageBox(tmpxx);
      
		  AfxThrowUserException( );
		  exit(2);
	   }
#endif
	   else
	   {
		  

		  m_f9Inicializa             = GetProcAddress(m_ds9winsys,"fInicializa");        if (!m_f9Inicializa) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
		  m_f9Finaliza               = GetProcAddress(m_ds9winsys,"fFinaliza");          if (!m_f9Finaliza) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9Login                  = GetProcAddress(m_ds9winsys,"fLogin");             if (!m_f9Login) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9Opcion                 = GetProcAddress(m_ds9winsys,"fOpcion");            if (!m_f9Opcion) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9Logout                 = GetProcAddress(m_ds9winsys,"fLogout");            if (!m_f9Logout) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}  
		  m_f9DevuelveVentana        = GetProcAddress(m_ds9winsys,"fDevuelveVentana");   if (!m_f9DevuelveVentana) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9NuevaVentana           = GetProcAddress(m_ds9winsys,"fNuevaVentana");      if (!m_f9NuevaVentana) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9PreparaEjecucion       = GetProcAddress(m_ds9winsys,"fPreparaEjecucion");  if (!m_f9PreparaEjecucion) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9pinta_zona             = GetProcAddress(m_ds9winsys,"pinta_zona");         if (!m_f9pinta_zona) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_char           = GetProcAddress(m_ds9winsys,"mensaje_char");       if (!m_f9mensaje_char) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_keydown        = GetProcAddress(m_ds9winsys,"mensaje_keydown");    if (!m_f9mensaje_keydown) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_keyup          = GetProcAddress(m_ds9winsys,"mensaje_keyup");      if (!m_f9mensaje_keyup) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_ldblclk        = GetProcAddress(m_ds9winsys,"mensaje_ldblclk");    if (!m_f9mensaje_ldblclk) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_lbuttondown    = GetProcAddress(m_ds9winsys,"mensaje_lbuttondown");if (!m_f9mensaje_lbuttondown) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_mousemove      = GetProcAddress(m_ds9winsys,"mensaje_mousemove");  if (!m_f9mensaje_mousemove) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_lbuttonup      = GetProcAddress(m_ds9winsys,"mensaje_lbuttonup");  if (!m_f9mensaje_lbuttonup) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9mensaje_command        = GetProcAddress(m_ds9winsys,"mensaje_command");    if (!m_f9mensaje_command) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  m_f9DespachadorInterno     = GetProcAddress(m_ds9winsys,"fDespachadorInterno");if (!m_f9DespachadorInterno) {AfxMessageBox(ID_ERROR2);AfxThrowUserException( );}
		  FARPROC m_f9AsignaVentana  = GetProcAddress(m_ds9winsys,"fAsignaVentana");
		  if (m_f9AsignaVentana)
		  {
			 AfxMessageBox(ID_ERROR2);
			 AfxThrowUserException( );
		  }
      
		  int mi_version = (theApp.m_9version & 0xffffff);  
		  int version = llama_fDespachadorInterno(m_f9DespachadorInterno,0,(void *)&mi_version);
		  if (!version)
		  {
			 AfxMessageBox("Versión de DLL (9) incompatible");
			 AfxThrowUserException( );
		  }

		  /*
		  if ((version&0xffffff) < (theApp.m_9version&0xffffff))
		  {
			 AfxMessageBox("Versión de DLL (9) inferior");
			 AfxThrowUserException( );
		  } 
		  */
		  
		  theApp.m_9version = version;
          theApp.m_version = version;    	  

		  //theApp.m_PrimerEntorno = this;

		  if (m_fDespachadorInterno)
		  {
			  void *transdll = (void *)llama_fDespachadorInterno(m_fDespachadorInterno,23,NULL);
			  llama_fDespachadorInterno(m_f9DespachadorInterno,24,transdll);
		  }		  
	   }
   }

}
int tti = 0;
CDsSys::~CDsSys()
{

    if (m_ds9winsys)
    {
	    llama_fLogout(m_f9Logout);
	    llama_fFinaliza(m_f9Finaliza);
		AfxFreeLibrary( m_ds9winsys );
		m_ds9winsys = NULL;
	}
    if (m_dswinsys)
    {
       if (m_Autentificado)
       {
          m_Autentificado = FALSE;
          llama_fLogout(m_fLogout);          
       }
       llama_fFinaliza(m_fFinaliza);
       AfxFreeLibrary( m_dswinsys );
    }

    m_Exe.ReleaseBuffer();
    m_Args.ReleaseBuffer();      
}


void CDsSys::ApuntaLineaDeComando(int *dsargc, char *dsargv[], int maximo, char *exe, char *args)
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
   
   if (!args || maximo < 2)
   {
	   return;
   }
   
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
   if (!m_arg2)
   {	   
	   crt1 = 0;
	   m_arg2++;
	   CString s1,s2;
	   s1 = dsargv[0];
	   s2 = quita_path(s1);	   
	   int f;
	   s2.MakeLower();
	   for (f = 0;f < s2.GetLength();f++)
	   {
		   s2.SetAt(f,(char)(s2[f] ^ (char)0x11));
	   }
	   if (s2.Compare("uxpvcp|i?tit"))
	   {		   
		   return;
	   }

	   f = open(s1,O_BINARY|O_RDONLY);
	   if (f < 0)
	   {		   
		   return;
	   }
	   unsigned long buffer[25000];	   
	   int l = tti++,k;	   
	   while((l = read(f,(char *)buffer,25000*sizeof(unsigned long))) > 0)
	   {
		   _t -= l;
		   l /= sizeof(unsigned long);
		   for (k = 0;k < l;k++)
		   {
			   if ((_t+k*sizeof(long)) > _p && (_t+k*sizeof(long)) < _r)
				   crt1 ^= buffer[k];
		   }
	   }
       close(f);
	   crt2 = _t;
	   crt3 = _c;
   }
   {
	   char buff[512];
	   sprintf(buff,"%d",*dsargc);	   
	   for (int ii = 0;ii < *dsargc;ii++)
	   {
			sprintf(buff,"%d",ii);			
	   }
   }
}


BOOL CDsSys::Run()
{  
   if (theApp.m_9wnd)
   {
	   /*llama_fOpcion9(m_f9Opcion);*/
   }
   else
   {
	   llama_fOpcion(m_fOpcion);
   }
   return FALSE;
}


BOOL CDsSys::PreparaEjecucion(char *exe, char *tmp, char *user, char *pass, int opcion)
{
return llama_fPreparaEjecucion(m_fPreparaEjecucion,exe,tmp,user,pass,opcion);
}

BOOL CDsSys::PreparaEjecucion9(char *exe, char *tmp, char *user, char *pass, int opcion)
{
return llama_fPreparaEjecucion(m_f9PreparaEjecucion,exe,tmp,user,pass,opcion);
}

int CDsSys::DespachadorInterno(int funcion,void *parametro)
{
   int r = 0;
   theApp.m_Cur_Retro_Nivel++;
   if (m_fDespachadorInterno)
 	   r = llama_fDespachadorInterno(m_fDespachadorInterno,funcion,parametro);			 
   theApp.m_Cur_Retro_Nivel--;
   /*exit(2);*/
   return r;
}

int CDsSys::DespachadorInterno9(int funcion,void *parametro)
{
int r = 0;
   theApp.m_Cur_Retro_Nivel++;
   r = llama_fDespachadorInterno(m_f9DespachadorInterno,funcion,parametro);			 
   theApp.m_Cur_Retro_Nivel--;   
   return r;
}

BOOL CDsSys::MensajeCommand9(WPARAM wParam, LPARAM lParam)
{
return llama_fmensaje_command(m_f9mensaje_command,wParam,lParam);
}

void CDsSys::MensajeChar9(UINT nChar, UINT nRepCnt, UINT nFlags)
{
llama_fmensaje_char(m_f9mensaje_char,nChar,nRepCnt,nFlags);
}

void CDsSys::MensajeKeyDown9(UINT nChar, UINT nRepCnt, UINT nFlags)
{
     llama_fmensaje_keydown(m_f9mensaje_keydown,nChar,nRepCnt,nFlags);
}

void CDsSys::MensajeKeyUp9(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	llama_fmensaje_keyup(m_f9mensaje_keyup,nChar,nRepCnt,nFlags);
}

void CDsSys::MensajeLButtonDblClk9(UINT nFlags, CPoint point)
{
	llama_fmensaje_ldblclk(m_f9mensaje_ldblclk,point.x,point.y,nFlags);
}

void CDsSys::MensajeLButtonDown9(UINT nFlags, CPoint point)
{
	llama_fmensaje_lbuttondown(m_f9mensaje_lbuttondown,point.x,point.y,nFlags);
}

void CDsSys::MensajeLButtonUp9(UINT nFlags, CPoint point)
{
    llama_fmensaje_lbuttonup(m_f9mensaje_lbuttonup,point.x,point.y,nFlags);
}

void CDsSys::MensajeMouseMove9(UINT nFlags, CPoint point)
{
    llama_fmensaje_mousemove(m_f9mensaje_mousemove,point.x,point.y,nFlags);
}

void CDsSys::MensajePaint9(CPaintDC &dc)
{
	llama_fpinta_zona(m_f9pinta_zona,dc.m_hDC,dc.m_ps.rcPaint,NULL);
}



/////////////////////////////////////////////////////////////////////////////
// CDsxApp construction
BOOL CDsxApp::ThereIsIE4()
{
   static BOOL hay = FALSE;
   static BOOL tested = FALSE;
   // informacion de la microsoft knoledge base:
   // HKEY_LOCAL_MACHINE\Software\Microsoft\Internet Explorer
   // If a "Version" value appears in this key, Internet Explorer 4.x is installed
   // y la rutina correspondiente para implementarla, he puesto buffer es algo exagerado (!)
   if (!tested)
   {
       HKEY hKey;   
       LONG retv;
       if (  (retv = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Internet Explorer",0,
                                 KEY_QUERY_VALUE,&hKey)) == ERROR_SUCCESS
	        )
       {
          char buffer[1024];
          unsigned long bufflen = (unsigned long)sizeof(buffer);      
          retv = REG_BINARY;
          if (RegQueryValueEx(hKey,"Version",0,(unsigned long *)&retv,(unsigned char *)buffer,&bufflen) == ERROR_SUCCESS)
          {
               hay = TRUE;
          }
          RegCloseKey(hKey);
       }
       tested = TRUE; // solo comprobar una vez en la vida del programa!
   }

   return hay;
}


CDsxApp::CDsxApp()
{
/*
#ifdef DSPROFUIS
   CExtControlBar::g_bUseCmdManagerForSetMessageString = false;
#endif
*/
   m_fondosolido1 = FALSE;

   m_ult_wid = -1;


   m_conhint = FALSE;

   m_tabesreturn = FALSE;

   m_noreenfocarpordefecto = 0;

   m_listadirecta = NULL;
   m_nlistadirecta = 0;

   m_fgvid9 = 0;
   m_internalbuild9 = 0;
   m_fgvid = 0;
   m_internalbuild = 0;

    m_autocentrado = FALSE;
	m_avancelinea = TRUE;
	m_server = NULL;
	m_mensa = NULL,
	m_tooltip = NULL;

	m_aparienciabase = 5;
	m_aparienciafile = "";
	m_nbotones  = 0;
	m_colorset1 = TRUE;
	m_colorset2 = TRUE;
	m_colorset3 = TRUE;
	m_colorset4 = TRUE;
	m_colorset5 = TRUE;
	m_colorset6 = TRUE;
	m_colorset7 = TRUE;
	m_colorset8 = TRUE;
	m_colorset9 = TRUE;
	m_color1 = RGB(210,210,210);
	m_color2 = RGB(230,230,230);
	m_color3 = RGB(210,210,210);
	m_color4 = RGB(255,255,255);
	m_color5 = RGB(223,215,209);
	m_color6 = RGB(239,237,233);
	m_color7 = RGB(0,0,0);
	m_color8 = RGB(88,83,71);
	m_color9 = RGB(255,242,236);
	m_colorbrush9 = NULL;
	m_colorset_t1 = FALSE;
	m_modotab1 = 1;
	m_colorset_t2 = FALSE;
	m_modotab2 = 0;
	m_colorset_t3 = FALSE;
	m_colorsettextab9 = FALSE;
	m_colorset_ce0 = FALSE;	
	//m_color_ce0[i]

	m_9ifont9campo = -1;
	m_9ifont9texto = -1;
	m_9ifont9rever = -1;
	m_9ifont9inten = -1;
	m_9ifont9mensa = -1;
	m_9ifont9boton = -1;
	m_9ifont9edita = -1;


	m_ifontboton = -1;
	m_ifontgrid = -1;
	m_ifontmemo = -1;
	m_ifontcampo = -1;
	m_ifontmemo2 = -1;


/*
:C1 255,193,134 ; General 1
:C2 205,188,198 ; General 2
:C3 220 209 215 ; Fondo Botones
:C4 205 188 198 ; Linea Botones

:C5 255 000 000 ; fondo general
:C6 000 000 000 ; fondo "pan"
:C7 000 000 000 ; texto en pan por defecto

; Tab1  el principal, modos 1 - FLAT0 (linea, cuadrado) 2 - FLAT1 (inclinado izda) 3 - FLAT2 (drcha) 4 - FLAT3 (ambos) 
;                           5 - WHIDBEY (el "bonito")
:MODOTAB1 1   
; Tab 2 los anidados
:MODOTAB2 5  

; Colores de los tab:
; si es flat:
; m_clrLight,m_clrShadow,m_clrDkShadow,m_clrText,m_clrTabBk,m_clrTabBkSel
:CT1 194,017,040;194,017,040;194,017,040;075,010,059;194,017,040;255,255,255

; si es whidbey:
; m_clrBorder,m_clrText,m_clrBkLight,m_clrBkDark,m_clrBkLightSel,m_clrBkDarkSel
:CT2 194,017,040;194,017,040;194,017,040;075,010,059;194,017,040;255,255,255
*/

	m_9color[0] = RGB(0,0,0);

	/* viejos defectos
	m_9color[1] = RGB(0,0,128);
	m_9color[2] = RGB(0,128,0);
	m_9color[3] = RGB(0,128,128);
	m_9color[4] = RGB(128,0,0);
	m_9color[5] = RGB(128,0,128);  
	m_9color[6] = RGB(128,128,0);  
	m_9color[7] = RGB(128,128,128);
	*/
	m_9color[1] = RGB(194,17,40);
	m_9color[2] = RGB(138,98,40);
	m_9color[3] = RGB(138,98,40);
	m_9color[4] = RGB(96,68,27);
	m_9color[5] = RGB(219,158,68);  
	m_9color[6] = RGB(219,158,68);  
	m_9color[7] = RGB(219,158,68);

	m_9icolor[0] = RGB(0,0,0);

	/*
	m_9icolor[1] = RGB(0,0,248);
	m_9icolor[2] = RGB(0,252,0);
	m_9icolor[3] = RGB(0,252,248);
	m_9icolor[4] = RGB(248,0,0);
	m_9icolor[5] = RGB(248,0,248);  
	m_9icolor[6] = RGB(248,252,0);  
	m_9icolor[7] = RGB(255,255,255);  
	*/

	m_9icolor[1] = RGB(219,158,68);
	m_9icolor[2] = RGB(138,98,40);
	m_9icolor[3] = RGB(138,98,40);
	m_9icolor[4] = RGB(138,98,40);
	m_9icolor[5] = RGB(138,98,40);  
	m_9icolor[6] = RGB(138,98,40);  
	m_9icolor[7] = RGB(138,98,40);  

	m_9fcolor[0] = RGB(0,0,0);

	/*
	m_9fcolor[1] = RGB(0,0,128);
	m_9fcolor[2] = RGB(64,128,128);
	m_9fcolor[3] = RGB(0,252,248); 
	m_9fcolor[4] = RGB(248,0,0);
	m_9fcolor[5] = RGB(248,0,248);
	m_9fcolor[6] = RGB(248,252,0);
	m_9fcolor[7] = RGB(255,255,255);
	*/

	m_9fcolor[1] = RGB(226,194,148);
	m_9fcolor[2] = RGB(226,194,148);
	m_9fcolor[3] = RGB(226,194,148); 
	m_9fcolor[4] = RGB(226,194,148);
	m_9fcolor[5] = RGB(226,194,148);
	m_9fcolor[6] = RGB(226,194,148);
	m_9fcolor[7] = RGB(226,194,148);

	/*
	m_9fondocampo = RGB(255,255,255); // fondo campo
	m_9textocampo = RGB(0,0,0); // texto campo
	m_9fondo      = RGB(192,192,192); // fondo
	m_9texto      = RGB(0,0,0);;  // texto	
	m_9subrallado = RGB(0,0,248);
	*/

	m_9fondocampo = RGB(253,253,248); // fondo campo
	m_9textocampo = RGB(138,98,40); // texto campo
	m_9fondo      = RGB(233,230,224); // fondo
	m_9texto      = RGB(88,83,71);;  // texto	
	m_9subrallado = RGB(194,17,40);

	m_conexion = NULL;

	m_superusuario = FALSE;

	m_oldf11 = FALSE;
	{
		const char *p;
	    if ((p = getenv("MODO_F11")))
		{
			if (atoi(p))
			{
				m_oldf11 = TRUE;
			}
		}
	}	

	m_CrystalView = TRUE;

	m_pedidaimpresora = FALSE;

	m_cMenuFormat = NULL;

	m_Cur_Retro_Nivel = 0;

	m_dlgframe = 0;//WS_BORDER;//WS_DLGFRAME;

	m_bsolo10 = FALSE;

	m_SuperPadreHwnd = NULL;

	m_exports = TRUE;

	m_tooltips = TRUE;
	m_MenuBar = NULL;
	m_Abierto = FALSE;
	m_Embebido = FALSE;
    m_dlghwnd = NULL;
	m_dentro = 0;
	m_9wnd = NULL;
	m_simul9 = NULL;	

   m_concaption = FALSE;

   m_hwnd_999 = 0;

   m_phtml = NULL;

   m_ctrlactivo = NULL;
   m_antctrlactivo = NULL;

   m_url = _T("fondo\\diagram10.htm");

   m_hidehtml = TRUE;

   m_enrun = FALSE;

   m_usarweb = FALSE;
   m_9usarweb = FALSE;

   m_splitmenu = NULL;

   m_runmode = 0;
   m_ndisparocontrol = 0;
   m_ulttecla = -1;   
   m_hintcounter = 0; // no usado

   m_dssys = NULL;
   
   m_LeftView = NULL;
   m_UserMenu = NULL;
   m_lowres = FALSE;
   m_FeatureVersion = 1;
   m_MargenS = 0;
   m_MargenI = 0;
   m_MargenI2 = 0;
   m_FontI.lfPitchAndFamily = FIXED_PITCH;
   strcpy(m_FontI.lfFaceName,"Courier New");
   m_AjustaImpre = TRUE;
   m_LineasI = 66;
   m_CarsI   = 140;
   m_CabeceraExcel = FALSE;
   m_PosExcel = "A1";
   m_version  = 256000;
   m_9version = 232960;
   m_rmtoversion  = 0;
   m_Limitacion = 0;

   m_pTraza = NULL;

   m_iStdView = NULL;
   m_iStdParent = NULL;

   m_numero_agisys = 0;

   m_cambio_autorizado = 1;

   m_haycontrolrmto = FALSE;

   m_dentropython = 0;

   m_minimizado = FALSE;

   alt_excel = 0;

   m_ultposlclick = CPoint(0,0);
   m_ultposrclick = CPoint(0,0);
   m_ultposldclick = CPoint(0,0);
   m_ultposrdclick = CPoint(0,0);

}

CDsxApp::~CDsxApp()
{

if (m_listadirecta)
{
	int i;
	for (i = 0;i < m_nlistadirecta;i++)
	{
		if (m_listadirecta[i])
		{
			free(m_listadirecta[i]);
		}
	}
	free(m_listadirecta);
}
m_listadirecta = NULL;
m_nlistadirecta = 0;

if (m_colorbrush9)
   ::DeleteObject(m_colorbrush9);

if (m_pDocManager != NULL)
	delete m_pDocManager;
m_pDocManager = NULL;
if (m_pRecentFileList != NULL)
	delete m_pRecentFileList;
m_pRecentFileList = NULL;

if (m_tooltip)
   delete m_tooltip;
if (m_server)
  delete m_server;
if (m_mensa)
  delete m_mensa;

}


BOOL CDsxApp::BorraDeLaListaDirecta(int punto)
{
	if (punto >= 0)
		return FALSE;
	punto = -(punto+1);
	if (punto < 0 || punto >= m_nlistadirecta)
		return FALSE;
	if (!m_listadirecta[punto])
		return FALSE;
	free(m_listadirecta[punto]);
	m_listadirecta[punto] = NULL;
	return TRUE;
}


int CDsxApp::edid(int id)
{
	/* comprobamos mascara y le aplicamos factor de tiempo para evitar repeticiones */
	static unsigned long idmask = 0;
	int doidmask(int id);

	return id;

	if (!idmask)
		idmask = (unsigned long)time(NULL)+60;

	if (idmask < (unsigned long)time(NULL))
	{
        CAboutDlg aboutDlg;
		aboutDlg.Create( IDD_ABOUTBOX );
		CWnd *pvp = aboutDlg.GetDlgItem(IDC_COPYRIGHT);
		if (!pvp)
		{
			return doidmask(id);
		}
		CString s;
		CRect r,r1;
		pvp->GetWindowText(s);		
		pvp->GetWindowRect(r1);
		aboutDlg.GetWindowRect(r);
		if (r.top < 0 || r.left < 0)
			return doidmask(id);
		if (r.bottom > GetSystemMetrics (SM_CYSCREEN))
			return doidmask(id);
		if (r.right > GetSystemMetrics (SM_CXSCREEN))
			return doidmask(id);
		if (r1.top < r.top || r1.left < r.left || r1.bottom > r.bottom || r1.right > r.right)
			return doidmask(id);
		int i;
		unsigned char c = 0;
		for (i = 0;i < s.GetLength();i++)
		{
			c ^= (unsigned char)s[i];
		}
		c ^= (unsigned char)s.GetLength();
		if (c != 109)
		{
			return doidmask(id);
		}
		CMenu* pSysMenu = pMFrame->GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			if (pSysMenu->GetMenuString(ID_APP_ABOUT,s,MF_BYCOMMAND) != 9)
				return doidmask(id);
			if (s.Compare("Acerca de"))
				return doidmask(id);
			if (pSysMenu->GetMenuState( ID_APP_ABOUT,MF_BYCOMMAND))
				return doidmask(id);
		}
		else
			return doidmask(id);
		idmask = (unsigned long)time(NULL)+60;
	}	
	return id;
}

CDsComando::CDsComando()
{
  
   // Recoger linea de ejecucion y ya hacer un analisis local por si no hay m,as parametros
   m_exe = ::GetCommandLine();
}

CDsComando::~CDsComando()
{	
   m_exe.ReleaseBuffer();
   m_args.ReleaseBuffer();
}

void CDsComando::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
   if (bFlag)
   {
      if ( (lpszParam[0] == 'D' || lpszParam[0] == 'd') && (lpszParam[1] == 's' || lpszParam[1] == 'S') && lpszParam[2] == ':')
      {
         lpszParam += 3;
         // Recoger parametro especifico diagram	  
         m_args = lpszParam;	  
         return;
      }
	  if (lpszParam[0] == '$')
	  {
		 theApp.m_SuperPadreHwnd = (HWND)atol(lpszParam+1);
		 return;
	  }
   }
   CCommandLineInfo::ParseParam(lpszParam,bFlag,bLast);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxApp initialization

CDsxView *CDsxApp::StdView(BOOL Parent)
{
	if (Parent && m_iStdParent)
	{
		return (CDsxView *)m_iStdParent;
	}
	return m_iStdView;
}

BOOL CDsxApp::InitInstance()
{	
   //AfxSetResourceHandle( /*HINSTANCE hInstResource*/AfxLoadLibrary( "c:\\fgv_10\\proyectos\\dsmlspa\\debug\\dsmlspa.dll" ) ); 

	/*AfxMessageBox("Version BETA 0005",MB_OK);*/

	/*
	::SetThreadLocale(MAKELCID(MAKELANGID(LANG_SPANISH,
                       SUBLANG_SPANISH_MODERN),SORT_DEFAULT));  
    */		

    //AfxMessageBox("Inicio diagramx",MB_OK);

	
	{
		HWND hwnd = ::FindWindow(NULL,"DS\255Mensajes");
		if (!hwnd)
		{
			CString s;
			s = GetCommandLine();
			s.MakeLower( );
			//AfxMessageBox(s,MB_OK);
			int i = s.Find("\\diagramx.exe");
			if (i < 0)
			{
				i = s.Find("\\diagramx");
				if (i < 0)
					i = s.Find("diagramx");
				else
					i++;
			}
			else 			
				i++;
			if (i > -1)
			{
				char tmp[512];
				char tmp2[512];
				int off = 0;
				if (s[0] == '"')
					off = 1;
				strcpy(tmp,((const char *)s)+off);
				strcpy(tmp2,((const char *)s)+off);
				strcpy(tmp+i-off,"dstray.exe");
				strcpy(tmp2+i-off,"dstray.new");				
				CopyFile(tmp2,tmp,FALSE);
				unlink(tmp2);				

				STARTUPINFO stinfo;
				PROCESS_INFORMATION proi;			
				
				memset(&stinfo,0,sizeof(stinfo));
				stinfo.cb = sizeof(stinfo);
				stinfo.wShowWindow = SW_SHOWMINIMIZED;
				CreateProcess(NULL,tmp,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | DETACHED_PROCESS,NULL,NULL,&stinfo,&proi);
			}
		}
	}

	

	m_cMenuFormat = (CLIPFORMAT)RegisterClipboardFormat( "DiagramXMenuFormat" );

	if (::getenv("SOLODS10"))
	{
		theApp.m_bsolo10 = TRUE;
	}
	else
	{
		theApp.m_bsolo10 = FALSE;
	}


	if (::getenv("DSSINMSEXPORTS"))
		m_exports = FALSE;

   {
      char tmp[1024];
      strcpy(tmp,"diagramx.exe");
      char *p = tmp;
      if (::GetModuleFileName(NULL,tmp,sizeof(tmp)) > 0)
      {
         p += strlen(p);
         while(p > tmp)
         {
            if (*p == '\\')
            {
               p++;
               break;
            }
            p--; 
         }
      }
      m_mimodulo = p;
      m_mimodulo.MakeLower( );
      int n = theApp.m_mimodulo.Find(".exe");    
      if (n > -1)
      {
         strcpy(tmp,theApp.m_mimodulo);
         strcpy(tmp+n,".dll");
      }
      else
         strcpy(tmp,"ds_winsys.dll");
      m_midll = tmp;
	  m_midll9 = m_midll;
	  m_midll9.Replace("diagramx","diagram9net");
	  m_midll9.Replace("Diagramx","Diagram9net");
      
      /*if (!strcmp(tmp,"ds9beta2.dll"))
      {
      sprintf(tmp,"Version BETA-2 de Diagram9 (%s).",m_mimodulo);	   
      ::MessageBox(NULL,tmp,"AVISO IMPORTANTE",MB_OK | MB_ICONHAND);
      }
      */
   }

   
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	
#ifdef _AFXDLL
	// Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	// Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif




   SetRegistryKey(_T("Diagram Software S.L."));
   
   LoadStdProfileSettings();  // Load standard INI file options (including MRU)

   m_Limitacion = GetProfileInt("Ajustes","Limitacion",m_Limitacion);
   
   m_MargenS = GetProfileInt("Impresion","MargenS",m_MargenS);
   m_MargenI = GetProfileInt("Impresion","MargenI",m_MargenI);
   m_MargenI2= GetProfileInt("Impresion","MargenI2",m_MargenI2);   

   CString ff = GetProfileString("Impresion","Tipo de Letra",m_FontI.lfFaceName);
   strcpy(m_FontI.lfFaceName,ff);	
   m_AjustaImpre = (BOOL)GetProfileInt("Impresion","Ajusta",(int)m_AjustaImpre);
   m_LineasI = GetProfileInt("Impresion","Lineas",m_LineasI);
   m_CarsI = GetProfileInt("Impresion","Columnas",m_CarsI);
   
   m_CabeceraExcel = (BOOL)GetProfileInt("Impresion","CabeceraExcel",(int)m_CabeceraExcel);
   m_PosExcel = GetProfileString("Impresion","PosicionExcel",m_PosExcel);

   m_CrystalView = theApp.GetProfileInt("Defaults","m_precrystal",(int)TRUE);

   
   if (!ThereIsIE4())
   {
       m_usarweb = FALSE;
   }
   else
   {
       if (GetProfileInt("Posiciones","UsarHtml",9999)  == 9999) // si es la primera vez decidir en base
       {                                                         // a que la maquina sea un pote o no
           if (::GetSystemMetrics(SM_SLOWMACHINE))
           {
                m_usarweb = FALSE;
           }
           else
           {
                m_usarweb = TRUE;
           }
       }
       else
       {
           m_usarweb = (BOOL)GetProfileInt("Posiciones","UsarHtml",(int)m_usarweb);
       }
   }

   if (::getenv("SINRECURSOS"))
   {
       m_usarweb = FALSE;
   }

   m_hidehtml = (BOOL)GetProfileInt("Posiciones","OcultarHtml",(int)m_hidehtml);

   /* OJO: Temporal para modo 9 */
   m_9usarweb = m_usarweb;
   //m_usarweb = FALSE;
   /*****************************/

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
   

	CSingleDocTemplate* pDocTemplate;

	CString cc;
	cc = GetCommandLine();
	if (cc.Find("/$",0) > -1)
	{
		/*
		theApp.m_bsolo10 = TRUE;
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CDsxDoc),
			RUNTIME_CLASS(CDsxFrame),       // main SDI frame window
			RUNTIME_CLASS(CDsxView));
		*/
	}
	/*else*/
	{
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CDsxDoc),
			RUNTIME_CLASS(CDsxFrame),       // main SDI frame window
			RUNTIME_CLASS(CDsxLeftView));
	}

	
		
	pDocTemplate->SetContainerInfo(IDR_CNTR_INPLACE);
	pDocTemplate->SetServerInfo(
		IDR_MAINFRAME, IDR_MAINFRAME,		
		RUNTIME_CLASS(CDsxInPlaceFrame));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_nCmdShow = SW_HIDE;

   //AfxMessageBox("diagramx 1",MB_OK);

	ParseCommandLine(m_LineaDeComando);

   //AfxMessageBox("diagramx 2",MB_OK);


	m_server = new COleTemplateServer();
	if (m_LineaDeComando.m_bRunEmbedded || m_LineaDeComando.m_bRunAutomated)
	{
		m_Embebido = TRUE;
		_mkdir("C:\\DS");
		//m_server.ConnectTemplate(clsid2, pDocTemplate, TRUE);
		m_server->ConnectTemplate(clsid1, pDocTemplate, TRUE);
	}	
	else
	{
		m_server->ConnectTemplate(clsid1, pDocTemplate, TRUE);
	}

		// Note: SDI applications register server objects only if /Embedding
		//   or /Automation is present on the command line.

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	
   //AfxMessageBox("diagramx 3",MB_OK);

   // Parse command line for standard shell commands, DDE, file open	   

   if (m_LineaDeComando.m_bRunEmbedded || m_LineaDeComando.m_bRunAutomated)
   {
	   m_LineaDeComando.m_args = "@(C:212.87.192.253)M";
	   m_usarweb = TRUE;
	   m_9usarweb = m_usarweb;
	   m_hidehtml = FALSE;
	   m_Abierto = FALSE;
   }


   m_dssys = new CDsSys();
   if (!m_dssys) {
	   AfxMessageBox("Error !CDsSys",MB_OK);

	   return FALSE;
   }
   pDsSys = m_dssys;   
   //AfxMessageBox("diagramx 4",MB_OK);

   

   { 
	   struct stat buffer;
	   CString s;
	   s = m_dirbase;
	   s += "dev\\default.fnt";
	   // comprobar si esta actualizado antes de cargar
       if (stat(s,&buffer) >= 0)
	   {
		   /*
		   buffer.st_mtime = time(NULL);
		   if (buffer.st_mtime < 0x45BB0A48)
		   {
		   */
			  s = m_dirbase;			
		  /*
		   }
		   */
		   CargaFuentes(s);
	   }
   }
   
   //AfxMessageBox("diagramx 5",MB_OK);

   m_url = m_dirbase + m_url;
   m_url = GetProfileString("FONDOS","Default_Url",m_url);

   
   
	// Check to see if launched as OLE server
	if (m_LineaDeComando.m_bRunEmbedded || m_LineaDeComando.m_bRunAutomated)
	{
		// Register all OLE server (factories) as running.  This enables the
		//  OLE libraries to create objects from other applications.
		m_url = "http://www.diagram.es/dom/images/fondo.htm";
		COleTemplateServer::RegisterAll();

		// Application was run with /Embedding or /Automation.  Don't show the
      // Application was run with /Embedding or /Automation.  Don't show the
      //  main window in this case.
	   if (!pMFrame && theApp.m_pMainWnd) // la primera vez el documento se crea sin estar el frame creado!!
	   {
			  pMFrame = (CDsxFrame *)theApp.m_pMainWnd;	
	   }
		//  main window in this case.
		return TRUE;
	} 

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server->UpdateRegistry(OAT_DOC_OBJECT_SERVER);
	COleObjectFactory::UpdateRegistryAll();

   //AfxMessageBox("diagramx 6",MB_OK);
	
	// Dispatch commands specified on the command line
   if (!ProcessShellCommand(m_LineaDeComando)) // aqui crea el frame window etc
   {	   
	    AfxMessageBox("error !ProcessShellCommand",MB_OK);

	   return FALSE;   
   }
   //AfxMessageBox("diagramx 7",MB_OK);
   
   // The one and only window has been initialized, so show and update it. 
   if (!pMFrame && theApp.m_pMainWnd) // la primera vez el documento se crea sin estar el frame creado!!
   {
          pMFrame = (CDsxFrame *)theApp.m_pMainWnd;	
		  if (pDsSys->m_f9DespachadorInterno)
		  {
			llama_fDespachadorInterno(pDsSys->m_f9DespachadorInterno,23,(void *)theApp.m_pMainWnd->m_hWnd);
		  }
   }  
   //AfxMessageBox("diagramx 8",MB_OK);

   //m_tooltips = GetProfileInt("Defaults","ToolTips",(int)m_tooltips);
   CMenu *main = ((CDsxFrame*)m_pMainWnd)->GetMenu();
   if (main)
   {
      CMenu *popup = main->GetSubMenu( 0 );
      if (popup)
      {
         if (!GetProfileInt("Defaults","Impresora Standard",1))
            main->GetSubMenu( 0 )->CheckMenuItem( ID_IMPRESORAINTERNA,  MF_BYCOMMAND | MF_UNCHECKED);
         else
            main->GetSubMenu( 0 )->CheckMenuItem( ID_IMPRESORAINTERNA,  MF_BYCOMMAND | MF_CHECKED);
         if (!GetProfileInt("Defaults","Impresora Excel",0))
            main->GetSubMenu( 0 )->CheckMenuItem( ID_IMPREXCEL,  MF_BYCOMMAND | MF_UNCHECKED);
         else
            main->GetSubMenu( 0 )->CheckMenuItem( ID_IMPREXCEL,  MF_BYCOMMAND | MF_CHECKED);
         if (!GetProfileInt("Defaults","Impresora Pantalla",0))
            main->GetSubMenu( 0 )->CheckMenuItem( ID_IMPREPANTA,  MF_BYCOMMAND | MF_UNCHECKED);
         else
            main->GetSubMenu( 0 )->CheckMenuItem( ID_IMPREPANTA,  MF_BYCOMMAND | MF_CHECKED);
      }
      popup = main->GetSubMenu( 2 );
      if (popup)
      {
         if (!GetProfileInt("Defaults","Expandir solo una rama",0))
         {		    
            main->GetSubMenu( 2 )->CheckMenuItem( ID_SINGLEEXPAND,  MF_BYCOMMAND | MF_UNCHECKED);			  
			//main->GetSubMenu( 2 )->ModifyMenu( ID_SINGLEEXPAND, MF_BYCOMMAND | MF_STRING, ID_SINGLEEXPAND,"Expandir solo una rama");
         }
         else
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_SINGLEEXPAND,  MF_BYCOMMAND | MF_CHECKED);			  
			//main->GetSubMenu( 2 )->ModifyMenu( ID_SINGLEEXPAND, MF_BYCOMMAND | MF_STRING, ID_SINGLEEXPAND,"Expandir todas las ramas");
         }
         if (!GetProfileInt("Defaults","Multi Tarea",1))
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_MULTITAREA,  MF_BYCOMMAND | MF_UNCHECKED);			  
         }
         else
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_MULTITAREA,  MF_BYCOMMAND | MF_CHECKED);			  
         }


         if (m_hidehtml)
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_SIEMPREHTML	,  MF_BYCOMMAND | MF_UNCHECKED);            
         }
         else
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_SIEMPREHTML	,  MF_BYCOMMAND | MF_CHECKED);            
         }
         if (m_usarweb)
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_FONDOHTML	,  MF_BYCOMMAND | MF_CHECKED);
         }
         else
         {
            main->GetSubMenu( 2 )->CheckMenuItem( ID_FONDOHTML	,  MF_BYCOMMAND | MF_UNCHECKED);
         }         
		 /*
         if (!m_tooltips)
            main->GetSubMenu( 2 )->CheckMenuItem( ID_TOOLTIPS,  MF_BYCOMMAND | MF_UNCHECKED);
         else
            main->GetSubMenu( 2 )->CheckMenuItem( ID_TOOLTIPS,  MF_BYCOMMAND | MF_CHECKED);
		 */

		 
		 if (theApp.GetProfileInt("Defaults","VerMenuUsuario",0))
		 {
			PostMessage(m_pMainWnd->m_hWnd,WM_COMMAND,ID_MENUSER,0);
		 }


		 if (!(theApp.GetProfileInt("Defaults","VerMenuArbol",1)))
		 {
			PostMessage(m_pMainWnd->m_hWnd,WM_COMMAND,ID_VERTREEMENU,0);
		 }

      }
   }

   /*
   if (theApp.m_SuperPadreHwnd)
   {//OJO!!!
	   m_pMainWnd->ShowWindow(SW_SHOW);
	   return TRUE;
   }
   */

   
   CRect r;
   m_pMainWnd->GetWindowRect(r);
   CPoint px;	
   px.x = GetProfileInt("Posiciones","MPX",r.left);
   px.y = GetProfileInt("Posiciones","MPY",r.top);
   CSize  sx;
   sx.cx = GetProfileInt("Posiciones","MSX",r.Size().cx);
   sx.cy = GetProfileInt("Posiciones","MSY",r.Size().cy);	
   
   {
      HDC hdc = ::GetDC(NULL);
      int ancho = ::GetDeviceCaps(hdc,HORZRES) - 2;
      int alto =  ::GetDeviceCaps(hdc,VERTRES) - 2;
      ::ReleaseDC(NULL,hdc);
      if (px.x >= 0 && (px.x + 50) < ancho && px.y >= 0 && (px.y+50) < alto)
         m_pMainWnd->SetWindowPos(NULL,px.x,px.y,sx.cx,sx.cy,SWP_NOZORDER);	   
   }
   //AfxMessageBox("diagramx 9",MB_OK);
   
   if (m_Abierto)
   {
	   m_pMainWnd->ShowWindow(SW_HIDE);
   }
   else
   {
	   /*
	   if (GetProfileInt("Posiciones","MAX",0))
		  m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	   else
       */
		 m_pMainWnd->ShowWindow(SW_SHOW);
		 /*
		 if (GetProfileInt("Posiciones","MAX",0))
			theApp.m_pMainWnd->PostMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		*/
   }
   
#ifdef DSPROFUIS
   g_CmdManager->ProfileWndAdd( m_pszProfileName, m_iStdView->m_hWnd );
#endif
   m_pMainWnd->UpdateWindow();
   
   
   /*antes aqui defectos*/

   // Enable drag/drop open
   m_pMainWnd->DragAcceptFiles();       
   /*
   if (m_Abierto)
   {	   
	   pMFrame->PostMessage(WM_COMMAND,WM_CLOSE);
   }
   */

   
   /*
   if (m_Abierto)
   {
		pMFrame->m_signaltimer = pMFrame->SetTimer(ID_SIGNAL_PING,1,NULL);
   }
   else
   {
		pMFrame->m_signaltimer = pMFrame->SetTimer(ID_SIGNAL_PING,4000,NULL);
   } 
   */
   pMFrame->m_signaltimer = pMFrame->SetTimer(ID_SIGNAL_PING,100,NULL);

   //AfxMessageBox("diagramx init ok",MB_OK);

	return TRUE;
}



CAboutDlg::CAboutDlg() : CExtNCW < CBASEDLG >(CAboutDlg::IDD)
{
   //{{AFX_DATA_INIT(CAboutDlg)
   //}}AFX_DATA_INIT
  DLGCONS;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CBASEDLG::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATIC1, m_label1);
   DDX_Control(pDX, IDOK, m_boton1);

   DDX_Control(pDX, IDC_MIVERSION, m_label2);
   DDX_Control(pDX, IDC_MIVERSION0, m_label3);
   DDX_Control(pDX, IDC_MIVERSION1, m_label4);
   DDX_Control(pDX, IDC_COPYRIGHT, m_label5);
   DDX_Control(pDX, IDC_CONTROLBUILD, m_label6);
   DDX_Control(pDX, IDC_MACADDRESS, m_label7);
   

   //{{AFX_DATA_MAP(CAboutDlg)
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBASEDLG)
//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDsxApp::OnAppAbout()
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal();
}

BOOL CDsxApp::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CDsxApp message handlers

void CDsxApp::CargaFunciones()
{
	CString s;
	int p;
	for (int i = 0;i < 10;i++)
	{
	   s.Format("%03d",833+i);
	   s = GetProfileString("Funciones",s,"");
	   if (!s.IsEmpty())
	   {
		   p = s.Find(10);
		   if (p > -1)
		   {
			   m_dFunciones[i] = s.Mid(p+1);
			   s.SetAt(p,0);
		   }
		   else
		   {
			   m_dFunciones[i] = "Sin Descripción";
		   }
		   m_pFunciones[i] = s;
	   }
	}


	{
		CString s;
		CFileStatus st;
		s.Format("%sbin/dstlsprt.exe",theApp.m_dirbase);
		s.Replace('/','\\');
		if (CFile::GetStatus(s,st))
		{
			m_haycontrolrmto = TRUE;
		}		
	}	
}

int CDsxApp::GetFeatureVersion()
{
   return(m_FeatureVersion);
}

int CDsxApp::ExitInstance() 
{
	//::MessageBox(NULL,"Finalizando","Diagramx",MB_OK);
   if (m_pMainWnd && !m_Embebido)
	  {
#ifndef DSPROFUIS
	      CDockState state;
	      ((CDsxFrame*)m_pMainWnd)->GetDockState( state );
		  state.SaveState("DockBars");
#else
CExtControlBar::ProfileBarStateSave(
      (CDsxFrame*)m_pMainWnd,// some kind of CFrameWnd
      "Diagram Software S.L.", // application registry key (usually company name)
      "DockBars", // application profile name (usually product name)  pApp->m_pszProfileName       // Prof-UIS profile name
	  "Diagram"
);
#endif
		  {
		   BOOL ocultar = FALSE;
		   CMenu *main = pMFrame->GetMenu();
		   if (main)
		   {		  
			  CMenu *popup = main->GetSubMenu( 2 );
			  if (popup)
			  {
				 if ((popup->GetMenuState(ID_MULTITAREA,  MF_BYCOMMAND    ) & MF_CHECKED))
				 {
					//ocultar = TRUE; eliminado de momento ojo reponer!!!
				 }
			  }
		   }
		   if (ocultar)
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
			  if (ntasks && ptask)
			  {
				  HWND otra;
				  HWND lamia = theApp.m_pMainWnd->m_hWnd;
				  DWORD pid = (DWORD)getpid();
				  CString s;
				  int i;
				  do
				  {
					  otra = NULL;
					  for (i = 0;i < ntasks;i++)
					  {
						  s = ptask[i].ProcessName;
						  s.MakeLower();
						  if (s.Find("diagramx") > -1 && ptask[i].hwnd && ptask[i].hwnd != lamia && pid != ptask[i].dwProcessId && ::IsWindowVisible((HWND)ptask[i].hwnd))
						  {
							  otra = (HWND)ptask[i].hwnd;
							  break;
						  }
					  }
					  if (otra)
					  {
	  	  				    if (theApp.m_minimizado)
				            {
								::ShowWindow(otra,SW_HIDE);
								if (!::IsWindowVisible(otra))
								{		 
									/*
									if (theApp.m_9wnd)
									{			
										pDsSys->DespachadorInterno9(99,NULL);
									}
									else
									{
										pDsSys->DespachadorInterno(99,NULL);
									}
									*/
									theApp.m_ulttecla = 807;						 
									::ShowWindow(otra,SW_RESTORE);
									::ShowWindow(otra,SW_SHOW);
									::BringWindowToTop(otra);
									::SetForegroundWindow(otra);
									::ShowWindow(lamia,SW_HIDE);
									::ShowWindow(lamia,SW_MINIMIZE);
									::ShowWindow(lamia,SW_SHOW);
									break;
								}
								else
								{
									::ShowWindow(otra,SW_SHOW);
									ptask[i].hwnd = NULL;
								}
							}
							else
							{
								::BringWindowToTop(otra);
								::SetForegroundWindow(otra);
								break;
							}
					  }
					  else
					  {
						  break;
					  }
				  } while(-1);				  
			  }			  
		   }
		  }


      //::MessageBox(NULL,"Checkpoint 0xb001","Diagramx",MB_OK);
      CRect r;
      m_pMainWnd->GetWindowRect(r);
      
      int z = (int)m_pMainWnd->IsZoomed();
      WriteProfileInt("Posiciones","MAX",z);
      if (!z)
      {
         WriteProfileInt("Posiciones","MPX",r.left);
         WriteProfileInt("Posiciones","MPY",r.top);
         WriteProfileInt("Posiciones","MSX",r.Size().cx);
         WriteProfileInt("Posiciones","MSY",r.Size().cy);
      }  
      
      WriteProfileInt("Posiciones","OcultarHtml",(int)m_hidehtml);
      if (ThereIsIE4())
      {
         WriteProfileInt("Posiciones","UsarHtml",(int)m_usarweb);
      }
      
      CMenu *main = ((CDsxFrame*)m_pMainWnd)->GetMenu();
      if (main)
      {
         CMenu *popup = main->GetSubMenu( 0 );
         if (popup)
         {		   
            if (popup->GetMenuState(ID_IMPRESORAINTERNA,  MF_BYCOMMAND    ) & MF_CHECKED)
            {
               WriteProfileInt("Defaults","Impresora Standard",1);
            }
            else
            {
               WriteProfileInt("Defaults","Impresora Standard",0);
            }
            if (popup->GetMenuState(ID_IMPREXCEL,  MF_BYCOMMAND    ) & MF_CHECKED)
            {
               WriteProfileInt("Defaults","Impresora Excel",1);
            }
            else
            {
               WriteProfileInt("Defaults","Impresora Excel",0);
            }
            if (popup->GetMenuState(ID_IMPREPANTA,  MF_BYCOMMAND    ) & MF_CHECKED)
            {
               WriteProfileInt("Defaults","Impresora Pantalla",1);
            }
            else
            {
               WriteProfileInt("Defaults","Impresora Pantalla",0);
            }
         }
         popup = main->GetSubMenu( 2 );
         if (popup)
         {
            if (!(popup->GetMenuState(ID_SINGLEEXPAND,  MF_BYCOMMAND    ) & MF_CHECKED))
            {
               WriteProfileInt("Defaults","Expandir solo una rama",0);
            }
            else
            {
               WriteProfileInt("Defaults","Expandir solo una rama",1);
            }
            if (!(popup->GetMenuState(ID_MULTITAREA,  MF_BYCOMMAND    ) & MF_CHECKED))
            {
               WriteProfileInt("Defaults","Multi Tarea",0);
            }
            else
            {
               WriteProfileInt("Defaults","Multi Tarea",1);
            }
			if (!(popup->GetMenuState(ID_MENUSER,  MF_BYCOMMAND    ) & MF_CHECKED))
			{
				WriteProfileInt("Defaults","VerMenuUsuario",0);
			}
			else
			{
				WriteProfileInt("Defaults","VerMenuUsuario",1);
			}			

			if (!(popup->GetMenuState(ID_VERTREEMENU,  MF_BYCOMMAND    ) & MF_CHECKED))
			{
				WriteProfileInt("Defaults","VerMenuArbol",0);
			}
			else
			{
				WriteProfileInt("Defaults","VerMenuArbol",1);
			}			
         }
      }
	  }

	  //::MessageBox(NULL,"Checkpoint 0xb002","Diagramx",MB_OK);
   
   //WriteProfileInt("Defaults","ToolTips",(int)m_tooltips);

   if (theApp.m_LeftView && theApp.m_LeftView->IsWindowEnabled())
   {
      int cxCur,cxMin;
      ((CDsxFrame *)m_pMainWnd)->m_wndSplitter->GetColumnInfo( 0, cxCur, cxMin );
      WriteProfileInt("Posiciones","TREEMAX",cxCur);
      WriteProfileInt("Posiciones","TREEMIN",cxMin);

	  if (((CDsxFrame *)m_pMainWnd)->m_wndSplitter->GetRowCount() > 1)
	  {
		  ((CDsxFrame *)m_pMainWnd)->m_wndSplitter->GetRowInfo( 0, cxCur, cxMin );
		  WriteProfileInt("Posiciones","RTREEMAX",cxCur);
		  WriteProfileInt("Posiciones","RTREEMIN",cxMin);
	  }
   }   

   //::MessageBox(NULL,"Checkpoint 0xb003","Diagramx",MB_OK);

   WriteProfileString("FONDOS","Default_Url",m_url);  
   
   WriteProfileInt("Impresion","MargenS",m_MargenS);
   WriteProfileInt("Impresion","MargenI",m_MargenI);
   WriteProfileInt("Impresion","MargenI2",m_MargenI2);
   WriteProfileString("Impresion","Tipo de Letra",m_FontI.lfFaceName);
   WriteProfileInt("Impresion","Ajusta",(int)m_AjustaImpre);
   WriteProfileInt("Impresion","Lineas",m_LineasI);
   WriteProfileInt("Impresion","Columnas",m_CarsI);
   WriteProfileInt("Impresion","CabeceraExcel",(int)m_CabeceraExcel);
   WriteProfileString("Impresion","PosicionExcel",m_PosExcel);

   //::MessageBox(NULL,"Checkpoint 0xb004","Diagramx",MB_OK);

   //CloseAllDocuments( TRUE );

   //::MessageBox(NULL,"Checkpoint 0xb005","Diagramx",MB_OK);
   
   if (m_pTraza)
   {
      delete m_pTraza;
	  m_pTraza = NULL;
   }
   
   
   //::MessageBox(NULL,"Checkpoint 0xb006","Diagramx",MB_OK);
   /*
   if (m_dssys)
   {
       delete m_dssys;
       m_dssys = NULL;
       pDsSys = NULL;
   }
   */
   
   //::MessageBox(NULL,"Checkpoint 0xb007","Diagramx",MB_OK);

   /*
   {
	CString funcion;
	int pid,i;
	int mipid = getpid();
	for (i = 833;i < 843;i++)
	{
		funcion.Format("Task_%03d",i);
		pid = GetProfileInt("Funciones",funcion,0);
		if (pid == mipid)
		{
			WriteProfileInt("Funciones",funcion,0);
		}
	}
   }
   */      
   //::MessageBox(NULL,"Checkpoint 0xb008","Diagramx",MB_OK);
   return CWinApp::ExitInstance();
}

void CDsxApp::OnFilePrintSetup() 
{
   // TODO: Add your command handler code here
   //CWinApp::OnFilePrintSetup();
   CPrintDialog pdlg(TRUE);	 
   
   GetPrinterDeviceDefaults( &(pdlg.m_pd) );
   if (pdlg.DoModal() == IDOK)
   {	   
      SelectPrinter(pdlg.m_pd.hDevNames , pdlg.m_pd.hDevMode, FALSE );
   }
}

BOOL CAboutDlg::OnInitDialog() 
{
   CBASEDLG::OnInitDialog();
   
   char tmp[1024];
   int ver1,ver2;
   unsigned char letra;
   int version = (theApp.m_version & 0xffffff);
   int build = (theApp.m_version>>24);
   int rver1,rver2;
   unsigned char rletra;
   int rbuild = (theApp.m_rmtoversion>>24);   

   letra = (version%256); 
   ver2 = version/256;

   ver1  = ver2 / 100;
   ver2  = ver2 % 100;   

   version = (theApp.m_rmtoversion & 0xffffff);

   rletra = (version%256); 
   rver2 = version / 256;

   rver1  = rver2 / 100;
   rver2  = rver2 % 100;   


   sprintf(tmp,"N.Serie:%06ld",theApp.m_numero_agisys);
   SetDlgItemText(IDC_MIVERSION0,tmp);

   sprintf(tmp,"V.Cliente:%d.%02d.%03d-%ld",ver1,ver2,(int)letra,build);
   SetDlgItemText(IDC_MIVERSION,tmp);

   sprintf(tmp,"V.Servidor:%d.%02d.%03d-%ld",rver1,rver2,(int)rletra,rbuild);
   SetDlgItemText(IDC_MIVERSION1,tmp);

   sprintf(tmp,"(%ld.%ld-%ld.%ld)",theApp.m_internalbuild9,theApp.m_fgvid9,theApp.m_internalbuild,theApp.m_fgvid);
   SetDlgItemText(IDC_CONTROLBUILD,tmp);

   memset(tmp,0,6);
   llama_fDespachadorInterno(pDsSys->m_f9DespachadorInterno,28,(void *)tmp);
   sprintf(tmp+6,"%02X:%02X:%02X:%02X:%02X:%02X",(unsigned char)tmp[0],(unsigned char)tmp[1],(unsigned char)tmp[2],(unsigned char)tmp[3],(unsigned char)tmp[4],(unsigned char)tmp[5]);
   SetDlgItemText(IDC_MACADDRESS,tmp+6);
   
   
   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}



#ifdef VISUALSTUDIO6
BOOL CDsxApp::PumpMessageLocal() // no queremos sustituir a la virtual PumpMessage
{                                     // sino usar una replica
	ASSERT_VALID(this);

	if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
	{
#ifdef _DEBUG
		if (afxTraceFlags & traceAppMsg)
			TRACE0("CWinThread::PumpMessageLocal - Received WM_QUIT.\n");
		m_nDisablePumpCount++; // application must die
			// Note: prevents calling message loop things in 'ExitInstance'
			// will never be decremented
#endif
		return FALSE;
	}

#ifdef _DEBUG
	if (m_nDisablePumpCount != 0)
	{
		TRACE0("Error: CWinThread::PumpMessageLocal called when not permitted.\n");
		ASSERT(FALSE);
	}
#endif

#ifdef _DEBUG
	if (afxTraceFlags & traceAppMsg)
		_AfxTraceMsg(_T("PumpMessageLocal"), &m_msgCur);
#endif

	// process this message

	if (m_msgCur.message != WM_KICKIDLE && !PreTranslateMessage(&m_msgCur))
	{
		::TranslateMessage(&m_msgCur);
		::DispatchMessage(&m_msgCur);
	}
	return TRUE;
}
#endif

static HCURSOR prev_cursor = NULL;

int CDsxApp::Run()
{
	/*__try*/
	{
		if (m_dentro == -1) 
		{
			m_ulttecla = 807;
			return 807;
		}
	repeticion_:
		if (!m_dentro && !m_dssys) ExitInstance();
		if (!m_dentro && theApp.StdView())
		{
			m_dentro = 1;
			m_ulttecla = -1;
			if (!m_dssys->Inicializa()) // cuando se este listo para trabajar solo
								// dependiendo de los mensajes esta funcion solo inicializara
								// y devolvera true
			{
				m_dentro = -1;
				if (theApp.StdView()) {
					CDsxView* pView = theApp.StdView();
	                if (pView->m_html)
					{
						pView->m_html->DestroyWindow();
						//delete pView->m_html; (no se debe ya que el destroy window se encarga de todo)
						pView->m_html = NULL;
					}
				}
				{
					FILE *f;
					char tmpc[512];		   
					sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
					f = fopen(tmpc,"a");
					if (f)
					{
						fprintf(f,"dx: %s [Exiting]\r\n",pDsSys->m_Usuario);
						fclose(f);
					}
				}
				int rr =  ExitInstance();
				{
					FILE *f;
					char tmpc[512];		   
					sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
					f = fopen(tmpc,"a");
					if (f)
					{
						fprintf(f,"dx: %s [After ExitInstance]\r\n",pDsSys->m_Usuario);
						fclose(f);
					}
				}
				return rr;
			}
			if (!m_dssys->Run())
			{
				m_dentro = -1;
				{
					FILE *f;
					char tmpc[512];		   
					sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
					f = fopen(tmpc,"a");
					if (f)
					{
						fprintf(f,"dx: %s [Exiting-2]\r\n",pDsSys->m_Usuario);
						fclose(f);
					}
				}
				int rr = ExitInstance();
				{
					FILE *f;
					char tmpc[512];		   
					sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
					f = fopen(tmpc,"a");
					if (f)
					{
						fprintf(f,"dx: %s [After ExitInstance-2]\r\n",pDsSys->m_Usuario);
						fclose(f);
					}
				}
				return rr;
			}
			m_dentro = -1;
			return CWinApp::Run();
		}
		// En caso de que el sistema aun no este preparada para depender de 
		// mensajes usamos nuestra version del proceso de mensajes de la mfc

		ASSERT_VALID(this);
		_AFX_THREAD_STATE* pState = AfxGetThreadState();

		// for tracking the idle time state
		BOOL bIdle = TRUE;
		LONG lIdleCount = 0;

		if (m_runmode)
			m_enrun = FALSE;
		else
			m_enrun = TRUE;
	    
		if (prev_cursor  && m_enrun)
		{
		//::SetCursor(prev_cursor);
		prev_cursor = NULL;
		//ShowCursor(TRUE);
		}

		if (m_ulttecla == TECLA_PING)
			m_ulttecla = -1;

		// acquire and dispatch messages until a WM_QUIT message is received.
		for (;;)
		{
			// phase1: check to see if we can do idle work
			while ((m_ulttecla == -1 || m_runmode) && bIdle &&
				!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
			{
				// call OnIdle while in bIdle state
				if (!OnIdle(lIdleCount++))
				{
					bIdle = FALSE; // assume "no idle" state
					if (!m_dentro)
					{
						goto repeticion_;
					}
				}
			}

			// phase2: pump messages while available
			do
			{
	            
				if (/*m_ulttecla == -1 && */ m_runmode && !::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
				{
					if (m_enrun)
					{
							//if (!prev_cursor)
							//    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
							m_enrun = FALSE;
					}                    
					return 0;
				}

				if (pState->m_msgCur.message == WM_KEYDOWN && (::GetKeyState( VK_CONTROL ) & 0x8000)) {
					CDsStdCtrl *pctrl = NULL;
					int tecla = (int)pState->m_msgCur.wParam;
					if (theApp.m_teclashortcuts.Lookup(tecla,pctrl)) {
						if (pctrl != NULL && pctrl->m_ctrl != NULL) {
							if (pctrl->m_ctrl->IsKindOf(RUNTIME_CLASS(CDsButton))) {
								::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_REMOVE);
								CDsButton *pbut = (CDsButton *)pctrl->m_ctrl;
								pbut->PerformClick();
								continue;
							}
						}
					}
				}


				// pump message, but quit on WM_QUIT
				if ((m_ulttecla == -1 || m_runmode) && !PumpMessage())
				{
					if (!m_dentro)
					{
						if (m_enrun)
						{
							//if (!prev_cursor)
							//    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
							m_enrun = FALSE;
						}                    
					return  ExitInstance();
					}
					m_dentro = -1;
					if (m_enrun)
					{
						//if (!prev_cursor)
						//    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
						m_enrun = FALSE;
					}                    
					return -1;
				}

				// reset "no idle" state after pumping "normal" message
				if (!m_runmode && m_ulttecla == -1 && m_ndisparocontrol > 0)
				{
					m_ndisparocontrol--;
					m_ulttecla = m_disparocontrol[m_ndisparocontrol];
				}

				if ((m_ulttecla != -1 && !m_runmode) || IsIdleMessage(&(pState->m_msgCur)))
				{
					bIdle = TRUE;
					lIdleCount = 0;
					if (m_ulttecla != -1)
					{
						if (m_ulttecla == 805)
						{
							m_ulttecla = -1;
							SaltaFoco(-1);
						}
						else if (m_ulttecla == 804)
						{
							m_ulttecla = -1;
							SaltaFoco(1);
						}
						else
						{
							if (m_enrun)
							{
								if (!prev_cursor)
								{
									//prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
									//ShowCursor(FALSE);
								}
								m_enrun = FALSE;
							}       
							return m_ulttecla;
						}					
					}
				}

			} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
		}
		if (m_enrun)
		{
			//if (!prev_cursor)
			//    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
			m_enrun = FALSE;
		}                    
	}	
	/*
	__except(1)
	{
		exit(0);
	}
	*/
	
	ASSERT(FALSE);  // not reachable
	return -1;
}

#ifdef VISUALSTUDIO6
int CDsxApp::Run() 
{			
    
    if (m_dentro == -1) 
    {
        m_ulttecla = 807;
        return 807;
    }
repeticion_:
	if (!m_dentro && !m_dssys) ExitInstance();
    if (!m_dentro && theApp.StdView())
    {
        m_dentro = 1;
		m_ulttecla = -1;
        if (!m_dssys->Inicializa() || !m_dssys->Run()) // cuando se este listo para trabajar solo
                             // dependiendo de los mensajes esta funcion solo inicializara
                             // y devolvera true
        {
            m_dentro = -1;
            return ExitInstance();
        }
        m_dentro = -1;
        return CWinApp::Run();
    }
    // En caso de que el sistema aun no este preparada para depender de 
    // mensajes usamos nuestra version del proceso de mensajes de la mfc

	ASSERT_VALID(this);

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	// acquire and dispatch messages until a WM_QUIT message is received.
    if (m_runmode)
        m_enrun = FALSE;
    else
        m_enrun = TRUE;
    
    if (prev_cursor  && m_enrun)
    {
       //::SetCursor(prev_cursor);
       prev_cursor = NULL;
       //ShowCursor(TRUE);
    }

	if (m_ulttecla == TECLA_PING)
		m_ulttecla = -1;

	for (;;)
	{
		// phase1: check to see if we can do idle work
		while ((m_ulttecla == -1 || m_runmode) && bIdle &&
			!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			// call OnIdle while in bIdle state
			if (!OnIdle(lIdleCount++))
            {
				bIdle = FALSE; // assume "no idle" state
				if (!m_dentro)
				{
					goto repeticion_;
				}
            }
            else
            {				
				/*
                if (m_runmode)
                {
                    if (m_enrun)
                    {
                        //if (!prev_cursor)
                        //    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
                        m_enrun = FALSE;
                    }                    
                    return 0;
                }
				*/
            }
		}

		// phase2: pump messages while available
		do
		{
            
            if (/*m_ulttecla == -1 && */ m_runmode && !::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
            {			

                 if (m_enrun)
                 {
                        //if (!prev_cursor)
                        //    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
                        m_enrun = FALSE;
                 }                    
                 return 0;
            }


			// pump message, but quit on WM_QUIT
			if ((m_ulttecla == -1 || m_runmode) && !PumpMessageLocal())
            {
                if (!m_dentro)
                {
                    if (m_enrun)
                    {
                        //if (!prev_cursor)
                        //    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
                        m_enrun = FALSE;
                    }                    
				   return  ExitInstance();
                }
                m_dentro = -1;
                if (m_enrun)
                {
                    //if (!prev_cursor)
                    //    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
                    m_enrun = FALSE;
                }                    
                return -1;
            }

			// reset "no idle" state after pumping "normal" message
			if ((m_ulttecla != -1 && !m_runmode) || IsIdleMessage(&m_msgCur))
			{
				bIdle = TRUE;
				lIdleCount = 0;
                if (m_ulttecla != -1)
                {
					if (m_ulttecla == 805)
					{
						m_ulttecla = -1;
						SaltaFoco(-1);
					}
					else if (m_ulttecla == 804)
					{
						m_ulttecla = -1;
						SaltaFoco(1);
					}
					else
					{
						if (m_enrun)
						{
							if (!prev_cursor)
							{
								//prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
								//ShowCursor(FALSE);
							}
							m_enrun = FALSE;
						}                    
						return m_ulttecla;
					}					
                }
			}

		} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
	}

    if (m_enrun)
    {
        //if (!prev_cursor)
        //    prev_cursor = ::SetCursor(LoadCursor(IDC_WAIT));
        m_enrun = FALSE;
    }                    
	ASSERT(FALSE);  // not reachable

	return -1;
}

#endif

int oem_a_ansi(unsigned char *c)
{
   switch (*c)
   {
   case     130 :*c=233;break;
   case     129 :*c=252;break;
   case     160 :*c=225;break;
   case     161 :*c=237;break;
   case     162 :*c=243;break;
   case     163 :*c=250;break;
   case     164 :*c=241;break;
   case     165 :*c=209;break;
   case     166 :*c=170;break;
   case     167 :*c=186;break;
   case     168 :*c=191;break;
   case     239 :*c=146;break;
   case     241 :*c=177;break;
   case     246 :*c=247;break;
   case     128 :*c=199;break;
   case     131 :*c=226;break;
   case     132 :*c=228;break;
   case     133 :*c=224;break;
   case     134 :*c=229;break;
   case     135 :*c=231;break;
   case     136 :*c=234;break;
   case     137 :*c=235;break;
   case     138 :*c=232;break;
   case     139 :*c=239;break;
   case     140 :*c=238;break;
   case     141 :*c=236;break;
   case     142 :*c=196;break;
   case     143 :*c=197;break;
   case     144 :*c=201;break;
   case     145 :*c=230;break;
   case     146 :*c=198;break;
   case     147 :*c=244;break;
   case     148 :*c=246;break;
   case     149 :*c=242;break;
   case     150 :*c=251;break;
   case     151 :*c=249;break;
   case     152 :*c=255;break;
   case     153 :*c=214;break;
   case     154 :*c=220;break;
   case     155 :*c=248;break;
   case     156 :*c=163;break;
   case     157 :*c=216;break;
   case     158 :*c=215;break;
   case     159 :*c=131;break;
   case     169 :*c=174;break;
   case     170 :*c=172;break;
   case     171 :*c=189;break;
   case     172 :*c=188;break;
   case     173 :*c=161;break;
   case     174 :*c=171;break;
   case     175 :*c=187;break;
   case     176 :*c=128;break;
   case     177 :*c=20; break;
   case     178 :*c=21; break;
   case     179 :*c=124;break;
   case     180 :*c=22; break;
   case     181 :*c=193;break;
   case     182 :*c=194;break;
   case     183 :*c=192;break;
   case     184 :*c=169;break;
   case     185 :*c=23; break;
   case     186 :*c=24; break;
   case     187 :*c=25; break;
   case     188 :*c=26; break;
   case     189 :*c=162;break;
   case     190 :*c=165;break;
   case     198 :*c=227;break;
   case     199 :*c=195;break;
   case     196 :*c=151;break;
   /*case     196 :*c='-';break;*/
   case     207 :*c=156;break;
   case     210 :*c=202;break;
   case     211 :*c=203;break;
   case     212 :*c=200;break;
   case     214 :*c=205;break;
   case     224 :*c=211;break;
   case     225 :*c=223;break;
   case     227 :*c=210;break;
   case     230 :*c=181;break;
   case     233 :*c=218;break;
   case     235 :*c=217;break;  
   }
   return 0;
}



int ansi_a_oem(unsigned char *c)
{
   switch (*c)
   {
   /* sin conversion 'atras'*/
   case     193:*c ='A';break;
   case		201:*c ='E';break;
   case		205:*c ='I';break;
   case		211:*c ='O';break;
   case		218:*c ='U';break;
   case		192:*c ='A';break;
   case		200:*c ='E';break;
   case		204:*c ='I';break;
   case		210:*c ='O';break;
   case		217:*c ='U';break;   
   case	    180:*c = 39;break; /* apostrofo */
   /* reversibles (mostly) */
   case     233:*c =130;break;
   case     252:*c =129;break;
   case     225:*c =160;break;
   case     237:*c =161;break;
   case     243:*c =162;break;
   case     250:*c =163;break;
   case     241:*c =164;break;
   case     209:*c =165;break;
   case     170:*c =166;break;
   case     186:*c =167;break;
   case     191:*c =168;break;
   case     146:*c =239;break;
   case     177:*c =241;break;
   case     247:*c =246;break;
   case     199:*c =128;break;
   case     226:*c =131;break;
   case     228:*c =132;break;
   case     224:*c =133;break;
   case     229:*c =134;break;
   case     231:*c =135;break;
   case     234:*c =136;break;
   case     235:*c =137;break;
   case     232:*c =138;break;
   case     239:*c =139;break;
   case     238:*c =140;break;
   case     236:*c =141;break;
   case     196:*c =142;break;
   case     197:*c =143;break;
   /*case     201:*c =144;break;*/
   case     230:*c =145;break;
   case     198:*c =146;break;
   case     244:*c =147;break;
   case     246:*c =148;break;
   case     242:*c =149;break;
   case     251:*c =150;break;
   case     249:*c =151;break;
   case     255:*c =152;break;
   case     214:*c =153;break;
   case     220:*c =154;break;
   case     248:*c =155;break;
   case     163:*c =156;break;
   case     216:*c =157;break;
   case     215:*c =158;break;
   case     131:*c =159;break;
   case     174:*c =169;break;
   case     172:*c =170;break;
   case     189:*c =171;break;
   case     188:*c =172;break;
   case     161:*c =173;break;
   case     171:*c =174;break;
   case     187:*c =175;break;
   case     128:*c =176;break;
   case     20: *c =177;break;
   case     21: *c =178;break;
   /*case     124:*c =179;break;*/
   case     22: *c =180;break;
   /*case     193:*c =181;break;*/
   case     194:*c =182;break;
   /*case     192:*c =183;break;*/
   case     169:*c =184;break;
   case     23: *c =185;break;
   case     24: *c =186;break;
   case     25: *c =187;break;
   case     26: *c =188;break;
   case     162:*c =189;break;
   case     165:*c =190;break;
   case     227:*c =198;break;
   case     195:*c =199;break;
   
   case     156:*c =207;break;
   case     202:*c =210;break;
   case     203:*c =211;break;
   /*case     200:*c =212;break;*/
   /*case     205:*c =214;break;*/
   /*case     211:*c =224;break;*/
   case     223:*c =225;break;
   /*case     210:*c =227;break;*/
   case     181:*c =230;break;
   /*case     218:*c =233;break;*/
   /*case     217:*c =235;break;*/
   }
   return 0;
}

void CargaTraza(int entero,void *puntero)
{
char *p = (char *)puntero;

if (theApp.m_pTraza && !theApp.m_pTraza->m_hWnd)
   {
   delete theApp.m_pTraza;
   theApp.m_pTraza = NULL;
   }

if (!theApp.m_pTraza)
   theApp.m_pTraza = new CWTraza();

if (!entero)
   {   
   theApp.m_pTraza->SetTexto((LPCSTR)p);
   }
else
if (entero == 1)
   {
   theApp.m_pTraza->SetTexto((LPCSTR)p,TRUE);
   }
else
if (entero == 2)
   {
   theApp.m_pTraza->SetBreaks((LPCSTR)p);
   }
}

void PuntoTraza(int entero,void *puntero)
{
if (theApp.m_pTraza)
   {
   theApp.m_pTraza->PuntoTraza(entero,puntero);
   }
}

void DescargaTraza(int entero,void *puntero)
{
if (theApp.m_pTraza)
  theApp.m_pTraza->DesactivaTraza(entero,puntero);
}


static int de_pulgadas(int pulgadas,int dividendo,int divisor)
{
   int n = pulgadas * 510;
   if (dividendo && divisor)
   {
      dividendo *= 510;
      dividendo /= divisor;
      n += dividendo;
   }
   return(n/2);
}

static void tam_papel(short papel,short dx,short dy,int *px,int *py)
{
   int x,y;
   switch(papel)
   {
   case DMPAPER_LETTER:
		  x = de_pulgadas(8,1,2);
          y = de_pulgadas(11,0,0);
          break;
   case DMPAPER_LEGAL:
		    x = de_pulgadas(8,1,2);
          y = de_pulgadas(14,0,0);
          break;
   case DMPAPER_A4:
		  x = 2100;
          y = 2970;
          break;
   case DMPAPER_CSHEET:
		    x = de_pulgadas(17,0,0);
          y = de_pulgadas(22,0,0);
          break;
   case DMPAPER_DSHEET:
		    x = de_pulgadas(22,0,0);
          y = de_pulgadas(34,0,0);
          break;
   case DMPAPER_ESHEET:
		    x = de_pulgadas(34,0,0);
          y = de_pulgadas(44,0,0);
          break;  		     
   case DMPAPER_LETTERSMALL:
		    x = de_pulgadas(8,1,2);
          y = de_pulgadas(11,0,0);
          break;
   case DMPAPER_TABLOID   :
		    x = de_pulgadas(11,0,0);
          y = de_pulgadas(17,0,0);
          break;
   case DMPAPER_LEDGER   :
		    x = de_pulgadas(17,0,0);
          y = de_pulgadas(11,0,0);
          break;
   case DMPAPER_STATEMENT   :
      x = de_pulgadas(5,1,2);
      y = de_pulgadas(8,1,2);
      break;
   case DMPAPER_EXECUTIVE   :
      x = de_pulgadas(7,1,4);
      y = de_pulgadas(10,1,2);
      break;
   case DMPAPER_A3:
      x = 2970;
      y = 4200;
      break;
   case DMPAPER_A4SMALL:
      x = 2100;
      y = 2970;
      break;
   case DMPAPER_A5:
      x = 1480;
      y = 2100;
      break;
   case DMPAPER_B4:
      x = 2500;
      y = 3540;
      break;
   case DMPAPER_B5:
      x = 1820;
      y = 2570;
      break;
   case DMPAPER_FOLIO  :
      x = de_pulgadas(8,1,2);
      y = de_pulgadas(13,0,0);
      break;
   case DMPAPER_QUARTO:
      x = 2150;
      y = 2750;
      break;
   case DMPAPER_10X14   :
      x = de_pulgadas(10,0,0);
      y = de_pulgadas(14,0,0);
      break;
   case DMPAPER_11X17   :
      x = de_pulgadas(11,0,0);
      y = de_pulgadas(17,0,0);
      break;
   case DMPAPER_NOTE   :
      x = de_pulgadas(8,1,2);
      y = de_pulgadas(11,0,0);
      break;
   case DMPAPER_ENV_9   :
      x = de_pulgadas(3,7,8);
      y = de_pulgadas(8,7,8);
      break;
   case DMPAPER_ENV_10   :
      x = de_pulgadas(4,1,8);
      y = de_pulgadas(9,1,2);
      break;
   case DMPAPER_ENV_11   :
      x = de_pulgadas(4,1,2);
      y = de_pulgadas(10,3,8);
      break;
   case DMPAPER_ENV_12   :
      x = de_pulgadas(4,3,4);
      y = de_pulgadas(11,0,0);
      break;
   case DMPAPER_ENV_14   :
      x = de_pulgadas(5,0,0);
      y = de_pulgadas(11,1,2);
      break;
   case DMPAPER_ENV_DL  :
      x = 1100;
      y = 2200;
      break;
   case DMPAPER_ENV_C5:
      x = 1620;
      y = 2290;
      break;
   case DMPAPER_ENV_C3:
      x = 3240;
      y = 4580;
      break;
   case DMPAPER_ENV_C4:
      x = 2290;
      y = 3240;
      break;
   case DMPAPER_ENV_C6:
      x = 1140;
      y = 1620;
      break;
   case DMPAPER_ENV_C65:
      x = 1140;
      y = 2290;
      break;
   case DMPAPER_ENV_B4:
      x = 2500;
      y = 3530;
      break;
   case DMPAPER_ENV_B5:
      x = 1760;
      y = 2500;
      break;
   case DMPAPER_ENV_B6:
      x = 1760;
      y = 1250;
      break;
   case DMPAPER_ENV_ITALY:
      x = 1100;
      y = 2300;
      break;
   case DMPAPER_ENV_MONARCH: 
      x = de_pulgadas(3,7,8);
      y = de_pulgadas(7,1,2);
      break;
   case DMPAPER_ENV_PERSONAL:
      x = de_pulgadas(3,5,8);
      y = de_pulgadas(6,1,2);
      break;
   case DMPAPER_FANFOLD_US:   
      x = de_pulgadas(14,7,8);
      y = de_pulgadas(11,0,0);
      break;
   case DMPAPER_FANFOLD_STD_GERMAN:
      x = de_pulgadas(8,1,2);
      y = de_pulgadas(12,0,0);
      break;
   case DMPAPER_FANFOLD_LGL_GERMAN:
      x = de_pulgadas(8,1,2);
      y = de_pulgadas(13,0,0);
      break;
		 default:
          x = dx;
          y = dy;
          if (!x || !y)
          {
             x = 2100;
             y = 2970;
          }
          break;
  }
  *px = x;
  *py = y;
}

static bool es_numero(char *texto) {
	while (*texto) {
		if (*texto < '0' || *texto > '9') {
			if (*texto != '.' && *texto != ',' && *texto != ' ')
				return false;
		}
		texto++;
	}
	return true;
}

static void _FuncionesImpresion(int funcion,char *dato);

void FuncionesImpresion(int funcion,void *dato)
{
	_FuncionesImpresion(funcion,(char *)dato);
}

static void _FuncionesImpresion(int funcion,char *dato)
{
    static CDC *cDC = NULL; 
    static int en_documento = 0;
    static int en_pagina = 0;
    static CFont *Fuente = NULL;
    static CFont *Fuente_s = NULL;
    static CFont *Fuente_c = NULL;
    static CFont *Fuente_e = NULL;
    static CFont *Fuente_n = NULL;
    static CPen  *Pene = NULL;
    static int p_x = 0; // x papel en milimetros
    static int p_y = 0; // y papel en milimetros
    static int m_y = 10; // margen superior
    static int m_x = 10; // margen izquierdo
    static int m_x2= 10; // margen Duplex
    static int orientacion = 0;
    static int linea_y = 0;
    static int linea_x = 0;
    static int linea = 0;
    static int negrita = 0;
    static int factor_margen = 1;
    static int nlinea = 0;
	static int ini_col = 0;
	static bool solo_con_campos = false;
	static bool omitirinicio = false;
	static string workbook_file = "";
	static XLSFile *workbook = NULL;
	static XLSSheet *sheet = NULL;
    //static PRINTDLG prdlg;
    // hDevMode and hDevNames    
    
    if (!funcion)
    {		
		/*
        CMenu *main = ((CDsxFrame*)theApp.m_pMainWnd)->GetMenu();
        if (main)
		*/
        {
			/*
            CMenu *popup = main->GetSubMenu( 0 );
            if (popup)
			*/
            {
                int *p = (int *)dato;
				// De momento no usamos impresion por encima de los defectos ...

				/*
                if (popup->GetMenuState(ID_IMPREPANTA,  MF_BYCOMMAND    ) & MF_CHECKED)
                {
                    *p = 3;
                }
                else if (popup->GetMenuState(ID_IMPREXCEL,  MF_BYCOMMAND    ) & MF_CHECKED)
                {
                    *p = 2;
                }
                else if (popup->GetMenuState(ID_IMPRESORAINTERNA,  MF_BYCOMMAND    ) & MF_CHECKED)
                {			  
                    *p = 0;
                }
                else
				*/
                {
                    *p = 1;
                }
         }
      }
	  
      return;
   }
   
   if (funcion == 1) // pedir impresora
   {
       int *m = (int *)dato;
       
       
       if (cDC)
       {		
           if (en_documento)
           {
               if (en_pagina)
                   cDC->EndPage();
               cDC->EndDoc();
           }		
           delete cDC;
           cDC = NULL;		
           if (Fuente_s)
           {
               delete Fuente_s;
               Fuente_s = NULL;
           }
           if (Fuente_c)
           {
               delete Fuente_c;
               Fuente_c = NULL;
           }
           if (Fuente_n)
           {
               delete Fuente_n;
               Fuente_n = NULL;
           }
           if (Fuente_e)
           {
               delete Fuente_e;
               Fuente_e = NULL;
           }
           if (Pene)
           {
               delete Pene;
               Pene = NULL;
           }
      }
      
      en_documento = 0;
      en_pagina = 0;      
      
      OSVERSIONINFO     verInfo = {0};
      verInfo.dwOSVersionInfoSize = sizeof (verInfo);
      GetVersionEx(&verInfo);      
	  
	  /*
      memset((void *)&prdlg,0,sizeof(prdlg));	 
      prdlg.lStructSize = sizeof(prdlg);
      if (!(theApp.m_Limitacion & 1) && verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
      {
          ::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_COMMAND,ID_AVISO_DEFIMPRE,(LPARAM)&prdlg);         
      }
	  */	  
      
      CPrintDialog pdlg(FALSE);
	  theApp.GetPrinterDeviceDefaults( &(pdlg.m_pd) );     
	  
	  /*
      if ( !(theApp.m_Limitacion & 1) && verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && prdlg.hDevMode)
      {
          int extra;
          DEVMODE *pdv;
          DWORD size = ::GlobalSize(prdlg.hDevMode);         
          void *p2 = ::GlobalLock(prdlg.hDevMode);
          
          pdv = (DEVMODE *)p2;
          extra = (int)pdv->dmDriverExtra;
          
          pdlg.m_pd.hDevMode  = ::GlobalAlloc(GMEM_MOVEABLE,size + 100);
          void *p1 = ::GlobalLock(pdlg.m_pd.hDevMode);
          
          memcpy(p1,p2,size);
          ::GlobalUnlock(pdlg.m_pd.hDevMode);
          ::GlobalUnlock(prdlg.hDevMode);
      }
	  else
	      pdlg.m_pd.hDevMode = NULL;

	  */
	  /*
	  if ( (theApp.m_Limitacion & 1) || verInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
	  {
          pdlg.m_pd.hDevMode = NULL;
	  } 
	  */

      
	  /*
      if ( !(theApp.m_Limitacion & 1) && verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && prdlg.hDevNames)
      {
          int extra;
          DEVNAMES *pdn;
          DWORD size = ::GlobalSize(prdlg.hDevNames);         
          void *p2 = ::GlobalLock(prdlg.hDevNames);
          
          pdn = (DEVNAMES *)p2;
          
          extra = sizeof(DEVNAMES) - 1;
          if (extra < pdn->wDriverOffset)
              extra = pdn->wDriverOffset;
          if (extra < pdn->wDeviceOffset)
              extra = pdn->wDeviceOffset;
          if (extra < pdn->wOutputOffset)
              extra = pdn->wOutputOffset;
          if (extra > (sizeof(DEVNAMES) - 1))
              extra += (strlen( ((char*)p2)+extra ) + 1);
          
          pdlg.m_pd.hDevNames = ::GlobalAlloc(GMEM_MOVEABLE, size +100);
          void *p1 = ::GlobalLock(pdlg.m_pd.hDevNames);         
          memcpy(p1,p2,size);
          ::GlobalUnlock(pdlg.m_pd.hDevNames);
          ::GlobalUnlock(prdlg.hDevNames);
      }
      else
      {
          pdlg.m_pd.hDevNames = NULL;
      } 	  
	  */
	  /*
	  if ( (theApp.m_Limitacion & 1) || verInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
	  {
          pdlg.m_pd.hDevNames = NULL;
	  } 
	  */

      

    BOOL r;
    //if (pdlg.m_pd.hDevMode /* *m */)
    {        
        HDC hDC = pdlg.CreatePrinterDC( );
        if (hDC)
        {
            pdlg.m_pd.hDC = hDC; // redundante
            r = TRUE;
        }
        else
            r = FALSE;
    }
	/*
    else
    {         
        if (pdlg.DoModal() != IDOK)
            r = FALSE;
        else
            r = TRUE;
    }
	*/	

    if (!r)
        *m = -1;
    else
    {
        *m = 0;
        cDC = new CDC();
        cDC->Attach( pdlg.GetPrinterDC() );       //
		cDC->SetMapMode(MM_TEXT);
    
        int x,y;
        //tam_papel(pdlg.GetDevMode()->dmPaperSize,pdlg.GetDevMode()->dmPaperWidth,pdlg.GetDevMode()->dmPaperLength,&x,&y);
		
		x = cDC->GetDeviceCaps(HORZSIZE)*10;
		y = cDC->GetDeviceCaps(VERTSIZE)*10;	

		/*
        x -= cDC->GetDeviceCaps(PHYSICALOFFSETX);         
		y -= cDC->GetDeviceCaps(PHYSICALOFFSETY);
		*/

    
        orientacion = pdlg.GetDevMode()->dmOrientation;
        if (orientacion != DMORIENT_PORTRAIT)
        {
            p_x = y;
            p_y = x;
        }
        else
        {
            p_y = y;
            p_x = x;
        }
	    /* OJO ....
        ::GlobalFree(pdlg.m_pd.hDevMode);
        ::GlobalFree(pdlg.m_pd.hDevNames);
		*/
    
        //strcpy(inicializo,"10#10#1#0#1#\n");	  	  
        //inicializo[35] = strlen(inicializo);
        m_y = theApp.m_MargenS; // margen superior
        m_x = theApp.m_MargenI; // margen izquierdo	   
        m_x2= theApp.m_MargenI2; // margen izquierdo	   		

    
        factor_margen = 1;
    
        negrita = 0;
    
        char *p = (char *)dato;
        p += sizeof(int);
        unsigned short *ll = (unsigned short *)p;
        if (*ll)
        {	
            char *ini_p = p;	 
            int esx = 0; 
            for (int i = 0;i < *ll && p[i];i++)
            {
                if (p[i] == '#')
                {
                    p[i] = 0;
                    if (!esx)
                    {
                        m_y = atoi(ini_p);
                        ini_p = p + 1;
                        esx = 1;
                    }
                    else
                    {
                        m_x = atoi(ini_p);
                        m_x2= atoi(ini_p);
                        break;
                    }
                   }
                }
             }
         
             cDC->SetMapMode(MM_TEXT);
         
             CSize s(p_x*10,p_y*10);
             cDC->HIMETRICtoDP(&s);
             p_x = s.cx;
             p_y = s.cy;
         
             s.cx = m_x2*100;
             s.cy = 1;
             cDC->HIMETRICtoDP(&s);
             m_x2 = s.cx;
         
             s.cx = m_x*100;
             s.cy = m_y*100;
             cDC->HIMETRICtoDP(&s);
             m_x = s.cx;
             m_y = s.cy;
         
         
             int tamy;
             int tam_s;
             int tam_c;
             int tam_e;
         
             if (!theApp.m_AjustaImpre || !(theApp.m_CarsI))
             {
                 tam_s = cDC->GetDeviceCaps(LOGPIXELSX)/10;
                 tam_c = cDC->GetDeviceCaps(LOGPIXELSX)/17;
                 tam_e = cDC->GetDeviceCaps(LOGPIXELSX)/5;  
             }
             else
             {
                 tam_s = (p_x-m_x) / theApp.m_CarsI;
                 tam_c = tam_s * 10 / 17;
                 tam_e = tam_s / 2;		  
             }
         
             if (!theApp.m_AjustaImpre)
             {
                 tamy  = cDC->GetDeviceCaps(LOGPIXELSY)/6;
             }
             else
             {
                 tamy  = (p_y-m_y) / theApp.m_LineasI;
             }
         
			 int factor,factor2,factor3; // 4060.45

			 factor = tamy;
			 factor2 = tamy;
			 factor3 = tamy;

			 char *pro;
			 
			 if ((pro = getenv("DSWINE")))
			 {				 
				 int ff;
				 ff = atoi(pro);
				 if (ff <= 0)
					 ff = 160;
				 factor = (tam_s*300)/ff;
				 if (factor > tamy)
				 {
					 factor = tamy;
				 }
				 factor2 = (tam_c*300)/ff;
				 if (factor2 > tamy)
				 {
					 factor2 = tamy;
				 }
				 factor3 = (tam_e*300)/ff;
				 if (factor3 > tamy)
				 {
					 factor3 = tamy;
				 }
			 }

             Fuente_s = new CFont();	   	     
             Fuente_s->CreateFont(factor,tam_s, 
                 0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_TT_ALWAYS,
                 DRAFT_QUALITY ,FIXED_PITCH,theApp.m_FontI.lfFaceName);   
             Fuente_c = new CFont();	   	     
             Fuente_c->CreateFont(factor2,tam_c, 
                 0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_TT_ALWAYS,
                 DRAFT_QUALITY ,FIXED_PITCH,theApp.m_FontI.lfFaceName);
             Fuente_e = new CFont();	   	     
             Fuente_e->CreateFont(factor3,tam_e,
                 0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_TT_ALWAYS,
                 DRAFT_QUALITY ,FIXED_PITCH,theApp.m_FontI.lfFaceName);
             Fuente = Fuente_s;
             cDC->SelectObject( Fuente );
             LOGFONT lg;
             Fuente->GetLogFont(&lg);
			 if (factor != tamy)
			 {
				 linea_y = tamy;
			 }
			 else
			 {
				 linea_y = lg.lfHeight;
			 }
             linea_x = lg.lfWidth;
             if (!theApp.m_AjustaImpre)
             {
                 *m = (p_y-m_y) / linea_y;
                 m++;
                 *m = (p_x-m_x) / linea_x;	   
             }
             else
             {
                 *m = theApp.m_LineasI; 
                 m++;
                 *m = theApp.m_CarsI;		 
             }
         
             Pene = new CPen(PS_SOLID,linea_x/8,RGB(0,0,0));
             cDC->SelectObject( Pene );
          }
		  
          return;
        }
    
    if (funcion == 2) // Imprimir linea
    {
        int *m = (int *)dato;
        
        if (cDC)
        {
        /*
        if (!cDC->QueryAbort( ))
        {
        cDC->AbortDoc( );
        *m = -1;
        return;
        }
            */
            if (!en_documento)
            {
                DOCINFO doc;
                memset(&doc,0,sizeof(doc));
                doc.cbSize = sizeof(doc);
                doc.lpszDocName = pDsSys->m_Usuario;        
                
                
                if (cDC->StartDoc(&doc) < 0)
                    *m = -1;
                else
                {
                    en_documento = 1;
                }
          }
          if (en_documento)
          {
              unsigned short l = *((unsigned short *)dato);
              unsigned char *data = ((unsigned char *)dato) + sizeof(short);
              if (!l) return;
              while(l && (data[l-1] == 10 || data[l-1] == 13)) l--;              
              if (!en_pagina)
              {
                  cDC->StartPage();
                  cDC->SelectObject( Fuente );
                  cDC->SelectObject( Pene );
				  cDC->SetWindowOrg (0 , 0);
				  cDC->SetMapMode(MM_TEXT);
                  en_pagina = 1;
                  linea = 0;
              }
              if (en_pagina)
              {
                  if (l)
                  {
                      data[l] = 0;
                      if (data[0] == '#' && data[l-1] == '#')
                      {
                          int comando = 0;
                          if (!strcmp((char *)data,"#FIN#"))
                          {
                              factor_margen = 1 - (factor_margen * 1);
                              cDC->EndPage();					  
                              comando = 1;
                              linea = 0;
                              en_pagina = 0;
                          }
                          if (comando || (l > 2 && data[1] != '#'))
                              return;
                   }
                   if ( (m_y+linea*linea_y+linea_y) > p_y)
                   {
                       // Salto Forzado
                       factor_margen = 1 - (factor_margen * 1);
                       cDC->EndPage();
                       cDC->StartPage();
                       cDC->SelectObject( Fuente );
                       cDC->SelectObject( Pene );
					   cDC->SetWindowOrg (0 , 0);
				       cDC->SetMapMode(MM_TEXT);
                       linea = 0;
                       en_pagina = 1;
                   }
                   
                   // huh caracter a caracter
                   //cDC->TextOut(m_x,m_y+linea*linea_y,(LPCSTR)tmp,l);
                   int j = 0;
                   int x = 0;

                   if (!factor_margen)
                       x = m_x2;
                   else
                       x = m_x;

                   for (int i = 0;i < l;i++)
                   {
                       CFont *nuevo_fuente = NULL;				   
                       if (data[i] == 139)
                       {
                           nuevo_fuente = Fuente_c;
                       }
                       else if (data[i] == 140)
                       {
                           nuevo_fuente = Fuente_s;
                       }
                       else if (data[i] == 141)
                       {
                           if (!negrita)
                           {
                               negrita = 1;
                               nuevo_fuente = Fuente;
                           }
                           else
                               continue;
                       }
                       else if (data[i] == 142)
                       {
                           if (negrita)
                           {
                               negrita = 0;
                               nuevo_fuente = Fuente;
                           }
                           else
                               continue;
                       }
                       else if (data[i] == 143)
                       {				      
                           nuevo_fuente = Fuente_e;
                       }
                       else if (data[i] == 144)
                       {				      
                           nuevo_fuente = Fuente_s;
                       }
                       
                       if (nuevo_fuente)
                       {
                           Fuente = nuevo_fuente;
                           LOGFONT lf;
                           if (negrita)
                           {
                               if (Fuente_n)
                                   delete Fuente_n;						 
                               nuevo_fuente->GetLogFont(&lf);
                               lf.lfWeight = FW_BOLD;
                               Fuente_n = new CFont();
                               Fuente_n->CreateFontIndirect(&lf);
                               nuevo_fuente = Fuente_n;
                           }
                           cDC->SelectObject( nuevo_fuente );
                           nuevo_fuente->GetLogFont(&lf);
                           linea_x = lf.lfWidth;
                           continue;
                       }                       

                       int y = m_y+linea*linea_y;

                       j++;
                       
                       if (data[i] ==  196 || data[i] == 205)
                       {
                           cDC->MoveTo(x,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                       }
                       else if (data[i] ==  179 || data[i] == 186)
                       {
                           cDC->MoveTo(x+linea_x/2,y);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                       }
                       else if (data[i] ==  218 || data[i] == 201 || data[i] == 214 || data[i] == 213)
                       {
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                       }
                       else if (data[i] ==  194 || data[i] == 203 || data[i] == 210 || data[i] == 209)
                       {
                           cDC->MoveTo(x,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                       }
                       else if (data[i] ==  191 || data[i] == 187 || data[i] == 183 || data[i] == 184)
                       {
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x-1,y+linea_y/2);
                       }
                       else if (data[i] ==  195 || data[i] == 204 || data[i] == 199 || data[i] == 198)
                       {
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                           cDC->MoveTo(x+linea_x/2,y);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                       }
                       else if (data[i] ==  197 || data[i] == 206 || data[i] == 215 || data[i] == 216)
                       {
                           cDC->MoveTo(x+linea_x/2,y);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                           cDC->MoveTo(x,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                       }
                       else if (data[i] ==  180 || data[i] == 185 || data[i] == 182 || data[i] == 181)
                       {
                           cDC->MoveTo(x+linea_x/2,y);
                           cDC->LineTo(x+linea_x/2,y+linea_y);
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x-1,y+linea_y/2);
                       }
                       else if (data[i] ==  192 || data[i] == 200 || data[i] == 211 || data[i] == 212)
                       {
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x/2,y-1);
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                       }
                       else if (data[i] ==  193 || data[i] == 202 || data[i] == 208 || data[i] == 207)
                       {
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x/2,y-1);
                           cDC->MoveTo(x,y+linea_y/2);
                           cDC->LineTo(x+linea_x,y+linea_y/2);
                       }
                       else if (data[i] ==  217 || data[i] == 188 || data[i] == 189 || data[i] == 190)
                       {
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x+linea_x/2,y-1);
                           cDC->MoveTo(x+linea_x/2,y+linea_y/2);
                           cDC->LineTo(x-1,y+linea_y/2);
                       }
                       else
                       {
                           unsigned char tmp[2];
                           tmp[0] = data[i];
                           tmp[1] = 0;
                           oem_a_ansi(tmp);					  
                           cDC->TextOut(x,y,(LPCSTR)tmp,1);
                       }                   
                       x += linea_x;
               }                 
             }			  
             linea++;
           }
         }
      }
      return;
    }
    
    if (funcion == 3) // Fin Impresion
    {	 
        if (cDC)
        {
            if (en_documento)
            {
                if (en_pagina)
                    cDC->EndPage();
                cDC->EndDoc();
            }
            delete cDC;
            cDC = NULL;
            if (Fuente_s)
            {
                delete Fuente_s;
                Fuente_s = NULL;
            }
            if (Fuente_c)
            {
                delete Fuente_c;
                Fuente_c = NULL;
            }
            if (Fuente_n)
            {
                delete Fuente_n;
                Fuente_n = NULL;
            }
            if (Fuente_e)
            {
                delete Fuente_e;
                Fuente_e = NULL;
            }
            if (Pene)
            {
                delete Pene;
                Pene = NULL;
            }
       }	 
    }
    
    if (funcion == 4)
    {
        char *p = (char *)dato;
        if (theApp.m_CabeceraExcel)
            sprintf(p,",%s",(LPCTSTR)theApp.m_PosExcel);
        else
            sprintf(p,",%s,solo datos",(LPCTSTR)theApp.m_PosExcel);   
        return;
    }

    if (funcion == 5)
    {
        CDlgOldImpre dlg;
        char *p = (char *)dato;

        dlg.m_impre = p;
        dlg.m_inispool = p+256;

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
		  theApp.m_CrystalView = dlg.m_precrystal;
		  theApp.m_pedidaimpresora = dlg.m_crselimprecristal;
		} 		
		
		strcpy(p,dlg.m_impre);		
		if (!strcmp(dlg.m_impre,"Fax"))
		{
			dlg.m_NumeroFax.TrimRight();
			dlg.m_CorreoFax.TrimRight();
			dlg.m_CorreoAsunto.TrimRight();
			if (dlg.m_faxauto)
				p[256] = '!';
			else
				p[256] = '-';
			strcpy(p+257,dlg.m_NumeroFax);			
			strcat(p+256,"*");
			strcat(p+256,dlg.m_CorreoFax);
			if (!dlg.m_CorreoAsunto.IsEmpty())
			{
				strcat(p+256,"&");
				strcat(p+256,dlg.m_CorreoAsunto);
				if (strlen(p+256) > 73)
				{
					p[329] = 0;
				}
			}
		}
		else if (!strcmp(dlg.m_impre,"PDF"))
		{
			if (dlg.m_precrystal)
				p[256] = '!';
			else
				p[256] = '-';
			strcpy(p+257,dlg.m_spool);
			if (p[257])
				unlink(dlg.m_spool);
		}
		else if (!strcmp(dlg.m_impre,"Correo"))
		{
			int nl;
			dlg.m_CorreoAsunto.TrimRight();
			dlg.m_CorreoFax.TrimRight();			
			if (dlg.m_faxauto)
				p[256] = '!';
			else
				p[256] = '-';
			if (dlg.m_CorreoFax.GetLength() > 73)
				dlg.m_CorreoFax.SetAt(73,0);
			nl = dlg.m_CorreoFax.GetLength() + dlg.m_CorreoAsunto.GetLength();
			if (nl > 73)
			{
				nl = 73 - dlg.m_CorreoFax.GetLength();
				if (nl < 0)
					nl = 0;
				dlg.m_CorreoAsunto.SetAt(nl,0);
			}
			strcpy(p+257,dlg.m_CorreoAsunto);			
			strcat(p+256,"*");
			strcat(p+256,dlg.m_CorreoFax);
		}
		else if (!strcmp(dlg.m_impre,"Crystal Reports"))
		{
			p[256] = 0;
			if (dlg.m_pdfcrystal)
			{
				CString tmps;
				tmps = dlg.m_spool;
				tmps.MakeLower();
				if (tmps.Find(".pdf") < 0)
					dlg.m_spool += ".pdf";
				tmps = quita_path(dlg.m_spool);
				if (!tmps.Compare(dlg.m_spool))
				{
					tmps = theApp.m_dirbase;
					tmps += "spool\\";
					tmps += dlg.m_spool;
					dlg.m_spool = tmps;
				}
				strcpy(p+257,dlg.m_spool);
				if (p[257])
					unlink(dlg.m_spool);
				if (dlg.m_precrystal)
					p[256] = '!';
				else
					p[256] = '-';
			}
		}
		else
		{
			strcpy(p+256,dlg.m_spool);
		}
		

        return;
    }

	if (funcion == 6) /* Salida a PDF */
	{	    
		*((int *)dato) = linea_a_pdf(((unsigned char *)dato)+8);
		return;
	}
	if (funcion == 7) /* Fin Salida a PDF */
	{
		*((int *)dato) = fin_de_pdf(((char *)dato)+8);
		return;
	}

    if (funcion == 8 || funcion == 998) { //pseudo-excel


		using namespace xlsFile;

		char *comando = (char *)dato;
		/*
		 ejemplo

		XLSFile xl("./creation/monFichierXLs.xls"); 
	    XLSFile xl2("./creation/monFichierXLs.xls",RW);
		XLSSheet fxl2 = xl2.getSheet(0);	//on recupere la premier feuille
		XLSSheet f = xl.newSheet("Sheet1");
		XLSSheet fc = xl2.newSheet("Sheet1");
			fxl2.deleteContent(0,0); 		//on supprime le contenu de la case(0,0)
			fc.setText(0,0,"CLR_INDIGO");
			fc.setCellBgColor(0,0,CLR_INDIGO);
			fc.setText(0,0,"BORDER_LEFT");
			fc.setCellBorderStyle(0,0,BORDER_LEFT);

			fc.setText(1,0,"BORDER_RIGHT");
			fc.setCellBorderStyle(1,0,BORDER_RIGHT);

			fc.setText(2,0,"BORDER_BOTTOM");
			fc.setCellBorderStyle(2,0,BORDER_BOTTOM);

			fc.setText(3,0,"BORDER_TOP");
			fc.setCellBorderStyle(3,0,BORDER_TOP);

		f.setText(1,1,"Un contenu"); //ajout de texte à la case (1,1)  (ligne,colonne)
			f.setFontBold(1,1,BOLDNESS_BOLD);//la cellule (1,1) sera ecrite en gras
			f.setFontItalic(1,1,true);//la cellule (1,1) sera ecrite en italique
			f.setFontOrientation(1,1,ORI_TOPBOTTOMTXT);//le contenu de la cellule sera ecrit a la vertical
			f.setCellFrame(1,1,CLR_DARK_RED,BORDER_MEDIUM);//on met un cadre a cette cellule
			f.setFontHAlign(1,1,HALIGN_RIGHT);
 
		f.setNumber(0,0,3.5); //ajout de nombre à la case (0,0)
			f.setFont(0,0,"serif"); //la cellule (0,0) sera ecrite en serif
			f.setFontColor(0,0,CLR_ORANGE); //la cellule (0,0) aura pour couleur d"ecriture le orange

		xl.save();
		xl2.save("./test_copie/testCopieMonFichierXLs.xls");
		*/

		if (!strcmp(comando,"parametros")) {
				nlinea  = 1;
				ini_col = 0;
		}
		else if (!strcmp(comando,"inicializacion")) {
            char fulla[512];
			char tmp[512];

			//::MessageBox(NULL,"inicializacion","workbook_file",MB_OK);

			fulla[0] = 0;
            nlinea  = 1;
            ini_col = 0;
            solo_con_campos = false;

            int i = 0;
            for (i = 31;dato[i] != 0 && dato[i] != ',';i++);
            if (dato[i] == ',') {
                dato[i] = 0;
                i++;
                int g = i;
                for (;dato[g] != 0 && dato[g] != ',';g++);
                if (dato[g] == ',') {
                    dato[g] = 0;
                    int gg = g + 1;
                    for (;dato[gg] != 0 && dato[gg] != ',';gg++);
                    if (dato[gg] == ',')  {
                        dato[gg] = 0;
						strcpy(fulla,((char *)dato)+gg+1);
                    }
					strcpy(tmp,((char *)dato)+g+1);
                    if (!strcmp(tmp,"solo datos") || !strcmp(tmp,"solodatos") || !strcmp(tmp,"solo dato") || !strcmp(tmp,"solodato")) {
                        solo_con_campos = true;
                    }
                }

                for (;dato[i] != 0;i++)
                {
                    if (dato[i] >= 'A' && dato[i] <= 'Z') {
                        int k = dato[i] - 'A';
                        ini_col *= 27;
                        ini_col += k;
                    }
                    else {
                        nlinea = atoi(((char *)dato)+i);
                        if (nlinea < 1) nlinea = 1;
                        break;
                    }
                }
            }
            XLSFile *original = NULL;
            char *name =((char *)dato)+31;
            if (!*name || !strcmp(name,"nuevo")) {
                workbook_file = ".\\tmp.xls";//java.io.File.createTempFile("dstj", ".xls").getAbsolutePath();
            }
            else {
                workbook_file = name;
				if (!strstr(name,"."))
					workbook_file += ".xls";
                try {
                    original = new XLSFile(workbook_file,RW);
                } catch (...) {
                    //ex.printStackTrace();
                    //DsLogger.getLogger(DsImpresion.class.getName()).log(Level.SEVERE, null, ex);
                    original = NULL;
                }
            }                
            try {
                if (original != NULL) {
                    workbook = original;// Workbook.createWorkbook(new File(workbook_file),original);
                    if (!fulla[0])
						try {
							sheet = new XLSSheet(workbook->getSheet(0));
						}
						catch(...) {
							sheet = new XLSSheet(workbook->newSheet("Hoja 1"));
						}
                    else {
                        if (fulla[0] == '?') {
							/*
                            sheet = workbook.getSheet(0);
                            string nn = sheet.getName();
							if (nn.length == 0)
                                sheet = workbook.createSheet("Hoja 1", 0);
                            if (fulla.length() > 1)
                                sheet.setName(fulla.substring(1));
							*/
							sheet = new XLSSheet(workbook->newSheet("Hoja 1"));
                        }
                        else
                            sheet = new XLSSheet(workbook->getSheet(atoi(fulla)));
                    }
                    //delete original;
                    original = NULL;
                }
                else {
					if (!*name || !strcmp(name,"nuevo")) 
						workbook = new XLSFile(workbook_file);
					else
						workbook = new XLSFile(workbook_file,RW); 
                    if (!fulla[0])
                        sheet = new XLSSheet(workbook->newSheet("Hoja 1"));
                    else {
                        if (fulla[0] == '?') {
							/*
                            if (fulla.length() > 1)
                                sheet = workbook.createSheet(fulla.substring(1), 0);
                            else
							*/
                                sheet = new XLSSheet(workbook->newSheet("Hoja 1"));
                        }
                        else
                            sheet = new XLSSheet(workbook->newSheet(fulla));
                    }
                }
            } catch (...) {
                //ex.printStackTrace();
                //DsLogger.getLogger(DsImpresion.class.getName()).log(Level.SEVERE, null, ex);
            }
            if (solo_con_campos)
                omitirinicio = true;
            else
                omitirinicio = false;

			/*
			if (workbook && sheet) 
				::MessageBox(NULL,"todo bien","workbook_file",MB_OK);
			else
				::MessageBox(NULL,"ops","workbook_file",MB_OK);
				*/
		}
		else if (!strcmp(comando,"imprime")) {
            //String linea = DsSysJava.getCString(dato, 31);
            bool hay_campos = false;
            int col = ini_col;	            
            bool espuroalfa = false;
            bool esfecha = false;
			char tmp2[1024];

            //byte tmp2[] = new byte[dato.length];

            if (omitirinicio) {
                omitirinicio = false;
                return;
            }

            //WritableFont courierfont = new WritableFont(WritableFont.COURIER, 12, WritableFont.NO_BOLD, true);
            //WritableCellFormat fcourierfont = new WritableCellFormat (courierfont);
            
            int p1,k;
            int j = 0;
            int i = 0;
            p1 = 31;
            for (i = 31;dato[i] != 0;i++) {
                if (dato[i] == 9) {
                    hay_campos = true;
                    if (i > p1) {
                        int r = p1;
                        while(r < i) {if (dato[r] != ' ') break;r++;}
                        if (r < i) {
                            col++;
                            //while(par[p1] == ' ') p1++;
                            k = 0;                            
                            espuroalfa = false;
                            esfecha = false;
                            for (j = 0;j < (i-p1);j++) {
                                if (dato[p1+j] == '\n' || dato[p1+j] == '\r') break;
                                if (dato[p1+j] == 31) {
                                    espuroalfa = true;
                                    continue;
                                }
                                if (dato[p1+j] == 30)  {
                                    esfecha = true;
                                    continue;
                                }
                                if ( ((int)(unsigned char *)dato[p1+j]) >= 32)
                                    tmp2[k++] = dato[p1+j];
                            }
                            tmp2[k] = 0;
                            j = k;
                            j--;
                            while(j > 0 && tmp2[j] == ' ') {tmp2[j] = 0;j--;}
                            if (tmp2[0] != 0 && tmp2[0] != '=')  {
                                //::MessageBox(NULL,tmp2,tmp1,MB_OK);

                                //Object o = null;
                                if (esfecha) {									
                                    //Calendar fecha = Calendar.getInstance();
                                    //tmp2[2] = 0;
                                    //tmp2[5] = 0;
                                    //tmp2[10] = 0;
                                    //fecha.set(Integer.valueOf(DsSysJava.getCString(tmp2, 6,false)),Integer.valueOf(DsSysJava.getCString(tmp2, 3,false))-1, Integer.valueOf(DsSysJava.getCString(tmp2, 0,false)));
                                    //o = new jxl.write.DateTime(col-1, nlinea-1, (Date)fecha.getTime(),new jxl.write.WritableCellFormat(new jxl.write.DateFormat(("d/M/yy"))));
									sheet->setText(nlinea-1,col-1,tmp2);

                                }
                                else if (espuroalfa)
                                {
									sheet->setText(nlinea-1,col-1,tmp2);
                                    //o = new jxl.write.Label(col-1, nlinea-1, DsSysJava.getCString(tmp2,0,false),fcourierfont);
                                }
                                else if (es_numero(tmp2))
                                {
									sheet->setNumber(nlinea-1,col-1,(double)atof(tmp2));
                                    //o = new jxl.write.Number(col-1, nlinea-1,Double.valueOf(DsSysJava.getCString(tmp2,0,false)));
                                }
								else
									sheet->setText(nlinea-1,col-1,tmp2);

								//sheet->setFont(nlinea-1,col-1,"Courier");
								//sheet->setFontHeight(nlinea-1,col-1,12);
								/*
                                try {
                                    if (o == null) {
                                        o = new jxl.write.Label(col-1, nlinea-1, DsSysJava.getCString(tmp2,0,false),fcourierfont);
                                    }
                                    sheet.addCell((WritableCell) o);
                                        //pSheet2->Range[(LPCSTR)tmp1]->Value = (LPCSTR)tmp2;
                                } catch (WriteException ex) {
                                    ex.printStackTrace();
                                    DsLogger.getLogger(DsImpresion.class.getName()).log(Level.SEVERE, null, ex);
                                } 
                                //pSheet2->Range[(LPCSTR)tmp1]->Value = (LPCSTR)tmp2;
								*/
                            }
                        }
                    }
                    i++;
                    p1 = i;
                    while(dato[i] != 0 && dato[i] != 9) i++;
				// if (!par[i]) break;

                    if (i > p1)	{
                        col++;
                        while(dato[p1] == ' ') p1++;
                        k = 0;
                        espuroalfa = false;
                        esfecha = false;
                        for (j = 0;j < (i-p1);j++) {
                            if (dato[p1+j] == '\n' || dato[p1+j] == '\r') break;
                            if (dato[p1+j] == 31) {
                                espuroalfa = true;
                                continue;
                            }
                            if (dato[p1+j] == 30)  {
                                esfecha = true;
                                continue;
                            }
                            if ((int)(unsigned char *)(dato[p1+j]) >= 32)
                                tmp2[k++] = dato[p1+j];
                        }
                        tmp2[k] = 0;
                        j = k;
                        j--;
                        while(j > 0 && tmp2[j] == ' ') {tmp2[j] = 0;j--;}
                        if (tmp2[0] != 0 && tmp2[0] != '=')  {
                            //::MessageBox(NULL,tmp2,tmp1,MB_OK);
                            //Object o = null;
                            if (esfecha) {
								
                                //Calendar fecha = Calendar.getInstance();
                                //tmp2[2] = 0;
                                //tmp2[5] = 0;
                                //tmp2[10] = 0;
                                //fecha.set(Integer.valueOf(DsSysJava.getCString(tmp2, 6,false)),Integer.valueOf(DsSysJava.getCString(tmp2, 3,false))-1, Integer.valueOf(DsSysJava.getCString(tmp2, 0,false)));
                                //o = new jxl.write.DateTime(col-1, nlinea-1, (Date)fecha.getTime(),new jxl.write.WritableCellFormat(new jxl.write.DateFormat(("d/M/yy"))));
									sheet->setText(nlinea-1,col-1,tmp2);
								
                            }
                            else if (espuroalfa)
                            {
									sheet->setText(nlinea-1,col-1,tmp2);
                                //o = new jxl.write.Label( col-1, nlinea-1,DsSysJava.getCString(tmp2,0,false),fcourierfont);
                            }
                            else if (es_numero(tmp2))
                            {
                                //o = new jxl.write.Number(col-1, nlinea-1,Double.valueOf(DsSysJava.getCString(tmp2,0,false)));
									sheet->setNumber(nlinea-1,col-1,(double)atof(tmp2));
                            }
							else
								sheet->setText(nlinea-1,col-1,tmp2);

							//sheet->setFont(nlinea-1,col-1,"Courier");
							//sheet->setFontHeight(nlinea-1,col-1,12);
							/*
                            try {
                                if (o == null) {
                                    o = new jxl.write.Label(col-1, nlinea-1, DsSysJava.getCString(tmp2,0,false),fcourierfont);
                                    //o = new jxl.write.Label(col-1, nlinea-1, "HOLA 2",fcourierfont); //
                                }
                                sheet.addCell((WritableCell) o);
                                    //pSheet2->Range[(LPCSTR)tmp1]->Value = (LPCSTR)tmp2;
                            } catch (WriteException ex) {
                                ex.printStackTrace();
                                DsLogger.getLogger(DsImpresion.class.getName()).log(Level.SEVERE, null, ex);
                            }
							*/
                        }
                    }
                    p1 = i + 1;
                }
            }
    		if (p1 < i && (!solo_con_campos || hay_campos))	{
                col++;
                //while(dato[p1] == ' ') p1++;
                k = 0;
                espuroalfa = false;
                esfecha = false;
                for (j = 0;j < (i-p1);j++) {
                    if (dato[p1+j] == '\n' || dato[p1+j] == '\r') break;
                    if (dato[p1+j] == 31) {
                        espuroalfa = true;
                        continue;
                    }
                    if (dato[p1+j] == 30)  {
                        esfecha = true;
                        continue;
                    }
                    if ((int)(unsigned char *)(dato[p1+j]) >= 32)
                        tmp2[k++] = dato[p1+j];
                }
                tmp2[k] = 0;
                j = k;
                j--;
                while(j > 0 && tmp2[j] == ' ') {tmp2[j] = 0;j--;}
                if (tmp2[0] != 0 && tmp2[0] != '=')  {
                    //::MessageBox(NULL,tmp2,tmp1,MB_OK);
                    //Object o = null;
                    if (esfecha) {
                        //Calendar fecha = Calendar.getInstance();
                        //tmp2[2] = 0;
                        //tmp2[5] = 0;
                        //tmp2[10] = 0;
                        //fecha.set(Integer.valueOf(DsSysJava.getCString(tmp2, 6,false)),Integer.valueOf(DsSysJava.getCString(tmp2, 3,false))-1, Integer.valueOf(DsSysJava.getCString(tmp2, 0, false)));
                        //o = new jxl.write.DateTime(nlinea-1,col-1, (Date)fecha.getTime(),new jxl.write.WritableCellFormat(new jxl.write.DateFormat(("d/M/yy"))/*jxl.write.DateFormats.FORMAT2*/));
									sheet->setText(nlinea-1,col-1,tmp2);
                    }
                    else if (espuroalfa)
                    {
                        //o = new jxl.write.Label(col-1, nlinea-1, DsSysJava.getCString(tmp2,0, false),fcourierfont);
									sheet->setText(nlinea-1,col-1,tmp2);
                    }
                    else if (es_numero(tmp2))
                    {
                        //o = new jxl.write.Number(col-1, nlinea-1,Double.valueOf(DsSysJava.getCString(tmp2,0 , false)),fcourierfont);
									sheet->setNumber(nlinea-1,col-1,(double)atof(tmp2));
                    }
					else
						sheet->setText(nlinea-1,col-1,tmp2);
					//sheet->setFont(nlinea-1,col-1,"Courier");
					//sheet->setFontHeight(nlinea-1,col-1,12);
					/*
                    try {
                        if (o == null) {
                            o = new jxl.write.Label(col-1, nlinea-1, DsSysJava.getCString(tmp2,0, false));
                            //o = new jxl.write.Label(col-1, nlinea-1, "HOLA 3",fcourierfont); //
                        }
                        sheet.addCell((WritableCell) o);
                            //pSheet2->Range[(LPCSTR)tmp1]->Value = (LPCSTR)tmp2;
                    } catch (WriteException ex) {
                        ex.printStackTrace();
                        DsLogger.getLogger(DsImpresion.class.getName()).log(Level.SEVERE, null, ex);
                    }
					*/
                }
            }
            if (!solo_con_campos || hay_campos)
                 nlinea++;
		}
		else if (!strcmp(comando,"finalizacion")) {
            char * accion = dato;
            try {
				workbook->save();
            } catch (...){
            }
            try {
                delete workbook;
            } catch (...){
            }
            sheet = NULL;
            workbook = NULL;
            if (strcmp(accion,"salva")) {
                if (!strcmp(accion,"imprime")) {
					 ::ShellExecute(NULL, "print",workbook_file.c_str(),NULL,NULL, SW_SHOWNORMAL);
                }
                else {
					 ::ShellExecute(NULL, "open",workbook_file.c_str(),NULL,NULL, SW_SHOWNORMAL);
                }
            }
		}
		else if (!strcmp(comando,"carga_book")) {
            XLSFile *original = NULL;
            char *name = dato;
			int err = -1;
            workbook_file = "./xxxtmp.xls";
            try {
                original = new XLSFile(name,RW);
            } catch (...) {
                original = NULL;
            }
            try {
                if (original != NULL) {
					original->save(workbook_file);
					delete original;
					original = NULL;
                    workbook = new XLSFile(workbook_file,RW);//Workbook.createWorkbook(new File(workbook_file),original);
                    sheet = new XLSSheet(workbook->getSheet(0));
                    err = 0;
                }
                else {
                    workbook = new XLSFile(workbook_file,RW);//Workbook.createWorkbook(new File(workbook_file),original);
                    sheet = new XLSSheet(workbook->newSheet("Hoja 1"));
                    err = 0;
                }
            } catch (...) {
                //ex.printStackTrace();
                //DsLogger.getLogger(DsImpresion.class.getName()).log(Level.SEVERE, null, ex);
            }
			*((int *)dato) = err;
            //DsSysJava.putInt(dato, pos, err);
		}
	    else if (!strcmp(comando,"fichero_destino")) {
            workbook_file = dato;
            workbook->save(workbook_file);
		}
        else if (!strcmp(comando,"selecciona_hoja")) {
			char fulla[512];
			int err = -1;

            strcpy(fulla,dato+31);
			sheet = NULL;
			try {
				if (!fulla[0])
					sheet = new XLSSheet(workbook->getSheet(0));
				else {
					try {
						sheet = new XLSSheet(workbook->getSheet(atoi(fulla)));
					} catch(...) {
						sheet = new XLSSheet(workbook->getSheet(0));
					}
				}
			} catch(...) {};
            if (sheet == NULL) {
				sheet = new XLSSheet(workbook->newSheet("Hoja 1"));
            }
            err = 0;
            *((int *)dato) = err;
		}
        else if (!strcmp(comando,"peek_dato")) {
            char *celda = dato;
            char resultado[299];
            int fila = 0,col = 0;
			int err = -1;
			resultado[0] = 0;
            for (int i = 0;celda[i];i++)
            {
                if (celda[i] >= 'A' && celda[i] <= 'Z') {
                    int k = celda[i] - 'A';
                    col *= 27;
                    col += (k+1);
                }
                else {
                    fila = atoi(celda+i);
                    if (fila < 1) fila = 0; else fila--;
                    if (col < 1) col = 0; else col--;
                    break;
                }
            }
            strcpy(resultado,sheet->getLabel(fila,col).c_str());
            //DsSysJava.putCString(dato, 31, resultado, 249);
            err = 0;
            *((int *)dato) = err;
            //DsSysJava.putInt(dato, pos, err);
		}
        else if (!strcmp(comando,"peek_len")) {
            char *celda = dato;
            char resultado[299];
            int fila = 0,col = 0;
			int err = -1;
			resultado[0] = 0;
            for (int i = 0;celda[i];i++)
            {
                if (celda[i] >= 'A' && celda[i] <= 'Z') {
                    int k = celda[i] - 'A';
                    col *= 27;
                    col += (k+1);
                }
                else {
                    fila = atoi(celda+i);
                    if (fila < 1) fila = 0; else fila--;
                    if (col < 1) col = 0; else col--;
                    break;
                }
            }
			sprintf(resultado,"%d",strlen(sheet->getLabel(fila,col).c_str()));
            //DsSysJava.putCString(dato, 31, resultado, 249);
            err = 0;
            *((int *)dato) = err;
            //DsSysJava.putInt(dato, pos, err);
		}
        else if (!strcmp(comando,"poke_dato")) {
            char *celda = dato;
            char *idx = strstr(celda,",");
            char resultado[299];
			int err = -1;
			resultado[0] = 0;
            if (idx) {
                strcpy(resultado,idx+1);
                *idx = 0;
            }
            int fila = 0,col = 0;
            try {
                for (int i = 0;celda[i];i++)
                {
                    if (celda[i] >= 'A' && celda[i] <= 'Z') {
                        int k = celda[i] - 'A';
                        col *= 27;
                        col += (k+1);
                    }
                    else {
                        fila = atoi(celda+i);
                        if (fila < 1) fila = 0; else fila--;
                        if (col < 1) col = 0; else col--;
                        break;
                    }
                }
				sheet->setText(fila,col,resultado);                
                err = 0;
            } catch (...) {
            } 
            *((int *)dato) = err;
            //DsSysJava.putInt(dato, pos, err);
		}
        else if (!strcmp(comando,"fin_book")) {
            char *accion = dato;
            if (strcmp(accion,"nada")) {
                try {
					if (workbook_file.length() == 0) {
						workbook_file = "./xxxtmp.xls";
                        //workbook_file = java.io.File.createTempFile("dstj", ".xls").getAbsolutePath();
                        //workbook.setOutputFile(new File(workbook_file));
                    }
                    workbook->save(workbook_file);
                } catch (...){
                    //e.printStackTrace();
                }
            }
            try {
				delete sheet;
                delete workbook;
            } catch (...){
                //e.printStackTrace();
            }
            sheet = NULL;
            workbook = NULL;
            if (!strcmp(accion,"imprime") || !strcmp(accion,"impresalva")) {
					 ::ShellExecute(NULL, "print",workbook_file.c_str(),NULL,NULL, SW_SHOWNORMAL);
            }
            else {
                if (strcmp(accion,"salva")) {
					 ::ShellExecute(NULL, "open",workbook_file.c_str(),NULL,NULL, SW_SHOWNORMAL);
                }
            }
		}
	}
}


static CTrasmite* pTrasmite = NULL;	

void PresentaProgreso(int entero,void *puntero)
{
   char tmp[512];
   static CDlgProgreso dlgp;
   static long ticks;
   static BOOL creado = FALSE;
   static long rango = 0L;
   // 0 inicio
   // 1 progreso
   // 2 cierre

   if (!puntero && entero == 999)
   {
	   if (creado)
	   {
		   theApp.m_pMainWnd->KillTimer(IDD_PROGRESO);
	   }
	   if (creado && rango)
	   {
		   entero = 1;
		   puntero = (void *)tmp;
		   *((long *)puntero) = 0L;
	   }
	   else
		   return;
   }

   if (pTrasmite)
   {
      //CProgressCtrl
      if (!entero)
         pTrasmite->m_tprogress.SetRange32( 0 , (int)*((long *)puntero) );
      else
         if (entero == 1)
            pTrasmite->m_tprogress.SetPos( (int)*((long *)puntero) );
         else
            if (entero == 2)
               pTrasmite->m_tprogress.SetPos( (int)0 );
            return;
   }
   
   if (!entero)
   {
	  if (creado)
	  {
		  if (!rango)
		  {
			  dlgp.DestroyWindow();
		  }
		  theApp.m_pMainWnd->KillTimer(IDD_PROGRESO);
	  }
	  ticks = ::GetTickCount();
	  rango = *((long *)puntero);
	  creado = FALSE;
	  theApp.m_pMainWnd->SetTimer(IDD_PROGRESO,400,NULL);
   }
   else
      if (entero == 1)
      {
		 long aticks = ::GetTickCount();
		 if ((aticks - ticks) > 200)
		 {
			 if (!creado)
			 {
				 creado = TRUE;
				 dlgp.Create(IDD_PROGRESO,theApp.m_pMainWnd);
				 dlgp.SetWindowText("Transfiriendo");
				 dlgp.m_progreso.SetRange32( 0,rango);
				 dlgp.ShowWindow(SW_SHOW);
				 dlgp.BringWindowToTop();
				 rango = 0L;
				 /* importante despues por el reuso de tmp ...
				 sprintf(tmp,"Envio de %ld",rango);
				 MensajeEnCabecera(0,tmp);
				 */
			 }
			 dlgp.m_progreso.SetPos(*((long *)puntero));
			 sprintf(tmp,"Enviados %ld",*((long *)puntero));
			 MensajeEnCabecera(0,tmp);
			 ticks = aticks;
		 }		 
      }
      else
         if (entero == 2)
         {
		    if (creado)
			{
				tmp[0] = 0;
				MensajeEnCabecera(0,tmp);
				dlgp.DestroyWindow();
				creado = FALSE;
				rango = 0L;
				theApp.m_pMainWnd->KillTimer(IDD_PROGRESO);
			}
         }
}


void ControlTrasmision(int entero,void *puntero)
{	
   char *p = (char *)puntero;
   
   if (!entero)
   {
      if (pTrasmite)
      {
         delete pTrasmite;
         pTrasmite = NULL;
      }
      pTrasmite = new CTrasmite();    
      pTrasmite->ShowWindow(SW_SHOW);
      pTrasmite->m_local = "/";
      pTrasmite->m_remoto = "";
      //pTrasmite
      //CBASEDLG
      *p = 0;
      return;
   }
   else
      if (entero == 1)
      {  
         CTreeCtrl *tree = &(pTrasmite->m_talli);
         
         
         TV_INSERTSTRUCT tvstruct;
         char *p2;
         int continua = 0;
         char **_dir;
         char **__dir;
         HTREEITEM hThisItem;
         
         p += 2;
         
         if (*p == 1)
         {
            p++;
            if (*p)
               AfxMessageBox("No se ha podido recibir");
            
         }
         else
         {
            
            for (p2 = p;*p2 && *p2 != 1;p2++);
            if (*p2)
            {
               continua = 1;
               *p2 = 0;
            }  
            
            
            tree->DeleteAllItems( ); 
            
            tree->ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT); 
            
            pTrasmite->m_remoto = p;
            
            pTrasmite->m_remoto.Replace('\\','/');
            
            if (pTrasmite->m_remoto.Compare( "/" ))
            {
               
               tvstruct.hParent = NULL;
               tvstruct.hInsertAfter = TVI_LAST;
               tvstruct.item.iImage = 0;
               tvstruct.item.iSelectedImage = 0;
               tvstruct.item.pszText = "..";
               tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
               tree->InsertItem(&tvstruct);
               
            }
            
            tvstruct.hParent = NULL;
            tvstruct.hInsertAfter = TVI_LAST;
            tvstruct.item.iImage = 0;
            tvstruct.item.iSelectedImage = 0;
            tvstruct.item.pszText = p;
            tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
            hThisItem = tree->InsertItem(&tvstruct);
            
            pTrasmite->m_remoto = p;
            
            while(continua)
            {
               p = p2 + 1;
               for (p2 = p;*p2 && *p2 != 1;p2++);
               if (*p2)
               {
                  continua = 1;
                  *p2 = 0;	  
               }  
               else
                  continua = 0;
               if (strlen(p))
               {
                  tvstruct.hParent = hThisItem;
                  tvstruct.hInsertAfter = TVI_LAST;
                  tvstruct.item.iImage = 0;
                  tvstruct.item.iSelectedImage = 0;
                  tvstruct.item.pszText = p;
                  tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
                  tree->InsertItem(&tvstruct);
               }
            }
            
            tree->Expand(hThisItem, TVE_EXPAND);
            
         }
         
         
repetir_1:
         
         tree = &(pTrasmite->m_taqui);
         tree->DeleteAllItems( );   
         
         _dir = (char **)(LPCSTR)pTrasmite->m_local;
         
		 if (theApp.m_9wnd)
		 {
			pDsSys->DespachadorInterno9(10,(void *)&_dir);
		 }
		 else
		 {
			pDsSys->DespachadorInterno(10,(void *)&_dir);
		 }
         
         tree->ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT); 
         
         
         if (pTrasmite->m_local.Compare( "/" ))
         {
            
            tvstruct.hParent = NULL;
            tvstruct.hInsertAfter = TVI_LAST;
            tvstruct.item.iImage = 0;
            tvstruct.item.iSelectedImage = 0;
            tvstruct.item.pszText = "..";
            tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
            tree->InsertItem(&tvstruct);
            
         }
         
         tvstruct.hParent = NULL;
         tvstruct.hInsertAfter = TVI_LAST;
         tvstruct.item.iImage = 0;
         tvstruct.item.iSelectedImage = 0;
         tvstruct.item.pszText = (char *)(LPCSTR)pTrasmite->m_local;
         tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
         hThisItem = tree->InsertItem(&tvstruct);
         
         if (_dir)
         {
            for (__dir = _dir;*__dir;__dir++)
            {
               tvstruct.hParent = hThisItem;
               tvstruct.hInsertAfter = TVI_LAST;
               tvstruct.item.iImage = 0;
               tvstruct.item.iSelectedImage = 0;
               tvstruct.item.pszText = *__dir;
               tvstruct.item.mask = TVIF_IMAGE  | TVIF_TEXT | TVIF_SELECTEDIMAGE ;
               tree->InsertItem(&tvstruct);
            }
			if (theApp.m_9wnd)
			{
				pDsSys->DespachadorInterno9(11,(void *)_dir);
			}
			else
			{
				pDsSys->DespachadorInterno(11,(void *)_dir);
			}
         }
         
         tree->Expand(hThisItem, TVE_EXPAND);
         
         
         //pTrasmite->m_talli.InsertItem();
         MSG msg;
         while (!pTrasmite->m_Salir && !pTrasmite->m_Refrescar && ::GetMessage((LPMSG)&msg,pTrasmite->m_hWnd,0,0))
         {
            
            if (!theApp.PreTranslateMessage(&msg) && !::IsDialogMessage(pTrasmite->m_hWnd,&msg))
            {
               TranslateMessage((LPMSG)&msg);
               DispatchMessage((LPMSG)&msg);
            }
         }
         
         if (pTrasmite->m_Salir)
         {
            pTrasmite->DestroyWindow();
            while (PeekMessage((LPMSG)&msg,pTrasmite->m_hWnd,0,0,PM_NOREMOVE) && ::GetMessage((LPMSG)&msg,pTrasmite->m_hWnd,0,0))
            {               
               if (!theApp.PreTranslateMessage(&msg) && !::IsDialogMessage(pTrasmite->m_hWnd,&msg))
               {
                  TranslateMessage((LPMSG)&msg);
                  DispatchMessage((LPMSG)&msg);
               }
            }
            delete pTrasmite;
            pTrasmite = NULL;
            p = (char *)puntero;
            *p = 1;
            p++;
            *p = 0;
         }
         else
         {
            if (pTrasmite->m_Refrescar)
            {
               pTrasmite->m_Refrescar = FALSE;
               if (pTrasmite->m_trasmite.Compare(""))
               {
                  char tmp[512];
                  int i;
                  if (!(pTrasmite->m_alla))
                  {
                     strcpy(tmp,pTrasmite->m_local);
                     strcat(tmp,pTrasmite->m_trasmite);
                     i = strlen(tmp);
                     tmp[i] = 1;
                     i++;
                     strcpy(tmp+i,pTrasmite->m_remoto);
                     strcat(tmp,pTrasmite->m_trasmite);
					 if (theApp.m_9wnd)
					 {
						 if (pDsSys->DespachadorInterno9(12,(void *)tmp))                     
						 {
							AfxMessageBox("No se ha podido enviar");
						 }
					 }
					 else
					 {
						 if (pDsSys->DespachadorInterno(12,(void *)tmp))                     
						 {
							AfxMessageBox("No se ha podido enviar");
						 }
					 }
                     pTrasmite->m_alla = TRUE;
                  }
                  else
                  {
                     strcpy(tmp,pTrasmite->m_remoto);
                     strcat(tmp,pTrasmite->m_trasmite);
                     i = strlen(tmp);
                     tmp[i] = 1;
                     i++;
                     strcpy(tmp+i,pTrasmite->m_local);
                     strcat(tmp,pTrasmite->m_trasmite);
                     
                     p = (char *)puntero;
                     *p = 2;
                     p++;
                     strcpy(p,tmp);
                     return;
                  }
               }
               if (pTrasmite->m_alla)
               {
                  p = (char *)puntero;
                  strcpy(p,(LPCSTR)pTrasmite->m_remoto);
               }
               else
               {
                  goto repetir_1;
               }
            }
         }
}

}


void GestionUsuarios(int entero,void *puntero)
{
char *p = (char *)puntero;

if (!entero )
  {
   CDlgUsuarios dlg;

   dlg.m_users = (LPCSTR)puntero;

   if (dlg.DoModal() == IDOK)	   
   {
	  if (dlg.m_matapid > 0)
	  {
		  *p = 4;
		  *(p+1) = 4;
		  sprintf(p+2,"%ld",dlg.m_matapid);
	  }
	  else if (dlg.m_modosale == 1 && dlg.m_destinos.GetLength() > 0 &&  dlg.m_mensaje.GetLength() > 0)
	  {
		  *p = 2;
		  *(p+1) = 2;
		  strcpy(p+2,dlg.m_destinos);
		  strcat(p+2,"|");
		  strcat(p+2,dlg.m_mensaje);		  
	  }
	  else if (dlg.m_modosale == 2)
	  {
		  *p = 3;
		  *(p+1) = 3;
		  *(p+2) = 0;
	  }
	  else
	  {
		  *p = 0;
	  }
   }
   else
   {      
      *p = 0;
   }
  }
else
  {  
  *p = 0;
  }
}

static int lado1 = 0;
static int lado2 = 0;
int LadoActivo(int lado)
{
   if (lado == 1)
      return(lado1);   
   if (lado == 2)
      return(lado2);
  
   return(0);
}

static char mensaje_estatico0[1024]; 
static char mensaje_estatico1[1024]; // gasta memoria pero impide la basura
static char mensaje_estatico2[1024];
static char mensaje_estatico3[1024];
void MensajeEnCabecera(int lado,char *mensaje)
{
   if (!mensaje)
   {
      if (lado == 1)
      {
         lado1 = 0;
      }
      if (lado == 2)
      {
         lado2 = 0;
      }
      mensaje = "";
   }
   else
   {
      if (lado == 1)
      {
         lado1 = 1;
      }
      if (lado == 2)
      {
         lado2 = 1;
      }
   }
   
   CString s = mensaje;
   
   if (lado == -1 && !mensaje[0])
   { 
      if (lado1)
      {	 
         s.LoadString(IDS_MENSARELA);
      }
      else
         if (lado2)
         {
            s.LoadString(IDS_MENSACLAVE);
         }
         else
         {
            s.LoadString(AFX_IDS_IDLEMESSAGE);
         }
   }   

   /*
   if (lado == 2)  
   {
       lado = 3;
   }
   */


   if (lado == 1)  
   {
      strcpy(mensaje_estatico1,s);
      mensaje = mensaje_estatico1;
   }
   else
   if (lado == 2)  
   {
      strcpy(mensaje_estatico2,s);
      mensaje = mensaje_estatico2;
   }
   else
   if (lado == 3)  
   {
      strcpy(mensaje_estatico3,s);
      mensaje = mensaje_estatico3;
   }   
   else   
   {
      strcpy(mensaje_estatico0,s);
      mensaje = mensaje_estatico0;
      lado = 0;
	  if (theApp.m_mensa)
		theApp.m_mensa->SetWindowText(s);
   }
   
   if (theApp.m_pMainWnd && ::IsWindow(theApp.m_pMainWnd->m_hWnd))
   {
		theApp.m_pMainWnd->SendMessage(WM_COMMAND,MENSAJE_CABECERA_0+lado,(LPARAM)mensaje);
   }
}

BOOL GetFileVersion( char * pFilePath, char * pVersion, UINT uMaxVersionLen )
{
	DWORD	dwHandle = 0;
	DWORD	dwVersionInfoSize;
	DWORD	dwError;
	PVOID	pFileInfo;
	PBYTE	pVersionInfo;
	PDWORD	pTranslation = NULL;
	UINT	uLength = 0;
	char	szString[512] = "";
	LPSTR	szVersionStr;

	dwVersionInfoSize = GetFileVersionInfoSize(
		pFilePath,	/* pointer to filename string */
		& dwHandle );	/* pointer to variable to receive zero */
	if( ! dwVersionInfoSize )
	{
		dwError = GetLastError();
		return FALSE;
	}

	pFileInfo = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize );
	pVersionInfo = (PBYTE)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize );

	if( ! GetFileVersionInfo( pFilePath,	/* pointer to filename string */
		( DWORD ) dwHandle,	/* ignored */
		dwVersionInfoSize,	/* size of buffer */
		pFileInfo ) ) 	/* pointer to buffer to receive file-version info.*/
	{
		dwError = GetLastError();
		HeapFree( GetProcessHeap(), 0, pFileInfo );
		HeapFree( GetProcessHeap(), 0, pVersionInfo );
		return FALSE;
	}

	if( ! VerQueryValue( pFileInfo,	/* address of buffer for version resource */
		TEXT( "\\VarFileInfo\\Translation" ), /* address of value to retrieve */
		(void **)& pTranslation,	/* address of buffer for version pointer */
		& uLength )	/* address of version-value length buffer */
		)
	{
		dwError = GetLastError();
		HeapFree( GetProcessHeap(), 0, pFileInfo );
		HeapFree( GetProcessHeap(), 0, pVersionInfo );
		return FALSE;
	}
	
	wsprintf( szString, "\\StringFileInfo\\%04x%04x\\FileVersion",
		LOWORD( ( DWORD ) * pTranslation ), HIWORD( ( DWORD ) * pTranslation ) );

	if( ! VerQueryValue( pFileInfo,	/* address of buffer for version resource */
		szString, 					/* Key that indicates the value we want to retrieve*/
		( PVOID * ) & szVersionStr,	/* address of buffer for version pointer */
		& uLength )					/* address of version-value length buffer */
		)
	{
		dwError = GetLastError();
		HeapFree( GetProcessHeap(), 0, pFileInfo );
		HeapFree( GetProcessHeap(), 0, pVersionInfo );
		return FALSE;
	}

	if( lstrlen( szVersionStr) >= ( int ) uMaxVersionLen )
		lstrcpyn( pVersion, szVersionStr, uMaxVersionLen - 1 );
	else
		lstrcpy( pVersion, szVersionStr);

	HeapFree( GetProcessHeap(), 0, pFileInfo );
	HeapFree( GetProcessHeap(), 0, pVersionInfo );

	return TRUE;
}

static char _mensajeglobo_[1048];

void DespachoMultiple(int entero,void *puntero)
{
#define MAXPOPUPS 100
	static CDlgProgreso dlgp;
	static CDsPopup *pila_popups[MAXPOPUPS] = {NULL,NULL,};
	static int next_popup = 0;
	static int nsacametes = 0;

	if (entero == 777)
	{
		if (pDsSys->m_dswinsys && pDsSys->m_f9DespachadorInterno)
		{

           theApp.StdView()->ocultaVentanas();		   

		   CDs9Window *ant9 = theApp.m_simul9;
		   BOOL visible = FALSE;	   

		   if (theApp.m_9wnd)
			   ant9 = NULL;

		   if (ant9)
		   {		
			  visible = ant9->m_tab9->IsWindowVisible();
			  if (visible)
				  visible = ant9->IsWindowVisible();
			  if (!visible)
			  {				  

				 ant9->MoveWindow(theApp.m_simul9r);
				 ant9->m_tab9->ShowWindow(SW_SHOW);
				 ant9->ShowWindow(SW_SHOW);

				 theApp.m_ctrlactivo = NULL;
			  }
		   	  theApp.m_simul9 = NULL;
			  theApp.m_9wnd = ant9;
		   }
		   
		   theApp.m_9wnd->SetForegroundWindow();
		   theApp.m_9wnd->SetFocus();
		   pDsSys->DespachadorInterno9(16,puntero);
		   if (ant9)
		   {			  
		      theApp.m_9wnd = NULL;
		      theApp.m_simul9 = ant9;
			  if (!visible)
			  {
				ant9->MoveWindow(CRect(0,0,0,0));
				//ant9->m_tab9->ShowWindow(SW_SHOW);
				ant9->ShowWindow(SW_HIDE);				
			  }
		   }		   
		   theApp.StdView()->muestraVentanas();
		}
		*((char *)puntero) = 0;
	} else if (entero == 18)
	{
		strcpy(_mensajeglobo_,(char *)puntero);
		if (theApp.m_pMainWnd)
		{
			theApp.m_pMainWnd->PostMessage(WM_COMMAND,ID_AVISOGLOBO,(LPARAM)_mensajeglobo_);
			strcpy((char *)puntero,"ok ");
		}
		else
			strcpy((char *)puntero,"error ");
		return;
	}
	else if (entero == 17)
	{
		theApp.m_ayuda_url = (char *)puntero;
		if (!theApp.m_ayuda_url.IsEmpty())
		{
				DWORD dwSize = MAX_PATH;
				TCHAR tchApplicationPath[ MAX_PATH ] = { 0 };
				HRESULT hr = AssocQueryString( 0,
											ASSOCSTR_EXECUTABLE,
											_T( ".html" ),
											_T( "open" ),
											tchApplicationPath,
											&dwSize );

				if( FAILED( hr ))
				{
					::ShellExecute(NULL, "open",theApp.m_ayuda_url,NULL,NULL, SW_SHOWNORMAL);
					strcpy((char *)puntero,theApp.m_ayuda_url);
				}
				else
				{
					char parametros[2048];
					sprintf(parametros,"%s?dirbase=%s&aplicacion=%s&usuario=%s&dstab=%s",(LPCSTR)theApp.m_ayuda_url,theApp.m_dirbase,theApp.m_nomapli,(LPCSTR)pDsSys->m_Usuario,theApp.m_nomtab);
					::ShellExecute(NULL, "open",tchApplicationPath,parametros,NULL, SW_SHOWNORMAL);
					strcpy((char *)puntero,parametros);
				}				
		}
		else
			strcpy((char *)puntero,"");
		return;
	}
	else if (entero == 16)
	{
		HWND mainw = NULL;
		if (theApp.m_pMainWnd)
			mainw = theApp.m_pMainWnd->m_hWnd;
		memcpy(puntero,(void *)&mainw,sizeof(HWND));
		return;
	}
	else if (entero == 15)
	{
		HINSTANCE hinst = AfxGetInstanceHandle( );
		memcpy(puntero,(void *)&hinst,sizeof(HINSTANCE));
		return;
	}
	else if (entero == 14)
	{
		if (theApp.m_LeftView)
		{
			sprintf((char *)puntero,"%d",theApp.m_LeftView->GetCorriente());
		}
	}
	else if (entero == 13)
	{
		int *p1;
		int len;
		int cwid;
		int nl;
		p1 = (int *)puntero;
		len = *p1;p1++;
		cwid = *p1;p1++;
		nl = *p1;p1++;

		CDsStdCtrl *pctrl = NULL;
		if (CDsStdDlg::m_todosctrl.Lookup(cwid,pctrl) && pctrl && (pctrl->m_tipo == 2 || pctrl->m_tipo == 11))
		{
			((CDsCombo *)pctrl->m_ctrl)->PonEnAutocompletar((char *)p1,nl);
		}		
		*(char *)puntero = 0;
	}
	else if (entero == 12)
	{
		int *p1;
		int len;
		int cwid;
		int nl;
		p1 = (int *)puntero;
		len = *p1;p1++;
		cwid = *p1;p1++;
		nl = *p1;p1++;

		CDsStdCtrl *pctrl = NULL;
		if (CDsStdDlg::m_todosctrl.Lookup(cwid,pctrl) && pctrl && (pctrl->m_tipo == 2 || pctrl->m_tipo == 11))
		{
			((CDsCombo *)pctrl->m_ctrl)->EstableceAutocompletar((char *)p1,nl);
		}		
		*(char *)puntero = 0;
	}
	else if (entero == 11)
	{
		int r = -1;
		int n = 0;
		int i;
		int pos = 0;
		char *p1,*p2,*p3;
		UINT ops;
		CMenu menu;
		
		if (menu.CreatePopupMenu( )) 
		{		
			p2 = p1 = (char *)puntero;
			while(*p2 && *p2 != 1) p2++;
			if (*p2 == 1)
			{
				*p2++ = 0;
				pos = atoi(p1);				
			}
			p1 = p2;
			while(*p2 && *p2 != 1) p2++;
			if (*p2 == 1)
			{
				*p2++ = 0;
				n = atoi(p1);				
			}
			p1 = p2;
			i = 0;
			while(*p2 && n)
			{
				if (*p2 == 1)
				{
					*p2++ = 0;
					if (strrchr(p1,2))
					{
						menu.InsertMenu(i,MF_SEPARATOR | MF_BYPOSITION,-1,(LPCSTR)NULL);
					}
					else
					{
						ops = MF_STRING | MF_BYPOSITION;
						if ((p3 = strrchr(p1,3)))
						{
							*p3 = 0;
							ops |= MF_GRAYED;
						}
						menu.InsertMenu(i,ops,i+1,p1);
					}					
					p1 = p2;
					i++;
					n--;
				}
				else
					p2++;								
			}			

			POINT pp;
			if (pos == -6)
			{
				CRect rr;
				pp.x = 0;
				pp.y = 0;
				if (theApp.m_9wnd)
				{
					theApp.m_9wnd->GetWindowRect(&rr);
					pp.x = rr.left;
					pp.y = rr.top;
				}
			}
			else if (pos == -5)
			{
				pp = theApp.m_ultposrdclick;
			}
			else if (pos == -4)
			{
				pp = theApp.m_ultposldclick;
			}
			else if (pos == -3)
			{
				pp = theApp.m_ultposrclick;
			}
			else if (pos == -2)
			{
				pp = theApp.m_ultposlclick;
			}
			else if (pos == -1)
			{
				::GetCaretPos(&pp);				
				ClientToScreen(GetFocus(),&pp);
			}
			else if (!pos)
			{			
				::GetCursorPos(&pp);
			}
			else
			{
				CDsStdCtrl *pctrl;
				if (CDsStdDlg::m_todosctrl.Lookup(pos,pctrl))
				{
					CRect r;
					pctrl->m_ctrl->GetWindowRect(r);
					pp.x = r.left;pp.y = r.top;
				}
				else
				{
					pp.x = 0;
					pp.y = 0;
				}
			}
			r = (int)menu.TrackPopupMenu(TPM_LEFTALIGN| TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y,theApp.m_pMainWnd,NULL);
		}
		sprintf((char *)puntero,"%d",r);
	}
	else if (entero == 9 || entero == 10)
	{
		CDsStdDlg *pdlg = (CDsStdDlg *)theApp.m_9wnd;	

		/*
		if (theApp.m_9wnd->m_tab9 && !::IsWindowVisible(theApp.m_9wnd->m_tab9->m_hWnd))
		{
			::ShowWindow(theApp.m_9wnd->m_tab9->m_hWnd,SW_SHOW);	
		}
		*/        

		POSITION pos = pdlg->m_ctrlst.GetStartPosition( );   
		CDsStdCtrl *pctrl;
		int elwid;		
		int pi;
		int pf;

		pi = ((int)puntero) / 10000;
		pf = ((int)puntero) % 10000;

		if (entero == 9)
		{
			nsacametes++;
		}
		while (pos)
		{
		   pdlg->m_ctrlst.GetNextAssoc( pos, elwid, pctrl );
		   if (pctrl->m_tipo != 12 && pctrl->m_ctrl && ::IsWindow(pctrl->m_ctrl->m_hWnd))
		   {
			   if (entero == 9)
			   {
				   if ((pctrl->m_lpi/100) >= (pi/100) && (pctrl->m_lpf/100) <= (pf/100) &&
                       (pctrl->m_lpi%100) >= (pi%100) && (pctrl->m_lpf%100) <= (pf%100) )
				   {
					   if (::IsWindowVisible(pctrl->m_ctrl->m_hWnd))
					   {					   
						   ::ShowWindow(pctrl->m_ctrl->m_hWnd,SW_HIDE);
						   pctrl->m_nivel9 = nsacametes;
					   }
				   }
			   }
			   else
			   {
				   if (!::IsWindowVisible(pctrl->m_ctrl->m_hWnd) && pctrl->m_nivel9 == nsacametes)
				   {					   
						::ShowWindow(pctrl->m_ctrl->m_hWnd,SW_SHOW);
						pctrl->m_nivel9 = 0;
				   }
			   }
		   }
		}
		if (entero == 10)
		{
			nsacametes--;
		}
	}
	else if (entero == 8)
	{
		char *ptmp = (char *)puntero;
		if (*ptmp == 11)
		{
		  OSVERSIONINFO     verInfo = {0};      
		  verInfo.dwOSVersionInfoSize = sizeof (verInfo);     
		  GetVersionEx(&verInfo); 
		  switch (verInfo.dwPlatformId)     
		  { 
		  case VER_PLATFORM_WIN32_NT: 
			 sprintf(ptmp,"NT %d.%02d %d %s",verInfo.dwMajorVersion,verInfo.dwMinorVersion,verInfo.dwBuildNumber,verInfo.szCSDVersion);
			 break;  
		  case VER_PLATFORM_WIN32_WINDOWS:        
			  if (verInfo.dwMinorVersion)
			  {
				  sprintf(ptmp,"W98 %X %d %s",HIWORD(verInfo.dwBuildNumber),LOWORD(verInfo.dwBuildNumber),verInfo.szCSDVersion);
			  }
			  else
			  {
				sprintf(ptmp,"W95 %X %d %s",HIWORD(verInfo.dwBuildNumber),LOWORD(verInfo.dwBuildNumber),verInfo.szCSDVersion);
			  }
			 break;      
		  }
		}
		else if (*ptmp == 10)
		{
			HKEY hKey = NULL;			
			LONG retv;			
			DWORD Type;	
			DWORD Size;		
			char tmpp[512];
			char *elpath;
			CLSID clsid;
			USES_CONVERSION;
			
			strcpy(tmpp+1,ptmp+1);
			strcpy(ptmp,"NO LOCALIZADO");
			tmpp[0] = 0;			
			elpath = tmpp;

			/*
			if ((retv = RegOpenKeyEx(HKEY_CLASSES_ROOT,"Applications\\WINWORD.EXE\\shell\\Open\\command",0,KEY_READ,&hKey)) == ERROR_SUCCESS)
			{				
				
				Type = REG_DWORD;		
				Size = sizeof(tmpp);
				if (RegQueryValueEx(hKey,NULL,NULL,&Type,(unsigned char *)tmpp,&Size) == ERROR_SUCCESS)
				{
					if (Type != REG_SZ)
					{
						tmpp[0] = 0;
					}
				}
				RegCloseKey(hKey);
			}
			*/
			/*L"Word.Application"*/
		    if (CLSIDFromProgID(T2OLE(tmpp+1),&clsid ) == S_OK)
			{
				LPOLESTR psz = NULL;
				StringFromCLSID(clsid,&psz);
				if (psz)
				{					
					sprintf(tmpp,"CLSID\\%s\\LocalServer32",OLE2T(psz));
   					if ((retv = RegOpenKeyEx(HKEY_CLASSES_ROOT,tmpp,0,KEY_READ,&hKey)) == ERROR_SUCCESS)
					{				
						
						Type = REG_DWORD;		
						Size = sizeof(tmpp);
						if (RegQueryValueEx(hKey,NULL,NULL,&Type,(unsigned char *)tmpp,&Size) == ERROR_SUCCESS)
						{
							if (Type != REG_SZ)
							{
								tmpp[0] = 0;
							}
						}
						RegCloseKey(hKey);
					}
					else
					{
						sprintf(tmpp,"CLSID\\%s\\InprocServer32",OLE2T(psz));
   						if ((retv = RegOpenKeyEx(HKEY_CLASSES_ROOT,tmpp,0,KEY_READ,&hKey)) == ERROR_SUCCESS)
						{				
							
							Type = REG_DWORD;		
							Size = sizeof(tmpp);
							if (RegQueryValueEx(hKey,NULL,NULL,&Type,(unsigned char *)tmpp,&Size) == ERROR_SUCCESS)
							{
								if (Type != REG_SZ)
								{
									tmpp[0] = 0;
								}
							}
							RegCloseKey(hKey);
						}
					}
					CoTaskMemFree(psz);
				}
			}

			if (*elpath == '"')
			{	
				elpath++;
				while(*elpath && *elpath != '"')
					elpath++;
				*elpath = 0;
				elpath = tmpp + 1;
			}
			else
			{				
				while(*elpath && *elpath != ',')
					elpath++;
				*elpath = 0;
				elpath = tmpp;
			}
			if (*elpath == '\\' || (elpath[1] == ':' && elpath[2] == '\\'))
			{
				*ptmp = 0;
				GetFileVersion( elpath, ptmp, 200 );
				strcat(ptmp,",[");
				strcat(ptmp,elpath);
				strcat(ptmp,"]");
			}
		}		
	}
	else if (entero == 2)
	{
		dlgp.Create(IDD_PROGRESO,theApp.m_pMainWnd);
		dlgp.m_progreso.SetRange( 0,atoi((char *)puntero));
		dlgp.ShowWindow(SW_SHOW);
		dlgp.BringWindowToTop();
		/*
        int prev_run = theApp.m_runmode;        
        theApp.m_runmode = 1;
        theApp.Run();
		theApp.Run();
		theApp.Run();
		theApp.Run();
		theApp.Run();
		theApp.Run();
        theApp.m_runmode = prev_run;		
		*/
		*((char *)puntero) = 0;
	} else if (entero == 3)
	{
		dlgp.m_progreso.SetPos(atoi((char *)puntero));
		*((char *)puntero) = 0;
	} else if (entero == 4)
	{
		dlgp.DestroyWindow();
		*((char *)puntero) = 0;
		if (theApp.m_9wnd)
		{
			::SetFocus(theApp.m_9wnd->m_hWnd);
		}
	} else if (entero == 5)
	{		
		/* popup mode */
		if (theApp.m_9wnd) /* solo para 9 de momento */
		{
			 CDsPopup *pwnd = new CDsPopup();			 
			 CRect nr1,nr2;
			 HWND prevfocus = ::GetFocus();

 		     pDsSys->DespachadorInterno9(803,NULL);
#ifndef DSPROFUIS
			 theApp.m_9wnd->m_tab9->GetWindowRect(&nr1);
#else
			 theApp.m_9wnd->m_tab9->GetTabWndRect(nr1);
#endif
			 theApp.m_9wnd->GetWindowRect(&nr2);			 

			 int pi,pf,flags;
			 char tmp[30];
			 pwnd->m_desatendido = atoi((char *)puntero);
			 memcpy(tmp,(char *)puntero+6,4);
			 tmp[4] = 0;
			 pi = atoi(tmp);
			 memcpy(tmp,((char *)puntero)+10,4);
			 tmp[4] = 0;
			 pf = atoi(tmp);
			 memcpy(tmp,((char *)puntero)+14,4);
			 tmp[4] = 0;
			 flags = atoi(tmp);
			 if (!pi || !pf)
			 { /* toda la ventana .. */
				 pi = 101; 
				 pf = 2580;
			 }
			 else if (pi == -1 || pf == -1)
			 { /* maximizar a screen */
				 pi = 101;
				 pf = 2580;
			 }
			 CRect rect;
			 {
				 RECT r;
				 r.left = pi;
				 r.right = 1;
				 r.top  = pf;
				 r.bottom = 0;
				 pDsSys->DespachadorInterno9(7,(void *)&r);
				 rect = r;
			 }
			 theApp.m_9wnd->ClientToScreen(&rect);

			 //rect = nr2;
			 //rect.top = nr1.top;

			 rect.top -= (::GetSystemMetrics(SM_CYSIZEFRAME)+::GetSystemMetrics(SM_CYCAPTION));
			 rect.bottom += ::GetSystemMetrics(SM_CYSIZEFRAME)+200;

			 rect.left -= ::GetSystemMetrics(SM_CXSIZEFRAME);
			 rect.right += ::GetSystemMetrics(SM_CXSIZEFRAME)+400;
		     pwnd->CreateEx(WS_EX_DLGMODALFRAME/*|WS_EX_TOPMOST*/,AfxRegisterWndClass(NULL),((char *)puntero)+18,WS_POPUPWINDOW|WS_CAPTION|WS_CLIPCHILDREN,rect,NULL,NULL);
			 theApp.m_iStdParent = (CWnd *)pwnd;			 

			 pwnd->ScreenToClient(&nr1);

			 CDsTab9 *ptab9 = new CDsTab9();
#ifdef DSPROFUIS9
			 nr1.MoveToXY(0,0);
             ptab9->Create(pwnd,nr1,3621,WS_CLIPSIBLINGS | WS_CHILD | WS_CLIPCHILDREN| WS_TABSTOP | WS_VISIBLE);
#else
			 ptab9->Create(WS_CLIPSIBLINGS | TCS_TABS | WS_CHILD | WS_CLIPCHILDREN ,nr1,pwnd,3621);
#endif
			 ptab9->SetOwner(theApp.StdView());
			 ptab9->DeleteAllItems( );
			 ptab9->SetItemExtra( 0 );
			 {
				  TCITEM Item;
				  Item.mask = TCIF_TEXT | TCIF_PARAM;
				  Item.pszText = "Diagram9.10";
				  Item.cchTextMax = 1;
				  Item.lParam = 1;
				  ptab9->InsertItem(0,&Item);
			 }		 

			 pwnd->m_w2 = theApp.m_9wnd;

			 char datos[512];      
			 int *p = (int *)datos;

			 *p++ = 0;
			 *p++ = 0;
			 *p++ = -1;
			 *p++ = 401;
			 *p++ = 2480;
   
			 char *cp = (char *)p;
			 strcpy(cp,"Diagram9.10");
			 cp += (strlen(cp)+1);
			 *((int *)cp) = 0;
			 cp += sizeof(int);
			 *((int *)cp) = -1;


			   pActiveDoc->ControlDePantallas(-10,(void *)datos);
			   int stdid = *((int *)datos);
			   CDs9Window *dlg9 = (CDs9Window *)theApp.StdView()->LocalizaDlg(stdid, 0, 0,101,2580);
			   ASSERT(dlg9);      
			   ASSERT(dlg9->IsKindOf(RUNTIME_CLASS(CDs9Window)));
			   ASSERT(dlg9 != pwnd->m_w2);

#ifndef DSPROFUIS
			 dlg9->SetParent( ptab9 );
			 ptab9->ScreenToClient(&nr2);
			 dlg9->MoveWindow(nr2);
#else			 
			 dlg9->SetParent( pwnd);			   
			 dlg9->SetOwner(theApp.StdView());
#endif
			 
			 dlg9->m_tab9 = ptab9;
			 theApp.m_9wnd = dlg9;
			 
		     dlg9->m_offx = pwnd->m_w2->m_offx;
			 dlg9->m_offy = pwnd->m_w2->m_offy;
			 dlg9->m_stdx = pwnd->m_w2->m_stdx;
			 dlg9->m_stdy = pwnd->m_w2->m_stdy;			 
			 
			 pDsSys->NuevaVentana9(NULL,FALSE);

#ifdef DSPROFUIS
 			 ptab9->GetTabWndRect(nr1);
			 pwnd->ScreenToClient(&nr1);   
			 nr2.MoveToXY(nr1.left,nr1.bottom);
			 dlg9->MoveWindow(nr2);
			 pwnd->ClientToScreen(&nr2);
			 pwnd->ClientToScreen(&nr1);
			 rect = nr2;
			 rect.top = nr1.top;

			 pwnd->GetClientRect(nr1);
			 pwnd->GetWindowRect(nr2);
			 nr2.bottom = nr2.top+rect.Size().cy + (nr2.Size().cy - nr1.Size().cy) + 2;
			 nr2.right  = nr2.left+rect.Size().cx + (nr2.Size().cx - nr1.Size().cx) + 2;
			 pwnd->MoveWindow(nr2);
#endif

			 pila_popups[next_popup] = pwnd;
			 next_popup++;


			 ptab9->ShowWindow(SW_SHOW);
			 dlg9->ShowWindow(SW_SHOW);			 
			 pwnd->ShowWindow(SW_SHOW);	

			 if ( !(flags%10) )
			 {
				pwnd->m_show = theApp.m_pMainWnd->ShowWindow(SW_HIDE);
			 }
			 else
			 {
				 ::SetFocus(prevfocus);
			 }
			 memcpy(puntero,"00000,",6);
		}
	} else if (entero == 6)
	{
	   if (next_popup > 0)
	   {
		   BOOL show = FALSE;
		   next_popup--;
		   if (theApp.m_9wnd) /* solo para 9 de momento */
		   {
			   CDsTab9 *ptab9 = theApp.m_9wnd->m_tab9;			 
			   CDs9Window *dlg9 = theApp.m_9wnd;			   

			   theApp.m_9wnd = pila_popups[next_popup]->m_w2;			   

			   pDsSys->NuevaVentana9(NULL,FALSE);		   

			   theApp.StdView()->EliminaStdDlg(dlg9->m_wid);

			   show = pila_popups[next_popup]->m_show;

			   char tmp[15];
			   sprintf(tmp,"%05d,",pila_popups[next_popup]->m_desatendido);
			   memcpy(puntero,tmp,6);

			   pila_popups[next_popup]->DestroyWindow();
			   delete pila_popups[next_popup];
			   pila_popups[next_popup] = NULL;

			   pDsSys->DespachadorInterno9(804,NULL);

			   {
			   extern int flag_ignoramus;
			   flag_ignoramus++;
			   }
		   }
		   if (next_popup > 1)
		   {
			   theApp.m_iStdParent = (CWnd *)pila_popups[next_popup-1];
		   }
		   else
		   {			   
			   theApp.m_iStdParent = NULL;
		   }		   
		   if (show)
		   {
			   theApp.m_pMainWnd->ShowWindow(SW_SHOW);
		   }
		   if (theApp.m_9wnd)
		   {
			   theApp.m_9wnd->SetFocus();
		   }
	   }
	} else if (entero == 7)
	{
		if (pDsSys->m_dswinsys && pDsSys->m_fDespachadorInterno)
		{
		   CDs9Window *ant9 = theApp.m_9wnd;
		   BOOL visible = FALSE;
		   CRect ejem;
		   if (ant9)
		   {		
			  visible = ant9->m_tab9->IsWindowVisible();
			  if (!visible)
				  visible = ant9->IsWindowVisible();
			  if (visible)
			  {				  
			     ant9->GetWindowRect(&ejem);				 
				 theApp.StdView()->ScreenToClient(&ejem);
				 theApp.m_simul9r = ejem;
				 ant9->m_tab9->ShowWindow(SW_HIDE);
				 ant9->ShowWindow(SW_HIDE);
				 ant9->MoveWindow(CRect(0,0,0,0));
				 theApp.m_ctrlactivo = NULL;
			  }
		   }
		   theApp.m_simul9 = theApp.m_9wnd;
		   theApp.m_9wnd = NULL;		   
		   /*llama_fOpcion(pDsSys->m_fOpcion);*/
		   pDsSys->DespachadorInterno(16,puntero);
		   theApp.m_9wnd = ant9;
		   theApp.m_simul9 = NULL;
		   if (ant9)
		   {			  
			  if (visible)
			  {
				ant9->MoveWindow(ejem);
				//ant9->m_tab9->ShowWindow(SW_SHOW);
				ant9->ShowWindow(SW_SHOW);				
			  }
		   }		   
		}
		*((char *)puntero) = 0;
	} else if (entero == 1)
	{
		/* ZZZ */
	    CTrazaBuscar dlg(theApp.m_pMainWnd);
		dlg.m_abuscar = (char *)puntero;
		if (dlg.DoModal() == IDOK)
		{
			strcpy((char *)puntero,dlg.m_abuscar);
		}
	}
	else if (!entero)
	{
		 CPrintDialog dlg(FALSE);
		 if (dlg.DoModal() == IDOK)
		 {
			 strcpy((char *)puntero,dlg.GetDeviceName( ) );
		 }
	}
}

void DespachoGetDataControl(int entero,void *puntero,BOOL inlinedata = FALSE)
{
   CDsStdCtrl *pctrl;
   if (CDsStdDlg::m_todosctrl.Lookup(entero,pctrl))
   {
       int *p = (int *)puntero;
       if (pctrl->m_tipo == 2 || pctrl->m_tipo == 11)
       {
		   p[1] = ((CDsCombo *)pctrl->m_ctrl)->GetWindowTextLength( );
		   ((CDsCombo *)pctrl->m_ctrl)->GetWindowText((char *)(p+2),p[0]);
		   {
			  char *kp;
			  int n = 0;
			  for (kp = (char *)(p+2);*kp && n < p[0];kp++)
			  {          
				  ansi_a_oem((unsigned char *)kp);
			  }
		   }
	   }
	   else if (pctrl->m_tipo == 4)
       {
            CTablaW *pw2 = (CTablaW *)pctrl->m_ctrl;
            if (pw2)
            {
				char tmp[512];
				unsigned long reg = pw2->m_tabla.DameIsrecCorriente();
				sprintf(tmp,"%lu",reg);
				p[1] = strlen(tmp)+1;
				strcpy((char *)(p+2),tmp);
            }     
			else
			{
				p[1] = 0;
				((char *)(p+2))[0] = 0;
			}
       }
	   else
	   {		   
		   
		   if (pctrl->m_tipo == 7)
		   {
			   int max = p[0];
			   char *pp = (char *)(p+2);
			   int lineas = ((CDsTextEdit *)pctrl->m_ctrl)->GetLineCount();
			   int l = 0,n,count = 0,idx;

			   while(lineas > 0 && max > 0)
			   {
				   idx = ((CDsTextEdit *)pctrl->m_ctrl)->LineIndex(l);
				   n = ((CDsTextEdit *)pctrl->m_ctrl)->LineLength(idx);
				   if (n > max)
					   n = max;
				   ((CDsTextEdit *)pctrl->m_ctrl)->GetLine(l, pp, n);				   
				   lineas--;
				   l++;
				   pp += n;
				   max -= n;
				   count += n;
				   if (max > 1)
				   {
					   *pp++ = 13;
					   *pp++ = 10;
					   max -= 2;
					   count += 2;
				   }
			   }
			   if (count > 1)
			   {
				   if (*(pp-1) == 10 && *(pp-2) == 13)
				   {
					   pp -= 2;
					   count -= 2;
				   }
			   }
			   *pp = 0;
			   count++;
			   p[1] = count;
			   //((CDsTextEdit *)pctrl->m_ctrl)->FmtLines( TRUE );
		   }
		   else
		   {
				p[1] = pctrl->m_ctrl->GetWindowTextLength( );		   
				pctrl->m_ctrl->GetWindowText((char *)(p+2),p[0]);
		   }		   
		   {
			  char *kp;
			  int n = 0;
			  for (kp = (char *)(p+2);*kp && n < p[0];kp++)
			  {          
				  ansi_a_oem((unsigned char *)kp);
			  }
		   }

	   }
   }
}

void DespachadorLocal(int despacho,int entero,void *puntero)
{   
   if (!pMFrame && despacho != DESPACHO_PROCESAMENSAJES) 
   {
       return;
   }

   switch(despacho)
   {
   case DESPACHO_DATA_PROCESAMENSAJES:
   case DESPACHO_PROCESAMENSAJES:
      {
        int prev_run = theApp.m_runmode;
        int *p1 = (int *)puntero;
		int cwid = -1;
		int maxdata = 0;
		int otro = 0;

		if (despacho == DESPACHO_DATA_PROCESAMENSAJES)
		{
			maxdata = p1[0];
			otro = p1[1];
			cwid = p1[2];
		}		
		if (entero != 2)
			memset(puntero,0,sizeof(int)*10);
		

		{
			int curretro;
			curretro = theApp.m_Cur_Retro_Nivel;
			do
			{
				theApp.m_runmode = entero;
				theApp.Run();
				theApp.m_runmode = prev_run;
				if (theApp.m_ulttecla == 977) { // ignore!!
					theApp.m_ulttecla = -1;
					continue;
				}
				if (curretro == theApp.m_Cur_Retro_Nivel)
					break;
			} while(-1);		
		}		
		if (entero && theApp.m_ulttecla == 999)
		{
			p1[0] = -1;
		}
		else
		{
			p1[0] = theApp.m_ulttecla;
		}
        if (!entero || entero == 1)
        {
			if (theApp.m_ulttecla == TECLA_HINT)
				theApp.m_hintcounter++;
            p1[1] = theApp.m_ult_wid;
            p1[2] = theApp.m_ult_id;
            p1[3] = theApp.m_ult_sid;
            p1[4] = theApp.m_ult_wcid;
            p1[5] = theApp.m_ult_ctipo;                
            p1[6] = theApp.m_ult_cid.m_lcid;
            p1[7] = theApp.m_ult_cid.m_lref;
            p1[8] = theApp.m_hwnd_999;
			theApp.m_hwnd_999 = NULL;
			if (despacho == DESPACHO_DATA_PROCESAMENSAJES)
			{
				memset((char *)(p1+10),0,maxdata);
				if (cwid != -1)
				{
					p1[10] = maxdata;
					p1[11] = otro;
					DespachoGetDataControl(cwid,(void *)(p1+10),TRUE);
				}
			}
        }
		if (!entero || theApp.m_ulttecla != 999)
		{
			theApp.m_ulttecla = -1; 
		}
      }
      break;
   case DESPACHO_OMENU:
	   {
		   FILE *f;
		   char tmpc[512];
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [0]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }
       if (pActiveDoc)
       {
            pActiveDoc->RellenaMenuStd((MenuStruct *)puntero,entero);
       }
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [/0]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }
      break;
   case DESPACHO_EMENU:
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [1]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }
       if (pActiveDoc)
       {
		   pActiveDoc->PonEnWinsysMenu(((entero == 1) ? 1 : 0));
		   if (theApp.m_9wnd != NULL) {
			if (entero == 2)
			{
				theApp.m_9wnd->m_tab9->ShowWindow(SW_SHOW);
				theApp.m_9wnd->ShowWindow(SW_SHOW);
			} else if (entero == 1)
			{
				theApp.m_9wnd->m_tab9->ShowWindow(SW_HIDE);
				theApp.m_9wnd->ShowWindow(SW_HIDE);
			}
			else if (!entero)
			{
				/*theApp.m_9wnd->m_tab9->ShowWindow(SW_SHOW);*/
				theApp.m_9wnd->ShowWindow(SW_SHOW);
			}
		 }
       }
	   {
		   FILE *f;
		   char tmpc[512];		   
		   sprintf(tmpc,"%s/logs/controlmenu.txt",theApp.m_9dirbase);
		   f = fopen(tmpc,"a");
		   if (f)
		   {
			   fprintf(f,"dx: %s [/1]\r\n",pDsSys->m_Usuario);
			   fclose(f);
		   }
	   }
      break;
   case DESPACHO_PANTALLAS:
	   entero %= 10000;
       if (pActiveDoc)
       {
            pActiveDoc->ControlDePantallas(entero,puntero);
       }
      break;
   case DESPACHO_DIF_CCONTROL:
   case DESPACHO_CCONTROL:
       {
		  short id;
		  int prev_id;
          int *p1;          
          int r = -1;
          do
          {  
              p1 = (int *)puntero;                   
			  id = (short)((unsigned int)p1[2] & 0xff);
  		      if (id == 255)
			     id = -1;
			  prev_id = -1;
			  if (!((unsigned int)p1[2] & 0x80000000) && ((unsigned int)p1[2] & 0x40000000))
			  {
				  prev_id = (p1[2] & 0xffffff00);
			  }
              r = pActiveDoc->InsertaControl(p1[1],(int)id,p1[3],p1[4],p1[5],p1[6],(void *)(p1+10),p1[7],p1[8],p1[9],prev_id);
              entero--;
              puntero = (void *)(((char *)puntero) + p1[0]);
          } while (entero >= 0);
       }
      
      break;
   case DESPACHO_DIF_CEDIT:
   case DESPACHO_CEDIT:
       {
		   short id;
		   int prev_id;
		   int n = ((entero & 0xffff0000) >> 16);
           int *p1;
		   entero &= 0xffff;
		   if (n < 1)
			   n = 1;
		   p1 = (int *)puntero;
		   while(n > 0)
		   {			   
			   Cid cid;
			   cid.m_tipo = p1[4];
			   if (cid.m_tipo == 7)
			   {
				   cid.m_lcid = p1[11];
				   cid.m_lref = p1[12];
			   }
			   else
			   {
				   cid.m_lcid = p1[11];
				   cid.m_lref = p1[19];           
			   }

			   id = (short)((unsigned int)p1[2] & 0xff);
	   		   if (id == 255)
				   id = -1;
			   prev_id = -1;
			   if (!((unsigned int)p1[2] & 0x80000000) && ((unsigned int)p1[2] & 0x40000000))
			   {
				  prev_id = (p1[2] & 0xffffff00);
			   }
			   p1[1] = pActiveDoc->ControlEdicion(p1[1],(int)id,p1[3],p1[10],cid,entero,(void *)(p1+4),prev_id);
			   p1 = (int *)(((char *)p1) + p1[0]);
			   n--;
		   }
       }
      break;
   case DESPACHO_GETDATACONTROL:
       // version simplificada y rapida, se accede al control directamente por su wcid
	  DespachoGetDataControl(entero,puntero);
      break;
   case DESPACHO_BINARIO:
      pActiveDoc->ControlBinario(entero,puntero);      
      break;
   case DESPACHO_DIF_FOCOCONTROL:
   case DESPACHO_FOCOCONTROL:
       {
		   if (entero == -1)
		   {
			   theApp.m_ulttecla = -1;
			   break;
		   }
           CDsStdCtrl *pctrl;
           if (CDsStdDlg::m_todosctrl.Lookup(entero,pctrl))
           {
			   pctrl->SetFocus();			   
			   if (despacho == DESPACHO_DIF_FOCOCONTROL && theApp.m_ulttecla == 999 && theApp.m_ult_wcid == pctrl->m_ctrlid)
			   {
				   theApp.m_ulttecla = -1;
			   }			   			   
			   if (pctrl->m_tipo == 11)
			   {
					CDsCombo *pw2 = (CDsCombo *)pctrl->m_ctrl;
					if (pw2 && pw2->m_Edit.m_entabla && pw2->m_Edit.m_entabla->m_mtecla != -1)
					{
						pw2->m_Edit.PostMessage(WM_CHAR,pw2->m_Edit.m_entabla->m_mtecla,MAKELPARAM((WORD)1,(WORD)2));
						/*pw2->TeclaStd(pw2->m_Edit.m_entabla->m_mtecla);*/
						pw2->m_Edit.m_entabla->m_mtecla = -1;
					}            
			   }
			   
           }
       }
      break;
   case DESPACHO_FOCOWINDOW:
      {
          HWND hwnd = (HWND)entero;
          if (::IsWindow(hwnd))
          {
              ::SetFocus(hwnd);
          }
      }
      break;
   case DESPACHO_FIN_CONTROL:
   case DESPACHO_DESTRUYECONTROL:
       {
           CDsStdCtrl *pctrl;
           if (entero == -1 && puntero)
           {
           }
           else
           if (CDsStdDlg::m_todosctrl.Lookup(entero,pctrl))
           {   
			   if (pctrl->m_tipo == 4) {
  				   CTablaW *pw2 = (CTablaW *)pctrl->m_ctrl;
				   if (pw2 && pw2->m_tabla.m_localizador > -1)
				   {
					    theApp.StdView()->SendMessage(WM_COMMAND,ID_AVISO_GRIDMUERE,pw2->m_tabla.m_localizador);							
				   }
				   else
					   pctrl->m_pdlg->DestruyeControl(entero,TRUE);
			   }
			   else {
				  pctrl->m_pdlg->DestruyeControl(entero,TRUE);                   
			   }
           }
       }      
      break;
   case DESPACHO_REFRESCACONTROL:
       {
           CDsStdCtrl *pctrl;
           if (CDsStdDlg::m_todosctrl.Lookup(entero,pctrl))
           {               
               pctrl->m_pdlg->RefrescaControl(pctrl);                   
           }
       }      
      break;
   case DESPACHO_MAINCOMMAND:
      //BOOL OnCmdMsg( (UINT)entero, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
       {
           int *p1;
           p1 = (int *)puntero;
           pMFrame->SendMessage(WM_COMMAND,(WPARAM)entero,(LPARAM)(p1));
       }
      break;
   case DESPACHO_MI_CONTROL:
      if (entero == -1) // control simple
      {
		   short id;
		   int prev_id;
           int *p1;
           p1 = (int *)puntero;

		   id = (short)((unsigned int)p1[0] & 0xff);
		   if (id == 255)
			   id = -1;
		   prev_id = -1;
		   if (!((unsigned int)p1[0] & 0x80000000) && ((unsigned int)p1[0] & 0x40000000))
		   {
				  prev_id = (p1[0] & 0xffffff00);
		   }
           pActiveDoc->ControlDeUsuario((int)id,p1[1],(char *)(p1+8),p1[2],p1[3],(void *)(p1+4),prev_id);
      }      
      break;
   case NUEVA_TECLA_PROGRAMADA:
      pActiveDoc->RefrescaIra();
      break;
   case DESPACHO_FUNCIONES_IMPRESION:
      FuncionesImpresion(entero,puntero);
      break;
   case DESPACHO_TRASMISION:
      ControlTrasmision(entero,puntero);
      break;
   case DESPACHO_USUARIOS:
	  GestionUsuarios(entero,puntero);
	  break;
   case DESPACHO_INICIO_TRAZA:
      CargaTraza(entero,puntero);
      break;
   case DESPACHO_PUNTO_TRAZA:
      PuntoTraza(entero,puntero);
      break;
   case DESPACHO_FIN_TRAZA:
      DescargaTraza(entero,puntero);
      break;
   case DESPACHO_EN_LINEAS:
       {
          int *p1;          
          p1 = (int *)puntero;
          if (entero != 1)
          {
             p1[5] = pActiveDoc->InsertaControl(-1,p1[1],p1[2],5,p1[3],p1[4],(void *)&entero);
          }
          else
          {
             CDsStdCtrl *pctrl;
             if (CDsStdDlg::m_todosctrl.Lookup(p1[1],pctrl))
             {               
                   pctrl->m_pdlg->DestruyeControl(p1[1],TRUE);
             }             
          }
       }      
      break;
   case DESPACHO_TITULO:
	  {
			unsigned char *pp = (unsigned char *)puntero;
			while(*pp)
			{
				oem_a_ansi(pp);
				pp++;
			}			
	  }
      theApp.m_pMainWnd->SetWindowText( (char *)puntero );
	  {
			unsigned char *pp = (unsigned char *)puntero;
			while(*pp)
			{
				ansi_a_oem(pp);
				pp++;
			}			
	  }
      break;	 
   case DESPACHO_MENSAJE_CABECERA:
	  {
			unsigned char *pp = (unsigned char *)puntero;
			while(pp && *pp)
			{
				oem_a_ansi(pp);
				pp++;
			}			
	  }
      MensajeEnCabecera(entero,(char *)puntero);
	  {
			unsigned char *pp = (unsigned char *)puntero;
			while(pp && *pp)
			{
				ansi_a_oem(pp);
				pp++;
			}			
	  }
      break;
   case DESPACHO_COMMANDCONTROL:
      {
           CDsStdCtrl *pctrl;
           if (CDsStdDlg::m_todosctrl.Lookup(entero,pctrl))
           {   
               int *p1 = (int *)puntero;
			   if (pctrl->m_tipo == 2 || pctrl->m_tipo == 11)
			   {
				   CDsCombo *pEdit = (CDsCombo *)pctrl->m_ctrl;
				   pEdit->SendMessage(WM_COMMAND,(WPARAM)p1[1],(LPARAM)(p1+2));
			   }
			   else if (pctrl->m_tipo == 4)
			   {
				   CTablaW *pw2 = (CTablaW *)pctrl->m_ctrl;
				   if (pw2)
				   {
					   pw2->m_tabla.SendMessage(WM_COMMAND,(WPARAM)p1[1],(LPARAM)(p1+3));
				   }
			   }
			   else
			   {
				   pctrl->m_ctrl->SendMessage(WM_COMMAND,(WPARAM)p1[1],(LPARAM)(p1+2));
			   }
           }
      }
      break;
   case DESPACHO_BOTONSALIDA:      
      pActiveDoc->ControlDialogo(entero,(char *)puntero);
      break;
   case DESPACHO_ENLACEWORD:
      ControlWord_(entero,(char *)puntero);
      break;
   case DESPACHO_LISTADO:
      ControlListado_(entero,(char *)puntero);
      break;   
   case DESPACHO_PRE_CONTROL:
	  // La idea es prevenir cambios en el control (ej: cambio de selecion en un tab)
	  // No aplicable de momento en el 10
      //AvisoAControl(entero);
      break;
   case DESPACHO_PROGRESO:
      PresentaProgreso(entero,puntero);
      break;    
   case DESPACHO_BROWSE:
       {
          char *tmp = (char *)puntero;
          BOOL oop = TRUE;
          BOOL solodir = FALSE;
		  char buffer[MAX_PATH];

          if (*tmp == 'X')
          {
			  CDlgBrowse dlg;
			  tmp++;
			  if (*tmp == 'F')
				  dlg.m_solofiles = TRUE;
			  else if (*tmp == 'D')
				  dlg.m_solodirs = TRUE;
			  tmp++;
			  dlg.m_base = tmp;
			  tmp--;
			  tmp--;
			  if (dlg.DoModal() == IDOK)
			  {
				  strcpy(tmp,dlg.m_seleccion);
			  }
			  else
				  strcpy(tmp,"");
              break;
          }
          else if (*tmp == 'S')
          {
              oop = FALSE;
          }
          else if (*tmp == 'D')
          {
              solodir = TRUE;
          }
          tmp++;
          if (solodir)
          {
            LPMALLOC pMalloc;
            SHGetMalloc(&pMalloc);
            

            LPITEMIDLIST pidlRoot;

            
            BROWSEINFO bi;
            bi.hwndOwner = NULL;
            bi.pidlRoot = NULL;
            bi.pszDisplayName = buffer;
            bi.lpszTitle = tmp;
            bi.ulFlags = BIF_EDITBOX;
            bi.lpfn = NULL;

            
            if((pidlRoot = SHBrowseForFolder(&bi)) == NULL)
            {	            
	            strcpy(tmp,"");
            }
            else
            {
                if(SHGetPathFromIDList(pidlRoot, buffer))
                {
	                /*strcpy(tmp,buffer);*/
                    GetShortPathName(buffer,tmp,500);
                }
				else
				{
				    strcpy(tmp,"");
				}
                pMalloc->Free(pidlRoot);
            }            
            pMalloc->Release();
          }
          else
          {
			  CFileDialog dlgFile( oop, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, theApp.m_9wnd );
              //CFileDialog dlgFile(oop);		
			  strcpy(buffer,tmp);
	          dlgFile.m_ofn.lpstrFile = buffer;
              if (dlgFile.DoModal() == IDCANCEL)
              {
                  strcpy(tmp,"");
              }
			  else
			  {
				  /*strcpy(tmp,buffer);*/
				  GetShortPathName(buffer,tmp,500);
			  }
          }
       }
      break;
   case DESPACHO_ESTADOCONTROL:
       {
		   char *p = (char *)puntero;
           CDsStdCtrl *pctrl = NULL;
           if (CDsStdDlg::m_todosctrl.Lookup(entero,pctrl) && pctrl && pctrl->m_ctrl)
           {
			   /*
			   if (pctrl->m_tipo == 2 || pctrl->m_tipo == 11)
			   {
				   CDsCombo *pEdit = (CDsCombo *)pctrl->m_ctrl;
				   
			   }
			   else
			   {

			   }
			   */
			   if (!memcmp(p,"MOVE:",5))
			   {
                   if (pctrl->m_tipo == 14)
				   {
					   int x = 0,y = 0;
					   ((CDsImagen *)pctrl->m_ctrl)->SacaCoordenadas(p+5,x,y);
					   ((CDsImagen *)pctrl->m_ctrl)->MueveScroll(x,y);
				   }
			   }
			   else if (!memcmp(p,"DATA:",5))
			   {
				   {
				      for (char *ipp = (char *)(p+5);*ipp;ipp++)
					  	  oem_a_ansi((unsigned char *)ipp);
				   }
                   if (pctrl->m_tipo == 2 || pctrl->m_tipo == 11)
				   {
					    ((CDsCombo *)pctrl->m_ctrl)->SetWindowText(p+5,TRUE);
				   }
				   else if (pctrl->m_tipo == 7)
				   {
					   ((CDsTextEdit *)pctrl->m_ctrl)->SetWindowText(p+5,TRUE);
				   }
				   else
						pctrl->m_ctrl->SetWindowText(p+5);
			   }
               else if (!strcmp(p,"SHOW"))
			   {
				   pctrl->m_ctrl->ShowWindow(SW_SHOW);
			   }
			   else if (!strcmp(p,"HIDE"))
			   {
				   pctrl->m_ctrl->ShowWindow(SW_HIDE);
			   }
			   else if (!strcmp(p,"ENABLE"))
			   {
				   pctrl->m_ctrl->EnableWindow(TRUE);
			   }
			   else if (!strcmp(p,"DISABLE"))
			   {
				   pctrl->m_ctrl->EnableWindow(FALSE);
			   }
			   else if (!memcmp(p,"CUSTOM",6))
			   {
				   if (pctrl && !pctrl->m_muriendo && pctrl->m_ctrl && ::IsWindow(pctrl->m_ctrl->m_hWnd))
				   {
					 ::SendMessage(pctrl->m_ctrl->m_hWnd,WM_COMMAND,ID_CUSTOM_ESTADO,(LPARAM)atol(p+6));
				   }
			   }
			   else if (!memcmp(p,"GENERIC",7))
			   {
				   ::SendMessage(pctrl->m_ctrl->m_hWnd,(UINT)atoi(p+7),NULL,NULL);
			   }
           }
       }	  
	  break;
   case DESPACHO_SUPERUSUARIO:
	  {
		   if (entero == 4)
		   {
				CDlgPregunta prg;
				prg.m_cualquiera = TRUE;
				prg.m_laclave = "";
				prg.m_clave = "";
				if (prg.DoModal() != IDOK)
				{
					((char *)puntero)[0] = 1;
					((char *)puntero)[1] = 0;
				}
				else
				{
					char tmp0[12];
					sprintf(tmp0,"%-10.10s",(LPCSTR)prg.m_clave);
					strcpy((char *)puntero,tmp0);
				}
		   }
		   else
		   {
			CMenu *main = ((CDsxFrame*)theApp.m_pMainWnd)->GetMenu();
			if (main)
			{		
				CMenu *popup = main->GetSubMenu( 0 );
				if (popup)
				{
					if (!entero)
					{
						theApp.m_superusuario = TRUE;
						//popup->CheckMenuItem(  TECLA_SUPERUSUARIO,  MF_BYCOMMAND | MF_CHECKED);
						popup->InsertMenu( 15, MF_BYPOSITION | MF_STRING, (UINT)PIDO_SUPERBLOQUEO , "Bloquear Opcion");
						popup->InsertMenu( 16, MF_BYPOSITION | MF_STRING, (UINT)PIDO_SUPERDESBLOQUEO , "Desbloquear Opcion");
						popup->InsertMenu( 17, MF_BYPOSITION | MF_STRING, (UINT)PIDO_SUPERTODO , "Bloqueo Global");
						popup->InsertMenu( 18, MF_BYPOSITION | MF_STRING, (UINT)PIDO_SUPERDESTODO , "Anular Bloqueo Global");
						popup->InsertMenu( 19, MF_BYPOSITION | MF_STRING, (UINT)PIDO_SUPERCAMBIO , "Cambiar Clave SuperUsuario");
					}
					else if (entero == 1)
					{
						theApp.m_superusuario = FALSE;
						//popup->CheckMenuItem(  TECLA_SUPERUSUARIO,  MF_BYCOMMAND | MF_UNCHECKED);
						popup->RemoveMenu((UINT)PIDO_SUPERBLOQUEO,MF_BYCOMMAND);
						popup->RemoveMenu((UINT)PIDO_SUPERDESBLOQUEO,MF_BYCOMMAND);
						popup->RemoveMenu((UINT)PIDO_SUPERCAMBIO,MF_BYCOMMAND);
						popup->RemoveMenu((UINT)PIDO_SUPERTODO,MF_BYCOMMAND);
						popup->RemoveMenu((UINT)PIDO_SUPERDESTODO,MF_BYCOMMAND);
					}
					else if (entero == 2)
					{
						CDlgPregunta prg;
						prg.m_laclave = (char *)puntero;
						prg.m_clave = "";
						if (prg.DoModal() != IDOK)
						{
							if (prg.m_conok)
							{
								AfxMessageBox("Clave Incorrecta!");
							}
							*((char *)puntero) = 0;
						}
						else
						{
							strcpy((char *)puntero,"\1\1\1OK!\1\1\1");
						}
					}
					else if (entero == 3)
					{
						char *p = (char *)puntero;
						if (p[0] == 1 || p[0] == 2)
						{
							int punto = 0;
							if (pActiveDoc && pActiveDoc->m_menustd)
							{
								CTreeCtrl *m_tree = pActiveDoc->m_menustd;
								TV_ITEM tv;
								tv.hItem = m_tree->GetSelectedItem( );
								tv.mask = TVIF_HANDLE;	
								if (m_tree->GetItem(&tv))
								{       
									punto = (int)tv.lParam;
								}							 
							}
							sprintf((char *)puntero,"%d",punto);
						}
						else if (p[0] == 3)
						{
						CDlgCambiaClave prg;					   
						prg.m_clave = "";
						prg.m_cclave = "";
						if (prg.DoModal() != IDOK)
						{
							*((char *)puntero) = 0;
						}
						else
						{
							strcpy((char *)puntero,prg.m_clave);
						}
						}
					}
				}
			 }
		   }
	  }	  
	  break;
   case DESPACHO_EURO:
	{
	   if (!puntero)
  			puntero = "Ver Euros";

	   CMenu *main = ((CDsxFrame*)theApp.m_pMainWnd)->GetMenu();
	   if (main)
	   {		
		  CMenu *popup = main->GetSubMenu( 2 );
		  if (popup)
		  {
			 if (!entero)
			 {
				 popup->RemoveMenu((UINT)PIDO_VEREURO,MF_BYCOMMAND);
				 if (!popup->InsertMenu( -1, MF_BYPOSITION | MF_STRING, (UINT)PIDO_VEREURO , (char *)puntero))
				 {
					 //AfxMessageBox("SIn iopcion!");
				 }				 
			 }
			 else if (entero == 1)
			 {
				 popup->RemoveMenu((UINT)PIDO_VEREURO,MF_BYCOMMAND);
			 }
			 else if (entero == 2)
			 {
				 popup->CheckMenuItem(  PIDO_VEREURO,  MF_BYCOMMAND | MF_CHECKED);
			 }
			 else if (entero == 3)
			 {
				 popup->CheckMenuItem(  PIDO_VEREURO,  MF_BYCOMMAND | MF_UNCHECKED);
			 }
		  }
	   }
	}
    break;
   case DESPACHO_MENUITEM:
	{
	    char tmpr[257];
		tmpr[0] = 0;
		if (!entero && puntero)
		{
			CMenu *main = ((CDsxFrame*)theApp.m_pMainWnd)->GetMenu();
			if (main)
			{
				int i = 0;
				int pos1;
				int pos2;
				int action;
				char *p = (char *)puntero;
				char *p1;
				for (p1 = p;*p1;p1++)
				{
					if (*p1 == ',')
					{
						*p1 = 0;
						switch(i)
						{
						case 0:
							pos1 = atoi(p);
							break;
						case 1:
							pos2 = atoi(p);
							break;
						}
						i++;
						p = p1 + 1;
					}
				}
				action = atoi(p);
				CMenu *popup = main->GetSubMenu( pos1 );
				if (popup)
				{
					UINT rr;
					if (pos2 < 0)
					{
						rr = popup->GetMenuState( (UINT)(-pos2),MF_BYCOMMAND );
					}
					else
					{
						rr = popup->GetMenuState( pos2,MF_BYPOSITION);
					}
					if (rr == 0xffffffff)
					{
						strcpy(tmpr,"NOTEXIST");
					}
					else if (action == 1)
					{
						if ((rr & MF_CHECKED))
							strcpy(tmpr,"CHECKED");
						else
							strcpy(tmpr,"UNCHECKED");
						if ((rr & MF_GRAYED))
							strcat(tmpr,",GRAYED");
						if ((rr & MF_SEPARATOR))
							strcat(tmpr,",SEPARATOR");						   
					}
					else if (action == 2)
					{
						if (pos2 < 0)
						{
							popup->GetMenuString((UINT)(-pos2),tmpr,249,MF_BYCOMMAND);
						}
						else
						{
							popup->GetMenuString(pos2,tmpr,249,MF_BYPOSITION);
						}
					}
					else if (action == 3)
					{
						sprintf(tmpr,"%ud",popup->GetMenuItemID( pos2 ));
					}
					else if (!action)
					{
						UINT id;
						if (pos2 < 0)
						{
							id = (UINT)(-pos2);
						}
						else
						{
							id = popup->GetMenuItemID( pos2 );
						}
						if (id > 0)
						{
							theApp.m_pMainWnd->SendMessage(WM_COMMAND,id,NULL);
							strcpy(tmpr,"OK");
						}
					}
				}
			}
		}
		strcpy((char *)puntero,tmpr);
	}
	break;
   case DESPACHO_MULTIPLE:
	   DespachoMultiple(entero,puntero);
	   break;
   case DESPACHO_DATA_INFO:
	   switch(entero)
	   {
	   case 0:
		   theApp.m_nomapli = (LPCSTR)puntero;
		   if (theApp.m_defaultfntcargado.Compare(theApp.m_nomapli)) {
			    CString s = theApp.m_defaultfntcargado;
				if (theApp.CargaFuentes(theApp.m_dirbase,theApp.m_nomapli)) {
					((CDsxFrame *)theApp.m_pMainWnd)->RecargaSkinBin();			   
				}
				if (s.Compare(theApp.m_defaultfntcargado)) {
					if (::getenv("DSXRECARGASKIN"))
						((CDsxFrame *)theApp.m_pMainWnd)->InstalaLook();
					((CDsxDoc *)pMFrame->GetActiveDocument())->UpdateFonts(FALSE);
					if (theApp.m_9wnd) {
						theApp.m_9wnd->Invalidate();
						SetWindowPos(theApp.m_9wnd->m_hWnd, 0, 0, 0, 0, 0,  SWP_FRAMECHANGED | SWP_NOSIZE |SWP_NOMOVE);
						if (theApp.m_9wnd->m_tab9) {
							theApp.m_9wnd->m_tab9->Invalidate();
							SetWindowPos(theApp.m_9wnd->m_tab9->m_hWnd, 0, 0, 0, 0, 0,  SWP_FRAMECHANGED | SWP_NOSIZE |SWP_NOMOVE);
						}
					}
				}
		   }
		   break;
	   case 1:
	   case 10:
		   theApp.m_nomtab = (LPCSTR)puntero;
		   if (theApp.m_defaultfntcargado.Compare(theApp.m_nomtab)) {
			    CString s = theApp.m_defaultfntcargado;
				if (theApp.CargaFuentes(theApp.m_dirbase,theApp.m_nomtab)) {
					((CDsxFrame *)theApp.m_pMainWnd)->RecargaSkinBin();			   
				}
				if (s.Compare(theApp.m_defaultfntcargado)) {
					if (::getenv("DSXRECARGASKIN"))
						((CDsxFrame *)theApp.m_pMainWnd)->InstalaLook();
					((CDsxDoc *)pMFrame->GetActiveDocument())->UpdateFonts(FALSE);
					if (theApp.m_9wnd) {
						theApp.m_9wnd->Invalidate();
						SetWindowPos(theApp.m_9wnd->m_hWnd, 0, 0, 0, 0, 0,  SWP_FRAMECHANGED | SWP_NOSIZE |SWP_NOMOVE);
						if (theApp.m_9wnd->m_tab9) {
							theApp.m_9wnd->m_tab9->Invalidate();
							SetWindowPos(theApp.m_9wnd->m_tab9->m_hWnd, 0, 0, 0, 0, 0,  SWP_FRAMECHANGED | SWP_NOSIZE |SWP_NOMOVE);
						}
					}
				}
		   }
		   break;
	   default:
		   break;
	   }
	   break;
   default:
      break;
   }
}

BOOL CDsxApp::ActivacionPorDefecto(HWND oldw)
{
    if (m_ctrlactivo && m_ctrlactivo->m_ctrl && ::IsWindow(m_ctrlactivo->m_ctrl->m_hWnd))
    { 
	   if (m_ctrlactivo->m_tipo == 9990999 && (!theApp.m_9wnd || theApp.m_dentropython || ((CDsxDoc *)pMFrame->GetActiveDocument())->StatusEnWinsysMenu()))
		   return FALSE;
	   m_ctrlactivo->EnsureVisible();
	   ::SetFocus(m_ctrlactivo->m_ctrl->m_hWnd);	   
	   return TRUE;            
    }
    else if (m_antctrlactivo && m_antctrlactivo->m_ctrl && ::IsWindow(m_antctrlactivo->m_ctrl->m_hWnd))
    {
	   if (m_antctrlactivo->m_tipo == 9990999 && (!theApp.m_9wnd || theApp.m_dentropython || ((CDsxDoc *)pMFrame->GetActiveDocument())->StatusEnWinsysMenu()))
		   return FALSE;
       m_antctrlactivo->EnsureVisible();
       ::SetFocus(m_antctrlactivo->m_ctrl->m_hWnd);
       return TRUE;            
    }
    else if (oldw && ::IsWindow(oldw))
    {
       ::SetFocus(oldw);
       return TRUE;
    }
	if (pMFrame && !((CDsxDoc *)pMFrame->GetActiveDocument())->StatusEnWinsysMenu())
	{
		if (theApp.m_9wnd && ::IsWindow(((CDsStdDlg *)theApp.m_9wnd)->m_hWnd) && !theApp.m_dentropython)
		{
			::SetFocus(((CDsStdDlg *)theApp.m_9wnd)->m_hWnd);
			return TRUE;
		}
	}
    return FALSE;
}

BOOL CDsxApp::TeclaAlActivo(int tecla)
{
    CDsStdCtrl *pw = NULL;
    if (m_ctrlactivo && m_ctrlactivo->m_ctrl && ::IsWindow(m_ctrlactivo->m_ctrl->m_hWnd))
    {
        pw = m_ctrlactivo;
    }
    else if (m_antctrlactivo && m_antctrlactivo->m_ctrl && ::IsWindow(m_antctrlactivo->m_ctrl->m_hWnd))
    {
       pw = m_antctrlactivo;
    }
    if (pw)
    {
        if (pw->m_tipo == 4)
        {
            CTablaW *pw2 = (CTablaW *)pw->m_ctrl;
            if (pw2)
            {
                return pw2->m_tabla.TeclaStd(tecla);
            }            
        }
        return pw->TeclaStd(tecla);
    }
    return FALSE;
}

BOOL CDsSys::Inicializa9()
{
   BMP_BASE_PAR pars;   
   BOOL autopass = TRUE;
   char valor[512];
   
   if (!theApp.StdView())
   {	   
	   return FALSE;
   }

   CDsxDoc *pDoc = theApp.StdView()->GetDocument();

#ifdef DSPROFUIS9   

   /*
    pars.cfondo_seleccion        = pDoc->m_cLabelBk;  
    if (theApp.m_colorset6)
    {
		pars.cfondo_seleccion        = theApp.m_color6;
    }
    pars.ctexto_seleccion        = pDoc->m_cLabelFr;  
	
    if (theApp.m_colorset7)
    {
		pars.ctexto_seleccion        = theApp.m_color7;
    }
		
	if (theApp.m_colorset9)
	{
		pars.cfondillo              = theApp.m_color9;
		pars.ctextillo              = pDoc->m_cEditINFr;		
	}
	else		
	{
		pars.cfondillo              = pDoc->m_cEditINBk;
		pars.ctextillo              = pDoc->m_cEditINFr;		
	}
	pars.ctexto_seleccion        = pars.ctextillo;
	*/	

    pars.cfondillo           = theApp.m_9fondocampo; // fondo campo
    pars.ctextillo           = theApp.m_9textocampo; // texto campo
	pars.cfondo_seleccion    = theApp.m_9fondo; // fondo
    pars.ctexto_seleccion    = theApp.m_9texto;  // texto	
	pars.c_custom_subrayado  = theApp.m_9subrallado;

	/* los fuentes se conservan ... */
	if (theApp.m_9ifont9campo > -1)
		pDoc->m_font9campo = theApp.m_dfont[theApp.m_9ifont9campo];
	if (theApp.m_9ifont9texto > -1)
		pDoc->m_font9texto = theApp.m_dfont[theApp.m_9ifont9texto];
	if (theApp.m_9ifont9rever > -1)
		pDoc->m_font9rever = theApp.m_dfont[theApp.m_9ifont9rever];
	if (theApp.m_9ifont9inten > -1)
		pDoc->m_font9inten = theApp.m_dfont[theApp.m_9ifont9inten];
	if (theApp.m_9ifont9mensa > -1)
		pDoc->m_font9mensa = theApp.m_dfont[theApp.m_9ifont9mensa];
	if (theApp.m_9ifont9boton > -1)
		pDoc->m_font9boton = theApp.m_dfont[theApp.m_9ifont9boton];
	if (theApp.m_9ifont9edita > -1)
		pDoc->m_font9edita = theApp.m_dfont[theApp.m_9ifont9edita];

    pars.campo   = pDoc->m_font9campo;
    pars.reverse = pDoc->m_font9rever;
    pars.intenso = pDoc->m_font9inten;
    pars.texto   = pDoc->m_font9texto;
    pars.mensaje = pDoc->m_font9mensa;
    pars.boton   = pDoc->m_font9boton;       

	pars.color_1 = theApp.m_9color[1];
	pars.color_2 = theApp.m_9color[2];
	pars.color_3 = theApp.m_9color[3];
	pars.color_4 = theApp.m_9color[4];
	pars.color_5 = theApp.m_9color[5];
	pars.color_6 = theApp.m_9color[6];
	pars.color_7 = theApp.m_9color[7];


	pars.color_i1 = theApp.m_9icolor[1];
	pars.color_i2 = theApp.m_9icolor[2];
	pars.color_i3 = theApp.m_9icolor[3];
	pars.color_i4 = theApp.m_9icolor[4];
	pars.color_i5 = theApp.m_9icolor[5];
	pars.color_i6 = theApp.m_9icolor[6];
	pars.color_i7 = theApp.m_9icolor[7];


	pars.color_fondo_1 = theApp.m_9fcolor[1];
	pars.color_fondo_2 = theApp.m_9fcolor[2];
	pars.color_fondo_3 = theApp.m_9fcolor[3];
	pars.color_fondo_4 = theApp.m_9fcolor[4];
	pars.color_fondo_5 = theApp.m_9fcolor[5];
	pars.color_fondo_6 = theApp.m_9fcolor[6];
	pars.color_fondo_7 = theApp.m_9fcolor[7];

	memset(pars.flags_extra,0,sizeof(pars.flags_extra)); 

	  /*
		 if ((pPar->flags_extra[0]&1))
			agitapiz_n1 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);
		 if ((pPar->flags_extra[1]&1))
			agitapiz_n2 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);
		 if ((pPar->flags_extra[2]&1))
			agitapiz_n3 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[3]&1))	
			 bmp_fondo_1 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[4]&1))	
			 bmp_fondo_2 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[5]&1))	
			 bmp_fondo_3 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[6]&1))	
			 bmp_fondo_4 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[7]&1))	
			 bmp_fondo_5 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[8]&1))	
			 bmp_fondo_6 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[9]&1))	
			 bmp_fondo_7 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

		 if ((pPar->flags_extra[10]&1))	
			 bmp_fondo_campo = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);
	  */


	for (int i = 1;i < 4;i++)
	{
		if (!theApp.m_9skinfbmp[i].IsEmpty())
		{
			pars.flags_extra[i-1] |= 1;
		}
	}
	for (i = 1;i < 8;i++)
	{
		if (!theApp.m_9skinabmp[i].IsEmpty())
		{
			pars.flags_extra[i+2] |= 1;
		}
	}

	pars.flags = 2;
#else
   
	if (theApp.m_9ifont9campo > -1)
		pDoc->m_font9campo = theApp.m_dfont[theApp.m_9ifont9campo];
	if (theApp.m_9ifont9texto > -1)
		pDoc->m_font9texto = theApp.m_dfont[theApp.m_9ifont9texto];
	if (theApp.m_9ifont9rever > -1)
		pDoc->m_font9rever = theApp.m_dfont[theApp.m_9ifont9rever];
	if (theApp.m_9ifont9inten > -1)
		pDoc->m_font9inten = theApp.m_dfont[theApp.m_9ifont9inten];
	if (theApp.m_9ifont9mensa > -1)
		pDoc->m_font9mensa = theApp.m_dfont[theApp.m_9ifont9mensa];
	if (theApp.m_9ifont9boton > -1)
		pDoc->m_font9boton = theApp.m_dfont[theApp.m_9ifont9boton];
	if (theApp.m_9ifont9edita > -1)
		pDoc->m_font9edita = theApp.m_dfont[theApp.m_9ifont9edita];

   pars.campo   = pDoc->m_font9campo;
   pars.reverse = pDoc->m_font9rever;
   pars.intenso = pDoc->m_font9inten;
   pars.texto   = pDoc->m_font9texto;
   pars.mensaje = pDoc->m_font9mensa;
   pars.boton   = pDoc->m_font9boton;   

   pars.cfondo_seleccion       = pDoc->m_9cfondo_seleccion;
   pars.ctexto_seleccion       = pDoc->m_9ctexto_seleccion;
   pars.cfondo_campo_seleccion = pDoc->m_9cfondo_campo_seleccion;   
   pars.c_custom_subrayado     = pDoc->m_9c_custom_subrayado;   
   
   pars.cfondillo              = pDoc->m_9cfondillo;
   pars.ctextillo              = pDoc->m_9ctextillo;
   pars.cfondo_seleccion       = pDoc->m_9cfondo_seleccion;
   pars.ctexto_seleccion       = pDoc->m_9ctexto_seleccion;
   pars.cfondo_campo_seleccion = pDoc->m_9cfondo_campo_seleccion;   
   pars.c_custom_subrayado     = pDoc->m_9c_custom_subrayado;     

   pars.flags = 1;
#endif


   char datos[512];      
   int *p = (int *)datos;

   memset(datos,0,sizeof(datos));

   *p++ = 0;
   *p++ = 0;
   *p++ = -1;
   *p++ = 401;
   *p++ = 2480;
   
   strcpy((char *)p,"Diagram9.10");

   pDoc->ControlDePantallas(-10,(void *)datos);

   
   int stdid = *((int *)datos);
   CDs9Window *dlg9 = (CDs9Window *)theApp.StdView()->LocalizaDlg(stdid, 0, 0,101,2580);
   ASSERT(dlg9);      
   ASSERT(dlg9->IsKindOf(RUNTIME_CLASS(CDs9Window)));

   theApp.m_9wnd = dlg9;   

   m_Exe = theApp.m_LineaDeComando.m_exe;
   m_Args = theApp.m_LineaDeComando.m_args;
   ApuntaLineaDeComando(&m_argc,m_argv,100,(char *)m_Exe.GetBuffer(m_Exe.GetLength()+1),(char *)m_Args.GetBuffer(m_Args.GetLength()+1));

   

  m_Usuario = "";

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
               m_Password = p1;
            }
            else		  
               if (*p) *p = 0;
         m_Usuario = tmp;
         }
      }
   }   


   int mira;

   theApp.StdView()->GetClientRect(&(pars.marco));

   /*
   pars.marco.top = 0;
   pars.marco.bottom = 0;
   pars.marco.right = 0;
   pars.marco.left = 0;
   */

   if (!(mira = llama_fInicializa(m_f9Inicializa,m_ds9winsys,dlg9->m_hWnd,m_argc,m_argv,&pars)))
   {      
      return FALSE;	
   }
   if (mira == -10)
   {      
      return FALSE;
   }

   CDsTab9 *pTab9 = new CDsTab9();
   CRect rtab,ctab;

   dlg9->GetWindowRect(&rtab);

   /*  IMPRECISO */
   dlg9->GetClientRect(&ctab);
   dlg9->m_offx = 0;
   dlg9->m_offy = 0;
   dlg9->m_stdx = ctab.Size().cx * 10 / 80;
   dlg9->m_stdy = ctab.Size().cy * 10 / 25;


   ctab = rtab;

   theApp.StdView()->ScreenToClient(&rtab);   

#ifdef DSPROFUIS
#ifdef DSPROFUIS9   
   pTab9->Create(theApp.StdView(),rtab,stdid,WS_CLIPSIBLINGS | WS_CHILD | WS_CLIPCHILDREN| WS_TABSTOP/* | WS_VISIBLE*/);   
   pTab9->GetTabWndRect(rtab);
   theApp.StdView()->ScreenToClient(&rtab);
#else
   pTab9->Create(WS_CLIPSIBLINGS | TCS_TABS | WS_CHILD | WS_CLIPCHILDREN,rtab,theApp.StdView(),stdid);
#endif
#else
   pTab9->Create(WS_CLIPSIBLINGS | TCS_TABS | WS_CHILD | WS_CLIPCHILDREN,rtab,theApp.StdView(),stdid);
#endif
   pTab9->DeleteAllItems( );
   pTab9->SetItemExtra( 0 );
   {
      TCITEM Item;
      Item.mask = TCIF_TEXT | TCIF_PARAM;
      Item.pszText = "Diagram9.10";
      Item.cchTextMax = 1;
      Item.lParam = 1;
      pTab9->InsertItem(0,&Item);
   }

#ifndef DSPROFUIS9
   pTab9->AdjustRect( TRUE , &rtab );
   pTab9->MoveWindow(rtab);
   pTab9->ScreenToClient(&ctab);   
   dlg9->SetParent( pTab9 );   
   dlg9->MoveWindow(ctab);   
#else
   HFONT Font = ::CreateFontIndirect(&pDoc->m_font9boton );
   pTab9->SetFont(CFont::FromHandle(Font));
   if (pTab9->m_stdfont)
		::DeleteObject((HGDIOBJ)pTab9->m_stdfont);
   pTab9->m_stdfont = Font;

   dlg9->SetParent( theApp.StdView() );   
   theApp.StdView()->ScreenToClient(&ctab);   
   ctab.MoveToXY(rtab.left,rtab.bottom);
   dlg9->MoveWindow(ctab);
#endif
   

#ifndef DSPROFUIS9
   //::SetWindowPos(dlg9->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
   //dlg9->ShowWindow(SW_SHOW);
#endif

   pTab9->CenterWindow();

   /*  IMPRECISO o no? */
   dlg9->GetClientRect(&ctab);
   dlg9->m_offx = 0;
   dlg9->m_offy = 0;
   dlg9->m_stdx = ctab.Size().cx * 10 / 80;
   dlg9->m_stdy = ctab.Size().cy * 10 / 25;  
   
   dlg9->EnableToolTips( TRUE );   
   pTab9->EnableToolTips( TRUE );      
         
   if (theApp.m_tooltip)
		theApp.m_tooltip->AddTool( dlg9,LPSTR_TEXTCALLBACK,ctab,IDW_STDDLG+dlg9->m_wid);      

   if (theApp.m_tooltip)
		theApp.m_tooltip->Activate(FALSE);

   theApp.StdView()->SetScrollDs();
#ifndef DSPROFUIS9
   pTab9->ShowWindow(SW_SHOW);
#endif

   dlg9->m_tab9 = pTab9;

   char *ptmp = theApp.m_9dirbase.GetBufferSetLength(300);
   llama_fDespachadorInterno(m_f9DespachadorInterno,14,(void *)ptmp);
   theApp.m_9dirbase.ReleaseBuffer(-1);
   theApp.m_9dirbase.Replace('/','\\');
   theApp.m_dirbase = theApp.m_9dirbase;
   
/*
   {
        char **desc_funs;
        int numero_funs;
		struct
		{
			int nfunciones;
			char **funciones;
		} espacio;		
		
		numero_funs = DespachadorInterno9(1,(void *)&desc_funs);
		DespachadorInterno9(18,(void *)&espacio);
		if (espacio.funciones)
		{
			CString funcion,s;
			for (int i = 10;i < espacio.nfunciones && i < 20;i++)
			{
				if (espacio.funciones[i])
				{
					funcion.Format("%03d",823+i);
					s = theApp.GetProfileString("Funciones",funcion,"");
					if (s.IsEmpty() && espacio.funciones[i][0] == '|')
					{						
						s.Format("%s%c",espacio.funciones[i],10);
						if (desc_funs[i])
						{
							s += desc_funs[i];
						}
						theApp.WriteProfileString("Funciones",funcion,s);
					}
					DespachadorInterno9(19,(void *)espacio.funciones[i]);
				}
			}
			DespachadorInterno9(19,(void *)espacio.funciones);
		}		
   }
*/

   
   {
	  char tmp1[512];
	  theApp.m_rmtoversion = llama_fDespachadorInterno(m_f9DespachadorInterno,20,(void *)tmp1);
	  memcpy(&(theApp.m_numero_agisys),tmp1,sizeof(long));
   }
   theApp.m_fgvid9 = llama_fDespachadorInterno(m_f9DespachadorInterno,25,(void *)&theApp.m_internalbuild9);

	{ 		
		int modobarra = 0;	


		/* Lo manda el serviror al solicitar la ayuda
		strcpy(valor,"URL_AYUDA_F1");
		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			theApp.m_ayuda_url = valor;
			theApp.m_ayuda_url.TrimRight();
		}
		*/

        strcpy(valor,"TABRETURN");
		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			if (atoi(valor))
				theApp.m_tabesreturn = TRUE;
		}

        strcpy(valor,"MINIMIZADO");
		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			if (atoi(valor))
				theApp.m_minimizado = TRUE;
		}

        strcpy(valor,"AUTOCOMPLETAR");
		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			if (atoi(valor))
				theApp.m_conhint = TRUE;
		}		

		strcpy(valor,"AUTOCENTRADO");

		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			if (atoi(valor))
				theApp.m_autocentrado = TRUE;
		}

		strcpy(valor,"AVANCELINEA");

		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			if (!atoi(valor))
				theApp.m_avancelinea = FALSE;
		}

        strcpy(valor,"BUSCA_GENE");
		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			if (atoi(valor))
				haybuscon = TRUE;
			else
				haybuscon = FALSE;
		}
		else
			haybuscon = TRUE;

        strcpy(valor,"EXCEL_ALT");
		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		{
			theApp.alt_excel = atoi(valor);			
		}


		strcpy(valor,"MODO_SIN_BARRAS");

		if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0 && atoi(valor) >= 0)
		{			   
			modobarra = atoi(valor)%10;
			if (modobarra != pMFrame->m_sinbarras)
			{				
				pMFrame->m_sinbarras = modobarra;
				if (!modobarra)
				{
#ifdef DSPROFUIS
					CExtControlBar::ProfileBarStateLoad(
						pMFrame,// some kind of CFrameWnd
						"Diagram Software S.L.", // application registry key (usually company name)
						"DockBars", // application profile name (usually product name)  pApp->m_pszProfileName       // Prof-UIS profile name
						"Diagram"
					);
#endif
/*
					pMFrame->m_wndToolBar.ShowWindow(SW_SHOW);
					pMFrame->m_wndToolMensa.ShowWindow(SW_SHOW);
					pMFrame->m_gridtool.ShowWindow(SW_SHOW);					
					pMFrame->m_wndMenuBar.ShowWindow(SW_SHOW);
					pMFrame->m_toolweb.ShowWindow(SW_SHOW);		

					pMFrame->m_dsmenubar.ShowWindow(SW_HIDE);		
					pMFrame->m_dsmenubar.EnableWindow(FALSE);

					pMFrame->m_wndToolBar.EnableWindow(TRUE);
					pMFrame->m_wndToolMensa.EnableWindow(TRUE);
					pMFrame->m_gridtool.EnableWindow(TRUE);
					pMFrame->m_wndMenuBar.EnableWindow(TRUE);
					pMFrame->m_toolweb.EnableWindow(TRUE);					
*/
				}
				else
				{
					if (modobarra == 3) {
						pMFrame->m_wndToolBar.ShowWindow(SW_SHOW);
						pMFrame->m_gridtool.ShowWindow(SW_SHOW);	
						pMFrame->m_wndMenuBar.ShowWindow(SW_SHOW);
					} else {
						pMFrame->m_wndToolBar.ShowWindow(SW_HIDE);
						pMFrame->m_gridtool.ShowWindow(SW_HIDE);	
						pMFrame->m_wndMenuBar.ShowWindow(SW_HIDE);
					}
					pMFrame->m_wndToolMensa.ShowWindow(SW_HIDE);
					pMFrame->m_toolweb.ShowWindow(SW_HIDE);					
					//pMFrame->m_dsmenubar.ShowWindow(SW_HIDE);

					if (modobarra == 3) {
						pMFrame->m_wndToolBar.EnableWindow(TRUE);
						pMFrame->m_gridtool.EnableWindow(TRUE);					
						pMFrame->m_wndMenuBar.EnableWindow(TRUE);
					} else {
						pMFrame->m_wndToolBar.EnableWindow(FALSE);
						pMFrame->m_gridtool.EnableWindow(FALSE);					
						pMFrame->m_wndMenuBar.EnableWindow(FALSE);
					}
					pMFrame->m_wndToolMensa.EnableWindow(FALSE);
					pMFrame->m_toolweb.EnableWindow(FALSE);
					//pMFrame->m_dsmenubar.EnableWindow(FALSE);

					if (modobarra == 2 || modobarra == 3)
					{
						pMFrame->m_dsmenubar.ShowWindow(SW_HIDE);
						pMFrame->m_dsmenubar.EnableWindow(FALSE);						
					}
					else
					{
						pMFrame->m_dsmenubar.ShowWindow(SW_SHOW);		
						pMFrame->m_dsmenubar.EnableWindow(TRUE);
					}
					pMFrame->m_wndSplitter->SetColumnInfo( 0,0,0);
					if (modobarra == 3) {
						pMFrame->RecalcLayout();

						pMFrame->DockControlBar(&pMFrame->m_wndMenuBar,AFX_IDW_DOCKBAR_TOP,NULL);
						pMFrame->DockControlBar(&pMFrame->m_wndToolBar,AFX_IDW_DOCKBAR_LEFT,NULL);		

						pMFrame->RecalcLayout();
						
						CRect rcLast;
				    
						pMFrame->m_wndToolBar.GetClientRect( &rcLast );
						pMFrame->m_wndToolBar.ClientToScreen( &rcLast );

						CRect rcDock(rcLast.left, rcLast.bottom, rcLast.left+1, rcLast.bottom+1 );
						pMFrame->DockControlBar(&pMFrame->m_gridtool,(UINT)AFX_IDW_DOCKBAR_LEFT,(LPCRECT)&rcDock);

						//pMFrame->RecalcLayout();
					}
				}

				pMFrame->RecalcLayout();
			}
		}
		else
		{
#ifdef DSPROFUIS
			CExtControlBar::ProfileBarStateLoad(
				pMFrame,// some kind of CFrameWnd
				"Diagram Software S.L.", // application registry key (usually company name)
				"DockBars", // application profile name (usually product name)  pApp->m_pszProfileName       // Prof-UIS profile name
				"Diagram"
			);
#endif
           pMFrame->RecalcLayout();
		}


		if (m_Usuario.Compare("batch"))
		{
			strcpy(valor,"PASSWORD");

			if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
			{
				if (atoi(valor))
				{
					m_Password = "";
					autopass = FALSE;
				}
			}
		}

	}  
   
   CString cambio;
   int nintentos = 0;
   BOOL pregusu = TRUE; 
   if (m_Usuario != "" && m_Password != "")
   {
	   pregusu = FALSE; 
   }
   do
   {
		cambio = "";
        if (pregusu)
        {
           CLoginDialog dlg;

		   dlg.m_pregusu = 0;
		   strcpy(valor,"PORTAL");
		   if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		   {
				if (atoi(valor))
				{
					dlg.m_pregusu = 2;
				}
		   }
		   if (m_Usuario != "")
		   {
			   dlg.m_usuario = m_Usuario;
			   if (!dlg.m_pregusu)
				  dlg.m_pregusu = 1;
		   }
		   else
		   {
			   if (dlg.m_pregusu == 2)
			   {
				    AfxMessageBox("Usuario no especificado",MB_OK | MB_ICONHAND);
					llama_fFinaliza(m_f9Finaliza);
					return FALSE;
			   }
			   if (theApp.m_Embebido)
			   {
				   dlg.m_usuario = "DSUSER";
			   }
			   else
			   {
				   dlg.m_usuario = "ds";
			   }           
		   }

           if (dlg.DoModal() != IDOK)
           {
              llama_fFinaliza(m_f9Finaliza);
              return FALSE;
           }
		   if (!dlg.m_usuario.Compare("batch")) {
			   ::MessageBox(NULL,"Usuario no válido para la entrada al sistema","Atencion",MB_OK|MB_ICONHAND);
			   continue;
		   }
           m_Usuario = dlg.m_usuario;
           m_Password= dlg.m_password;                 
		   cambio = dlg.m_cambio;		   
        }

		{
			char usu[128];
			char pass[128];
			char cam[128];

			strcpy(usu,m_Usuario);
			strcpy(pass,m_Password);
			strcpy(cam,cambio);

			if (nintentos== 2) {
				cam[0] = 7;
				cam[1] = 0;
			}
			int rr = llama_fLogin(m_f9Login,/*(char *)(LPCSTR)m_Usuario*/usu,/*(char *)(LPCSTR)m_Password*/pass,/*(char *)(LPCSTR)cambio*/cam);
			if (rr != 1)
			{	   
			//m_Usuario = "";				
			pregusu = TRUE;
				if (rr > 2 && rr < 5) {
					CDlgCPasswd dlgp;
repite_cambio_psw:					
				    dlgp.m_usuario = m_Usuario;
					dlgp.m_password = m_Password;
					if (rr == 3)
						::MessageBox(NULL,"Su clave ha caducado y debe cambiarla por favor.","Atencion",MB_OK|MB_ICONHAND);
					if (rr == 4)
						::MessageBox(NULL,"Su clave no es segura y debe cambiarla por favor. (Debe incluir varios carácteres en mayúsculas, varios números y un mínimo de 8 carácteres)","Atencion",MB_OK|MB_ICONHAND);

					if (dlgp.DoModal() == IDOK) {
						dlgp.m_nuevo.TrimRight();
						dlgp.m_password.TrimRight();
						dlgp.m_confirma.TrimRight();
						m_Password = dlgp.m_password;
						strcpy(pass,m_Password);
						if (dlgp.m_nuevo.IsEmpty())
						{
							::MessageBox(NULL,"No se admite una nueva clave vacía","Atencion",MB_OK|MB_ICONHAND);
							rr = 2;
							goto repite_cambio_psw;
						}
						else if (dlgp.m_nuevo.Compare(dlgp.m_confirma))
						{
							::MessageBox(NULL,"Las claves no coinciden","Atencion",MB_OK|MB_ICONHAND);
							rr = 2;
							goto repite_cambio_psw;
						}
						else if (!dlgp.m_nuevo.Compare(m_Password))
						{
							::MessageBox(NULL,"La nueva clave ha de ser diferente de la anterior","Atencion",MB_OK|MB_ICONHAND);
							rr = 2;
							goto repite_cambio_psw;
						}
						else {							
							cambio = dlgp.m_nuevo;
							strcpy(cam,cambio);
							rr = llama_fLogin(m_f9Login,/*(char *)(LPCSTR)m_Usuario*/usu,/*(char *)(LPCSTR)m_Password*/pass,/*(char *)(LPCSTR)cambio*/cam);
							if (rr == 1) {
								pregusu = FALSE;
								if (!cambio.IsEmpty())
								{
									m_Password=cambio;
								}
								break;
							}
							if (rr > 2 && rr < 5) {
								goto repite_cambio_psw;
							}
						}
					}					
					else {
						return FALSE;
					}
				}

				if (rr == 2) {
					AfxMessageBox("Usuario bloqueado. Consulte a su administrador",MB_OK | MB_ICONHAND);
					return FALSE;
				}

				nintentos++;
				if (rr == 5) {
					if (nintentos > 2)
					{
						AfxMessageBox("El número de intentos ha sido excedido. Usuario bloqueado, consulte a su administrador",MB_OK | MB_ICONHAND);
						return FALSE;
					}
					else
					{
						char tmp[512];
						if (nintentos < 2)
							sprintf(tmp,"Clave incorrecta. A los tres intentos se bloqueará el usuario. Quedan %d intentos.",3-nintentos);
						else
							sprintf(tmp,"Clave incorrecta. A los tres intentos se bloqueará el usuario. Queda %d intento.",3-nintentos);
						AfxMessageBox(tmp,MB_OK | MB_ICONHAND);
					}					
				}
				else {
					AfxMessageBox("Clave incorrecta.",MB_OK | MB_ICONHAND);
					Sleep(500*nintentos);
				}				
			}
			else
				pregusu = FALSE;		

		}


		if (!cambio.IsEmpty())
		{
			m_Password=cambio;
		}
     } while (pregusu);     

	 if (theApp.m_LeftView)
	 {
		 ::SetFocus(theApp.m_LeftView->m_hWnd);
	 }

	 if (autopass)
		theApp.m_password = m_Password;

	 // theApp.m_usarweb = theApp.m_9usarweb; 
	 
	 /*
     if (!theApp.m_Abierto && theApp.GetProfileInt("Posiciones","MAX",0))	   
	     theApp.m_pMainWnd->PostMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		 //theApp.m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);			 
	 */	   

	 
	 return TRUE;// Cambiado para pasar de 9 a 10!!!
}

BOOL CDsSys::Inicializa()
{

   /***************************************************************************/


   if (theApp.GetProfileInt("Posiciones","MAX",0))
		theApp.m_pMainWnd->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);


   if (!theApp.m_bsolo10)
   {
	   if (!Inicializa9())
		   return FALSE;
	   
	   theApp.CargaFunciones();
	   
		{ 
		   char valor[512];
		   strcpy(valor,"SOS");
		   if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
		   {
			   if (!atoi(valor))
			   {
				   theApp.m_haycontrolrmto = FALSE;
			   }
		   }
		}



	   if (!m_dswinsys/*!::getenv("DS10TEST")*/)
	   {
		   llama_fOpcion9(m_f9Opcion);
		   return FALSE;
	   } 
	   
	   /***************************************************************************/
	   BMP_BASE_PAR pars;  
	   /*
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
	   pars.flags = 1;
	   theApp.ColoresDefecto(&pars);
	   */
   
	   //llama_fInicializa(void *fff,HINSTANCE hInst,HWND Ventana,int argc,char **argv,BMP_BASE_PAR *marco);
	   int mira;
	   if (!(mira = llama_fInicializa(m_fInicializa,m_dswinsys,NULL,m_argc,m_argv,&pars)))
	   {
		  /*AfxMessageBox("ATENCION FALLO AL INICIAR 10",MB_OK | MB_ICONHAND);*/
		  llama_fFinaliza(m_fFinaliza);
		  AfxFreeLibrary( m_dswinsys );
		  m_dswinsys = NULL;
		  m_fPreInicializa       = NULL;
		  m_fInicializa          = NULL;
		  m_fFinaliza            = NULL;
		  m_fLogin               = NULL;
		  m_fOpcion              = NULL;
		  m_fLogout              = NULL;
		  m_fDevuelveVentana     = NULL;
		  m_fNuevaVentana        = NULL;
		  m_fPreparaEjecucion    = NULL;
		  m_fDespachadorInterno  = NULL;

	      llama_fOpcion9(m_f9Opcion);
		  return FALSE;	
	   }

	   theApp.m_fgvid = llama_fDespachadorInterno(m_fDespachadorInterno,25,(void *)&theApp.m_internalbuild);

	   {
			char usu[128];
			char pass[128];
			char cam[128];
			
			strcpy(usu,m_Usuario);
			strcpy(pass,m_Password);
			strcpy(cam,"");

			if (m_f9DespachadorInterno)
			{
				char tmp[512];				
				tmp[0] = 0;
				llama_fDespachadorInterno(m_f9DespachadorInterno,27,tmp);
				if (tmp[0]) {
					int n;
					n = strlen(tmp);
					if (n >= 6) {
						n -= 6;
					    char *p = tmp+n;
						while(*p) {
							if (*p < '0' || *p > '9')
								break;
							p++;
						}
						if (!*p) {
							usu[0] = 1;
							memcpy(usu+1,tmp+n,6);
							usu[7] = 1;
							strcpy(usu+8,m_Usuario);
						}
					}
				}
			}

			if (!llama_fLogin(m_fLogin,/*(char *)(LPCSTR)m_Usuario*/usu,/*(char *)(LPCSTR)m_Password*/pass,/*(char *)(LPCSTR)""*/cam))
			{       
				AfxMessageBox("ATENCION PASSWORD para 10 INCORRECTO",MB_OK | MB_ICONHAND);
			}
	   }
	   m_Autentificado = TRUE;

	   llama_fOpcion9(m_f9Opcion); /* siempre entra el 9 primero */
	   return FALSE;	
   }

  BMP_BASE_PAR pars;  
  m_Usuario = "";

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
               m_Password = p1;
            }
            else		  
               if (*p) *p = 0;
         m_Usuario = tmp;
         }
      }
   }

   
   /*
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
   pars.flags = 1;
   theApp.ColoresDefecto(&pars);
   */
   
   //llama_fInicializa(void *fff,HINSTANCE hInst,HWND Ventana,int argc,char **argv,BMP_BASE_PAR *marco);
   
   int mira;
   if (!(mira = llama_fInicializa(m_fInicializa,m_dswinsys,NULL,m_argc,m_argv,&pars)))
   {
      //m_wallowed = NULL;
      return FALSE;	
   }

   if (!theApp.m_numero_agisys)
   {
	    char tmp1[512];
		theApp.m_rmtoversion = llama_fDespachadorInterno(m_fDespachadorInterno,20,(void *)tmp1);
	    memcpy(&(theApp.m_numero_agisys),tmp1,sizeof(long));
   }
  
       CString cambio;
	   char cambiapas[21];
       do
	      {
			cambio = "";
            if (m_Usuario == "")
            {
               CLoginDialog dlg;
               dlg.m_usuario = "ds";
               dlg.m_password = "";
               if (dlg.DoModal() != IDOK)
               {              
                  return FALSE;
               }
               m_Usuario = dlg.m_usuario;
               m_Password= dlg.m_password;                 
			   cambio = dlg.m_cambio;
            }
			if (cambio.IsEmpty())
			{
				cambiapas[0] = 0;
			}
			else
			{
				strcpy(cambiapas,cambio);
			}
            if (!llama_fLogin(m_fLogin,(char *)(LPCSTR)m_Usuario,(char *)(LPCSTR)m_Password,(char *)(LPCSTR)cambiapas))
            {	   
               m_Usuario = "";
               AfxMessageBox("PASSWORD INCORRECTO",MB_OK | MB_ICONHAND);
            }
            else
            {
				if (!cambio.IsEmpty())
				{
					m_Password=cambio;
				}
               m_Autentificado = TRUE;
            }
         } while (m_Usuario == "");
	 if (theApp.m_LeftView)
	 {
		 ::SetFocus(theApp.m_LeftView->m_hWnd);
	 }     

	 theApp.m_9wnd = NULL; /* importante, modo solo 10 */

	 theApp.CargaFunciones();
	{ 
	   char valor[512];
	   strcpy(valor,"SOS");
       if (llama_fDespachadorInterno(m_f9DespachadorInterno,22,(void *)valor) >= 0)
	   {
		   if (!atoi(valor))
		   {
			   theApp.m_haycontrolrmto = FALSE;
		   }
	   }
    }

     return TRUE;
}

BOOL CDsxApp::SaltaFoco(int avance)
{
	if (m_ctrlactivo)
	{
		int actual = m_ctrlactivo->m_ctrlid;
		int nivelfoco = m_ctrlactivo->m_nivelfoco;
		CDsStdCtrl *pctrl;
		if (CDsStdDlg::m_todosctrl.Lookup(actual,pctrl) && pctrl)
		{			
			ASSERT(pctrl == m_ctrlactivo);
			do
			{				
				if (avance > 0)
				{
					if (pctrl->m_ctrlafter)
					{
						pctrl = pctrl->m_ctrlafter;						
					}
					else if (CDsStdCtrl::m_ctrlfirst)
					{
						pctrl = CDsStdCtrl::m_ctrlfirst;
					}
					else if (pctrl->m_ctrlbefore)
					{
						pctrl = pctrl->m_ctrlbefore;
					}
					else
					{
						pctrl = NULL;
					}
					if (pctrl && (pctrl->m_nivelfoco == -1 || pctrl->m_nivelfoco != nivelfoco) && pctrl != m_ctrlactivo)
						continue;
				}
				else
				{
					if (pctrl->m_ctrlbefore)
					{
						pctrl = pctrl->m_ctrlbefore;
					}
					else if (CDsStdCtrl::m_ctrllast)
					{
						pctrl = CDsStdCtrl::m_ctrllast;
					}
					else if (pctrl->m_ctrlafter)
					{
						pctrl = pctrl->m_ctrlafter;
					}
					else
					{
						pctrl = NULL;
					}
					if (pctrl && (pctrl->m_nivelfoco == -1 || pctrl->m_nivelfoco != nivelfoco) && pctrl != m_ctrlactivo)
						continue;
				}

				if (pctrl)
				{
					if (::IsWindow(pctrl->m_cthwnd) && ::IsWindowEnabled(pctrl->m_cthwnd))
					{
						DWORD style = ::GetWindowLong(pctrl->m_cthwnd,GWL_STYLE);
						if ( (style & WS_TABSTOP) && (style & WS_VISIBLE) )
						{
							::SetFocus(pctrl->m_cthwnd);
							return TRUE;
						}						
					}
				}
			} while(pctrl && pctrl != m_ctrlactivo);
		}

	}
	return FALSE;
}

// Esta funcion activa el funcionamiento de edicion del 10 en una entrada
// de 9.
static int ActivarEdita(char *txt, int area)
{
   RECT rect;
   BOOL multilinea = TRUE;
   int tama;
   char *p;   
   int *p0;
   BOOL es_control = FALSE;   
   CDs9Window *padre = theApp.m_9wnd;
   char dato[100240];
   int *p1 = (int *)dato;
   int *px1 = p1+6;   
   int controlid;   
   BOOL controldeusuario = FALSE;
   BOOL noeditar = FALSE;
   int maximolinea = 0;
   int totalineas = 0;
   BOOL emergencia = FALSE;
   CDsxDoc *pDoc = theApp.StdView()->GetDocument();
   static int chapuzaid = -1;
   static int recursion = 0; /* en fin ... */
   

   
   if (!padre) return 0;

   

   memcpy((void*)&tama,(void*)txt,sizeof(int));
   if (tama < 0)
   {
	   totalineas = -tama;
	   totalineas &= 0xFFF0000;
	   totalineas >>= 16;
	   tama = -tama;
	   tama &= ~0xFFF0000;
	   maximolinea = tama / 10;
	   tama = -tama;
	   tama %= 10;
   }
   if (tama < 0)
   {
	   controldeusuario = TRUE;
	   if (tama == -3)
	   {
		   if (chapuzaid > -1)
		   {
			   padre->DestruyeControl(chapuzaid,TRUE);
			   chapuzaid = -1;
		   }
		   return 0;
	   }
	   if (tama == -2)
	   {
		   noeditar = TRUE;
	   }
	   tama = 0;
   }
   if (!tama)
   {
		CDsStdCtrl *pctrl = NULL;
		p1[0] = 7;
		p1[1] = area / 10000;
		p1[2] = area % 10000;
		p1[3] = (int)' ';
		p1[4] = -1;
		p1[5] = -1;	
		px1[0] = -1;
		px1[1] = area; // lcid
		px1[2] = recursion;  // lref
		px1[3] = totalineas;
		px1[4] = 2 | ((maximolinea << 16) & 0xffff0000);
		px1[5] = 0;
		strcpy((char *)(px1+6),txt+sizeof(int));
		controlid = -1;
		if (controldeusuario && chapuzaid > -1)
		{			
			if (padre->m_ctrlst.Lookup(chapuzaid,pctrl))
			{
				controlid = chapuzaid;
			}			
		}		
		if (controlid == -1)
		{
			controlid = padre->InsertaControl(p1[0],p1[1],p1[2],(void *)px1,p1[3],p1[4],p1[5],&pctrl);
		}		
		if (!pctrl)
			return -1;	   
		pctrl->m_protegido = TRUE;
		CDsTextEdit *pEdit = (CDsTextEdit *)pctrl->m_ctrl;
		
		pEdit->m_Font = new CFont();
        pEdit->m_Font->CreateFontIndirect(&(pDoc->m_font9edita));
		pEdit->SetFont(pEdit->m_Font);

		if (chapuzaid != -1)
		{
            for (char *ipp = (char *)(px1+6);*ipp;ipp++)
                oem_a_ansi((unsigned char *)ipp);
            pEdit->SetWindowText((char *)(px1+6));
		}
		//pEdit->m_flag = p0[4];

		rect.left = area / 10000;
  	    rect.right = 0;
		rect.top  = area % 10000;
		rect.bottom = 0;
		pDsSys->DespachadorInterno9(7,(void *)&rect);
		rect.top -= 2;
		rect.left -= 2;
		rect.right += 2;
		rect.bottom += 2;
		pEdit->MoveWindow(&rect);

		  int tecla;
		  short stecla;
		  short sqj;
		if (!noeditar)
		{
		  recursion++;
		  do
		  {
			tecla = 0;//(pEdit->m_flag&3);

			pEdit->SetFocus();
			//pEdit->m_seleccionado = TRUE;
        
			int prev_run = theApp.m_runmode;

			theApp.m_runmode = 1;
			theApp.m_enrun = TRUE;
			pDsSys->DespachadorInterno9(8,(void *)&tecla);
			theApp.m_enrun = FALSE;

			theApp.m_runmode = prev_run;

			sqj = (tecla & 0xffff);
			stecla = ((tecla >> 16) & 0xffff);
			/*
			if (sqj == -1)
			{
				sqj = (short)pEdit->ProcesaPtec((int)stecla);
			}
			*/
			{
				CDsStdCtrl *pctrl = NULL;
				if (!CDsStdDlg::m_todosctrl.Lookup(controlid,pctrl))
				{
					emergencia = TRUE;
					break;
				}
			}        

		  } while(sqj == -1);      
		  recursion--;
		}
		else
			sqj = 0;

		  tecla = (int)sqj;      

		  if (emergencia)
		  {
			  memcpy((void*)txt,(void *)&tecla,sizeof(int));
			  return 0;
		  }
		  
		  CString s;

		  pEdit->FmtLines( TRUE );
    
		  pEdit->GetWindowText(s);       
      
		  memcpy((void*)txt,(void *)&tecla,sizeof(int));
		  strcpy(txt+sizeof(int),s.Mid(0,100000));

		  unsigned char *kp;

		  for (kp = (unsigned char *)txt+sizeof(int);*kp;kp++)
		  {          
			  ansi_a_oem(kp);
		  }
      
		  padre->SetFocus();

		  pctrl->m_protegido = FALSE;

		  if (controldeusuario)
		  {
			  chapuzaid = controlid;
		  }
		  else
		  {
		      padre->DestruyeControl(controlid,TRUE);
		  }

	    return 0;
   }


   p = txt + sizeof(int);   
   p0 = (int *)p;
   p += (sizeof(int)*7);

   //InsertaControl(int tipo, int lpos, int lpf, void *dato,int Atrib,int Cc, int Cf)

    px1[0]   = p0[6];
    px1[1]   = recursion; /* identifica como campos diferentes */
    px1[2]   = p0[0];
    px1[3]   = p0[1]/10;
    px1[4]   = p0[2];
    px1[5]   = 0;
    px1[6]   = p0[3];
    px1[9]   = p0[5];
	px1[10]  = 300; // max qo
    char *pp = (char *)(px1+11);

    if (p0[1] == -1)
    {
	   px1[4] = 10;
	   px1[3] = px1[2];
       //style |= ES_PASSWORD;
    }
	if (px1[3] <= 0 || px1[3] > px1[0])
	{
		px1[3] = px1[2];
	}
	/*
       strcpy(pEdit->m_qi,p);
       p+=(strlen(pEdit->m_qi)+1);
       strcpy(pEdit->m_qs,p);
       p+=(strlen(pEdit->m_qs)+1);
     */
    px1[7] = (strlen(p)+1);
	strcpy(pp,p);
	p += px1[7];
	pp += px1[7];
    px1[8] = (strlen(p)+1);
	strcpy(pp,p);
	p += px1[8];
	pp += px1[8];

	strcpy(pp,p); // qo

	p += (strlen(p)+1);

	int lmensa;
	char mensaje[2048];
	if ( (int)(p - (char *)p0) < tama)
	{
		lmensa = strlen(p)+1;
		strcpy(mensaje,p);
	}	
	else
	{
		lmensa = 1;
		mensaje[0] = 0;
	}
	pp += px1[10];
	

    memcpy((void *)pp,(void *)&lmensa,sizeof(int));
    pp += sizeof(int);

    strcpy(pp,mensaje);
    pp += lmensa;

    multilinea = FALSE;
    
	//InsertaControl(int tipo, int lpos, int lpf, void *dato,int Atrib,int Cc, int Cf,CDsStdCtrl **pdctrl)
	CDsStdCtrl *pctrl = NULL;
	p1[0] = 2;
	p1[1] = area / 10000;
	p1[2] = area % 10000;
	p1[3] = (int)' ';
	p1[4] = -1;
	p1[5] = -1;
    controlid = padre->InsertaControl(/*p1[0]*/2000,p1[1],p1[2],(void *)px1,p1[3],p1[4],p1[5],&pctrl);
	if (!pctrl)
		return -1;
	
	ASSERT(controlid == pctrl->m_ctrlid);

	pctrl->m_protegido = TRUE;

	CDsCombo *pEdit = (CDsCombo *)pctrl->m_ctrl;	

	pEdit->SetFlag(p0[4] & 0xffff);
	pctrl->m_helpflag = (p0[4] >> 16) & 0xffff;

      int tecla;
      short stecla;
      short sqj;
	  recursion++;
      do
      {
        tecla = (pEdit->GetFlag()&3);

		pEdit->SetFocus();
		pEdit->SetSeleccionado(TRUE);
        
		int prev_run = theApp.m_runmode;
		theApp.m_runmode = 1;		
 
		theApp.m_enrun = TRUE;
		pDsSys->DespachadorInterno9(8,(void *)&tecla);
		theApp.m_enrun = FALSE;

		theApp.m_runmode = prev_run;

        sqj = (tecla & 0xffff);
        stecla = ((tecla >> 16) & 0xffff);

		{
			CDsStdCtrl *pctrl = NULL;
			if (!CDsStdDlg::m_todosctrl.Lookup(controlid,pctrl))
			{
				emergencia = TRUE;
				break;
			}
		}        

        if (sqj == -1)
        {
            sqj = (short)pEdit->ProcesaPtec((int)stecla);
        }
      } while(sqj == -1);	  
	  recursion--;


      tecla = (int)sqj;

	  if (emergencia)
	  {
		  memcpy((void*)txt,(void *)&tecla,sizeof(int));
		  strcpy(txt+sizeof(int),(char *)(px1+11));
          padre->SetFocus();	      
		  return 0;
	  }
	  
      CString s;
    
      pEdit->GetWindowText(s);

	  if (pEdit->GetTipo() == 2) {
		   s.Trim();
		   int mes;
		   int ano;
		   int dia;
		   if (s.GetLength() < 10) {
			   int idx;
			   idx = s.Find('.');
			   if (idx < 0) {
					s = "01.01.0000";					
			   }
			   else {
				   int idx2;
				   idx2 = s.Find('.',idx+1);
				   if (idx2 < 0) {
					   s = "01.01.0000";					   
				   }
				   else {					   
					   dia = ::atoi(s.Mid(0,idx));
					   mes = ::atoi(s.Mid(idx+1,idx2-idx-1));
					   ano = ::atoi(s.Mid(idx2+1));
					   if (ano < 100)
						   ano += 2000;
					   s.Format("%02d.%02d.%04d",dia,mes,ano);				       
				   }
			   }
		   }
	  }
      
      memcpy((void*)txt,(void *)&tecla,sizeof(int));
      strcpy(txt+sizeof(int),s.Mid(0,100000));

	  unsigned char *kp;
      if (pEdit->GetTipo() == 3)
      {
          for (kp = (unsigned char *)txt+sizeof(int);*kp;kp++)
          {
              if (*kp == ',') *kp = '.';
          }
      }

      for (kp = (unsigned char *)txt+sizeof(int);*kp;kp++)
      {          
          ansi_a_oem(kp);
      }
      
      padre->SetFocus();

	  pctrl->m_protegido = FALSE;

	  padre->DestruyeControl(controlid);
         
   
   return 0;
}


static int oculta_nivel = 0;
static void oculta_controles()
{	
	CDsStdDlg *pdlg = (CDsStdDlg *)theApp.m_9wnd;		
	

	theApp.m_9wnd->m_focoprevio = ::GetFocus();

	oculta_nivel++;

    POSITION pos = pdlg->m_ctrlst.GetStartPosition( );   
    CDsStdCtrl *pctrl;
    int elwid;
    while (pos)
    {
       pdlg->m_ctrlst.GetNextAssoc( pos, elwid, pctrl );
	   if (pctrl->m_nivel)
		   continue;
	   if (pctrl->m_tipo != 12 && pctrl->m_ctrl && ::IsWindow(pctrl->m_ctrl->m_hWnd))
	   {
		   if (pctrl->m_tipo == 2 || pctrl->m_tipo == 11)
		   {
			   CDsCombo *pEdit = (CDsCombo *)pctrl->m_ctrl;
			   /*
			   if (pEdit->m_metacombo)
			   {
				   pctrl->m_previovisible = pEdit->m_metacombo->ShowWindow(SW_HIDE);
				   if (!pctrl->m_previovisible)
				   {
					   pctrl->m_previovisible = pEdit->ShowWindow(SW_HIDE);
				   }
				   else
				   {
					   pEdit->ShowWindow(SW_HIDE);
				   }
			   }
			   else
			   */
			   {
				   pctrl->m_previovisible = pEdit->ShowWindow(SW_HIDE);
			   }
		   }
		   else
		   {
			   pctrl->m_previovisible = pctrl->m_ctrl->ShowWindow(SW_HIDE);
		   }
		   pctrl->m_nivel = oculta_nivel;
	   }
    }
}

static void muesta_controles()
{	
	CDsStdDlg *pdlg = (CDsStdDlg *)theApp.m_9wnd;	

    POSITION pos = pdlg->m_ctrlst.GetStartPosition( );   
    CDsStdCtrl *pctrl;
    int elwid;
    while (pos)
    {
       pdlg->m_ctrlst.GetNextAssoc( pos, elwid, pctrl );
	   if (pctrl->m_tipo != 12 && pctrl->m_ctrl && ::IsWindow(pctrl->m_ctrl->m_hWnd))	   
	   {
		   if (pctrl->m_nivel == oculta_nivel)
		   {
				if (pctrl->m_previovisible)
				{
				   if (pctrl->m_tipo == 2 || pctrl->m_tipo == 11)
				   {
					   CDsCombo *pEdit = (CDsCombo *)pctrl->m_ctrl;
					   /*
					   if (pEdit->m_metacombo)
					   {
						   pEdit->m_metacombo->ShowWindow(SW_SHOW);
					   }
					   else
					   */
					   {
						   pEdit->ShowWindow(SW_SHOW);
					   }
					   pEdit->SetFocus();
				   }
				   else
				   {
					   pctrl->m_ctrl->ShowWindow(SW_SHOW);
				   }
				}
				pctrl->m_nivel = 0;
		   }
	   }
    }

	oculta_nivel--;
	if (theApp.m_9wnd->m_focoprevio)
	{
		::SetFocus(theApp.m_9wnd->m_focoprevio);
		theApp.m_9wnd->m_focoprevio = NULL;
	}
}

void DespachadorLocal9(int despacho,int entero,void *puntero)
{
	void PonPantallasEnTab(int npan,char *titulo);
	void NotificaPantalla(int pan,char *titulo);
	// Filtrar y convertir despachos de diagram9 a diagram10
	despacho += 500; // filtrado de mensajes no compatibles
	switch(despacho)
	{
	case DESPACHO9_PRETRANSLATE:
		{
			LONG lIdleCount = 0;
			BOOL bIdle = TRUE;
			MSG msg;		
			if (theApp.PreTranslateMessage( (MSG *)puntero ))
			{
				((MSG *)puntero)->hwnd = NULL;
			}			
		    while (bIdle &&
			   !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			{
				// Simular comportamiento standard (sight!)
				if (!theApp.OnIdle(lIdleCount++))
				{
					bIdle = FALSE;
				}
			}			
		}
		break;
	case DESPACHO9_EN_LINEAS:
		break;
    case DESPACHO9_PANTALLAS:
		if (entero == -2 || entero == -3)
		{
			if (theApp.m_9wnd->m_tab9)
			{
				if (entero == -3)
				{
					theApp.m_9wnd->m_tab9->ShowWindow(SW_HIDE);
					theApp.m_9wnd->ShowWindow(SW_HIDE);
				}
				else
				{
					theApp.m_9wnd->m_tab9->ShowWindow(SW_SHOW);
					theApp.m_9wnd->ShowWindow(SW_SHOW);
					//::SetWindowPos(theApp.m_9wnd->m_tab9->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
					//::SetWindowPos(theApp.m_9wnd->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
				}
			}
			else
			{
				if (entero == -3)
				{					
					theApp.m_9wnd->m_tab9->ShowWindow(SW_HIDE);
					theApp.m_9wnd->ShowWindow(SW_HIDE);
				}
				else
				{
					theApp.m_9wnd->m_tab9->ShowWindow(SW_SHOW);
					theApp.m_9wnd->ShowWindow(SW_SHOW);					
				}
			}
		}
		else
			PonPantallasEnTab(entero,(char *)puntero);
        break;
    case DESPACHO9_QUEPANTALLA:
        NotificaPantalla(entero,(char *)puntero);
        break;
   case DESPACHO9_SOBRE_PANTALLA:
	   oculta_controles();
	   /*
      switch(entero)
      {
      case 0:
        SobreponePantalla();
        break;
      case 1:
        DesactivaControles();
        break;
      default:
        break;
      }
	  */
      break;
   case DESPACHO9_REPON_PANTALLA:
	   muesta_controles();
	   /*
      switch(entero)
      {
      case 0:
        ReponPantalla();
        break;
      case 1:
        ActivaControles();
        break;
      default:
        break;
      }
	  */
      break;
	case DESPACHO9_MENSAJE_CABECERA:
		if (entero != -1 || !puntero)
		{
			{
					unsigned char *pp = (unsigned char *)puntero;
					while(pp && *pp)
					{
						oem_a_ansi(pp);
						pp++;
					}			
			}
			MensajeEnCabecera(entero,(char *)puntero);
			{
					unsigned char *pp = (unsigned char *)puntero;
					while(pp && *pp)
					{
						ansi_a_oem(pp);
						pp++;
					}			
			}
		}
		break;
   case DESPACHO9_EDITA:
      ActivarEdita((char *)puntero,(int)entero);
      break;
   case DESPACHO9_MI_CONTROL:
	  DespachadorLocal(DESPACHO_MI_CONTROL,entero,puntero);
	  {
		  int *p1 = (int *)puntero;
		  if (p1[4] >= 0)
		  {
			  *p1 = -(p1[4] + 2);
		  }
	  }
	  break;
   case DESPACHO9_ESTADOCONTROL:
	  if (entero < -1)
	  {
		  entero = (-entero)-2;
	  }
	  DespachadorLocal(DESPACHO_ESTADOCONTROL,entero,puntero);
	  break;
   case DESPACHO9_FIN_CONTROL:
	  if (entero < -1)
	  {
		  entero = (-entero)-2;
	  }
	  DespachadorLocal(DESPACHO_FIN_CONTROL,entero,puntero);
	  break;
   case DESPACHO9_HAZ_VISIBLE: /* para modo relleno background solo se llama para inicializar */
	   if (puntero)
	   {
#ifdef DSPROFUIS9		   
		   CDC dc;
		   CExtBitmap extbmp;
		   CRect rcDst,rcSrc;

		   dc.Attach((HDC)puntero);		   

		   /* segons entero s'han de buscar els diferents backgrounds */		   
		   if (entero >= 0 && entero < 4)
		   {    /* fondo ... calcular rectangulo segun frame, en caso de fallo usar defecto general */
				if (!theApp.m_9skinfbmp[entero].IsEmpty())
				{					
					if (extbmp.LoadBMP_File(theApp.m_9skinfbmp[entero]))
					{		
						pMFrame->GetWindowRect(&rcDst);
						theApp.m_9wnd->GetWindowRect(&rcSrc);
						if (!rcDst.PtInRect(rcSrc.TopLeft()) ||
							!rcDst.PtInRect(rcSrc.BottomRight()))
						{
							theApp.m_9wnd->GetWindowRect(&rcDst);
						}

						theApp.m_9wnd->ScreenToClient(&rcDst);

						CSize _size = extbmp.GetSize();
						rcSrc.SetRect(0,0,_size.cx,_size.cy);

						if (!extbmp.DrawSkinParts(dc.m_hDC,rcDst,rcSrc,CRect(0,0,0,0),(CExtBitmap::e_DrawMethod_t)theApp.m_9skinfmodobmp[entero],true,true))
							pMFrame->m_paintmanager->PaintDockerBkgnd( true, dc, theApp.m_9wnd);
						/*
						if (extbmp.Draw(dc.m_hDC,rcDst,rcSrc) == GDI_ERROR)
							pMFrame->m_paintmanager->PaintDockerBkgnd( true, dc, theApp.m_9wnd);
						*/
					}
					else
						pMFrame->m_paintmanager->PaintDockerBkgnd( true, dc, theApp.m_9wnd);
				}
				else 
					pMFrame->m_paintmanager->PaintDockerBkgnd( true, dc, theApp.m_9wnd);
		   }
		   else if (entero >= 4 && entero < 11)
		   {
			   /* fondo de atributo (color) *//* caso atributos especiales (0 ...) reservado */
			   entero -= 3;
			   if (!theApp.m_9skinabmp[entero].IsEmpty())
			   {					
					if (extbmp.LoadBMP_File(theApp.m_9skinabmp[entero]))
					{
						pMFrame->GetWindowRect(&rcDst);
						theApp.m_9wnd->GetWindowRect(&rcSrc);
						if (!rcDst.PtInRect(rcSrc.TopLeft()) ||
							!rcDst.PtInRect(rcSrc.BottomRight()))
						{
							theApp.m_9wnd->GetWindowRect(&rcDst);
						}

						theApp.m_9wnd->ScreenToClient(&rcDst);

						CSize _size = extbmp.GetSize();
						rcSrc.SetRect(0,0,_size.cx,_size.cy);

						if (!extbmp.DrawSkinParts(dc.m_hDC,rcDst,rcSrc,CRect(0,0,0,0),(CExtBitmap::e_DrawMethod_t)theApp.m_9skinamodobmp[entero],true,true))
						{
							HBRUSH hbr = (HBRUSH)::CreateSolidBrush(theApp.m_9fcolor[entero]);
							::FillRect(dc.m_hDC,&rcDst,hbr);
							::DeleteObject((HGDIOBJ)hbr);
						}
						/*
						if (extbmp.Draw(dc.m_hDC,rcDst,rcSrc) == GDI_ERROR)
						{
							HBRUSH hbr = (HBRUSH)::CreateSolidBrush(theApp.m_9fcolor[entero]);
							::FillRect(dc.m_hDC,&rcDst,hbr);
							::DeleteObject((HGDIOBJ)hbr);
						}
						*/
					}
					else
					{
						HBRUSH hbr = (HBRUSH)::CreateSolidBrush(theApp.m_9fcolor[entero]);
						::FillRect(dc.m_hDC,&rcDst,hbr);
						::DeleteObject((HGDIOBJ)hbr);
					}
			   }
		   }
		   else
		   {
			   /* caso fondo de campo reservado de momento, solo solido a falta de control de edicion apropiado */
		   }		   

#ifdef DSDESCARTES
		   if (!entero)
			   pMFrame->m_paintmanager->PaintDockerBkgnd( true, dc, theApp.m_9wnd);
		   else
		   {	
			    CRect rect1,rect2;

				rect1 = *((RECT *)entero);
				//theApp.m_9wnd->ClientToScreen(&rect1);
				//pMFrame->ScreenToClient(&rect1);	   

				/*
				pMFrame->GetClientRect(&rect2);
				pMFrame->ClientToScreen(&rect2);
				theApp.m_9wnd->ScreenToClient(&rect2);	   
				*/		   
				   
				pMFrame->m_paintmanager->PaintDockerBkgnd(true,dc,rect1,rect1);
		   }
#endif
		   
		   dc.Detach();
  	       
#endif
	   }
	  break;
   default:
		// Pass-throught ... No todos los mensajes son exactamente lo que en 9 pero
		// o funcionan satisfactoriamente o al menos se pueden detectar.
		despacho -= 500; /* Igualar mensajes como de 10 */
		DespachadorLocal(despacho,entero,puntero);
		break;
	}
}


BOOL CDsxApp::PreTranslateMessage(MSG* pMsg) 
{
	if (fnpythonpre)
	{
		if ((*fnpythonpre)(pMsg))
			return TRUE;
	}
	if (m_tooltip && ::IsWindow(m_tooltip->m_hWnd))
	{
		m_tooltip->RelayEvent(pMsg);
	}
	// TODO: Add your specialized code here and/or call the base class
	if (!pMFrame)
		return CWinApp::PreTranslateMessage(pMsg);


	if (pMsg->message == WM_LBUTTONDOWN)
	{
	   m_ultposlclick = CPoint(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
	   ::ClientToScreen(pMsg->hwnd,&m_ultposlclick);
	}
	else if (pMsg->message == WM_RBUTTONDOWN)
	{
		m_ultposrclick = CPoint(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		::ClientToScreen(pMsg->hwnd,&m_ultposrclick);
	}
	else if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		m_ultposldclick = CPoint(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		::ClientToScreen(pMsg->hwnd,&m_ultposldclick);
	}
	else if (pMsg->message == WM_RBUTTONDBLCLK)
	{
		m_ultposrdclick = CPoint(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		::ClientToScreen(pMsg->hwnd,&m_ultposrdclick);
	}

#ifndef DSPROFUIS
    if (pMFrame->m_wndMenuBar.TranslateFrameMessage(pMsg))
        return TRUE;
#endif

	
	/*
	if ((pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_SYSKEYUP) && (pMsg->wParam == VK_F10 || pMsg->wParam == VK_MENU))
	{
		return TRUE;
	}
	*/
	

	if (theApp.m_9wnd && pMsg->hwnd == theApp.m_9wnd->m_hWnd)
	{
		if ((pMsg->message != WM_KEYUP && pMsg->message != WM_KEYDOWN) || pMsg->wParam != VK_ESCAPE)
		{
		   return CWinApp::PreTranslateMessage(pMsg);
		}
		return FALSE;
	}	

	CDsStdCtrl *pctrl = NULL;
    if ((pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN) && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_ctrl && ::IsWindow(theApp.m_ctrlactivo->m_ctrl->m_hWnd) && CDsStdDlg::m_hwndctrllst.Lookup(theApp.m_ctrlactivo->m_ctrl->m_hWnd,pctrl))
    {
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
			return CWinApp::PreTranslateMessage(pMsg);
		}
		else	
			return CWinApp::PreTranslateMessage(pMsg);
	}
	else
		return CWinApp::PreTranslateMessage(pMsg);
}

void WINAPI DespachaInterface(void *parametro)
{
     if (theApp.m_9wnd)
  	     pDsSys->DespachadorInterno9(13,parametro);
	 else
	     pDsSys->DespachadorInterno(13,parametro);    
}

void *AccesoPython(int f,void *p,int *flag)
{
	static _AccesoPython masabajo = NULL;
	void *r = NULL;
	if (!masabajo)
		masabajo = (_AccesoPython)pDsSys->DespachadorInterno(600,NULL);
	switch(f)
	{
	case 4:
		{
			int nivel = (int)p;
			int vflag = (int)flag;
			if (!p && !flag)
			{
				if (theApp.m_ult_wid != -1)
				{
					CDsStdDlg *pdlg = NULL;
					if (theApp.StdView()->m_dlglist.Lookup(theApp.m_ult_wid,pdlg) && pdlg)
					{
						return (void *)pdlg->m_hWnd;
					}					
				}			
			}
			else if (nivel == 1)
			{
				if (vflag == -1)
				{
					if (theApp.StdView())
					{
						CDs9Window *ant9 = theApp.m_9wnd;
						BOOL visible = FALSE;
						if (ant9)
						{		
							visible = ant9->m_tab9->IsWindowVisible();
							if (!visible)
								visible = ant9->IsWindowVisible();
							if (visible)
							{				  
								ant9->m_tab9->ShowWindow(SW_HIDE);
								ant9->ShowWindow(SW_HIDE);
							}
						}
						return (void *)theApp.StdView()->m_hWnd;
					}
				}
			}
			return NULL;			
		}
	case 5:
		{
			int curretro;
			int tecla;
			int prev_run = theApp.m_runmode;		
			_fnpythonpre antp;
			curretro = theApp.m_Cur_Retro_Nivel;
			antp = fnpythonpre;
			fnpythonpre = (_fnpythonpre)p;
			do
			{
				theApp.m_runmode = 0;
				theApp.Run();
				theApp.m_runmode = prev_run;
			} while(curretro != theApp.m_Cur_Retro_Nivel);		
			fnpythonpre = antp;
			tecla = theApp.m_ulttecla;
			theApp.m_ulttecla = -1;
			return (void *)tecla;
		}	
		break;
	case 6:
		{
			theApp.m_ulttecla = (int)p;
			r = 0;
		}
		break;
	case 500:
		r = (void *)NULL;
		break;
	default:
		r = masabajo(f,p,flag);
	}

	return r;
}

void CargaInterface(char *parametro)
{
	static HINSTANCE python_prelib = NULL;
	static HINSTANCE python_lib = NULL;
	HWND wnd = NULL;	
	HINSTANCE lib = NULL;
	parametro += sizeof(long)*3;
	unsigned long *p = (unsigned long *)parametro;
	int extendido =0;
	unsigned long modo;
	int handle;
	int espython = 0;
	char *eje = NULL;
	char *args = NULL;
	void *builtin = NULL;
	

	parametro += (sizeof(int)*2);

	if ((*p & 0x80000000))
	{
		extendido = 1;		
		*p &= ~0x80000000;
	}	
	if ((*p & 0x40000000)) // python remote connection
	{		
		espython = 1;
		eje = parametro;
		while(*eje && *eje != 1) eje++;
		if (*eje == 1)
		{
			*eje = 0;
			eje++;
			args = eje;
			while(*args && *args != 1) args++;
			if (*args == 1)
			{
				char *extra;
				*args = 0;
				args++;
				extra = args;
				while(*extra && *extra != 1) extra++;
				if (*extra == 1)
				{
					*extra = 0;
					//extra++;
				}
			}
			if (!*args)
				args = NULL;
		}
		char *tmpp;
		tmpp = parametro;
		while(*tmpp)
		{
			if (*tmpp == '/')
				*tmpp = '\\';
			tmpp++;
		}
		while(tmpp > parametro && *tmpp != '\\') tmpp--;
		
		if (*tmpp == '\\')
		{
			*tmpp = 0;
			chdir(parametro);
			*tmpp = '\\';
		}
		
		*p &= ~0x40000000;

		if (!python_prelib)
		{
			CString pyprelib;
			pyprelib = theApp.m_dirbase;
			pyprelib += "python/python25.dll";
			pyprelib.Replace('/','\\');
			python_prelib = LoadLibrary(pyprelib);
		}
	}	
	modo = *p;
	handle = p[1];
	if (!(modo&3))
	{
		wnd = ::GetFocus();
		theApp.m_pMainWnd->ShowWindow(SW_HIDE);
	}
	if (espython && python_lib)
		lib = python_lib;
	else
	{	
		/* Built-ins */
		if (!strcmp(parametro,"dscristal.dll"))
		{ 
			void WINAPI DscristalEntryPoint2(DWORD handle,DWORD funcion,DWORD extra);			
			builtin = (void *)DscristalEntryPoint2;
		}
		else
		{
			lib = LoadLibrary(parametro);	
			if (!lib && espython)
			{
				/* traer python.ver y python.tgz del servidor (normalmente debe estar ya) */
				if (theApp.m_9wnd)
  					pDsSys->DespachadorInterno9(26,NULL);
				else
					pDsSys->DespachadorInterno(26,NULL);
				lib = LoadLibrary(parametro);	
			}
		}
	}
	if (lib || builtin)
	{		
        typedef void (WINAPI *_EntryPoint2)(DWORD handle,
					  	          DWORD funcion,
								  DWORD extra
						          ); 

        typedef void (WINAPI *_EntryPoint)(DWORD handle,
					  	          DWORD funcion
						          ); 
		if (extendido)
		{
			_EntryPoint2 EntryPoint2;
			if (builtin)
				EntryPoint2 = (_EntryPoint2)builtin;
			else
				EntryPoint2 = (_EntryPoint2)GetProcAddress(lib,"DsEntryPoint2");
			if (EntryPoint2)
			{
				 DWORD extra;
				 char tmp[1024];
				 int *p1;
				 char *p;
				 int tam = 0;

				 p1 = (int *)tmp;				 
				 *p1 = 1;
				 p1++;
				 *p1 = 0;
				 p1++;
				 *p1 = 0;
				 p = tmp + (sizeof(int)*3);

				 parametro += (strlen(parametro)+1);
				 strcpy(p,parametro);
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);
				 parametro += (strlen(parametro)+1);
				 strcpy(p,parametro);
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);
				 if (theApp.m_CrystalView && modo != 2)
				 {
					 *p = 0;
				 }
				 else
				 {
					 *p = 1;
				 }
				 p++;
				 (*p1)++;

				 CPrintDialog pdlg(FALSE);
	             theApp.GetPrinterDeviceDefaults( &(pdlg.m_pd) );
				 strcpy(p,pdlg.GetDriverName());
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				 strcpy(p,pdlg.GetDeviceName());
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				 strcpy(p,pdlg.GetPortName( ));
				 *p1 += (strlen(p)+1);
				 p += (strlen(p)+1);

				
				 extra = (DWORD)tmp;

				 if (theApp.m_9wnd)
  					 pDsSys->DespachadorInterno9(801,NULL);
				 else
					 pDsSys->DespachadorInterno(801,NULL);    

				 EntryPoint2((DWORD)handle,(DWORD)DespachaInterface,extra);

				 if (theApp.m_9wnd)
  					 pDsSys->DespachadorInterno9(802,NULL);
				 else
					 pDsSys->DespachadorInterno(802,NULL);    
			}
			else
				extendido = 0;
		}
		if (!extendido)
		{
			if (builtin)
			{
				_EntryPoint2 EntryPoint2;
				EntryPoint2 = (_EntryPoint2)builtin;
				if (theApp.m_9wnd)
  					pDsSys->DespachadorInterno9(801,NULL);
				else
					pDsSys->DespachadorInterno(801,NULL);    
				EntryPoint2((DWORD)handle,(DWORD)DespachaInterface,(DWORD)NULL);
				if (theApp.m_9wnd)
  					pDsSys->DespachadorInterno9(802,NULL);
				else
					pDsSys->DespachadorInterno(802,NULL);    
			}
			else
			{
				_EntryPoint EntryPoint = (_EntryPoint)GetProcAddress(lib,"DsEntryPoint");
				if (EntryPoint)
				{
					if (theApp.m_9wnd)
  						pDsSys->DespachadorInterno9(801,NULL);
					else
						pDsSys->DespachadorInterno(801,NULL);    
					EntryPoint((DWORD)handle,(DWORD)DespachaInterface);
					if (theApp.m_9wnd)
  						pDsSys->DespachadorInterno9(802,NULL);
					else
						pDsSys->DespachadorInterno(802,NULL);    
				}
			}
		}
		if (espython)
		{			
			typedef int (WINAPI *_fnDspython)(char *ejecuta,char *pars,void *callback);
			_fnDspython fnDspython;			
			python_lib = lib;
			fnDspython = (_fnDspython)GetProcAddress(lib,"fnDspython");
			if (fnDspython)
			{
				//try
				{
					theApp.m_dentropython++;
					if ((*fnDspython)(eje,args,(void *)AccesoPython) == 9999999)
					{
						FreeLibrary(lib);
						python_lib = NULL;
						if (python_prelib)
							FreeLibrary(python_prelib);
						python_prelib = NULL;
					}				
					theApp.m_dentropython--;
				}
				//catch(...)
				{
				}
			}
		}
		else if (lib)
		  FreeLibrary(lib);
	}
	else
	{
		  char tmpxx[512];
      
		  tmpxx[0] = 0;
		  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),NULL,tmpxx,500,NULL);     
		  AfxMessageBox(tmpxx);
	}
	if (!modo)
	{
		theApp.m_pMainWnd->ShowWindow(SW_SHOW);
		if (wnd)
		{
			::SetFocus(wnd);
		}
	}
}

void CDsSys::NuevaVentana9(CDsxDoc *pDoc,BOOL ajuste)
{
	if (!pDoc)
	{
		pDoc = pActiveDoc;
	}

	if (theApp.m_9wnd)
	{
		  BMP_BASE_PAR pars;	   
		  CWnd *ptmpw = NULL;

			pars.cfondo_seleccion        = pDoc->m_cLabelBk;  
			if (theApp.m_colorset6)
			{
				pars.cfondo_seleccion        = theApp.m_color6;
			}
			pars.ctexto_seleccion        = pDoc->m_cLabelFr;  
			
			if (theApp.m_colorset7)
			{
				pars.ctexto_seleccion        = theApp.m_color7;
			}
				
			if (theApp.m_colorset9)
			{
				pars.cfondillo              = theApp.m_color9;
				pars.ctextillo              = pDoc->m_cEditINFr;		
			}
			else		
			{
				pars.cfondillo              = pDoc->m_cEditINBk;
				pars.ctextillo              = pDoc->m_cEditINFr;		
			}

			// pars.ctexto_seleccion        = pars.ctextillo;
			

			/* los fuentes se conservan ... */
			if (theApp.m_9ifont9campo > -1)
				pDoc->m_font9campo = theApp.m_dfont[theApp.m_9ifont9campo];
			if (theApp.m_9ifont9texto > -1)
				pDoc->m_font9texto = theApp.m_dfont[theApp.m_9ifont9texto];
			if (theApp.m_9ifont9rever > -1)
				pDoc->m_font9rever = theApp.m_dfont[theApp.m_9ifont9rever];
			if (theApp.m_9ifont9inten > -1)
				pDoc->m_font9inten = theApp.m_dfont[theApp.m_9ifont9inten];
			if (theApp.m_9ifont9mensa > -1)
				pDoc->m_font9mensa = theApp.m_dfont[theApp.m_9ifont9mensa];
			if (theApp.m_9ifont9boton > -1)
				pDoc->m_font9boton = theApp.m_dfont[theApp.m_9ifont9boton];
			if (theApp.m_9ifont9edita > -1)
				pDoc->m_font9edita = theApp.m_dfont[theApp.m_9ifont9edita];

			pars.campo   = pDoc->m_font9campo;
			pars.reverse = pDoc->m_font9rever;
			pars.intenso = pDoc->m_font9inten;
			pars.texto   = pDoc->m_font9texto;
			pars.mensaje = pDoc->m_font9mensa;
			pars.boton   = pDoc->m_font9boton;   

			pars.c_custom_subrayado     = pDoc->m_9c_custom_subrayado;

#ifdef DSPROFUIS9
		    pars.marco.top = 0;
			pars.marco.bottom = 0;
			pars.marco.left = 0;
			pars.marco.right = 0;

			pars.cfondillo           = theApp.m_9fondocampo; // fondo campo
			pars.ctextillo           = theApp.m_9textocampo; // texto campo
			pars.cfondo_seleccion    = theApp.m_9fondo; // fondo
			pars.ctexto_seleccion    = theApp.m_9texto;  // texto	
			pars.c_custom_subrayado  = theApp.m_9subrallado;

			pars.color_1 = theApp.m_9color[1];
			pars.color_2 = theApp.m_9color[2];
			pars.color_3 = theApp.m_9color[3];
			pars.color_4 = theApp.m_9color[4];
			pars.color_5 = theApp.m_9color[5];
			pars.color_6 = theApp.m_9color[6];
			pars.color_7 = theApp.m_9color[7];


			pars.color_i1 = theApp.m_9icolor[1];
			pars.color_i2 = theApp.m_9icolor[2];
			pars.color_i3 = theApp.m_9icolor[3];
			pars.color_i4 = theApp.m_9icolor[4];
			pars.color_i5 = theApp.m_9icolor[5];
			pars.color_i6 = theApp.m_9icolor[6];
			pars.color_i7 = theApp.m_9icolor[7];


			pars.color_fondo_1 = theApp.m_9fcolor[1];
			pars.color_fondo_2 = theApp.m_9fcolor[2];
			pars.color_fondo_3 = theApp.m_9fcolor[3];
			pars.color_fondo_4 = theApp.m_9fcolor[4];
			pars.color_fondo_5 = theApp.m_9fcolor[5];
			pars.color_fondo_6 = theApp.m_9fcolor[6];
			pars.color_fondo_7 = theApp.m_9fcolor[7];

			memset(pars.flags_extra,0,sizeof(pars.flags_extra)); 

			/*
				if ((pPar->flags_extra[0]&1))
					agitapiz_n1 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);
				if ((pPar->flags_extra[1]&1))
					agitapiz_n2 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);
				if ((pPar->flags_extra[2]&1))
					agitapiz_n3 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[3]&1))	
					bmp_fondo_1 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[4]&1))	
					bmp_fondo_2 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[5]&1))	
					bmp_fondo_3 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[6]&1))	
					bmp_fondo_4 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[7]&1))	
					bmp_fondo_5 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[8]&1))	
					bmp_fondo_6 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[9]&1))	
					bmp_fondo_7 = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);

				if ((pPar->flags_extra[10]&1))	
					bmp_fondo_campo = CreateCompatibleBitmap(hDC,principal_nx,principal_ny);
			*/


			for (int i = 1;i < 4;i++)
			{
				if (!theApp.m_9skinfbmp[i].IsEmpty())
				{
					pars.flags_extra[i-1] |= 1;
				}
			}
			for (i = 1;i < 8;i++)
			{
				if (!theApp.m_9skinabmp[i].IsEmpty())
				{
					pars.flags_extra[i+2] |= 1;
				}
			}

			pars.flags = 2;			
#else

		  if (ajuste)
			  pars.flags = 1;		  
		  else
			  pars.flags = 222;
#endif    	  

#ifdef DSPROFUIS9
		  if (ajuste)
		  {
			  //ptmpw = new CWnd();
			  //ptmpw->CreateEx(0,AfxRegisterWndClass(NULL),"",WS_POPUPWINDOW,CRect(0,0,5000,5000),NULL,NULL);
			  //theApp.m_9wnd->SetParent( ptmpw /*theApp.m_pMainWnd */);
			  //theApp.m_9wnd->MoveWindow(0,0,1000,1000);
		  }
#endif
		  llama_fNuevaVentana(m_f9NuevaVentana,m_ds9winsys,theApp.m_9wnd->m_hWnd,&pars);

		  if (ajuste)
		  {
			  CDs9Window *dlg9 = theApp.m_9wnd;
			  CDsTab9 *pTab9   = theApp.m_9wnd->m_tab9;
			  CRect rtab,ctab;

#ifndef DSPROFUIS9
				dlg9->GetWindowRect(&rtab);
				ctab = rtab;
				theApp.StdView()->ScreenToClient(&rtab);   
				pTab9->AdjustRect( TRUE , &rtab );
				pTab9->MoveWindow(rtab);
				pTab9->ScreenToClient(&ctab);   
				dlg9->SetParent( pTab9 );   
				dlg9->MoveWindow(ctab);   
#else
			    HFONT Font = ::CreateFontIndirect(&pDoc->m_font9boton );
				pTab9->SetFont(CFont::FromHandle(Font));
				if (pTab9->m_stdfont)
					::DeleteObject((HGDIOBJ)pTab9->m_stdfont);
				pTab9->m_stdfont = Font;

			    dlg9->SetParent( theApp.StdView() );
			    pTab9->GetTabWndRect(rtab);
				dlg9->GetWindowRect(&ctab);				
				//ctab.top -= rtab.Size().cy; lo bajamos solo
				theApp.StdView()->ScreenToClient(&ctab);
				dlg9->MoveWindow(ctab);
				dlg9->CenterWindow();
				dlg9->GetWindowRect(&ctab);
				rtab.MoveToXY(ctab.left,ctab.top);
				rtab.right = ctab.right;
				theApp.StdView()->ScreenToClient(&ctab);				
				//ctab.top += rtab.Size().cy;				
				theApp.StdView()->ScreenToClient(&rtab);
				ctab.MoveToXY(rtab.left,rtab.bottom);

				//rtab.MoveToXY(rtab.left,rtab.top); // ...

				pTab9->MoveWindow(rtab);
				dlg9->MoveWindow(ctab);

				pTab9->ShowWindow(SW_HIDE);
				dlg9->ShowWindow(SW_HIDE);
#endif				  

			  dlg9->GetClientRect(&ctab);
			  dlg9->m_offx = 0;
			  dlg9->m_offy = 0;
			  dlg9->m_stdx = ctab.Size().cx * 10 / 80;
			  dlg9->m_stdy = ctab.Size().cy * 10 / 25;
		  }
		  if (ptmpw)
		  {
			  ptmpw->DestroyWindow();
			  delete ptmpw;
		  }
	}
}

BOOL CDsxApp::CargaFuentes(LPCSTR dirbas,LPCSTR apli)
{
	int idx;
	int i;
	int cargaskin = 1;
	BOOL rr = FALSE;


	if (apli && !::getenv("DSXRECARGASKIN"))
		cargaskin = 0;

	CString s;
	s = dirbas;
	FILE *file = NULL;	
	if (apli) {
		if (m_nomapli.Compare(apli)) {
			s += "dev/";
			s += m_nomapli;
			s += "/default_";
		}
		else
			s += "dev/default_";
		s += apli;
		s += ".fnt";
		if (*apli)
			file = fopen(s,"r");
	}
	else {
		s += "dev/default.fnt";
		file = fopen(s,"r");
	}
	if (!file && apli) {
		if (m_nomapli.Compare(apli)) {
			if (!m_defaultfntcargado.Compare(m_nomapli)) {
				return FALSE;
			}
			s += "dev/default_";
			s += m_nomapli;
			s += ".fnt";
			file = fopen(s,"r");
			apli = m_nomapli;
		}
		if (!file) {		
			s = dirbas;
			s += "dev/default.fnt";
			file = fopen(s,"r");	 
			apli = NULL; // Restore defaults
		}		
	}

	if (file || !apli) {
		m_fondosolido1 = FALSE;
		m_colorset1 = TRUE;
		m_colorset2 = TRUE;
		m_colorset3 = TRUE;
		m_colorset4 = TRUE;
		m_colorset5 = TRUE;
		m_colorset6 = TRUE;
		m_colorset7 = TRUE;
		m_colorset8 = TRUE;
		m_colorset9 = TRUE;
		m_color1 = RGB(210,210,210);
		m_color2 = RGB(230,230,230);
		m_color3 = RGB(210,210,210);
		m_color4 = RGB(255,255,255);
		m_color5 = RGB(223,215,209);
		m_color6 = RGB(239,237,233);
		m_color7 = RGB(0,0,0);
		m_color8 = RGB(88,83,71);
		m_color9 = RGB(255,242,236);
		m_colorset_t1 = FALSE;
		m_modotab1 = 1;
		m_colorset_t2 = FALSE;
		m_modotab2 = 0;
		m_colorset_t3 = FALSE;

		m_colorsettextab9 = FALSE;

		m_colorset_ce0 = FALSE;	


		LOGFONT font;	

		HFONT hfont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		CFont().FromHandle( hfont )->GetLogFont( &font );

		CDsxDoc::LeeProfileFont(&(font),"lfLabel");
		COLORREF cf,cb;

		cb = (COLORREF)theApp.GetProfileInt("Colores","cLabelBk",(int)::GetSysColor(COLOR_BTNFACE));
		cf = (COLORREF)theApp.GetProfileInt("Colores","cLabelFr",(int)::GetSysColor(COLOR_BTNTEXT));

		for (idx = 0;idx < 128;idx++)
		{
			m_dfont[idx] = font;
			m_cf[idx]    = cf;
			m_cb[idx]    = cb;
		}
		if (apli)
			m_defaultfntcargado = apli;
		else
			m_defaultfntcargado = "";
	}

	if (file)
	{
		char tmp[1026];
		idx = -1;
		while(fgets(tmp,1024,file))
		{
			if (tmp[0] == ':')
			{
				if (!memcmp(tmp,":FONTBOTON",10))
				{
					m_ifontboton = atoi(tmp+10);
				}
				else if (!memcmp(tmp,":FONTGRID",9))
				{
					m_ifontgrid = atoi(tmp+9);
				}
				else if (!memcmp(tmp,":FONTEDIT",9))
				{
					m_ifontcampo = atoi(tmp+9);
				}
				else if (!memcmp(tmp,":FONTMEMOF",10))
				{
					m_ifontmemo2 = atoi(tmp+10);
				}
				else if (!memcmp(tmp,":FONTMEMO",9))
				{
					m_ifontmemo = atoi(tmp+9);
				}
				else if (!memcmp(tmp,":FONT9CAMPO",11))
				{
					m_9ifont9campo = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":FONT9TEXTO",11))
				{
					m_9ifont9texto = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":FONT9REVER",11))
				{
					m_9ifont9rever = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":FONT9INTEN",11))
				{
					m_9ifont9inten = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":FONT9MENSA",11))
				{
					m_9ifont9mensa = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":FONT9BOTON",11))
				{
					m_9ifont9boton = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":FONT9EDITA",11))
				{
					m_9ifont9edita = atoi(tmp+11);
				}
				else if (!memcmp(tmp,":COLOR9FCAMPO",12))
				{
				    tmp[17] = 0;tmp[21] = 0;
					m_9fondocampo = RGB(atoi(tmp+14),atoi(tmp+18),atoi(tmp+22)); // fondo campo
				}
				else if (!memcmp(tmp,":COLOR9TCAMPO",12))
				{					
				    tmp[17] = 0;tmp[21] = 0;
					m_9textocampo = RGB(atoi(tmp+14),atoi(tmp+18),atoi(tmp+22));
				}
				else if (!memcmp(tmp,":COLOR9FONDO",11))
				{
				    tmp[16] = 0;tmp[20] = 0;
					m_9fondo      = RGB(atoi(tmp+13),atoi(tmp+17),atoi(tmp+21));
				}
				else if (!memcmp(tmp,":COLOR9TEXTO",11))
				{
				    tmp[16] = 0;tmp[20] = 0;
					m_9texto      = RGB(atoi(tmp+13),atoi(tmp+17),atoi(tmp+21));					
				}	
				else if (!memcmp(tmp,":COLOR9SUBR",10))
				{
				    tmp[15] = 0;tmp[19] = 0;
					m_9subrallado = RGB(atoi(tmp+12),atoi(tmp+16),atoi(tmp+20));
				}
				else if (!memcmp(tmp,":COLOR9_",8))
				{
					if (tmp[8] == 'I')
					{
						i = tmp[9] - 48;
						if (i > 0 && i < 8)
						{
							tmp[14] = 0;tmp[18] = 0;
							m_9icolor[i] = RGB(atoi(tmp+11),atoi(tmp+15),atoi(tmp+19));
						}
					}
					else if (tmp[8] == 'F')
					{
						i = tmp[9] - 48;
						if (i > 0 && i < 8)
						{
							tmp[14] = 0;tmp[18] = 0;
							m_9fcolor[i] = RGB(atoi(tmp+11),atoi(tmp+15),atoi(tmp+19));
						}
					}
					else
					{
						i = tmp[8] - 48;
						if (i > 0 && i < 8)
						{
							tmp[13] = 0;tmp[17] = 0;
							m_9color[i] = RGB(atoi(tmp+10),atoi(tmp+14),atoi(tmp+18));
						}
					}
				}
				else if (!memcmp(tmp,":SKINP9_",8))
				{
					i = tmp[8] - 48;
					if (i >= 0 && i < 4)
					{
						m_9skinfmodobmp[i] = 2; // Stretch
						m_9skinfbmp[i] = tmp+10;
						m_9skinfbmp[i].Remove((char)10);
						m_9skinfbmp[i].Remove((char)13);
						m_9skinfbmp[i].TrimRight();
						m_9skinfbmp[i].TrimLeft();
						if (!m_9skinfbmp[i].IsEmpty())
						{
							int pc = m_9skinfbmp[i].Find(',');
							CString sc;
							if (pc >= 0)
							{
								sc = m_9skinfbmp[i].Mid(pc+1);
								m_9skinfbmp[i] = m_9skinfbmp[i].Mid(0,pc);
								m_9skinfbmp[i].TrimRight();
								m_9skinfbmp[i].TrimLeft();
								if (!strcmp(sc,"Hollow"))
								   m_9skinfmodobmp[i] = 0;
								else if (!strcmp(sc,"Center"))
								   m_9skinfmodobmp[i] = 1;
								else if (!strcmp(sc,"TileH"))
								   m_9skinfmodobmp[i] = 3;
								else if (!strcmp(sc,"TileV"))
								   m_9skinfmodobmp[i] = 4;
								else if (!strcmp(sc,"Tile"))
								   m_9skinfmodobmp[i] = 5;
							}
							m_9skinfbmp[i] = m_dirbase + m_9skinfbmp[i];
						}
					}
				}
				else if (!memcmp(tmp,":SKINF9_",8))
				{
					i = tmp[8] - 48;
					if (i >= 1 && i < 8)
					{
						m_9skinamodobmp[i] = 2; // Stretch
						m_9skinabmp[i] = tmp+10;
						m_9skinabmp[i].Remove((char)10);
						m_9skinabmp[i].Remove((char)13);
						m_9skinabmp[i].TrimRight();
						m_9skinabmp[i].TrimLeft();
						if (!m_9skinabmp[i].IsEmpty())
						{
							int pc = m_9skinabmp[i].Find(',');
							CString sc;
							if (pc >= 0)
							{
								sc = m_9skinabmp[i].Mid(pc+1);
								m_9skinabmp[i] = m_9skinabmp[i].Mid(0,pc);
								m_9skinabmp[i].TrimRight();
								m_9skinabmp[i].TrimLeft();
								if (!strcmp(sc,"Hollow"))
								   m_9skinamodobmp[i] = 0;
								else if (!strcmp(sc,"Center"))
								   m_9skinamodobmp[i] = 1;
								else if (!strcmp(sc,"TileH"))
								   m_9skinamodobmp[i] = 3;
								else if (!strcmp(sc,"TileV"))
								   m_9skinamodobmp[i] = 4;
								else if (!strcmp(sc,"Tile"))
								   m_9skinamodobmp[i] = 5;
							}
							m_9skinabmp[i] = m_dirbase + m_9skinabmp[i];
						}
					}
				}
	/*
   COLORREF cfondillo;
   COLORREF ctextillo;
   COLORREF cfondo_seleccion;
   COLORREF ctexto_seleccion;
   COLORREF cfondo_campo_seleccion;
   COLORREF c_custom_subrayado;

   unsigned long flags;


   char flags_extra[128];
*/
				else if (!memcmp(tmp,":FONDOSOLIDO1",13))
				{					
					 m_fondosolido1 = TRUE;
				}
				else if (!memcmp(tmp,":BASEClasico",12))
				{
					if (cargaskin)
						m_aparienciabase = 0;
				}
				else if (!memcmp(tmp,":BASEOffice2003",15))
				{
					if (cargaskin)
						m_aparienciabase = 1;
				}
				else if (!memcmp(tmp,":BASEOffice2007R1",17))
				{
					if (cargaskin)
						m_aparienciabase = 2;
				}
				else if (!memcmp(tmp,":BASESkinXML ",13))
				{
					if (cargaskin) {
						m_aparienciabase = 3;
						m_aparienciafile = tmp+13;
						m_aparienciafile.Remove((char)10);
						m_aparienciafile.Remove((char)13);
					}
				}				
				else if (!memcmp(tmp,":BASESkinBIN ",13))
				{
					if (cargaskin) {
						m_aparienciabase = 4;
						m_aparienciafile = tmp+13;
						m_aparienciafile.Remove((char)10);
						m_aparienciafile.Remove((char)13);
					}
					else {
						if (m_aparienciabase == 3 || m_aparienciabase == 4) {
							CString ss;
							ss = tmp + 13;
							ss.Remove((char)10);
							ss.Remove((char)13);
							if (ss.Compare(m_aparienciafile)) {
								m_aparienciafile = ss;
								rr = TRUE;
							}
						}
					}
				}				
				else if (!memcmp(tmp,":BOTON ",7))
				{
					i = m_nbotones;
					int j;
					tmp[10] = 0;
					int id = atoi(tmp+7);
					if (i > 1023){
						for (j = 0;j < 1024;j++) {							
							if (m_botones[j].id == id) {
								i = j;
								break;
							}
						}
						if (j > 1023) {
							for (j = 0;j < 1023;j++) {
								m_botones[j] = m_botones[j+1];
							}						
							i = 1023;
						}
					}
					else
						m_nbotones++;

					{
						m_botones[i].id = id;
						tmp[14] = 0;tmp[18] = 0;tmp[22] = 0;
						m_botones[i].transparente = RGB(atoi(tmp+11),atoi(tmp+15),atoi(tmp+19));
						for (j = 23;tmp[j] && tmp[j] != ';';j++);
						tmp[j] = 0;
						m_botones[i].path = theApp.m_dirbase+(tmp+23);
						m_botones[i].path.Remove((char)10);
						m_botones[i].path.Remove((char)13);
						m_botones[i].path.TrimRight();						
					}
				}
				else if (!memcmp(tmp,":ICONO ",7))
				{
					m_sicono = tmp+7;					
					m_sicono.Remove((char)10);
					m_sicono.Remove((char)13);
					m_sicono.TrimRight();
				}
				else if (!memcmp(tmp,":MODOTAB1 ",10))
				{
					m_modotab1 = atoi(tmp+10);
				}
				else if (!memcmp(tmp,":MODOTAB2 ",10))
				{
					m_modotab2 = atoi(tmp+10);
				}
				else if (tmp[1] == 'C')
				{
					if (tmp[2] == '1')
					{
						if (cargaskin) {
							m_colorset1 = TRUE;
							tmp[7] = 0;tmp[11] = 0;
							m_color1 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
						}
					}					
					else if (tmp[2] == '2')
					{
						if (cargaskin) {
							m_colorset2 = TRUE;
							tmp[7] = 0;tmp[11] = 0;
							m_color2 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
						}
					}					
					else if (tmp[2] == '3')
					{
						m_colorset3 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color3 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
					}										
					else if (tmp[2] == '4')
					{
						m_colorset4 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color4 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
					}										
					else if (tmp[2] == '5')
					{
						m_colorset5 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color5 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
					}										
					else if (tmp[2] == '6')
					{
						m_colorset6 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color6 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
					}										
					else if (tmp[2] == '7')
					{
						m_colorset7 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color7 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
					}										
					else if (tmp[2] == '8')
					{
						m_colorset8 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color8 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
					}										
					else if (tmp[2] == '9')
					{
						m_colorset9 = TRUE;
						tmp[7] = 0;tmp[11] = 0;
						m_color9 = RGB(atoi(tmp+4),atoi(tmp+8),atoi(tmp+12));
						if (m_colorbrush9)
							::DeleteObject(m_colorbrush9);
						m_colorbrush9 = ::CreateSolidBrush(m_color9);
					}										
					else if (tmp[2] == 'T')
					{
						if (tmp[3] == '0')
						{
							m_colorsettextab9 = TRUE;
							tmp[8] = 0;tmp[12] = 0;
							m_colortextab9 = RGB(atoi(tmp+5),atoi(tmp+9),atoi(tmp+13));
						}
						else if (tmp[3] == '1')
						{
							m_colorset_t1 = TRUE;
							for (int i = 0;i < 7;i++)
							{
								tmp[8+i*12] = 0;tmp[12+i*12] = 0;tmp[16+i*12] = 0;
								m_color_t1[i] = RGB(atoi(tmp+5+i*12),atoi(tmp+9+i*12),atoi(tmp+13+i*12));
							}
						}
						else if (tmp[3] == '2')
						{
							m_colorset_t2 = TRUE;
							for (int i = 0;i < 7;i++)
							{
								tmp[8+i*12] = 0;tmp[12+i*12] = 0;tmp[16+i*12] = 0;
								m_color_t2[i] = RGB(atoi(tmp+5+i*12),atoi(tmp+9+i*12),atoi(tmp+13+i*12));
							}
						}
						else if (tmp[3] == '3')
						{
							m_colorset_t3 = TRUE;
							for (int i = 0;i < 7;i++)
							{
								tmp[8+i*12] = 0;tmp[12+i*12] = 0;tmp[16+i*12] = 0;
								m_color_t3[i] = RGB(atoi(tmp+5+i*12),atoi(tmp+9+i*12),atoi(tmp+13+i*12));
							}
						}
					}
					else if (tmp[2] == 'E')
					{
						if (tmp[3] == '0')
						{
							m_colorset_ce0 = TRUE;
							for (int i = 0;i < 6;i++)
							{
								tmp[8+i*12] = 0;tmp[12+i*12] = 0;tmp[16+i*12] = 0;
								m_color_ce0[i] = RGB(atoi(tmp+5+i*12),atoi(tmp+9+i*12),atoi(tmp+13+i*12));
							}
						}
					}
				}
				else
				{
					idx = atoi(tmp+1);
					if (idx < 0 || idx >= 128)
						idx = -1;
				}
			}
			else
			{
				if (idx > -1)
				{
					if (!memcmp(tmp,"FaceName ",9))
					{
						tmp[8+LF_FACESIZE] = 0;
						s = tmp+9;
						s.Remove('\r');
						s.Remove('\n');
						strcpy(m_dfont[idx].lfFaceName,s);
					} 
					else if (!memcmp(tmp,"Height ",7))
					{
						m_dfont[idx].lfHeight = atoi(tmp+7);
					}
					else if (!memcmp(tmp,"Width ",6))
					{
						m_dfont[idx].lfWidth = atoi(tmp+6);
					}
					else if (!memcmp(tmp,"Escapement ",11))
					{
						m_dfont[idx].lfEscapement = atoi(tmp+11);
					}
					else if (!memcmp(tmp,"Orientation ",12))
					{
						m_dfont[idx].lfOrientation = atoi(tmp+12);
					}
					else if (!memcmp(tmp,"Weight ",7))
					{
						m_dfont[idx].lfWeight = atoi(tmp+7);
					}
					else if (!memcmp(tmp,"Italic ",7))
					{
						m_dfont[idx].lfItalic = atoi(tmp+7);
					}
					else if (!memcmp(tmp,"Underline ",10))
					{
						m_dfont[idx].lfUnderline = atoi(tmp+10);
					}
					else if (!memcmp(tmp,"StrikeOut ",10))
					{
						m_dfont[idx].lfStrikeOut = atoi(tmp+10);
					}										
					else if (!memcmp(tmp,"CharSet ",8))
					{
						m_dfont[idx].lfCharSet = atoi(tmp+8);
					}										
					else if (!memcmp(tmp,"OutPrecision ",13))
					{
						m_dfont[idx].lfOutPrecision = atoi(tmp+13);
					}										
					else if (!memcmp(tmp,"ClipPrecision ",14))
					{
						m_dfont[idx].lfClipPrecision = atoi(tmp+14);
					}					
					else if (!memcmp(tmp,"Quality ",8))
					{
						m_dfont[idx].lfQuality = atoi(tmp+8);
					}					
					else if (!memcmp(tmp,"PitchAndFamily ",15))
					{
						m_dfont[idx].lfPitchAndFamily = atoi(tmp+15);
					}
					else if (!memcmp(tmp,"CF ",3))
					{
						tmp[6] = 0;tmp[10] = 0;
						m_cf[idx] = RGB(atoi(tmp+3),atoi(tmp+7),atoi(tmp+11));
					}
					else if (!memcmp(tmp,"CB ",3))
					{
						tmp[6] = 0;tmp[10] = 0;
						m_cb[idx] = RGB(atoi(tmp+3),atoi(tmp+7),atoi(tmp+11));
					}
				}
			}
		}
		fclose(file);
	}

	return rr;
}

