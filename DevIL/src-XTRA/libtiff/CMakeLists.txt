PROJECT(LIBTIFF)

#FILE(GLOB LIBTIFF_SRCS *.c *.h )

SET(LIBTIFF_SRCS

	t4.h
	tiff.h
	tiffcomp.h
	tiffconf.h
	tiffio.h
	tiffiop.h
	tiffvers.h
	tif_dir.h
	tif_fax3.h
	tif_predict.h
	uvcode.h

	fax3sm_winnt.c
	tif_aux.c
	tif_close.c
	tif_codec.c
	tif_color.c
	tif_compress.c
	tif_dir.c
	tif_dirinfo.c
	tif_dirread.c
	tif_dirwrite.c
	tif_dumpmode.c
	tif_error.c
	tif_extension.c
	tif_fax3.c
	tif_flush.c
	tif_getimage.c
	tif_jpeg.c
	tif_luv.c
	tif_lzw.c
	tif_next.c
	tif_ojpeg.c
	tif_open.c
	tif_packbits.c
	tif_pixarlog.c
	tif_predict.c
	tif_print.c
	tif_read.c
	tif_strip.c
	tif_swab.c
	tif_thunder.c
	tif_tile.c
	tif_version.c
	tif_warning.c
	tif_write.c
	tif_zip.c
)

SET(LIBRARY_OUTPUT_PATH		${DEVIL_LIB_DEST_DIR}	)


# for windows add the .def and .rc files to the source list on DLL builds
IF(WIN32)
	ADD_DEFINITIONS(-DIL_USE_PRAGMA_LIBS)
  SET(LIBTIFF_SRCS ${LIBTIFF_SRCS} tif_win32.c  )
  IF(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DLIBTIFF_DLL)
    SET(LIBTIFF_SRCS ${LIBTIFF_SRCS} libtiff.def libtiff.rc  )
  ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

SOURCE_GROUP(code FILES ${LIBTIFF_SRCS} )

ADD_LIBRARY(DevIL_libtiff LIBTIFF_SRCS)
