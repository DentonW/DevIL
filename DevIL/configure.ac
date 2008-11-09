dnl 
dnl Autoconf script for DevIL Library
dnl Written by Meloni Dario
dnl modified by Matej Tyc, report bugs to <matej.tyc@gmail.com>
dnl 

dnl
dnl configure.in initialization
dnl
AC_INIT([DevIL], 
	1.7.2,
	[matej.tyc@gmail.com])
AC_CONFIG_SRCDIR([src-IL/src/il_main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

dnl
dnl Minimum Autoconf version
dnl
AC_PREREQ(2.52)

DEVIL_MAJOR=1
DEVIL_MINOR=7
DEVIL_REVISION=2
DEVIL_VERSION=${DEVIL_MAJOR}${DEVIL_MINOR}${DEVIL_REVISION}

dnl
dnl Note that the libtool version has nothing to do
dnl with the actual DevIL version, it is a version number
dnl describing the interface version (see libtool doc)
dnl wich only changes when the interface changes  
dnl
DEVIL_LTVERSION=1:0:0
AC_SUBST(DEVIL_LTVERSION)

ADD_CFLAGS([$CFLAGS])

dnl
dnl Test build options
dnl
AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [Compile a debug version of the library])],
              [],
	      [enable_debug="no"])

AC_ARG_ENABLE([release],
	      [AS_HELP_STRING([--disable-release],
			      [Compile a debug version of the library, default enabled if not building debug version])],
              [],
	      [enable_release="yes"
	       test "x$enable_debug" = "xyes" && enable_release="no"])

AC_ARG_ENABLE([asm],
	      [AS_HELP_STRING([--disable-asm],
			      [Use assembler routines if possible, default enable if not building debug version])],
	      [],
	      [enable_asm="yes"
	       test "x$enable_debug" = "xyes" && enable_asm="no"])

AS_IF([test "x$enable_release" = "xyes"],
      [ADD_CFLAGS([-O3])],
      [test "x$enable_debug" = "xyes"],
      [ADD_CFLAGS([-DNOINLINE -Wall -g -O0])
       AC_DEFINE([DEBUG],
		 [],
		 ["Enable debug code features"]) ])

dnl
dnl Basic conf
dnl

SUPPORTED_FORMATS=""
SUPPORTED_API=""
SUPPORTED_EXTENSION=""
AC_REVISION([$Revision: 1.36 $])
# AC_CONFIG_AUX_DIR(libtools)
dnl AC_PREFIX_DEFAULT(/usr/local)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
dnl AC_LANG([C++])
AC_LANG([C])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_C_BIGENDIAN
dnl AC_LIBTOOL_WIN32_DLL
AC_PATH_XTRA

AC_ARG_ENABLE([monolithic],
	      [AS_HELP_STRING([--enable-monolithic],
			      [Builds only one library instead of separate ones (IL, ILU, ILUT -> DevIL). Default set to 'disabled'])],
              [],
	      [enable_monolithic="no"])
AM_CONDITIONAL([BUILD_MONOLITHIC],
	       [test "x$enable_monolithic" = "xyes"])

AC_ARG_ENABLE([IL],
	      [AS_HELP_STRING([--enable-IL[[=yes/no]]],
			      [Build IL part of DevIL. Default set to yes])],
              [],
	      [enable_IL="yes"])
AM_CONDITIONAL([BUILD_IL],
	       [test "x$enable_IL" = "xyes"])

AC_ARG_ENABLE([ILU],
	      [AS_HELP_STRING([--enable-ILU[[=yes/no]]],
			      [Build ILU part of DevIL. Default set to no])],
              [],
	      [enable_ILU="no"])
AM_CONDITIONAL([BUILD_ILU],
	       [test "x$enable_ILU" = "xyes"])

AC_ARG_ENABLE([ILUT],
	      [AS_HELP_STRING([--enable-ILUT[[=yes/no]]],
			      [Build ILUT part of DevIL. Default set to no])],
              [],
	      [enable_ILUT="no"])
AM_CONDITIONAL([BUILD_ILUT],
	       [test "x$enable_ILUT" = "xyes"])


dnl Test for restrict c/c++ keyword
AC_MSG_CHECKING([restrict keyword])
AC_COMPILE_IFELSE([[void foo(void * restrict test){}]],
	          [AC_DEFINE([RESTRICT_KEYWORD],
			     [],
			     [restric keyword available])
	           restrict_keyword="found"]
	          [AC_MSG_RESULT([yes])],
		  [AC_MSG_RESULT([no])])

AS_IF([test ! "$restrict_keyword" = "found"],
dnl checking if the -std=gnu99 flag is required
      [TMP_CFLAGS="$CFLAGS"
       CFLAGS="$CFLAGS -std=gnu99"
       AC_MSG_CHECKING([restrict gnu99 keyword])
       AC_COMPILE_IFELSE([[void foo(void * restrict test){}]],
	                 [AC_DEFINE([RESTRICT_KEYWORD],
				    [],
				    [restric keyword available])
	                  AC_MSG_RESULT(yes)
	                  CFLAGS="$TMP_CFLAGS"
                          ADD_CFLAGS(-std=gnu99 -fgnu89-inline)],
			 [AC_MSG_RESULT(no)
	                  CFLAGS="$TMP_CFLAGS"]) ])

dnl must add AC_CACHE_CHECK support where possible

AM_INIT_AUTOMAKE(["DevIL"], 
		 [${DEVIL_MAJOR}.${DEVIL_MINOR}.${DEVIL_REVISION}])
AM_CONFIG_HEADER([include/IL/config.h])
AM_DISABLE_STATIC
AM_PROG_LIBTOOL
dnl AM_MAINTAINER_MODE

TEST_EXT([Altivec],
	 [-faltivec -maltivec],
	 [vector int vec;vec=vec_splat_u32(0);],
	 [ALTIVEC_GCC],
	 [],
	 [altivec])

TEST_EXT([SSE],
	 [-msse],
	 [__m128 v;v=_mm_sub_ss(v,v);],
	 [SSE],
	 [#include <xmmintrin.h>],
	 [sse])

TEST_EXT([SSE2],
	 [-msse2],
	 [__m128i v;v=_mm_add_epi32(v,v);],
	 [SSE2],
	 [#include <emmintrin.h>],
	 [sse2])

TEST_EXT([SSE3],
	 [-msse3],
	 [__m128d v;v=_mm_addsub_pd(v,v);],
	 [SSE3],
	 [#include <pmmintrin.h>],
	 [sse3])

AS_IF([test "x$enable_asm" = "xyes"],
      [TEST_ASM([i[[3456]]86*],
                [X86_ASM],
	        [GCC_X86_ASM],
                [x86])
       TEST_ASM([x86_64*],
                [X86_64_ASM],
                [GCC_X86_64_ASM],
                [x86_64])
       TEST_ASM([powerpc*],
                [PPC_ASM],
                [GCC_PCC_ASM],
                [ppc]) ])

dnl
dnl Test warning build options
dnl
AC_ARG_ENABLE([warning],
              [AC_HELP_STRING([--enable-warning],
			      [Compile breaks on warnings])],
	      [build_warning="$enable_warning"],
	      [build_warning="no"])

AS_IF([test "$build_warning" = "yes"],
      [ADD_CFLAGS([-Wall -Werror])],
      [])

dnl
dnl Memory alignment functions
dnl
AC_CHECK_FUNC([valloc], 
	      [AC_DEFINE([VALLOC],
			 [],
			 [valloc memory allocation]) ])
AC_CHECK_FUNC([posix_memalign], 
	      [AC_DEFINE([POSIX_MEMALIGN],
			 [],
			 [posix_memalign memory allocation]) ])

AC_CHECK_FUNC([memalign], 
	      [AC_DEFINE([MEMALIGN],
			 [],
			 [memalign memory allocation]) ])

AC_CHECK_HEADER([mm_malloc.h],
		[AC_DEFINE([MM_MALLOC],
			   [],
			   [mm_malloc memory allocation]) ])

dnl
dnl Libraries development checking
dnl
AC_CHECK_HEADER([jpeglib.h],
		[],
		[no_jpeg="yes"])

AC_CHECK_HEADER([tiffio.h],
		[],
		[no_tiff="yes"])

AC_CHECK_HEADER([lcms/lcms.h],
		[],
		[no_lcms="yes"])

AC_CHECK_HEADER([lcms.h],
		[lcms_nodirinclude=yes])

AC_CHECK_HEADER([libmng.h],
		[],
		[no_mng="yes"])

AC_CHECK_HEADER([jasper/jasper.h], 
		[], 
		[no_jp2="yes"])

AC_CHECK_HEADER([zlib.h], 
		[use_z="yes"], 
		[use_z="no"])

dnl Special code for LCMS
AS_IF([test "x$lcms_nodirinclude" = "xyes"],
      [no_lcms="no"],
      [AC_DEFINE([LCMS_NODIRINCLUDE],
		 [],
		 [LCMS include without lcms/ support]) ])

dnl Special code for libjpeg
AC_DEFINE([IL_USE_JPEGLIB_UNMODIFIED],
	  [],
	  [Use libjpeg without modification. always enabled.])


SET_LIB([jpeg])

SET_LIB([tiff])

SET_LIB([lcms])

SET_LIB([mng])

SET_LIB([jp2])

SET_LIB([png])

SET_LIB([z])

dnl libpng configuration
AC_MSG_CHECKING([libpng])
LIBPNG_INCLUDE=""
LIBPNG=""
AC_ARG_WITH([png],
	    [AC_HELP_STRING([--with-pngdir=path],
			    [Specify the libpng installation path])],
            [libpng_app="$withval/bin/libpng-config"
             no_png="no"])

AS_IF([test ! "$with_png" = "no"],
      [AS_IF([test -z "$libpng_app"],
	     [libpng_app=$(which libpng-config)]) ])

AS_IF([test ! -e "$libpng_app"],
      [libpng_app=""])

AS_IF([test -z "$libpng_app"],
      [AC_DEFINE([IL_NO_PNG],
		 [],
		 [PNG support])
       AC_MSG_RESULT([no])
       no_png="yes"],
      [LIBPNG=`$libpng_app --ldflags`
       LIBPNG_INCLUDE=`$libpng_app --cflags`
       AC_MSG_RESULT([yes])
       no_png="no"])
dnl Just in case the library is not installed in the default header path

TEST_FORMAT([bmp], [BMP])
TEST_FORMAT([dcx], [DCX])
TEST_FORMAT([doom], [WAD])
TEST_FORMAT([gif], [GIF])
TEST_FORMAT([hdr], [HDR])
TEST_FORMAT([icns], [ICNS])
TEST_FORMAT([icon], [ICON])
TEST_FORMAT([jpeg], [JPG])
TEST_FORMAT([jp2], [JP2])
TEST_FORMAT([lcms], [LCMS])
TEST_FORMAT([lif], [LIF])
TEST_FORMAT([mdl], [MDL])
TEST_FORMAT([mng], [MNG])
TEST_FORMAT([pcx], [PCX])
TEST_FORMAT([pcd], [PCD])
TEST_FORMAT([pic], [PIC])
TEST_FORMAT([pix], [PIX])
TEST_FORMAT([png], [PNG])
TEST_FORMAT([pnm], [PNM])
TEST_FORMAT([psd], [PSD])
TEST_FORMAT([psp], [PSP])
TEST_FORMAT([pxr], [PXR])
TEST_FORMAT([raw], [RAW])
TEST_FORMAT([sgi], [SGI])
TEST_FORMAT([tga], [TGA])
TEST_FORMAT([tiff], [TIF])
TEST_FORMAT([xpm], [XPM])
TEST_FORMAT([wal], [WAL])

AC_DEFINE([IL_NO_EXR],
	  [],
	  [The EXR support is causing problems right now])
AC_DEFINE([IL_NO_WDP],
	  [],
	  [The WDP support is causing problems as well])

AS_IF([test "$no_png" = "yes"],
      [LIBPNG=""
       LIBPNG_INCLUDE=""])
AC_SUBST([LIBPNG])
AC_SUBST([LIBPNG_INCLUDE])

TEST_API([allegro])
TEST_API([directx8])
TEST_API([directx9])
TEST_API([opengl])
TEST_API([sdl])
TEST_API([win32])
TEST_API([x11])
TEST_API([xshm])

dnl
dnl ILUT APIs library checking
dnl

dnl if AM_PATH_SDL is missing, replace it with a dummy test
dnl This is not going to be the case since the sdl.m4 is in m4 dir from now on :-)

dnl SDL
HSDL=""
LIBSDL=""
AS_IF([test "$use_sdl" = "yes"],
      [AM_PATH_SDL(1.2.5,
                   [use_sdl="yes"],
		   [use_sdl="no"]) ])
AM_CONDITIONAL([USE_SDL],
	       [test $use_sdl = "yes"])

AS_IF([test "$use_sdl" = "yes"],
      [HSDL="$SDL_CFLAGS"
       LIBSDL="$SDL_LIBS"
       AC_DEFINE([ILUT_USE_SDL],
		 [],
		 [Support SDL API])
       SUPPORTED_API=$SUPPORTED_API"SDL "])

AC_SUBST([LIBSDL])
AC_SUBST([HSDL])

dnl X11
LIBX11=""
AS_IF([test "$use_x11" = "yes"],
      [AC_CHECK_HEADER([X11/Xlib.h],
		       [],
		       [use_x11=no])
       AS_IF([test "$use_x11" = "yes"],
	     [AC_DEFINE([ILUT_USE_X11],
			[],
			[Support X11 API])
              SUPPORTED_API=$SUPPORTED_API"X11 "
	      AC_CHECK_HEADER([X11/extensions/XShm.h],
			      [],
			      [use_xshm=no],
			      [[#include <X11/Xlib.h>]])
	      AS_IF([test "$use_xshm" = "yes"],
		    [AC_DEFINE([ILUT_USE_XSHM],
			       [],
			       [Support X11 XShm extension])
		     SUPPORTED_API=$SUPPORTED_API"XShm ";
		     LIBX11="-lX11 -lXext"])
	      AS_CASE([$target],
		      [*apple-darwin*],
		      [LIBX11="$LIBX11 -L/usr/X11R6/lib"]) ]) ])
AC_SUBST([LIBX11])

dnl OpenGL
AS_CASE([$target],
        [*apple-darwin*],
	[AC_CHECK_HEADER(OpenGL/gl.h,,use_opengl=no)],
        [AC_CHECK_HEADER(GL/gl.h,,use_opengl=no) ])

AS_IF([test "$use_opengl" = "yes"],
      [AS_CASE([$target],
               [*-*-cygwin* | *-*-mingw32*],
               [LIBGL="-lopengl32 -lglu32"],
               [*-apple-darwin*],
               [LIBGL='-Wl,-framework -Wl,OpenGL'],
               [AC_CHECK_LIB([GL],
			     [glLoadIdentity],
			     [LIBGL="-lGL"])
	        AC_CHECK_LIB([GLU],
			     [gluPerspective],
			     [LIBGL="-lGLU"]) ])
       AC_DEFINE([ILUT_USE_OPENGL],
	         [],
	         [Support OpenGL API])
       SUPPORTED_API=$SUPPORTED_API"OpenGL "],
      [LIBGL=""])
AC_SUBST([LIBGL])

dnl Allegro
LIBALLEGRO=""
HALLEGRO=""
AS_IF([test "$use_allegro" = "yes"],
      [AC_CHECK_PROG([allegro_config], 
		     [allegro-config], 
		     [yes], 
		     [no], 
		     []) ])

AS_IF([test "$allegro_config" = "yes"],
      [AC_DEFINE([ILUT_USE_ALLEGRO],
		 [],
		 [Support Allegro API])
       SUPPORTED_API=$SUPPORTED_API"Allegro "
       LIBALLEGRO=`allegro-config --libs`
       HALLEGRO=`allegro-config --cflags`])

AM_CONDITIONAL([USE_ALLEGRO],
	       [test "$use_allegro" = "yes"])

AC_SUBST([LIBALLEGRO])
AC_SUBST([HALLEGRO])

dnl Win32
AS_CASE([$host],
        [*mingw32*],
        [LIBIL_CFLAGS="$LIBIL_CFLAGS -mno-cygwin"
         LIBILU_CFLAGS="$LIBILU_CFLAGS -mno-cygwin"
         LIBILUT_CFLAGS="$LIBILUT_CFLAGS -mno-cygwin"
         VERSIONINFO=""],
        [*cygwin*],
        [LIBILUT_WIN32LIBPATH="-L/usr/lib/w32api"
         VERSIONINFO=""],
        [use_win32="no"
         VERSIONINFO="-version-info $DEVIL_LTVERSION"])

AC_SUBST([VERSIONINFO])
AS_IF([test "$use_win32" = "no"],
      [LIBGDI32=""],
      [LIBGDI32="-lgdi32"])

AC_SUBST([LIBGDI32])

dnl DirectX 8
AS_IF([test $use_directx8 = yes],
      [AC_CHECK_HEADER([d3d8.h],
		       [],
		       [use_directx8=no]) ])

AS_IF([test $use_directx8 = yes],
      [AC_DEFINE([ILUT_USE_DIRECTX8],
		 [],
		 [Support DirectX8 API])
       SUPPORTED_API=$SUPPORTED_API"DirectX8 "],
       [])

dnl DirectX 9
AS_IF([test $use_directx9 = yes],
      [AC_CHECK_HEADER([d3d9.h],
		       [],
		       [use_directx9=no]) ])

AS_IF([test $use_directx9 = yes],
      [AC_DEFINE([ILUT_USE_DIRECTX9],
		 [],
		 [Support DirectX9 API])
       SUPPORTED_API=$SUPPORTED_API"DirectX9 "],
       [])

dnl Mac OS X Specific test
AS_CASE([$target],
        [*-apple-darwin*],
	[AC_DEFINE([MAX_OS_X],
		   [],
		   [Building on Mac OS X])],
        [])

dnl If we're on Windows we need the -no-undefined flag to get dlls
WIN32_EXTRA_LDFLAGS=

AC_MSG_CHECKING([for the suffix of shared libraries])
AS_CASE([$host_os],
	[cygwin* | mingw*],
	[WIN32_EXTRA_LDFLAGS="-no-undefined"])

AC_SUBST([WIN32_EXTRA_LDFLAGS])

dnl Final substitution
AC_SUBST([LIBIL_CFLAGS])
AC_SUBST([LIBILU_CFLAGS])
AC_SUBST([LIBILUT_CFLAGS])
AC_SUBST([LIBILUT_WIN32LIBPATH])

dnl
dnl Ouput files to generate
dnl note: keep the list on one line separated by single spaces 
dnl (otherwise MSYS compilation breaks)
AC_CONFIG_FILES([Makefile lib/Makefile docs/Makefile]) 
# src-IL/Makefile  src-IL/src/Makefile  src-IL/include/Makefile  src-ILU/Makefile  src-ILU/src/Makefile  src-ILU/include/Makefile  src-ILUT/Makefile  src-ILUT/src/Makefile  src-ILUT/include/Makefile  include/Makefile  include/IL/Makefile
AC_OUTPUT

echo
echo +----------------------------------------+
echo \|     IL library  Supported formats      \|
echo +----------------------------------------+-------+
echo $SUPPORTED_FORMATS
echo
echo +----------------------------------------+
echo \|      ILUT library  Supported APIs      \|
echo +----------------------------------------+-------+
echo $SUPPORTED_API
echo
echo +----------------------------------------+
echo \|      Detected Machine Extensions       \|
echo +----------------------------------------+-------+
echo $SUPPORTED_EXTENSION
echo
