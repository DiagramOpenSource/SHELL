#pragma once
#include "afxdtctl.h"


// CDsCalendar dialog

class CDsCalendar : public CExtNCW < CBASEDLG >
{
	DECLARE_DYNAMIC(CDsCalendar)

public:
	CDsCalendar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDsCalendar();

	CWnd *m_pedit;

// Dialog Data
	enum { IDD = IDD_DIALOGCALENDAR };
   CDLGBUTTON m_ctrlok;
   CDLGBUTTON m_ctrlcancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	CMonthCalCtrl m_cal;
};
