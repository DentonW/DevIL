//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 12/04/2001 <--Y2K Compliant! =]
//
// Filename: openilut/internal.h
//
// Description: Internal stuff for ILUT
//
//-----------------------------------------------------------------------------


#ifndef INTERNAL_H
#define INTERNAL_H

#define _IL_BUILD_LIBRARY
#define _ILU_BUILD_LIBRARY
#define _ILUT_BUILD_LIBRARY

#include <string.h>

#ifdef _MSC_VER
	#if _MSC_VER > 1000
		#pragma once
		#pragma intrinsic(memcpy)
		#pragma intrinsic(memset)
	#endif // _MSC_VER > 1000
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <IL/ilut.h>

#include <stdlib.h>
#include "ilut_error.h"

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
} ILimage;


extern ILimage *iCurImage;


ILimage	*MakeGLCompliant(ILimage *Image);
ILvoid	ilutDefaultStates(ILvoid);


// Internal library functions in DevIL
ILAPI ILvoid		ILAPIENTRY	iBindImageTemp(ILvoid);
ILAPI ILvoid		ILAPIENTRY ilCloseImage(ILimage *Image);
ILAPI ILimage*		ILAPIENTRY iConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILimage*		ILAPIENTRY ilCopyImage_(ILimage *Src);
ILAPI ILboolean		ILAPIENTRY ilCopyImageAttr(ILimage *Dest, ILimage *Src);
ILAPI ILpal*		ILAPIENTRY iCopyPal(ILvoid);
ILAPI ILimage*		ILAPIENTRY ilGetCurImage(ILvoid);
ILAPI ILuint		ILAPIENTRY ilGetCurName(ILvoid);
ILAPI ILubyte*		ILAPIENTRY iGetFlipped(ILimage *Image);
ILAPI ILboolean		ILAPIENTRY ilIsValidPal(ILpal *Palette);
ILAPI ILenum		ILAPIENTRY ilTypeFromExt(const char *FileName);
ILAPI ILimage*		ILAPIENTRY ilNewImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILubyte Bpc);
ILAPI ILvoid		ILAPIENTRY ilSetCurImage(ILimage *Image);
ILAPI ILvoid		ILAPIENTRY ilSetError(ILenum Error);

ILAPI ILubyte		ILAPIENTRY ilGetBppFormat(ILenum Format);
ILAPI ILubyte		ILAPIENTRY ilGetBppPal(ILenum PalType);
ILAPI ILubyte		ILAPIENTRY ilGetBppType(ILenum Type);
ILAPI ILenum		ILAPIENTRY ilGetPalBaseType(ILenum PalType);
ILAPI ILuint		ILAPIENTRY ilNextPower2(ILuint Num);


// Internal library functions in ILU
ILAPI ILimage* ILAPIENTRY iluScale_(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth);


// ImageLib Utility Toolkit's OpenGL Functions
#ifdef ILUT_USE_OPENGL
ILboolean ilutGLInit();
#endif


// ImageLib Utility Toolkit's Win32 Functions
#ifdef ILUT_USE_WIN32
ILboolean ilutWin32Init();
#endif


#endif//INTERNAL_H
