//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/21/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_dds-save.c
//
// Description: Saves a DirectDraw Surface (.dds) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include "il_dds.h"
#include "il_manip.h"
#include <limits.h>


#ifndef IL_NO_DDS

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
	if (!Compress(iCurImage, DXTCFormat))
		return IL_FALSE;

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

	// @TODO:  Fix the pre-multiplied alpha problem.
	if (DXTCFormat == IL_DXT2)
		DXTCFormat = IL_DXT3;
	else if (DXTCFormat == IL_DXT4)
		DXTCFormat = IL_DXT5;

	switch (DXTCFormat)
	{
		case IL_DXT1:
			FourCC = IL_MAKEFOURCC('D','X','T','1');
			break;
		case IL_DXT2:
			FourCC = IL_MAKEFOURCC('D','X','T','2');
			break;
		case IL_DXT3:
			FourCC = IL_MAKEFOURCC('D','X','T','3');
			break;
		case IL_DXT4:
			FourCC = IL_MAKEFOURCC('D','X','T','4');
			break;
		case IL_DXT5:
			FourCC = IL_MAKEFOURCC('D','X','T','5');
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

#endif//IL_NO_DDS


ILuint ILAPIENTRY ilGetDXTCData(ILvoid *Buffer, ILuint BufferSize, ILenum DXTCFormat)
{
	if (Buffer == NULL) {  // Return the number that will be written with a subsequent call.
		if (ilNextPower2(iCurImage->Width) != iCurImage->Width ||
			ilNextPower2(iCurImage->Height) != iCurImage->Height ||
			ilNextPower2(iCurImage->Depth) != iCurImage->Depth) {
				return 0;
		}

		switch (DXTCFormat)
		{
			case IL_DXT1:
				return iCurImage->Width * iCurImage->Height / 16 * 8;
			case IL_DXT3:
			case IL_DXT5:
				return iCurImage->Width * iCurImage->Height / 16 * 16;
			default:
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return 0;
		}
	}

	if (DXTCFormat == iCurImage->DxtcFormat && iCurImage->DxtcSize && iCurImage->DxtcData) {
		memcpy(Buffer, iCurImage->DxtcData, IL_MIN(BufferSize, iCurImage->DxtcSize));
		return IL_MIN(BufferSize, iCurImage->DxtcSize);
	}

	iSetOutputLump(Buffer, BufferSize);
	return Compress(iCurImage, DXTCFormat);
}


ILushort *CompressTo565(ILimage *Image)
{
	ILimage		*TempImage;
	ILushort	*Data;
	ILuint		i, j;

	if ((Image->Type != IL_UNSIGNED_BYTE && Image->Type != IL_BYTE) || Image->Format == IL_COLOUR_INDEX) {
		TempImage = iConvertImage(iCurImage, IL_BGR, IL_UNSIGNED_BYTE);  // @TODO: Needs to be BGRA.
		if (TempImage == NULL)
			return NULL;
	}
	else {
		TempImage = Image;
	}

	Data = (ILushort*)ialloc(iCurImage->Width * iCurImage->Height * 2);
	if (Data == NULL) {
		if (TempImage != Image)
			ilCloseImage(TempImage);
		return NULL;
	}

	switch (Image->Format)
	{
		case IL_RGB:
			for (i = 0, j = 0; i < TempImage->SizeOfPlane; i += 3, j++) {
				Data[j]  = (TempImage->Data[i  ] >> 3) << 11;
				Data[j] |= (TempImage->Data[i+1] >> 2) << 5;
				Data[j] |=  TempImage->Data[i+2] >> 3;
			}
			break;

		case IL_RGBA:
			for (i = 0, j = 0; i < TempImage->SizeOfPlane; i += 4, j++) {
				Data[j]  = (TempImage->Data[i  ] >> 3) << 11;
				Data[j] |= (TempImage->Data[i+1] >> 2) << 5;
				Data[j] |=  TempImage->Data[i+2] >> 3;
			}
			break;

		case IL_BGR:
			for (i = 0, j = 0; i < TempImage->SizeOfPlane; i += 3, j++) {
				Data[j]  = (TempImage->Data[i+2] >> 3) << 11;
				Data[j] |= (TempImage->Data[i+1] >> 2) << 5;
				Data[j] |=  TempImage->Data[i  ] >> 3;
			}
			break;

		case IL_BGRA:
			for (i = 0, j = 0; i < TempImage->SizeOfPlane; i += 4, j++) {
				Data[j]  = (TempImage->Data[i+2] >> 3) << 11;
				Data[j] |= (TempImage->Data[i+1] >> 2) << 5;
				Data[j] |=  TempImage->Data[i  ] >> 3;
			}
			break;

		case IL_LUMINANCE:
			for (i = 0, j = 0; i < TempImage->SizeOfPlane; i++, j++) {
				Data[j]  = (TempImage->Data[i] >> 3) << 11;
				Data[j] |= (TempImage->Data[i] >> 2) << 5;
				Data[j] |=  TempImage->Data[i] >> 3;
			}
			break;
	}

	if (TempImage != Image)
		ilCloseImage(TempImage);

	return Data;
}


ILuint Compress(ILimage *Image, ILenum DXTCFormat)
{
	ILushort	*Data, Block[16], ex0, ex1;
	ILuint		x, y, i, BitMask;//, Rms1, Rms2;
	ILubyte		*Alpha, AlphaBlock[16], AlphaBitMask[6], AlphaOut[16], a0, a1;
	ILboolean	HasAlpha;
	ILuint		Count = 0;

	if (ilNextPower2(iCurImage->Width) != iCurImage->Width ||
		ilNextPower2(iCurImage->Height) != iCurImage->Height ||
		ilNextPower2(iCurImage->Depth) != iCurImage->Depth) {
			ilSetError(IL_BAD_DIMENSIONS);
			return 0;
	}

	Data = CompressTo565(Image);
	if (Data == NULL)
		return 0;

	Alpha = ilGetAlpha(IL_UNSIGNED_BYTE);
	if (Alpha == NULL) {
		ifree(Data);
		return 0;
	}

	switch (DXTCFormat)
	{
		case IL_DXT1:
			for (y = 0; y < Image->Height; y += 4) {
				for (x = 0; x < Image->Width; x += 4) {
					GetAlphaBlock(AlphaBlock, Alpha, Image, x, y);
					HasAlpha = IL_FALSE;
					for (i = 0 ; i < 16; i++) {
						if (AlphaBlock[i] < 128) {
							HasAlpha = IL_TRUE;
							break;
						}
					}

					GetBlock(Block, Data, Image, x, y);
					ChooseEndpoints(Block, &ex0, &ex1);
					CorrectEndDXT1(&ex0, &ex1, HasAlpha);
					SaveLittleUShort(ex0);
					SaveLittleUShort(ex1);
					if (HasAlpha)
						BitMask = GenBitMask(ex0, ex1, 3, Block, AlphaBlock, NULL);
					else
						BitMask = GenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					SaveLittleUInt(BitMask);
					Count += 8;
				}		
			}
			break;

		/*case IL_DXT2:
			for (y = 0; y < Image->Height; y += 4) {
				for (x = 0; x < Image->Width; x += 4) {
					GetAlphaBlock(AlphaBlock, Alpha, Image, x, y);
					for (i = 0; i < 16; i += 2) {
						iputc((ILubyte)(((AlphaBlock[i] >> 4) << 4) | (AlphaBlock[i+1] >> 4)));
					}

					GetBlock(Block, Data, Image, x, y);
					PreMult(Block, AlphaBlock);
					ChooseEndpoints(Block, &ex0, &ex1);
					SaveLittleUShort(ex0);
					SaveLittleUShort(ex1);
					BitMask = GenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					SaveLittleUInt(BitMask);
				}		
			}
			break;*/

		case IL_DXT3:
			for (y = 0; y < Image->Height; y += 4) {
				for (x = 0; x < Image->Width; x += 4) {
					GetAlphaBlock(AlphaBlock, Alpha, Image, x, y);
					for (i = 0; i < 16; i += 2) {
						iputc((ILubyte)(((AlphaBlock[i] >> 4) << 4) | (AlphaBlock[i+1] >> 4)));
					}

					GetBlock(Block, Data, Image, x, y);
					ChooseEndpoints(Block, &ex0, &ex1);
					SaveLittleUShort(ex0);
					SaveLittleUShort(ex1);
					BitMask = GenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					SaveLittleUInt(BitMask);
					Count += 16;
				}		
			}
			break;

		case IL_DXT5:
			for (y = 0; y < Image->Height; y += 4) {
				for (x = 0; x < Image->Width; x += 4) {
					GetAlphaBlock(AlphaBlock, Alpha, Image, x, y);
					ChooseAlphaEndpoints(AlphaBlock, &a0, &a1);
					GenAlphaBitMask(a0, a1, 6, AlphaBlock, AlphaBitMask, AlphaOut);
					/*Rms2 = RMSAlpha(AlphaBlock, AlphaOut);
					GenAlphaBitMask(a0, a1, 8, AlphaBlock, AlphaBitMask, AlphaOut);
					Rms1 = RMSAlpha(AlphaBlock, AlphaOut);
					if (Rms2 <= Rms1) {  // Yeah, we have to regenerate...
						GenAlphaBitMask(a0, a1, 6, AlphaBlock, AlphaBitMask, AlphaOut);
						Rms2 = a1;  // Just reuse Rms2 as a temporary variable...
						a1 = a0;
						a0 = Rms2;
					}*/
					iputc(a0);
					iputc(a1);
					iwrite(AlphaBitMask, 1, 6);

					GetBlock(Block, Data, Image, x, y);
					ChooseEndpoints(Block, &ex0, &ex1);
					SaveLittleUShort(ex0);
					SaveLittleUShort(ex1);
					BitMask = GenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					SaveLittleUInt(BitMask);
					Count += 16;
				}
			}
			break;
	}

	ifree(Data);

	return Count;
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


ILboolean GetAlphaBlock(ILubyte *Block, ILubyte *Data, ILimage *Image, ILuint XPos, ILuint YPos)
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


ILuint GenBitMask(ILushort ex0, ILushort ex1, ILuint NumCols, ILushort *In, ILubyte *Alpha, Color888 *OutCol)
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
		Colours[3].r = (Colours[0].r + Colours[1].r) / 2;
		Colours[3].g = (Colours[0].g + Colours[1].g) / 2;
		Colours[3].b = (Colours[0].b + Colours[1].b) / 2;
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
		if (Alpha) {  // Test to see if we have 1-bit transparency
			if (Alpha[i] < 128) {
				Mask[i] = 3;  // Transparent
				if (OutCol) {
					OutCol[i].r = Colours[3].r;
					OutCol[i].g = Colours[3].g;
					OutCol[i].b = Colours[3].b;
				}
				continue;
			}
		}

		// If no transparency, try to find which colour is the closest.
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


ILvoid GenAlphaBitMask(ILubyte a0, ILubyte a1, ILuint Num, ILubyte *In, ILubyte *Mask, ILubyte *Out)
{
	ILubyte Alphas[8], M[16];
	ILuint	i, j, Closest, Dist;

	Alphas[0] = a0;
	Alphas[1] = a1;

	// 8-alpha or 6-alpha block?    
	if (Num == 8) {    
		// 8-alpha block:  derive the other six alphas.    
		// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
		Alphas[2] = (6 * Alphas[0] + 1 * Alphas[1] + 3) / 7;	// bit code 010
		Alphas[3] = (5 * Alphas[0] + 2 * Alphas[1] + 3) / 7;	// bit code 011
		Alphas[4] = (4 * Alphas[0] + 3 * Alphas[1] + 3) / 7;	// bit code 100
		Alphas[5] = (3 * Alphas[0] + 4 * Alphas[1] + 3) / 7;	// bit code 101
		Alphas[6] = (2 * Alphas[0] + 5 * Alphas[1] + 3) / 7;	// bit code 110
		Alphas[7] = (1 * Alphas[0] + 6 * Alphas[1] + 3) / 7;	// bit code 111  
	}    
	else {  
		// 6-alpha block.    
		// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
		Alphas[2] = (4 * Alphas[0] + 1 * Alphas[1] + 2) / 5;	// Bit code 010
		Alphas[3] = (3 * Alphas[0] + 2 * Alphas[1] + 2) / 5;	// Bit code 011
		Alphas[4] = (2 * Alphas[0] + 3 * Alphas[1] + 2) / 5;	// Bit code 100
		Alphas[5] = (1 * Alphas[0] + 4 * Alphas[1] + 2) / 5;	// Bit code 101
		Alphas[6] = 0x00;										// Bit code 110
		Alphas[7] = 0xFF;										// Bit code 111
	}

	for (i = 0; i < 16; i++) {
		Closest = UINT_MAX;
		for (j = 0; j < 8; j++) {
			Dist = abs((ILint)In[i] - (ILint)Alphas[j]);
			if (Dist < Closest) {
				Closest = Dist;
				M[i] = j;
			}
		}
	}

	if (Out) {
		for (i = 0; i < 16; i++) {
			Out[i] = Alphas[M[i]];
		}
	}

	// First three bytes.
	Mask[0] = (M[0]) | (M[1] << 3) | ((M[2] & 0x03) << 6);
	Mask[1] = (M[2] & 0x04) | (M[3] << 1) | (M[4] << 4) | ((M[5] & 0x01) << 7);
	Mask[2] = (M[5] & 0x06) | (M[6] << 2) | (M[7] << 5);

	// Second three bytes.
	Mask[3] = (M[8]) | (M[9] << 3) | ((M[10] & 0x03) << 6);
	Mask[4] = (M[10] & 0x04) | (M[11] << 1) | (M[12] << 4) | ((M[13] & 0x01) << 7);
	Mask[5] = (M[13] & 0x06) | (M[14] << 2) | (M[15] << 5);

	return;
}


ILuint RMSAlpha(ILubyte *Orig, ILubyte *Test)
{
	ILuint	RMS = 0, i;
	ILint	d;

	for (i = 0; i < 16; i++) {
		d = Orig[i] - Test[i];
		RMS += d*d;
	}

	//RMS /= 16;

	return RMS;
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


ILvoid ChooseAlphaEndpoints(ILubyte *Block, ILubyte *a0, ILubyte *a1)
{
	ILuint	i;
	ILuint	Lowest = 0xFF, Highest = 0;

	for (i = 0; i < 16; i++) {
		if (Block[i] < Lowest) {
			*a1 = Block[i];  // a1 is the lower of the two.
			Lowest = Block[i];
		}
		else if (Block[i] > Highest) {
			*a0 = Block[i];  // a0 is the higher of the two.
			Highest = Block[i];
		}
	}

	return;
}


ILvoid CorrectEndDXT1(ILushort *ex0, ILushort *ex1, ILboolean HasAlpha)
{
	ILushort Temp;

	if (HasAlpha) {
		if (*ex0 > *ex1) {
			Temp = *ex0;
			*ex0 = *ex1;
			*ex1 = Temp;
		}
	}
	else {
		if (*ex0 < *ex1) {
			Temp = *ex0;
			*ex0 = *ex1;
			*ex1 = Temp;
		}
	}

	return;
}


ILvoid PreMult(ILushort *Data, ILubyte *Alpha)
{
	Color888	Colour;
	ILuint		i;

	for (i = 0; i < 16; i++) {
		ShortToColor888(Data[i], &Colour);
		Colour.r = (ILubyte)(((ILuint)Colour.r * Alpha[i]) >> 8);
		Colour.g = (ILubyte)(((ILuint)Colour.g * Alpha[i]) >> 8);
		Colour.b = (ILubyte)(((ILuint)Colour.b * Alpha[i]) >> 8);

		/*Colour.r = (ILubyte)(Colour.r * (Alpha[i] / 255.0));
		Colour.g = (ILubyte)(Colour.g * (Alpha[i] / 255.0));
		Colour.b = (ILubyte)(Colour.b * (Alpha[i] / 255.0));*/

		Data[i] = Color888ToShort(&Colour);
		ShortToColor888(Data[i], &Colour);
	}

	return;
}
