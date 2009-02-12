//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/12/2009
//
// Filename: src-IL/src/il_ftx.c
//
// Description: Reads from an .ftx file.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_FTX

ILboolean iLoadFtxInternal(void);


//! Reads a FTX file
ILboolean ilLoadFtx(ILconst_string FileName)
{
	ILHANDLE	FtxFile;
	ILboolean	bFtx = IL_FALSE;
	
	FtxFile = iopenr(FileName);
	if (FtxFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bFtx;
	}

	bFtx = ilLoadFtxF(FtxFile);
	icloser(FtxFile);

	return bFtx;
}


//! Reads an already-opened FTX file
ILboolean ilLoadFtxF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadFtxInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a FTX
ILboolean ilLoadFtxL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadFtxInternal();
}


// Internal function used to load the FTX.
ILboolean iLoadFtxInternal(void)
{
	ILuint Width, Height, HasAlpha;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Width = GetLittleUInt();
	Height = GetLittleUInt();
	HasAlpha = GetLittleUInt();  // Kind of backwards from what I would think...

	if (HasAlpha == 0) {  // BGRA format
		if (!ilTexImage(Width, Height, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, NULL))
			return IL_FALSE;
	}
	else if (HasAlpha == 1) {  // BGR format
		if (!ilTexImage(Width, Height, 1, 3, IL_BGR, IL_UNSIGNED_BYTE, NULL))
			return IL_FALSE;
	}
	else {  // Unknown format
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	if (iread(iCurImage->Data, 1, iCurImage->SizeOfData) != iCurImage->SizeOfData)
		return IL_FALSE;

	return ilFixImage();
}

#endif//IL_NO_FTX

