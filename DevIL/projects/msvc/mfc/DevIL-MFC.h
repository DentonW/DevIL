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

CBitmap *ilutConvertToCBitmap();
CBitmap *ilutMFCLoadImage(const ILstring FileName);

#endif // __MFCIL_H__
#endif // __mfcil_h__
