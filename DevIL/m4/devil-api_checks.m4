
dnl
dnl Checks for SDL presence, adds cflags to ILUT_CFLAGS and libs to ILUT_LIBS
dnl also sets use_sdl variable
dnl
AC_DEFUN([SETTLE_SDL],
         [TEST_API([sdl])
          AS_IF([test "x$enable_sdl" = "xyes"],
                [AM_PATH_SDL(1.2.5,
                             [use_sdl="yes"],
		             [use_sdl="no"]) ])
          AS_IF([test "x$use_sdl" = "xyes"],
                [AC_DEFINE([ILUT_USE_SDL],
	                   [],
	                   [Support SDL API])
                 ILUT_CFLAGS="$SDL_CFLAGS $ILUT_CFLAGS"
                 ILUT_LIBS="$SDL_LIBS $ILUT_LIBS"
                 SUPPORTED_API="$SUPPORTED_API SDL"]) ])

dnl
dnl Checks for OpenGL implementations, external macros from Autoconf macro archives used
dnl Checks for GL, GLU (needed for DevIL) and for GLUT (one of the examples uses this)
dnl sets use_ogl and have_glut variables
dnl
AC_DEFUN([SETTLE_OPENGL],
         [TEST_API(opengl)
          AS_IF([test "x$enable_opengl" = "xyes"],
                [AX_CHECK_GL
                 AX_CHECK_GLU
                 AX_CHECK_GLUT
                 AS_IF([test "x$no_gl" != "xyes" -a "x$no_glu" != "xyes"],
                       [use_ogl="yes"
		        ILUT_LIBS="$GLU_LIBS $GL_LIBS $ILUT_LIBS" 
                        AS_IF([test "x$no_glut" != "xyes"],
                              [have_glut="yes"
                               ILUT_LIBS="$GLUT_LIBS  $ILUT_LIBS"])
                        AC_DEFINE([ILUT_USE_OPENGL],
	                          [1],
	                          [Define if system supports OpenGL API])
                        SUPPORTED_API="$SUPPORTED_API OpenGL"]) ]) ])

dnl
dnl The check for Allegro game programming library.
dnl The part before "official" AM_PATH_ALLEGRO is for unofficial autotools builds
dnl sets use_allegro variable
dnl
AC_DEFUN([SETTLE_ALLEGRO],
[TEST_API([allegro])
AS_IF([test "x$enable_allegro" = "xyes"],
[AC_CHECK_HEADER([allegro.h],
		[AC_CHECK_LIB([alleg],
			      [main],
			      [use_allegro="yes"
			       ILUT_LIBS="-lalleg $ILUT_LIBS"]) 
	         AC_CHECK_LIB([allegro_unsharable],
			      [main],
			      [ILUT_LIBS="-lalleg_unsharable $ILUT_LIBS"])
                 AC_CHECK_LIB([alleg42],
			      [main],
			      [use_allegro="yes"
			       ILUT_LIBS="-lalleg42 $ILUT_LIBS"]) ])
AS_IF([test "x$use_allegro" != "xyes"],
      [AM_PATH_ALLEGRO(4.2.0, 
                       [use_allegro="yes"
                        ILUT_LIBS="$allegro_LIBS $ILUT_LIBS"
                        ILUT_CFLAGS="$allegro_CFLAGS $ILUT_CFLAGS"], 
                       [use_allegro="no"]) ]) ])
AS_IF([test "x$use_allegro" = "xyes"],
      [AC_DEFINE([ILUT_USE_ALLEGRO],
		 [],
		 [Support Allegro API])
       SUPPORTED_API="$SUPPORTED_API Allegro"]) ])

dnl
dnl 32-bit Windows support
dnl checks for gdi32 lib presence
dnl sets use_w32 variable
dnl
AC_DEFUN([SETTLE_W32],
         [TEST_API([w32])
          AS_IF([test "x$enable_w32" = "xyes"],
                [AC_CHECK_LIB([gdi32],
		              [main],
		              [ILUT_LIBS="-lgdi32 $ILUT_LIBS"
                               use_w32="yes"
                               SUPPORTED_API="$SUPPORTED_API w32"]) ]) ])

dnl
dnl check for DirectX 8
dnl sets use_directx8 variable
dnl
AC_DEFUN([SETTLE_DX8],
[TEST_API([directx8])
AS_IF([test "x$enable_directx8" = "xyes"],
      [AC_CHECK_HEADER([d3d8.h],
		       [AC_DEFINE([ILUT_USE_DIRECTX8],,
		                  [Support DirectX8 API]) 
                        use_directx8="yes"
                        SUPPORTED_API="$SUPPORTED_API DirectX8"]) ]) ])

dnl
dnl DirectX 9
dnl sets use_directx9 variable
dnl
AC_DEFUN([SETTLE_DX9],
[TEST_API([directx9])
AS_IF([test "x$enable_directx9" = "xyes"],
      [AC_CHECK_HEADER([d3d9.h],
		       [AC_DEFINE([ILUT_USE_DIRECTX9],,
		                  [Support DirectX9 API])
                        use_directx9="yes"
                        SUPPORTED_API="$SUPPORTED_API DirectX9"]) ]) ])

dnl
dnl DirectX 10
dnl sets use_directx10 variable
dnl
AC_DEFUN([SETTLE_DX10],
[TEST_API([directx10])
AS_IF([test "x$enable_directx10" = "xyes"],
      [AC_CHECK_HEADER([d3d10.h],
		       [AC_DEFINE([ILUT_USE_DIRECTX10],,
		                  [Support DirectX10 API])
                        use_directx10="yes"
                        SUPPORTED_API="$SUPPORTED_API DirectX10"]) ]) ])
dnl
dnl Checks for X11 developement stuff and for XShm
dnl sets use_x11 and use_xshm vars
dnl
AC_DEFUN([SETTLE_X11],
         [TEST_API([x11])
          AS_IF([test "x$enable_x11" = "xyes"],
                [AC_CHECK_HEADER([X11/Xlib.h],
	 	                 [use_x11="yes"],
		                 [use_x11="no"]) 
                 AC_CHECK_LIB([X11],
                              [main],
                              [dnl The use_x11 var is either "yes" already, or we don't want "no" to be overwritten
                               ILUT_LIBS="-lX11 $ILUT_LIBS"],
                              [use_x11="no"]) ])
          TEST_API([shm])
          AS_IF([test "x$enable_shm" = "xyes"],
                [AC_CHECK_HEADER([X11/extensions/XShm.h],
	                         [use_xshm="yes"],
		                 [use_xshm="no"],
		                 [[#include <X11/Xlib.h>]]) 
                 AC_CHECK_LIB([Xext],
                              [main],
                              [use_shm="yes"
                               ILUT_LIBS="-lXext $ILUT_LIBS"],
                              [use_shm="no"]) ])
	  TEST_API([render])
          AS_IF([test "x$enable_render" = "xyes"],
                [AC_CHECK_HEADER([X11/extensions/Xrender.h],
                                 [use_xrender="yes"],
                                 [use_xrender="no"],
                                 [[#include <X11/Xlib.h>]])
                 AC_CHECK_LIB([Xrender],
                              [main],
                              [use_render="yes"
                               ILUT_LIBS="-lXrender $ILUT_LIBS"],
                              [use_render="no"]) ])

          AS_IF([test "x$use_x11" != "xno"],
	        [AC_DEFINE([ILUT_USE_X11],
		           [1],
		           [Support X11 API])
                 SUPPORTED_API="$SUPPORTED_API X11"]) 
                 AS_IF([test "$use_xshm" = "yes"],
		       [AC_DEFINE([ILUT_USE_XSHM],
			          [],
			          [Support X11 XShm extension])
		        SUPPORTED_API="$SUPPORTED_API XShm"]) 
                 AS_IF([test "$use_xrender" = "yes"],
                       [AC_DEFINE([ILUT_USE_XRENDER],
                                  [],
                                  [Support X11 XRender extension])
                        SUPPORTED_API="$SUPPORTED_API XRender"]) ])
