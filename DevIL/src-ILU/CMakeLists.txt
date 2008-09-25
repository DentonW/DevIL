PROJECT(DevILU)

FILE(GLOB DevILU_SRCS src/*.c)
FILE(GLOB DevILU_INC include/*.h ../include/IL/config.h ../include/IL/devil_internal_exports.h ../include/IL/ilu.h)
FILE(GLOB DevILU_RSRC)

INCLUDE_DIRECTORIES(

	${DevILU_SOURCE_DIR}/include
)

#SET(DevILU_SRCS ${DevILU_SRCS} ${DEVILU_INCS} )

SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)

#
# for windows add the .def and .rc files to the source list
#
IF(WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
	IF(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DILU_DLL)
		SET(DevILU_SRCS ${DevILU_SRCS} msvc/ilu.def)
		SET(DevILU_RSRC ${DevILU_RSRC} "msvc/resources/IL Logo.ico" msvc/ILU.rc)
		SET(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	ELSE(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DIL_STATIC_LIB)
	ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)


SOURCE_GROUP("Source Files" FILES src/*.c)
SOURCE_GROUP("Header Files" FILES ${DevILU_INC} )
SOURCE_GROUP("Resource Files" FILES ${DevILU_RSRC} )

# Remove SHARED to create a static library
ADD_LIBRARY(DevILU SHARED ${DevILU_SRCS} ${DevILU_INC} ${DevILU_RSRC})


## add link sub library info

TARGET_LINK_LIBRARIES(DevILU

	DevIL
)

INCLUDE_DIRECTORIES(

	${DevIL_SOURCE_DIR}/../include
	${DevIL_SOURCE_DIR}/include
)
