#if !defined(AFX_DIALOGOIMPRE_H__E9DE7D1F_AACE_11D2_BE1F_004F4905CBCF__INCLUDED_)
#define AFX_DIALOGOIMPRE_H__E9DE7D1F_AACE_11D2_BE1F_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogoImpre.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogoImpre dialog

class CDialogoImpre : public CDialog
{
   // Construction
public:	
   CDialogoImpre(CWnd* pParent = NULL);   // standard constructor
   
   // Dialog Data
   //{{AFX_DATA(CDialogoImpre)
   enum { IDD = IDD_DIALOGOIMPRE };
   CComboBox	m_Font;
   int		m_MargenI;
   int		m_MargenS;
   int		m_Lineas;
   BOOL	m_Ajusta;
   int		m_Cols;
   BOOL	m_cabeceraexcel;
   CString	m_posexcel;
   int		m_MargenI2;
   //}}AFX_DATA
   
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDialogoImpre)
public:
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL
   
   // Implementation
public:
   CString m_SFont;   
protected:
   
   // Generated message map functions
   //{{AFX_MSG(CDialogoImpre)
   virtual BOOL OnInitDialog();
   afx_msg void OnImpreajusta();
   afx_msg void OnChangeMargeni();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOIMPRE_H__E9DE7D1F_AACE_11D2_BE1F_004F4905CBCF__INCLUDED_)
