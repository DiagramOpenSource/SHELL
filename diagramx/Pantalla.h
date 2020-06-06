
#ifdef YANOEXISTE

#if !defined(AFX_PANTALLA_H__3693EE97_A3BB_11D2_BE13_004F4905CBCF__INCLUDED_)
#define AFX_PANTALLA_H__3693EE97_A3BB_11D2_BE13_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pantalla.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPantalla window

class CPantalla : public CWnd
{
   // Construction
public:
   CPantalla(CWnd *Hijo);
   
   // Attributes
public:
   
   // Operations
public:
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CPantalla)
   //}}AFX_VIRTUAL
   
   // Implementation
public:
   void AjustaMinimize();
   CString Clase();
   virtual ~CPantalla();
   
   // Generated message map functions
protected:
   //{{AFX_MSG(CPantalla)
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
private:
   CWnd * m_hijo;
   CString m_clase;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANTALLA_H__3693EE97_A3BB_11D2_BE13_004F4905CBCF__INCLUDED_)

#endif