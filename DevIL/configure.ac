dnl 
dnl Autoconf script for DevIL Library
dnl Written by Edouard Gomez <ed.gomez@wanadoo.fr>
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
DEVIL_REVISION=7
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

AC_REVISION($Revision: 1.5 $)
AC_CONFIG_AUX_DIR(libtools)
AC_PREFIX_DEFAULT(/usr/local)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_LANG([C])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_C_BIGENDIAN
AC_LIBTOOL_WIN32_DLL
AC_PATH_XTRA

AM_INIT_AUTOMAKE("DevIL", $DEVIL_MAJOR.$DEVIL_MINOR.$DEVIL_REVISION)
AM_CONFIG_HEADER(include/IL/config.h)
AM_DISABLE_STATIC
AM_PROG_LIBTOOL
AM_MAINTAINER_MODE

dnl
dnl Altives Check
dnl

AC_MSG_CHECKING(Altivec gcc extension)
CFLAGS_TMP=$CFLAGS
CFLAGS="-faltivec"
AC_COMPILE_IFELSE([
int main() {
	vector unsigned int vec;
	vec = vec_splat_u32(0);
	return 0;
}],
[AC_DEFINE([ALTIVEC_GCC],,[Altivec GCC extension found])] [AC_DEFINE([VECTORMEM],,[Memory is vector aligned])] [CFLAGS="$CFLAGS_TMP -faltivec -maltivec";] [AC_MSG_RESULT(yes)],
[CFLAGS=$CFLAGS_TMP;] [AC_MSG_RESULT(no)] );

dnl
dnl SSE
dnl
CFLAGS_TMP=$CFLAGS
CFLAGS="-msse"
AC_MSG_CHECKING(SSE)
AC_COMPILE_IFELSE([
#include <xmmintrin.h>
int main() {
        __m128 v;
        v = _mm_sub_ss(v,v);
}], [AC_DEFINE([SSE],,[Use SSE cpu extension])]
 [CFLAGS="$CFLAGS_TMP -msse";] [AC_DEFINE([VECTORMEM],,[Memory is vector aligned])][AC_MSG_RESULT(yes)], [CFLAGS=$CFLAGS_TMP;] [AC_MSG_RESULT(no)])

dnl
dnl SSE2
dnl
CFLAGS_TMP=$CFLAGS
CFLAGS="-msse2"
AC_MSG_CHECKING(SSE2)
AC_COMPILE_IFELSE([
#include <emmintrin.h>
int main() {
        __m128i v;
        v = _mm_add_epi32(v,v);
}], [AC_DEFINE([SSE2],,[Use SSE2 cpu extension])]
 [CFLAGS="$CFLAGS_TMP -msse2";] [AC_DEFINE([VECTORMEM],,[Memory is vector aligned])][AC_MSG_RESULT(yes)], [CFLAGS=$CFLAGS_TMP;] [AC_MSG_RESULT(no)])

dnl
dnl SSE3
dnl
CFLAGS_TMP=$CFLAGS
CFLAGS="-msse3"
AC_MSG_CHECKING(SSE3)
AC_COMPILE_IFELSE([
#include <emmintrin.h>
int main() {
        __m128d v;
        v = _mm_addsub_pd(v,v);
}], [AC_DEFINE([SSE3],,[Use SSE3 cpu extension])]
 [CFLAGS="$CFLAGS_TMP -msse3";] [AC_DEFINE([VECTORMEM],,[Memory is vector aligned])][AC_MSG_RESULT(yes)], [CFLAGS=$CFLAGS_TMP;] [AC_MSG_RESULT(no)])

dnl
dnl Memory alignment functions
dnl
AC_CHECK_FUNC( valloc, [AC_DEFINE([VALLOC],,[valloc memory alloctaion found])] )
AC_CHECK_FUNC( posix_memalign, [AC_DEFINE([POSIX_MEMALIGN],,[posix_memalign memory alloctaion found])] )
AC_CHECK_FUNC( memalign, [AC_DEFINE([MEMALIGN],,[memalign memory alloctaion found])])


CFLAGS="-Wall $CFLAGS"
LIBIL_CFLAGS="$CFLAGS"
LIBILU_CFLAGS="$CFLAGS"
LIBILUT_CFLAGS="$CFLAGS"

dnl
dnl Default supported formats : all except lcms
dnl
dnl no_bmp="no"
dnl no_dcx="no"
dnl no_dds="no"
dnl no_doom="no"
dnl no_gif="no"
dnl no_hdr="no"
dnl no_icon="no"
dnl no_jpeg="no"
dnl no_lif="no"
dnl no_mdl="no"
dnl no_mng="no"
dnl no_pcd="no"
dnl no_pcx="no"
dnl no_pic="no"
dnl no_pix="no"
dnl no_png="no"
dnl no_pnm="no"
dnl no_psd="no"
dnl no_psp="no"
dnl no_pxr="no"
dnl no_lcms="no"
dnl no_raw="no"
dnl no_sgi="no"
dnl no_tga="no"
dnl no_tiff="no"
dnl no_wal="no"
dnl no_xpm="no"

dnl
dnl Arguments
dnl
AC_ARG_ENABLE(bmp,
[  --disable-bmp           Doesn't compile bmp support],
[case "$enable_bmp" in
  no)
    no_bmp="yes"
    ;;
  *)
    no_bmp="no"
    ;;
esac],
no_bmp="no")

AC_ARG_ENABLE(dcx,
[  --disable-dcx           Doesn't compile dcx support],
[case "$enable_dcx" in
  no)
    no_dcx="yes"
    ;;
  *)
    no_dcx="no"
    ;;
esac],
no_dcx="no")

AC_ARG_ENABLE(doom,
[  --disable-doom          Doesn't compile wad support],
[case "$enable_doom" in
  no)
    no_doom="yes"
    ;;
  *)
    no_doom="no"
    ;;
esac],
no_doom="no")

AC_ARG_ENABLE(gif,
[  --disable-gif           Doesn't compile gif support],
[case "$enable_gif" in
  no)
    no_gif="yes"
    ;;
  *)
    no_gif="no"
    ;;
esac],
no_gif="no")

AC_ARG_ENABLE(hdr,
[  --disable-hdr           Doesn't compile hdr support],
[case "$enable_hdr" in
  no)
    no_hdr="yes"
    ;;
  *)
    no_hdr="no"
    ;;
esac],
no_hdr="no")

AC_ARG_ENABLE(icon,
[  --disable-icon          Doesn't compile icon support],
[case "$enable_icon" in
  no)
    no_icon="yes"
    ;;
  *)
    no_icon="no"
    ;;
esac],
no_icon="no")

AC_ARG_ENABLE(jpeg,
[  --disable-jpeg          Doesn't compile jpeg support],
[case "$enable_jpeg" in
  no)
    no_jpeg="yes"
    ;;
  *)
    no_jpeg="no"
    ;;
esac],
no_jpeg="no")

AC_ARG_ENABLE(lif,
[  --disable-lif           Doesn't compile lif support],
[case "$enable_lif" in
  no)
    no_lif="yes"
    ;;
  *)
    no_lif="no"
    ;;
esac],
no_lif="no")

AC_ARG_ENABLE(lcms,
[  --disable-lcms          Doesn't compile lcms support],
[case "$enable_lcms" in
  no)
    no_lcms="yes"
    ;;
  *)
    no_lcms="no"
    ;;
esac],
no_lcms="no")

AC_ARG_ENABLE(mdl,
[  --disable-mdl           Doesn't compile mdl support],
[case "$enable_mdl" in
  no)
    no_mdl="yes"
    ;;
  *)
    no_mdl="no"
    ;;
esac],
no_mdl="no")

AC_ARG_ENABLE(mng,
[  --disable-mng           Doesn't compile mng support],
[case "$enable_mng" in
  no)
    no_mng="yes"
    ;;
  *)
    no_mng="no"
    ;;
esac],
no_mng="no")

AC_ARG_ENABLE(pcd,
[  --disable-pcd           Doesn't compile pcd support],
[case "$enable_pcd" in
  no)
    no_pcd="yes"
    ;;
  *)
    no_pcd="no"
    ;;
esac],
no_pcd="no")

AC_ARG_ENABLE(pcx,
[  --disable-pcx           Doesn't compile pcx support],
[case "$enable_pcx" in
  no)
    no_pcx="yes"
    ;;
  *)
    no_pcx="no"
    ;;
esac],
no_pcx="no")

AC_ARG_ENABLE(pic,
[  --disable-pic           Doesn't compile pic support],
[case "$enable_pic" in
  no)
    no_pic="yes"
    ;;
  *)
    no_pic="no"
    ;;
esac],
no_pic="no")

AC_ARG_ENABLE(pix,
[  --disable-pix           Doesn't compile pix support],
[case "$enable_pix" in
  no)
    no_pix="yes"
    ;;
  *)
    no_pix="no"
    ;;
esac],
no_pix="no")

AC_ARG_ENABLE(png,
[  --disable-png           Doesn't compile png support],
[case "$enable_png" in
  no)
    no_png="yes"
    ;;
  *)
    no_png="no"
    ;;
esac],
no_png="no")

AC_ARG_ENABLE(pnm,
[  --disable-pnm           Doesn't compile pnm support],
[case "$enable_pnm" in
  no)
    no_pnm="yes"
    ;;
  *)
    no_pnm="no"
    ;;
esac],
no_pnm="no")

AC_ARG_ENABLE(psd,
[  --disable-psd           Doesn't compile psd support],
[case "$enable_psd" in
  no)
    no_psd="yes"
    ;;
  *)
    no_psd="no"
    ;;
esac],
no_psd="no")

AC_ARG_ENABLE(psp,
[  --disable-psp           Doesn't compile psp support],
[case "$enable_psp" in
  no)
    no_psp="yes"
    ;;
  *)
    no_psp="no"
    ;;
esac],
no_psp="no")

AC_ARG_ENABLE(pxr,
[  --disable-pxr           Doesn't compile pxr support],
[case "$enable_pxr" in
  no)
    no_pxr="yes"
    ;;
  *)
    no_pxr="no"
    ;;
esac],
no_pxr="no")

AC_ARG_ENABLE(lcms,
[  --disable-lcms          Doesn't compile lcms support],
[case "$enable_lcms" in
  no)
    no_lcms="yes"
    ;;
  *)
    no_lcms="no"
    ;;
esac],
no_lcms="no")

AC_ARG_ENABLE(raw,
[  --disable-raw           Doesn't compile raw support],
[case "$enable_raw" in
  no)
    no_raw="yes"
    ;;
  *)
    no_raw="no"
    ;;
esac],
no_raw="no")

AC_ARG_ENABLE(sgi,
[  --disable-sgi           Doesn't compile sgi support],
[case "$enable_sgi" in
  no)
    no_sgi="yes"
    ;;
  *)
    no_sgi="no"
    ;;
esac],
no_sgi="no")

AC_ARG_ENABLE(tga,
[  --disable-tga           Doesn't compile tga support],
[case "$enable_tga" in
  no)
    no_tga="yes"
    ;;
  *)
    no_tga="no"
    ;;
esac], no_tga="no")

AC_ARG_ENABLE(tiff,
[  --disable-tiff          Doesn't compile tiff support],
[case "$enable_tiff" in
  no)
    no_tiff="yes"
    ;;
  *)
    no_tiff="no"
    ;;
esac],
no_tiff="no")

AC_ARG_ENABLE(wal,
[  --disable-wal           Doesn't compile wal support],
[case "$enable_wal" in
  no)
    no_wal="yes"
    ;;
  *)
    no_wal="no"
    ;;
esac],
no_wal="no")

AC_ARG_ENABLE(xpm,
[  --disable-xpm           Doesn't compile xpm support],
[case "$enable_xpm" in
  no)
    no_xpm="yes"
    ;;
  *)
    no_xpm="no"
    ;;
esac],
no_xpm="no")

AC_ARG_ENABLE(allegro,
[  --disable-allegro       Doesn't compile allegro support],
use_allegro=$enable_allegro, use_allegro="yes")

AC_ARG_ENABLE(directx,
[  --disable-directx       Doesn't compile directx support],
use_directx=$enable_directx, use_directx="yes")

AC_ARG_ENABLE(opengl,
[  --disable-opengl        Doesn't compile opengl support],
use_opengl=$enable_opengl, use_opengl="yes")

AC_ARG_ENABLE(sdl,
[  --disable-sdl           Doesn't compile sdl support],
use_sdl=$enable_sdl, use_sdl="yes")

AC_ARG_ENABLE(win32,
[  --disable-win32         Doesn't compile win32 support],
use_win32=$enable_win32, use_win32="yes")

AC_ARG_ENABLE(release,
[  --disable-release         Compile a debug version of the library],
[case "$enable_release" in
  yes)
    debug_symbols="no"
    ;;
  no)
    debug_symbols="yes"
    ;;
esac],
debug_symbols="no")

if test "$debug_symbols" = "yes" ; then
	CFLAGS="-Wall -g"
  LIBIL_CFLAGS="-Wall -g"
  LIBILU_CFLAGS="-Wall -g"
  LIBILUT_CFLAGS="-Wall -g"
fi

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
		use_win32="no"
		use_directx="no"
		VERSIONINFO="-version-info $DEVIL_LTVERSION"
              ;;
esac
AC_SUBST(VERSIONINFO)

dnl
dnl Libraries development checking
dnl
CPPFLAGS=$X_CFLAGS
AC_SUBST(X_CFLAGS)
AC_CHECK_HEADER(jpeglib.h, ,no_jpeg="yes")
AC_CHECK_HEADER(tiffio.h, ,no_tiff="yes")
AC_CHECK_HEADER(png.h, ,no_png="yes")
AC_CHECK_HEADER(lcms/lcms.h, ,no_lcms="yes")
AC_CHECK_HEADER(lcms.h, ,no_lcms="yes";lcms_nodirinclude="yes")
AC_CHECK_HEADER(libmng.h, ,no_mng="yes")
AC_CHECK_HEADER(d3d8.h, , use_directx="no")
case "$target" in
 	*apple-darwin*)
		AC_CHECK_HEADER(OpenGL/gl.h, [have_gl_hdr=yes], use_opengl="no")
							;;
	*)
		AC_CHECK_HEADER(GL/gl.h, [have_gl_hdr=yes], use_opengl="no")
							;;
esac
dnl AC_CHECK_HEADER(SDL.h, , use_sdl="no")
AC_CHECK_HEADER(zlib.h, LIBZ="-lz", LIBZ="")
AC_SUBST(LIBZ)

dnl
dnl Now we can check for bin libraries
dnl
if test "$no_jpeg" = "no" ; then
  AC_CHECK_LIB(jpeg, jpeg_start_decompress, LIBJPG="-ljpeg", LIBJPG="", )
  if test "$LIBJPG" != "-ljpeg" ; then
    no_jpeg="yes"
  else
    AC_CHECK_LIB(jpeg, jpeg_init_exif_field, use_exif="yes", use_exif="no", )
    if test "$use_exif" = "no" ; then
      AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED,,[Use jpeglib unmoified])
    fi
    AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED)
  fi
else
  LIBJPG=""
fi
AC_SUBST(LIBJPG)

case "$target" in
*-apple-darwin*)
    AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED)
    AC_DEFINE(MAX_OS_X,,[Building on Mac OS X])
esac

if test "$no_tiff" = "no" ; then
   AC_CHECK_LIB(tiff, TIFFReadRGBAImage, LIBTIFF="-ltiff", LIBTIFF="", -lm)
   if test "$LIBTIFF" != "-ltiff" ; then
     no_tiff="yes"
   fi
else
  LIBTIFF=""
fi
AC_SUBST(LIBTIFF)

if test "$no_png" = "no" ; then
   AC_CHECK_LIB(png, png_start_read_image, LIBPNG="-lpng", LIBPNG="", `libpng-config --libs`)
   if test "$LIBPNG" != "-lpng" ; then
     no_png="yes"
   fi
else
  LIBPNG=""
fi
AC_SUBST(LIBPNG)

if test "$no_lcms" = "no" ; then
   AC_CHECK_LIB(lcms,  cmsOpenProfileFromFile, LIBLCMS="-llcms", LIBLCMS="", -lm)
   if test "$LIBLCMS" != "-llcms" ; then
     no_lcms="yes"
   fi
else
  LIBLCMS=""
fi
AC_SUBST(LIBLCMS)

if test "$no_mng" = "no" ; then
   AC_CHECK_LIB(mng, mng_setcb_openstream, LIBMNG="-lmng", LIBMNG="", )
   if test "$LIBMNG" != "-lmng" ; then
     no_mng="yes"
   fi
else
  LIBMNG=""
fi
AC_SUBST(LIBMNG)

dnl
dnl ILUT APIs checking
dnl

dnl Check for SDL
AM_PATH_SDL(1.2.5,use_sdl="yes",use_sdl="no")
HSDL="$SDL_CFLAGS"
LIBSDL="$SDL_LIBS"
AC_SUBST(LIBSDL)
AC_SUBST(HSDL)

AC_LANG([C++])

if test "$use_opengl" = "yes" ; then
case "$target" in
*-*-cygwin* | *-*-mingw32*)
	LIBGL="-lopengl32 -lglu32"
	;;
*-apple-darwin*)
	LIBGL="-Wl,-framework -Wl,OpenGL"
	;;
*-*-*)
	AC_CHECK_LIB(GL,glLoadIdentity,[LIBGL="-lGL"])
	AC_CHECK_LIB(GLU,gluPerspective,[LIBGL="-lGLU"])
	;;
esac
   if test "x$have_gl_hdr" != "xyes" ; then
     use_opengl="no"
     LIBGL=""
   fi
else
  LIBGL=""
fi

AC_SUBST(LIBGL)

if test "$use_allegro" = "yes" ; then
  AC_CHECK_PROG(allegro_config, allegro-config, yes, no, )
  if test $allegro_config = no ; then
    use_allegro="no"
    LIBALLEGRO=""
    HALLEGRO=""
  else
    LIBALLEGRO=`allegro-config --libs`
    HALLEGRO=`allegro-config --cflags`
  fi
fi
AC_SUBST(LIBALLEGRO)
AC_SUBST(HALLEGRO)

if test "$use_win32" = "no" ; then
  LIBGDI32=""
else
  LIBGDI32="-lgdi32"
fi
AC_SUBST(LIBGDI32)

dnl
dnl Prepares substitutions in config.h
dnl

SUPPORTED_FORMATS=""

if test "$no_bmp"  = "yes"  ; then
  AC_DEFINE(IL_NO_BMP,,[BMP support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"BMP "
fi
AM_CONDITIONAL(IL_NO_BMP, test x$no_bmp != "xyes")

if test "$no_dcx"  = "yes"  ; then
  AC_DEFINE(IL_NO_DCX,,[DCX support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"DCX "
fi
AM_CONDITIONAL(IL_NO_DCX, test x$no_dcx != "xyes")

if test "$no_dds"  = "yes"  ; then
  AC_DEFINE(IL_NO_DDS,,[DDS support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"DDS "
fi
AM_CONDITIONAL(IL_NO_DDS, test x$no_dds != "xyes")

if test "$no_doom" = "yes" ; then
  AC_DEFINE(IL_NO_DOOM,,[DOOM support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"WAD "
fi
AM_CONDITIONAL(IL_NO_DOOM, test x$no_doom != "xyes")

if test "$no_gif"  = "yes"  ; then
  AC_DEFINE(IL_NO_GIF,,[GIF support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"GIF "
fi
AM_CONDITIONAL(IL_NO_GIF, test x$no_gif != "xyes")

if test "$no_hdr"  = "yes"  ; then
  AC_DEFINE(IL_NO_HDR,,[HDR support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"HDR "
fi
AM_CONDITIONAL(IL_NO_HDR, test x$no_hdr != "xyes")

if test "$no_icon" = "yes"  ; then
  AC_DEFINE(IL_NO_ICO,,[ICO support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"ICO "
fi
AM_CONDITIONAL(IL_NO_ICO, test x$no_icon != "xyes")

if test "$no_jpeg" = "yes"  ; then
  AC_DEFINE(IL_NO_JPG,,[JPEG support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"JPG "
fi
AM_CONDITIONAL(IL_NO_JPG, test x$no_jpeg != "xyes")

if test "$no_lif"  = "yes"  ; then
  AC_DEFINE(IL_NO_LIF,,[LIF support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"LIF "
fi
AM_CONDITIONAL(IL_NO_LIF, test x$no_lif != "xyes")

if test "$no_lcms"  = "yes"  ; then
  if test "$lcms_nodirinclude" = "yes" ; then
    AC_DEFINE(LCMS_NODIRINCLUDE,,[LCMS include without lcms/ support])
  fi
   AC_DEFINE(IL_NO_LCMS,,[LCMS support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"LCMS "
fi
AM_CONDITIONAL(IL_NO_LCMS, test x$no_lcms != "xyes")

if test "$no_mdl"  = "yes"  ; then
  AC_DEFINE(IL_NO_MDL,,[MDL support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"MDL "
fi
AM_CONDITIONAL(IL_NO_MDL, test x$no_mdl != "xyes")

if test "$no_mng"  = "yes"  ; then
  AC_DEFINE(IL_NO_MNG,,[MNG support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"MNG "
fi
AM_CONDITIONAL(IL_NO_MNG, test x$no_mng != "xyes")

if test "$no_pcd"  = "yes"  ; then
  AC_DEFINE(IL_NO_PCD,,[PCD support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PCD "
fi
AM_CONDITIONAL(IL_NO_PCD, test x$no_pcd != "xyes")

if test "$no_pcx"  = "yes"  ; then
  AC_DEFINE(IL_NO_PCX,,[PCX support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PCX "
fi
AM_CONDITIONAL(IL_NO_PCX, test x$no_pcx != "xyes")

if test "$no_pic"  = "yes"  ; then
  AC_DEFINE(IL_NO_PIC,,[PIC support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PIC "
fi
AM_CONDITIONAL(IL_NO_PIC, test x$no_pic != "xyes")

if test "$no_pix"  = "yes"  ; then
  AC_DEFINE(IL_NO_PIX,,[PIX support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PIX "
fi
AM_CONDITIONAL(IL_NO_PIX, test x$no_pix != "xyes")

if test "$no_png"  = "yes"  ; then
  AC_DEFINE(IL_NO_PNG,,[PNG support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PNG "
fi
AM_CONDITIONAL(IL_NO_PNG, test x$no_png != "xyes")

if test "$no_pnm"  = "yes"  ; then
  AC_DEFINE(IL_NO_PNM,,[PNM support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PNM "
fi
AM_CONDITIONAL(IL_NO_PNM, test x$no_pnm != "xyes")

if test "$no_psd"  = "yes"  ; then
  AC_DEFINE(IL_NO_PSD,,[PSD support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PSD "
fi
AM_CONDITIONAL(IL_NO_PSD, test x$no_psd != "xyes")

if test "$no_psp"  = "yes"  ; then
  AC_DEFINE(IL_NO_PSP,,[PSP support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PSP "
fi
AM_CONDITIONAL(IL_NO_PSP, test x$no_psp != "xyes")

if test "$no_pxr"  = "yes"  ; then
  AC_DEFINE(IL_NO_PXR,,[PXR support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"PXR "
fi
AM_CONDITIONAL(IL_NO_PXR, test x$no_pxr != "xyes")

if test "$no_raw"  = "yes"  ; then
  AC_DEFINE(IL_NO_RAW,,[RAW support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"RAW "
fi
AM_CONDITIONAL(IL_NO_RAW, test x$no_raw != "xyes")

if test "$no_sgi"  = "yes"  ; then
  AC_DEFINE(IL_NO_SGI,,[SGI support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"SGI "
fi
AM_CONDITIONAL(IL_NO_SGI, test x$no_sgi != "xyes")

if test "$no_tga"  = "yes"  ; then
  AC_DEFINE(IL_NO_TGA,,[TGA support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"TGA "
fi
AM_CONDITIONAL(IL_NO_TGA, test x$no_tga != "xyes")

if test "$no_tiff" = "yes"  ; then
  AC_DEFINE(IL_NO_TIF,,[TIFF support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"TIF "
fi
AM_CONDITIONAL(IL_NO_TIF, test x$no_tiff != "xyes")

if test "$no_wal"  = "yes"  ; then
  AC_DEFINE(IL_NO_WAL,,[WAL support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"WAL "
fi
AM_CONDITIONAL(IL_NO_WAL, test x$no_wal != "xyes")

if test "$no_xpm"  = "yes"  ; then
  AC_DEFINE(IL_NO_XPM,,[XPM support])
else
  SUPPORTED_FORMATS=$SUPPORTED_FORMATS"XPM "
fi
AM_CONDITIONAL(IL_NO_XPM, test x$no_xpm != "xyes")


AC_MSG_RESULT([ ])
AC_MSG_RESULT([+----------------------------------------+])
AC_MSG_RESULT([|     IL library  Supported formats      |])
AC_MSG_RESULT([+----------------------------------------+-------+])
AC_MSG_RESULT([$SUPPORTED_FORMATS])


SUPPORTED_API=""

if test "$use_allegro" = "yes" ; then
  AC_DEFINE(ILUT_USE_ALLEGRO,,[Support Allegro API])
  SUPPORTED_API=$SUPPORTED_API"Allegro "
fi

if test "$use_directx" = "yes" ; then
  AC_DEFINE(ILUT_USE_DIRECTX8,,[Support DirectX8 API])
  SUPPORTED_APPI=$SUPPORTED_API"Directx8 "
fi

if test "$use_opengl" = "yes" ; then
  AC_DEFINE(ILUT_USE_OPENGL,,[Support OpenGL API])
  SUPPORTED_API=$SUPPORTED_API"OpenGL "
  case "$target" in
    *-apple-darwin*)
	LIBILUT_CFLAGS=$LIBILUT_CFLAGS" -framework OpenGL"
  esac
fi

if test "$use_sdl" = "yes" ; then
  AC_DEFINE(ILUT_USE_SDL,,[Support SDL API])
  SUPPORTED_API=$SUPPORTED_API"SDL "
fi

AC_MSG_RESULT([ ])
AC_MSG_RESULT([+----------------------------------------+])
AC_MSG_RESULT([|      ILUT library  Supported APIs      |])
AC_MSG_RESULT([+----------------------------------------+-------+])
AC_MSG_RESULT([$SUPPORTED_API])

AC_MSG_RESULT([ ])

dnl
dnl Final vars
dnl
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
