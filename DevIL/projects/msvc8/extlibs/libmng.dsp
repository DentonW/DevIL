# Microsoft Developer Studio Project File - Name="libmng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libmng - Win32 Dynamic
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libmng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libmng.mak" CFG="libmng - Win32 Dynamic"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libmng - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libmng - Win32 Dynamic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libmng - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libmng - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libmng___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBMNG_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBMNG_EXPORTS" /D "MNG_BUILD_DLL" /YX /FD /c
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

!ELSEIF  "$(CFG)" == "libmng - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Dynamic"
# PROP BASE Intermediate_Dir "Dynamic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libmng___Win32_Dynamic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBMNG_EXPORTS" /D "MNG_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBMNG_EXPORTS" /D "MNG_BUILD_DLL" /YX /FD /c
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

!ELSEIF  "$(CFG)" == "libmng - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libmng___Win32_Debug0"
# PROP BASE Intermediate_Dir "libmng___Win32_Debug0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib\debug"
# PROP Intermediate_Dir "libmng___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBMNG_EXPORTS" /D "MNG_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBMNG_EXPORTS" /D "MNG_BUILD_DLL" /YX /FD /c
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

!ENDIF 

# Begin Target

# Name "libmng - Win32 Release"
# Name "libmng - Win32 Dynamic"
# Name "libmng - Win32 Debug"
# Begin Group "mng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_callback_xs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_chunk_io.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_chunk_prc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_chunk_xs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_cms.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_display.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_dither.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_error.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_filter.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_hlapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_jpeg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_object_prc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_pixels.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_prop_xs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_read.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_trace.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_write.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libmng\libmng_zlib.c
# End Source File
# End Group
# Begin Group "jpg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcexif.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcphuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdexif.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdphuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jerror.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jexifsup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jidctred.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibJPEG\jutils.c
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
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

SOURCE=..\..\..\extlibs\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zutil.c
# End Source File
# End Group
# Begin Group "lcms"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmscam97.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmscnvrt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmserr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsgamma.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsgmt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsintrp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsio1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmslut.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsmatsh.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsmtrx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmspack.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmspcs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmssamp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmswtpnt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LittleCMS\src\cmsxform.c
# End Source File
# End Group
# End Target
# End Project
