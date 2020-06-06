// DsxIpFrame.h : interface of the CDsxInPlaceFrame class
//

#if !defined(AFX_DSXIPFRAME_H__797A0E46_E74E_43B5_84EF_A77458F68D25__INCLUDED_)
#define AFX_DSXIPFRAME_H__797A0E46_E74E_43B5_84EF_A77458F68D25__INCLUDED_

#include "DsxMainFrm.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDsxInPlaceFrame : public COleDocIPFrameWnd
{
	DECLARE_DYNCREATE(CDsxInPlaceFrame)
public:
	CDsxInPlaceFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxInPlaceFrame)
	public:
	virtual BOOL OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void Serialize(CArchive& ar);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDsxInPlaceFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar   m_wndStatusBar;
	CMainToolBar m_wndToolBar;	
	CMenuBar     m_wndMenuBar;

	//CToolBar    m_wndToolBar;
	//CDialogBar m_wndDlgBar;
	//CReBar m_wndReBar;
	COleDropTarget	m_dropTarget;
	COleResizeBar   m_wndResizeBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CDsxInPlaceFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXIPFRAME_H__797A0E46_E74E_43B5_84EF_A77458F68D25__INCLUDED_)
