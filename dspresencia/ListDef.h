// ListDef.h: interface for the CListDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTDEF_H__0053E26F_F78A_43BB_A9D9_765D44B9A854__INCLUDED_)
#define AFX_LISTDEF_H__0053E26F_F78A_43BB_A9D9_765D44B9A854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CListDef : public wxListCtrl  
{
public:
	CListDef();
	virtual ~CListDef();

	FI_DEF *m_def;
	BOOL Rellena(void *datos,long len,BOOL comprimido,char *path = NULL);	
	int m_curcol;
	void OnSelected( wxListEvent &event );
	void OnDblClick( wxMouseEvent &event );
private:
    DECLARE_EVENT_TABLE()
};

#endif // !defined(AFX_LISTDEF_H__0053E26F_F78A_43BB_A9D9_765D44B9A854__INCLUDED_)
