cmake_minimum_required(VERSION 2.6)


PROJECT(ImageLib)


OPTION(BUILD_SHARED_LIBS "Build with shared (.DLL .SO) libraries." ON)


SET(DEVIL_INCS

	${DEVIL_SOURCE_DIR}/include/IL/config.h
	${DEVIL_SOURCE_DIR}/include/IL/devil_internal_exports.h
	${DEVIL_SOURCE_DIR}/include/IL/il.h
	${DEVIL_SOURCE_DIR}/include/IL/il_wrap.h
	${DEVIL_SOURCE_DIR}/include/IL/ilu.h
	${DEVIL_SOURCE_DIR}/include/IL/ilu_region.h
	${DEVIL_SOURCE_DIR}/include/IL/ilut.h
)

SUBDIRS(
#	src-XTRA
	src-IL
	src-ILU
	src-ILUT

#	examples
)


# need to check for vc6 or vc2003 output dir
IF(BUILD_SHARED_LIBS)

	SET(DEVIL_LIB_DEST_DIR	${CMAKE_CURRENT_SOURCE_DIR}/lib/	)

ELSE(BUILD_SHARED_LIBS)

	SET(DEVIL_LIB_DEST_DIR	${CMAKE_CURRENT_SOURCE_DIR}/lib/	)

ENDIF(BUILD_SHARED_LIBS)



EXPORT_LIBRARY_DEPENDENCIES(CMakeDepends.txt)