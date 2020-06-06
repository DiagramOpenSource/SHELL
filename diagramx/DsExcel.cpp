// DsExcel.cpp: implementation of the CDsExcel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "diagramx.h"
#include "DsExcel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDsExcel::CDsExcel()
{
    m_cargado = FALSE;
    m_paramorir = FALSE;
}

CDsExcel::~CDsExcel()
{
    m_ExcelEventSink.m_pDsExcel = NULL;
}

void CDsExcel::DumpComError(const _com_error& e) const
{
	CString ComErrorMessage; 
	ComErrorMessage.Format("COM Error: 0x%08lX. %s",e.Error(), e.ErrorMessage());
	AfxMessageBox(ComErrorMessage);
}

BOOL CDsExcel::AbreExcel(LPCSTR nombre)
{
    m_cargado = TRUE;
	try
	{
		HRESULT hr = m_pXL.CreateInstance(__uuidof(Excel::Application));		
        if (nombre[0])
        {
            m_pBook = m_pXL->Workbooks->Open(nombre);            
        }
        else
        {
            m_pBook = m_pXL->Workbooks->Add((long)Excel::xlWorksheet);
        }
        m_pSheet = m_pXL->ActiveSheet;
        m_ExcelEventSink.Advise(m_pXL,IID_IExcelAppEventSink);
		m_ExcelEventSink.Advise(m_pBook, IID_IExcelBookEventSink);
        m_ExcelEventSink.m_pDsExcel = this;
    }
	catch ( ... )
	{
		m_cargado = FALSE;
	}    

    return m_cargado;
}


/*----------------------------------------------------------------------------*/

CXConnectionAdvisor::CXConnectionAdvisor(REFIID iid) : m_iid(iid)
{
	m_pConnectionPoint = NULL;
	m_AdviseCookie = 0;
}

/*----------------------------------------------------------------------------*/

CXConnectionAdvisor::~CXConnectionAdvisor()
{
	Unadvise();
}


/*----------------------------------------------------------------------------*/

BOOL CXConnectionAdvisor::Advise(IUnknown* pSink, IUnknown* pSource)
{	
	// Advise already done 
	if (m_pConnectionPoint != NULL)
	{
		return FALSE;
	}

	BOOL Result = FALSE;

	IConnectionPointContainer* pConnectionPointContainer;

	if (FAILED(pSource->QueryInterface(
					IID_IConnectionPointContainer,
					(void**)&pConnectionPointContainer)))
	{
		return FALSE;
	}

	if (SUCCEEDED(pConnectionPointContainer->FindConnectionPoint(m_iid, &m_pConnectionPoint)))
	{
		if (SUCCEEDED(m_pConnectionPoint->Advise(pSink, &m_AdviseCookie)))
		{
			Result = TRUE;
		}
		else
		{
			m_pConnectionPoint->Release();
			m_pConnectionPoint = NULL;
			m_AdviseCookie = 0;
		}
	}
	pConnectionPointContainer->Release();
	return Result;
}

/*----------------------------------------------------------------------------*/

BOOL CXConnectionAdvisor::Unadvise()
{	
	if (m_pConnectionPoint != NULL)
	{
		HRESULT hr = m_pConnectionPoint->Unadvise(m_AdviseCookie);
		// If the server is gone, ignore the error
		// ASSERT(SUCCEEDED(hr));
		m_pConnectionPoint->Release();
		m_pConnectionPoint = NULL;
		m_AdviseCookie = 0;
	}
	return TRUE;
}



/*----------------------------------------------------------------------------*/

BEGIN_MESSAGE_MAP(CExcelEventSink, CCmdTarget)
	//{{AFX_MSG_MAP(CExcelEventSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/

BEGIN_DISPATCH_MAP(CExcelEventSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CExcelEventSink)
	DISP_FUNCTION(CExcelEventSink, "Startup",		OnAppStartup,			VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CExcelEventSink, "Quit",			OnAppQuit,				VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CExcelEventSink, "DocumentChange",	OnAppDocumentChange,	VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CExcelEventSink, "New",			OnDocNew,				VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CExcelEventSink, "Open",			OnDocOpen,				VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CExcelEventSink, "Close",			OnDocClose,				VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CExcelEventSink, "Startup",			0x01, OnAppStartup,			VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CExcelEventSink, "Quit",			0x02, OnAppQuit,			VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CExcelEventSink, "DocumentChange",	0x03, OnAppDocumentChange,	VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CExcelEventSink, "New",				0x04, OnDocNew,				VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CExcelEventSink, "Open",			0x05, OnDocOpen,			VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CExcelEventSink, "Close",			0x06, OnDocClose,			VT_EMPTY, VTS_NONE)

  //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/*----------------------------------------------------------------------------*/

BEGIN_INTERFACE_MAP(CExcelEventSink, CCmdTarget)
	INTERFACE_PART(CExcelEventSink, IID_IExcelAppEventSink, Dispatch)
	INTERFACE_PART(CExcelEventSink, IID_IExcelBookEventSink, Dispatch)
END_INTERFACE_MAP()

/*----------------------------------------------------------------------------*/

IMPLEMENT_DYNCREATE(CExcelEventSink, CCmdTarget)

/*----------------------------------------------------------------------------*/

CExcelEventSink::CExcelEventSink() :
					m_AppEventsAdvisor(IID_IExcelAppEventSink), 
					m_BookEventsAdvisor(IID_IExcelBookEventSink)
{	
    m_indice = -1;
    m_pDsExcel = NULL;
	EnableAutomation();
}

/*----------------------------------------------------------------------------*/

CExcelEventSink::~CExcelEventSink()
{
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnAppStartup() 
{
	// You will never receive this event 
	//AfxMessageBox("Quit event received");
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnAppQuit() 
{
	//AfxMessageBox("AppQuit event received");
	//AfxGetMainWnd()->PostMessage(WM_COMMAND, IDCANCEL, 0L);
    if (m_pDsExcel)
    {
        m_pDsExcel->m_paramorir = TRUE;
    }
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnAppDocumentChange() 
{
	//AfxMessageBox("AppDocumentChange event received");
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnDocNew() 
{
	//AfxMessageBox("DocNew event received");
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnDocOpen() 
{
	//AfxMessageBox("DocOpen event received");
}

/*----------------------------------------------------------------------------*/

void CExcelEventSink::OnDocClose() 
{
	//m_pExcelLauncher->OnDocClose();
    //AfxMessageBox("DocClose event received");
}

/*----------------------------------------------------------------------------*/

BOOL CExcelEventSink::Advise(IUnknown* pSource, REFIID iid)
{
	// This GetInterface does not AddRef
	IUnknown* pUnknownSink = GetInterface(&IID_IUnknown);
	if (pUnknownSink == NULL)
	{
		return FALSE;
	}

	if (iid == IID_IExcelAppEventSink)
	{
		return m_AppEventsAdvisor.Advise(pUnknownSink, pSource);
	}
	else if (iid == IID_IExcelBookEventSink)
	{
		return m_BookEventsAdvisor.Advise(pUnknownSink, pSource);
	}
	else 
	{
		return FALSE;
	}
}

/*----------------------------------------------------------------------------*/
	
BOOL CExcelEventSink::Unadvise(REFIID iid)
{
	if (iid == IID_IExcelAppEventSink)
	{
		return m_AppEventsAdvisor.Unadvise();
	}
	else if (iid == IID_IExcelBookEventSink)
	{
		return m_BookEventsAdvisor.Unadvise();
	}
	else 
	{
		return FALSE;
	}
}

/*----------------------------------------------------------------------------*/
