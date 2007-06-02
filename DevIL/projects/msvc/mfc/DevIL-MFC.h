//-----------------------------------------------------------------------------
//
// DevIL MFC Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/13/2002 <--Y2K Compliant! =]
//
// Filename: mfc/devil-mfc.h
//
// Description: MFC CBitmap loading routines
//
//-----------------------------------------------------------------------------


#ifndef __mfcil_h_
#ifndef __MFCIL_H__

#define __mfcil_h_
#define __MFCIL_H__

#include <IL/ilut.h>

// Internal to IL but makes things easier for us:
struct ILimage;
ILAPI ILimage*	ILAPIENTRY iConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILubyte*	ILAPIENTRY iGetPaddedData(ILimage *Image);
ILAPI ILimage*	ILAPIENTRY ilGetCurImage(void);
ILAPI ILvoid	ILAPIENTRY ilCloseImage(ILimage *Image);
ILAPI ILvoid	ILAPIENTRY iBindImageTemp(void);

// Functions for the static library.
CBitmap *ilutConvertToCBitmap();
CBitmap *ilutMFCLoadImage(const ILstring FileName);

#endif // __MFCIL_H__
#endif // __mfcil_h__
