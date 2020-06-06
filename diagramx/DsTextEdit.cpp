// DsTextEdit.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsTextEdit.h"
#include "DsStdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsTextEdit

CDsTextEdit::CDsTextEdit()
{
    m_dsctrl = NULL;
	m_Font = NULL;
	m_maximolinea = 0;
	m_lineas = 0;
	m_topelineas = 0;
	m_gsel = 0;
}

CDsTextEdit::~CDsTextEdit()
{
	if (m_Font)
	{
		delete m_Font;
	}
}


BEGIN_MESSAGE_MAP(CDsTextEdit, BASECDsTEdit)
	//{{AFX_MSG_MAP(CDsTextEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsTextEdit message handlers

BOOL CDsTextEdit::OnCmdMsg(UINT _nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	UINT nID = _nID & 0xffff;
	BOOL sele = FALSE;
	{
		int startPos, endPos;
		GetSel(startPos, endPos);	   
		if (startPos != endPos && startPos >= 0)
			sele = TRUE;
	}
	switch(nID)
	{
	case ID_EDIT_SELECT_ALL:
		return TRUE;
	case ID_EDIT_CLEAR:
		return sele;
	case ID_EDIT_COPY:
		return sele;
	case ID_EDIT_CUT:
		return sele;
	case ID_EDIT_PASTE:
		return TRUE;
	case ID_EDIT_UNDO:
		return CanUndo();
	default:
		break;
	}
	return BASECDsTEdit::OnCmdMsg(_nID, nCode, pExtra, pHandlerInfo);
}

BOOL CDsTextEdit::OnCommand(WPARAM wParam, LPARAM lParam) 
{

	if ((wParam&0xffff) == ID_EDIT_COPY)
	{		
		Copy();
		return TRUE;
	}
	if ((wParam&0xffff) == ID_EDIT_CUT)
	{
		Cut();
		return TRUE;
	}
	if ((wParam&0xffff) == ID_EDIT_PASTE)
	{
		Paste();
		return TRUE;
	}

	if ((wParam&0xffff) == ID_EDIT_UNDO)
	{
		Undo();
		return TRUE;
	}

	if ((wParam&0xffff) == ID_EDIT_SELECT_ALL)
	{
        SetSel(0,GetWindowTextLength()+1);
		return TRUE;
	}

	if ((wParam&0xffff) == ID_EDIT_CLEAR)
	{
		Clear();
		return TRUE;
	}

	if ((wParam&0xffff) == ID_GRIDNUEVACOLUMNA)
	{
		TeclaStd(TECLA_HINT);
		return TRUE;
	}
		

	/*
    if ((wParam&0xffff) == ID_PTECS)
    {
        int *p1 = (int *)lParam;
        ProcesaPtec(*p1);
        return TRUE;
    }
	*/

    return BASECDsTEdit::OnCommand(wParam, lParam);
}


BOOL CDsTextEdit::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
        int tecla = CDsStdDlg::DeterminaTecla(pMsg->wParam);
        if ((pMsg->wParam == VK_TAB && tecla == 802) || tecla == 805 || tecla == 804 || tecla == 806 || tecla == 807 || (tecla >= 820 && tecla < 863))
        {
            TeclaStd(tecla);
            return TRUE;
        }
        return BASECDsTEdit::PreTranslateMessage(pMsg);
    }
    else
	    return BASECDsTEdit::PreTranslateMessage(pMsg);
}

BOOL CDsTextEdit::TeclaStd(int tecla)
{
    if (m_dsctrl)
    {
        return m_dsctrl->TeclaStd(tecla);
    }
    return FALSE;
}

void CDsTextEdit::OnSetFocus(CWnd* pOldWnd) 
{
    HWND oldw = NULL;	

    if (pOldWnd)
        oldw = pOldWnd->m_hWnd;

	BASECDsTEdit::OnSetFocus(pOldWnd);

	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
    theApp.m_ctrlactivo = m_dsctrl;

	if (!theApp.m_hwnd_999)
		theApp.m_hwnd_999 = (int)oldw;
    TeclaStd(999);

	if (m_dsctrl)
		m_dsctrl->EnsureVisible();        
}

void CDsTextEdit::OnKillFocus(CWnd* pNewWnd) 
{
	BASECDsTEdit::OnKillFocus(pNewWnd);
	
	/*
   if (theApp.m_ctrlactivo == m_dsctrl)
    {
        theApp.m_antctrlactivo = theApp.m_ctrlactivo;
        theApp.m_ctrlactivo = NULL;
    }
	*/
}

void CDsTextEdit::OnUpdate() 
{	
   if (m_topelineas > 0)
   {
	   int n = GetLineCount();
	   if (m_lineas < n)
	   {
		   if (n > m_topelineas )
		   {
			   SetWindowText(m_guarda);
			   SetSel(m_gsel,FALSE);
		   }
	   }	   	   
   }   
}

void CDsTextEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if (m_topelineas > 0)
   {
	  m_lineas = GetLineCount();
	  GetWindowText(m_guarda);
	  m_gsel = GetSel();
   }


    //DefWindowProc(WM_CHAR, nChar, MAKELONG(nRepCnt, nFlags));
    BASECDsTEdit::OnChar(nChar, nRepCnt, nFlags);
    int isp = isprint(nChar);
	if (theApp.m_conhint && isp && (theApp.m_ulttecla == -1 || theApp.m_ulttecla == TECLA_PING))
	{
		//TeclaStd(TECLA_HINT);
		PostMessage(WM_COMMAND,ID_GRIDNUEVACOLUMNA);		
	}
}

void CDsTextEdit::OnChange() 
{
   /*
   if (m_maximolinea > 0)
   {
	   //int isp = isprint(nChar);
	   //if (isp)
	   {
		   FmtLines( TRUE );
		   CPoint point = GetCaretPos();
		   DWORD pos = CharFromPos( point );
		   int linea = (int)HIWORD(pos);
		   CString s;
		   char buff[1024];		   
		   int n = GetLine(linea,buff,1023);
		   if (n >= m_maximolinea)
		   {
			   pos = LOWORD(pos);
			   SetSel((int)pos+2,(int)pos+2);
			   			   
			   //point = PosFromChar( pos + 10 );
			   //SetCaretPos(point);
			   
			   return;
		   }
	   }
   }
   */
	
}

void CDsTextEdit::SetWindowText (LPCTSTR Text,BOOL tipohint)
{	
	
	CString so;

	BASECDsTEdit::GetWindowText(so);
	if (!so.Compare(Text))
	{
		return;
	}
	
	//CPoint careto = GetCaretPos();
	if (tipohint)
	{
       int startPos, endPos;	   
	   if (::GetFocus() == m_hWnd)
	   {
		 GetSel(startPos, endPos);	   
		 int n = strlen(so);
		 if (!memcmp(so,Text,n))
		 {
			BASECDsTEdit::SetWindowText(Text);
			if (startPos == endPos)
			{
				startPos = n;
				endPos = strlen(Text);
			}
			SetSel(startPos, endPos+1);
		 }
	   }
	}
	else
	{
	   BASECDsTEdit::SetWindowText (Text);
	}
	//SetCaretPos(careto);
	RedrawWindow(
			NULL,
			NULL,
			RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW
			|RDW_FRAME|RDW_ALLCHILDREN
			);
	//Invalidate();
}

void CDsTextEdit::OnLButtonDown(UINT nFlags,CPoint point)
{		
	int hazlo = 0;
	if (GetFocus()->m_hWnd != m_hWnd)
		hazlo = 1;
	BASECDsTEdit::OnLButtonDown(nFlags, point);
	if (m_dsctrl->m_pdlg->m_parchear)
	{
		if (hazlo)
		{
			OnSetFocus(GetFocus());
		}
		int n = CharFromPos(point);
		int nCharIndex = LOWORD(n);
		SetSel(nCharIndex,nCharIndex);
	}
}

void CDsTextEdit::OnMouseMove(UINT nFlags,CPoint point)
{		
	if (m_dsctrl->m_pdlg->m_parchear && (nFlags & MK_LBUTTON) && GetFocus()->m_hWnd == m_hWnd)
	{
		int n = CharFromPos(point);
		//int nLineIndex = HIWORD(n);
		int nCharIndex = LOWORD(n);
		int startPos = -1, endPos = -1;

		GetSel(startPos, endPos);
		if (startPos < 0 || startPos == endPos)
		{
			startPos = endPos = nCharIndex; 
			endPos++;
		}
		else if (startPos >= nCharIndex)
		{
			startPos = nCharIndex;
		}
		else if (endPos < nCharIndex || endPos > nCharIndex)
		{
			endPos = nCharIndex;
		}
		SetSel(startPos, endPos);
	}
	BASECDsTEdit::OnMouseMove(nFlags, point);
}

void CDsTextEdit::OnRButtonDown(UINT nFlags,CPoint point)
{		
	BASECDsTEdit::OnRButtonDown(nFlags, point);
#ifdef DSPROFUIS
	if (m_dsctrl->m_pdlg->m_parchear)
	{		
		CWnd* pWnd = this;
		CPoint pos = point;
		ClientToScreen(&pos);

	LPCTSTR strProfileName =
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() );
		if( strProfileName == NULL ){
			CEdit::OnContextMenu( pWnd, pos );
			return;
		}
	CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
		if( !pPopup->CreatePopupMenu( GetSafeHwnd() ) )
		{
			ASSERT( FALSE );
			delete pPopup;
			CEdit::OnContextMenu( pWnd, pos );
			return;
		}
	static struct
	{
		UINT m_nCmdID;
		LPCTSTR m_sMenuText;
	} arrCmds[] =
	{
		{ ID_EDIT_UNDO, _T("&Undo") },
	//	{ ID_EDIT_REDO, _T("&Redo") },
		{ ID_SEPARATOR, NULL },
		{ ID_EDIT_CUT, _T("Cu&t") },
		{ ID_EDIT_COPY, _T("&Copy") },
		{ ID_EDIT_PASTE, _T("&Paste") },
		{ ID_EDIT_CLEAR, _T("Cl&ear") },
		{ ID_SEPARATOR, NULL },
		{ ID_EDIT_SELECT_ALL, _T("Select &All") },
	};
		for( int i = 0; i < sizeof(arrCmds)/sizeof(arrCmds[0]); i++ )
		{
			if( arrCmds[i].m_nCmdID == ID_SEPARATOR )
			{
				VERIFY( pPopup->ItemInsert( ID_SEPARATOR ) );
				continue;
			} // if( arrCmds[i].m_nCmdID == ID_SEPARATOR )
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					strProfileName,
					arrCmds[i].m_nCmdID
					);
			if( pCmdItem == NULL )
				pCmdItem =
					g_CmdManager->CmdAllocPtr(
						strProfileName,
						arrCmds[i].m_nCmdID
						);
			ASSERT( pCmdItem != NULL );
			if( pCmdItem == NULL )
				continue;
			if( pCmdItem->m_sMenuText.IsEmpty() )
			{
				CExtSafeString sText;
				if( g_ResourceManager->LoadString( sText, arrCmds[i].m_nCmdID ) )
				{
					sText.Replace( _T("\t"), _T(" ") );
					sText.Replace( _T("\r"), _T("") );
					sText.TrimLeft();
					sText.TrimRight();
					if( ! sText.IsEmpty() )
					{
						int nSep = sText.ReverseFind('\n');
						if( nSep < 0 )
						{
							pCmdItem->m_sMenuText = sText;
						} // if( nSep < 0 )
						else
						{
							int nLen = sText.GetLength();
							pCmdItem->m_sMenuText = sText.Right( nLen - nSep );
							pCmdItem->m_sMenuText.TrimLeft();
							pCmdItem->m_sMenuText.TrimRight();
						} // else from if( nSep < 0 )
					} // if( ! sText.IsEmpty() )
				}
				if( pCmdItem->m_sMenuText.IsEmpty() )
					pCmdItem->m_sMenuText = arrCmds[i].m_sMenuText;
			} // if( pCmdItem->m_sMenuText.IsEmpty() )
			pCmdItem->StateSetBasic();
			VERIFY( pPopup->ItemInsert( arrCmds[i].m_nCmdID ) );
		} // for( int i = 0; i < sizeof(arrCmds)/sizeof(arrCmds[0]); i++ )
	CRect rcClient;
		GetClientRect( &rcClient );
		ClientToScreen( &rcClient );
		if( !rcClient.PtInRect( pos ) )
			pos = rcClient.CenterPoint();
		SetFocus();
		
	HWND hWndOwn = m_hWnd;
		if(	! pPopup->TrackPopupMenu(
				TPMX_OWNERDRAW_FIXED|TPMX_DO_MESSAGE_LOOP,
				pos.x, 
				pos.y
				) 
			)
		{
			ASSERT( FALSE );
			delete pPopup;
			if( ! ::IsWindow( hWndOwn ) )
				return;
			CEdit::OnContextMenu( pWnd, pos );
			return;
		}
		else
		{
			if( ! ::IsWindow( hWndOwn ) )
				return;
			VERIFY(
				RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW
				|RDW_FRAME|RDW_ALLCHILDREN
				)
				);
		}
	}
#endif
}

