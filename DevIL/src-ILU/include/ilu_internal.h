//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 12/04/2001 <--Y2K Compliant! =]
//
// Filename: openilu/ilu_internal.h
//
// Description: Internal stuff for ILU
//
//-----------------------------------------------------------------------------


#ifndef INTERNAL_H
#define INTERNAL_H

#include <string.h>

#ifdef _MSC_VER
	#if _MSC_VER > 1000
		#pragma once
		#pragma intrinsic(memcpy)
		#pragma intrinsic(memset)
		#pragma comment(linker, "/NODEFAULTLIB:libc")
		#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	#endif // _MSC_VER > 1000
#endif

#define _IL_BUILD_LIBRARY
#define _ILU_BUILD_LIBRARY

// Standard headers
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Local headers
#define _IL_BUILD_LIBRARY
#define _ILU_BUILD_LIBRARY
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <IL/ilu.h>
#include "ilu_error.h"


// From DevIL's internal.h:
#ifdef _WIN32_WCE
	#include <windows.h>
	#define IL_TEXT(s) ((char*)TEXT(s))
#elif _WIN32
	#include <windows.h>
	#define IL_TEXT(s) (s)
#else
	#define IL_TEXT(s) (s)
	#define TEXT(s) (s)
#endif


#ifdef _MSC_VER
	#define INLINE __inline
	#define FINLINE __forceinline
#else
	#define INLINE
	#define FINLINE
#endif


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
	ILuint	OffX, OffY;			// offset of the image
} ILimage;


extern ILimage *iluCurImage;


// Internal library functions in DevIL
ILAPI ILboolean		ILAPIENTRY ilClearImage_(ILimage *Image);
ILAPI ILvoid		ILAPIENTRY ilCloseImage(ILimage *Image);
ILAPI ILimage*		ILAPIENTRY iConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILboolean		ILAPIENTRY ilCopyImageAttr(ILimage *Dest, ILimage *Src);
ILAPI ILimage*		ILAPIENTRY ilCopyImage_(ILimage *Src);
ILAPI ILvoid		ILAPIENTRY ilGetClear(ILvoid *Colours, ILenum Format, ILenum Type);
ILAPI ILimage*		ILAPIENTRY ilGetCurImage(ILvoid);
ILAPI ILuint		ILAPIENTRY ilGetCurName(ILvoid);
ILAPI ILimage*		ILAPIENTRY ilNewImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILubyte Bpc);
ILAPI ILboolean		ILAPIENTRY ilResizeImage(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILubyte Bpc);
ILAPI ILvoid		ILAPIENTRY ilSetCurImage(ILimage *Image);
ILAPI ILvoid		ILAPIENTRY ilSetError(ILenum Error);
ILAPI ILvoid		ILAPIENTRY ilSetPal(ILpal *Pal);

ILAPI ILubyte		ILAPIENTRY ilGetBppFormat(ILenum Format);
ILAPI ILubyte		ILAPIENTRY ilGetBppPal(ILenum PalType);
ILAPI ILubyte		ILAPIENTRY ilGetBppType(ILenum Type);
ILAPI ILenum		ILAPIENTRY ilGetPalBaseType(ILenum PalType);
ILAPI ILuint		ILAPIENTRY ilNextPower2(ILuint Num);


// Useful global variables
extern ILdouble		IL_PI;
extern ILdouble		IL_DEGCONV;


// Internal functions
ILfloat				ilCos(ILfloat Angle);
ILfloat				ilSin(ILfloat Angle);
ILint				ilRound(ILfloat Num);
ILuint				iluScaleAdvanced(ILuint Width, ILuint Height, ILenum Filter);


ILAPI ILimage* ILAPIENTRY iluRotate_(ILimage *Image, ILfloat Angle);
ILAPI ILimage* ILAPIENTRY iluRotate3D_(ILimage *Image, ILfloat x, ILfloat y, ILfloat z, ILfloat Angle);
ILAPI ILimage* ILAPIENTRY iluScale_(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth);

#endif//INTERNAL_H
