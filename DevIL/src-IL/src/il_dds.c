//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 12/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/il_dds.c
//
// Description: Reads from a DirectDraw Surface (.dds) file.
//
//-----------------------------------------------------------------------------


//
//
// Note:  Almost all this code is from nVidia's DDS-loading example at
//	http://www.nvidia.com/view.asp?IO=dxtc_decompression_code
//	and from the specs at
//	http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dx8_c/hh/dx8_c/graphics_using_0j03.asp
//
//


#include "il_internal.h"
#ifndef IL_NO_DDS
#include "il_dds.h"


//! Checks if the file specified in FileName is a valid .bmp file.
ILboolean ilIsValidDds(const ILstring FileName)
{
	ILHANDLE	DdsFile;
	ILboolean	bDds = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("dds"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bDds;
	}

	DdsFile = iopenr(FileName);
	if (DdsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDds;
	}

	bDds = ilIsValidDdsF(DdsFile);
	icloser(DdsFile);

	return bDds;
}


//! Checks if the ILHANDLE contains a valid .bmp file at the current position.
ILboolean ilIsValidDdsF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidDds();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid .bmp lump.
ILboolean ilIsValidDdsL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidDds();
}


// Internal function used to get the .bmp header from the current file.
ILvoid iGetDdsHead(DDSHEAD *Header)
{
	iread(Header->Signature, 1, 4);
	Header->Size1 = GetLittleInt();
	Header->Flags1 = GetLittleInt();
	Header->Height = GetLittleInt();
	Header->Width = GetLittleInt();
	Header->LinearSize = GetLittleInt();
	Header->Depth = GetLittleInt();
	Header->MipMapCount = GetLittleInt();
	Header->AlphaBitDepth = GetLittleInt();
	iread(Header->NotUsed, sizeof(ILint), 10);
	Header->Size2 = GetLittleInt();
	Header->Flags2 = GetLittleInt();
	Header->FourCC = GetLittleInt();
	Header->RGBBitCount = GetLittleInt();
	Header->RBitMask = GetLittleInt();
	Header->GBitMask = GetLittleInt();
	Header->BBitMask = GetLittleInt();
	Header->RGBAlphaBitMask = GetLittleInt();
	Header->ddsCaps1 = GetLittleInt();
	Header->ddsCaps2 = GetLittleInt();
	Header->ddsCaps3 = GetLittleInt();
	Header->ddsCaps4 = GetLittleInt();
	Header->TextureStage = GetLittleInt();

	return;
}


// Internal function to get the header and check it.
ILboolean iIsValidDds()
{
	ILboolean	IsValid;
	DDSHEAD		Head;

	iGetDdsHead(&Head);
	iseek(-(ILint)sizeof(DDSHEAD), IL_SEEK_CUR);  // Go ahead and restore to previous state

	IsValid = iCheckDds(&Head);

	return IsValid;
}


// Internal function used to check if the HEADER is a valid .bmp header.
ILboolean iCheckDds(DDSHEAD *Head)
{
	if (strnicmp(Head->Signature, "DDS ", 4))
		return IL_FALSE;
	if (Head->Size1 != 124)
		return IL_FALSE;
	if (Head->Size2 != 32)
		return IL_FALSE;
	if (Head->Width == 0 || Head->Height == 0)
		return IL_FALSE;
	return IL_TRUE;
}


//! Reads a .bmp file
ILboolean ilLoadDds(const ILstring FileName)
{
	ILHANDLE	DdsFile;
	ILboolean	bDds = IL_FALSE;

	DdsFile = iopenr(FileName);
	if (DdsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDds;
	}

	bDds = ilLoadDdsF(DdsFile);
	icloser(DdsFile);

	return bDds;
}


//! Reads an already-opened .bmp file
ILboolean ilLoadDdsF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadDdsInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .bmp
ILboolean ilLoadDdsL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadDdsInternal();
}


ILboolean iLoadDdsInternal()
{
	CompData = NULL;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iGetDdsHead(&Head);
	Width = Head.Width;
	Height = Head.Height;

	ReadData();
	DecodePixelFormat();
	AllocImage();
	Decompress();
	ReadMipmaps();

	if (CompData)
		free(CompData);

	ilFixImage();

	return IL_TRUE;
}


// Reads the compressed data
ILboolean ReadData()
{
	ILuint	Bps;
	ILint	i;
	ILubyte	*Temp;

	if (CompData) {
		ifree(CompData);
		CompData = NULL;
	}

	if (Head.Flags1 & DDS_LINEARSIZE) {
		CompData = (ILubyte*)malloc(Head.LinearSize);
		if (CompData == NULL) {
			ilSetError(IL_OUT_OF_MEMORY);
			return IL_FALSE;
		}

		iread(CompData, 1, Head.LinearSize);
    }
    else {
		Bps = Width * Head.RGBBitCount / 8;
		CompSize = Head.LinearSize * Height;
		CompLineSize = Bps;

		CompData = (ILubyte*)malloc(Head.LinearSize * Height);
		if (CompData == NULL) {
			ilSetError(IL_OUT_OF_MEMORY);
			return IL_FALSE;
		}

        Temp = CompData;
        for (i = 0; i < Height; i++) {
            iread(Temp, 1, Bps);
            Temp += Head.LinearSize;
        }
    }

	return IL_TRUE;
}


ILboolean AllocImage()
{
	if (CompFormat != PF_ARGB) {
		if (!ilTexImage(Width, Height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL)) {
			free(CompData);
			return IL_FALSE;
		}
	}
	else {
		if (Head.RGBAlphaBitMask == 0) {
			if (!ilTexImage(Width, Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL)) {
				free(CompData);
				return IL_FALSE;
			}
		}
		else {
			if (!ilTexImage(Width, Height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL)) {
				free(CompData);
				return IL_FALSE;
			}
		}	
	}
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	Image = iCurImage;

	return IL_TRUE;
}


ILvoid DecodePixelFormat()
{
    switch (Head.FourCC)
    {
        case 0:
            // This dds texture isn't compressed so write out ARGB format
			CompFormat = PF_ARGB;
            break;

        case MAKEFOURCC('D','X','T','1'):
			CompFormat = PF_DXT1;
            break;

        case MAKEFOURCC('D','X','T','2'):
			CompFormat = PF_DXT2;
            break;

        case MAKEFOURCC('D','X','T','3'):
			CompFormat = PF_DXT3;
            break;

        case MAKEFOURCC('D','X','T','4'):
			CompFormat = PF_DXT4;
            break;

        case MAKEFOURCC('D','X','T','5'):
			CompFormat = PF_DXT5;
            break;

		default:
			CompFormat = PF_UNKNOWN;
			break;
    }
	return;
}


ILboolean Decompress()
{
	switch (CompFormat)
	{
		case PF_ARGB:
			return DecompressARGB();

		case PF_DXT1:
			return DecompressDXT1();

		case PF_DXT2:
			return DecompressDXT2();

		case PF_DXT3:
			return DecompressDXT3();	

		case PF_DXT4:
			return DecompressDXT4();

		case PF_DXT5:
			return DecompressDXT5();		

		case PF_UNKNOWN:
			return IL_FALSE;
	}

	return IL_FALSE;
}


ILboolean ReadMipmaps()
{
	ILint	i;
	ILuint	CompFactor;
	ILubyte	Bpp;

	if (!(Head.Flags1 & DDS_LINEARSIZE))
		return IL_FALSE;  // @TODO: Change. Cannot accurately calculate the LinearSize yet.

	if (!(Head.Flags1 & DDS_MIPMAPCOUNT))
		  if (Head.MipMapCount)
			  return IL_FALSE;
		  else
			  return IL_TRUE;

	if (CompFormat != PF_ARGB || Head.RGBAlphaBitMask != 0)
		Bpp = 4;
	else
		Bpp = 3;

	CompFactor = (Width * Height * Bpp) / Head.LinearSize;
	Image = iCurImage;

	// Added by Kenneth Hurley 01/23/2002.
	if (!(Head.Flags1 & DDS_MIPMAPCOUNT))
	{
		Head.MipMapCount = 1;
	}

	for (i = 0; i < Head.MipMapCount - 1; i++) {
		Width = IL_MAX(Width / 2, 1);
		Height = IL_MAX(Height / 2, 1);

		Image->Next = ilNewImage(Width, Height, 1, Bpp, 1);
		if (Image->Next == NULL)
			return IL_FALSE;
		Image = Image->Next;
		Image->Origin = IL_ORIGIN_UPPER_LEFT;

		// Smallest block possible is 8 for DXT1 and 16 for DXT2+.
		Head.LinearSize = Width * Height * Bpp / CompFactor;
		Head.LinearSize = IL_MAX(Head.LinearSize, CompFormat == PF_DXT1 ? 8 : 16);

		ReadData();
		Decompress();
	}

	iCurImage->Mipmaps = iCurImage->Next;
	iCurImage->Next = NULL;
	iCurImage->NumMips = Head.MipMapCount - 1;

	return IL_TRUE;
}


ILboolean DecompressDXT1()
{
	int			x, y, i, j, k, Select;
	ILubyte		*Temp;
	Color565	*color_0, *color_1;
	Color8888	colours[4], *col;
	ILuint		bitmask, Offset;


	Temp = CompData;
	for (y = 0; y < Height; y += 4) {
		for (x = 0; x < Width; x += 4) {
			if (y >= Height || x >= Width)
				break;

			color_0 = ((Color565*)Temp);
			color_1 = ((Color565*)(Temp+2));
			bitmask = ((ILuint*)Temp)[1];
			Temp += 8;

			colours[0].r = color_0->nRed << 3;
			colours[0].g = color_0->nGreen << 2;
			colours[0].b = color_0->nBlue << 3;
			colours[0].a = 0xFF;

			colours[1].r = color_1->nRed << 3;
			colours[1].g = color_1->nGreen << 2;
			colours[1].b = color_1->nBlue << 3;
			colours[1].a = 0xFF;


			if (*((ILushort*)color_0) > *((ILushort*)color_1)) {
				// Four-color block: derive the other two colors.    
				// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block.
				colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
				colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
				colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
				colours[2].a = 0xFF;

				colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
				colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
				colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
				colours[3].a = 0xFF;
			}    
			else { 
				// Three-color block: derive the other color.
				// 00 = color_0,  01 = color_1,  10 = color_2,
				// 11 = transparent.
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block. 
				colours[2].b = (colours[0].b + colours[1].b) / 2;
				colours[2].g = (colours[0].g + colours[1].g) / 2;
				colours[2].r = (colours[0].r + colours[1].r) / 2;
				colours[2].a = 0xFF;

				colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
				colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
				colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
				colours[3].a = 0x00;
			}

			for (j = 0, k = 0; j < 4; j++) {
				for (i = 0; i < 4; i++, k++) {
					if (j >= Height || i >= Width)
						break;

					Select = (bitmask & (0x03 << k*2)) >> k*2;
					col = &colours[Select];

					Offset = (y + j) * Image->Bps + (x + i) * Image->Bpp;
					Image->Data[Offset + 0] = col->r;
					Image->Data[Offset + 1] = col->g;
					Image->Data[Offset + 2] = col->b;
					Image->Data[Offset + 3] = col->a;
				}
			}
		}
	}

	return IL_TRUE;
}


ILboolean DecompressDXT2()
{
	// Can do color & alpha same as dxt3, but color is pre-multiplied 
	//   so the result will be wrong unless corrected. 
	DecompressDXT3();
	CorrectPreMult();

	return IL_FALSE;
}


ILboolean DecompressDXT3()
{
	int			x, y, i, j, k, Select;
	ILubyte		*Temp;
	Color565	*color_0, *color_1;
	Color8888	colours[4], *col;
	ILuint		bitmask, Offset;
	ILushort	word;
	DXTAlphaBlockExplicit *alpha;


	Temp = CompData;
	for (y = 0; y < Height; y += 4) {
		for (x = 0; x < Width; x += 4) {
			alpha = (DXTAlphaBlockExplicit*)Temp;
			Temp += 8;
			color_0 = ((Color565*)Temp);
			color_1 = ((Color565*)(Temp+2));
			bitmask = ((ILuint*)Temp)[1];
			Temp += 8;

			colours[0].r = color_0->nRed << 3;
			colours[0].g = color_0->nGreen << 2;
			colours[0].b = color_0->nBlue << 3;
			colours[0].a = 0xFF;

			colours[1].r = color_1->nRed << 3;
			colours[1].g = color_1->nGreen << 2;
			colours[1].b = color_1->nBlue << 3;
			colours[1].a = 0xFF;

			// Four-color block: derive the other two colors.    
			// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
			// These 2-bit codes correspond to the 2-bit fields 
			// stored in the 64-bit block.
			colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
			colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
			colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
			colours[2].a = 0xFF;

			colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
			colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
			colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
			colours[3].a = 0xFF;

			k = 0;
			for (j = 0; j < 4; j++) {
				for (i = 0; i < 4; i++, k++) {

					Select = (bitmask & (0x03 << k*2)) >> k*2;
					col = &colours[Select];

					Offset = (y + j) * Image->Bps + (x + i) * Image->Bpp;
					Image->Data[Offset + 0] = col->r;
					Image->Data[Offset + 1] = col->g;
					Image->Data[Offset + 2] = col->b;
				}
			}

			for (j = 0; j < 4; j++) {
				word = alpha->row[j];
				for (i = 0; i < 4; i++) {
					Offset = (y + j) * Image->Bps + (x + i) * Image->Bpp + 3;
					Image->Data[Offset] = word & 0x0F;
					Image->Data[Offset] = Image->Data[Offset] | (Image->Data[Offset] << 4);
					word >>= 4;
				}
			}

		}
	}

	return IL_TRUE;
}


ILboolean DecompressDXT4()
{
	// Can do color & alpha same as dxt5, but color is pre-multiplied 
	//   so the result will be wrong unless corrected. 
	DecompressDXT5();
	CorrectPreMult();

	return IL_FALSE;
}


ILboolean DecompressDXT5()
{
	int			x, y, i, j, k, Select;
	ILubyte		*Temp;
	Color565	*color_0, *color_1;
	Color8888	colours[4], *col;
	ILuint		bitmask, Offset;
	ILubyte		alphas[8], *alphamask;
	ILuint		bits;


	Temp = CompData;
	for (y = 0; y < Height; y += 4) {
		for (x = 0; x < Width; x += 4) {
			alphas[0] = Temp[0];
			alphas[1] = Temp[1];
			alphamask = Temp + 2;
			Temp += 8;
			color_0 = ((Color565*)Temp);
			color_1 = ((Color565*)(Temp+2));
			bitmask = ((ILuint*)Temp)[1];
			Temp += 8;

			colours[0].r = color_0->nRed << 3;
			colours[0].g = color_0->nGreen << 2;
			colours[0].b = color_0->nBlue << 3;
			colours[0].a = 0xFF;

			colours[1].r = color_1->nRed << 3;
			colours[1].g = color_1->nGreen << 2;
			colours[1].b = color_1->nBlue << 3;
			colours[1].a = 0xFF;

			// Four-color block: derive the other two colors.    
			// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
			// These 2-bit codes correspond to the 2-bit fields 
			// stored in the 64-bit block.
			colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
			colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
			colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
			colours[2].a = 0xFF;

			colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
			colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
			colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
			colours[3].a = 0xFF;

			k = 0;
			for (j = 0; j < 4; j++) {
				for (i = 0; i < 4; i++, k++) {

					Select = (bitmask & (0x03 << k*2)) >> k*2;
					col = &colours[Select];

					Offset = (y + j) * Image->Bps + (x + i) * Image->Bpp;
					Image->Data[Offset + 0] = col->r;
					Image->Data[Offset + 1] = col->g;
					Image->Data[Offset + 2] = col->b;
				}
			}

			// 8-alpha or 6-alpha block?    
			if (alphas[0] > alphas[1]) {    
				// 8-alpha block:  derive the other six alphas.    
				// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
				alphas[2] = (6 * alphas[0] + 1 * alphas[1] + 3) / 7;	// bit code 010
				alphas[3] = (5 * alphas[0] + 2 * alphas[1] + 3) / 7;	// bit code 011
				alphas[4] = (4 * alphas[0] + 3 * alphas[1] + 3) / 7;	// bit code 100
				alphas[5] = (3 * alphas[0] + 4 * alphas[1] + 3) / 7;	// bit code 101
				alphas[6] = (2 * alphas[0] + 5 * alphas[1] + 3) / 7;	// bit code 110
				alphas[7] = (1 * alphas[0] + 6 * alphas[1] + 3) / 7;	// bit code 111  
			}    
			else {  
				// 6-alpha block.    
				// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
				alphas[2] = (4 * alphas[0] + 1 * alphas[1] + 2) / 5;	// Bit code 010
				alphas[3] = (3 * alphas[0] + 2 * alphas[1] + 2) / 5;	// Bit code 011
				alphas[4] = (2 * alphas[0] + 3 * alphas[1] + 2) / 5;	// Bit code 100
				alphas[5] = (1 * alphas[0] + 4 * alphas[1] + 2) / 5;	// Bit code 101
				alphas[6] = 0x00;										// Bit code 110
				alphas[7] = 0xFF;										// Bit code 111
			}

			// Note: Have to separate the next two loops,
			//	it operates on a 6-byte system.

			// First three bytes
			bits = *((ILint*)alphamask);
			for (j = 0; j < 2; j++) {
				for (i = 0; i < 4; i++) {
					Offset = (y + j) * Image->Bps + (x + i) * Image->Bpp + 3;
					Image->Data[Offset] = alphas[bits & 0x07];
					bits >>= 3;
				}
			}

			// Last three bytes
			bits = *((ILint*)&alphamask[3]);
			for (j = 2; j < 4; j++) {
				for (i = 0; i < 4; i++) {
					Offset = (y + j) * Image->Bps + (x + i) * Image->Bpp + 3;
					Image->Data[Offset] = alphas[bits & 0x07];
					bits >>= 3;
				}
			}


		}
	}

	return IL_TRUE;
}


ILvoid CorrectPreMult()
{
	static ILuint i;

	for (i = 0; i < Image->SizeOfData; i += 4) {
		Image->Data[i]   = (ILubyte)(((ILuint)Image->Data[i]   << 8) / Image->Data[i+3]);
		Image->Data[i+1] = (ILubyte)(((ILuint)Image->Data[i+1] << 8) / Image->Data[i+3]);
		Image->Data[i+2] = (ILubyte)(((ILuint)Image->Data[i+2] << 8) / Image->Data[i+3]);
	}

	return;
}


ILboolean DecompressARGB()
{
	ILuint	i, ReadI, RedL, RedR, GreenL, GreenR, BlueL, BlueR, AlphaL, AlphaR;
	ILubyte	*Temp;

	GetBitsFromMask(Head.RBitMask, &RedL, &RedR);
	GetBitsFromMask(Head.GBitMask, &GreenL, &GreenR);
	GetBitsFromMask(Head.BBitMask, &BlueL, &BlueR);
	GetBitsFromMask(Head.RGBAlphaBitMask, &AlphaL, &AlphaR);
	Temp = CompData;

	for (i = 0; i < Image->SizeOfData; i += Image->Bpp) {
		ReadI = *((ILuint*)Temp);
		Temp += (Head.RGBBitCount / 8);

		Image->Data[i]   = ((ReadI & Head.RBitMask) >> RedR) << RedL;
		Image->Data[i+1] = ((ReadI & Head.GBitMask) >> GreenR) << GreenL;
		Image->Data[i+2] = ((ReadI & Head.BBitMask) >> BlueR) << BlueL;

		if (Image->Bpp == 4) {
			Image->Data[i+3] = ((ReadI & Head.RGBAlphaBitMask) >> AlphaR) << AlphaL;
			if (AlphaL >= 7) {
				Image->Data[i+3] = Image->Data[i+3] ? 0xFF : 0x00;
			}
			else if (AlphaL >= 4) {
				Image->Data[i+3] = Image->Data[i+3] | (Image->Data[i+3] >> 4);
			}
		}
	}

	return IL_TRUE;
}


ILvoid GetBitsFromMask(ILuint Mask, ILuint *ShiftLeft, ILuint *ShiftRight)
{
	ILuint Temp, i;

	if (Mask == 0) {
		*ShiftLeft = *ShiftRight = 0;
		return;
	}

	Temp = Mask;
	for (i = 0; i < 32; i++, Temp >>= 1) {
		if (Temp & 1)
			break;
	}
	*ShiftRight = i;

	// Temp is preserved, so use it again:
	for (i = 0; i < 8; i++, Temp >>= 1) {
		if (!(Temp & 1))
			break;
	}
	*ShiftLeft = 8 - i;

	return;
}


#endif//IL_NO_DDS
