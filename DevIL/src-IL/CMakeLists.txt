PROJECT(DevIL)

FILE(GLOB DevIL_SRCS src/*.c src/*.cpp)
FILE(GLOB DevIL_INC include/*.h ../include/IL/config.h ../include/IL/devil_internal_exports.h ../include/IL/il.h)
FILE(GLOB DevIL_RSRC)
FILE(GLOB DevIL_TXT)

INCLUDE_DIRECTORIES(

	${DevIL_SOURCE_DIR}/include
	${DevIL_SOURCE_DIR}/../include
)

#SET(DevIL_INC ${DEVIL_INCS} ${DEVIL_SRCS})

SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)

#
# for windows add the .def and .rc files to the source list
#
IF(WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
	IF(BUILD_SHARED_LIBS)
		#ADD_DEFINITIONS(-DIL_DLL -DMNG_DLL -DMNG_USE_DLL -DJPEGDLL)
		ADD_DEFINITIONS(-DIL_DLL -DJPEGSTATIC)
		SET(DevIL_SRCS ${DevIL_SRCS} msvc/il.def)
		SET(DevIL_RSRC ${DevIL_RSRC} "msvc/resources/IL Logo.ico" msvc/IL.rc)
        SET(DevIL_TXT ${DevIL_TXT} ../AUTHORS ../ChangeLog ../CREDITS ../libraries.txt ../README ../README.cmake ../TODO)
#		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib ")
		SET(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	ELSE(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DIL_STATIC_LIB -DJPEGSTATIC)
	ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

SOURCE_GROUP("Source Files" FILES src/*.c src/*.cpp)
SOURCE_GROUP("Header Files" FILES ${DevIL_INC} )
SOURCE_GROUP("Resource Files" FILES ${DevIL_RSRC} )
SOURCE_GROUP("Text Files" FILES ${DevIL_TXT} )

IF(BUILD_SHARED_LIBS)
    ADD_LIBRARY(DevIL SHARED ${DevIL_SRCS} ${DevIL_INC} ${DevIL_RSRC} ${DevIL_TXT})
ELSE(BUILD_SHARED_LIBS)
    ADD_LIBRARY(DevIL ${DevIL_SRCS} ${DevIL_INC} ${DevIL_RSRC} ${DevIL_TXT})
ENDIF(BUILD_SHARED_LIBS)


# configure include dirs and linkage for the optional external libs
set(libs "")
set(incs "")

if(NOT IL_NO_PNG)
    list(APPEND incs ${PNG_INCLUDE_DIRS} )
    list(APPEND libs ${PNG_LIBRARIES} )
endif(NOT IL_NO_PNG)

if(NOT IL_NO_JPG)
    list(APPEND incs ${JPEG_INCLUDE_DIRS} )
    list(APPEND libs ${JPEG_LIBRARIES} )
endif(NOT IL_NO_JPG)

if(NOT IL_NO_TIF)
    list(APPEND incs ${TIFF_INCLUDE_DIRS} )
    list(APPEND libs ${TIFF_LIBRARIES} )
endif(NOT IL_NO_TIF)

if(NOT IL_NO_JP2)
    list(APPEND incs ${JASPER_INCLUDE_DIR} )
    list(APPEND libs ${JASPER_LIBRARIES} )
endif(NOT IL_NO_JP2)

if(NOT IL_NO_MNG)
    list(APPEND incs ${MNG_INCLUDE_DIRS} )
    list(APPEND libs ${MNG_LIBRARIES} )
endif(NOT IL_NO_MNG)

if(NOT IL_NO_LCMS)
    list(APPEND incs ${LCMS_INCLUDE_DIRS} )
    list(APPEND libs ${LCMS_LIBRARIES} )
endif(NOT IL_NO_LCMS)

if(NOT IL_NO_EXR)
    list(APPEND incs ${OPENEXR_INCLUDE_DIRS} )
    list(APPEND libs ${OPENEXR_LIBRARIES} )
endif(NOT IL_NO_EXR)

if(IL_USE_DXTC_NVIDIA)
    list(APPEND incs ${NVTT_INCLUDE_DIR} )
    list(APPEND libs ${NVTT_LIBRARY} )
endif(IL_USE_DXTC_NVIDIA)

if(IL_USE_DXTC_NVIDIA)
    list(APPEND incs ${NVTT_INCLUDE_DIR} )
    list(APPEND libs ${NVTT_LIBRARY} )
endif(IL_USE_DXTC_NVIDIA)

# TODO: does Squish require linking/includes?


include_directories(${incs})
target_link_libraries(DevIL ${libs})


# Installation

install (TARGETS DevIL
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install (FILES ../include/IL/il.h DESTINATION include/IL)
