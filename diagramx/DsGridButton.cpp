// DsGridButton.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsGridButton.h"
#include "TablaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsGridButton

CDsGridButton::CDsGridButton(CTablaView *tabla)
{
    m_tabla = tabla;
    m_tipo = 0;
    m_nume = 0;
}

CDsGridButton::~CDsGridButton()
{
}


BEGIN_MESSAGE_MAP(CDsGridButton, CButton)
	//{{AFX_MSG_MAP(CDsGridButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsGridButton message handlers

void CDsGridButton::OnClicked() 
{
    if (m_tabla)
    {
        if (m_tabla->m_editando)
        {
            theApp.ActivacionPorDefecto();
            return;
        }
        m_tabla->SetFocus();
    }
    else
        return;

    if (m_tipo)
    {        
        POINT pp;
        CRect r;        
        GetWindowRect(r);
        pp.x = r.left;
        pp.y = r.top;
        if (m_tabla->OpcionesConsulta(pp))
        {           
           return;
        }
    }
    else
    {
        m_tabla->NuevoCorriente(m_tabla->GetTopIndex()+m_nume,FALSE);
    }
}

void CDsGridButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);	
    
    if (m_tabla)
    {
        if (m_tabla->m_editando)
        {
                theApp.ActivacionPorDefecto();
                return;
        }
        m_tabla->SetFocus();
    }
    else
	    pOldWnd->SetFocus();	
}

void CDsGridButton::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CButton::OnLButtonDown(nFlags, point);
    OnClicked();	
}

void CDsGridButton::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CButton::OnRButtonDown(nFlags, point);
	if (!m_tabla)
		return;
	OnClicked();
	if (!m_tipo)
	{
		POINT pp;
		CRect r;        
		GetWindowRect(r);
		pp.x = r.left;
		pp.y = r.top;
		if (m_tabla->OpcionesConsulta(pp))
		{           
		   return;
		}
	}
}
