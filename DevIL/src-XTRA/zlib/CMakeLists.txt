PROJECT(ZLIB)

FILE(GLOB ZLIB_SRCS *.c *.h )



# for windows add the .def and .rc files to the source list
IF(WIN32)
  IF(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DZLIB_DLL)
    SET(ZLIB_SRCS ${ZLIB_SRCS} zlib.def zlib.rc  )
  ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

SOURCE_GROUP(code FILES ${ZLIB_SRCS} )

ADD_LIBRARY(DevIL_zlib ZLIB_SRCS)
