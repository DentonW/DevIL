# Microsoft Developer Studio Project File - Name="libjpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libjpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibJpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibJpeg.mak" CFG="libjpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libjpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libjpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libjpeg - Win32 Dynamic" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libjpeg"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libjpeg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libjpeg___Win32_Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\extlibs\lib\ibjpeg.lib"

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\extlibs\lib\debug"
# PROP Intermediate_Dir "libjpeg___Win32_Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Dynamic"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libjpeg___Win32_Dynamic"
# PROP BASE Intermediate_Dir "libjpeg___Win32_Dynamic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\extlibs\lib"
# PROP Intermediate_Dir "libjpeg___Win32_Dynamic"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"libjpeg.lib"
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libjpeg - Win32 Release"
# Name "libjpeg - Win32 Debug"
# Name "libjpeg - Win32 Dynamic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctred.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jutils.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jconfig.h

!IF  "$(CFG)" == "libjpeg - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jconfig.h
InputName=jconfig

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jconfig.h
InputName=jconfig

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Dynamic"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jconfig.h
InputName=jconfig

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmorecfg.h

!IF  "$(CFG)" == "libjpeg - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jmorecfg.h
InputName=jmorecfg

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jmorecfg.h
InputName=jmorecfg

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Dynamic"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jmorecfg.h
InputName=jmorecfg

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jpeglib.h

!IF  "$(CFG)" == "libjpeg - Win32 Release"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jpeglib.h
InputName=jpeglib

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jpeglib.h
InputName=jpeglib

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Dynamic"

# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jpeglib.h
InputName=jpeglib

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\include

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jversion.h
# End Source File
# End Group
# End Target
# End Project
