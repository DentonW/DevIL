PROJECT(ILU)

FILE(GLOB ILU_SRCS src/*.c)
FILE(GLOB ILU_INC include/*.h ../include/IL/config.h ../include/IL/devil_internal_exports.h ../include/IL/ilu.h)
FILE(GLOB ILU_RSRC)

INCLUDE_DIRECTORIES(

	${ILU_SOURCE_DIR}/include
)

#SET(ILU_SRCS ${ILU_SRCS} ${ILU_INCS} )

SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)

#
# for windows add the .def and .rc files to the source list
#
IF(WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
	IF(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DILU_DLL)
		SET(ILU_SRCS ${ILU_SRCS} msvc/ilu.def)
		SET(ILU_RSRC ${ILU_RSRC} "msvc/resources/IL Logo.ico" msvc/ILU.rc)
		SET(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	ELSE(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DIL_STATIC_LIB)
	ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)


SOURCE_GROUP("Source Files" FILES src/*.c)
SOURCE_GROUP("Header Files" FILES ${ILU_INC} )
SOURCE_GROUP("Resource Files" FILES ${ILU_RSRC} )

# Remove SHARED to create a static library
ADD_LIBRARY(ILU SHARED ${ILU_SRCS} ${ILU_INC} ${ILU_RSRC})


## add link sub library info

TARGET_LINK_LIBRARIES(ILU

	DevIL
)

INCLUDE_DIRECTORIES(

	${DevIL_SOURCE_DIR}/../include
	${DevIL_SOURCE_DIR}/include
)
