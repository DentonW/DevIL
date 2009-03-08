all_include = $(srcdir)/../include

il_src = $(srcdir)/../src-IL/src
il_include = $(srcdir)/../src-IL/include
ilu_src = $(srcdir)/../src-ILU/src
ilu_include = $(srcdir)/../src-ILU/include
ilut_src = $(srcdir)/../src-ILUT/src
ilut_include = $(srcdir)/../src-ILUT/include

ildir = $(includedir)/IL

lib_LTLIBRARIES =
il_HEADERS =  $(all_include)/IL/devil_cpp_wrapper.hpp
AM_LDFLAGS = -version-info @DEVIL_LTVERSION@
AM_CFLAGS = $(GENERAL_CFLAGS)

if WINDOWS_BUILD
AM_LDFLAGS += -no-undefined
endif #WINDOWS_BUILD

if BUILD_IL

lib_LTLIBRARIES += libIL.la 
il_HEADERS += $(all_include)/IL/il.h

endif #BUILD_IL

if BUILD_ILU

lib_LTLIBRARIES += libILU.la 
il_HEADERS += $(all_include)/IL/ilu.h $(all_include)/IL/ilu_region.h

endif #BUILD_ILU

if BUILD_ILUT

lib_LTLIBRARIES += libILUT.la 
il_HEADERS += $(all_include)/IL/ilut.h

endif #BUILD_ILUT

libIL_la_CPPFLAGS = -I $(il_include) -I $(all_include)
libIL_la_CFLAGS = $(AM_CFLAGS) @IL_CFLAGS@
libIL_la_CXXFLAGS = $(AM_CFLAGS) @IL_CFLAGS@

libIL_la_SOURCES = $(il_src)/il_alloc.c $(il_src)/il_doom.c $(il_src)/il_stack.c $(il_src)/il_pcx.c $(il_src)/il_rle.c $(il_src)/il_bits.c $(il_src)/il_endian.c $(il_src)/il_exr.cpp $(il_src)/il_jpeg.c $(il_src)/il_pic.c $(il_src)/il_sgi.c $(il_src)/il_bmp.c $(il_src)/il_error.c $(il_src)/il_lif.c $(il_src)/il_png.c $(il_src)/il_states.c $(il_src)/il_convbuff.c $(il_src)/il_files.c $(il_src)/il_main.c $(il_src)/il_pnm.c $(il_src)/il_targa.c $(il_src)/il_wdp.c $(il_src)/il_convert.c $(il_src)/il_gif.c $(il_src)/il_manip.c $(il_src)/il_profiles.c $(il_src)/il_tiff.c $(il_src)/il_cut.c $(il_src)/il_header.c $(il_src)/il_mdl.c $(il_src)/il_quantizer.c $(il_src)/il_utility.c $(il_src)/il_dcx.c $(il_src)/il_icon.c $(il_src)/il_mng.c $(il_src)/il_raw.c $(il_src)/il_wal.c $(il_src)/il_dds.c $(il_src)/il_internal.c $(il_src)/il_pal.c $(il_src)/il_rawdata.c $(il_src)/il_devil.c $(il_src)/il_io.c $(il_src)/il_pcd.c $(il_src)/il_register.c $(il_src)/il_psd.c $(il_src)/il_fastconv.c $(il_src)/il_neuquant.c $(il_src)/il_dds-save.c $(il_src)/il_psp.c $(il_src)/il_pix.c $(il_src)/il_pxr.c $(il_src)/il_vtf.c $(il_src)/il_xpm.c $(il_src)/il_hdr.c $(il_src)/il_icns.c $(il_src)/il_jp2.c $(il_src)/altivec_common.c $(il_src)/altivec_typeconversion.c $(il_src)/il_sun.c $(il_src)/il_nvidia.cpp $(il_src)/il_wbmp.c $(il_src)/il_squish.cpp $(il_src)/il_iff.c $(il_src)/il_ilbm.c $(il_src)/il_size.c $(il_src)/il_tpl.c $(il_src)/il_fits.c $(il_src)/il_iwi.c $(il_src)/il_blp.c $(il_src)/il_ftx.c $(il_src)/il_dicom.c $(il_src)/il_rot.c $(il_src)/il_texture.c $(il_src)/il_dpx.c $(il_src)/il_utx.cpp $(il_src)/il_mp3.c

libIL_la_SOURCES += $(il_include)/altivec_common.h $(il_include)/altivec_typeconversion.h $(il_include)/il_alloc.h $(il_include)/il_bits.h $(il_include)/il_bmp.h $(il_include)/il_dcx.h $(il_include)/il_dds.h $(il_include)/il_doompal.h $(il_include)/il_dpx.h $(il_include)/il_endian.h $(il_include)/il_exr.h $(il_include)/il_files.h $(il_include)/il_gif.h $(il_include)/il_hdr.h $(il_include)/il_icns.h $(il_include)/il_icon.h $(il_include)/il_internal.h $(il_include)/il_jp2.h $(il_include)/il_jpeg.h $(il_include)/il_lif.h $(il_include)/il_manip.h $(il_include)/il_mdl.h $(il_include)/il_pal.h $(il_include)/il_pcx.h $(il_include)/il_pic.h $(il_include)/il_pnm.h $(il_include)/il_psd.h $(il_include)/il_psp.h $(il_include)/il_q2pal.h $(il_include)/il_register.h $(il_include)/il_rle.h $(il_include)/il_sgi.h $(il_include)/il_stack.h $(il_include)/il_states.h $(il_include)/il_targa.h $(il_include)/il_utx.h $(il_include)/il_vtf.h $(il_include)/il_wdp.h $(all_include)/IL/devil_internal_exports.h
libIL_la_LDFLAGS = @IL_LIBS@ $(AM_LDFLAGS)

libILU_la_CPPFLAGS = -I $(ilu_include) -I $(all_include)
libILU_la_CFLAGS = $(AM_CFLAGS) @ILU_CFLAGS@
libILU_la_SOURCES = $(ilu_src)/ilu_alloc.c $(ilu_src)/ilu_internal.c $(ilu_src)/ilu_noise.c $(ilu_src)/ilu_scale.c $(ilu_src)/ilu_error.c $(ilu_src)/ilu_main.c $(ilu_src)/ilu_rotate.c $(ilu_src)/ilu_scaling.c $(ilu_src)/ilu_filter.c $(ilu_src)/ilu_manip.c $(ilu_src)/ilu_scale2d.c $(ilu_src)/ilu_states.c $(ilu_src)/ilu_filter_rcg.c $(ilu_src)/ilu_mipmap.c $(ilu_src)/ilu_scale3d.c $(ilu_src)/ilu_utilities.c $(ilu_src)/ilu_region.c

libILU_la_SOURCES += $(ilu_include)/ilu_alloc.h $(ilu_include)/ilu_filter.h $(ilu_include)/ilu_internal.h $(ilu_include)/ilu_region.h $(ilu_include)/ilu_states.h $(all_include)/IL/devil_internal_exports.h $(all_include)/IL/ilu_region.h $(ilu_include)/ilu_error/ilu_err-arabic.h $(ilu_include)/ilu_error/ilu_err-dutch.h $(ilu_include)/ilu_error/ilu_err-english.h $(ilu_include)/ilu_error/ilu_err-french.h $(ilu_include)/ilu_error/ilu_err-german.h $(ilu_include)/ilu_error/ilu_err-japanese.h $(ilu_include)/ilu_error/ilu_err-spanish.h 

libILU_la_LDFLAGS = @ILU_LIBS@ $(AM_LDFLAGS)
libILU_la_LIBADD = libIL.la

libILUT_la_CPPFLAGS = -I $(ilut_include) -I $(all_include)
libILUT_la_CFLAGS = $(AM_CFLAGS) @ILUT_CFLAGS@
libILUT_la_SOURCES = $(ilut_src)/ilut_main.c $(ilut_src)/ilut_states.c $(ilut_src)/ilut_internal.c  
libILUT_la_SOURCES += $(ilut_include)/ilut_internal.h $(ilut_include)/ilut_states.h $(all_include)/IL/devil_internal_exports.h
libILUT_la_LDFLAGS = @ILUT_LIBS@ $(AM_LDFLAGS)
libILUT_la_LIBADD = libILU.la

if USE_ALLEGRO
libILUT_la_SOURCES += $(ilut_src)/ilut_allegro.c $(ilut_include)/ilut_allegro.h 
endif #USE_ALLEG

if USE_DIRECTX
libILUT_la_SOURCES += $(ilut_src)/ilut_directx.c
endif #USE_DIRECTX

if USE_DIRECTX9
libILUT_la_SOURCES += $(ilut_src)/ilut_directx9.c
endif #USE_DIRECTX

if USE_SDL
libILUT_la_SOURCES += $(ilut_src)/ilut_sdlsurface.c 
endif #USE_SDL

if USE_OPENGL
libILUT_la_SOURCES += $(ilut_include)/ilut_opengl.h $(ilut_src)/ilut_opengl.c 
endif #USE_OPENGL

if USE_X11
libILUT_la_SOURCES += $(ilut_src)/ilut_x11.c
endif #USE_X11

if USE_W32
libILUT_la_SOURCES += $(ilut_src)/ilut_win32.c  
endif #USE_W32

