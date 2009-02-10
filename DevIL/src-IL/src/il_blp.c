//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/09/2009
//
// Filename: src-IL/src/il_blp.c
//
// Description: Reads from a Blizzard Texture (.blp).
//                Specifications were found at http://www.wowwiki.com/BLP_files.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_BLP
#include "il_dds.h"


typedef struct BLP2HEAD {
	uint8_t    Magic[4];       // "BLP2" magic number
	uint32_t   Type;           // Texture type: 0 = JPG, 1 = DXTC
	uint8_t    Compression;    // Compression mode: 1 = raw, 2 = DXTC
	uint8_t    AlphaBits;      // 0, 1, or 8
	uint8_t    AlphaType;      // 0, 1, 7 or 8
	uint8_t    HasMips;        // 0 = no mips levels, 1 = has mips (number of levels determined by image size)
	uint32_t   Width;          // Image width in pixels
	uint32_t   Height;         // Image height in pixels
	uint32_t   MipOffsets[16]; // The file offsets of each mipmap, 0 for unused
	uint32_t   MipLengths[16]; // The length of each mipmap data block
} BLP2HEAD;


// Data formats
#define BLP_JPG		0
#define BLP_DXTC	1

ILboolean iIsValidBlp(void);
ILboolean iCheckBlp(BLPHEAD *Header);
ILboolean iLoadBlpInternal(void);


//! Checks if the file specified in FileName is a valid BLP file.
ILboolean ilIsValidBlp(ILconst_string FileName)
{
	ILHANDLE	BlpFile;
	ILboolean	bBlp = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("blp"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bBlp;
	}
	
	BlpFile = iopenr(FileName);
	if (BlpFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBlp;
	}
	
	bBlp = ilIsValidBlpF(BlpFile);
	icloser(BlpFile);
	
	return bBlp;
}


//! Checks if the ILHANDLE contains a valid BLP file at the current position.
ILboolean ilIsValidBlpF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidBlp();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid BLP lump.
ILboolean ilIsValidBlpL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidBlp();
}


// Internal function used to get the BLP header from the current file.
ILboolean iGetBlpHead(BLPHEAD *Header)
{

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidBlp(void)
{
	BLPHEAD Header;

	if (!iGetBlpHead(&Header))
		return IL_FALSE;
	iseek(-148, IL_SEEK_CUR);
	
	return iCheckBlp(&Header);
}


// Internal function used to check if the HEADER is a valid BLP header.
ILboolean iCheckBlp(BLPHEAD *Header)
{

	return IL_TRUE;
}


//! Reads a BLP file
ILboolean ilLoadBlp(ILconst_string FileName)
{
	ILHANDLE	BlpFile;
	ILboolean	bBlp = IL_FALSE;
	
	BlpFile = iopenr(FileName);
	if (BlpFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBlp;
	}

	bBlp = ilLoadBlpF(BlpFile);
	icloser(BlpFile);

	return bBlp;
}


//! Reads an already-opened BLP file
ILboolean ilLoadBlpF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadBlpInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a BLP
ILboolean ilLoadBlpL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadBlpInternal();
}


// Internal function used to load the BLP.
ILboolean iLoadBlpInternal(void)
{
	BLP2HEAD	Header;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	return ilFixImage();
}

#endif//IL_NO_BLP
