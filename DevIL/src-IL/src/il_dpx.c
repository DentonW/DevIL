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


//@TODO: Use size of header instead of sizeof.
ILboolean DpxGetFileInfo(DPX_FILE_INFO *FileInfo)
{
	//if (iread(FileInfo, sizeof(DPX_FILE_INFO), 1) != 1)
	if (iread(FileInfo, 768, 1) != 1)
		return IL_FALSE;
	return IL_TRUE;
}


ILboolean DpxGetImageInfo(DPX_IMAGE_INFO *ImageInfo)
{
	//if (iread(FileInfo, sizeof(DPX_FILE_INFO), 1) != 1)
	if (iread(ImageInfo, sizeof(DPX_IMAGE_INFO), 1) != 1)
		return IL_FALSE;
	return IL_TRUE;
}


// Internal function used to load the DPX.
ILboolean iLoadDpxInternal(void)
{
	DPX_FILE_INFO	FileInfo;
	DPX_IMAGE_INFO	ImageInfo;
	ILuint			i;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if (!DpxGetFileInfo(&FileInfo))
		return IL_FALSE;
	if (!DpxGetImageInfo(&ImageInfo))
		return IL_FALSE;
	i = itell();
	printf("%d\n", ImageInfo.pixels_per_line);


	return ilFixImage();
}

#endif//IL_NO_DPX

