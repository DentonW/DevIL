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

#ifdef __cplusplus
extern "C" {
#endif

ILboolean ilLoadExr(ILconst_string FileName);

#ifdef __cplusplus
}
#endif

ILboolean iLoadExrInternal(ILconst_string FileName);

#endif//EXR_H
