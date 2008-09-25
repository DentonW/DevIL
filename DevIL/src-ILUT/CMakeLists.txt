PROJECT(DevILUT)

FILE(GLOB DevILUT_SRCS src/*.c)
FILE(GLOB DevILUT_INC include/*.h ../include/IL/config.h ../include/IL/devil_internal_exports.h ../include/IL/ilut.h)
FILE(GLOB DevILUT_RSRC)

INCLUDE_DIRECTORIES(

	${DevILUT_SOURCE_DIR}/include
)

#SET(DevILUT_SRCS ${DevILUT_SRCS} ${DEVILUT_INCS} )


SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)

#
# for windows add the .def and .rc files to the source list
#
IF(WIN32)
	ADD_DEFINITIONS(-DALLEGRO_MSVC)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
	IF(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DILUT_DLL)
		SET(DevILU_SRCS ${DevILU_SRCS} msvc/ilu.def)
		SET(DevILU_RSRC ${DevILU_RSRC} "msvc/resources/IL Logo.ico" msvc/ILU.rc)
		SET(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	ELSE(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DIL_STATIC_LIB)
	ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

SOURCE_GROUP("Source Files" FILES src/*.c)
SOURCE_GROUP("Header Files" FILES ${DevILUT_INC} )
SOURCE_GROUP("Resource Files" FILES ${DevILUT_RSRC} )

# Remove SHARED to create a static library
ADD_LIBRARY(DevILUT SHARED ${DevILUT_SRCS} ${DevILUT_INC} ${DevILUT_RSRC})


## add link sub library info

TARGET_LINK_LIBRARIES(DevILUT

	DevIL
	DevILU
)

INCLUDE_DIRECTORIES(

	${DevIL_SOURCE_DIR}/../include
	${DevIL_SOURCE_DIR}/include
	${DevILU_SOURCE_DIR}/include
)
