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
ILboolean iGetFitsHead(/*FITSHEAD *Header*/)
{

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidFits(void)
{
	/*FITSHEAD Header;

	if (!iGetFitsHead(&Header))
		return IL_FALSE;
	iseek(-, IL_SEEK_CUR);*/
	
	return iCheckFits(/*&Header*/);
}


// Internal function used to check if the HEADER is a valid FITS header.
ILboolean iCheckFits(FITSHEAD *Header)
{

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

	return ilFixImage();
}

#endif//IL_NO_FITS
