//-----------------------------------------------------------------------------
//
// DevIL GDI+ Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/13/2002 <--Y2K Compliant! =]
//
// Filename: gdi+/devil-gdi+.cpp
//
// Description: GDI+ loading routines
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "DevIL-GDI+.h"


// Basic Palette struct
typedef struct ILpal
{
	ILubyte	*Palette;		// the image palette (if any)
	ILuint	PalSize;		// size of the palette (in bytes)
	ILenum	PalType;		// the palette types below (0x0500 range)
} ILpal;


// The Fundamental Image struct
typedef struct ILimage
{
	ILuint	Width;				// the image's width
	ILuint	Height;				// the image's height
	ILuint	Depth;				// the image's depth
	ILubyte	Bpp;				// bytes per pixel (now number of channels)
	ILubyte	Bpc;				// bytes per channel
	ILuint	Bps;				// bytes per scanline (components for IL)
	ILubyte	*Data;				// the image data
	ILuint	SizeOfData;			// the total size of the data (in bytes)
	ILuint	SizeOfPlane;		// SizeOfData in a 2d image, size of each plane slice in a 3d image (in bytes)
	ILenum	Format;				// image format (in IL enum style)
	ILenum	Type;				// image type (in IL enum style)
	ILenum	Origin;				// origin of the image
	ILpal	Pal;				// palette details
	ILuint	Duration;			// length of the time to display this "frame"
	ILenum	CubeFlags;			// cube map flags for sides present in chain
	struct	ILimage *Mipmaps;	// mipmapped versions of this image terminated by a NULL - usu. NULL
	struct	ILimage *Next;		// next image in the chain - usu. NULL
	struct	ILimage *Layers;	// subsequent layers in the chain - usu. NULL
	ILuint	NumNext;			// number of images following this one (0 when not parent)
	ILuint	NumMips;			// number of mipmaps (0 when not parent)
	ILuint	NumLayers;			// number of layers (0 when not parent)
	ILuint	*AnimList;			// animation list
	ILuint	AnimSize;			// animation list size
	ILvoid	*Profile;			// colour profile
	ILuint	ProfileSize;		// colour profile size
	ILuint	OffX, OffY;			// offset of the image
} ILimage;

ILAPI ILimage*	ILAPIENTRY iConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILubyte*	ILAPIENTRY iGetPaddedData(ILimage *Image);
ILAPI ILimage*	ILAPIENTRY ilGetCurImage(void);
ILAPI ILvoid	ILAPIENTRY ilCloseImage(ILimage *Image);
ILAPI ILvoid	ILAPIENTRY iBindImageTemp(void);


// @TODO: Use 48-bit and 64-bit types.
Bitmap *ilutConvertToBitmap()
{
	Bitmap	*Bmp = NULL;
	ILubyte	*Data = NULL;
	ILimage	*Image = NULL, *GdiCurImage = NULL;

	GdiCurImage = ilGetCurImage();
	if (GdiCurImage == NULL) {
		return NULL;
	}

	if (GdiCurImage->Format == IL_BGRA && GdiCurImage->Format == IL_UNSIGNED_BYTE) {
		Image = GdiCurImage;
		return new Bitmap(Image->Width, Image->Height, Image->Bps, PixelFormat32bppARGB, Image->Data);
	}
	else if (GdiCurImage->Format == IL_BGRA || GdiCurImage->Format == IL_RGBA) {
		Image = iConvertImage(IL_BGRA, IL_UNSIGNED_BYTE);
		if (Image == NULL)
			return NULL;
		Bmp = new Bitmap(Image->Width, Image->Height, Image->Bps, PixelFormat32bppARGB, Image->Data);
		ilCloseImage(Image);
		return Bmp;
	}
	else if (GdiCurImage->Format == IL_LUMINANCE && 
		(GdiCurImage->Type == IL_UNSIGNED_SHORT || GdiCurImage->Type == IL_SHORT)) {
			Image = GdiCurImage;
			return new Bitmap(Image->Width, Image->Height, Image->Bps, PixelFormat16bppGrayScale, Image->Data);
	}
	else if (GdiCurImage->Format == IL_LUMINANCE) {
		Image = iConvertImage(IL_LUMINANCE, IL_UNSIGNED_SHORT);
		if (Image == NULL)
			return NULL;
		Bmp = new Bitmap(Image->Width, Image->Height, Image->Bps, PixelFormat16bppGrayScale, Image->Data);
		ilCloseImage(Image);
		return Bmp;
	}
	else {
		Image = iConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
		if (Image == NULL)
			return NULL;
		Bmp = new Bitmap(Image->Width, Image->Height, Image->Bps, PixelFormat24bppRGB, Image->Data);
		ilCloseImage(Image);
		return Bmp;
	}

	return NULL;  // Something really screwy has happened here...
}


Bitmap *ilutGDILoadImage(const ILstring FileName)
{
	Bitmap *Bmp;

	iBindImageTemp();
	if (!ilLoadImage(FileName))
		return NULL;

	Bmp = ilutConvertToBitmap();

	return Bmp;
}


