


PROJECT(DEVIL)


OPTION(BUILD_SHARED_LIBS "Build with shared (.DLL .SO) libraries." OFF)


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
	src-XTRA
	src-IL
	src-ILU
	src-ILUT

	examples
)


EXPORT_LIBRARY_DEPENDENCIES(CMakeDepends.txt)