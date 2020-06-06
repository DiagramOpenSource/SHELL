// DsxCntrItem.h : interface of the CDsxCntrItem class
//

#if !defined(AFX_DSXCNTRITEM_H__F5BA10B4_6134_4BBD_AD5E_50022F008D15__INCLUDED_)
#define AFX_DSXCNTRITEM_H__F5BA10B4_6134_4BBD_AD5E_50022F008D15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDsxDoc;
class CDsxView;

class CDsxCntrItem : public COleDocObjectItem
{
	DECLARE_SERIAL(CDsxCntrItem)

// Constructors
public:
	CDsxCntrItem(CDsxDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CDsxDoc* GetDocument()
		{ return (CDsxDoc*)COleDocObjectItem::GetDocument(); }
	CDsxView* GetActiveView()
		{ return (CDsxView*)COleDocObjectItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDsxCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL CanActivate();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDsxCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSXCNTRITEM_H__F5BA10B4_6134_4BBD_AD5E_50022F008D15__INCLUDED_)
