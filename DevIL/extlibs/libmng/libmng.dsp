# Microsoft Developer Studio Project File - Name="libmng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libmng - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libmng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libmng.mak" CFG="libmng - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libmng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libmng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libmng - Win32 Dynamic" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libmng"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libmng - Win32 Debug"

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
MTL=midl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\zlib" /I "..\ImageLib\lcms\src" /I "..\libjpeg" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"libmng_trace.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\zlib" /I "..\lcms\src" /I "..\libjpeg" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"libmng_trace.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\Debug\libmng.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\Debug\libmng.lib"

!ELSEIF  "$(CFG)" == "libmng - Win32 Release"

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
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\zlib" /I "..\ImageLib\lcms\src" /I "..\libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"libmng_trace.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\zlib" /I "..\lcms\src" /I "..\libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"libmng_trace.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\Release\libmng.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\libmng.lib"

!ELSEIF  "$(CFG)" == "libmng - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libmng___Win32_Dynamic"
# PROP BASE Intermediate_Dir "libmng___Win32_Dynamic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Dynamic"
# PROP Intermediate_Dir "Dynamic"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\zlib" /I "..\lcms\src" /I "..\libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"libmng_trace.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\zlib" /I "..\lcms\src" /I "..\libjpeg" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"libmng_trace.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\libmng.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\libmng.lib"

!ENDIF 

# Begin Target

# Name "libmng - Win32 Debug"
# Name "libmng - Win32 Release"
# Name "libmng - Win32 Dynamic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\libmng_callback_xs.c
# End Source File
# Begin Source File

SOURCE=.\libmng_chunk_io.c
# End Source File
# Begin Source File

SOURCE=.\libmng_chunk_prc.c
# End Source File
# Begin Source File

SOURCE=.\libmng_chunk_xs.c
# End Source File
# Begin Source File

SOURCE=.\libmng_cms.c
# End Source File
# Begin Source File

SOURCE=.\libmng_display.c
# End Source File
# Begin Source File

SOURCE=.\libmng_dither.c
# End Source File
# Begin Source File

SOURCE=.\libmng_error.c
# End Source File
# Begin Source File

SOURCE=.\libmng_filter.c
# End Source File
# Begin Source File

SOURCE=.\libmng_hlapi.c
# End Source File
# Begin Source File

SOURCE=.\libmng_jpeg.c
# End Source File
# Begin Source File

SOURCE=.\libmng_object_prc.c
# End Source File
# Begin Source File

SOURCE=.\libmng_pixels.c
# End Source File
# Begin Source File

SOURCE=.\libmng_prop_xs.c
# End Source File
# Begin Source File

SOURCE=.\libmng_read.c
# End Source File
# Begin Source File

SOURCE=.\libmng_trace.c
# ADD BASE CPP /Yc"libmng_trace.h"
# ADD CPP /Yc"libmng_trace.h"
# End Source File
# Begin Source File

SOURCE=.\libmng_write.c
# End Source File
# Begin Source File

SOURCE=.\libmng_zlib.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\libmng.h
# End Source File
# Begin Source File

SOURCE=.\libmng_chunk_io.h
# End Source File
# Begin Source File

SOURCE=.\libmng_chunk_prc.h
# End Source File
# Begin Source File

SOURCE=.\libmng_chunks.h
# End Source File
# Begin Source File

SOURCE=.\libmng_cms.h
# End Source File
# Begin Source File

SOURCE=.\libmng_conf.h
# End Source File
# Begin Source File

SOURCE=.\libmng_data.h
# End Source File
# Begin Source File

SOURCE=.\libmng_display.h
# End Source File
# Begin Source File

SOURCE=.\libmng_dither.h
# End Source File
# Begin Source File

SOURCE=.\libmng_error.h
# End Source File
# Begin Source File

SOURCE=.\libmng_filter.h
# End Source File
# Begin Source File

SOURCE=.\libmng_jpeg.h
# End Source File
# Begin Source File

SOURCE=.\libmng_memory.h
# End Source File
# Begin Source File

SOURCE=.\libmng_object_prc.h
# End Source File
# Begin Source File

SOURCE=.\libmng_objects.h
# End Source File
# Begin Source File

SOURCE=.\libmng_pixels.h
# End Source File
# Begin Source File

SOURCE=.\libmng_read.h
# End Source File
# Begin Source File

SOURCE=.\libmng_trace.h
# End Source File
# Begin Source File

SOURCE=.\libmng_types.h
# End Source File
# Begin Source File

SOURCE=.\libmng_write.h
# End Source File
# Begin Source File

SOURCE=.\libmng_zlib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
