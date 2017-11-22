cmake_minimum_required(VERSION 2.6)

project(ILUT)

file(GLOB ILUT_SRCS src/*.cpp)
file(GLOB ILUT_INC include/*.h ../include/IL/devil_internal_exports.h ../include/IL/ilut.h)
file(GLOB ILUT_RSRC)

include_directories( include ../include )

# Find glut
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

# Find opengl
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

# for windows add the .def and .rc files to the source list
if(WIN32)
	add_definitions(-DALLEGRO_MSVC)
	add_definitions(-DIL_USE_PRAGMA_LIBS)
	if(BUILD_SHARED_LIBS)
		add_definitions(-DILUT_DLL)
        set(ILUT_SRCS ${ILUT_SRCS} msvc/ilut.def)
		if(UNICODE)
			set(ILUT_RSRC ${ILUT_RSRC} "msvc/resources/IL Logo.ico" "msvc/ILUT Unicode.rc")
		else(UNICODE)
			set(ILUT_RSRC ${ILUT_RSRC} "msvc/resources/IL Logo.ico" "msvc/ILUT.rc")
		endif(UNICODE)
		if(MSVC)
			set(CMAKE_C_FLAGS_RELEASE "/MT /O2")
		endif(MSVC)
	else(BUILD_SHARED_LIBS)
		add_definitions(-DIL_STATIC_LIB)
	endif(BUILD_SHARED_LIBS)
endif(WIN32)

source_group("Source Files" FILES src/*.cpp)
source_group("Header Files" FILES ${ILUT_INC} )
source_group("Resource Files" FILES ${ILUT_RSRC} )

# Remove SHARED to create a static library
add_library(ILUT SHARED ${ILUT_SRCS} ${ILUT_INC} ${ILUT_RSRC})
set_target_properties(ILUT PROPERTIES SOVERSION 1)

## add link sub library info
target_link_libraries(ILUT
	IL
	ILU
	${OPENGL_LIBRARIES}
	${GLUT_LIBRARY}
)

# generate pkg-config file
configure_file( pkgconfig/ILUT.pc.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ILUT.pc @ONLY)


if(UNICODE)
	MESSAGE("Compiling ILUT Unicode")
	add_definitions(-DUNICODE -D_UNICODE)
endif(UNICODE)

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

