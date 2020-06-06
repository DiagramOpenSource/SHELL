#if !defined(AFX_STATICLABEL_H__BE520B86_C836_11D3_B2FC_004F4905CBCF__INCLUDED_)
#define AFX_STATICLABEL_H__BE520B86_C836_11D3_B2FC_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticLabel window


#ifdef DSPROFUIS
#define DSBASECStatic CExtLabel
#else
#define DSBASECStatic CStatic
#endif

class CStaticLabel : public DSBASECStatic
{
// Construction
public:
	CStaticLabel();

// Attributes
public:
   CFont        *m_font;
   CBrush       *m_pBkBrush;
   COLORREF     m_BkColor;
   COLORREF     m_FrColor;
   CDsxDoc      *m_pDoc;
   CDsStdCtrl   *m_dsctrl;
   char         *m_maparaya;
   int          m_maparaya_size;
   BOOL         m_group;
   int          m_Atrib;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	void PonFont();
	BOOL Atributo(int Atrib,int Cc,int Cf);
	virtual ~CStaticLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticLabel)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICLABEL_H__BE520B86_C836_11D3_B2FC_004F4905CBCF__INCLUDED_)
