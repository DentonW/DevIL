# Microsoft Developer Studio Project File - Name="zlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak" CFG="zlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 Dynamic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "zlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zlib___Win32_Debug"
# PROP BASE Intermediate_Dir "zlib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\extlibs\lib\debug"
# PROP Intermediate_Dir "zlib___Win32_Debug"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Zi /Od /D "WIN32" /D "_DEBUG" /FD /GZ /c
# ADD CPP /nologo /W3 /Zi /Od /D "WIN32" /D "_DEBUG" /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zlib___Win32_Release"
# PROP BASE Intermediate_Dir "zlib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "zlib___Win32_Release"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /FD /c
# ADD CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zlib___Win32_Dynamic"
# PROP BASE Intermediate_Dir "zlib___Win32_Dynamic"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "zlib___Win32_Dynamic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ZLIB_DLL" /FD /c
# ADD CPP /nologo /MD /W3 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ZLIB_DLL" /FD /c
MTL=midl.exe
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "zlib - Win32 Debug"
# Name "zlib - Win32 Release"
# Name "zlib - Win32 Dynamic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\contrib\asm386\gvmat32c.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\gzio.c
# ADD BASE CPP /Yc"zutil.h"
# ADD CPP /Yc"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infblock.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infcodes.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffast.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inflate.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inftrees.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infutil.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\zlib.def
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\msdos\zlib.rc

!IF  "$(CFG)" == "zlib - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "zlib - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "zlib - Win32 Dynamic"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zutil.c
# ADD BASE CPP /Yu"zutil.h"
# ADD CPP /Yu"zutil.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zutil.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
