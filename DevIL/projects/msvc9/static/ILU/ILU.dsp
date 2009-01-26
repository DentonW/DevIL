# Microsoft Developer Studio Project File - Name="ILU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

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
!MESSAGE "ILU - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ILU - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ILU - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../src-ILU/include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "IL_STATIC_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ILU - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../src-ILU/include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "IL_STATIC_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\ILU-d.lib"

!ENDIF 

# Begin Target

# Name "ILU - Win32 Release"
# Name "ILU - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_alloc.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_error.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_filter.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_filter_rcg.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_internal.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_main.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_manip.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_mipmap.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_noise.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_rotate.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_scale.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_scale2d.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_scale3d.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_scaling.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_states.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\src\ilu_utilities.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\IL\ilu.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\include\ilu_alloc.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\include\ilu_filter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\include\ilu_internal.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\include\ilu_mipmap.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-ILU\include\ilu_states.h"
# End Source File
# End Group
# End Target
# End Project
