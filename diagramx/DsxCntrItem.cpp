// DsxCntrItem.cpp : implementation of the CDsxCntrItem class
//

#include "stdafx.h"
#include "diagramx.h"

#include "DsxDoc.h"
#include "dsxView.h"
#include "DsxCntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsxCntrItem implementation

IMPLEMENT_SERIAL(CDsxCntrItem, COleDocObjectItem, 0)

CDsxCntrItem::CDsxCntrItem(CDsxDoc* pContainer)
	: COleDocObjectItem(pContainer)
{
	// TODO: add one-time construction code here
	
}

CDsxCntrItem::~CDsxCntrItem()
{
	// TODO: add cleanup code here
	
}

void CDsxCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleDocObjectItem::OnChange(nCode, dwParam);

	// When an item is being edited (either in-place or fully open)
	//  it sends OnChange notifications for changes in the state of the
	//  item or visual appearance of its content.

	// TODO: invalidate the item by calling UpdateAllViews
	//  (with hints appropriate to your application)

	GetDocument()->UpdateAllViews(NULL);
		// for now just update ALL views/no hints
}

BOOL CDsxCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// During in-place activation CDsxCntrItem::OnChangeItemPosition
	//  is called by the server to change the position of the in-place
	//  window.  Usually, this is a result of the data in the server
	//  document changing such that the extent has changed or as a result
	//  of in-place resizing.
	//
	// The default here is to call the base class, which will call
	//  COleDocObjectItem::SetItemRects to move the item
	//  to the new position.

	if (!COleDocObjectItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: update any cache you may have of the item's rectangle/extent

	return TRUE;
}


void CDsxCntrItem::OnActivate()
{
}

void CDsxCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleDocObjectItem::OnDeactivateUI(bUndoable);

    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CDsxCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class first to read in COleDocObjectItem data.
	// Since this sets up the m_pDocument pointer returned from
	//  CDsxCntrItem::GetDocument, it is a good idea to call
	//  the base class Serialize first.
	COleDocObjectItem::Serialize(ar);

	// now store/retrieve data specific to CDsxCntrItem
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

BOOL CDsxCntrItem::CanActivate()
{
	// Editing in-place while the server itself is being edited in-place
	//  does not work and is not supported.  So, disable in-place
	//  activation in this case.
	CDsxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc)));
	if (pDoc->IsInPlaceActive())
		return FALSE;

	// otherwise, rely on default behavior
	return COleClientItem::CanActivate();
}

/////////////////////////////////////////////////////////////////////////////
// CDsxCntrItem diagnostics

#ifdef _DEBUG
void CDsxCntrItem::AssertValid() const
{
	COleDocObjectItem::AssertValid();
}

void CDsxCntrItem::Dump(CDumpContext& dc) const
{
	COleDocObjectItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
