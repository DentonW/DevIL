//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000 by Denton Woods
// Last modified:  11/23/2000 <--Y2K Compliant! =]
//
// Filename: openilut/allegro.c
//
// Description:  Allegro functions for images
//
//-----------------------------------------------------------------------------


#include "ilut_internal.h"
#ifdef ILUT_USE_ALLEGRO
#include "alleg.h"

ILboolean ilConvertPal(ILenum DestFormat);

// Does not account for converting luminance...
BITMAP* ILAPIENTRY ilutConvertToAlleg(PALETTE Pal)
{
	BITMAP *Bitmap;
	ILimage *TempImage;
	ILuint i = 0, j = 0;

	iCurImage = ilGetCurImage();

	if (iCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	// Should be IL_BGR(A), but Djgpp screws up somewhere along the line.
	if (iCurImage->Format == IL_RGB || iCurImage->Format == IL_RGBA) {
		iluSwapColours();
	}

	if (iCurImage->Origin == IL_ORIGIN_LOWER_LEFT)
		iluFlipImage();
	if (iCurImage->Type > IL_UNSIGNED_BYTE) {}  // Can't do anything about this right now...
	if (iCurImage->Type == IL_BYTE) {}  // Can't do anything about this right now...

	Bitmap = create_bitmap_ex(iCurImage->Bpp * 8, iCurImage->Width, iCurImage->Height);
	if (Bitmap == NULL) {
		ilSetError(ILUT_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	memcpy(Bitmap->dat, iCurImage->Data, iCurImage->SizeOfData);

	// Should we make this toggleable?
	if (iCurImage->Bpp == 8 && iCurImage->Pal.PalType != IL_PAL_NONE) {
		// Use the image's palette if there is one

		// ilConvertPal is destructive to the original image
		// @TODO:  Use new ilCopyPal!!!
		TempImage = ilNewImage(iCurImage->Width, iCurImage->Height, iCurImage->Depth, iCurImage->Bpp, 1);
		ilCopyImageAttr(TempImage, iCurImage);
		ilSetCurImage(TempImage);

		if (!ilConvertPal(IL_PAL_RGB24)) {
			destroy_bitmap(Bitmap);
			ilSetError(ILUT_ILLEGAL_OPERATION);
			return NULL;
		}

		for (; i < iCurImage->Pal.PalSize && i < 768; i += 3, j++) {
			Pal[j].r = TempImage->Pal.Palette[i+0];
			Pal[j].g = TempImage->Pal.Palette[i+1];
			Pal[j].b = TempImage->Pal.Palette[i+2];
			Pal[j].filler = 255;
		}

		ilCloseImage(TempImage);
		ilSetCurImage(iCurImage);
	}

	return Bitmap;
}


#ifndef _WIN32_WCE
BITMAP* ILAPIENTRY ilutAllegLoadImage(const char *FileName)
{
	ILuint	ImgId;
	PALETTE	Pal;

	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	if (!ilLoadImage(FileName)) {
		ilDeleteImages(1, &ImgId);
		return 0;
	}

	ilDeleteImages(1, &ImgId);

	return ilutConvertToAlleg(Pal);
}
#endif//_WIN32_WCE


// Unfinished
ILboolean ILAPIENTRY ilutAllegFromBitmap(BITMAP *Bitmap)
{
	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Bitmap == NULL || Bitmap->w == 0 || Bitmap->h == 0) {
		ilSetError(ILUT_INVALID_PARAM);
		return IL_FALSE;
	}

	if (!ilTexImage(Bitmap->w, Bitmap->h, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;

	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;  // I have no idea.

	return IL_TRUE;
}


#endif//ILUT_USE_ALLEGRO
