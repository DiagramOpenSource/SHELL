// DsCalendar.cpp : implementation file
//

#include "stdafx.h"
#include "diagramx.h"
#include "DsCalendar.h"


// CDsCalendar dialog

IMPLEMENT_DYNAMIC(CDsCalendar, CBASEDLG)
CDsCalendar::CDsCalendar(CWnd* pParent /*=NULL*/)
	: CExtNCW < CBASEDLG >(CDsCalendar::IDD, pParent)
{
	m_pedit = NULL;
	DLGCONS;
}

CDsCalendar::~CDsCalendar()
{
}

void CDsCalendar::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_cal);
	DDX_Control(pDX, IDOK, m_ctrlok);
	DDX_Control(pDX, IDCANCEL, m_ctrlcancel);
}


BEGIN_MESSAGE_MAP(CDsCalendar, CBASEDLG)
END_MESSAGE_MAP()

void CDsCalendar::OnOK() 
{	
	if (m_pedit)
	{
		SYSTEMTIME date;
		CString fecha;
		memset(&date,0,sizeof(date));
		m_cal.GetCurSel( &date  );
		fecha.Format("%02d.%02d.%04d",date.wDay,date.wMonth,date.wYear);
		m_pedit->SetWindowText(fecha);
	}	
	CBASEDLG::OnOK();
}

// CDsCalendar message handlers
