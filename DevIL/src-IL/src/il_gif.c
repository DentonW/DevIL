//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/13/2002 <--Y2K Compliant! =]
//
// Filename: il/il_gif.c
//
// Description: Reads from a Graphics Interchange Format (.gif) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_GIF

#include "il_gif.h"


//! Checks if the file specified in FileName is a valid Gif file.
ILboolean ilIsValidGif(const ILstring FileName)
{
	ILHANDLE	GifFile;
	ILboolean	bGif = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("gif"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bGif;
	}

	GifFile = iopenr(FileName);
	if (GifFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bGif;
	}

	bGif = ilIsValidGifF(GifFile);
	icloser(GifFile);

	return bGif;
}


//! Checks if the ILHANDLE contains a valid Gif file at the current position.
ILboolean ilIsValidGifF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidGif();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid Gif lump.
ILboolean ilIsValidGifL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidGif();
}


// Internal function to get the header and check it.
ILboolean iIsValidGif()
{
	char Header[6];
	
	iread(Header, 1, 6);
	iseek(-6, IL_SEEK_CUR);

	if (!strncmp(Header, "GIF87A", 6))
		return IL_TRUE;
	if (!strncmp(Header, "GIF89A", 6))
		return IL_TRUE;

	return IL_FALSE;
}


//! Reads a Gif file
ILboolean ilLoadGif(const ILstring FileName)
{
	ILHANDLE	GifFile;
	ILboolean	bGif = IL_FALSE;

	GifFile = iopenr(FileName);
	if (GifFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bGif;
	}

	bGif = ilLoadGifF(GifFile);
	icloser(GifFile);

	return bGif;
}


//! Reads an already-opened Gif file
ILboolean ilLoadGifF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadGifInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Gif
ILboolean ilLoadGifL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadGifInternal();
}


// Internal function used to load the Gif.
ILboolean iLoadGifInternal()
{
	ILimage			*Image = NULL;
	ILenum			LibErr;
	ILint			i, j, k;
	ILubyte			*BaseData = NULL;
	ILuint			x, y, BaseW, BaseH, CurW, CurH, XOff, YOff;
	GIFHEAD			Header;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iIsValidGif())
		return IL_FALSE;

	iread(&Header, sizeof(Header), 1);

	

	




	ilFixImage();

	return IL_TRUE;
}

#endif //IL_NO_GIF
