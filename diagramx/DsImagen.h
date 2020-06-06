#if !defined(AFX_DSIMAGEN_H__C47465F4_6294_4696_96A4_0B4A7B4DB616__INCLUDED_)
#define AFX_DSIMAGEN_H__C47465F4_6294_4696_96A4_0B4A7B4DB616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DsImagen.h : header file
//



class CDsStdCtrl;

/////////////////////////////////////////////////////////////////////////////
// CDsImagen window

class CDsImagen : public CExtScrollWnd
{
// Construction
public:
	CDsImagen();

// Attributes
public:
   BOOL m_dentro;
   CDsStdCtrl *m_pstdctrl;

   CString m_path;
   HBITMAP m_hbmp;
   HBITMAP m_abmp;
   CSize m_totalsize;

   CPoint m_posicion;

   BOOL m_conscroll;
// Operations
public:

	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const
	{
		nDirection;
		return CSize( 1, 1 );
	}
	virtual CSize OnSwGetLineSize( int nDirection ) const
	{
		nDirection;
		return CSize( 1, 1 );
	}
	
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
	);

	void MueveScroll(int x,int y);
	char *SacaCoordenadas(char *p,int &px,int &py);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsImagen)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadImage(LPCSTR path,int scale,int ipx = 0,int ipy = 0);
	BOOL LoadImage(LPCSTR path);
	virtual ~CDsImagen();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
protected:
	//{{AFX_MSG(CDsImagen)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSIMAGEN_H__C47465F4_6294_4696_96A4_0B4A7B4DB616__INCLUDED_)
