// Arbol.cpp: implementation of the Arbol class.
//
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#include "dspresenciaApp.h"
#include "DlgPresencia.h"
#include "ListaUsers.h"
#include "ListDef.h"
#include "resource.h"
#ifdef WIN32
#pragma warning(disable: 4228)
#include <mapi.h>
#pragma warning(default: 4228)
#endif
#include "Arbol.h"

BEGIN_EVENT_TABLE(CArbol, wxTreeCtrl)       
   EVT_TREE_SEL_CHANGED(-1,CArbol::OnSelect)	
END_EVENT_TABLE()



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CArbol::CArbol()
{

}

CArbol::~CArbol()
{

}

void CArbol::OnSelect(wxTreeEvent &event)
{
	wxTreeItemId item;
	CItemData *data;
	int departamento = 0;
	int estado,oe;
	int flag;
	int privilegio;
	int eltipocliente;
	wxString nombre,destado;
	
	m_users->DeleteAllItems();
	item = event.GetItem();
	data = (CItemData *)GetItemData(item);
	if (data)
	{
		int i;				
		for (i = 0;i < nlista_users;i++)
		{
			EstadoItem(lista_users[i],estado,flag,privilegio,nombre,destado,eltipocliente,departamento);
			if (data->m_departamento >= 93000 && data->m_departamento < 93999)
			{
			      if (departamento >= 90000)
				     continue;

				  oe = estado;
				  if (oe == 2)
					  oe = 0;
				  oe += 93000;
				  if (data->m_departamento == oe)
				  {
					  nombre += (char)_charseparador;	
					  nombre += destado;	
					  m_users->CreaItem(nombre,*((long *)lista_users[i]),estado,flag,eltipocliente,departamento,_userflags[i]);
				  }
				  continue;
			}

			/*
			{
				int k;
				if ((k = nombre.Find(1)) > -1)
				{
					nombre.Truncate(k);
				}
			}
			*/
			if ((data->m_departamento == -1 && departamento < 90000) || departamento == data->m_departamento ||
				(departamento < 90000 && data->m_departamento < 90000 && data->m_departamento >= 1000 && departamento >= 1000 && !(data->m_departamento%1000) && data->m_departamento/1000 == departamento/1000)
				)
			{				
				nombre += (char)_charseparador;	
				nombre += destado;	
				m_users->CreaItem(nombre,*((long *)lista_users[i]),estado,flag,eltipocliente,departamento,_userflags[i]);
			}
		}
	}
}

BOOL CArbol::EstaIncluida(long ip)
{  
  wxTreeItemId id = GetSelection();
  CItemData *data;
  int departamento = 0;    
  int flag;
  int privilegio;
  int eltipocliente;
  int estado,i,oe;
  unsigned long userflag;
  wxString nombre,destado;

  data = (CItemData *)GetItemData(id);
  if (data)
  {
	  if ((estado = EstadoPorIp(ip,flag,privilegio,nombre,destado,eltipocliente,departamento,userflag)) > -1)
	  {
		  if (data->m_departamento >= 93000 && data->m_departamento < 93999)
		  {
			  if (departamento >= 90000)
				  return FALSE;

			  oe = estado;
			  if (oe == 2)
				  oe = 0;
			  oe += 93000;
			  if (data->m_departamento != oe)
			  {
				  m_users->BorraIP(ip);
				  return FALSE;
			  }			  
			  wxString nom;
			  if (m_users->PorIp(ip,nom) < 0)
			  {
				 int ListaPorIp(long de,wxString &nombre);

				 i = ListaPorIp(ip,nom);
				 if (i < 0) i = 0;

				 nombre += (char)_charseparador;	
				 nombre += destado;	

				 m_users->CreaItem(nombre,ip,estado,flag,eltipocliente,departamento,_userflags[i]);
			  }
			  return TRUE;
		  }

		  if ((data->m_departamento == -1 && departamento < 90000) || data->m_departamento == departamento)
		  {
			  return TRUE;
		  }
		  if (departamento < 90000 && data->m_departamento < 90000 && data->m_departamento >= 1000 && departamento >= 1000 && !(data->m_departamento%1000) &&
			  data->m_departamento/1000 == departamento/1000)
		  {
			  return TRUE;
		  }
	  } 
  }
  return FALSE;
}


BOOL CArbol::SeleccionaDepartamento(int departamento)
{
	long cookie;
	wxTreeItemId itemroot = GetRootItem();
	wxTreeItemId item;
	CItemData *data;

    item = GetFirstChild(itemroot, cookie);
	while(item)
	{
		data = (CItemData *)GetItemData(item);
		if (data && data->m_departamento == departamento)
		{
			SelectItem(item);
			return TRUE;
		}
		item = GetNextChild(itemroot, cookie);
	}
	return FALSE;
}


BOOL CArbol::BuscaSubdepartamento(wxTreeItemId itemroot,int departamento,BOOL borrar)
{
	long cookie = 0L;	
	wxTreeItemId item;
	CItemData *data;

    item = GetFirstChild(itemroot, cookie);
	while(item)
	{
		data = (CItemData *)GetItemData(item);
		if (data && data->m_departamento == departamento)
		{			
			if (borrar)
			{
				int dep1;
				int estado;
				int flag;
				int privilegio;
				int eltipocliente;
				wxString nombre,destado;				
				for (int i = 0;i < nlista_users;i++)
				{
					EstadoItem(lista_users[i],estado,flag,privilegio,nombre,destado,eltipocliente,dep1);
					if (dep1 == departamento)
						return TRUE;
				}
				Delete(item);
			}
			return TRUE;
		}
		item = GetNextChild(itemroot, cookie);
	}
	return FALSE;
}
