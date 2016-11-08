cmake_minimum_required(VERSION 2.6)


project(ImageLib)

# include our custom modules
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (TestBigEndian)

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




add_subdirectory(src-IL)
#add_subdirectory(src-ILU)
#add_subdirectory(src-ILUT)


# need to check for vc6 or vc2003 output dir
if(BUILD_SHARED_LIBS)
	set(DEVIL_LIB_DEST_DIR	${CMAKE_CURRENT_SOURCE_DIR}/lib/	)
else(BUILD_SHARED_LIBS)
	set(DEVIL_LIB_DEST_DIR	${CMAKE_CURRENT_SOURCE_DIR}/lib/	)
endif(BUILD_SHARED_LIBS)


