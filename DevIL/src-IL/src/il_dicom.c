//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/14/2009
//
// Filename: src-IL/src/il_dicom.c
//
// Description: Reads from a Digital Imaging and Communications in Medicine
//				(DICOM) file.  Specifications can be found at 
//                http://en.wikipedia.org/wiki/Dicom.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_DICOM

typedef struct DICOMHEAD
{
	ILuint	Width;

} DICOMHEAD;

ILboolean	iIsValidDicom(void);
ILboolean	iCheckDicom(DICOMHEAD *Header);
ILboolean	iLoadDicomInternal(void);


//! Checks if the file specified in FileName is a valid DICOM file.
ILboolean ilIsValidDicom(ILconst_string FileName)
{
	ILHANDLE	DicomFile;
	ILboolean	bDicom = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("dicom")) && !iCheckExtension(FileName, IL_TEXT("dcm"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bDicom;
	}
	
	DicomFile = iopenr(FileName);
	if (DicomFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDicom;
	}
	
	bDicom = ilIsValidDicomF(DicomFile);
	icloser(DicomFile);
	
	return bDicom;
}


//! Checks if the ILHANDLE contains a valid DICOM file at the current position.
ILboolean ilIsValidDicomF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidDicom();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid DICOM lump.
ILboolean ilIsValidDicomL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidDicom();
}


// Internal function used to get the DICOM header from the current file.
ILboolean iGetDicomHead(DICOMHEAD *Header)
{

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidDicom(void)
{
	DICOMHEAD	Header;
	ILuint		Pos = itell();

	if (!iGetDicomHead(&Header))
		return IL_FALSE;
	// The length of the header varies, so we just go back to the original position.
	iseek(Pos, IL_SEEK_CUR);

	return iCheckDicom(&Header);
}


// Internal function used to check if the HEADER is a valid DICOM header.
ILboolean iCheckDicom(DICOMHEAD *Header)
{
return IL_FALSE;
	//return IL_TRUE;
}


//! Reads a DICOM file
ILboolean ilLoadDicom(ILconst_string FileName)
{
	ILHANDLE	DicomFile;
	ILboolean	bDicom = IL_FALSE;
	
	DicomFile = iopenr(FileName);
	if (DicomFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDicom;
	}

	bDicom = ilLoadDicomF(DicomFile);
	icloser(DicomFile);

	return bDicom;
}


//! Reads an already-opened DICOM file
ILboolean ilLoadDicomF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadDicomInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a DICOM
ILboolean ilLoadDicomL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadDicomInternal();
}


// Internal function used to load the DICOM.
ILboolean iLoadDicomInternal(void)
{
	DICOMHEAD	Header;

ilSetError(IL_FORMAT_NOT_SUPPORTED);
return IL_FALSE;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetDicomHead(&Header))
		return IL_FALSE;
	if (!iCheckDicom(&Header))
		return IL_FALSE;


	return ilFixImage();
}



#endif//IL_NO_DICOM
