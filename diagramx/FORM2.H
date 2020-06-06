// form2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CForm2 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CForm2 : public CFormView
{
    DECLARE_DYNCREATE(CForm2)
public:

    // Need these public for view switching and initialization
    CForm2();        
    virtual ~CForm2();

// Form Data
public:
    //{{AFX_DATA(CForm2)
    enum { IDD = IDD_VIEWFORM2 };
    UINT    m_nField3;
    UINT    m_nField4;
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
	//{{AFX_VIRTUAL(CForm2)
	public:
    virtual void OnInitialUpdate();
    virtual void DoDataExchange(CDataExchange* pDX);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CForm2)
    afx_msg void OnClickedScreen1();
    afx_msg void OnClickedViewdata();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
