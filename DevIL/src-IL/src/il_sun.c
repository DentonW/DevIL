//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/06/2009
//
// Filename: src-IL/src/il_sun.c
//
// Description: Reads from a Sun RAS file.  Specifications available from
//				http://www.fileformat.info/format/sunraster/egff.htm.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_SUN

ILboolean	iLoadSunInternal(void);
ILboolean	iIsValidSun(void);

typedef struct SUNHEAD
{
	ILuint MagicNumber;      // Magic (identification) number
	ILuint Width;            // Width of image in pixels
	ILuint Height;           // Height of image in pixels
	ILuint Depth;            // Number of bits per pixel
	ILuint Length;           // Size of image data in bytes
	ILuint Type;             // Type of raster file
	ILuint ColorMapType;     // Type of color map
	ILuint ColorMapLength;   // Size of the color map in bytes
} SUNHEAD;

// Data storage types
#define IL_SUN_OLD		0x00
#define IL_SUN_STANDARD	0x01
#define IL_SUN_BYTE_ENC	0x02
#define IL_SUN_RGB		0x03
#define IL_SUN_TIFF		0x04
#define IL_SUN_IFF		0x05
#define IL_SUN_EXPER	0xFFFF  // Experimental, not supported.

// Colormap types
#define IL_SUN_NO_MAP	0x00
#define IL_SUN_RGB_MAP	0x01
#define IL_SUN_RAW_MAP	0x02


//! Checks if the file specified in FileName is a valid Sun file.
ILboolean ilIsValidSun(ILconst_string FileName)
{
	ILHANDLE	SunFile;
	ILboolean	bSun = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("sun"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bSun;
	}
	
	SunFile = iopenr(FileName);
	if (SunFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bSun;
	}
	
	bSun = ilIsValidSunF(SunFile);
	icloser(SunFile);
	
	return bSun;
}


//! Checks if the ILHANDLE contains a valid Sun file at the current position.
ILboolean ilIsValidSunF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidSun();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid Sun lump.
ILboolean ilIsValidSunL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidSun();
}


// Internal function used to get the Sun header from the current file.
ILboolean iGetSunHead(SUNHEAD *Header)
{
	Header->MagicNumber = GetBigUInt();
	Header->Width = GetBigUInt();
	Header->Height = GetBigUInt();
	Header->Depth = GetBigUInt();
	Header->Length = GetBigUInt();
	Header->Type = GetBigUInt();
	Header->ColorMapType = GetBigUInt();
	Header->ColorMapLength = GetBigUInt();

	return IL_TRUE;
}


// Internal function used to check if the HEADER is a valid SUN header.
ILboolean iCheckSun(SUNHEAD *Header)
{
	if (Header->MagicNumber != 0x59A66A95)  // Magic number is always 0x59A66A95.
		return IL_FALSE;
	if (Header->Width == 0 || Header->Height == 0)  // 0 dimensions are meaningless.
		return IL_FALSE;
	// These are the only valid depths that I know of.
	if (Header->Depth != 1 && Header->Depth != 8 && Header->Depth != 24 && Header->Depth != 32)
		return IL_FALSE;
	if (Header->Type > IL_SUN_STANDARD && Header->Type != IL_SUN_RGB)  //@TODO: Support further types.
		return IL_FALSE;
	if (Header->ColorMapType > IL_SUN_RGB_MAP)  //@TODO: Find out more about raw map.
		return IL_FALSE;
	// Color map cannot be 0 if there is a map indicated.
	if (Header->ColorMapType > IL_SUN_NO_MAP && Header->ColorMapLength == 0)
		return IL_FALSE;

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidSun()
{
	SUNHEAD Head;

	if (!iGetSunHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(SUNHEAD), IL_SEEK_CUR);
	
	return iCheckSun(&Head);
}


// Reads a Sun file
ILboolean ilLoadSun(ILconst_string FileName)
{
	ILHANDLE	SunFile;
	ILboolean	bSun = IL_FALSE;

	SunFile = iopenr(FileName);
	if (SunFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bSun;
	}

	iSetInputFile(SunFile);

	bSun = ilLoadSunF(SunFile);

	icloser(SunFile);

	return bSun;
}


//! Reads an already-opened Sun file
ILboolean ilLoadSunF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadSunInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Sun
ILboolean ilLoadSunL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadSunInternal();
}


ILboolean iLoadSunInternal(void)
{
	SUNHEAD	Header;
	ILuint	i, Padding;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	//@TODO: Right now, iGetSunHead cannot fail.
	if (!iGetSunHead(&Header) || !iCheckSun(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	switch (Header.Depth)
	{
		case 8:
			if (Header.ColorMapType == IL_SUN_NO_MAP) {  // Greyscale image
				if (!ilTexImage(Header.Width, Header.Height, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL))
					return IL_FALSE;
			}
			else {  // Colour-mapped image
				if (!ilTexImage(Header.Width, Header.Height, 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL))
					return IL_FALSE;
				iCurImage->Pal.Palette = (ILubyte*)ialloc(Header.ColorMapLength);  // Allocate color map.
				if (iCurImage->Pal.Palette == NULL)
					return IL_FALSE;
				if (iread(iCurImage->Pal.Palette, 1, Header.ColorMapLength) != Header.ColorMapLength) {  // Read color map.
					ilSetError(IL_FILE_READ_ERROR);
					return IL_FALSE;
				}

				iCurImage->Pal.PalSize = Header.ColorMapLength;
				iCurImage->Pal.PalType = IL_PAL_RGB24;
			}

			Padding = (2 - (iCurImage->Bps % 2)) % 2;  // Must be padded on a 16-bit boundary (2 bytes)
			for (i = 0; i < Header.Height; i++) {
				iread(iCurImage->Data + i * Header.Width, 1, iCurImage->Bps);
				if (Padding)  // Only possible for padding to be 0 or 1.
					igetc();
			}

			break;

		case 24:
			if (Header.Type == IL_SUN_RGB) {
				if (!ilTexImage(Header.Width, Header.Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL))
					return IL_FALSE;
			}
			else {
				if (!ilTexImage(Header.Width, Header.Height, 1, 3, IL_BGR, IL_UNSIGNED_BYTE, NULL))
					return IL_FALSE;
			}

			if (!ilTexImage(Header.Width, Header.Height, 1, 3, IL_BGR, IL_UNSIGNED_BYTE, NULL))
				return IL_FALSE;
			Padding = (2 - (iCurImage->Bps % 2)) % 2;  // Must be padded on a 16-bit boundary (2 bytes)
			for (i = 0; i < Header.Height; i++) {
				iread(iCurImage->Data + i * Header.Width * 3, 1, iCurImage->Bps);
				if (Padding)  // Only possible for padding to be 0 or 1.
					igetc();
			}
			break;


		default:  // Should have already been checked with iGetSunHead.
			return IL_FALSE;
	}

	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	return ilFixImage();
}


#endif//IL_NO_SUN

