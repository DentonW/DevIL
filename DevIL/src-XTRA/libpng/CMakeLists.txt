PROJECT(LIBPNG)

# source files for png
SET(PNG_SRCS
	pngget.c
	pngrio.c
	pngwrite.c
	png.c
	pngmem.c
	pngrtran.c
	pngtrans.c
	pngwtran.c
	pngerror.c 
	pngpread.c
	pngrutil.c
	pngvcrd.c 
	pngwutil.c
	pnggccrd.c 
	pngread.c 
	pngset.c
	pngwio.c

	pngconf.h
	png.h
)

SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)

IF(WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
  IF(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DPNG_BUILD_DLL -DZLIB_DLL -DPNG_NO_MODULEDEF)
  ELSE(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DPNG_STATIC)
  ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

SOURCE_GROUP(code FILES ${PNG_SRCS} )

ADD_LIBRARY(DevIL_libpng PNG_SRCS)

TARGET_LINK_LIBRARIES(DevIL_libpng
	DevIL_zlib
)

INCLUDE_DIRECTORIES(

	 ${ZLIB_SOURCE_DIR}
)
