//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/21/2002 <--Y2K Compliant! =]
//
// Filename: il/il_dds-save.c
//
// Description: Saves a DirectDraw Surface (.dds) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_DDS
#include "il_dds.h"
#include "il_manip.h"
#include <limits.h>



//! Writes a Dds file
ILboolean ilSaveDds(const ILstring FileName)
{
	ILHANDLE	DdsFile;
	ILboolean	bDds = IL_FALSE;

	if (ilGetBoolean(IL_FILE_MODE) == IL_FALSE) {
		if (iFileExists(FileName)) {
			ilSetError(IL_FILE_ALREADY_EXISTS);
			return IL_FALSE;
		}
	}

	DdsFile = iopenw(FileName);
	if (DdsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDds;
	}

	bDds = ilSaveDdsF(DdsFile);
	iclosew(DdsFile);

	return bDds;
}


//! Writes a Dds to an already-opened file
ILboolean ilSaveDdsF(ILHANDLE File)
{
	iSetOutputFile(File);
	return iSaveDdsInternal();
}


//! Writes a Dds to a memory "lump"
ILboolean ilSaveDdsL(ILvoid *Lump, ILuint Size)
{
	iSetOutputLump(Lump, Size);
	return iSaveDdsInternal();
}


// Internal function used to save the Dds.
ILboolean iSaveDdsInternal()
{
	ILenum DXTCFormat, Origin;

	if (ilNextPower2(iCurImage->Width) != iCurImage->Width ||
		ilNextPower2(iCurImage->Height) != iCurImage->Height ||
		ilNextPower2(iCurImage->Depth) != iCurImage->Depth) {
			ilSetError(IL_BAD_DIMENSIONS);
			return IL_FALSE;
	}

	Origin = iCurImage->Origin;
	if (Origin != IL_ORIGIN_UPPER_LEFT)
		ilFlipImage();

	DXTCFormat = iGetInt(IL_DXTC_FORMAT);
	WriteHeader(iCurImage, DXTCFormat);
	Compress(iCurImage, DXTCFormat);



	if (Origin != IL_ORIGIN_UPPER_LEFT)
		ilFlipImage();

	return IL_FALSE;
}


// @TODO:  Finish this, as it is incomplete.
ILboolean WriteHeader(ILimage *Image, ILenum DXTCFormat)
{
	ILuint i, FourCC, Flags1 = 0, Flags2 = 0, ddsCaps1 = 0, LinearSize;

	Flags1 |= DDS_LINEARSIZE;
	Flags2 |= DDS_FOURCC;

	switch (DXTCFormat)
	{
		case IL_DXT1:
			FourCC = MAKEFOURCC('D','X','T','1');
			break;
		case IL_DXT2:
			FourCC = MAKEFOURCC('D','X','T','2');
			break;
		case IL_DXT3:
			FourCC = MAKEFOURCC('D','X','T','3');
			break;
		case IL_DXT4:
			FourCC = MAKEFOURCC('D','X','T','4');
			break;
		case IL_DXT5:
			FourCC = MAKEFOURCC('D','X','T','5');
			break;
		default:
			// Error!
			return IL_FALSE;
	}

	iwrite("DDS ", 1, 4);
	SaveLittleUInt(124);		// Size1
	SaveLittleUInt(Flags1);		// Flags1
	SaveLittleUInt(Image->Height);
	SaveLittleUInt(Image->Width);

	if (DXTCFormat == IL_DXT1) {
		LinearSize = Image->Width * Image->Height / 16 * 8;
	}
	else {
		LinearSize = Image->Width * Image->Height / 16 * 16;
	}
	SaveLittleUInt(LinearSize);	// LinearSize
	SaveLittleUInt(0);			// Depth
	SaveLittleUInt(0);			// MipMapCount
	SaveLittleUInt(0);			// AlphaBitDepth

	for (i = 0; i < 10; i++)
		SaveLittleUInt(0);		// Not used

	SaveLittleUInt(32);			// Size2
	SaveLittleUInt(Flags2);		// Flags2
	SaveLittleUInt(FourCC);		// FourCC
	SaveLittleUInt(0);			// RGBBitCount
	SaveLittleUInt(0);			// RBitMask
	SaveLittleUInt(0);			// GBitMask
	SaveLittleUInt(0);			// BBitMask
	SaveLittleUInt(0);			// RGBAlphaBitMask
	ddsCaps1 |= DDS_TEXTURE;
	SaveLittleUInt(ddsCaps1);	// ddsCaps1
	SaveLittleUInt(0);			// ddsCaps2
	SaveLittleUInt(0);			// ddsCaps3
	SaveLittleUInt(0);			// ddsCaps4
	SaveLittleUInt(0);			// TextureStage

	return IL_TRUE;
}


ILushort *CompressTo565(ILimage *Image)
{
	ILimage		*TempImage;
	ILushort	*Data;
	ILuint		i, j;

	if ((Image->Type != IL_UNSIGNED_BYTE && Image->Type != IL_BYTE) || Image->Format == IL_COLOUR_INDEX) {
		TempImage = iConvertImage(IL_BGR, IL_UNSIGNED_BYTE);  // Needs to be BGRA.
		if (TempImage == NULL)
			return NULL;
	}

	Data = (ILushort*)ialloc(iCurImage->Width * iCurImage->Height * 2);
	if (Data == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		ilCloseImage(TempImage);
		return NULL;
	}

	switch (Image->Format)
	{
		case IL_RGB:
			for (i = 0, j = 0; i < iCurImage->SizeOfPlane; i += 3, j++) {
				Data[j]  = (iCurImage->Data[i  ] >> 3) << 11;
				Data[j] |= (iCurImage->Data[i+1] >> 2) << 5;
				Data[j] |=  iCurImage->Data[i+2] >> 3;
			}
			break;

		case IL_RGBA:
			for (i = 0, j = 0; i < iCurImage->SizeOfPlane; i += 4, j++) {
				Data[j]  = (iCurImage->Data[i  ] >> 3) << 11;
				Data[j] |= (iCurImage->Data[i+1] >> 2) << 5;
				Data[j] |=  iCurImage->Data[i+2] >> 3;
			}
			break;

		case IL_BGR:
			for (i = 0, j = 0; i < iCurImage->SizeOfPlane; i += 3, j++) {
				Data[j]  = (iCurImage->Data[i+2] >> 3) << 11;
				Data[j] |= (iCurImage->Data[i+1] >> 2) << 5;
				Data[j] |=  iCurImage->Data[i  ] >> 3;
			}
			break;

		case IL_BGRA:
			for (i = 0, j = 0; i < iCurImage->SizeOfPlane; i += 4, j++) {
				Data[j]  = (iCurImage->Data[i+2] >> 3) << 11;
				Data[j] |= (iCurImage->Data[i+1] >> 2) << 5;
				Data[j] |=  iCurImage->Data[i  ] >> 3;
			}
			break;

		case IL_LUMINANCE:
			return NULL;
			//break;
	}

	return Data;
}


ILboolean Compress(ILimage *Image, ILenum DXTCFormat)
{
	ILushort	*Data, Block[16], ex0, ex1;
	ILuint		x, y, BitMask;

	Data = CompressTo565(Image);
	if (Data == NULL)
		return IL_FALSE;


	for (y = 0; y < Image->Height; y += 4) {
		for (x = 0; x < Image->Width; x += 4) {
			GetBlock(Block, Data, Image, x, y);
			ChooseEndpoints(Block, &ex0, &ex1);
			SaveLittleUShort(ex0);
			SaveLittleUShort(ex1);
			BitMask = GenBitMask(ex0, ex1, 4, Block, NULL);
			SaveLittleUInt(BitMask);
		}		
	}


	ifree(Data);

	return IL_TRUE;
}


// Assumed to be 16-bit (5:6:5).
ILboolean GetBlock(ILushort *Block, ILushort *Data, ILimage *Image, ILuint XPos, ILuint YPos)
{
	ILuint x, y, i = 0, Offset;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			Offset = (YPos + y) * Image->Width + (XPos + x);
			Block[i++] = Data[Offset];
		}
	}

	return IL_TRUE;
}


ILvoid ShortToColor565(ILushort Pixel, Color565 *Colour)
{
	Colour->nRed   = (Pixel & 0xF800) >> 11;
	Colour->nGreen = (Pixel & 0x07E0) >> 5;
	Colour->nBlue  = (Pixel & 0x001F);
	return;
}


ILvoid ShortToColor888(ILushort Pixel, Color888 *Colour)
{
	Colour->r = ((Pixel & 0xF800) >> 11) << 3;
	Colour->g = ((Pixel & 0x07E0) >> 5)  << 2;
	Colour->b = ((Pixel & 0x001F))       << 3;
	return;
}


ILushort Color565ToShort(Color565 *Colour)
{
	return (Colour->nRed << 11) | (Colour->nGreen << 5) | (Colour->nBlue);
}


ILushort Color888ToShort(Color888 *Colour)
{
	return ((Colour->r >> 3) << 11) | ((Colour->g >> 2) << 5) | (Colour->b >> 3);
}


ILuint GenBitMask(ILushort ex0, ILushort ex1, ILuint NumCols, ILushort *In, Color888 *OutCol)
{
	ILuint		i, j, Closest, Dist, BitMask = 0;
	ILubyte		Mask[16];
	Color888	c, Colours[4];

	ShortToColor888(ex0, &Colours[0]);
	ShortToColor888(ex1, &Colours[1]);
	if (NumCols == 3) {
		Colours[2].r = (Colours[0].r + Colours[1].r) / 2;
		Colours[2].g = (Colours[0].g + Colours[1].g) / 2;
		Colours[2].b = (Colours[0].b + Colours[1].b) / 2;
	}
	else {  // NumCols == 4
		Colours[2].r = (2 * Colours[0].r + Colours[1].r + 1) / 3;
		Colours[2].g = (2 * Colours[0].g + Colours[1].g + 1) / 3;
		Colours[2].b = (2 * Colours[0].b + Colours[1].b + 1) / 3;
		Colours[3].r = (Colours[0].r + 2 * Colours[1].r + 1) / 3;
		Colours[3].g = (Colours[0].g + 2 * Colours[1].g + 1) / 3;
		Colours[3].b = (Colours[0].b + 2 * Colours[1].b + 1) / 3;
	}

	for (i = 0; i < 16; i++) {
		Closest = UINT_MAX;
		ShortToColor888(In[i], &c);
		for (j = 0; j < NumCols; j++) {
			Dist = Distance(&c, &Colours[j]);
			if (Dist < Closest) {
				Closest = Dist;
				Mask[i] = j;
				if (OutCol) {
					OutCol[i].r = Colours[j].r;
					OutCol[i].g = Colours[j].g;
					OutCol[i].b = Colours[j].b;
				}
			}
		}
	}

	for (i = 0; i < 16; i++) {
		BitMask |= (Mask[i] << (i*2));
	}

	return BitMask;
}


ILuint Distance(Color888 *c1, Color888 *c2)
{
	return  (c1->r - c2->r) * (c1->r - c2->r) +
			(c1->g - c2->g) * (c1->g - c2->g) +
			(c1->b - c2->b) * (c1->b - c2->b);
}


ILvoid ChooseEndpoints(ILushort *Block, ILushort *ex0, ILushort *ex1)
{
	ILuint		i, j;
	Color888	Colours[16];
	ILuint		Farthest = 0, d;

	for (i = 0; i < 16; i++) {
		ShortToColor888(Block[i], &Colours[i]);
	}

	for (i = 0; i < 16; i++) {
		for (j = i+1; j < 16; j++) {
			d = Distance(&Colours[i], &Colours[j]);
			if (d > Farthest) {
				Farthest = d;
				*ex0 = Block[i];
				*ex1 = Block[j];
			}
		}
	}

	return;
}

#endif//IL_NO_DDS
