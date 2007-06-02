//-----------------------------------------------------------------------------
//
// DevIL MFC Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/07/2002 <--Y2K Compliant! =]
//
// Filename: mfc/devil-mfc.cpp
//
// Description: MFC CBitmap loading routines
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "DevIL-MFC.h"


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


CBitmap *ilutConvertToCBitmap()
{
	CBitmap	*Bmp = NULL;
	ILubyte	*Data = NULL;
	ILimage	*Image = NULL, *MfcCurImage = NULL;

	Bmp = new CBitmap;
	if (Bmp == NULL)
		return NULL;

	MfcCurImage = ilGetCurImage();
	if (MfcCurImage == NULL) {
		return NULL;
	}

	if (MfcCurImage->Format == IL_BGRA && MfcCurImage->Format == IL_UNSIGNED_BYTE) {
		Image = MfcCurImage; 
	}
	else if (MfcCurImage->Format == IL_BGRA || MfcCurImage->Format == IL_RGBA) {
		Image = iConvertImage(IL_BGRA, IL_UNSIGNED_BYTE);
		if (Image == NULL) {
			delete Bmp;
			return NULL;
		}
	}
	else {
		Image = iConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
		if (Image == NULL) {
			delete Bmp;
			return NULL;
		}
	}

	Data = iGetPaddedData(Image);
	if (Data == NULL) {
		if (Image != MfcCurImage)
			ilCloseImage(Image);
		delete Bmp;
		return NULL;
	}

	if (!Bmp->CreateBitmap(Image->Width, Image->Height, 1, Image->Bpp << 3, Image->Data)) {
		if (Image != MfcCurImage)
			ilCloseImage(Image);
		free(Data);
		delete Bmp;
		return NULL;
	}

	return Bmp;
}


CBitmap *ilutMFCLoadImage(const ILstring FileName)
{
	CBitmap *Bmp;

	iBindImageTemp();
	if (!ilLoadImage(FileName))
		return NULL;

	Bmp = ilutConvertToCBitmap();

	return Bmp;
}


