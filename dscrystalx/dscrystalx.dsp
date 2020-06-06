# Microsoft Developer Studio Project File - Name="dscrystalx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=dscrystalx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dscrystalx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dscrystalx.mak" CFG="dscrystalx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dscrystalx - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dscrystalx - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dscrystalx - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40a /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "dscrystalx - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ   /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40a /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "dscrystalx - Win32 Release"
# Name "dscrystalx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\crdesignerctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\crvtrackcursorinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\crystalreportviewer4.cpp
# End Source File
# Begin Source File

SOURCE=.\dscrystalx.cpp
# End Source File
# Begin Source File

SOURCE=.\dscrystalx.rc
# End Source File
# Begin Source File

SOURCE=.\dscrystalxDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\crdesignerctrl.h
# End Source File
# Begin Source File

SOURCE=.\crvtrackcursorinfo.h
# End Source File
# Begin Source File

SOURCE=.\crystalreportviewer4.h
# End Source File
# Begin Source File

SOURCE=.\dscrystalx.h
# End Source File
# Begin Source File

SOURCE=.\dscrystalxDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dscrystalx.ico
# End Source File
# Begin Source File

SOURCE=.\res\dscrystalx.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section dscrystalx : {2A3A1C02-F9DB-11D3-808E-00A0C9DAC83F}
# 	2:5:Class:CCRDesignerCtrl
# 	2:10:HeaderFile:crdesignerctrl.h
# 	2:8:ImplFile:crdesignerctrl.cpp
# End Section
# Section dscrystalx : {134C838B-E65A-11D3-BFF4-00A0C9DA4FA2}
# 	2:5:Class:CCrystalReportViewer4
# 	2:10:HeaderFile:crystalreportviewer4.h
# 	2:8:ImplFile:crystalreportviewer4.cpp
# End Section
# Section dscrystalx : {13FA5946-561C-11D1-BE3F-00A0C95A6A5C}
# 	2:5:Class:CCRVTrackCursorInfo
# 	2:10:HeaderFile:crvtrackcursorinfo.h
# 	2:8:ImplFile:crvtrackcursorinfo.cpp
# End Section
# Section dscrystalx : {2A3A1C00-F9DB-11D3-808E-00A0C9DAC83F}
# 	2:21:DefaultSinkHeaderFile:crdesignerctrl.h
# 	2:16:DefaultSinkClass:CCRDesignerCtrl
# End Section
# Section dscrystalx : {C4847596-972C-11D0-9567-00A0C9273C2A}
# 	2:21:DefaultSinkHeaderFile:crystalreportviewer4.h
# 	2:16:DefaultSinkClass:CCrystalReportViewer4
# End Section
