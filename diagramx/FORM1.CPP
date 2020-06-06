//****************************************************************************
// File: FORM1.CPP
//
// Implement CFormView derived CForm1 class with two data fields (#1 and #2),
// and three buttons to choose other views.  (First, being this view, is 
// disabled).  DDX/DDV integer variables are associated with the edit fields.
// 
// File was originally created by using Class Wizard to create
// a CFormView derived class and associate it with a dialog template.
//
// Functions (non App/Class Wizard generated, added, or modified):
//
//        CForm1::OnInitialUpdate()    - Update DDX variables from document and
//                                      resize frame around form view
//
//        CForm1::SetFromDoc()        - Load DDX variables from document
//
//        CForm1::SetToDoc()            - Save and validate DDX variables to doc
//
//        CForm1::OnClickedScreen2()    
//        CForm1::OnClickedViewdata() - Handlers for buttons to switch views
//
// Development Team: Tarn Faulkner
//
// Written by Microsoft Product Support Services, Languages Developer Support
// Copyright (c) 1996 Microsoft Corporation. All rights reserved.
//****************************************************************************

#include "stdafx.h"
#include "vswap.h"
#include "form1.h"

#include "vswapdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CForm1

IMPLEMENT_DYNCREATE(CForm1, CFormView)

CForm1::CForm1()
    : CFormView(CForm1::IDD)
{
    //{{AFX_DATA_INIT(CForm1)
    m_nField1 = 0;
    m_nField2 = 0;
    //}}AFX_DATA_INIT
}

CForm1::~CForm1()
{
}

void CForm1::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CForm1)
    DDX_Text(pDX, IDC_EDIT_FIELD1, m_nField1);
    DDV_MinMaxUInt(pDX, m_nField1, 0, 100);
    DDX_Text(pDX, IDC_EDIT_FIELD2, m_nField2);
    DDV_MinMaxUInt(pDX, m_nField2, 0, 100);
    //}}AFX_DATA_MAP
}


//***********************************************************************
// CForm1::OnInitialUpdate()
//
// Purpose:
//
//        Update DDX variables from document on startup,
//          and resize frame around view
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
void CForm1::OnInitialUpdate()
{
    CFrameWnd * pFrame;
    CRect rectFrame, rectView;

    CFormView::OnInitialUpdate();
    
    // Update form's data from document
    SetFromDoc();

    // Resize parent to fit dialog template exactly    
    // while not resizing beyond size of screen
    VERIFY( pFrame = GetParentFrame() );
    pFrame->GetClientRect( rectFrame );
    GetClientRect( rectView );
    if ( rectFrame.Width() < rectView.Width()
         || rectFrame.Height() < rectView.Height() )
    {        
        // Resize so can be refit to any template that fits the screen
        pFrame->MoveWindow( 0, 0, GetSystemMetrics(SM_CXSCREEN), 
                        GetSystemMetrics(SM_CYSCREEN), FALSE ); 
    }
    pFrame->RecalcLayout();
    ResizeParentToFit(TRUE);    // Shrink to fit template
}


//***********************************************************************
// CForm1::SetFromDoc()
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
void CForm1::SetFromDoc()
{
    CVswapDoc * pDoc = (CVswapDoc*) GetDocument();
    
    m_nField1 = pDoc->Field(0);
    m_nField2 = pDoc->Field(1);
    
    UpdateData(FALSE);    // Set controls to these
}

//***********************************************************************
// CForm1::SetToDoc()
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
BOOL CForm1::SetToDoc()
{
    CVswapDoc * pDoc = (CVswapDoc*) GetDocument();

    // Allow for data validation    
    if ( !UpdateData(TRUE) )
        return FALSE;

    pDoc->SetField( 0, m_nField1 );
    pDoc->SetField( 1, m_nField2 );
    
    return TRUE;
}


BEGIN_MESSAGE_MAP(CForm1, CFormView)
    //{{AFX_MSG_MAP(CForm1)
    ON_BN_CLICKED(ID_SCREEN2, OnClickedScreen2)
    ON_BN_CLICKED(ID_VIEWDATA, OnClickedViewdata)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForm1 message handlers


//***********************************************************************
// CForm1::OnClickedScreen2()
//
// Purpose:
//
//        Button handler.  Switches view to CForm2
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
void CForm1::OnClickedScreen2()
{
    // Will do data validation and saving
    ((CVswapApp*)AfxGetApp())->SwitchView( 1 );
}


//***********************************************************************
// CForm1::OnClickedViewdata()
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
void CForm1::OnClickedViewdata()
{   
    // Will do data validation and saving
    ((CVswapApp*)AfxGetApp())->SwitchView( 2 );    
}
