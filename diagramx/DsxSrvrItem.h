// DsxSrvrItem.h : interface of the CDsxSrvrItem class
//

#if !defined(AFX_DSXSRVRITEM_H__41A443BE_97DC_4D87_B8B6_8FE96F0D0063__INCLUDED_)
#define AFX_DSXSRVRITEM_H__41A443BE_97DC_4D87_B8B6_8FE96F0D0063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDsxSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CDsxSrvrItem)

// Constructors
public:
	CDsxSrvrItem(CDsxDoc* pContainerDoc);

// Attributes
	CDsxDoc* GetDocument() const
		{ return (CDsxDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDsxSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXSRVRITEM_H__41A443BE_97DC_4D87_B8B6_8FE96F0D0063__INCLUDED_)
