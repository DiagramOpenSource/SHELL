
//
// DsWord.cpp: implementation of the CDsWord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "diagramx.h"
#include "DsWord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static CDsWord *lista_words[100];
static int nlista_words = 0;


void ControlWord_(int entero,char *puntero);

CDsWord::CDsWord()
{
    m_cargado = FALSE;
    m_paramorir = FALSE;
}

BOOL CDsWord::AbreWord(LPCSTR nombre,BOOL doble)
{
    m_cargado = TRUE;
	try
	{
		HRESULT hr = m_pWord.CreateInstance(__uuidof(Word::Application));		

        if (doble)
		{
			m_pWord->Options->CheckGrammarAsYouType = VARIANT_FALSE;
			m_pWord->Options->CheckSpellingAsYouType = VARIANT_FALSE;
		}

        if (nombre[0])
        {
            m_pDoc = m_pWord->Documents->Open(&_variant_t(nombre));
        }
        else
        {
            m_pDoc = m_pWord->Documents->Add();
        }
		if (doble)
		{
			m_pDoc->ShowGrammaticalErrors = VARIANT_FALSE;
			m_pDoc->ShowSpellingErrors = VARIANT_FALSE;
		}
        m_WordEventSink.Advise(m_pWord,IID_IWordAppEventSink);
		m_WordEventSink.Advise(m_pDoc, IID_IWordDocEventSink);
        m_WordEventSink.m_pDsWord = this;
		if (doble)
		{
			m_pDoc2 = m_pWord->Documents->Add();
			m_pDoc2->ShowGrammaticalErrors = VARIANT_FALSE;
			m_pDoc2->ShowSpellingErrors = VARIANT_FALSE;			

			
			float le = m_pDoc->PageSetup->LeftMargin;
            float ri = m_pDoc->PageSetup->RightMargin;
			float to = m_pDoc->PageSetup->TopMargin;
			float bo = m_pDoc->PageSetup->BottomMargin;
			float gu = m_pDoc->PageSetup->Gutter;
    
			m_pDoc2->PageSetup->PageWidth = m_pDoc->PageSetup->PageWidth;
			m_pDoc2->PageSetup->PageHeight = m_pDoc->PageSetup->PageHeight;
			//long o = (long)m_pDoc->PageSetup->Orientation;
			//m_pDoc2->PageSetup->PutOrientation(m_pDoc->PageSetup->GetOrientation());
			//m_pDoc2->PageSetup->FirstPageTray = m_pDoc->PageSetup->FirstPageTray;
			//m_pDoc2->PageSetup->OtherPagesTray = m_pDoc->PageSetup->OtherPagesTray;
			//m_pDoc2->PageSetup->VerticalAlignment = m_pDoc->PageSetup->VerticalAlignment;
			m_pDoc2->PageSetup->MirrorMargins = m_pDoc->PageSetup->MirrorMargins;
			m_pDoc2->PageSetup->HeaderDistance = m_pDoc->PageSetup->HeaderDistance;
			m_pDoc2->PageSetup->FooterDistance = m_pDoc->PageSetup->FooterDistance;
			//m_pDoc2->PageSetup->SectionStart = m_pDoc->PageSetup->SectionStart ;
			m_pDoc2->PageSetup->OddAndEvenPagesHeaderFooter = m_pDoc->PageSetup->OddAndEvenPagesHeaderFooter;
			m_pDoc2->PageSetup->DifferentFirstPageHeaderFooter = m_pDoc->PageSetup->DifferentFirstPageHeaderFooter;
			m_pDoc2->PageSetup->SuppressEndnotes = m_pDoc->PageSetup->SuppressEndnotes;
			//m_pDoc2->PageSetup->PaperSize = m_pDoc->PageSetup->PaperSize;
			m_pDoc2->PageSetup->TwoPagesOnOne = m_pDoc->PageSetup->TwoPagesOnOne;			
			//m_pDoc2->PageSetup->CharsLine = m_pDoc->PageSetup->CharsLine;
			//m_pDoc2->PageSetup->LinesPage = m_pDoc->PageSetup->LinesPage;
			//m_pDoc2->PageSetup->ShowGrid = m_pDoc->PageSetup->ShowGrid;

			m_pDoc2->PageSetup->LeftMargin = le;
            m_pDoc2->PageSetup->RightMargin = ri;
			m_pDoc2->PageSetup->TopMargin = to;
			m_pDoc2->PageSetup->BottomMargin = bo;

			/*
			float PageWidth = m_pDoc->PageSetup->;
			float PageHeight = m_pDoc->PageSetup->;
			enum WdOrientation Orientation = m_pDoc->PageSetup->;
			enum WdPaperTray FirstPageTray = m_pDoc->PageSetup->;
			enum WdPaperTray OtherPagesTray = m_pDoc->PageSetup->;
			enum WdVerticalAlignment VerticalAlignment ;
			long MirrorMargins;
			float HeaderDistance;
			float FooterDistance;
			enum WdSectionStart SectionStart;
			long OddAndEvenPagesHeaderFooter;
			long DifferentFirstPageHeaderFooter;
			long SuppressEndnotes;
			enum WdPaperSize PaperSize;
			VARIANT_BOOL TwoPagesOnOne;
			VARIANT_BOOL GutterOnTop;
			float CharsLine;
			float LinesPage;
			VARIANT_BOOL ShowGrid;
			*/

			m_pDoc2->Styles->Item(&_variant_t((long)Word::wdStyleNormal))->Font->Name = m_pDoc->Styles->Item(&_variant_t((long)Word::wdStyleNormal))->Font->Name;
			m_pDoc2->Styles->Item(&_variant_t((long)Word::wdStyleNormal))->Font->Bold = m_pDoc->Styles->Item(&_variant_t((long)Word::wdStyleNormal))->Font->Bold;
			m_pDoc2->Styles->Item(&_variant_t((long)Word::wdStyleNormal))->Font->Size = m_pDoc->Styles->Item(&_variant_t((long)Word::wdStyleNormal))->Font->Size;


			long ns = m_pDoc->Sections->Count;
			long no = m_pDoc2->Sections->Count;
			Word::RangePtr R;
			if (ns > 0 && no > 0)
			{	
				if (m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterPrimary)->Exists == VARIANT_TRUE)
				{				
					R = m_pDoc2->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterPrimary)->Range;
					R->FormattedText = m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterPrimary)->Range->FormattedText;
					ns = R->Paragraphs->Count;
					if (ns > 1)
						R->Paragraphs->Item(ns)->Range->Delete();
				}
				if (m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->Exists == VARIANT_TRUE)
				{
					R = m_pDoc2->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->Range;
					R->FormattedText = m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->Range->FormattedText;
					ns = R->Paragraphs->Count;
					if (ns > 1)
						R->Paragraphs->Item(ns)->Range->Delete();
				}				
				if (m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterFirstPage)->Exists == VARIANT_TRUE)
				{
					R = m_pDoc2->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterFirstPage)->Range;
					R->FormattedText = m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterFirstPage)->Range->FormattedText;
					ns = R->Paragraphs->Count;
					if (ns > 1)
						R->Paragraphs->Item(ns)->Range->Delete();
				}
				if (m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterFirstPage)->Exists == VARIANT_TRUE)
				{
					R = m_pDoc2->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterFirstPage)->Range;
					R->FormattedText = m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterFirstPage)->Range->FormattedText;
					ns = R->Paragraphs->Count;
					if (ns > 1)
						R->Paragraphs->Item(ns)->Range->Delete();
				}
				if (m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterEvenPages)->Exists == VARIANT_TRUE)
				{
					R = m_pDoc2->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterEvenPages)->Range;
					R->FormattedText = m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterEvenPages)->Range->FormattedText;
					ns = R->Paragraphs->Count;
					if (ns > 1)
						R->Paragraphs->Item(ns)->Range->Delete();
				}
				if (m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterEvenPages)->Exists == VARIANT_TRUE)
				{
					R = m_pDoc2->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterEvenPages)->Range;
					R->FormattedText = m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterEvenPages)->Range->FormattedText;
					ns = R->Paragraphs->Count;
					if (ns > 1)
						R->Paragraphs->Item(ns)->Range->Delete();
				}
			}

			/*
			char tmp[300];
			for (int i = 1;i < 256;i++)
			{
				tmp[i-1] = i;
			}
			tmp[i-1] = '\n';
			tmp[i] = 0;
			m_pDoc2->Range()->Text = _bstr_t((LPCSTR)tmp);
			*/
		}
    }
	catch ( ... )
	{
		m_cargado = FALSE;
		try
		{
			m_pWord->Quit(&_variant_t(VARIANT_FALSE));
		}
		catch ( ... )
		{
		}
	}    

    return m_cargado;
}


CDsWord::~CDsWord()
{
    m_WordEventSink.m_pDsWord = NULL;
}


void CDsWord::DumpComError(const _com_error& e) const
{
	CString ComErrorMessage; 
	ComErrorMessage.Format("COM Error: 0x%08lX. %s",e.Error(), e.ErrorMessage());
	AfxMessageBox(ComErrorMessage);
}


/*----------------------------------------------------------------------------*/

CConnectionAdvisor::CConnectionAdvisor(REFIID iid) : m_iid(iid)
{
	m_pConnectionPoint = NULL;
	m_AdviseCookie = 0;
}

/*----------------------------------------------------------------------------*/

CConnectionAdvisor::~CConnectionAdvisor()
{
	Unadvise();
}


/*----------------------------------------------------------------------------*/

BOOL CConnectionAdvisor::Advise(IUnknown* pSink, IUnknown* pSource)
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

BOOL CConnectionAdvisor::Unadvise()
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

BEGIN_MESSAGE_MAP(CWordEventSink, CCmdTarget)
	//{{AFX_MSG_MAP(CWordEventSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/

BEGIN_DISPATCH_MAP(CWordEventSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CWordEventSink)
	DISP_FUNCTION(CWordEventSink, "Startup",		OnAppStartup,			VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWordEventSink, "Quit",			OnAppQuit,				VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWordEventSink, "DocumentChange",	OnAppDocumentChange,	VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWordEventSink, "New",			OnDocNew,				VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWordEventSink, "Open",			OnDocOpen,				VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWordEventSink, "Close",			OnDocClose,				VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CWordEventSink, "Startup",			0x01, OnAppStartup,			VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CWordEventSink, "Quit",			0x02, OnAppQuit,			VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CWordEventSink, "DocumentChange",	0x03, OnAppDocumentChange,	VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CWordEventSink, "New",				0x04, OnDocNew,				VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CWordEventSink, "Open",			0x05, OnDocOpen,			VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CWordEventSink, "Close",			0x06, OnDocClose,			VT_EMPTY, VTS_NONE)

  //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/*----------------------------------------------------------------------------*/

BEGIN_INTERFACE_MAP(CWordEventSink, CCmdTarget)
	INTERFACE_PART(CWordEventSink, IID_IWordAppEventSink, Dispatch)
	INTERFACE_PART(CWordEventSink, IID_IWordDocEventSink, Dispatch)
END_INTERFACE_MAP()

/*----------------------------------------------------------------------------*/

IMPLEMENT_DYNCREATE(CWordEventSink, CCmdTarget)

/*----------------------------------------------------------------------------*/

CWordEventSink::CWordEventSink() :
					m_AppEventsAdvisor(IID_IWordAppEventSink), 
					m_DocEventsAdvisor(IID_IWordDocEventSink)
{	
    m_indice = -1;
    m_pDsWord = NULL;
	EnableAutomation();
}

/*----------------------------------------------------------------------------*/

CWordEventSink::~CWordEventSink()
{
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnAppStartup() 
{
	// You will never receive this event 
	//AfxMessageBox("Quit event received");
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnAppQuit() 
{
	//AfxMessageBox("AppQuit event received");
	//AfxGetMainWnd()->PostMessage(WM_COMMAND, IDCANCEL, 0L);
    /*
    if (m_indice > -1 && lista_words[m_indice])
    {        
        lista_words[m_indice]->m_paramorir = TRUE; // suicidio        
    }
    */
    if (m_pDsWord)
    {
        m_pDsWord->m_paramorir = TRUE;
    }
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnAppDocumentChange() 
{
	//AfxMessageBox("AppDocumentChange event received");
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnDocNew() 
{
	//AfxMessageBox("DocNew event received");
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnDocOpen() 
{
	//AfxMessageBox("DocOpen event received");
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::OnDocClose() 
{
	//m_pWordLauncher->OnDocClose();
    //AfxMessageBox("DocClose event received");
}

/*----------------------------------------------------------------------------*/

BOOL CWordEventSink::Advise(IUnknown* pSource, REFIID iid)
{
	// This GetInterface does not AddRef
	IUnknown* pUnknownSink = GetInterface(&IID_IUnknown);
	if (pUnknownSink == NULL)
	{
		return FALSE;
	}

	if (iid == IID_IWordAppEventSink)
	{
		return m_AppEventsAdvisor.Advise(pUnknownSink, pSource);
	}
	else if (iid == IID_IWordDocEventSink)
	{
		return m_DocEventsAdvisor.Advise(pUnknownSink, pSource);
	}
	else 
	{
		return FALSE;
	}
}

/*----------------------------------------------------------------------------*/
	
BOOL CWordEventSink::Unadvise(REFIID iid)
{
	if (iid == IID_IWordAppEventSink)
	{
		return m_AppEventsAdvisor.Unadvise();
	}
	else if (iid == IID_IWordDocEventSink)
	{
		return m_DocEventsAdvisor.Unadvise();
	}
	else 
	{
		return FALSE;
	}
}

/*----------------------------------------------------------------------------*/

void CWordEventSink::SetLauncher()
{
	
}


/*----------------------------------------------------------------------------*/

int _BuscaTextoWord(int i,char *tmpxx)
{
	int solounavez = 0;
	if (*tmpxx == '!' && *(tmpxx+1) == '@')
	{
		tmpxx += 2;
		solounavez = 1;		
		if (lista_words[i]->m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->Exists != VARIANT_TRUE)
		{
			return FALSE;				
		}
		lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->Range;
		lista_words[i]->m_pRange->Select();
	}
	else
	{
		lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range();
		lista_words[i]->m_pRange->Select();
	}
	do
	{
		lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
		lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx));
		if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
		{
			return TRUE;
		}
		else
		{		
			if (solounavez == 1)
			{				
				if (lista_words[i]->m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterPrimary)->Exists != VARIANT_TRUE)
				{
					return FALSE;				
				}				
				solounavez = 2;
				lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Sections->Item(1)->Headers->Item(Word::wdHeaderFooterPrimary)->Range;
				lista_words[i]->m_pRange->Select();
				continue;
			}
			if (solounavez)
			{
				return FALSE;
			}			
			Word::ShapesPtr Shapes;
			Shapes = lista_words[i]->m_pDoc->GetShapes();
			long nc = Shapes->GetCount();
			long icc;
			for (icc = 1; icc <= nc;icc++)
			{
				//if (lista_words[i]->m_FormFields[i]->Type == 
				Shapes->Item(&_variant_t(icc))->Select();
				lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
				lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx));
				if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
				{
					return TRUE;
				}
			}

			if (icc >= nc)
			{
				Word::FramesPtr Frames;

				Frames = lista_words[i]->m_pDoc->GetFrames();

				long nc = Frames->GetCount();
				long icc;
				for (icc = 0; icc < nc;icc++)
				{
					//if (lista_words[i]->m_FormFields[i]->Type == 
					Frames->Item(icc)->Select();
					lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
					lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx));
					if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
					{
						return TRUE;
					}
				}

				if (icc >= nc)
				{
					Word::FormFieldsPtr FormFields;

					FormFields =  lista_words[i]->m_pDoc->GetFormFields();
					long nc = FormFields->GetCount();
					long icc;
					for (icc = 1; icc <= nc;icc++)
					{
						//if (lista_words[i]->m_FormFields[i]->Type == 
						FormFields->Item(&_variant_t(icc))->Select();
						lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
						lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx));
						if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
						{
							return TRUE;
						}
					}
				}
			}
		}
		break;
	} while(-1);
	return FALSE;
}

static float sFont = 0.0;
static char Fontn[500];
static char bFont;
static void PonFont(/*Word::RangePtr &R,*/char *tmpxx,float size,char bold)
{
	if (tmpxx)
	{
		strcpy(Fontn,tmpxx);
		sFont = size;
		bFont = bold;
	}
	/*
	if (!sFont) return;
	R->Font->Name = _bstr_t((LPCSTR)(Fontn));
	R->Font->Size = sFont;
	if (bFont == 'N')
	{
		R->Font->Bold = VARIANT_FALSE;
	}
	else
	{
		R->Font->Bold = VARIANT_TRUE;
	}
	*/
}

static void AsignaTextoAWord(Word::RangePtr &R,char *_txt,int idx)
{
   unsigned char *p1 = (unsigned char *)_txt;
   unsigned char *txt = (unsigned char *)_txt;   
   long simbolo = 0;
   char tmp[1024];
   int i;      
   int primera = 1;
   long start;
   long end;
   long bold;
/*
&#9484;&#9516;&#9488

&#9500;&#9532;&#9508;&#9472;&#9474

&#9492;&#9524;&#9496;
*/
   
   start = R->Start;
   end = start;

   while(*p1)
   {	
	  switch(*p1)
	  {
	  case 179:
	  case 186:
		  simbolo = 9474;
		  break;
	  /*
	  case 196:
	  case 205:
		  simbolo = 9472;
		  break;
	  */
	  case 218:
	  case 201:
	  case 214:
	  case 213:
		  simbolo = 9484;
		  break;
	  case 194:
	  case 203:
	  case 210:
	  case 209:		  		  
		  simbolo = 9516;
		  break;
	  case 191:
	  case 187:
	  case 183:
	  case 184:
		  simbolo = 9488;		  
		  break;

	  case 195:
	  case 204:
	  case 199:
	  case 198:		  
		  simbolo = 9500;
		  break;
	  case 197:
	  case 206:
	  case 215:
	  case 216:
		  simbolo = 9532;
		  break;
	  case 180:
	  case 185:
	  case 182:
	  case 181:
		  simbolo = 9508;
		  break;

	  case 192:
	  case 200:
	  case 211:
	  case 212:
		  simbolo = 9492;
		  break;
	  case 193:
	  case 202:
	  case 208:
	  case 207:		  		  
		  simbolo = 9524;
		  break;
	  case 217:
	  case 188:
	  case 189:
	  case 190:
		  simbolo = 9496;
		  break;
	  }	  	  
	  if (simbolo)
	  {
		  if (primera)
		  {
			  primera = 0;
			  R->Text = "";
		  }	      
		  if (p1 > txt)
		  {
			  i = (int)(p1-txt);
			  memcpy(tmp,txt,i);
			  tmp[i] = 0;
			  start = R->End;
			  R->Text = _bstr_t((LPCSTR)tmp);			  			  
			  end = R->End;
			  R->Collapse(&_variant_t((long)Word::wdCollapseEnd));
		  } 		   
		  
		  /*
		  R->Text = _bstr_t((LPCSTR)".");
		  R->Collapse(&_variant_t((long)Word::wdCollapseEnd));
		  */
		  end++;
		  
		  
		  /*
		  R->Collapse(&_variant_t((long)Word::wdCollapseEnd));
		  */
		  R->Collapse(&_variant_t((long)Word::wdCollapseStart));		  
		  R->InsertSymbol(simbolo,&vtMissing,&_variant_t(VARIANT_TRUE));
		  R->MoveStart(&_variant_t((long)Word::wdCharacter),&_variant_t((long)1));		  
		  simbolo = 0;
		  txt = p1 + 1;
	  }
	  else	  	  
	  {
		  oem_a_ansi(p1);
		  end++;
	  }
	  p1++;
   }
   if (p1 > txt)
   {
	  i = (int)(p1-txt);
	  memcpy(tmp,txt,i);
	  tmp[i] = 0;
	  R->Text = _bstr_t((LPCSTR)tmp);
	  primera = 0;
   }
   else
   {
	   if (primera)
	   {
		   R->Text = _bstr_t((LPCSTR)"");
	   }
   }


   if (!primera && sFont)
   {
		Word::SelectionPtr S;
	    end++;
	    lista_words[idx]->m_pDoc->Range(&_variant_t(start),&_variant_t(end))->Select();
	    S = lista_words[idx]->m_pWord->Selection;
		S->Range->Font->Name = _bstr_t((LPCSTR)(Fontn));
		S->Range->Font->Size = sFont;
		if (bFont == 'N')
		{
			bold = VARIANT_FALSE;
		}
		else
		{
			bold = VARIANT_TRUE;
		}
		S->Range->Font->Bold = bold;
		S->Range->Bold = bold;
   }
}


#ifdef SOLOX
void ControlWord_(int entero,char *puntero)
{
	BOOL doble = FALSE;
	
    switch(entero)
    {
    case 0:
        {
			char *fff = puntero;			

			if (*puntero == '#')
			{
				fff = puntero + 1;
				doble = TRUE;
			}
			else
			{
				fff = puntero;
			}
            for (int i = 0;i < nlista_words;i++)
            {
                if (!lista_words[i])
                {
                    break;
                }
                else
                {
                    if (lista_words[i]->m_paramorir)
                    {
                        delete lista_words[i];
                        lista_words[i] = NULL;
                        break;
                    }
                }
            }
            lista_words[i] = new CDsWord();

            CString ss;
            if (fff[0] == ':' && fff[1] == '/')
            {
                ss = theApp.m_dirbase;
                ss += (LPCSTR)(fff+2);                
            }
            else
            {
                ss = (LPCSTR)fff;
            }
            ss.Replace('/','\\');

            if (lista_words[i]->AbreWord(ss,doble))
            {
				sFont = 0.0;
                lista_words[i]->m_pWord->Visible = VARIANT_FALSE;
                if (i == nlista_words)
                {
                    nlista_words++;
                }
		        lista_words[i]->m_WordEventSink.Advise(lista_words[i]->m_pWord, IID_IWordAppEventSink);
		        //ASSERT(Res == TRUE);
		        lista_words[i]->m_WordEventSink.Advise(lista_words[i]->m_pDoc, IID_IWordDocEventSink);
                lista_words[i]->m_WordEventSink.m_indice = i;
		        //ASSERT(Res == TRUE);
            }
            else
            {
                delete lista_words[i];
                i = -1;
            }
            ::sprintf(puntero,"%d",i);
        }
        break;
    case 1:
        {
            int i;
			int solodoble = 0;
			if (*puntero == '#')
			{
			   if (*(puntero+1) == '#')
			   {
				   solodoble = 1;
				   i = atoi(puntero+2);
			   }
			   else
				   i = atoi(puntero+1);
			   doble = TRUE;
			}
			else
			{
				i = atoi(puntero);
			}
			
                if (i >= 0 && i < nlista_words && lista_words[i])
                {
					Word::_DocumentPtr pDoc;
					if (doble && solodoble)
					{
						pDoc = lista_words[i]->m_pDoc2;
					}
					else
					{
						pDoc = lista_words[i]->m_pDoc;
					}
	                try
                    {    
						char *ptmp;
                        for (ptmp = puntero;*ptmp && *ptmp != 1 && *ptmp != 3;ptmp++);
                        if (*ptmp == 1)
                        {
                            ptmp++;
                            if (*ptmp)
                            {
                                CString ss;

                                if (ptmp[0] == ':' && ptmp[1] == '/')
                                {
                                    ss = theApp.m_dirbase;
                                    ss += (LPCSTR)(ptmp+2);                
                                }
                                else
                                {
                                    ss = (LPCSTR)ptmp;
                                }
                                ss.Replace('/','\\');

                                pDoc->SaveAs(&_variant_t(ss));
                            }
                            else
                            {   
                                pDoc->Save();
                            }
                        }
						if (doble)
						{							
							pDoc->Close();
							if (!solodoble)
							{
								lista_words[i]->m_pDoc = lista_words[i]->m_pDoc2;
							}
							lista_words[i]->m_pDoc2 = NULL;
						}
						else
						{
							if (*ptmp != 3)
							{
								lista_words[i]->m_pWord->Quit(&_variant_t(VARIANT_FALSE));
							}
						}
                    }
	                catch (...)
	                {
		                i = -1;
	                }
					if (!doble)
					{
						if (lista_words[i])
						{
							delete lista_words[i];
							lista_words[i] = NULL;
						}
					}
                }
                else
                    i = -1;
            ::sprintf(puntero,"%d",i);
        }
        break;
    case 2:
        {
            char tmpxx[512];            
            int sal = -1;			
            int i;
			if (*puntero == '#')
			{
			   i = atoi(puntero+1);
			   doble = TRUE;
			}
			else
			{
				i = atoi(puntero);
			}
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        ::strupr(tmpxx);
                        ::strupr(ptmp);
                        if (!strcmp(tmpxx,"QUIT"))
                        {
                            sal = 0;
                            try
                            {
                                if (!strcmp(ptmp,"SI"))
                                {
                                    lista_words[i]->m_pWord->Quit(&_variant_t(VARIANT_FALSE));
                                    delete lista_words[i];
                                    lista_words[i] = NULL;
                                    strcpy(puntero,"SI");
                                } 
                                else if (lista_words[i]->m_paramorir)
                                {
                                    delete lista_words[i];
                                    lista_words[i] = NULL;
                                    strcpy(puntero,"SI");
                                }
                                else
                                {
                                    strcpy(puntero,"NO");
                                    ::Sleep(100);
                                }
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }
                        else if (!strcmp(tmpxx,"VISIBLE"))
                        {                            
                            sal = 0;
                            try
                            {
                                if (!strcmp(ptmp,"MINIMIZADO"))
                                {
									lista_words[i]->m_pWord->Visible = VARIANT_TRUE;
									lista_words[i]->m_pWord->WindowState = Word::wdWindowStateMinimize;
								}
								else if (!strcmp(ptmp,"SI"))
                                {
                                    lista_words[i]->m_pWord->Visible = VARIANT_TRUE;
                                }
                                else if (!strcmp(ptmp,"NO"))                                
                                {
                                    lista_words[i]->m_pWord->Visible = VARIANT_FALSE;                                    
                                }
                                if (lista_words[i]->m_pWord->Visible == VARIANT_TRUE)
                                {
                                    strcpy(puntero,"SI");
                                }
                                else
                                {
                                    strcpy(puntero,"NO");
                                }
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }
                        else if (!strcmp(tmpxx,"LIMPIAFINAL"))
                        {
                            sal = 0;
                            long mipunto = 0L;
                            try
                            {
                                do
                                {
                                    long start = lista_words[i]->m_pDoc->Range()->End - 2;
                                    long end = lista_words[i]->m_pDoc->Range()->End-1;

                                    if (start == mipunto)
                                    {
                                        break;
                                    }
                                    mipunto = start;

                                    lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range(&_variant_t(start),&_variant_t(end));
                                    _bstr_t tt = lista_words[i]->m_pRange->Text;
                                    char *ptt = tt;                                    
                                    if (tt.length() == 1 && ptt[0] == ' ' || ptt[0] == (char)13 || ptt[0] == (char)10)
                                    {
                                        lista_words[i]->m_pRange->Delete();
                                        sal++;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                } while(-1);
                                ::sprintf(puntero,"%d",sal);
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }
                        else if (!strcmp(tmpxx,"PAGINACION"))
                        {
                            sal = 0;                            
                            try
                            {
                                  
                                  long sp,np;
                                  if (*ptmp == '=')
                                  {
                                      sp = atoi(ptmp+1);
                                      lista_words[i]->m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->PageNumbers->StartingNumber = sp;
                                  }
                                  sp = lista_words[i]->m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->PageNumbers->StartingNumber;                                  
                                  np = lista_words[i]->m_pDoc->Sections->Item(1)->Range->GetInformation(Word::wdNumberOfPagesInDocument);
                                  ::sprintf(puntero,"%ld:%ld,%ld",sp+np,sp,np);
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }

                    }
                }
            }            
            if (sal == -1)
            {
                ::sprintf(puntero,"%d",sal);
            }
        }
        break;
    case 3:
        {
            char tmpxx[512];            
            int sal = -1;
            int i;
			if (*puntero == '#')
			{
			   i = atoi(puntero+1);
			   doble = TRUE;
			}
			else
			{
				i = atoi(puntero);
			}
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        sal = 0;

                        CString ss;

                        if (ptmp[0] == ':' && ptmp[1] == '/')
                        {
                            ss = theApp.m_dirbase;
                            ss += (LPCSTR)(ptmp+2);                
                        }
                        else
                        {
                            ss = (LPCSTR)ptmp;
                        }
                        ss.Replace('/','\\');

                        ptmp = (char *)(LPCSTR)ss;

                        try
                        {
                            switch(tmpxx[0])
                            {
                            case '!':
                                {
									if (_BuscaTextoWord(i,tmpxx+1))
									{
                                        lista_words[i]->m_Selection->Range->Delete();
										if (ptmp[0] == '@')
										{
											Word::InlineShapes Shapes;
											Shapes = lista_words[i]->m_pDoc->GetInlineShapes();
											Shapes->AddPicture(ptmp+1, VARIANT_FALSE, VARIANT_TRUE, lista_words[i]->m_Selection->Range);
										}
										else
										{
											lista_words[i]->m_Selection->Range->InsertFile(_bstr_t((LPCSTR)ptmp));
										}
                                    }
                                    else
                                        sal = -1;
                                }
                                break;
                            case '#':
                                {
                                    long nf = (long)::atoi(tmpxx+1);
                                    lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range()->Fields->Item(nf)->Result;
                                    lista_words[i]->m_pRange->InsertFile(_bstr_t((LPCSTR)ptmp));
                                }
                                break;
                            case '&':
                                {   
									Word::FieldsPtr  Fields;                                    
									if (doble)
									{
										lista_words[i]->m_Fields2 = lista_words[i]->m_pDoc2->Range()->Fields;
										Fields = lista_words[i]->m_Fields2;
									}
									else
									{
										lista_words[i]->m_Fields = lista_words[i]->m_pDoc->Range()->Fields;
										Fields = lista_words[i]->m_Fields;
									}
                                    long nc = lista_words[i]->m_Fields->Count;
									char pp[1024];
                                    for (long cc = 1L;cc <= nc;cc++)
                                    {
										strcpy(pp,Fields->Item(cc)->Code->Text);
                                        if (::strstr(pp,tmpxx+1))
                                        {
											if (doble)
											{
												lista_words[i]->m_pRange2 = Fields->Item(cc)->Result;
												lista_words[i]->m_pRange2->InsertFile(_bstr_t((LPCSTR)ptmp));
											}
											else
											{
												lista_words[i]->m_pRange = Fields->Item(cc)->Result;
												lista_words[i]->m_pRange->InsertFile(_bstr_t((LPCSTR)ptmp));
											}
                                            break;
                                        }
                                    }                                    
                                }
                                break;
                            default:
                                {
                                    lista_words[i]->m_pRange  = lista_words[i]->m_pDoc->Range();
                                    lista_words[i]->m_pRange->InsertFile(_bstr_t((LPCSTR)ptmp));
                                }
                                break;
                            }                            
                        }
                        catch( ... )
                        {
                            sal = -1;
                        }
                    }
                }
            }
            ::sprintf(puntero,"%d",sal);
        }
        break;
    case 4:
        {
            char tmpxx[512];
            int sal = -1;
            int i;
			if (*puntero == '#')
			{
			   i = atoi(puntero+1);
			   doble = TRUE;
			}
			else
			{
				i = atoi(puntero);
			}
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        sal = 0;
                        try
                        {                            
                            switch(tmpxx[0])
                            {
                            case '#':
                                {
                                    long nf = (long)::atoi(tmpxx+1);
									Word::FieldsPtr  Fields;                                    
									if (doble)
									{
										lista_words[i]->m_Fields2 = lista_words[i]->m_pRange2->Fields;
										Fields = lista_words[i]->m_Fields2;
									}
									else
									{
										lista_words[i]->m_Fields = lista_words[i]->m_pDoc->Range()->Fields;
										Fields = lista_words[i]->m_Fields;
									}									
									AsignaTextoAWord(Fields->Item(nf)->Result,ptmp,i);
                                }
                                break;
                            case '&':
                                {                                       
                                    lista_words[i]->m_Fields = lista_words[i]->m_pDoc->Range()->Fields;
                                    long nc = lista_words[i]->m_Fields->Count;
                                    for (long cc = 1L;cc <= nc;cc++)
                                    {
                                        if (::strstr(lista_words[i]->m_Fields->Item(cc)->Code->Text,tmpxx+1))
                                        {
                                            lista_words[i]->m_pRange = lista_words[i]->m_Fields->Item(cc)->Result;                                            
											AsignaTextoAWord(lista_words[i]->m_pRange,ptmp,i);
                                        }
                                    }                                    
                                }
                                break;
                            default:
                                {
									if (_BuscaTextoWord(i,tmpxx))
									{
                                        lista_words[i]->m_Selection->Range->Delete();                                        
                                        lista_words[i]->m_Selection->Range->InsertFile(_bstr_t((LPCSTR)ptmp));
									}                                    
									else
										sal = -1;
                                }
                                break;
                            } 
                            
                        }
                        catch( ... )
                        {
                            sal = -1;
                        }
                    }

                }                
            }
            else
                i = -1;
            ::sprintf(puntero,"%d",i);
        }
        break;
    case 5:
        {
            int i;
			if (*puntero == '#')
			{
			   i = atoi(puntero+1);
			   doble = TRUE;
			}
			else
			{
				i = atoi(puntero);
			}
                if (i >= 0 && i < nlista_words && lista_words[i])
                {
	                try
                    {
						char *ptmp;
                        for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                        if (*ptmp == 1)
                        {
                            ptmp++;
                            ::strupr(ptmp);
                            if (!::strcmp(ptmp,"DIALOGO"))
                            {   
                                lista_words[i]->m_pDoc->Activate();
                                /*i = (int)*/lista_words[i]->m_pWord->Dialogs->Item(Word::wdDialogFilePrint)->Show();
                            }
                            else
                            {
                                lista_words[i]->m_pDoc->PrintOut();
                            }
                        }
                    }
	                catch (...)
	                {
		                i = -1;
	                }
                }
                else
                    i = -1;
            ::sprintf(puntero,"%d",i);
        }
        break;
	case 6:
        {
            char tmpxx[512];            
            int sal = -1;
            int i;
			if (*puntero == '#')
			{
			   i = atoi(puntero+1);
			   doble = TRUE;
			}
			else
			{
				i = atoi(puntero);
			}

            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        sal = 0;

                        try
                        {
                            switch(tmpxx[0])
                            {
                            case '!':
								if (_BuscaTextoWord(i,tmpxx+1))
								{
									lista_words[i]->m_Selection->Range->Delete();
									AsignaTextoAWord(lista_words[i]->m_Selection->Range,ptmp,i);
									::sprintf(puntero,"%d",sal);
								}
								else
									sal = -1;
                                break;
							case'?':
								if (!_BuscaTextoWord(i,tmpxx+1))
								{
									sal = -1;
								}
								else
								{
									_variant_t start,end;
									start = lista_words[i]->m_Selection->Range->Start;
									if (!*ptmp)
									{
										end = lista_words[i]->m_Selection->Range->End;
									}
									else
									{
										start = lista_words[i]->m_Selection->Range->End;
										if (!_BuscaTextoWord(i,ptmp))
										{
											sal = -2;
										}
										else
										{
											end = lista_words[i]->m_Selection->Range->Start;
										}
									}
									if (sal >= 0)
									{
										lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range(&start,&end);
										sprintf(puntero,"%ld,%ld",(long)start,(long)end);
									}
								}
								break;
							case '@':
								{
                                    lista_words[i]->m_Fields = lista_words[i]->m_pRange->Fields;
                                    long nc = lista_words[i]->m_Fields->Count;									
									puntero[0] = 0;
                                    for (long cc = 1L;cc <= nc;cc++)
                                    {										
										strcat(puntero,lista_words[i]->m_Fields->Item(cc)->Code->Text);
										strcat(puntero,"\002");
										sprintf(puntero+strlen(puntero),"%ld",(long)lista_words[i]->m_Fields->Item(cc)->Code->Start);
									    strcat(puntero,"\002");
										sprintf(puntero+strlen(puntero),"%ld",(long)lista_words[i]->m_Fields->Item(cc)->Code->End);
										strcat(puntero,"\001");
                                    }
								}								
								break;
							case'|':
								{
									_variant_t v1,v2;
									if (tmpxx[1] == 'A')
									{
										v1 = lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes;
										v2 = lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll;
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes = VARIANT_TRUE;
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll = VARIANT_TRUE;
									}
									// el que llama debe asegurarse suficiente room 
									strcpy(puntero,lista_words[i]->m_pRange->Text);
									if (tmpxx[1] == 'A')
									{
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes = v1;
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll = v2;
									}
								}
								break;
							case'&':
								{
									_variant_t start,end;
									_variant_t v1,v2;
									long ll;
									Word::_DocumentPtr pDoc;
									if (doble)
									{
										pDoc = lista_words[i]->m_pDoc2;
									}
									else
									{
										pDoc = lista_words[i]->m_pDoc;
									}
									if (1/*tmpxx[1] == 'A'*/)
									{
										v1 = pDoc->ActiveWindow->View->ShowFieldCodes;
										v2 = pDoc->ActiveWindow->View->ShowAll;
										pDoc->ActiveWindow->View->ShowFieldCodes = VARIANT_TRUE;
										pDoc->ActiveWindow->View->ShowAll = VARIANT_TRUE;
									}
									start = (long)atol(tmpxx+1);
									end   = (long)atol(ptmp);
									if ((long)end < (long)start)
									{
										if (doble)
										{
											lista_words[i]->m_pRange2 = pDoc->Range();
										}
										else
										{
											lista_words[i]->m_pRange = pDoc->Range();
										}
									}
									else
									{
										if (doble)
										{
											lista_words[i]->m_pRange2 = pDoc->Range(&start,&end);
										}
										else
										{
											lista_words[i]->m_pRange = pDoc->Range(&start,&end);
										}
									}
									if (doble)
									{
										start = lista_words[i]->m_pRange2->Start;
										end = lista_words[i]->m_pRange2->End;
										ll = (long)strlen(lista_words[i]->m_pRange2->Text);
									}
									else
									{
										start = lista_words[i]->m_pRange->Start;
										end = lista_words[i]->m_pRange->End;
										ll = (long)strlen(lista_words[i]->m_pRange->Text);
									}
									sprintf(puntero,"%ld,%ld,%ld",(long)start,(long)end,ll);
									if (1/*tmpxx[1] == 'A'*/)
									{
										pDoc->ActiveWindow->View->ShowFieldCodes = v1;
										pDoc->ActiveWindow->View->ShowAll = v2;
									}
								}
								break;
							case '=':
								{									
									if (doble)
									{
										if (*ptmp == '+')
										{											
											lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
											ptmp++;
										}
										if (*ptmp == '=')
										{
											ptmp++;
											lista_words[i]->m_pRange2->Text = _bstr_t((LPCSTR)(ptmp));
										}
										else if (*ptmp == '>')
										{
											_variant_t v1,v2;
									        long lineao,linead;

											v1 = lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes;
											v2 = lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll;
											lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes = VARIANT_FALSE;
											lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll = VARIANT_FALSE;
											
											lista_words[i]->m_pRange2->FormattedText = lista_words[i]->m_pRange->FormattedText;

										    lineao = lista_words[i]->m_pRange->GetInformation(Word::wdVerticalPositionRelativeToPage);
										    linead = lista_words[i]->m_pRange2->GetInformation(Word::wdVerticalPositionRelativeToPage);

											lineao -= linead;
											//lineao -= (long)lista_words[i]->m_pWord->LinesToPoints(1);

											if (lineao > 0)
											{
												long pp = 1;												
												lista_words[i]->m_pRange2->Paragraphs->Item(pp)->LineSpacingRule = Word::wdLineSpaceExactly;
												lista_words[i]->m_pRange2->Paragraphs->Item(pp)->SpaceBefore = (float)lineao;
											}

										    lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes = v1;
										    lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll = v2;										
										}
										else
										    lista_words[i]->m_pRange2->FormattedText = lista_words[i]->m_pRange->FormattedText;
									}
									else
									{										
										if (tmpxx[1] == '+')
										{
											lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range();
											lista_words[i]->m_pRange->Collapse(&_variant_t((long)Word::wdCollapseEnd));
										}
										AsignaTextoAWord(lista_words[i]->m_pRange,ptmp,i);
									}
								}
								break;
							case '$':
								{
									_variant_t tipo;
									long count = -1;
									int xx = 1;
									if (tmpxx[xx] == '+')
									{
										xx++;
										lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range();
									}
									if (lista_words[i]->m_pRange)
									{
										lista_words[i]->m_pRange->Collapse(&_variant_t((long)Word::wdCollapseEnd));
										lista_words[i]->m_Fields = lista_words[i]->m_pRange->Fields;
										
										if (!memcmp(tmpxx+xx,"QUOTE",5))
										{
											tipo = (long)Word::wdFieldQuote;
											count = 0;
											xx += 6;
										}
										else
										{
											tipo = (long)Word::wdFieldPrivate;
										}											
										lista_words[i]->m_Fields->Add(lista_words[i]->m_pRange,&tipo,&_variant_t(_bstr_t((LPCSTR)(ptmp))),&_variant_t((long)FALSE));
										if (count > -1)
										{
											lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range();
											lista_words[i]->m_Fields = lista_words[i]->m_pRange->Fields;
											count = lista_words[i]->m_Fields->Count;											
											AsignaTextoAWord(lista_words[i]->m_Fields->Item(count)->Result,tmpxx+xx,i);
										}
									}
								}
								break;
							case '%': // fonts
								{
								    PonFont(tmpxx+2,(float)atof(ptmp),tmpxx[1]);
								}
								break;
							case '/':
								{
									lista_words[i]->m_pDoc->PageSetup->LeftMargin = (float)atof(tmpxx+1);
            						lista_words[i]->m_pDoc->PageSetup->RightMargin = (float)atof(tmpxx+1);									
								}
								break;
							case '\\':
								{
									lista_words[i]->m_pDoc->PageSetup->TopMargin = (float)atof(tmpxx+1);
									lista_words[i]->m_pDoc->PageSetup->BottomMargin = (float)atof(tmpxx+1);									
								}
								break;
							case '>':
								{
									_variant_t start,end;
									_variant_t v1,v2;
									long lineao,linead;
									Word::RangePtr R;
									static long o_linead = 0;
									if (doble)
									{										
										v1 = lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes;
										v2 = lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll;
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes = VARIANT_FALSE;
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll = VARIANT_FALSE;
										
										start = (long)atol(tmpxx+1);
										if (*ptmp == '/')
										{
											Word::ShapePtr L = NULL;
											float pw = lista_words[i]->m_pDoc2->PageSetup->PageWidth;
											float lm = lista_words[i]->m_pDoc2->PageSetup->LeftMargin;
											float rm = lista_words[i]->m_pDoc2->PageSetup->RightMargin;

											end = (long)(((long)start) + (long)1);
											R = lista_words[i]->m_pDoc->Range(&start,&end);
											lineao = R->GetInformation(Word::wdVerticalPositionRelativeToPage);											
											L = lista_words[i]->m_pDoc2->Shapes->AddLine((float)lm,(float)o_linead,(float)(pw-rm),(float)lineao,&vtMissing);
											if (L != NULL)
											{
												L->Line->Weight = (float)0.25;
												L->Line->PutDashStyle(Office::msoLineRoundDot);
											}
										}
										else
										{
											end   = (long)atol(ptmp);											
											R = lista_words[i]->m_pDoc->Range(&start,&end);

											/* TRATAMIENTO A BASE DE INSERTAR UN PARRAFO */
											lineao = R->GetInformation(Word::wdVerticalPositionRelativeToPage);
											lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
											linead = lista_words[i]->m_pRange2->GetInformation(Word::wdVerticalPositionRelativeToPage);
											//lineao /= 20;
											//linead /= 20;
											if (linead < lineao)
											{
												long pp;
												start = lista_words[i]->m_pRange2->Start;
												pp = start;
												if (pp > 0)
												{
													pp -= 1;
													start = pp;
													lista_words[i]->m_pRange2->Start = start;											
													pp = lista_words[i]->m_pRange2->Paragraphs->Count;
													lista_words[i]->m_pRange2->Paragraphs->Item(pp)->LineSpacingRule = Word::wdLineSpaceExactly;
													lista_words[i]->m_pRange2->Paragraphs->Item(pp)->SpaceAfter = (float)(lineao-linead);
													lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
												}
												while(*ptmp && *ptmp != '/') ptmp++;
												if (*ptmp == '/')
												{
													o_linead = linead;
												}
											}											
										}
										
										/* TRATAMIENTO A BASE DE CREAR UN FRAME */
										/* con twips 
										lineao = R->GetInformation(Word::wdVerticalPositionRelativeToPage);
										lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
										linead = lista_words[i]->m_pRange2->GetInformation(Word::wdVerticalPositionRelativeToPage);
										lineao /= 20;
										linead /= 20;
										if (linead < lineao)
										{
											lista_words[i]->m_pDoc2->Shapes->AddTextbox(Office::msoTextOrientationHorizontal,1,linead,1,lineao-linead);
											lista_words[i]->m_pRange2 = lista_words[i]->m_pDoc2->Range();
											lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
										}
										*/


										/* TRATAMIENTO A BASE DE LINEAS
										lineao = R->GetInformation(Word::wdFirstCharacterLineNumber);
										lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
										linead = lista_words[i]->m_pRange2->GetInformation(Word::wdFirstCharacterLineNumber);
										while(linead < lineao)
										{
											lista_words[i]->m_pRange2->Text = _bstr_t((LPCSTR)"\n");

										 	lista_words[i]->m_pRange2->Font->Name = R->Font->Name;
											lista_words[i]->m_pRange2->Font->Size = R->Font->Size;
											lista_words[i]->m_pRange2->Font->Bold = R->Font->Bold;
											lista_words[i]->m_pRange2->Bold = R->Bold;

											lista_words[i]->m_pRange2->Collapse(&_variant_t((long)Word::wdCollapseEnd));
											linead++;
										}
										*/

										lista_words[i]->m_pDoc->ActiveWindow->View->ShowFieldCodes = v1;
										lista_words[i]->m_pDoc->ActiveWindow->View->ShowAll = v2;										
									}
								}
								break;
                            default:
                                {
                                    lista_words[i]->m_pRange  = lista_words[i]->m_pDoc->Range();
									AsignaTextoAWord(lista_words[i]->m_pRange,ptmp,i);
									::sprintf(puntero,"%d",sal);
                                }
                                break;
                            }                            
                        }
                        catch( ... )
                        {
                            sal = -1;
                        }
                    }
                }
            }
			if (sal < 0)
			{
				::sprintf(puntero,"%d",sal);
			}
        }
		break;
    }
}
#else
void ControlWord_(int entero,char *puntero)
{
    switch(entero)
    {
    case 0:
        {
			int i;
            for (i = 0;i < nlista_words;i++)
            {
                if (!lista_words[i])
                {
                    break;
                }
                else
                {
                    if (lista_words[i]->m_paramorir)
                    {
                        delete lista_words[i];
                        lista_words[i] = NULL;
                        break;
                    }
                }
            }
            lista_words[i] = new CDsWord();

            CString ss;
            if (puntero[0] == ':' && puntero[1] == '/')
            {
                ss = theApp.m_dirbase;
                ss += (LPCSTR)(puntero+2);                
            }
            else
            {
                ss = (LPCSTR)puntero;
            }
            ss.Replace('/','\\');

            if (lista_words[i]->AbreWord(ss))
            {
                lista_words[i]->m_pWord->Visible = VARIANT_FALSE;
                if (i == nlista_words)
                {
                    nlista_words++;
                }
		        lista_words[i]->m_WordEventSink.Advise(lista_words[i]->m_pWord, IID_IWordAppEventSink);
		        //ASSERT(Res == TRUE);

		        lista_words[i]->m_WordEventSink.Advise(lista_words[i]->m_pDoc, IID_IWordDocEventSink);
                lista_words[i]->m_WordEventSink.m_indice = i;
		        //ASSERT(Res == TRUE);
            }
            else
            {
                delete lista_words[i];
                i = -1;
            }
            ::sprintf(puntero,"%d",i);
        }
        break;
    case 1:
        {
            int i = ::atoi(puntero);
                if (i >= 0 && i < nlista_words && lista_words[i])
                {
	                try
                    {    
						char *ptmp;

                        for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                        if (*ptmp == 1)
                        {
                            ptmp++;
                            if (*ptmp)
                            {
                                CString ss;

                                if (ptmp[0] == ':' && ptmp[1] == '/')
                                {
                                    ss = theApp.m_dirbase;
                                    ss += (LPCSTR)(ptmp+2);                
                                }
                                else
                                {
                                    ss = (LPCSTR)ptmp;
                                }
                                ss.Replace('/','\\');

                                lista_words[i]->m_pDoc->SaveAs(&_variant_t(ss));
                            }
                            else
                            {   
                                lista_words[i]->m_pDoc->Save();
                            }
                        }
                        lista_words[i]->m_pWord->Quit(&_variant_t(VARIANT_FALSE));
                    }
	                catch (...)
	                {
		                i = -1;
	                }
                    if (lista_words[i])
                    {
                        delete lista_words[i];
                        lista_words[i] = NULL;
                    }
                }
                else
                    i = -1;
            ::sprintf(puntero,"%d",i);
        }
        break;
    case 2:
        {
            char tmpxx[512];
            int i = ::atoi(puntero);
            int sal = -1;            
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        ::strupr(tmpxx);
                        ::strupr(ptmp);
                        if (!strcmp(tmpxx,"QUIT"))
                        {
                            sal = 0;
                            try
                            {
                                if (!strcmp(ptmp,"SI"))
                                {
                                    lista_words[i]->m_pWord->Quit(&_variant_t(VARIANT_FALSE));
                                    delete lista_words[i];
                                    lista_words[i] = NULL;
                                    strcpy(puntero,"SI");
                                } 
                                else if (lista_words[i]->m_paramorir)
                                {
                                    delete lista_words[i];
                                    lista_words[i] = NULL;
                                    strcpy(puntero,"SI");
                                }
                                else
                                {
                                    strcpy(puntero,"NO");
                                    ::Sleep(100);
                                }
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }
                        else if (!strcmp(tmpxx,"VISIBLE"))
                        {                            
                            sal = 0;
                            try
                            {
                                if (!strcmp(ptmp,"MINIMIZADO"))
                                {
									lista_words[i]->m_pWord->Visible = VARIANT_TRUE;
									lista_words[i]->m_pWord->WindowState = Word::wdWindowStateMinimize;
								}
								else if (!strcmp(ptmp,"SI"))
                                {
                                    lista_words[i]->m_pWord->Visible = VARIANT_TRUE;
                                }
                                else if (!strcmp(ptmp,"NO"))                                
                                {
                                    lista_words[i]->m_pWord->Visible = VARIANT_FALSE;                                    
                                }
                                if (lista_words[i]->m_pWord->Visible == VARIANT_TRUE)
                                {
                                    strcpy(puntero,"SI");
                                }
                                else
                                {
                                    strcpy(puntero,"NO");
                                }
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }
                        else if (!strcmp(tmpxx,"LIMPIAFINAL"))
                        {
                            sal = 0;
                            long mipunto = 0L;
                            try
                            {
                                do
                                {
                                    long start = lista_words[i]->m_pDoc->Range()->End - 2;
                                    long end = lista_words[i]->m_pDoc->Range()->End-1;

                                    if (start == mipunto)
                                    {
                                        break;
                                    }
                                    mipunto = start;

                                    lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range(&_variant_t(start),&_variant_t(end));
                                    _bstr_t tt = lista_words[i]->m_pRange->Text;
                                    char *ptt = tt;                                    
									if (!ptt)
										break;
                                    if (tt.length() == 1 && ptt[0] == ' ' || ptt[0] == (char)13 || ptt[0] == (char)10)
                                    {
                                        lista_words[i]->m_pRange->Delete();
                                        sal++;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                } while(-1);
                                ::sprintf(puntero,"%d",sal);
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }
                        else if (!strcmp(tmpxx,"PAGINACION"))
                        {
                            sal = 0;                            
                            try
                            {
                                  
                                  long sp,np;
                                  if (*ptmp == '=')
                                  {
                                      sp = atoi(ptmp+1);
                                      lista_words[i]->m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->PageNumbers->StartingNumber = sp;
                                  }
                                  sp = lista_words[i]->m_pDoc->Sections->Item(1)->Footers->Item(Word::wdHeaderFooterPrimary)->PageNumbers->StartingNumber;                                  
                                  np = lista_words[i]->m_pDoc->Sections->Item(1)->Range->GetInformation(Word::wdNumberOfPagesInDocument);
                                  ::sprintf(puntero,"%ld:%ld,%ld",sp+np,sp,np);
                            }
                            catch( ... )
                            {
                                sal = -1;
                            }
                        }

                    }
                }
            }            
            if (sal == -1)
            {
                ::sprintf(puntero,"%d",sal);
            }
        }
        break;
    case 3:
        {
            char tmpxx[512];
            int i = ::atoi(puntero);
            int sal = -1;
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        sal = 0;

                        CString ss;

                        if (ptmp[0] == ':' && ptmp[1] == '/')
                        {
                            ss = theApp.m_dirbase;
                            ss += (LPCSTR)(ptmp+2);                
                        }
                        else
                        {
                            ss = (LPCSTR)ptmp;
                        }
                        ss.Replace('/','\\');

                        ptmp = (char *)(LPCSTR)ss;

                        try
                        {
                            switch(tmpxx[0])
                            {
                            case '!':
                                {
									if (_BuscaTextoWord(i,tmpxx+1))
									{
                                        //lista_words[i]->m_Selection->Range->Delete();
										if (ptmp[0] == '@')
										{
											_variant_t a(VARIANT_FALSE),b(VARIANT_TRUE),c((IUnknown*)lista_words[i]->m_Selection->Range);
											_bstr_t s = (const char *)(ptmp+1);
											Word::InlineShapesPtr Shapes;
											lista_words[i]->m_Selection->Range->Delete();
											//::MessageBox(NULL,s,"AddPicture",MB_OK);
											Shapes = lista_words[i]->m_pDoc->GetInlineShapes();
											Shapes->AddPicture(s, &a, &b, &c);
											//::MessageBox(NULL,"Ok ... al menos llego","AddPicture",MB_OK);
										}
										else
										{										
											lista_words[i]->m_Selection->Range->Delete();
											lista_words[i]->m_Selection->Range->InsertFile(_bstr_t((LPCSTR)ptmp));
										}
                                    }
                                    else
                                        sal = -1;
                                }
                                break;
                            case '#':
                                {
                                    long nf = (long)::atoi(tmpxx+1);
                                    lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range()->Fields->Item(nf)->Result;
                                    lista_words[i]->m_pRange->InsertFile(_bstr_t((LPCSTR)ptmp));
                                }
                                break;
                            case '&':
                                {   
                                    lista_words[i]->m_Fields = lista_words[i]->m_pDoc->Range()->Fields;
                                    long nc = lista_words[i]->m_Fields->Count;
									long cc;
                                    for (cc = 1L;cc <= nc;cc++)
                                    {
                                        if (::strstr((const char *)lista_words[i]->m_Fields->Item(cc)->Code->Text,(const char *)(tmpxx+1)))
                                        {
                                            lista_words[i]->m_pRange = lista_words[i]->m_Fields->Item(cc)->Result;
                                            lista_words[i]->m_pRange->InsertFile(_bstr_t((LPCSTR)ptmp));
                                            break;
                                        }
                                    }                                    
                                }
                                break;
                            default:
                                {
                                    lista_words[i]->m_pRange  = lista_words[i]->m_pDoc->Range();
                                    lista_words[i]->m_pRange->InsertFile(_bstr_t((LPCSTR)ptmp));
                                }
                                break;
                            }                            
                        }
                        catch( ... )
                        {
                            sal = -1;
                        }
                    }
                }
            }
            ::sprintf(puntero,"%d",sal);
        }
        break;
    case 4:
        {
            char tmpxx[512];
            int i = ::atoi(puntero);
            int sal = -1;
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        sal = 0;
                        try
                        {
                            
                            switch(tmpxx[0])
                            {
                            case '#':
                                {
                                    long nf = (long)::atoi(tmpxx+1);
                                    lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range()->Fields->Item(nf)->Result;
                                    lista_words[i]->m_pRange->Text = _bstr_t((LPCSTR)ptmp);
                                }
                                break;
                            case '&':
                                {                                       
                                    lista_words[i]->m_Fields = lista_words[i]->m_pDoc->Range()->Fields;
                                    long nc = lista_words[i]->m_Fields->Count;
                                    for (long cc = 1L;cc <= nc;cc++)
                                    {
                                        if (::strstr((const char *)lista_words[i]->m_Fields->Item(cc)->Code->Text,(const char *)(tmpxx+1)))
                                        {
                                            lista_words[i]->m_pRange = lista_words[i]->m_Fields->Item(cc)->Result;
                                            lista_words[i]->m_pRange->Text = _bstr_t((LPCSTR)ptmp);
                                        }
                                    }                                    
                                }
                                break;
                            default:
                                {
									if (_BuscaTextoWord(i,tmpxx))
									{
                                        //lista_words[i]->m_Selection->Range->Delete();                                        
                                        lista_words[i]->m_Selection->Range->InsertFile(_bstr_t((LPCSTR)ptmp));
									}                                    
									else
										sal = -1;
                                }
                                break;
                            } 
                            
                        }
                        catch( ... )
                        {
                            sal = -1;
                        }
                    }


                    /*
                    for (;*ptmp && *ptmp != 1;ptmp++);
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        //Range(&_variant_t((long)0),&_variant_t((long)0));
                        long start = lista_words[i]->m_pDoc->Paragraphs->Last->Range->End - 1;
                        long end = start;

                        long rc = lista_words[i]->m_pDoc->StoryRanges->Count;

                        enum Word::WdStoryType sto = (enum Word::WdStoryType)1;
                        bool ok;

                        for (long ri = 0;ri <= 20;ri++)
                        {
                            //StoryRanges
                            //MailMerge
                            ok = true;
                            try
                            {
                                lista_words[i]->m_pRange = lista_words[i]->m_pDoc->StoryRanges->Item(sto);
                            }
                            catch(...)
                            {
                                ok = false;
                            }

                            sto = (enum Word::WdStoryType)((long)sto + 1);

                            if (ok)
                            {                                
                                lista_words[i]->m_Fields = lista_words[i]->m_pRange->Fields;
                                long count = lista_words[i]->m_Fields->Count;                            
                                for (long ii = 1;ii <= count;ii++)
                                {
                                    //enum Word::WdFieldType typo;
                                    //lista_words[i]->m_Field = lista_words[i]->m_Fields->Item(ii);
                                    //typo = lista_words[i]->m_Field->GetType();
                                    //if (typo == Word::wdFieldQuote)
                                    //{                                
                                        //if (::strstr(lista_words[i]->m_Fields->Item(ii)->Code->Text,"campo"))
                                        {
                                            sprintf(tmpxx,"es el numero %ld",ii);
                                            lista_words[i]->m_Fields->Item(ii)->Result->Text = _bstr_t(tmpxx);
                                        }
                                    //}
                                }
                            }
                        }
                        
                        //lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Range(&_variant_t(start),&_variant_t(end));
                        
                        //lista_words[i]->m_pRange->InsertFile(_bstr_t(ptmp));
                        //InsertAfter(_bstr_t((LPCSTR)s));
                        //lista_words[i]->m_pDoc->Merge(_bstr_t(ptmp));
                            //SaveAs(&_variant_t((LPCSTR)ptmp));
                    }
                    */
                }                
            }
            else
                i = -1;
            ::sprintf(puntero,"%d",i);
        }
        break;
    case 5:
        {
            int i = ::atoi(puntero);
                if (i >= 0 && i < nlista_words && lista_words[i])
                {
	                try
                    {
						char *ptmp;
                        for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                        if (*ptmp == 1)
                        {
                            ptmp++;
                            ::strupr(ptmp);
                            if (!::strcmp(ptmp,"DIALOGO"))
                            {   
                                lista_words[i]->m_pDoc->Activate();
                                /*i = (int)*/lista_words[i]->m_pWord->Dialogs->Item(Word::wdDialogFilePrint)->Show();
                            }
                            else
                            {
                                lista_words[i]->m_pDoc->PrintOut();
                            }
                        }                                                
                    }
	                catch (...)
	                {
		                i = -1;
	                }
                }
                else
                    i = -1;
            ::sprintf(puntero,"%d",i);
        }
        break;
	case 6:
        {
            char tmpxx[512];
            int i = ::atoi(puntero);
            int sal = -1;
			int multiple = 0,k;
            if (i >= 0 && i < nlista_words && lista_words[i])
            {
				char *ptmp;
                for (ptmp = puntero;*ptmp && *ptmp != 1;ptmp++);
                if (*ptmp == 1)
                {
                    ptmp++;
                    int punto = 0;
                    for (;*ptmp && *ptmp != 1;ptmp++)
                    {
                        tmpxx[punto] = *ptmp;
                        punto++;
                    }                        
                    if (*ptmp == 1)
                    {
                        ptmp++;
                        tmpxx[punto] = 0;
                        sal = 0;

						for (k = 0;ptmp[k] && ptmp[k] != 2;k++);
						if (ptmp[k] == 2)
						{
							int xx = strlen(tmpxx);
							multiple = 1;
							xx++;							
							while(xx > 0)
							{
								tmpxx[xx] = tmpxx[xx-1];
								xx--;								
							}							
							tmpxx[0] = '!';
						}

						/*
                        CString ss;

                        if (ptmp[0] == ':' && ptmp[1] == '/')
                        {
                            ss = theApp.m_dirbase;
                            ss += (LPCSTR)(ptmp+2);                
                        }
                        else
                        {
                            ss = (LPCSTR)ptmp;
                        }
                        ss.Replace('/','\\');

                        ptmp = (char *)(LPCSTR)ss;
						*/

                        try
                        {
                            switch(tmpxx[0])
                            {
                            case '!':
                                {
									// analizar comando multiple 
									sal = 0;
									char tmpx1[512];
									char tmpx2[512];
									char *ptmpx;
									int nn;
									if (multiple)
									{
										ptmpx = ptmp;
										for (k = 0;k < atoi(tmpxx+1);k++)
										{
											tmpx1[0] = 0;
											tmpx2[0] = 0;
											if (*ptmpx)
											{
												for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
												{
													tmpx1[nn] = *ptmpx;
													ptmpx++;
												}
												tmpx1[nn] = 0;
												if (*ptmpx)
												{
													ptmpx++;
													for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
													{
														tmpx2[nn] = *ptmpx;
														ptmpx++;
													}
													tmpx2[nn] = 0;
													if (*ptmpx)
													{
														ptmpx++;
													}
												}
											}
											do
											{
												if (_BuscaTextoWord(i,tmpx1))
												{
													//lista_words[i]->m_Selection->Range->Delete();
													lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)tmpx2);
													sal++;
												}
												else												
												    break;
											} while(-1);
										}
										ptmpx = ptmp;
										for (k = 0;k < atoi(tmpxx+1);k++)
										{
											tmpx1[0] = 0;
											tmpx2[0] = 0;
											if (*ptmpx)
											{
												for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
												{
													tmpx1[nn] = *ptmpx;
													ptmpx++;
												}
												tmpx1[nn] = 0;
												if (*ptmpx)
												{
													ptmpx++;
													for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
													{
														tmpx2[nn] = *ptmpx;
														ptmpx++;
													}
													tmpx2[nn] = 0;
													if (*ptmpx)
													{
														ptmpx++;
													}
												}
											}
											Word::ShapesPtr Shapes;
											Shapes = lista_words[i]->m_pDoc->GetShapes();
											long nc = Shapes->GetCount();
											long icc;
											for (icc = 1; icc <= nc;icc++)
											{
												do
												{
													Shapes->Item(&_variant_t(icc))->Select();
													lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
													lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpx1));
													if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
													{
														//lista_words[i]->m_Selection->Range->Delete();
														lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)tmpx2);
														sal++;
													}
													else
													{
														break;
													}
												} while(-1);
											}
										}
										ptmpx = ptmp;
										for (k = 0;k < atoi(tmpxx+1);k++)
										{
											tmpx1[0] = 0;
											tmpx2[0] = 0;
											if (*ptmpx)
											{
												for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
												{
													tmpx1[nn] = *ptmpx;
													ptmpx++;
												}
												tmpx1[nn] = 0;
												if (*ptmpx)
												{
													ptmpx++;
													for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
													{
														tmpx2[nn] = *ptmpx;
														ptmpx++;
													}
													tmpx2[nn] = 0;
													if (*ptmpx)
													{
														ptmpx++;
													}
												}
											}
											Word::FramesPtr Frames;

											Frames = lista_words[i]->m_pDoc->GetFrames();

											long nc = Frames->GetCount();
											long icc;
											for (icc = 0; icc < nc;icc++)
											{
												do
												{
												Frames->Item(icc)->Select();
												lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
												lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpx1));
												if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
												{
													//lista_words[i]->m_Selection->Range->Delete();
													lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)tmpx2);
													sal++;
												}
												else
												{
													break;
												}
												} while(-1);
											}
										}
										ptmpx = ptmp;
										for (k = 0;k < atoi(tmpxx+1);k++)
										{
											tmpx1[0] = 0;
											tmpx2[0] = 0;
											if (*ptmpx)
											{
												for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
												{
													tmpx1[nn] = *ptmpx;
													ptmpx++;
												}
												tmpx1[nn] = 0;
												if (*ptmpx)
												{
													ptmpx++;
													for (nn = 0;*ptmpx && *ptmpx != 2;nn++)
													{
														tmpx2[nn] = *ptmpx;
														ptmpx++;
													}
													tmpx2[nn] = 0;
													if (*ptmpx)
													{
														ptmpx++;
													}
												}
											}
											Word::FormFieldsPtr FormFields;

											FormFields =  lista_words[i]->m_pDoc->GetFormFields();
											long nc = FormFields->GetCount();
											long icc;
											for (icc = 1; icc <= nc;icc++)
											{				
												do
												{
												FormFields->Item(&_variant_t(icc))->Select();
												lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
												lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpx1));
												if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
												{
													//lista_words[i]->m_Selection->Range->Delete();
													lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)tmpx2);
													sal++;
												}
												else
												{
													break;
												}
												} while(-1);
											}
										}
									}
									else
									{
										if (_BuscaTextoWord(i,tmpxx+1))
										{
											//lista_words[i]->m_Selection->Range->Delete();
											lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)ptmp);
										}
										else
										{		
											Word::ShapesPtr Shapes;
											Shapes = lista_words[i]->m_pDoc->GetShapes();
											long nc = Shapes->GetCount();
											long icc;
											for (icc = 1; icc <= nc;icc++)
											{
												do
												{
												Shapes->Item(&_variant_t(icc))->Select();
												lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
												lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx+1));
												if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
												{
													//lista_words[i]->m_Selection->Range->Delete();
													lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)ptmp);
													break;
												}
												else
												{
													break;
												}
												} while(-1);
											}

											if (icc >= nc)
											{
												Word::FramesPtr Frames;

												Frames = lista_words[i]->m_pDoc->GetFrames();

												long nc = Frames->GetCount();
												long icc;
												for (icc = 0; icc < nc;icc++)
												{
													do
													{
													Frames->Item(icc)->Select();
													lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
													lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx+1));
													if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
													{
														//lista_words[i]->m_Selection->Range->Delete();
														lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)ptmp);
														break;
													}
													else
													{
														break;
													}
													}while(-1);
												}

												if (icc >= nc)
												{
													Word::FormFieldsPtr FormFields;											

													FormFields =  lista_words[i]->m_pDoc->GetFormFields();
													long nc = FormFields->GetCount();
													long icc;
													for (icc = 1; icc <= nc;icc++)
													{		
														do
														{
														FormFields->Item(&_variant_t(icc))->Select();
														lista_words[i]->m_Selection = lista_words[i]->m_pWord->Selection;
														lista_words[i]->m_Selection->Find->Execute(&_variant_t((LPCSTR)tmpxx+1));
														if (lista_words[i]->m_Selection->Find->Found == VARIANT_TRUE)
														{
															//lista_words[i]->m_Selection->Range->Delete();
															lista_words[i]->m_Selection->Range->Text = _bstr_t((LPCSTR)ptmp);
															break;
														}
														else
														{
															break;
														}
														} while(-1);
													}
													if (icc >= nc)
														sal = -1;
												}
											}
										}
									}
                                }
                                break;
							case '%': // fonts
								{
								    PonFont(tmpxx+2,(float)atof(ptmp),tmpxx[1]);
								}
								break;
							case '}':
								{
									int j,k,l = 0;
									int tabla = 1,fila = 1,columna = 1;
									char tmp[512];
									strcpy(tmp,tmpxx+1);
									k = 0;
									for (j = 0;tmp[j];j++)
									{
										if (tmp[j] == '}')
										{
											tmp[j] = 0;
											if (l == 0)
												tabla = atoi(tmp+k);
											else if (l == 1)
												fila = atoi(tmp+k);
											else if (l == 2)
											{
												columna = atoi(tmp+k);
												k = j + 1;
												break;
											}
											k = j + 1;
											l++;											
										}
									}
									{
										Word::CellPtr micell;
										micell = lista_words[i]->m_pDoc->Tables->Item(tabla)->Cell(fila,columna);
										//lista_words[i]->m_pDoc->Tables->Item(tabla)->Rows->Item(fila)->Cells->Item(columna);
										if (tmp[k] == '+')
										{
											_variant_t t;											
											k++;
											if (tmp[k] == 'F')
											{												
												t = micell->Row;
												for (j = 0;j < atoi(ptmp);j++)
												{
													lista_words[i]->m_pDoc->Tables->Item(tabla)->Rows->Add(/*&t*/);
												}
											}
											else if (tmp[k] == 'C')
											{
												t = micell;
												for (j = 0;j < atoi(ptmp);j++)
												{
													lista_words[i]->m_pDoc->Tables->Item(tabla)->Rows->Item(fila)->Cells->Add(/*&t*/);
												}
											}
											else if (tmp[k] == 'G')
											{
												lista_words[i]->m_pDoc->Tables->Item(tabla)->Rows->Item(fila)->Select();
												lista_words[i]->m_pWord->Selection->Copy();
											}											
											else if (tmp[k] == 'P')
											{												
												lista_words[i]->m_pRange = lista_words[i]->m_pDoc->Tables->Item(tabla)->Range;
												lista_words[i]->m_pRange->Start = lista_words[i]->m_pRange->End;
												lista_words[i]->m_pRange->Paste();
											}
										}
										else
										{
											if (tmp[k] == '/')
											{
												micell->Range->Text = micell->Range->Text+_bstr_t((LPCSTR)ptmp);
											}
											else
											{
												micell->Range->Text = _bstr_t((LPCSTR)ptmp);
											}
										}
									}									
								}
								break;
                            default:
                                {
                                    lista_words[i]->m_pRange  = lista_words[i]->m_pDoc->Range();
                                    lista_words[i]->m_pRange->Text = _bstr_t((LPCSTR)ptmp);
                                }
                                break;
                            }                            
                        }
                        catch( ... )
                        {
                            sal = -1;
                        }
                    }
                }
            }
            ::sprintf(puntero,"%d",sal);
        }
		break;
	case 7:
		{
			int i = ::atoi(puntero);
			if (i >= 0 && i < nlista_words && lista_words[i])
			{
				_variant_t v1,v2;				
				Word::_DocumentPtr pDoc = lista_words[i]->m_pDoc;
				Word::RangePtr R;
				_bstr_t txt;
				//char *tt;
				CString s;
				
				v1 = pDoc->ActiveWindow->View->ShowFieldCodes;
				v2 = pDoc->ActiveWindow->View->ShowAll;
				pDoc->ActiveWindow->View->ShowFieldCodes = VARIANT_TRUE;
				pDoc->ActiveWindow->View->ShowAll = VARIANT_TRUE;

				R = pDoc->Range();
				
				txt = R->Text;		
				//tt = (char *)malloc(txt.length()+1);
				//strcpy(tt,txt);
				//R->Text = _bstr_t(tt);
				//free(tt);			
				s = (char *)txt;
				R->Text = _bstr_t(s);


				pDoc->ActiveWindow->View->ShowFieldCodes = v1;
				pDoc->ActiveWindow->View->ShowAll = v2;
			}
		}
    }
}
#endif