project(DevIL)

file(GLOB DevIL_SRCS src/*.c src/*.cpp)
file(GLOB DevIL_INC include/*.h ../include/config.h ../include/IL/devil_internal_exports.h ../include/IL/il.h)
file(GLOB DevIL_RSRC)
file(GLOB DevIL_TXT)

include_directories( include ../include )

# generate config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

#
# for windows add the .def and .rc files to the source list
#
if(WIN32)
	add_definitions(-DIL_USE_PRAGMA_LIBS)
	if(BUILD_SHARED_LIBS)
		#ADD_DEFINITIONS(-DIL_DLL -DMNG_DLL -DMNG_USE_DLL -DJPEGDLL)
		add_definitions(-DIL_DLL -DJPEGSTATIC)
		set(DevIL_SRCS ${DevIL_SRCS} msvc/il.def)
		set(DevIL_RSRC ${DevIL_RSRC} "msvc/resources/IL Logo.ico" msvc/IL.rc)
        set(DevIL_TXT ${DevIL_TXT} ../AUTHORS ../ChangeLog ../CREDITS ../libraries.txt ../README ../README.cmake ../TODO)
#		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib ")
		set(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	else(BUILD_SHARED_LIBS)
		add_definitions(-DIL_STATIC_LIB -DJPEGSTATIC)
	endif(BUILD_SHARED_LIBS)
endif(WIN32)

source_group("Source Files" FILES src/*.c src/*.cpp)
source_group("Header Files" FILES ${DevIL_INC} )
source_group("Resource Files" FILES ${DevIL_RSRC} )
source_group("Text Files" FILES ${DevIL_TXT} )

if(BUILD_SHARED_LIBS)
    add_library(IL SHARED ${DevIL_SRCS} )
else(BUILD_SHARED_LIBS)
    add_library(IL ${DevIL_SRCS} )
endif(BUILD_SHARED_LIBS)


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
target_link_libraries(IL ${libs})


# Installation

install (TARGETS IL
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install (FILES ../include/IL/il.h DESTINATION include/IL)
