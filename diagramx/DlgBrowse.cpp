// DlgBrowse.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgBrowse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowse dialog


CDlgBrowse::CDlgBrowse(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgBrowse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBrowse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_solofiles = FALSE;
	m_solodirs = FALSE;
	m_seleccion = "";
	m_base = "";
	DLGCONS;
}


void CDlgBrowse::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBrowse)
	DDX_Control(pDX, IDC_SELE, m_sele);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBrowse, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgBrowse)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_SELE, OnItemexpandedSele)
	ON_NOTIFY(TVN_SELCHANGING, IDC_SELE, OnSelchangingSele)
	ON_NOTIFY(NM_DBLCLK, IDC_SELE, OnDblclkSele)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowse message handlers

BOOL CDlgBrowse::OnInitDialog() 
{
   CBASEDLG::OnInitDialog();

   /*
   m_sele.SetBkColor( GetDocument()->m_cGridBk );  
   m_sele.SetTextColor( GetDocument()->m_cGridFr );   
   */
   
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
   
   
   
   m_sele.SetImageList(&m_ImageList, TVSIL_NORMAL);
   
   m_sele.DeleteAllItems();    
   
   HTREEITEM pariente = NULL;
   pariente = m_sele.InsertItem(TVIF_IMAGE | TVIF_TEXT,
	  m_base,
	  0,
	  0,
	  0,
	  0,
	  1,
	  NULL,
	  TVI_FIRST);

   TV_INSERTSTRUCT Nuevo;

   Nuevo.hParent = pariente;
   Nuevo.hInsertAfter = TVI_LAST;					
   Nuevo.item.pszText = "vacio";
   Nuevo.item.cchTextMax = 6;
   Nuevo.item.iImage = 2;
   Nuevo.item.iSelectedImage = 2;
   Nuevo.item.lParam = 0;
   Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;  

   m_sele.InsertItem( &Nuevo );

   m_sele.Expand(pariente, TVE_EXPAND );

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgBrowse::BorraSubOpciones(HTREEITEM item)
{
   CTreeCtrl *pNew = &m_sele;
   HTREEITEM hNextItem;
   HTREEITEM hChildItem = pNew->GetChildItem(item);   

   while (hChildItem != NULL)
   {
	  if (pNew->ItemHasChildren(hChildItem))
	  {
		  BorraSubOpciones(hChildItem);
	  }
	  hNextItem = pNew->GetNextItem(hChildItem, TVGN_NEXT);
	  pNew->DeleteItem(hChildItem);
	  hChildItem = hNextItem;
   }
   return TRUE;
}


void CDlgBrowse::OnItemexpandedSele(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString s;
	HTREEITEM padre;
	HTREEITEM pariente;
	TV_INSERTSTRUCT Nuevo;
	char _tmp[5000];
	char *tmp;
	char *p0;
	int n = 0;
	int memoria = 0;

    HCURSOR  ocur = ::SetCursor(::LoadCursor(AfxGetResourceHandle( ),MAKEINTRESOURCE(IDC_WAIT)));
    ::ShowCursor(TRUE);

    if (!(pNMTreeView->itemNew.state & TVIS_EXPANDED))
	{
		BorraSubOpciones(pNMTreeView->itemNew.hItem);
	    Nuevo.hParent = pNMTreeView->itemNew.hItem;
	    Nuevo.hInsertAfter = TVI_LAST;					
	    Nuevo.item.pszText = "vacio";
	    Nuevo.item.cchTextMax = 6;
	    Nuevo.item.iImage = 2;
	    Nuevo.item.iSelectedImage = 2;
	    Nuevo.item.lParam = 0;
	    Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	    m_sele.InsertItem( &Nuevo );	
		::SetCursor(ocur);
		*pResult = 0;
		return;
	}

	BorraSubOpciones(pNMTreeView->itemNew.hItem);
	
	padre = pNMTreeView->itemNew.hItem;
	s = m_sele.GetItemText( padre );	
	while((padre = m_sele.GetParentItem( padre )))
	{
		s = "/" + s;
		s = m_sele.GetItemText( padre ) + s;
	}	

	s.Replace("\\","/");
	s += "/*";

	s.Replace("//","/");
	s.Replace("//","/");
	

	do
	{
        char *p;
        long tam;
        long respuesta;
        long modo = 1;
        int operacion;
		int localizador = 0;		
	
		if (!memoria)
		{
			tmp = _tmp;			
			memset(_tmp,0,sizeof(_tmp));
			memoria = 4900;
		}
		else
		{
			memoria += 100;
			tmp = (char *)malloc(memoria);
			memset(tmp,0,memoria);
		}	
		
		p = tmp;

		operacion = 4;

        memcpy((void *)p,(void *)&localizador,sizeof(int));
        p += sizeof(int);

        tam = (sizeof(long) * 4) + memoria;
        respuesta = tam;

        memcpy((void *)p,(void *)&tam,sizeof(long));
        p += sizeof(long);	
        memcpy((void *)p,(void *)&respuesta,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&modo,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&operacion,sizeof(int));
        p += sizeof(int);
		
		strcpy(p,s);
        
		if (pDsSys->m_f9DespachadorInterno)
			pDsSys->DespachadorInterno9(13,(void *)tmp);
		else if (pDsSys->m_fDespachadorInterno)
		{
			pDsSys->DespachadorInterno(13,(void *)tmp);
		}

		memcpy(&n,p,sizeof(int));
		p += sizeof(int);
		memcpy(&memoria,p,sizeof(int));
		p += sizeof(int);
		p0 = p;

		if (n || !memoria)
			break;
	} while(-1);

	
	if (n <= 0)
	{
	   Nuevo.hParent = pNMTreeView->itemNew.hItem;
	   Nuevo.hInsertAfter = TVI_LAST;					
	   Nuevo.item.pszText = "vacio";
	   Nuevo.item.cchTextMax = 6;
	   Nuevo.item.iImage = 2;
	   Nuevo.item.iSelectedImage = 2;
	   Nuevo.item.lParam = 0;
	   Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	   m_sele.InsertItem( &Nuevo );	
	}
	else
	{
		for (int i = 0;i < n;i++,p0 += (strlen(p0)+1))
		{
		   Nuevo.hParent = pNMTreeView->itemNew.hItem;
		   Nuevo.hInsertAfter = TVI_LAST;					
		   Nuevo.item.pszText = p0+2;
		   Nuevo.item.cchTextMax = strlen(p0+2)+1;		   
		   Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		   if (*p0 == '*')
		   {
			   if (m_solofiles)			   
				   continue;			   
   			   Nuevo.item.iImage = 0;
			   Nuevo.item.iSelectedImage = 0;
			   Nuevo.item.lParam = 1;
		   }
		   else
		   {
			   if (m_solodirs)
				   continue;			   
   			   Nuevo.item.iImage = 2;
			   Nuevo.item.iSelectedImage = 2;
			   Nuevo.item.lParam = 0;
		   }
		   pariente = m_sele.InsertItem( &Nuevo );
		   if (*p0 == '*')
		   {
			   Nuevo.hParent = pariente;
			   Nuevo.hInsertAfter = TVI_LAST;					
			   Nuevo.item.pszText = "vacio";
			   Nuevo.item.cchTextMax = 6;
			   Nuevo.item.iImage = 2;
			   Nuevo.item.iSelectedImage = 2;
			   Nuevo.item.lParam = 0;
			   Nuevo.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			   m_sele.InsertItem( &Nuevo );	
		   }		   
		}
	}

	m_sele.SortChildren( pNMTreeView->itemNew.hItem );


	if (tmp != _tmp)
		free(tmp);
	
	::SetCursor(ocur);
	*pResult = 0;
}

void CDlgBrowse::OnSelchangingSele(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgBrowse::OnDblclkSele(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}



void CDlgBrowse::OnOK() 
{
	// TODO: Add extra validation here
	int esdir = 0;

	m_seleccion = "";

	HTREEITEM padre;
	padre = m_sele.GetSelectedItem();
	if (padre)
		esdir = m_sele.GetItemData( padre );
	while(padre)
	{
		m_seleccion = m_sele.GetItemText( padre ) + m_seleccion;
		padre = m_sele.GetParentItem( padre );
		if (padre)
		{
			m_seleccion = "/" + m_seleccion;
		}
	}

	if (esdir)
		m_seleccion = m_seleccion + "/";


	m_seleccion.Replace("\\","/");	
	m_seleccion.Replace("//","/");
    m_seleccion.Replace("//","/");

	CBASEDLG::OnOK();
}

void CDlgBrowse::OnCancel() 
{	
	m_seleccion = "";
	CBASEDLG::OnCancel();
}
