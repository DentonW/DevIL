//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 08/29/2008
//
// Filename: src-IL/include/il_exr.h
//
// Description: Reads from an OpenEXR (.exr) file.
//
//-----------------------------------------------------------------------------


#ifndef EXR_H
#define EXR_H

#include "il_internal.h"

//#ifdef __cplusplus
//extern "C" {
//#endif
//
//ILboolean ilLoadExr(ILconst_string FileName);
//
//#ifdef __cplusplus
//}
//#endif

typedef struct EXRHEAD
{
	ILuint		MagicNumber;		// File signature (0x76, 0x2f, 0x31, 0x01)
	ILuint		Version;			// Treated as two bitfields
} IL_PACKSTRUCT EXRHEAD;

//@TODO: Should I just do these as enums?
#define EXR_UINT 0
#define EXR_HALF 1
#define EXR_FLOAT 2

#define EXR_NO_COMPRESSION    0
#define EXR_RLE_COMPRESSION   1
#define EXR_ZIPS_COMPRESSION  2
#define EXR_ZIP_COMPRESSION   3
#define EXR_PIZ_COMPRESSION   4
#define EXR_PXR24_COMPRESSION 5
#define EXR_B44_COMPRESSION   6
#define EXR_B44A_COMPRESSION  7

ILboolean iIsValidExr();
ILboolean iCheckExr(EXRHEAD *Header);
ILboolean iLoadExrInternal(ILconst_string FileName);
ILboolean ExrGetFormatType(ILuint *Format, ILuint *Type, ILubyte *ChanOrder);

#endif//EXR_H
