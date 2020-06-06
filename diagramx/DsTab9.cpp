// DsTab9.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsxView.h"
#include "Ds9Window.h"
#include "DsTab9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsTab9

CDsTab9::CDsTab9()
{
	m_cursel = 0;
#ifdef DSPROFUIS9
	m_nosel = 0;
	m_truco = new CWnd();
	m_truco->Create(NULL,"Truco",WS_CHILD,CRect(10,10,10,10),theApp.m_pMainWnd,8261,NULL);

	m_estab9 = TRUE;
#endif
}

CDsTab9::~CDsTab9()
{
#ifdef DSPROFUIS9
	if (m_truco)
	{
		m_truco->DestroyWindow();
		delete m_truco;
	}
#endif
}


BEGIN_MESSAGE_MAP(CDsTab9, BASEDsTab9)
	//{{AFX_MSG_MAP(CDsTab9)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsTab9 message handlers

/* asuntos del 9 'tal cual'*/
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


int flag_ignoramus = 0;

void PonPantallasEnTab(int npan,char *titulo)
{
	if (flag_ignoramus > 0)
	{
		flag_ignoramus--;
		return;
	}

   if (npan == 0xffff)
   {
      p_b_titulo = titulo;
      p_b_titulo.TrimRight();
      if (p_titulo != "")
         return;
   }
   else
   {

      int i;

      if (npan == 0 && titulo == NULL)
      {
          p_npan = a_p_npan;
          p_titulo = a_p_titulo;
          for (i = 0;i < 100;i++)
          {
              p_titulos[i] = a_p_titulos[i];
          }       
          p_b_titulo = a_p_b_titulo;
          b_titulo   = a_b_titulo;
          p_MiContador++;
      }
      else
      {
          a_p_npan = p_npan;
          a_p_titulo = p_titulo;
          for (i = 0;i < 100;i++)
          {
              a_p_titulos[i] = p_titulos[i];
          }       
          a_p_b_titulo = p_b_titulo;
          a_b_titulo   = b_titulo;

          p_MiContador++;
          p_npan = npan;
          if (p_npan < 101)
          {
             p_titulo = titulo;   
             p_titulo.TrimRight();
          }
          else
          {
             p_titulo = p_b_titulo;
             int tope = p_npan - 100;
             if (tope > 100) tope = 100;
             char **arrai = (char **)titulo;	 
             for (i = 0;i < tope;i++)
             {
                p_titulos[i] = arrai[i];
                p_titulos[i].TrimRight();
             }
          }
       }
   }


   if (!theApp.m_9wnd || !::IsWindow(theApp.m_9wnd->m_hWnd) || !theApp.m_9wnd->m_tab9)
	   return;

   CDsTab9 *tab9 = theApp.m_9wnd->m_tab9;
   
   tab9->DeleteAllItems( );
   tab9->SetItemExtra( 0 );
 
   CString tmp;
   if (p_npan > 1)
   {
		if (p_npan > 100)
		   tmp.Format("%s",(LPCSTR)p_titulos[0]);
		else
		   tmp.Format("%s -1-",(LPCSTR)p_titulo);
   }
   else
   {
		if (p_titulo == "")
		   tmp.Format("%s",(LPCSTR)p_b_titulo);
		else
		   tmp.Format("%s",(LPCSTR)p_titulo);
   }
   TCITEM Item;
   Item.mask = TCIF_TEXT | TCIF_PARAM;
   Item.pszText = (char *)(LPCSTR)tmp;
   Item.cchTextMax = tmp.GetLength();
   Item.lParam = 1;
   Item.iImage = -1;
#ifdef DSPROFUIS9
   tab9->m_textos.SetAtGrow(0,CString(Item.pszText));   
   tab9->m_dlgs.SetAtGrow(0,theApp.m_9wnd);
   tab9->m_flag.SetAtGrow(0,0);   
   tab9->m_nosel++;
   tab9->PageInsert(tab9->m_truco,Item.pszText,NULL,true,0,false);   
   tab9->m_nosel--;
   {
	 CRect rtab,ctab;
	 tab9->GetTabWndRect(rtab);
	 theApp.m_9wnd->GetWindowRect(ctab);
	 ctab.MoveToXY(rtab.left,rtab.bottom);
     theApp.m_9wnd->GetParent()->ScreenToClient(&ctab);   
	 theApp.m_9wnd->MoveWindow(ctab);
   }

#else
   if (tab9->InsertItem(0,&Item) < 0)
   {
		/*ERROR*/
		//m_pMainFrame->PostMessage(WM_QUIT);
   }   
   //theApp.m_9wnd->InvalidateRect(NULL);
#endif
   int tope = (p_npan < 101 ? p_npan : p_npan - 100);
   for (int i = 2;i <= tope;i++)
   {
		if (p_npan > 100)
		   tmp.Format("%s",(LPCSTR)p_titulos[i-1]);
		else
		   tmp.Format("   -%d-",i);
		Item.mask = TCIF_TEXT| TCIF_PARAM;
		Item.pszText = (char *)(LPCSTR)tmp;
		Item.cchTextMax = tmp.GetLength();
		Item.lParam = i;
		Item.iImage = -1;
#ifdef DSPROFUIS9
		tab9->m_textos.SetAtGrow(i-1,CString(Item.pszText));
		tab9->m_dlgs.SetAtGrow(i-1,theApp.m_9wnd);   
		tab9->m_flag.SetAtGrow(i-1,0);   
		tab9->m_nosel++;
		tab9->PageInsert(tab9->m_truco,Item.pszText,NULL,true,i-1,false);		
		tab9->m_nosel--;
#else
		tab9->InsertItem(i-1,&Item);
#endif
   }
#ifdef DSPROFUIS9
   tab9->SetCurSel( 0 );
#endif
   tab9->m_cursel = 0;

   theApp.m_9wnd->SetFocus();

}


void NotificaPantalla(int pan,char *titulo)
{
   if (!theApp.m_9wnd || !theApp.m_9wnd->m_tab9)
	   return;

   /*
   if (theApp.m_9wnd->m_tab9 && !::IsWindowVisible(theApp.m_9wnd->m_tab9->m_hWnd))
   {
	  ::ShowWindow(theApp.m_9wnd->m_tab9->m_hWnd,SW_SHOW);	
   }
   */
   

   CDsTab9 *tab9 = theApp.m_9wnd->m_tab9;

   int op = pan;
   int old_op = tab9->GetCurSel();
   if (op != old_op)
   {
	   if (op < tab9->GetItemCount())
	   {
		  tab9->PintaOpcion(old_op,op);	   
		  tab9->SetCurSel( op );
	   }
   }
}


#ifdef DSPROFUIS9
int CDsTab9::SetCurSel(int nItem )
{
	m_nosel++;
	PageSelectionSet(nItem);
	m_nosel--;
	return nItem;
}


void CDsTab9::DeleteAllItems( )
{
	BOOL verla;
	verla = ShowWindow(SW_HIDE);		
	m_nosel++;
	CDsTabCtrl::DeleteAllItems( );
	m_nosel--;
	if (verla)
		ShowWindow(SW_SHOW);
}

bool CDsTab9::OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		)
{	
	if (nNewItemIndex >= 0 && nNewItemIndex < m_flag.GetCount() && (m_flag[nNewItemIndex]&1))
		return false;

	bool ret = CExtTabPageContainerFlatWnd::OnTabWndSelectionChange(nOldItemIndex,nNewItemIndex,bPreSelectionTest);
	if (ret && bPreSelectionTest)
	{
		if (!m_nosel)
		{
			int n = GetItemCount();
			if (nNewItemIndex < n && nOldItemIndex < n)
			{
				if (nNewItemIndex != nOldItemIndex && m_flag.GetCount() > 0)
				{
					LRESULT Result = 0;
					OnSelchange((NMHDR*)nNewItemIndex, &Result);
				}
				return false;
			}
		}
	}

	return ret;

	/*

	if (m_nosel)
		return ret;

	//if (ret)
	{
		LRESULT Result = 0;
		if (nNewItemIndex != nOldItemIndex && m_flag.GetCount() > 0)
		{
			OnSelchange(NULL, &Result);
		}
	}
	return ret;
	*/
}
#endif

void CDsTab9::PintaOpcion(int old_op, int op)
{
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
}

void CDsTab9::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int cur;
#ifdef DSPROFUIS9
   if (m_nosel)
	   return;
   cur = (int)pNMHDR;
#else
   cur = GetCurSel();
#endif   
   if (cur != -1 && cur != m_cursel)
   {
      int cuantos;
      short mensaje;
      int ant_sel = m_cursel;
      if (cur < m_cursel)
      {
         mensaje = SOLICITA_RETROCESO;
/*
#ifdef DSPROFUIS9
		 cur = m_cursel - 1;
#endif
*/
         cuantos = m_cursel - cur - 1;
      }
      else
      {
         mensaje = SOLICITA_AVANCE;
/*
#ifdef DSPROFUIS9
		 cur = m_cursel + 1;
#endif
*/
         cuantos = cur - m_cursel - 1;
      }

	  

      int mensa = (int)(unsigned short)mensaje;	   
      m_cursel = cur;
      if (theApp.m_9wnd)
      {		  
         theApp.m_9wnd->PostMessage(WM_COMMAND,(WPARAM)mensa,(LPARAM)cuantos);
      }
      PintaOpcion(ant_sel,cur);	   
      *pResult = 1;
   }
   else
      *pResult = 0;
}

void CDsTab9::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (!m_hWnd) return;

   if ((nFlags & MK_LBUTTON))
   {				
	  CRect rtab;
	  CView *pView = (CView *)GetParent();
	  GetWindowRect(&rtab);
      CRectTracker tracker(&rtab,CRectTracker::hatchedBorder);
      CPoint p(point.x+rtab.left,point.y+rtab.top);
      if (tracker.Track(this,p,FALSE,(CWnd *)pView))
      {			 
		 pView->ScreenToClient(&rtab);		 
		 rtab.OffsetRect(tracker.m_rect.left,tracker.m_rect.top);
		 MoveWindow(rtab);    
         return;
      }		
   }
   BASEDsTab9::OnMouseMove(nFlags, point);
}

BOOL CDsTab9::PreTranslateMessage(MSG* pMsg) 
{	
	return BASEDsTab9::PreTranslateMessage(pMsg);
}

void CDsTab9::GetTabWndRect(CRect &rect)
{
#ifndef DSPROFUIS9
    BASEDsTab9::GetWindowRect(rect);
#else
	m_pWndTab->GetWindowRect(rect);
#endif
}


void CDsTab9::CenterWindow( CWnd* pAlternateOwner )
{
#ifdef DSPROFUIS9
#else
	CWnd::CenterWindow( pAlternateOwner );
#endif
}