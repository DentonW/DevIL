//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openilu/utilities.c
//
// Description: Utility functions
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_alloc.h"


ILvoid ILAPIENTRY iluDeleteImage(ILuint Id)
{
	ilDeleteImages(1, &Id);
	return;
}


ILuint ILAPIENTRY iluGenImage()
{
	static ILuint Id;
	ilGenImages(1, &Id);
	ilBindImage(Id);
	return Id;
}


//! Retrieves information about the current bound image.
ILvoid ILAPIENTRY iluGetImageInfo(ILinfo *Info)
{
	iCurImage = ilGetCurImage();
	if (iCurImage == NULL || Info == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return;
	}

	Info->Id			= ilGetCurName();
	Info->Data			= ilGetData();
	Info->Width			= iCurImage->Width;
	Info->Height		= iCurImage->Height;
	Info->Depth			= iCurImage->Depth;
	Info->Bpp			= iCurImage->Bpp;
	Info->SizeOfData	= iCurImage->SizeOfData;
	Info->Format		= iCurImage->Format;
	Info->Type			= iCurImage->Type;
	Info->Origin		= iCurImage->Origin;
	Info->Palette		= iCurImage->Pal.Palette;
	Info->PalType		= iCurImage->Pal.PalType;
	Info->PalSize		= iCurImage->Pal.PalSize;
	Info->NumNext		= iCurImage->NumNext;
	Info->NumMips		= iCurImage->NumMips;
	Info->NumLayers		= iCurImage->NumLayers;

	return;
}
