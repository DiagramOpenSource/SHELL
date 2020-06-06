// Arbol.h: interface for the Arbol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARBOL_H__3D51BFB4_74AF_40F1_95AB_CFC67D7F7CA8__INCLUDED_)
#define AFX_ARBOL_H__3D51BFB4_74AF_40F1_95AB_CFC67D7F7CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CItemData : public wxTreeItemData
{
public:
	CItemData(int departamento)
	{
		m_departamento = departamento;
	}
	virtual ~CItemData()
	{
	}
    int m_departamento;
};

class CArbol : public wxTreeCtrl  
{
public:
	CArbol();
	virtual ~CArbol();
	void OnSelect(wxTreeEvent &event);
	CListaUsers *m_users;
	BOOL EstaIncluida(long ip);
	BOOL SeleccionaDepartamento(int departamento);
	BOOL BuscaSubdepartamento(wxTreeItemId itemroot,int departamento,BOOL borrar);
private:
    DECLARE_EVENT_TABLE()
};

#endif // !defined(AFX_ARBOL_H__3D51BFB4_74AF_40F1_95AB_CFC67D7F7CA8__INCLUDED_)
