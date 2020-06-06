//****************************************************************************
// File: FORM2.CPP
//
// Implement CFormView derived CForm2 class with two data fields (#3 and #4),
// and three buttons to choose other views.  (Second, being this view, is 
// disabled).  DDX/DDV integer variables are associated with the edit fields.
// 
// File was originally created by using Class Wizard to create
// a CFormView derived class and associate it with a dialog template.
//
// Functions (non App/Class Wizard generated, added, or modified):
//
//        CForm2::OnInitialUpdate()    - Update DDX variables from document and
//                                      resize frame around form view
//
//        CForm2::SetFromDoc()        - Load DDX variables from document
//
//        CForm2::SetToDoc()            - Save and validate DDX variables to doc
//
//        CForm2::OnClickedScreen1()    
//        CForm2::OnClickedViewdata() - Handlers for buttons to switch views
//
// Development Team: Tarn Faulkner
//
// Written by Microsoft Product Support Services, Languages Developer Support
// Copyright (c) 1996 Microsoft Corporation. All rights reserved.
//****************************************************************************

#include "stdafx.h"
#include "vswap.h"
#include "form2.h"

#include "vswapdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CForm2

IMPLEMENT_DYNCREATE(CForm2, CFormView)

CForm2::CForm2()
    : CFormView(CForm2::IDD)
{
    //{{AFX_DATA_INIT(CForm2)
    m_nField3 = 0;
    m_nField4 = 0;
    //}}AFX_DATA_INIT
}

CForm2::~CForm2()
{
}

void CForm2::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CForm2)
    DDX_Text(pDX, IDC_EDIT_FIELD3, m_nField3);
    DDV_MinMaxUInt(pDX, m_nField3, 0, 100);
    DDX_Text(pDX, IDC_EDIT_FIELD4, m_nField4);
    DDV_MinMaxUInt(pDX, m_nField4, 0, 100);
    //}}AFX_DATA_MAP
}


//***********************************************************************
// CForm2::OnInitialUpdate()
//
// Purpose:
//
//        Update DDX variables from document on startup.
//
// Parameters:
//
//        None.
//
// Returns:
//
//        None.
//
// Comments:
//
// History:
//
//   Date   Comment                                           Initials
// ======== ================================================= ========
// 11/30/93 Created                                             TF
//***********************************************************************
void CForm2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    SetFromDoc();
}


//***********************************************************************
// CForm2::SetFromDoc()
//
// Purpose:
//
//        Called to update DDX variables from document.
//
// Parameters:
//
//        None.
//
// Returns:
//
//        None.
//
// Comments:
//
// History:
//
//   Date   Comment                                           Initials
// ======== ================================================= ========
// 11/30/93 Created                                             TF
//***********************************************************************
void CForm2::SetFromDoc()
{
    CVswapDoc * pDoc = (CVswapDoc*) GetDocument();
    
    m_nField3 = pDoc->Field( 2 );
    m_nField4 = pDoc->Field( 3 );
    
    UpdateData(FALSE);    // Set controls to these
}


//***********************************************************************
// CForm2::SetToDoc()
//
// Purpose:
//
//        Called to save and validate DDX variables to document.
//
// Parameters:
//
//        None.
//
// Returns:
//
//        BOOL        - TRUE if validation succeeds, else FALSE
//
// Comments:
//
// History:
//
//   Date   Comment                                           Initials
// ======== ================================================= ========
// 11/30/93 Created                                             TF
//***********************************************************************
BOOL CForm2::SetToDoc()
{
    CVswapDoc * pDoc = (CVswapDoc*) GetDocument();

    if ( !UpdateData(TRUE) )
        return FALSE;
    
    pDoc->SetField( 2, m_nField3 );
    pDoc->SetField( 3, m_nField4 );
    
    return TRUE;
}


BEGIN_MESSAGE_MAP(CForm2, CFormView)
    //{{AFX_MSG_MAP(CForm2)
    ON_BN_CLICKED(ID_SCREEN1, OnClickedScreen1)
    ON_BN_CLICKED(ID_VIEWDATA, OnClickedViewdata)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForm2 message handlers


//***********************************************************************
// CForm2::OnClickedScreen1()
//
// Purpose:
//
//        Button handler.  Switches view to CForm1
//
// Parameters:
//
//        None.
//
// Returns:
//
//        None.
//
// Comments:
//
// History:
//
//   Date   Comment                                           Initials
// ======== ================================================= ========
// 11/30/93 Created                                             TF
//***********************************************************************
void CForm2::OnClickedScreen1()
{
    // Will do data validation and saving
    ((CVswapApp*)AfxGetApp())->SwitchView( 0 );
}


//***********************************************************************
// CForm2::OnClickedViewdata()
//
// Purpose:
//
//        Button handler.  Switches view to CVswapView.
//
// Parameters:
//
//        None.
//
// Returns:
//
//        None.
//
// Comments:
//
// History:
//
//   Date   Comment                                           Initials
// ======== ================================================= ========
// 11/30/93 Created                                             TF
//***********************************************************************
void CForm2::OnClickedViewdata()
{              
    // Will do data validation and saving
    ((CVswapApp*)AfxGetApp())->SwitchView( 2 );    
}
