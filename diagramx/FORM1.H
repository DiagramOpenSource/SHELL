// form1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CForm1 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CForm1 : public CFormView
{
    DECLARE_DYNCREATE(CForm1)
public:
    // Need these public for view switching and initialization
    CForm1();            
    virtual ~CForm1();

// Form Data
public:
    //{{AFX_DATA(CForm1)
    enum { IDD = IDD_VIEWFORM1 };
    UINT    m_nField1;
    UINT    m_nField2;
    //}}AFX_DATA

// Attributes
public:

// Operations
public:
    
    // For field updating and saving                   
    void SetFromDoc();
    BOOL SetToDoc();    // Called from SwitchView()

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForm1)
	public:
    virtual void OnInitialUpdate();
    virtual void DoDataExchange(CDataExchange* pDX);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CForm1)
    afx_msg void OnClickedScreen2();
    afx_msg void OnClickedViewdata();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
