dnl 
dnl Autoconf script for DevIL Library
dnl Written by Meloni Dario
dnl 

dnl
dnl configure.in initialization
dnl
AC_INIT
AC_CONFIG_SRCDIR([README])

dnl
dnl Minimum Autoconf version
dnl
AC_PREREQ(2.52)

DEVIL_MAJOR=1
DEVIL_MINOR=6
DEVIL_REVISION=8
DEVIL_VERSION=$DEVIL_MAJOR$DEVIL_MINOR$DEVIL_REVISION

dnl
dnl Note that the libtool version has nothing to do
dnl with the actual DevIL version, it is a version number
dnl describing the interface version (see libtool doc)
dnl wich only changes when the interface changes  
dnl
DEVIL_LTVERSION=1:0:0
AC_SUBST(DEVIL_LTVERSION)

dnl
dnl Basic conf
dnl

SUPPORTED_FORMATS=""
SUPPORTED_API=""
SUPPORTED_EXTENSION=""
AC_REVISION($Revision: 1.14 $)
AC_CONFIG_AUX_DIR(libtools)
AC_PREFIX_DEFAULT(/usr/local)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_LANG([C++])
AC_LANG([C])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_C_BIGENDIAN
AC_LIBTOOL_WIN32_DLL
AC_PATH_XTRA


AC_DEFUN([ADD_CFLAGS],[
LIBIL_CFLAGS="$LIBIL_CFLAGS $1"
LIBILU_CFLAGS="$LIBILU_CFLAGS $1"
LIBILUT_CFLAGS="$LIBILUT_CFLAGS $1"
CFLAGS="$CFLAGS $1"
])

ADD_CFLAGS([$CFLAGS])

dnl must add AC_CACHE_CHECK support where possible

AM_INIT_AUTOMAKE("DevIL", $DEVIL_MAJOR.$DEVIL_MINOR.$DEVIL_REVISION)
AM_CONFIG_HEADER(include/IL/config.h)
AM_DISABLE_STATIC
AM_PROG_LIBTOOL
AM_MAINTAINER_MODE

dnl
dnl Check CPU Extensions
dnl
AC_DEFUN([TEST_EXT],[
AC_ARG_ENABLE($6,AC_HELP_STRING([--disable-$6],[Doesn't compile $1 support]),
[use_$1=$enable_$1],[use_$1=yes])
AC_MSG_CHECKING([$1])
CFLAGS_TMP=$CFLAGS
CFLAGS=$CFLAGS" $2"
AC_COMPILE_IFELSE([
$5
int main() {
$3
return 0;}]
,[
if test "$use_$1" = "yes" ; then
  AC_DEFINE([$4],,[$1 extension found])
  AC_DEFINE([VECTORMEM],,[Memory must be vector aligned])
  ADD_CFLAGS([$2])
  AC_MSG_RESULT(yes)
  SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "
else
  AC_MSG_RESULT(disabled)
fi]
,[AC_MSG_RESULT(no)])
 [CFLAGS=$CFLAGS_TMP]
]) 

TEST_EXT([Altivec],[-faltivec -maltivec],[vector int vec;vec=vec_splat_u32(0);],[ALTIVEC_GCC],[],[altivec])
TEST_EXT([SSE],[-msse],[__m128 v;v=_mm_sub_ss(v,v);],[SSE],[#include <xmmintrin.h>],[sse])
TEST_EXT([SSE2],[-msse2],[__m128i v;v=_mm_add_epi32(v,v);],[SSE2],[#include <emmintrin.h>],[sse2])
TEST_EXT([SSE3],[-msse3],[__m128d v;v=_mm_addsub_pd(v,v);],[SSE3],[#include <emmintrin.h>],[sse3])

dnl
dnl Check CPU Assembly
dnl
AC_DEFUN([TEST_ASM],[
AC_MSG_CHECKING([$4])
case $host_cpu in 
	$1) SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$2 "
		AC_DEFINE([$3],,[$2 assembly found]) 
		AC_MSG_RESULT(yes)   ;;
	*)	AC_MSG_RESULT(no)    ;;
esac
]) 

TEST_ASM([i[[3456]]86*],[X86_ASM],[GCC_X86_ASM],x86)
TEST_ASM([x86_64*],[X86_64_ASM],[GCC_X86_64_ASM],x86_64)
TEST_ASM([powerpc*],[PPC_ASM],[GCC_PCC_ASM],ppc)

dnl
dnl Test release build options
dnl
AC_ARG_ENABLE(release,
AC_HELP_STRING([--disable-release],[Compile a debug version of the library]),
[build_release="$enable_release"],[build_release="no"])

if test "$build_release" = "no" ; then
	ADD_CFLAGS([-Wall -g -O0])
else
	ADD_CFLAGS([-O3])
fi

dnl
dnl Test warning build options
dnl
AC_ARG_ENABLE(warning,
AC_HELP_STRING([--enable-warning],[Compile breaks on warnings]),
[build_warning="$enable_warning"],[build_warning="no"])

if test "$build_warning" = "yes" ; then
	ADD_CFLAGS([-Wall -g -Werror -DNOINLINE])
fi

dnl
dnl Memory alignment functions
dnl
AC_CHECK_FUNC(  valloc, [AC_DEFINE([VALLOC],,[valloc memory allocation])] )
AC_CHECK_FUNC(  posix_memalign, [AC_DEFINE([POSIX_MEMALIGN],,[posix_memalign memory allocation])] )
AC_CHECK_FUNC(  memalign, [AC_DEFINE([MEMALIGN],,[memalign memory allocation])])
AC_CHECK_HEADER(mm_malloc.h,[AC_DEFINE([MM_MALLOC],,[mm_malloc memory allocation])])

dnl
dnl Libraries development checking
dnl
AC_CHECK_HEADER(jpeglib.h,,no_jpeg=yes)
AC_CHECK_HEADER(tiffio.h,,no_tiff=yes)
dnl AC_CHECK_HEADER(png.h,,no_png=yes)
no_png=no
AC_CHECK_HEADER(lcms/lcms.h,,no_lcms=yes)
AC_CHECK_HEADER(lcms.h,lcms_nodirinclude=yes)
AC_CHECK_HEADER(libmng.h,,no_mng="yes")
AC_CHECK_HEADER(zlib.h, use_z=yes, use_z=no)

dnl Special code for LCMS
if test "$lcms_nodirinclude" = "yes" ; then
	no_lcms=no
else
	AC_DEFINE(LCMS_NODIRINCLUDE,,[LCMS include without lcms/ support])	
fi

dnl
dnl Formats
dnl
AC_DEFUN([TEST_FORMAT],[
AC_ARG_ENABLE([$1],AC_HELP_STRING([--disable-$1],[Doesn't compile $1 support]),[use_$1="$enable_$1"],[use_$1="yes"])
if test "$no_$1" = "yes" ; then	
	AC_DEFINE([IL_NO_$2],,[$2 support])
else
if test "$use_$1" = "yes" ; then
	SUPPORTED_FORMATS=$SUPPORTED_FORMATS"$2 "
else
	AC_DEFINE([IL_NO_$2],,[$2 support])
fi
fi
])

TEST_FORMAT(bmp,BMP)
TEST_FORMAT(dcx,DCX)
TEST_FORMAT(doom,WAD)
TEST_FORMAT(gif,GIF)
TEST_FORMAT(hdr,HDR)
TEST_FORMAT(icon,ICON)
TEST_FORMAT(jpeg,JPG)
TEST_FORMAT(lcms,LCMS)
TEST_FORMAT(lif,LIF)
TEST_FORMAT(mdl,MDL)
TEST_FORMAT(mng,MNG)
TEST_FORMAT(pcx,PCX)
TEST_FORMAT(pcd,PCD)
TEST_FORMAT(pic,PIC)
TEST_FORMAT(pix,PIX)
TEST_FORMAT(png,PNG)
TEST_FORMAT(pnm,PNM)
TEST_FORMAT(psd,PSD)
TEST_FORMAT(psp,PSP)
TEST_FORMAT(pxr,PXR)
TEST_FORMAT(raw,RAW)
TEST_FORMAT(sgi,SGI)
TEST_FORMAT(tga,TGA)
TEST_FORMAT(tiff,TIF)
TEST_FORMAT(xpm,XPM)
TEST_FORMAT(wal,WAL)

dnl Special code for libjpeg
AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED,,[Use libjpeg without modification. always enabled.])

dnl
dnl Setup Libraries
dnl
AC_DEFUN([SET_LIB],[
LIB$2=""
AC_ARG_ENABLE($1dir,AC_HELP_STRING([--with-$1dir=path],[Specify the lib$1 installation path]),
	ADD_CFLAGS([-I$enable_$1dir/include])
	LIB$2="-L$enable_$1dir/lib"
	no_$1=no
	use_$1=yes )
if test ! "$no_$1" = "yes" ; then
	if test "$use_$1" = "yes" ; then
		if test -z "$3" ; then
			LIB$2="$LIB$2 -l$1"
		else
			LIB$2="$3"
		fi
	fi
fi
AC_SUBST(LIB$2) ])

SET_LIB(jpeg,JPG)
SET_LIB(tiff,TIFF)
SET_LIB(lcms,LCMS)
SET_LIB(mng,MNG)
SET_LIB(z,Z)

libpng_app=$(which libpng-config)
if [ -z "$libpng_app" ] ; then
	LIBPNG=""
	AC_SUBST(LIBPNG)
else
	SET_LIB(png,PNG,`libpng-config --ldflags`)
fi

dnl Just in case the library is not installed in the default header path
LIBPNG_INCLUDE=`libpng-config --I_opts`
AC_SUBST(LIBPNG_INCLUDE)

dnl
dnl ILUT APIs checking
dnl
AC_DEFUN([TEST_API],[
AC_ARG_ENABLE($1,AC_HELP_STRING([--disable-$1],[Doesn't compile $1 support]),
[use_$1=$enable_$1],[use_$1=yes])])

TEST_API(allegro)
TEST_API(directx8)
TEST_API(directx9)
TEST_API(opengl)
TEST_API(sdl)
TEST_API(win32)

dnl
dnl ILUT APIs library checking
dnl

dnl if AM_PATH_SDL is missing, replace it with a dummy test
m4_ifndef([AM_PATH_SDL],[AC_DEFUN([AM_PATH_SDL],[
	AC_MSG_CHECKING([SDL $1])
  	AC_MSG_RESULT(missing macros)
  	$3])])

dnl SDL
HSDL=""
LIBSDL=""
if test "$use_sdl" = "yes" ; then
	AM_PATH_SDL(1.2.5,use_sdl="yes",use_sdl="no")
	if test "$use_sdl" = "yes" ; then
		HSDL="$SDL_CFLAGS"
		LIBSDL="$SDL_LIBS"
		AC_DEFINE(ILUT_USE_SDL,,[Support SDL API])
  		SUPPORTED_API=$SUPPORTED_API"SDL "
  	fi
fi
AC_SUBST(LIBSDL)
AC_SUBST(HSDL)

dnl OpenGL
case "$target" in
 	*apple-darwin*)
		AC_CHECK_HEADER(OpenGL/gl.h,,use_opengl=no) ;;
	*)
		AC_CHECK_HEADER(GL/gl.h,,use_opengl=no) ;;
esac
if test "$use_opengl" = "yes" ; then
case "$target" in
*-*-cygwin* | *-*-mingw32*)
	LIBGL="-lopengl32 -lglu32"
	;;
*-apple-darwin*)
	dnl must test if the LIBILUT CFLAGS done here is really usefull. should not...
	LIBGL="-Wl,-framework -Wl,OpenGL"
	;;
*-*-*)
	AC_CHECK_LIB(GL,glLoadIdentity,[LIBGL="-lGL"])
	AC_CHECK_LIB(GLU,gluPerspective,[LIBGL="-lGLU"])
	;;
esac
   AC_DEFINE(ILUT_USE_OPENGL,,[Support OpenGL API])
   SUPPORTED_API=$SUPPORTED_API"OpenGL "
else
  LIBGL=""
fi
AC_SUBST(LIBGL)

dnl Allegro
LIBALLEGRO=""
HALLEGRO=""
if test $use_allegro = yes ; then
  AC_CHECK_PROG(allegro_config, allegro-config, yes, no, )
  if test $allegro_config = yes ; then
  	AC_DEFINE(ILUT_USE_ALLEGRO,,[Support Allegro API])
    SUPPORTED_API=$SUPPORTED_API"Allegro "
    LIBALLEGRO=`allegro-config --libs`
    HALLEGRO=`allegro-config --cflags`
  fi
fi
AC_SUBST(LIBALLEGRO)
AC_SUBST(HALLEGRO)

dnl Win32
case "$host" in
  *mingw32*)
		LIBIL_CFLAGS=$LIBIL_CFLAGS" -mno-cygwin"
		LIBILU_CFLAGS=$LIBILU_CFLAGS" -mno-cygwin"
		LIBILUT_CFLAGS=$LIBILUT_CFLAGS" -mno-cygwin"
		VERSIONINFO=""
              ;;
  *cygwin*)
		LIBILUT_WIN32LIBPATH="-L/usr/lib/w32api"
		VERSIONINFO=""
              ;;
  *)
		use_win32=no
		VERSIONINFO="-version-info $DEVIL_LTVERSION"
              ;;
esac
AC_SUBST(VERSIONINFO)
if test "$use_win32" = "no" ; then
  LIBGDI32=""
else
  LIBGDI32="-lgdi32"
fi
AC_SUBST(LIBGDI32)

dnl DirectX 8
if test $use_directx8 = yes ; then
	AC_CHECK_HEADER(d3d8.h,,use_directx8=no)
	if test $use_directx8 = yes ; then
		AC_DEFINE(ILUT_USE_DIRECTX8,,[Support DirectX8 API])
	    SUPPORTED_API=$SUPPORTED_API"DirectX8 "
	fi
fi

dnl DirectX 9
if test $use_directx9 = yes ; then
	AC_CHECK_HEADER(d3d9.h,,use_directx9=no)
	if test $use_directx9 = yes ; then
		AC_DEFINE(ILUT_USE_DIRECTX9,,[Support DirectX9 API])
 		SUPPORTED_API=$SUPPORTED_API"DirectX9 "
	fi
fi

dnl Mac OS X Specific test
case "$target" in
    *-apple-darwin*) AC_DEFINE(MAX_OS_X,,[Building on Mac OS X])
esac
dnl Final substitution
AC_SUBST(LIBIL_CFLAGS)
AC_SUBST(LIBILU_CFLAGS)
AC_SUBST(LIBILUT_CFLAGS)
AC_SUBST(LIBILUT_WIN32LIBPATH)

dnl
dnl Ouput files to generate
dnl note: keep the list on one line separated by single spaces 
dnl (otherwise MSYS compilation breaks)
AC_CONFIG_FILES([Makefile src-IL/Makefile src-IL/src/Makefile src-IL/include/Makefile src-ILU/Makefile src-ILU/src/Makefile src-ILU/include/Makefile src-ILUT/Makefile src-ILUT/src/Makefile src-ILUT/include/Makefile include/Makefile include/IL/Makefile ])
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
echo


