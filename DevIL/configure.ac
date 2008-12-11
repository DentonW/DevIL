dnl 
dnl Autoconf script for DevIL Library
dnl Written by Meloni Dario
dnl modified by Matěj Týč

dnl
dnl configure.in initialization
dnl
AC_INIT([DevIL], 
	1.7.3,
	[bubla@users.sourceforge.net])
AC_CONFIG_HEADERS([include/IL/config.h])
AC_CONFIG_SRCDIR([src-IL/src/il_main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

dnl
dnl Minimum Autoconf version
dnl
AC_PREREQ(2.52)

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
AC_REVISION([$Revision: 1.39 $])
dnl AC_PREFIX_DEFAULT(/usr/local)
AC_CANONICAL_HOST
dnl AC_LANG([C++])
AC_LANG([C])
AC_PROG_CC
AC_PROG_CXX
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
	      [AS_HELP_STRING([--with-IL[[=yes/no]]],
	                      [Build IL part of DevIL. Default set to yes])],
              [],
	      [enable_IL="yes"])
AM_CONDITIONAL([BUILD_IL],
	       [test "x$enable_IL" = "xyes"])

AC_ARG_ENABLE([ILU],
	      [AS_HELP_STRING([--with-ILU[[=yes/no]]],
			      [Build ILU part of DevIL. Default set to no])],
              [],
	      [enable_ILU="no"])
AM_CONDITIONAL([BUILD_ILU],
	       [test "x$enable_ILU" = "xyes"])

AC_ARG_ENABLE([ILUT],
	      [AS_HELP_STRING([--with-ILUT[[=yes/no]]],
			      [Build ILUT part of DevIL. Default set to no])],
              [],
	      [enable_ILUT="no"])
AM_CONDITIONAL([BUILD_ILUT],
	       [test "x$enable_ILUT" = "xyes"])

dnl Test for restrict c/c++ keyword
DEVIL_CHECK_RESTRICT
dnl checking if the -std=gnu99 flag is required
AS_IF([test  "x$restrict_keyword" = "xfound"],
      [DEVIL_CHECK_RESTRICT_GNU99])

dnl must add AC_CACHE_CHECK support where possible

AM_INIT_AUTOMAKE([-Wall])
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
			 [1],
			 [define if you have valloc memory allocation]) ])
AC_CHECK_FUNC([posix_memalign], 
	      [AC_DEFINE([POSIX_MEMALIGN],
			 [1],
			 [define if you have posix_memalign memory allocation]) ])

AC_CHECK_FUNC([memalign], 
	      [AC_DEFINE([MEMALIGN],
			 [1],
			 [define if you have memalign memory allocation]) ])

AC_CHECK_HEADER([mm_malloc.h],
		[AC_DEFINE([MM_MALLOC],
			   [1],
			   [define if you have mm_malloc.h header]) ])

dnl
dnl Libraries development checking
dnl

AC_ARG_WITH([zlib],
            [AC_HELP_STRING([--with-zlib=[[yes/no]]],
                            [Do wou want to use zlib?])],
	    [],
            [enable_zlib="yes"])

AS_IF([test "x$enable_zlib" = "xyes"],
      [AC_CHECK_HEADER([zlib.h], 
		       [use_z="yes"], 
		       [use_z="no"]) 
       AS_IF([test "$enable_zlib" = "yes"],
             [AC_CHECK_LIB([z],
                           [main]) ]) ])

TEST_FORMAT([bmp], [BMP])
TEST_FORMAT([dcx], [DCX])
TEST_FORMAT([doom],[WAD])
TEST_FORMAT([gif], [GIF])
TEST_FORMAT([hdr], [HDR])
TEST_FORMAT([icns],[ICNS])
TEST_FORMAT([icon],[ICON])
TEST_FORMAT([jpeg],[JPG], [SETTLE_JPEG])
TEST_FORMAT([jp2], [JP2], [SETTLE_JASPER])
TEST_FORMAT([lcms],[LCMS], [SETTLE_LCMS])
TEST_FORMAT([lif], [LIF])
TEST_FORMAT([mdl], [MDL])
TEST_FORMAT([mng], [MNG], [SETTLE_MNG])
TEST_FORMAT([pcx], [PCX])
TEST_FORMAT([pcd], [PCD])
TEST_FORMAT([pic], [PIC])
TEST_FORMAT([pix], [PIX])
TEST_FORMAT([png], [PNG], [SETTLE_PNG])
TEST_FORMAT([pnm], [PNM])
TEST_FORMAT([psd], [PSD])
TEST_FORMAT([psp], [PSP])
TEST_FORMAT([pxr], [PXR])
TEST_FORMAT([raw], [RAW])
TEST_FORMAT([sgi], [SGI])
TEST_FORMAT([tga], [TGA])
TEST_FORMAT([tiff],[TIF], [SETTLE_TIFF])
TEST_FORMAT([xpm], [XPM])
TEST_FORMAT([wal], [WAL])

AC_DEFINE([IL_NO_EXR],
	  [],
	  [The EXR support is causing problems right now])
AC_DEFINE([IL_NO_WDP],
	  [],
	  [The WDP support is causing problems as well])

dnl
dnl ILUT APIs library checking
dnl
AS_IF([test "x$enable_ILUT" = "xyes"],
      [SETTLE_SDL
       SETTLE_X11
       SETTLE_OPENGL
       SETTLE_ALLEGRO
       SETTLE_DX8
       SETTLE_DX9
       SETTLE_W32])
 
dnl AM_CONDITIONAL([USE_SDL],
dnl               [test "x$use_sdl" = "xyes"])
 
dnl AM_CONDITIONAL([USE_ALLEGRO],
dnl	       [test "x$use_allegro" = "xyes"])

dnl Win32
AS_CASE([$host],
        [*mingw32*],
        [LIBIL_CFLAGS="$LIBIL_CFLAGS -mno-cygwin"
         LIBILU_CFLAGS="$LIBILU_CFLAGS -mno-cygwin"
         LIBILUT_CFLAGS="$LIBILUT_CFLAGS -mno-cygwin"],
        [*cygwin*],
        [LIBILUT_WIN32LIBPATH="-L/usr/lib/w32api"],
        [*darwin*],
	[AC_DEFINE([MAX_OS_X],
		   [],
		   [Building on Mac OS X]) ])

dnl AC_SUBST([VERSIONINFO])

dnl If we're on Windows we need the -no-undefined flag to get dlls
WIN32_EXTRA_LDFLAGS=

dnl AC_MSG_CHECKING([for the suffix of shared libraries])
AS_CASE([$host_os],
	[cygwin* | mingw*],
	[WIN32_EXTRA_LDFLAGS="-no-undefined"])

dnl AC_SUBST([WIN32_EXTRA_LDFLAGS])

dnl Final substitution
AC_SUBST([IL_CFLAGS])
AC_SUBST([IL_LIBS])
AC_SUBST([ILU_CFLAGS])
AC_SUBST([ILU_LIBS])
AC_SUBST([ILUT_CFLAGS])
AC_SUBST([ILUT_LIBS])
AC_SUBST([LIBILUT_WIN32LIBPATH])

dnl
dnl Ouput files to generate
dnl note: keep the list on one line separated by single spaces 
dnl (otherwise MSYS compilation breaks)
AC_CONFIG_FILES([Makefile lib/Makefile docs/Makefile data/DevIL.pc data/IL.pc data/ILU.pc data/ILUT.pc]) 
# src-IL/Makefile  src-IL/src/Makefile  src-IL/include/Makefile  src-ILU/Makefile  src-ILU/src/Makefile  src-ILU/include/Makefile  src-ILUT/Makefile  src-ILUT/src/Makefile  src-ILUT/include/Makefile  include/Makefile  include/IL/Makefile
AC_OUTPUT

AC_MSG_NOTICE([+----------------------------------------+])
AS_IF([test "x$enable_IL" = "xyes"],
      [AC_MSG_NOTICE([\|     IL library  Supported formats      \|])
       AC_MSG_NOTICE([+----------------------------------------+-------+])
       AC_MSG_NOTICE([$SUPPORTED_FORMATS]) ],
       [AC_MSG_NOTICE([\|     IL disabled (huh?)     \|]) ])

AC_MSG_NOTICE([+----------------------------------------+])
AS_IF([test "x$enable_ILU" = "xyes"],
      [AC_MSG_NOTICE([\|     ILU part enabled      \|]) ],
      [AC_MSG_NOTICE([\|     ILU part disabled      \|]) ])

AC_MSG_NOTICE([+----------------------------------------+])
AS_IF([test "x$enable_ILUT" = "xyes"],
      [AC_MSG_NOTICE([\|      ILUT library  Supported APIs      \|])
      AC_MSG_NOTICE([+----------------------------------------+-------+])
      AC_MSG_NOTICE([$SUPPORTED_API])],
      [AC_MSG_NOTICE([\|     ILUT part disabled      \|]) ])

AC_MSG_NOTICE([+----------------------------------------+])
AC_MSG_NOTICE([\|      Detected Machine Extensions       \|])
AC_MSG_NOTICE([+----------------------------------------+-------+])
AC_MSG_NOTICE([$SUPPORTED_EXTENSION])
AC_MSG_NOTICE([+----------------------------------------+])
