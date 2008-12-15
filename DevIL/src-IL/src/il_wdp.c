//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 09/26/2008
//
// Filename: src-IL/src/il_wdp.c
//
// Description: Reads a microsoft hd photo (.wdp)
//   based very much on the Microsoft HD Photo Device Porting Kit 1.0
//   available at 
//   http://www.microsoft.com/downloads/details.aspx?FamilyID=285eeffd-d86c-48c3-ab93-3abd5ee7f1ce&displaylang=en.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_WDP
#include "il_wdp.h"


//! Reads a .wdp file
ILboolean ilLoadWdp(ILconst_string FileName)
{
	ILboolean	bWdp = IL_FALSE;

	bWdp = iLoadWdpInternal(FileName);

	return bWdp;
}


ILboolean iLoadWdpInternal(ILconst_string FileName)
{

	return IL_TRUE;
}

#endif//IL_NO_WDP
