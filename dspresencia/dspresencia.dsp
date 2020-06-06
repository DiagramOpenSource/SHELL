# Microsoft Developer Studio Project File - Name="dspresencia" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=dspresencia - Win32 Test - Win32 ReleaseDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dspresencia.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dspresencia.mak" CFG="dspresencia - Win32 Test - Win32 ReleaseDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dspresencia - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dspresencia - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "dspresencia - Win32 Test - Win32 DebugDLL" (based on "Win32 (x86) Console Application")
!MESSAGE "dspresencia - Win32 Test - Win32 ReleaseDLL" (based on "Win32 (x86) Console Application")
!MESSAGE "dspresencia - Win32 Adade" (based on "Win32 (x86) Console Application")
!MESSAGE "dspresencia - Win32 OTP" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Zp4 /MD /W3 /GX /O2 /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /D "NDEBUG" /D "FGVDOS" /D "FGVMEMORYFILE" /D "FGVGZIP" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /D "FGVNFS" /YX /FD /c
# ADD BASE RSC /l 0x40a /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib C:\wx2\lib\wx.lib C:\wx2\lib\xpm.lib C:\wx2\lib\png.lib C:\wx2\lib\zlib.lib C:\wx2\lib\jpeg.lib C:\wx2\lib\tiff.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /nodefaultlib:"msvcrtd.lib" /out:"dspresencia.exe" /libpath:"e:\wx2\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /D "_DEBUG" /D DEBUG=1 /D "__WXDEBUG__" /D "FGVDOS" /D "FGVMEMORYFILE" /D "FGVGZIP" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /D "FGVNFS" /FR /Yu"wx/wxprec.h" /FD /GZ /c
# ADD BASE RSC /l 0x40a /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib C:\wx2\lib\wxd.lib C:\wx2\lib\xpmd.lib C:\wx2\lib\pngd.lib C:\wx2\lib\zlibd.lib C:\wx2\lib\jpegd.lib C:\wx2\lib\tiffd.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /out:"dspresencia.exe" /pdbtype:sept /libpath:"e:\wx2\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dspresencia___Win32_Test___Win32_DebugDLL"
# PROP BASE Intermediate_Dir "dspresencia___Win32_Test___Win32_DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dspresencia___Win32_Test___Win32_DebugDLL"
# PROP Intermediate_Dir "dspresencia___Win32_Test___Win32_DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "e:\wx2\include" /D "_DEBUG" /D DEBUG=1 /D "__WXDEBUG__" /D WXUSINGDLL=1 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /Fp"Debug/dspresencia.pch" /Yu"wx/wxprec.h" /Fo"Debug/" /Fd"Debug/" /FD /GZ /c
# ADD BASE RSC /l 0x40a /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wx22_5d.lib /nologo /subsystem:windows /pdb:"Debug/dspresencia.pdb" /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /out:"dspresencia.exe" /pdbtype:sept /libpath:"e:\wx2\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dspresencia___Win32_Test___Win32_ReleaseDLL"
# PROP BASE Intermediate_Dir "dspresencia___Win32_Test___Win32_ReleaseDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dspresencia___Win32_Test___Win32_ReleaseDLL"
# PROP Intermediate_Dir "dspresencia___Win32_Test___Win32_ReleaseDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /O1 /Ob2 /I "e:\wx2\include" /D "_DEBUG" /D "NDEBUG" /D WXUSINGDLL=1 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /YX /Fo"Release/" /Fd"Release/" /FD /GZ /c
# ADD BASE RSC /l 0x40a /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wx22_5.lib /nologo /subsystem:windows /incremental:no /pdb:"Release/dspresencia.pdb" /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /out:"dspresencia.exe" /pdbtype:sept /libpath:"e:\wx2\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dspresencia___Win32_Adade"
# PROP BASE Intermediate_Dir "dspresencia___Win32_Adade"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dspresencia___Win32_Adade"
# PROP Intermediate_Dir "dspresencia___Win32_Adade"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "FGVDOS" /D "FGVMEMORYFILE" /D "FGVGZIP" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /D "NDEBUG" /D "FGVDOS" /D "FGVMEMORYFILE" /D "FGVGZIP" /D "CLIENTE" /D "ADADE" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /YX /FD /c
# ADD BASE RSC /l 0x40a /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wx.lib xpm.lib png.lib zlib.lib jpeg.lib tiff.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /nodefaultlib:"msvcrtd.lib" /out:"dspresencia.exe" /libpath:"e:\wx2\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib C:\wx2\lib\wx.lib C:\wx2\lib\xpm.lib C:\wx2\lib\png.lib C:\wx2\lib\zlib.lib C:\wx2\lib\jpeg.lib C:\wx2\lib\tiff.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /nodefaultlib:"msvcrtd.lib" /libpath:"e:\wx2\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dspresencia___Win32_OTP"
# PROP BASE Intermediate_Dir "dspresencia___Win32_OTP"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dspresencia___Win32_OTP"
# PROP Intermediate_Dir "dspresencia___Win32_OTP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /D "NDEBUG" /D "FGVDOS" /D "FGVMEMORYFILE" /D "FGVGZIP" /D "CLIENTE" /D "ADADE" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /D "NDEBUG" /D "FGVDOS" /D "FGVMEMORYFILE" /D "FGVGZIP" /D "CLIENTE" /D "OTP" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "SINDSDB" /YX /FD /c
# ADD BASE RSC /l 0x40a /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wx.lib xpm.lib png.lib zlib.lib jpeg.lib tiff.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /nodefaultlib:"msvcrtd.lib" /libpath:"e:\wx2\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib C:\wx2\lib\wx.lib C:\wx2\lib\xpm.lib C:\wx2\lib\png.lib C:\wx2\lib\zlib.lib C:\wx2\lib\jpeg.lib C:\wx2\lib\tiff.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /nodefaultlib:"msvcrtd.lib" /libpath:"e:\wx2\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "dspresencia - Win32 Release"
# Name "dspresencia - Win32 Debug"
# Name "dspresencia - Win32 Test - Win32 DebugDLL"
# Name "dspresencia - Win32 Test - Win32 ReleaseDLL"
# Name "dspresencia - Win32 Adade"
# Name "dspresencia - Win32 OTP"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Arbol.cpp
# End Source File
# Begin Source File

SOURCE=.\common.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CONNECT.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgAtender.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPresencia.cpp

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DlgVolume.cpp
# End Source File
# Begin Source File

SOURCE=..\..\fuente\dsgen\dsgprot.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dspresencia.rc
# End Source File
# Begin Source File

SOURCE=.\dspresenciaApp.cpp

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /Yc"wx/wxprec.h"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /Yc"wx/wxprec.h"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

# ADD CPP /Fp"Debug/dspresencia.pch" /Yc"wx/wxprec.h"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

# ADD CPP /Fp"Release/dspresencia.pch" /Yc"wx/wxprec.h"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /Yc"wx/wxprec.h"
# ADD CPP /Yc"wx/wxprec.h"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /Yc"wx/wxprec.h"
# ADD CPP /Yc"wx/wxprec.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dspresenciaFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.cpp

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ListaUsers.cpp
# End Source File
# Begin Source File

SOURCE=.\ListDef.cpp
# End Source File
# Begin Source File

SOURCE=.\principal.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\fuente\ds_system\rdeci.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\ds_system\rfiltro.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\fuente\dsgen\rldef.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\fuente\ds_system\rquita.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\fuente\ds_system\rstring.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\fuente\ds_system\rwatof.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPEAKER.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\ULAW.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\volume.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VOX.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Arbol.h
# End Source File
# Begin Source File

SOURCE=.\DlgAtender.h
# End Source File
# Begin Source File

SOURCE=.\DlgExplorer.h
# End Source File
# Begin Source File

SOURCE=.\DlgPresencia.h
# End Source File
# Begin Source File

SOURCE=.\DlgVolume.h
# End Source File
# Begin Source File

SOURCE=.\dspresenciaApp.h
# End Source File
# Begin Source File

SOURCE=.\dspresenciaFrm.h
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.h
# End Source File
# Begin Source File

SOURCE=.\ListaUsers.h
# End Source File
# Begin Source File

SOURCE=.\ListDef.h
# End Source File
# Begin Source File

SOURCE=.\NETFONE.H
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Tcontrol.h
# End Source File
# Begin Source File

SOURCE=.\ULAW.H
# End Source File
# Begin Source File

SOURCE=.\VOX.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\wx\msw\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\colours.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\confamo.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cross.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\disable.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\dll1.bin
# End Source File
# Begin Source File

SOURCE=.\dspresencia.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\enconf.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\error.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\icon9.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\iconinac.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\iconpres.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\idxml.bin
# End Source File
# Begin Source File

SOURCE=.\wx\msw\info.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\item.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\llamadae.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\oculto.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\plot_dwn.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\plot_enl.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\plot_shr.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\plot_up.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\plot_zin.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\plot_zot.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\question.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\ring.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\root.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\size.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\sleeping.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\teatiend.bmp
# End Source File
# Begin Source File

SOURCE=Z:\teatienden.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\tick.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\tip.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\warning.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\watch1.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\wxresize.ico
# End Source File
# End Group
# Begin Group "gzip"

# PROP Default_Filter ".c"
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\bits.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\crypt.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\deflate.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\getopt.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\gzip.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\inflate.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\trees.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\unlzh.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\unlzw.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\unpack.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\unzip.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\util.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\fgv\fuente\gziplib\zip.c

!IF  "$(CFG)" == "dspresencia - Win32 Release"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

!ENDIF 

# End Source File
# End Group
# Begin Group "gsm"

# PROP Default_Filter ".c"
# Begin Source File

SOURCE=.\GSM\ADD.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\CODE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\DEBUG.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\DECODE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_CREA.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_DECO.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_DEST.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_ENCO.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_EXPL.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_IMPL.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_OPTI.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\GSM_PRIN.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\LONG_TER.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\LPC.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\PREPROCE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\RPE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\SHORT_TE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GSM\TABLE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# ADD BASE CPP /I ".\gsm\inc"
# SUBTRACT BASE CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX
# ADD CPP /I ".\gsm\inc"
# SUBTRACT CPP /I "c:\wx2\include" /I "c:\fgv_10\include" /I ".\lpc10" /I ".\md5" /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "lpc10"

# PROP Default_Filter ".c"
# Begin Source File

SOURCE=.\LPC10\ANALYS.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\BSYNZ.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\CHANNEL.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\DCBIAS.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\DECODE_lpc10.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\DEEMP.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\DIFMAG.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\DYPTRK.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\ENCODE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\ENERGY.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\FILTER.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\HAM84.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\HP100.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\INITIAL.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\INVERT.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\IRC2PC.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\IVFILT.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\LPC10.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\LPFILT31.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\MEDIAN.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\MLOAD.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\ONSET.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\PITSYN.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\PLACEA.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\PLACEV.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\PREEMP.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\RANDOM.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\RCCHK.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\SYNTHS.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\TBDM.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\TRANS.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\VOICE.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\VOICE_NN.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LPC10\VPARMS.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "md5"

# PROP Default_Filter ".c"
# Begin Source File

SOURCE=.\md5\MD5.C

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ds_k_int.dll
# End Source File
# Begin Source File

SOURCE=..\..\..\hook\keydll3\Release\ds_k_int.dll
# End Source File
# Begin Source File

SOURCE=.\dspresencia.xpm
# PROP Exclude_From_Scan -1

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\dstvncvw.exe
# End Source File
# Begin Source File

SOURCE=.\res\dstvncvw.exe.tmp
# End Source File
# Begin Source File

SOURCE=.\makefile
# PROP Exclude_From_Scan -1

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\makefile.gtk
# PROP Exclude_From_Scan -1

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\makefile.mtf
# PROP Exclude_From_Scan -1

!IF  "$(CFG)" == "dspresencia - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Test - Win32 ReleaseDLL"

!ELSEIF  "$(CFG)" == "dspresencia - Win32 Adade"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "dspresencia - Win32 OTP"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\omnithread_rt.dll
# End Source File
# Begin Source File

SOURCE=.\res\omnithread_rt.dll.tmp
# End Source File
# Begin Source File

SOURCE=.\ringout.wav
# End Source File
# Begin Source File

SOURCE=.\res\VNCHooks.dll
# End Source File
# Begin Source File

SOURCE=.\res\VNCHooks.dll.tmp
# End Source File
# Begin Source File

SOURCE=.\res\WinDStvnc.exe
# End Source File
# Begin Source File

SOURCE=.\res\WinDStvnc.exe.tmp
# End Source File
# Begin Source File

SOURCE=.\res\zlib.dll
# End Source File
# Begin Source File

SOURCE=.\res\zlib.dll.tmp
# End Source File
# End Target
# End Project
