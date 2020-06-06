#if !defined(AFX_DSHTML_H__532B4813_BD39_11D3_B2E9_004F4905CBCF__INCLUDED_)
#define AFX_DSHTML_H__532B4813_BD39_11D3_B2E9_004F4905CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsHtml.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDsHtml html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CDsxDoc;
class CDsxView;

class CDsHtml : public CHtmlView
{
public:
    CDsHtml();           // protected constructor used by dynamic creation
protected:	
	DECLARE_DYNCREATE(CDsHtml)

    CDsxDoc* GetDocument();

    BOOL m_iniciado;

    CDsxView * m_pView;

// html Data
public:
	//{{AFX_DATA(CDsHtml)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:   

// Operations
public:
    void Navigate2( LPCTSTR lpszURL, DWORD dwFlags = 0, LPCTSTR lpszTargetFrameName = NULL, LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL, DWORD dwPostDataLen = 0 );


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsHtml)
	public:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Navega();
	virtual ~CDsHtml();
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDsHtml)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSHTML_H__532B4813_BD39_11D3_B2E9_004F4905CBCF__INCLUDED_)
