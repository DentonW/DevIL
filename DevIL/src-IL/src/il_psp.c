//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 03/02/2002 <--Y2K Compliant! =]
//
// Filename: openil/il_psp.c
//
// Description: Reads a Paint Shop Pro file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include "il_psp.h"
#ifndef IL_NO_PSP


ILubyte PSPSignature[32] = {
	0x50, 0x61, 0x69, 0x6E, 0x74, 0x20, 0x53, 0x68, 0x6F, 0x70, 0x20, 0x50, 0x72, 0x6F, 0x20, 0x49,
	0x6D, 0x61, 0x67, 0x65, 0x20, 0x46, 0x69, 0x6C, 0x65, 0x0A, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00
};

ILubyte GenAttHead[4] = {
	0x7E, 0x42, 0x4B, 0x00
};



#endif//IL_NO_PSP