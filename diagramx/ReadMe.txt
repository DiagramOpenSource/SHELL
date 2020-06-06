
========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : diagramx
========================================================================


AppWizard has created this diagramx application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your diagramx application.

diagramx.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

diagramx.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CDsxApp application class.

diagramx.cpp
    This is the main application source file that contains the application
    class CDsxApp.

diagramx.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

diagramx.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\diagramx.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file diagramx.rc.

res\diagramx.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.

diagramx.reg
    This is an example .REG file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .REG
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.

diagramx.odl
    This file contains the Object Description Language source code for the
    type library of your application.


/////////////////////////////////////////////////////////////////////////////

For the main frame window:

DsxMainFrm.h, DsxMainFrm.cpp
    These files contain the frame class CDsxFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CDsxFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in diagramx.rc to add
    toolbar buttons.

res\IToolbar.bmp
	This bitmap file is used to create tiled images for the toolbar
	when your server application is in-place activated inside another
	container.  This toolbar is constructed in the CDsxInPlaceFrame
	class.  This bitmap is similar to the bitmap in res\Toolbar.bmp
	except that it has many non-server commands removed.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

DsxDoc.h, DsxDoc.cpp - the document
    These files contain your CDsxDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CDsxDoc::Serialize).

dsxView.h, dsxView.cpp - the view of the document
    These files contain your CDsxView class.
    CDsxView objects are used to view CDsxDoc objects.


/////////////////////////////////////////////////////////////////////////////

AppWizard has also created classes specific to OLE

DsxCntrItem.h, DsxCntrItem.cpp - this class is used to
	manipulate OLE objects.  OLE objects are usually displayed by your
	CDsxView class and serialized as part of your CDsxDoc class.

DsxSrvrItem.h, DsxSrvrItem.cpp - this class is used to
	connect your CDsxDoc class to the OLE system, and optionally
	provide links to your document.
DsxIpFrame.h, DsxIpFrame.cpp - this class is derived
	from COleIPFrameWnd and controls all frame features during
	in-place activation.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named diagramx.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////



<?xml-stylesheet type="text/xsl" href="simple.xsl" ?>

<?xml version='1.0' encoding='UTF-8'?>
<xsl:stylesheet version='2.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:fn='http://www.w3.org/2005/02/xpath-functions' xmlns:xdt='http://www.w3.org/2005/02/xpath-datatypes'>
    <xsl:output version='1.0' encoding='UTF-8' indent='no' omit-xml-declaration='no' media-type='text/html' />
    <xsl:template match='/'>
        <html>
            <head>
                <title />
            </head>
            <body>
                <h1>
                    <center>
                        <p>
                            <p align='center'>
                                <h5 align='right'>
                                    <xsl:for-each select='consulta__u_dsempres_dscomer9_def_agifa024'>
                                        <h5>Fecha</h5>
                                    </xsl:for-each>
                                </h5>
                                <p>
                                    <span style='font-family:Verdana; font-size:large; '>Título</span>
                                </p>
                                <center align='left'>
                                    <xsl:for-each select='consulta__u_dsempres_dscomer9_def_agifa024'>
                                        <br />
                                        <table border='1'>
                                            <thead>
                                                <tr>
                                                    <td style='background-color:#FFFFE5; '>
                                                        <span style='background-color:#FFFFE5; color:black; font-family:Verdana; font-weight:bold; '>Título1</span>
                                                    </td>
                                                </tr>
                                            </thead>
                                            <tfoot>
                                                <tr>
                                                    <td style='border-top-color:gray; border-top-style:solid; border-top-width:thin; '>
                                                        <span style='font-family:Verdana; font-weight:bold; '>Total1</span>
                                                    </td>
                                                </tr>
                                            </tfoot>
                                            <tbody>
                                                <xsl:for-each select='registro'>
                                                    <tr>
                                                        <td>
                                                            <xsl:for-each select='C001'>
                                                                <span style='font-family:Verdana; '>
                                                                    <xsl:apply-templates />
                                                                </span>
                                                            </xsl:for-each>
                                                        </td>
                                                    </tr>
                                                </xsl:for-each>
                                            </tbody>
                                        </table>
                                        <br />
                                    </xsl:for-each>
                                    <br />
                                </center>
                                <h5>
                                    <div align='right'>
                                        <span style='left:auto; position:relative; right:auto; '>- </span>
                                        <span style='left:auto; position:relative; right:auto; '> -</span>
                                    </div>
                                </h5>
                                <center>
                                    <br />
                                </center>
                            </p>
                        </p>
                    </center>
                </h1>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
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
static BOOL EnviaPPaquete(SOCKET sock,PPaquete *sendbuf)
{
    int bytessent,totalbytes;
    int i;

    totalbytes = LEN_CABECERA;

    bytessent = 0;			  
    while(bytessent < totalbytes)
    {
#ifndef WIN32
		errno = 0;
#endif
       i = send(sock,((char *)&(sendbuf->c)) + bytessent,totalbytes - bytessent,0);
       if (i == SOCKET_ERROR)
       {
#ifdef FGVDOS
          if (WSAGetLastError() != WSAEWOULDBLOCK)
          {
             
             return FALSE;
          }
#else
		  if (errno == EINTR)
			  continue;
          if (errno != EWOULDBLOCK)
          {
             return FALSE;
          }
#endif
          
          continue;
       }
       else
          bytessent += i;
    }

	bytessent  = 0;	
	totalbytes = LEN_DATA(sendbuf);
	
	/* tests */
	
	while(bytessent < totalbytes)
	{
#ifndef WIN32
		errno = 0;
#endif
	   i = send(sock,((char *)(sendbuf->buffer)) + bytessent,totalbytes - bytessent,0);
	   if (i == SOCKET_ERROR)
	   {
#ifdef FGVDOS
		  if (WSAGetLastError() != WSAEWOULDBLOCK)
		  {
         
			 return FALSE;
		  }
#else
		  if (errno == EINTR)
			  continue;
		  if (errno != EWOULDBLOCK)
		  {
			 return FALSE;
		  }
#endif
      
		  continue;
	   }
	   else
		  bytessent += i;
	}	
	
	
    return TRUE;            
}


AppWizard has created this diagramx application for you.  This application

not "only" demonstrates the basics of using the Microsoft Foundation classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that make up your diagramx application.

