# Microsoft Developer Studio Project File - Name="ILU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ILU - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ILU.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ILU.mak" CFG="ILU - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ILU - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ILU - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ILU - Win32 Dynamic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ILU"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ILU - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../lib/debug"
# PROP BASE Intermediate_Dir "../src/obj/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib/debug"
# PROP Intermediate_Dir "../src/obj/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../lib/debug/ilu-d.dll" /pdbtype:sept
# Begin Special Build Tool
TargetPath=\DevIL\lib\debug\ilu-d.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\projects\msvc\insdll.bat $(TargetPath)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ILU - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../lib"
# PROP BASE Intermediate_Dir "../src/obj"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "../src/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
TargetPath=\DevIL\lib\ILU.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\projects\msvc\insdll.bat $(TargetPath)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ILU - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../lib"
# PROP BASE Intermediate_Dir "../src/obj/dynamic"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "../src/obj/dynamic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../lib/ilu-l.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../lib/ilu-l.dll"
# Begin Special Build Tool
TargetPath=\DevIL\lib\ilu-l.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\projects\msvc\insdll.bat $(TargetPath)
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ILU - Win32 Debug"
# Name "ILU - Win32 Release"
# Name "ILU - Win32 Dynamic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ilu.def
# End Source File
# Begin Source File

SOURCE=..\src\ilu_error.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_filter.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_filter_rcg.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_internal.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_main.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_manip.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_mipmap.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_noise.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_rotate.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_scale.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_scale2d.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_scale3d.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_states.c
# End Source File
# Begin Source File

SOURCE=..\src\ilu_utilities.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\il\ilu.h
# End Source File
# Begin Source File

SOURCE=..\include\ilu_error.h
# End Source File
# Begin Source File

SOURCE=..\include\ilu_filter.h
# End Source File
# Begin Source File

SOURCE=..\include\ilu_internal.h
# End Source File
# Begin Source File

SOURCE=..\include\ilu_mipmap.h
# End Source File
# Begin Source File

SOURCE=..\include\ilu_states.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\resources\IL Logo.ico"
# End Source File
# Begin Source File

SOURCE=.\ILU.rc
# End Source File
# End Group
# End Target
# End Project
