
// DsStdDlg.cpp : implementation file
//

#include "stdafx.h"
#include <comdef.h>
#include "Shlwapi.h"
#include "diagramx.h"
#include "DsxMainFrm.h"
#include "DsStdDlg.h"
#include "DsxView.h"
#include "DsxDoc.h"
#include "DsTabCtrl.h"
#include "DsEdit.h"
#include "DsCombo.h"
#include "DsButton.h"
#include "TablaView.h"
#include "DsCtrlScroll.h"
#include "DsListBox.h"
#include "DsTextEdit.h"
#include "StaticLabel.h"
#include "DsImagen.h"
#include "Ds9Window.h"
#include "DsTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDsStdCtrl *CDsStdCtrl::m_ctrlfirst = NULL;
CDsStdCtrl *CDsStdCtrl::m_ctrllast = NULL;
int pmask = 0;

CDsOCX::CDsOCX(LPCSTR NombreOCX)
{
    USES_CONVERSION;

    m_punk = NULL; 
    m_pdisp = NULL;
    m_TypeInfo = NULL;

    HRESULT hr = ::CLSIDFromProgID(T2COLE(NombreOCX),&m_clsid);
           
    if (hr == S_OK )
    {
        hr = ::CoCreateInstance(m_clsid,NULL,CLSCTX_ALL ,IID_IUnknown, (void FAR* FAR*)&m_punk);
        if (hr == S_OK )
        {
            hr = m_punk->QueryInterface(IID_IDispatch, (void FAR* FAR*)&m_pdisp);
            if (hr == S_OK )
            {    
                /*
                hr = m_pdisp->GetTypeInfo( 0 , LOCALE_SYSTEM_DEFAULT , &m_TypeInfo );
                if (hr == S_OK)
                {                   
                    TYPEATTR *pattr;
                    hr = m_TypeInfo->GetTypeAttr(&pattr);
                    if (hr == S_OK)
                    {                        
		                for (int n = 0 ; n < pattr->cFuncs ; n++)
                        {
			                //DumpFunc( m_TypeInfo, pattr, n) ;
                        }

		                for (n = 0 ; n < pattr->cVars ; n++)
                        {
			                //DumpConst( m_TypeInfo, pattr, n  ) ;
                        }
                        m_TypeInfo->ReleaseTypeAttr( pattr ) ;
                    }
                }
                */
                AttachDispatch( m_pdisp);
                return;
            }
        }
    }
    if (m_punk) m_punk->Release();     
    if (m_pdisp) m_pdisp->Release();     
    m_punk = NULL; 
    m_pdisp = NULL;
}
    
CDsOCX::~CDsOCX()
{    
    if (m_pdisp) m_pdisp->Release();     
    //if (m_punk) m_punk->Release();     
    m_punk = NULL; 
    m_pdisp = NULL;
}




BOOL CDsOCX::Invoca(LPCSTR funcion,HWND hdc,long x,long y,long dx,long dy,void *parametros)
{
    if (!m_punk || !m_pdisp) return FALSE;

    USES_CONVERSION;

    DISPID dispid;
    OLECHAR FAR* szMember = (OLECHAR FAR*)T2COLE(funcion);


    HRESULT hr = m_pdisp->GetIDsOfNames(IID_NULL,&szMember,1,LOCALE_SYSTEM_DEFAULT,&dispid);

    if (hr == S_OK)
    {
        int *p1 = (int *)parametros;
        int npars = p1[0];
        char *px = (char *)(p1+npars+1);        
        int lenn;
        void *pars[100];
        VARTYPE vtRet = VT_EMPTY;
        void *pvRet = NULL;
		

      	DISPPARAMS dispparams;
	    memset(&dispparams, 0, sizeof dispparams);
        dispparams.cArgs = npars-1;
		if (hdc)
		{
			dispparams.cArgs += 5;
		}

		VARIANT* pArg = new VARIANT[dispparams.cArgs];		
		dispparams.rgvarg = pArg;
		memset(pArg, 0, sizeof(VARIANT) * dispparams.cArgs);

        pArg += dispparams.cArgs - 1;
		if (hdc)
		{
			pArg->vt = VT_I4;
			pArg->lVal = (long)hdc;
			--pArg;
			pArg->vt = VT_I4;
			pArg->lVal = (long)x;
			--pArg;
			pArg->vt = VT_I4;
			pArg->lVal = (long)y;
			--pArg;
			pArg->vt = VT_I4;
			pArg->lVal = (long)dx;
			--pArg;
			pArg->vt = VT_I4;
			pArg->lVal = (long)dy;
			--pArg;
		}

		int i;
        for (i = 1;i <= npars;i++)
        {
            switch(p1[i])
            {
            case 1:
            case 3:
                if (p1[i] == 3)
                {
                    lenn = *((int *)px);
                    px += sizeof(int);
                }
                else
                {
                    lenn = 251;
                }       
                if (i == 1)
                {
                    vtRet = VT_BSTR | VT_BYREF;
                    pars[i-1] = (void *)SysAllocString(T2COLE(px));
				    pvRet = (void *)&(pars[i-1]);                    
                }
                else
                {
                    pArg->vt = VT_BSTR | VT_BYREF;
                    pars[i-1] = (void *)SysAllocString(T2COLE(px));
					/*pArg->bstrVal = (BSTR)(pars[i-1]);*/
				    pArg->pbstrVal = (BSTR *)&(pars[i-1]);
                }
                px += lenn;
                break;
            case 2:
                if (i == 1)
                {
                    vtRet = VT_I4;
                    pvRet = (void *)(int)*((double *)px);
                }
                else
                {
                    pArg->vt = VT_I4|VT_BYREF;
                    pars[i-1] = (void *)(long)*((double *)px);
					/*pArg->lVal = (long)pars[i-1];*/
                    pArg->plVal = (long *)&pars[i-1];
                }
                break;
            default:
                break;
            }

            if (i > 1)
            {
              --pArg; // get ready to fill next argument
            }
        }       

        try
        {
	         VARIANT* pvarResult = NULL;
	         VARIANT vaResult;
	         AfxVariantInit(&vaResult);
	         if (vtRet != VT_EMPTY)
		        pvarResult = &vaResult;

             EXCEPINFO excepInfo;
	         memset(&excepInfo, 0, sizeof excepInfo);

             UINT nArgErr = (UINT)-1;  // initialize to invalid arg

             SCODE sc = m_lpDispatch->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD,
		          &dispparams, pvarResult, &excepInfo, &nArgErr);
             
             
	         if (FAILED(sc))
             {
		        VariantClear(&vaResult);
		        if (sc != DISP_E_EXCEPTION)
		        {
			        // non-exception error code
			        AfxThrowOleException(sc);
				}

		        // make sure excepInfo is filled in
		        if (excepInfo.pfnDeferredFillIn != NULL)
			        excepInfo.pfnDeferredFillIn(&excepInfo);

		        // allocate new exception, and fill it
		        COleDispatchException* pException =
			        new COleDispatchException(NULL, 0, excepInfo.wCode);
		        ASSERT(pException->m_wCode == excepInfo.wCode);
		        if (excepInfo.bstrSource != NULL)
		        {
			        pException->m_strSource = excepInfo.bstrSource;
			        SysFreeString(excepInfo.bstrSource);
		        }
		        if (excepInfo.bstrDescription != NULL)
		        {
			        pException->m_strDescription = excepInfo.bstrDescription;
			        SysFreeString(excepInfo.bstrDescription);
		        }
		        if (excepInfo.bstrHelpFile != NULL)
		        {
			        pException->m_strHelpFile = excepInfo.bstrHelpFile;
			        SysFreeString(excepInfo.bstrHelpFile);
		        }
		        pException->m_dwHelpContext = excepInfo.dwHelpContext;
		        pException->m_scError = excepInfo.scode;

		        // then throw the exception
		        THROW(pException);
		        ASSERT(FALSE);  // not reached
             }
        }
        catch(COleException *e)
        {
          char buf[512];
          sprintf(buf, "COleException. SCODE: %08lx en metodo %s.", (long)e->m_sc,funcion);
          ::MessageBox(NULL, buf, "OleException", MB_SETFOREGROUND | MB_OK);
        }
        catch(COleDispatchException *e)
        {
           char tmp[512];
           sprintf(tmp,"%s : metodo %s.",e->m_strDescription,funcion);
           AfxMessageBox(tmp);         
        }
        catch(...)
        {
           char tmp[256];
           sprintf(tmp,"General Exception al invocar metodo %s.",funcion);
           ::MessageBox( NULL,tmp, "Catch-all",MB_SETFOREGROUND | MB_OK);
        }

        px = (char *)(p1+npars+1);                       
        npars = p1[0];
        for (i = 1;i <= npars;i++)
        {
            switch(p1[i])
            {
            case 1:
            case 3:
                SysFreeString((BSTR)pars[i-1]);  
                break;
            default:
                break;
            }
        }

		delete[] dispparams.rgvarg;

        return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CDsStdCtrl

CDsStdCtrl::CDsStdCtrl()
{   
   m_esde9 = 0;
   m_helpflag = 0;
   m_previovisible = FALSE;
   m_nivel = 0;
   m_nivel9 = 0;
   m_ctrl = NULL;
   m_tipo = -1;
   m_ctrlid = -1;
   m_lpi = -1;
   m_lpf = -1;
   m_pdlg = NULL;
   m_cid.m_lcid = -1; // identificacion del control en relacion al id-sid 
   m_cid.m_lref = -1; 
   m_cid.m_tipo = -1; 
   m_p_id = -1;
   m_evento = -1;
   m_cthwnd = NULL;
   m_antcid_ctrlid = -1;   
   m_alto = -1;
   m_ancho = -1;
   m_stdx = -1;
   m_stdy = -1;
   m_offx = -1;
   m_offy = -1;
   m_esw = TRUE;
   m_muriendo = FALSE;
   m_protegido = FALSE;


   m_ctrlbefore = NULL;
   m_ctrlafter = NULL;

   m_usado = 0;   

   m_nivelfoco = 0;
}

CDsStdCtrl::~CDsStdCtrl()
{
  ASSERT(m_usado == 0);
  ASSERT(!m_protegido);
  m_muriendo = TRUE;
  if (m_antcid_ctrlid != -1)
  {
      CDsStdCtrl *pctrl = NULL;
      if (CDsStdDlg::m_todosctrl.Lookup(m_antcid_ctrlid,pctrl) && pctrl->m_ctrlid == m_antcid_ctrlid && pctrl->m_pdlg == m_pdlg)
      {
          if (CDsStdDlg::ComparaCid(m_cid,pctrl->m_cid))
          {
              pctrl->m_pdlg->m_cidctrllst[pctrl->m_pdlg->CidKey(pctrl->m_cid)] = pctrl;
          }
      }      
  }
  if (m_ctrlid != -1)
  {
      CDsStdCtrl *pctrl = NULL;
      if (CDsStdDlg::m_todosctrl.Lookup(m_ctrlid,pctrl))
      {
		 ASSERT(pctrl == this);
         CDsStdDlg::m_todosctrl.RemoveKey(m_ctrlid);
      }
  }

  if (m_evento != -1)
  {
     long tam;
     long respuesta;
     long modo = 0;
     int operacion = 999;
     char *p;
     char tmp[512];   

     p = tmp;

     memcpy((void *)p,(void *)&m_evento,sizeof(int));
     p += sizeof(int);

     tam = (sizeof(long) * 7) + (sizeof(int)*2) + 1;
     respuesta = tam;

     memcpy((void *)p,(void *)&tam,sizeof(long));
     p += sizeof(long);	
     memcpy((void *)p,(void *)&respuesta,sizeof(long));
     p += sizeof(long);
     memcpy((void *)p,(void *)&modo,sizeof(long));
     p += sizeof(long);
     memcpy((void *)p,(void *)&operacion,sizeof(int));
     p += sizeof(int);    

	 if (m_pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window))) // modo 9
	 {
		pDsSys->DespachadorInterno9(13,(void *)tmp);
	 }
	 else
	 {
		pDsSys->DespachadorInterno(13,(void *)tmp);
	 }
  }


  if (theApp.m_antctrlactivo == this)
  {
     theApp.m_antctrlactivo = NULL;
  }
  if (theApp.m_ctrlactivo == this)
  {
     theApp.m_ctrlactivo = theApp.m_antctrlactivo;
     theApp.m_antctrlactivo = NULL;
  }


  if (m_ctrl)
  {
      if (m_esw)
      {
         m_ctrl->DestroyWindow();
         delete m_ctrl;
      }      
      else
      {
          /****!!!!!****/
          delete ((CDsOCX *)m_ctrl);
      }
  }
  if (m_ctrlbefore)
  {
	  ASSERT(m_ctrlbefore->m_ctrlafter == this);
	  m_ctrlbefore->m_ctrlafter = m_ctrlafter;
  }
  if (m_ctrlafter)
  {
	  ASSERT(m_ctrlafter->m_ctrlbefore == this);
	  m_ctrlafter->m_ctrlbefore = m_ctrlbefore;
  }
  if (m_ctrlfirst == this)
  {
	  ASSERT(m_ctrlbefore == NULL);
	  m_ctrlfirst = m_ctrlafter;
  }
  if (m_ctrllast == this)
  {
	  ASSERT(m_ctrlafter == NULL);
	  m_ctrllast = m_ctrlbefore;
  }

  if (m_cthwnd)
  {
      CDsStdCtrl *pctrl = NULL;
      if (CDsStdDlg::m_hwndctrllst.Lookup(m_cthwnd,pctrl) && pctrl == this)
      {
          CDsStdDlg::m_hwndctrllst.RemoveKey(m_cthwnd);
      }      
  }
}

int CDsStdDlg::ComparaLPos(int pi, int pf,BOOL dentro)
{

   if (dentro)
   {
        if ( (pi / 100) < (pf/100) ) return -1;
        if ( (pi % 100) < (pf%100) ) return -1;
        if (pi != pf) return 1;
   }
   else
   {
        if ( (pi / 100) > (pf/100) ) return 1;
        if ( (pi % 100) > (pf%100) ) return 1;
        if (pi != pf) return -1;
   }

return 0;
}


CRect CDsStdCtrl::MarcoControl(BOOL estab)
{
    CRect rect;
    CDsStdDlg *pDlg = m_pdlg;
    int lpos = m_lpi;
    int lpf =  m_lpf;


	if (m_esde9)
	{
		rect.left = m_lpi;
		//if (!tama)
		  rect.right = 1;
		/*
		else
		  rect.right = 0;
		*/
		rect.top  = m_lpf;
		rect.bottom = 0;
		pDsSys->DespachadorInterno9(7,(void *)&rect);	
		
		//rect.top -= 1;
		
		if (m_esde9 == 1)
		{
			rect.left -= 2;
			rect.right += 3;
			rect.bottom += 3;
		}
		return rect;
	}


    lpos -= pDlg->m_lpi;
    lpf -= pDlg->m_lpi;
    lpf += 101;
    
    if (estab)
    {
        rect.top = pDlg->m_ult_hijo_mosaico.top + pDlg->m_offy + (pDlg->m_stdy * (lpos/100))/10;       
        rect.left = pDlg->m_ult_hijo_mosaico.left + pDlg->m_offx + (pDlg->m_stdx * (lpos%100))/10;

        rect.right = pDlg->m_ult_hijo_mosaico.left + pDlg->m_offx + (pDlg->m_stdx * (lpf%100))/10;
        rect.bottom = pDlg->m_ult_hijo_mosaico.top + pDlg->m_offy + (pDlg->m_stdy * (lpf/100))/10;
    }
    else
    {
		int ajuste = 1;
		DWORD stylo = pDlg->GetStyle();

		if ((stylo & WS_BORDER) || (stylo & WS_DLGFRAME) || (stylo & WS_THICKFRAME))
		{
			ajuste = 0;
		}

        rect.top = /*pDlg->m_ult_hijo.top +*/ pDlg->m_offy + (pDlg->m_stdy * (lpos/100))/10;       
        rect.left = /* pDlg->m_ult_hijo.left +*/ pDlg->m_offx + (pDlg->m_stdx * (lpos%100))/10;

        rect.right = /*pDlg->m_ult_hijo.left +*/ pDlg->m_offx + (pDlg->m_stdx * (lpf%100))/10;
        rect.bottom = /*pDlg->m_ult_hijo.top +*/ pDlg->m_offy + (pDlg->m_stdy * (lpf/100))/10;

		if ((m_tipo == 3 || m_tipo == 13 ) && (rect.top < pDlg->m_stdrect.bottom && rect.bottom >= (pDlg->m_stdrect.bottom+ajuste) && rect.bottom < (pDlg->m_stdrect.bottom+3+ajuste)))
		{
			rect.bottom = pDlg->m_stdrect.bottom-3;
		}
    }

    switch(m_tipo)
    {
    case 3:
        rect.right -= ::GetSystemMetrics(SM_CXEDGE);
        break;
    }

	/*
	CRect crect;
	pDlg->GetClientRect(crect);
    if (rect.bottom > crect.bottom)
	{
		rect.bottom = crect.bottom;
	}
	*/

    return rect;
}

CWnd *CDsStdCtrl::SetFocus()
{
   CWnd *retval = NULL;
   if (this != theApp.m_ctrlactivo && theApp.m_ctrlactivo && theApp.m_ctrlactivo->m_tipo == 2 && theApp.m_ctrlactivo->m_ctrl)
   { 
	   if (((CDsCombo *)theApp.m_ctrlactivo->m_ctrl)->m_calendario)
	   {
		  ((CDsCombo *)theApp.m_ctrlactivo->m_ctrl)->SetDroppedState(FALSE);
		  ((CDsCombo *)theApp.m_ctrlactivo->m_ctrl)->m_Edit.KillFocus();
	   }
   }

   ((CDsxFrame*)theApp.m_pMainWnd)->SetActiveView((CView *)m_pdlg->m_pView);   

   if (m_tipo == 2 || m_tipo == 11)
   {
	   retval = ((CDsCombo *)m_ctrl)->SetFocus();
   }
   else if (m_tipo == 7)
   {
	   retval = ((CDsTextEdit *)m_ctrl)->SetFocus();
   }
   else if (m_tipo == 4)
   {
	   retval = ((CTablaW *)m_ctrl)->SetFocus();
   }
   else
   {	   
	   retval = m_ctrl->SetFocus();
	   if (retval)
	   {
		  theApp.m_antctrlactivo = theApp.m_ctrlactivo;
		  theApp.m_ctrlactivo = this;	
	   }
   }

   return retval;
}


static COLORREF rgb_string(char *p) {
	COLORREF color;
	int r,g,b;

	r = (*p-48)*100 + (*(p+1)-48)*10 + (*(p+1)-48);
	p += 3;
	g = (*p-48)*100 + (*(p+1)-48)*10 + (*(p+1)-48);
	p += 3;
	b = (*p-48)*100 + (*(p+1)-48)*10 + (*(p+1)-48);

	color = RGB(r,g,b);

	return color;
}

// real_create: crea el control en cascada 
// para que la Create pueda manipular de modo generico el control una vez creado
CWnd * CDsStdCtrl::real_create(int tipo, int lpos, int lpf, void *dato, CDsStdDlg *pDlg,int Atrib,int Cc, int Cf, int pre_id)
{

	int *px1 = (int *)dato;
	
	if ( (pre_id & 0x80000000) || !(pre_id & 0x40000000) )
	{
		m_ctrlid = theApp.edid(CDsStdDlg::NewHandle());		
	}
    else
	{
		m_ctrlid = theApp.edid(pre_id);
	}

    m_p_id = pDlg->m_id+pDlg->m_replicante;

    CDsStdDlg::m_todosctrl[m_ctrlid] = this;

    if (tipo == 5)
    {
        // barras no tan 'libres' de scroll
        int n_lpf = lpf;
        POSITION pos = pDlg->m_ctrlst.GetStartPosition( );   
        CDsStdCtrl *elotro;
        int elwid;
        while (pos)
        {
           pDlg->m_ctrlst.GetNextAssoc( pos, elwid, elotro );
           if ( (elotro->m_lpi%100) > (n_lpf%100))
           {
               n_lpf = n_lpf/100*100 + (elotro->m_lpi%100);
           }
        }
        lpos = n_lpf - (lpf-lpos);
        lpf = n_lpf;
    }

    m_lpi = lpos;
    m_lpf = lpf;

    m_ancho = (m_lpf % 100) - (m_lpi % 100) + 1;
    m_alto  = (m_lpf / 100) - (m_lpi / 100) + 1;

    m_stdx  = pDlg->m_stdx;
    m_stdy  = pDlg->m_stdy;
    m_offx  = 0;
    m_offy  = 0;


    m_tipo = tipo;

    lpos -= pDlg->m_lpi;
    lpf -= pDlg->m_lpi;
    lpf += 101;

    m_pdlg = pDlg;

    m_cid.m_lcid = -1; // a definir segun control
    m_cid.m_lref = pDlg->m_sid;
    m_cid.m_tipo = m_tipo;

    CRect rect;

    if (tipo == 999)
    {
       if ( (pDlg->m_ult_sid%1000) == *((int *)dato) )
       {
           rect = MarcoControl(TRUE);
       }
       else
       {        
           rect = MarcoControl(FALSE);
       }
    }
    else
    {
		if (tipo != 2)
		{
		   if (m_esde9)
		  	 m_esde9 = 2;
		}
        rect = MarcoControl(FALSE);		
    }
    
    
    if (tipo == 2 || tipo == 11) // edicion
    {  

	   rect.right += (::GetSystemMetrics(SM_CXDLGFRAME)*2);

       m_cid.m_lcid = px1[1];
       m_cid.m_lref = px1[9];

       if (tipo == 2)
       {
		   CDsStdCtrl *pctrl;
           int otro = pDlg->BuscaControlPorCid(2,m_cid,&pctrl);
           if (otro > -1)
           {
			   if (!pDlg->DestruyeControl(otro))
			   {
				   m_antcid_ctrlid = otro;
			   }
           }
           /*
           else
           {
               if ( !(px1[1] & 0xffff0000) )
               {
                   int posicional = px1[1] | (m_lpi << 16);
                   otro = pDlg->BuscaControlPorCid(2,posicional);
                   if (otro > -1)
                   {
                       pDlg->DestruyeControl(otro);
                   }
               }
               else
               {
                   int posicional = px1[1] & 0xffff;
                   otro = pDlg->BuscaControlPorCid(2,posicional);
                   if (otro > -1)
                   {
                       pDlg->DestruyeControl(otro);
                   }
               }
           }
           */
       }       

       CDsCombo *w1 = new CDsCombo(FALSE);

	   if (pDlg == theApp.m_9wnd)
	   {
		  w1->m_Esde9 = TRUE;
	   }

       w1->SetView(pDlg->m_pView);
       w1->SetDoc(pDlg->m_pDoc);
       w1->SetDsCtrl(this);

	   w1->SetAtributos(Atrib,Cc,Cf);

       switch((Atrib&~128))
       {
           case 'R':
             w1->SetBkBrush(new CBrush());
             w1->GetBkBrush()->CreateSolidBrush(pDlg->m_pDoc->m_cLabelRBk);
             w1->SetBkColor(pDlg->m_pDoc->m_cLabelRBk);
             w1->SetFrColor(pDlg->m_pDoc->m_cLabelRFr);
             break;
           case 'I':
             w1->SetBkBrush(new CBrush());
             w1->GetBkBrush()->CreateSolidBrush(pDlg->m_pDoc->m_cLabelIBk);
             w1->SetBkColor(pDlg->m_pDoc->m_cLabelIBk);
             w1->SetFrColor(pDlg->m_pDoc->m_cLabelIFr);
             break;             
       }

	   
	   
	   if (rect.Size().cy > 20)
		  rect.bottom -= ((pDlg->m_stdy/6)/10);
	   else if (rect.Size().cy > 18)
		  rect.bottom -= ((pDlg->m_stdy/8)/10);
	   else if (rect.Size().cy > 15)
		  rect.bottom -= ((pDlg->m_stdy/10)/10);
	   
       
       DWORD otro_estilo = 0;

       
       char *pp = (char *)(px1+11);
       // control edicion
       // px1[0] == cwid (en este caso a poner *)
       // px1[1] == cid
       // px1[2] == maxqc
       // px1[3] == qc (minimo visualizable)
       // px1[4] == ctipo
       // px1[5] == modificable 
       // px1[6] == decimales
       // px1[7] == len qi
       // px1[8] == len qs
       // px1[9] == len qd-qo?              

       w1->Setmaxqc(px1[2]);
       w1->Setqc(px1[3]);
       w1->Settipo(px1[4]);
       if (w1->GetTipo() == 10)
       {
            w1->Settipo(1);
            otro_estilo |= ES_PASSWORD;
       }

       w1->Setqh(1);
       w1->Setdecimales(px1[6]);

	   w1->Setqi(pp);       

       pp += px1[7];

	   w1->Setqs(pp);
       
       pp += px1[8];

       if ( px1[5] )
       {
           otro_estilo |= ES_READONLY;
       }
	   else
	   {
		   otro_estilo |= WS_TABSTOP;
	   }

       
       if (px1[4] == 3)
       {
           otro_estilo |= ES_RIGHT;
       }
       else
       {
           otro_estilo |= ES_LEFT;
       }

	   if (w1->GetTipo() == 1)
	   {
		   if (strstr(w1->Getqs(),"<"))
		   {
			   otro_estilo |= ES_LOWERCASE;
		   } else if (strstr(w1->Getqs(),">"))
		   {
			   otro_estilo |= ES_UPPERCASE;
		   }
		   char *ptmp1;
		   if ((ptmp1 = strstr(w1->Getqs(),"#")))
		   {
			   int maxqc;
			   char usi[15];
			   int i,j;

			   maxqc = w1->Getqc();
			   i = 1;
			   j = 0;
			   while(j < 14 && ptmp1[i] && ptmp1[i] >= '0' && ptmp1[i] <= '9')
			   {
				   usi[j] = ptmp1[i];
				   j++;
				   i++;
			   }
			   if (j)
			   {
				  usi[j] = 0;
				  j = atoi(usi);
				  if (j > 0)
				  {
					  maxqc = j;
				  }
			   }			   
			   w1->Setmaxqc(maxqc);
		   }
		   if (strstr(w1->Getqs(),"+"))
		   {
			   otro_estilo &= ~ES_LEFT;
			   otro_estilo |= ES_RIGHT;
		   }
	   }


       if (px1[4] == 3 && !px1[6])
       {
           //otro_estilo |= ES_NUMBER;
       }
       
       /*
       if (  == 1 )
       {
           //otro_estilo |= WS_BORDER;           
           rect.right+= 4;
       }
       */	   
       CWnd *el_padre = pDlg;
       if (tipo == 11)
       {
          CDsStdCtrl *padre_pw = (CDsStdCtrl *)px1[0];
          if (padre_pw)
          {
             if (padre_pw->m_tipo == 4)
             {
                 CTablaView *ptablaw = &(((CTablaW *)padre_pw->m_ctrl)->m_tabla);
                 el_padre = ptablaw;             				 
                 ptablaw->PosicionElemento( &rect, ptablaw->m_cursel ,px1[1]/*+COLUMNA_INICIAL*/);
                 w1->SetTabla(ptablaw);
             }
             else
                 ASSERT(FALSE);
          }
       }	   

       if (!w1->Create( otro_estilo | WS_CHILD | ES_AUTOHSCROLL /*| ES_NOHIDESEL*/, rect, el_padre ,IDW_STDDLG+m_ctrlid))
       {
           delete w1;
       }
       else
       {
		   char *ipp;

           m_ctrl = (CWnd *)w1;
           w1->SetMargins( 0, 0 );

           for (ipp = pp;*ipp;ipp++)
                oem_a_ansi((unsigned char *)ipp);

           w1->SetWindowText(pp);

           w1->SetLimitText(w1->Getmaxqc());

           pp += (px1[10] + sizeof(int));

           for (ipp = pp;*ipp;ipp++)
                oem_a_ansi((unsigned char *)ipp);
		   
		   RellenaToolTip(pp);           

		   //w1->SendMessage(TTM_SETMAXTIPWIDTH,0,100);

		   if (pDlg->IsKindOf(RUNTIME_CLASS(CDs9Window)))
		   {			   
			   w1->SetFont(pDlg->m_pDoc->m_cf9Campo);
		   }
		   else
		   {
			   w1->SetFont(pDlg->m_pDoc->m_cfEdit);
		   }

           if ( tipo != 11 && !(otro_estilo & WS_BORDER) )
           {
               w1->ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);
			   /*w1->ModifyStyle(0,WS_BORDER,SWP_FRAMECHANGED);*/
           }

		   if (!m_esde9)
		   {
				w1->AjustaTamano();
		   }

		   w1->AjustaCombo();

           w1->ShowWindow(SW_SHOW);		   

           ::SetWindowPos(w1->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

           px1[0] = m_ctrlid;           

           return (CWnd *)w1;
       }
    }
    else
    if (tipo == 1 || tipo == 9) // texto estatico
    {
       if (tipo == 9)
       {
          m_cid.m_lcid = lpos*10000+lpf;
       }
       else
       {
          m_cid.m_lcid = lpos;
       }

       if (0)
	   {
           int otro = pDlg->BuscaControlPorCid(1,m_cid);
           if (otro > -1)
           {
               if (!pDlg->DestruyeControl(otro))
               {
                   m_antcid_ctrlid = otro;
               }
           }
       }       

       CStaticLabel *w1 = new CStaticLabel();

       w1->m_pDoc = pDlg->m_pDoc;
       w1->m_dsctrl = this;       

       WORD otro_estilo = SS_LEFT;

       char tmp[512];
       char *p1 = (char *)dato;

       if (tipo != 9)
       {
           if (*p1 && *(p1+strlen(p1)-1) == ':')
           {
               otro_estilo = SS_RIGHT;
           
               strcpy(tmp,p1);
               p1 = tmp + strlen(tmp)-1;
               *p1 = '.';
               while(p1 > tmp)
               {
                   if (*p1 == '.' || *p1 == ' ')
                       *p1 = 0;
                   else
                       break;
                   p1--;
               }
               strcat(tmp,":");
               dato = (void *)tmp;
           }
       }
       else
       {
           w1->m_group = TRUE;
       }

       BOOL es_raya = TRUE;

       if (tipo != 9)
       {
		   char *ipp;
           for (ipp = (char *)dato;*ipp;ipp++)
           {
               if (*ipp < 0 || *ipp > 12)
               {
                   es_raya = FALSE;
                   oem_a_ansi((unsigned char *)ipp);
               }
           }
       }
       //CBrush    *m_pBkBrush;
       //COLORREF  m_BkColor;
       //COLORREF  m_FrColor;
       //int Atrib,int Cc, int Cf   
	   w1->Atributo(Atrib,Cc,Cf);

       if (es_raya)
       {
            w1->m_maparaya = (char *)::malloc( m_ancho * m_alto + 1);
			w1->m_maparaya_size = m_ancho * m_alto + 1;
            ::memset(w1->m_maparaya,0,m_ancho * m_alto + 1);

            if (w1->m_group)
            {
                char *pp = w1->m_maparaya;
				int i;
                for (i = 1;i < (m_ancho-1);i++)
                {
                    pp[i] = 2;
                }
                pp += (m_alto-1) * m_ancho;
                for (i = 1;i < (m_ancho-1);i++)
                {
                    pp[i] = 2;
                }
                pp = w1->m_maparaya;
                for (i = 1;i < (m_alto-1);i++)
                {
                    pp[i*m_ancho] = 1;
                    pp[i*m_ancho+m_ancho-1] = 1;
                }
                pp[0] = 3;
                pp[m_ancho-1] = 5;
                pp[(m_alto-1)*m_ancho] = 9;
                pp[(m_alto-1)*m_ancho+m_ancho-1] = 11;
            }
            else
            {
				int l = ::strlen((char *)dato);
				if (l >= w1->m_maparaya_size)
					l = w1->m_maparaya_size - 1;
                ::memcpy(w1->m_maparaya,dato,l);
            }
       }

       if (!w1->Create((char *)dato, otro_estilo | WS_CHILD |SS_NOPREFIX, rect, pDlg ,IDW_STDDLG+m_ctrlid))
       {
           delete w1;           
       }
       else
       {
           if (w1->m_group)
           {
              w1->ModifyStyleEx(0,WS_EX_TRANSPARENT,SWP_FRAMECHANGED);
           }           
           m_ctrl = (CWnd *)w1;
		   w1->PonFont();		             

           ::SetWindowPos(w1->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);  
		   //w1->ShowWindow(SW_SHOW);

           return (CWnd *)w1;
       }       
    }
    else
    if (tipo == 3 || tipo == 13) // boton estardard
    {
        CDsButton *w1 = new CDsButton();
        int *p1 = (int *)dato;
		char tmptexto[256];


		DWORD otro_estilo = BS_MULTILINE;
		m_tipo = 3; // 13 == 3

        w1->m_pDoc = pDlg->m_pDoc;
        // CButton

		if (tipo == 13)
		{
		   w1->Izquierdo();
		}				

		int itx = 0;
		char *ipp;
        for (ipp = (char *)(p1+3);*ipp;ipp++)
		{
			if (*ipp == '&') { // Acelerador
                int tecla = *(ipp+1);
				if (tecla > 0 && tecla < 256) {
					w1->m_tecladirecta = tecla;
					theApp.m_teclashortcuts[tecla] = this;
				}
 			}

		   if (*ipp == '{' && *(ipp+1) == '[')
		   {
			   ipp += 2; // {[255000128,128000255]}

			   w1->m_idimagen = atoi(ipp);
			   w1->m_FrColor = rgb_string(ipp);
			   w1->m_BkColor = rgb_string(ipp+10);
			   w1->m_pBkBrush = new CBrush();
			   w1->m_pBkBrush->CreateSolidBrush(w1->m_BkColor);

			   while (*ipp && *ipp != ']' && *(ipp+1) != '}')
				   ipp++;
			   if (!*ipp)
				   break;
			   ipp += 2;
			   if (!*ipp)
				   break;
		   } if (*ipp == '{' && *(ipp+1) == '{')
		   {
			   ipp += 2;
			   w1->m_idimagen = atoi(ipp);
			   while (*ipp && *ipp != '}' && *(ipp+1) != '}')
				   ipp++;
			   if (!*ipp)
				   break;
			   ipp += 2;			   
			   if (!*ipp)
				   break;
		   }
		   else
		   {
			   tmptexto[itx] = *ipp;
			   oem_a_ansi((unsigned char *)&(tmptexto[itx]));
			   itx++;		   
		   }
		}
		tmptexto[itx] = 0;

        if (!w1->Create( (LPCTSTR)tmptexto, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP | otro_estilo, rect, pDlg,IDW_STDDLG+m_ctrlid))
        {
            delete w1;
        }
        else
        {        

			if (theApp.m_ifontboton > -1) {
					w1->m_Font = new CFont();					
					w1->m_Font->CreateFontIndirect(&(theApp.m_dfont[theApp.m_ifontboton]));
					w1->SetFont(w1->m_Font);
			}
			else
				w1->SetFont(pDlg->m_pDoc->m_cfLabel);

			m_cid.m_lcid = p1[1];
			if (m_cid.m_lcid < 0)
			{
				m_cid.m_lcid = -m_cid.m_lcid;
				m_cid.m_lcid--;
				w1->m_escampo = TRUE;
			}
			m_cid.m_lref = (int)(short)(p1[2] & 0xffff);
            w1->m_teclarespuesta = (int)(short)(p1[0] & 0xffff);
			m_nivelfoco = (int)(short)((p1[2] >> 16) & 0xffff);
			if (!w1->m_teclarespuesta)
				m_evento = ((p1[0] >> 16) & 0xffff);
            
            m_ctrl = (CWnd *)w1;
            w1->m_pstdctrl = this;
            px1[0] = m_ctrlid;
			
			/*
			{				
				extern CDsStdCtrl *dispara_botones[8];
				if (strstr(tmptexto,"Ptes. Conf.")) 
					dispara_botones[0] = this;
				else if (strstr(tmptexto,"Confirmados"))
					dispara_botones[1] = this;
				else if (strstr(tmptexto,"A Calibrar"))
					dispara_botones[2] = this;
				else if (strstr(tmptexto,"Reservados"))
					dispara_botones[3] = this;
				else if (strstr(tmptexto,"Sin Reserva"))
					dispara_botones[4] = this;
				else if (strstr(tmptexto,"Ptes. Servir"))
					dispara_botones[5] = this;
				else if (strstr(tmptexto,"Todos"))
					dispara_botones[6] = this;
			}
			*/
            return (CWnd *)w1;            
        }
    }
    else if (tipo == 4) // grid estardard
    {        
        // GRID!!
        char *p = (char *)dato;
        char *titulo;
        struct Info_Entra_Campos *columnas;
        long registros;
        long inicial;
        int localizador;
        int g_modo;
        int p1,p2;        
        int padre;
        CTablaW *ptablaw = new CTablaW();

        if (ptablaw)
        {
          p += sizeof(long)*3;
          localizador = *((int *)p);
          p += sizeof(int);
          registros = *((long *)p);
          p += sizeof(long);
          inicial = *((long *)p);
          p += sizeof(long);
          titulo = p;
          p += (strlen(titulo) + 1);
          g_modo = *((int *)p);
          p += sizeof(int);
          p1 = *((int *)p);
          p += sizeof(int);
          p2 = *((int *)p);
          p += sizeof(int);

          //wid = *((int *)p);
          p += sizeof(int);
          //id = *((int *)p);
          p += sizeof(int);
          //sid = *((int *)p);
          p += sizeof(int);

          m_cid.m_lcid = *((int *)p);

  		  if (m_cid.m_lcid < 0)
		  {
			  m_cid.m_lcid = -m_cid.m_lcid;
			  m_cid.m_lcid--;
			  ptablaw->m_escampo = TRUE;
		  }

		  p += sizeof(int);
          m_cid.m_lref = *((int *)p);
		  if (ptablaw->m_escampo && m_cid.m_lref < 0)
		  {
			  m_cid.m_lref = -m_cid.m_lref;
			  m_cid.m_lref--;			  
			  CDsStdCtrl *pctrl = NULL;
			  if (CDsStdDlg::m_todosctrl.Lookup(m_cid.m_lref,pctrl) && pctrl && pctrl->m_tipo == 4 && pctrl->m_ctrl)
			  {				  
				  CRect rr;
			      CTablaView *ptablaw = &(((CTablaW *)pctrl->m_ctrl)->m_tabla);				  
				  ptablaw->PosicionElemento( &rr, ptablaw->m_cursel , m_cid.m_lcid /*+COLUMNA_INICIAL*/,TRUE );
				  ptablaw->ClientToScreen(&rr);
				  pDlg->ScreenToClient(&rr);
				  rect.OffsetRect( rr.left-rect.left, rr.bottom-rect.top );
				  ptablaw->m_escampo = FALSE;
				  /* falta reajuste si no cabe bien */
			  }
		  }
		  p += sizeof(int);

          padre = *((int *)p);
          p += sizeof(int);

          columnas = (struct Info_Entra_Campos *)p;

		  char *pextra = p + columnas->Len;


          CFont *pFont = NULL;
		  if (theApp.m_ifontgrid > -1) {
		      pFont = new CFont();
		      pFont->CreateFontIndirect(&(theApp.m_dfont[theApp.m_ifontgrid]));
		  }
          

          //m_gmodo  bits: 0 = consulta simple  1 = cambio clave 2 = ventana parcial 3 = movible 4 = ajustable
  
          CWnd *pariente;

          if ( !(g_modo & 4) || (g_modo & 64))
          {              
              pariente = (CWnd *)pDlg->m_pView;
			  if ((g_modo & 64))
			  {
				  pDlg->ClientToScreen(&rect);
				  pariente->ScreenToClient(&rect);
			  }
          }
          else
          {
              pariente = (CWnd *)pDlg;
          }

          ptablaw->m_dsctrl = this;		  
		  if ((g_modo & 0x1000000))
		  {
			 RECT r;
			 r.left = m_lpi;
			 r.right = 1;
			 r.top  = m_lpf;
			 r.bottom = 0;
			 pDsSys->DespachadorInterno9(7,(void *)&r);
			 rect = r;
		     if ( !(g_modo & 4) || (g_modo & 64))
			 {
				  pDlg->ClientToScreen(&rect);
				  pariente->ScreenToClient(&rect);
			 }
		  }
  
          ptablaw->Crea(pariente,(LPCSTR)titulo,columnas,registros,inicial,m_ctrlid,pFont,localizador,g_modo,&rect,padre,pDlg->m_pView,pextra);

          /*
          if (!m_visible && m_pEDs)
          {
             HazVisible();
          }
          */

          //m_pEDs->ShowWindow(SW_HIDE);
          if ( !(g_modo & 4) )
          {
             //pEDs->EnableWindow(FALSE);
          }

          p = ((char *)dato)+sizeof(long)*3;
          memcpy((void *)p,(void *)&m_ctrlid,sizeof(int)); /* el grid pone el id a su modo */
          p += sizeof(int);
          memcpy((void *)p,(void *)&(ptablaw->m_tabla.m_curcol),sizeof(int));

          m_ctrl = (CWnd *)ptablaw;
		  
		  if (!(g_modo & 0x2000000))
		  {
			//ptablaw->ShowWindow(SW_SHOW);
			::SetWindowPos( ptablaw->m_hWnd, (HWND)HWND_TOP ,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			ptablaw->ShowWindow(SW_SHOW);
			ptablaw->BringWindowToTop();
		  }

          if (!(g_modo & 8) || (g_modo & 64))
          {
             ptablaw->SetFocus();
          }
		  {
			  CRect r1,r2;
			  ptablaw->GetWindowRect(&r1);
			  pDlg->m_pView->GetWindowRect(&r2);
			  if (!r2.PtInRect( CPoint(r1.left,r1.top) )     ||
				  !r2.PtInRect( CPoint(r1.left,r1.bottom) )  ||
				  !r2.PtInRect( CPoint(r1.right,r1.top) )    ||
				  !r2.PtInRect( CPoint(r1.right,r1.bottom) ) 
				  )
			  {
				pDlg->m_pView->SetScrollDs();
			  }
		  }		  

          return m_ctrl;
          }
        int menos = -1;
        p = ((char *)dato)+sizeof(long)*3;
        memcpy((void *)p,(void *)&menos,sizeof(int));
    }
    else
    if (tipo == 5)
    {
        CDsCtrlScroll *w1;
        int vertical = !*((int *)dato);
        w1 = new CDsCtrlScroll();

        DWORD modo  = 0;
        
        if (vertical)
        {
          modo = SBS_VERT | SBS_RIGHTALIGN;
          rect.right += ::GetSystemMetrics(SM_CXVSCROLL);
        }
        else
        {
          modo = SBS_HORZ | SBS_BOTTOMALIGN;
          rect.bottom += ::GetSystemMetrics(SM_CYHSCROLL);
        }

        w1->Create(modo| WS_TABSTOP,rect,pDlg,IDW_STDDLG+m_ctrlid);   
        w1->SetScrollRange( 0, 100, FALSE );
        w1->SetScrollPos( 50, FALSE);
        w1->ShowScrollBar( TRUE );
        m_ctrl = (CWnd *)w1;
        w1->m_dsctrl = this;

        return (CWnd *)w1;
    }
    else if (tipo == 6)
    {        
        CDsListBox *w1;
        w1 = new CDsListBox();

        w1->m_idmensa = px1[0];
        w1->m_dsctrl = this;

        w1->Create(LBS_HASSTRINGS | LBS_USETABSTOPS | LBS_STANDARD | WS_VISIBLE | WS_TABSTOP,rect,pDlg,IDW_STDDLG+m_ctrlid);

        w1->ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);

        w1->SetFont(pDlg->m_pDoc->m_cfEdit);

        m_ctrl = (CWnd *)w1;
        
        w1->SetTabStops( 11 );

        //px1[0] id
        //px1[1] defecto
        //px1[2] numero

        
        char usi2[50];
        ::sprintf(usi2,"%d",px1[2]);
        char usi[50];
        ::sprintf(usi,"%%0%dd ..\t%%s",::strlen(usi2));

        char *p = (char *)(px1+3);
        CString s;
		int i;
        for (i = 0;i < px1[2];i++)
        {
            s.Format(usi,i,p);
            w1->AddString(s);
            p += (strlen(p)+1);
        }
        int defecto = px1[1] - 1;
        if (defecto < 0) defecto = 0;
        if (defecto >= px1[2]) defecto = px1[2]-1;

        w1->SetCurSel( defecto );

        px1[0] = m_ctrlid;
        
        
        return (CWnd *)w1;
    }
    else if (tipo == 7)
    {
        CDsTextEdit *w1;

        w1 = new CDsTextEdit();

        DWORD otro_estilo = ES_LEFT | ES_MULTILINE | ES_WANTRETURN /*| ES_NOHIDESEL */;        

		int flags = px1[4];
		int maximolinea = ((flags & 0xffff0000) >> 16);
		flags &= 0xffff;

		if ((flags & 4))
		{
			maximolinea = 0;
		}

		w1->m_maximolinea = maximolinea;
		w1->m_topelineas = px1[3];

		if ((flags & 1))
			otro_estilo |= ES_AUTOHSCROLL | WS_HSCROLL;
		if ((flags & 2))
			otro_estilo |= ES_AUTOVSCROLL | WS_VSCROLL;
        
        if (!w1->Create(otro_estilo | WS_CHILD | WS_TABSTOP, rect, pDlg ,IDW_STDDLG+m_ctrlid))
        {
            delete w1;
        }
        else
		{
            m_ctrl = (CWnd *)w1;

            px1[0] = m_ctrlid;

            m_cid.m_lcid = px1[1];
            m_cid.m_lref = px1[2];

			//w1->FmtLines( TRUE );
			
			int topelin = px1[3];			
            int tope = px1[5];
            if (tope)
            {
                //w1->SetLimitText( tope + 1 + topelin );// CESC OJO
            }	


			if (!(flags & 4)/* && tope && topelin > 0*/ && maximolinea > 0)
			{			
				CRect r2;
				CPoint pp;
				w1->GetClientRect(r2);

				//pp.x = (r2.Size().cx / (tope / topelin));
				pp.x = (r2.Size().cx / maximolinea);
				/*
				if (!(flags & 2))
					pp.y = (r2.Size().cy / topelin)-2;
				else
				*/
					pp.y = 0;
				
				CDC *cdc = w1->GetDC();
				cdc->DPtoLP(&pp,1);
				w1->ReleaseDC(cdc);

				w1->m_Font = new CFont();	   	     					 
				if (theApp.m_ifontmemo2 > -1) {
					w1->m_Font = new CFont();					
					w1->m_Font->CreateFontIndirect(&(theApp.m_dfont[theApp.m_ifontmemo2]));
					w1->SetFont(w1->m_Font);
				}
				else {
					w1->m_Font->CreateFont(pp.y,pp.x, 
					 0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_TT_ALWAYS,
					 DEFAULT_QUALITY ,FIXED_PITCH,theApp.m_FontI.lfFaceName);   
				}
			    
				cdc = w1->GetDC();				
				CFont *oldf = (CFont *)cdc->SelectObject( w1->m_Font );
				CSize ss = cdc->GetTextExtent( "M" , 1 );
				pp.x = ss.cx;
				pp.y = ss.cy;
				cdc->LPtoDP(&pp,1);
				ss.cx = pp.x;
				ss.cy = pp.y;
				cdc->SelectObject( oldf );
			    w1->ReleaseDC(cdc);
				//int nLeft = (r2.Size().cx - ss.cx * ((tope/topelin)+1))/2;
				int nLeft = (r2.Size().cx - ss.cx * ((maximolinea)+1))/2;
			    int nRight = nLeft;
				w1->SetFont(w1->m_Font);
				if (nLeft >= 0 && nRight >= 0)
			 		w1->SetMargins( nLeft, nRight );
			}
			else
			{
				if (theApp.m_ifontmemo > -1) {
					w1->m_Font = new CFont();					
					w1->m_Font->CreateFontIndirect(&(theApp.m_dfont[theApp.m_ifontmemo]));
					w1->SetFont(w1->m_Font);
				}
				else
					w1->SetFont(pDlg->m_pDoc->m_cfEdit);
			}
            char *pp = (char *)(px1+6);
			char *ipp;
            for (ipp = pp;*ipp;ipp++)
                oem_a_ansi((unsigned char *)ipp);

            w1->SetWindowText(pp);

            w1->m_dsctrl = this;

            w1->ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);

            w1->ShowWindow(SW_SHOW);

            ::SetWindowPos(w1->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

            return (CWnd *)w1;
        }
    }
    else if (tipo == 8)
    {   // group box
        CDsButton *w1 = new CDsButton();        

        w1->m_pDoc = pDlg->m_pDoc;
        w1->m_group = TRUE;

       switch((Atrib&~128))
       {
           case 'R':
             w1->m_pBkBrush = new CBrush();
             w1->m_pBkBrush->CreateSolidBrush(pDlg->m_pDoc->m_cLabelRBk);
             w1->m_BkColor = pDlg->m_pDoc->m_cLabelRBk;
             w1->m_FrColor = pDlg->m_pDoc->m_cLabelRFr;
             break;
           case 'I':
             w1->m_pBkBrush = new CBrush();
             w1->m_pBkBrush->CreateSolidBrush(pDlg->m_pDoc->m_cLabelIBk);
             w1->m_BkColor = pDlg->m_pDoc->m_cLabelIBk;
             w1->m_FrColor = pDlg->m_pDoc->m_cLabelIFr;
             break;             
       }
        // CButton
        if (!w1->Create( (LPCTSTR)(dato), WS_CHILD  | WS_VISIBLE | BS_GROUPBOX | WS_TABSTOP, rect, pDlg,IDW_STDDLG+m_ctrlid))
        {
            delete w1;
        }
        else
        {   
			/*
			if (theApp.m_ifontboton > -1) {
				w1->m_Font = new CFont();					
				w1->m_Font->CreateFontIndirect(&(theApp.m_dfont[theApp.m_ifontboton]));
				w1->SetFont(w1->m_Font);
			}
			else*/
				w1->SetFont(pDlg->m_pDoc->m_cfLabel);                
            m_cid.m_lcid = m_lpi*1000+m_lpf;
            m_ctrl = (CWnd *)w1;
            w1->m_pstdctrl = this;
            ::SetWindowPos(w1->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
            /*px1[0] = m_ctrlid;*/
            return (CWnd *)w1;            
        }
    }
    else if (tipo == 12)
    {
        m_esw = FALSE;
        CDsOCX *w1;
        w1 = new CDsOCX((LPCTSTR)dato);
        m_ctrl = (CWnd *)w1;
        return (CWnd *)w1;
    }
	else if (tipo == 14)
	{
		CDsImagen *w1;
		int *p1 = (int *)dato;
		w1 = new CDsImagen();
		w1->m_pstdctrl = this;
		/*
        if (!w1->Create(NULL,(LPCTSTR)(p1+3), WS_CHILD  | WS_VISIBLE | WS_CLIPCHILDREN |WS_CLIPSIBLINGS, rect, pDlg,IDW_STDDLG+m_ctrlid))
		*/
		//(LPCTSTR)(p1+3)
		/*
		if (!w1->Create(pDlg,rect,IDW_STDDLG+m_ctrlid,0L, WS_CHILD  | WS_VISIBLE | WS_CLIPCHILDREN |WS_CLIPSIBLINGS, NULL))
		*/		
		if (!w1->Create(::AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,NULL,NULL,NULL),(LPCTSTR)(p1+3), WS_CHILD  | WS_VISIBLE | WS_CLIPCHILDREN |WS_CLIPSIBLINGS|WS_HSCROLL|WS_VSCROLL, rect, pDlg,IDW_STDDLG+m_ctrlid))
        {
            delete w1;
        }
        else
        { 
			w1->LoadImage((LPCTSTR)(p1+3));
            w1->SetFont(pDlg->m_pDoc->m_cfLabel);                
            m_cid.m_lcid = m_lpi*1000+m_lpf;
            m_ctrl = (CWnd *)w1;
            /*w1->m_pstdctrl = this;*/
            ::SetWindowPos(w1->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);            
			px1[0] = m_ctrlid;
            return (CWnd *)w1;            
        }
	}
	else if (tipo == 15) // TREE
	{
		CDsTreeCtrl *w1;
		int *p1 = (int *)dato;
		w1 = new CDsTreeCtrl();
		w1->m_pstdctrl = this;

        if (!w1->Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_TRACKSELECT,
            rect, pDlg,IDW_STDDLG+m_ctrlid))
        {
            delete w1;
        }
        else
        {
			char *pchar = (char *)(p1+3);
			char *pn;
			char *pnamefile = NULL;

			pn = pchar;
			while(*pn)
			{
				if (*pn == '{' && *(pn+1) == '{')
				{
					*pn = 0;
					pn += 2;
					pnamefile = pn;
					while(*pn)
					{
						if (*pn == '}' && *(pn+1) == '}')
						{
							*pn = 0;
							break;
						}
						pn++;
					}
					break;
				}
				pn++;
			}			
			w1->Inicializa();
			{
				for (char *ipp = pchar;*ipp;ipp++)
					oem_a_ansi((unsigned char *)ipp);
			}
			w1->InsertItem(TVIF_IMAGE | TVIF_TEXT,
					_T(pchar),
					0,
					0,
					0,
					0,
					0,
					NULL,
					TVI_FIRST);	
			{
				for (char *ipp = pchar;*ipp;ipp++)
					ansi_a_oem((unsigned char *)ipp);
			}
            m_cid.m_lcid = m_lpi*1000+m_lpf;
            m_ctrl = (CWnd *)w1;            
            px1[0] = m_ctrlid;

			if (pnamefile)
			{
				FILE *f = fopen(pnamefile,"r");
				//FILE *f = fopen("c:\\arbol.txt","r");
				char tmpx[512];
				char *tmpfile = tmpx+sizeof(int);
				if (f)
				{		
					w1->m_acelerado = TRUE;
					while(fgets(tmpfile,500,f))
					{						
						// corta final de linea y trim de los espacios						
						   pn = tmpfile;while(*pn) {if (*pn == 10 || *pn == 13){*pn = 0;break;}	pn++;}
						   while(pn >= tmpfile){if (*pn != ' ')break;*pn = 0;pn--;}

						memset(tmpx,0,sizeof(int));
						w1->OnCommand(ID_COMMANDCONTROL, (LPARAM)tmpx);
					}
					w1->m_acelerado = FALSE;
					fclose(f);
				}
			}
            return (CWnd *)w1;            
        }
	}
	else if (tipo == 999) // Tab (lo dejamos como tipo maximo!!!!!)
    {
        m_cid.m_lcid = *((int *)dato);
        {            
           int otro = pDlg->BuscaControlPorCid(999,m_cid);
           if (otro > -1)
           {
               if (!pDlg->DestruyeControl(otro))
               {
                   m_antcid_ctrlid = otro;
               }
           }
        }

        CDsTabCtrl *w1 = new CDsTabCtrl(); 
        w1->m_pDoc = pDlg->m_pDoc;

#ifdef DSPROFUIS
        if (!w1->Create(pDlg,rect,IDW_STDDLG+m_ctrlid,WS_CHILD | WS_CLIPCHILDREN| WS_TABSTOP | WS_VISIBLE))
#else
		if (!w1->Create(TCS_TABS| WS_CHILD | WS_CLIPCHILDREN| WS_TABSTOP,rect,pDlg,IDW_STDDLG+m_ctrlid))
#endif
        {
            delete w1;
        }
        else
        {            
            w1->DeleteAllItems();

            w1->SetFont(pDlg->m_pDoc->m_cfTitulos);

            w1->SetItemExtra( 0 );
            TCITEM Item;

            Item.mask = TCIF_TEXT;
            Item.pszText = "";
            Item.cchTextMax = 0;      
            w1->m_dlgs.SetAtGrow( 0, (CDsStdDlg *)NULL );
            w1->InsertItem(0,&Item);

            w1->ShowWindow(SW_SHOW);

            m_ctrl = (CWnd *)w1;

            w1->m_dsctrl = this;

            px1[0] = m_ctrlid;

            return (CWnd *)w1;
        }
    }

    if ( ((unsigned int)m_ctrlid & 0x80000000) || !((unsigned int)m_ctrlid & 0x40000000) )
	{
		CDsStdDlg::m_freehandles[m_ctrlid] = NULL;
	}
    CDsStdDlg::m_todosctrl.RemoveKey(m_ctrlid);
    m_ctrlid = -1;
    return NULL;
}


CWnd * CDsStdCtrl::Create(int tipo, int lpos, int lpf, void *dato,CDsStdDlg *pDlg,int Atrib,int Cc, int Cf,int pre_id)
{
    CWnd *pWnd = real_create(tipo,lpos,lpf,dato,pDlg,Atrib,Cc, Cf,pre_id);
    if (m_esw && pWnd)
    {
		 if (!m_ctrlfirst)
		 {
			 m_ctrlfirst = this;
		 }
		 m_ctrlbefore = m_ctrllast;
		 if (m_ctrlbefore)
		 {
			 m_ctrlbefore->m_ctrlafter = this;
		 }
		 m_ctrllast = this;
		 /*
		 m_tooltip.Create(pDlg,TTS_NOPREFIX | 0x40);
		 m_tooltip.AddTool(pWnd,"PRUEBA");
		 m_tooltip.Activate(TRUE);
		 */
         m_cthwnd = pWnd->m_hWnd;  // acceso a la clase de control por el handle de la ventana del control
         CDsStdDlg::m_hwndctrllst[m_cthwnd] = this;
         pWnd->ModifyStyle(0,WS_CLIPSIBLINGS/*,SWP_NOSIZE|SWP_NOMOVE*/);
         //m_tooltiptext.Format("Control id %d",m_cid);
         m_pdlg->CompruebaPushV(this);
    }
    return pWnd;
}


int CDsStdCtrl::ControlEdicion(int flag,void *datos,int pre_id)
{
    int *px1 = (int *)datos;
	BOOL hacambiado = FALSE;


	/* comprobar pre_id ??? */
	if ((px1[0] == 2 || px1[0] == 7) && !((unsigned int)pre_id & 0x80000000) && ((unsigned int)pre_id & 0x40000000) && pre_id != m_ctrlid)
	{
		/* Se esta reutilizando el control, cambiar m_ctrlid a pre_id */
	    CDsStdCtrl *pctrl = NULL;		
		void *ppx;
	    if (CDsStdDlg::m_freehandles.Lookup(m_ctrlid,ppx))
		{
		    CDsStdDlg::m_freehandles[m_ctrlid] = NULL;
		}		  
        if (CDsStdDlg::m_todosctrl.Lookup(m_ctrlid,pctrl))
		{  
		   ASSERT(pctrl == this);
           CDsStdDlg::m_todosctrl.RemoveKey(m_ctrlid);
        }
		m_pdlg->m_ctrlst.RemoveKey(m_ctrlid);        
		m_ctrlid = pre_id;
		CDsStdDlg::m_todosctrl[m_ctrlid] = this;
		m_pdlg->m_ctrlst[m_ctrlid] = this;
	}

	ASSERT(px1[0] == 11 || ((unsigned int)pre_id & 0x80000000) || !((unsigned int)pre_id & 0x40000000) || pre_id == m_ctrlid);

    if (px1[0] == 11) // control de edicion temporal dentro de otro control
    {                      
        px1[6] = (int)this;
        int r = m_pdlg->InsertaControl(px1[0],px1[1],px1[2],(void *)(px1+6),px1[3],px1[4],px1[5],NULL,pre_id);
		return r;
    }

	if (px1[0] == 7)
	{
		CDsTextEdit *w1 = (CDsTextEdit *)m_ctrl;
		char *pp = (char *)(px1+12);
		char *ipp;
        for (ipp = pp;*ipp;ipp++)
            oem_a_ansi((unsigned char *)ipp);		
        w1->SetWindowText(pp);		

		px1[6] = m_ctrlid;
		
		return m_ctrlid; // de momento el control de edicion no cambia
	}
       // control edicion
    int tipo = px1[0];
    int lpi  = px1[1];
    int lpf  = px1[2];
    int o_lpi = m_lpi;
    int o_lpf = m_lpf;
    Cid lcid;
    lcid.m_tipo = px1[0];


    int Atrib = px1[3];
    int Cc    = px1[4];
    int Cf    = px1[5];

    px1 += 3; // reajusto teniendo en cuenta que esta el atributo!!!

    char *pp = (char *)(px1+14);
	char *pqi,*pqs;
    
    lcid.m_lcid = px1[4];    
    lcid.m_lref = px1[12];
    


    CRect rect;
    CDsCombo *w1;
    BOOL cambiapos = FALSE;

    /*
    if (m_tipo == 4)
    {
        w1 = new CDsCombo();
        w1->m_pView = m_pdlg->m_pView;
        w1->m_dsctrl = this;
        CTablaW *pw = (CTablaW *)m_ctrl;        
        pw->m_tabla.PosicionElemento(rect,pw->m_tabla.m_cursel,/*COLUMNA_INICIAL+*-/lcid.m_lcid);
        pw->m_tabla.m_editando = TRUE;
    }
    else
    */
    {
        ASSERT(m_tipo == 2);
        //ASSERT(CDsStdDlg::ComparaCid(m_cid,lcid,FALSE));
		if (!CDsStdDlg::ComparaCid(m_cid,lcid,FALSE))
		{
			//px1[6] = m_ctrlid;
			return -1;
		}

        w1 = (CDsCombo *)m_ctrl;
        if (m_lpi != lpi)
        {
           m_lpi = lpi;
           cambiapos = TRUE;
        }
        if (m_lpf != lpf)
        {
           m_lpf = lpf;
           cambiapos = TRUE;
        }

        lpi -= m_pdlg->m_lpi;
        lpf -= m_pdlg->m_lpi;
        lpf += 101;

        if (cambiapos)
        {
           rect.top = m_pdlg->m_ult_hijo.top + m_pdlg->m_offy + (m_pdlg->m_stdy * (lpi/100))/10;
           rect.left = m_pdlg->m_ult_hijo.left + m_pdlg->m_offx + (m_pdlg->m_stdx * (lpi%100))/10;
           rect.right = m_pdlg->m_ult_hijo.left + m_pdlg->m_offx + (m_pdlg->m_stdx * (lpf%100))/10;
           rect.bottom = m_pdlg->m_ult_hijo.top + m_pdlg->m_offy + (m_pdlg->m_stdy * (lpf/100))/10;
           if (rect.Size().cy > 20)
              rect.bottom -= ((m_pdlg->m_stdy/6)/10);
           else
           if (rect.Size().cy > 18)
              rect.bottom -= ((m_pdlg->m_stdy/8)/10);
           else
           if (rect.Size().cy > 15)
              rect.bottom -= ((m_pdlg->m_stdy/10)/10);
        }
        else
        {
            w1->GetWindowRect(rect);
            m_pdlg->ScreenToClient(rect);
        }

        if ( (px1[8] && !(w1->Getestilo() & ES_READONLY)) || (!px1[8] && (w1->Getestilo() & ES_READONLY)))
        {
            if (px1[8])
            {
                w1->SetReadOnly( TRUE );
                w1->Setestilo(w1->Getestilo() | ES_READONLY);
				w1->Setestilo(w1->Getestilo() & ~WS_TABSTOP);
			    w1->ModifyStyle(WS_TABSTOP,0,SWP_FRAMECHANGED);
            }
            else
            {
                w1->SetReadOnly( FALSE );
                w1->Setestilo(w1->Getestilo() & ~ES_READONLY);
				w1->Setestilo(w1->Getestilo() | WS_TABSTOP);
				w1->ModifyStyle(0,WS_TABSTOP,SWP_FRAMECHANGED);
            }
			if (w1->EsPosicional())
			{
				hacambiado = TRUE;
			}
        }

    }

    DWORD otro_estilo = 0;

    // aqui + 3
    // px1[0] == cwid (en este caso a poner *)
    // px1[1] == cid
    // px1[2] == maxqc
    // px1[3] == qc (minimo visualizable)
    // px1[4] == ctipo
    // px1[5] == modificable 
    // px1[6] == decimales
    // px1[7] == len qi
    // px1[8] == len qs
    // px1[9] == len qd-qo? 

	
	pqi = pp;
	pp += px1[10];
	pqs = pp;
	pp += px1[11];

	if (w1->HaCambiado(px1[5],px1[6],px1[7],px1[9],pqi,pqs))
	{
		hacambiado = TRUE;
		w1->Setmaxqc(px1[5]);
		w1->SetLimitText(w1->Getmaxqc());
		w1->Setqc(px1[6]);
		w1->Settipo(px1[7]);
		if (w1->GetTipo() == 10)
		{
			w1->Settipo(1);
			otro_estilo |= ES_PASSWORD;
		}
		w1->Setqh(1);
		w1->Setdecimales(px1[9]);

		w1->Setqi(pqi);		
		w1->Setqs(pqs);		

		if ( px1[8] )
		{
		   otro_estilo |= ES_READONLY;
		}
		else
		{
			otro_estilo |= WS_TABSTOP;
		}

		if (px1[7] == 3)
		{
		   otro_estilo |= ES_RIGHT;
		}
		else
		{
		   otro_estilo |= ES_LEFT;
		}

		if (w1->GetTipo() == 1)
		{
		   if (strstr(w1->Getqs(),"<"))
		   {
			   otro_estilo |= ES_LOWERCASE;
		   } else if (strstr(w1->Getqs(),">"))
		   {
			   otro_estilo |= ES_UPPERCASE;
		   }
		   char *ptmp1;
		   if ((ptmp1 = strstr(w1->Getqs(),"#")))
		   {
			   int maxqc;
			   char usi[15];
			   int i,j;

			   maxqc = w1->Getqc();
			   i = 1;
			   j = 0;
			   while(j < 14 && ptmp1[i] && ptmp1[i] >= '0' && ptmp1[i] <= '9')
			   {
				   usi[j] = ptmp1[i];
				   j++;
				   i++;
			   }
			   if (j)
			   {
				  usi[j] = 0;
				  j = atoi(usi);
				  if (j > 0)
				  {
					  maxqc = j;
				  }
			   }			   
			   w1->Setmaxqc(maxqc);
		   }
		   if (strstr(w1->Getqs(),"+"))
		   {
			   otro_estilo &= ~ES_LEFT;
			   otro_estilo |= ES_RIGHT;
		   }
		}


		if (px1[7] == 3 && !px1[9])
		{
			//otro_estilo |= ES_NUMBER;
		}    
	}

	char *ipp;
    for (ipp = pp;*ipp;ipp++)
        oem_a_ansi((unsigned char *)ipp);

    /*
    if (m_tipo == 4)
    {
        CTablaW *pw = (CTablaW *)m_ctrl;
        if (!w1->Create( otro_estilo | WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE, rect, &pw->m_tabla ,IDW_STDDLG+m_ctrlid))
        {
            return -1;
        }
        
        w1->SetFont(m_pdlg->m_pDoc->m_cfGrid);
        
        ASSERT(!pw->m_tabla.m_dsedit);
        pw->m_tabla.m_dsedit = w1;
    }
    else
    */
    {
        if (w1->Getestilo() == -1)
        {
            if (!w1->Create( otro_estilo | WS_CHILD | ES_AUTOHSCROLL, rect, m_pdlg ,IDW_STDDLG+m_ctrlid))
            {
                return -1;
            }

            w1->SetFont(m_pdlg->m_pDoc->m_cfEdit);            

            w1->ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);

            w1->AjustaTamano();

            w1->SetLimitText(w1->Getmaxqc());

            w1->ShowWindow(SW_SHOW);
        }
        else
        {
            if (cambiapos)
            {
                w1->MoveWindow(rect);                
                // si es un campo "puro" crear uno posicional-readonly en la posicion antigua
				/*
                if (!(m_cid.m_lcid & 0xffff0000))
                {
                    int *p1 = (int *)datos;
                    int o_lcid = p1[7];
                    int o_read = p1[11];
                    CString ss;

                    p1[11] = 1;
                    p1[7] = (p1[7]&0xffff) | (lpi<<16);
                    int nid2 = m_pdlg->InsertaControl(tipo,o_lpi,o_lpf,(void *)(p1+6),Atrib,Cc,Cf,NULL,pre_id);
                    w1->GetWindowText(ss);
                    CDsStdCtrl *potro;
                    if (m_pdlg->m_ctrlst.Lookup(nid2,potro))
                    {
                        potro->m_ctrl->SetWindowText(ss);
                    }
                    p1[7] = o_lcid;
                    p1[11] = o_read;
                }
				*/
            }
        }        
    }


	if (w1->SetAtributos(Atrib,Cc,Cf))
	{
		w1->DeleteBkBrush();

		switch((Atrib&~128))
		{
		   case 'R':
			 w1->SetBkBrush(new CBrush());
			 w1->GetBkBrush()->CreateSolidBrush(m_pdlg->m_pDoc->m_cLabelRBk);
			 w1->SetBkColor(m_pdlg->m_pDoc->m_cLabelRBk);
			 w1->SetFrColor(m_pdlg->m_pDoc->m_cLabelRFr);
			 break;
		   case 'I':
			 w1->SetBkBrush(new CBrush());
			 w1->GetBkBrush()->CreateSolidBrush(m_pdlg->m_pDoc->m_cLabelIBk);
			 w1->SetBkColor(m_pdlg->m_pDoc->m_cLabelIBk);
			 w1->SetFrColor(m_pdlg->m_pDoc->m_cLabelIFr);
			 break;             
		}
	}

    w1->SetWindowText(pp);

    pp += (px1[13] + sizeof(int));

    for (ipp = pp;*ipp;ipp++)
    {
        oem_a_ansi((unsigned char *)ipp);
    }
	
	RellenaToolTip(pp);

	//w1->SendMessage(TTM_SETMAXTIPWIDTH,0,100);

	if (hacambiado || cambiapos)
	{

		w1->AjustaCombo();

		::SetWindowPos(w1->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}

    if ((flag & 1))
    {      
       if (m_pdlg->m_pater)
       {
           if (!::IsWindowVisible(m_pdlg->m_pater->m_hWnd))
           {
               m_pdlg->m_pater->m_pView->ActivaDlg(m_pdlg->m_pater->m_wid, m_pdlg->m_pater->m_id, m_pdlg->m_pater->m_sid,TRUE);
           }
       }
       w1->SetFlag((flag >> 1));
	   
	   w1->m_Edit.m_noConfirma = TRUE;
       w1->SetFocus();
	   w1->m_Edit.m_noConfirma = FALSE;
	  
    }    

    px1[3] = m_ctrlid;
    return m_ctrlid;
}

BOOL CDsStdCtrl::DisparaEventoUsuario(int operacion,LPCSTR contenido)
{
	//theApp.m_noreenfocarpordefecto++;
    if (m_evento != -1)
    {
        long tam;
        long respuesta;
        long modo = 1;        
        char *p;
        char tmp[512];   

		//m_usado++;

        p = tmp;

        memcpy((void *)p,(void *)&m_evento,sizeof(int));
        p += sizeof(int);

        tam = (sizeof(long) * 7) + (sizeof(int)*2) + 2;
		if (contenido)
			tam += strlen(contenido);
        respuesta = tam;

        memcpy((void *)p,(void *)&tam,sizeof(long));
        p += sizeof(long);	
        memcpy((void *)p,(void *)&respuesta,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&modo,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&operacion,sizeof(int));
        p += sizeof(int);
		if (contenido)
			strcpy(p,contenido); // no debe de exceder de 249 chars
		else
			*p = 0;		

		if (m_pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window))) // modo 9
		{
			pDsSys->DespachadorInterno9(13,(void *)tmp);
			m_pdlg->PostMessage(WM_COMMAND,(WPARAM)BUSCA_PTECS,(LPARAM)0);
			m_pdlg->SetFocus();
			TeclaStd(TECLA_PING);
		}
		else
		{
			int wid = m_pdlg->m_wid;
			int id = m_pdlg->m_id;
			int sid = m_pdlg->m_sid;
			int wcid = m_ctrlid;
			int ctipo = m_tipo;
			int replicante = m_pdlg->m_replicante;
			Cid cid = m_cid;
			CDsxView *pview = m_pdlg->m_pView;

			pDsSys->DespachadorInterno(13,(void *)tmp);

			pview->TeclaStd(TECLA_PING,wid,replicante+id,sid,wcid,ctipo,cid);
			//TeclaStd(TECLA_PING);
		}
		

		//m_usado--;
		
		//theApp.m_noreenfocarpordefecto--;
        return TRUE;
    }
	//theApp.m_noreenfocarpordefecto--;
    return FALSE;
}

BOOL CDsStdCtrl::TeclaStd(int tecla)
{
    if (tecla == ID_ANTERIORPANE)
    {
        return m_pdlg->CambiaPane(-1);
    } else if (tecla == ID_SIGUIENTEPANE)
    {
        return m_pdlg->CambiaPane(1);        
    }
    else if (tecla == 992 && (m_tipo == 2 || m_tipo == 11) && ((CDsCombo *)m_ctrl)->m_concombo)
	{
		((CDsCombo *)m_ctrl)->OnDropdownButton();
		return TRUE;
	}
	else
    {
        return m_pdlg->TeclaStd(tecla,m_ctrlid,m_tipo,m_cid);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CDsStdDlg

IMPLEMENT_DYNCREATE(CDsStdDlg, STDDLGBASE)

int CDsStdDlg::m_contaid = 1; /* el cero y el -1 invalidos */


int CDsStdDlg::DsStdX = 71;
int CDsStdDlg::DsStdY = 200;

int CDsStdDlg::DsStdOffX = 3;
int CDsStdDlg::DsStdOffY = 3;

int CDsStdDlg::DsMnStdX = 70;
int CDsStdDlg::DsMnStdY = 180;
int CDsStdDlg::DsMnStdOffX = 0;
int CDsStdDlg::DsMnStdOffY = 0;


CFreeHandles CDsStdDlg::m_freehandles;
CStdControlLst CDsStdDlg::m_todosctrl;
CStdCtrlHwnd   CDsStdDlg::m_hwndctrllst;

int doidmask(int id)
{
	//pmask |= id;
	return id;
}

CDsStdDlg::CDsStdDlg()
{
   m_parchear = FALSE;

   m_modoespecial = FALSE;

   m_es9 = FALSE;
   m_evento = -1;

   m_forceframe = FALSE;

   m_noframe   = FALSE;

   m_noscroll  = FALSE;

   m_falso     = FALSE;

   m_id        = -1;
   m_sid       = -1;
   m_sid_padre = -1;
   m_lpi       = -1;
   m_lpf       = -1;

   m_wid       = -1;

   m_pView     = NULL;
   m_pDoc      = NULL;

   m_nombre    = "";

   m_nivel     = -1;

   m_maparaya  = NULL;
   m_maparaya_size = 0;

   m_padretab = NULL;
   m_opcionpadre = -1;

   m_ult_hijo.SetRectEmpty();
   m_ult_hijo_mosaico.SetRectEmpty();

   m_ult_vertical = TRUE;

   m_ult_grow = FALSE;

   m_ult_sid = -1;

   m_ult_first = TRUE;
    
   m_npush = 0;

   m_padre_ult_vertical = TRUE;
   m_padre_ult_grow = FALSE;
   m_padre_ult_first = TRUE;
   m_padre_ult_hijo.SetRectEmpty();


   m_creation_rect.SetRectEmpty();

   m_padre = NULL;

   m_referencia = NULL;

   m_replicante = 0;

   m_pater = NULL;

   m_IdAntFoco = -1;

   /*
   m_cLabelBk;
   m_cLabelFr;
   */
   m_bLabelBk = NULL;

}

CDsStdDlg::~CDsStdDlg()
{

   SacameDelPushv(this);

   CDsStdDlg *padre;
   int padre_wid;

   POSITION pos = m_ctrlst.GetStartPosition( );   
   CDsStdCtrl *nada;
   int wid;
   while (pos)
   {
       m_ctrlst.GetNextAssoc( pos, wid, nada );       
	   if (!nada->m_muriendo)
	   {
		  ASSERT(!nada->m_protegido);
		  delete nada;
		  //m_freehandles[wid] = NULL;       
	   }	   
	   
       if ( ((unsigned int)wid & 0x80000000) || !((unsigned int)wid & 0x40000000) )
	   {		  
		  void *ppx;
		  if (CDsStdDlg::m_freehandles.Lookup(wid,ppx))
		  {
				CDsStdDlg::m_freehandles[wid] = NULL;
		  }		  
	   }       
   }
   m_ctrlst.RemoveAll();
   if (m_maparaya)
   {
       ::free((void *)m_maparaya);
	   m_maparaya_size = 0;
   }
   m_cidctrllst.RemoveAll();

   padre = m_pView->BuscaStdDlg(m_id,m_sid_padre,padre_wid);
   if (padre)
   {
       padre->m_listahijos.RemoveKey(m_wid);
	   if (padre->m_listahijos.GetCount() < 1)
	   {
			padre->m_ult_hijo_mosaico = padre->m_stdrect;
			padre->m_ult_first = TRUE;
	   }
   }  

   if (m_wid != -1)
   {
	    m_pView->m_dlglist.RemoveKey(m_wid);
		if ( ((unsigned int)m_wid & 0x80000000) || !((unsigned int)m_wid & 0x40000000) )
		{
			CDsStdDlg::m_freehandles[m_wid] = NULL;
		}
   }
   if (m_bLabelBk)
      delete m_bLabelBk;
   m_bLabelBk = NULL;
}


BEGIN_MESSAGE_MAP(CDsStdDlg, STDDLGBASE)    
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, DsStdDlgToolTip )
	//{{AFX_MSG_MAP(CDsStdDlg)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(MCN_SELECT,6411, OnSelCalendario)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDsStdDlg message handlers


void CDsStdDlg::OnSelCalendario(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (pNMHDR->hwndFrom)
	{
		::SendMessage(pNMHDR->hwndFrom,WM_COMMAND,MCN_SELECT,MCN_SELECT);
	}	
	*pResult = 0;
}


void CDsStdDlg::OnDestroy() 
{
    if (m_padretab)
    {
        m_padretab->EliminaOpcion(this);
        m_padretab = NULL;
    }
	STDDLGBASE::OnDestroy();	
}


BOOL CDsStdDlg::DsStdDlgToolTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult) 
                                //UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{	
	static CString ss;
	if (!theApp.m_tooltips)
		return FALSE;
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
       CDsStdCtrl *pctrl = NULL;
       HWND hwnd = (HWND)nID;
       if (CDsStdDlg::m_hwndctrllst.Lookup(hwnd,pctrl))
       {        
        if (pctrl->m_tooltiptext.IsEmpty())
		{
			/*pTTT->lpszText = "Diagram Software";
			return TRUE; */
            return FALSE;
		}
		
		CWnd *wnd = CWnd::FromHandle(pNMHDR->hwndFrom);				
		wnd->ModifyStyle(0,0x40);		
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 200);
		ss = RecogeAyudaPorPosicion();
		ss = pctrl->m_tooltiptext + ss;
        pTTT->lpszText = (char *)(LPCSTR)ss;
		//m_tooltiptext = "Flecha Arriba cambiar de clave\r\nFlecha Abajo cambiar de clave\r\nAv-Pag cambio clave\r\nRe-Pag cambio clave\r\nEnter continuar\r\nEsc o Ctrl-Q Salir\r\nF11 - consultar relacion\r\nCtrl-Flecha Abajo desplegar grid\r\nShift-F11 consultar claves";		   
        return TRUE;        
        }
    }

    /*pTTT->lpszText = "Diagram Software";
    return TRUE; */

    return(FALSE);
}

int CDsStdDlg::NewHandle()
{
   int wid;
   POSITION pos = m_freehandles.GetStartPosition( );   
   void *nada;
   if (pos)
   {
       m_freehandles.GetNextAssoc( pos, wid, nada );
       m_freehandles.RemoveKey(wid);       
   }
   else
   {
       wid = m_contaid++;
   }
   return wid;
}

BOOL CDsStdDlg::ComparaCid(Cid &c1, Cid &c2,BOOL Absoluto)
{
   int lcid1 = c1.m_lcid;
   int lcid2 = c2.m_lcid;
   if (!Absoluto)
   {
       lcid1 &= 0xffff;
       lcid2 &= 0xffff;
   }
   if (lcid1 != lcid2) return FALSE;
   if (c1.m_lref != c2.m_lref) return FALSE;
   if (c1.m_tipo != c2.m_tipo) return FALSE;
   return TRUE;
}

int CDsStdDlg::DeterminaTecla(UINT nChar)
{
   int comando = 0;

   if ((::GetKeyState( VK_CONTROL ) & 0x8000)) {
	    CDsStdCtrl *pctrl = NULL;
		int tecla = (int)nChar;
		if (theApp.m_teclashortcuts.Lookup(tecla,pctrl)) {
			if (pctrl != NULL && pctrl->m_ctrl != NULL) {
				if (pctrl->m_ctrl->IsKindOf(RUNTIME_CLASS(CDsButton))) {
					CDsButton *pbut = (CDsButton *)pctrl->m_ctrl;
					pbut->PerformClick();
					return 977;
				}
			}
		}
	}

   if (nChar == 'S' && (::GetKeyState( VK_MENU ) & 0x8000) && (::GetKeyState( VK_LSHIFT ) & VK_LSHIFT) &&
	   (::GetKeyState( VK_RSHIFT ) & VK_RSHIFT))
   {
	   pMFrame->PostMessage(WM_COMMAND,TECLA_SUPERUSUARIO,0);
	   return 0;
   }


   if (nChar == 'T' && (::GetKeyState( VK_MENU ) & 0x8000) && (::GetKeyState( VK_LSHIFT ) & VK_LSHIFT) &&
	   (::GetKeyState( VK_RSHIFT ) & VK_RSHIFT))
   {
	   pMFrame->PostMessage(WM_COMMAND,ID_TRASMITIR,0);
	   return 0;
   }

   if ((::GetKeyState( VK_MENU ) & 0x8000) && nChar != VK_NEXT && nChar != VK_PRIOR && nChar != VK_UP && nChar != VK_DOWN)
       return 0;

   switch(nChar)
   {
   case VK_CANCEL:
       comando = 807;
       break;
   case VK_ESCAPE:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 998;
       else
		   comando = 806;
		   /*comando = 807;*/           
       break;          
   case VK_F1:
	   comando = 822;
       break;  
   case VK_F2:
       comando = 824;
       break;
   case VK_F3:
       comando = 825;
       break;
   case VK_F4:
       comando = 826;
       break;
   case VK_F5:
       comando = 827;
       break;
   case VK_F6:
       comando = 828;
       break;
   case VK_F7:
       comando = 829;
       break;
   case VK_F8:
       comando = 830;
       break;
   case VK_F9:
       comando = 831;
       break;
   case VK_F10:
       comando = 832;
       break;
   case VK_F11:
	   if (theApp.m_oldf11)
	   {
		   if ( (::GetKeyState( VK_SHIFT ) & 0x8000) )
				comando = 821;
		   else
				comando = 820;
	   }
	   else
	   {
		   if ( (::GetKeyState( VK_SHIFT ) & 0x8000) )
				comando = 820;
		   else
				comando = 821;
	   }
       break;   
   case VK_F12:
       comando = 823;
       break;
   case VK_DELETE:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 997;
       else
           comando = 819;       
       break;
   case VK_INSERT:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 996;
       else
           comando = 818;
       break;
   case VK_END:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 817;
       else
           comando = 816;
       break;
   case VK_HOME:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 813;
       else
           comando = 812;
       break;
   case VK_LEFT:           
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 995;
       else
           comando = 810;
       break;
   case VK_UP:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 994;
       else
           comando = 808;
       break;
   case VK_RIGHT:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 993;
       else
           comando = 811;
       break;       
   case VK_DOWN:           
       if ((::GetKeyState( VK_CONTROL ) & 0x8000) || (::GetKeyState( VK_MENU ) & 0x8000))
           comando = 992;
       else
           comando = 809;
       break;        
   case VK_PRIOR:
       if ((::GetKeyState( VK_MENU ) & 0x8000))
       {            
           //comando = ID_ANTERIORPANE;
		   comando = 991;
       } else if ((::GetKeyState( VK_CONTROL ) & 0x8000))
	   {
		   //comando = 991;
		 comando = ID_ANTERIORPANE;
	   }           
       else
           comando = 814;
       break;
   case VK_NEXT:
       if ((::GetKeyState( VK_MENU ) & 0x8000))
       {            
           //comando = ID_SIGUIENTEPANE;
		   comando = 990;
       }
       else if ((::GetKeyState( VK_CONTROL ) & 0x8000))
	   {           
		   //comando = 990;
		   comando = ID_SIGUIENTEPANE;
	   }
       else
           comando = 815;
       break;
   case VK_TAB:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
       {           
           if ( (::GetKeyState( VK_SHIFT ) & 0x8000) )
           {
               comando = 990;
           }
           else
           {
			   if (theApp.m_tabesreturn)
				   comando = 988;
			   else
				   comando = 989;
           }
       }
       else
       {
           if ( (::GetKeyState( VK_SHIFT ) & 0x8000) )
           {
               comando = 805;
           }
           else
           {
			   if (theApp.m_tabesreturn)
				   comando = 802;
			   else
				   comando = 804;
           }
       }
       break;
   case VK_RETURN:
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
           comando = 988;
       else
           comando = 802;
       break;
   default:
       break;
   }
   if (comando >= 823 && comando <= 832)
   {
       if ((::GetKeyState( VK_CONTROL ) & 0x8000) && (::GetKeyState( VK_SHIFT ) & 0x8000))
       {
           comando += 30;
       }
       else
       if ((::GetKeyState( VK_CONTROL ) & 0x8000))
       {
           if (comando == 826)
           {
              comando = 806;
           }
           else
           {
              comando += 10;
           }
       }
       else
       if ( (::GetKeyState( VK_SHIFT ) & 0x8000) )
       {
           comando += 20;
       }
   }
   if (!comando && nChar == 'Q' && (::GetKeyState( VK_CONTROL ) & 0x8000))
   {
       comando = 807;
   }
   if (!comando && nChar >= 'A' && nChar <= 'Z' && (::GetKeyState( VK_CONTROL ) & 0x8000))
   {
       comando = 962 + (nChar - 'A');
   }
   if (!comando && nChar >= '0' && nChar <= '9' && (::GetKeyState( VK_CONTROL ) & 0x8000))
   {
       comando = 952 + (nChar - '0');
   }

   return comando;
}

CWnd * CDsStdDlg::CreaTab(int sid_d,int lpi,int lpf)
{
    CDsStdCtrl *pctrl;

    int sid_puro = sid_d % 1000; // sid del def puro

    
    POSITION pos = m_ctrlst.GetStartPosition( );   
   
    int wid;
    while (pos)
    {
       m_ctrlst.GetNextAssoc( pos, wid, pctrl );
       if (pctrl->m_tipo == 999 && pctrl->m_cid.m_lcid == sid_puro)
       {           
           return pctrl->m_ctrl;
       }
    }

    pctrl = new CDsStdCtrl();

    // para que los tabs tambien crezcan decidir flag
    //...
    //

    CRect rhijoant = m_ult_hijo_mosaico;
    if (!m_ult_first && m_ult_grow)
    {
        int dif;
        if (m_ult_vertical)
        {
            dif = m_ult_hijo_mosaico.bottom - m_ult_hijo_mosaico.top;
            m_ult_hijo_mosaico.top = m_ult_hijo_mosaico.bottom;
            m_ult_hijo_mosaico.bottom = m_ult_hijo_mosaico.top + dif;
        }
        else
        {
            dif = m_ult_hijo_mosaico.right - m_ult_hijo_mosaico.left;
            m_ult_hijo_mosaico.left = m_ult_hijo_mosaico.right;
            m_ult_hijo_mosaico.right = m_ult_hijo_mosaico.left + dif;
        }
    }


    int iddd = sid_puro;
    if (pctrl->Create(999,lpi,lpf,(void *)&iddd,this))
    {        
        ((CDsTabCtrl *)pctrl->m_ctrl)->m_popcion = sid_d/1000;

        m_ult_first = FALSE;
        m_ult_grow  = FALSE;
        m_ctrlst[pctrl->m_ctrlid] = pctrl;
        pctrl->m_ctrl->GetWindowRect(m_ult_hijo_mosaico);
        ScreenToClient(m_ult_hijo_mosaico);
        m_ult_hijo_mosaico.left -= m_offx;
        m_ult_hijo_mosaico.top -= m_offy;

        Crece();
        return pctrl->m_ctrl;
    }
    else
    {
        m_ult_hijo_mosaico = rhijoant;
		if (!pctrl->m_muriendo)
			delete pctrl;        	
    }
    return NULL;
}

BOOL CDsStdDlg::Create(void *datos, CDsxView *pView, int& stdid)
{
   int *p = (int *)datos;
   int flag = 0;
   int referenciaid = 0;

   /*static int contabug = 0;*/


   DWORD estylo = WS_CHILD | WS_CLIPCHILDREN |WS_CLIPSIBLINGS | WS_TABSTOP;

   m_id        = *p++;
   m_sid       = *p++;
   m_sid_padre = *p++;
   m_lpi       = *p++;
   m_lpf       = *p++;
   
   /*
   if (m_id == 2 && m_sid == 0 && m_lpi == 848 && m_lpf == 1998)
   {
	   ASSERT(FALSE);
   }
   
   if (m_lpf == 3498)
   {
	   contabug++;
	   if (contabug == 12)
			contabug = 0;
   }
   */

   for (char *ipp = (char *)p;*ipp;ipp++)
     oem_a_ansi((unsigned char *)ipp);
   m_nombre    = (LPCSTR)p;
   {
	   char *pp = (char *)p;
	   pp += (strlen(pp)+1);
	   p = (int *)pp;
   }

   flag = *p;
   p++;
   referenciaid = *p;

   if (flag == -1)
	   flag = 0;


   CWnd *pPadre = (CWnd *)pView;

   int wid;
   if ( ((unsigned int)stdid & 0x80000000) || !((unsigned int)stdid & 0x40000000) )
   {
		wid = CDsStdDlg::NewHandle();		
   }
   else
   {
		wid = stdid;
   }  
   stdid = wid;
   m_pView = pView;
   m_pDoc  = m_pView->GetDocument();


   if (!(flag & 1) && m_id > 0 && referenciaid != -1)
   {
		CDsStdDlg *referencia;						
		if (!m_sid && !m_sid_padre)
			referencia = m_pView->LocalizaDlg(-1, m_id-1, referenciaid,-1,-1,TRUE);
		else
			referencia = m_pView->LocalizaDlg(-1, m_id, referenciaid,-1,-1,TRUE);
		if (!referencia)
		{
			referencia = m_pView->LocalizaDlg(-1,-1, referenciaid,-1,-1);
		}
		if (referencia)
		{
			pPadre = (CWnd *)referencia;
		}
   }

   if (referenciaid == -1)
	   referenciaid = 0;
  
   m_nivel = 0;

   CRect rect;     


   m_cLabelBk = m_pDoc->m_cLabelBk;
   m_cLabelFr = m_pDoc->m_cLabelFr; // el repintado por cambio de defecto instantaneo
                                    // lo omitimos de momento ...
   m_bLabelBk = new CBrush();
   m_bLabelBk->CreateSolidBrush(m_cLabelBk);

   m_wid = wid;

   int asigna_tab = -1;   
   int mosaico = 0;
   CRect rhijoant;   

   CDsStdDlg *DlgPadre = NULL;


   if (m_sid)
   {
       int padre_wid;
       m_pater = m_pView->BuscaStdDlg(m_id,0,padre_wid);
   }
   
   if (m_sid)
   {
       CDsStdDlg *padre;
       int padre_wid;
       padre = m_pView->BuscaStdDlg(m_id,m_sid_padre,padre_wid);
       if (padre)
       {
          m_nivel = padre->m_nivel + 1;
          DlgPadre = padre;
    
		  if (!m_nombre.Mid(0,1).Compare("}"))
		  {
			 m_noscroll = TRUE;
			 m_nombre = m_nombre.Mid(1);
		  }
		  if (!m_nombre.Mid(0,1).Compare("]"))
		  {
			 m_noframe = TRUE;
			 m_nombre = m_nombre.Mid(1);
		  }
		  
          if (!m_nombre.Compare("")) /* si no tiene nombre la pestaña no crear tab */
          {
              pPadre = (CWnd *)padre;
          }
          else          
          {
              if (!m_nombre.Mid(0,2).Compare("||") || !m_nombre.Mid(0,2).Compare("|-"))
              {            
				    if (!m_nombre.Mid(2,1).Compare("!"))
					{
						padre->m_ult_hijo_mosaico = padre->m_stdrect;
						padre->m_ult_first = TRUE;
					}
                    rhijoant = padre->m_ult_hijo_mosaico;
                    padre->m_ult_grow = TRUE;
                    if (!m_nombre.Mid(0,2).Compare("||"))
                        padre->m_ult_vertical = TRUE;
                    else
                        padre->m_ult_vertical = FALSE;

					if (m_nombre.GetLength() > 2 && !m_nombre.Mid(2,1).Compare("*"))
					{
						m_nombre.Delete(2);
						m_forceframe = TRUE;
					}
                    if (!padre->m_ult_first)
                    {
                        int dif;
						{
							if (m_nombre.GetLength() > 2)
							{
								CString sbhijo = m_nombre.Mid(2);
								int lsid = m_sid%1000+atoi(sbhijo)*1000;
								int lwid = -1;
								CDsStdDlg *lfamilia = m_pView->BuscaStdDlg(m_id,lsid,lwid);
								if (lfamilia)
								{
									padre->m_ult_hijo_mosaico = lfamilia->m_stdrect;
								}
							}
							if (padre->m_ult_vertical)
							{
								dif = padre->m_ult_hijo_mosaico.bottom - padre->m_ult_hijo_mosaico.top;
								padre->m_ult_hijo_mosaico.top = padre->m_ult_hijo_mosaico.bottom;
								padre->m_ult_hijo_mosaico.bottom = padre->m_ult_hijo_mosaico.top + dif;
							}
							else
							{
								dif = padre->m_ult_hijo_mosaico.right - padre->m_ult_hijo_mosaico.left;
								padre->m_ult_hijo_mosaico.left = padre->m_ult_hijo_mosaico.right;
								padre->m_ult_hijo_mosaico.right = padre->m_ult_hijo_mosaico.left + dif;
							}
						}
                    }
                    mosaico = 1;
                    pPadre = (CWnd *)padre;
              }
              else
              {
                  int upi,upf;
                  
                  if (CDsStdDlg::ComparaLPos(m_lpi,padre->m_lpi,TRUE) >= 0)
                      upi = m_lpi;
                  else
                      upi = padre->m_lpi;
                  if (CDsStdDlg::ComparaLPos(m_lpf,padre->m_lpf,FALSE) <= 0)
                      upf = m_lpf;
                  else
                      upf = padre->m_lpf;                  

                  pPadre = padre->CreaTab(m_sid,upi,upf); 
                  if (pPadre)
                  {
                      asigna_tab = m_sid/1000;
                      if (asigna_tab != ((CDsTabCtrl *)pPadre)->m_popcion)
                      {                    
                         CDsStdDlg *primero = ((CDsTabCtrl *)pPadre)->m_dlgs[0];
                         if (primero)
                         {
                             if (CDsStdDlg::ComparaLPos(m_lpi,primero->m_lpi,TRUE) > 0)
                                m_lpi = primero->m_lpi;
                             if (CDsStdDlg::ComparaLPos(m_lpf,primero->m_lpf,FALSE) < 0)
                                m_lpf = primero->m_lpf;
                         }
                      }
                  }
              }
          }
       }
   }

   m_ancho = (m_lpf % 100) - (m_lpi % 100) + 1;
   m_alto  = (m_lpf / 100) - (m_lpi / 100) + 1;
   
   pPadre->GetClientRect(rect);
   if (pPadre == pView)
   {
       rect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
   }
   

   if (asigna_tab > -1)
   {  
      // ((CDsTabCtrl *)pPadre)->AdjustRect( FALSE , rect ) ;
      // si es enesimo tab coger el tamaño del primero a menos que este sea mas grande
   }
   else
   {     
      if (mosaico)
      {          
          CDsStdDlg *padre = (CDsStdDlg *)pPadre;          
          int osx;
          int osy;

          if (rect.top == padre->m_ult_hijo_mosaico.top && rect.left == padre->m_ult_hijo_mosaico.left)
          {
			  // /* PRUEBA
              if ((m_lpi%100) >= (padre->m_lpi%100))
                rect.left = rect.left +  ( ((m_lpi % 100) - (padre->m_lpi % 100)) * padre->m_stdx)/10;
              if ((m_lpi/100) >= (padre->m_lpi/100))
                rect.top  = rect.top + ( ((m_lpi / 100) - (padre->m_lpi / 100)) * padre->m_stdy)/10;
			  // */
              osx = rect.Size().cx;
              osy = rect.Size().cy;
          }
          else
          {
              osx = rect.Size().cx;
              osy = rect.Size().cy;
              rect = padre->m_ult_hijo_mosaico;
          }

          if ((m_lpf%100) <= (padre->m_lpf%100))
             rect.right = rect.left + (padre->m_stdx * m_ancho)/10;
          else
             rect.right = rect.left + osx;
             
          if ((m_lpf/100) <= (padre->m_lpf/100))
             rect.bottom = rect.top + (padre->m_stdy * m_alto)/10;
          else
             rect.bottom = rect.top + osy;
          
          padre->m_ult_hijo_mosaico = rect;
		  if (m_forceframe)
		  {
			  padre->m_ult_hijo_mosaico.bottom += 10;
			  padre->m_ult_hijo.bottom += 10;
		  }

      }
      else if (m_sid)  
      {   
          CDsStdDlg *padre = NULL;
          /*
          if (m_sid_padre)
          {
              int wid_padre = -1;
              padre = m_pView->BuscaStdDlg(m_id,0,wid_padre);
          }
          else
          */
          {
              padre = (CDsStdDlg *)pPadre;
          }
          if (padre)
          {
			   // /* PRUEBA

              if ((m_lpi%100) >= (padre->m_lpi%100))
                rect.left = rect.left +  ( ((m_lpi % 100) - (padre->m_lpi % 100)) * padre->m_stdx)/10;
              if ((m_lpi/100) >= (padre->m_lpi/100))
                rect.top  = rect.top + ( ((m_lpi / 100) - (padre->m_lpi / 100)) * padre->m_stdy)/10;
              if ((m_lpf%100) <= (padre->m_lpf%100))
                rect.right = rect.left + (padre->m_stdx * m_ancho)/10;
              if ((m_lpf/100) <= (padre->m_lpf/100))
                rect.bottom = rect.top + (padre->m_stdy * m_alto)/10;

			   // */
          }
      }
	  else
	  {
		  if (!m_sid && rect.left == 0 && rect.top == 0)
		  {
			  if ((m_lpi/100) > 5)
			  {
				  rect.top += (CDsStdDlg::DsStdOffY + (CDsStdDlg::DsStdY * ((m_lpi/100) - 5))/10);
			  }
			  if ((m_lpi%100) > 1)
			  {
				  rect.left += (CDsStdDlg::DsStdOffX + (CDsStdDlg::DsStdX * ((m_lpi%100) - 1))/10);
			  }
		  }
	  }
   }

   m_stdrect = rect;
   m_inistdrect = rect;
   m_creation_rect = rect;

   if (!m_noframe)
   {
	   if (!m_sid)
	   {
		   if (theApp.m_concaption)
		   {
			   estylo |= WS_THICKFRAME;
			   estylo |= WS_CAPTION;
		   }
		   else
		   {
			   estylo |= theApp.m_dlgframe;
		   }       
		   m_pater = this;
	   }
	   else
	   {   
           if (m_forceframe)
		   {
			   estylo |= theApp.m_dlgframe;
		   }
		   else if (asigna_tab < 0 && (m_sid_padre > 0 || m_falso || (!mosaico && DlgPadre && DlgPadre->m_ult_sid != -1)))
		   {
			   estylo |= theApp.m_dlgframe;
		   }
	   }
   }
   else
   {
	   if (!m_sid)
	   {
			m_pater = this;
	   }
   }

   DWORD ExStyle = 0;

   if (flag > 1)
   {
	   m_modoespecial = TRUE;
	   estylo = WS_CHILD | WS_CLIPCHILDREN |WS_CLIPSIBLINGS | WS_TABSTOP;
   }
   if ((flag & 2))
   {
	   estylo |= WS_DLGFRAME;
   }
   if ((flag & 4))
   {
	   estylo |= WS_THICKFRAME;
   }
   if ((flag & 8))
   {
	   estylo |= WS_BORDER;
   }
   if ((flag & 16))
   {	   
	   m_parchear = TRUE;
	   estylo |= (WS_CAPTION | WS_OVERLAPPED);
	   estylo |= WS_THICKFRAME;
   }
   if ((flag & 32))
   {
	   ExStyle |= WS_EX_CLIENTEDGE;
   }  
   if ((flag & 64))
   {
	   ExStyle |= WS_EX_TOOLWINDOW;
   }  
   if ((flag & 128))
   {
	   ExStyle |= WS_EX_STATICEDGE;
   }
   if ((flag & 256))
   {
	   ExStyle |= WS_EX_WINDOWEDGE;
   }  
   if ((flag & 512))
   {
	   ExStyle |= WS_EX_TRANSPARENT;
   }  
   if ((flag & 1024))
   {
	   ExStyle |= WS_EX_DLGMODALFRAME;
   }   


   AjustaSegunEstilo(rect,estylo);   

   /*
   if (m_sid && m_sid_padre > 0)
   {
       ExStyle |= WS_EX_TOOLWINDOW;
       estylo |= WS_CAPTION;
   }
   */
   BOOL r = STDDLGBASE::CreateEx(ExStyle,AfxRegisterWndClass(NULL),m_nombre,estylo,rect,pPadre,IDW_STDDLG+wid);
   if (r)
   {       
       ::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)m_bLabelBk->m_hObject);

       if (mosaico)
       {
           CDsStdDlg *padre = (CDsStdDlg *)pPadre;           
           padre->m_ult_first = FALSE;
           padre->Crece();
       }
       
       GetClientRect(rect);

       if (m_sid)
       {
		   /*// PRUEBA
           m_offx  = CDsStdDlg::DsStdOffX;
           m_offy  = CDsStdDlg::DsMnStdOffY;
           m_stdx = CDsStdDlg::DsStdX;
           m_stdy = CDsStdDlg::DsMnStdY;
		   */
		   

           // /* COMENTADO POR PRUEBA AJUSTE HIJO
           m_offx  = CDsStdDlg::DsMnStdOffX;
           m_offy  = CDsStdDlg::DsMnStdOffY;
		   
           if (DlgPadre)
           {			   
               m_stdx = DlgPadre->m_stdx/* - 1*/;
               m_stdy = DlgPadre->m_stdy - 20;
               if (m_stdx < CDsStdDlg::DsMnStdX)
                  m_stdx = CDsStdDlg::DsMnStdX;
               if (m_stdy < CDsStdDlg::DsMnStdY)
                  m_stdy = CDsStdDlg::DsMnStdY;			  
			  
           }
           else		   
           {
               m_stdx = CDsStdDlg::DsMnStdX;
               m_stdy = CDsStdDlg::DsMnStdY;
           }   
		   // */
           

           /*
           m_stdx = ((rect.Size().cx)*10) / m_ancho;
           m_stdy = ((rect.Size().cy)*10) / m_alto;       

           m_offx = (rect.Size().cx - ((m_ancho * m_stdx) / 10))/2;
           if (m_offx < 3)
              m_offx = 3;       
           m_offy = (rect.Size().cy - ((m_alto * m_stdy) / 10))/2;
           if (m_offy < 3)
              m_offy = 3;
           */
       }
       else
       {
           m_offx  = CDsStdDlg::DsStdOffX;
           m_offy  = CDsStdDlg::DsStdOffY;
           m_stdx = CDsStdDlg::DsStdX;
           m_stdy = CDsStdDlg::DsStdY;          
       }

       if (mosaico)
       {
           // prueba ... ajustar a 0 siempre que no este en los limites superiores
           m_offy = 0;
       }       

       /*
       if (!m_sid)
       {
           m_stdx = ((rect.Size().cx-m_offx*2)*10) / m_ancho;
           m_stdy = ((rect.Size().cy-m_offy*2)*10) / m_alto;           
       }
       else
       */
       {
           rect.right = rect.left +  m_offx * 2 + (m_stdx * m_ancho)/10 /*- 1*/;
           rect.bottom = rect.top +  m_offy * 2 + (m_stdy * m_alto)/10 - 1;

           if (m_sid && asigna_tab < 0 && !mosaico)
           {
               CRect otro = m_creation_rect;
               otro.right = otro.left + rect.Size().cx;
               otro.bottom = otro.top + rect.Size().cy;
               AjustaSegunEstilo(otro,estylo);
               MoveWindow(otro);
           }
       }


         m_ult_hijo_mosaico = rect;
         m_ult_hijo_mosaico.right = m_ult_hijo_mosaico.left +  m_offx + (m_stdx * m_ancho)/10;
         m_ult_hijo_mosaico.bottom = m_ult_hijo_mosaico.top +  m_offy + (m_stdy * m_alto)/10;
         m_ult_hijo = m_ult_hijo_mosaico;

         m_stdrect = rect;
         m_inistdrect = rect;       
       
       if (pPadre)
       {
           if (!m_sid)
           {
               AjustaSegunEstilo(rect,GetStyle());
               ClientToScreen(rect);
               pPadre->ScreenToClient(rect);
			   if (pPadre == m_pView)
			   {        
					CRect tmp1;
					int x = 0;
					int y = 0;
					
					
					if ((flag & 1) && m_id > 0)
					{
						CDsStdDlg *referencia;						
						referencia = m_pView->LocalizaDlg(-1, m_id, referenciaid,-1,-1,TRUE);
						if (!referencia)
						{
							referencia = m_pView->LocalizaDlg(-1,-1, referenciaid,-1,-1);
						}
						if (referencia)
						{
							CRect r1;

							referencia->GetWindowRect(r1);
							pPadre->ScreenToClient(r1);
							x = r1.left + (((m_lpi%100)-1)*referencia->m_stdx)/10;
						    y = r1.top + (((m_lpi/100)-5)*referencia->m_stdy)/10;
							m_referencia = referencia;
						}
					}
					else										
					{
						pPadre->GetWindowRect(tmp1);
						x = (tmp1.Size().cx - rect.Size().cx) / 2;
						y = (tmp1.Size().cy - rect.Size().cy) / 2;
					}
					if (x < 0) x = 0;
					if (y < 0) y = 0;
					rect.MoveToXY(x,y);
			   }
               MoveWindow(rect);
               m_pView->SetScrollDs();
           }
           else
           {
			   if (!m_parchear && pPadre != m_pView)
			   {
				   if (pPadre->IsKindOf(RUNTIME_CLASS(CDsStdDlg)))
				   {
						m_parchear = ((CDsStdDlg *)pPadre)->m_parchear;
				   }
			   }

               AjustaScroll();
           }
       }
       

       m_maparaya = (char *)::malloc( m_ancho * m_alto + 1);
	   m_maparaya_size = m_ancho * m_alto + 1;
       ::memset(m_maparaya,0,m_ancho * m_alto);

       if (asigna_tab > -1)
       {
           ((CDsTabCtrl *)pPadre)->OpcionTab(asigna_tab,this);
		   
		   /* OJO!! */
		   /*
		   ((CDsTabCtrl *)pPadre)->AdjustRect( FALSE , rect ) ;
		   if (rect.top != m_stdrect.top)
		   {
			   int d = m_stdrect.top - rect.top;
			   CRect r3;
			   ((CDsTabCtrl *)pPadre)->GetWindowRect(r3);
			   ((CDsTabCtrl *)pPadre)->GetParent()->ScreenToClient(r3);
			   r3.bottom += d;
			   ((CDsTabCtrl *)pPadre)->MoveWindow(r3);
			   ((CDsTabCtrl *)pPadre)->m_dsctrl->m_pdlg->Crece();			   
			   rect = m_stdrect;
			   if (((CDsTabCtrl *)pPadre)->m_dsctrl->m_pdlg->m_padre)
				   ((CDsTabCtrl *)pPadre)->m_dsctrl->m_pdlg->m_padre->Crece();			   
		   }
		   */
		   
       }
       pView->m_dlglist[wid] = this;
       
       if (DlgPadre)
       {
           if (mosaico && m_sid)
           {
               DlgPadre->m_ult_hijo_mosaico = m_stdrect;
               ClientToScreen(rect);
               DlgPadre->ScreenToClient(rect);
               MoveWindow(rect);
           }
           DlgPadre->CompruebaPushV(this);
           m_padre_ult_hijo = DlgPadre->m_ult_hijo_mosaico;           
           m_padre_ult_vertical = DlgPadre->m_ult_vertical;
           m_padre_ult_grow = DlgPadre->m_ult_grow;
           m_padre_ult_first = DlgPadre->m_ult_first;

           DlgPadre->m_ult_sid = m_sid;
       }

       EnableToolTips(TRUE);

       m_padre = DlgPadre;
   }
   else
   {
       if (mosaico)
       {
           ((CDsStdDlg *)pPadre)->m_ult_hijo_mosaico = rhijoant;
       }
	   if ( ((unsigned int)wid & 0x80000000) || !((unsigned int)wid & 0x40000000) )
	   {
			CDsStdDlg::m_freehandles[wid] = NULL;
	   }       
       m_wid = -1;
   }
   return r;
}

int CDsStdDlg::ControlUsuario(char *nom, int pi, int pf, void *dato,int pre_id)
{
    char data[512];
    int *p1 = (int *)data;
    int *p0 = (int *)dato;
	Cid cid;
	int tipo9 = 1;
	char *pnom;

	if (theApp.m_9wnd == this)
	{
		tipo9 = 1000;
	}

	int i;
    for (i = 0;nom[i] && nom[i] != ',';i++);
    if (nom[i] == ',') i++;
    
    if (p0[0])
    {
        p1[0] = p0[1];
    }
    else
    {
        p1[0] = 0;
    }
	p1[1] = p0[2];
	p1[2] = p0[3];

	pnom = (char *)(p1+3);
    strcpy(pnom,nom+i);
    
    int ctrlid = -1;
	try {
		if (!memcmp(nom,"TREECTRL",8))
		{
			ctrlid = InsertaControl(15*tipo9, pi, pf,(void *)data,' ',-1,-1,NULL,pre_id);
		}
		else if (!memcmp(nom,"EVENTOPAN",9))
		{
			m_evento = p0[1];
			return 0;
		}
		else if (!memcmp(nom,"IMAGEN,",7))
		{
			CDsStdCtrl *pctrl = NULL;
			cid.m_lcid = pi*1000+pf;
			cid.m_lref = m_sid;
			cid.m_tipo = 14;
			ctrlid = BuscaControlPorCid(14,cid,&pctrl);
			if (ctrlid > -1)
			{
				CDsImagen *w1 = (CDsImagen *)pctrl->m_ctrl;
				w1->LoadImage((char *)(p1+3));			
			}
			else
			{			
				ctrlid = InsertaControl(14*tipo9, pi, pf,(void *)data,' ',-1,-1,NULL,pre_id);
			}				
		}
		else if (!memcmp(nom,"LABEL,",6))
		{
			CDsStdCtrl *pctrl = NULL;
			cid.m_lcid = pi;
			cid.m_lref = m_sid;
			cid.m_tipo = 1;
			ctrlid = BuscaControlPorCid(1,cid,&pctrl);
			if (ctrlid > -1)
			{
				DestruyeControl(ctrlid);
			}
			int Atrib = 32;
			{			
				if (pnom[0] == '{' && pnom[1] == '{')
				{
					char *p = pnom+2;
					Atrib = atoi(p);
					Atrib |= 128;
					Atrib <<= 8;
					while(*p) 
					{
						if (*p == '}' && *(p+1) == '}')
							break;
						p++;
					}
					if (*p)
					{
						p += 2;
					}
					strcpy(pnom,p);
				}
			}
			ctrlid = InsertaControl(1*tipo9, pi, pf,(void *)pnom,Atrib,-1,-1,NULL,pre_id);
		}
		else if (!memcmp(nom,"LBOTON,",7))
		{
			ctrlid = InsertaControl(13*tipo9, pi, pf,(void *)data,' ',-1,-1,NULL,pre_id);
		}
		else
		{
			ctrlid = InsertaControl(3*tipo9, pi, pf,(void *)data,' ',-1,-1,NULL,pre_id);
		}

		CDsStdCtrl *pctrl;
		if (m_ctrlst.Lookup(ctrlid,pctrl))
		{
			if (!p0[0])
			{
				pctrl->m_evento = p0[1];
			}
		}
	} catch(...)
	{
	}
    p0[0] = ctrlid;
    return ctrlid;
}

int CDsStdDlg::InsertaControl(int tipo, int lpos, int lpf, void *dato,int Atrib,int Cc, int Cf,CDsStdCtrl **pdctrl,int pre_id)
{

    if (m_falso && lpos > 0 && lpf > 0)
    {
        BOOL tocado = FALSE;
        if ((lpos/100) < (m_lpi/100))
        {
           m_lpi = (lpos/100)*100 + m_lpi%100;
           tocado = TRUE;
        }
        if ((lpos%100) < (m_lpi%100))
        {
           m_lpi = (m_lpi/100)*100 + lpos%100;
           tocado = TRUE;
        }
        if ((lpf/100) > (m_lpf/100))
        {
           m_lpf = (lpf/100)*100 + m_lpf%100;
           tocado = TRUE;
        }
        if ((lpf%100) > (m_lpf%100))
        {
           m_lpf = (m_lpf/100)*100 + lpf%100;
           tocado = TRUE;
        }
        if (tocado && m_padre)
        {
            m_ancho = (m_lpf % 100) - (m_lpi % 100) + 1;
            m_alto  = (m_lpf / 100) - (m_lpi / 100) + 1;

            m_maparaya = (char *)::realloc( (void *)m_maparaya, m_ancho * m_alto + 1);
			m_maparaya_size = m_ancho * m_alto + 1;
           ::memset(m_maparaya,0,m_ancho * m_alto);

            CRect rect;
            CDsStdDlg *padre = m_padre;

            padre->GetClientRect(rect);

            if ((m_lpi%100) >= (padre->m_lpi%100))
               rect.left = rect.left +  ( ((m_lpi % 100) - (padre->m_lpi % 100)) * m_stdx)/10;
            if ((m_lpi/100) >= (padre->m_lpi/100))
               rect.top  = rect.top + ( ((m_lpi / 100) - (padre->m_lpi / 100)) * m_stdy)/10;
            if ((m_lpf%100) <= (padre->m_lpf%100))
               rect.right = rect.left + (m_stdx * m_ancho)/10;
            if ((m_lpf/100) <= (padre->m_lpf/100))
               rect.bottom = rect.top + (m_stdy * m_alto)/10;

            m_stdrect = rect;
            m_inistdrect = rect;
            m_creation_rect = rect;

            AjustaSegunEstilo(rect,GetStyle());            

            MoveWindow(rect);
        }
    }


    if (tipo == 0) // rayas se tratan desde la ventana 'dialogo' directamente
    {
       int tamx = ((lpf%100) - (lpos%100)) + 1;
       int tamy = ((lpf/100) - (lpos/100)) + 1;
       int maxx = ((tamx < m_ancho) ? tamx : m_ancho);
       int maxy = ((tamy < m_alto) ? tamy : m_alto);
       char *p = (char *)dato;
       
       int mpx = (lpos%100) - (m_lpi%100);
       int mpy = (lpos/100) - (m_lpi/100);

       int o_mpx = 0;
       int o_mpy = 0;

       if (m_bLabelBk)
          delete m_bLabelBk;
	   m_bLabelBk = NULL;
	   if ( (Atrib & 0x8000) )
	   {
		   int idx;
		   idx = (Atrib >> 8) & ~128;
		   if (idx >= 0 && idx < 128)
		   {
			   COLORREF cb,cf;
			   cf = (COLORREF)Cc;
			   cb = (COLORREF)Cf;
               m_cLabelBk = cb;
               m_cLabelFr = cf;
		   }
	   }
       else switch((Atrib&~128))
       {
           case 'R':
             m_cLabelBk = m_pDoc->m_cLabelRBk;
             m_cLabelFr = m_pDoc->m_cLabelRFr;
             break;
           case 'I':
             m_cLabelBk = m_pDoc->m_cLabelIBk;
             m_cLabelFr = m_pDoc->m_cLabelIFr;
             break;             
       }
	   m_bLabelBk = new CBrush();
	   m_bLabelBk->CreateSolidBrush(m_cLabelBk);   

	   //::SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)m_bLabelBk->m_hObject);
       
       if (mpx < 0)
       {
           o_mpx = -mpx;
           mpx = 0;
       }
       if (mpy < 0)
       {
           o_mpy = -mpy;
           mpy = 0;
       }

	   int pl;
       for (int j = 0;j < maxy;j++)
       {
		   int i;
           for (i = 0;i < maxx;i++)
           {
			   pl = (mpy+j)* m_ancho + mpx + i; 
			   if (pl < m_maparaya_size)
				   m_maparaya[ pl ] = p[ (j+o_mpy) * tamx + i + o_mpx];
           }
       }
       if (::IsWindow(m_hWnd))
          InvalidateRect(NULL);
       return m_wid;
    }
	if (tipo == 1)
	{
		int lp1;
        lp1 = lpos - m_lpi;

	   Cid cid;
       cid.m_lref = m_sid;
       cid.m_tipo = tipo;
       cid.m_lcid = lp1;
       
       {
		   CDsStdCtrl *pctrl = NULL;
           int otro = BuscaControlPorCid(tipo,cid,&pctrl);
           if (otro > -1 && pctrl)
           {
			   BOOL es_raya = TRUE;
			   char * ipp;
			   for (ipp = (char *)dato;*ipp;ipp++)
			   {
				   if (*ipp < 0 || *ipp > 12)
				   {
					   es_raya = FALSE;
					   break;
				   }
			   }
			   if (!es_raya && pctrl->m_lpi == lpos && pctrl->m_lpf == lpf)
			   {
				   if (((CStaticLabel *)pctrl->m_ctrl)->Atributo(Atrib,Cc,Cf))
				   {
					   ((CStaticLabel *)pctrl->m_ctrl)->PonFont();
				   }
				   for (ipp = (char *)dato;*ipp;ipp++)
				   {
					   oem_a_ansi((unsigned char *)ipp);
				   }
				   ((CStaticLabel *)pctrl->m_ctrl)->SetWindowText((LPCSTR)dato);				   
				   return pctrl->m_ctrlid;
			   }
			   else
			   {
				   DestruyeControl(otro);
			   }
           }
       }
	}


    CDsStdCtrl *pctrl;

    pctrl = new CDsStdCtrl();
	if (tipo > 999)
	{
		tipo /= 1000;
		pctrl->m_esde9 = 1;
	}
    if (pctrl->Create(tipo,lpos,lpf,dato,this,Atrib,Cc,Cf,pre_id))
    {
        m_ctrlst[pctrl->m_ctrlid] = pctrl;
        //if (pctrl->m_tipo == 1 || pctrl->m_tipo == 2)
        {
            m_cidctrllst[CidKey(pctrl->m_cid)] = pctrl;
        }
		if (pdctrl)
		{
			*pdctrl = pctrl;
		}
        return pctrl->m_ctrlid;
    }
    else
    {
		if (!pctrl->m_muriendo)
		{
			delete pctrl;        
		}
    }

    return -1;
}


void CDsStdDlg::PintaRayas(CPaintDC &dc,char *maparaya,COLORREF cbk,int alto,int ancho,int stdx,int stdy, int offx,int offy,CRect &ult_hijo,CPoint &orig,CBrush *bfondo,int msize)
{
    char raya;
    int px;
    int mpx = (stdx/2)/10;
    int py;
    int mpy = (stdy/2)/10;
    int fpx = stdx/10;
    int fpy = stdy/10;   
    CRect rr;	

    if ( ((cbk & 0x00FF0000) > 0x003F0000) )
    {
        cbk -= 0x00400000;
    }                        
    if ( ((cbk & 0x0000FF00) > 0x00003F00) )
    {
        cbk -= 0x00004000;
    }
    if ( ((cbk & 0x000000FF) > 0x0000003F) )
    {
        cbk -= 0x00000040;
    }

    //RGB(128,128,128)
    CPen pl(PS_SOLID,1,RGB(255,255,255)),ph(PS_SOLID,1,cbk);    
    CPen *p_old;

    CRect rect(dc.m_ps.rcPaint);


	int pll;
    for (int j = 0;j < alto;j++)
    {
        py = offy + (stdy * j)/10;
        fpy = (offy + (stdy * (j+1))/10) - py;
        py += (orig.y + ult_hijo.top);

		int i;
        for (i = 0;i < ancho;i++)
        {
            px = offx + (stdx * i)/10;
            fpx = (offx + (stdx * (i+1))/10) - px;
            px += (orig.x + ult_hijo.left);

            
            if (  ( (py >= (rect.top-20) && py <= (rect.bottom+20)) || 
                    ((py+fpy) >= (rect.top-20) && (py+fpy) <= (rect.bottom+20)) ) &&
                  ( (px >= (rect.left-20) && px <= (rect.right+20)) || 
                  ((px+fpx) >= (rect.left-20) && (px+fpx) <= (rect.right+20)) ) 
               )            
            {
				pll = j*ancho+i;
				if (pll >= msize)
					continue;

                raya = maparaya[pll];
                if (raya && bfondo)
                {
                    rr.SetRect(px,py,px+fpx,py+fpy);
                    dc.FillRect(rr,bfondo);
                }
                switch(raya)
                {
                case 1:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+fpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+fpy);
                    dc.SelectObject(p_old);
                    break;
                case 2:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                    dc.SelectObject(p_old);
                    break;
                case 3:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py+mpy);
                    dc.LineTo(px+mpx,py+fpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py+mpy+1);
                    dc.LineTo(px+mpx+1,py+fpy);
                
                    dc.MoveTo(px+mpx+2,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx+1,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 4:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                
                    dc.MoveTo(px+mpx+1,py+mpy+2);
                    dc.LineTo(px+mpx+1,py+fpy);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py+mpy+2);
                    dc.LineTo(px+mpx,py+fpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 5:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+mpx+1,py+mpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+mpx,py+mpy+1);
                
                    dc.MoveTo(px+mpx+1,py+mpy);
                    dc.LineTo(px+mpx+1,py+fpy);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py+mpy);
                    dc.LineTo(px+mpx,py+fpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 6:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+fpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+fpy);
                
                    dc.MoveTo(px+mpx+2,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx+2,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 7:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+fpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+fpy);
                
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 8:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+fpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+fpy);
                
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+mpx,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+mpx,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 9:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+mpy+1);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+mpy);
                
                    dc.MoveTo(px+mpx,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx+1,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 10:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+mpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+mpy);
                
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+fpx,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+fpx,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                case 11:
                    p_old = (CPen*)dc.SelectObject(&ph);
                    dc.MoveTo(px+mpx,py);
                    dc.LineTo(px+mpx,py+mpy);
                    dc.SelectObject(&pl);
                    dc.MoveTo(px+mpx+1,py);
                    dc.LineTo(px+mpx+1,py+mpy+1);
                
                    dc.MoveTo(px,py+mpy+1);
                    dc.LineTo(px+mpx+2,py+mpy+1);
                    dc.SelectObject(&ph);
                    dc.MoveTo(px,py+mpy);
                    dc.LineTo(px+mpx+1,py+mpy);
                
                    dc.SelectObject(p_old);
                    break;
                default:
                    break;
                }
            }
        }
    }
}


void CDsStdDlg::OnPaint()
{    

    CPoint orig;

    orig.x = -GetScrollPos(SB_HORZ);
    orig.y = -GetScrollPos(SB_VERT);

	CPaintDC dc(this); // device context for painting
    
    PintaRayas(dc,m_maparaya,m_cLabelBk,m_alto,m_ancho,m_stdx,m_stdy,m_offx,m_offy,m_ult_hijo,orig,NULL,m_maparaya_size);
}

BOOL CDsStdDlg::Crece(BOOL solorecalcula)
{
    CRect ant,nuevo,hijo;
    BOOL rr = FALSE;
	if (solorecalcula)
	{
		if (!m_sid)
		{
			GetClientRect(ant);
			GetClientRect(nuevo);
			m_ancho = (m_lpf % 100) - (m_lpi % 100) + 1;
			m_alto  = (m_lpf / 100) - (m_lpi / 100) + 1;
			nuevo.right = nuevo.left +  m_offx * 2 + (m_stdx * m_ancho)/10 - 1;
			nuevo.bottom = nuevo.top +  m_offy * 2 + (m_stdy * m_alto)/10 - 1;		
		}
		else
			return FALSE;
	}
	else
	{
		if (m_sid)
		{
			ant = m_stdrect;
		}
		else
		{
			GetClientRect(ant);
			ant.bottom -= ((m_stdy*19)/10) - ((m_stdy*18)/10);
		}
		hijo = m_ult_hijo_mosaico;
    
		nuevo.UnionRect(ant,hijo);
	}
    if (nuevo.Size().cy > ant.Size().cy || nuevo.Size().cx > ant.Size().cx)
    {
        m_ult_hijo.right  = nuevo.right;
        m_ult_hijo.bottom = nuevo.bottom;
        m_ult_hijo.top    = nuevo.bottom - ((m_alto * m_stdy)/10);
        m_ult_hijo.left   = nuevo.right  - ((m_ancho * m_stdx)/10);

        if (m_sid)
        {
            m_stdrect = nuevo;
        }
        else
        {
            CRect r;
            GetWindowRect(r);
            int x = 0,y = 0;
            int cx = r.Size().cx + (nuevo.Size().cx - ant.Size().cx);
            int cy = r.Size().cy + (nuevo.Size().cy - ant.Size().cy);
            CWnd *parent = GetParent();
            if (parent && ::IsWindow(parent->m_hWnd))
            {
                parent->ScreenToClient(r);
                x = r.left;
                y = r.top;
            }

            m_ult_hijo.top  +=  m_stdy/10;
            m_ult_hijo.left +=  m_stdx/10;

            MoveWindow(x,y,cx,cy);        
        }
        rr = TRUE;
    }
    if (rr)
    {
        if (m_sid)
        {
            AjustaScroll();
        }
        else
        {
            if (m_pView)
                m_pView->SetScrollDs();
        }
    }
    return rr;
}

int CDsStdDlg::ControlEdicion(int cwid,Cid cid,int flag,void *datos,int pre_id)
{
   CDsStdCtrl *pctrl = NULL;
   if (cwid != -1)
   {
       if (CDsStdDlg::m_todosctrl.Lookup(cwid,pctrl))
       {
           int *p1 = (int *)datos;

           if (p1[0] != 11 && pctrl->m_tipo != 2 && pctrl->m_tipo != 7)
           {
               //ASSERT(FALSE);
               cwid = InsertaControl(p1[0],p1[1],p1[2],(void *)(p1+6),p1[3],p1[4],p1[5],NULL,pre_id);
		       if ((flag & 1))
			   {      
				   if (CDsStdDlg::m_todosctrl.Lookup(cwid,pctrl))
				   {
					   ((CDsCombo *)pctrl->m_ctrl)->SetFlag((flag>>1));
				   }
			   }
			   return cwid;
           }
           else
           {
               if (m_id == pctrl->m_pdlg->m_id && m_sid == pctrl->m_pdlg->m_sid  && (p1[0] != 11 || CDsStdDlg::ComparaCid(pctrl->m_cid,cid,FALSE))) /* confirmar la validez */
               {
                    return pctrl->ControlEdicion(flag,datos,pre_id);
               }
           }
       }
   }

   int *p1 = (int *)datos;

   if (p1[0] == 11) 
   {
       if (pctrl)
            return pctrl->ControlEdicion(flag,datos,pre_id);
       ASSERT(FALSE);
       return -1;
   }
   
   
   POSITION pos = m_ctrlst.GetStartPosition( );      
   int wid;
   while (pos)
   {
       m_ctrlst.GetNextAssoc( pos, wid, pctrl );
       if ((pctrl->m_tipo == 2 || pctrl->m_tipo == 7) && m_id == pctrl->m_pdlg->m_id && CDsStdDlg::ComparaCid(pctrl->m_cid,cid))
       {
           return pctrl->ControlEdicion(flag,datos,pre_id);
       }
   }

   return InsertaControl(p1[0],p1[1],p1[2],(void *)(p1+6),p1[3],p1[4],p1[5],NULL,pre_id);
}

int CDsStdDlg::BuscaControlPorCid(int tipo,Cid cid,CDsStdCtrl **rctrl)
{
   CDsStdCtrl *pctrl;
   void *hp;

   if (/*tipo == 1 || tipo == 2*/ 1)
   {
        if (m_cidctrllst.Lookup(CidKey(cid),hp) && (pctrl = (CDsStdCtrl *)hp) && pctrl->m_pdlg == this && pctrl->m_tipo == tipo && CDsStdDlg::ComparaCid(pctrl->m_cid,cid))
        {
			if (rctrl)
			{
				*rctrl = pctrl;
			}
            return pctrl->m_ctrlid;
        }
   }
   else
   {
       POSITION pos = m_ctrlst.GetStartPosition( );      
       int wid;
       while (pos)
       {
           m_ctrlst.GetNextAssoc( pos, wid, pctrl );
           if (pctrl->m_tipo == tipo && CDsStdDlg::ComparaCid(pctrl->m_cid,cid) && pctrl->m_pdlg == this)
           {   
				if (rctrl)
				{
					*rctrl = pctrl;
				}			   
               return wid;
           }
       }
   }
	if (rctrl)
	{
		*rctrl = NULL;
	}
   return -1;
}

BOOL CDsStdDlg::DestruyeControl(int cwid,BOOL Siempre)
{
    CDsStdCtrl *pctrl;

    if (m_ctrlst.Lookup(cwid,pctrl) && pctrl->m_pdlg == this)
    {
		if (pctrl->m_protegido)
		{
			return FALSE;
		}

		if (pctrl->m_nivel)
		{
			return FALSE;
		}

        if (!Siempre && m_pater->EstaEnPushv(pctrl))
        {
            return FALSE;
        }

        //if (pctrl->m_tipo == 1 || pctrl->m_tipo == 2)
        {
            m_cidctrllst.RemoveKey(CidKey(pctrl->m_cid));
        }
        
		if (!pctrl->m_muriendo)
		{
			delete pctrl;		
			/*
			CDsStdDlg::m_freehandles[cwid] = NULL;
			m_ctrlst.RemoveKey(cwid);        
			CDsStdDlg::m_todosctrl.RemoveKey(cwid);
			*/
		}		
		{
			
			void *ppx;
			CDsStdCtrl *pp2;
			if ( ((unsigned int)cwid & 0x80000000) || !((unsigned int)cwid & 0x40000000) )
			{
				if (CDsStdDlg::m_freehandles.Lookup(cwid,ppx))
				{
					CDsStdDlg::m_freehandles[cwid] = NULL;
				}
			}
			if (m_ctrlst.Lookup(cwid,pp2))
			{
				m_ctrlst.RemoveKey(cwid);
			}
			if (CDsStdDlg::m_todosctrl.Lookup(cwid,pp2))
			{
				CDsStdDlg::m_todosctrl.RemoveKey(cwid);
			}

			//theApp.m_teclashortcuts // no... que cada uno se encargue 
			
		}

        return TRUE;
    }    
    else
    {
        ASSERT(FALSE);
    }

    return FALSE;
}

int PopV(int nulo);
int PushV(int pi,int pf);    
#define MAX_PUSHV 500
CDsPushV *m_pushv[MAX_PUSHV];
int m_npush;


int CDsStdDlg::PushV(int pi, int pf)
{
    m_pushv[m_npush] = new CDsPushV(pi,pf);
    m_npush++;
    return m_npush;
}

int CDsStdDlg::PopV(int nulo,int *dest)
{

    if (m_npush)
    {
        m_npush--;
        if (m_pushv[m_npush])
        {
            if (nulo)
            {
                m_pushv[m_npush]->Anulacion();
            }
            m_pushv[m_npush]->ListaSids(dest);
            delete m_pushv[m_npush];
            m_pushv[m_npush] = NULL;

            if (!nulo)
                ReajustaRect();
        }
    }
    return m_npush;
}

int CDsStdDlg::BorraDescendencia(int *lista,int pi, int pf)
{

   if (pi > 0 && pf > 0)
   {
       char datos[512];
       int *p1 = (int *)datos;
       CDsStdDlg *pdlg = new CDsStdDlg();
       pdlg->m_falso = TRUE;
       int stdid = -1;
       memset(datos,0,sizeof(datos));
       p1[0] = m_id+m_replicante;
       p1[1] = pi*10000+pf;
       p1[2] = m_sid;
       p1[3] = pi;
       p1[4] = pf;
       if (!pdlg->Create((void *)datos,m_pView,stdid))
       {
          delete pdlg;
       }
       else
       {
           pdlg->ShowWindow(SW_SHOW);
           ::SetWindowPos(pdlg->m_hWnd,(HWND)HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);           
           ::SetWindowPos(pdlg->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
       }
       return 0;
   }

   if (m_npush) // Falta incluir elementos existentes y ocultarlos para en el popv reactivarlos
   {
       lista[0] = -1;
       return 0;
   }

   POSITION pos = m_listahijos.GetStartPosition( );
   int wid;
   int n = 0;
   CDsStdDlg *pdlg;
   int o_sid;

   if (pi < -1)
   {
       pi = -(pi);
   }

   if (pi ==  -1 && pf == -1)
   {
       while (pos)
       {
           m_listahijos.GetNextAssoc( pos, wid, pdlg );

           if (pdlg->m_id != (m_id+m_replicante)) continue;

           n += pdlg->BorraDescendencia(lista+n);
           if (!pdlg->m_falso)
           {
               lista[n] = pdlg->m_sid;
               n++;
           }

           o_sid = pdlg->m_sid;

       
           CDsxView *pview = pdlg->m_pView;
           /*
           pview->EliminaStdDlg(pdlg->m_wid);
           */       
           /*pview->m_dlglist.RemoveKey(wid);*/

           pdlg->DestroyWindow();
           delete pdlg;       

           if (!o_sid)
           {
             pview->SetScrollDs();
           }
       }
   }

   CDsStdCtrl *pctrl;
   pos = m_ctrlst.GetStartPosition( );
   while(pos)
   {
       m_ctrlst.GetNextAssoc( pos, wid, pctrl );

       if (pctrl->m_p_id != (m_id+m_replicante)) continue;

       if (pi != -1 && pf != -1)
       {
           if (pctrl->m_lpi < pi || pctrl->m_lpf > pf)
           {
               continue;
           }
       }

       if (pctrl->m_pdlg == this && pctrl->m_tipo == 999)
       {
           DestruyeControl(wid);
       }
   }

   m_ult_sid = -1;

   lista[n] = -1;
   return n;
}

int CDsStdDlg::InsertaPushv(CDsStdDlg *pdlg)
{
    int n;
	int i;
    for (i = m_npush-1;i >= 0;i--)
    {
        if (m_pushv[i])
        {
            n = m_pushv[i]->InsertaDlg(pdlg);
            if (n > -1) return n;
        }
    }
    return -1;
}

BOOL CDsStdDlg::EstaEnPushv(CDsStdCtrl *pctrl)
{
    int cwid = pctrl->m_ctrlid;
    CDsStdCtrl *opctrl;
	int i;
    for (i = m_npush-1;i >= 0;i--)
    {
        if (m_pushv[i])
        {
            if (m_pushv[i]->m_push_ctrllst.Lookup(cwid,opctrl) && opctrl)
                return TRUE;
        }
    }
    return FALSE;
}

int CDsStdDlg::InsertaPushv(CDsStdCtrl *pctrl)
{
	int i;
    for (i = m_npush-1;i >= 0;i--)
    {
        if (m_pushv[i])
        {
           if (!m_pushv[i]->EsInsertableCtrl(pctrl))
           {
               return -1;
           }
        }
    }    

    int n;
    for (i = m_npush-1;i >= 0;i--)
    {
        if (m_pushv[i])
        {
            n = m_pushv[i]->InsertaCtrl(pctrl);
            if (n > -1) return n;
        }
    }    
    return -1;
}


int CDsStdDlg::CompruebaPushV(CDsStdDlg *pdlg)
{    

    m_listahijos[pdlg->m_wid] = pdlg;

	if (!m_pater) return 0;
    return m_pater->InsertaPushv(pdlg);
}


int CDsStdDlg::CompruebaPushV(CDsStdCtrl *pctrl)
{    
    if (!m_pater) return 0;
	return m_pater->InsertaPushv(pctrl);    
}


BOOL CDsStdDlg::ReajustaRect()
{
   CRect rr,rr1;
   CRect ult_hijo;
   BOOL  ult_vertical;
   BOOL  ult_grow;
   BOOL  ult_first;   

   GetClientRect(rr1);

   if (rr1.Size().cx <= m_creation_rect.Size().cx && rr1.Size().cy <= m_creation_rect.Size().cy)
   {
       return FALSE;
   }

   POSITION pos = m_listahijos.GetStartPosition( );
   int wid;
   CDsStdDlg *pdlg;

   if (!pos)
   {
       ult_vertical = TRUE;
       ult_grow = FALSE;
       ult_first = TRUE;
       ult_hijo.SetRectEmpty();       
       if (m_sid)
       {
            m_stdrect = m_inistdrect;
            m_ult_hijo_mosaico = ult_hijo;
            m_ult_grow = ult_grow;
            m_ult_vertical = ult_vertical;
            m_ult_first = ult_first;
            AjustaScroll();
            return TRUE;
       }
   }
   while (pos)
   {
       m_listahijos.GetNextAssoc( pos, wid, pdlg );
       pdlg->GetWindowRect(&rr);
       ScreenToClient(rr);
       if (rr.left < rr1.left)
       {
         rr1.left = rr.left;
       }
       if (rr.right < rr1.right)
       {
         rr1.right = rr.right;
       }
       if (rr.top < rr1.top)
       {
         rr1.top = rr.top;
       }
       if (rr.bottom > rr1.bottom)
       {
         rr1.bottom = rr.bottom;
       }       
       ult_hijo = pdlg->m_padre_ult_hijo;
       ult_vertical = pdlg->m_padre_ult_vertical;
       ult_grow = pdlg->m_padre_ult_grow;
       ult_first = pdlg->m_padre_ult_first;
   }   

   if (rr1.Size().cx < m_creation_rect.Size().cx)
   {
       rr1.left = m_creation_rect.left;
       rr1.right = m_creation_rect.right;
   }
   if (rr1.Size().cy < m_creation_rect.Size().cy)
   {
       rr1.top = m_creation_rect.top;
       rr1.bottom = m_creation_rect.bottom;
   }

   AjustaSegunEstilo(rr1,GetStyle());
   

   GetWindowRect(rr);
   if (rr.Size().cx > rr1.Size().cx || rr.Size().cy > rr1.Size().cy)
   {
       m_ult_hijo_mosaico = ult_hijo;
       m_ult_grow = ult_grow;
       m_ult_vertical = ult_vertical;
       m_ult_first = ult_first;

       if (m_sid)
       {
            m_stdrect = m_ult_hijo_mosaico;
            m_stdrect.bottom--;
            m_stdrect.right--;
            AjustaScroll();
       }
       else
       {
            ::SetWindowPos(m_hWnd,NULL,0,0,rr1.Size().cx,rr1.Size().cy,SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOMOVE);
            m_pView->SetScrollDs();
       }       
   }

   return TRUE;
}

BOOL CDsStdDlg::AjustaScroll()
{
	SetScrollPos(SB_HORZ, 0);
	SetScrollPos(SB_VERT, 0);
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	if (!m_noscroll)
	{
		SCROLLINFO info;
		info.fMask = SIF_PAGE|SIF_RANGE;
		info.nMin = 0;

		CRect r;
		GetClientRect(r);
		if (r.Size().cx < (m_stdrect.Size().cx - (m_stdx/30)))
		{
			int rr = m_stdrect.Size().cx;
			int p = r.Size().cx;
			if (r.Size().cy < (m_stdrect.Size().cy- (m_stdy/30)))
			{
				p -= ::GetSystemMetrics(SM_CYVSCROLL);
			}
			EnableScrollBarCtrl(SB_HORZ, TRUE);
			info.nPage = p;
			info.nMax = rr-1;
			if (!SetScrollInfo(SB_HORZ, &info, TRUE))
				SetScrollRange(SB_HORZ, 0, rr , TRUE);
		}

		if (r.Size().cy < (m_stdrect.Size().cy - (m_stdy/30)))
		{
			int rr = m_stdrect.Size().cy;
			int p = r.Size().cy;
			if (r.Size().cx < (m_stdrect.Size().cx- (m_stdx/30)))
			{
				p -= ::GetSystemMetrics(SM_CXHSCROLL);
			}
			EnableScrollBarCtrl(SB_VERT, TRUE);
			info.nPage = p;
			info.nMax = rr-1;
			if (!SetScrollInfo(SB_VERT, &info, TRUE))
				SetScrollRange(SB_VERT, 0, rr , TRUE);
		}
	}

    return TRUE;
}


void CDsStdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;

	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
    {
		return;
    }
	
    OnScroll(MAKEWORD(nSBCode, -1), nPos);
	//STDDLGBASE::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDsStdDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;
	
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
    {
       return;
    }
	
    OnScroll(MAKEWORD(-1, nSBCode), nPos);
	//STDDLGBASE::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDsStdDlg::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    CRect r;
    GetClientRect(r);
	
    CSize m_pageDev,m_lineDev;
	m_pageDev.cx = r.Size().cx; //m_totalDev.cx / 10;
    m_pageDev.cy = r.Size().cy; //m_totalDev.cy / 10;
	m_lineDev.cx = m_stdx/10;
	m_lineDev.cy = m_stdy/10;


	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.cx;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.cy;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
		UpdateWindow();

	return bResult;
}

BOOL CDsStdDlg::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		sizeScroll.cx = 0;
	}

	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
        CRect r;
        GetClientRect(r);
		ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
        {
			SetScrollPos(SB_HORZ, x);
            if ( (-(x-xOrig)) < 0)
            {
                r.left = r.right - (x-xOrig) - 1;
            }
            else
            {
                r.right = r.left + (x-xOrig) + 1;
            }
        }
		if (y != yOrig)
        {
			SetScrollPos(SB_VERT, y);
            if ( (-(y-yOrig)) < 0)
            {
                r.top = r.bottom - (y-yOrig) - 1;
            }
            else
            {
                r.bottom = r.top + (y-yOrig) + 1;
            }
        }
        InvalidateRect(r);
        UpdateWindow();
    }
	return TRUE;
}

BOOL CDsStdDlg::RefrescaControl(CDsStdCtrl *pctrl)
{
    if (pctrl->m_pdlg == this && pctrl->m_ctrl && ::IsWindow(pctrl->m_ctrl->m_hWnd))
    {
        if (pctrl->m_tipo == 4) // caso GRID
        {
            CTablaW *ptablaw = (CTablaW *)pctrl->m_ctrl;
            return ptablaw->m_tabla.Refresca();
        }
		else
		{
			pctrl->m_ctrl->InvalidateRect(NULL);
		}
    }
    return FALSE;
}

BOOL CDsStdDlg::ScrollCtrls(int pi, int pf, int n)
{   
   CRect rect,/*ri,*/r1,r2,r3;
   
   int nl = n % 100;
   int nc = n / 100;


   int lpi = pi;
   int lpf = pf;   
   
   lpi -= m_lpi;
   lpf -= m_lpi;   

   rect.top = m_ult_hijo.top + m_offy + (m_stdy * (lpi/100))/10;       
   rect.left = m_ult_hijo.left + m_offx + (m_stdx * (lpi%100))/10;

   rect.right = m_ult_hijo.left + m_offx + (m_stdx * (lpf%100))/10;
   rect.bottom = m_ult_hijo.top + m_offy + (m_stdy * (lpf/100))/10;   
   
   CWnd *wnds[500];
   int nwnds = 0;

   /*
   int nx,ny;
   ny = 0;
   ri = rect;
   if (nl > 0)
   {
       ny = m_ult_hijo.top + m_offy + (m_stdy * (lpi/100+nl))/10;
       ny -= rect.top;       
       ri.top = ri.bottom - ny;
   }
   else
   if (nl < 0)
   {
       ny = m_ult_hijo.top + m_offy + (m_stdy * (lpf/100+nl))/10;
       ny -= rect.bottom;       
       ri.bottom = ri.top - ny;
   }
   nx = 0;
   if (nc > 0)
   {
       nx = m_ult_hijo.left + m_offx + (m_stdx * (lpi%100+nc))/10;            
       nx -= rect.left;
       ri.left = ri.right - nx;
   }
   else
   if (nc < 0)
   {
       nx = m_ult_hijo.left + m_offx + (m_stdx * (lpf%100+nc))/10;
       nx -= rect.right;
       ri.right = ri.left - nx;
   } 
   */

   
   int i;

   /*
   if (nl > 0)
   {
       
   }
   m_maparaya
   */

   SetRedraw(FALSE);
   
   CDsStdCtrl *pctrl;
   int wid;
   int otrop = 0;
   POSITION pos = m_ctrlst.GetStartPosition( );   

   while (pos)
   {
       m_ctrlst.GetNextAssoc( pos, wid, pctrl );
       /* de momento el scroll afecta a estaticos y campos 'estaticos' */
       if (/*pctrl->m_tipo == 1 || (pctrl->m_tipo == 2 && pctrl->m_cid.m_lcid & 0xffff0000)*/
		    1 || (pctrl->m_tipo != 2 || !(pctrl->m_cid.m_lcid & 0xffff0000)) )
       {
           if (CDsStdDlg::ComparaLPos(pctrl->m_lpi,pi,TRUE) >= 0 && CDsStdDlg::ComparaLPos(pctrl->m_lpf,pf,FALSE) <= 0)
           {
               if (pctrl->m_tipo == 2)
               {
                  otrop = (pctrl->m_cid.m_lcid & 0xffff0000) >> 16;
               }
               r1 = pctrl->MarcoControl();
               if (nl)
               {                   
                   pctrl->m_lpi += (nl*100);
                   pctrl->m_lpf += (nl*100);
                   otrop += (nl*100);
               }
               if (nc)
               {
                   pctrl->m_lpi += nc;
                   pctrl->m_lpf += nc;
                   otrop += nc;
               }

               if (pctrl->m_tipo == 2)
               {                   
                   CDsStdCtrl *opctrl;
                   void *hp;
                   if (m_cidctrllst.Lookup(CidKey(pctrl->m_cid),hp) && (opctrl =  (CDsStdCtrl *)hp) && opctrl == pctrl)
                   {
                       m_cidctrllst.RemoveKey(CidKey(pctrl->m_cid));
                   }
                   pctrl->m_cid.m_lcid = (pctrl->m_cid.m_lcid & 0xffff) | (otrop << 16);                   
                   m_cidctrllst[CidKey(pctrl->m_cid)] = pctrl;
                   ((CDsCombo *)pctrl->m_ctrl)->SetReadOnly( TRUE );
                   ((CDsCombo *)pctrl->m_ctrl)->Setestilo(((CDsCombo *)pctrl->m_ctrl)->Getestilo() | ES_READONLY);
               }

               if (CDsStdDlg::ComparaLPos(pctrl->m_lpi,pi,TRUE) < 0 || CDsStdDlg::ComparaLPos(pctrl->m_lpf,pf,FALSE) > 0)
               {
                   DestruyeControl(wid);
               }
               else
               {                   
                   r2 = pctrl->MarcoControl();
                   pctrl->m_ctrl->GetWindowRect(r3);
                   pctrl->m_ctrl->GetParent()->ScreenToClient(r3);
                   r3.OffsetRect(CSize(r2.left-r1.left,r2.top-r1.top));                   
                   pctrl->m_ctrl->MoveWindow(r3,FALSE);
                   wnds[nwnds] = pctrl->m_ctrl;
                   nwnds++;
               }
           }
       }
   }   


   SetRedraw(TRUE);
   
   for (i = 0;i < nwnds;i++)
   {
       wnds[i]->RedrawWindow();
   }
   RedrawWindow(rect);
   
   return TRUE;
}

BOOL CDsStdDlg::TeclaStd(int tecla, int wcid, int ctipo, Cid cid)
{
  return m_pView->TeclaStd(tecla,m_wid,m_replicante+m_id,m_sid,wcid,ctipo,cid);
}


static void hexa(char *buf,unsigned int num,int tam)
{
  static char _hexa[17] = "0123456789ABCDEF";
  buf[tam] = 0;
  while(tam)
  {
      tam--;
      buf[tam] = _hexa[ (num & 0xf) ];
      num >>= 4;
  }
}

LPCSTR CDsStdDlg::CidKey(Cid cid)
{    
   hexa(m_cidkeybuf,(unsigned int)cid.m_tipo,3);
   hexa(m_cidkeybuf+3,(unsigned int)cid.m_lref,8);
   hexa(m_cidkeybuf+11,(unsigned int)cid.m_lcid,8);

   return (LPCSTR)m_cidkeybuf;
}


BOOL CDsStdDlg::EnsureVisible(CRect *r)
{
   if (m_padre && !m_padre->m_sid && m_padre->m_pView && !::IsWindowVisible(m_padre->m_hWnd))
   {
       m_padre->m_pView->ActivaDlg(m_padre->m_wid, m_padre->m_id, m_padre->m_sid,TRUE);
   }


    if (m_sid_padre && m_padre)
    {
        m_padre->EnsureVisible(r);
    }

    if (m_padretab)
    {
        ::SetWindowPos(m_padretab->m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
        m_padretab->SetDlgActivo(this);
    }
    else
    {
        ::SetWindowPos(m_hWnd,(HWND)HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    }

    CSize s(0,0);
    CRect rect = r;
    CRect c;

    GetClientRect(c);    
    ScreenToClient(rect);
    if (rect.top < c.top)
    {
        s.cy = rect.top - c.top;
    }
    if (rect.bottom > c.bottom)
    {
        s.cy = rect.bottom - c.bottom;
    }
    if (rect.left < c.left)
    {
        s.cx = rect.left - c.left;
    }
    if (rect.right > c.right)
    {
        s.cx = rect.right - c.right;
    }
    if (s.cx || s.cy)
    {
        OnScrollBy(s, TRUE);
    }    

    return m_pView->EnsureVisible(r);
}


BOOL CDsStdDlg::DisparaEventoUsuario(int operacion,LPCSTR contenido)
{
	//theApp.m_noreenfocarpordefecto++;
    if (m_evento != -1)
    {
        long tam;
        long respuesta;
        long modo = 1;        
        char *p;
        char tmp[512];   

		//m_usado++;

        p = tmp;

        memcpy((void *)p,(void *)&m_evento,sizeof(int));
        p += sizeof(int);

        tam = (sizeof(long) * 7) + (sizeof(int)*2) + 2;
		if (contenido)
			tam += strlen(contenido);
        respuesta = tam;

        memcpy((void *)p,(void *)&tam,sizeof(long));
        p += sizeof(long);	
        memcpy((void *)p,(void *)&respuesta,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&modo,sizeof(long));
        p += sizeof(long);
        memcpy((void *)p,(void *)&operacion,sizeof(int));
        p += sizeof(int);
		if (contenido)
			strcpy(p,contenido); // no debe de exceder de 249 chars
		else
			*p = 0;

		if (IsKindOf(RUNTIME_CLASS(CDs9Window))) // modo 9
		{
			pDsSys->DespachadorInterno9(13,(void *)tmp);
			PostMessage(WM_COMMAND,(WPARAM)BUSCA_PTECS,(LPARAM)0);
			SetFocus();
			m_pView->TeclaStd(TECLA_PING,m_wid,m_replicante+m_id,m_sid,-1,-1,NULO_CID);			
		}
		else
		{
			int wid = m_wid;
			int id = m_id;
			int sid = m_sid;
			int wcid = -1;
			int ctipo = -1;
			int replicante = m_replicante;
			Cid cid = NULO_CID;
			CDsxView *pview = m_pView;

			pDsSys->DespachadorInterno(13,(void *)tmp);

			pview->TeclaStd(TECLA_PING,wid,replicante+id,sid,wcid,ctipo,cid);
			//TeclaStd(TECLA_PING);
		}
		

		//m_usado--;
		
		//theApp.m_noreenfocarpordefecto--;
        return TRUE;
    }
	//theApp.m_noreenfocarpordefecto--;
    return FALSE;
}


CDsPushV::CDsPushV(int pi,int pf)
{
    m_pi = pi;    
    m_pf = pf;
    m_locked = FALSE;
	//m_focus = NULL;
	m_focus = ::GetFocus();
}

CDsPushV::~CDsPushV()
{

    m_locked = TRUE;

    POSITION pos = m_push_ctrllst.GetStartPosition( );
    int wid;
    CDsStdCtrl *pctrl;
    while (pos)
    {
       m_push_ctrllst.GetNextAssoc( pos, wid, pctrl );
	   if (CDsStdDlg::m_todosctrl.Lookup(wid,pctrl))
	   {
		   pctrl->m_pdlg->DestruyeControl(wid,TRUE);
	   }
    }

    pos = m_push_dlglist.GetStartPosition( );
    wid;
    CDsStdDlg *pdlg;
    while (pos)
    {
       m_push_dlglist.GetNextAssoc( pos, wid, pdlg );
       CDsxView *pview = pdlg->m_pView;
       if (pview)
       {
            pview->EliminaStdDlg(pdlg->m_wid);
       }
    }
	if (m_focus)
	{
		::SetFocus(m_focus);
	}
}

int CDsPushV::Anulacion()
{
    m_push_dlglist.RemoveAll( );
    return 0;
}

BOOL CDsPushV::EsInsertableCtrl(CDsStdCtrl *pctrl)
{
    if (pctrl->m_tipo == 2 && !(pctrl->m_cid.m_lcid&0xffff0000)) return FALSE;

    CDsStdDlg *pdlg;
    if (!m_push_dlglist.Lookup(pctrl->m_pdlg->m_wid,pdlg) || !pdlg)
    {
        return TRUE;
    }    

    return FALSE;
}

int CDsPushV::InsertaCtrl(CDsStdCtrl *pctrl)
{
    if (CDsStdDlg::ComparaLPos(m_pi,pctrl->m_lpi,TRUE) <= 0 && CDsStdDlg::ComparaLPos(m_pf,pctrl->m_lpf,FALSE) >= 0)
    {
        int cwid = pctrl->m_ctrlid;
        m_push_ctrllst[cwid] = pctrl;
        return cwid;
    }
    return -1;
}

int CDsPushV::InsertaDlg(CDsStdDlg *pdlg)
{
    if (CDsStdDlg::ComparaLPos(m_pi,pdlg->m_lpi,TRUE) <= 0 && CDsStdDlg::ComparaLPos(m_pf,pdlg->m_lpf,FALSE) >= 0)
    {
        int wid = pdlg->m_wid;
        m_push_dlglist[wid] = pdlg;
        return wid;
    }
    return -1;
}

int CDsPushV::ListaSids(int *lista)
{
    int n = 0;
    POSITION pos = m_push_dlglist.GetStartPosition( );
    int wid;
    CDsStdDlg *pdlg;
    while (pos)
    {
       m_push_dlglist.GetNextAssoc( pos, wid, pdlg );
       if (!pdlg->m_falso)
       {
           lista[n] = (pdlg->m_sid & 0xffff) | ((pdlg->m_id << 16) & 0xffff0000) | 0x40000000;
           n++;
       }
    }
    lista[n] = -1;

    return n;
}



void CDsStdDlg::AjustaSegunEstilo(CRect &rect, DWORD stylo)
{
   if (m_falso)
   {
       if ( (stylo & WS_THICKFRAME) )
       {      
           if ((stylo & WS_CAPTION))
           {
               rect.top -= ::GetSystemMetrics(SM_CYCAPTION);
           }

           rect.bottom += ::GetSystemMetrics(SM_CYSIZEFRAME);
           rect.right  += ::GetSystemMetrics(SM_CXSIZEFRAME);
           rect.top -= ::GetSystemMetrics(SM_CYSIZEFRAME);
           rect.left  -= ::GetSystemMetrics(SM_CXSIZEFRAME);
       }
       else
       if ( (stylo & WS_DLGFRAME) )
       {
           rect.bottom += ::GetSystemMetrics(SM_CYDLGFRAME);
           rect.right  += ::GetSystemMetrics(SM_CXDLGFRAME);
           rect.top -= ::GetSystemMetrics(SM_CYDLGFRAME);
           rect.left  -= ::GetSystemMetrics(SM_CXDLGFRAME);
       }
       if ( (stylo & WS_BORDER) )
       {
           rect.bottom += ::GetSystemMetrics(SM_CYBORDER);
           rect.right  += ::GetSystemMetrics(SM_CXBORDER);
           rect.top -= ::GetSystemMetrics(SM_CYBORDER);
           rect.left  -= ::GetSystemMetrics(SM_CXBORDER);
       }
   }
   else
   {
       if ( (stylo & WS_THICKFRAME) )
       {       
           if ((stylo & WS_CAPTION))
           {
               rect.bottom += ::GetSystemMetrics(SM_CYCAPTION);
           }
           rect.bottom += (::GetSystemMetrics(SM_CYSIZEFRAME)*2)+1;
           rect.right  += (::GetSystemMetrics(SM_CXSIZEFRAME)*2)+1;
       }
       else
       if ( (stylo & WS_DLGFRAME) )
       {
           rect.bottom += (::GetSystemMetrics(SM_CYDLGFRAME)*2)+1;
           rect.right  += (::GetSystemMetrics(SM_CXDLGFRAME)*2)+1;
       }
       
       if ( (stylo & WS_BORDER) )
       {
           rect.bottom += (::GetSystemMetrics(SM_CYBORDER)*2);
           rect.right  += (::GetSystemMetrics(SM_CXBORDER)*2);
       }
   }
}


BOOL CDsStdDlg::CambiaPane(int cambio,BOOL sube)
{
    if (m_padretab)
    {
        return m_padretab->CambiaDeOpcion(cambio,TRUE);
    }

    POSITION pos = m_ctrlst.GetStartPosition( );   
    CDsStdCtrl *phijo;
    int wid;
    while (pos)
    {
       m_ctrlst.GetNextAssoc( pos, wid, phijo );       
       if (phijo->m_tipo == 999)
       {
           return ((CDsTabCtrl *)phijo->m_ctrl)->CambiaDeOpcion(cambio,TRUE);
       }
    }

    if (!sube)
    {
       POSITION pos = m_listahijos.GetStartPosition( );
       int wid;       
       CDsStdDlg *pdlg;
       while (pos)
       {
           m_listahijos.GetNextAssoc( pos, wid, pdlg );
           if (pdlg->CambiaPane(cambio,FALSE)) return TRUE;
       }
    }

    if (m_padre)
    {
        return m_padre->CambiaPane(cambio,TRUE);
    }

    return FALSE;
}

BOOL CDsStdDlg::SacameDelPushv(CDsStdDlg *pdlg)
{
    if (m_pater && m_pater != this)
    {
        return m_pater->SacameDelPushv(pdlg);
    }    
    CDsStdDlg *xpdlg;
    int wid;
	int i;
    for (i = m_npush-1;i >= 0;i--)
    {
        if (m_pushv[i] && !m_pushv[i]->m_locked)
        {           
            xpdlg = NULL;
            wid = pdlg->m_wid;
            if (m_pushv[i]->m_push_dlglist.Lookup(wid,xpdlg) && xpdlg == pdlg)
            {
                m_pushv[i]->m_push_dlglist.RemoveKey(wid);
                return TRUE;            
            }
        }
    }
    return FALSE;
}

int CDsStdDlg::ControlDialogo(int modo,char *datos)
{
      {
        int prev_run = theApp.m_runmode;
        int *p1 = (int *)datos;

        CDsStdCtrl *pctrl = NULL;
        if (p1[5] > -1 && CDsStdDlg::m_todosctrl.Lookup(p1[5],pctrl) && pctrl)
        {
            pctrl->m_ctrl->SetFocus();
        }
        
        p1[5] = 806;		
        int boton = InsertaControl(3, p1[3],p1[4], (void *)(p1+5));

		p1[5] = boton;
      }

      return 0;
}

int CDsStdDlg::OpcionesVentana(char *data)
{	
	if (!strcmp(data,"SINBASICO"))
	{
		SetParent(NULL);
		pMFrame->ShowWindow(SW_HIDE);
	}
	else if (!strcmp(data,"CONBASICO"))
	{
		pMFrame->ShowWindow(SW_SHOW);
		SetParent(m_padre);		
	}
	else if (!strcmp(data,"SHOW"))
		ShowWindow(SW_SHOW);
	else if (!strcmp(data,"HIDE"))
		ShowWindow(SW_HIDE);
	else if (!strcmp(data,"ENABLE"))
		EnableWindow(TRUE);
	else if (!strcmp(data,"DISABLE"))
		EnableWindow(FALSE);
	else if (!strcmp(data,"MODAL"))
	{
		EnableWindow(TRUE);

		POSITION pos;
		int wid;
		CDsStdDlg *pdlgb;
		pos = m_pView->m_dlglist.GetStartPosition();
		while(pos)
		{
			pdlgb = NULL;
			m_pView->m_dlglist.GetNextAssoc( pos, wid, pdlgb );
			if (pdlgb && pdlgb != this)
			{
				if (!IsChild(pdlgb) && !pdlgb->IsChild(this))
					pdlgb->EnableWindow(FALSE);
			}
		}
	}
	else if (!strcmp(data,"MODELESS"))
	{
		EnableWindow(TRUE);

		POSITION pos;		
		int wid;
		CDsStdDlg *pdlgb;
		pos = m_pView->m_dlglist.GetStartPosition();
		while(pos)
		{
			pdlgb = NULL;
			m_pView->m_dlglist.GetNextAssoc( pos, wid, pdlgb );
			if (pdlgb && pdlgb != this)
			{
				pdlgb->EnableWindow(TRUE);
			}
		}
	}
	else if (!memcmp(data,"FLAG:",5))
	{
		/*
   if (flag > 1)
   {
	   m_modoespecial = TRUE;
	   estylo = WS_CHILD | WS_CLIPCHILDREN |WS_CLIPSIBLINGS | WS_TABSTOP;
   }
   if ((flag & 2))
   {
	   estylo |= WS_DLGFRAME;
   }
   if ((flag & 4))
   {
	   estylo |= WS_THICKFRAME;
   }
   if ((flag & 8))
   {
	   estylo |= WS_BORDER;
   }
   if ((flag & 16))
   {
	   estylo |= WS_CAPTION;
	   estylo |= WS_THICKFRAME;
   }
   if ((flag & 32))
   {
	   ExStyle |= WS_EX_CLIENTEDGE;
   }  
   if ((flag & 64))
   {
	   ExStyle |= WS_EX_TOOLWINDOW;
   }  
   if ((flag & 128))
   {
	   ExStyle |= WS_EX_STATICEDGE;
   }
   if ((flag & 256))
   {
	   ExStyle |= WS_EX_WINDOWEDGE;
   }  
   if ((flag & 512))
   {
	   ExStyle |= WS_EX_TRANSPARENT;
   }  
   if ((flag & 1024))
   {
	   ExStyle |= WS_EX_DLGMODALFRAME;
   }   


   AjustaSegunEstilo(rect,estylo);   

		*/
	}
	return 0;
}

BOOL CDsStdCtrl::EnsureVisible()
{
    CRect r(0,0,0,0);    

	/*
    if (m_pdlg && m_pdlg->IsKindOf(RUNTIME_CLASS(CDs9Window))) 
    {
		return TRUE;
	}
	*/

    if (m_ctrl)
    {
        m_ctrl->GetWindowRect(r);
    }
    if (m_pdlg)
    {
        if (m_tipo == 4)
        {
            CTablaW *pw = (CTablaW *)m_ctrl;
            if (pw)
            {
               if ( !(pw->m_gmodo & 8) )
               {
                   pw->BringWindowToTop();
                   return TRUE;
               }
            }
        }
        return m_pdlg->EnsureVisible(&r);
    }
    return FALSE;
}

BOOL CDsStdDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return STDDLGBASE::OnCommand(wParam, lParam);
}

void CDsStdCtrl::RellenaToolTip(char *pp)
{	
    m_tooltiptext = "Esc o Ctrl-Q Salir\r\n";
    if ((m_helpflag & 1)) /* clave */
	{
		m_tooltiptext += "Flecha Arriba cambiar de clave\r\nFlecha Abajo cambiar de clave\r\nAv-Pag cambio clave\r\nRe-Pag cambio clave\r\nEnter continuar\r\nCtrl-Flecha Abajo desplegar grid\r\nF11 consultar\r\n";
	}
    if ((m_helpflag & 2)) /* Relacion */
	{
		if (!(m_helpflag & 1))
		{
			m_tooltiptext += "Ctrl-Flecha Abajo desplegar grid\r\n";
			m_tooltiptext += "F11 - consultar\r\n";
		}
		m_tooltiptext += "Shift-F11 consultar relacion\r\n";
	}
    if ((m_helpflag & 4)) /* Campo standard */
	{		
        m_tooltiptext += "Shift-Tab o Flecha Arriba campo anterior\r\n";
        m_tooltiptext += "Tab o Flecha Abajo campo siguiente\r\n";
        m_tooltiptext += "Av-Pag pantalla siguiente o validar\r\n";
        m_tooltiptext += "Re-Pag pantalla anterior\r\n";
	}
    if ((m_helpflag & 8)) /* Campo Linea */
	{
        m_tooltiptext += "Shift-Tab o Flecha Arriba linea anterior\r\n";
        m_tooltiptext += "Tab o Flecha Abajo linea siguiente\r\n";
        m_tooltiptext += "Av-Pag pantalla siguiente\r\n";
        m_tooltiptext += "Re-Pag pantalla anterior\r\n";
		m_tooltiptext += "Enter entrar datos de la linea\r\n";
	}
	if (pp)
	{
		m_tooltiptext += pp;
	}
}

char *CDsStdDlg::RecogeAyudaPorPosicion()
{
	if (!theApp.m_9wnd)
		return "";

	static char tmp[2256];
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);

    char *p;
    long tam;
    long respuesta;
    long modo = 1;
    int operacion;
	int localizador = 0;	
	int maxi = 256;
	int x,y;
    p = tmp;

	{
	   CRect rect;
	   RECT r;
	   r.left = 401;
	   r.right = 1;
	   r.top  = 2480;
	   r.bottom = 0;
	   pDsSys->DespachadorInterno9(7,(void *)&r);
	   rect = r;
	   x = (pt.x / (rect.Size().cx / 80)) + 1;
	   y = (pt.y / (rect.Size().cy / 21)) + 4;
	}

	operacion = 2;

    memcpy((void *)p,(void *)&localizador,sizeof(int));
    p += sizeof(int);

    tam = (sizeof(long) * 4) + maxi;
    respuesta = tam;

    memcpy((void *)p,(void *)&tam,sizeof(long));
    p += sizeof(long);	
    memcpy((void *)p,(void *)&respuesta,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&modo,sizeof(long));
    p += sizeof(long);
    memcpy((void *)p,(void *)&operacion,sizeof(int));
    p += sizeof(int);

	memcpy((void *)p,(void *)&x,sizeof(int));
	p += sizeof(int);
	memcpy((void *)p,(void *)&y,sizeof(int));
	p += sizeof(int);
	memcpy((void *)p,(void *)&maxi,sizeof(int));
	p += sizeof(int);
	strcpy(p,"");
    
	pDsSys->DespachadorInterno9(13,(void *)tmp);
	if (*p)
	{
		pDsSys->DespachadorInterno9(17,(void *)p);
	}
	return p;
}

BOOL CDsStdDlg::EraseBkgnd(CDC* pDC) 
{
	CRect  rect;
	GetClientRect(rect);
	
#ifdef DSPROFUIS		    

	if (theApp.m_fondosolido1)
	{
			pDC->FillSolidRect(
				&rect,
				m_cLabelBk);
			return TRUE;
	}

	if (theApp.m_aparienciabase == 3)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CExtPaintManagerSkin *)pMFrame->m_paintmanager)->PaintDockerBkgnd( true, *pDC, this , m_cLabelBk))		
			)
			pDC->FillSolidRect(
				&rect,
				m_cLabelBk
				);
	}
	else if (theApp.m_aparienciabase == 2)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CDsProfuisCustomOffice2007_R1 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, *pDC, this , m_cLabelBk))		
			)
			pDC->FillSolidRect(
				&rect,
				m_cLabelBk
				);
	}
	else if (theApp.m_aparienciabase == 1)
	{
		if(		(! g_PaintManager->GetCb2DbTransparentMode(this) )
			||	(!((CDsProfuisCustomOffice2003 *)pMFrame->m_paintmanager)->DSPaintDockerBkgnd( true, *pDC, this , m_cLabelBk))		
			)
			pDC->FillSolidRect(
				&rect,
				m_cLabelBk
				);
	}

    return TRUE;
#else   
	pDC->FillRect(rect,m_bLabelBk);
	return TRUE;
#endif
}

BOOL CDsStdDlg::OnEraseBkgnd(CDC* pDC) 
{
	return EraseBkgnd(pDC);
}

