cmake_minimum_required(VERSION 2.6)


PROJECT(ImageLib)

#include (CheckIncludeFile)
#include (CheckFunctionExists)
include (TestBigEndian)

OPTION(BUILD_SHARED_LIBS "Build with shared (.DLL .SO) libraries." ON)

# various compiler stuff
test_big_endian(WORDS_BIGENDIAN)


# check availability of external libs
find_package(PNG)
find_package(TIFF)
find_package(JPEG)
find_package(Jasper)
# TODO: check for libmng, openexr, squish, lcms, nvidia texture tools... others?

# TODO: add options for manually configuring formats (eg IL_NO_GAMES)


if(NOT FOUND_PNG)
    set(IL_NO_PNG 1)
endif(NOT FOUND_PNG)

if(NOT FOUND_JPEG)
    set(IL_NO_JPG 1)
endif(NOT FOUND_JPEG)

if(NOT FOUND_TIFF)
    set(IL_NO_TIF 1)
endif(NOT FOUND_TIFF)

if(NOT FOUND_MNG)
    set(IL_NO_MNG 1)
endif(NOT FOUND_MNG)

# should offer user exr support as an option if openexr lib is found
if(NOT FOUND_OPENEXR)
    set(IL_NO_EXR 1)
endif(NOT FOUND_OPENEXR)

# jpeg2000 support requires libjasper
if(NOT FOUND_JASPER)
    set(IL_NO_JP2 1)
endif(NOT FOUND_JASPER)

# can use lcms lib?
if(NOT FOUND_LCMS)
    set(IL_NO_LCMS 1)
endif(NOT FOUND_LCMS)

# TODO: utx doesn't currently compile (as C, anyway)
set(IL_NO_UTX 1)


# generate the config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/IL/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/include/IL/config.h)

#SET(DEVIL_INCS
#
#	${DEVIL_SOURCE_DIR}/include/IL/config.h
#	${DEVIL_SOURCE_DIR}/include/IL/devil_internal_exports.h
#	${DEVIL_SOURCE_DIR}/include/IL/il.h
#	${DEVIL_SOURCE_DIR}/include/IL/il_wrap.h
#	${DEVIL_SOURCE_DIR}/include/IL/ilu.h
#	${DEVIL_SOURCE_DIR}/include/IL/ilu_region.h
#	${DEVIL_SOURCE_DIR}/include/IL/ilut.h
#)

add_subdirectory(src-IL)
add_subdirectory(src-ILU)
add_subdirectory(src-ILUT)


# need to check for vc6 or vc2003 output dir
IF(BUILD_SHARED_LIBS)
	SET(DEVIL_LIB_DEST_DIR	${CMAKE_CURRENT_SOURCE_DIR}/lib/	)
ELSE(BUILD_SHARED_LIBS)
	SET(DEVIL_LIB_DEST_DIR	${CMAKE_CURRENT_SOURCE_DIR}/lib/	)
ENDIF(BUILD_SHARED_LIBS)


