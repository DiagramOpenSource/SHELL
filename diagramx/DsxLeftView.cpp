// DsxLeftView.cpp : implementation of the CDsxLeftView class
//

#include "stdafx.h"
#include "diagramx.h"

#include "DsxMainFrm.h"
#include "DsxDoc.h"
#include "DsxView.h"
#include "DsxLeftView.h"
#include "DsStdDlg.h"
#include "Ds9Window.h"

#include "SkinScrollWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsxLeftView

IMPLEMENT_DYNCREATE(CDsxLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CDsxLeftView, CTreeView)
	//{{AFX_MSG_MAP(CDsxLeftView)
ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)	
ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsxLeftView construction/destruction

CDsxLeftView::CDsxLeftView()
{   
   if (!theApp.m_LeftView)
      theApp.m_LeftView = this;
   else
	  theApp.m_UserMenu = this;

}

CDsxLeftView::~CDsxLeftView()
{
   if (theApp.m_LeftView == this)
      theApp.m_LeftView = NULL;
   else
	  theApp.m_UserMenu = NULL;
}

BOOL CDsxLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
  //cs.style |= WS_VISIBLE;  
  if (theApp.m_UserMenu == this)
  {
	 cs.style |= TVS_EDITLABELS;
  }
  return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxLeftView drawing

void CDsxLeftView::OnDraw(CDC* pDC)
{
	CTreeView::OnDraw(pDC);
	/*
   CDsxDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   */      
}

void CDsxLeftView::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClip, rcClient;	
	dc.GetClipBox( &rcClip );	
	GetClientRect(&rcClient);

	// Create a compatible memory DC 
	CDC memDC;
	memDC.CreateCompatibleDC( &dc );	

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );

	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );	

	if (!rcClip.IsRectEmpty())
	{
		CDC fondoDC;
		CBitmap bitmapfondo;

		fondoDC.CreateCompatibleDC( &dc );
		bitmapfondo.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
		fondoDC.SelectObject( &bitmapfondo );

		if (theApp.m_aparienciabase == 3)
		{
			if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
				||	(! ((CExtPaintManagerSkin *)pMFrame->m_paintmanager)->PaintDockerBkgnd( true, fondoDC , this))
				)
				fondoDC.FillSolidRect(
					&rcClip,
					GetTreeCtrl().GetBkColor()
					);	
		}
		else if (theApp.m_aparienciabase == 2)
		{
			if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
				||	(! ((CDsProfuisCustomOffice2007_R1 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, fondoDC , this , GetTreeCtrl().GetBkColor()))		
				)
				fondoDC.FillSolidRect(
					&rcClip,
					GetTreeCtrl().GetBkColor()
					);	
		}
		else if (theApp.m_aparienciabase == 1)
		{
			if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
				||	(! ((CDsProfuisCustomOffice2003 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, fondoDC , this , GetTreeCtrl().GetBkColor()))		
				)
				fondoDC.FillSolidRect(
					&rcClip,
					GetTreeCtrl().GetBkColor()
					);	
		}
		
		CExtPaintManager::stat_TransparentBlt(fondoDC.m_hDC,
											rcClip.left, rcClip.top, -1, -1,
											memDC.m_hDC,
											rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
											GetTreeCtrl().GetBkColor()
											);
		dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(),
				rcClip.Height(), &fondoDC,
				rcClip.left, rcClip.top, SRCCOPY );
	}
	
}



BOOL CDsxLeftView::OnEraseBkgnd(CDC* pDC) 
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
			||	(! ((CExtPaintManagerSkin *)pMFrame->m_paintmanager)->PaintDockerBkgnd( true, *pDC, this ))
			)

			pDC->FillSolidRect(
				&rect,
				colorfondo
				);
	}
    else if (theApp.m_aparienciabase == 2)
    {
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(! ((CDsProfuisCustomOffice2007_R1 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, *pDC, this , colorfondo))
			)

			pDC->FillSolidRect(
				&rect,
				colorfondo
				);
	}
	else if (theApp.m_aparienciabase == 1)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(! ((CDsProfuisCustomOffice2003 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, *pDC, this , colorfondo))		
			)
			pDC->FillSolidRect(
				&rect,
				colorfondo
				);
	}
	return TRUE;
#else	
	return FALSE;
#endif
}


void CDsxLeftView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
   if (pMFrame)
   {
       ((CDsxView *)pMFrame->GetActiveView())->OnPrint(pDC,pInfo);
       return;
   }
   CTreeView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CDsxLeftView printing

BOOL CDsxLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
   if (pMFrame)
   {
       return ((CDsxView *)pMFrame->GetActiveView())->OnPreparePrinting(pInfo);
   }
   return DoPreparePrinting(pInfo);
}

void CDsxLeftView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   if (pMFrame)
   {
       ((CDsxView *)pMFrame->GetActiveView())->OnBeginPrinting(pDC,pInfo);
       return;
   }
   CTreeView::OnBeginPrinting(pDC,pInfo);
}

void CDsxLeftView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   if (pMFrame)
   {
       ((CDsxView *)pMFrame->GetActiveView())->OnEndPrinting(pDC,pInfo);
       return;
   }
   CTreeView::OnEndPrinting(pDC,pInfo);
}

void CDsxLeftView::RellenaMenuPropio()
{
	   CTreeCtrl *tree = &GetTreeCtrl();
	   HTREEITEM pariente = NULL;
	   HTREEITEM primero = tree->InsertItem(TVIF_IMAGE | TVIF_TEXT,
		  _T("Menú Propio"),
		  0,
		  0,
		  0,
		  0,
		  0,
		  NULL,
		  TVI_FIRST);

	   CString funcion,s,mif;
	   s = theApp.GetProfileString("MenuPropio","Indice","");
	   int pp,i,aa;
	   TV_INSERTSTRUCT Nuevo;		
	   HTREEITEM nueva = NULL;
	   while(!s.IsEmpty())
	   {
		   funcion = s.Mid(0,5);
		   if (funcion.GetLength() < 5 || funcion[0] != 'M')
			   break;
		   s.Replace(funcion,"");
		   mif = theApp.GetProfileString("MenuPropio",funcion,"");
		   if (!mif.IsEmpty())
		   {
			   pp = mif.Find(10);
			   if (pp > -1)
			   {
				    int j;
					CString s1;
					s1 = mif.Mid(pp+1);
					j = s1.Find(10);
					if (j > -1)
					{
						mif.SetAt(pp+1+j,0);
						j = atoi((const char *)s1+j+1);
						pariente = BuscaItemData(primero,-(j+1));
						if (!pariente)
						{
							pariente = primero;
						}
					}
					else
						pariente = primero;

				    i = atoi(((const char *)funcion) + 1);
					Nuevo.hParent = pariente;
					Nuevo.hInsertAfter = TVI_LAST;					
					Nuevo.item.pszText = (char *)((const char *)mif) + 1 + pp;
					Nuevo.item.cchTextMax = strlen(((const char *)mif) + 1 + pp);

					if (!pp)
					{
						Nuevo.item.iImage = 0;
						Nuevo.item.iSelectedImage = 0;
					}
					else
					{
						Nuevo.item.iImage = 2;
						Nuevo.item.iSelectedImage = 2;					
					}

					Nuevo.item.lParam = -(i+1);
					Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					nueva = tree->InsertItem( &Nuevo );	

					if (i >= theApp.m_nlistadirecta)
					{	
						aa = theApp.m_nlistadirecta;
						theApp.m_nlistadirecta = i+1;
						if (!theApp.m_listadirecta)
						{							
							theApp.m_listadirecta = (char **)malloc(sizeof(char *)*theApp.m_nlistadirecta);
						}
						else
						{							
							theApp.m_listadirecta = (char **)realloc((void *)theApp.m_listadirecta,sizeof(char *)*theApp.m_nlistadirecta);
						}
						for (;aa < theApp.m_nlistadirecta;aa++)
						{
							theApp.m_listadirecta[aa] = NULL;
						}
					}
					mif.SetAt(pp,0);
					if (theApp.m_listadirecta[i])
						free((void *)theApp.m_listadirecta[i]);
					theApp.m_listadirecta[i] = (char *)malloc(strlen(mif)+1);
					strcpy(theApp.m_listadirecta[i],mif);
			   }
		   }
	   }
	   if (nueva)
	   {
		   tree->ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT);       
		   tree->Expand(primero, TVE_EXPAND    );
		   tree->SelectItem(nueva);
	   }
}

void CDsxLeftView::OnInitialUpdate()
{
   //g_CmdManager->ProfileWndAdd("diagramx",m_hWnd);

   CTreeView::OnInitialUpdate();

   if (!m_pDocument)
   {
	   if (theApp.m_LeftView)
	   {
		  theApp.m_LeftView->GetDocument()->AddView( (CView*)this );
		  //m_pDocument = theApp.m_LeftView->m_pDocument;
		   if (!m_pDocument)
		   {
			   return;
		   }
	   }
   }


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
   GetTreeCtrl().SetBkColor( colorfondo );  
   //GetTreeCtrl().SetTextColor( GetDocument()->m_cGridFr );   
#else
   GetTreeCtrl().SetBkColor( GetDocument()->m_cGridBk );  
   GetTreeCtrl().SetTextColor( GetDocument()->m_cGridFr );   
#endif   

   
   m_ImageList.Create(16, 15, TRUE, 3, 2);
   
   CBitmap bitmap;
   
   bitmap.LoadBitmap(IDB_TREEFOL);
   m_ImageList.Add(&bitmap, (COLORREF)0xFFFFFF);
   bitmap.DeleteObject();
   
   bitmap.LoadBitmap(IDB_TREEOPEN);
   m_ImageList.Add(&bitmap, (COLORREF)0xFFFFFF);
   bitmap.DeleteObject();
   
   bitmap.LoadBitmap(IDB_TREEFIL);
   m_ImageList.Add(&bitmap, (COLORREF)0xFFFFFF);
   bitmap.DeleteObject();
   
   bitmap.LoadBitmap(IDB_TREESEL);
   m_ImageList.Add(&bitmap, (COLORREF)0xFFFFFF);
   bitmap.DeleteObject();
   
   
   CTreeCtrl *tree = &GetTreeCtrl();

   //SkinWndScroll(tree);

   tree->SetImageList(&m_ImageList, TVSIL_NORMAL);
   
   tree->DeleteAllItems();    
   
   if (theApp.m_UserMenu == this)
   {
	   RellenaMenuPropio();
   }
   else
   {
	   tree->InsertItem(TVIF_IMAGE | TVIF_TEXT,
		  _T("Sin Opciones"),
		  0,
		  0,
		  0,
		  0,
		  0,
		  NULL,
		  TVI_FIRST);
   }
}

/////////////////////////////////////////////////////////////////////////////
// CDsxLeftView diagnostics

#ifdef _DEBUG
void CDsxLeftView::AssertValid() const
{
   CTreeView::AssertValid();
}

void CDsxLeftView::Dump(CDumpContext& dc) const
{
   CTreeView::Dump(dc);
}

CDsxDoc* CDsxLeftView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDsxDoc)));
   return (CDsxDoc*)m_pDocument;
}
#endif //_DEBUG




void CDsxLeftView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

   //!pNMTreeView->itemNew.lParam
   if (GetTreeCtrl().ItemHasChildren( pNMTreeView->itemNew.hItem ))
   {
      TV_ITEM tv;
	     if (pNMTreeView->itemNew.state & TVIS_EXPANDED)
	       {
           tv.hItem = pNMTreeView->itemNew.hItem;
           tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
           tv.iImage = 1;
           tv.iSelectedImage = 1;
           GetTreeCtrl().SetItem(&tv);
        }
        else
           /*if (pNMTreeView->hdr.code == TVE_COLLAPSE)*/
	       {
           PintaColapso(pNMTreeView->itemNew.hItem);
        }
		 }	   
   *pResult = 0;
}



void CDsxLeftView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
   
   EjecutaSeleccion();
   *pResult = 0;
}


int CDsxLeftView::GetCorriente()
{
  TV_ITEM tv;
  tv.hItem = GetTreeCtrl().GetSelectedItem( );
  tv.mask = TVIF_HANDLE | TVIF_PARAM;	
  if (GetTreeCtrl().GetItem(&tv))
  {       
     LPARAM punto = tv.lParam;	 
	 if (punto > 0)
     {
		return MID_XPRIMERA+(punto-1);        
     }
  }
  return 0;
}

BOOL CDsxLeftView::EjecutaSeleccion()
{
  TV_ITEM tv;
  tv.hItem = GetTreeCtrl().GetSelectedItem( );
  tv.mask = TVIF_HANDLE | TVIF_PARAM;	
  if (GetTreeCtrl().GetItem(&tv))
  {       
     LPARAM punto = tv.lParam;	 
	 if (punto && pMFrame)
     {        
        ((CDsxDoc *)pMFrame->GetActiveDocument())->OpcionDeMenu((int)punto);
        return TRUE;
     }
  }
  
  return FALSE;
}

void CDsxLeftView::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{  
   EjecutaSeleccion();
   HTREEITEM item = GetTreeCtrl().GetSelectedItem( );
   if (GetTreeCtrl().ItemHasChildren( item ))
   {
      GetTreeCtrl().Expand(item , TVE_EXPAND );
      TV_ITEM tv;
      UINT state = GetTreeCtrl().GetItemState(item,TVIS_EXPANDED);
      if (!(state & TVIS_EXPANDED))
      {		  
         tv.hItem = item;
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
         tv.iImage = 0;
         tv.iSelectedImage = 0;
         GetTreeCtrl().SetItem(&tv);
      }
      else
      {
         tv.hItem = item;
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
         tv.iImage = 1;
         tv.iSelectedImage = 1;
         GetTreeCtrl().SetItem(&tv);
      }
   }
   *pResult = 1;
}

void CDsxLeftView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NMTVKEYDOWN* pTVKeyDown = (NMTVKEYDOWN*)pNMHDR;  

   
   if (pTVKeyDown->wVKey == VK_SPACE && GetTreeCtrl().ItemHasChildren( GetTreeCtrl().GetSelectedItem( ) ) /*!GetTreeCtrl().GetItemData(GetTreeCtrl().GetSelectedItem( ))*/)
   {
      GetTreeCtrl().Expand( GetTreeCtrl().GetSelectedItem( ), TVE_TOGGLE );       
      TV_ITEM tv;
      UINT state = GetTreeCtrl().GetItemState(GetTreeCtrl().GetSelectedItem( ),TVIS_EXPANDED);
      if (!(state & TVIS_EXPANDED))
      {		  
         tv.hItem = GetTreeCtrl().GetSelectedItem( );
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
         tv.iImage = 0;
         tv.iSelectedImage = 0;
         GetTreeCtrl().SetItem(&tv);
      }
      else
      {
         tv.hItem = GetTreeCtrl().GetSelectedItem( );
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
         tv.iImage = 1;
         tv.iSelectedImage = 1;
         GetTreeCtrl().SetItem(&tv);
      }
      *pResult = 1;
      return;
   }
   

   int comando = CDsStdDlg::DeterminaTecla(pTVKeyDown->wVKey);
   if (comando)
   {
      if (pMFrame)
      {  
		  if (comando != 802 || !((CDsxDoc *)pMFrame->GetActiveDocument())->StatusEnWinsysMenu())
		  {
			 ((CDsxDoc *)pMFrame->GetActiveDocument())->TeclaStd(comando);
			  *pResult = 1;
			  return;
		  }      
	  }
   }

   *pResult = 0;
}

void CDsxLeftView::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{  
   if (!pMFrame) return;

   if (!((CDsxDoc *)pMFrame->GetActiveDocument())->StatusEnWinsysMenu())
   {
      int cxCur,cxMin;
      CDsxFrame *mw = (CDsxFrame *)theApp.m_pMainWnd;
      mw->m_wndSplitter->GetColumnInfo( 1, cxCur, cxMin );
      if (!cxCur)
      {            
		    mw->m_wndSplitter->SetColumnInfo( 0, 0, cxMin );
            mw->m_wndSplitter->SetColumnInfo( 1, 10000, cxMin );
            mw->m_wndSplitter->RecalcLayout();
      }

      if (theApp.m_antctrlactivo && theApp.m_antctrlactivo->m_ctrl)
      {
		  theApp.m_antctrlactivo->SetFocus();		  
      }
      else
      {
		  if (theApp.m_9wnd)
		  {
			  theApp.m_9wnd->SetFocus();
		  }
		  else
		  {
			((CDsxDoc *)pMFrame->GetActiveDocument())->PonFoco();
		  }
      }

      *pResult = 1;
   }
   else
   {
    *pResult = 0;
     int cxCur,cxMin;     
     pMFrame->m_wndSplitter->GetColumnInfo( 0, cxCur, cxMin );
     if (!cxCur)
     {
        //int cxCur2;        
        //pMFrame->m_wndSplitter->GetColumnInfo( 1, cxCur2, cxMin );            
        pMFrame->m_wndSplitter->SetColumnInfo( 0, 10000, cxMin );
        pMFrame->m_wndSplitter->RecalcLayout();
     }      
   }
}


BOOL CDsxLeftView::PreTranslateMessage(MSG* pMsg) 
{	
   
   if (pMsg->hwnd == m_hWnd)
   {
     if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL || (pMsg->wParam == 'Q' && (::GetKeyState( VK_CONTROL ) & 0x8000) )))
     {
       if (pMFrame)
       {        
          if (pMsg->wParam == VK_ESCAPE)
             pActiveDoc->TeclaStd(806);
          else
             pActiveDoc->TeclaStd(807);
       }      
     }       
   }	
   return CTreeView::PreTranslateMessage(pMsg);
}

void CDsxLeftView::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{	  
   *pResult = 0;
}

void CDsxLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
   // TODO: Add your control notification handler code here
   GetTreeCtrl().EnsureVisible( pNMTreeView->itemNew.hItem );
   
   if (pMFrame && pActiveDoc->StatusEnWinsysMenu() && pNMTreeView->itemNew.lParam)
   {
       short punto = MID_XPRIMERA+(unsigned short)(pNMTreeView->itemNew.lParam - 1);
       pActiveDoc->TeclaStd(-punto);
   }

   *pResult = 0;
}


void CDsxLeftView::PintaColapso(HTREEITEM hitem)
{
   TV_ITEM tv;
   tv.hItem = hitem;
   tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
   tv.iImage = 0;
   tv.iSelectedImage = 0;
   GetTreeCtrl().SetItem(&tv);
}

BOOL CDsxLeftView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (pMFrame && (wParam&0xffff) == ID_FOCUS_TREE || (wParam&0xffff) == ID_UNFOCUS_TREE)
    {
		if (IsWindowEnabled() && !pMFrame->m_sinbarras)
		{
			 if (pActiveDoc->StatusEnWinsysMenu())
			 {
				 SetFocus();

				 int cxCur,cxMin;             

				 pMFrame->m_wndSplitter->GetColumnInfo( 0, cxCur, cxMin );
				 if (!cxCur)
				 {            
					//int cxCur2;
					//pMFrame->m_wndSplitter->GetColumnInfo( 1, cxCur2, cxMin );
					pMFrame->m_wndSplitter->SetColumnInfo( 0, 10000, cxMin );
					pMFrame->m_wndSplitter->RecalcLayout();
				 }
				 else
				 {
					//pMFrame->m_wndSplitter->GetColumnInfo( 1, cxCur, cxMin );
				 }
			 }
			 else
			 {
				 int cxCur,cxMin;
             
				 pMFrame->m_wndSplitter->GetColumnInfo( 1, cxCur, cxMin );             
				 if (!cxCur)
				 {                
					pMFrame->m_wndSplitter->SetColumnInfo( 0, 0, cxMin );
					pMFrame->m_wndSplitter->SetColumnInfo( 1, 10000, cxMin );
					pMFrame->m_wndSplitter->RecalcLayout();
				 }
				 else
				 {
					//pMFrame->m_wndSplitter->GetColumnInfo( 1, cxCur, cxMin );
				 }
			 }
		}
        return TRUE;
    }
    if ((wParam&0xffff) >= 800 && (wParam&0xffff) <= 999)
    {
      if (pActiveDoc)
      {        
         pActiveDoc->TeclaStd((wParam&0xffff));
         return TRUE;
      }       
    }
    return CTreeView::OnCommand(wParam, lParam);
}


static CString CreateShortcut(CString sData, CString sDataFile, CString sArguments)
{
	HRESULT hres;
	IShellLink* pIShellLink;


	sData.Replace('<','-');
	sData.Replace('>','-');
	sData.Replace('\\','-');
	sData.Replace('/','-');
	sData.Replace(':','-');
	sData.Replace('*','-');
	sData.Replace('?','-');
	sData.Replace('"','-');

	char szTempPath[MAX_PATH];
	GetTempPath(MAX_PATH, szTempPath);
	
	CString sLinkFile;
	sLinkFile.Format("%s%s.lnk", szTempPath, sData);

	unlink(sLinkFile);

	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
	IID_IShellLink, (LPVOID *)&pIShellLink);

	if (SUCCEEDED(hres))
	{
		IPersistFile *pIPersistFile;

		hres = pIShellLink->SetPath(sDataFile);

		pIShellLink->SetIconLocation(sDataFile,5);

		//if (!SUCCEEDED(hres))
		//	AfxMessageBox("SetPath failed!");

        pIShellLink->SetArguments(  sArguments );

		/*pIShellLink->SetHotkey(MAKEWORD(VK_F8,HOTKEYF_ALT | HOTKEYF_CONTROL));*/

		hres = pIShellLink->QueryInterface(IID_IPersistFile, 
		(LPVOID *)&pIPersistFile);

		if (SUCCEEDED(hres))
		{
			USES_CONVERSION;
			hres = pIPersistFile->Save(T2OLE(sLinkFile), TRUE);
			//if (!SUCCEEDED(hres))
			//	AfxMessageBox("Save failed!");

			pIPersistFile->Release();
		}

		pIShellLink->Release();
	}

	return sLinkFile;
}

void CDsxLeftView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	  *pResult = 0;

	  char exe[1024];
	  char par[1024];
	  char tmpt[1024];
	  CString titulo;
	  TVITEM item;

	  tmpt[0] = 0;
	  memset((void *)&item,0,sizeof(item));
	  item.mask = TVIF_TEXT | TVIF_HANDLE;
	  item.hItem = pNMTreeView->itemNew.hItem;
	  item.pszText   = tmpt;
	  item.cchTextMax= sizeof(tmpt)-1;
	  GetTreeCtrl().GetItem( &item );
	  if (GetTreeCtrl().GetRootItem() == item.hItem)
	  {
		  return;
	  }
	  
	  {       
		 LPARAM punto = pNMTreeView->itemNew.lParam;
		 int directa = (int)punto;
		 if (directa  < 0)
		 {
			 directa = -(directa+1);
			 if (directa < theApp.m_nlistadirecta && pDsSys && theApp.m_listadirecta[directa])
			 {
				 if (!theApp.m_listadirecta[directa][0])
					 return;                 

				 if (theApp.m_9wnd || theApp.m_simul9)
				 {
					 pDsSys->PreparaEjecucion9((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)theApp.m_password,(int)0);
				 }
				 else
				 {
					 pDsSys->PreparaEjecucion((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)theApp.m_password,(int)0);
				 }
				 char *pp = exe;
				 pp += (strlen(exe)+1);
				 if (*pp == 7 && *(pp+1) == 7)
				 {
					 pp+=2;
					 strcpy(pp,theApp.m_listadirecta[directa]+1);
				 }
				 else
				 {
					return;
				 }
				 memcpy(par,"/DS:\"",5);
				 int i;
				 for (i = 0;par[i];i++)
				 {
					 if (par[i] == 3)
					 {
						 par[i] = 0;
						 titulo = ""; /*par+i+1;*/
						 break;
					 }
				 }
				 strcat(par,"\"");
			 }
		 } else if (pMFrame)
		 {  
			 if (theApp.m_9wnd || theApp.m_simul9)
			 {
				 pDsSys->PreparaEjecucion9((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)theApp.m_password,(int)punto);
			 }
			 else
			 {
				 pDsSys->PreparaEjecucion((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)theApp.m_password,(int)punto);
			 }

			 memcpy(par,"/DS:\"",5);
			 int i;
			 for (i = 0;par[i];i++)
			 {
				 if (par[i] == 3)
				 {
					 par[i] = 0;
					 titulo = par+i+1;
					 break;
				 }
			 }
			 strcat(par,"\"");
		 }
		 else
			 return;

		 if (punto > 0)
		 {  /* Lo saco del menu standard .. no del tree ... 
  			 CMenu *main = pMFrame->GetMenu();
			 if (main)
			 {		  
				  BOOL BuscaComando_Menu(CMenu *menu,UINT comando,CString &nombre);

				  CMenu *popup = main->GetSubMenu( 5 );		  
				  if (popup)
				  {
					 UINT comando;
					 CString s;
					 comando = punto+MID_XPRIMERA-1;
					 BuscaComando_Menu(popup,comando,s);
					 s += " ";
					 titulo = theApp.m_nomstdmenu + " " + s + titulo;
				  }
			 }   
			 */
			 titulo = theApp.m_nomstdmenu + " " + tmpt + " " + titulo;
			 
		 }
		 else if (punto < 0)
		 {
			 //titulo = theApp.m_nomstdmenu + " " + tmpt + " " + titulo;
			 titulo = tmpt;
		 }
		 else
		 {
			 titulo = theApp.m_nomstdmenu + " " + titulo;
		 }
	  }


	HGLOBAL handle = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE ,1024);
	char *lpClipData = (char *)::GlobalLock(handle);
	*lpClipData++ = 1;
	*lpClipData++ = 0;
	*lpClipData++ = 0;
	*lpClipData++ = 1;

	strcpy(lpClipData,exe);
	lpClipData += (strlen(exe)+1);
	strcpy(lpClipData,par);
	lpClipData += (strlen(par)+1);
	strcpy(lpClipData,titulo);
	lpClipData += (strlen(titulo)+1);
	*lpClipData = 0;
	char *pp = exe;
	pp += (strlen(exe)+1);
	if (*pp == 7 && *(pp+1) == 7)
	{
		pp+=2;
		strcpy(lpClipData,pp);
		lpClipData+=(strlen(pp)+1);

		
		{
			int i;
			for (i = 0;par[i];i++)
			{
				if (par[i] == '$' && par[i+1] == '$' && par[i+2] == '$')
				{
					i += 3;
					for(;par[i];i++)
					{
						if (par[i] == '#')
						{
							i++;						
							strcpy(par+i," //");
							strcat(par+i,pp);
							for (i++;par[i];i++)
							{
								if (par[i] == ' ')
									par[i] = '&';
							}
							strcat(par,"\"");
							break;
						}
					}
					break;
				}
			}
		}

	}
	else
	{
		*lpClipData++ = 0;
	}
	::GlobalUnlock(handle);

	

	  CString destino = CreateShortcut(titulo,exe,par);

  	  COleDataSource *poleSourceObj = new COleDataSource;	

	  //poleSourceObj->CacheGlobalData( cTreeFormat, handle );

      CSharedFile sf( GMEM_MOVEABLE|GMEM_DDESHARE|
                      GMEM_ZEROINIT);
      CString iText; 
                   
      DROPFILES a; 
       //CF_HDROP clipboard format...
      ::ZeroMemory(&a,sizeof(a));
      a.pFiles = sizeof(DROPFILES);

      a.fWide = FALSE;
      sf.Write(&a, sizeof(a));
      CHAR c = '\0';
	  /*
      iText = "c:\\test";

      sf.Write(iText, iText.GetLength());
	  */
      iText = destino;
      sf.Write(iText, iText.GetLength());
      sf.Write(&c,sizeof(CHAR));

      HGLOBAL hMem = sf.Detach();
      if (!hMem) 
	  {
		  unlink(destino);
          return;
	  }


	poleSourceObj->CacheGlobalData(CF_HDROP, hMem);


	poleSourceObj->CacheGlobalData(theApp.m_cMenuFormat, handle);	


	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop(DROPEFFECT_MOVE);

	delete poleSourceObj;

	unlink(destino); // Por si el destino no mueve

#ifdef NADA_BORRARSIELDRAGESANADA
	if (dropeffect == DROPEFFECT_NONE || theApp.m_UserMenu == this)
	{
		int i;

		i = pNMTreeView->itemNew.lParam;
		if (!i)
		{
		   char tmpx[1024];
		   TVITEM item;		   
		   item.hItem = pNMTreeView->itemNew.hItem;
		   item.mask = TVIF_TEXT | TVIF_HANDLE;
		   item.pszText = tmpx;
		   item.cchTextMax = sizeof(tmpx)-1;
		   GetTreeCtrl().GetItem(&item);
		   if (!strcmp(tmpx,"Busqueda"))
		   {
			   BorraBusqueda();
		   }
		   else
		   {
			   BorraMenuPropio();
		   }
		}		
		else
		{
		   if (GetTreeCtrl().ItemHasChildren(pNMTreeView->itemNew.hItem))
		   {
				BorraSubOpciones(pNMTreeView->itemNew.hItem);
		   }
			if (i < 0)
			{
				i = -(i+1);
				CString funcion,s;
				funcion.Format("M%04d",i);
				theApp.WriteProfileString("MenuPropio",funcion,"");
				s = theApp.GetProfileString("MenuPropio","Indice","");
				s.Replace(funcion,"");
				theApp.WriteProfileString("MenuPropio","Indice",s);
			}
			theApp.BorraDeLaListaDirecta(pNMTreeView->itemNew.lParam);
			GetTreeCtrl().DeleteItem(pNMTreeView->itemNew.hItem);
		}
	}
#endif
	
	*pResult = 0;
}

static int cuantos_rebotes = 0;
void CDsxLeftView::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeView::OnSetFocus(pOldWnd);

	if (theApp.m_Embebido && theApp.m_dlghwnd && ::IsWindow(theApp.m_dlghwnd))
	{
		if (cuantos_rebotes > 10)
		{
			cuantos_rebotes = 0;
			theApp.m_dlghwnd = NULL;
			return;
		}
		::SetFocus(theApp.m_dlghwnd);
		cuantos_rebotes++;
	}
	
	/*
	if (pMFrame && !((CDsxDoc *)pMFrame->GetActiveDocument())->StatusEnWinsysMenu())
	{
		if ((CDsStdDlg *)theApp.m_9wnd && ::IsWindow(((CDsStdDlg *)theApp.m_9wnd)->m_hWnd))
		{
			if (pOldWnd && pOldWnd->m_hWnd != ((CDsStdDlg *)theApp.m_9wnd)->m_hWnd)
			{
				::SetFocus(((CDsStdDlg *)theApp.m_9wnd)->m_hWnd);
			}
		}
	}
	*/
}


void CDsxLeftView::BuscaYRellenaMenuUser(LPCSTR patern)
{
	if (theApp.m_LeftView != this) return;
	
	CString s1,s = patern;
	s.MakeLower();
	if (!theApp.m_UserMenu)
	{
		CDsxFrame *mw = (CDsxFrame *)theApp.m_pMainWnd;
		if (mw)
		{
			mw->SendMessage(WM_COMMAND,ID_MENUSER);
		}
	}
	if (theApp.m_UserMenu)
	{		
		BorraBusqueda();

		CTreeCtrl *pNew = &(theApp.m_UserMenu->GetTreeCtrl());
		HTREEITEM primero;
	    primero = pNew->InsertItem(TVIF_IMAGE | TVIF_TEXT,
		  _T("Busqueda"),
		  0,
		  0,
		  0,
		  0,
		  0,
		  NULL,
		  TVI_FIRST);		

		{
            char *p;
            long tam;
            long respuesta;
            long modo = 1;
            int operacion;
			int localizador = 0;
			int memoria;
            char tmp[512];
			char *p0;
            p = tmp;

			operacion = 0;

            memcpy((void *)p,(void *)&localizador,sizeof(int));
            p += sizeof(int);

            tam = (sizeof(long) * 4) + 300;
            respuesta = tam;

            memcpy((void *)p,(void *)&tam,sizeof(long));
            p += sizeof(long);	
            memcpy((void *)p,(void *)&respuesta,sizeof(long));
            p += sizeof(long);
            memcpy((void *)p,(void *)&modo,sizeof(long));
            p += sizeof(long);
            memcpy((void *)p,(void *)&operacion,sizeof(int));
            p += sizeof(int);

			strcpy(p,(LPCSTR)s);
            
			if (pDsSys->m_f9DespachadorInterno)
				pDsSys->DespachadorInterno9(13,(void *)tmp);
			else if (pDsSys->m_fDespachadorInterno)
			{
				pDsSys->DespachadorInterno(13,(void *)tmp);
			}

			memcpy(&memoria,p,sizeof(int));

			if (memoria > 0)
			{
				p0 = (char *)malloc(512+memoria);
				p = p0;

				operacion = 1;

				memcpy((void *)p,(void *)&localizador,sizeof(int));
				p += sizeof(int);

				tam = (sizeof(long) * 4) + 100 + memoria;
				respuesta = tam;

				memcpy((void *)p,(void *)&tam,sizeof(long));
				p += sizeof(long);	
				memcpy((void *)p,(void *)&respuesta,sizeof(long));
				p += sizeof(long);
				memcpy((void *)p,(void *)&modo,sizeof(long));
				p += sizeof(long);
				memcpy((void *)p,(void *)&operacion,sizeof(int));
				p += sizeof(int);
				*p = 0;

				if (pDsSys->m_f9DespachadorInterno)
					pDsSys->DespachadorInterno9(13,(void *)p0);
				else if (pDsSys->m_fDespachadorInterno)
					pDsSys->DespachadorInterno(13,(void *)p0);

				TV_INSERTSTRUCT Nuevo;
				char *p1,*p2;
				int param = 0;
				
				int nslots = 0;
                int freeslots = 0;
				for (p1 = p;*p1;p1 += (strlen(p1)+1)) 
					nslots++;

				int i;
				for (i = 0;i < theApp.m_nlistadirecta;i++)
				{
					if (!theApp.m_listadirecta[i])
						freeslots++;
				}

				int ndir = theApp.m_nlistadirecta;
				if (freeslots < nslots)
				{
					if (!theApp.m_listadirecta)
					{
						theApp.m_nlistadirecta = nslots;
						theApp.m_listadirecta = (char **)malloc(sizeof(char *)*theApp.m_nlistadirecta);
					}
					else
					{
						theApp.m_nlistadirecta += (nslots-freeslots);
						theApp.m_listadirecta = (char **)realloc((void *)theApp.m_listadirecta,sizeof(char *)*theApp.m_nlistadirecta);
					}
					for (;ndir < theApp.m_nlistadirecta;ndir++)
					{
						theApp.m_listadirecta[ndir] = NULL;
					}
				}

				while(strlen(p))
				{
					p1 = p;
					p += (strlen(p)+1);

					p2 = p1;					
					while(*p2)
					{
						if (*p2 == 1)
						{
							*p2 = 0;
							p2++;
							break;
						}
						p2++;
					}
					for (ndir = 0;ndir < theApp.m_nlistadirecta;ndir++)
					{
						if (!theApp.m_listadirecta[ndir])
							break;
					}
					ASSERT(ndir < theApp.m_nlistadirecta);

					theApp.m_listadirecta[ndir] = (char *)malloc(strlen(p2)+1);
					strcpy(theApp.m_listadirecta[ndir],p2);
					
					param = -(ndir + 1);

					Nuevo.hParent = primero;
					Nuevo.hInsertAfter = TVI_LAST;					
					Nuevo.item.pszText = p1;
					Nuevo.item.cchTextMax = strlen(p1);
					Nuevo.item.iImage = 2;
					Nuevo.item.iSelectedImage = 2;
					
					Nuevo.item.lParam = param;
					Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					pNew->InsertItem( &Nuevo );
					
				}

				free((void *)p0);
			}
		}

		/*
		TV_ITEM hNextItem;
		TV_INSERTSTRUCT Nuevo;
		HTREEITEM next;

		hNextItem.hItem = pTree->GetRootItem();
		while(hNextItem.hItem)
		{
			next = hNextItem.hItem;
			hNextItem.pszText = tmpx;
			hNextItem.cchTextMax = sizeof(tmpx)-1;
			hNextItem.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			if (pTree->GetItem(&hNextItem))
			{       
			    LPARAM punto = hNextItem.lParam;
				if (punto)
				{  
					s1 = pTree->GetItemText( next );
					s1.MakeLower();
					if (s1.Find(s) >= 0)
					{
						Nuevo.hParent = primero;
						Nuevo.hInsertAfter = TVI_LAST;
						Nuevo.item = hNextItem;
						Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						pNew->InsertItem( &Nuevo );
					}
				}
			}

			hNextItem.hItem = GetNextItem( next);
		}
		*/

		pNew->Expand(primero,TVE_EXPAND);
		pNew->SelectItem( primero );
	}
}

HTREEITEM CDsxLeftView::GetNextItem(HTREEITEM hItem)
{
        HTREEITEM       hti;

        if( GetTreeCtrl().ItemHasChildren( hItem ) )
                return GetTreeCtrl().GetChildItem( hItem );           // return first child
        else{
                while( (hti = GetTreeCtrl().GetNextSiblingItem( hItem )) == NULL ){
                        if( (hItem = GetTreeCtrl().GetParentItem( hItem ) ) == NULL )
                                return NULL;
                }
        }
        return hti;
}


BOOL CDsxLeftView::BorraBusqueda()
{
	if (theApp.m_UserMenu)
	{		
		CTreeCtrl *pNew = &(theApp.m_UserMenu->GetTreeCtrl());
		HTREEITEM primero;
		char tmpx[1024];		

		primero = pNew->GetRootItem();
		tmpx[0] = 0;
		while(primero)
		{
		   TVITEM item;		   
		   item.hItem = primero;
		   item.mask = TVIF_TEXT | TVIF_HANDLE;
		   item.pszText = tmpx;
		   item.cchTextMax = sizeof(tmpx)-1;
		   pNew->GetItem(&item);
		   if (!strcmp(tmpx,"Busqueda"))
		   {
			   if (pNew->ItemHasChildren(primero))
			   {
				   HTREEITEM hNextItem;
				   HTREEITEM hChildItem = pNew->GetChildItem(primero);

				   while (hChildItem != NULL)
				   {
					  theApp.BorraDeLaListaDirecta( pNew->GetItemData( hChildItem )  );
					  hNextItem = pNew->GetNextItem(hChildItem, TVGN_NEXT);					  
					  pNew->DeleteItem(hChildItem);
					  hChildItem = hNextItem;
				   }
			   }			   
			   pNew->DeleteItem(item.hItem);
			   return TRUE;
		   }
		   primero = pNew->GetNextItem(primero, TVGN_NEXT);
		}	

	}
	return FALSE;
}

HTREEITEM CDsxLeftView::BuscaItemData(HTREEITEM primero,int data)
{
   CTreeCtrl *pNew = &(theApp.m_UserMenu->GetTreeCtrl());   
   HTREEITEM hChildItem = pNew->GetChildItem(primero);
   HTREEITEM ret = NULL;
   int i;

   while (hChildItem != NULL)
   {
	  i = pNew->GetItemData( hChildItem );
	  if (i == data)
	  {
		  return hChildItem;
	  }
	  if (pNew->ItemHasChildren(hChildItem))
	  {
		  if ((ret = BuscaItemData(hChildItem,data)))
		  {
			  return ret;
		  }
	  }
	  hChildItem = pNew->GetNextItem(hChildItem, TVGN_NEXT);	  
   }
   return NULL;
}

BOOL CDsxLeftView::BorraSubOpciones(HTREEITEM item)
{
   CTreeCtrl *pNew = &(theApp.m_UserMenu->GetTreeCtrl());
   HTREEITEM hNextItem;
   HTREEITEM hChildItem = pNew->GetChildItem(item);
   int i;

   while (hChildItem != NULL)
   {
	  if (pNew->ItemHasChildren(hChildItem))
	  {
		  BorraSubOpciones(hChildItem);
	  }
	  i = pNew->GetItemData( hChildItem );
	  if (i < 0)
	  {
			i = -(i+1);
			CString funcion,s;
			funcion.Format("M%04d",i);
			theApp.WriteProfileString("MenuPropio",funcion,"");
			s = theApp.GetProfileString("MenuPropio","Indice","");
			s.Replace(funcion,"");
			theApp.WriteProfileString("MenuPropio","Indice",s);
	  }
	  theApp.BorraDeLaListaDirecta( pNew->GetItemData( hChildItem )  );
	  hNextItem = pNew->GetNextItem(hChildItem, TVGN_NEXT);
	  pNew->DeleteItem(hChildItem);
	  hChildItem = hNextItem;
   }
   return TRUE;
}

BOOL CDsxLeftView::BorraMenuPropio(BOOL todo)
{
	if (theApp.m_UserMenu)
	{		
		CTreeCtrl *pNew = &(theApp.m_UserMenu->GetTreeCtrl());
		HTREEITEM primero;
		char tmpx[1024];		

		primero = pNew->GetRootItem();
		tmpx[0] = 0;
		while(primero)
		{
		   TVITEM item;		   
		   item.hItem = primero;
		   item.mask = TVIF_TEXT | TVIF_HANDLE;
		   item.pszText = tmpx;
		   item.cchTextMax = sizeof(tmpx)-1;
		   pNew->GetItem(&item);
		   if (strcmp(tmpx,"Busqueda"))
		   {
			   if (pNew->ItemHasChildren(primero))
			   {
				   BorraSubOpciones(primero);
			   }			   			   
			   if (todo)
			   {
				   pNew->DeleteItem(primero);
			   }
			   return TRUE;
		   }
		   primero = pNew->GetNextItem(primero, TVGN_NEXT);
		}	

	}
	return FALSE;
}

void CDsxLeftView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;		
	

	*pResult = 0;
}

void CDsxLeftView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if ((int)pTVDispInfo->item.lParam < 0)
	{
		int i;
		i = (int)pTVDispInfo->item.lParam;
		i = -i;
		i--;		
	
		CString funcion,s;

		s = pTVDispInfo->item.pszText;
		s.TrimRight();
		if (s.GetLength() < 2)
		{
			*pResult = 0;
			return;
		}
		funcion.Format("M%04d",i);
		s = theApp.GetProfileString("MenuPropio",funcion,"");

		if (!s.IsEmpty())
		{
			i = s.Find(10);
			if (i >= 0)
			{
					int j;			
					CString s1;
					s1 = s.Mid(i+1);
					j = s1.Find(10);		
					s = s.Mid(0,i+1);
					s += pTVDispInfo->item.pszText;
					if (j > -1)
					{
						s1 = s1.Mid(j);
						s += s1;
					}
					theApp.WriteProfileString("MenuPropio",funcion,s);
			}
		}
		*pResult = TRUE;
	}
	else
		*pResult = 0;
}

void CDsxLeftView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    UINT uFlags;
    HTREEITEM htItem = GetTreeCtrl().HitTest(point, &uFlags);
    if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) 
	{
            /*m_pOldSel = GetTreeCtrl().GetSelectedItem();*/
            GetTreeCtrl().SelectItem(htItem);//, TVGN_DROPHILITE);			
			ShowPopupMenu( point );
    }

	//CTreeView::OnRButtonDown(nFlags, point);
}

/*
void CDsxLeftView::OnContextMenu(CWnd* pWnd, CPoint point)
{
        UINT uFlags;
        CTreeCtrl&      treeCtrl = GetTreeCtrl();
        CPoint ptTree = point;
        treeCtrl.ScreenToClient(&ptTree);
        HTREEITEM htItem = treeCtrl.HitTest(ptTree, &uFlags);

        if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) 
		{
                ShowPopupMenu( point );
                //treeCtrl.SetItemState(htItem, 0, TVIS_DROPHILITED);
        }
        else
                CTreeView::OnContextMenu(pWnd, point);

}
*/

void CDsxLeftView::ShowPopupMenu( CPoint& point )
{

        if (point.x == -1 && point.y == -1)
		{
                //keystroke invocation
                CRect rect;
                GetClientRect(rect);
                ClientToScreen(rect);

                point = rect.TopLeft();
                point.Offset(5, 5);
        }

        CMenu menu;       

		menu.CreatePopupMenu();

		if (theApp.m_UserMenu != this)
		{
			menu.InsertMenu(0,MF_STRING | MF_BYPOSITION,6,"Asignar a tecla de acceso directo");
		}
		else
		{
			menu.InsertMenu(0,MF_STRING | MF_BYPOSITION,1,"Crear carpeta");
			menu.InsertMenu(1,MF_STRING | MF_BYPOSITION,2,"Renombrar");
			menu.InsertMenu(2,MF_STRING | MF_BYPOSITION,3,"Borrar");
			menu.InsertMenu(3,MF_SEPARATOR | MF_BYPOSITION,-1,"");
			menu.InsertMenu(4,MF_STRING | MF_BYPOSITION,4,"Exportar");
			menu.InsertMenu(5,MF_STRING | MF_BYPOSITION,5,"Importar");
			menu.InsertMenu(6,MF_SEPARATOR | MF_BYPOSITION,-1,"");
			menu.InsertMenu(7,MF_STRING | MF_BYPOSITION,6,"Asignar a tecla de acceso directo");
		}

        CMenu* pPopup = &menu;
        ASSERT(pPopup != NULL);
        CWnd* pWndPopupOwner = this;

		/*
        while (pWndPopupOwner->GetStyle() & WS_CHILD)
                pWndPopupOwner = pWndPopupOwner->GetParent();
		*/

		pWndPopupOwner->ClientToScreen(&point);

		HTREEITEM htItem = GetTreeCtrl().GetSelectedItem( );

		if (htItem != NULL)
		{
			int r = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y,
					pWndPopupOwner);

			if (r >= 1 && r < 7)
			{
				r--;				
				if (!r)
				{
					int image = -1,selimage = -1;
					HTREEITEM after = TVI_LAST;
					GetTreeCtrl().GetItemImage( htItem, image, selimage);
					if (image > 1)
					{
						after = htItem;
						htItem = GetTreeCtrl().GetParentItem( htItem );
					}
					if (htItem)
					{
						int i;
						for (i = 0;i < theApp.m_nlistadirecta;i++)
						{
							if (!theApp.m_listadirecta[i])
								break;
						}
						if (i >= theApp.m_nlistadirecta)
						{
							i = theApp.m_nlistadirecta;
							if (!theApp.m_listadirecta)
							{
								theApp.m_nlistadirecta = 1;
								theApp.m_listadirecta = (char **)malloc(sizeof(char *)*theApp.m_nlistadirecta);
							}
							else
							{
								theApp.m_nlistadirecta ++;
								theApp.m_listadirecta = (char **)realloc((void *)theApp.m_listadirecta,sizeof(char *)*theApp.m_nlistadirecta);
							}
						}
						theApp.m_listadirecta[i] = (char *)malloc(1);
						theApp.m_listadirecta[i][0] = 0;

						HTREEITEM nueva = GetTreeCtrl().InsertItem(TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_SELECTEDIMAGE,
														_T("Nueva carpeta"),
														0,
														0,
														0,
														0,
														(LPARAM)(-(i+1)),
														htItem,
														after);
						if (nueva)
						{
							CString funcion,s;
							funcion.Format("M%04d",i);

							s = "";						
							s += (char)10;
							s += "Nueva carpeta";
							{
								int j;				
								j = GetTreeCtrl().GetItemData(htItem);
								if (j < 0)
								{
									char tmp[20];
									s += (char)10;
									sprintf(tmp,"%d",-(j+1));
									s += tmp;
								}				
							}
							theApp.WriteProfileString("MenuPropio",funcion,s);
							s = theApp.GetProfileString("MenuPropio","Indice","");
							if (s.Find(funcion) < 0)
							{
								s+=funcion;
								theApp.WriteProfileString("MenuPropio","Indice",s);
							}

							GetTreeCtrl().ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT);
							GetTreeCtrl().Expand(htItem, TVE_EXPAND    );
							GetTreeCtrl().SelectItem(nueva);

							GetTreeCtrl().EditLabel( nueva );
						}
					}
				}
				else if (r == 1)
				{
					GetTreeCtrl().EditLabel( htItem );
				}
				else if (r == 2)
				{
					if (GetTreeCtrl().GetParentItem( htItem ))
					{
						int i = GetTreeCtrl().GetItemData( htItem );
						if (i < 0)
						{
							i = -(i+1);
							CString funcion,s;
							funcion.Format("M%04d",i);
							theApp.WriteProfileString("MenuPropio",funcion,"");
							s = theApp.GetProfileString("MenuPropio","Indice","");
							s.Replace(funcion,"");
							theApp.WriteProfileString("MenuPropio","Indice",s);
						}
						theApp.BorraDeLaListaDirecta(GetTreeCtrl().GetItemData( htItem ));
						GetTreeCtrl().DeleteItem(htItem);
					}
				}
				else if (r == 3)
				{
					char buffer[512];
					CFileDialog dlgFile(FALSE,"xmp","menupropio.xmp",OFN_OVERWRITEPROMPT,"*.xmp");
					strcpy(buffer,"menupropio.xmp");
					dlgFile.m_ofn.lpstrFile = buffer;
					if (dlgFile.DoModal() == IDOK)
					{
						CString s,mif,funcion;
						try
						{					   
							CStdioFile file(buffer,CFile::modeWrite | CFile::modeCreate | CFile::typeText);
							s = theApp.GetProfileString("MenuPropio","Indice","");
							file.WriteString(s);
							file.WriteString("\n");
							while(!s.IsEmpty())
							{
							funcion = s.Mid(0,5);
							if (funcion.GetLength() < 5 || funcion[0] != 'M')
								break;
							s.Replace(funcion,"");
							mif = theApp.GetProfileString("MenuPropio",funcion,"");
							mif.Replace('\n',14);
							file.WriteString(mif);
							file.WriteString("\n");
							}
							file.Close();
						}
						catch(...)
						{
						}
					}
				}
				else if (r == 4)
				{
					char buffer[512];
					CFileDialog dlgFile(TRUE,"xmp","menupropio.xmp",0,"*.xmp");		
					strcpy(buffer,"menupropio.xmp");
					dlgFile.m_ofn.lpstrFile = buffer;
					if (dlgFile.DoModal() == IDOK)
					{
						BorraMenuPropio(TRUE);

						CString s,mif,funcion;
						try
						{					   
							CStdioFile file(buffer,CFile::modeRead | CFile::typeText);

							file.ReadString(s);

							s.TrimRight('\n');
							s.TrimRight('\r');
							s.TrimRight('\n');

							int i;
							for (i = 0;i < s.GetLength();i += 5)
							{
								if (s[i] != 'M')
									break;
							}
							if (i >= s.GetLength())
							{						
								theApp.WriteProfileString("MenuPropio","Indice",s);
								while(!s.IsEmpty())
								{
								file.ReadString(mif);
								mif.TrimRight('\n');
								mif.TrimRight('\r');
								mif.TrimRight('\n');
								mif.Replace(14,'\n');
								if (mif.IsEmpty())
									break;

								funcion = s.Mid(0,5);
								if (funcion.GetLength() < 5 || funcion[0] != 'M')
									break;
								s.Replace(funcion,"");
								theApp.WriteProfileString("MenuPropio",funcion,mif);
								}
							}
							file.Close();						
						}
						catch(...)
						{
						}
						RellenaMenuPropio();					
					}
				}
				else if (r == 5)
				{
					AsignaAtecla(GetTreeCtrl().GetItemData(htItem));
				}			
			}
		}
}

void CDsxLeftView::AsignaAtecla(int nitem)
{
	  char exe[1024];
	  char par[1024];
	  char tmpt[1024];
	  char opcion[1024];
	  CString titulo;


	  tmpt[0] = 0;
	  
	  {       
		 LPARAM punto = nitem;
		 int directa = (int)punto;
		 if (directa  < 0)
		 {
			 directa = -(directa+1);
			 if (directa < theApp.m_nlistadirecta && pDsSys && theApp.m_listadirecta[directa])
			 {
				 if (!theApp.m_listadirecta[directa][0])
					 return;

				 if (theApp.m_9wnd || theApp.m_simul9)
				 {
					 pDsSys->PreparaEjecucion9((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,(int)0);
				 }
				 else
				 {
					 pDsSys->PreparaEjecucion((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,(int)0);
				 }
				 char *pp = exe;
				 pp += (strlen(exe)+1);
				 if (*pp == 7 && *(pp+1) == 7)
				 {
					 pp+=2;
					 strcpy(pp,theApp.m_listadirecta[directa]+1);
				 }
				 else
				 {
					return;
				 }
				 memcpy(par,"/DS:\"",5);
				 int i;
				 for (i = 0;par[i];i++)
				 {
					 if (par[i] == 3)
					 {
						 par[i] = 0;
						 titulo = ""; /*par+i+1;*/
						 break;
					 }
				 }
				 strcat(par,"\"");
			 }
		 } else if (pMFrame)
		 {  
			 if (theApp.m_9wnd || theApp.m_simul9)
			 {
				 pDsSys->PreparaEjecucion9((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,(int)punto);
			 }
			 else
			 {
				 pDsSys->PreparaEjecucion((char *)exe,(char *)par+5,(char *)(LPCSTR)pDsSys->m_Usuario,(char *)(LPCSTR)pDsSys->m_Password,(int)punto);
			 }

			 memcpy(par,"/DS:\"",5);
			 int i;
			 for (i = 0;par[i];i++)
			 {
				 if (par[i] == 3)
				 {
					 par[i] = 0;
					 titulo = par+i+1;
					 break;
				 }
			 }
			 strcat(par,"\"");
		 }
		 else
			 return;

		 if (punto > 0)
		 {  /* Lo saco del menu standard .. no del tree ... 
  			 CMenu *main = pMFrame->GetMenu();
			 if (main)
			 {		  
				  BOOL BuscaComando_Menu(CMenu *menu,UINT comando,CString &nombre);

				  CMenu *popup = main->GetSubMenu( 5 );		  
				  if (popup)
				  {
					 UINT comando;
					 CString s;
					 comando = punto+MID_XPRIMERA-1;
					 BuscaComando_Menu(popup,comando,s);
					 s += " ";
					 titulo = theApp.m_nomstdmenu + " " + s + titulo;
				  }
			 }   
			 */
			 titulo = theApp.m_nomstdmenu + " " + tmpt + " " + titulo;
			 
		 }
		 else if (punto < 0)
		 {
			 //titulo = theApp.m_nomstdmenu + " " + tmpt + " " + titulo;
			 titulo = tmpt;
		 }
		 else
		 {
			 titulo = theApp.m_nomstdmenu + " " + titulo;
		 }
	  }
	
	opcion[0] = 0;
	char *pp = exe;
	pp += (strlen(exe)+1);
	if (*pp == 7 && *(pp+1) == 7)
	{
		pp+=2;
		strcpy(opcion,pp);
		
		/*
		{
			int i;
			for (i = 0;par[i];i++)
			{
				if (par[i] == '$' && par[i+1] == '$' && par[i+2] == '$')
				{
					i += 3;
					for(;par[i];i++)
					{
						if (par[i] == '#')
						{
							i++;						
							strcpy(par+i," //");
							strcat(par+i,pp);
							for (i++;par[i];i++)
							{
								if (par[i] == ' ')
									par[i] = '&';
							}
							strcat(par,"\"");
							break;
						}
					}
					break;
				}
			}
		}
		*/

	}


	{
        CMenu popmenu;
		CWnd *pWnd = this;
        popmenu.CreatePopupMenu();
        int n = 0;
		CString ss,s;
		int i;
        for (i = 0;i < 10;i++)
        {
		  if (!i)
		  {
			  s.Format("Ctrl-F12 = ");
		  }
		  else
		  {
			  s.Format("Ctrl-F%d = ",i+1);
		  }

          if (!theApp.m_pFunciones[i].IsEmpty())
          {		
             ss = theApp.m_dFunciones[i];
             ss.TrimLeft();
             ss.TrimRight();
          }
		  else
			 ss = "Vacio";
		  s += ss;
          popmenu.InsertMenu(i,MF_STRING | MF_BYPOSITION,833+i,s);
          n++;
        }       
       
        POINT pp;
        ::GetCursorPos(&pp);
        int tecla = (int)popmenu.TrackPopupMenu(TPM_CENTERALIGN| TPM_NONOTIFY | TPM_RETURNCMD,pp.x,pp.y-10,pWnd,NULL);


		if (tecla >= 833 && tecla < 843)
		{
			CString funcion,s;

			funcion.Format("%03d",tecla);

			s = "|";
			s += opcion;
			s += (char)10;
			s += titulo;
			theApp.WriteProfileString("Funciones",funcion,s);

			i = tecla - 833;
			theApp.m_pFunciones[i] = "|";
			theApp.m_pFunciones[i] += opcion;
			theApp.m_dFunciones[i] = titulo;

			pActiveDoc->RefrescaIra();
		}
	}
}


