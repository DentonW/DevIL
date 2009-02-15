all_include = $(srcdir)/../include
devildir = @datadir@/devil
examplesdir = $(devildir)/examples
example_srcdir = ../examples

AM_CPPFLAGS = -I $(all_include)

il_library = ../lib/libIL.la 
ilu_library = ../lib/libILU.la
ilut_library = ../lib/libILUT.la

EXTRA_DIST =

bin_PROGRAMS =
examples_PROGRAMS = 

if BUILD_ILU
bin_PROGRAMS += ilur
endif #BUILD_ILU

ilur_SOURCES = ../src-ILU/ilur/ilur.c
ilur_LDADD = $(ilu_library)

if BUILD_EXAMPLES

examples_PROGRAMS += IL_override IL_read IL_simple 

if BUILD_ILUT

examples_PROGRAMS += ILUT_cpp_wrapper 

if USE_ALLEGRO
examples_PROGRAMS += ILUT_allegro
endif #USE_ALLEGRO

if USE_DIRECTX
examples_PROGRAMS += ILUT_d3d ILUT_windows
endif #USE_DIRECTX

if USE_SDL
examples_PROGRAMS += ILUT_sdl
endif #USE_SDL

if USE_W32
examples_PROGRAMS += ILUT_animation
endif #USE_W32

if HAVE_GLUT
if USE_X11
examples_PROGRAMS += ILUT_gl ILUT_volume
endif #USE_X11
endif #HAVE_GLUT

endif #BUILD_ILUT

endif #BUILD_EXAMPLES

ILUT_allegro_SOURCES = $(example_srcdir)/allegro_example/allegtest.c
ILUT_allegro_CFLAGS = $(alleg_CFLAGS)
ILUT_allegro_LDFLAGS = $(ilut_library) 

ILUT_sdl_SOURCES = $(example_srcdir)/sdl_example/sdl_test.c
ILUT_sdl_CFLAGS = $(SDL_CFLAGS)
ILUT_sdl_LDFLAGS = $(ilut_library)

ILUT_animation_SOURCES = $(example_srcdir)/animation_example/AnimTest.cpp $(example_srcdir)/animation_example/resource.h $(example_srcdir)/animation_example/AnimTest.rc
#ILUT_animation_CFLAGS =
ILUT_animation_LDFLAGS = $(ilut_library)
EXTRA_DIST += $(example_srcdir)/animation_example/OpenIL.ico

ILUT_d3d_SOURCES = $(example_srcdir)/direct3d_example/d3dtest.cpp
ILUT_d3d_LDFLAGS = $(ilut_library)

ILUT_cpp_wrapper_SOURCES = $(example_srcdir)/cpp_wrapper_example/il_wrap.cpp
ILUT_cpp_wrapper_LDFLAGS = $(ilut_library) 

IL_override_SOURCES = $(example_srcdir)/override_example/iotest.c

ILUT_gl_SOURCES = $(example_srcdir)/opengl_example/gltest.c $(example_srcdir)/opengl_example/gltest.h
ILUT_gl_LDFLAGS = $(ilut_library)

IL_read_SOURCES = $(example_srcdir)/register_read_example/readtest.c

IL_simple_SOURCES = $(example_srcdir)/simple_example/simple.c

if BUILD_ILU
IL_override_LDFLAGS = $(ilu_library)
IL_read_LDFLAGS = $(ilu_library)
IL_simple_LDFLAGS = $(ilu_library)
else	#not BUILD_ILU
IL_override_LDFLAGS = $(il_library)
IL_read_LDFLAGS = $(il_library)
IL_simple_LDFLAGS = $(il_library)
endif	#not BUILD_ILU

ILUT_volume_SOURCES = $(example_srcdir)/volume_example/3dtest.h $(example_srcdir)/volume_example/3dtest.c
ILUT_volume_LDFLAGS = $(ilut_library)

ILUT_windows_SOURCES = $(example_srcdir)/windows_example/BatchConv.cpp $(example_srcdir)/windows_example/WindowsTest.cpp $(example_srcdir)/windows_example/WindowsTest.h $(example_srcdir)/windows_example/resource.h $(example_srcdir)/windows_example/WindowsTest.rc
ILUT_windows_LDFLAGS =  $(ilut_library)
EXTRA_DIST += $(example_srcdir)/windows_example/resources/OpenIL.ico

.rc.o:
	$(RC) $(RCFLAGS) $< -o $@
