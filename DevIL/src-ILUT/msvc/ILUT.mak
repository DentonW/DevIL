# Microsoft Developer Studio Generated NMAKE File, Based on ILUT.dsp
!IF "$(CFG)" == ""
CFG=ILUT - WIN32 DEBUG
!MESSAGE No configuration specified. Defaulting to ILUT - WIN32 DEBUG.
!ENDIF 

!IF "$(CFG)" != "ILUT - Win32 Debug" && "$(CFG)" != "ILUT - Win32 Dynamic" && "$(CFG)" != "ILUT - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ILUT.mak" CFG="ILUT - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ILUT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ILUT - Win32 Dynamic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ILUT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "ILUT - Win32 Debug"

OUTDIR=.\../../lib/debug
INTDIR=.\../src/obj/debug
# Begin Custom Macros
OutDir=.\../../lib/debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ilut-d.dll"

!ELSE 

ALL : "ILU - Win32 Debug" "IL - Win32 Debug" "$(OUTDIR)\ilut-d.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"IL - Win32 DebugCLEAN" "ILU - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ILUT.res"
	-@erase "$(INTDIR)\ilut_directx.obj"
	-@erase "$(INTDIR)\ilut_internal.obj"
	-@erase "$(INTDIR)\ilut_main.obj"
	-@erase "$(INTDIR)\ilut_opengl.obj"
	-@erase "$(INTDIR)\ilut_sdlsurface.obj"
	-@erase "$(INTDIR)\ilut_states.obj"
	-@erase "$(INTDIR)\ilut_win32.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ilut-d.dll"
	-@erase "$(OUTDIR)\ilut-d.exp"
	-@erase "$(OUTDIR)\ilut-d.ilk"
	-@erase "$(OUTDIR)\ilut-d.lib"
	-@erase "$(OUTDIR)\ilut-d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILUT_EXPORTS" /Fp"$(INTDIR)\ILUT.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ILUT.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ILUT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=delayimp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\ilut-d.pdb" /debug /machine:I386 /def:".\ilut.def" /out:"$(OUTDIR)\ilut-d.dll" /implib:"$(OUTDIR)\ilut-d.lib" /pdbtype:sept /delayload:d3d8.dll /delayload:sdl.dll 
LINK32_OBJS= \
	"$(INTDIR)\ilut_directx.obj" \
	"$(INTDIR)\ilut_internal.obj" \
	"$(INTDIR)\ilut_main.obj" \
	"$(INTDIR)\ilut_opengl.obj" \
	"$(INTDIR)\ilut_sdlsurface.obj" \
	"$(INTDIR)\ilut_states.obj" \
	"$(INTDIR)\ilut_win32.obj" \
	"$(INTDIR)\ILUT.res" \
	"$(OUTDIR)\DevIL-d.lib" \
	"$(OUTDIR)\ilu-d.lib"

"$(OUTDIR)\ilut-d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL-1.2.5\lib\debug\ilut-d.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib/debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "ILU - Win32 Debug" "IL - Win32 Debug" "$(OUTDIR)\ilut-d.dll"
   ..\..\projects\msvc\insdll.bat \DevIL-1.2.5\lib\debug\ilut-d.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "ILUT - Win32 Dynamic"

OUTDIR=.\../../lib
INTDIR=.\../src/obj/dynamic
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ilut-l.dll"

!ELSE 

ALL : "ILU - Win32 Dynamic" "IL - Win32 Dynamic" "$(OUTDIR)\ilut-l.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"IL - Win32 DynamicCLEAN" "ILU - Win32 DynamicCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ILUT.res"
	-@erase "$(INTDIR)\ilut_directx.obj"
	-@erase "$(INTDIR)\ilut_internal.obj"
	-@erase "$(INTDIR)\ilut_main.obj"
	-@erase "$(INTDIR)\ilut_opengl.obj"
	-@erase "$(INTDIR)\ilut_sdlsurface.obj"
	-@erase "$(INTDIR)\ilut_states.obj"
	-@erase "$(INTDIR)\ilut_win32.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ilut-l.dll"
	-@erase "$(OUTDIR)\ilut-l.exp"
	-@erase "$(OUTDIR)\ilut-l.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILUT_EXPORTS" /Fp"$(INTDIR)\ILUT.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ILUT.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ILUT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=delayimp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ilut-l.pdb" /machine:I386 /def:".\ilut.def" /out:"$(OUTDIR)\ilut-l.dll" /implib:"$(OUTDIR)\ilut-l.lib" /delayload:d3d8.dll /delayload:sdl.dll 
LINK32_OBJS= \
	"$(INTDIR)\ilut_directx.obj" \
	"$(INTDIR)\ilut_internal.obj" \
	"$(INTDIR)\ilut_main.obj" \
	"$(INTDIR)\ilut_opengl.obj" \
	"$(INTDIR)\ilut_sdlsurface.obj" \
	"$(INTDIR)\ilut_states.obj" \
	"$(INTDIR)\ilut_win32.obj" \
	"$(INTDIR)\ILUT.res" \
	"$(OUTDIR)\DevIL-l.lib" \
	"$(OUTDIR)\ilu-l.lib"

"$(OUTDIR)\ilut-l.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL-1.2.5\lib\ilut-l.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "ILU - Win32 Dynamic" "IL - Win32 Dynamic" "$(OUTDIR)\ilut-l.dll"
   ..\..\projects\msvc\insdll.bat \DevIL-1.2.5\lib\ilut-l.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "ILUT - Win32 Release"

OUTDIR=.\../../lib
INTDIR=.\../src/obj
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ILUT.dll"

!ELSE 

ALL : "ILU - Win32 Release" "IL - Win32 Release" "$(OUTDIR)\ILUT.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"IL - Win32 ReleaseCLEAN" "ILU - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ILUT.res"
	-@erase "$(INTDIR)\ilut_directx.obj"
	-@erase "$(INTDIR)\ilut_internal.obj"
	-@erase "$(INTDIR)\ilut_main.obj"
	-@erase "$(INTDIR)\ilut_opengl.obj"
	-@erase "$(INTDIR)\ilut_sdlsurface.obj"
	-@erase "$(INTDIR)\ilut_states.obj"
	-@erase "$(INTDIR)\ilut_win32.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ILUT.dll"
	-@erase "$(OUTDIR)\ILUT.exp"
	-@erase "$(OUTDIR)\ILUT.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILUT_EXPORTS" /Fp"$(INTDIR)\ILUT.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ILUT.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ILUT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=delayimp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ILUT.pdb" /machine:I386 /def:".\ilut.def" /out:"$(OUTDIR)\ILUT.dll" /implib:"$(OUTDIR)\ILUT.lib" /delayload:d3d8.dll /delayload:sdl.dll 
LINK32_OBJS= \
	"$(INTDIR)\ilut_directx.obj" \
	"$(INTDIR)\ilut_internal.obj" \
	"$(INTDIR)\ilut_main.obj" \
	"$(INTDIR)\ilut_opengl.obj" \
	"$(INTDIR)\ilut_sdlsurface.obj" \
	"$(INTDIR)\ilut_states.obj" \
	"$(INTDIR)\ilut_win32.obj" \
	"$(INTDIR)\ILUT.res" \
	"$(OUTDIR)\DevIL.lib" \
	"$(OUTDIR)\ILU.lib"

"$(OUTDIR)\ILUT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL-1.2.5\lib\ILUT.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "ILU - Win32 Release" "IL - Win32 Release" "$(OUTDIR)\ILUT.dll"
   ..\..\projects\msvc\insdll.bat \DevIL-1.2.5\lib\ILUT.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

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
!IF EXISTS("ILUT.dep")
!INCLUDE "ILUT.dep"
!ELSE 
!MESSAGE Warning: cannot find "ILUT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ILUT - Win32 Debug" || "$(CFG)" == "ILUT - Win32 Dynamic" || "$(CFG)" == "ILUT - Win32 Release"
SOURCE=..\src\ilut_directx.c

"$(INTDIR)\ilut_directx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilut_internal.c

"$(INTDIR)\ilut_internal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilut_main.c

"$(INTDIR)\ilut_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilut_opengl.c

"$(INTDIR)\ilut_opengl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilut_sdlsurface.c

"$(INTDIR)\ilut_sdlsurface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilut_states.c

"$(INTDIR)\ilut_states.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilut_win32.c

"$(INTDIR)\ilut_win32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ILUT.rc

"$(INTDIR)\ILUT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "ILUT - Win32 Debug"

"IL - Win32 Debug" : 
   cd "\DevIL-1.2.5\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Debug" 
   cd "..\..\src-ILUT\msvc"

"IL - Win32 DebugCLEAN" : 
   cd "\DevIL-1.2.5\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\src-ILUT\msvc"

!ELSEIF  "$(CFG)" == "ILUT - Win32 Dynamic"

"IL - Win32 Dynamic" : 
   cd "\DevIL-1.2.5\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Dynamic" 
   cd "..\..\src-ILUT\msvc"

"IL - Win32 DynamicCLEAN" : 
   cd "\DevIL-1.2.5\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Dynamic" RECURSE=1 CLEAN 
   cd "..\..\src-ILUT\msvc"

!ELSEIF  "$(CFG)" == "ILUT - Win32 Release"

"IL - Win32 Release" : 
   cd "\DevIL-1.2.5\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Release" 
   cd "..\..\src-ILUT\msvc"

"IL - Win32 ReleaseCLEAN" : 
   cd "\DevIL-1.2.5\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\src-ILUT\msvc"

!ENDIF 

!IF  "$(CFG)" == "ILUT - Win32 Debug"

"ILU - Win32 Debug" : 
   cd "\DevIL-1.2.5\src-ILU\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\ILU.mak" CFG="ILU - Win32 Debug" 
   cd "..\..\src-ILUT\msvc"

"ILU - Win32 DebugCLEAN" : 
   cd "\DevIL-1.2.5\src-ILU\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\ILU.mak" CFG="ILU - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\src-ILUT\msvc"

!ELSEIF  "$(CFG)" == "ILUT - Win32 Dynamic"

"ILU - Win32 Dynamic" : 
   cd "\DevIL-1.2.5\src-ILU\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\ILU.mak" CFG="ILU - Win32 Dynamic" 
   cd "..\..\src-ILUT\msvc"

"ILU - Win32 DynamicCLEAN" : 
   cd "\DevIL-1.2.5\src-ILU\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\ILU.mak" CFG="ILU - Win32 Dynamic" RECURSE=1 CLEAN 
   cd "..\..\src-ILUT\msvc"

!ELSEIF  "$(CFG)" == "ILUT - Win32 Release"

"ILU - Win32 Release" : 
   cd "\DevIL-1.2.5\src-ILU\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\ILU.mak" CFG="ILU - Win32 Release" 
   cd "..\..\src-ILUT\msvc"

"ILU - Win32 ReleaseCLEAN" : 
   cd "\DevIL-1.2.5\src-ILU\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\ILU.mak" CFG="ILU - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\src-ILUT\msvc"

!ENDIF 


!ENDIF 

