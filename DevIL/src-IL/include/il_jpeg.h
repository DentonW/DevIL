//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/jpeg.h
//
// Description: Jpeg (.jpg) functions
//
//-----------------------------------------------------------------------------

#ifndef JPEG_H
#define JPEG_H

#include "il_internal.h"

ILboolean iCheckJpg(ILubyte Header[2]);
ILboolean iIsValidJpg(ILvoid);

#ifndef IL_USE_IJL
	ILboolean iLoadJpegInternal(ILvoid);
	ILboolean iSaveJpegInternal(ILvoid);
#else
	ILboolean iLoadJpegInternal(const ILstring FileName, ILvoid *Lump, ILuint Size);
	ILboolean iSaveJpegInternal(const ILstring FileName, ILvoid *Lump, ILuint Size);
#endif

#endif//JPEG_H
