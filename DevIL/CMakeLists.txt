cmake_minimum_required(VERSION 2.6)

project(ImageLib)
# include our custom modules
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")


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

# Sets the output folders
set(LIBDIR "../lib/")
set(LIBDIR "${LIBDIR}${EX_PLATFORM_NAME}")
if(UNICODE)
	set(LIBDIR "${LIBDIR}/unicode")
endif(UNICODE)

set(IL_RUNTIME_DIR ${LIBDIR} CACHE PATH "Output location for Devil .dlls/.so")
set(IL_LIBRARY_DIR ${LIBDIR} CACHE PATH "Output location for Devil .libs")
# Sets the output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${IL_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${IL_LIBRARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${IL_LIBRARY_DIR})

add_subdirectory(src-IL)

option(IL_BUILD_ILU "build ILU library" ON)
if (IL_BUILD_ILU)
    add_subdirectory(src-ILU)
endif()

option(IL_BUILD_ILUT "build ILUT library" ON)
#Could avoid this extra check with CMakeDependentOption
#But that would require bumping cmake version to 3.1
if (IL_BUILD_ILUT AND (NOT IL_BUILD_ILU))
    message(FATAL_ERROR "Can't build ILUT without ILU. Enable IL_BUILD_ILU")
elseif(IL_BUILD_ILUT)
    add_subdirectory(src-ILUT)
endif()

option(IL_TESTS "build DevIL tests" ON)
if (IL_TESTS)
    ENABLE_TESTING()
    Add_Subdirectory(test)
endif()

