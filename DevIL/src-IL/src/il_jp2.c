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
//	ILHANDLE		Jp2File;
	ILboolean		bRet = IL_FALSE;
	jas_stream_t	*Stream;

	/*Jp2File = iopenr(FileName);
	if (Jp2File == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bJp2;
	}

	bJp2 = iLoadJp2Internal();
	icloser(Jp2File);*/

	if (jas_init())
	{
		ilSetError(IL_LIB_JP2_ERROR);
		return IL_FALSE;
	}
	Stream = jas_stream_fopen(FileName, "rb");
	if (!Stream)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	bRet = iLoadJp2Internal(Stream, NULL);
	// Close the input stream.
	jas_stream_close(Stream);

	return bRet;
}


// No way to do this with the JasPer library.
//! Reads an already-opened Jpeg2000 file.
/*ILboolean ilLoadJp2F(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadJp2Internal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}*/


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


#endif//IL_NO_JP2
