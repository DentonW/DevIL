//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 09/15/2001 <--Y2K Compliant! =]
//
// Filename: openil/gif.c
//
// Description: Reads from a Graphics Interchange Format (.gif) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_GIF
#include <gif_lib.h>


ILboolean iIsValidGif(ILvoid);

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


ILboolean ilLoadGif(const ILstring FileName)
{
	GifFileType		*GifFile;
	ColorMapObject	*ColorMap;
	ILimage			*Image = NULL;
	ILenum			LibErr;
	ILint			i, j, k;
	ILubyte			*BaseData = NULL;
	ILuint			x, y, BaseW, BaseH, CurW, CurH, XOff, YOff;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	GifFile = DGifOpenFileName(FileName);
	if (GifFile == NULL) {
		LibErr = GifLastError();
		switch(LibErr)
		{
			case D_GIF_ERR_OPEN_FAILED:
				ilSetError(IL_COULD_NOT_OPEN_FILE);
				break;
			case D_GIF_ERR_NOT_ENOUGH_MEM:
				ilSetError(IL_OUT_OF_MEMORY);
				break;
			default:
				ilSetError(IL_UNKNOWN_ERROR);
		}
		return IL_FALSE;
	}

	// Read in the remainder of the gif file
	if (DGifSlurp(GifFile) == GIF_ERROR /*|| GifFile->SColorResolution != 6*/) {
		LibErr = GifLastError();
		ilSetError(IL_LIB_GIF_ERROR);
		DGifCloseFile(GifFile);
		return IL_FALSE;
	}

	BaseW = 0;  BaseH = 0;  // Just to get rid of warnings about 'possibly being uninitialized'.
	iCurImage->NumNext = GifFile->ImageCount - 1;
	for (k = 0; k < GifFile->ImageCount; k++) {
		if (k == 0) {
			if (!ilTexImage(GifFile->SWidth, GifFile->SHeight, 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL)) {
				DGifCloseFile(GifFile);
				return IL_FALSE;
			}
			iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
			Image = iCurImage;
			BaseData = Image->Data;
			BaseW = Image->Width;
			BaseH = Image->Height;
		}
		else {
			//Image->Next = ilNewImage(GifFile->SavedImages[k].ImageDesc.Width, GifFile->SavedImages[k].ImageDesc.Height, 1, 1, 1);
			Image->Next = ilNewImage(BaseW, BaseH, 1, 1, 1);
			Image = Image->Next;
			memcpy(Image->Data, BaseData, Image->SizeOfData);
			if (Image == NULL) {
				DGifCloseFile(GifFile);
				return IL_FALSE;
			}
			Image->Origin = IL_ORIGIN_UPPER_LEFT;
			Image->Format = IL_COLOUR_INDEX;
		}

		// See if this is our new "base image", since .gif can update just portions of an image.
		if (GifFile->SavedImages[k].ImageDesc.Width == (ILint)BaseW && GifFile->SavedImages[k].ImageDesc.Height == (ILint)BaseH) {
			BaseData = Image->Data;
			BaseW = Image->Width;
			BaseH = Image->Height;
			memcpy(Image->Data, GifFile->SavedImages[k].RasterBits, Image->SizeOfData);
		}
		else {
			CurW = GifFile->SavedImages[k].ImageDesc.Width;
			CurH = GifFile->SavedImages[k].ImageDesc.Height;
			XOff = GifFile->SavedImages[k].ImageDesc.Left;
			YOff = GifFile->SavedImages[k].ImageDesc.Top;

			for (y = 0; y < CurH; y++) {
				memcpy(Image->Data + (y + YOff) * Image->Width + XOff,
					GifFile->SavedImages[k].RasterBits + y * CurW, CurW);
			}
		}

		ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
		Image->Pal.Palette = (ILubyte*)ialloc(ColorMap->ColorCount * 3);
		if (Image->Pal.Palette == NULL) {
			ilSetError(IL_OUT_OF_MEMORY);
			DGifCloseFile(GifFile);
			return IL_FALSE;
		}
		Image->Pal.PalSize = ColorMap->ColorCount * 3;
		Image->Pal.PalType = IL_PAL_RGB24;

		for (i = 0, j = 0; i < ColorMap->ColorCount; i++, j += 3) {
			Image->Pal.Palette[j] = ColorMap->Colors[i].Red;
			Image->Pal.Palette[j+1] = ColorMap->Colors[i].Green;
			Image->Pal.Palette[j+2] = ColorMap->Colors[i].Blue;
		}

		// Find the current image's duration, if any, by checking in the extensions blocks (89a gifs only).
		for (x = 0; x < (ILuint)GifFile->SavedImages[k].ExtensionBlockCount; x++) {
			if (GifFile->SavedImages[k].Function == GRAPHICS_EXT_FUNC_CODE) {
				Image->Duration = *(ILushort*)(GifFile->SavedImages[k].ExtensionBlocks[x].Bytes+1);
				// The specification says 1/100 second, but it really is different...
				Image->Duration = Image->Duration * 14;
			}
		}
	}


	DGifCloseFile(GifFile);
	ilFixImage();

	return IL_TRUE;
}
    
#endif //IL_NO_GIF
