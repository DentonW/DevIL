# Microsoft Developer Studio Project File - Name="libpng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libpng - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak" CFG="libpng - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Dynamic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "libpng - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libpng___Win32_Debug"
# PROP BASE Intermediate_Dir "libpng___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\extlibs\lib\debug"
# PROP Intermediate_Dir "libpng___Win32_Debug"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Zi /Od /I "..\.." /I "..\..\..\zlib" /D "DEBUG" /D "_DEBUG" /D PNG_DEBUG=1 /D "WIN32" /Yu"png.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Zi /Od /I "..\.." /I "..\..\..\zlib" /D "DEBUG" /D "_DEBUG" /D PNG_DEBUG=1 /D "WIN32" /Yu"png.h" /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpng - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libpng___Win32_Release"
# PROP BASE Intermediate_Dir "libpng___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libpng___Win32_Release"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /O1 /I "..\.." /I "..\..\..\zlib" /D "WIN32" /D "NDEBUG" /Yu"png.h" /FD /c
# ADD CPP /nologo /W3 /O1 /I "..\.." /I "..\..\..\zlib" /D "WIN32" /D "NDEBUG" /Yu"png.h" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /i "..\.." /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpng - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libpng___Win32_Dynamic"
# PROP BASE Intermediate_Dir "libpng___Win32_Dynamic"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libpng___Win32_Dynamic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /O1 /I "..\.." /I "..\..\..\zlib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PNG_BUILD_DLL" /D "ZLIB_DLL" /Yu"png.h" /FD /c
# ADD CPP /nologo /MD /W3 /O1 /I "..\.." /I "..\..\..\zlib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PNG_BUILD_DLL" /D "ZLIB_DLL" /Yu"png.h" /FD /c
MTL=midl.exe
RSC=rc.exe
# ADD BASE RSC /l 0x409 /i "..\.." /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /out:".\win32\libpng\dll\libpng.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "libpng - Win32 Debug"
# Name "libpng - Win32 Release"
# Name "libpng - Win32 Dynamic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\png.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\projects\msvc\png.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\projects\msvc\png32ms.def
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngerror.c
# ADD BASE CPP /Yc"png.h"
# ADD CPP /Yc"png.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngget.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngset.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngvcrd.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\png.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngconf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\readme.txt
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
