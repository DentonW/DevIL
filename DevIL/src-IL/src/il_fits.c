//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/03/2009
//
// Filename: src-IL/src/il_fits.c
//
// Description: Reads from a Flexible Image Transport System (.fits) file.
//                Specifications were found at 
//                http://www.fileformat.info/format/fits.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_FITS

ILboolean iIsValidFits(void);
ILboolean iCheckFits(FITSHEAD *Header);
ILboolean iLoadFitsInternal(void);
ILuint GetCardImage(FITSHEAD *Header);

typedef struct FITSHEAD
{
	ILboolean	IsSimple;
	ILint	BitsPixel;
	ILint	NumAxes;  // Number of dimensions / axes
	ILint	Width;
	ILint	Height;
	ILint	Depth;
	ILint	NumChans;

} FITSHEAD;

ILenum
{
	CARD_READ_FAIL = -1,
	CARD_END = 1,
	CARD_SIMPLE,
	CARD_NOT_SIMPLE,
	CARD_BITPIX,
	CARD_NUMAXES,
	CARD_AXIS

};


//! Checks if the file specified in FileName is a valid FITS file.
ILboolean ilIsValidFits(ILconst_string FileName)
{
	ILHANDLE	FitsFile;
	ILboolean	bFits = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("fits"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bFits;
	}
	
	FitsFile = iopenr(FileName);
	if (FitsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bFits;
	}
	
	bFits = ilIsValidFitsF(FitsFile);
	icloser(FitsFile);
	
	return bFits;
}


//! Checks if the ILHANDLE contains a valid FITS file at the current position.
ILboolean ilIsValidFitsF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidFits();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid FITS lump.
ILboolean ilIsValidFitsL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidFits();
}


// Internal function used to get the FITS header from the current file.
ILboolean iGetFitsHead(FITSHEAD *Header)
{
	ILenum	CardKey;

//@TODO: Use something other than memset?
	memset(Header, 0, sizeof(Header));  // Clear the header to all 0s first.

	do {
		CardKey = GetCardImage(Header);
		if (CardKey == CARD_END)  // End of the header
			break;
		if (CardKey == CARD_READ_FAIL)
			return IL_FALSE;
	} while (!ieof());

	// The header should never reach the end of the file.
	if (ieof())
		return IL_FALSE;  // Error needed?

	// The header must always be a multiple of 2880, so we skip the padding bytes (spaces).
	iseek((2880 - (itell() % 2880)) % 2880, IL_SEEK_CUR);

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidFits(void)
{
	FITSHEAD	Header;
	ILuint		Pos = itell();

	if (!iGetFitsHead(&Header))
		return IL_FALSE;
	// The length of the header varies, so we just go back to the original position.
	iseek(Pos, IL_SEEK_CUR);

	return iCheckFits(&Header);
}


// Internal function used to check if the HEADER is a valid FITS header.
ILboolean iCheckFits(FITSHEAD *Header)
{
return IL_FALSE;
	return IL_TRUE;
}


//! Reads a FITS file
ILboolean ilLoadFits(ILconst_string FileName)
{
	ILHANDLE	FitsFile;
	ILboolean	bFits = IL_FALSE;
	
	FitsFile = iopenr(FileName);
	if (FitsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bFits;
	}

	bFits = ilLoadFitsF(FitsFile);
	icloser(FitsFile);

	return bFits;
}


//! Reads an already-opened FITS file
ILboolean ilLoadFitsF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadFitsInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a FITS
ILboolean ilLoadFitsL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadFitsInternal();
}


// Internal function used to load the FITS.
ILboolean iLoadFitsInternal(void)
{
	FITSHEAD	Header;
	//ILuint		Bpc; - Needed?
	ILenum		Format, Type;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetFitsHead(&Header))
		return IL_FALSE;
	if (!iCheckFits(&Header))
		return IL_FALSE;

	switch (Header.BitsPixel)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_SIGNED_SHORT;
			break;
		case 32:
			Type = IL_SIGNED_INT;
			break;
		case -32:
			Type = IL_FLOAT;
			break;
		case -64:
			Type = IL_DOUBLE;
			break;
		default:  // No default error needed, since iCheckFits did this already.
	}
	Bpc = ilGetBpcType(Type);  // Bytes per channel of Type

	switch (Header.NumAxes)
	{
		case 1:  // Just a 1D image
			Format = IL_LUMINANCE;
			Header.Height = 1;
			Header.Depth = 1;
			Header.NumChans = 1;
			break;

		case 2:  // Assuming it is a 2D image (width+height)
			Format = IL_LUMINANCE;
			Header.Depth = 1;
			Header.NumChans = 1;
			break;

		case 3:
			// If NumChans > 4, we have to instead do a volume image.
			if (Header.NumChans > 4) {
				Format = IL_LUMINANCE;
				Header.Depth = Header.NumChans;
				Header.NumChans = 1;
			}
			else {
//@TODO: We are going to assume that the formats are the same right now.  Change needed?
				Format = ilGetFormatBpp(Header.NumChans);
				Header.Depth = 1;
			}
			break;

		case 4:
			// We cannot deal with more than 4 channels in an image.
			if (Header.NumChans > 4) {
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}

		default:  // Should already be taken care of by iCheckFits.
	}

	if (!ilTexImage(Header.Width, Header.Height, Header.Depth, Header.NumAxes, Format, Type, NULL))
		return IL_FALSE;

	if (iread(iCurImage->Data, 1, iCurImage->SizeOfData) != iCurImage->SizeOfData)
		return IL_FALSE;

Intentionally left here to cause compilation error -
  Make sure ilConvertImage can deal with signed formats.
  Write iCheckFits.

	return ilFixImage();
}


//@TODO: NAXISx have to come in order.  Check this!
ILenum GetCardImage(FITSHEAD *Header)
{
	ILuint	Buffer[80];

	if (iread(Buffer, 1, 80) != 80) {  // Each card image is exactly 80 bytes long.
		return CARD_READ_FAIL;


//@TODO: Use something other than !stricmp?
	if (!stricmp(Buffer, "END", 3))
		return CARD_END;

	else if (!stricmp(Buffer, "SIMPLE", 6)) {
		// The true value 'T' is always in the 30th position.
		if (Buffer[29] != 'T') {
			// We cannot support FITS files that do not correspond to the standard.
			Header->IsSimple = IL_FALSE;  //@TODO: Does this even need to be set?  Should exit loading anyway.
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return CARD_NOT_SIMPLE;
		}
		Header->IsSimple = IL_TRUE;
		return CARD_SIMPLE;
	}

	else if (!stricmp(Buffer, "BITPIX")) {
		// The specs state that BITPIX has to come after SIMPLE.
		if (Header->IsSimple != IL_TRUE) {
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}
		if (GetCardInt(Buffer, &Header->BitsPixel) != IL_TRUE)
			return CARD_READ_FAIL;
//@TODO: Should I do this check from the calling function?  Does it really matter?
		if (Header->BitsPixel == 0) {
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return CARD_READ_FAIL;
		}
		return CARD_BITPIX;
	}

	else if (!stricmp(Buffer, "NAXIS")) {
		if (GetCardInt(Buffer, &Header->NumAxes) != IL_TRUE)
			return CARD_READ_FAIL;
//@TODO: Should I do this check from the calling function?  Does it really matter?
		if (Header->NumAxes < 1 || Header->NumAxes > 4) {
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return CARD_READ_FAIL;
		}
		return CARD_NUMAXES;
	}

	else if (!stricmp(Buffer, "NAXIS1")) {
		if (Header->NumAxes == 0) {  // Has not been initialized, and it has to come first.
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}
		// First one will always be the width.
		if (GetCardInt(Buffer, &Header->Width) != IL_TRUE)
			return CARD_READ_FAIL;
		return CARD_AXIS;
	}

	else if (!stricmp(Buffer, "NAXIS2")) {
		if (Header->NumAxes == 0) {  // Has not been initialized, and it has to come first.
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}
		// Cannot have a 2nd axis for 0 or 1.
		if (Header->NumAxes == 0 || Header->NumAxes == 1) {
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}
		
//@TODO: We are assuming that this is the height right now.  Could it just be a
//  1D image with multiple bytes per pixel?
		if (GetCardInt(Buffer, &Header->Height) != IL_TRUE)
			return CARD_READ_FAIL;
		return CARD_AXIS;
	}

	else if (!stricmp(Buffer, "NAXIS3")) {
		if (Header->NumAxes == 0) {  // Has not been initialized, and it has to come first.
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}
		// Cannot have a 3rd axis for 0, 1 and 2.
		if (Header->NumAxes < 3) {
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}

//@TODO: We are assuming that this is the bpp of the image.  Could it be the depth?
		if (Header->NumAxes == 3) {
			if (GetCardInt(Buffer, &Header->NumChans) != IL_TRUE)
				return CARD_READ_FAIL;
		}
		else {  // This is guaranteed to be 4 by the earlier check on NumAxes
			if (GetCardInt(Buffer, &Header->Depth) != IL_TRUE)
				return CARD_READ_FAIL;
		}
		return CARD_AXIS;
	}

	else if (!stricmp(Buffer, "NAXIS4")) {
		if (Header->NumAxes == 0) {  // Has not been initialized, and it has to come first.
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}
		// Cannot have a 4th axis for NumAxes < 4.
		if (Header->NumAxes < 4) {
			ilSetError(IL_INVALID_HEADER);
			return CARD_READ_FAIL;
		}

//@TODO: We are assuming that this is the bpp of the image.  Could it be something other?
		if (GetCardInt(Buffer, &Header->NumChans) != IL_TRUE)
			return CARD_READ_FAIL;
		return CARD_AXIS;
	}

	return CARD_READ_FAIL;
}


ILboolean GetCardInt(char *Buffer, ILint *Val)
{
	ILuint	i;
	char	ValString[22];

	if (Buffer[7] != '=' && Buffer[8] != '=')
		return IL_FALSE;
	for (i = 9; i < 29; i++) {
		if (Buffer[i] != ' ' || Buffer[i] != 0)  // Right-aligned with ' ' or 0, so skip.
			break;
	}
	if (i == 29)  // Did not find the integer in positions 10 - 30.
		return IL_FALSE;
  //@TODO: Safest way to do this?
	memcpy(ValString, &Buffer[i-1], 29-i);
	ValString[29-i] = 0;  // Terminate the string.

	//@TODO: Check the return somehow?
	*Val = atoi(ValString);

	return IL_TRUE;
}

#endif//IL_NO_FITS
