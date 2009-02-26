//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/26/2009
//
// Filename: src-IL/src/il_dpx.c
//
// Description: Reads from a Digital Picture Exchange (.dpx) file.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_DPX
#include "il_dpx.h"

ILboolean iLoadDpxInternal(void);


//! Reads a DPX file
ILboolean ilLoadDpx(ILconst_string FileName)
{
	ILHANDLE	DpxFile;
	ILboolean	bDpx = IL_FALSE;

	DpxFile = iopenr(FileName);
	if (DpxFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDpx;
	}

	bDpx = ilLoadDpxF(DpxFile);
	icloser(DpxFile);

	return bDpx;
}


//! Reads an already-opened DPX file
ILboolean ilLoadDpxF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadDpxInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a DPX
ILboolean ilLoadDpxL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadDpxInternal();
}


// Internal function used to load the DPX.
ILboolean iLoadDpxInternal(void)
{

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}


	return ilFixImage();
}

#endif//IL_NO_DPX

