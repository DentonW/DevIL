# Microsoft Developer Studio Generated NMAKE File, Based on ILU.dsp
!IF "$(CFG)" == ""
CFG=ILU - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ILU - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ILU - Win32 Debug" && "$(CFG)" != "ILU - Win32 Release" && "$(CFG)" != "ILU - Win32 Dynamic"
!MESSAGE Invalid configuration "$(CFG)" specified.
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

!IF  "$(CFG)" == "ILU - Win32 Debug"

OUTDIR=.\../../lib/debug
INTDIR=.\../src/obj/debug
# Begin Custom Macros
OutDir=.\../../lib/debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ilu-d.dll"

!ELSE 

ALL : "IL - Win32 Debug" "$(OUTDIR)\ilu-d.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"IL - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ILU.res"
	-@erase "$(INTDIR)\ilu_alloc.obj"
	-@erase "$(INTDIR)\ilu_error.obj"
	-@erase "$(INTDIR)\ilu_filter.obj"
	-@erase "$(INTDIR)\ilu_filter_rcg.obj"
	-@erase "$(INTDIR)\ilu_internal.obj"
	-@erase "$(INTDIR)\ilu_main.obj"
	-@erase "$(INTDIR)\ilu_manip.obj"
	-@erase "$(INTDIR)\ilu_mipmap.obj"
	-@erase "$(INTDIR)\ilu_noise.obj"
	-@erase "$(INTDIR)\ilu_rotate.obj"
	-@erase "$(INTDIR)\ilu_scale.obj"
	-@erase "$(INTDIR)\ilu_scale2d.obj"
	-@erase "$(INTDIR)\ilu_scale3d.obj"
	-@erase "$(INTDIR)\ilu_states.obj"
	-@erase "$(INTDIR)\ilu_utilities.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ilu-d.dll"
	-@erase "$(OUTDIR)\ilu-d.exp"
	-@erase "$(OUTDIR)\ilu-d.ilk"
	-@erase "$(OUTDIR)\ilu-d.lib"
	-@erase "$(OUTDIR)\ilu-d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /Fp"$(INTDIR)\ILU.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ILU.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ILU.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\ilu-d.pdb" /debug /machine:I386 /def:".\ilu.def" /out:"$(OUTDIR)\ilu-d.dll" /implib:"$(OUTDIR)\ilu-d.lib" /pdbtype:sept 
DEF_FILE= \
	".\ilu.def"
LINK32_OBJS= \
	"$(INTDIR)\ilu_alloc.obj" \
	"$(INTDIR)\ilu_error.obj" \
	"$(INTDIR)\ilu_filter.obj" \
	"$(INTDIR)\ilu_filter_rcg.obj" \
	"$(INTDIR)\ilu_internal.obj" \
	"$(INTDIR)\ilu_main.obj" \
	"$(INTDIR)\ilu_manip.obj" \
	"$(INTDIR)\ilu_mipmap.obj" \
	"$(INTDIR)\ilu_noise.obj" \
	"$(INTDIR)\ilu_rotate.obj" \
	"$(INTDIR)\ilu_scale.obj" \
	"$(INTDIR)\ilu_scale2d.obj" \
	"$(INTDIR)\ilu_scale3d.obj" \
	"$(INTDIR)\ilu_states.obj" \
	"$(INTDIR)\ilu_utilities.obj" \
	"$(INTDIR)\ILU.res" \
	"$(OUTDIR)\DevIL-d.lib"

"$(OUTDIR)\ilu-d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL\lib\debug\ilu-d.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib/debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "IL - Win32 Debug" "$(OUTDIR)\ilu-d.dll"
   ..\..\projects\msvc\insdll.bat \DevIL\lib\debug\ilu-d.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "ILU - Win32 Release"

OUTDIR=.\../../lib
INTDIR=.\../src/obj
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ILU.dll"

!ELSE 

ALL : "IL - Win32 Release" "$(OUTDIR)\ILU.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"IL - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ILU.res"
	-@erase "$(INTDIR)\ilu_alloc.obj"
	-@erase "$(INTDIR)\ilu_error.obj"
	-@erase "$(INTDIR)\ilu_filter.obj"
	-@erase "$(INTDIR)\ilu_filter_rcg.obj"
	-@erase "$(INTDIR)\ilu_internal.obj"
	-@erase "$(INTDIR)\ilu_main.obj"
	-@erase "$(INTDIR)\ilu_manip.obj"
	-@erase "$(INTDIR)\ilu_mipmap.obj"
	-@erase "$(INTDIR)\ilu_noise.obj"
	-@erase "$(INTDIR)\ilu_rotate.obj"
	-@erase "$(INTDIR)\ilu_scale.obj"
	-@erase "$(INTDIR)\ilu_scale2d.obj"
	-@erase "$(INTDIR)\ilu_scale3d.obj"
	-@erase "$(INTDIR)\ilu_states.obj"
	-@erase "$(INTDIR)\ilu_utilities.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ILU.dll"
	-@erase "$(OUTDIR)\ILU.exp"
	-@erase "$(OUTDIR)\ILU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /Fp"$(INTDIR)\ILU.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ILU.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ILU.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ILU.pdb" /machine:I386 /def:".\ilu.def" /out:"$(OUTDIR)\ILU.dll" /implib:"$(OUTDIR)\ILU.lib" 
DEF_FILE= \
	".\ilu.def"
LINK32_OBJS= \
	"$(INTDIR)\ilu_alloc.obj" \
	"$(INTDIR)\ilu_error.obj" \
	"$(INTDIR)\ilu_filter.obj" \
	"$(INTDIR)\ilu_filter_rcg.obj" \
	"$(INTDIR)\ilu_internal.obj" \
	"$(INTDIR)\ilu_main.obj" \
	"$(INTDIR)\ilu_manip.obj" \
	"$(INTDIR)\ilu_mipmap.obj" \
	"$(INTDIR)\ilu_noise.obj" \
	"$(INTDIR)\ilu_rotate.obj" \
	"$(INTDIR)\ilu_scale.obj" \
	"$(INTDIR)\ilu_scale2d.obj" \
	"$(INTDIR)\ilu_scale3d.obj" \
	"$(INTDIR)\ilu_states.obj" \
	"$(INTDIR)\ilu_utilities.obj" \
	"$(INTDIR)\ILU.res" \
	"$(OUTDIR)\DevIL.lib"

"$(OUTDIR)\ILU.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL\lib\ILU.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "IL - Win32 Release" "$(OUTDIR)\ILU.dll"
   ..\..\projects\msvc\insdll.bat \DevIL\lib\ILU.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "ILU - Win32 Dynamic"

OUTDIR=.\../../lib
INTDIR=.\../src/obj/dynamic
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ilu-l.dll"

!ELSE 

ALL : "IL - Win32 Dynamic" "$(OUTDIR)\ilu-l.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"IL - Win32 DynamicCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ILU.res"
	-@erase "$(INTDIR)\ilu_alloc.obj"
	-@erase "$(INTDIR)\ilu_error.obj"
	-@erase "$(INTDIR)\ilu_filter.obj"
	-@erase "$(INTDIR)\ilu_filter_rcg.obj"
	-@erase "$(INTDIR)\ilu_internal.obj"
	-@erase "$(INTDIR)\ilu_main.obj"
	-@erase "$(INTDIR)\ilu_manip.obj"
	-@erase "$(INTDIR)\ilu_mipmap.obj"
	-@erase "$(INTDIR)\ilu_noise.obj"
	-@erase "$(INTDIR)\ilu_rotate.obj"
	-@erase "$(INTDIR)\ilu_scale.obj"
	-@erase "$(INTDIR)\ilu_scale2d.obj"
	-@erase "$(INTDIR)\ilu_scale3d.obj"
	-@erase "$(INTDIR)\ilu_states.obj"
	-@erase "$(INTDIR)\ilu_utilities.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ilu-l.dll"
	-@erase "$(OUTDIR)\ilu-l.exp"
	-@erase "$(OUTDIR)\ilu-l.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ILU_EXPORTS" /Fp"$(INTDIR)\ILU.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ILU.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ILU.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ilu-l.pdb" /machine:I386 /def:".\ilu.def" /out:"$(OUTDIR)\ilu-l.dll" /implib:"$(OUTDIR)\ilu-l.lib" 
DEF_FILE= \
	".\ilu.def"
LINK32_OBJS= \
	"$(INTDIR)\ilu_alloc.obj" \
	"$(INTDIR)\ilu_error.obj" \
	"$(INTDIR)\ilu_filter.obj" \
	"$(INTDIR)\ilu_filter_rcg.obj" \
	"$(INTDIR)\ilu_internal.obj" \
	"$(INTDIR)\ilu_main.obj" \
	"$(INTDIR)\ilu_manip.obj" \
	"$(INTDIR)\ilu_mipmap.obj" \
	"$(INTDIR)\ilu_noise.obj" \
	"$(INTDIR)\ilu_rotate.obj" \
	"$(INTDIR)\ilu_scale.obj" \
	"$(INTDIR)\ilu_scale2d.obj" \
	"$(INTDIR)\ilu_scale3d.obj" \
	"$(INTDIR)\ilu_states.obj" \
	"$(INTDIR)\ilu_utilities.obj" \
	"$(INTDIR)\ILU.res" \
	"$(OUTDIR)\DevIL-l.lib"

"$(OUTDIR)\ilu-l.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL\lib\ilu-l.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "IL - Win32 Dynamic" "$(OUTDIR)\ilu-l.dll"
   ..\..\projects\msvc\insdll.bat \DevIL\lib\ilu-l.dll
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
!IF EXISTS("ILU.dep")
!INCLUDE "ILU.dep"
!ELSE 
!MESSAGE Warning: cannot find "ILU.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ILU - Win32 Debug" || "$(CFG)" == "ILU - Win32 Release" || "$(CFG)" == "ILU - Win32 Dynamic"
SOURCE=..\src\ilu_alloc.c

"$(INTDIR)\ilu_alloc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_error.c

"$(INTDIR)\ilu_error.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_filter.c

"$(INTDIR)\ilu_filter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_filter_rcg.c

"$(INTDIR)\ilu_filter_rcg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_internal.c

"$(INTDIR)\ilu_internal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_main.c

"$(INTDIR)\ilu_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_manip.c

"$(INTDIR)\ilu_manip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_mipmap.c

"$(INTDIR)\ilu_mipmap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_noise.c

"$(INTDIR)\ilu_noise.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_rotate.c

"$(INTDIR)\ilu_rotate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_scale.c

"$(INTDIR)\ilu_scale.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_scale2d.c

"$(INTDIR)\ilu_scale2d.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_scale3d.c

"$(INTDIR)\ilu_scale3d.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_states.c

"$(INTDIR)\ilu_states.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\ilu_utilities.c

"$(INTDIR)\ilu_utilities.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ILU.rc

"$(INTDIR)\ILU.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "ILU - Win32 Debug"

"IL - Win32 Debug" : 
   cd "\DevIL\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Debug" 
   cd "..\..\src-ILU\msvc"

"IL - Win32 DebugCLEAN" : 
   cd "\DevIL\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\src-ILU\msvc"

!ELSEIF  "$(CFG)" == "ILU - Win32 Release"

"IL - Win32 Release" : 
   cd "\DevIL\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Release" 
   cd "..\..\src-ILU\msvc"

"IL - Win32 ReleaseCLEAN" : 
   cd "\DevIL\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\src-ILU\msvc"

!ELSEIF  "$(CFG)" == "ILU - Win32 Dynamic"

"IL - Win32 Dynamic" : 
   cd "\DevIL\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Dynamic" 
   cd "..\..\src-ILU\msvc"

"IL - Win32 DynamicCLEAN" : 
   cd "\DevIL\src-IL\msvc"
   $(MAKE) /$(MAKEFLAGS) /F ".\IL.mak" CFG="IL - Win32 Dynamic" RECURSE=1 CLEAN 
   cd "..\..\src-ILU\msvc"

!ENDIF 


!ENDIF 

