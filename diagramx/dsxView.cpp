// dsxView.cpp : implementation of the CDsxView class
//

#include "stdafx.h"
#include "diagramx.h"

#include "DsxDoc.h"
#include "DsxCntrItem.h"
#include "dsxView.h"

#include "DsxMainFrm.h"
#include "DsStdDlg.h"
#include "DsTabCtrl.h"
#include "DlgErrorFichero.h"

#include "Ds9Window.h"

#include "SkinScrollWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int Post_ID_AVISO_MUEREMETA = 0;
/////////////////////////////////////////////////////////////////////////////
// CDsxView

IMPLEMENT_DYNCREATE(CDsxView, CScrollView /*CScrollView*/)

BEGIN_MESSAGE_MAP(CDsxView, CScrollView /*CScrollView*/)
	//{{AFX_MSG_MAP(CDsxView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
    ON_WM_DESTROY()
    ON_WM_SETFOCUS()
    ON_WM_SIZE()
    ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
    ON_UPDATE_COMMAND_UI(ID_OLE_INSERT_NEW, OnUpdateOleInsertNew)
    ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDsxView construction/destruction

CDsxView::CDsxView()
{
   m_pSelection = NULL;      
   m_errorf = NULL;
   m_html = NULL;
   m_bmp  = NULL;
   m_listaoculta = NULL;
}

CDsxView::~CDsxView()
{
   FinErrorFichero();

   POSITION pos = m_dlglist.GetStartPosition( );
   int wid;
   CDsStdDlg *pdlg;
   while(pos)
   {
       m_dlglist.GetNextAssoc( pos, wid, pdlg );
       pdlg->DestroyWindow();
       delete pdlg;       
   }
   m_dlglist.RemoveAll();   

   if (m_bmp)
   {
       delete m_bmp;
   }
   // m_html no se destruye ya que el propio destroy window (invocado al destruirse la view)
   // libera la memoria.
   /*
   if (m_html)
   {
       if (::IsWindow(m_html->m_hWnd))
       {
           m_html->DestroyWindow();
       }
       else
           delete m_html;
   }
   */
}

BOOL CDsxView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs
   cs.style |= WS_CLIPCHILDREN;

   return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxView drawing

void CDsxView::OnDraw(CDC* pDC)
{	
   //CDsxDoc* pDoc = GetDocument();
   //ASSERT_VALID(pDoc);
   if (pDC->IsPrinting( ))
   {
      //pDC->ExtTextOut(0,0,ETO_OPAQUE,NULL,"HOLA HOLA",9,NULL);
   }
   else 
   {
	   
	   /*
	     if (!(theApp.m_usarweb) && theApp.m_9wnd)
		 {
			 return;
		 }
		*/

         if (theApp.m_usarweb && !m_html)
         {
			 try
			 {
				ActivaHtml();
			 } catch( ... )
			 {
				 theApp.m_usarweb = FALSE;
				 m_html = NULL;
			 }
         }

//#ifndef DSPROFUIS9
         if (!(GetDocument()->StatusEnWinsysMenu())) 
         {
            if (m_html)
            {
                if (theApp.m_hidehtml)
                {
                   m_html->ShowWindow(SW_HIDE);
                }
                else
                {
                   m_html->ShowWindow(SW_SHOW);
                }
                return;
            }
            if (theApp.m_hidehtml)
                return;
         }
//#endif

         if (m_html)             
         {           
            m_html->ShowWindow(SW_SHOW);
            return;
         }

		 if (!(GetDocument()->StatusEnWinsysMenu())) 
			 return;

         //return;
         
         CRect rect;
         
         GetClientRect(rect);         

         if (!pDC->RectVisible(rect))
         {
             return;
         }

         if (!m_bmp)
         {             
 		     HANDLE mi_bmp;         
             CString s;
             m_bmp = new CBitmap();

             s = theApp.m_dirbase;
             
             s += "logonet.bmp";             

             mi_bmp = ::LoadImage(theApp.m_hInstance,s,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE | LR_LOADFROMFILE );
             if (mi_bmp)
             {
                m_bmp->Attach(mi_bmp);                
             }
         } 		 
		 BITMAP bmp,bmp2;
		 HBITMAP hbmp2,hbmpold;
		 CDC dc;
		 int px,py;         
		 int buffersize,buffersize2;
		 BYTE* buffer,* buffer2;
		 int i,x;
		 BYTE* lpb1,*lpb2;	 		 

		 GetClientRect(rect);

		 ::GetObject (m_bmp->m_hObject, sizeof(BITMAP), &bmp);

         px = (rect.Size().cx - bmp.bmWidth) / 2;
         py = (rect.Size().cy - bmp.bmHeight) / 2;

		 hbmp2 = ::CreateCompatibleBitmap(pDC->m_hDC,bmp.bmWidth,bmp.bmHeight);
		 dc.CreateCompatibleDC(pDC);
		 hbmpold = (HBITMAP)dc.SelectObject(hbmp2);
		 dc.BitBlt(0,0,bmp.bmWidth,bmp.bmHeight,pDC,px,py,SRCCOPY);

		 ::GetObject (hbmp2, sizeof(BITMAP), &bmp2);

         buffersize = bmp.bmWidthBytes * bmp.bmHeight;
		 buffersize2 = bmp2.bmWidthBytes * bmp2.bmHeight;
		 buffer = (BYTE*)malloc(buffersize);
		 buffer2 = (BYTE*)malloc(buffersize2);
		 m_bmp->GetBitmapBits(buffersize,buffer);
		 ::GetBitmapBits(hbmp2,buffersize2,buffer2);

		 lpb1 = buffer;
		 lpb2 = buffer2;
		 for (i = 0;i < bmp.bmHeight;i++)
		 {			
			for (x = 0;x < bmp.bmWidth;x++)
			{
				if (lpb1[0] != 255 || lpb1[1] != 255 || lpb1[2] != 255)
				{
					lpb2[0] = lpb1[0];
					lpb2[1] = lpb1[1];
					lpb2[2] = lpb1[2];
				}
				lpb1 += 4;
				lpb2 += 4;
			}
		 }
		 ::SetBitmapBits(hbmp2,buffersize2,buffer2);

		 free(buffer);
		 free(buffer2);

		 pDC->BitBlt(px,py,bmp.bmWidth,bmp.bmHeight,&dc,0,0,SRCCOPY);

		 dc.SelectObject(hbmpold);
		 ::DeleteObject(hbmp2);

   }
}

/////////////////////////////////////////////////////////////////////////////
// CDsxView printing

BOOL CDsxView::OnPreparePrinting(CPrintInfo* pInfo)
{
   
   if (!CView::DoPreparePrinting(pInfo))
      return FALSE;
   
   if (!COleDocObjectItem::OnPreparePrinting(this, pInfo))
      return FALSE;
   
   return TRUE;
}

void CDsxView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   // TODO: add extra initialization before printing
}

void CDsxView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   // TODO: add cleanup after printing
}

void CDsxView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
   // TODO: add customized printing code here
   if(pInfo->m_bDocObject)
      COleDocObjectItem::OnPrint(this, pInfo, TRUE);
   else
   {
       /*
      if (theApp.m_PrimerEntorno && theApp.m_PrimerEntorno->m_wallowed)
      {		   	   
         CRect r,r1;
         {
            CWnd *e = (CWnd *)theApp.m_PrimerEntorno;
            e->GetWindowRect(r);
            pDC->BitBlt(0,0,r.Size().cx,r.Size().cy,CDC().FromHandle(::GetDC(e->m_hWnd)),0,0,SRCCOPY);
         }
         
         {
            CWindowDs *w = theApp.m_PrimerEntorno->m_wallowed;           
            w->GetWindowRect(r1);
            pDC->BitBlt(r1.left-r.left,r1.top-r.top,r1.Size().cx,r1.Size().cy,CDC().FromHandle(::GetDC(w->m_hWnd)),0,0,SRCCOPY);
         }
         
      }
      */
      //CView::OnPrint(pDC, pInfo);
   }
}


void CDsxView::OnInitialUpdate()
{
   CScrollView::OnInitialUpdate();
   
   
   // TODO: remove this code when final selection model code is written
   m_pSelection = NULL;    // initialize selection
   
   //Active documents should always be activated
   CDsxDoc* pDoc = (CDsxDoc*) GetDocument();
   if (pDoc != NULL)
   {
      // activate the first one
      POSITION posItem = pDoc->GetStartPosition();
      if (posItem != NULL)
      {
         CDocItem* pItem = pDoc->GetNextItem(posItem);
         
         // only if it's an Active document
         COleDocObjectItem *pDocObjectItem =
            DYNAMIC_DOWNCAST(COleDocObjectItem, pItem);
         
         if (pDocObjectItem != NULL)
         {
            pDocObjectItem->DoVerb(OLEIVERB_SHOW, this);
         }
      }
   }
   
   CSize s(1,1);
   SetScrollSizes(MM_TEXT,s);  
   SetScrollDs();

   theApp.m_iStdView = this;


   //ActivaHtml();
}


void CDsxView::OnDestroy()
{
   // Deactivate the item on destruction; this is important
   // when a splitter view is being used.
   CScrollView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CDsxView::IsSelected(const CObject* pDocItem) const
{
   // The implementation below is adequate if your selection consists of
   //  only CDsxCntrItem objects.  To handle different selection
   //  mechanisms, the implementation here should be replaced.
   
   // TODO: implement this function that tests for a selected OLE client item
   
   return pDocItem == m_pSelection;
}

void CDsxView::OnInsertObject()
{
   // Invoke the standard Insert Object dialog box to obtain information
   //  for new CDsxCntrItem object.
   if (!theApp.GetFeatureVersion())
      return;
   
   COleInsertDialog dlg;
   if (dlg.DoModal(COleInsertDialog::DocObjectsOnly) != IDOK)
      return;
   
   BeginWaitCursor();
   
   CDsxCntrItem* pItem = NULL;
   TRY
   {
      // Create new item connected to this document.
      CDsxDoc* pDoc = GetDocument();
      ASSERT_VALID(pDoc);
      pItem = new CDsxCntrItem(pDoc);
      ASSERT_VALID(pItem);
      
      // Initialize the item from the dialog data.
      if (!dlg.CreateItem(pItem))
         AfxThrowMemoryException();  // any exception will do
      ASSERT_VALID(pItem);
      
      pItem->DoVerb(OLEIVERB_SHOW, this);
      
      ASSERT_VALID(pItem);
      
      // As an arbitrary user interface design, this sets the selection
      //  to the last item inserted.
      
      // TODO: reimplement selection as appropriate for your application
      
      m_pSelection = pItem;   // set selection to last inserted item
      pDoc->UpdateAllViews(NULL);
   }
   CATCH(CException, e)
   {
      if (pItem != NULL)
      {
         ASSERT_VALID(pItem);
         pItem->Delete();
      }
      AfxMessageBox(IDP_FAILED_TO_CREATE);
   }
   END_CATCH
      
      EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CDsxView::OnCancelEditCntr()
{
   // Close any in-place active item on this view.
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL)
   {
      pActiveItem->Close();
   }
   ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CDsxView::OnSetFocus(CWnd* pOldWnd)
{
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL &&
      pActiveItem->GetItemState() == COleClientItem::activeUIState)
   {
      // need to set focus to this item if it is in the same view
      CWnd* pWnd = pActiveItem->GetInPlaceWindow();
      if (pWnd != NULL)
      {
         pWnd->SetFocus();   // don't call the base class
         return;
      }
   }

    if (theApp.m_noreenfocarpordefecto == 0)
	{
    if (theApp.ActivacionPorDefecto())
              return;
	}

    CScrollView::OnSetFocus(pOldWnd);
}



void CDsxView::OnSize(UINT nType, int cx, int cy)
{
   CScrollView::OnSize(nType, cx, cy);   
   
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL)
      pActiveItem->SetItemRects();

   SetScrollDs();
   if (m_html && ::IsWindow(m_html->m_hWnd))
   {
       CRect rect;
       GetClientRect(rect);
       m_html->MoveWindow(rect);
   }
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CDsxView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxView diagnostics

#ifdef _DEBUG
void CDsxView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDsxView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDsxDoc* CDsxView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDsxDoc)));
	return (CDsxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDsxView message handlers

void CDsxView::OnUpdateOleInsertNew(CCmdUI* pCmdUI)
{   
   if (!theApp.GetFeatureVersion())
      pCmdUI->Enable( FALSE );   
}

void CDsxView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   // TODO: Add your message handler code here and/or call default
   //CScrollView::OnLButtonDown(nFlags, point);
}

void CDsxView::SetScrollDs()
{   
   CSize size,size1,size2;     
   int mapmode = 0;
   RECT rr;
   CRect rr1,rro;
   BOOL hay = FALSE;
   
   
   GetDeviceScrollSizes( mapmode,size,size1,size2);
   if (!mapmode) return;
   

   GetWindowRect(&rr1);
   rro = rr1;
   
   HWND hwnd = ::GetTopWindow(m_hWnd);
   while(hwnd)
   {
     if (m_html && hwnd == m_html->m_hWnd) 
     {
         hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT);
         continue;
     }
     hay = TRUE;
     ::GetWindowRect(hwnd,&rr);
     if (rr.left < rr1.left)
     {
         rr1.left = rr.left;
     }
     if (rr.right < rr1.right)
     {
         rr1.right = rr.right;
     }
     if (rr.top < rr1.top)
     {
         rr1.top = rr.top;
     }
     if (rr.bottom > rr1.bottom)
     {
         rr1.bottom = rr.bottom;
     }
     hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT);   
   }

   if (!hay)
   {
       CSize s(1,1);
       SetScrollSizes(mapmode,s);
       return;
   }


   if (rro.Size().cx == rr1.Size().cx  && rro.Size().cy == rr1.Size().cy) return;

   size = rr1.Size();

   SetScrollSizes(mapmode,size);
   
   POINT pt;
   pt.x = rr1.left - rro.left;
   if (pt.x < 0) pt.x = 0;
   pt.y = rr1.top - rro.top;
   if (pt.y < 0) pt.y = 0;


	if (m_nMapMode != MM_TEXT)
	{
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
		dc.LPtoDP((LPPOINT)&pt);
	}

	// now in device coordinates - limit if out of range
	int xMax = GetScrollLimit(SB_HORZ);
	int yMax = GetScrollLimit(SB_VERT);
	if (pt.x < 0)
		pt.x = 0;
	else if (pt.x > xMax)
		pt.x = xMax;
	if (pt.y < 0)
		pt.y = 0;
	else if (pt.y > yMax)
		pt.y = yMax;

	if (pt.x < 0)
		return;
	if (pt.y < 0)
		return;

	ScrollToDevicePosition(pt);   

}

BOOL CDsxView::OnCommand(WPARAM wParam, LPARAM lParam) 
{	
	if ( (wParam  & 0xffff) == ID_AVISO_GRIDMUERE)
    {
         char *p;
         long tam;
         long respuesta;
         long modo = 1;
         int operacion = 9999;
         char tmp[512];
         int localizador = (int)lParam;

         p = tmp;

         memcpy((void *)p,(void *)&localizador,sizeof(int));
         p += sizeof(int);

         tam = (sizeof(long) * 4) + (sizeof(int)*3) + 100;
         respuesta = tam;

         memcpy((void *)p,(void *)&tam,sizeof(long));
         p += sizeof(long);	
         memcpy((void *)p,(void *)&respuesta,sizeof(long));
         p += sizeof(long);
         memcpy((void *)p,(void *)&modo,sizeof(long));
         p += sizeof(long);
         memcpy((void *)p,(void *)&operacion,sizeof(int));
         p += sizeof(int);         

		 if (theApp.m_9wnd)
		 {
			pDsSys->DespachadorInterno9(13,(void *)tmp);
		 }
		 else
		 {
			pDsSys->DespachadorInterno(13,(void *)tmp);
		 }

         memcpy((void *)&operacion,(void *)p,sizeof(int));
         if (operacion != -1)
         {
            CDsStdCtrl *pctrl;
            if (CDsStdDlg::m_todosctrl.Lookup(operacion,pctrl))
            {
                if (pctrl->m_ctrl && ::IsWindow(pctrl->m_ctrl->m_hWnd))
                {
                   pctrl->m_ctrl->SetFocus();
                }
            }
         }
         return TRUE;
    }
    else
	if ( (wParam & 0xffff) == ID_REAJUSTASCROLL)
    {
        SetScrollDs();
        return TRUE;
    }
    else
	if ( (wParam  & 0xffff) == ID_AVISO_MUEREMETA)
    {	   
	   if (Post_ID_AVISO_MUEREMETA)
	   {
		   /*CDsMetaCombo *pMeta = (CDsMetaCombo *)lParam;
		   pMeta->DestroyWindow();
		   delete pMeta;*/
		   Post_ID_AVISO_MUEREMETA--;		   
		   /*
           if (GetDocument()->m_metacombo == pMeta)
		   {
			   GetDocument()->m_metacombo = NULL;
		   }
		   */
	   }	   
	   return TRUE;
    }
    else
    {
	   return CScrollView::OnCommand(wParam, lParam);
    }
}

int CDsxView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{	
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1; 

#ifdef DSPROFUIS	
/**** Especial */
	//SkinWndScroll(this);
/**** fin Especial */

#ifdef DSPROFUIS_ANOMALO
	// eliminado el resto de codigo de custom scrollbars para scrollview -> No van a nivel de mfc (esta incompleta parece)
	m_wndScrollBarH.m_eSO = CExtScrollBar::__ESO_BOTTOM;
	m_wndScrollBarV.m_eSO = CExtScrollBar::__ESO_RIGHT;
	if( ! m_wndScrollBarV.Create(
			WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN,
			CRect(10,10,10,10),
			this,
			1
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if( ! m_wndScrollBarH.Create(
			WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_BOTTOMALIGN,
			CRect(10,10,10,10),
			this,
			2
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	m_wndScrollBarH.SyncReservedSpace( &m_wndScrollBarV );
	m_wndScrollBarV.SyncReservedSpace( &m_wndScrollBarH );
#endif

	/*
	::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)::CreateSolidBrush((COLORREF)g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT )));
	*/
#else
    ::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)::GetSysColorBrush(COLOR_3DFACE));
#endif

	EnableToolTips(TRUE);
	
	return 0;
}

BOOL CDsxView::OnEraseBkgnd(CDC* pDC) 
{
	CRect  rect;
	GetClientRect(rect);

#ifdef DSPROFUIS
	COLORREF colorfondo;
	if (theApp.m_colorset5)
	{
		colorfondo = theApp.m_color5;
	}
	else
	{
		colorfondo = g_PaintManager->GetColor(
				CExtPaintManager::CLR_3DFACE_OUT, this
				);
	}

	
	if (theApp.m_aparienciabase == 3)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CExtPaintManagerSkin *)pMFrame->m_paintmanager)->PaintDockerBkgnd( true, *pDC, this , colorfondo))		
			)
			pDC->FillSolidRect(
				&rect,
				colorfondo
				);
	}
	else if (theApp.m_aparienciabase == 2)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CDsProfuisCustomOffice2007_R1 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, *pDC, this , colorfondo))		
			)
			pDC->FillSolidRect(
				&rect,
				colorfondo
				);
	}
	else if (theApp.m_aparienciabase == 1)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CDsProfuisCustomOffice2003 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, *pDC, this , colorfondo))		
			)
			pDC->FillSolidRect(
				&rect,
				colorfondo
				);
	}
	return TRUE;
#else
	::FillRect(pDC->m_hDC,rect,::GetSysColorBrush(COLOR_3DFACE));
	return TRUE;
#endif
}


void CDsxView::ocultaVentanas()
{
    int wid;
	if (m_listaoculta)
		return;
    CDsStdDlg *pdlg;
	m_listaoculta = new CDsStdDlg *[500];	
	int nlistaoculta = 0;
    POSITION pos = m_dlglist.GetStartPosition( );
    while (pos) {
		pdlg = NULL;
        m_dlglist.GetNextAssoc( pos, wid, pdlg );
		if (pdlg != NULL && pdlg->m_sid == 0) {
			if (pdlg->IsWindowVisible()) {
				m_prioridad[nlistaoculta] = 500;
				m_listaoculta[nlistaoculta++] = pdlg;			
			}
		}
    }
	m_listaoculta[nlistaoculta] = NULL;

	int nivelprioridad = 0;
	HWND hwnd = ::GetTopWindow(this->m_hWnd);
	while(hwnd) {
		for (int i = 0;m_listaoculta[i];i++) {
			if (m_listaoculta[i]->m_hWnd == hwnd) {
				m_prioridad[i] =  nivelprioridad;
				break;
			}
		}
		hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT);
	}
	for (int i = 0;m_listaoculta[i];i++) {
		m_listaoculta[i]->ShowWindow(SW_HIDE);
	}
}

void CDsxView::muestraVentanas() {
   if (!m_listaoculta)
	   return;

    int nivelprioridad = 500;
	while(nivelprioridad >= 0) {
		for (int i = 0;m_listaoculta[i];i++) {
			if (nivelprioridad == m_prioridad[i]) {
			   m_listaoculta[i]->ShowWindow(SW_SHOW);
			}
		}
		nivelprioridad--;
	}
	delete m_listaoculta;
	m_listaoculta = NULL;
}

BOOL CDsxView::EliminaStdDlg(int stdid)
{
   CDsStdDlg *pdlg;
   
   if (m_dlglist.Lookup(stdid,pdlg))
   {
       ASSERT( stdid == pdlg->m_wid );
       /* eliminar todas sus dependientes!!! */
       if (!pdlg->m_replicante)
       {
         m_dlglist.RemoveKey(stdid);
       }

       CDsStdDlg *d_pdlg;
       int d_wid;

       POSITION pos = pdlg->m_listahijos.GetStartPosition( );

       while (pos)
       {
           pdlg->m_listahijos.GetNextAssoc( pos, d_wid, d_pdlg );
           if ((pdlg->m_id+pdlg->m_replicante) == d_pdlg->m_id || !pdlg->m_replicante)
           {
              EliminaStdDlg(d_wid);
           }
       }
       /* Ojo con la borra descendecia */
       if (!pdlg->m_replicante)
       {
           int o_sid = pdlg->m_sid;

           pdlg->DestroyWindow();
           delete pdlg;       

           if (!o_sid)
           {
             SetScrollDs();
           }
       }
       else
       {
           int wid;
           CDsStdCtrl *pctrl;
           pos = pdlg->m_ctrlst.GetStartPosition( );
           while(pos)
           {
               pdlg->m_ctrlst.GetNextAssoc( pos, wid, pctrl );

               if (pctrl->m_p_id == (pdlg->m_id+pdlg->m_replicante))
               {
                   if (pctrl->m_pdlg == pdlg)
                   {
                       pdlg->DestruyeControl(wid);
                   }
               }
           }

           pdlg->m_replicante--;
       }
       return TRUE;
   }
   return FALSE;
}

CDsStdDlg * CDsxView::BuscaStdDlg(int id, int sid, int& stdid)
{
   POSITION pos = m_dlglist.GetStartPosition( );
   int wid;
   CDsStdDlg *pdlg;
   while(pos)
   {
       m_dlglist.GetNextAssoc( pos, wid, pdlg );       
       if ((pdlg->m_id+pdlg->m_replicante) == id && pdlg->m_sid == sid)
       {
		   if (pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window)) && pdlg != theApp.m_9wnd)
			   continue;
           stdid = wid;
           return pdlg;
       }
   }
//   ASSERT(FALSE);
   return NULL;
}

int CDsxView::InsertaControl(int wid,int id,int sid, int tipo, int lpos, int lpf,void *dato,int Atrib, int Cc, int Cf,int prev_id)
{
   CDsStdDlg *pdlg = LocalizaDlg(wid,id,sid,lpos,lpf);
   if (!pdlg)
   {
       ASSERT(FALSE);
       // usar el '0'
       pdlg = LocalizaDlg(-1,id,0,lpos,lpf);
   }
   if (pdlg)
   {
       return pdlg->InsertaControl(tipo,lpos,lpf,dato,Atrib,Cc,Cf,NULL,prev_id);
   }

   // no deberia pasar por aqui
   ASSERT(FALSE);   
   return -1;
}

CDsStdDlg* CDsxView::ActivaDlg(int wid, int id, int sid,BOOL activar)
{
   CDsStdDlg *pdlg = LocalizaDlg(wid,id,sid,-1,-1);
   if (pdlg)
   {
       if (pdlg->m_id != id)
       {
           return pdlg;
       }
       if (activar)
       {
           if (!pdlg->m_padretab || pdlg->m_padretab->GetCurSel() == pdlg->m_opcionpadre)
           {
               pdlg->ShowWindow(SW_SHOW);               
               if (pdlg->m_sid_padre)
               {
                    ::SetWindowPos(pdlg->m_hWnd,(HWND)HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
               }
               ::SetWindowPos(pdlg->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);               
           }               
       }
       else
       {
           pdlg->ShowWindow(SW_HIDE);
       }
   }
   return pdlg;
}

CDsStdDlg* CDsxView::LocalizaDlg(int wid, int id, int sid,int lpi,int lpf,BOOL best)
{
   CDsStdDlg *pdlg = NULL;
   int fid = id;

   if (wid == -1 && id == -1 && sid == -1) // buscar la mas esterior que entre dentro de las coordenadas logicas
   {
       POSITION pos = m_dlglist.GetStartPosition( );
       CDsStdDlg *pdlgb;
       while(pos)
       {
           m_dlglist.GetNextAssoc( pos, wid, pdlgb );
           if (lpi == -1 || lpi >= pdlgb->m_lpi && lpf <= pdlgb->m_lpf)
           {
                if (!pdlg || pdlgb->m_nivel > pdlg->m_nivel)
				{
				   if (pdlgb->IsKindOf(RUNTIME_CLASS(CDs9Window)) && pdlgb != theApp.m_9wnd)
					   continue;
                   pdlg = pdlgb;                
				}
           }
       }
   }
   else
   {
       if (wid == -1)
       {
           POSITION pos = m_dlglist.GetStartPosition( );       
           CDsStdDlg *pdlgb = NULL;
		   CDsStdDlg *bestpdlgb = NULL;
           while(pos)
           {
               m_dlglist.GetNextAssoc( pos, wid, pdlgb );
			   if (id == -1 || best || (pdlgb->m_id+pdlgb->m_replicante) == id)
			   {
				   if (pdlgb->IsKindOf(RUNTIME_CLASS(CDs9Window)) && pdlgb != theApp.m_9wnd)
					   continue;
				   if (pdlgb->m_sid == sid)
				   {
					   if (!best || sid)
					   {
						    if (!pdlg || id == -1 || (pdlg->m_id <= id && (id-pdlgb->m_id) < (id-pdlg->m_id)))
							{
								pdlg = pdlgb;
								if (!best)
									break;
							}
					   }					   
					   else if ((pdlgb->m_id+pdlgb->m_replicante) <= fid)
					   {
						    fid = pdlgb->m_id+pdlgb->m_replicante;
							bestpdlgb = pdlgb;
					   }					   
				   }
				   else if (!sid && !bestpdlgb)
				   {
					   bestpdlgb = pdlgb;
				   }
			   }
			   /*
               if (pdlgb->m_id == id && pdlgb->m_sid == sid || (!sid && pdlgb->m_replicante))
               {
				   if (pdlgb->IsKindOf(RUNTIME_CLASS(CDs9Window)) && pdlgb != theApp.m_9wnd)
					   continue;
                   pdlg = pdlgb;
                   break; // de momento solo una pantalla igual ...
               } 
			   */
           }
		   if (!pdlg && bestpdlgb)
		   {
			   pdlg = bestpdlgb;
		   }
       }
       else
       {
            m_dlglist.Lookup(wid,pdlg);
       }
   }
   /*ASSERT(pdlg);*/
   /*
   if (pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window)))
   {
	   ASSERT(FALSE);
   }
   */
   return (pdlg);
}

int CDsxView::ControlEdicion(int wid, int id, int sid,int cwid , Cid cid, int flag ,void *datos,int pre_id)
{
    CDsStdDlg* pdlg;

    if (cwid == -1)
        pdlg = LocalizaDlg(wid,id,sid,-1,-1);
    else
    {
        CDsStdCtrl *pctrl = NULL;
        if (CDsStdDlg::m_todosctrl.Lookup(cwid,pctrl))
        {
            int *p1 = (int *)datos;            
            if ((id != -1 && pctrl->m_pdlg->m_id != id) || (sid != -1 && pctrl->m_pdlg->m_sid != sid)  || (p1[0] != 11 && (pctrl->m_tipo != 2 || !CDsStdDlg::ComparaCid(pctrl->m_cid,cid))))
            {
                //ASSERT(FALSE);
                pctrl = NULL;
            }
            else            
            {
                pdlg = pctrl->m_pdlg;
            }
        }
        if (!pctrl)
        {
            pdlg = LocalizaDlg(wid,id,sid,-1,-1); // ultimo intento
        }
        else
        {
            return pctrl->ControlEdicion(flag,datos,pre_id);
        }
    }

    if (pdlg)
    {
        return pdlg->ControlEdicion(cwid,cid,flag,datos,pre_id);
    }

    return -1;
}

BOOL CDsxView::TeclaStd(int tecla,int wid,int id,int sid,int wcid,int ctipo,Cid cid)
{
    return ((CDsxDoc *)GetDocument())->TeclaStd(tecla,wid,id,sid,wcid,ctipo,cid);
}

BOOL CDsxView::EnsureVisible(CRect *pwrect)
{
    CRect r = *pwrect,r2;
    ScreenToClient(r);
    GetClientRect(r2);

    //CExtScrollBar
    int scrollsize = 20;

    CPoint pt = GetScrollPosition();

    if (r.left > (r2.right-scrollsize))
    {
        pt.x += (r.left - (r2.right-scrollsize));
    }
    if (r.left < r2.left)
    {
        pt.x -= (r2.left - r.left);
    }

    if (r.top > (r2.bottom-scrollsize))
    {
        pt.y += (r.top - (r2.bottom-scrollsize));
    }
    if (r.top < r2.top)
    {
        pt.y -= (r2.top - r.top);
    }
    
    ScrollToPosition( pt );

    return TRUE;
}

int CDsxView::ErrorFichero(char *data1, char *data2, char *data3, int ok)
{
   int r = 0;
   FinErrorFichero();
   m_errorf = new CDlgErrorFichero();

   m_errorf->m_edit1 = data1;
   m_errorf->m_edit2 = data2;
   m_errorf->m_edit3 = data3;
   m_errorf->Create(IDD_ERRORFICHERO,this);
   CRect rr,rr1;
   GetWindowRect(rr);
   m_errorf->GetWindowRect(rr1);  
   rr1.OffsetRect((rr.left+rr.Size().cx/2-rr1.Size().cx/2)-rr1.left,(rr.top+rr.Size().cy/2-rr1.Size().cy/2)-rr1.top);
   m_errorf->MoveWindow(rr1);
   m_errorf->BringWindowToTop();
   m_errorf->ShowWindow(SW_SHOW);
   CWnd *pwnd = m_errorf->GetDlgItem(IDOK);
   if (ok)
   {       
       if (pwnd)
       {
           pwnd->EnableWindow(TRUE);
           pwnd->SetFocus();
       }
       r = m_errorf->RunModalLoop( 0 );
       FinErrorFichero();
   }   
   else
   {
       if (pwnd)
       {
           pwnd->EnableWindow(FALSE);
	   }
   }
   return r;
}

BOOL CDsxView::FinErrorFichero()
{
   if (m_errorf)
   {
       if (::IsWindow(m_errorf->m_hWnd))
       {
		   m_errorf->EndDialog(IDOK);		   
           m_errorf->DestroyWindow();
       }
       delete m_errorf;
       m_errorf = NULL;
       return TRUE;
   }
   return FALSE;
}


BOOL CDsxView::ActivaHtml()
{

     if (!theApp.m_usarweb || m_html) return FALSE;

     CRect rect(0,0,10,10);

     if ( (m_html = new CDsHtml()) )
     {        
        if (!m_html->Create(NULL, NULL, WS_CHILD | WS_CLIPSIBLINGS  ,rect,this, IDR_HTMLVIEW, NULL))
        {
            delete m_html;
            m_html = NULL;
        }
        else
        {
            m_html->OnInitialUpdate();
        }
     }
     if (m_html)
     {
         GetClientRect(rect);
         m_html->MoveWindow(rect);
         return TRUE;
     }
     return FALSE;
}

//DEL void CDsxView::OnExportaExcel() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

//DEL void CDsxView::OnExportaWord() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }
