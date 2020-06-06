// DsCtrlScroll.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsCtrlScroll.h"
#include "DsStdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsCtrlScroll

CDsCtrlScroll::CDsCtrlScroll()
{
    m_dsctrl = NULL;
}

CDsCtrlScroll::~CDsCtrlScroll()
{
}


BEGIN_MESSAGE_MAP(CDsCtrlScroll, CExtScrollBar)
	//{{AFX_MSG_MAP(CDsCtrlScroll)
	ON_WM_HSCROLL_REFLECT()
	ON_WM_VSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsCtrlScroll message handlers

void CDsCtrlScroll::HScroll(UINT nSBCode, UINT nPos) 
{
    int x = 0;
	switch (nSBCode)
	{
	    case SB_TOP:
		    x = SOLICITA_INICIO;
		    break;
	    case SB_BOTTOM:
		    x = SOLICITA_FINAL;
		    break;
	    case SB_LINEUP:
		    x = SOLICITA_SCROLLUP;
		    break;
	    case SB_LINEDOWN:
		    x = SOLICITA_SCROLLDOWN;
		    break;
	    case SB_PAGEUP:
		    x = 814;
		    break;
	    case SB_PAGEDOWN:
		    x = 815;
		    break;
	    case SB_THUMBTRACK:
            if ( (nPos - GetScrollPos()) < 0)
                x = 814;
		    else
                x = 815;
		    break;
    }
    if (x)
    {
        if (!theApp.TeclaAlActivo(x))
        {
            if (m_dsctrl)
            {
                m_dsctrl->TeclaStd(x);
            }	
        }
    }

}

void CDsCtrlScroll::VScroll(UINT nSBCode, UINT nPos) 
{	
    int x = 0;
	switch (nSBCode)
	{
	    case SB_TOP:
		    x = SOLICITA_INICIO;
		    break;
	    case SB_BOTTOM:
		    x = SOLICITA_FINAL;
		    break;
	    case SB_LINEUP:
		    x = SOLICITA_SCROLLUP;
		    break;
	    case SB_LINEDOWN:
		    x = SOLICITA_SCROLLDOWN;
		    break;
	    case SB_PAGEUP:
		    x = 814;
		    break;
	    case SB_PAGEDOWN:
		    x = 815;
		    break;
	    case SB_THUMBTRACK:
            if ( (nPos - GetScrollPos()) < 0)
                x = 814;
		    else
                x = 815;
		    break;
    }
    if (x)
    {
        if (!theApp.TeclaAlActivo(x))
        {
            if (m_dsctrl)
            {
                m_dsctrl->TeclaStd(x);
            }	
        }
    }
}
