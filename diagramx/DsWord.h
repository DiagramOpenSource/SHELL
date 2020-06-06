// DsWord.h: interface for the CDsWord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSWORD_H__075AA8EA_7D7C_4DBE_8289_04A108122A8A__INCLUDED_)
#define AFX_DSWORD_H__075AA8EA_7D7C_4DBE_8289_04A108122A8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define IMPATTRS rename("DocumentProperties", "DocProps")

#pragma warning (disable:4146)
#import <C:\fgv_10\libsoffice\mso97.dll> IMPATTRS

#define _CommandBars  CommandBars
#define _CommandBarsPtr  CommandBarsPtr

#pragma warning (default:4146)
#import <C:\fgv_10\libsoffice\vbeext1.olb>
#pragma warning (disable:4786)
#import <C:\fgv_10\libsoffice\msword8.olb> IMPATTRS rename("ExitWindows", "WordExitWindows")    
#pragma warning (default:4786)

class CDsWord;

class CConnectionAdvisor  
{
public:
	CConnectionAdvisor(REFIID iid);
	BOOL Advise(IUnknown* pSink, IUnknown* pSource);
	BOOL Unadvise();
	virtual ~CConnectionAdvisor();

private:
	CConnectionAdvisor();
	CConnectionAdvisor(const CConnectionAdvisor& ConnectionAdvisor);
	REFIID m_iid;
	IConnectionPoint* m_pConnectionPoint;
	DWORD m_AdviseCookie;
};

/*----------------------------------------------------------------------------*/

const IID IID_IWordAppEventSink = __uuidof(Word::ApplicationEvents);
const IID IID_IWordDocEventSink = __uuidof(Word::DocumentEvents);

/*----------------------------------------------------------------------------*/

class CWordEventSink : public CCmdTarget
{
	DECLARE_DYNCREATE(CWordEventSink)

public:
	CWordEventSink();
	virtual ~CWordEventSink();
	BOOL Advise(IUnknown* pSource, REFIID iid);
	BOOL Unadvise(REFIID iid);
	void SetLauncher();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordEventSink)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

    int m_indice;
    CDsWord *m_pDsWord;
protected:

	// Generated message map functions
	//{{AFX_MSG(CWordEventSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CWordEventSink)
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
	CConnectionAdvisor m_AppEventsAdvisor;
	CConnectionAdvisor m_DocEventsAdvisor;	
};


class CDsWord  
{
public:
	CDsWord();
    BOOL AbreWord(LPCSTR nombre,BOOL doble = FALSE);
	virtual ~CDsWord();
    void DumpComError(const _com_error& e) const;
	Word::_ApplicationPtr m_pWord;
	/*CWordEventSink m_WordEventSink;*/
	Word::_DocumentPtr m_pDoc;
	Word::_DocumentPtr m_pDoc2;
	Word::RangePtr   m_pRange;
	Word::RangePtr   m_pRange2;
    Word::FieldsPtr  m_Fields;    
	Word::FieldsPtr  m_Fields2;    
    Word::SelectionPtr m_Selection;
	Word::SelectionPtr m_Selection2;
    BOOL m_cargado;
    BOOL m_paramorir;

    CWordEventSink m_WordEventSink;
};

#endif // !defined(AFX_DSWORD_H__075AA8EA_7D7C_4DBE_8289_04A108122A8A__INCLUDED_)
