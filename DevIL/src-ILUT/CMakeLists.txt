cmake_minimum_required(VERSION 2.6)

project(ILUT)

file(GLOB ILUT_SRCS src/*.c src/*.cpp msvc9/il.def)
file(GLOB ILUT_INC include/*.h ../include/IL/devil_internal_exports.h ../include/IL/ilut.h)
file(GLOB ILUT_RSRC)

include_directories( include ../include )

# for windows add the .def and .rc files to the source list
if(WIN32)
	add_definitions(-DALLEGRO_MSVC)
	add_definitions(-DIL_USE_PRAGMA_LIBS)
	if(BUILD_SHARED_LIBS)
		add_definitions(-DILUT_DLL)
        #set(ILUT_SRCS ${ILUT_SRCS} msvc/ilut.def)
		set(ILUT_RSRC ${ILUT_RSRC} "msvc/resources/IL Logo.ico" msvc/ILUT.rc)
		set(CMAKE_C_FLAGS_RELEASE "/MT /O2")
	else(BUILD_SHARED_LIBS)
		add_definitions(-DIL_STATIC_LIB)
	endif(BUILD_SHARED_LIBS)
endif(WIN32)

source_group("Source Files" FILES src/*.c src/*.cpp)
source_group("Header Files" FILES ${ILUT_INC} )
source_group("Resource Files" FILES ${ILUT_RSRC} )

# Remove SHARED to create a static library
add_library(ILUT SHARED ${ILUT_SRCS} ${ILUT_INC} ${ILUT_RSRC})

## add link sub library info
target_link_libraries(ILUT
	IL
	ILU
)

# generate pkg-config file
configure_file( pkgconfig/ILUT.pc.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ILUT.pc @ONLY)

set_target_properties(ILUT PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../lib")


# Installation
install (TARGETS ILUT
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install (FILES ../include/IL/ilut.h DESTINATION include/IL)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ILUT.pc
	DESTINATION lib/pkgconfig
)

