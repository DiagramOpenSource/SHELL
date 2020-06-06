// DsListBox.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsListBox.h"
#include "DsStdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsListBox

CDsListBox::CDsListBox()
{
}

CDsListBox::~CDsListBox()
{
    if (theApp.m_ctrlactivo == m_dsctrl)
    {        
        theApp.m_ctrlactivo = NULL;
    } else if (theApp.m_antctrlactivo == m_dsctrl)
    {        
        theApp.m_antctrlactivo = NULL;
    }
}


BEGIN_MESSAGE_MAP(CDsListBox, CListBox)
	//{{AFX_MSG_MAP(CDsListBox)
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_WM_VKEYTOITEM_REFLECT()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsListBox message handlers

void CDsListBox::OnSetfocus() 
{

	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
    theApp.m_ctrlactivo = m_dsctrl;

    m_dsctrl->EnsureVisible();
	return;
}

void CDsListBox::OnKillfocus() 
{
	/*
    if (theApp.m_ctrlactivo == m_dsctrl)
    {
        theApp.m_ctrlactivo = NULL;
    }
	*/
    return;
}

void CDsListBox::OnDblclk() 
{
    m_dsctrl->TeclaStd(m_idmensa+GetCurSel());
    return;
}

int CDsListBox::VKeyToItem(UINT nKey, UINT nIndex) 
{
	return -1;
}

void CDsListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDsListBox::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
        int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
        if (comando == 802)
        {
            m_dsctrl->TeclaStd(m_idmensa+GetCurSel());
            return TRUE;
        }
        if (comando == 806 || comando == 807)
        {
            m_dsctrl->TeclaStd(comando);
            return TRUE;
        }        
    }    		
	return CListBox::PreTranslateMessage(pMsg);
}
