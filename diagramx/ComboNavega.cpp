// ComboNavega.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "ComboNavega.h"
#include "DsxMainfrm.h"
#include "DsHtml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboNavega

CComboNavega::CComboNavega()
{
}

CComboNavega::~CComboNavega()
{
}


BEGIN_MESSAGE_MAP(CComboNavega, BASECComboNavega)
	//{{AFX_MSG_MAP(CComboNavega)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboNavega message handlers

void CComboNavega::OnSelendok() 
{
    if (theApp.m_phtml)
        theApp.m_phtml->Navega();
}



BOOL CComboNavega::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
        switch(pMsg->wParam)
        {
        /*
        case VK_ESCAPE:
            TeclaStd(806);
            return TRUE;
        */
        case VK_RETURN:
            if (!GetDroppedState( ))
            {
                CString s;
                GetWindowText(s);
                Inserta(s);
                if (theApp.m_phtml)
                    theApp.m_phtml->Navega();
                return TRUE;
            }
        }
    }    
	return BASECComboNavega::PreTranslateMessage(pMsg);
}

int CComboNavega::Inserta(CString &s)
{
   int item = FindStringExact( -1, s );
   if (item >= 0) return item;

   item = AddString( s );
   SetCurSel(item);

   return item;
}
