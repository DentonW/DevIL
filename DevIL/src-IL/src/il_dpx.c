//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/26/2009
//
// Filename: src-IL/src/il_dpx.c
//
// Description: Reads from a Digital Picture Exchange (.dpx) file.
//				Specifications for this format were	found at
//				http://www.cineon.com/ff_draft.php and
//				http://www.fileformat.info/format/dpx/.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_DPX
#include "il_dpx.h"
#include "il_bits.h"

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


ILboolean DpxGetFileInfo(DPX_FILE_INFO *FileInfo)
{
	//if (iread(FileInfo, 768, 1) != 1)
	//	return IL_FALSE;

	FileInfo->MagicNum = GetBigUInt();
	FileInfo->Offset = GetBigUInt();
	iread(FileInfo->Vers, 8, 1);
	FileInfo->FileSize = GetBigUInt();
	FileInfo->DittoKey = GetBigUInt();
	FileInfo->GenHdrSize = GetBigUInt();
	FileInfo->IndHdrSize = GetBigUInt();
	FileInfo->UserDataSize = GetBigUInt();
	iread(FileInfo->FileName, 100, 1);
	iread(FileInfo->CreateTime, 24, 1);
	iread(FileInfo->Creator, 100, 1);
	iread(FileInfo->Project, 200, 1);
	if (iread(FileInfo->Copyright, 200, 1) != 1)
		return IL_FALSE;
	FileInfo->Key = GetBigUInt();
	iseek(104, IL_SEEK_CUR);  // Skip reserved section.

	return IL_TRUE;
}


ILboolean GetImageElement(DPX_IMAGE_ELEMENT *ImageElement)
{
	ILuint i;
	ImageElement->DataSign = GetBigUInt();
	ImageElement->RefLowData = GetBigUInt();
	iread(&ImageElement->RefLowQuantity, 1, 4);
	ImageElement->RefHighData = GetBigUInt();
	iread(&ImageElement->RefHighQuantity, 1, 4);
	ImageElement->Descriptor = igetc();
	ImageElement->Transfer = igetc();
	ImageElement->Colorimetric = igetc();
	ImageElement->BitSize = igetc();
	ImageElement->Packing = GetBigUShort();
	ImageElement->Encoding = GetBigUShort();
	ImageElement->DataOffset = GetBigUInt();
	ImageElement->EolPadding = GetBigUInt();
	ImageElement->EoImagePadding = GetBigUInt();
	if (iread(ImageElement->Description, 32, 1) != 1)
		return IL_FALSE;
i = itell();
	return IL_TRUE;
}


ILboolean DpxGetImageInfo(DPX_IMAGE_INFO *ImageInfo)
{
	ILuint i;

	//if (iread(ImageInfo, sizeof(DPX_IMAGE_INFO), 1) != 1)
	//	return IL_FALSE;
	ImageInfo->Orientation = GetBigUShort();
	ImageInfo->NumElements = GetBigUShort();
	ImageInfo->Width = GetBigUInt();
	ImageInfo->Height = GetBigUInt();

	for (i = 0; i < 8; i++) {
		GetImageElement(&ImageInfo->ImageElement[i]);
	}

	iseek(52, IL_SEEK_CUR);  // Skip padding bytes.

	return IL_TRUE;
}


ILboolean DpxGetImageOrient(DPX_IMAGE_ORIENT *ImageOrient)
{
	ImageOrient->XOffset = GetBigUInt();
	ImageOrient->YOffset = GetBigUInt();
	iread(&ImageOrient->XCenter, 4, 1);
	iread(&ImageOrient->YCenter, 4, 1);
	ImageOrient->XOrigSize = GetBigUInt();
	ImageOrient->YOrigSize = GetBigUInt();
	iread(ImageOrient->FileName, 100, 1);
	iread(ImageOrient->CreationTime, 24, 1);
	iread(ImageOrient->InputDev, 32, 1);
	if (iread(ImageOrient->InputSerial, 32, 1) != 1)
		return IL_FALSE;
	ImageOrient->Border[0] = GetBigUShort();
	ImageOrient->Border[1] = GetBigUShort();
	ImageOrient->Border[2] = GetBigUShort();
	ImageOrient->Border[3] = GetBigUShort();
	ImageOrient->PixelAspect[0] = GetBigUInt();
	ImageOrient->PixelAspect[1] = GetBigUInt();
	iseek(28, IL_SEEK_CUR);  // Skip reserved bytes.

	return IL_TRUE;
}


// Internal function used to load the DPX.
ILboolean iLoadDpxInternal(void)
{
	DPX_FILE_INFO		FileInfo;
	DPX_IMAGE_INFO		ImageInfo;
	DPX_IMAGE_ORIENT	ImageOrient;
	BITFILE		*File;
	ILuint		i, NumElements, CurElem = 0;
	ILushort	Val, *ShortData;
	ILubyte		Data[5];


	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if (!DpxGetFileInfo(&FileInfo))
		return IL_FALSE;
	if (!DpxGetImageInfo(&ImageInfo))
		return IL_FALSE;
	if (!DpxGetImageOrient(&ImageOrient))
		return IL_FALSE;

	if (!ilTexImage(ImageInfo.Width, ImageInfo.Height, 1, 3, IL_RGB, IL_UNSIGNED_SHORT, NULL))
		return IL_FALSE;
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	iseek(FileInfo.Offset, IL_SEEK_SET);

	if (ImageInfo.ImageElement[CurElem].Packing == 1) {
		ShortData = (ILushort*)iCurImage->Data;
		NumElements = iCurImage->SizeOfData / 2;
		switch (ImageInfo.ImageElement[CurElem].BitSize)
		{
			case 10:
				for (i = 0; i < NumElements;) {
					iread(Data, 1, 4);
					Val = ((Data[0] << 2) + ((Data[1] & 0xC0) >> 6)) << 6;  // Use the first 10 bits of the word-aligned data.
					ShortData[i++] = Val | ((Val & 0x3F0) >> 4);  // Fill in the lower 6 bits with a copy of the higher bits.
					Val = (((Data[1] & 0x3F) << 4) + ((Data[2] & 0xF0) >> 4)) << 6;  // Use the next 10 bits.
					ShortData[i++] = Val | ((Val & 0x3F0) >> 4);  // Same fill
					Val = (((Data[2] & 0x0F) << 6) + ((Data[3] & 0xFC) >> 2)) << 6;  // And finally use the last 10 bits (ignores the last 2 bits).
					ShortData[i++] = Val | ((Val & 0x3F0) >> 4);  // Same fill
				}
				break;

			//case 1:
			//case 8:
			//case 12:
			//case 16:
			//case 32:
			default:
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
		}
	}
	else if (ImageInfo.ImageElement[0].Packing == 0) {
		/*File = bfile(iGetFile());
		if (File == NULL)
			return IL_FALSE;  //@TODO: Error?
		ShortData = (ILushort*)iCurImage->Data;
		NumElements = iCurImage->SizeOfData / 2;
		for (i = 0; i < NumElements; i++) {
			bread(&Val, 1, 10, File);
			ShortData[i] = Val << 6;
		}
		bclose(File);*/

		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	else {
		return IL_FALSE;  //@TODO: Take care of this in an iCheckDpx* function.
	}


	return ilFixImage();
}

#endif//IL_NO_DPX

