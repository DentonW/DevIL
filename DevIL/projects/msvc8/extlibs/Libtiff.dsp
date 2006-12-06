# Microsoft Developer Studio Project File - Name="Libtiff" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Libtiff - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Libtiff.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Libtiff.mak" CFG="Libtiff - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Libtiff - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Libtiff - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Libtiff - Win32 Dynamic" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Libtiff - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libtiff___Win32_Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Libtiff - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\extlibs\lib\debug"
# PROP Intermediate_Dir "libtiff___Win32_Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Libtiff - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Libtiff___Win32_Dynamic"
# PROP BASE Intermediate_Dir "Libtiff___Win32_Dynamic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libtiff___Win32_Dynamic"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Libtiff - Win32 Release"
# Name "Libtiff - Win32 Debug"
# Name "Libtiff - Win32 Dynamic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\fax3sm_winnt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_aux.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_close.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_codec.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_dir.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_dirinfo.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_dirread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_dirwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_dumpmode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_error.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_fax3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_flush.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_getimage.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_jpeg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_luv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_lzw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_msdos.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTiff\libtiff\tif_next.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_open.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_packbits.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_pixarlog.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_predict.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_print.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_read.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_strip.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_swab.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_thunder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_tile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_version.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_warning.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_write.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibTIFF\libtiff\tif_zip.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
