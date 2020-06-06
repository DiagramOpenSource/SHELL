// DlgRegCambio.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DlgRegCambio.h"


// DlgRegCambio dialog

IMPLEMENT_DYNAMIC(DlgRegCambio, CBASEDLG)
DlgRegCambio::DlgRegCambio(CTablaView *pTabla,CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(DlgRegCambio::IDD, pParent)
{
	m_pTabla = pTabla;

	DLGCONS;
}

DlgRegCambio::~DlgRegCambio()
{
}

void DlgRegCambio::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CHECK1, m_check1);

	DDX_Control(pDX, IDC_COMBO1, Usuario);
	/*
	DDX_Control(pDX, IDC_EDIT1, Desde);
	DDX_Control(pDX, IDC_EDIT2, Hasta);
	*/
	DDX_Control(pDX, IDC_DATETIMEPICKER1, Desde);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, Hasta);

	DDX_Control(pDX, IDC_COMBO2, Campo);
	DDX_Control(pDX, IDC_EDIT3, ValorAnterior);
	DDX_Control(pDX, IDC_EDIT4, ValorNuevo);

	DDX_Control(pDX, IDC_HORADESDE, desdeh);
	DDX_Control(pDX, IDC_HORAHASTA, hastah);

	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_RADIO2, m_radio2);
	DDX_Control(pDX, IDC_RADIO3, m_radio3);		
	DDX_Control(pDX, IDC_RADIO4, m_radio4);
	DDX_Control(pDX, IDC_RADIO5, m_radio5);
	DDX_Control(pDX, IDC_RADIO6, m_radio6);

	DDX_Control(pDX, IDC_STATIC1, m_label1);
	DDX_Control(pDX, IDC_STATIC2, m_label2);
	DDX_Control(pDX, IDC_STATIC3, m_label3);
	DDX_Control(pDX, IDC_STATIC4, m_label4);
	DDX_Control(pDX, IDC_STATIC5, m_label5);
	DDX_Control(pDX, IDC_STATIC6, m_label6);

	DDX_Control(pDX, IDOK, m_boton1);
	DDX_Control(pDX, IDCANCEL, m_boton2);
}


BEGIN_MESSAGE_MAP(DlgRegCambio, CBASEDLG)
	ON_BN_CLICKED(IDC_RADIO2, pondesdehasta)
	ON_BN_CLICKED(IDC_RADIO3, pondesdehasta)
	ON_BN_CLICKED(IDC_RADIO4, pondesdehasta)
	ON_BN_CLICKED(IDC_RADIO5, pondesdehasta)
END_MESSAGE_MAP()


// DlgRegCambio message handlers
BOOL DlgRegCambio::OnInitDialog()
{
	CBASEDLG::OnInitDialog();
	CString s;


	char tmp[5204];
	char *p;
	int ida = 4;
	int vuelta = 5004;
	int *pn;
	int nusers = 0;
	memset(tmp,0,sizeof(tmp));
	p = tmp + (sizeof(long) * 3) + (sizeof(int)*2);
	*((int *)p) = 5004;
	m_pTabla->EnviaRetro(25,ida,vuelta,tmp);
	p += sizeof(int);
	pn = (int *)p;
	p += (sizeof(int)+3);
	if (*pn == 0) {
		AfxMessageBox("Opcion no configurada",MB_OK | MB_ICONHAND);
		DlgRegCambio::OnCancel();
		return TRUE;
	}
	while(*pn > 0) {
	    p[10] = 0;       
		s = p;
		s.TrimRight();
		Usuario.AddString(s);
		nusers++;
		p += 36;
		(*pn)--;
	}
	if (nusers == 1) {
		Usuario.SetCurSel(0);
		Usuario.EnableWindow(FALSE);
	}

	Campo.AddString("Todos");
	Campo.SetCurSel(0);
	for (int i = 0;i < m_pTabla->m_def->m_info->n_campos;i++) {
		s.Format("%03d ",m_pTabla->m_def->Campo(i,TRUE)->Id);
		s += m_pTabla->m_def->c_nombre(i,TRUE);
		/*if (s.Mid(0,1).Compare("~"))*/
		Campo.AddString(s);
	}
	CExtRadioButton *pButton = (CExtRadioButton *)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(1);
	Desde.SetMode(CExtDateTimeWnd::date);
	Hasta.SetMode(CExtDateTimeWnd::date);	

	COleDateTime t1 = COleDateTime::GetCurrentTime();
	int dia = t1.GetDay(),mes = t1.GetMonth(),ano = t1.GetYear();
	Hasta.SetDate(ano,mes,dia);
	dia -= 7;
	if (dia < 0) {
		mes--;
		if (mes < 1) {
			mes += 12;
			ano--;
		}
		t1.SetDate(ano,mes,31);
		dia += t1.GetDay();
	}
	Desde.SetDate(ano,mes,dia);	
	m_notabla = FALSE;

	return TRUE;
}

void DlgRegCambio::pondesdehasta() {		
	UpdateData(TRUE);
	m_sel = 5;
	if (m_radio1.GetCheck()) {
		m_sel = 0;		
	}
	else  {
		COleDateTime t1 = COleDateTime::GetCurrentTime();
		int dia = t1.GetDay(),mes = t1.GetMonth(),ano = t1.GetYear();	
		Hasta.SetDate(ano,mes,dia);
		if (m_radio2.GetCheck()) {
		m_sel = 1;
		}
		else if (m_radio3.GetCheck()) {
			m_sel = 2;	
			dia -= 7;
			if (dia < 0) {
				mes--;
				if (mes < 1) {
					mes += 12;
					ano--;
				}
				t1.SetDate(ano,mes,31);
				dia += t1.GetDay();
			}	
		}
		else if (m_radio4.GetCheck()) {
			m_sel = 3;
			mes--;
			if (mes < 1) {
				mes += 12;
				ano--;
			}
			t1.SetDate(ano,mes,dia);
			dia = t1.GetDay();
		}
		else if (m_radio5.GetCheck()) {
			m_sel = 4;
			mes -= 3;
			if (mes < 1) {
				mes += 12;
				ano--;
			}
			t1.SetDate(ano,mes,dia);
			dia = t1.GetDay();
		}
		Desde.SetDate(ano,mes,dia);
	}	
}

void DlgRegCambio::OnOK() 
{
	pondesdehasta();
	m_notabla = m_check1.GetCheck();

	m_usu = "";
	int n = Usuario.GetCurSel();
	if (n > -1) {
		Usuario.GetLBText(n,m_usu);
		m_usu.TrimRight();
	}
	m_campo = Campo.GetCurSel();    
	//Desde.GetWindowText(m_desde);
	//Hasta.GetWindowText(m_hasta);
	ValorAnterior.GetWindowText(m_valant);
	ValorNuevo.GetWindowText(m_valnue);


	GetDlgItem(IDC_HORADESDE)->GetWindowText(m_desdehora);
	GetDlgItem(IDC_HORAHASTA)->GetWindowText(m_hastahora);

	 
	m_desde.Format("%02d-%02d-%04d",Desde.GetDay(),Desde.GetMonth(),Desde.GetYear());
	m_hasta.Format("%02d-%02d-%04d",Hasta.GetDay(),Hasta.GetMonth(),Hasta.GetYear());

    CBASEDLG::OnOK();	
}

