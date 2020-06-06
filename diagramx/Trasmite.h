
#if !defined(AFX_TRASMITE_H__73F1FC13_F3DE_11D2_B144_004F4905CBCF__INCLUDED_)
#define AFX_TRASMITE_H__73F1FC13_F3DE_11D2_B144_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Trasmite.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrasmite dialog

extern "C" 
{
   void ControlTrasmision(int entero,void *puntero);
}

extern unsigned long crt1;
extern unsigned long crt2;
extern unsigned long crt3;

class CTrasmite : public CExtNCW < CBASEDLG >
{
   // Construction
public:
   CTrasmite(CWnd* pParent = NULL);   // standard constructor
   
   friend void ControlTrasmision(int entero,void *puntero);
   
   // Dialog Data
   //{{AFX_DATA(CTrasmite)
   enum { IDD = IDD_TRASMITE };
   CProgressCtrl	m_tprogress;
   CTreeCtrl	m_taqui;
   CTreeCtrl	m_talli;
   CString	m_Paso;
   //}}AFX_DATA
   
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTrasmite)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL
   
   // Implementation
public:
   
   CString m_trasmite;
   BOOL m_alla;
   BOOL m_Salir;
   BOOL m_Refrescar;
   CString m_local;
   CString m_remoto;
   
protected:
   
   // Generated message map functions
   //{{AFX_MSG(CTrasmite)
   virtual BOOL OnInitDialog();
   afx_msg void OnDblclkTalli(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDblclkTaqui(NMHDR* pNMHDR, LRESULT* pResult);
   virtual void OnOK();
   afx_msg void OnClose();
   afx_msg void OnDestroy();
   virtual void OnCancel();
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRASMITE_H__73F1FC13_F3DE_11D2_B144_004F4905CBCF__INCLUDED_)
