# Microsoft Developer Studio Generated NMAKE File, Based on WindowsTest.dsp
!IF "$(CFG)" == ""
CFG=WindowsTest - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WindowsTest - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WindowsTest - Win32 Release" && "$(CFG)" != "WindowsTest - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WindowsTest.mak" CFG="WindowsTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WindowsTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WindowsTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WindowsTest - Win32 Release"

OUTDIR=.\../bin
INTDIR=.\obj
# Begin Custom Macros
OutDir=.\../bin
# End Custom Macros

ALL : "$(OUTDIR)\WindowsTest.exe"


CLEAN :
	-@erase "$(INTDIR)\BatchConv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WindowsTest.obj"
	-@erase "$(INTDIR)\WindowsTest.res"
	-@erase "$(OUTDIR)\WindowsTest.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WindowsTest.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WindowsTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib gdi32.lib comdlg32.lib shell32.lib colorpicker.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\WindowsTest.pdb" /machine:I386 /out:"$(OUTDIR)\WindowsTest.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BatchConv.obj" \
	"$(INTDIR)\WindowsTest.obj" \
	"$(INTDIR)\WindowsTest.res"

"$(OUTDIR)\WindowsTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WindowsTest - Win32 Debug"

OUTDIR=.\../bin/debug
INTDIR=.\obj/debug
# Begin Custom Macros
OutDir=.\../bin/debug
# End Custom Macros

ALL : "$(OUTDIR)\WindowsTest.exe"


CLEAN :
	-@erase "$(INTDIR)\BatchConv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WindowsTest.obj"
	-@erase "$(INTDIR)\WindowsTest.res"
	-@erase "$(OUTDIR)\WindowsTest.exe"
	-@erase "$(OUTDIR)\WindowsTest.ilk"
	-@erase "$(OUTDIR)\WindowsTest.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WindowsTest.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WindowsTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib gdi32.lib comdlg32.lib shell32.lib colorpicker.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\WindowsTest.pdb" /debug /machine:I386 /out:"$(OUTDIR)\WindowsTest.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BatchConv.obj" \
	"$(INTDIR)\WindowsTest.obj" \
	"$(INTDIR)\WindowsTest.res"

"$(OUTDIR)\WindowsTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("WindowsTest.dep")
!INCLUDE "WindowsTest.dep"
!ELSE 
!MESSAGE Warning: cannot find "WindowsTest.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WindowsTest - Win32 Release" || "$(CFG)" == "WindowsTest - Win32 Debug"
SOURCE=.\BatchConv.cpp

"$(INTDIR)\BatchConv.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WindowsTest.cpp

"$(INTDIR)\WindowsTest.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WindowsTest.rc

"$(INTDIR)\WindowsTest.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

