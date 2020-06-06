// ListaUsers.cpp: implementation of the CListaUsers class.
//
//////////////////////////////////////////////////////////////////////

#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif
#include "wx/wxprec.h"
#include "dspresenciaApp.h"
#include "ListaUsers.h"


BEGIN_EVENT_TABLE(CListaUsers, wxListCtrl)    
   EVT_LIST_ITEM_SELECTED(LIST_CTRL, CListaUsers::OnSelect)	
   EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, CListaUsers::OnActivate)	
   EVT_LIST_BEGIN_DRAG(LIST_CTRL, CListaUsers::OnLDrag)	
   EVT_LIST_BEGIN_RDRAG(LIST_CTRL, CListaUsers::OnRDrag)	
   EVT_LEFT_DOWN(CListaUsers::OnLeftDown)
   EVT_RIGHT_DCLICK(CListaUsers::OnRDClick)
   EVT_KEY_DOWN(CListaUsers::OnKeyDown)   
   EVT_DROP_FILES(CListaUsers::OnDropFiles)	
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListaUsers::CListaUsers()
{
	m_padre = NULL;
}

CListaUsers::~CListaUsers()
{

}

void CListaUsers::OnDropFiles(wxDropFilesEvent& event)
{
	int i,j;
	long item;
	unsigned long ip;
	wxString ss;
	int flags;
	wxPoint pp;

	POINT wp;
	::GetCursorPos(&wp);
	pp.x = wp.x;
	pp.y = wp.y;
	pp = ScreenToClient(pp);
	flags = wxLIST_HITTEST_ONITEM;	
	item = HitTest(pp, flags);
	if (item >= 0 && item < GetItemCount())
	{
		ip = GetItemData(item);
		m_padre->ActivaMensaje(ip,TRUE,FALSE);
		for (i = 0;i < 500;i++)
		{
			if (m_padre->m_dlgmensaje[i] && m_padre->m_dlgmensaje[i]->m_ip == (long)ip)
			{
				break;
			}
		}
		if (i < 500)
		{
			ss = "¿Enviar a ";
			ss += m_padre->m_dlgmensaje[i]->GetLabel();
			ss += " ";
			for (j = 0 ;j  < event.m_noFiles;j++)
			{
				ss += event.m_files[j];
				if (j < (event.m_noFiles-1))
					ss += ";";
				else
					ss += "?";
			}
			if (::MessageBox((HWND)GetHandle(),ss,"Envio de ficheros",MB_YESNO|MB_TOPMOST|MB_ICONQUESTION) == IDYES)
			{
				for (j = 0 ;j  < event.m_noFiles;j++)
				{
					m_padre->m_dlgmensaje[i]->EnviaFile(event.m_files[j]);
				}
			}
		}
	}
	//wxPoint m_pos
	//wxString* GetFiles() const
    //int GetNumberOfFiles() const
	// wxPoint GetPosition() const
}

bool CListaUsers::PonEstado(char **_p,int *elprivilegio,wxString *pdestado,int *prevestado,unsigned long userflag)
{
	bool cambia = false;	
	char *p = *_p;
	long ip = *((long *)p);

	//LockWindowUpdate((HWND)GetHandle());

	// Tamaño usado de mensaje:
	// sizeof(long)+sizeof(int)*4+
	// strlen(p+sizeof(int)*2)+1 ...y le sigue otro string
	//
	p += sizeof(long);
	int estado = *((int *)p);
	int flag = estado & 0xffff0000;
	flag >>= 16;
	estado &= 0xffff;
	p += sizeof(int);
	int privilegio = *((int *)p);
	p += sizeof(int);
	wxString nombre = p;
	p += (strlen(p)+1);
	wxString destado = p;
	p += (strlen(p)+1);
	int eltipocliente = *((int *)p);
	p += sizeof(int);
	int departamento = *((int *)p);
	p += sizeof(int);
	*_p = p;

	if (pdestado)
	{
		*pdestado = destado;
	}
	long nitems = GetItemCount();	

	{
		int k;
		if ((k = nombre.Find(1)) > -1)
		{
			nombre.Truncate(k);
		}
	}

	wxString desc;

	/*
	if (ip == dame_the_name())
	{
		if (*elprivilegio != privilegio)
		{
			int i;
			*elprivilegio = privilegio;
			for (i = 0;i < 500;i++)
			{
				if (!m_padre->m_faltaimagen[i] && m_padre->m_imagenes[i])
				{
					m_padre->PonImagenEstado(i);
				}
			}
		}		
		if (m_padre)
		{
			m_padre->m_minombre = nombre;
			m_padre->m_estado = (enum ElEstado)estado;
			m_padre->m_destado = destado;
			elflag = flag;
			m_padre->ActualizaIcono();
		}		
	}
	*/

	/*
	if (*elprivilegio > 29)
	{
		if (estado == 2)
		{
		   destado = "!"+destado;
		}
	}
	*/
	
	desc = nombre;
	desc += (char)_charseparador;	
	desc += destado;	

	//wxString tmpn;
	
	bool reordenar = false;

	for (long i = 0;i < nitems;i++)
	{
		if (GetItemData(i) == ip)
		{	
			
			/*
			tmpn = GetItemText(i);
			if (nombre.Cmp())
			{
				reordenar = true;
			}
			*/
			cambia = false;
			if (desc.Cmp(GetItemText(i)))
			{
				cambia = true;
			}
			else
			{
				if (m_estados[i] != estado)
				{
					cambia = true;
				}
			}
			if (m_flags[i] != flag || m_tipos[i] != eltipocliente)
			{
				cambia = true;
			}			

			if (prevestado)
			{
				*prevestado = (int)m_estados[i];
			}
			m_flags[i] = flag;
			m_estados[i] = estado;			
			m_tipos[i] = eltipocliente;
			m_departamento[i] = departamento;
			m_userflags[i] = userflag;
			if (cambia)
			{
				SetItem(i,0, desc, DameImagen(ip,estado,flag,eltipocliente));
			}
			/*
			if (cambia && m_padre && (GetItemState(i,wxLIST_STATE_SELECTED) & wxLIST_STATE_SELECTED))
			{
				m_padre->InfoItem(i,this);
			}
			*/
			break;
		}
	}
	if (i >= nitems)
	{
		if (m_padre)
		{
			m_padre->m_textoestado.SetValue("");
		}
		if ((eltipocliente & TIPO_OCULTO))
		{
			i = i;
		}
		InsertItem(i,desc,DameImagen(ip,estado,flag,eltipocliente));
		m_estados[i] = estado;
		m_userflags[i] = userflag;
		m_flags[i] = flag;
		m_departamento[i] = departamento;
		m_tipos[i] = eltipocliente;
		SetItemData(i,ip);
		reordenar = true;
	}	
	if (reordenar)
	{
		OrdenaItems();
		cambia = true;
	}
	//LockWindowUpdate((HWND)NULL);
	if (cambia)
	{
		InvalidateRect((HWND)GetHandle(),NULL,FALSE);
		UpdateWindow((HWND)GetHandle());
	    if (m_padre->m_conferencia && m_padre->m_conferencia->m_propietario)
		{
			CListaUsers *pusers = m_padre->m_conferencia->m_users;
			if (pusers != this)
			{
				if (pusers->EstadoPorIp(ip,estado,flag) > -1)
				{					
					HWND hwnd = (HWND)pusers->GetHandle();
					InvalidateRect(hwnd,NULL,FALSE);
					UpdateWindow(hwnd);
				}
			}
		}
	}


	return cambia;
}


void CListaUsers::OrdenaItems()
{
	wxString s1;
	int nitems = GetItemCount();
	int notra = 0;
	wxString desc[500];
	int estados[500];
	long ips[500];	
	unsigned long userflags[500];
	int flags[500];
	int tipos[500];
	int departamento[500];

	for (int i = 0;i < nitems;i++)
	{
		s1 = GetItemText(i);
		for (int j = 0;j <notra;j++)
		{
			if (s1.Cmp(desc[j]) < 0)
			{
				break;
			}			
		}
		for (int k = notra;k > j;k--)
		{
			desc[k] = desc[k-1];
			estados[k] = estados[k-1];
			userflags[k] = m_userflags[k-1];
			flags[k] = m_flags[k-1];
			departamento[k] = m_departamento[k-1];
			tipos[k] = m_tipos[k-1];
			ips[k] = ips[k-1];
		}
		desc[j] = s1;
		estados[j] = m_estados[i];
		userflags[j] = m_userflags[i];
		flags[j] = m_flags[i];
		departamento[j] = m_departamento[i];
		tipos[j] = m_tipos[i];
		ips[j] = GetItemData(i);
		notra++;
	}
	ClearAll();
	for (i = 0;i < notra;i++)
	{
		InsertItem(i,desc[i],DameImagen(ips[i],estados[i],flags[i],tipos[i]));
		m_estados[i] = estados[i];
		m_userflags[i] = userflags[i];
		m_flags[i] = flags[i];
		m_departamento[i] = departamento[i];
		m_tipos[i] = tipos[i];
		SetItemData(i,ips[i]);
	}
}

void CListaUsers::OnSelect(wxListEvent& event)
{
	/*
	int item = event.GetIndex();	
	if (m_padre)
	{
		m_padre->InfoItem(item,this);		
	    wxString txt = GetItemText(item);	    	    
		int pi = txt.Find(_charseparador);
		if (pi > -1)
		{
			m_padre->m_selNombre = txt.Mid(0,pi);
		}
		m_padre->PonSelIp(GetItemData(item));
	}
	*/
}

int CListaUsers::EstadoPorIp(long ip,int estado,int flag)
{
	int nitems = GetItemCount();
	for (int i = 0;i < nitems;i++)
	{
		if (GetItemData(i) == ip)
		{
			if (m_estados[i] != -1)
			{
				m_estados[i] = estado;
				m_flags[i] = flag;
			}
			return i;
		}
	}
	return -1;
}

bool CListaUsers::CreaItem(wxString &Nombre,long ip,int estado,int flag,int tipo,int departamento,unsigned long userflag)
{
	bool cambia = false;		
	
	int privilegio = 0;

	long nitems = GetItemCount();	

	wxString desc = Nombre;


	
	bool reordenar = false;

	for (long i = 0;i < nitems;i++)
	{
		if (GetItemData(i) == ip)
		{				
			cambia = false;
			if (desc.Cmp(GetItemText(i)))
			{
				cambia = true;
			}
			else
			{
				if (m_estados[i] != estado)
				{
					cambia = true;
				}
			}

			if (flag != m_flags[i] || tipo != m_tipos[i])
			{
				cambia = true;
			}
			
			m_estados[i] = estado;
			m_flags[i] = flag;
			m_tipos[i] = tipo;
			m_departamento[i] = departamento;
			m_userflags[i] = userflag;
			if (cambia)
			{
				SetItem(i,0, desc, DameImagen(ip,estado,flag,tipo));
			}
			break;
		}
	}
	if (i >= nitems)
	{
		InsertItem(i,desc,DameImagen(ip,estado,flag,tipo));
		m_estados[i] = estado;
		m_userflags[i] = userflag;
		m_flags[i] = flag;
		m_tipos[i] = tipo;
		m_departamento[i] = departamento;
		SetItemData(i,ip);
		//reordenar = true;
	}	
	if (reordenar)
	{
		OrdenaItems();
		cambia = true;
	}
	if (cambia)
	{
		InvalidateRect((HWND)GetHandle(),NULL,FALSE);
		UpdateWindow((HWND)GetHandle());
	}
	return cambia;
}


void CListaUsers::OnLDrag(wxListEvent& event)
{
	int item = event.GetIndex();	
	if (item >= 0)
	{		
		if (&(m_padre->m_listausers) == this)
		{
			m_padre->QuitaFavorito(GetItemData(item));
		}
		else
		{
			m_padre->PonFavorito(GetItemData(item));
		}
	}
}


void CListaUsers::OnRDrag(wxListEvent& event)
{
	int item = event.GetIndex();	
	if (item >= 0)
	{		
		if (m_padre->m_conferencia && m_padre->m_conferencia->m_propietario == dame_the_name())
		{
			if (GetItemData(item) == dame_the_name())
				return;
			m_padre->PonEnConferencia(GetItemData(item),(BOOL)(m_padre->m_conferencia->m_users == this));
			return;
		}
		if (&(m_padre->m_listausers) == this)
		{
			m_padre->QuitaFavorito(GetItemData(item));
		}
		else
		{
			m_padre->PonFavorito(GetItemData(item));
		}
	}
}

void CListaUsers::OnActivate(wxListEvent& event)
{
	int item = event.GetIndex();	

	if (m_padre)
	{
		m_padre->ActivaMensaje(GetItemData(item),FALSE,TRUE);
	}
}

void CListaUsers::OnLeftDown(wxMouseEvent& event)
{
	{
		int item;
		int flags = wxLIST_HITTEST_ONITEM;
		wxPoint point = event.GetPosition();
		item  = HitTest(point, flags);
		if (m_padre && item >= 0 && item < GetItemCount())
		{
			m_padre->InfoItem(item,this);		
			wxString txt = GetItemText(item);	    			
			int pi = txt.Find(_charseparador);
			if (pi > -1)
			{
				m_padre->m_selNombre = txt.Mid(0,pi);
			}
			m_padre->PonSelIp(GetItemData(item));
		}
	}
	event.Skip();
}

void CListaUsers::OnRDClick(wxMouseEvent& event)
{	
	/*
	static int uno = 1;
	if (uno)
	*/
	{
		int item;
		int flags = wxLIST_HITTEST_ONITEM;
		wxPoint point = event.GetPosition();
		item  = HitTest(point, flags);
		if (m_padre && item >= 0 && item < GetItemCount())
		{
			m_padre->OtroEstado(item,this);	
		}
	}
	/*
	else
	{
		uno++;
		AddPendingEvent(event);
	}
	*/
	event.Skip();
}	


int CListaUsers::PorIp(long ip,wxString &nombre)
{
	int nitems = GetItemCount();
	for (int i = 0;i < nitems;i++)
	{
		if (GetItemData(i) == ip)
		{	
			nombre = GetItemText(i);
			int pi = nombre.Find(_charseparador);
			if (pi > -1)
			{
				nombre = nombre.Mid(0,pi);
			}
			return i;
		}
	}
	return -1;
}

int CListaUsers::DameImagen(long ip,int estado,int flag,int tipo)
{
	if (m_padre)
	{
		return m_padre->DameImagen(ip,estado,flag,tipo);
	}
	return 0;
}


int CListaUsers::CambiaIP(long vieja, long nueva)
{
	int nitems = GetItemCount();
	for (int i = 0;i < nitems;i++)
	{
		if (GetItemData(i) == vieja)
		{	
			SetItemData(i,nueva);
			return i;
		}
	}
	return -1;
}

int CListaUsers::BorraIP(long vieja)
{
	int nitems = GetItemCount();
	int departamento = -1;
	for (int i = 0;i < nitems;i++)
	{
		if (GetItemData(i) == vieja)
		{
			nitems--;
			departamento = m_departamento[i];
			for (int j = i;j < nitems;j++)
			{
				m_estados[j] = m_estados[j+1];
				m_userflags[j] = m_userflags[j+1];
				m_flags[j] = m_flags[j+1];
				m_departamento[j] = m_departamento[j+1];
				m_tipos[j] = m_tipos[j+1];
			}
			DeleteItem(i);
			return i;
		}
	}
	return -1;
}

int CListaUsers::PorNombre(const char *Nombre, long &ip)
{
	wxString nombre;
	wxString nom = Nombre;
	{
		int k;
		if ((k = nom.Find(1)) > -1)
		{
			nom.Truncate(k);
		}
	}
	int nitems = GetItemCount();
	for (int i = 0;i < nitems;i++)
	{
		nombre = GetItemText(i);
		int pi = nombre.Find(_charseparador);
		if (pi > -1)
		{
			nombre = nombre.Mid(0,pi);
		}
		{
			int k;
			if ((k = nombre.Find(1)) > -1)
			{
				nombre.Truncate(k);
			}
		}
		if (!nombre.Cmp(nom))
		{
			ip = GetItemData(i);
			return i;
		}
	}
	return -1;
}

void CListaUsers::OnKeyDown(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_F5)
	{
		m_padre->EnviaComando(0);
	}
	else
	{
		event.Skip();
	}
}
