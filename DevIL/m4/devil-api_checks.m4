

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
                 SUPPORTED_API=$SUPPORTED_API"SDL "]) 
          AM_CONDITIONAL([USE_SDL],
                         [test "x$use_sdl" = "xyes"]) ])

AC_DEFUN([SETTLE_OPENGL_OLD],
         [TEST_API(opengl)
AS_IF([test "x$enable_opengl" = "xyes"],
[AS_CASE(["$target"],
                  [*-*-cygwin* | *-*-mingw32*],
                  [AC_CHECK_LIB([opengl32],
                                [main],
                                [use_ogl="yes"
                                 ILUT_LIBS="-lopengl32 $ILUT_LIBS"])
                   AC_CHECK_LIB([glu32],
                                [main],
                                [use_glu="yes"
                                 ILUT_LIBS="-lglu32 $ILUT_LIBS"])],
                  [LIBGL="-lopengl32 -lglu32"],
                  [*-apple-darwin*],
                  [LIBGL='-Wl,-framework -Wl,OpenGL'],
                  [AC_CHECK_LIB([GL],
	                        [glLoadIdentity],
			        [use_ogl="yes"
			         ILUT_LIBS="-lGL $ILUT_LIBS"])
                   AC_CHECK_LIB([GLU],
                                [gluPerspective],
                                [use_glu="yes"
			         ILUT_LIBS="-lGLU $ILUT_LIBS"]) ]) ])
   	AS_IF([test "x$use_ogl" = "xyes" -a "x$use_glu" = "xyes" ],
              [AC_DEFINE([ILUT_USE_OPENGL],
	                 [1],
	                 [Define if system supports OpenGL API])
            SUPPORTED_API=$SUPPORTED_API"OpenGL "]) ])

AC_DEFUN([SETTLE_OPENGL],
         [TEST_API(opengl)
          AS_IF([test "x$enable_opengl" = "xyes"],
                [AX_CHECK_GL
                 AX_CHECK_GLU
                 AS_IF([test "x$no_gl" != "xyes" -a "x$no_glu" != "xyes"],
                       [use_ogl="yes"
		        ILUT_LIBS="$GLU_LIBS $GL_LIBS $ILUT_LIBS" 
                        AC_DEFINE([ILUT_USE_OPENGL],
	                          [1],
	                          [Define if system supports OpenGL API])
                        SUPPORTED_API=$SUPPORTED_API"OpenGL "]) ]) ])

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
       SUPPORTED_API=$SUPPORTED_API"Allegro "]) 
AM_CONDITIONAL([USE_ALLEGRO],
	       [test "x$use_allegro" = "xyes"]) ])

dnl DirectX 8
AC_DEFUN([SETTLE_DX8],
[AS_IF([test "x$enable_directx8" = "xyes"],
      [AC_CHECK_HEADER([d3d8.h],
		       [],
		       [use_directx8=no]) ])

AS_IF([test "x$have_directx8" = "xyes"],
      [AC_DEFINE([ILUT_USE_DIRECTX8],
		 [],
		 [Support DirectX8 API])
       SUPPORTED_API=$SUPPORTED_API"DirectX8 "],
       []) ])

dnl DirectX 9
AC_DEFUN([SETTLE_DX9],
[AS_IF([test "x$enable_directx9" = "yes"],
      [AC_CHECK_HEADER([d3d9.h],
		       [],
		       [use_directx9=no]) ])

AS_IF([test "x$use_directx9" = "yes"],
      [AC_DEFINE([ILUT_USE_DIRECTX9],
		 [],
		 [Support DirectX9 API])
       SUPPORTED_API=$SUPPORTED_API"DirectX9 "]) ])

AC_DEFUN([SETTLE_X11],
[TEST_API(x11)
TEST_API(shm)
AS_IF([test "x$enable_x11" = "xyes"],
      [AC_CHECK_HEADER([X11/Xlib.h],
		       [use_x11="yes"],
		       [use_x11="no"]) 
       AC_CHECK_LIB([X11],
                    [main],
                    [use_x11="yes"
                     ILUT_LIBS="-lX11 $ILUT_LIBS"],
                    [use_x11="no"]) ])
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

AS_IF([test "x$use_x11" != "xno"],
	     [AC_DEFINE([ILUT_USE_X11],
			[],
			[Support X11 API])
              SUPPORTED_API=$SUPPORTED_API"X11 "]) 
dnl              AS_CASE([$target],
dnl		      [*apple-darwin*],
dnl		      [LIBX11="$LIBX11 -L/usr/X11R6/lib"])
AS_IF([test "$use_xshm" = "yes"],
		    [AC_DEFINE([ILUT_USE_XSHM],
			       [],
			       [Support X11 XShm extension])
		     SUPPORTED_API=$SUPPORTED_API"XShm "]) ])

