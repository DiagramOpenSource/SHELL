# Microsoft Developer Studio Project File - Name="wcmfc32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wcmfc32 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Wcmfc32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Wcmfc32.mak" CFG="wcmfc32 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wcmfc32 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wcmfc32 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wcmfc32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\release"
# PROP Intermediate_Dir ".\release"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE F90 /Ox /I "Release/" /c /nologo /MT
# ADD F90 /Ox /I "release/" /c /nologo /MT
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "WCMFCLIB" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "wcmfc32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\debug"
# PROP Intermediate_Dir ".\debug"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE F90 /Zi /I "Debug/" /c /nologo /MT
# ADD F90 /Zi /I "debug/" /c /nologo /MT
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "WCMFCLIB" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386

!ENDIF 

# Begin Target

# Name "wcmfc32 - Win32 Release"
# Name "wcmfc32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Basegrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Bmcell.cpp
# End Source File
# Begin Source File

SOURCE=.\Cell.cpp
# End Source File
# Begin Source File

SOURCE=.\Cellarra.cpp
# End Source File
# Begin Source File

SOURCE=.\Cellgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Cellindi.cpp
# End Source File
# Begin Source File

SOURCE=.\Cellmatr.cpp
# End Source File
# Begin Source File

SOURCE=.\Cellrang.cpp
# End Source File
# Begin Source File

SOURCE=.\Colorbut.cpp
# End Source File
# Begin Source File

SOURCE=.\Colorpag.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctxtcell.cpp
# End Source File
# Begin Source File

SOURCE=.\Fontpage.cpp
# End Source File
# Begin Source File

SOURCE=.\Libmain.cpp
# End Source File
# Begin Source File

SOURCE=.\Rtxtcell.cpp
# End Source File
# Begin Source File

SOURCE=.\Stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Textcell.cpp
# End Source File
# Begin Source File

SOURCE=.\wcmfc32.def
# End Source File
# Begin Source File

SOURCE=.\Wcmfcres.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\basegrid.h
# End Source File
# Begin Source File

SOURCE=.\bmcell.h
# End Source File
# Begin Source File

SOURCE=.\cell.h
# End Source File
# Begin Source File

SOURCE=.\cellarra.h
# End Source File
# Begin Source File

SOURCE=.\cellgrid.h
# End Source File
# Begin Source File

SOURCE=.\cellindi.h
# End Source File
# Begin Source File

SOURCE=.\cellmatr.h
# End Source File
# Begin Source File

SOURCE=.\cellrang.h
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\colorbut.h
# End Source File
# Begin Source File

SOURCE=.\colorpag.h
# End Source File
# Begin Source File

SOURCE=.\ctxtcell.h
# End Source File
# Begin Source File

SOURCE=.\fontpage.h
# End Source File
# Begin Source File

SOURCE=.\rtxtcell.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\textcell.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
