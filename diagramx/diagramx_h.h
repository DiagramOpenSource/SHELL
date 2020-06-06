
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Fri Dec 20 14:31:14 2002
 */
/* Compiler settings for diagramx.odl:
    Os, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __diagramx_h_h__
#define __diagramx_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDiagramx_FWD_DEFINED__
#define __IDiagramx_FWD_DEFINED__
typedef interface IDiagramx IDiagramx;
#endif 	/* __IDiagramx_FWD_DEFINED__ */


#ifndef __Document_FWD_DEFINED__
#define __Document_FWD_DEFINED__

#ifdef __cplusplus
typedef class Document Document;
#else
typedef struct Document Document;
#endif /* __cplusplus */

#endif 	/* __Document_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __Diagramx_LIBRARY_DEFINED__
#define __Diagramx_LIBRARY_DEFINED__

/* library Diagramx */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Diagramx,0xF2E28EC9,0x6E55,0x4776,0x94,0x74,0x80,0x90,0xB9,0xA9,0xFC,0x33);

#ifndef __IDiagramx_DISPINTERFACE_DEFINED__
#define __IDiagramx_DISPINTERFACE_DEFINED__

/* dispinterface IDiagramx */
/* [uuid] */ 


DEFINE_GUID(DIID_IDiagramx,0x1A2764C8,0x3127,0x430E,0x95,0xF7,0xED,0x2A,0x54,0x5C,0x62,0xEE);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1A2764C8-3127-430E-95F7-ED2A545C62EE")
    IDiagramx : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDiagramxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDiagramx * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDiagramx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDiagramx * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDiagramx * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDiagramx * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDiagramx * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDiagramx * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDiagramxVtbl;

    interface IDiagramx
    {
        CONST_VTBL struct IDiagramxVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDiagramx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDiagramx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDiagramx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDiagramx_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDiagramx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDiagramx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDiagramx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDiagramx_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Document,0x712EA093,0x6417,0x4176,0xBB,0x69,0xD2,0xA7,0x06,0x81,0xA7,0xD4);

#ifdef __cplusplus

class DECLSPEC_UUID("712EA093-6417-4176-BB69-D2A70681A7D4")
Document;
#endif
#endif /* __Diagramx_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


