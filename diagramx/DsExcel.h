// DsExcel.h: interface for the CDsExcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSEXCEL_H__4434D5B3_C4FF_4FBF_9AD7_6B2BB5B8D063__INCLUDED_)
#define AFX_DSEXCEL_H__4434D5B3_C4FF_4FBF_9AD7_6B2BB5B8D063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable:4146)
#import <C:\fgv_10\libsoffice\mso97.dll>

#define _CommandBars  CommandBars
#define _CommandBarsPtr  CommandBarsPtr

#pragma warning (default:4146)
#import <C:\fgv_10\libsoffice\vbeext1.olb>

#pragma warning (disable:4786)
#pragma warning (disable:4192)
#define IMPATTRS rename("DocumentProperties", "DocProps")
#import <C:\fgv_10\libsoffice\excel8.olb> IMPATTRS \
    rename("DialogBox", "DialogBoxXL") rename("RGB", "RBG_XL")
#pragma warning (default:4786)
#pragma warning (default:4192)


class CXConnectionAdvisor   
{
public:
	CXConnectionAdvisor (REFIID iid);
	BOOL Advise(IUnknown* pSink, IUnknown* pSource);
	BOOL Unadvise();
	virtual ~CXConnectionAdvisor ();

private:
	CXConnectionAdvisor ();
	CXConnectionAdvisor (const CXConnectionAdvisor & ConnectionAdvisor);
	REFIID m_iid;
	IConnectionPoint* m_pConnectionPoint;
	DWORD m_AdviseCookie;
};

/*----------------------------------------------------------------------------*/

const IID IID_IExcelAppEventSink = __uuidof(Excel::AppEvents);
const IID IID_IExcelBookEventSink = __uuidof(Excel::WorkbookEvents);

/*----------------------------------------------------------------------------*/
class CDsExcel;

class CExcelEventSink : public CCmdTarget
{
	DECLARE_DYNCREATE(CExcelEventSink)

public:
	CExcelEventSink();
	virtual ~CExcelEventSink();
	BOOL Advise(IUnknown* pSource, REFIID iid);
	BOOL Unadvise(REFIID iid);
	void SetLauncher();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExcelEventSink)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

    int m_indice;
    CDsExcel *m_pDsExcel;
protected:

	// Generated message map functions
	//{{AFX_MSG(CExcelEventSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CExcelEventSink)
	afx_msg void OnAppStartup();
	afx_msg void OnAppQuit();
	afx_msg void OnAppDocumentChange();
	afx_msg void OnDocNew();
	afx_msg void OnDocOpen();
	afx_msg void OnDocClose();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	CXConnectionAdvisor  m_AppEventsAdvisor;
	CXConnectionAdvisor  m_BookEventsAdvisor;	
};



class CDsExcel  
{
public:
	CDsExcel();
	virtual ~CDsExcel();
    void DumpComError(const _com_error& e) const;
    BOOL AbreExcel(LPCSTR nombre);

    Excel::_ApplicationPtr m_pXL;
    Excel::_WorkbookPtr    m_pBook;
    Excel::_WorksheetPtr   m_pSheet;
    Excel::RangePtr        m_pRange;

    BOOL m_cargado;
    BOOL m_paramorir;

    CExcelEventSink m_ExcelEventSink;
};

#endif // !defined(AFX_DSEXCEL_H__4434D5B3_C4FF_4FBF_9AD7_6B2BB5B8D063__INCLUDED_)
