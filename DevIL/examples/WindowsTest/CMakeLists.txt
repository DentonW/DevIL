PROJECT(Example_WindowsTest)

FILE(GLOB Example_WindowsTest_SRCS *.c *.cpp *.h *.rc )

INCLUDE_DIRECTORIES(

	${CMAKE_CURRENT_SOURCE_DIR}
	${DevIL_SOURCE_DIR}/include
	${DevIL_SOURCE_DIR}/../include
)

LINK_DIRECTORIES(

	${DEVIL_LIB_DEST_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${DevIL_XTRA_SOURCE_DIR}/colorpicker
)

#
# for windows add the .def and .rc files to the source list
#
IF(WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
	IF(BUILD_SHARED_LIBS)
	ELSE(BUILD_SHARED_LIBS)
		ADD_DEFINITIONS(-DIL_STATIC_LIB)
	ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

SOURCE_GROUP(code FILES ${Example_WindowsTest_SRCS} )

ADD_EXECUTABLE(Example_WindowsTest WIN32 Example_WindowsTest_SRCS)

## add link sub library info

TARGET_LINK_LIBRARIES(Example_WindowsTest
	
	DevIL
	DevILU
	DevILUT
)

INCLUDE_DIRECTORIES(

	 ${DevIL_SOURCE_DIR}
	 ${DevILU_SOURCE_DIR}
	 ${DevILUT_SOURCE_DIR}
)


#
# Copy all needed dlls into the output dir, so stuff just works from vcc
#
IF(WIN32)
IF(BUILD_SHARED_LIBS)

	ADD_CUSTOM_COMMAND( TARGET Example_WindowsTest POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy
		${DevIL_XTRA_SOURCE_DIR}/colorpicker/ColorPicker.dll
		${CMAKE_CFG_INTDIR}
	)
	ADD_CUSTOM_COMMAND( TARGET Example_WindowsTest POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy
		${DEVIL_LIB_DEST_DIR}/*.dll
		${CMAKE_CFG_INTDIR}
	)
ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)
