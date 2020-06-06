#pragma once


// CDlgOperaciones dialog

class CDlgOperaciones : public CExtNCW < CBASEDLG >
{
	DECLARE_DYNAMIC(CDlgOperaciones)

public:
	CDlgOperaciones(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOperaciones();

// Dialog Data
	enum { IDD = IDD_OPERACIONES };

    CDLGCOMBO m_buscarpor;
	CDLGRADIO m_boton1;
	CDLGRADIO m_boton2;
	CDLGRADIO m_boton3;

	CDLGBUTTON m_boton4;
	CDLGBUTTON m_boton5;

	CDLGLABEL m_label1;


	CTablaView *m_pTabla;
	int m_col;
	int m_sel;
	int m_opera;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
