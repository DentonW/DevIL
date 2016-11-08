cmake_minimum_required(VERSION 2.6)

project(DevIL)

# include our custom modules
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (TestBigEndian)


file(GLOB DevIL_SRCS src/*.c src/*.cpp)
file(GLOB DevIL_INC include/*.h ../include/config.h ../include/IL/devil_internal_exports.h ../include/IL/il.h)
file(GLOB DevIL_RSRC)
file(GLOB DevIL_TXT)

include_directories( include ../include )

# for windows add the .def and .rc files to the source list
if(WIN32)
    #add_definitions(-DIL_USE_PRAGMA_LIBS)
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



option(BUILD_SHARED_LIBS "Build with shared (.DLL .SO) libraries." ON)

# various compiler stuff
test_big_endian(WORDS_BIGENDIAN)


# check availability of optional external libs
find_package(PNG)
find_package(TIFF)
find_package(JPEG)
find_package(Jasper)
find_package(Squish)
find_package(NVTT)      # NVidia texture tools
find_package(OpenEXR)
find_package(LCMS)

# TODO: check for libmng, others?


# TODO: utx doesn't currently compile (as C, anyway)
set(IL_NO_UTX 1)

# TODO: WDP requires Microsoft HD Photo Device Porting Kit 1.0
set(IL_NO_WDP 1)

# TODO: add options for manually configuring formats (eg IL_NO_GAMES)


# present options for the libraries which are available
if(PNG_FOUND)
    option(IL_NO_PNG "Disable PNG support (libpng)" 0)
else(PNG_FOUND)
    set(IL_NO_PNG 1)
endif(PNG_FOUND)

if(TIFF_FOUND)
    option(IL_NO_TIF "Disable TIFF support (libtiff)" 0)
else(TIFF_FOUND)
    set(IL_NO_TIF 1)
endif(TIFF_FOUND)

if(JPEG_FOUND)
    option(IL_NO_JPG "Disable JPEG support (libjpeg)" 0)
else(JPEG_FOUND)
    set(IL_NO_JPG 1)
endif(JPEG_FOUND)

if(OPENEXR_FOUND)
    option(IL_NO_EXR "Disable EXR support (openEXR)" 0)
else(OPENEXR_FOUND)
    set(IL_NO_EXR 1)
endif(OPENEXR_FOUND)

if(JASPER_FOUND)
    option(IL_NO_JP2 "Disable JP2 support (libjasper)" 0)
else(JASPER_FOUND)
    set(IL_NO_JP2 1)
endif(JASPER_FOUND)

if(MNG_FOUND)
    option(IL_NO_MNG "Disable MNG support (libmng)" 0)
else(MNG_FOUND)
    set(IL_NO_MNG 1)
endif(MNG_FOUND)

if(LCMS_FOUND)
    option(IL_NO_LCMS "Disable LCMS support (Little CMS)" 0)
    #TODO: be more clever about lcms include... it smells wrong.
    option(LCMS_NODIRINCLUDE "Include lcms.h instead of lcms/lcms.h" 1)
else(LCMS_FOUND)
    set(IL_NO_LCMS 1)
endif(LCMS_FOUND)

if(NVTT_FOUND)
    option(IL_USE_DXTC_NVIDIA "Use Nvidia Texture Tools (NVTT) for DXTC support" 1)
else(NVTT_FOUND)
    set(IL_USE_DXTC_NVIDIA 0)
endif(NVTT_FOUND)

if(SQUISH_FOUND)
    option(IL_USE_DXTC_SQUISH "Use libsquish for DXTC support" 1)
else(SQUISH_FOUND)
    set(IL_USE_DXTC_SQUISH 0)
endif(SQUISH_FOUND)







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

# generate config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)



# generate pkg-config file
# TODO: add Requires.private or Libs.private
# (needed to support static linking?)
# TODO: sort out version number
configure_file( pkgconfig/IL.pc.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/IL.pc @ONLY)


# Installation
install (TARGETS IL
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install (FILES ../include/IL/il.h DESTINATION include/IL)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/IL.pc
	DESTINATION lib/pkgconfig
)

