// DlgUsuarios.cpp : implementation file
//

#include "stdafx.h"
#include "Diagramx.h"
#include "DlgUsuarios.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUsuarios dialog


CDlgUsuarios::CDlgUsuarios(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDlgUsuarios::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUsuarios)
	m_mensaje = _T("");
	//}}AFX_DATA_INIT
	m_matapid = -1;
	m_modosale = 0;
	DLGCONS;
}


void CDlgUsuarios::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUsuarios)
	DDX_Control(pDX, IDC_LISTUSERS, m_GridCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_mensaje);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUsuarios, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgUsuarios)
	ON_BN_CLICKED(IDC_BUTTON1, OnEnviar)
	ON_BN_CLICKED(IDC_BUTTON2, OnBorrar)
	ON_BN_CLICKED(IDUSUREFRESCA, OnUsurefresca)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUsuarios message handlers

BOOL CDlgUsuarios::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
    m_GridCtrl.PrepareControl(0);

	LV_ITEM			lvitem;
	LV_COLUMN		lvcolumn;
	CRect rect;
	m_GridCtrl.GetWindowRect( &rect );

	int longi = 0;

	// Insert columns using the order field
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Pid";
	lvcolumn.iSubItem = 0;
	lvcolumn.iOrder = 0;
	lvcolumn.cx = 40;
	longi += lvcolumn.cx;
	m_GridCtrl.InsertColumn(0, &lvcolumn);


	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Cpu";
	lvcolumn.iSubItem = 1;
	lvcolumn.iOrder = 1;
	lvcolumn.cx = 35;
	longi += lvcolumn.cx;
	m_GridCtrl.InsertColumn(1, &lvcolumn);
	

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Usuario";
	lvcolumn.iSubItem = 2;
	lvcolumn.iOrder = 2;
	lvcolumn.cx = 70;
	longi += lvcolumn.cx;
	m_GridCtrl.InsertColumn(2, &lvcolumn);


	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Localizacion";
	lvcolumn.iSubItem = 3;
	lvcolumn.iOrder = 3;
	lvcolumn.cx = 80;
	longi += lvcolumn.cx;
	m_GridCtrl.InsertColumn(3, &lvcolumn);


	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Fecha";
	lvcolumn.iSubItem = 4;
	lvcolumn.iOrder = 4;
	lvcolumn.cx = 70;
	longi += lvcolumn.cx;
	m_GridCtrl.InsertColumn(4, &lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Hora";
	lvcolumn.iSubItem = 5;
	lvcolumn.iOrder = 5;
	lvcolumn.cx = 70;
	longi += lvcolumn.cx;

	m_GridCtrl.InsertColumn(5, &lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Rtme";
	lvcolumn.iSubItem = 6;
	lvcolumn.iOrder = 6;
	lvcolumn.cx = 60;
	longi += lvcolumn.cx;

	m_GridCtrl.InsertColumn(6, &lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Ejecutable";
	lvcolumn.iSubItem = 7;
	lvcolumn.iOrder = 7;
	lvcolumn.cx = 80;
	longi += lvcolumn.cx;

	m_GridCtrl.InsertColumn(7, &lvcolumn);

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Opcion";
	lvcolumn.iSubItem = 8;
	lvcolumn.iOrder = 8;
	lvcolumn.cx = rect.Width( ) - longi - 4;
	longi += lvcolumn.cx;

	m_GridCtrl.InsertColumn(8, &lvcolumn);


	//122 000-000 'ds' LOCAL 05.05.1999 16:40:20 (9.09D);

	int i = 0,j = 0,iItem = 0;
	char tmp[1024];
	char tmp2[256];
	int cpu;

	


	for (i = 0;i < m_users.GetLength();i++)
	{
		if(m_users[i] == ';')
		{			
			int subnumero = 0;

			cpu = -1;

			strcpy(tmp,m_users.Mid(j,i-j));
			char *p = tmp,*p1;

			while(*p && *p != 7) p++;
			if (*p == 7)
			{
				cpu = atoi(p+1);
				*p = 0;
			}

			p = tmp;
		    while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}			

			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem;
			lvitem.iSubItem = 0;
			lvitem.iImage = 0;
			lvitem.pszText = tmp;
			m_GridCtrl.InsertItem(&lvitem);

            while(*p && *p != '-') p++;
			if (*p == '-')
			   {
			   p++;
			   p1 = p;
			   while(*p && *p != ' ') p++;
			   if (*p == ' ')
			     {
				 *p = 0;
				 p++;
				 subnumero = ::atoi(p1);
				 }
			   }

			while(*p && *p != '\'') p++;
			if (*p == '\'') 
			{
				p++;
				p1 = p;
				while(*p && *p != '\'') p++;
				if (*p == '\'') {*p = 0;p++;}
				while(*p == ' ') p++;
			}

			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 2;
			lvitem.iImage = 0;
			sprintf(tmp2,"%s(%d)",p1,subnumero+1);
			lvitem.pszText = tmp2;
			m_GridCtrl.SetItem(&lvitem);

			p1 = p;
		    while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 3;
			lvitem.iImage = 0;
			lvitem.pszText = p1;
			m_GridCtrl.SetItem(&lvitem);


			p1 = p;
		    while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 4;
			lvitem.iImage = 0;
			lvitem.pszText = p1;
			m_GridCtrl.SetItem(&lvitem);

			p1 = p;
  	        while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 5;
			lvitem.iImage = 0;
			lvitem.pszText = p1;
			m_GridCtrl.SetItem(&lvitem);

			p1 = p;
  	        while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 6;
			lvitem.iImage = 0;
			lvitem.pszText = p1;
			m_GridCtrl.SetItem(&lvitem);

			p1 = p;
  	        while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			/* dirbase */

			p1 = p;
  	        while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}


			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 7;
			lvitem.iImage = 0;
			lvitem.pszText = p1;
			m_GridCtrl.SetItem(&lvitem);

			/* saltar fecha hora opcion */
			p1 = p;			
  	        while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			p1 = p;
			/*
  	        while(*p && *p != ' ') p++;
			if (*p == ' ') {*p = 0;p++;}

			p1 = p;
			*/
			lvitem.mask = LVIF_TEXT;
		    lvitem.iItem = iItem;
			lvitem.iSubItem = 8;
			lvitem.iImage = 0;
			lvitem.pszText = p1;
			m_GridCtrl.SetItem(&lvitem);

			if (cpu > -1)
			{
				sprintf(tmp2,"%02d%%",cpu);

				lvitem.mask = LVIF_TEXT;
				lvitem.iItem = iItem;
				lvitem.iSubItem = 1;
				lvitem.iImage = 0;
				lvitem.pszText = tmp2;
				m_GridCtrl.SetItem(&lvitem);
			}


			j = i+1;
			iItem++;
		}
	}

	if (j < i)
	{
		m_mensaje = m_users.Mid(j);
		UpdateData(FALSE);
	}

	
	
	return TRUE;	              
}



void CDlgUsuarios::OnEnviar() 
{
	UpdateData(TRUE);
	m_modosale = 1;
    m_destinos = "";

	POSITION pos = m_GridCtrl.GetFirstSelectedItemPosition( );
	if (!pos)
	{
		for (int nItem = 0;nItem < m_GridCtrl.GetItemCount();nItem++)
		{
			  LVITEM Item;
			  char buf[512];
			  memset(&Item,0,sizeof(LVITEM));
			  Item.mask  = LVIF_TEXT;
			  Item.iItem = nItem;  
			  buf[0] = 0;
			  Item.pszText = buf;
			  Item.cchTextMax = sizeof(buf)-1;
			  m_GridCtrl.GetItem( &Item );
			  if (buf[0])         
			  {
				  m_destinos += buf;
				  m_destinos += ';';
			  }		
		}
	}
	else
	while(pos)
	{
		  int nItem = m_GridCtrl.GetNextSelectedItem(pos);
		  LVITEM Item;
		  char buf[512];
		  memset(&Item,0,sizeof(LVITEM));
		  Item.mask  = LVIF_TEXT;
		  Item.iItem = nItem;  
		  buf[0] = 0;
		  Item.pszText = buf;
		  Item.cchTextMax = sizeof(buf)-1;
		  m_GridCtrl.GetItem( &Item );
		  if (buf[0])         
		  {
			  m_destinos += buf;
			  m_destinos += ';';
		  }		
	}	
	EndDialog(IDOK);
}

void CDlgUsuarios::OnBorrar() 
{
	UpdateData(TRUE);
	m_mensaje = "";
	
	POSITION pos = m_GridCtrl.GetFirstSelectedItemPosition( );
	while(pos)
	{
		  int nItem = m_GridCtrl.GetNextSelectedItem(pos);
		  m_GridCtrl.SetItemState( nItem, !LVIS_SELECTED , LVIS_SELECTED );
	}	

	UpdateData(FALSE);
}

void CDlgUsuarios::OnUsurefresca() 
{
	UpdateData(TRUE);
	m_modosale = 2;
	EndDialog(IDOK);
}
