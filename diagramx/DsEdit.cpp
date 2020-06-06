
// DsEdit.cpp : implementation file
// Edicion mono linea (campos)

#include "stdafx.h"
#include "Diagramx.h"
#include "DsEdit.h"
#include "DsCombo.h"
#include "DsxDoc.h"
#include "DsxView.h"
#include "DsStdDlg.h"
#include "DsTabCtrl.h"
#include "TablaView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDsEdit

CDsEdit::CDsEdit()
{
   m_seleccionado = FALSE;
   m_pBkBrush = NULL;
   m_multilinea = FALSE;
   m_maxqc = 0;
   m_qi[0] = 0;
   m_qs[0] = 0;
   m_tipo = 0;
   m_decimales = 0;
   m_qh = 0;
   m_flag = 0;
   m_overwrite = FALSE;
   m_desactivado = 0;
   m_pView = NULL;
   m_pDoc  = NULL;
   m_dsctrl = NULL;
   /*m_metacombo = NULL;*/
   m_estilo = -1;
   m_qc = 0;  
   
   m_entabla = NULL;
   m_fococonfirmado = FALSE;
   m_no999 = FALSE;

   m_pCombo = NULL;

   m_comborel = FALSE;

   m_otrofoco = NULL;

   m_noConfirma = FALSE;

   m_Attrib = 0;
   m_Cc = 0;
   m_Cf = 0;

   m_justificado = FALSE;

   m_teclaseleccion = 0;
}

CDsEdit::CDsEdit(BOOL multilinea)
{
   
   m_seleccionado = FALSE;
   m_pBkBrush = NULL;
   m_multilinea = FALSE;
   m_maxqc = 0;
   m_qi[0] = 0;
   m_qs[0] = 0;
   m_tipo = 0;
   m_decimales = 0;
   m_qh = 0;
   m_flag = 0;
   m_overwrite = FALSE;
   m_desactivado = 0;
   m_pView = NULL;
   m_pDoc  = NULL;
   m_dsctrl = NULL;
   /*m_metacombo = NULL;*/
   m_estilo = -1;
   m_qc = 0;
   
   m_multilinea = multilinea;
   m_entabla = NULL;
   m_fococonfirmado = FALSE;
   m_no999 = FALSE;

   m_pCombo = NULL;

   m_comborel = FALSE;

   m_otrofoco = NULL;

   m_noConfirma = FALSE;

   m_Attrib = 0;
   m_Cc = 0;
   m_Cf = 0;

   m_justificado = FALSE;

   m_teclaseleccion = 0;
}

CDsEdit::~CDsEdit()
{
    if (theApp.m_ctrlactivo == m_dsctrl)
    {        
        theApp.m_ctrlactivo = NULL;
    }
    else if (theApp.m_antctrlactivo == m_dsctrl)
    {
        theApp.m_antctrlactivo = NULL;
    }

    if (m_entabla)
    {
        m_entabla->m_editando = FALSE;        
    }

    if (m_pBkBrush)
        delete m_pBkBrush;
}

BEGIN_MESSAGE_MAP(CDsEdit, BASECDsEdit)
//{{AFX_MSG_MAP(CDsEdit)
    ON_WM_CHAR()	
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
    ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()



void CDsEdit::OnPaint() 
{		
	BASECDsEdit::OnPaint();
	return;
}

BOOL CDsEdit::OnEraseBkgnd(CDC* pDC)
{
	return BASECDsEdit::OnEraseBkgnd(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CDsEdit message handlers
static BOOL es_caracter(int nchar)
{    
 switch(nchar)
 {
 case VK_LBUTTON:
 case    VK_RBUTTON:
 case    VK_CANCEL :
 case VK_MBUTTON:
 case VK_BACK : 
 case VK_TAB:   
 case VK_CLEAR  :
 case VK_RETURN :
 case VK_SHIFT  :
 case VK_CONTROL:
 case VK_MENU : 
 case VK_PAUSE  :
 case VK_CAPITAL:
 case VK_KANA :  
 case VK_JUNJA :  
 case VK_FINAL    :
 case VK_KANJI  :   
 case VK_ESCAPE     :
 case VK_CONVERT  : 
 case VK_NONCONVERT :
 case VK_ACCEPT   : 
 case VK_MODECHANGE :
 case VK_PRIOR    : 
 case VK_NEXT   :  
 case VK_END  :      
 case VK_HOME       :
 case VK_LEFT     :  
 case VK_UP     :    
 case VK_RIGHT:      
 case VK_DOWN       :
 case VK_SELECT   :  
 case VK_PRINT  :    
 case VK_EXECUTE    :
 case VK_SNAPSHOT :  
 case VK_INSERT :    
 case VK_DELETE     :
 case VK_HELP     :  
 case VK_LWIN   :    
 case VK_RWIN :      
 case VK_APPS       :
 case VK_NUMLOCK    :
 case VK_SCROLL:     
 case VK_LSHIFT     :
 case VK_RSHIFT   :  
 case VK_LCONTROL   :
 case VK_RCONTROL :  
 case VK_LMENU  :    
 case VK_RMENU:      
 case VK_ATTN       :
 case VK_CRSEL    :  
 case VK_EXSEL  :    
 case VK_EREOF:      
 case VK_PLAY :      
 case VK_ZOOM:      
 case VK_NONAME :   
 case VK_PA1  :    
 case VK_OEM_CLEAR :
     return FALSE;
 default:
     break;
 }
return TRUE;
}




void CDsEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    WPARAM  comando = 0;

    /*
    if (!m_multilinea)
    {
        comando = CDsStdDlg::DeterminaTecla(nChar);        
    }
   else
   {
       if (nChar == VK_ESCAPE)
	      {
           comando = 806;
	      }
   }
   */
   if  (comando)
   {
       TeclaStd(comando);
   }
   else
   {
       int isp = isprint(nChar);
       int startPos, endPos;
       if (m_overwrite && isp)
       {            
           GetSel(startPos, endPos);
           if (startPos == endPos)
           {
               SetSel(startPos, endPos+1);
               Clear();
           }	  
       }
       if (isp && (m_tipo == 3 || m_tipo == 2))
       {		   
		   if ((nChar < '0' || nChar > '9') && nChar != '.' && nChar != ',' && nChar != '-') 
		   {
			   GetSel(startPos, endPos);
			   return;
		   }
		   /*
           if ((nChar >= 'A' && nChar <= 'Z') || (nChar >= 'a' && nChar <= 'z')) 
		   {
			   GetSel(startPos, endPos);
			   if (m_tipo != 3 || startPos || (nChar != 'E' && nChar != 'P'))
					return;
		   }
		   */
		   if (m_tipo == 2)
		   {
			   GetSel(startPos, endPos);
			   if (startPos == 0)
			   {

				   CString s;
				   GetWindowText(s);
				   if (!s.Compare("01.01.0000"))
				   {					   
					   s.Format("%02d.%02d.%04d",CTime::GetCurrentTime().GetDay(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetYear());
					   SetWindowText(s);
					   SetSel( 0, 2);
				   }

			   }
		   }
       }

	   if (m_tipo == 1 && m_maxqc == 1 && m_qi[0])
	   {
		   if ((nChar >= 'A' && nChar <= 'Z') || (nChar >= 'a' && nChar <= 'z'))
		   {
			   int j;
			   for (j = 0;m_qi[j];j++)
			   {
				   if (m_qi[j] == (int)nChar) break;
			   }
			   if (!m_qi[j]) return;
		   }
	   }

       BASECDsEdit::OnChar(nChar, nRepCnt, nFlags);	   

       if (!m_multilinea)
       {
           if (!m_overwrite && isp && m_tipo == 2 && m_maxqc > 6)
           {
               GetSel(startPos, endPos);
               if (startPos == endPos)
               {
				   if (startPos == 0)
				   {

				   }
				   else if (startPos == 2)
                       SetSel(3,5);          
                   else if (m_maxqc == 8)
                   {
                       SetSel(6,8);
                   }
                   else
                   {
                       if (startPos == 5)
                           SetSel(8,10);
                       else if (startPos == 9)
                       {
                           CString s;
                           GetWindowText(s);
						   if (::atoi(s.Mid(0,2)) == 0 && ::atoi(s.Mid(3,2)) == 0) {
                                   s.SetAt(6,'0');
                                   s.SetAt(7,'0');
                                   SetWindowText(s);
                                   SetSel(9,10);
						   }
                           else if (::atoi(s.Mid(8,1)) < 6)
                           {
                               if (::atoi(s.Mid(6,2)) == 19 || ::atoi(s.Mid(6,2)) == 0)
                               {
                                   s.SetAt(6,'2');
                                   s.SetAt(7,'0');
                                   SetWindowText(s);
                                   SetSel(9,10);
                               }
                           }
                           else
                           {
                               if (::atoi(s.Mid(6,2)) == 20 || ::atoi(s.Mid(6,2)) == 0)
                               {
                                   s.SetAt(6,'1');
                                   s.SetAt(7,'9');
                                   SetWindowText(s);
                                   SetSel(9,10);
                               }
                           }
                       }
                   }
               }
           }
           if ((m_flag & 4))
           {
               GetSel(startPos, endPos);
               if (startPos == endPos && startPos == m_maxqc)
               {
                   comando = 802; // simular return por autosal
                   TeclaStd(comando);
               }
           }
       }

	   if (isp)
	   {
			if (GetCombo()->m_autocompletar)
			{
				CString s,s1;
				GetWindowText(s);
				int index = GetCombo()->m_lst.SelectString(-1,s);
				if (index != LB_ERR)
				{
					GetCombo()->m_lst.GetText(index,s1);
					GetCombo()->SetWindowText(s1,TRUE);
				}
			}
			else if (theApp.m_conhint && isp && m_tipo == 1 && (theApp.m_ulttecla == -1 || theApp.m_ulttecla == TECLA_PING))
			{
				TeclaStd(TECLA_HINT);
			}
	   }
    }
}

#define SOLICITA_FINAL      -32002
#define SOLICITA_INICIO     -32003

void CDsEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   WPARAM comando = 0;
   BOOL procesa = TRUE;

   if (pmask)
	   return;
   if (!m_multilinea)
   {      

       comando = CDsStdDlg::DeterminaTecla(nChar);

       switch(comando)
       {
       case 819:
           if (  !(m_flag&2) )
           {
              comando = 0;
           }
           break;
       case 818:
           if (  !(m_flag&2) )
           {
              m_overwrite = !m_overwrite;
              procesa = FALSE;
              comando = 0;
           }
           break;
       case 816:
           if ( !(m_flag&1) )
              comando = 0;
           break;
       case 812:
           if ( !(m_flag&1) )
              comando = 0;
           break;
       case 810:
           if ((m_flag & 4) || (m_flag&1))
           {
               int startPos,endPos;
               GetSel(startPos, endPos);
               if (startPos == endPos && startPos == 0)
               {                   
                   comando = 805; /* campo antes */
               }
               else
                   comando = 0;
           }
           else
               comando = 0;
           break;      
       case 811:
           if ((m_flag & 4) || (m_flag&1))
           {
               int startPos,endPos;

               GetSel(startPos, endPos);
               if (endPos == GetWindowTextLength( ))
               {
                   comando = 804; // campo despues
               }
               else
			   {
                   comando = 0;
			   }
           }
           else
		   {
               comando = 0;
		   }
           break;       
       case 991:
           comando = SOLICITA_INICIO;
           break;
       case 990:
           comando = SOLICITA_FINAL;
           break;
       default:
            break;
       }
   }
   else
   {
      if (nChar == VK_ESCAPE)
      {
		   comando = 807;          
      }
   }

    if (comando)
    {
      TeclaStd(comando);
    }
    else  if (procesa)
	{		
		short state;

		state = ::GetKeyState( VK_CONTROL ) & 0x8000;
		if (state && (nChar == 'C' || nChar == 'c'))
		{
			Copy();
		}
		else if (state && (nChar == 'X' || nChar == 'x'))
		{
			Cut();
		}
		else if (state && (nChar == 'V' || nChar == 'v'))
		{
			Paste();
		}
		else
		{
			BASECDsEdit::OnKeyDown(nChar, nRepCnt, nFlags);	
		}
	}
}

HBRUSH CDsEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{

	if (theApp.m_colorset_ce0)
	{
		if ((m_estilo & ES_READONLY) || !IsWindowEnabled())
		{
			m_clrText = theApp.m_color_ce0[4];
			m_clrBack = theApp.m_color_ce0[5];
		}
		else if (::GetFocus() == m_hWnd)
		{
			m_clrText = theApp.m_color_ce0[2];
			m_clrBack = theApp.m_color_ce0[3];
		}
		else
		{
			m_clrText = theApp.m_color_ce0[0];
			m_clrBack = theApp.m_color_ce0[1];
		}
        pDC->SetBkColor( m_clrBack );
        pDC->SetTextColor( m_clrText );
		return (HBRUSH)m_pDoc->m_bEditBk->m_hObject;
	}


    if (m_estilo & ES_READONLY)
    {
        if (m_pBkBrush)
        {		
			m_clrBack = m_BkColor;
			m_clrText = m_FrColor ;
            pDC->SetBkColor( m_BkColor );
            pDC->SetTextColor( m_FrColor );
	        return (HBRUSH)m_pBkBrush->m_hObject;
        }
		m_clrBack = m_dsctrl->m_pdlg->m_cLabelBk ;
		m_clrText = m_dsctrl->m_pdlg->m_cLabelFr ;
        pDC->SetBkColor( m_dsctrl->m_pdlg->m_cLabelBk );
        pDC->SetTextColor( m_dsctrl->m_pdlg->m_cLabelFr );       
	    return (HBRUSH)m_dsctrl->m_pdlg->m_bLabelBk->m_hObject;
    }	

    HWND hwnd = ::GetFocus();

    if (hwnd == m_hWnd)
    {		
		if (theApp.m_colorset9)
		{
			m_clrBack = theApp.m_color9 ;
			m_clrText = m_pDoc->m_cEditINFr;
			pDC->SetBkColor( theApp.m_color9 );
			pDC->SetTextColor( m_pDoc->m_cEditINFr );       
			return (HBRUSH)theApp.m_colorbrush9;
		}
		else		
		{
			m_clrBack = m_pDoc->m_cEditINBk ;
			m_clrText = m_pDoc->m_cEditINFr ;
			pDC->SetBkColor( m_pDoc->m_cEditINBk );
			pDC->SetTextColor( m_pDoc->m_cEditINFr );       
			return (HBRUSH)m_pDoc->m_bEditINBk->m_hObject;
		}
    }
 

    if (m_pBkBrush)
    {
		m_clrBack = m_BkColor ;
		m_clrText = m_FrColor ;
        pDC->SetBkColor( m_BkColor );
        pDC->SetTextColor( m_FrColor );
	    return (HBRUSH)m_pBkBrush->m_hObject;
    }

	m_clrBack = m_pDoc->m_cEditBk;
	m_clrText = m_pDoc->m_cEditFr ;
    pDC->SetBkColor( m_pDoc->m_cEditBk );
    pDC->SetTextColor( m_pDoc->m_cEditFr );       
	return (HBRUSH)m_pDoc->m_bEditBk->m_hObject;
}


int CDsEdit::ProcesaPtec(int ptec)
{
  int startPos = 0,endPos = 0;
  int qj = -1;

  if ((ptec/100000) == 1) {
	  ptec %= 100000;
	  m_teclaseleccion = ptec;
	  if (!ptec)
		  return qj;
  }
  switch(ptec)
  {
  case 816: // end
      SetSel(GetWindowTextLength( )+1,GetWindowTextLength( )+1);
      break;
  case 812: // home
      SetSel(0,0);
      break;
  case 808: // left
      GetSel(startPos, endPos);
      if ((m_flag & 4))
      {           
           if (startPos == endPos && startPos == 0)
           {
               qj = 2;
               break;
           }
      }
      SetSel(endPos-1,endPos-1);      
      break;      
  case 809: // right
      GetSel(startPos, endPos);
      if ((m_flag & 4))
      {           
           if (startPos == endPos && endPos == m_maxqc)
           {
               qj = 3;
               break;
           }
      }	  
      SetSel(endPos+1,endPos+1);
      break;
  default:      
      if (ptec < 256)
      {
         unsigned char c = (unsigned char)ptec;
         oem_a_ansi(&c);
         if (isprint(c))
         {              
              GetSel(startPos, endPos);
              if (startPos != endPos)
              {
                  Clear();
              }
              CString s;
              GetWindowText(s);
              s.Insert(startPos,c);
              SetWindowText(s);
              SetSel(startPos+1,startPos+1);
              if ((m_flag & 4))
              {
                  GetSel(startPos, endPos);
                   if (startPos == endPos && startPos == m_maxqc)
                   {
                       qj = 0;
                   }
              }
             //keybd_event() 
         }
      }      
      break;
  }

return qj;
}

void CDsEdit::OnSetFocus(CWnd* pOldWnd) 
{	

	if (m_no999/* || (theApp.m_ctrlactivo == m_dsctrl && m_otrofoco && pOldWnd && pOldWnd->m_hWnd == m_otrofoco)*/)
	{
		BASECDsEdit::OnSetFocus(pOldWnd);
		return;
	}

    HWND oldw = NULL;

	m_fococonfirmado = FALSE;

    if (pOldWnd)
        oldw = pOldWnd->m_hWnd;
	BASECDsEdit::OnSetFocus(pOldWnd);

	if (!theApp.m_hwnd_999)
		theApp.m_hwnd_999 = (int)oldw;
    TeclaStd(999);
}

void CDsEdit::ConfirmaFoco()
{
	
	m_no999 = TRUE;
	SetFocus();
	m_no999 = FALSE;	
	
	m_fococonfirmado = TRUE;
	theApp.m_hintcounter = 0;

	/*
    if (theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 2 && theApp.m_ctrlactivo->m_ctrl)
    { // falta una helper general de clean-up
	   if (((CDsCombo *)theApp.m_ctrlactivo->m_ctrl)->m_calendario)
		  ((CDsCombo *)theApp.m_ctrlactivo->m_ctrl)->SetDroppedState(FALSE);
    }
	*/

	theApp.m_antctrlactivo = theApp.m_ctrlactivo;
	/*
	if (theApp.m_antctrlactivo)
	{
		theApp.m_antctrlactivo->m_tipo
	}
	*/
    theApp.m_ctrlactivo = m_dsctrl;	

    EnsureVisible();
    
    m_overwrite = FALSE;
	if (m_teclaseleccion){
		ProcesaPtec(m_teclaseleccion);
		return;
	}
    if (!m_multilinea && !m_seleccionado)
    {
         if (m_tipo == 2)
         {
            if (m_maxqc == 6)
            {
               m_overwrite = TRUE;
            }
            else
               SetSel( 0, 2);
         }
         else
            SetSel( 0, m_maxqc);
    }      

}

void CDsEdit::corrigeFecha() {
       static int diames[13]=
       {
       0,31,28,31,30,31,30,31,31,30,31,30,31
       };      
       BOOL err = FALSE;

       CString s;
       GetWindowText(s);
	   if (s.GetLength() < 6)
	   {
		   s = "01.01.0000";
		   SetWindowText(s);
	   }
	   else
	   {		   
		   s.Trim();
		   int mes;
		   int ano;
		   int dia;
		   if (s.GetLength() < 10) {
			   int idx;
			   idx = s.Find('.');
			   if (idx < 0) {
					s = "01.01.0000";
					err = TRUE;
			   }
			   else {
				   int idx2;
				   idx2 = s.Find('.',idx+1);
				   if (idx2 < 0) {
					   s = "01.01.0000";
					   err = TRUE;
				   }
				   else {					   
					   dia = ::atoi(s.Mid(0,idx));
					   mes = ::atoi(s.Mid(idx+1,idx2-idx-1));
					   ano = ::atoi(s.Mid(idx2+1));
					   if (ano < 100)
						   ano += 2000;
					   s.Format("%02d.%02d.%04d",dia,mes,ano);
				       err = TRUE;
				   }
			   }
		   }
		   mes = ::atoi(s.Mid(3,2));
		   ano = ::atoi(s.Mid(6,4));
		   dia = ::atoi(s.Mid(0,2));
		   if (mes == 0 && dia == 0) {
				err = TRUE;
				mes = 1;
				dia = 1;
				ano = 0;
		   }
		   else  {
			if (ano < 100) {
				if (mes != 1 || dia != 1 || ano != 0) {
						ano += 2000;
						err = TRUE;
				}
			}
			if (mes < 1)
			{
				err = TRUE;
				mes = 1;
			} 
			else if (mes > 12)
			{
				err = TRUE;
				mes = 12;
			}

			int dias = diames[mes];
			if (mes == 2)
			{           
				if ((ano%4 == 0) && (ano%100 != 0) || (ano%400 == 0))
					dias++;
			}       
			if (dia < 1)
			{
				dia = 1;
				err = TRUE;
	            
			} else if (dia > dias)
			{
				dia = dias;
				err = TRUE;
			}
		   }
               
		   if (err)
		   {
			   //s = _T("Fecha Incorrecta: ") + s;
			   //AfxMessageBox(s,MB_OK | MB_ICONHAND);
			   s.Format("%02d.%02d.%04d",dia,mes,ano);
			   SetWindowText(s);
			   //m_fococonfirmado = TRUE;
			   //Invalidate();			   
			   //((CWnd *)this)->SetFocus();
			   return;
		   }
	   }

}

void CDsEdit::KillFocus()
{
	if (!m_fococonfirmado)
	{
		Invalidate();
		return;
	}	

	m_fococonfirmado = FALSE;	  

	if (m_tipo == 3)
	{
       CString s;
       GetWindowText(s);
	   if (!s.IsEmpty())
	   {
		   if (s[0] == 'E')
		   {
			  s.Format("%lf",(double)atof(((LPCSTR)s)+1)*166.386);
			  SetWindowText(s);		  
		   }
		   else if (s[0] == 'P')
		   {
			  double euros = (double)atof(((LPCSTR)s)+1);
			  euros /= (double)166.386;
			  s.Format("%lf",euros);
			  SetWindowText(s);
		   }
	   }
	}
	else if (m_tipo == 2)
    {
	   corrigeFecha();
    }	
	if (GetCombo()->m_autocompletar && GetCombo()->GetDroppedState())
	{
		GetCombo()->UnDrop();
	}
    Invalidate();
}

void CDsEdit::OnKillFocus(CWnd* pNewWnd) 
{
	HWND nueva = NULL;
	if (pNewWnd)
	{
		nueva = pNewWnd->m_hWnd;
	}
    BASECDsEdit::OnKillFocus(pNewWnd);    

	/*
	if (nueva && nueva == m_otrofoco)
	{
		return;
	}
	*/
	KillFocus();
}

BOOL CDsEdit::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYUP)
    {
		int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
        if (comando == 805 || comando == 804 || comando == 806 || comando == 807 || comando == 802)
        {            
            return TRUE;        
        }
	}

    if (pMsg->message == WM_KEYDOWN)
    {
        int comando = CDsStdDlg::DeterminaTecla(pMsg->wParam);
        if (comando == 805 || comando == 804 || comando == 806 || comando == 807 || comando == 802)
        {
            TeclaStd(comando);
            return TRUE;        
        }
        return BASECDsEdit::PreTranslateMessage(pMsg);
    }
    else
	    return BASECDsEdit::PreTranslateMessage(pMsg);
}

BOOL CDsEdit::TeclaStd(int tecla)
{
    if (m_dsctrl)
    {
        return m_dsctrl->TeclaStd(tecla);
    }
    return FALSE;
}

BOOL CDsEdit::EnsureVisible()
{    
    return m_dsctrl->EnsureVisible();        
}

BOOL CDsEdit::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{	
    m_estilo = dwStyle & ~(WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE);	
    BOOL r = BASECDsEdit::Create( dwStyle, rect, pParentWnd, nID );

    if (r && m_entabla)
    {
       m_entabla->m_editando = TRUE;       
       m_entabla->m_dsedit = GetCombo();
    }	
    return r;
}

BOOL CDsEdit::AjustaTamano()
{
   ASSERT(FALSE);
   return FALSE;
}

BOOL CDsEdit::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if ((wParam&0xffff) == ID_EDIT_COPY)
	{
		Copy();
		return TRUE;
	}
	if ((wParam&0xffff) == ID_EDIT_CUT)
	{
		Cut();
		return TRUE;
	}
	if ((wParam&0xffff) == ID_EDIT_PASTE)
	{
		Paste();
		return TRUE;
	}

	if ((wParam&0xffff) == ID_EDIT_UNDO)
	{
		Undo();
		return TRUE;
	}

    if ((wParam&0xffff) >= 800 && (wParam&0xffff) <= 999)
    {
        TeclaStd((wParam&0xffff));
        return TRUE;
    }
    else if ((wParam&0xffff) == ID_PTECS)
    {
        int *p1 = (int *)lParam;
        ProcesaPtec(*p1);
        return TRUE;
    }
    else
    {
	    return BASECDsEdit::OnCommand(wParam, lParam);
    }
}

void CDsEdit::OnDestroy() 
{
    if (theApp.m_antctrlactivo == m_dsctrl)
    {
       theApp.m_antctrlactivo = NULL;
    }
	BASECDsEdit::OnDestroy();
}


void CDsEdit::GetWindowText( CString& rString)
{	
    BASECDsEdit::GetWindowText(rString);
	if (m_tipo == 2)
	{
		rString.TrimRight();
		if (rString.IsEmpty() || !rString.Compare("  .  ."))
		{
			rString = "01.01.0000";
		}
	}
}

int CDsEdit::GetWindowText( LPTSTR lpszStringBuf, int nMaxCount )
{
    int r = BASECDsEdit::GetWindowText( lpszStringBuf, nMaxCount );
	if (m_tipo == 2)
	{
		CString s = lpszStringBuf;
		s.TrimRight();
		if (s.IsEmpty() || !s.Compare("  .  ."))
		{
			strcpy(lpszStringBuf,"01.01.0000");
		}
		else {
			corrigeFecha();
			r = BASECDsEdit::GetWindowText( lpszStringBuf, nMaxCount );
		}
	}
	return r;
}

int CDsEdit::GetWindowTextLength( )
{
    return BASECDsEdit::GetWindowTextLength( );
}

void CDsEdit::SetWindowText( LPCTSTR lpszString )
{
	if (m_tipo == 2 && !strcmp(lpszString,"01.01.0000"))
	{
		BASECDsEdit::SetWindowText( "          " );
	}
	else
		BASECDsEdit::SetWindowText( lpszString );
}


void CDsEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   WPARAM comando = 0;  

   if (!m_multilinea)
   {
       comando = CDsStdDlg::DeterminaTecla(nChar);
	   if (comando == 805 || comando == 990 || comando == 989 || comando == 804) return;
   }
	
	BASECDsEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


CWnd* CDsEdit::SetFocus( )
{
	CWnd *pwndr = CWnd::SetFocus( );
	if (!m_no999 && !m_noConfirma)
	{
		ConfirmaFoco();
	}
	return pwndr;
}

void CDsEdit::OnLButtonDown(UINT nFlags,CPoint point)
{		
	BASECDsEdit::OnLButtonDown(nFlags, point);
	OnSetFocus(GetFocus());
}

void CDsEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	/*
    if (m_entabla)
    {
        m_entabla->DobleClick();
    }
	*/
	if (m_fococonfirmado)
	{
		TeclaStd(TECLA_DOBLECLICK);	
	}
	BASECDsEdit::OnLButtonDblClk(nFlags, point);
}
