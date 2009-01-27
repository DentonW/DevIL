//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/26/2009
//
// Filename: src-IL/src/il_tpl.c
//
// Description: Reads from a Gamecube Texture Palette (.tpl).
//                Specifications were found at 
//                http://pabut.homeip.net:8000/yagcd/chap14.html.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_TPL


typedef struct TPLHEAD
{
	ILuint	Magic;
	ILuint	nTextures;
	ILuint	HeaderSize;
} TPLHEAD;

// Data formats
#define TPL_I4		0
#define TPL_I8		1
#define TPL_IA4		2
#define TPL_IA8		3
#define TPL_RGB565	4
#define TPL_RGB5A3	5
#define TPL_RGBA8	6
#define TPL_CI4		8
#define TPL_CI8		9
#define TPL_CI14X2	10
#define TPL_CMP		14

// Wrapping
#define TPL_CLAMP	0
#define	TPL_REPEAT	1
#define TPL_MIRROR	2


ILboolean iIsValidTpl(void);
ILboolean iCheckTpl(TPLHEAD *Header);
ILboolean iLoadTplInternal(void);


//! Checks if the file specified in FileName is a valid TPL file.
ILboolean ilIsValidTpl(ILconst_string FileName)
{
	ILHANDLE	TplFile;
	ILboolean	bTpl = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("tpl"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bTpl;
	}
	
	TplFile = iopenr(FileName);
	if (TplFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTpl;
	}
	
	bTpl = ilIsValidTplF(TplFile);
	icloser(TplFile);
	
	return bTpl;
}


//! Checks if the ILHANDLE contains a valid TPL file at the current position.
ILboolean ilIsValidTplF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidTpl();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid TPL lump.
ILboolean ilIsValidTplL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidTpl();
}


// Internal function used to get the TPL header from the current file.
ILboolean iGetTplHead(TPLHEAD *Header)
{
	Header->Magic = GetBigUInt();
	Header->nTextures = GetBigUInt();
	Header->HeaderSize = GetBigUInt();
	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidTpl(void)
{
	TPLHEAD Header;

	if (!iGetTplHead(&Header))
		return IL_FALSE;
	iseek(-12, IL_SEEK_CUR);
	
	return iCheckTpl(&Header);
}


// Internal function used to check if the HEADER is a valid TPL header.
ILboolean iCheckTpl(TPLHEAD *Header)
{
	// The file signature is 0x0020AF30.
	if (Header->Magic != 0x0020AF30)
		return IL_FALSE;
	// Only valid header size is 0x0C.
	if (Header->HeaderSize != 0x0C)
		return IL_FALSE;
	// We have to have at least 1 texture.
	if (Header->nTextures == 0)
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads a TPL file
ILboolean ilLoadTpl(ILconst_string FileName)
{
	ILHANDLE	TplFile;
	ILboolean	bTpl = IL_FALSE;
	
	TplFile = iopenr(FileName);
	if (TplFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTpl;
	}

	bTpl = ilLoadTplF(TplFile);
	icloser(TplFile);

	return bTpl;
}


//! Reads an already-opened TPL file
ILboolean ilLoadTplF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadTplInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a TPL
ILboolean ilLoadTplL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadTplInternal();
}


// Internal function used to load the TPL.
ILboolean iLoadTplInternal(void)
{
	TPLHEAD		Header;
	ILimage		*Image, *BaseImage = iCurImage;
	ILuint		CurName, Pos, TexOff, PalOff, DataFormat, Bpp, DataOff, WrapS, WrapT;
	ILuint		x, y, xBlock, yBlock;
	ILenum		Format;
	ILushort	Width, Height, ShortPixel;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	CurName = ilGetCurName();
	
	if (!iGetTplHead(&Header))
		return IL_FALSE;
	if (!iCheckTpl(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	// Points to the beginning of the texture headers.
	Pos = itell();

	//@TODO: Read more than one image if it exists.
	TexOff = GetBigUInt();
	PalOff = GetBigUInt();
	// Go to the texture header.
	iseek(TexOff, IL_SEEK_SET);
	/*if (iseek(TexOff, IL_SEEK_SET) != TexOff)
		return IL_FALSE;*/

	Height = GetBigUShort();
	Width = GetBigUShort();
	DataFormat = GetBigUInt();
	TexOff = GetBigUInt();
	WrapS = GetBigUInt();
	WrapT = GetBigUInt();
	if (WrapS == TPL_REPEAT || WrapS == TPL_MIRROR) {
		// By the specs, repeated and mirrored textures must have dimensions of power of 2.
		if ((Width != ilNextPower2(Width)) || (Height != ilNextPower2(Height))) {
			ilSetError(IL_ILLEGAL_FILE_VALUE);
			return IL_FALSE;
		}
	}

	// Go to the actual texture data.
	iseek(TexOff, IL_SEEK_SET);
	/*if (iseek(TexOff, IL_SEEK_SET) != TexOff)
		return IL_FALSE;*/

	switch (DataFormat)
	{
		case TPL_RGB565:
			Format = IL_RGB;
			Bpp = 3;
			break;

		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}

	if (!ilTexImage(Width, Height, 1, Bpp, Format, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;  // Just a guess right now.

	switch (DataFormat)
	{
		case TPL_RGB565:
			// 4x4 tiles of RGB565 data
			for (y = 0; y < iCurImage->Height; y += 4) {
				for (x = 0; x < iCurImage->Width; x += 4) {
					for (yBlock = 0; yBlock < 4 && y + yBlock < iCurImage->Height; yBlock++) {
						DataOff = iCurImage->Bps * (y + yBlock) + iCurImage->Bpp * x;
						for (xBlock = 0; xBlock < 4 && x + xBlock < iCurImage->Width; xBlock++) {
							ShortPixel = GetBigUShort();
							iCurImage->Data[DataOff] = ((ShortPixel & 0xF800) >> 8) | ((ShortPixel & 0xE000) >> 13); // Red
							iCurImage->Data[DataOff+1] = ((ShortPixel & 0x7E0) >> 3) | ((ShortPixel & 0x600) >> 9); // Green
							iCurImage->Data[DataOff+2] = ((ShortPixel & 0x1f) << 3) | ((ShortPixel & 0x1C) >> 2); // Blue
							DataOff += 3;
						}
					}
				}
			}

			break;
	}



	ilBindImage(CurName);  // Set to parent image first.
	return ilFixImage();
}

#endif//IL_NO_TPL
