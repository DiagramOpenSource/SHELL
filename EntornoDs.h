
#ifdef YANOEXISTE

#if !defined(AFX_ENTORNODS_H__5E4B6413_88F6_11D2_BDF0_004F4905CBCF__INCLUDED_)
#define AFX_ENTORNODS_H__5E4B6413_88F6_11D2_BDF0_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntornoDs.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CEntornoDs window
class CDiagram9Doc;
class CDiagram9View;
class CLeftView;
class CMainFrame;
class CDsComando;
class CWindowDs;
class CThreadDs;
class CTabMenu;
class CPantalla;
class CDiagram9View;
class CPantalla;
class CDsEdit;
class CTablaView;
class CTablaW;

//Class CEntornoDs :: Public CTabCtrl
#define PADRECEntornoDs CWnd
class CEntornoDs : public CWnd
{
   friend  CWindowDs;
   friend  CThreadDs;
   friend  CMainFrame;
   friend  CDiagram9Doc;
   friend  CLeftView;
   friend  CTabMenu;
   friend  CDiagram9View;
   friend  CPantalla;
   friend  CDsEdit;
   friend  CTablaView;
   friend  CTablaW;
   
   // Construction
public:
   CEntornoDs(CDiagram9Doc *pDoc,CMainFrame *pMainFrame);
   
   int SetCurSel( int nItem );
   
   friend void RellenaMiMenu(MenuStruct *mi_menu,int defid);
   friend void PonEnWinsysMenu(int status);
   friend void RefrescaIra(void);
   friend void DespachoEnLineas(int modo,RECT *r);
   friend void CreaMiControl(int entero,char *puntero);
   friend void EliminaMiControl(int entero);
   friend void AvisoAControl(int entero);
   friend void SobreponePantalla(void);
   friend void ReponPantalla(void);
   friend void MensajeEnCabecera(int lado,char *mensaje);
   friend void FuncionesImpresion(int funcion,void *dato);
   friend void HazVisible(void);
   friend int  ActivarEdita(char *txt,int area);
   friend void ControlTrasmision(int entero,void *puntero);
   friend void EntradaBinaria(int entero,void *puntero);
   friend void LlamaPretranslate(MSG *msg);
   friend int  DesactivaControles(void);
   friend int  ActivaControles(void);

   friend int SinHebra(CEntornoDs *m_pEDs);

   
private:	
   void ApuntaLineaDeComando(int *dsargc,char *dsargv[],int maximo,char *exe,char *args);
   CString m_Args;
   CString m_Exe;
   int m_argc;
   char * m_argv[100];    
   CDiagram9View *m_pView;
   CLeftView     *m_pLeftView;
   CMainFrame    *m_pMainFrame;
   BOOL m_Autentificado;
   CThreadDs *m_Ejecucion;
   CRect m_rtab;
   BOOL m_EnEjecucion;
   CWindowDs * m_wallowed;
   
private:	
   CPantalla * m_LocalParent;
   CEntornoDs * m_pEntornoPrevio;
   int m_avisocontrol;
   int m_cursel;
   void PintaOpcion(int old_op,int op);
   HINSTANCE m_dswinsys;
   FARPROC m_fInicializa;
   FARPROC m_fFinaliza;
   FARPROC m_fLogin;
   FARPROC m_fOpcion;
   FARPROC m_fLogout;
   FARPROC m_fDevuelveVentana;
   FARPROC m_fNuevaVentana;
   FARPROC m_fPreparaEjecucion;
   FARPROC m_fpinta_zona;
   FARPROC m_fmensaje_char;
   FARPROC m_fmensaje_keydown;
   FARPROC m_fmensaje_keyup;
   FARPROC m_fmensaje_ldblclk;
   FARPROC m_fmensaje_lbuttondown;
   FARPROC m_fmensaje_mousemove;
   FARPROC m_fmensaje_lbuttonup;
   FARPROC m_fmensaje_command;
   FARPROC m_fDespachadorInterno;
   
   // Attributes
public:
   
   // Operations
public:
   
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CEntornoDs)
protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
   //}}AFX_VIRTUAL
   
   // Implementation
public:
    BOOL GetItemRect( int nItem, LPRECT lpRect );
	void SetColores();
	void FontGrid();
   void GuardaFonts();
   CDiagram9Doc  *m_pDoc;
   void FontDefault();
   void FontAuto();
   void FontInten();
   void FontTexto();
   void FontBoton();
   void FontMensa();
   void OtrosFonts();
   int m_wid;
   int m_tabid;
   CEntornoDs(CEntornoDs *Previo);
   void ReSize();   
   void CalculaRect();
   BOOL EnEjecucion();
   void Ejecuta(CDiagram9View *pView = NULL,CLeftView *pLeftView = NULL,CMainFrame *pMainFrame = NULL);
   BOOL LiberaUsuario();
   BOOL Autentifica();
   virtual ~CEntornoDs();
   
   // Generated message map functions
protected:
   //{{AFX_MSG(CEntornoDs)
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
   
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTORNODS_H__5E4B6413_88F6_11D2_BDF0_004F4905CBCF__INCLUDED_)

#endif