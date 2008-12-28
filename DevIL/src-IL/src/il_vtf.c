//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 12/27/2008
//
// Filename: src-IL/src/il_vtf.c
//
// Description: Reads from and writes to a Valve Texture Format (.vtf) file.
//                These are used in Valve's Source games.  VTF specs available
//                from http://developer.valvesoftware.com/wiki/VTF.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_VTF
#include "il_vtf.h"
#include "il_dds.h"


//@TODO: Get rid of these globals.
static VTFHEAD	Head;


//! Checks if the file specified in FileName is a valid VTF file.
ILboolean ilIsValidVtf(ILconst_string FileName)
{
	ILHANDLE	VtfFile;
	ILboolean	bVtf = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("vtf"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bVtf;
	}
	
	VtfFile = iopenr(FileName);
	if (VtfFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bVtf;
	}
	
	bVtf = ilIsValidVtfF(VtfFile);
	icloser(VtfFile);
	
	return bVtf;
}


//! Checks if the ILHANDLE contains a valid VTF file at the current position.
ILboolean ilIsValidVtfF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidVtf();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid VTF lump.
ILboolean ilIsValidVtfL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidVtf();
}


// Internal function used to get the VTF header from the current file.
ILboolean iGetVtfHead(VTFHEAD *Header)
{
	iread(Header->Signature, 1, 4);
	Header->Version[0] = GetLittleUInt();
	Header->Version[1] = GetLittleUInt();
	Header->HeaderSize = GetLittleUInt();
	Header->Width = GetLittleUShort();
	Header->Height = GetLittleUShort();
	Header->Flags = GetLittleUInt();
	Header->Frames = GetLittleUShort();
	Header->FirstFrame = GetLittleUShort();
	iseek(4, IL_SEEK_CUR);  // Padding
	Header->Reflectivity[0] = GetLittleFloat();
	Header->Reflectivity[1] = GetLittleFloat();
	Header->Reflectivity[2] = GetLittleFloat();
	iseek(4, IL_SEEK_CUR);  // Padding
	Header->BumpmapScale = GetLittleFloat();
	Header->HighResImageFormat = GetLittleUInt();
	Header->MipmapCount = (ILubyte)igetc();
	Header->LowResImageFormat = GetLittleUInt();
	Header->LowResImageWidth = (ILubyte)igetc();
	Header->LowResImageHeight = (ILubyte)igetc();
	Header->Depth = GetLittleUShort();

	iseek(Header->HeaderSize - sizeof(VTFHEAD), IL_SEEK_CUR);

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidVtf()
{
	if (!iGetVtfHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(VTFHEAD), IL_SEEK_CUR);
	
	return iCheckVtf(&Head);
}


//@TODO: Add more checks.
// Should we check for Frames, MipmapCount and Depth != 0?

// Internal function used to check if the HEADER is a valid VTF header.
ILboolean iCheckVtf(VTFHEAD *Header)
{
	// The file signature is "VTF\0".
	if ((Header->Signature[0] != 'V') || (Header->Signature[1] != 'T') || (Header->Signature[2] != 'F')
		|| (Header->Signature[3] != 0))
		return IL_FALSE;
	// Are there other versions available yet?
	if (Header->Version[0] != 7)
		return IL_FALSE;
	// We have 7.0 through 7.4 as of 12/27/2008.
	if (Header->Version[1] > 4)
		return IL_FALSE;
	// May change in future version of the specifications.
	//  80 is through version 7.2, and 96 is through 7.4.
	if ((Header->HeaderSize != 80) && (Header->HeaderSize != 104))
		return IL_FALSE;

	// 0 is an invalid dimension
	if (Header->Width == 0 || Header->Height == 0)
		return IL_FALSE;
	// Width and Height must be powers of 2.
	if ((ilNextPower2(Header->Width) != Header->Width) || (ilNextPower2(Header->Height) != Header->Height))
		return IL_FALSE;
	if ((ilNextPower2(Header->LowResImageWidth) != Header->LowResImageWidth)
		|| (ilNextPower2(Header->LowResImageHeight) != Header->LowResImageHeight))
		return IL_FALSE;
	// In addition, the LowResImage has to have dimensions no greater than 16.
	if ((Header->LowResImageWidth > 16) || (Header->LowResImageHeight > 16)
		|| (Header->LowResImageWidth == 0) || (Header->LowResImageHeight == 0))
		return IL_FALSE;
	// And the LowResImage has to have dimensions less than or equal to the main image.
	if ((Header->LowResImageWidth > Header->Width) || (Header->LowResImageHeight > Header->Height))
		return IL_FALSE;
	// The LowResImage must be in DXT1 format.
	if (Header->LowResImageFormat != IMAGE_FORMAT_DXT1)
		return IL_FALSE;
	
	return IL_TRUE;
}


//! Reads a VTF file
ILboolean ilLoadVtf(ILconst_string FileName)
{
	ILHANDLE	VtfFile;
	ILboolean	bVtf = IL_FALSE;
	
	VtfFile = iopenr(FileName);
	if (VtfFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bVtf;
	}

	bVtf = ilLoadVtfF(VtfFile);
	icloser(VtfFile);

	return bVtf;
}


//! Reads an already-opened VTF file
ILboolean ilLoadVtfF(ILHANDLE File) {
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadVtfInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a VTF
ILboolean ilLoadVtfL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadVtfInternal();
}


// Internal function used to load the VTF.
ILboolean iLoadVtfInternal()
{
	ILboolean	bVtf = IL_TRUE;
	ILimage		*Image;
	ILuint		Width, Height, Depth, SizeOfData;
	ILenum		Format, Type;
	ILint		i, j;
	ILuint		Channels, Bpc;
	ILubyte		*CompData = NULL;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if (!iGetVtfHead(&Head))
		return IL_FALSE;
	if (!iCheckVtf(&Head)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	//@TODO: Take care of volume textures soon.
	if (Head.Depth != 1) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	//@TODO: Take care of animation chains soon.
	if (Head.Frames > 1) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}

	// Read the low resolution image first.  This is just a thumbnail.
	//  Just read it and discard it for right now.
	//  The block size is 8, and the compression ratio is 6:1.
	SizeOfData = max(Head.LowResImageWidth * Head.LowResImageHeight / 2, 8);
	iseek(SizeOfData, IL_SEEK_CUR);

	//@TODO: Make this a helper function that set channels, bpc and format.
	switch (Head.HighResImageFormat)
	{
		case IMAGE_FORMAT_DXT1:
		case IMAGE_FORMAT_DXT3:
		case IMAGE_FORMAT_DXT5:
			Channels = 4;
			Bpc = 1;
			Format = IL_RGBA;
			Type = IL_UNSIGNED_BYTE;
			break;
		case IMAGE_FORMAT_BGR888:
			Channels = 3;
			Bpc = 1;
			Format = IL_BGR;
			Type = IL_UNSIGNED_BYTE;
			break;
		case IMAGE_FORMAT_BGRA8888:
			Channels = 4;
			Bpc = 1;
			Format = IL_BGRA;
			Type = IL_UNSIGNED_BYTE;
			break;
		case IMAGE_FORMAT_RGBA16161616:  // 16-bit shorts
			Channels = 4;
			Bpc = 2;
			Format = IL_RGBA;
			Type = IL_UNSIGNED_SHORT;
			break;
		case IMAGE_FORMAT_RGBA16161616F:  // 16-bit floats
			Channels = 4;
			Bpc = 4;
			Format = IL_RGBA;
			Type = IL_FLOAT;
			break;

		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}

	if (!ilTexImage(Head.Width, Head.Height, Head.Depth, Channels, Format, Type, NULL))
		return IL_FALSE;
	// The origin should be in the upper left.
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	Image = iCurImage;

	// Next comes the high resolution images.  The mipmaps are in order from smallest to largest in the file.
	//  We want them the other way around.
	Width = iCurImage->Width;  Height = iCurImage->Height;  Depth = iCurImage->Depth;
	for (i = 0; i < Head.MipmapCount - 1; i++) {
		// 1 is the smallest dimension possible.
		Width = (Width >> 1) == 0 ? 1 : (Width >> 1);
		Height = (Height >> 1) == 0 ? 1 : (Height >> 1);
		Depth = (Depth >> 1) == 0 ? 1 : (Depth >> 1);

		Image->Next = ilNewImage(Width, Height, Depth, Channels, Bpc);
		if (Image->Next == NULL)  //@TODO: Do we need to do any cleanup here?
			return IL_FALSE;
		Image->Origin = IL_ORIGIN_UPPER_LEFT;
		Image = Image->Next;
	}

	for (i = 0; i < Head.MipmapCount; i++) {
		Image = iCurImage;
		// We want to put the smallest mipmap at the end, but it is first in the file.
		for (j = 1; j < Head.MipmapCount - i; j++) {
			Image = Image->Next;  // Move down the linked list.
		}

		switch (Head.HighResImageFormat)
		{
			// DXT1 compression
			case IMAGE_FORMAT_DXT1:
				// The block size is 8.
				SizeOfData = max(Image->Width * Image->Height / 2, 8);
				CompData = ialloc(SizeOfData);  // Gives a 6:1 compression ratio
				iread(CompData, 1, SizeOfData);
				if (ilGetInteger(IL_KEEP_DXTC_DATA) == IL_TRUE) {
					Image->DxtcSize = SizeOfData;
					Image->DxtcData = CompData;
					Image->DxtcFormat = IL_DXT5;
					CompData = NULL;
				}
				bVtf = DecompressDXT1(Image, CompData);
				break;

			// DXT3 compression
			case IMAGE_FORMAT_DXT3:
				// The block size is 16.
				SizeOfData = max(Image->Width * Image->Height, 16);
				CompData = ialloc(SizeOfData);  // Gives a 4:1 compression ratio
				iread(CompData, 1, SizeOfData);
				if (ilGetInteger(IL_KEEP_DXTC_DATA) == IL_TRUE) {
					Image->DxtcSize = SizeOfData;
					Image->DxtcData = CompData;
					Image->DxtcFormat = IL_DXT3;
					CompData = NULL;
				}
				bVtf = DecompressDXT3(Image, CompData);
				break;

			// DXT5 compression
			case IMAGE_FORMAT_DXT5:
				// The block size is 16.
				SizeOfData = max(Image->Width * Image->Height, 16);
				CompData = ialloc(SizeOfData);  // Gives a 4:1 compression ratio
				iread(CompData, 1, SizeOfData);
				if (ilGetInteger(IL_KEEP_DXTC_DATA) == IL_TRUE) {
					Image->DxtcSize = SizeOfData;
					Image->DxtcData = CompData;
					Image->DxtcFormat = IL_DXT5;
					CompData = NULL;
				}
				bVtf = DecompressDXT5(Image, CompData);
				break;

			// Uncompressed BGR(A) data (24-bit and 32-bit)
			case IMAGE_FORMAT_BGR888:
			case IMAGE_FORMAT_BGRA8888:
				// Just copy the data over - no compression.
				if (iread(Image->Data, 1, Image->SizeOfData) != Image->SizeOfData)
					bVtf = IL_FALSE;
				else
					bVtf = IL_TRUE;
				break;

			// Uncompressed 16-bit shorts
			case IMAGE_FORMAT_RGBA16161616:
				// Just copy the data over - no compression.
				if (iread(Image->Data, 1, Image->SizeOfData) != Image->SizeOfData)
					bVtf = IL_FALSE;
				else
					bVtf = IL_TRUE;
				break;

			// Uncompressed 16-bit floats (must be converted to 32-bit)
			case IMAGE_FORMAT_RGBA16161616F:
				SizeOfData = Image->Width * Image->Height * Image->Depth * Image->Bpp * 2;
				CompData = ialloc(SizeOfData);  // Not compressed data
				if (iread(CompData, 1, SizeOfData) != SizeOfData) {
					bVtf = IL_FALSE;
					break;
				}
				bVtf = iConvFloat16ToFloat32((ILuint*)Image->Data, (ILushort*)CompData, SizeOfData / 2);
				break;
		}

		ifree(CompData);
		CompData = NULL;
		if (bVtf == IL_FALSE)  //@TODO: Do we need to do any cleanup here?
			return IL_FALSE;
	}



//@TODO: Cannot be like this for animation chains!
	iCurImage->Mipmaps = iCurImage->Next;
	iCurImage->Next = NULL;

	ilBindImage(ilGetCurName());  // Set to parent image first.
	return ilFixImage();
}

#endif//IL_NO_VTF
