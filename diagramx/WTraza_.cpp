// WTraza.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "WTraza.h"
#include "DsxDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CVarTraza

CVarTraza::CVarTraza()
{
m_pListEdit = NULL;
m_activa = FALSE;
}

CVarTraza::~CVarTraza()
{
if (m_pListEdit)
   delete m_pListEdit;
}


BEGIN_MESSAGE_MAP(CVarTraza, CListCtrl)
	//{{AFX_MSG_MAP(CVarTraza)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarTraza message handlers


BOOL CVarTraza::PrepareControl(CWnd *pariente,CRect &area,CFont *font)
{

    if (!Create( WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, area , pariente , ID_TRAZAVAR ))
	   return FALSE;

	ASSERT( m_hWnd );

	if (font)
	   SetFont(font);	


	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP |  LVS_EX_INFOTIP /*| LVS_EX_CHECKBOXES*/;
	
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles );

	Desactiva();
	
	LV_COLUMN		lvcolumn;
	CRect rect;
	GetWindowRect( &rect );

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Dato";
	lvcolumn.iSubItem = 0;
	lvcolumn.iOrder = 0;
	lvcolumn.cx = 60;	
	InsertColumn(0, &lvcolumn);	

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Valor";
	lvcolumn.iSubItem = 1;
	lvcolumn.iOrder = 1;
	lvcolumn.cx = rect.Width() - 60 - 2;
	InsertColumn(1, &lvcolumn);

    DeleteAllItems( );

	InsertaUno(0,"","");

	return TRUE;
}

void CVarTraza::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW *pCD = (NMLVCUSTOMDRAW*)pNMHDR;
	
	*pResult = 0;
}


void CVarTraza::Desactiva()
{
    m_activa = FALSE;
	//SetBkColor( RGB(0,0,255) );
	//Invalidate( );
}

void CVarTraza::Activa()
{
    m_activa = TRUE;
	//SetBkColor( RGB(255,255,255) );
	//Invalidate( );
}

void CVarTraza::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
}

void CVarTraza::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;	

	CString str = pDispInfo->item.pszText;
	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;

    CInPlaceEdit* pListEdit = new CInPlaceEdit( item, subitem, str );
	
	CRect  rect( 0,0,1,1 );
	DWORD dwStyle = ES_LEFT;
	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL;
	if (pListEdit->Create( dwStyle, rect, this , ID_TRAZASINBOLFIN ))
	{
	   m_pListEdit = pListEdit;
	   PositionControl( m_pListEdit, item, subitem );	
	   m_pListEdit->CalculateSize();	   
	}
	else
	{
	   delete pListEdit;	   
	}	
	
	*pResult = 1;
}

void CVarTraza::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	int ulitem = GetItemCount() - 1;
	// This is coming from the grid list control notification.
	if( m_pListEdit )
		{
		CString str;
		if( pDispInfo->item.pszText)
		    {
			str = pDispInfo->item.pszText;
			if (!subitem)
			  {
			  str.TrimLeft();
			  str.TrimRight();
			  }
			SetItemText( item, subitem, str );
			if (!subitem)
			  {
		      if (!str.Compare(""))
			    {
                if (item < ulitem)
			       DeleteItem( item );
			    }
              else
			    {
			    if (item == ulitem)
			       {  			  
			       InsertaUno(ulitem+1,"","");               
			       }			
				CWTraza *m_ptraza = (CWTraza *)GetParent();
				if (m_ptraza)
				   {
				   m_ptraza->PideVar(item);
				   }
				}
			  }
			else
			  {
			  CWTraza *m_ptraza = (CWTraza *)GetParent();
			  if (m_ptraza)
				   {
				   m_ptraza->PideVal(item);
				   }
			  }
			}
		delete m_pListEdit;
		m_pListEdit = 0;
		}
	*pResult = 0;
}

void CVarTraza::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LVHITTESTINFO ht;
	int state = 0;

	ht.pt = point;
	int rval = ListView_SubItemHitTest( m_hWnd, &ht );
    if (rval >= 0)
	   {
	   state = GetItemState( ht.iItem, LVIS_FOCUSED );
	   }   

    if (m_pListEdit)
	   {
	   if( GetFocus() != this ) SetFocus();
	   }

    CListCtrl::OnLButtonDown(nFlags, point);	

	if (!state || !m_activa || (ht.iItem == (GetItemCount()-1) &&  ht.iSubItem)) 
    {	  
	  return;
	}

	CString str;
	str = GetItemText( ht.iItem, ht.iSubItem );
    LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_BEGINLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = ht.iItem;
	dispinfo.item.iSubItem = ht.iSubItem;
	dispinfo.item.pszText = (LPTSTR)((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
}


BOOL CVarTraza::PositionControl(CWnd *pWnd, int iItem, int iSubItem)
	{
	ASSERT( pWnd && pWnd->m_hWnd );
	ASSERT( iItem >= 0 );
	// Make sure that the item is visible
	if( !EnsureVisible( iItem, TRUE ) ) return NULL;
	
	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	ASSERT( iSubItem >= 0 && iSubItem < nColumnCount );
	if( iSubItem >= nColumnCount || 
		// We have to take into account that the header may be reordered
		GetColumnWidth(Header_OrderToIndex( pHeader->m_hWnd,iSubItem)) < 5 )
		{
		return 0;
		}
	
	// Get the header order array to sum the column widths up to the selected cell.
	int *orderarray = new int[ nColumnCount ];
	Header_GetOrderArray( pHeader->m_hWnd, nColumnCount, orderarray );
	int offset = 0;
	int i;
	for( i = 0; orderarray[i] != iSubItem; i++ )
		offset += GetColumnWidth( orderarray[i] );
	int colwidth = GetColumnWidth( iSubItem );
	delete[] orderarray;
	
	CRect rect;
	GetItemRect( iItem, &rect, LVIR_BOUNDS );
	
	// Scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + colwidth + rect.left > rcClient.right )
        {
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );		
		GetItemRect( iItem, &rect, LVIR_BOUNDS );		
        }		

	rect.left += offset+4;
	rect.right = rect.left + colwidth - 3 ;
	// The right end of the control should not go past the edge 
	// of the grid control.
	if( rect.right > rcClient.right) 
		rect.right = rcClient.right;
	pWnd->MoveWindow( &rect );

	return 1;	
	}


void CVarTraza::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( GetFocus() != this ) SetFocus();	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CVarTraza::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( GetFocus() != this ) SetFocus();		
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CVarTraza::InsertaUno(int item, LPCSTR texto1, LPCSTR texto2)
{

	LVITEM		lvitem;

	memset(&lvitem,0,sizeof(lvitem));
    
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = item;
	lvitem.iSubItem = 0;
	lvitem.pszText = (char *)texto1;

    InsertItem( &lvitem );

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = item;
	lvitem.iSubItem = 1;
	lvitem.pszText = (char *)texto2;

    SetItem( &lvitem );

}


/////////////////////////////////////////////////////////////////////////////
// CWTraza

CWTraza::CWTraza()
{
m_visible = FALSE;
m_rect = CRect(6,6,355,455);
m_menu.LoadMenu(IDR_MENUTRAZA);
CreateEx(0, AfxRegisterWndClass(NULL),
	"Traza Interna 1.0", WS_POPUPWINDOW  | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN,
	 m_rect,NULL,(int)m_menu.m_hMenu);
m_pFont = NULL;
m_activa = FALSE;
m_split.Create(WS_CLIPSIBLINGS | WS_CHILD/*|WS_BORDER|WS_DLGFRAME*/|WS_VISIBLE, CRect(0,0,0,0),this,ID_TRAZASINBOLFIN-1,TRUE);
m_modoavance = 0;
m_continuar = FALSE;
m_salto = FALSE;
m_corre = FALSE;
}


CWTraza::~CWTraza()
{
if (m_pFont)
  delete m_pFont;
}


BEGIN_MESSAGE_MAP(CWTraza, CWnd)
	//{{AFX_MSG_MAP(CWTraza)
	ON_WM_SIZE()
	ON_COMMAND(ID_TRAZA_AVANCEUNO, OnTrazaAvanceuno)
	ON_COMMAND(ID_TRAZA_CORRER, OnTrazaCorrer)
	ON_COMMAND(ID_TRAZA_AVANCECONTINUO, OnTrazaAvancecontinuo)
	ON_COMMAND(ID_TRAZA_AVANCESALTO, OnTrazaAvancesalto)
	ON_COMMAND(ID_TRAZA_TODOSBREAK, OnTrazaTodosbreak)
	ON_COMMAND(ID_TRAZA_ABRIR, OnTrazaAbrir)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TRAZA_BUSCAR, OnTrazaBuscar)
	ON_COMMAND(ID_TRAZA_SIGUIENTE, OnTrazaSiguiente)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWTraza message handlers

BOOL CWTraza::SetTexto(LPCSTR texto, BOOL append)
{
if (!m_visible)
   {   
   CRect r;
   ShowWindow(SW_SHOW);

   /*
   if (theApp.m_EntornoActivo)
      {
      m_pFont = new CFont();
      m_pFont->CreateFontIndirect(&(theApp.m_EntornoActivo->m_pDoc->m_fontrever));
      SetFont(m_pFont);
      }
   */

   GetClientRect(&r);
   r.bottom -= 100;
   m_edit.PrepareControl(this,r,m_pFont); 

   GetClientRect(&r);
   r.top = r.bottom - 100;
   m_vars.PrepareControl(this,r,m_pFont);  

   m_split.SetPanes(&m_edit,&m_vars);
   
   m_visible = TRUE;   
   }

m_edit.m_buslinea = -1;

return m_edit.PonTexto(texto,append);
#ifdef EDITWIN
if (append)
   {
   int tam = m_edit.GetWindowTextLength();
   char *pp = new char[ tam +10 + strlen(texto) ];

   m_edit.GetWindowText( pp , tam + 1);
   pp[tam] = 0;
   strcpy(pp+tam,texto);

   m_edit.SetWindowText((LPCSTR)pp);

   delete pp; 
   }
else
   m_edit.SetWindowText(texto);

return TRUE;
#endif
}

void CWTraza::OnSize(UINT nType, int cx, int cy) 
{

	CWnd::OnSize(nType, cx, cy);

    if (nType == SIZE_MINIMIZED || !m_edit.m_hWnd) return;   

	m_split.Reajusta();
   
}


void CWTraza::PuntoTraza(int entero, void *puntero)
{
char *p = (char *)puntero;
static int pide_variables = 0;
static int break_salto = 0;
static char break_fichero[10];
if (m_edit.m_hWnd)
  {		   

     if (break_salto)
	    {
		int j;

	    for (j = 0;j < 50;j++)
		   {
		   if (!strcmp(m_edit.ficheros[j],break_fichero))
		      {
			  break_salto--;
			  if (!strcmp(break_fichero,m_edit.m_fichero))
			     m_edit.SetCheck(break_salto,FALSE);
			  m_edit.m_breaks[j].RemoveKey(break_salto);
			  break;
			  }
		   }
		break_salto = 0;
		}


     if (!pide_variables && entero == (m_edit.m_linea+1)) return;

     /* refrescar variables */
	 if (p[1] == 1 && pide_variables > 0)
	    {
        m_vars.SetItemText( pide_variables-1, 1, p+2 );
		}

licencia_1:	 
     if (pide_variables < (m_vars.GetItemCount( )-1))
	    {
		CString s;
		p[1] = 1;
		p[2] = 1;
		s = m_vars.GetItemText( pide_variables, 0 );
		s.TrimLeft();
		s.TrimRight();
		strcpy(p+2,s);
		pide_variables++;
		return;
	    }	
	 

     pide_variables = 0;
	 p[1] = 0;
	 m_edit.Situate(entero);     	 

	 if (!m_modoavance)
	    {
        if (!m_activa)
	       {
           m_vars.Activa();        
	       m_menu.EnableMenuItem( ID_TRAZA_AVANCEUNO , MF_BYCOMMAND | MF_ENABLED );
		   m_menu.EnableMenuItem( ID_TRAZA_CORRER , MF_BYCOMMAND | MF_ENABLED );
		   m_menu.EnableMenuItem( ID_TRAZA_AVANCESALTO , MF_BYCOMMAND | MF_ENABLED );
		   m_menu.EnableMenuItem( ID_TRAZA_AVANCECONTINUO , MF_BYCOMMAND | MF_ENABLED );
		   m_menu.EnableMenuItem( ID_TRAZA_TODOSBREAK , MF_BYCOMMAND | MF_ENABLED );
		   //m_menu.EnableMenuItem( ID_TRAZA_ABRIR , MF_BYCOMMAND | MF_ENABLED );
	       m_activa = TRUE;
		   } 
        MSG msg;
		int disp;
		m_continuar = FALSE;
		SetFocus();

        while (!m_continuar && ::GetMessage((LPMSG)&msg,m_hWnd,0,0))
           {
            if (theApp.PreTranslateMessage(&msg))
                disp = 0;
            else
                disp = 1;          

		   
           if(msg.message == WM_KEYDOWN)
             {
             switch( msg.wParam )
                {
                case VK_F3:
				    OnTrazaBuscar();
					disp = 0;
				    break;
                case VK_F4:
				    OnTrazaSiguiente();
					disp = 0;
				    break;
				case VK_F5:
				    OnTrazaCorrer();
				    disp = 0;
				    break;
				case VK_F6:
				    OnTrazaAvancecontinuo();
				    disp = 0;
				    break;					
				case VK_F7:
					OnTrazaAvanceuno();
					disp = 0;
                    break;
                case VK_F8:
				    OnTrazaAvancesalto();
					disp = 0;
                    break;
                case VK_F10:
				    OnTrazaTodosbreak();
					disp = 0;
                    break;
                default:
					break;                    
                }            
             }
		   else
           if(msg.message == WM_KEYUP)
             {
             switch( msg.wParam )
                {
				case VK_F3:
				case VK_F4:
				case VK_F5:
				case VK_F6:
				case VK_F7:
                case VK_F8:					
				case VK_F10:					
					disp = 0;
                    break;
                default:
					break;                    
                }            
			 }
		   if (disp)
		     {
             TranslateMessage((LPMSG)&msg);
             DispatchMessage((LPMSG)&msg);
			 }
           }
		m_continuar = FALSE;
		if (m_edit.m_buslinea > -1) m_edit.SetItemState( m_edit.m_buslinea , 0 , LVIS_DROPHILITED );
		if (m_corre)
		   {
           p[0] = 1;
		   p[1] = 0;
		   m_corre = FALSE;
		   }
        else
        if (m_salto)
		   {
		   int ii = m_edit.ind_fichero;
		   int ll = m_edit.m_linea+1;
		   BOOL verdad;

		   if (!m_edit.m_breaks[ii].Lookup( ll, verdad ) || !verdad)
		      {
			  break_salto = ll+1;
			  strcpy(break_fichero,m_edit.m_fichero);
			  m_edit.m_breaks[ii][ll] = TRUE;
			  m_refresca_breaks = TRUE;
			  }

		   p[0] = 1;
		   p[1] = 0;
		   m_salto = FALSE;			  		   
		   }

        if (m_activa)
	       {
           m_vars.Desactiva();
           //m_menu.EnableMenuItem( ID_TRAZA_AVANCEUNO , MF_BYCOMMAND | MF_GRAYED );
	       m_menu.EnableMenuItem( ID_TRAZA_CORRER , MF_BYCOMMAND | MF_GRAYED );
	       m_menu.EnableMenuItem( ID_TRAZA_AVANCESALTO , MF_BYCOMMAND | MF_GRAYED );
	       m_menu.EnableMenuItem( ID_TRAZA_AVANCECONTINUO , MF_BYCOMMAND | MF_GRAYED );
	       m_menu.EnableMenuItem( ID_TRAZA_TODOSBREAK , MF_BYCOMMAND | MF_GRAYED );
	       m_menu.EnableMenuItem( ID_TRAZA_ABRIR , MF_BYCOMMAND | MF_GRAYED );
	       m_activa = FALSE;
	       }

		if (m_modoavance == 2)
		   {
		   m_modoavance = 0;
		   p[0] = 3;
		   p[1] = 0;
		   PostMessage( WM_CLOSE );
		   }
		else
		if (m_modoavance == 3)
		   {		   
		   m_modoavance = 0;
		   p[1] = 0;
		   goto licencia_1;
		   }
		else
		if (m_modoavance >= 5)
		   {
		   pide_variables = m_vars.GetItemCount( );
		   CString s;
		   p[1] = 3;		   

		   s = m_vars.GetItemText( m_modoavance - 5, 0 );
		   s.TrimLeft();
		   s.TrimRight();
		   strcpy(p+2,s);

		   int l = strlen(p+2);
		   p[l+2] = 1;
		   s = m_vars.GetItemText( m_modoavance - 5, 1 );
           strcpy(p+3+l,s);

		   m_modoavance = 0;
		   }
		else
		   {
		   if (m_refresca_breaks)
		     {		     
		     /* componer break points */
			 int d = 2;
		     p[1] = 2;
             int j,tope,i;
			 for (j = 0;j < 50;j++)
			   {
			   if (m_edit.ficheros[j][0])
			      {
                  sprintf(p+d,"%-10.10s",m_edit.ficheros[j]);
				  d += 10;
                  tope = m_edit.m_breaks[j].GetCount( );
                  sprintf(p+d,"%5d",tope);
				  d+=5;
                  i = 0;

	              POSITION pos;
	              int item;
	              BOOL verdad;
	              pos = m_edit.m_breaks[j].GetStartPosition( );
	              while(pos)
	                {
		            m_edit.m_breaks[j].GetNextAssoc( pos, item, verdad );
		            if (verdad)
		               {
			           sprintf(p+d,"%5d",item+1);
					   d+=5;
					   i++;
					   if (i >= tope) break;
			           }
		            }
				  for(;i < tope;i++)
				    {
		            sprintf(p+d,"%5d",0);
					d+=5;
					}
				  }
			   }
		     p[d] = 0;
		     m_refresca_breaks = FALSE;
			 }
		   }
		}
  }
}


void CWTraza::DesactivaTraza(int entero, void *puntero)
{

    if (m_activa)
	  {
      m_vars.Desactiva();
      m_menu.EnableMenuItem( ID_TRAZA_AVANCEUNO , MF_BYCOMMAND | MF_GRAYED );
	  m_menu.EnableMenuItem( ID_TRAZA_CORRER , MF_BYCOMMAND | MF_GRAYED );
	  m_menu.EnableMenuItem( ID_TRAZA_AVANCESALTO , MF_BYCOMMAND | MF_GRAYED );
	  m_menu.EnableMenuItem( ID_TRAZA_AVANCECONTINUO , MF_BYCOMMAND | MF_GRAYED );
	  m_menu.EnableMenuItem( ID_TRAZA_TODOSBREAK , MF_BYCOMMAND | MF_GRAYED );
	  m_menu.EnableMenuItem( ID_TRAZA_ABRIR , MF_BYCOMMAND | MF_GRAYED );
	  m_activa = FALSE;
	  }
/*
#define ID_TRAZA_AVANCEUNO              32805
#define ID_TRAZA_AVANCECONTINUO         32806
#define ID_TRAZA_CORRER                 32807
#define ID_TRAZA_PONBREAK               32808
#define ID_TRAZA_QUITARBREAK            32809
#define ID_TRAZA_TODOSBREAK             32810
#define ID_TRAZA_AVANCESALTO            32811
#define ID_TRAZA_PONDATO                32812
#define ID_TRAZA_QUITARDATO             32813
#define ID_TRAZA_SITUACION              32814
*/
}


void CWTraza::OnTrazaAvanceuno() 
{
m_continuar = TRUE;
m_modoavance = 0;
m_salto = FALSE;
m_corre = FALSE;
}

void CWTraza::OnTrazaCorrer() 
{
m_continuar = TRUE;
m_corre = TRUE;
m_modoavance = 0;
m_salto = FALSE;
}

void CWTraza::OnTrazaAvancecontinuo() 
{
m_continuar = TRUE;
m_modoavance = 1;	
m_salto = FALSE;
m_corre = FALSE;
}

void CWTraza::OnTrazaAvancesalto() 
{
m_continuar = TRUE;
m_salto = TRUE;
m_modoavance = 0;	
m_corre = FALSE;
}

void CWTraza::OnTrazaAbrir() 
{
m_continuar = TRUE;
m_modoavance = 0;
m_salto = FALSE;
m_corre = FALSE;
/* FALTA */
}

void CWTraza::OnTrazaTodosbreak() 
{
int j;
for (j = 0;j < 50;j++)
   {   
   if (j != m_edit.ind_fichero)
      {
	  m_edit.ficheros[j][0] = 0;
	  m_edit.m_breaks[j].RemoveAll();
	  }
   else
      {
	  POSITION pos;
	  int item;
	  BOOL verdad;
	  pos = m_edit.m_breaks[j].GetStartPosition( );
	  while(pos)
	     {
		 m_edit.m_breaks[j].GetNextAssoc( pos, item, verdad );
		 if (verdad)
		    {
			m_edit.SetCheck(item,FALSE);
			}
		 }
      m_edit.m_breaks[j].RemoveAll();
	  }
   }
ActivaBreak();
}


BOOL CWTraza::SetBreaks(LPCSTR pp)
{

	char fichero[10];
	char tmpx[6];
	int tope,j,i;
	int k,nuevo;


    for (k = 0;k < 50;k++)
	    {
	  	m_edit.ficheros[k][0] = 0;
		m_edit.m_breaks[k].RemoveAll();
		}
	m_edit.ind_fichero = -1;

    while (*pp)
	   {
	   if (strlen(pp) < 15) break;

       memcpy(fichero,pp,9);
	   fichero[9] = 0;
	   for (char *kk = fichero+8;kk >= fichero;kk--)
	      {
		  if (*kk == ' ') *kk = 0;
		  }

	   pp += 10;
       memcpy(tmpx,pp,5);
	   tmpx[5] =0;
       tope = atoi(tmpx);
	
	   pp += 5;
	   if ((int)strlen(pp) < (tope*5)) break;

	   nuevo = -1;
       for (k = 0;k < 50;k++)
	      {
		  if (!m_edit.ficheros[k][0]) {nuevo = k;continue;}
		  if (!strcmp(m_edit.ficheros[k],fichero))
		     {
			 nuevo = k;
			 break;
			 }
		  }
	   if (nuevo == -1) nuevo = 0;
	   strcpy(m_edit.ficheros[nuevo],fichero);

	   if (!strcmp(fichero,m_edit.m_fichero))
	      {
          m_edit.ind_fichero = nuevo;
		  }

	   for (j = 0;j < tope;j++)
		   {
           memcpy(tmpx,pp,5);
	       tmpx[5] =0;
           i = atoi(tmpx) - 1;
           
		   m_edit.m_breaks[nuevo][i] = TRUE;
		   if (nuevo == m_edit.ind_fichero)
		      {
              m_edit.SetCheck(i);
			  }           
		   pp += 5;
		   }
	   }

	if (m_edit.ind_fichero == -1)
	   {
	   nuevo = -1;
       for (k = 0;k < 50;k++)
	      {
		  if (!m_edit.ficheros[k][0]) {nuevo = k;continue;}
		  if (!strcmp(m_edit.ficheros[k],m_edit.m_fichero))
		     {
			 nuevo = k;
			 break;
			 }
		  }
	   if (nuevo == -1) nuevo = 0;
	   strcpy(m_edit.ficheros[nuevo],m_edit.m_fichero);
       m_edit.ind_fichero = nuevo;
	   }
   m_refresca_breaks = FALSE;
return TRUE;
}


void CWTraza::PideVar(int var)
{
m_continuar = TRUE;
m_modoavance = 0;
m_salto = FALSE;
m_corre = FALSE;
m_modoavance = 3;
}

void CWTraza::PideVal(int var)
{
m_continuar = TRUE;
m_modoavance = 0;
m_salto = FALSE;
m_corre = FALSE;
m_modoavance = 5 + var;
}

void CWTraza::ActivaBreak()
{
m_refresca_breaks = TRUE;
}


void CWTraza::OnTrazaBuscar()
{
if (m_buscar.DoModal() == IDOK)
   {
   if (m_edit.Busca(m_buscar.m_abuscar))
      m_haybusqueda = TRUE;
   }
else
   m_haybusqueda = FALSE;
BringWindowToTop();
}

void CWTraza::OnTrazaSiguiente() 
{
if (m_haybusqueda)
   {
   if (!m_edit.Busca(m_buscar.m_abuscar))
      m_haybusqueda = FALSE;
   }
}

void CWTraza::OnClose() 
{

    if (m_activa)
	   {
       m_continuar = TRUE;
       m_corre = TRUE;
       m_modoavance = 2;
       m_salto = FALSE;
	   return;       
	   }
	theApp.m_pTraza = NULL;	
	CWnd::OnClose();
    delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

CInPlaceEdit::CInPlaceEdit(int iItem, int iSubItem, CString sInitText)
:m_sInitText( sInitText )
	{
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bESC = FALSE;
	}

CInPlaceEdit::~CInPlaceEdit()
	{
	}


BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
        {
		if( nChar == VK_ESCAPE )
			m_bESC = TRUE;
		GetParent()->SetFocus();
		return;
        }
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	CString str;
	GetWindowText(str);
	
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iItem;
	dispinfo.item.iSubItem = m_iSubItem;
	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = m_bESC ? 0 : str.GetLength();
	
	GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(), 
		(LPARAM)&dispinfo );
}

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

    SetFocus();
	CFont* font = GetParent()->GetFont();
	SetFont(font);
	SetWindowText( m_sInitText );	
	CalculateSize();
	SetSel( 0, -1 );

	return 0;
}


BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
        {
		SHORT sKey = GetKeyState( VK_CONTROL);
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| sKey
			)
			{
			::TranslateMessage(pMsg);
			/* Strange but true:
			If the edit control has ES_MULTILINE and ESC
			is pressed the parent is destroyed if the 
			message is dispatched.  In this 
			case the parent is the list control. */
			if( !(GetStyle() & ES_MULTILINE) || pMsg->wParam != VK_ESCAPE )
				{
				::DispatchMessage(pMsg);
				}
			return TRUE;                    // DO NOT process further
			}
        }
	
	return CEdit::PreTranslateMessage(pMsg);
}


void CInPlaceEdit::CalculateSize()
	{
	// Get text extent
	CString str;    

	GetWindowText( str );
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size;

	// Get client rect
	CRect rect, parentrect;
	GetClientRect( &rect );
	GetParent()->GetClientRect( &parentrect );
	
	// Transform rect to parent coordinates
	ClientToScreen( &rect );
	GetParent()->ScreenToClient( &rect );
	
	if( !(GetStyle() & ES_MULTILINE ) )
		{
		size = dc.GetTextExtent( str );
		dc.SelectObject( pFontDC );
		size.cx += 5;                           // add some extra buffer
		}
	else
		{
		CRect thinrect( rect );  // To measure the skinniest text box
		CRect widerect( rect );  // To measure the wides text box
		widerect.right = parentrect.right;
		// Use the shortest of the two box sizes.
		int thinheight = dc.DrawText( str, &thinrect, DT_CALCRECT|DT_NOPREFIX|DT_LEFT|DT_EXPANDTABS|DT_WORDBREAK );
		int wideheight = dc.DrawText( str, &widerect, DT_CALCRECT|DT_NOPREFIX|DT_LEFT|DT_EXPANDTABS|DT_WORDBREAK );
		if(wideheight && thinheight >= wideheight )
			{
			size.cy = wideheight + 5;
			size.cx = widerect.right - widerect.left + 5;
			}
		else
			{
			size.cy = thinheight + 5;
			size.cx = thinrect.right - thinrect.left + 5;
			}
		}
	
	// Check whether control needs to be resized
	// and whether there is space to grow
	int changed = 0;
	if( size.cx > rect.Width() )
		{
		if( size.cx + rect.left < parentrect.right-2 )
			rect.right = rect.left + size.cx;
		else
			rect.right = parentrect.right-2;
		changed = 1;
		}
	if( size.cy > rect.Height() )
		{
		if( size.cy + rect.top < parentrect.bottom-2 )
			rect.bottom = rect.top + size.cy;
		else
			{
			rect.bottom = parentrect.bottom-2;
			ShowScrollBar( SB_VERT );
			}
		changed = 1;
		}
	// If the size became larger rposition the window.
	if( changed )
		MoveWindow( &rect );
	}






/////////////////////////////////////////////////////////////////////////////
// CFonTraza

CFonTraza::CFonTraza()
{
ind_fichero = 0;
memset(ficheros,0,sizeof(ficheros));
m_fichero[0] = 0;
}

CFonTraza::~CFonTraza()
{
}


BEGIN_MESSAGE_MAP(CFonTraza, CListCtrl)
	//{{AFX_MSG_MAP(CFonTraza)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFonTraza message handlers

BOOL CFonTraza::PrepareControl(CWnd *pariente, CRect &area,CFont *font)
{
    if (!Create( WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, area , pariente , ID_TRAZAEDIT ))
	   return FALSE;

	ASSERT( m_hWnd );

	DWORD styles = LVS_EX_FULLROWSELECT |  LVS_EX_INFOTIP | LVS_EX_CHECKBOXES;
	
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles );

	if (font)
	   SetFont(font);	
	
	LV_COLUMN		lvcolumn;
	CRect rect;
	GetWindowRect( &rect );

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Linea";
	lvcolumn.iSubItem = 0;
	lvcolumn.iOrder = 0;
	lvcolumn.cx = GetStringWidth( "12345" ) + 25;
	InsertColumn(0, &lvcolumn);	


	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Fuente";
	lvcolumn.iSubItem = 1;
	lvcolumn.iOrder = 1;
	lvcolumn.cx = rect.Width() - lvcolumn.cx;
	InsertColumn(1, &lvcolumn);	

    DeleteAllItems( );

	return TRUE;
}

BOOL CFonTraza::PonTexto(LPCSTR texto,BOOL append)
{

   ShowWindow(SW_HIDE);
   if (!append)
     {
     DeleteAllItems();
	 m_linea = -1;
	 }

   int items = GetItemCount();

   LVITEM		lvitem;
   char tmp[512];
 
   memset(&lvitem,0,sizeof(lvitem));

   int j = 0,k,colmax;
   if (append)
      colmax = GetColumnWidth( 1 );   
   else
      {
      colmax = 0;      
	  }
   for (int i = 0;texto[i];i++)
   { 
     if (texto[i] == 13 && texto[i+1] == 10)
	   {
       memcpy(tmp,texto+j,i-j);
	   tmp[i-j] = 0;
	   if (tmp[0] == 1)
	     {
		 LVCOLUMN colum;
		 colum.mask = LVCF_TEXT;
		 colum.pszText = tmp+1;
		 SetColumn( 1,&colum);

		 strcpy(m_fichero,tmp+1);

         int k,nuevo = -1;
		 for (k = 0;k < 50;k++)
		    {
			if (nuevo == -1 && !ficheros[k][0]) nuevo = k;
			if (!strcmp(ficheros[k],m_fichero))
			   {
			   nuevo = k;
			   break;
			   }
			}
		 if (nuevo == -1) nuevo = 0;
		 strcpy(ficheros[nuevo],m_fichero);
		 ind_fichero = nuevo;
		 m_breaks[ind_fichero].RemoveAll();

		 }
	   else
	     {
	     tmp[5] = 0;

         lvitem.mask = LVIF_TEXT;
         lvitem.iItem = items;
         lvitem.iSubItem = 0;
         lvitem.pszText = tmp;

	     InsertItem( &lvitem );

         lvitem.mask = LVIF_TEXT;
         lvitem.iItem = items++;
         lvitem.iSubItem = 1;
         lvitem.pszText = tmp+6;

	     SetItem( &lvitem );

	     k = GetStringWidth( (LPCTSTR)(tmp+6) ) + 20;
	     if (k > colmax) colmax = k;      
	   
	     i++;
	     j = i + 1;
		 }
	   }
   }


   SetColumnWidth( 1 , colmax );
   ShowWindow(SW_SHOW);   

return TRUE;
}

BOOL CFonTraza::Situate(int linea)
{
EnsureVisible( linea-1, TRUE );
int old = m_linea;
m_linea = linea-1;
if (m_buslinea > -1) SetItemState( m_buslinea , 0 , LVIS_DROPHILITED );
m_buslinea = m_linea;
if (old != -1)
   {
   SetItemState( old, 0  , LVIS_SELECTED );
   }
SetItemState( m_linea, LVIS_SELECTED , LVIS_SELECTED );
return TRUE;
}

void CFonTraza::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uNewState & LVIS_SELECTED) != (pNMListView->uOldState & LVIS_SELECTED))
	   {
       if ((pNMListView->uNewState & LVIS_SELECTED) && pNMListView->iItem != m_linea ||
	      !(pNMListView->uNewState & LVIS_SELECTED) && pNMListView->iItem == m_linea)
	      {
	      *pResult = 1;
	      return;
	      }	
		}

	*pResult = 0;
}


void CFonTraza::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState & LVIS_SELECTED) == (pNMListView->uOldState & LVIS_SELECTED))
	   {
	   BOOL cambiar = FALSE;
	   if (GetCheck( pNMListView->iItem ))
	      {
		  if (!m_breaks[ind_fichero].Lookup(pNMListView->iItem , cambiar ))
		     cambiar = TRUE;
          m_breaks[ind_fichero][ pNMListView->iItem ] = TRUE;          
		  }
	   else
	      {		  
		  if (m_breaks[ind_fichero].Lookup(pNMListView->iItem , cambiar ))
             cambiar = TRUE;
		  m_breaks[ind_fichero].RemoveKey( pNMListView->iItem );
		  }
	   CWTraza *m_ptraza = (CWTraza *)GetParent();
	   if (m_ptraza && cambiar)
		  {
		  m_ptraza->ActivaBreak();
		  }
	   }			
	*pResult = 0;
}

void CFonTraza::OnLButtonDblClk(UINT nFlags, CPoint point) 
{	
    
	if (m_linea != -1) 
	  {
	  if (m_buslinea > -1) SetItemState( m_buslinea , 0 , LVIS_DROPHILITED );
	  EnsureVisible( m_linea, TRUE );
	  m_buslinea = m_linea;
	  }
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CFonTraza::Busca(LPCSTR s)
{
int tope = GetItemCount();
int hasta = m_linea;
int i;
CString r;

if (m_buslinea == -1) m_buslinea = m_linea;

SetItemState( m_buslinea , 0 , LVIS_DROPHILITED );

for (i = m_buslinea + 1;i < tope;i++)
   {
   r = GetItemText( i, 1);
   if (r.Find(s) > -1)
      {
	  m_buslinea = i;
	  EnsureVisible( i, TRUE);
	  SetItemState( i , LVIS_DROPHILITED , LVIS_DROPHILITED );
	  return TRUE;
	  }
   }
for (i = 0;i < m_buslinea;i++)
   {
   r = GetItemText( i, 1);
   if (r.Find(s) > -1)
      {
	  m_buslinea = i;
	  EnsureVisible( i, TRUE);
	  SetItemState( i , LVIS_DROPHILITED, LVIS_DROPHILITED );
	  return TRUE;
	  }
   }
return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CTrazaBuscar dialog


CTrazaBuscar::CTrazaBuscar(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CTrazaBuscar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrazaBuscar)
	m_abuscar = _T("");
	//}}AFX_DATA_INIT	
	DLGCONS;
}


void CTrazaBuscar::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrazaBuscar)
	DDX_Text(pDX, IDC_TRAZA_ABUSCAR, m_abuscar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrazaBuscar, CBASEDLG)
	//{{AFX_MSG_MAP(CTrazaBuscar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrazaBuscar message handlers


BOOL CWTraza::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}
