PROJECT(LIBJPEG)

# memmgr back ends: compile only one of these into a working library
# (For now, let's use the mode that requires the image fit into memory.
# This is the recommended mode for Win32 anyway.)
SET(systemdependent_SRCS jmemnobs.c)

# library object files common to compression and decompression
SET(common_SRCS 
jcomapi.c jutils.c jerror.c jmemmgr.c
)

# compression library object files
SET(compression_SRCS
jcapimin.c jcapistd.c jctrans.c jcparam.c jdatadst.c jcinit.c 
jcmaster.c jcmarker.c jcmainct.c jcprepct.c jccoefct.c jccolor.c
jcsample.c jchuff.c jcphuff.c jcdctmgr.c jfdctfst.c jfdctflt.c 
jfdctint.c
)

# decompression library object files
SET(decompression_SRCS
jdapimin.c jdapistd.c jdtrans.c jdatasrc.c jdmaster.c 
jdinput.c jdmarker.c jdhuff.c jdphuff.c jdmainct.c jdcoefct.c 
jdpostct.c jddctmgr.c jidctfst.c jidctflt.c jidctint.c jidctred.c 
jdsample.c jdcolor.c jquant1.c jquant2.c jdmerge.c
)

FILE(GLOB libjpeg_HEADERS *.h )

SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)

IF (WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
   IF (BUILD_SHARED_LIBS)
      ADD_DEFINITIONS( -DJPEGDLL -DUSE_WINDOWS_MESSAGEBOX )
   ELSE (BUILD_SHARED_LIBS)
      ADD_DEFINITIONS( -DJPEGSTATIC -DUSE_WINDOWS_MESSAGEBOX )
   ENDIF (BUILD_SHARED_LIBS)
ENDIF (WIN32)


SOURCE_GROUP(code FILES ${systemdependent_SRCS} ${common_SRCS} ${compression_SRCS} ${decompression_SRCS} ${libjpeg_HEADERS} )

ADD_LIBRARY(DevIL_libjpeg systemdependent_SRCS common_SRCS compression_SRCS decompression_SRCS libjpeg_HEADERS)

