// WindowDs.cpp : implementation file
//

#include "stdafx.h"

#ifdef YANOEXISTE

#include "Diagram9.h"
#include "Diagram9View.h"
#include "WindowDs.h"
#include "EntornoDs.h"
#include "TabMenu.h"
#include "Pantalla.h"
#include "DsEdit.h"
#include "Diagram9Doc.h"
#include "TablaView.h"
#include "MainFrm.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindowDs

CWindowDs::CWindowDs(CEntornoDs *pEDs)
{
   m_pEDs = pEDs;
   m_rect.SetRectEmpty();
   m_IndScrollBar = 0;
   m_ScrollBar[0] = NULL;
   m_TabMenu[0] = NULL;
   m_IndTabMenu = 0;
   m_salvastdpan = NULL;
   m_Edit[0] = NULL;
   m_IndEdit = 0;
   m_Grid[0] = NULL;
   m_IndGrid = 0; 
   m_visible = TRUE;
   m_nivel_recursion = 0;
}

CWindowDs::~CWindowDs()
{
   while (m_IndScrollBar) 
   {
      if (m_ScrollBar[m_IndScrollBar])
         delete m_ScrollBar[m_IndScrollBar];
      m_IndScrollBar--;
   }
   if (m_ScrollBar[0])
      delete m_ScrollBar[0];
   while (m_IndTabMenu)
   {
      if (m_TabMenu[m_IndTabMenu])
      {
         delete m_TabMenu[m_IndTabMenu];
      }
      m_IndTabMenu--;
   }
   if (m_TabMenu[0])
   {
      delete m_TabMenu[0];
   }
   while(m_IndEdit)
   {
      if (m_Edit[m_IndEdit])
      {
         delete m_Edit[m_IndEdit];
      }
      m_IndEdit--;
   }
   if (m_Edit[0])
   {
      delete m_Edit[0];
   }

   
   for (int i = 0;i < m_IndGrid;i++)
   {
     if (m_Grid[i])
	    {
	    delete m_Grid[i];
	    }
	 m_Grid[i] = NULL;
   }
   while(m_IndGrid > 0)
   {
    if (!m_Grid[m_IndGrid-1])
       m_IndGrid--;
    else
       break;
   }   
}


BEGIN_MESSAGE_MAP(CWindowDs, CWnd)
//{{AFX_MSG_MAP(CWindowDs)
ON_WM_CHAR()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
ON_WM_PAINT()
ON_WM_VSCROLL()
ON_WM_SETFOCUS()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWindowDs message handlers

void CWindowDs::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))
      llama_fmensaje_char(m_pEDs->m_fmensaje_char,nChar,nRepCnt,nFlags);	
   else    
      SubeVentanaBuena();
   
   //PARENTCWindowDs::OnChar(nChar, nRepCnt, nFlags);
}

void CWindowDs::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fmensaje_ldblclk(m_pEDs->m_fmensaje_ldblclk,point.x,point.y,nFlags);
   else
      SubeVentanaBuena();
   // TODO: Add your message handler code here and/or call default	
   //PARENTCWindowDs::OnLButtonDblClk(nFlags, point);
}

void CWindowDs::OnLButtonDown(UINT nFlags, CPoint point) 
{
   
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fmensaje_lbuttondown(m_pEDs->m_fmensaje_lbuttondown,point.x,point.y,nFlags);
   else
      SubeVentanaBuena();
   // TODO: Add your message handler code here and/or call default	
   //PARENTCWindowDs::OnLButtonDown(nFlags, point);
}

void CWindowDs::OnLButtonUp(UINT nFlags, CPoint point) 
{
   
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fmensaje_lbuttonup(m_pEDs->m_fmensaje_lbuttonup,point.x,point.y,nFlags);
   else
      SubeVentanaBuena();
   // TODO: Add your message handler code here and/or call default	
   // PARENTCWindowDs::OnLButtonUp(nFlags, point);
}

void CWindowDs::OnMouseMove(UINT nFlags, CPoint point) 
{
   
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fmensaje_mousemove(m_pEDs->m_fmensaje_mousemove,point.x,point.y,nFlags);
   // TODO: Add your message handler code here and/or call default	
   //PARENTCWindowDs::OnMouseMove(nFlags, point);
}

void CWindowDs::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fmensaje_keydown(m_pEDs->m_fmensaje_keydown,nChar,nRepCnt,nFlags);
   else
      SubeVentanaBuena();
   // TODO: Add your message handler code here and/or call default	
   //PARENTCWindowDs::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CWindowDs::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fmensaje_keyup(m_pEDs->m_fmensaje_keyup,nChar,nRepCnt,nFlags);
   else
      SubeVentanaBuena();
   // TODO: Add your message handler code here and/or call default	
   //PARENTCWindowDs::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CWindowDs::OnPaint() 
{    
   
    PARENTCWindowDs::OnPaint();
    return;

    MSG msg;
    if (::PeekMessage(&msg,this->m_hWnd,WM_PAINT,WM_PAINT,PM_NOREMOVE))  
    {
        ::Sleep(15);
    }

   CPaintDC dc(this); // device context for painting
  
  
   CBrush *hh = CBrush().FromHandle((HBRUSH)(::GetStockObject(LTGRAY_BRUSH)));
   
   dc.FillRect(&dc.m_ps.rcPaint,hh);
   
   if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fpinta_zona(m_pEDs->m_fpinta_zona,dc.m_hDC,dc.m_ps.rcPaint,NULL);	
   else
   {
      if (m_salvastdpan && m_pEDs->m_wallowed == this)
      {
         llama_fpinta_zona(m_pEDs->m_fpinta_zona,dc.m_hDC,dc.m_ps.rcPaint,m_salvastdpan);
      }
   }
   
   if (!m_visible && m_pEDs)
   {
      HazVisible();
   }
   
   
}

BOOL CWindowDs::OnCommand(WPARAM wParam, LPARAM lParam) 
{

   //PostMessage(WM_QUIT);
   
   if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this)
	  {       

      SubeVentanaBuena();
      
      if ((wParam & 0xffff) == ACTIVA_NUEVO_ENTORNO)
      {
         CEntornoDs *ne = (CEntornoDs *)lParam;
         CRect r;			 
         CWindowDs *no = theApp.m_EntornoActivo->m_wallowed;
         //no->GetWindowRect(&r);       
         r = no->m_rect;
         CWindowDs *nw = new CWindowDs(ne);	   
         nw->m_rect = r;
         if (nw->CreateEx(0,ClaseLocal,"Diagram10_tab",WS_CHILD | WS_CLIPCHILDREN,nw->m_rect,ne,ne->m_wid))
         {            
            theApp.m_EntornoActivo->EnableWindow( FALSE );
            theApp.m_EntornoActivo->m_wallowed->DesactivaControles();
            if (theApp.m_EntornoActivo->m_LocalParent)
            {
               theApp.m_EntornoActivo->m_LocalParent->EnableWindow( FALSE );
            }
            
            char tmp[(sizeof(void *)+sizeof(HWND))*2];
            char *p = tmp;
            memset(p,0,(sizeof(void *)+sizeof(HWND))*2);
            p += (sizeof(void *)+sizeof(HWND));
            memcpy(p,(void *)&(nw->m_hWnd),sizeof(HWND));
            p += sizeof(HWND);	
            ne->m_wallowed = nw;
            theApp.m_EntornoActivo = ne;
            llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,3,(void *)tmp);
            p = tmp;		  
            memcpy((void *)&(nw->m_salvastdpan),p,sizeof(void *));		  
            // comprobar validez de la ventana
            HWND comprueba;
            memcpy((void *)&comprueba,p,sizeof(HWND));
            
            ASSERT(comprueba == no->m_hWnd);
            
            nw->CalculaRect();
            
            p += sizeof(HWND);
            memcpy((void *)&(no->m_salvastdpan),p,sizeof(void *));
            // invisible
            llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,6,NULL);
            nw->m_visible = FALSE;
            /*
            ne->m_pView->ShowWindow(SW_SHOW);
            ne->m_pView->BringWindowToTop();
            ne->m_pView->EnableWindow( TRUE );
            ne->m_LocalParent->BringWindowToTop();
            ne->m_LocalParent->EnableWindow( TRUE );
            ne->m_LocalParent->SetActiveWindow( );
            ne->m_LocalParent->SetForegroundWindow( );
            //ne->m_LocalParent->SetWindowPos(&wndTopMost ,0,0,0,0,SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
            ne->m_LocalParent->ShowWindow(SW_SHOW);
            ne->ShowWindow(SW_SHOW);
            nw->ShowWindow(SW_SHOW);
            nw->SetFocus();
            */
         }
         else
         {
            // NO hace falta el repon pantalla pues nunca se activo la nueva.
            delete nw;
            delete ne;
         }
         return TRUE;
      }     
      else
	  if ( (wParam  & 0xffff) == ID_AVISO_GRIDMUERE)
      {
            char *p;
            long tam;
            long respuesta;
            long modo = 1;
            int operacion = 9999;
            char tmp[256];
            int localizador = (int)lParam;

            p = tmp;

            memcpy((void *)p,(void *)&localizador,sizeof(int));
            p += sizeof(int);

            tam = (sizeof(long) * 4) + (sizeof(int)*2) + 1;
            respuesta = tam;

            memcpy((void *)p,(void *)&tam,sizeof(long));
            p += sizeof(long);	
            memcpy((void *)p,(void *)&respuesta,sizeof(long));
            p += sizeof(long);
            memcpy((void *)p,(void *)&modo,sizeof(long));
            p += sizeof(long);
            memcpy((void *)p,(void *)&operacion,sizeof(int));
            p += sizeof(int);
            llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,13,(void *)tmp);
            return TRUE;
      }
        
      if (llama_fmensaje_command(m_pEDs->m_fmensaje_command,wParam & 0xffff,lParam)) return TRUE;
	  }
   else
	  {
      if (m_pEDs && m_pEDs->m_wallowed == this)
         if (llama_fmensaje_command(m_pEDs->m_fmensaje_command,wParam & 0xffff,lParam)) return TRUE;	    
	  }
   
   return PARENTCWindowDs::OnCommand(wParam, lParam);
}

void CWindowDs::ActivaScrollBar(CRect *rect,BOOL vertical)
{
   
   if (m_ScrollBar[m_IndScrollBar]) 
   {
      m_ScrollBar[m_IndScrollBar]->ShowScrollBar(FALSE);
      m_IndScrollBar++;
   }
   m_ScrollBar[m_IndScrollBar] = new CScrollBar();
   int modo;
   if (vertical)
      modo = SBS_VERT | SBS_RIGHTALIGN;
   else
      modo = SBS_HORZ | SBS_BOTTOMALIGN;
   m_ScrollBar[m_IndScrollBar]->Create(modo,*rect,this,SCROLLBAR_DE_LINEAS+m_IndScrollBar);

   //m_ScrollBar->SetScrollInfo(,FALSE);
   m_ScrollBar[m_IndScrollBar]->SetScrollRange( 0, 100, FALSE );
   m_ScrollBar[m_IndScrollBar]->SetScrollPos( 50, FALSE);
   m_ScrollBar[m_IndScrollBar]->ShowScrollBar( TRUE );
}

void CWindowDs::DesactivaScrollBar()
{
   
   if (m_ScrollBar[m_IndScrollBar]) 
   {   
      delete m_ScrollBar[m_IndScrollBar];
      m_ScrollBar[m_IndScrollBar] = NULL;
      if (m_IndScrollBar) m_IndScrollBar--;
      if (m_ScrollBar[m_IndScrollBar])
      {
         m_ScrollBar[m_IndScrollBar]->ShowScrollBar(TRUE);
      }
   }
}

void CWindowDs::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{	
   
   if (!theApp.m_EntornoActivo) {PARENTCWindowDs::OnVScroll(nSBCode, nPos, pScrollBar);return;}
   
   if (theApp.m_EntornoActivo->m_wallowed == this && pScrollBar == m_ScrollBar[m_IndScrollBar])
   {
      short code = 0;
      switch(nSBCode)
      {
      case SB_LINEUP:
         code = 808; //SOLICITA_SCROLLUP;
         break;
      case SB_PAGEUP:
         code = 814;//SOLICITA_RETROCESO;
         break;
      case SB_LINEDOWN:
         code = 809;//SOLICITA_SCROLLDOWN;
         break;
      case SB_PAGEDOWN:		  
         code = 815;//SOLICITA_AVANCE;
         break;
      case SB_TOP:
         code = SOLICITA_INICIO;
         break;
      case SB_BOTTOM:
         code = SOLICITA_FINAL;
         break;
      case SB_THUMBPOSITION:
         if (!nPos )
         {
            code = SOLICITA_INICIO;
         }
         else
            if (nPos == 100)
            {
               code = SOLICITA_FINAL;
            }
            break;
      case SB_THUMBTRACK:
         if (nPos < 50)
            code = SOLICITA_INICIO;
         else
            if (nPos > 50)
               code = SOLICITA_FINAL;
            break;
      default:
         break;
      }
      if (code)
      {
         //HWND old_cap = ::GetCapture();
         //if (old_cap)
         //   ::ReleaseCapture();
         SetFocus();
         //PostMessage(WM_COMMAND,(WPARAM)(unsigned short)code,(LPARAM)0);		  
         llama_fmensaje_command(m_pEDs->m_fmensaje_command,(WPARAM)(unsigned short)code,(LPARAM)0);		  
         //if (old_cap)
         //   {
         //	 ::SetCapture(old_cap);
         //	 }
      }
      else
         PARENTCWindowDs::OnVScroll(nSBCode, nPos, pScrollBar);
   }
   else
      PARENTCWindowDs::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CWindowDs::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
   if (!theApp.m_EntornoActivo) {PARENTCWindowDs::OnHScroll(nSBCode, nPos, pScrollBar);return;}
   
   if (theApp.m_EntornoActivo->m_wallowed == this && pScrollBar == m_ScrollBar[m_IndScrollBar])
   {
      short code = 0;
      switch(nSBCode)
      {
      case SB_LINEUP:
         code = 808; //SOLICITA_SCROLLUP;
         break;
      case SB_PAGEUP:
         code = 814;//SOLICITA_RETROCESO;
         break;
      case SB_LINEDOWN:
         code = 809;//SOLICITA_SCROLLDOWN;
         break;
      case SB_PAGEDOWN:		  
         code = 815;//SOLICITA_AVANCE;
         break;
      case SB_TOP:
         code = SOLICITA_INICIO;
         break;
      case SB_BOTTOM:
         code = SOLICITA_FINAL;
         break;
      case SB_THUMBPOSITION:
         if (!nPos )
         {
            code = SOLICITA_INICIO;
         }
         else
            if (nPos == 100)
            {
               code = SOLICITA_FINAL;
            }
            break;
      case SB_THUMBTRACK:
         if (nPos < 50)
            code = SOLICITA_INICIO;
         else
            if (nPos > 50)
               code = SOLICITA_FINAL;
            break;
      default:
         break;
      }
      if (code)
      {
         //HWND old_cap = ::GetCapture();
         //if (old_cap)
         //   ::ReleaseCapture();
         SetFocus();
         //PostMessage(WM_COMMAND,(WPARAM)(unsigned short)code,(LPARAM)0);		  
         llama_fmensaje_command(m_pEDs->m_fmensaje_command,(WPARAM)(unsigned short)code,(LPARAM)0);		  
         //if (old_cap)
         //   {
         //	 ::SetCapture(old_cap);
         //	 }
      }
      else
         PARENTCWindowDs::OnHScroll(nSBCode, nPos, pScrollBar);
   }
   else
      PARENTCWindowDs::OnHScroll(nSBCode, nPos, pScrollBar);
	
	
}


void CWindowDs::ActivaTabMenu(char *opcion, CRect *rect, int codigo, int precodigo, int defecto)
{
   
   if (m_TabMenu[m_IndTabMenu]) 
   {
      /*m_TabMenu[m_IndTabMenu]->ShowWindow(SW_HIDE);*/ // no tiene validez el concepto de tab unico
      m_IndTabMenu++;
   }
   
   m_TabMenu[m_IndTabMenu] = new CTabMenu((CWnd *)this,codigo,precodigo);
   CTabMenu *ptab = m_TabMenu[m_IndTabMenu];
   
   if (!ptab->Create(TCS_TABS | WS_CHILD | WS_CLIPCHILDREN,*rect,this,codigo))
   {
      return;
   }
   //ptab->ModifyStyleEx(0,WS_EX_TRANSPARENT);   
   
   ptab->DeleteAllItems();
   
   ptab->SetItemExtra( 0 );
   
   int total = 0;
   char *opciones[100];
   char mi_opcion[1024];
   char *pop = mi_opcion;
   strcpy(mi_opcion,opcion);
   
   opciones[total] = mi_opcion;
   total++;
   while(*pop)
   {
      if (*pop == 9)
      {
         *pop = 0;	  
         opciones[total] = pop+1;  
         total++;
      }
      pop++;
   }
   
   TCITEM Item;
   for (int i = 0;i < total;i++)
   {
      Item.mask = TCIF_TEXT;
      Item.pszText = opciones[i];
      Item.cchTextMax = strlen(opciones[i]);  
      ptab->InsertItem(i,&Item);  
   }
   
   //CRect ItemR;
   //ptab->GetItemRect(0,&ItemR);
   ptab->SetCurSel(defecto-1);
   ptab->ShowWindow(SW_SHOW);
   ptab->m_activo = TRUE;
   ptab->NotificaOpcion();
}

void CWindowDs::DesactivaTabMenu(int codigo)
{
   
   for (int i = 0;i <= m_IndTabMenu;i++)
   {
      if (m_TabMenu[i] && m_TabMenu[i]->m_codigo == codigo) 
      {   
         delete m_TabMenu[i];
         m_TabMenu[i] = NULL;
         if (m_IndTabMenu == i)
         {
            while(m_IndTabMenu > 0 && !m_TabMenu[m_IndTabMenu])
               m_IndTabMenu--;
         }
      }
   }
}


void CWindowDs::OnSetFocus(CWnd* pOldWnd) 
{
   PARENTCWindowDs::OnSetFocus(pOldWnd);   
   PonFoco();
  
}

void CWindowDs::CalculaRect()
{
   
   if (m_pEDs)
   {
      CRect tmp1,tmp2;
      m_pEDs->GetWindowRect(&tmp2);
      GetWindowRect(&tmp1);
      m_rect.SetRect(tmp1.left-tmp2.left,
         tmp1.top-tmp2.top,
         tmp1.right-tmp2.left,
         tmp1.bottom-tmp2.top);
      m_rect.NormalizeRect();
   }
}

void CWindowDs::SubeVentanaBuena() // inutil por la desabilitacion de la ventana
{
   return;
   
   if (m_pEDs != theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_LocalParent)
   {
      theApp.m_EntornoActivo->m_LocalParent->BringWindowToTop();
      theApp.m_EntornoActivo->m_LocalParent->ShowWindow(SW_SHOWNORMAL);
   }
}

void CWindowDs::HazVisible()
{
   if (m_visible) return;
   
   
   m_visible = TRUE;
   if (m_pEDs && m_pEDs->m_pView)
   {
      m_pEDs->m_pView->ShowWindow(SW_SHOW);
      m_pEDs->m_pView->BringWindowToTop();
      m_pEDs->m_pView->EnableWindow( TRUE );
      if (m_pEDs->m_LocalParent)
	     {
         m_pEDs->m_LocalParent->BringWindowToTop();
         m_pEDs->m_LocalParent->EnableWindow( TRUE );
         m_pEDs->m_LocalParent->SetActiveWindow( );
         m_pEDs->m_LocalParent->SetForegroundWindow( );	
         m_pEDs->m_LocalParent->ShowWindow(SW_SHOW);
      }
      m_pEDs->ShowWindow(SW_SHOW);
   }
   ShowWindow(SW_SHOW);
   SetFocus();
   
   /*
   ne->m_pView->ShowWindow(SW_SHOW);
   ne->m_pView->BringWindowToTop();
   ne->m_pView->EnableWindow( TRUE );
   ne->m_LocalParent->BringWindowToTop();
   ne->m_LocalParent->EnableWindow( TRUE );
   ne->m_LocalParent->SetActiveWindow( );
   ne->m_LocalParent->SetForegroundWindow( );
   //ne->m_LocalParent->SetWindowPos(&wndTopMost ,0,0,0,0,SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
   ne->m_LocalParent->ShowWindow(SW_SHOW);
   ne->ShowWindow(SW_SHOW);
   nw->ShowWindow(SW_SHOW);
   nw->SetFocus();
	  */
   
}

//CEdit *pEdita = new CEdit();
//pEdita->Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_LEFT | ES_WANTRETURN | WS_CHILD  |
//          WS_VISIBLE  | WS_TABSTOP | ES_NOHIDESEL | WS_HSCROLL | WS_VSCROLL,CRect(50,50,400,400),&DsWindow,3111);


int CWindowDs::ActivaEdita(char *txt, int area)
{
   RECT rect;
   BOOL multilinea = TRUE;
   int tama;
   char *p;   
   int id_control;
   int id_campo;
   BOOL es_control = FALSE;
   CFont *font = NULL;
   CWnd *padre = (CWnd *)this;  
   
   
   if (!theApp.m_EntornoActivo) return 0;


   memcpy((void*)&tama,(void*)txt,sizeof(int));


   p = txt + sizeof(int);   

   CDsEdit *pEdit;

   DWORD style = 0;

   if (!tama)
   {
       multilinea = TRUE;
       pEdit = new CDsEdit(multilinea);
   }
   else
   {
       multilinea = FALSE;
       pEdit = new CDsEdit(multilinea);

       memcpy((void*)&(pEdit->m_maxqc),(void*)p,sizeof(int));
       p += sizeof(int);
       memcpy((void*)&(pEdit->m_qh),(void*)p,sizeof(int));
       p += sizeof(int);
       memcpy((void*)&(pEdit->m_tipo),(void*)p,sizeof(int));
       p += sizeof(int);
       memcpy((void*)&(pEdit->m_decimales),(void*)p,sizeof(int));
       p += sizeof(int);
       memcpy((void*)&(pEdit->m_flag),(void*)p,sizeof(int));
       p += sizeof(int);
       memcpy((void*)&(id_control),(void*)p,sizeof(int));
       p += sizeof(int);
       memcpy((void*)&(id_campo),(void*)p,sizeof(int));
       p += sizeof(int);
       
       if ((pEdit->m_flag & 8))
       {
           es_control = TRUE;
       }

       strcpy(pEdit->m_qi,p);
       p+=(strlen(pEdit->m_qi)+1);
       strcpy(pEdit->m_qs,p);
       p+=(strlen(pEdit->m_qs)+1);

       if (strstr(pEdit->m_qs,">"))
       {
           style |= ES_UPPERCASE;
       }
       else
       if (strstr(pEdit->m_qs,"<"))
       {
           style |= ES_LOWERCASE;
       }

       if (pEdit->m_qh == -1)
       {
           style |= ES_PASSWORD;
       }

       if (pEdit->m_tipo == 3 && !pEdit->m_decimales && ::atoi(pEdit->m_qs) >= 0 && ::atoi(pEdit->m_qi) >= 0)
       {
           style |= ES_NUMBER;
       }
   }
   
   if (!es_control)
   {
       rect.left = area / 10000;
       if (!tama)
          rect.right = 1;
       else
          rect.right = 0;
       rect.top  = area % 10000;
       rect.bottom = 0;
       llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,7,(void *)&rect);
   }
   else
   {
       switch(area)
       {
       case 0: // GRID
           /*
           if (id_control > -1 && m_Grid[id_control])
           {
               m_Grid[id_control]->m_tabla.m_curcol = id_campo+COLUMNA_INICIAL;
               m_Grid[id_control]->m_tabla.PosicionElemento(&rect,m_Grid[id_control]->m_tabla.m_cursel,id_campo+COLUMNA_INICIAL);
               padre = (CWnd *)&m_Grid[id_control]->m_tabla;
               font = m_Grid[id_control]->m_tabla.GetFont();
           }
           */
           DebugBreak();
           break;
       default:
           rect.top = 0;
           rect.bottom = 50;
           rect.left = 0;
           rect.right = 100;
           break;
       }
   }
   
   if (multilinea)
   {
       style |= (ES_AUTOVSCROLL | ES_MULTILINE | ES_LEFT | ES_WANTRETURN | WS_CHILD  |
      WS_VISIBLE  | WS_TABSTOP | ES_NOHIDESEL /*| WS_VSCROLL*/);
   }
   else
   {
       style |= (ES_AUTOHSCROLL | ES_LEFT /*| ES_WANTRETURN*/ | WS_CHILD  |
      WS_VISIBLE  | WS_TABSTOP | ES_NOHIDESEL /*| WS_VSCROLL*/);
   }

   // CEdit
   if (pEdit->Create(style,rect,padre,IDW_EDIT + m_IndEdit))
   {
      //::SetClassLong(pEdit->m_hWnd,GCL_HBRBACKGROUND,(LONG)::GetStockObject(LTGRAY_BRUSH));              
      if (font)
      {
           pEdit->SetFont(font);  
      }
      else
      if (theApp.m_EntornoActivo)
      {
           pEdit->m_pFont = new CFont();           
           pEdit->m_pFont->CreateFontIndirect(&(theApp.m_EntornoActivo->m_pDoc->m_fontcampo));
           pEdit->SetFont(pEdit->m_pFont);
      }   

      if (!multilinea && pEdit->m_maxqc)
      {
           pEdit->SetLimitText( pEdit->m_maxqc );
      }
      
      if (!multilinea && pEdit->GetMargins( ))
      {
          if (pEdit->m_maxqc > 2)
              pEdit->SetMargins( 0, 0 );
      }


      if (m_Edit[m_IndEdit])
         m_IndEdit++;
      m_Edit[m_IndEdit] = pEdit;

      unsigned char *kp;

      if (pEdit->m_tipo == 3)
      {
          for (kp = (unsigned char *)p;*kp;kp++)
          {
              if (*kp == '.') *kp = ',';
          }
      }
      for (kp = (unsigned char *)p;*kp;kp++)
      {
          oem_a_ansi(kp);
      }
            
      pEdit->SetWindowText((LPCSTR)p);
     
      HWND phW = ::SetFocus(pEdit->m_hWnd);
      if (!multilinea)
      {
         if (pEdit->m_tipo == 2)
         {
            if (pEdit->m_maxqc == 6)
            {
               pEdit->m_overwrite = TRUE;
            }
            else
               pEdit->SetSel( 0, 2);
         }
         else
            pEdit->SetSel( 0, pEdit->m_maxqc);
      }
      
      //pEdit->RunModalLoop(0 /*MLF_NOIDLEMSG*/);

      if (es_control && !area)
      {
          m_Grid[id_control]->m_tabla.m_editando = TRUE;
      }

      int tecla;
      short stecla;
      short sqj;
      do
      {
        tecla = (pEdit->m_flag&3);
        llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,8,(void *)&tecla);
        sqj = (tecla & 0xffff);
        stecla = ((tecla >> 16) & 0xffff);
        if (sqj == -1)
        {
            sqj = (short)pEdit->ProcesaPtec((int)stecla);
        }
      } while(sqj == -1);      

      tecla = (int)sqj;
      
      CString s;
    
      if (multilinea)
         pEdit->FmtLines( TRUE );
      pEdit->GetWindowText(s);       
      
      memcpy((void*)txt,(void *)&tecla,sizeof(int));
      strcpy(txt+sizeof(int),s.Mid(0,5000));

      if (pEdit->m_tipo == 3)
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

      delete m_Edit[m_IndEdit];
      m_Edit[m_IndEdit] = NULL;
      if (m_IndEdit)
         m_IndEdit--;

      if (es_control && !area)
      {
          m_Grid[id_control]->m_tabla.m_editando = FALSE;          
      }
      
      if (phW && ::IsWindow(phW))
      {
         ::SetFocus(phW);
      }
   }
   else
   {
      int tecla = 7;
      memcpy((void*)txt,(void *)&tecla,sizeof(int));
      txt[sizeof(int)] = 0;
      delete pEdit;
   }   

   return 0;
}

BOOL CWindowDs::CreaGrid(void *puntero)
{
char *p = (char *)puntero;
char *titulo;
struct Info_Entra_Campos *columnas;
long registros;
long inicial;
int localizador;
int g_modo;
int p1,p2;
RECT rect;
int padre;
CTablaW *ptablaw = new CTablaW();

if (ptablaw)
  {
  p += sizeof(long)*3;
  localizador = *((int *)p);
  p += sizeof(int);
  registros = *((long *)p);
  p += sizeof(long);
  inicial = *((long *)p);
  p += sizeof(long);
  titulo = p;
  p += (strlen(titulo) + 1);
  g_modo = *((int *)p);
  p += sizeof(int);
  p1 = *((int *)p);
  p += sizeof(int);
  p2 = *((int *)p);
  p += sizeof(int);
  padre = *((int *)p);
  p += sizeof(int);

  columnas = (struct Info_Entra_Campos *)p;

  //PonPantallasEnTab(1,titulo);  

  for (int i = 0;i < m_IndGrid;i++)
  {
      if (!m_Grid[i]) break;
  }

  CFont *pFont = new CFont();
  pFont->CreateFontIndirect(&(theApp.m_EntornoActivo->m_pDoc->m_fontgrid));

  //m_gmodo  bits: 0 = consulta simple  1 = cambio clave 2 = ventana parcial 3 = movible 4 = ajustable

  
  CWnd *pariente;

  if ( !(g_modo & 4) )
  {
      pariente = new CWnd();
      pariente->Attach( m_pEDs->m_pView->m_hWnd );  
  }
  else
  {
      pariente = (CWnd *)this;
      rect.left = p1;
      rect.right = 2;      
      rect.top  = p2;
      rect.bottom = 2;
      llama_fDespachadorInterno(theApp.m_EntornoActivo->m_fDespachadorInterno,7,(void *)&rect);   
  }
  
  ptablaw->Crea(pariente,(LPCSTR)titulo,columnas,registros,inicial,i,pFont,localizador,g_modo,&rect,padre,NULL);

  if ( !(g_modo & 4) )
  {
      pariente->Detach();
      delete pariente;
  }

  m_Grid[i] = ptablaw;
  ptablaw->BringWindowToTop();
  
  if (i == m_IndGrid)
     m_IndGrid++;

  ptablaw->SetFocus();

  if (!m_visible && m_pEDs)
  {
     HazVisible();
  }

  //m_pEDs->ShowWindow(SW_HIDE);
  if ( !(g_modo & 4) )
  {
     m_pEDs->EnableWindow(FALSE);
  }

  p = ((char *)puntero)+sizeof(long)*3;
  memcpy((void *)p,(void *)&i,sizeof(int));
  p += sizeof(int);
  memcpy((void *)p,(void *)&(ptablaw->m_tabla.m_curcol),sizeof(int));


  return TRUE;
  }
int menos = -1;

p = ((char *)puntero)+sizeof(long)*3;
memcpy((void *)p,(void *)&menos,sizeof(int));

return FALSE;
}

BOOL CWindowDs::EliminaGrid(int ind_grid)
{    
CWnd *pw = NULL;

    if (ind_grid >= 0 && ind_grid < m_IndGrid)
    {
        if (m_Grid[ind_grid])
        {
            int g_modo = m_Grid[ind_grid]->m_gmodo;

            if (m_Grid[ind_grid]->m_tabla.m_confocus)
            {
                if (m_Grid[ind_grid]->m_padre > -1 && m_Grid[m_Grid[ind_grid]->m_padre])
                {
                    for (int i = m_IndGrid - 1;i >= 0 ;i--)
                    {
                        if (i != ind_grid && m_Grid[i] && m_Grid[i]->m_padre == m_Grid[ind_grid]->m_padre)
                        {
                            pw = (CWnd *)&m_Grid[ i ]->m_tabla;                            
                            break;
                        }
                    }
                    if (!pw && m_Grid[ind_grid]->m_padre > -1 && m_Grid[ m_Grid[ind_grid]->m_padre ])
                    {
                        pw = (CWnd *)&m_Grid[ m_Grid[ind_grid]->m_padre ]->m_tabla;                        
                    }
                }
                else
                {
                    for (int i = m_IndGrid - 1;i >= 0 ;i--)
                    {
                        if (i != ind_grid && m_Grid[i] && m_Grid[i]->m_padre > -1)
                        {
                            pw = (CWnd *)&m_Grid[ i ]->m_tabla;                            
                            break;
                        }
                    }
                    if (!pw)
                    {
                        for (int i = m_IndGrid - 1;i >= 0 ;i--)
                        {
                            if (i != ind_grid && m_Grid[ i ])
                            {
                                pw = (CWnd *)&m_Grid[ i ]->m_tabla;                                
                                break;
                            }
                        }
                    }
                }
            }
            delete m_Grid[ind_grid];
            m_Grid[ind_grid] = NULL;
            for (int i = m_IndGrid-1;i >= 0 && !m_Grid[i];i--)
            {
                m_IndGrid--;
            }

            for (i = 0;i < m_IndGrid;i++)
            {
                if (m_Grid[ i ] && !(m_Grid[ i ]->m_gmodo & 4)) 
                {
                    if (!pw)
                    {
                        pw = (CWnd *)&m_Grid[ i ]->m_tabla;
                    }
                    break;
                }
            }

            if ( i >= m_IndGrid)
            {
                m_pEDs->EnableWindow(TRUE);               
            }
            if (!(g_modo & 4))
            {
                ::SendMessage(m_pEDs->m_pView->m_hWnd,WM_COMMAND,ID_REAJUSTASCROLL,0);
            }
            if (!pw)
            {
                pw = (CWnd *)m_pEDs;
            }
            pw->BringWindowToTop();
            pw->SetFocus();
            return TRUE;
        }        
    }

    if (!pw)
    {
        m_pEDs->BringWindowToTop();
        SetFocus();
    }

    return FALSE;
}


BOOL CWindowDs::DesactivaControles(BOOL recursion)
{   
   int i;

   if (recursion)
       m_nivel_recursion++;

   if (!recursion)
   {
       for (int i = 0;i <= m_IndScrollBar;i++)
       {
           if (m_ScrollBar[i])
           {
               m_ScrollBar[i]->EnableWindow(FALSE);
           }
       }
   
       for (i = 0;i <= m_IndTabMenu;i++)
       {
           if (m_TabMenu[i])
           {
               m_TabMenu[i]->EnableWindow(FALSE);
           }
       }
   }

   for (i = 0;i <= m_IndEdit;i++)
   {
       if (m_Edit[i])
       {
           if (recursion)
           {
                if (!m_Edit[i]->m_desactivado)
                {
                    m_Edit[i]->EnableWindow(FALSE);
                    m_Edit[i]->ShowWindow(SW_HIDE);
                    m_Edit[i]->m_desactivado = m_nivel_recursion;
                }
           }
           else
                m_Edit[i]->EnableWindow(FALSE);
       }
   }


   if (!recursion) // de momento
   {
       for (i = 0;i < m_IndGrid;i++)
       {
            if (m_Grid[i])
            {
                m_Grid[i]->EnableWindow(FALSE);
            }
       }
   }

   return FALSE;
}

BOOL CWindowDs::ActivaControles(BOOL recursion)
{
   int i;

   if (!recursion)
   {
       for (int i = 0;i <= m_IndScrollBar;i++)
       {
           if (m_ScrollBar[i])
           {
               m_ScrollBar[i]->EnableWindow(TRUE);
           }
       }
   
       for (i = 0;i <= m_IndTabMenu;i++)
       {
           if (m_TabMenu[i])
           {
               m_TabMenu[i]->EnableWindow(TRUE);
           }
       }
   }

   for (i = 0;i <= m_IndEdit;i++)
   {
       if (m_Edit[i])
       {
           if (recursion)
           {
               if (m_Edit[i]->m_desactivado >= m_nivel_recursion)
               {
                   m_Edit[i]->EnableWindow(TRUE);
                   m_Edit[i]->ShowWindow(SW_SHOW);
                   m_Edit[i]->SetFocus(); // el foco habria de ser por ultimo con foco
                   m_Edit[i]->m_desactivado = 0;
               }
           }
           else
           {
               m_Edit[i]->EnableWindow(TRUE);
               m_Edit[i]->SetFocus(); // el foco habria de ser por ultimo con foco
           }
       }
   }

   if (!recursion)
   {
       for (i = 0;i < m_IndGrid;i++)
       {
            if (m_Grid[i])
            {
                m_Grid[i]->EnableWindow(TRUE);
                if (m_Grid[i]->m_tabla.m_confocus)
                {
                    m_Grid[i]->BringWindowToTop();
                    m_Grid[i]->SetFocus(); // el foco habria de ser por ultimo con foco
                }
            }
       }
   }

   if (recursion)
       m_nivel_recursion--;

   return FALSE;
}





BOOL CWindowDs::OnEraseBkgnd(CDC* pDC) 
{
    /*
    RECT r;

    pDC->GetClipBox( &r );
    //GetClientRect( &r );
    if ((!theApp.m_EntornoActivo && m_pEDs->m_wallowed == this) || (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed == this))	
      llama_fpinta_zona(m_pEDs->m_fpinta_zona,pDC->m_hDC,r,NULL);	
    */

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}



void CWindowDs::PonFoco()
{
   if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed && theApp.m_EntornoActivo->m_wallowed != this	)
      theApp.m_EntornoActivo->m_wallowed->PonFoco();	
   else
   {
      if (m_Edit[m_IndEdit] && m_Edit[m_IndEdit]->m_hWnd)
      {
         m_Edit[m_IndEdit]->SetFocus();
      }
      else
      {
          for (int i = 0;i < m_IndGrid;i++)
          {
              if (m_Grid[i] && m_Grid[i]->m_hWnd && m_Grid[i]->m_tabla.m_confocus)
              {
                  m_Grid[i]->SetFocus();
                  break;
              }
          }
          if (i >= m_IndGrid || !m_Grid[i]) // hay que poner ventana con el foco  corriente en variable
          {
              CWnd *pWnd = GetFocus();
              if (!pWnd || pWnd->m_hWnd != m_hWnd)
              {
                  SetFocus();
              }
          }
      }
   }
}

BOOL CWindowDs::PintaCampo(int tipo, int id_control, int id_campo,char *dato)
{
    switch(tipo)
    {
    case IDW_WGRID:
        theApp.m_EntornoActivo->m_wallowed->m_Grid[id_control]->m_tabla.PintaIdCampo(id_campo,dato);
        return TRUE;        
    default:
        break;
    }
    return FALSE;
}

BOOL CWindowDs::PreTranslateMessage(MSG* pMsg) 
{
    if (m_tooltip.m_hWnd)
       m_tooltip.RelayEvent( pMsg );
	
	return PARENTCWindowDs::PreTranslateMessage(pMsg);
}


int CWindowDs::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (PARENTCWindowDs::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)::GetStockObject(LTGRAY_BRUSH));
	
	return 0;
}


int DesactivaControles()
{
    if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed)
    {
        theApp.m_EntornoActivo->m_wallowed->DesactivaControles(TRUE);        
    }

    return 0;
}

int ActivaControles()
{
    if (theApp.m_EntornoActivo && theApp.m_EntornoActivo->m_wallowed)
    {
        theApp.m_EntornoActivo->m_wallowed->ActivaControles(TRUE);
    }

    return 0;
}


#endif