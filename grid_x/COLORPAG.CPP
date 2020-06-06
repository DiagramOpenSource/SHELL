#include "stdafx.h"
#include "color.h"      
#include "wcmfcres.h"
#include "colorpag.h"
      
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// WCColorPage Property Page

// WCColorPage is a property sheet that allows the selection of a color.
// WCMFCRES.RC must be included into the project resource file for any
// project that uses this class -- unless you use the DLL version.

COLORREF WCColorPage::m_ColorArray[] = 
{
 	COLOR_BLACK,
 	COLOR_DKBLUE,
 	COLOR_DKGREEN,
 	COLOR_DKCYAN,
 	COLOR_DKRED,
 	COLOR_DKMAGENTA,
 	COLOR_BROWN,
 	COLOR_DKGRAY,
 	COLOR_LTGRAY,
 	COLOR_LTBLUE,
 	COLOR_LTGREEN,
 	COLOR_LTCYAN,
 	COLOR_LTRED,
 	COLOR_LTMAGENTA,
 	COLOR_YELLOW,
 	COLOR_WHITE
};

WCColorPage::WCColorPage()
	: CPropertyPage(IDD_WC_COLORPAGE), m_Color(0)
{
	//{{AFX_DATA_INIT(WCColorPage)
	//}}AFX_DATA_INIT
	m_Btn1.SetColor(m_ColorArray[0]);
	m_Btn2.SetColor(m_ColorArray[1]);
	m_Btn3.SetColor(m_ColorArray[2]);
	m_Btn4.SetColor(m_ColorArray[3]);
	m_Btn5.SetColor(m_ColorArray[4]);
	m_Btn6.SetColor(m_ColorArray[5]);
	m_Btn7.SetColor(m_ColorArray[6]);
	m_Btn8.SetColor(m_ColorArray[7]);
	m_Btn9.SetColor(m_ColorArray[8]);
	m_Btn10.SetColor(m_ColorArray[9]);
	m_Btn11.SetColor(m_ColorArray[10]);
	m_Btn12.SetColor(m_ColorArray[11]);
	m_Btn13.SetColor(m_ColorArray[12]);
	m_Btn14.SetColor(m_ColorArray[13]);
	m_Btn15.SetColor(m_ColorArray[14]);
	m_Btn16.SetColor(m_ColorArray[15]);
}

void WCColorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WCColorPage)
	DDX_Control(pDX, IDC_BLUEBAR, m_BlueBar);
	DDX_Control(pDX, IDC_GREENBAR, m_GreenBar);
	DDX_Control(pDX, IDC_REDBAR, m_RedBar);
	DDX_Control(pDX, IDC_SAMPLE, m_SampleBtn);
	DDX_Control(pDX, IDC_BUTTON9, m_Btn9);
	DDX_Control(pDX, IDC_BUTTON8, m_Btn8);
	DDX_Control(pDX, IDC_BUTTON7, m_Btn7);
	DDX_Control(pDX, IDC_BUTTON6, m_Btn6);
	DDX_Control(pDX, IDC_BUTTON5, m_Btn5);
	DDX_Control(pDX, IDC_BUTTON4, m_Btn4);
	DDX_Control(pDX, IDC_BUTTON3, m_Btn3);
	DDX_Control(pDX, IDC_BUTTON2, m_Btn2);
	DDX_Control(pDX, IDC_BUTTON16, m_Btn16);
	DDX_Control(pDX, IDC_BUTTON15, m_Btn15);
	DDX_Control(pDX, IDC_BUTTON14, m_Btn14);
	DDX_Control(pDX, IDC_BUTTON13, m_Btn13);
	DDX_Control(pDX, IDC_BUTTON12, m_Btn12);
	DDX_Control(pDX, IDC_BUTTON11, m_Btn11);
	DDX_Control(pDX, IDC_BUTTON10, m_Btn10);
	DDX_Control(pDX, IDC_BUTTON1, m_Btn1);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate == TRUE)
	{
	 	m_Color = m_SampleBtn.GetColor();
	}
	else
	{
		m_RedBar.SetScrollRange(0, 255, FALSE);
		m_GreenBar.SetScrollRange(0, 255, FALSE);
		m_BlueBar.SetScrollRange(0, 255, FALSE);
		                     
		m_RedBar.SetScrollPos(GetRValue(m_Color));
		m_GreenBar.SetScrollPos(GetGValue(m_Color));
		m_BlueBar.SetScrollPos(GetBValue(m_Color));
		m_SampleBtn.SetColor(m_Color);
	}
}

BEGIN_MESSAGE_MAP(WCColorPage, CPropertyPage)
	//{{AFX_MSG_MAP(WCColorPage)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON2, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON3, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON4, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON5, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON6, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON7, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON8, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON9, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON10, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON11, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON12, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON13, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON14, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON15, SomeButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON16, SomeButtonClicked)
END_MESSAGE_MAP()


void WCColorPage::SomeButtonClicked()
{
	const MSG* msg = GetCurrentMessage();
	m_Color = m_ColorArray[msg->wParam - IDC_BUTTON1];
	UpdateData(FALSE);

	m_SampleBtn.Invalidate();
	//TRACE("Message %i %i %i %i\n", msg->message, msg->wParam, msg->lParam);
}
BOOL WCColorPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void WCColorPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar)
	{
		int pos = pScrollBar->GetScrollPos();
		switch (nSBCode)
		{
		 	case SB_LEFT:
		 		if (pos > 0) 
		 		{
		 			pScrollBar->SetScrollPos(--pos);
		 		}
		 		break;
		 	case SB_RIGHT:
		 		if (pos < 255)
		 		{
		 		 	pScrollBar->SetScrollPos(++pos);
		 		}
		 		break;
		 	case SB_THUMBTRACK:
		 		pScrollBar->SetScrollPos(nPos);
		}
		m_SampleBtn.SetColor(RGB(m_RedBar.GetScrollPos(), m_GreenBar.GetScrollPos(), 
			m_BlueBar.GetScrollPos()));
		m_SampleBtn.Invalidate(FALSE);
	}
	else
	{
		CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
	} 
}
