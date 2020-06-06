#pragma once


// CDlgOrdenar dialog
class CTablaView;

class CDlgOrdenar : public CExtNCW < CBASEDLG >
{
	DECLARE_DYNAMIC(CDlgOrdenar)

public:
	CDlgOrdenar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOrdenar();

// Dialog Data
	enum { IDD = IDD_ORDENAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	CDLGCOMBO m_ordenar;
	CDLGCOMBO m_luegopor;
	CDLGCOMBO m_luegopor2;

	CDLGLABEL m_label1;
	CDLGLABEL m_label2;
	CDLGLABEL m_label3;
	CDLGGROUP m_label4;
	CDLGGROUP m_label5;
	CDLGGROUP m_label6;

	CDLGRADIO m_boton1;
	CDLGRADIO m_boton2;
	CDLGRADIO m_boton3;
	CDLGRADIO m_boton4;
	CDLGRADIO m_boton5;
	CDLGRADIO m_boton6;

	CDLGBUTTON m_boton7;
	CDLGBUTTON m_boton8;

	CTablaView *m_pTabla;

	int m_col;

	int m_sel1;
	int m_sel2;
	int m_sel3;

	BOOL m_inver1;
	BOOL m_inver2;
	BOOL m_inver3;
};
