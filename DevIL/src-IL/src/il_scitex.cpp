//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2017 by Denton Woods
// Last modified: 01/21/2017
//
// Filename: src-IL/src/il_scitex.cpp
//
// Description: Reads from a Scitex Continuous Tone (.ct) file.
//                Specifications were found at http://fileformats.archiveteam.org/wiki/Scitex_CT
//				  and http://web.archive.org/web/20130506073138/http://electricmessiah.org/press/the-scitex-ct-file-format/
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_SCITEX
//#include "il_scitex.h"


typedef struct SCITEXHEAD
{
	ILubyte	Comment[80];
	ILubyte	Sig[2];			// Signature, should be "CT"
	ILubyte	Units;			// 0 for mm, 1 for inches
	ILubyte	NumChans;		// Number of channels
	ILushort ColorModel;	// Color model (7=RGB, 8=GREYSCALE, 15=CMYK)
	ILfloat	HeightUnits;	// Physical height
	ILfloat	WidthUnits;		// Physical width
	ILuint	HeightPixels;
	ILuint	WidthPixels;
} SCITEXHEAD;

ILboolean iIsValidScitex(void);
ILboolean iLoadScitexInternal(void);
ILboolean iCheckScitex(SCITEXHEAD *Header);


// Internal function used to get the Scitex header from the current file.
ILboolean iGetScitexHead(SCITEXHEAD *Header)
{
	char HeightUnits[15], WidthUnits[15], HeightPixels[13], WidthPixels[13];

	if (iread(Header->Comment, 1, 80) != 80)
		return IL_FALSE;
	if (iread(Header->Sig, 1, 2) != 2)
		return IL_FALSE;
	if (iseek(942, IL_SEEK_CUR) == -1)
		return IL_FALSE;
	Header->Units = igetc();
	Header->NumChans = igetc();
	Header->ColorModel = GetBigUShort();
	if (iread(&HeightUnits, 1, 14) != 14)
		return IL_FALSE;
	if (iread(&WidthUnits, 1, 14) != 14)
		return IL_FALSE;
	if (iread(&HeightPixels, 1, 12) != 12)
		return IL_FALSE;
	if (iread(&WidthPixels, 1, 12) != 12)
		return IL_FALSE;

	HeightUnits[14] = NULL;
	WidthUnits[14] = NULL;
	HeightPixels[12] = NULL;
	WidthPixels[12] = NULL;

	Header->HeightUnits = (ILfloat)atof(HeightUnits);
	Header->WidthUnits = (ILfloat)atof(WidthUnits);
	Header->HeightPixels = (ILint)atoi(HeightPixels);
	Header->WidthPixels = (ILint)atof(WidthPixels);

	switch (Header->ColorModel)
	{
		case 7:
			if (Header->NumChans != 3)
				return IL_FALSE;
			break;
		case 8:
			if (Header->NumChans != 1)
				return IL_FALSE;
			break;
		//case 15:
		//default:
	}

	return IL_TRUE;
}


//! Checks if the file specified in FileName is a valid BLP file.
ILboolean ilIsValidScitex(ILconst_string FileName)
{
	ILHANDLE	ScitexFile;
	ILboolean	bScitex = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("ct"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bScitex;
	}

	ScitexFile = iopenr(FileName);
	if (ScitexFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bScitex;
	}

	bScitex = ilIsValidScitexF(ScitexFile);
	icloser(ScitexFile);

	return bScitex;
}


//! Checks if the ILHANDLE contains a valid Scitex file at the current position.
ILboolean ilIsValidScitexF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidScitex();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid BLP lump.
ILboolean ilIsValidScitexL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidScitex();
}


// Internal function to get the header and check it.
ILboolean iIsValidScitex(void)
{
	SCITEXHEAD Header;

	if (!iGetScitexHead(&Header))
		return IL_FALSE;
	iseek(-2048, IL_SEEK_CUR);

	return iCheckScitex(&Header);
}


// Internal function used to check if the HEADER is a valid BLP header.
ILboolean iCheckScitex(SCITEXHEAD *Header)
{
	// The file signature is 'CT'.
	if (Header->Sig[0] != 'C' || Header->Sig[1] != 'T')
		return IL_FALSE;
	if (Header->Units != 0 && Header->Units != 1)
		return IL_FALSE;
	//@TODO: Handle CMYK version
	if (Header->ColorModel != 7 && Header->ColorModel != 8 /*&& Header->ColorModel != 15*/)
		return IL_FALSE;
	if (Header->HeightUnits <= 0 || Header->WidthUnits <= 0)
		return IL_FALSE;
	if (Header->HeightPixels <= 0 || Header->WidthPixels <= 0)
		return IL_FALSE;

	return IL_TRUE;
}



//! Reads a BLP file
ILboolean ilLoadScitex(ILconst_string FileName)
{
	ILHANDLE	ScitexFile;
	ILboolean	bScitex = IL_FALSE;

	ScitexFile = iopenr(FileName);
	if (ScitexFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bScitex;
	}

	bScitex = ilLoadScitexF(ScitexFile);
	icloser(ScitexFile);

	return bScitex;
}


//! Reads an already-opened BLP file
ILboolean ilLoadScitexF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadScitexInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a BLP
ILboolean ilLoadScitexL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadScitexInternal();
}


// Internal function used to load the BLP.
ILboolean iLoadScitexInternal(void)
{
	SCITEXHEAD Header;
	ILuint Pos = 0;
	ILubyte *RowData;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetScitexHead(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	if (!iCheckScitex(&Header)) {
		return IL_FALSE;
	}

	iseek(2048, IL_SEEK_SET);
	RowData = (ILubyte*)ialloc(Header.WidthPixels);
	if (RowData == NULL)
		return IL_FALSE;

	switch (Header.ColorModel)
	{
		case 7:
			if (!ilTexImage(Header.WidthPixels, Header.HeightPixels, 1, Header.NumChans, 0, IL_UNSIGNED_BYTE, NULL))
			{
				ifree(RowData);
				return IL_FALSE;
			}
			iCurImage->Format = IL_RGB;

			for (ILuint h = 0; h < iCurImage->Height; h++)
			{
				for (ILuint c = 0; c < iCurImage->Bpp; c++)
				{
					if (iread(RowData, 1, Header.WidthPixels) != Header.WidthPixels)
					{
						ifree(RowData);
						return IL_FALSE;
					}
					for (ILuint w = 0; w < iCurImage->Width; w++)
					{
						iCurImage->Data[Pos + w * 3 + c] = RowData[w];
					}
					if (iCurImage->Width % 2 != 0)  // Pads to even width
						igetc();
				}
				Pos += Header.WidthPixels * 3;
			}
			break;

		case 8:
			if (!ilTexImage(Header.WidthPixels, Header.HeightPixels, 1, Header.NumChans, 0, IL_UNSIGNED_BYTE, NULL))
			{
				ifree(RowData);
				return IL_FALSE;
			}
			iCurImage->Format = IL_LUMINANCE;

			for (ILuint h = 0; h < iCurImage->Height; h++)
			{
				iread(iCurImage->Data + Pos, 1, iCurImage->Width);
				if (iCurImage->Width % 2 != 0)  // Pads to even width
					igetc();
				Pos += Header.WidthPixels;
			}

			break;

		//case 15:
		//default:
	}
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	return ilFixImage();
};



#endif//IL_NO_SCITEX
