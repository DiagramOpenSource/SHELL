
// DsCombo.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsStdDlg.h"
#include "DsEdit.h"
#include "DsCombo.h"
#include "DsxMainFrm.h"
#include "DsxDoc.h"
#include "DsxView.h"
#include "DsStdDlg.h"
#include "DsTabCtrl.h"
#include "TablaView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDsCombo

BEGIN_MESSAGE_MAP(CDSMonthCalCtrl, CMonthCalCtrl)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDsCalendario, CWnd)    
	ON_NOTIFY(MCN_SELECT,6411, OnSelCalendario)
END_MESSAGE_MAP()


void CDsCalendario::OnSelCalendario(NMHDR* pNMHDR, LRESULT* pResult)
{
	/*
	if (m_pedit)
	{
		SYSTEMTIME date;
		CString fecha;
		memset(&date,0,sizeof(date));
		m_cal.GetCurSel( &date  );
		fecha.Format("%02d.%02d.%04d",date.wDay,date.wMonth,date.wYear);
		m_pedit->SetWindowText(fecha);
	}
	*/
}

BOOL CDSMonthCalCtrl::PreTranslateMessage(MSG* pMsg) 
{	
	/*
	CDsCalendario *padre = (CDsCalendario *)GetParent();
	if (padre && padre->m_pedit)
	{
		if (pMsg->message == WM_KEYUP)
		{
			int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
			if (comando) //  == 805 || comando == 804 || comando == 806 || comando == 807 || comando == 802 || comando == 809 || comando == 808
			{            
				return TRUE;
			}
		}
		else if (pMsg->message == WM_KEYDOWN)
		{
			int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
			if (comando)
			{
				padre->m_pedit->TeclaStd(comando);				
				return TRUE;        
			}
			return CBASEMonthCalCtrl::PreTranslateMessage(pMsg);
		}
    }
	*/
	return CBASEMonthCalCtrl::PreTranslateMessage(pMsg);		
}

void CDSMonthCalCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CBASEMonthCalCtrl::OnKillFocus(pNewWnd);
}

BOOL CDsCalendario::PreTranslateMessage(MSG* pMsg) 
{	
	if (m_pedit)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_CANCEL || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB)
			{
				m_pedit->TeclaStd(TECLA_COMBO);
				return TRUE;
			}
		}
		else if (pMsg->message == WM_KEYUP)
		{
			if (pMsg->wParam == VK_CANCEL || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB)
			{				
				return TRUE;
			}
		}
		/*
		if (pMsg->message == WM_KEYUP)
		{
			int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
			if (comando) //  == 805 || comando == 804 || comando == 806 || comando == 807 || comando == 802 || comando == 809 || comando == 808
			{            
				return TRUE;
			}
		}
		else if (pMsg->message == WM_KEYDOWN)
		{
			int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
			if (comando)
			{
				m_pedit->TeclaStd(comando);				
				return TRUE;        
			}
			return CWnd::PreTranslateMessage(pMsg);
		}
		*/
    }
	return CWnd::PreTranslateMessage(pMsg);		
}


CDsComboDropWnd::CDsComboDropWnd() : m_pCombo (NULL)
{
   m_CloseReason = e_KillFocus;
   m_bDropped = FALSE;
}

CDsComboDropWnd::~CDsComboDropWnd()
{
}

BEGIN_MESSAGE_MAP(CDsComboDropWnd, CWnd)    
	//{{AFX_MSG_MAP(CDsComboDropWnd)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_LBN_KILLFOCUS(IDC_DSCOMBO_LIST, OnKillFocusList )
	ON_LBN_SETFOCUS(IDC_DSCOMBO_LIST, OnSetFocusList )
	ON_LBN_DBLCLK(IDC_DSCOMBO_LIST, OnDblClkList )
	ON_LBN_SELCHANGE(IDC_DSCOMBO_LIST, OnSelChangeList )
    ON_WM_VKEYTOITEM()	
END_MESSAGE_MAP()

void CDsComboDropWnd::OnKillFocusList()
{	

	HWND nueva = ::GetFocus();

	if (m_pCombo->m_autocompletar && nueva == m_pCombo->m_Edit.m_hWnd)
		return;

	if (nueva == m_hWnd || (m_pCombo && (m_pCombo->m_lst.m_hWnd == nueva || ::IsChild(m_pCombo->m_lst.m_hWnd,nueva))))
		return;

	BOOL bClickedOnButton = FALSE;
	BOOL bClickedOnEdit =  FALSE;
	if (m_pCombo)
	{
		//Focus will be on the drop down button focus lost because of
		//mouse click to dialog client area. Check cursor position to
		//find where mouse click was.
		bClickedOnButton = m_pCombo->IsMouseOnButton ();
		bClickedOnEdit =  m_pCombo->IsMouseOnEdit ();
			
		ShowWindow (SW_HIDE);	

		//Treat click on dropdown box the same as a selection
		if (bClickedOnButton)
		{
			m_CloseReason = e_Select;
			OnSelection ();
			m_pCombo->SetDroppedState (FALSE);
			//Don't clear the DroppedState flag yet to prevent
			//the mouse click on the button from popping up the tree again.
		}
		else if (bClickedOnEdit)
		{
			m_CloseReason = e_Select;
			OnSelection ();
			m_pCombo->SetDroppedState (FALSE);
		}		
		else
		{
			m_pCombo->SetDroppedState (FALSE);
			m_pCombo->m_Edit.OnKillFocus(CWnd::FromHandle(nueva));
		}
		

		if (m_CloseReason == e_KillFocus)
		{
			//SelectString (m_pCombo->m_BeginPath);
		}				
	}


	
	if (!bClickedOnButton && !bClickedOnEdit && nueva != m_pCombo->m_OtraHwnd && nueva != m_pCombo->m_hWnd && nueva != m_pCombo->m_lst.m_hWnd && nueva != m_pCombo->m_Edit.m_hWnd && nueva != m_hWnd && nueva != m_pCombo->m_DropDownButton.GetSafeHwnd())
	{
	    //m_pCombo->SendParentComboMessage (CBN_KILLFOCUS);
	}
	
}

void CDsComboDropWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	//m_pCombo->SendParentComboMessage (CBN_SETFOCUS);	
    if (m_pCombo && m_pCombo->m_lista && !m_pCombo->m_autocompletar)
	{
		m_pCombo->m_lst.SetFocus();
	}	
}

void CDsComboDropWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
}


void CDsComboDropWnd::OnSelection ()
{	
	if (m_pCombo)
	{
		CString Text;
		if (m_pCombo->m_lista)
		{
			int item = m_pCombo->m_lst.GetCurSel();
			if (item >= 0)
			{
				m_pCombo->m_lst.GetText(item,Text);
				m_pCombo->m_Edit.SetWindowText(Text);
				m_pCombo->SetWindowText(Text);
			}
		}

		//m_pCombo->SendParentComboMessage (CBN_SELENDOK);
		m_CloseReason = e_Select;
	}
	ShowWindow (SW_HIDE);
}

void CDsComboDropWnd::OnSetFocusList()
{

}

void CDsComboDropWnd::OnDblClkList()
{
	OnSelection();
	if (m_pCombo)
	{
		m_pCombo->TeclaStd(802);
	}
}

void CDsComboDropWnd::OnSelChangeList()
{
	if (m_pCombo && !GetCombo()->m_autocompletar)
	{
		int item = m_pCombo->m_lst.GetCurSel();
		if (item >= 0)
		{
			CString s;
			m_pCombo->m_lst.GetText(item,s);
			m_pCombo->m_Edit.SetWindowText(s);
		}
	}
}


int CDsComboDropWnd::OnVKeyToItem( UINT nKey, CListBox* pListBox, UINT nIndex )
{
    if (nKey == 13)
	{
		OnSelection();
		if (m_pCombo)
		{
			m_pCombo->TeclaStd(802);
		}
		return -2;
	}
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CDsComboEdit

CDsComboEdit::CDsComboEdit()
{
}

CDsComboEdit::~CDsComboEdit()
{
}


BEGIN_MESSAGE_MAP(CDsComboEdit, CDsEdit)
	//{{AFX_MSG_MAP(CDsComboEdit)	
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
	//ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)	
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_ALT_DOWN, OnAltDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDsComboEdit message handlers


void CDsComboEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDsEdit::OnLButtonDown(nFlags, point);
	/*
	UNREFERENCED_PARAMETER (point);
	UNREFERENCED_PARAMETER (nFlags);

	CDsCombo* pParent = static_cast<CDsCombo*>(GetParent ());
	_ASSERTE (pParent);
	if (pParent)
	{
		//Drop down the tree control
		CButton& DropDownButton = pParent->GetDropDownButton ();
		pParent->PostMessage (WM_COMMAND, MAKEWPARAM(DropDownButton.GetDlgCtrlID(), BN_CLICKED), (LPARAM) DropDownButton.GetSafeHwnd ());
	}
	*/
}


void CDsComboEdit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDsEdit::OnActivate(nState, pWndOther, bMinimized);
	
	//Invalidate ();	
}

/*
void CDsComboEdit::OnSetfocus() 
{
	// CDsEdit::OnSetfocus();


	//SetSel(0,-1);

	//m_pCombo->SendParentComboMessage (CBN_SETFOCUS);
}
*/


/////////////////////////////////////////////////////////////////////////////
// CDsCombo

CDsCombo::CDsCombo()
{
	m_calendario = NULL;
	m_Drop.SetCombo (this);
	m_ComboWidth = 20;
	m_ComboHeight = 0;
	m_bDroppedState = FALSE;
	m_lista = TRUE;
	m_autocompletar = FALSE;
	m_Creando = FALSE;
	m_OtraHwnd = NULL;	
	m_concombo = FALSE;
	m_desplegar = FALSE;
	m_esrelacion = FALSE;
	m_nbutton = 0;
	m_Esde9 = FALSE;
}

CDsCombo::CDsCombo(BOOL multilinea)
{
	m_calendario = NULL;
	m_Drop.SetCombo (this);
	m_ComboWidth = 20;
	m_ComboHeight = 0;
	m_bDroppedState = FALSE;
	m_lista = TRUE;
	m_autocompletar = FALSE;
	m_Creando = FALSE;
	m_OtraHwnd = NULL;	
	m_Edit.m_multilinea = multilinea;
	m_concombo = FALSE;
	m_desplegar = FALSE;
	m_esrelacion = FALSE;
	m_nbutton = 0;
	m_Esde9 = FALSE;
}

CDsCombo::~CDsCombo()
{
	if (m_calendario)
	{
		if (IsWindow(m_calendario->m_hWnd))
		{
			m_calendario->DestroyWindow();				
		}
		delete m_calendario;
	}
}


BEGIN_MESSAGE_MAP(CDsCombo, CBASEMICOMBO)	
	//{{AFX_MSG_MAP(CDsCombo)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED (IDC_DSCOMBO_BUTTON, OnDropdownButton )
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, DsEditToolTip )
END_MESSAGE_MAP()

void CDsCombo::SetDroppedState (BOOL bState)
{
	m_bDroppedState = bState;
	if (bState == FALSE && m_calendario)
	{
		if (IsWindow(m_calendario->m_hWnd))
		{
			m_calendario->DestroyWindow();
			
		}
		delete m_calendario;
		m_calendario = NULL;
	}
}


BOOL CDsCombo::DsEditToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult) 
{	
	static CString ss;
	if (!theApp.m_tooltips)
		return FALSE;
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
       CDsStdCtrl *pctrl = m_Edit.m_dsctrl;
	   /*
       HWND hwnd = (HWND)nID;
       if (CDsStdDlg::m_hwndctrllst.Lookup(hwnd,pctrl))
       {        
        if (pctrl->m_tooltiptext.IsEmpty())
		{
			/*pTTT->lpszText = "Diagram Software";
			return TRUE; */
        //    return FALSE;
		//}
	   if (pctrl)
	   {
		
		CWnd *wnd = CWnd::FromHandle(pNMHDR->hwndFrom);				
		wnd->ModifyStyle(0,0x40);		
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 200);
		ss = pctrl->m_pdlg->RecogeAyudaPorPosicion();
		ss = pctrl->m_tooltiptext + ss;
        pTTT->lpszText = (char *)(LPCSTR)ss;
		//m_tooltiptext = "Flecha Arriba cambiar de clave\r\nFlecha Abajo cambiar de clave\r\nAv-Pag cambio clave\r\nRe-Pag cambio clave\r\nEnter continuar\r\nEsc o Ctrl-Q Salir\r\nF11 - consultar relacion\r\nCtrl-Flecha Abajo desplegar grid\r\nShift-F11 consultar claves";		   
        return TRUE;        
        }
    }

    /*pTTT->lpszText = "Diagram Software";
    return TRUE; */

    return(FALSE);
}



/////////////////////////////////////////////////////////////////////////////
// CDsCombo message handlers

LRESULT CDsCombo::SendParentComboMessage (UINT Action)
{
	CWnd* pParent = GetParent ();
	if (pParent && pParent->GetSafeHwnd ())
	{
		return pParent->SendMessage (WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(), Action),(LPARAM) GetSafeHwnd ());
	}

	return 0L;
}

void CDsCombo::SetWindowText (LPCTSTR Text,BOOL tipohint)
{	
	if (m_nbutton)
	{
		if (m_nbutton == 1)
		{
			if (!*Text)
			{
				m_button[0].ShowWindow(SW_HIDE);
				CRect rect;
				GetWindowRect(rect);
				GetParent()->ScreenToClient(rect);				
				GetParent()->InvalidateRect(rect);
				return;
			}
			if (!m_button[0].IsWindowVisible())
			{
				m_button[0].ShowWindow(SW_SHOW);
			}
		}
		char *p = strstr(m_Edit.m_qi,Text);
		int sel = -1;
		if (p)
		{
			sel = (int)(p - m_Edit.m_qi);
		}
		for (int i = 0;i < m_nbutton;i++)
		{
			if (i == sel)
			{
				m_button[i].SetCheck(1);
			}
			else
			{
				m_button[i].SetCheck(0);
			}
		}
		return;
	}

	CString so;
	m_Edit.GetWindowText(so);
	if (!so.Compare(Text))
	{
		return;
	}
	if (tipohint)
	{
       int startPos, endPos;
	   if (::GetFocus() == m_Edit.m_hWnd)
	   {
		 m_Edit.GetSel(startPos, endPos);	   
		 int n = strlen(so);
		 if (!memcmp(so,Text,n))
		 {
			m_Edit.SetWindowText(Text);
			if (startPos == endPos)
			{
				startPos = n;
				endPos = strlen(Text);
			}
			m_Edit.SetSel(startPos, endPos+1);
		 }
	   }
	}
	else
	{
	   m_Edit.SetWindowText (Text);
	}
	CBASEMICOMBO::SetWindowText (Text);
	if (m_lista && ::IsWindow(m_lst.m_hWnd))
	{
		int sel = m_lst.FindString(-1,Text);
		m_lst.SetCurSel(sel);		
	}
	//Invalidate ();
}

CString CDsCombo::GetWindowText ()
{
	CString Text;
	if (m_nbutton)
	{		
		Text = "";
		for (int i = 0;i < m_nbutton;i++)
		{
			if (m_button[i].GetCheck() == 1)
			{
				Text = m_Edit.m_qi[i];
			}
		}
		if (m_nbutton == 1 && Text.IsEmpty())
		{
			Text = m_Edit.m_qi[1];
		}
	}
	else
	    m_Edit.GetWindowText(Text);
	return Text;
}

void CDsCombo::GetWindowText (CString &Text)
{		
	if (m_nbutton)
	{
		Text = "";
		for (int i = 0;i < m_nbutton;i++)
		{
			if (m_button[i].GetCheck() == 1)
			{
				Text = m_Edit.m_qi[i];
			}
		}
		if (m_nbutton == 1 && Text.IsEmpty())
		{
			Text = m_Edit.m_qi[1];
		}
	}
	else
		m_Edit.GetWindowText(Text);
}

int CDsCombo::GetWindowText( LPTSTR lpszStringBuf, int nMaxCount ) 
{		
	if (m_nbutton)
	{
		lpszStringBuf[0] = 0;
		for (int i = 0;i < m_nbutton;i++)
		{
			if (m_button[i].GetCheck() == 1)
			{
				lpszStringBuf[0] = m_Edit.m_qi[i];
			}
		}
		if (m_nbutton == 1 && !lpszStringBuf[0])
		{
			lpszStringBuf[0] = m_Edit.m_qi[1];
		}		 
		lpszStringBuf[1] = 0;
	    return 1;
	}
	else
	    return m_Edit.GetWindowText( lpszStringBuf, nMaxCount );
}

int CDsCombo::GetWindowTextLength( )
{
	if (m_nbutton)
	  return 1;
	return m_Edit.GetWindowTextLength( );
}

void CDsCombo::UnDrop()
{
	SetDroppedState (FALSE);
	if (m_esrelacion)
	{
		if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
		{
			theApp.m_ctrlactivo->TeclaStd(806);
		}
	}
	else
	{
		m_Drop.OnSelection();		
	}
}

void CDsCombo::OnDropdownButton ()
{	
    if (GetDroppedState ())
	{
		UnDrop();
	}
	else
	{		
		if (!(m_editstyle & ES_READONLY))
		{		
			if (theApp.m_ctrlactivo == m_Edit.m_dsctrl)
			{
				DisplayDrop();
			}
			else
			{			
				//((BASECDsEdit *)&m_Edit)->SetFocus();
				m_desplegar = TRUE;
			}
		}
		else
		{
			::MessageBox(NULL,"Campo no Editable","Atencion!",MB_OK|MB_ICONHAND);
		}
	}

	if (::IsWindow(m_Edit.m_hWnd))
	{
		if (!GetDroppedState())
		{
			((BASECDsEdit *)&m_Edit)->SetFocus();
		}
		/*
		else
		{
			m_Edit.SetSel(0,0);
		}
		*/
	}
	//m_Edit.Invalidate ();
}


void CDsCombo::CalculateDroppedRect(LPRECT lpDroppedRect)
{
	_ASSERTE (lpDroppedRect);

	if (!lpDroppedRect)
	{
		return;
	}

	CRect rectCombo;
	GetWindowRect(&rectCombo);

	//adjust to either the top or bottom
	int DropTop = rectCombo.bottom;
	int ScreenHeight = GetSystemMetrics (SM_CYSCREEN);
	if ((DropTop + m_ComboHeight) > ScreenHeight)
	{
		DropTop = rectCombo.top - m_ComboHeight;
	}

	//adjust to either the right or left
	int DropLeft = rectCombo.left;
	int ScreenWidth = GetSystemMetrics (SM_CXSCREEN);
	if ((DropLeft + m_ComboWidth) > ScreenWidth)
	{
		DropLeft = rectCombo.right - m_ComboWidth;
	}

	lpDroppedRect->left  = DropLeft;
	lpDroppedRect->top   = DropTop;
	lpDroppedRect->bottom = DropTop + m_ComboHeight;
	lpDroppedRect->right  = DropLeft + m_ComboWidth;
    
}


void CDsCombo::DisplayDrop()
{
	SetDroppedState (TRUE);
	m_desplegar = FALSE;
	if (m_Edit.m_tipo == 2)
	{
		CRect rect;
		CRect rect2;
		CPoint point;
		if (m_calendario)
		{
			if (IsWindow(m_calendario->m_hWnd))
			{
				m_calendario->DestroyWindow();				
			}
			delete m_calendario;
		}
		
		GetWindowRect(&rect);

		point.x = 0;
		point.y = 0;		

		rect.top = rect.bottom;
#ifdef CALENDARIOPROUIS
		rect.bottom += 200;
		rect.right += 200;
#else
		rect.bottom += 150;
		rect.right += 50;
#endif
		
#ifdef DIALOGOCALENDARIO
		m_calendario = new CDsCalendar();
		m_calendario->Create(CDsCalendar::IDD);
		m_calendario->m_pedit = (CWnd *)&m_Edit;
#else

		m_calendario = new CDsCalendario();
		m_calendario->m_pedit = &m_Edit;
		m_calendario->CreateEx(WS_EX_TOPMOST,AfxRegisterWndClass(NULL),"Calendario",WS_POPUP,rect,pMFrame,NULL);
#endif
#ifdef CALENDARIOPROUIS
/*
	virtual bool Create(
		CWnd * pParentWnd,
		const RECT & rcWnd = CRect( 0, 0, 0, 0 ),
		UINT nDlgCtrlID = UINT( IDC_STATIC ),
		CSize sizeCalendarDimensionsMin = CSize(1,1),
		CSize sizeCalendarDimensionsMax = CSize(1,1),
		DWORD dwWindowStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
		DWORD dwDatePickerStyle = __EDPWS_DEFAULT,
		CCreateContext * pContext = NULL
		);
*/
		if (!m_calendario->m_cal.Create(m_calendario,CRect(0,0,200,200),6411))
		{
			ASSERT(FALSE);
		}
#else
#ifndef DIALOGOCALENDARIO
		m_calendario->m_cal.Create(WS_BORDER,point,m_calendario,6411);
#endif
#endif

#ifndef DIALOGOCALENDARIO
		m_calendario->SetOwner(this);		
#endif

		



		/*MCN_SELECT MCN_SELCHANGE */

		CString fecha;
		m_Edit.GetWindowText(fecha);
		SYSTEMTIME date;
		memset(&date,0,sizeof(date));
		date.wYear = atoi(fecha.Mid(6,4));
		date.wMonth = atoi(fecha.Mid(3,2));
		date.wDay = atoi(fecha.Mid(0,2));

#ifdef CALENDARIOPROUIS
		{
			COleDateTime odate(date);
		    m_calendario->m_cal.CurrentDateSet(odate);
		}
		m_calendario->ShowWindow(SW_SHOW);
		m_calendario->m_cal.UpdateDatePickerWnd(true);
#else
		m_calendario->m_cal.SetCurSel( &date );
#ifdef DIALOGOCALENDARIO
		
		m_calendario->GetWindowRect(&rect2);
		m_calendario->MoveWindow(rect.left,rect.top,rect2.Size().cx,rect2.Size().cy);
#else		
		m_calendario->m_cal.ShowWindow(SW_SHOW);
#endif
		m_calendario->ShowWindow(SW_SHOW);
#ifdef DIALOGOCALENDARIO
		m_calendario->GetWindowRect(&rect);				
		m_calendario->m_ctrlok.SetFocus();
#else
		m_calendario->m_cal.GetWindowRect(&rect);				
#endif
		pMFrame->GetWindowRect(&rect2);
		if (rect2.bottom < rect.bottom)
		{
			rect.OffsetRect(0,rect2.bottom-rect.bottom);
		}
		if (rect2.right < rect.right)
		{
			rect.OffsetRect(rect2.right-rect.right,0);
		}
		m_calendario->MoveWindow(rect);	
#endif

	}
	else if (m_esrelacion)
	{
		if (theApp.m_ulttecla == 999) /* OJO */
		{
			theApp.m_ulttecla = -1;
		}
       m_Edit.TeclaStd(TECLA_COMBO);
	} else if (m_lista)
	{
		CString s;
		if (!m_autocompletar)
		{
			m_lst.SetFocus();
			m_Edit.GetWindowText(s);
			int sel = m_lst.FindString(-1,s);
			m_lst.SetCurSel(sel);
		}
		
		CRect rect(0,0,200,200);

		CalculateDroppedRect (&rect);

		m_Drop.InitDisplay ();	

		m_Drop.SetWindowPos (&wndNoTopMost, rect.left, rect.top,
							 rect.Width (), rect.Height (), 0 );

		//::SetWindowPos(m_Drop.m_hWnd,(HWND)HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);           		
		m_Drop.GetClientRect(rect);
		m_lst.MoveWindow(rect);
		m_lst.GetWindowRect(rect);                
		m_Drop.MoveWindow(rect);
		m_Drop.ShowWindow(SW_SHOW);
		m_Drop.BringWindowToTop();		

		if (m_autocompletar)
		{
			((BASECDsEdit *)&m_Edit)->SetFocus();
		}
	}		
}


void CDsCombo::OnCancel()
{
	//restore inital path
	//m_Drop.SelectString (m_BeginPath);
	//SendParentComboMessage (CBN_SELENDCANCEL);
}

BOOL CDsCombo::CreateDrop(DWORD extra)
{	
	CWnd* pParent = GetParent ();
	if (!pParent && pParent->GetSafeHwnd ())
	{
		return FALSE;
	}

	if (!m_lista)
	{
		m_ComboWidth = 0;
		m_ComboHeight = 0;
	}
	else
	{
		CRect r;
		GetWindowRect(&r);
		m_ComboWidth  = r.Size().cx;
		m_ComboHeight = r.Size().cy;
	}

	CRect treeRect (0,0, m_ComboWidth, m_ComboHeight);

	//can't have a control Id with WS_POPUP style
    if (! m_Drop.CreateEx ( 0,
		                   AfxRegisterWndClass(NULL), NULL,
						   	WS_POPUP | WS_BORDER | WS_CLIPCHILDREN/*| TVS_SINGLEEXPAND | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS*/,
							treeRect, pParent, 0, NULL))
	{
		return FALSE;
	}

	CRect lrect;

	m_Drop.GetClientRect(lrect);
	m_Drop.SetFont(m_Edit.GetFont ());

	if (m_lista)
	{
		m_lst.Create(extra | WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | LBS_HASSTRINGS | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_USETABSTOPS,lrect,&m_Drop,IDC_DSCOMBO_LIST);
		m_lst.SetFont(m_Edit.GetFont ());
	}

	return TRUE;
}


BOOL CDsCombo::CreateButton ()
{	
	CRect btnRect;	

//Get the bitmap size and add border space	
	CBitmap bitmapDown;
	CDC* pDC = GetDC();
	CDC tempDC;
	BITMAP bm;

	bitmapDown.LoadBitmap (_T("CBU"));
    tempDC.CreateCompatibleDC (pDC);
	CBitmap* pOldBitmap = tempDC.SelectObject (&bitmapDown);
	bitmapDown.GetObject (sizeof(BITMAP), (LPSTR)&bm);
	tempDC.SelectObject (pOldBitmap);

	GetWindowRect (&btnRect);
	btnRect.right += bm.bmWidth;
	GetParent()->ScreenToClient(&btnRect);
	CBASEMICOMBO::MoveWindow( &btnRect );

	GetClientRect (&btnRect);

	btnRect.left = btnRect.right - bm.bmWidth;
	
	
     m_DropDownButton.CreateEx( 0, 
      _T("BUTTON"), NULL,
      BS_PUSHBUTTON | BS_NOTIFY | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
      btnRect, this, IDC_DSCOMBO_BUTTON);

	m_DropDownButton.LoadBitmaps (_T("CBU"), _T("CBD"));

	m_DropDownButton.MoveWindow (&btnRect);
	//m_DropDownButton.SizeToContent();

	//m_Edit.m_otrofoco = m_DropDownButton.m_hWnd;

	return TRUE;
}


BOOL CDsCombo::CreateEdit ()
{	
	CRect editRect;
	GetClientRect (&editRect);

	m_Edit.SetCombo(this);
	
	BOOL bCreate = m_Edit.Create(m_editstyle | WS_VISIBLE | WS_CHILD|WS_TABSTOP,  editRect, this, IDC_DSCOMBO_EDIT);

	if (bCreate)
	{
		CWnd* pParent = GetParent ();
		CFont* pFont = pParent->GetFont ();
		m_Edit.SetFont (pFont);
	}

	return bCreate;
}

//Removes the original ComboBox control instead of subclassing it
BOOL CDsCombo::SubclassDlgItem (UINT nID, CWnd* pParent)
{
	CComboBox* pCombo = static_cast<CComboBox*>(pParent->GetDlgItem (nID));
	if (!pCombo)
	{
		return FALSE;
	}

	CRect DroppedRect;
	pCombo->GetDroppedControlRect (&DroppedRect);
	m_ComboWidth = DroppedRect.Width ();
	m_ComboHeight = DroppedRect.Height ();


 	CRect rect;
	pCombo->GetWindowRect (&rect);
	pParent->ScreenToClient (&rect);

   CString className = AfxRegisterWndClass(   CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
						::LoadCursor(NULL, IDC_ARROW),
						(HBRUSH) ::GetStockObject(WHITE_BRUSH));


   BOOL bCreate =  CreateEx(WS_EX_CLIENTEDGE, // 3D  client edge 
		 className, NULL,
		 WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		 rect, pParent, nID);

	_ASSERTE (bCreate);

	if (!bCreate)
	{
		return FALSE;
	}

	//Set Z Order to follow after original combo
	SetWindowPos (pCombo, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	UpdateWindow ();

	pCombo->DestroyWindow (); //remove original window

	return TRUE; 
}

int CDsCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBASEMICOMBO::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Creando = TRUE;

	m_Drop.m_hWnd = NULL;
	m_DropDownButton.m_hWnd = NULL;
	m_lista = FALSE;

	if (m_nbutton)
	{
		CRect r;
		GetClientRect(r);
		if (m_nbutton == 1)
		{
			m_button[0].Create("",BS_CHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP,r,this,IDC_DSCOMBO_EDIT);
		}
		else
		{
			int h = r.Size().cy;			
			for (int i = 0;i < m_nbutton;i++)
			{
				r.bottom = r.top + h;
				m_button[i].Create("",BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,r,this,IDC_DSCOMBO_EDIT);
				r.top += h;
			}
		}
		if ((m_editstyle & ES_READONLY))
		{
			for (int i = 0;i < m_nbutton;i++)
			{
				m_button[i].EnableWindow(FALSE);
			}
		}
		//AjustaBotonRect();
	}
	else
	{
		if (!CreateEdit())
		{
			return -1;
		}	
	}

	m_Creando = FALSE;
	EnableToolTips(TRUE);

	return 0;
}

void CDsCombo::ShowDropDown( BOOL bShowIt)
{
	if (bShowIt)
	{
		if (!GetDroppedState ())
		{
			DisplayDrop ();
		}
	}
	else if (!GetDroppedState ())
	{
		m_Drop.ShowWindow (SW_HIDE);
		SetDroppedState (FALSE);
	}
}


int CDsCombo::SetDroppedWidth( UINT nWidth )
{
	int retval = 0;
	if (nWidth >= 0)
	{
		m_ComboWidth = nWidth;
		if (GetDroppedState ())
		{
			DisplayDrop ();
		}
	}
	else
	{
		retval = CB_ERR;
	}
	return retval;
}

int CDsCombo::GetDroppedWidth( )
{
	return m_ComboWidth;
}

int CDsCombo::SetDroppedHeight (UINT nHeight)
{
	int retval = 0;
	if (nHeight >= 0)
	{
		m_ComboHeight = nHeight;
		if (GetDroppedState ())
		{
			DisplayDrop ();
		}
	}
	else
	{
		retval = CB_ERR;
	}
	return retval;
}

int CDsCombo::GetDroppedHeight ()
{
	return m_ComboHeight;
}

BOOL CDsCombo::GetDroppedState( )
{
	return m_bDroppedState;
}

void CDsCombo::GetDroppedControlRect (LPRECT pRect)
{
	_ASSERTE (pRect);
	if (pRect)
	{
		CalculateDroppedRect (pRect);
	}
}

void CDsCombo::OnSetFocus(CWnd* pOldWnd) 
{
	CBASEMICOMBO::OnSetFocus(pOldWnd);	

    HWND oldw = NULL;
    if (pOldWnd)
        oldw = pOldWnd->m_hWnd;
	if (!theApp.m_hwnd_999)
		theApp.m_hwnd_999 = (int)oldw;

	if (!m_nbutton)
	{
	    ((BASECDsEdit *)&m_Edit)->SetFocus();
	}
	else
	{
		m_button[0].SetFocus();
	}
}

BOOL CDsCombo::IsMouseOnButton() 
{
	BOOL bMouseOn = FALSE;
	CPoint point;
	if (m_DropDownButton.m_hWnd && GetCursorPos (&point))
	{
		CRect rect;
		m_DropDownButton.GetWindowRect (&rect);
		if (rect.PtInRect (point))
		{
			bMouseOn = TRUE;
		}
	}
	return bMouseOn;
}

BOOL CDsCombo::IsMouseOnEdit() 
{
	BOOL bMouseOn = FALSE;
	if (m_nbutton)
		return FALSE;
	CPoint point;
	if (GetCursorPos (&point))
	{
		CRect rect;
		m_Edit.GetWindowRect (&rect);
		if (rect.PtInRect (point))
		{
			bMouseOn = TRUE;
		}
	}
	return bMouseOn;
}

BOOL CDsCombo::IsEditHighlightOn () 
{
	if (m_nbutton)
		return FALSE;
	BOOL bHighlightOn = FALSE;
	HWND hFocus = ::GetFocus ();

	if (hFocus == GetSafeHwnd () || 
		hFocus == m_Edit.GetSafeHwnd  () ||
		(m_DropDownButton.m_hWnd && hFocus == m_DropDownButton.GetSafeHwnd  ()))
	{
		bHighlightOn = TRUE;
	}

	return bHighlightOn;
}


void CDsCombo::OnKillFocus(CWnd* pNewWnd) 
{
	HWND nueva = NULL;
	if (pNewWnd)
	{
		nueva = pNewWnd->m_hWnd;
	}
	CBASEMICOMBO::OnKillFocus(pNewWnd);
	
	if (m_nbutton)
		return;

	//m_Edit.Invalidate ();
}


void CDsCombo::SetFont( CFont* pFont, BOOL bRedraw )
{
	CBASEMICOMBO::SetFont(pFont, bRedraw);
	if (m_nbutton)
		return;
	m_Edit.SetFont( pFont, bRedraw );
	if (m_Drop.m_hWnd)
	{
		m_Drop.SetFont( pFont, bRedraw );
	}
	if (m_lista)
	{
		m_lst.SetFont( pFont, bRedraw );
	}	
}


BOOL CDsCombo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (!m_nbutton)
	{
		if (HIWORD(wParam) == BN_KILLFOCUS	)
		{
			m_Edit.Invalidate ();
		}
	}
    if ((wParam & 0xffff) == ID_CUSTOM_ESTADO)
    {
		if (lParam == 1002)
		{
		   if (m_concombo)
		   {
				if (GetDroppedState ())
				{
					SetDroppedState (FALSE);		
					if (!m_esrelacion)
					{
						m_Drop.OnSelection();		
					}
				}
		   }
		   
           m_Edit.m_noConfirma = TRUE;
		   m_Edit.SetFocus();
		   m_Edit.m_noConfirma = FALSE;
		   
		} 
		return TRUE;
	} else if ((wParam&0xffff) >= 800 && (wParam&0xffff) <= 999)
    {
        m_Edit.TeclaStd((wParam&0xffff));
        return TRUE;
    }
    else if ((wParam&0xffff) == ID_PTECS)
    {
        int *p1 = (int *)lParam;
        m_Edit.ProcesaPtec(*p1);
        return TRUE;
    }    
	if ((wParam&0xffff) == ID_EDIT_COPY || (wParam&0xffff) == ID_EDIT_CUT || (wParam&0xffff) == ID_EDIT_PASTE || (wParam&0xffff) == ID_EDIT_UNDO)
	{
		m_Edit.SendMessage(WM_COMMAND,wParam,lParam);		
		return TRUE;
	}
	return CBASEMICOMBO::OnCommand(wParam, lParam);
}


int CDsCombo::AddString ( LPCTSTR lpszString , int maxl)
{	
    if (m_lista)
	{
		int item = m_lst.AddString (lpszString);
		int nl;
		if (item >= 0 && (nl = m_lst.GetCount()) > 1)
		{
			if (maxl && nl < maxl)
				m_ComboHeight += ((m_ComboHeight-2) / (m_lst.GetCount()-1));
		}
		return item;
	}
	else
		return -1;
}


int CDsCombo::SelectString( int item, LPCTSTR lpszString)
{
    if (m_lista)
	{
		item = m_lst.SelectString( item, lpszString);
		if (item >= 0)
		{
			CString  String;
			m_lst.GetText( item, String );
			m_Edit.SetWindowText(String);
		}
	}
	return item;
}

void CDsCombo::ResetContent ()
{
    if (m_lista)
	{
		m_ComboHeight = m_iniComboHeight;
		m_lst.ResetContent();	
	}
}

BOOL CDsCombo::TeclaStd(int tecla)
{
	return m_Edit.TeclaStd(tecla);
}


void CDsComboEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (nChar == VK_DOWN && (::GetKeyState( VK_CONTROL ) & 0x8000))
	{
		m_pCombo->OnDropdownButton();
	}
	else
	{
	    CDsEdit::OnKeyDown(nChar, nRepCnt, nFlags);	
	}
}

void CDsComboEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    CDsEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CDsComboEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDsEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDsComboEdit::Copia(CDsEdit *pEdit)
{
	m_multilinea = pEdit->m_multilinea;
    m_maxqc = pEdit->m_maxqc;
    m_qc = pEdit->m_qc;
    m_qh = pEdit->m_qh;
    m_tipo = pEdit->m_tipo;
    m_decimales = pEdit->m_decimales;
    m_flag = pEdit->m_flag;
    m_overwrite = pEdit->m_overwrite;
    m_desactivado = pEdit->m_desactivado;
    memcpy(m_qs,pEdit->m_qs,sizeof(m_qs));
	memcpy(m_qi,pEdit->m_qi,sizeof(m_qi));
    m_dsctrl = pEdit->m_dsctrl;
    m_pView = pEdit->m_pView;
    m_pDoc = pEdit->m_pDoc;
    /*m_metacombo = NULL;*/
    m_entabla = pEdit->m_entabla;
    m_estilo = pEdit->m_estilo;
}


void CDsComboEdit::ConfirmaFoco()
{
	CDsEdit::ConfirmaFoco();
	if (m_pCombo->m_desplegar)
	{
		if (!m_pCombo->GetDroppedState ())
		{
			if (theApp.m_ctrlactivo == m_dsctrl)
			{
				m_pCombo->DisplayDrop();
			}				
		}
		m_pCombo->m_desplegar = FALSE;
	}
}

void CDsComboEdit::OnKillFocus(CWnd* pNewWnd)
{
	if (pNewWnd && m_pCombo && (pNewWnd->m_hWnd == m_pCombo->m_lst.m_hWnd || pNewWnd->m_hWnd == m_pCombo->m_Drop.m_hWnd || m_pCombo->IsMouseOnButton() || (m_pCombo->m_calendario && (pNewWnd->m_hWnd == m_pCombo->m_calendario->m_hWnd  || m_pCombo->m_calendario->IsChild(pNewWnd)))))
	{
		return;
	}
	CDsEdit::OnKillFocus(pNewWnd);
}

void CDsComboEdit::OnSetFocus(CWnd* pOldWnd)
{
	CDsEdit::OnSetFocus(pOldWnd);	
}

void CDsCombo::SetFlag(int flag)
{
	m_Edit.m_flag = (flag & 0xffff);
}

int CDsCombo::GetFlag()
{
	return m_Edit.m_flag;
}

void CDsCombo::SetSeleccionado(BOOL sel)
{
    m_Edit.m_seleccionado = sel;
}

int CDsCombo::ProcesaPtec(int ptec)
{
	return m_Edit.ProcesaPtec(ptec);
}

int CDsCombo::GetTipo()
{
	return m_Edit.m_tipo;
}

void CDsCombo::SetView(CDsxView *pView)
{
    m_Edit.m_pView = pView;
}

void CDsCombo::SetDoc(CDsxDoc *pDoc)
{
    m_Edit.m_pDoc = pDoc;
}

void CDsCombo::SetDsCtrl(CDsStdCtrl *pctrl)
{
	m_Edit.m_dsctrl = pctrl;
}

void CDsCombo::SetBkBrush(CBrush *pbrush)
{
	m_Edit.m_pBkBrush = pbrush;
}

CBrush * CDsCombo::GetBkBrush()
{
	return m_Edit.m_pBkBrush;
}

void CDsCombo::SetBkColor(COLORREF color)
{
	m_Edit.m_BkColor = color;

	m_Edit.SetBkColor(color);
	
}

void CDsCombo::SetFrColor(COLORREF color)
{
	m_Edit.m_FrColor = color;
	m_Edit.SetTextColor(color);
}

void CDsCombo::Setmaxqc(int maxqc)
{
	m_Edit.m_maxqc = maxqc;
}

void CDsCombo::Setqc(int qc)
{
	m_Edit.m_qc = qc;
}

void CDsCombo::Settipo(int tipo)
{
	m_Edit.m_tipo = tipo;
}

void CDsCombo::Setqh(int qh)
{
	m_Edit.m_qh = qh;
}

void CDsCombo::Setdecimales(int deci)
{
	m_Edit.m_decimales = deci;
}

void CDsCombo::Setqi(const char *qi)
{
	strcpy(m_Edit.m_qi,qi);
}

void CDsCombo::Setqs(const char *qs)
{		
	strcpy(m_Edit.m_qs,qs);
	char *p = strstr(m_Edit.m_qs,"\010\010");
	if (p)
	{
		*p = 0;
		m_Edit.m_comborel = TRUE;
	}
	else
		m_Edit.m_comborel = FALSE;
}


char * CDsCombo::Getqs()
{
	return m_Edit.m_qs;
}

int CDsCombo::Getqc()
{
	return m_Edit.m_qc;
}


void CDsCombo::SetTabla(CTablaView *ptabla)
{
	m_Edit.m_entabla = ptabla;
}

BOOL CDsCombo::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	static CString clase;
	m_editstyle = dwStyle;
    m_Edit.m_estilo = dwStyle & ~(WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE);
	DWORD estilo = (dwStyle & ~WS_CHILD) | (dwStyle & ~WS_VISIBLE) | WS_CLIPCHILDREN;

	if (clase.IsEmpty())
	{
		clase = AfxRegisterWndClass(NULL);
	}
	DWORD exs = 0;
	CRect r1 = rect;
	if (!m_Edit.m_entabla && m_Edit.m_tipo == 1 && m_Edit.m_maxqc == 1 && ::strlen(m_Edit.m_qi) && strstr(m_Edit.m_qs,"\""))
	{
		if (!strcmp(m_Edit.m_qi,"SN") || !strcmp(m_Edit.m_qi,"SNsn"))
		{
			m_nbutton = 1;
		}
		else
		{
			m_nbutton = ::strlen(m_Edit.m_qi);
			int h = r1.Size().cy;
			r1.bottom = r1.top + h * m_nbutton;
		}
	}

	if (m_nbutton)
	{
		exs |= WS_EX_TRANSPARENT;
		estilo &= ~WS_DLGFRAME;
		estilo &= ~WS_BORDER;
	}
	BOOL r = CreateEx( exs, clase,"",estilo,r1,pParentWnd,nID);

	if (r)
	{
		CWnd* pParent = GetParent ();
		CFont* pFont = pParent->GetFont ();
		if (!m_nbutton)
			m_Edit.SetFont (pFont);
	}
    if (r && m_Edit.m_entabla)
    {
       m_Edit.m_entabla->m_editando = TRUE;       
       m_Edit.m_entabla->m_dsedit = m_Edit.GetCombo();
    }
    return r;
}

void CDsCombo::SetMargins(UINT nleft, UINT nRight)
{
   if (!m_nbutton)
	  m_Edit.SetMargins(nleft,nRight);
}

void CDsCombo::SetLimitText(UINT nMax)
{
   if (!m_nbutton)
 	 m_Edit.SetLimitText(nMax);
}

int CDsCombo::Getmaxqc()
{
	return m_Edit.m_maxqc;
}

BOOL CDsCombo::AjustaTamano()
{
   if (m_Edit.m_dsctrl->m_tipo != 2) return FALSE;

   BOOL ajustado = FALSE;
   int minimo = m_Edit.m_qc;
   CRect rect;   
   
   GetWindowRect(rect);
   m_Edit.m_dsctrl->m_pdlg->ScreenToClient(rect);

   if (minimo < 4)
   {
       char minimun[300];

       if (m_Edit.m_tipo == 3 || m_Edit.m_tipo == 2)
            minimun[0] = '0';
       else
            minimun[0] = 'X';
       memset(minimun+1,'0',minimo-1);
       minimun[minimo] = 0;

       CDC *pDC = GetDC();       
       pDC->SelectObject( m_Edit.m_pDoc->m_cfEdit );
       CSize s = pDC->GetTextExtent(minimun,minimo);
       s.cx += (::GetSystemMetrics(SM_CXEDGE)*3);
       if (s.cx > rect.Size().cx)
       {
           rect.right = rect.left + s.cx;
           MoveWindow(rect);
       }       

       ReleaseDC(pDC);
       ajustado = TRUE;
   }
   else
   if (m_Edit.m_tipo == 2 /*|| m_tipo == 3*/)
   {
       char minimun[300];
       memset(minimun,'0',minimo);
       minimun[minimo] = 0;
       CDC *pDC = GetDC();       
       pDC->SelectObject( m_Edit.m_pDoc->m_cfEdit );
       CSize s = pDC->GetTextExtent(minimun,minimo);              
       ReleaseDC(pDC);
       s.cx += (::GetSystemMetrics(SM_CXEDGE)*2);
       if (s.cx < rect.Size().cx)
       {
           rect.right = rect.left + s.cx;
           MoveWindow(rect);
       }
       ajustado = TRUE;
   }   

   return ajustado;
}

void CDsCombo::MoveWindow( LPCRECT lpRect, BOOL bRepaint)
{
   CBASEMICOMBO::MoveWindow( lpRect, bRepaint);
   AjustaEditRect();
}

BOOL CDsCombo::ModifyStyleEx( DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{		
	if (m_nbutton /* || (m_editstyle & ES_READONLY)*/)
	{		
		dwAdd &= ~WS_EX_DLGMODALFRAME;
		dwAdd &= ~WS_EX_CLIENTEDGE;
		dwAdd &= ~WS_EX_WINDOWEDGE;
	}
   BOOL r = CBASEMICOMBO::ModifyStyleEx( dwRemove, dwAdd, nFlags);
   AjustaEditRect();
   return r;
}


DWORD CDsCombo::Getestilo()
{
	return m_Edit.m_estilo;
}

void CDsCombo::Setestilo(DWORD estilo)
{
	m_Edit.m_estilo = estilo;
}

BOOL CDsCombo::SetReadOnly(BOOL bReadOnly)
{
	if (bReadOnly)
	{
		m_editstyle |= ES_READONLY;
	}
	else
	{
		m_editstyle &= ~ES_READONLY;
	}
	if (m_nbutton)
	{
		for (int sel = 0;sel < m_nbutton;sel++)
		{
			if (bReadOnly)
			{
				m_button[sel].EnableWindow(FALSE);
			}
			else
			{
				m_button[sel].EnableWindow(TRUE);
			}
		}		
		return !bReadOnly;
	}
	BOOL r = m_Edit.SetReadOnly(bReadOnly);
	
	if (EsPosicional() && m_Edit.m_tipo == 1 && m_Edit.m_maxqc == 1 && ::strlen(m_Edit.m_qi))
		AjustaCombo();

	return r;
}

void CDsCombo::DeleteBkBrush()
{
    if (m_Edit.m_pBkBrush)
    {
        delete m_Edit.m_pBkBrush;
        m_Edit.m_pBkBrush = NULL;
    }
}

void CDsCombo::AjustaCombo()
{
	if (m_nbutton /*|| (m_editstyle & ES_READONLY)*/)
		return;
    if (m_Edit.m_tipo == 2 /* || (m_Edit.m_tipo == 1 && m_Edit.m_maxqc == 1 && ::strlen(m_Edit.m_qi) && (!(m_Edit.GetStyle()&ES_READONLY) || !EsPosicional()))*/)
    {		
        CString s;
        //GetWindowText(s);
        char *pp;
		if (!m_concombo)
		{
		   m_concombo = TRUE;
		   if (m_Edit.m_tipo == 1)
			  m_lista = TRUE;
		   else
			  m_lista = FALSE;
           CreateButton();
		   CreateDrop();		   
        }
        else
        {           
           ResetContent( );
		   CRect r;
		   GetWindowRect(&r);
		   m_ComboWidth  = r.Size().cx;
		   m_ComboHeight = r.Size().cy;
        }
		if (m_Edit.m_tipo == 1)
		{
			char tmp[2];
			tmp[1] = 0;
			for (pp = m_Edit.m_qi;*pp;pp++)
			{
			   tmp[0] = *pp;
			   AddString( tmp );
			}		
			m_Edit.GetWindowText(s);
			int sel = m_lst.FindString(-1,s);
			m_lst.SetCurSel(sel);        
		}
    }	
	else if (m_Edit.m_comborel || ( m_Esde9 && (LadoActivo(1) || LadoActivo(2)) )  )
	{
		if (!m_concombo)
		{
		   m_esrelacion = TRUE;
		   m_concombo = TRUE;
		   m_lista = FALSE;
           CreateButton();
		   CreateDrop();		   
        }
	}
	else
	{
		if (m_concombo)
		{
		    m_esrelacion = FALSE;
		    m_concombo = FALSE;
		    m_lista = FALSE;
			if (::IsWindow(m_lst.m_hWnd))
			{
				m_lst.DestroyWindow();
			}
			if (::IsWindow(m_Drop.m_hWnd))
			{
				m_Drop.DestroyWindow();
			}
			if (::IsWindow(m_DropDownButton.m_hWnd))
			{
				CRect r1,r2;
				m_DropDownButton.GetWindowRect(r1);
				GetWindowRect(r2);
				r2.right -= r1.Size().cx;
				m_DropDownButton.DestroyWindow();
				GetParent()->ScreenToClient(r2);
				MoveWindow(r2);
			}
		}
	}
}

void CDsCombo::AjustaEditRect()
{
   CRect rect;
   GetClientRect (rect);
   if (m_nbutton)
   {
	   AjustaBotonRect();
       return;
   }
   if (m_concombo)
   {
	   CRect r1;
	   m_DropDownButton.GetWindowRect(&r1);
	   rect.right -= r1.Size().cy;
   }
   m_Edit.MoveWindow(rect);
}

CWnd* CDsCombo::SetFocus( )
{	
	CWnd *cwnd = GetFocus();
	if (cwnd && cwnd->m_hWnd == m_lst.m_hWnd)
	{
		return cwnd;
	}
	if (m_nbutton)
	{
		for (int sel = 0;sel < m_nbutton;sel++)
		{
			if (&(m_button[sel]) == cwnd)
			{
				return cwnd;
			}
		}
		return m_button[0].SetFocus();
	}
	else
	{
	   if (m_concombo)
	   {		   
			if (!m_autocompletar && GetDroppedState() && (!cwnd || !m_calendario || (cwnd->m_hWnd != m_calendario->m_hWnd && !m_calendario->IsChild(cwnd) )))
			{
				SetDroppedState (FALSE);		
				if (!m_esrelacion)
				{
					m_Drop.OnSelection();		
				}
			}			
	   }
	   /*
		_AFX_THREAD_STATE* pThreadState = _afxThreadState.GetData();
		CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
		if (pToolTip)
		{
			TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND;
			ti.hwnd = m_hWnd;
			ti.uId = (UINT_PTR)m_hWnd;
			if (!pToolTip->SendMessage(TTM_GETTOOLINFO, 0, (LPARAM)&ti))
			{
				ASSERT(ti.uFlags == TTF_IDISHWND);
				ASSERT(ti.hwnd == m_hWnd);
				ASSERT(ti.uId == (UINT_PTR)m_hWnd);
				VERIFY(pToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));
			}

		   TOOLINFO tiHit; memset(&tiHit, 0, sizeof(TOOLINFO));
		   tiHit.cbSize = sizeof(TOOLINFO);
		   CPoint point;
		   CRect rect;
		   m_Edit.GetWindowRect(rect);
		   ScreenToClient(rect);
		   point = rect.CenterPoint();
		   int nHit = OnToolHitTest(point, &tiHit);
		   if (nHit != -1)
		   {
				ti = tiHit;
				ti.uFlags &= ~(TTF_NOTBUTTON|TTF_ALWAYSTIP);
				if (m_nFlags & WF_TRACKINGTOOLTIPS)
					ti.uFlags |= TTF_TRACK;
				VERIFY(pToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));
				//if ((tiHit.uFlags & TTF_ALWAYSTIP) || IsTopParentActive())
				{
					// allow the tooltip to popup when it should
					pToolTip->SendMessage(TTM_ACTIVATE, TRUE);
					if (m_nFlags & WF_TRACKINGTOOLTIPS)
						pToolTip->SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

					// bring the tooltip window above other popup windows
					::SetWindowPos(pToolTip->m_hWnd, HWND_TOP, 0, 0, 0, 0,
						SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
					::ShowWindow(pToolTip->m_hWnd,SW_SHOW);
				}
		   }
		}
		*/
	   

	   return m_Edit.SetFocus();
	}
}

void CDsCombo::AjustaBotonRect()
{
	if (!m_nbutton) 
		return;
    CRect rect;
    GetClientRect (rect);
	int h = rect.Size().cy / m_nbutton;
	for (int i = 0;i < m_nbutton;i++)
	{
		rect.bottom = rect.top + h;
		m_button[i].MoveWindow(rect);
		rect.top += h;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDsCheckBox

CDsCheckBox::CDsCheckBox()
{	
	m_noclick = FALSE;
}

CDsCheckBox::~CDsCheckBox()
{
}


BEGIN_MESSAGE_MAP(CDsCheckBox, BASECDsCheckBox)
	//{{AFX_MSG_MAP(CDsCheckBox)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()	
	ON_WM_CTLCOLOR_REFLECT( )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsCheckBox message handlers

void CDsCheckBox::OnClicked() 
{
	CDsCombo *pCombo = (CDsCombo *)GetParent();
	if (!pCombo)
		return;
	if (pCombo->m_nbutton == 1)
	{
		if (!GetCheck())
		{
			SetCheck(1);
		}
		else
		{
			SetCheck(0);
		}
	}
	else
	{
		if (m_noclick)
			return;
		int osel = -1;
		int sel;
		for (sel = 0;sel < pCombo->m_nbutton;sel++)
		{
			if (&(pCombo->m_button[sel]) == this)
			{
				osel = sel;
				break;
			}
		}
		if (GetCheck())
		{
			osel++;
			if (pCombo->m_nbutton <= osel)
			{
				osel = 0;
			}
		}
		for (sel = 0;sel < pCombo->m_nbutton;sel++)
		{
			if (sel == osel)
			{
				pCombo->m_button[sel].SetCheck(1);
				if (&(pCombo->m_button[sel]) != this)
				{
					pCombo->m_button[sel].SetFocus();
				}
			}
			else
			{
				pCombo->m_button[sel].SetCheck(0);
			}
		}		
	}
	pCombo->m_Edit.TeclaStd(TECLA_CHECK);
}

void CDsCheckBox::OnSetFocus(CWnd* pOldWnd)
{	

	CDsCombo *pCombo = (CDsCombo *)GetParent();
	if (!pCombo || pCombo->m_Edit.m_no999)
	{
		BASECDsCheckBox::OnSetFocus(pOldWnd);
		Invalidate();
		return;
	}

    HWND oldw = NULL;

	pCombo->m_Edit.m_fococonfirmado = FALSE;

    if (pOldWnd)
        oldw = pOldWnd->m_hWnd;
	m_noclick = TRUE;
	BASECDsCheckBox::OnSetFocus(pOldWnd);
	m_noclick = FALSE;

	if (!theApp.m_hwnd_999)
		theApp.m_hwnd_999 = (int)oldw;
	
	if (pCombo)
	{
		pCombo->m_Edit.TeclaStd(999);
	}
	Invalidate();
}

void CDsCheckBox::OnKillFocus(CWnd* pNewWnd) 
{
	BASECDsCheckBox::OnKillFocus(pNewWnd);
}

CWnd* CDsCheckBox::SetFocus( )
{
	CWnd *pwndr = CBASEMICOMBO::SetFocus( );
	CDsCombo *pCombo = (CDsCombo *)GetParent();
	if (pCombo)
	{
		if (!pCombo->m_Edit.m_no999)
		{
			ConfirmaFoco();
		}
	}
	return pwndr;
}


void CDsCheckBox::ConfirmaFoco()
{
	CDsCombo *pCombo = (CDsCombo *)GetParent();

	pCombo->m_Edit.m_no999 = TRUE;
	SetFocus();
	pCombo->m_Edit.m_no999 = FALSE;		
	
	pCombo->m_Edit.m_fococonfirmado = TRUE;

	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
    theApp.m_ctrlactivo = pCombo->m_Edit.m_dsctrl;		
    pCombo->m_Edit.EnsureVisible();	

	Invalidate();
}


HBRUSH CDsCheckBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	CDsCombo *pCombo = (CDsCombo *)GetParent();
	if (!pCombo)
		return NULL;

    HWND hwnd = ::GetFocus();

    if (hwnd == m_hWnd)
    {		
		return (HBRUSH)::GetStockObject(BLACK_BRUSH);
    }
	return NULL;
}

BOOL CDsCombo::PreTranslateMessage(MSG* pMsg) 
{
	return CBASEMICOMBO::PreTranslateMessage(pMsg);
}

BOOL CDsCheckBox::PreTranslateMessage(MSG* pMsg) 
{
	CDsCombo *pCombo = (CDsCombo *)GetParent();
	if (pCombo)
	{
		if (pMsg->message == WM_KEYUP)
		{
			int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
			if (comando) //  == 805 || comando == 804 || comando == 806 || comando == 807 || comando == 802 || comando == 809 || comando == 808
			{            
				return TRUE;
			}
		}

		if (pMsg->message == WM_KEYDOWN)
		{
			int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
			if (comando)
			{
				pCombo->m_Edit.TeclaStd(comando);				
				return TRUE;        
			}
			return BASECDsCheckBox::PreTranslateMessage(pMsg);
		}
    }
	return BASECDsCheckBox::PreTranslateMessage(pMsg);		
}

BOOL CDsCombo::HaCambiado(int maxqc, int qc, int tipo, int decimales, char *pqi, char *pqs)
{
	if (m_Edit.m_qc != qc)
		return TRUE;
	if (m_Edit.m_maxqc != maxqc)
		return TRUE;
	if (m_Edit.m_tipo != tipo)
		return TRUE;
	if (m_Edit.m_decimales != decimales)
		return TRUE;
	if (strcmp(m_Edit.m_qi,pqi))
		return TRUE;
	

	char qs[256];
	strcpy(qs,pqs);
	BOOL comborel;
	char *p = strstr(qs,"\010\010");
	if (p)
	{
		*p = 0;
		comborel = TRUE;
	}
	else
		comborel = FALSE;
	if (strcmp(m_Edit.m_qs,qs))
		return TRUE;
	if (m_Edit.m_comborel != comborel)
		return TRUE;

	return FALSE;
}

BOOL CDsCombo::SetAtributos(int Atrib, int Cc, int Cf)
{
   BOOL r = FALSE;
   if (m_Edit.m_Attrib != Atrib)
   {
	   m_Edit.m_Attrib = Atrib;
	   r = TRUE;
   }
   if (m_Edit.m_Cc != Cc)
   {
	   m_Edit.m_Cc = Cc;
	   r = TRUE;
   }
   if (m_Edit.m_Cf != Cf)
   {
	   m_Edit.m_Cf = Cf;
	   r = TRUE;
   }
   return r;
}

BOOL CDsCombo::EsPosicional()
{
	if (m_Edit.m_dsctrl)
	{
		if ( (m_Edit.m_dsctrl->m_cid.m_lcid & 0xffff0000) )
			return TRUE;
	}
	return FALSE;
}

BOOL CDsComboEdit::PreTranslateMessage(MSG* pMsg) 
{		
	return CDsEdit::PreTranslateMessage(pMsg);
}

void CDsComboEdit::OnAltDown() 
{
	if (m_pCombo->m_concombo)
	{
		m_pCombo->OnDropdownButton();
	}	
}

void CDsCombo::OnParentNotify(UINT message, LPARAM lParam) 
{
	CBASEMICOMBO::OnParentNotify(message, lParam);		
}

void CDsCombo::PonEnAutocompletar(char *p,int nl)
{
	int maxl = *((int *)p);
	p += sizeof(int);

	while(nl > 0)
	{
		for (unsigned char *p1 = (unsigned char *)p;*p1;p1++)
			oem_a_ansi(p1);
		AddString ( p , maxl );
		p += (strlen(p)+1);
		nl--;
	}
}

void CDsCombo::EstableceAutocompletar(char *p,int nl)
{
	if (GetDroppedState())
	{
		SetDroppedState (FALSE);
		if (m_esrelacion)
		{
			if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 4)
			{
				theApp.m_ctrlactivo->TeclaStd(806);
			}
		}
		else
		{
			m_Drop.OnSelection();		
		}
	}
	if (m_concombo)
	{
		if (::IsWindow(m_lst.m_hWnd))
		{
			m_lst.DestroyWindow();
		}
		if (::IsWindow(m_Drop.m_hWnd))
		{
			m_Drop.DestroyWindow();
		}
	}
	else
		CreateButton();

	m_esrelacion = FALSE;
	m_concombo = TRUE;	
	m_lista = TRUE;
	m_autocompletar = TRUE;
	CreateDrop(LBS_SORT);

	PonEnAutocompletar(p,nl);
}