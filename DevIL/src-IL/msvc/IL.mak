# Microsoft Developer Studio Generated NMAKE File, Based on IL.dsp
!IF "$(CFG)" == ""
CFG=IL - WIN32 DEBUG
!MESSAGE No configuration specified. Defaulting to IL - WIN32 DEBUG.
!ENDIF 

!IF "$(CFG)" != "IL - Win32 Debug" && "$(CFG)" != "IL - Win32 Dynamic" && "$(CFG)" != "IL - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IL.mak" CFG="IL - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IL - Win32 Dynamic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "IL - Win32 Debug"

OUTDIR=.\../../lib
INTDIR=.\../src/obj/debug
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

ALL : "$(OUTDIR)\DevIL-d.dll"


CLEAN :
	-@erase "$(INTDIR)\IL.res"
	-@erase "$(INTDIR)\il_alloc.obj"
	-@erase "$(INTDIR)\il_bits.obj"
	-@erase "$(INTDIR)\il_bmp.obj"
	-@erase "$(INTDIR)\il_convbuff.obj"
	-@erase "$(INTDIR)\il_convert.obj"
	-@erase "$(INTDIR)\il_cut.obj"
	-@erase "$(INTDIR)\il_dcx.obj"
	-@erase "$(INTDIR)\il_dds.obj"
	-@erase "$(INTDIR)\il_devil.obj"
	-@erase "$(INTDIR)\il_doom.obj"
	-@erase "$(INTDIR)\il_endian.obj"
	-@erase "$(INTDIR)\il_error.obj"
	-@erase "$(INTDIR)\il_fastconv.obj"
	-@erase "$(INTDIR)\il_files.obj"
	-@erase "$(INTDIR)\il_gif.obj"
	-@erase "$(INTDIR)\il_header.obj"
	-@erase "$(INTDIR)\il_icon.obj"
	-@erase "$(INTDIR)\il_internal.obj"
	-@erase "$(INTDIR)\il_io.obj"
	-@erase "$(INTDIR)\il_jpeg.obj"
	-@erase "$(INTDIR)\il_lif.obj"
	-@erase "$(INTDIR)\il_main.obj"
	-@erase "$(INTDIR)\il_manip.obj"
	-@erase "$(INTDIR)\il_mdl.obj"
	-@erase "$(INTDIR)\il_mng.obj"
	-@erase "$(INTDIR)\il_neuquant.obj"
	-@erase "$(INTDIR)\il_pal.obj"
	-@erase "$(INTDIR)\il_pcd.obj"
	-@erase "$(INTDIR)\il_pcx.obj"
	-@erase "$(INTDIR)\il_pic.obj"
	-@erase "$(INTDIR)\il_png.obj"
	-@erase "$(INTDIR)\il_pnm.obj"
	-@erase "$(INTDIR)\il_profiles.obj"
	-@erase "$(INTDIR)\il_psd.obj"
	-@erase "$(INTDIR)\il_quantizer.obj"
	-@erase "$(INTDIR)\il_raw.obj"
	-@erase "$(INTDIR)\il_rawdata.obj"
	-@erase "$(INTDIR)\il_register.obj"
	-@erase "$(INTDIR)\il_rle.obj"
	-@erase "$(INTDIR)\il_sgi.obj"
	-@erase "$(INTDIR)\il_stack.obj"
	-@erase "$(INTDIR)\il_states.obj"
	-@erase "$(INTDIR)\il_targa.obj"
	-@erase "$(INTDIR)\il_tiff.obj"
	-@erase "$(INTDIR)\il_utility.obj"
	-@erase "$(INTDIR)\il_wal.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\DevIL-d.dll"
	-@erase "$(OUTDIR)\DevIL-d.exp"
	-@erase "$(OUTDIR)\DevIL-d.ilk"
	-@erase "$(OUTDIR)\DevIL-d.lib"
	-@erase "$(OUTDIR)\DevIL-d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IL_EXPORTS" /Fp"$(INTDIR)\IL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\IL.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"../src/obj/debug/DevIL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\DevIL-d.pdb" /debug /machine:I386 /def:".\il.def" /out:"$(OUTDIR)\DevIL-d.dll" /implib:"$(OUTDIR)\DevIL-d.lib" /pdbtype:sept 
DEF_FILE= \
	".\il.def"
LINK32_OBJS= \
	"$(INTDIR)\il_alloc.obj" \
	"$(INTDIR)\il_bits.obj" \
	"$(INTDIR)\il_bmp.obj" \
	"$(INTDIR)\il_convbuff.obj" \
	"$(INTDIR)\il_convert.obj" \
	"$(INTDIR)\il_cut.obj" \
	"$(INTDIR)\il_dcx.obj" \
	"$(INTDIR)\il_dds.obj" \
	"$(INTDIR)\il_devil.obj" \
	"$(INTDIR)\il_doom.obj" \
	"$(INTDIR)\il_endian.obj" \
	"$(INTDIR)\il_error.obj" \
	"$(INTDIR)\il_fastconv.obj" \
	"$(INTDIR)\il_files.obj" \
	"$(INTDIR)\il_gif.obj" \
	"$(INTDIR)\il_header.obj" \
	"$(INTDIR)\il_icon.obj" \
	"$(INTDIR)\il_internal.obj" \
	"$(INTDIR)\il_io.obj" \
	"$(INTDIR)\il_jpeg.obj" \
	"$(INTDIR)\il_lif.obj" \
	"$(INTDIR)\il_main.obj" \
	"$(INTDIR)\il_manip.obj" \
	"$(INTDIR)\il_mdl.obj" \
	"$(INTDIR)\il_mng.obj" \
	"$(INTDIR)\il_neuquant.obj" \
	"$(INTDIR)\il_pal.obj" \
	"$(INTDIR)\il_pcd.obj" \
	"$(INTDIR)\il_pcx.obj" \
	"$(INTDIR)\il_pic.obj" \
	"$(INTDIR)\il_png.obj" \
	"$(INTDIR)\il_pnm.obj" \
	"$(INTDIR)\il_profiles.obj" \
	"$(INTDIR)\il_psd.obj" \
	"$(INTDIR)\il_quantizer.obj" \
	"$(INTDIR)\il_raw.obj" \
	"$(INTDIR)\il_rawdata.obj" \
	"$(INTDIR)\il_register.obj" \
	"$(INTDIR)\il_rle.obj" \
	"$(INTDIR)\il_sgi.obj" \
	"$(INTDIR)\il_stack.obj" \
	"$(INTDIR)\il_states.obj" \
	"$(INTDIR)\il_targa.obj" \
	"$(INTDIR)\il_tiff.obj" \
	"$(INTDIR)\il_utility.obj" \
	"$(INTDIR)\il_wal.obj" \
	"$(INTDIR)\IL.res"

"$(OUTDIR)\DevIL-d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL\lib\DevIL-d.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\DevIL-d.dll"
   ..\..\projects\msvc\insdll.bat \DevIL\lib\DevIL-d.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "IL - Win32 Dynamic"

OUTDIR=.\../../lib
INTDIR=.\../src/obj/dynamic
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

ALL : "$(OUTDIR)\DevIL-l.dll"


CLEAN :
	-@erase "$(INTDIR)\IL.res"
	-@erase "$(INTDIR)\il_alloc.obj"
	-@erase "$(INTDIR)\il_bits.obj"
	-@erase "$(INTDIR)\il_bmp.obj"
	-@erase "$(INTDIR)\il_convbuff.obj"
	-@erase "$(INTDIR)\il_convert.obj"
	-@erase "$(INTDIR)\il_cut.obj"
	-@erase "$(INTDIR)\il_dcx.obj"
	-@erase "$(INTDIR)\il_dds.obj"
	-@erase "$(INTDIR)\il_devil.obj"
	-@erase "$(INTDIR)\il_doom.obj"
	-@erase "$(INTDIR)\il_endian.obj"
	-@erase "$(INTDIR)\il_error.obj"
	-@erase "$(INTDIR)\il_fastconv.obj"
	-@erase "$(INTDIR)\il_files.obj"
	-@erase "$(INTDIR)\il_gif.obj"
	-@erase "$(INTDIR)\il_header.obj"
	-@erase "$(INTDIR)\il_icon.obj"
	-@erase "$(INTDIR)\il_internal.obj"
	-@erase "$(INTDIR)\il_io.obj"
	-@erase "$(INTDIR)\il_jpeg.obj"
	-@erase "$(INTDIR)\il_lif.obj"
	-@erase "$(INTDIR)\il_main.obj"
	-@erase "$(INTDIR)\il_manip.obj"
	-@erase "$(INTDIR)\il_mdl.obj"
	-@erase "$(INTDIR)\il_mng.obj"
	-@erase "$(INTDIR)\il_neuquant.obj"
	-@erase "$(INTDIR)\il_pal.obj"
	-@erase "$(INTDIR)\il_pcd.obj"
	-@erase "$(INTDIR)\il_pcx.obj"
	-@erase "$(INTDIR)\il_pic.obj"
	-@erase "$(INTDIR)\il_png.obj"
	-@erase "$(INTDIR)\il_pnm.obj"
	-@erase "$(INTDIR)\il_profiles.obj"
	-@erase "$(INTDIR)\il_psd.obj"
	-@erase "$(INTDIR)\il_quantizer.obj"
	-@erase "$(INTDIR)\il_raw.obj"
	-@erase "$(INTDIR)\il_rawdata.obj"
	-@erase "$(INTDIR)\il_register.obj"
	-@erase "$(INTDIR)\il_rle.obj"
	-@erase "$(INTDIR)\il_sgi.obj"
	-@erase "$(INTDIR)\il_stack.obj"
	-@erase "$(INTDIR)\il_states.obj"
	-@erase "$(INTDIR)\il_targa.obj"
	-@erase "$(INTDIR)\il_tiff.obj"
	-@erase "$(INTDIR)\il_utility.obj"
	-@erase "$(INTDIR)\il_wal.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DevIL-l.dll"
	-@erase "$(OUTDIR)\DevIL-l.exp"
	-@erase "$(OUTDIR)\DevIL-l.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IL_EXPORTS" /Fp"$(INTDIR)\IL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\IL.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"../src/obj/dynamic/DevIL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=delayimp.lib user32.lib gdi32.lib comdlg32.lib shell32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\DevIL-l.pdb" /machine:I386 /def:".\il.def" /out:"$(OUTDIR)\DevIL-l.dll" /implib:"$(OUTDIR)\DevIL-l.lib" /OPT:NOWIN98 /delayload:libpng3.dll /delayload:lcms.dll 
DEF_FILE= \
	".\il.def"
LINK32_OBJS= \
	"$(INTDIR)\il_alloc.obj" \
	"$(INTDIR)\il_bits.obj" \
	"$(INTDIR)\il_bmp.obj" \
	"$(INTDIR)\il_convbuff.obj" \
	"$(INTDIR)\il_convert.obj" \
	"$(INTDIR)\il_cut.obj" \
	"$(INTDIR)\il_dcx.obj" \
	"$(INTDIR)\il_dds.obj" \
	"$(INTDIR)\il_devil.obj" \
	"$(INTDIR)\il_doom.obj" \
	"$(INTDIR)\il_endian.obj" \
	"$(INTDIR)\il_error.obj" \
	"$(INTDIR)\il_fastconv.obj" \
	"$(INTDIR)\il_files.obj" \
	"$(INTDIR)\il_gif.obj" \
	"$(INTDIR)\il_header.obj" \
	"$(INTDIR)\il_icon.obj" \
	"$(INTDIR)\il_internal.obj" \
	"$(INTDIR)\il_io.obj" \
	"$(INTDIR)\il_jpeg.obj" \
	"$(INTDIR)\il_lif.obj" \
	"$(INTDIR)\il_main.obj" \
	"$(INTDIR)\il_manip.obj" \
	"$(INTDIR)\il_mdl.obj" \
	"$(INTDIR)\il_mng.obj" \
	"$(INTDIR)\il_neuquant.obj" \
	"$(INTDIR)\il_pal.obj" \
	"$(INTDIR)\il_pcd.obj" \
	"$(INTDIR)\il_pcx.obj" \
	"$(INTDIR)\il_pic.obj" \
	"$(INTDIR)\il_png.obj" \
	"$(INTDIR)\il_pnm.obj" \
	"$(INTDIR)\il_profiles.obj" \
	"$(INTDIR)\il_psd.obj" \
	"$(INTDIR)\il_quantizer.obj" \
	"$(INTDIR)\il_raw.obj" \
	"$(INTDIR)\il_rawdata.obj" \
	"$(INTDIR)\il_register.obj" \
	"$(INTDIR)\il_rle.obj" \
	"$(INTDIR)\il_sgi.obj" \
	"$(INTDIR)\il_stack.obj" \
	"$(INTDIR)\il_states.obj" \
	"$(INTDIR)\il_targa.obj" \
	"$(INTDIR)\il_tiff.obj" \
	"$(INTDIR)\il_utility.obj" \
	"$(INTDIR)\il_wal.obj" \
	"$(INTDIR)\IL.res"

"$(OUTDIR)\DevIL-l.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL\lib\DevIL-l.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\DevIL-l.dll"
   ..\..\projects\msvc\insdll.bat \DevIL\lib\DevIL-l.dll
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "IL - Win32 Release"

OUTDIR=.\../../lib
INTDIR=.\../src/obj
# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

ALL : "$(OUTDIR)\DevIL.dll"


CLEAN :
	-@erase "$(INTDIR)\IL.res"
	-@erase "$(INTDIR)\il_alloc.obj"
	-@erase "$(INTDIR)\il_bits.obj"
	-@erase "$(INTDIR)\il_bmp.obj"
	-@erase "$(INTDIR)\il_convbuff.obj"
	-@erase "$(INTDIR)\il_convert.obj"
	-@erase "$(INTDIR)\il_cut.obj"
	-@erase "$(INTDIR)\il_dcx.obj"
	-@erase "$(INTDIR)\il_dds.obj"
	-@erase "$(INTDIR)\il_devil.obj"
	-@erase "$(INTDIR)\il_doom.obj"
	-@erase "$(INTDIR)\il_endian.obj"
	-@erase "$(INTDIR)\il_error.obj"
	-@erase "$(INTDIR)\il_fastconv.obj"
	-@erase "$(INTDIR)\il_files.obj"
	-@erase "$(INTDIR)\il_gif.obj"
	-@erase "$(INTDIR)\il_header.obj"
	-@erase "$(INTDIR)\il_icon.obj"
	-@erase "$(INTDIR)\il_internal.obj"
	-@erase "$(INTDIR)\il_io.obj"
	-@erase "$(INTDIR)\il_jpeg.obj"
	-@erase "$(INTDIR)\il_lif.obj"
	-@erase "$(INTDIR)\il_main.obj"
	-@erase "$(INTDIR)\il_manip.obj"
	-@erase "$(INTDIR)\il_mdl.obj"
	-@erase "$(INTDIR)\il_mng.obj"
	-@erase "$(INTDIR)\il_neuquant.obj"
	-@erase "$(INTDIR)\il_pal.obj"
	-@erase "$(INTDIR)\il_pcd.obj"
	-@erase "$(INTDIR)\il_pcx.obj"
	-@erase "$(INTDIR)\il_pic.obj"
	-@erase "$(INTDIR)\il_png.obj"
	-@erase "$(INTDIR)\il_pnm.obj"
	-@erase "$(INTDIR)\il_profiles.obj"
	-@erase "$(INTDIR)\il_psd.obj"
	-@erase "$(INTDIR)\il_quantizer.obj"
	-@erase "$(INTDIR)\il_raw.obj"
	-@erase "$(INTDIR)\il_rawdata.obj"
	-@erase "$(INTDIR)\il_register.obj"
	-@erase "$(INTDIR)\il_rle.obj"
	-@erase "$(INTDIR)\il_sgi.obj"
	-@erase "$(INTDIR)\il_stack.obj"
	-@erase "$(INTDIR)\il_states.obj"
	-@erase "$(INTDIR)\il_targa.obj"
	-@erase "$(INTDIR)\il_tiff.obj"
	-@erase "$(INTDIR)\il_utility.obj"
	-@erase "$(INTDIR)\il_wal.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DevIL.dll"
	-@erase "$(OUTDIR)\DevIL.exp"
	-@erase "$(OUTDIR)\DevIL.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IL_EXPORTS" /D "IL_STATIC_LIB" /Fp"$(INTDIR)\IL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\IL.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"../src/obj/DevIL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=user32.lib gdi32.lib comdlg32.lib shell32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\DevIL.pdb" /machine:I386 /def:".\il.def" /out:"$(OUTDIR)\DevIL.dll" /implib:"$(OUTDIR)\DevIL.lib" /OPT:NOWIN98 
DEF_FILE= \
	".\il.def"
LINK32_OBJS= \
	"$(INTDIR)\il_alloc.obj" \
	"$(INTDIR)\il_bits.obj" \
	"$(INTDIR)\il_bmp.obj" \
	"$(INTDIR)\il_convbuff.obj" \
	"$(INTDIR)\il_convert.obj" \
	"$(INTDIR)\il_cut.obj" \
	"$(INTDIR)\il_dcx.obj" \
	"$(INTDIR)\il_dds.obj" \
	"$(INTDIR)\il_devil.obj" \
	"$(INTDIR)\il_doom.obj" \
	"$(INTDIR)\il_endian.obj" \
	"$(INTDIR)\il_error.obj" \
	"$(INTDIR)\il_fastconv.obj" \
	"$(INTDIR)\il_files.obj" \
	"$(INTDIR)\il_gif.obj" \
	"$(INTDIR)\il_header.obj" \
	"$(INTDIR)\il_icon.obj" \
	"$(INTDIR)\il_internal.obj" \
	"$(INTDIR)\il_io.obj" \
	"$(INTDIR)\il_jpeg.obj" \
	"$(INTDIR)\il_lif.obj" \
	"$(INTDIR)\il_main.obj" \
	"$(INTDIR)\il_manip.obj" \
	"$(INTDIR)\il_mdl.obj" \
	"$(INTDIR)\il_mng.obj" \
	"$(INTDIR)\il_neuquant.obj" \
	"$(INTDIR)\il_pal.obj" \
	"$(INTDIR)\il_pcd.obj" \
	"$(INTDIR)\il_pcx.obj" \
	"$(INTDIR)\il_pic.obj" \
	"$(INTDIR)\il_png.obj" \
	"$(INTDIR)\il_pnm.obj" \
	"$(INTDIR)\il_profiles.obj" \
	"$(INTDIR)\il_psd.obj" \
	"$(INTDIR)\il_quantizer.obj" \
	"$(INTDIR)\il_raw.obj" \
	"$(INTDIR)\il_rawdata.obj" \
	"$(INTDIR)\il_register.obj" \
	"$(INTDIR)\il_rle.obj" \
	"$(INTDIR)\il_sgi.obj" \
	"$(INTDIR)\il_stack.obj" \
	"$(INTDIR)\il_states.obj" \
	"$(INTDIR)\il_targa.obj" \
	"$(INTDIR)\il_tiff.obj" \
	"$(INTDIR)\il_utility.obj" \
	"$(INTDIR)\il_wal.obj" \
	"$(INTDIR)\IL.res"

"$(OUTDIR)\DevIL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

TargetPath=\DevIL\lib\DevIL.dll
SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\../../lib
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\DevIL.dll"
   ..\..\projects\msvc\insdll.bat \DevIL\lib\DevIL.dll
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
!IF EXISTS("IL.dep")
!INCLUDE "IL.dep"
!ELSE 
!MESSAGE Warning: cannot find "IL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IL - Win32 Debug" || "$(CFG)" == "IL - Win32 Dynamic" || "$(CFG)" == "IL - Win32 Release"
SOURCE=..\src\il_alloc.c

"$(INTDIR)\il_alloc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_bits.c

"$(INTDIR)\il_bits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_bmp.c

"$(INTDIR)\il_bmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_convbuff.c

"$(INTDIR)\il_convbuff.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_convert.c

"$(INTDIR)\il_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_cut.c

"$(INTDIR)\il_cut.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_dcx.c

"$(INTDIR)\il_dcx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_dds.c

"$(INTDIR)\il_dds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_devil.c

"$(INTDIR)\il_devil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_doom.c

"$(INTDIR)\il_doom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_endian.c

"$(INTDIR)\il_endian.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_error.c

"$(INTDIR)\il_error.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_fastconv.c

"$(INTDIR)\il_fastconv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_files.c

"$(INTDIR)\il_files.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_gif.c

"$(INTDIR)\il_gif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_header.c

"$(INTDIR)\il_header.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_icon.c

"$(INTDIR)\il_icon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_internal.c

"$(INTDIR)\il_internal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_io.c

"$(INTDIR)\il_io.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_jpeg.c

"$(INTDIR)\il_jpeg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_lif.c

"$(INTDIR)\il_lif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_main.c

"$(INTDIR)\il_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_manip.c

"$(INTDIR)\il_manip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_mdl.c

"$(INTDIR)\il_mdl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_mng.c

"$(INTDIR)\il_mng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_neuquant.c

"$(INTDIR)\il_neuquant.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_pal.c

"$(INTDIR)\il_pal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_pcd.c

"$(INTDIR)\il_pcd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_pcx.c

"$(INTDIR)\il_pcx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_pic.c

"$(INTDIR)\il_pic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_png.c

"$(INTDIR)\il_png.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_pnm.c

"$(INTDIR)\il_pnm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_profiles.c

"$(INTDIR)\il_profiles.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_psd.c

"$(INTDIR)\il_psd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_quantizer.c

"$(INTDIR)\il_quantizer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_raw.c

"$(INTDIR)\il_raw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_rawdata.c

"$(INTDIR)\il_rawdata.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_register.c

"$(INTDIR)\il_register.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_rle.c

"$(INTDIR)\il_rle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_sgi.c

"$(INTDIR)\il_sgi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_stack.c

"$(INTDIR)\il_stack.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_states.c

"$(INTDIR)\il_states.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_targa.c

"$(INTDIR)\il_targa.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_tiff.c

"$(INTDIR)\il_tiff.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_utility.c

"$(INTDIR)\il_utility.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\il_wal.c

"$(INTDIR)\il_wal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\IL.rc

"$(INTDIR)\IL.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

