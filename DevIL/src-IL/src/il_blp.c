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

//@TODO: Add support for the BLP1 format as well.

#include "il_internal.h"
#ifndef IL_NO_BLP
#include "il_dds.h"


typedef struct BLP2HEAD
{
	ILubyte	Sig[4];         // "BLP2" signature
	ILuint	Type;           // Texture type: 0 = JPG, 1 = DXTC
	ILubyte	Compression;    // Compression mode: 1 = raw, 2 = DXTC
	ILubyte	AlphaBits;      // 0, 1, or 8
	ILubyte	AlphaType;      // 0, 1, 7 or 8
	ILubyte	HasMips;        // 0 = no mips levels, 1 = has mips (number of levels determined by image size)
	ILuint	Width;          // Image width in pixels
	ILuint	Height;         // Image height in pixels
	ILuint	MipOffsets[16]; // The file offsets of each mipmap, 0 for unused
	ILuint	MipLengths[16]; // The length of each mipmap data block
} BLP2HEAD;

// Data formats
#define BLP_TYPE_JPG		0
#define BLP_TYPE_DXTC_RAW	1
#define BLP_RAW				1
#define BLP_DXTC			2

ILboolean iIsValidBlp2(void);
ILboolean iCheckBlp2(BLP2HEAD *Header);
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
	bRet = iIsValidBlp2();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid BLP lump.
ILboolean ilIsValidBlpL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidBlp2();
}


// Internal function used to get the BLP header from the current file.
ILboolean iGetBlp2Head(BLP2HEAD *Header)
{
	ILuint i;

	iread(Header->Sig, 1, 4);
	Header->Type = GetLittleUInt();
	Header->Compression = igetc();
	Header->AlphaBits = igetc();
	Header->AlphaType = igetc();
	Header->HasMips = igetc();
	Header->Width = GetLittleUInt();
	Header->Height = GetLittleUInt();
	for (i = 0; i < 16; i++)
		Header->MipOffsets[i] = GetLittleUInt();
	for (i = 0; i < 16; i++)
		Header->MipLengths[i] = GetLittleUInt();

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidBlp2(void)
{
	BLP2HEAD Header;

	if (!iGetBlp2Head(&Header))
		return IL_FALSE;
	iseek(-148, IL_SEEK_CUR);
	
	return iCheckBlp2(&Header);
}


// Internal function used to check if the HEADER is a valid BLP header.
ILboolean iCheckBlp2(BLP2HEAD *Header)
{
	// The file signature is 'BLP2'.
	if (strncmp(Header->Sig, "BLP2", 4))
		return IL_FALSE;
	// Valid types are JPEG and DXTC.  JPEG is not common, though.
	//  WoW only uses DXTC.
	if (Header->Type != BLP_TYPE_JPG && Header->Type != BLP_TYPE_DXTC_RAW)
		return IL_FALSE;
	// For BLP_TYPE_DXTC_RAW, we can have RAW and DXTC compression.
	if (Header->Compression != BLP_RAW && Header->Compression != BLP_DXTC)
		return IL_FALSE;
	// Alpha bits can only be 0, 1 and 8.
	if (Header->AlphaBits != 0 && Header->AlphaBits != 1 && Header->AlphaBits != 8)
		return IL_FALSE;
	// Alpha type can only be 0, 1, 7 and 8.
	if (Header->AlphaType != 0 && Header->AlphaType != 1 && Header->AlphaType != 7 && Header->AlphaType != 8)
		return IL_FALSE;
	// Width or height of 0 makes no sense.
	if (Header->Width == 0 || Header->Height == 0)
		return IL_FALSE;

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
ILboolean ilLoadBlpL(const void *Lump, const ILuint Size)
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

	if (!iGetBlp2Head(&Header))
		return IL_FALSE;
	if (!iCheckBlp2(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}


	return ilFixImage();
}

#endif//IL_NO_BLP
