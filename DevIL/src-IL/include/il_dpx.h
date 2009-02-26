//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/26/2009
//
// Filename: src-IL/include/il_dpx.h
//
// Description: Reads from a .dox file.
//
//-----------------------------------------------------------------------------


#ifndef DPX_H
#define DPX_H

#include "il_internal.h"


#ifdef _WIN32
#pragma pack(push, packed_struct, 1)
#endif
typedef struct DPXHEAD
{

} IL_PACKSTRUCT DPXHEAD;
#ifdef _WIN32
#pragma pack(pop, packed_struct)
#endif

// For checking and reading
ILboolean iIsValidDpx(void);
ILboolean iCheckDpx(DPXHEAD *Header);
ILboolean iLoadDpxInternal(void);

#endif//PCX_H
