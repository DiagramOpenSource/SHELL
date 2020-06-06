// GridListCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "GridListCtrl1.h"
#include "DlgMata.h"
#include "DlgUsuarios.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridListCtrl

CGridListCtrl::CGridListCtrl()
	{
	m_CurSubItem = -1;	
	}

CGridListCtrl::~CGridListCtrl()
	{
	}


BEGIN_MESSAGE_MAP(CGridListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CGridListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridListCtrl message handlers

BOOL CGridListCtrl::PrepareControl(WORD wStyle)
	{
	m_wStyle = wStyle;
	ASSERT( m_hWnd );
    DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	dwStyle |= LVS_SHOWSELALWAYS;
 
	// Make sure we have report view and send edit label messages.
    SetWindowLong( m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT );

	// Enable the full row selection and the drag drop of headers.
	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP |  LVS_EX_INFOTIP;
	// LVS_EX_CHECKBOXES |
	// Use macro since this is new and not in MFC.
	ListView_SetExtendedListViewStyleEx(m_hWnd, styles, styles );

	return TRUE;
	}



BOOL CGridListCtrl::PositionControl( CWnd * pWnd, int iItem, int iSubItem )
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
		rect.left -= size.cx;
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


void CGridListCtrl::MakeColumnVisible(int nCol)
	{
	if( nCol < 0 )
		return;
	// Get the order array to total the column offset.
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int colcount = pHeader->GetItemCount();
	ASSERT( nCol < colcount );
	int *orderarray = new int[ colcount ];
	Header_GetOrderArray( pHeader->m_hWnd, colcount, orderarray );
	// Get the column offset
	int offset = 0;
	for( int i = 0; orderarray[i] != nCol; i++ )
		offset += GetColumnWidth( orderarray[i] );
	int colwidth = GetColumnWidth( nCol );
	delete[] orderarray;
	
	CRect rect;
	GetItemRect( 0, &rect, LVIR_BOUNDS );
	
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + colwidth + rect.left > rcClient.right )
        {
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
        }
	}




int CGridListCtrl::IndexToOrder( int iIndex )
	{
	// Since the control only provide the OrderToIndex macro,
	// we have to provide the IndexToOrder.  This translates
	// a column index value to a column order value.
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int colcount = pHeader->GetItemCount();
	int *orderarray = new int[ colcount ];
	Header_GetOrderArray( pHeader->m_hWnd, colcount, orderarray );
	int i;
	for( i=0; i<colcount; i++ )
	{
		if( orderarray[i] == iIndex )
		{
			delete orderarray;
			return i;
		}
	}
	delete orderarray;
	return -1;
	}

void CGridListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
	{
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	}

void CGridListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
	{
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	}

void CGridListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	LVHITTESTINFO ht;
	ht.pt = point;
	// Test for which subitem was clicked.
	// Use macro since this is new and not in MFC.
	int rval = ListView_SubItemHitTest( m_hWnd, &ht );

	// Store the old column number and set the new column value.
	int oldsubitem = m_CurSubItem;
	m_CurSubItem = IndexToOrder( ht.iSubItem );

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	// Make the column fully visible.
	// We have to take into account that the columns may be reordered
	MakeColumnVisible( Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem ) );

	// Store old state of the item.
	int state = GetItemState( ht.iItem, LVIS_FOCUSED );

	// Call default left button click is here just before we might bail.	
	CListCtrl::OnLButtonDblClk(nFlags, point);  
	
	if( !state 
		|| m_CurSubItem == -1 
		|| oldsubitem != m_CurSubItem ) return;

	// If we are in column 0 make sure that the user clicked on 
	// the item label.
    if( 0 == ht.iSubItem )
        {
        CDlgMata dlg;
        if (dlg.DoModal() == IDOK)
            {
            if (!dlg.m_clavemata.Compare("HAMPERDIN"))
			{		
              LVITEM Item;
              char buf[512];
              memset(&Item,0,sizeof(LVITEM));
              Item.mask  = LVIF_TEXT;
              Item.iItem = ht.iItem;  
              buf[0] = 0;
              Item.pszText = buf;
              Item.cchTextMax = sizeof(buf)-1;
              GetItem( &Item );
              if (buf[0])         
               {			   
               int matapid = ::atoi(buf);
               if (matapid > 0)
                  {				  
				  CDlgUsuarios *pw = (CDlgUsuarios *)GetParent();
				  if (pw)
				     {
					 pw->m_matapid = matapid;
				     pw->EndDialog(IDOK);
					 }
                  }
               }
			}
		 else
		    {
			AfxMessageBox("Clave Incorrecta");
			}
         }
      }
}



void CGridListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	// This function is called by the control in different 
	// stages during the control drawing process.

	//NMLVCUSTOMDRAW *pCD = (NMLVCUSTOMDRAW*)pNMHDR;
	// By default set the return value to do the default behavior.
	*pResult = CDRF_DODEFAULT;
	}

