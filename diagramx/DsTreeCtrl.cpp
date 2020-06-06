// DsTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsxMainFrm.h"
#include "DsxDoc.h"
#include "DsTreeCtrl.h"
#include "DsStdDlg.h"
#include "DsxView.h"


// CDsTreeCtrl

IMPLEMENT_DYNAMIC(CDsTreeCtrl, CTreeCtrl)
CDsTreeCtrl::CDsTreeCtrl()
{
    m_ndata = 0;
	m_group = FALSE;
	m_pstdctrl = NULL;
	m_acelerado = FALSE;
}

CDsTreeCtrl::~CDsTreeCtrl()
{
	/*
	while(m_ndata)
	{
		m_ndata--;
		delete m_data[m_ndata];
	}
	*/
	DeleteCache(&m_cache);
}

void CDsTreeCtrl::DeleteCache(CArray< struct cachenivel *, struct cachenivel *> *pcache) {
    struct cachenivel *ele;   
	int nc = pcache->GetCount();
	for (int i = 0;i < nc;i++) {
		ele = pcache->GetAt(i);
		DeleteCache(&(ele->m_cache));
		delete ele;
	}	   
}


BEGIN_MESSAGE_MAP(CDsTreeCtrl, CTreeCtrl)
   ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
   ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
   ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
   ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
END_MESSAGE_MAP()

// CDsTreeCtrl message handlers


void CDsTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NMTVKEYDOWN* pTVKeyDown = (NMTVKEYDOWN*)pNMHDR;  

   
   if (pTVKeyDown->wVKey == VK_SPACE && ItemHasChildren( GetSelectedItem( ) ) )
   {
      Expand( GetSelectedItem( ), TVE_TOGGLE );       
      TV_ITEM tv;
      UINT state = GetItemState(GetSelectedItem( ),TVIS_EXPANDED);
      if (!(state & TVIS_EXPANDED))
      {		  
         tv.hItem = GetSelectedItem( );
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
         tv.iImage = 0;
         tv.iSelectedImage = 0;
         SetItem(&tv);
      }
      else
      {
         tv.hItem = GetSelectedItem( );
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
         tv.iImage = 1;
         tv.iSelectedImage = 1;
         SetItem(&tv);
      }
      *pResult = 1;
      return;
   }
   

   int comando = CDsStdDlg::DeterminaTecla(pTVKeyDown->wVKey);
   if (comando)
   {
      if (pMFrame)
      {  
		  if (comando != 802)
		  {
			 //((CDsxDoc *)pMFrame->GetActiveDocument())->TeclaStd(comando);
			  *pResult = 0;
			  return;
		  }
		  else
		  {
			    int n;
				HTREEITEM htItem = GetSelectedItem( );
				n = GetItemData(htItem);
				CString s;
				if (n > 0 && n <= m_ndata)
				{
					n--;
					if (m_data[n])
						s = *m_data[n]; 
				}
				if (!m_group && m_pstdctrl && !(m_pstdctrl+pmask)->DisparaEventoUsuario(2,s))
				{
					//TeclaStd(m_teclarespuesta);
				}    
				*pResult = 1;
				return;
		  }
	  }
   }

   *pResult = 0;
}

void CDsTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint point;
	UINT uFlags;
	int n = 0;
	GetCursorPos(&point);
	ScreenToClient(&point);
    HTREEITEM htItem = HitTest(point, &uFlags);
    if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) 
	{            
        SelectItem(htItem);
		n = GetItemData(htItem);    
		CString s;
		if (n > 0 && n <= m_ndata)
		{
			n--;
			if (m_data[n])
				s = *m_data[n]; 
		}
		if (!m_group && m_pstdctrl && !(m_pstdctrl+pmask)->DisparaEventoUsuario(1,s))
		{
			//TeclaStd(m_teclarespuesta);
		}    
	}	
   *pResult = 0;
}

HTREEITEM CDsTreeCtrl::LocalizaNivel(HTREEITEM primero,int *item,int nivel)
{
   HTREEITEM hChildItem = NULL;
   int n;
   int count = item[nivel];
   int nc = 0;
   int i;
   CArray< struct cachenivel *, struct cachenivel *> *pcache = NULL;
   struct cachenivel *ele;

   if (m_acelerado) {
	   pcache = &m_cache;
	   for (i = 0;i < nivel;i++) {
		 nc = pcache->GetCount();
		 n = item[i] - 1;
		 if (nc <= n) {
			 m_acelerado = FALSE;
			 break;
		 }
		 ele = pcache->GetAt(n);
		 pcache = &(ele->m_cache);		 
	   }
   }

   if (m_acelerado) {
	   if (!pcache)
		   m_acelerado = FALSE;
	   else  {
		   nc = pcache->GetCount();
	   }
   }
   

   if (m_acelerado && nc > 0) {	   
	   if (nc >= count) {		   
		   n = count - 1;
		   ele = pcache->GetAt(n);
		   return ele->item;
	   }
	   n = nc - 1;
	   ele = pcache->GetAt(n);
	   hChildItem = ele->item;
   }
   else {
	   if (nc > 0 && pcache != NULL) {
		   pcache->RemoveAll();
	   }
	   n = 0;
	   hChildItem = GetChildItem(primero);
   }   
   while (hChildItem != NULL)
   {
	   if (m_acelerado) {
		   ele = new struct cachenivel;
		   ele->item = hChildItem;
		   pcache->SetAtGrow(n,ele);
	   }
	   n++;
	   if (count == n)
	   {
		   return hChildItem;
	   }
	   hChildItem = GetNextItem(hChildItem, TVGN_NEXT);	  
   }
   return NULL;
}

HTREEITEM CDsTreeCtrl::BuscaItemData(HTREEITEM primero,LPCSTR data,BOOL contenido)
{   
   HTREEITEM hChildItem = NULL;
   HTREEITEM ret = NULL;
   int i;

   if (!primero) 
	  hChildItem = GetRootItem();
   else
	  hChildItem = GetChildItem(primero);

   while (hChildItem != NULL)
   {
	  i = GetItemData( hChildItem );
	  if (contenido)
	  {
		  CString s1,s2;
		  if (i > 0 && i <= m_ndata)
		  {
			  s1 = *m_data[i-1];
			  if (!s1.Compare(data))
			 	 return hChildItem;
		  }
	  }
	  else if (i == atoi(data))
	  {
		  return hChildItem;
	  }

	  if (ItemHasChildren(hChildItem))
	  {
		  if ((ret = BuscaItemData(hChildItem,data,contenido)))
		  {
			  return ret;
		  }
	  }
	  hChildItem = GetNextItem(hChildItem, TVGN_NEXT);	  
   }
   return NULL;
}

HTREEITEM CDsTreeCtrl::DeterminaItem(const char *data,HTREEITEM &hParent,CString &nombre,CString &opcion)
{
	char tmp[1024];
	char *p,*p0;
	int nitem = 0;
	int item[100];
	bool fin = false;		
	HTREEITEM hitem = NULL;

	strcpy(tmp,data);
	p = tmp;
	p0 = tmp;
	while(!fin && *p)
	{
		if (*p == ':')
			fin = true;
		if (*p == '_' || *p == ',' || *p == ':')  
		{
			item[nitem] = atoi(p0);
			nitem++;
			p0 = p+1;
			*p = 0;
		}
		p++;
	}
	if (fin)
	{
		opcion = "";
		p0 = p;
		while(*p)
		{
			if (*p == '{' && *(p+1) == '{')
			{
				*p = 0;
				p++;
				*p = 0;
				p++;
				opcion  = p;
				opcion.Replace("}}","");
				break;
			}
			p++;
		}
		for (char *ipp = p0;*ipp;ipp++)
			oem_a_ansi((unsigned char *)ipp);
		nombre = p0;		
		for (ipp = p0;*ipp;ipp++)
			ansi_a_oem((unsigned char *)ipp);
		hParent = GetRootItem();
		for (int i = 0;i < nitem;i++)
		{
			hitem = LocalizaNivel(hParent,item,i);
			if (!hitem)
				break;
			hParent = hitem;
		}
	}
	else
	{
		hParent = NULL;
		return NULL;
	}
	return hitem;
}

CString CDsTreeCtrl::CreaRutaItem(HTREEITEM hitem)
{
	int n = 1;
	CString s1;
	HTREEITEM parent = GetParentItem(hitem);
	if (parent == GetRootItem())
		parent = NULL;
	while((hitem = GetPrevSiblingItem(hitem)))
		n++;
	s1.Format("%d",n);
	if (parent)
	{
		CString s2;
		s2 = CreaRutaItem(parent);
		s2 += ',';
		s2 += s1;
		return  s2;
	}
	return s1;
}

BOOL CDsTreeCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ((wParam & 0xffff) == ID_COMMANDCONTROL)
	{
		int *p1 = (int *)lParam;
		char *data = (char *)(p1+1);
		switch(p1[0])
		{
			// 10,10,10:Acceso          {{0 aceites fich agifa041 aceites}}			
		case 0:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				hitem = DeterminaItem(data,hParent,nombre,opcion);
				if (hitem || hParent)
				{
					if (!hitem)
					{						
						TV_INSERTSTRUCT Nuevo;
						if (!ItemHasChildren(hParent))
						{
							TV_ITEM tv;
						    tv.hItem = hParent;
							tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
							tv.iImage = 0;
							tv.iSelectedImage = 0;			
							SetItem(&tv);
						}
						Nuevo.hParent = hParent;
						Nuevo.hInsertAfter = TVI_LAST;		
						Nuevo.item.pszText = (LPSTR)(const char *)nombre;
						Nuevo.item.cchTextMax = strlen(nombre);
						Nuevo.item.iImage = 2;
						Nuevo.item.iSelectedImage = 2;
						m_data.SetAtGrow( m_ndata,new CString(opcion));
						//m_data[m_ndata] = new CString(opcion);
						Nuevo.item.lParam = ++m_ndata;
						Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						InsertItem( &Nuevo );
						p1[0]  =  m_ndata;
					}
					else
					{
						TV_ITEM tv;
						int n = GetItemData(hitem);
						if (n > 0 && n <= m_ndata)
						{
							n--;
							if (m_data[n])
							{
								delete m_data[n];

								m_data.SetAtGrow( n,NULL);								
							}
							m_data.SetAtGrow(n,new CString(opcion));
							//m_data[n] = new CString(opcion);
							n++;
						}
						tv.hItem = hitem;
						tv.mask = TVIF_HANDLE | /*TVIF_IMAGE | TVIF_SELECTEDIMAGE | */TVIF_TEXT | TVIF_PARAM ;
						/*
						tv.iImage = 0;
						tv.iSelectedImage = 0;
						*/
						tv.pszText = (LPSTR)(const char *)nombre;
						tv.cchTextMax = strlen(nombre);
						tv.lParam = n;
						SetItem(&tv);
						p1[0]  =  n;
					}
					Invalidate();
				}
				else
					p1[0] = -1;
			}
			break;
		case 1:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
				{
					p1[0] = GetItemData( hitem );
					if (p1[0] > 0 && p1[0] <= m_ndata)
					{
						strcpy(data,*m_data[p1[0]-1]);
					}
					DeleteItem(hitem);
					Invalidate();					
				}
				else
					p1[0] = -1;
			}
			break;
		case 2:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					 hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					 hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					 hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
				{
					SelectItem(hitem);
					p1[0] = GetItemData( hitem );
					if (p1[0] > 0 && p1[0] <= m_ndata)
					{
						strcpy(data,*m_data[p1[0]-1]);
					}
				}
				else
				{
					SelectItem(NULL);
					p1[0] = -1;
				}
			}
			break;
		case 3:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
				{
					p1[0] = GetItemData( hitem );
					if (p1[0] > 0 && p1[0] <= m_ndata)
					{
						strcpy(data,*m_data[p1[0]-1]);
					}					
				}
				else
					p1[0] = -1;
			}
			break;
		case 4:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
				{
					p1[0] = GetItemData( hitem );					
					strcpy(data,GetItemText(hitem));
					for (char *ipp = data;*ipp;ipp++)
						ansi_a_oem((unsigned char *)ipp);
				}
				else
					p1[0] = -1;
			}
			break;
		case 5:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
					hitem = GetChildItem(hitem);
				if (hitem)
				{					
					p1[0] = GetItemData( hitem );					
					if (p1[0] > 0 && p1[0] <= m_ndata)
					{
						strcpy(data,*m_data[p1[0]-1]);
					}
				}
				else
					p1[0] = -1;
			}
			break;
		case 6:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
					hitem = GetNextItem(hitem, TVGN_NEXT);
				if (hitem)
				{					
					p1[0] = GetItemData( hitem );					
					if (p1[0] > 0 && p1[0] <= m_ndata)
					{
						strcpy(data,*m_data[p1[0]-1]);
					}
				}
				else
					p1[0] = -1;
			}
			break;
		case 7:
			{
				HTREEITEM hitem,hParent = NULL;
				CString nombre,opcion;
				if (data[0] == '[')
				{
					hitem = BuscaItemData(NULL,data+1,TRUE);
				}
				else if (data[0] == ':')
				{
					hitem = BuscaItemData(NULL,data+1);
				}
				else
				{
					hitem = DeterminaItem(data,hParent,nombre,opcion);
				}
				if (hitem)
				{
					CString s1;
					p1[0] = GetItemData( hitem );
					s1 = CreaRutaItem(hitem);
					strcpy(data,s1);
				}
				else
					p1[0] = -1;
			}
			break;

		default:
			break;
		}
		return TRUE;
	}
    else if ((wParam & 0xffff) == ID_CUSTOM_ESTADO)
    {		
		if (lParam == 1001)
		{
			//m_noesconsulta = FALSE;
		} else if (lParam == 1100)
		{
			//m_noesconsulta = TRUE;
		}
		return TRUE;
	}
    else
	{
		return CTreeCtrl::OnCommand(wParam, lParam);
	}
}

void CDsTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)pNMHDR;
      
   EnsureVisible( pNMTreeView->itemNew.hItem );
   
	     int n = pNMTreeView->itemNew.lParam;
		 CString s;
		 s.Format("%5d:",n);
		 if (n > 0 && n <= m_ndata)
		 {
			n--;
			if (m_data[n])
				s += *m_data[n]; 
		 }
		 if (!m_group && m_pstdctrl && !(m_pstdctrl+pmask)->DisparaEventoUsuario(0,s))
		 {
			//TeclaStd(m_teclarespuesta);
		 }    

   *pResult = 0;
}

void CDsTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

   //!pNMTreeView->itemNew.lParam
   if (ItemHasChildren( pNMTreeView->itemNew.hItem ))
   {
      TV_ITEM tv;
        tv.hItem = pNMTreeView->itemNew.hItem;
         tv.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	     if (pNMTreeView->itemNew.state & TVIS_EXPANDED)
	     {
            tv.iImage = 1;
            tv.iSelectedImage = 1;            
        }
        else           
	    {			
			tv.iImage = 0;
			tv.iSelectedImage = 0;			
        }
		SetItem(&tv);
   }	   
   *pResult = 0;
}

void CDsTreeCtrl::Inicializa()
{
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
   SetBkColor( colorfondo );  
   //GetTreeCtrl().SetTextColor( GetDocument()->m_cGridFr );   
#else
   /*
   SetBkColor( GetDocument()->m_cGridBk );  
   SetTextColor( GetDocument()->m_cGridFr );   
   */
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
   
   
   SetImageList(&m_ImageList, TVSIL_NORMAL);
   
   DeleteAllItems();

   ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_TRACKSELECT | TVS_SHOWSELALWAYS );
}

