//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/03/2008
//
// Filename: src-IL/src/il_exr.c
//
// Description: Reads from and writes to an OpenEXR (.exr) file.
//                EXR specs available from http://www.openexr.com.
//                from 
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_EXR
#include "il_exr.h"


//! Checks if the file specified in FileName is a valid EXR file.
ILboolean ilIsValidExr(ILconst_string FileName)
{
	ILHANDLE	ExrFile;
	ILboolean	bExr = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("exr"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bExr;
	}
	
	ExrFile = iopenr(FileName);
	if (ExrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bExr;
	}
	
	bExr = ilIsValidExrF(ExrFile);
	icloser(ExrFile);
	
	return bExr;
}


//! Checks if the ILHANDLE contains a valid EXR file at the current position.
ILboolean ilIsValidExrF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidExr();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid EXR lump.
ILboolean ilIsValidExrL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidExr();
}


// Internal function used to get the EXR header from the current file.
ILboolean iGetExrHead(EXRHEAD *Header)
{
	Header->MagicNumber = GetLittleUInt();
	Header->Version = GetLittleUInt();

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidExr()
{
	EXRHEAD Head;

	if (!iGetExrHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(EXRHEAD), IL_SEEK_CUR);
	
	return iCheckExr(&Head);
}


// Internal function used to check if the HEADER is a valid EXR header.
ILboolean iCheckExr(EXRHEAD *Header)
{
	// The file magic number (signature) is 0x76, 0x2f, 0x31, 0x01
	if (Header->MagicNumber != 0x762F3101)
		return IL_FALSE;
	// The only valid version so far is version 2.  The upper value has
	//  to do with tiling.
	if (Header->Version != 0x002 && Header->Version != 0x202)
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads an EXR file
ILboolean ilLoadExr(ILconst_string FileName)
{
	ILHANDLE	ExrFile;
	ILboolean	bExr = IL_FALSE;
	
	ExrFile = iopenr(FileName);
	if (ExrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bExr;
	}

	bExr = ilLoadExrF(ExrFile);
	icloser(ExrFile);

	return bExr;
}


//! Reads an already-opened EXR file
ILboolean ilLoadExrF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadExrInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains an EXR
ILboolean ilLoadExrL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadExrInternal();
}


ILboolean iExrReadString(char *String, ILuint Length)
{
	ILuint	i;

	if (String == NULL) {
		ilSetError(IL_INTERNAL_ERROR);
		return IL_FALSE;
	}

	for (i = 0; i < Length; i++) {
		String[i] = igetc();
		if (String[i] == 0)
			break;
	}

	// Error if we have already hit the end of the file.
	if (ieof()) {
		ilSetError(IL_FILE_READ_ERROR);
		return IL_FALSE;
	}

	// Will never get to this point if there is a proper terminator (NULL).
	if (i == Length) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	return IL_TRUE;
}


// Internal function used to load the EXR.
ILboolean iLoadExrInternal()
{
	EXRHEAD	Header;
	ILubyte	Attribute[32], AttType[32], ChanOrder[4], ChanName[32];
	ILuint	AttSize, i, Pos;
	ILint	NumChannels = -1, PixType = -1;
	ILuint	PLinear, XSampling, YSampling;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if (!iGetExrHead(&Head))
		return IL_FALSE;
	if (!iCheckExr(&Head)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	// Read in the "header", which consists of a list of attributes.

	while (!ieof()) {
		// Each attribute name is 1 to 31 characters (NULL-terminated)
		if (!iExrReadString(Attribute, 32))
			return IL_FALSE;  // Error is already set.

		if (Attribute[0] == 0)  // Signals end of attribute list.
			break;

		if (!strncmp(Attributes, "channels", strlen("channels")) {
			if (!iExrReadString(AttType, 32))  // Read the attribute type (up to 31 bytes plus NULL).
				return IL_FALSE;
			if (strncmp(AttType, "chlist", strlen("chlist")) {  // Only can be "chlist" for "channels".
				ilSetError(IL_INVALID_FILE_HEADER);
				return IL_FALSE;
			}
			AttSize = GetLittleUInt();  // Read the size of the attribute.
			if (AttSize == 0) {
				ilSetError(IL_INVALID_FILE_HEADER);
				return IL_FALSE;
			}

			Pos = itell();  // Need to make sure that the attribute size is correct.

			for (i = 0; i < AttSize; ) {  // Increased inside the loop.
				// Add to the number of channels found.
				if (NumChannels == -1)
					NumChannels = 1;
				else
					NumChannels++;

				if (NumChannels > 4) {  // We cannot support more than 4 channels in a file right now.
					ilSetError(IL_FORMAT_NOT_SUPPORTED);
					return IL_FALSE;
				}

				if (!iExrReadString(ChanName, 32))  // Channels are named separately.
					return IL_FALSE;

				// R, G, B are stored in the first byte, and that's it.  Anything else is invalid for our use.
				//@TODO: Make this not so!
				if (ChanName[0] == 'R' || ChanName[0] == 'G' || ChanName[0] == 'B') {
					// ChanOrder is where we're going to keep this "truncated" information.
					ChanOrder[NumChannels-1] = ChanName[0];
				}
				else {
					ilSetError(IL_FORMAT_NOT_SUPPORTED);
					return IL_FALSE;
				}

				if (PixType == -1)  // Not yet initialized
					PixType = GetLittleUInt();
				else {  //@TODO: We cannot handle formats with differing storage types for each channel.
					if (PixType != GetLittleUInt()) {
						ilSetError(IL_FORMAT_NOT_SUPPORTED);
						return IL_FALSE;
					}
				}

				//@TODO: pLinear, not sure what it does.
				PLinear = igetc();
				if (PLinear != 0 && PLinear != 1) {  // Must be either 0 or 1
					ilSetError(IL_INVALID_FILE_HEADER);
					return IL_FALSE;
				}

				iseek(3, IL_SEEK_CUR);  // Reserved values
				//@TODO: Find out what these next two values are used for.
				XSampling = GetLittleUInt();  // No information in the specs
				YSampling = GetLittleUInt();  // Same as above

				AttSize += (strlen(ChanName) + 1 + 16);
			}
		}
		else {  // We do not care about this attribute right now.
			if (!iExrReadString(AttType, 32))  // Not sure on what is a maximum for these.
				return IL_FALSE;
			AttSize = GetLittleUInt();  // Get the attribute size.
			iseek(AttSize, IL_SEEK_CUR);  // Just skip it for now (AttSize bytes).
		}
	}

	





	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;  // Correct?

	ilFixImage();

	return IL_TRUE;
}

#endif//IL_NO_EXR
