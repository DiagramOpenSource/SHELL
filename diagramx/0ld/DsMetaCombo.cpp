// DsMetaCombo.cpp : implementation file
//
#include "stdafx.h"
#include "diagramx.h"
#include "DsxDoc.h"
#include "DsxView.h"
#include "DsMetaCombo.h"
#include "DsEdit.h"
#include "DsStdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int Post_ID_AVISO_MUEREMETA;

/////////////////////////////////////////////////////////////////////////////
// CDsMetaCombo

CDsMetaCombo::CDsMetaCombo()
{
    m_dsedit = NULL;
	m_comborel = FALSE;	
}

CDsMetaCombo::~CDsMetaCombo()
{
    /*
    if (theApp.m_ctrlactivo == m_dsedit->m_dsctrl)
    {        
        theApp.m_ctrlactivo = NULL;
    } else if (theApp.m_antctrlactivo == m_dsedit->m_dsctrl)
    {        
        theApp.m_antctrlactivo = NULL;
    }
    */
	if (m_dsedit && m_dsedit->m_metacombo == this)
	{
		m_dsedit->m_metacombo = NULL;
	}

}


BEGIN_MESSAGE_MAP(CDsMetaCombo, CDsCombo)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, CDsComboDropWndToolTip )
	//{{AFX_MSG_MAP(CDsMetaCombo)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_SETFOCUS, OnSetfocus)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsMetaCombo message handlers

BOOL CDsMetaCombo::CDsComboDropWndToolTip (UINT id, NMHDR * pNMHDR, LRESULT * pResult) 
                                //UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
	if (m_dsedit && m_dsedit->m_dsctrl && m_dsedit->m_dsctrl->m_pdlg)
	{
		pNMHDR->idFrom = (UINT)m_dsedit->m_hWnd;
		return m_dsedit->m_dsctrl->m_pdlg->DsStdDlgToolTip((UINT)m_dsedit->m_hWnd,pNMHDR,pResult);
	}
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	pTTT->lpszText = "Diagram Software";
	return TRUE;
}


void CDsMetaCombo::OnKillfocus() 
{
    Muere();
    if (theApp.m_ctrlactivo == m_dsedit->m_dsctrl)
    {
        theApp.m_ctrlactivo = NULL;
    }
}

void CDsMetaCombo::Muere()
{
	if (Post_ID_AVISO_MUEREMETA) return;

    if (m_dsedit && m_dsedit->m_metacombo == this)
    {
        ShowWindow(SW_HIDE);

        CString s;

        GetWindowText(s);

        ((CEdit *)m_dsedit)->SetWindowText(s);

        
        ((CDsxDoc *)m_dsedit->m_pView->GetDocument())->m_metacombo = NULL;
		if (!m_dsedit->m_dsctrl || !(m_dsedit->m_dsctrl->m_nivel))
		{
			m_dsedit->ShowWindow(SW_SHOW);        
		}
        m_dsedit->m_metacombo = NULL;

		Post_ID_AVISO_MUEREMETA++;
        m_dsedit->m_pView->PostMessage(WM_COMMAND,ID_AVISO_MUEREMETA,(LPARAM)this);
    }       
       //DestroyWindow();
       //delete this;
}

BOOL CDsMetaCombo::CreateEdit ()
{
	m_Edit.Copia(m_dsedit);
	return CDsCombo::CreateEdit();
}


BOOL CDsMetaCombo::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class


	if (pMsg->message == WM_KEYDOWN)
    {        
       WPARAM nChar = pMsg->wParam;

        //CDsCombo         

       int comando = CDsStdDlg::DeterminaTecla(nChar);
       switch(comando)
       {
       case 819:
           comando = 0;
           break;
       case 818:
           comando = 0;           
           break;
       case 816:
           comando = 0;
           break;
       case 812:
           comando = 0;
           break;
       case 810:
           comando = 0;
           break;      
       case 811:
           comando = 0;
           break;       
       case 991:
           comando = SOLICITA_INICIO;
           break;
       case 990:
           comando = SOLICITA_FINAL;
           break;
	   case 992:
	   case 809:
		   
		   comando = 0;
		   /*
		   ShowDropDown( TRUE );
		   */
		   break;
       default:
            break;
       }
       if (comando)
       {
           if (m_dsedit)
              m_dsedit->TeclaStd(comando);
           return TRUE;
       }
	   	   
	   
	   if ((nChar >= 'A' && nChar <= 'Z') || (nChar >= 'a' && nChar <= 'z'))
	   {
		   if (m_dsedit && m_dsedit->m_tipo == 1 && m_dsedit->m_maxqc == 1 && m_dsedit->m_qi[0])
		   {
			   for (int j = 0;m_dsedit->m_qi[j];j++)
			   {
				   if (m_dsedit->m_qi[j] == (char)nChar) break;
			   }
			   if (!m_dsedit->m_qi[j]) return TRUE;
		   }
	   }
    }
	return CDsCombo::PreTranslateMessage(pMsg);
}

BOOL CDsMetaCombo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
    if ((wParam&0xffff) >= 800 && (wParam&0xffff) <= 999)
    {
        m_dsedit->TeclaStd((wParam&0xffff));
        return TRUE;
    }
    else	
	{
		return CDsCombo::OnCommand(wParam, lParam);	    
	}
}

void CDsMetaCombo::OnSetfocus() 
{
   
   theApp.m_ctrlactivo = m_dsedit->m_dsctrl;
}

int CDsMetaCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDsCombo::OnCreate(lpCreateStruct) == -1)
		return -1;    

	CDsStdDlg::m_hwndctrllst[m_hWnd] = m_dsedit->m_dsctrl;

    ::SetWindowPos(m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	
	return 0;
}

void CDsMetaCombo::OnDestroy() 
{
    CDsStdDlg::m_hwndctrllst.RemoveKey(m_hWnd);
	CDsCombo::OnDestroy();	
}

void CDsMetaCombo::OnSelchange() 
{
	if (m_dsedit)
    {
        CString s;
        GetWindowText(s);
        ((CEdit *)m_dsedit)->SetWindowText(s);
    }
}

void CDsMetaCombo::OnEditchange() 
{
	if (m_dsedit)
    {
        CString s;
        GetWindowText(s);
        ((CEdit *)m_dsedit)->SetWindowText(s);
    }
}

void CDsMetaCombo::OnDropdown() 
{	
	
	if (m_comborel)
	{
       if (m_dsedit)
	   {		  
          m_dsedit->TeclaStd(TECLA_COMBO);		  
	   }
	}
}

void CDsMetaCombo::OnCloseup() 
{	
}


void CDsMetaCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDsCombo::OnLButtonDown(nFlags, point);
}

void CDsMetaCombo::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDsCombo::OnLButtonDblClk(nFlags, point);
}

BOOL CDsMetaCombo::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDsCombo::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CDsMetaCombo::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	/*
	if (message == WM_COMMAND)
	{
		if (((wParam>>16)&0xffff) == CBN_SELENDOK)
		{
           if (m_dsedit)
              m_dsedit->TeclaStd(802);
		}
	}
	*/
	
	return CDsCombo::OnChildNotify(message, wParam, lParam, pLResult);
}


void CDsMetaCombo::EsRelacion(BOOL relacion)
{
   m_comborel = relacion;
   m_lista    = !relacion;
}


BOOL CDsMetaCombo::TeclaStd(int tecla)
{
	if (m_dsedit)
	  return m_dsedit->TeclaStd(tecla);
	else
	  return FALSE;
}