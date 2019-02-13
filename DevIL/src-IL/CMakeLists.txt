cmake_minimum_required(VERSION 2.8)
project(DevIL)

include (TestBigEndian)

option(BUILD_SHARED_LIBS "Build with shared (.DLL .SO) libraries." ON)

# libSquish not currently supported by CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DevIL_SOURCE_DIR}/cmake/Modules")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB DevIL_SRCS src/*.cpp)
file(GLOB DevIL_INC include/*.h ../include/config.h ../include/IL/devil_internal_exports.h ../include/IL/il.h)
file(GLOB DevIL_RSRC)
file(GLOB DevIL_TXT)

# From http://stackoverflow.com/questions/17317350/compiling-32-and-64-bit
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "DevIL, 64 bit compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE( "DevIL, 32 bit compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

include_directories( include ../include )

# for windows add the .def and .rc files to the source list
if(WIN32)
    #add_definitions(-DIL_USE_PRAGMA_LIBS)
	if(BUILD_SHARED_LIBS)
		#ADD_DEFINITIONS(-DIL_DLL -DMNG_DLL -DMNG_USE_DLL -DJPEGDLL)
		add_definitions(-DIL_DLL -DJPEGSTATIC)
        # note: .def not used for now - __declspec() in source handles export.
        set(DevIL_SRCS ${DevIL_SRCS} msvc/il.def)
		if(${MSVC_VERSION} GREATER 1899) # MSVC 2015 or later
			option(DEVIL_INCLUDE_RESOURCES "DevIL: compile-in resources" OFF)
		else()
			option(DEVIL_INCLUDE_RESOURCES "DevIL: compile-in resources" ON)
		endif()
		if(DEVIL_INCLUDE_RESOURCES)
			if(UNICODE)
				set(DevIL_RSRC ${DevIL_RSRC} "msvc/resources/IL Logo.ico" "msvc/IL Unicode.rc")
			else(UNICODE)
				set(DevIL_RSRC ${DevIL_RSRC} "msvc/resources/IL Logo.ico" "msvc/IL.rc")
			endif(UNICODE)
		endif(DEVIL_INCLUDE_RESOURCES)
        set(DevIL_TXT ${DevIL_TXT} ../AUTHORS ../ChangeLog ../CREDITS ../Libraries.txt ../README.md ../README.cmake ../TODO)
#		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib ")
		set(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	else(BUILD_SHARED_LIBS)
		add_definitions(-DIL_STATIC_LIB -DJPEGSTATIC)
	endif(BUILD_SHARED_LIBS)
endif(WIN32)

source_group("Source Files" FILES src/*.cpp)
source_group("Header Files" FILES ${DevIL_INC} )
source_group("Resource Files" FILES ${DevIL_RSRC} )
source_group("Text Files" FILES ${DevIL_TXT} )

if(BUILD_SHARED_LIBS)
    add_library(IL SHARED ${DevIL_SRCS} ${DevIL_INC} ${DevIL_RSRC} ${DevIL_TXT})
    set_target_properties(IL PROPERTIES SOVERSION 1)
else(BUILD_SHARED_LIBS)
    add_library(IL ${DevIL_SRCS} ${DevIL_INC} ${DevIL_RSRC} ${DevIL_TXT})
endif(BUILD_SHARED_LIBS)

# various compiler stuff
test_big_endian(WORDS_BIGENDIAN)


# Check availability of optional external libs, 
# Allow users to specify manual locations in their own cmake file like so:
# SET(IL_JPEG_LIB ON CACHE BOOL "" FORCE) 
# SET(JPEG_INCLUDE_DIRS ...)
# SET(JPEG_LIBRARIES ...)

#LibPng
option(IL_PNG_LIB "Custom location for LibPng" OFF)
if(NOT IL_PNG_LIB)
  find_package(PNG)
endif(NOT IL_PNG_LIB)

#LibJpeg
option(IL_JPEG_LIB "Custom location for LibJPEG" OFF)
if(NOT IL_JPEG_LIB)
  find_package(JPEG)
endif(NOT IL_JPEG_LIB)

#TIFF
option(IL_TIFF_LIB "Custom location for TIFF lib" OFF)
if(NOT IL_TIFF_LIB)
  find_package(TIFF)
endif(NOT IL_TIFF_LIB)

#Jasper
option(IL_JASPER_LIB "Custom location for Jasper" OFF)
if(NOT IL_JASPER_LIB)
  find_package(Jasper)
endif(NOT IL_JASPER_LIB)

#Squish
option(IL_SQUISH_LIB "Custom location for libSquish" OFF)
if(NOT IL_SQUISH_LIB)
#find_package(Squish)
  find_package(libSquish)
endif(NOT IL_SQUISH_LIB)

# NVidia texture tools
option(IL_NVTT_LIB "Custom location for NVTT" OFF)
if(NOT IL_NVTT_LIB)
  find_package(NVTT)
endif(NOT IL_NVTT_LIB)

#OpenEXR
option(IL_OPENEXR_LIB "Custom location for OpenEXR" OFF)
if(NOT IL_OPENEXR_LIB)
  find_package(OpenEXR)
endif(NOT IL_OPENEXR_LIB)

#LCMS2
option(IL_LCMS2_LIB "Custom location for LCMS2" OFF)
if(NOT IL_LCMS2_LIB)
  #find_package(LCMS)
  find_package(LCMS2)
endif(NOT IL_LCMS2_LIB)

#MNG
option(IL_MNG_LIB "Custom location for MNG" OFF)
if(NOT IL_MNG_LIB)
  find_package(MNG)
endif(NOT IL_MNG_LIB)

# TODO: check for libmng, others?

# TODO: WDP requires Microsoft HD Photo Device Porting Kit 1.0
set(IL_NO_WDP 1)

# TODO: add options for manually configuring formats (eg IL_NO_GAMES)

# present options for the libraries which are available
if(NOT ${IL_PNG_LIB})
  if(${PNG_FOUND})
      option(IL_NO_PNG "Disable PNG support (libpng)" 0)
  else(${PNG_FOUND})
      set(IL_NO_PNG 1)
  endif(${PNG_FOUND})
endif(NOT ${IL_PNG_LIB})

if(NOT ${IL_TIFF_LIB})
  if(TIFF_FOUND)
      option(IL_NO_TIF "Disable TIFF support (libtiff)" 0)
  else(TIFF_FOUND)
      set(IL_NO_TIF 1)
  endif(TIFF_FOUND)
endif(NOT ${IL_TIFF_LIB})

if(NOT ${IL_JPEG_LIB})
  if(${JPEG_FOUND})
      option(IL_NO_JPG "Disable JPEG support (libjpeg)" 0)
  else(${JPEG_FOUND})
      set(IL_NO_JPG 1)
  endif(${JPEG_FOUND})
endif(NOT ${IL_JPEG_LIB})

if(NOT ${IL_OPENEXR_LIB})
  if(OPENEXR_FOUND)
      option(IL_NO_EXR "Disable EXR support (openEXR)" 0)
  else(OPENEXR_FOUND)
      set(IL_NO_EXR 1)
  endif(OPENEXR_FOUND)
endif(NOT ${IL_OPENEXR_LIB})

if(NOT ${IL_JASPER_LIB})
  if(JASPER_FOUND)
      option(IL_NO_JP2 "Disable JP2 support (libjasper)" 0)
  else(JASPER_FOUND)
      set(IL_NO_JP2 1)
  endif(JASPER_FOUND)
endif(NOT ${IL_JASPER_LIB})

if(NOT ${IL_MNG_LIB})
  if(MNG_FOUND)
      option(IL_NO_MNG "Disable MNG support (libmng)" 0)
  else(MNG_FOUND)
      set(IL_NO_MNG 1)
  endif(MNG_FOUND)
endif(NOT ${IL_MNG_LIB})

if(NOT ${IL_LCMS2_LIB})
  if(LCMS2_FOUND)
      option(IL_NO_LCMS "Disable LCMS support (Little CMS)" 0)
      #TODO: be more clever about lcms include... it smells wrong.
      option(LCMS_NODIRINCLUDE "Include lcms.h instead of lcms/lcms.h" 1)
  else(LCMS2_FOUND)
      set(IL_NO_LCMS 1)
  endif(LCMS2_FOUND)
endif(NOT ${IL_LCMS2_LIB})

if(NOT ${IL_NVTT_LIB})
  if(NVTT_FOUND)
      option(IL_USE_DXTC_NVIDIA "Use Nvidia Texture Tools (NVTT) for DXTC support" 1)
  else(NVTT_FOUND)
      set(IL_USE_DXTC_NVIDIA 0)
  endif(NVTT_FOUND)
endif(NOT ${IL_NVTT_LIB})

if(NOT ${IL_SQUISH_LIB})
  if(LIBSQUISH_FOUND)
      option(IL_USE_DXTC_SQUISH "Use libsquish for DXTC support" 1)
  else(LIBSQUISH_FOUND)
      set(IL_USE_DXTC_SQUISH 0)
  endif(LIBSQUISH_FOUND)
endif(NOT ${IL_SQUISH_LIB})

# Sets the output folders
set(LIBDIR "../lib/")
set(LIBDIR "${LIBDIR}${EX_PLATFORM_NAME}")
if(UNICODE)
	set(LIBDIR "${LIBDIR}/unicode")
endif(UNICODE)
set_target_properties(IL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${LIBDIR})
set_target_properties(IL PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBDIR})
set_target_properties(IL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIBDIR})

if(MSVC)
	# Linux uses libIL.so, libILU.so, and libILUT.so, but Windows uses
	#  DevIL.dll, ILU.dll, and ILUT.dll.
	set_target_properties(IL PROPERTIES OUTPUT_NAME "DevIL")
endif(MSVC)

if(UNICODE)
	MESSAGE("DevIL, Compiling IL Unicode")
	add_definitions(-DUNICODE -D_UNICODE)
endif(UNICODE)
#if(COMPILER_MSVC)

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
  list(APPEND incs ${MNG_INCLUDE_DIR} )
  list(APPEND libs ${MNG_LIBRARIES} )
endif(NOT IL_NO_MNG)

if(NOT IL_NO_LCMS)
  list(APPEND incs ${LCMS2_INCLUDE_DIRS} )
  list(APPEND libs ${LCMS2_LIBRARIES} )
endif(NOT IL_NO_LCMS)

if(NOT IL_NO_EXR)
  list(APPEND incs ${OPENEXR_INCLUDE_DIR} )
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

if(IL_USE_DXTC_SQUISH)
    list(APPEND incs ${LIBSQUISH_INCLUDE_DIR} )
    #list(APPEND libs ${LIBSQUISH_LIBRARY} )
	# Deals with strangeness in Visual Studio linking DevIL debug to libsquish release
	if(LIBSQUISH_LIBRARY_DEBUG)
		set(SQUISH_LINK_LIBRARY optimized ${LIBSQUISH_LIBRARY} debug ${LIBSQUISH_LIBRARY_DEBUG})
	else(LIBSQUISH_LIBRARY_DEBUG)
		set(SQUISH_LINK_LIBRARY ${LIBSQUISH_LIBRARY})
	endif(LIBSQUISH_LIBRARY_DEBUG)
	list(APPEND libs ${SQUISH_LINK_LIBRARY} )
endif(IL_USE_DXTC_SQUISH)


include_directories(${incs} "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(IL ${libs})

# generate config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)



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

