
#if !defined(AFX_DSEDIT_H__F1E66721_E2D3_11D2_BEA9_004F4905CBCF__INCLUDED_)
#define AFX_DSEDIT_H__F1E66721_E2D3_11D2_BEA9_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsEdit window

class CDsCombo;
class CDsxView;
class CDsxDoc;
class CDsStdCtrl;
/*class CDsMetaCombo;*/
class CTablaView;

#ifdef DSPROFUIS
#define BASECDsEdit CExtEdit
#else
#define BASECDsEdit CEdit
#endif
class CDsEdit : public BASECDsEdit 
{
   // Construction
public:
   CDsEdit();
   
   // Attributes
public:
   int m_Attrib;
   int m_Cc;
   int m_Cf;
   BOOL m_justificado;
   CBrush    *m_pBkBrush;
   COLORREF  m_BkColor;
   COLORREF  m_FrColor;

   int m_teclaseleccion;

   // Operations
public:
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDsEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);	
	//}}AFX_VIRTUAL
   
   // Implementation
public:
	BOOL AjustaTamano();
    BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

	HWND m_otrofoco;

	CWnd* SetFocus( );
	virtual void ConfirmaFoco();
	BOOL m_fococonfirmado;
	BOOL m_no999;
	BOOL m_noConfirma;
	BOOL m_seleccionado;

	BOOL EnsureVisible();
	BOOL TeclaStd(int tecla);
	int ProcesaPtec(int ptec);
	CDsEdit(BOOL multilinea);
	BOOL m_multilinea;


    void GetWindowText( CString& rString);
    int GetWindowText( LPTSTR lpszStringBuf, int nMaxCount );
    int GetWindowTextLength( );
    void SetWindowText( LPCTSTR lpszString );

	void corrigeFecha();
	void KillFocus();
   
   virtual ~CDsEdit();
   int m_maxqc;
   int m_qc;
   int m_qh;
   int m_tipo;
   int m_decimales;
   int m_flag;
   char m_qs[100];
   char m_qi[100];   
   BOOL m_overwrite;
   int m_desactivado;
   BOOL m_comborel;

   CDsStdCtrl  *m_dsctrl;

   CDsxView *m_pView;

   CDsxDoc  *m_pDoc;

   /*CDsMetaCombo *m_metacombo;*/

   CTablaView *m_entabla;

   DWORD m_estilo; 
   CDsCombo* m_pCombo;

   void SetCombo (CDsCombo* pCombo)
   {
		m_pCombo = pCombo;
   }

   CDsCombo* GetCombo ()
   {
		return m_pCombo;
   }
   
   // Generated message map functions
protected:
   //{{AFX_MSG(CDsEdit)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG	
   
   DECLARE_MESSAGE_MAP()
};

// 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSEDIT_H__F1E66721_E2D3_11D2_BEA9_004F4905CBCF__INCLUDED_)
