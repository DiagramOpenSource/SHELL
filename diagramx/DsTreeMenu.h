#if !defined(AFX_DSTREEMENU_H__6889B80A_981C_11D3_B2A3_004F4905CBCF__INCLUDED_)
#define AFX_DSTREEMENU_H__6889B80A_981C_11D3_B2A3_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsTreeMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsTreeMenu frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DialogBuscaMenu.h"

class CMiSplit    : public CSplitterWnd
{
public:
    CMiSplit();
	virtual ~CMiSplit();
	CDialogBuscaMenu *m_buscadlg;    
protected:
virtual void OnDrawSplitter(CDC* pDC, ESplitType nType,const CRect& rectArg);
COLORREF  m_clrBtnFace;
COLORREF  m_clrBtnShadow;
COLORREF  m_clrBtnHilite;
COLORREF  m_clrBtnText;
COLORREF  m_clrWindowFrame;
};

#ifdef DSPROFUIS
class CDsTreeMenu : public CExtWS < CView >
#else
class CDsTreeMenu : public CView
#endif
{
	DECLARE_DYNCREATE(CDsTreeMenu)
protected:
	CDsTreeMenu();           // protected constructor used by dynamic creation

// Attributes
protected:
public:
	CMiSplit    m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsTreeMenu)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDsTreeMenu();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDsTreeMenu)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSTREEMENU_H__6889B80A_981C_11D3_B2A3_004F4905CBCF__INCLUDED_)
