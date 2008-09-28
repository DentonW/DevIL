//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 08/24/2008
//
// Filename: src-IL/src/il_jp2.c
//
// Description: Jpeg-2000 (.jp2) functions
//
//-----------------------------------------------------------------------------


// @TODO:  Look in jas_stream.c of JasPer and make own version of
//  jas_stream_fopen that will return a jas_stream_t and
//  work on file streams instead of just a filename.

#include "il_internal.h"
#ifndef IL_NO_JP2
#include <jasper/jasper.h>
#include "il_jp2.h"

//! Reads a Jpeg2000 file.
ILboolean ilLoadJp2(ILconst_string FileName)
{
	ILHANDLE	Jp2File;
	ILboolean	bRet;

	Jp2File = iopenr(FileName);
	if (Jp2File == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	bRet = ilLoadJp2F(Jp2File);
	icloser(Jp2File);

	return bRet;
}


//! Reads an already-opened Jpeg2000 file.
ILboolean ilLoadJp2F(ILHANDLE File)
{
	ILuint			FirstPos;
	ILboolean		bRet;
	jas_stream_t	*Stream;

	iSetInputFile(File);
	FirstPos = itell();

	if (jas_init())
	{
		ilSetError(IL_LIB_JP2_ERROR);
		return IL_FALSE;
	}
	Stream = iJp2ReadStream();
	if (!Stream)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	bRet = iLoadJp2Internal(Stream, NULL);
	// Close the input stream.
	jas_stream_close(Stream);

	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Jpeg2000 stream.
ILboolean ilLoadJp2L(const ILvoid *Lump, ILuint Size)
{
	return ilLoadJp2LInternal(Lump, Size, NULL);
}


//! This is separated so that it can be called for other file types, such as .icns.
ILboolean ilLoadJp2LInternal(const ILvoid *Lump, ILuint Size, ILimage *Image)
{
	ILboolean		bRet;
	jas_stream_t	*Stream;

	if (jas_init())
	{
		ilSetError(IL_LIB_JP2_ERROR);
		return IL_FALSE;
	}
	Stream = jas_stream_memopen((char*)Lump, Size);
	if (!Stream)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	bRet = iLoadJp2Internal(Stream, Image);
	// Close the input stream.
	jas_stream_close(Stream);

	return bRet;
}


// Internal function used to load the Jpeg2000 stream.
ILboolean iLoadJp2Internal(jas_stream_t	*Stream, ILimage *Image)
{
	jas_image_t		*Jp2Image = NULL;
	jas_matrix_t	*origdata;
	ILuint			x, y, c;
	ILimage			*TempImage;

	// Decode image
	Jp2Image = jas_image_decode(Stream, -1, 0);
	if (!Jp2Image)
	{
		ilSetError(IL_ILLEGAL_FILE_VALUE);
		jas_stream_close(Stream);
		return IL_FALSE;
	}

	// We're not supporting anything other than 8 bits/component yet.
	if (jas_image_cmptprec(Jp2Image, 0) != 8)
	{
		jas_image_destroy(Jp2Image);
		ilSetError(IL_ILLEGAL_FILE_VALUE);
		return IL_FALSE;
	}

	switch (jas_image_numcmpts(Jp2Image))
	{
		case 3:
			if (Image == NULL) {
				ilTexImage(jas_image_width(Jp2Image), jas_image_height(Jp2Image), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
				TempImage = iCurImage;
			}
			else {
				ifree(Image->Data);  // @TODO: Not really the most efficient way to do this...
				ilInitImage(Image, jas_image_width(Jp2Image), jas_image_height(Jp2Image), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
				TempImage = Image;
			}
			break;
		case 4:
			if (Image == NULL) {
				ilTexImage(jas_image_width(Jp2Image), jas_image_height(Jp2Image), 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
				TempImage = iCurImage;
			}
			else {
				ifree(Image->Data);  // @TODO: Not really the most efficient way to do this...
				ilInitImage(Image, jas_image_width(Jp2Image), jas_image_height(Jp2Image), 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
				TempImage = Image;
			}
			break;
		default:
			jas_image_destroy(Jp2Image);
			ilSetError(IL_ILLEGAL_FILE_VALUE);
			return IL_FALSE;
	}
	TempImage->Origin = IL_ORIGIN_UPPER_LEFT;

	// JasPer stores the data channels separately.
	//  I am assuming RGBA format.  Is it possible for other formats to be included?
	for (c = 0; c < TempImage->Bpp; c++)
	{
		origdata = jas_matrix_create(TempImage->Height, TempImage->Width);
		if (!origdata)
		{
			ilSetError(IL_LIB_JP2_ERROR);
			return IL_FALSE;  // @TODO: Error
		}
		// Have to convert data into an intermediate matrix format.
		if (jas_image_readcmpt(Jp2Image, c, 0, 0, TempImage->Width, TempImage->Height, origdata))
		{
			return IL_FALSE;
		}

		for (y = 0; y < TempImage->Height; y++)
		{
			for (x = 0; x < TempImage->Width; x++)
			{
				TempImage->Data[y * TempImage->Width * TempImage->Bpp + x * TempImage->Bpp + c] = origdata->data_[y * origdata->numcols_ + x];
			}
		}

		jas_matrix_destroy(origdata);
	}

	jas_image_destroy(Jp2Image);

	ilFixImage();

	return IL_TRUE;
}



static int iJp2_file_read(jas_stream_obj_t *obj, char *buf, int cnt)
{
	obj;
	return iread(buf, 1, cnt);
}

static int iJp2_file_write(jas_stream_obj_t *obj, char *buf, int cnt)
{
	obj;
	return iwrite(buf, 1, cnt);
}

static long iJp2_file_seek(jas_stream_obj_t *obj, long offset, int origin)
{
	obj;

	// We could just pass origin to iseek, but this is probably more portable.
	switch (origin)
	{
		case SEEK_SET:
			return iseek(offset, IL_SEEK_SET);
		case SEEK_CUR:
			return iseek(offset, IL_SEEK_CUR);
		case SEEK_END:
			return iseek(offset, IL_SEEK_END);
	}
	return 0;  // Failed
}

static int iJp2_file_close(jas_stream_obj_t *obj)
{
	obj;
	return 0;  // We choose when we want to close the file.
}

static jas_stream_ops_t jas_stream_devilops = {
	iJp2_file_read,
	iJp2_file_write,
	iJp2_file_seek,
	iJp2_file_close
};

static jas_stream_t *jas_stream_create(void);
static void jas_stream_destroy(jas_stream_t *stream);
static void jas_stream_initbuf(jas_stream_t *stream, int bufmode, char *buf, int bufsize);


// Modified version of jas_stream_fopen and jas_stream_memopen from jas_stream.c of JasPer
//  so that we can use our own file routines.
jas_stream_t *iJp2ReadStream()
{
	jas_stream_t *stream;
	jas_stream_memobj_t *obj;

	if (!(stream = jas_stream_create())) {
		return 0;
	}

	/* A stream associated with a memory buffer is always opened
	for both reading and writing in binary mode. */
	stream->openmode_ = JAS_STREAM_READ | JAS_STREAM_BINARY;

	/* We use buffering whether it is from memory or a file. */
	jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

	/* Select the operations for a memory stream. */
	stream->ops_ = &jas_stream_devilops;

	/* Allocate memory for the underlying memory stream object. */
	if (!(obj = jas_malloc(sizeof(jas_stream_memobj_t)))) {
		jas_stream_destroy(stream);
		return 0;
	}
	stream->obj_ = (void *) obj;

	/* Initialize a few important members of the memory stream object. */
	obj->myalloc_ = 0;
	obj->buf_ = 0;

	// Shouldn't need any of this.

	///* If the buffer size specified is nonpositive, then the buffer
	//is allocated internally and automatically grown as needed. */
	//if (bufsize <= 0) {
	//	obj->bufsize_ = 1024;
	//	obj->growable_ = 1;
	//} else {
	//	obj->bufsize_ = bufsize;
	//	obj->growable_ = 0;
	//}
	//if (buf) {
	//	obj->buf_ = (unsigned char *) buf;
	//} else {
	//	obj->buf_ = jas_malloc(obj->bufsize_ * sizeof(char));
	//	obj->myalloc_ = 1;
	//}
	//if (!obj->buf_) {
	//	jas_stream_close(stream);
	//	return 0;
	//}

	//if (bufsize > 0 && buf) {
	//	/* If a buffer was supplied by the caller and its length is positive,
	//	  make the associated buffer data appear in the stream initially. */
	//	obj->len_ = bufsize;
	//} else {
	//	/* The stream is initially empty. */
	//	obj->len_ = 0;
	//}
	//obj->pos_ = 0;
	
	return stream;
}


// The following functions are taken directly from jas_stream.c of JasPer,
//  since they are designed to be used within JasPer only.

static void jas_stream_initbuf(jas_stream_t *stream, int bufmode, char *buf,
  int bufsize)
{
	/* If this function is being called, the buffer should not have been
	  initialized yet. */
	assert(!stream->bufbase_);

	if (bufmode != JAS_STREAM_UNBUF) {
		/* The full- or line-buffered mode is being employed. */
		if (!buf) {
			/* The caller has not specified a buffer to employ, so allocate
			  one. */
			if ((stream->bufbase_ = jas_malloc(JAS_STREAM_BUFSIZE +
			  JAS_STREAM_MAXPUTBACK))) {
				stream->bufmode_ |= JAS_STREAM_FREEBUF;
				stream->bufsize_ = JAS_STREAM_BUFSIZE;
			} else {
				/* The buffer allocation has failed.  Resort to unbuffered
				  operation. */
				stream->bufbase_ = stream->tinybuf_;
				stream->bufsize_ = 1;
			}
		} else {
			/* The caller has specified a buffer to employ. */
			/* The buffer must be large enough to accommodate maximum
			  putback. */
			assert(bufsize > JAS_STREAM_MAXPUTBACK);
			stream->bufbase_ = JAS_CAST(uchar *, buf);
			stream->bufsize_ = bufsize - JAS_STREAM_MAXPUTBACK;
		}
	} else {
		/* The unbuffered mode is being employed. */
		/* A buffer should not have been supplied by the caller. */
		assert(!buf);
		/* Use a trivial one-character buffer. */
		stream->bufbase_ = stream->tinybuf_;
		stream->bufsize_ = 1;
	}
	stream->bufstart_ = &stream->bufbase_[JAS_STREAM_MAXPUTBACK];
	stream->ptr_ = stream->bufstart_;
	stream->cnt_ = 0;
	stream->bufmode_ |= bufmode & JAS_STREAM_BUFMODEMASK;
}

static jas_stream_t *jas_stream_create()
{
	jas_stream_t *stream;

	if (!(stream = jas_malloc(sizeof(jas_stream_t)))) {
		return 0;
	}
	stream->openmode_ = 0;
	stream->bufmode_ = 0;
	stream->flags_ = 0;
	stream->bufbase_ = 0;
	stream->bufstart_ = 0;
	stream->bufsize_ = 0;
	stream->ptr_ = 0;
	stream->cnt_ = 0;
	stream->ops_ = 0;
	stream->obj_ = 0;
	stream->rwcnt_ = 0;
	stream->rwlimit_ = -1;

	return stream;
}

static void jas_stream_destroy(jas_stream_t *stream)
{
	/* If the memory for the buffer was allocated with malloc, free
	this memory. */
	if ((stream->bufmode_ & JAS_STREAM_FREEBUF) && stream->bufbase_) {
		jas_free(stream->bufbase_);
		stream->bufbase_ = 0;
	}
	jas_free(stream);
}

#endif//IL_NO_JP2
