# Microsoft Developer Studio Project File - Name="diagramx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=diagramx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "diagramx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "diagramx.mak" CFG="diagramx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "diagramx - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "diagramx - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "diagramx - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Op /I "..\paintlib\paintlib\win\paintlib\src" /I "..\paintlib\paintlib\win\paintlib" /I "..\paintlib\paintlib\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FGVDOS" /D "FGVWIN" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40a /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\paintlib\lib\Release\paintlib.lib ..\paintlib\lib\Release\libjpeg.lib version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "diagramx - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\paintlib\paintlib\win\paintlib\src" /I "..\paintlib\paintlib\win\paintlib" /I "..\paintlib\paintlib\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "FGVDOS" /D "FGVWIN" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40a /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\paintlib\lib\Debug\paintlib.lib ..\paintlib\lib\Debug\libjpeg.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "diagramx - Win32 Release"
# Name "diagramx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ComboNavega.cpp
# End Source File
# Begin Source File

SOURCE=.\diagramx.cpp
# End Source File
# Begin Source File

SOURCE=.\diagramx.odl
# End Source File
# Begin Source File

SOURCE=.\diagramx.rc
# End Source File
# Begin Source File

SOURCE=.\DialogBuscaMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBrowse.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCambiaClave.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCambiaPasswd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCol.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCPasswd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgErrorFichero.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExporta.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGridFiltro.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGridRellena.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIndexado.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgKActiva.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMata.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewRela.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOldImpre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPregunta.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgreso.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSoporte.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUsuarios.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Ds9Window.cpp
# End Source File
# Begin Source File

SOURCE=.\DsButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DsCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\DsCtrlScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\DsEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DsExcel.cpp
# End Source File
# Begin Source File

SOURCE=.\DsGridButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DsHtml.cpp
# End Source File
# Begin Source File

SOURCE=.\DsImagen.cpp
# End Source File
# Begin Source File

SOURCE=.\DsListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DsPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\DsStdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DsTab9.cpp
# End Source File
# Begin Source File

SOURCE=.\DsTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DsTextEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DsToolBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DsTreeMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DsWord.cpp
# End Source File
# Begin Source File

SOURCE=.\DsxCntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DsxDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DsxIpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DsxLeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\DsxMainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DsxSrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\dsxView.cpp
# End Source File
# Begin Source File

SOURCE=.\EnviaCorreo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridListCtrl1.cpp
# End Source File
# Begin Source File

SOURCE=.\ListEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\llama_a_ds_winsys.c
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Pantalla.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\TablaView.cpp
# End Source File
# Begin Source File

SOURCE=.\Trasmite.cpp
# End Source File
# Begin Source File

SOURCE=.\WTraza.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ComboNavega.h
# End Source File
# Begin Source File

SOURCE=.\diagramx.h
# End Source File
# Begin Source File

SOURCE=.\DialogBuscaMenu.h
# End Source File
# Begin Source File

SOURCE=.\DlgBrowse.h
# End Source File
# Begin Source File

SOURCE=.\DlgCambiaClave.h
# End Source File
# Begin Source File

SOURCE=.\DlgCambiaPasswd.h
# End Source File
# Begin Source File

SOURCE=.\DlgCol.h
# End Source File
# Begin Source File

SOURCE=.\DlgCPasswd.h
# End Source File
# Begin Source File

SOURCE=.\DlgErrorFichero.h
# End Source File
# Begin Source File

SOURCE=.\DlgExporta.h
# End Source File
# Begin Source File

SOURCE=.\DlgGridFiltro.h
# End Source File
# Begin Source File

SOURCE=.\DlgGridRellena.h
# End Source File
# Begin Source File

SOURCE=.\DlgIndexado.h
# End Source File
# Begin Source File

SOURCE=.\DlgKActiva.h
# End Source File
# Begin Source File

SOURCE=.\DlgMata.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewRela.h
# End Source File
# Begin Source File

SOURCE=.\DlgOldImpre.h
# End Source File
# Begin Source File

SOURCE=.\DlgPregunta.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgreso.h
# End Source File
# Begin Source File

SOURCE=.\DlgSoporte.h
# End Source File
# Begin Source File

SOURCE=.\DlgUsuarios.h
# End Source File
# Begin Source File

SOURCE=.\DrawButton.h
# End Source File
# Begin Source File

SOURCE=.\Ds9Window.h
# End Source File
# Begin Source File

SOURCE=.\DsButton.h
# End Source File
# Begin Source File

SOURCE=.\DsCombo.h
# End Source File
# Begin Source File

SOURCE=.\DsCtrlScroll.h
# End Source File
# Begin Source File

SOURCE=.\dsedit.h
# End Source File
# Begin Source File

SOURCE=.\DsExcel.h
# End Source File
# Begin Source File

SOURCE=.\DsGridButton.h
# End Source File
# Begin Source File

SOURCE=.\DsHtml.h
# End Source File
# Begin Source File

SOURCE=.\DsImagen.h
# End Source File
# Begin Source File

SOURCE=.\DsListBox.h
# End Source File
# Begin Source File

SOURCE=.\DsPopup.h
# End Source File
# Begin Source File

SOURCE=.\DsStdDlg.h
# End Source File
# Begin Source File

SOURCE=.\DsTab9.h
# End Source File
# Begin Source File

SOURCE=.\DsTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DsTextEdit.h
# End Source File
# Begin Source File

SOURCE=.\DsToolBarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DsTreeMenu.h
# End Source File
# Begin Source File

SOURCE=.\DsWord.h
# End Source File
# Begin Source File

SOURCE=.\DsxCntrItem.h
# End Source File
# Begin Source File

SOURCE=.\DsxDoc.h
# End Source File
# Begin Source File

SOURCE=.\DsxIpFrame.h
# End Source File
# Begin Source File

SOURCE=.\DsxLeftView.h
# End Source File
# Begin Source File

SOURCE=.\DsxMainFrm.h
# End Source File
# Begin Source File

SOURCE=.\DsxSrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\dsxView.h
# End Source File
# Begin Source File

SOURCE=.\EnviaCorreo.h
# End Source File
# Begin Source File

SOURCE=.\GridListCtrl1.h
# End Source File
# Begin Source File

SOURCE=.\listedit.h
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\Pantalla.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.h
# End Source File
# Begin Source File

SOURCE=.\StaticLabel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\TablaView.h
# End Source File
# Begin Source File

SOURCE=.\Trasmite.h
# End Source File
# Begin Source File

SOURCE=.\wtraza.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmdir1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmdir2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmfile1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmfile2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\botgnues.bmp
# End Source File
# Begin Source File

SOURCE=.\res\botgridn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\carpeta_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ComboTreeShow\res\cbd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ComboTreeShow\res\cbu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\clave.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Copia de DsxDoc.ico"
# End Source File
# Begin Source File

SOURCE=.\res\dflechad.bmp
# End Source File
# Begin Source File

SOURCE=.\res\diagramx.ico
# End Source File
# Begin Source File

SOURCE=.\res\diagramx.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DsxDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ficha_an.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_ba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_bu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_ca.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_im.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_mo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_nu.ico
# End Source File
# Begin Source File

SOURCE=.\res\ficha_pr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_si.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_ul.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ficha_va.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gridsel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=".\res\Icono Oficial 256.ICO"
# End Source File
# Begin Source File

SOURCE=".\res\Icono Oficial.ICO"
# End Source File
# Begin Source File

SOURCE=.\res\idr_ion2.ico
# End Source File
# Begin Source File

SOURCE=.\res\indicato.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\modificar_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nueva.ico
# End Source File
# Begin Source File

SOURCE=.\res\punto_am.bmp
# End Source File
# Begin Source File

SOURCE=.\res\punto_ro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\punto_ve.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticscon1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticscon2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolmens.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\diagramx.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
