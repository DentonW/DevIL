# Microsoft Developer Studio Project File - Name="IL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=IL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IL.mak" CFG="IL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "IL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IL - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../src-IL/include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "IL_STATIC_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\DevIL.lib"

!ELSEIF  "$(CFG)" == "IL - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../src-IL/include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "IL_STATIC_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\DevIL-d.lib"

!ENDIF 

# Begin Target

# Name "IL - Win32 Release"
# Name "IL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_alloc.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_bits.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_bmp.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_convbuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_convert.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_cut.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_dcx.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_dds-save.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_dds.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_devil.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_doom.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_endian.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_error.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_fastconv.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_files.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_gif.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_header.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_icon.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_internal.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_io.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_jpeg.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_lif.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_main.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_manip.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_mdl.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_mng.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_neuquant.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_pal.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_pcd.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_pcx.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_pic.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_png.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_pnm.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_profiles.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_psd.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_psp.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_quantizer.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_raw.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_rawdata.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_register.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_rle.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_sgi.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_stack.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_states.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_targa.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_tiff.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_utility.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\src\il_wal.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\IL\il.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_alloc.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_bits.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_bmp.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_dcx.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_dds.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_doompal.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_endian.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_error.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_files.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_gif.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_icon.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_internal.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_jpeg.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_lif.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_manip.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_mdl.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_pal.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_pcx.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_pic.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_pnm.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_psd.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_psp.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_q2pal.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_register.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_rle.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_sgi.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_stack.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_states.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\src-IL\include\il_targa.h"
# End Source File
# End Group
# End Target
# End Project
