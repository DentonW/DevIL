//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2001 by Denton Woods
// Copyright (C) 2001 Nelson Rush.
// Last modified:  8/14/01
//
// Filename: openilut/sdlsurface.c
//
// Description:  SDL Surface functions for images
//
//-----------------------------------------------------------------------------


#include "ilut_internal.h"
#ifdef ILUT_USE_SDL
#include <SDL.h>

#ifdef  _MSC_VER
	#pragma comment(lib, "sdl.lib")
#endif//_MSC_VER

int isBigEndian;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	isBigEndian = 1;
	rmask = 0x000000FF;
	gmask = 0x0000FF00;
	bmask = 0x00FF0000;
	amask = 0;
#else
	isBigEndian = 0;
	rmask = 0x00FF0000;
	gmask = 0x0000FF00;
	bmask = 0x000000FF;
	amask = 0;

#endif

//ILboolean ilConvertPal(ILenum DestFormat);

ILpal *Pal;

// Does not account for converting luminance...
SDL_Surface * ILAPIENTRY ilutConvertToSDLSurface(unsigned int flags)
{
	SDL_Surface *Bitmap;
	ILuint	i = 0, j = 0, Pad, BppPal;
	ILubyte	*Dest;

	iCurImage = ilGetCurImage();

	if (iCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	// Should be IL_BGR(A).
	if (iCurImage->Format == IL_RGB || iCurImage->Format == IL_RGBA) {
		if (!isBigEndian)
			iluSwapColours();
	}

	if (iCurImage->Origin == IL_ORIGIN_LOWER_LEFT)
		iluFlipImage();
	if (iCurImage->Type > IL_UNSIGNED_BYTE) {}  // Can't do anything about this right now...
	if (iCurImage->Type == IL_BYTE) {}  // Can't do anything about this right now...

	Bitmap = SDL_CreateRGBSurface(flags,iCurImage->Width,iCurImage->Height,iCurImage->Bpp * 8,
					rmask,gmask,bmask,amask);

	if (Bitmap == NULL) {
		ilSetError(ILUT_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	if (SDL_MUSTLOCK(Bitmap))
		SDL_LockSurface(Bitmap);

	Pad = Bitmap->pitch - iCurImage->Bps;
	if (Pad == 0) {
		memcpy(Bitmap->pixels, iCurImage->Data, iCurImage->SizeOfData);
	}
	else {  // Must pad the lines on some images.
		Dest = Bitmap->pixels;
		for (i = 0; i < iCurImage->Height; i++) {
			memcpy(Dest, iCurImage->Data + i * iCurImage->Bps, iCurImage->Bps);
			memset(Dest + iCurImage->Bps, 0, Pad);
			Dest += Bitmap->pitch;
		}
	}

	if (SDL_MUSTLOCK(Bitmap))
		SDL_UnlockSurface(Bitmap);

	if (iCurImage->Bpp == 1) {
		BppPal = ilGetBppPal(iCurImage->Pal.PalType);
		switch (iCurImage->Pal.PalType)
		{
			case IL_PAL_RGB24:
			case IL_PAL_RGB32:
			case IL_PAL_RGBA32:
				for (i = 0; i < iCurImage->Pal.PalSize / BppPal; i++) {
					(Bitmap->format)->palette->colors[i].r = iCurImage->Pal.Palette[i*BppPal+0];
					(Bitmap->format)->palette->colors[i].g = iCurImage->Pal.Palette[i*BppPal+1];
					(Bitmap->format)->palette->colors[i].b = iCurImage->Pal.Palette[i*BppPal+2];
					(Bitmap->format)->palette->colors[i].unused = 255;
				}
				break;
			case IL_PAL_BGR24:
			case IL_PAL_BGR32:
			case IL_PAL_BGRA32:
				for (i = 0; i < iCurImage->Pal.PalSize / BppPal; i++) {
					(Bitmap->format)->palette->colors[i].b = iCurImage->Pal.Palette[i*BppPal+0];
					(Bitmap->format)->palette->colors[i].g = iCurImage->Pal.Palette[i*BppPal+1];
					(Bitmap->format)->palette->colors[i].r = iCurImage->Pal.Palette[i*BppPal+2];
					(Bitmap->format)->palette->colors[i].unused = 255;
				}
				break;
			default:
				ilSetError(IL_INTERNAL_ERROR);  // Do anything else?
		}
	}

	return Bitmap;
}


#ifndef _WIN32_WCE
SDL_Surface* ILAPIENTRY ilutSDLSurfaceLoadImage(char *FileName)
{
	SDL_Surface *Surface;

	iBindImageTemp();
	if (!ilLoadImage(FileName)) {
		ilDeleteImages(1, &ImgId);
		return NULL;
	}

	Surface = ilutConvertToSDLSurface(SDL_SWSURFACE);

	return Surface;
}
#endif//_WIN32_WCE


// Unfinished
ILboolean ILAPIENTRY ilutSDLSurfaceFromBitmap(SDL_Surface *Bitmap)
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

	return IL_TRUE;
}


#endif//ILUT_USE_SDL