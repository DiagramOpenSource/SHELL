#if !defined(AFX_DSCOMBO_H__E59E9F33_BC58_40A1_B23A_E23B224B59A7__INCLUDED_)
#define AFX_DSCOMBO_H__E59E9F33_BC58_40A1_B23A_E23B224B59A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsCombo.h : header file
//

#include "DsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDsCheckBox window
class CDsCombo;

#ifdef DSPROFUIS
#define BASECDsCheckBox CButton
#else
#define BASECDsCheckBox CButton
#endif


class CDsCheckBox : public BASECDsCheckBox
{
// Construction
public:
	CDsCheckBox();

// Attributes
public:	
	BOOL m_noclick;


// Operations
public:	
	void ConfirmaFoco();

// Overrides
	CWnd* SetFocus( );
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsCheckBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsCheckBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsCheckBox)
	afx_msg void OnClicked();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);	
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CDsCombo;

class CDsComboDropWnd  : public CExtWRB <  CWnd >
{
friend class CDsCombo;

enum CloseReason { e_Cancel, e_Select, e_KillFocus };

// Construction
public:
	CDsComboDropWnd();

// Attributes
protected:
	void SetCombo (CDsCombo* pCombo)
	{
		m_pCombo = pCombo;
	}

	CDsCombo* GetCombo ()
	{
		return m_pCombo;
	}

	BOOL GetDroppedState () { return m_bDropped; }

	void InitDisplay () { m_CloseReason = e_KillFocus; }


protected:
	CDsCombo* m_pCombo;	
	CloseReason m_CloseReason;
	BOOL        m_bDropped;

protected:

	void OnSelection ();

// Operations
public:


// Overrides
	//afx_msg 
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsComboDropWnd)	
	protected:	
	virtual void OnSetFocus( CWnd* pOldWnd );
	virtual void OnKillFocus( CWnd* pNewWnd );
	virtual int OnVKeyToItem( UINT nKey, CListBox* pListBox, UINT nIndex );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsComboDropWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDsComboDropWnd)	
	//}}AFX_MSG
    void OnKillFocusList();
	void OnSetFocusList();
	void OnDblClkList();
	void OnSelChangeList();
	void OnKeyList();


	DECLARE_MESSAGE_MAP()
};


class CDsComboEdit : public CDsEdit
{
   friend class CDsCombo;
   friend class CDsComboDropWnd;

// Construction
public:
	CDsComboEdit();

// Attributes
public:
	virtual void ConfirmaFoco();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsComboEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Copia(CDsEdit *pEdit);
	virtual ~CDsComboEdit();

    
	// Generated message map functions
protected:
	//{{AFX_MSG(CDsComboEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//afx_msg void OnSetfocus();	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnAltDown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CDsCombo window
class CDsCheckBox;
class CDsStdCtrl;
#define DIALOGOCALENDARIO
#include "DsCalendar.h"

#ifdef CALENDARIOPROUIS
#define CBASEMonthCalCtrl CExtDatePickerWnd
#else
#define CBASEMonthCalCtrl CMonthCalCtrl
#endif

class CDSMonthCalCtrl : public CBASEMonthCalCtrl
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_MESSAGE_MAP()
};

class CDsCalendario : public CWnd
{
public:
	CDsCalendario()
	{
		m_pedit = NULL;
	};
	virtual ~CDsCalendario()
	{
	};
	CDsComboEdit *m_pedit;
#ifdef CALENDARIOPROUIS
	CBASEMonthCalCtrl m_cal;
#else
	CDSMonthCalCtrl m_cal;
#endif
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:		
	afx_msg void OnSelCalendario(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};

#define CBASEMICOMBO CWnd

class CDsCombo : public CExtWRB < CBASEMICOMBO  >
{
  friend class CDsComboDropWnd;
  friend class CDsComboEdit;
  friend class CDsCheckBox;
  friend class CDsStdCtrl;
  friend class CDsEdit;

// Construction
public:
	CDsCombo();
	virtual ~CDsCombo();

// Attributes
public:
//Child control access functions
	CDsComboDropWnd& GetDrop ()           { return m_Drop; }
	CDsComboEdit& GetEdit ()              { return m_Edit; }
	CBitmapButton& GetDropDownButton () { return m_DropDownButton; }

	CDsCheckBox m_button[20];
	int         m_nbutton;

	int  SetDroppedHeight (UINT nHeight);
	int  GetDroppedHeight ();
#ifdef DIALOGOCALENDARIO
	CDsCalendar *m_calendario;
#else
	CDsCalendario *m_calendario;
#endif
	

//CComboBox emulation functions
	void SetWindowText (LPCTSTR Text,BOOL tipohint = FALSE);
	CString GetWindowText ();
	int GetWindowTextLength( );
	void GetWindowText (CString &text);
	int GetWindowText( LPTSTR lpszStringBuf, int nMaxCount );
	void ShowDropDown( BOOL bShowIt = TRUE );
    void ResetContent ();
	int  SetDroppedWidth( UINT nWidth );
	int  GetDroppedWidth( );
	BOOL GetDroppedState( );	
	void GetDroppedControlRect (LPRECT pRect);
	CWnd* SetFocus( );
	

	void SetFont( CFont* pFont, BOOL bRedraw = TRUE );  
	
    int AddString ( LPCTSTR lpszString, int maxl = 0);

	void MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );

    int SelectString( int item, LPCTSTR lpszString);
    virtual BOOL TeclaStd(int tecla);

	BOOL ModifyStyleEx( DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 );

// Operations
public:
	BOOL m_concombo;
	BOOL m_esrelacion;
	BOOL m_lista;
	BOOL m_autocompletar;
	CListBox m_lst;
	BOOL m_Creando;
	HWND m_OtraHwnd;	

	DWORD m_editstyle;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	public:
	virtual BOOL SubclassDlgItem (UINT nID, CWnd* pParent);

// Implementation
public:
	void AjustaBotonRect();
	void AjustaEditRect();
	void AjustaCombo();
	void DeleteBkBrush();
	BOOL SetReadOnly(BOOL bReadOnly);
	void Setestilo(DWORD estilo);
	DWORD Getestilo();
	int Getmaxqc();
	void SetLimitText(UINT nMax);
	void SetMargins(UINT nleft,UINT nRight);
	void SetTabla(CTablaView *ptabla);
	CDsCombo(BOOL multilinea);
	int Getqc();
	char * Getqs();
	void Setqs(const char *qs);
	void Setqi(const char *qi);
	void Setdecimales(int deci);
	void Setqh(int qh);
	void Settipo(int tipo);
	void Setqc(int qc);
	void Setmaxqc(int maxqc);
	void SetFrColor(COLORREF color);
	void SetBkColor(COLORREF color);
	CBrush * GetBkBrush();
	void SetBkBrush(CBrush *pbrush);
	void SetDsCtrl(CDsStdCtrl *pctrl);
	void SetDoc(CDsxDoc *pDoc);
	void SetView(CDsxView *pView);
	int GetTipo();
	void SetSeleccionado(BOOL sel);
	int GetFlag();
	void SetFlag(int flag);
	int ProcesaPtec(int ptec);
	
	BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	BOOL AjustaTamano();

	BOOL m_desplegar;

	void SetDroppedState (BOOL bState);

	void PonEnAutocompletar(char *p,int nl);

	void EstableceAutocompletar(char *p,int nl);

	void UnDrop();
protected:
	void DisplayDrop();
	BOOL CreateDrop (DWORD extra = 0);
	BOOL CreateButton ();
	virtual BOOL CreateEdit ();
	

	LRESULT SendParentComboMessage ( UINT Action);
	void CalculateDroppedRect(LPRECT lpDroppedRect);
    void OnCancel ();

	BOOL IsMouseOnEdit(); 
	BOOL IsMouseOnButton(); 

	BOOL IsEditHighlightOn ();

	// Generated message map functions
protected:	
	//{{AFX_MSG(CDsCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	//}}AFX_MSG
	afx_msg BOOL DsEditToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult);

	void OnDropdownButton ();

	DECLARE_MESSAGE_MAP()

public:
	BOOL EsPosicional();
	BOOL SetAtributos(int Atrib,int Cc,int Cf);
	BOOL HaCambiado(int maxqc,int qc,int tipo,int decimales,char *pqi,char *pqs);
	CDsComboEdit    m_Edit;
	BOOL			m_Esde9;
protected:
	CDsComboDropWnd m_Drop;	
	//CDsButton m_DropDownButton;
	CBitmapButton   m_DropDownButton;
	CString         m_BeginPath;
	int             m_ComboWidth;
	int             m_ComboHeight;	
	int				m_iniComboHeight;
	BOOL            m_bDroppedState;
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSCOMBO_H__E59E9F33_BC58_40A1_B23A_E23B224B59A7__INCLUDED_)
