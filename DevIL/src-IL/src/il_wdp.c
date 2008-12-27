//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 12/14/2008
//
// Filename: src-IL/src/il_wdp.c
//
// Description: Reads a Microsoft HD Photo (.wdp or .hdp)
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_WDP
#include "il_wdp.h"


//! Checks if the file specified in FileName is a valid Wdp file.
ILboolean ilIsValidWdp(ILconst_string FileName)
{
	ILHANDLE	WdpFile;
	ILboolean	bWdp = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("wdp")) &&
		!iCheckExtension(FileName, IL_TEXT("hdp"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bWdp;
	}
	
	WdpFile = iopenr(FileName);
	if (WdpFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bWdp;
	}
	
	bWdp = ilIsValidWdpF(WdpFile);
	icloser(WdpFile);
	
	return bWdp;
}


//! Checks if the ILHANDLE contains a valid Wdp file at the current position.
ILboolean ilIsValidWdpF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidWdp();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid Wdp lump.
ILboolean ilIsValidWdpL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidWdp();
}


// Internal function used to get the Wdp header from the current file.
ILboolean iGetWdpHead(WDPHEAD *Header)
{
	// Must be "II"
	Header->Encoding[0] = (ILubyte)igetc();
	Header->Encoding[1] = (ILubyte)igetc();
	// Must be 0xBC
	Header->UniqueID = (ILubyte)igetc();
	// Currently 1
	Header->Version = (ILubyte)igetc();
	// Offset from the beginning of the file
	Header->Offset = GetLittleUInt();
	
	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidWdp()
{
	WDPHEAD	Head;
	
	if (!iGetWdpHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(WDPHEAD), IL_SEEK_CUR);
	
	return iCheckWdp(&Head);
}


// Internal function used to check if the HEADER is a valid Wdp header.
ILboolean iCheckWdp(WDPHEAD *Header)
{
	if (Header->Encoding[0] != 'I' || Header->Encoding[1] != 'I')
		return IL_FALSE;
	if (Header->UniqueID != 0xBC)
		return IL_FALSE;
	// Currently 1... 0 is prerelease.
	if (Header->Version != 1)
		return IL_FALSE;
	
	return IL_TRUE;
}


//! Reads a WDP file
ILboolean ilLoadWdp(ILconst_string FileName)
{
	ILHANDLE	WdpFile;
	ILboolean	bWdp = IL_FALSE;

	WdpFile = iopenr(FileName);
	if (WdpFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bWdp;
	}

	bWdp = ilLoadWdpF(WdpFile);
	icloser(WdpFile);

	return bWdp;
}


//! Reads an already-opened WDP file
ILboolean ilLoadWdpF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadWdpInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a WDP image
ILboolean ilLoadWdpL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadWdpInternal();
}


// Internal function used to load the Wdp.
ILboolean iLoadWdpInternal()
{
	WDPHEAD		Header;
	WDPIFD		Dir;
	WDPGUID		Guid;
	ILushort	NumEntries;
	WDPIMGHEAD	ImgHead;
	WDPIMGPLANE	ImgPlane;
	WDPDCQUANT	DcQuant;
	WDPTILE		Tile;
	ILuint		TempInt, TempInt1, i;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetWdpHead(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	if (!iCheckWdp(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	// Offset is from the beginning of the file.
	iseek(Header.Offset, IL_SEEK_SET);

	NumEntries = GetLittleUShort();

	Dir.Tag = GetLittleUShort();
	Dir.Type = GetLittleUShort();
	Dir.Count = GetLittleUInt();
	Dir.ValOff = GetLittleUInt();
	Dir.NextOff = GetLittleUInt();

	GetLittleUInt();

	iseek(Dir.ValOff, IL_SEEK_SET);
	if (Dir.Type == 1) {
		//iseek(Dir.Count * 1, IL_SEEK_CUR);
//@TODO: Take care of other types.

		Guid.First = GetLittleUInt();
		Guid.Second = GetLittleUShort();
		Guid.Third = GetLittleUShort();
		Guid.Fourth = GetLittleUInt();
		Guid.Fifth = GetLittleUInt();
	}

	// Support other formats
	if (Guid.First != 0x6fddc324 || Guid.Second != 0x4e03 || Guid.Third != 0x4bfe ||
		Guid.Fourth != 0x773d85b1) {
		ilSetError(IL_ILLEGAL_FILE_VALUE);
		return IL_FALSE;
	}

	// Read the image header.
	//@TODO: Take care of more formats
	switch (Guid.Fifth)
	{
		// 8-bit RGB (24 bpp)
		case 0x0cc98d76:
			ImgHead.GDISignature[0] = GetLittleUInt();
			ImgHead.GDISignature[1] = GetLittleUInt();
			ImgHead.Codec    = (ILubyte)igetc();
			ImgHead.Flags[0] = (ILubyte)igetc();
			ImgHead.Flags[1] = (ILubyte)igetc();
			ImgHead.Format   = (ILubyte)igetc();

			// GUID is "WMPHOTO", and codec version is only 1 in the current spec (1.0).
			if (ImgHead.GDISignature[0] != 0x48504D57 || ImgHead.GDISignature[1] != 0x004F544F ||
				((ImgHead.Codec & WDP_CODEC) >> 4) != 1) {
				ilSetError(IL_ILLEGAL_FILE_VALUE);
				return IL_FALSE;
			}

			//@TODO: Take care of more formats.
			if (!(ImgHead.Format & WDP_RGB)) {
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}
			//@TODO: Take care of extra pixels.
			if (ImgHead.Flags[1] & WDP_WINDOWING) {
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}
			//@TODO: Alpha channel
			if (ImgHead.Flags[1] & WDP_ALPHACHANNEL) {
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}

			if (ImgHead.Flags[1] & WDP_SHORT_HEADER) {
				ImgHead.Width  = GetBigUShort() + 1;
				ImgHead.Height = GetBigUShort() + 1;
			} else {
				ImgHead.Width  = GetBigUInt() + 1;
				ImgHead.Height = GetBigUInt() + 1;
			}

			if (ImgHead.Flags[0] & WDP_TILING_FLAG) {
				ImgHead.VertTiles = (ILubyte)igetc();
				TempInt = igetc();
				ImgHead.VertTiles = ImgHead.VertTiles + ((TempInt & 0xF0) << 4);
				UInt(&ImgHead.VertTiles);  // Take care of endianness

				ImgHead.HorzTiles = (ILubyte)igetc() << 8;
				ImgHead.HorzTiles = ImgHead.VertTiles + (TempInt & 0x0F);
				UInt(&ImgHead.VertTiles);  // Take care of endianness

				//@TODO:  Interpret tiles
				return IL_FALSE;

				for (i = 0; i < ImgHead.VertTiles; i++) {
					if (ImgHead.Flags[1] & WDP_SHORT_HEADER) {
						igetc();
					}
					else {
						GetBigUShort();
					}
				}
			}

			//@TODO: Take orientation into account.

			// Read the image plane header
			ImgPlane.Flags1 = (ILubyte)igetc();
			TempInt = (ImgPlane.Flags1 & WDP_CLR_FMT)>>5;
			if ((TempInt == WDP_YUV_444) || (TempInt == WDP_N_CHANNEL)) {
				//@TODO: Do anything with this for YUV_444?
				ImgPlane.Color = (ILubyte)igetc();
			}

			if (TempInt == WDP_N_CHANNEL) {
				ImgPlane.NumChannels = ((ImgPlane.Color & WDP_NUM_CHANS) >> 4) + 1;
			}
			else if (TempInt == WDP_Y_ONLY) {
				ImgPlane.NumChannels = 1;
			}
			else if ((TempInt == WDP_YUV_420) || (TempInt == WDP_YUV_422) || (TempInt == WDP_YUV_444)) {
				ImgPlane.NumChannels = 3;
			}
			else if ((TempInt == WDP_CMYK) || (TempInt == WDP_BAYER)) {
				ImgPlane.NumChannels = 4;
			}
			else {  // The above if statements have all the possible values.
				ilSetError(IL_ILLEGAL_FILE_VALUE);
				return IL_FALSE;
			}


			//@TODO: Implement
			//if ((ImgPlane.Flags1 & WDP_CLR_FMT)>>5 == WDP_YUV_444) {
			//	ilSetError(IL_FORMAT_NOT_SUPPORTED);
			//	return IL_FALSE;
			//}
			//@TODO: Implement
			if ((ImgPlane.Flags1 & WDP_CLR_FMT)>>5 == WDP_BAYER) {
				ImgPlane.Bayer = (ILubyte)igetc();
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}

			TempInt = ImgHead.Format & WDP_BITDEPTH;
			//@TODO: Implement
			if (TempInt == WDP_BD_16 || TempInt == WDP_BD_16S || TempInt == WDP_BD_32 || TempInt == WDP_BD_32S) {
				ImgPlane.ShiftBits = (ILubyte)igetc();
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}
			//@TODO: Implement
			if (TempInt == WDP_BD_32F) {
				ImgPlane.Mantissa = (ILubyte)igetc();
				ImgPlane.Expbias  = (ILubyte)igetc();
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}

			// Read the second set of image plane header flags.
			ImgPlane.Flags2 = (ILubyte)igetc();
			//@TODO: Implement
			if (ImgPlane.Flags2 & WDP_DC_FRAME) {
				if (ImgPlane.NumChannels == 1) {
					DcQuant.ChMode = WDP_CH_UNIFORM;
				}
				else {
					DcQuant.ChMode = (ImgPlane.Flags2 & 0x60) >> 5;
				}

				if (DcQuant.ChMode == WDP_CH_UNIFORM) {
					TempInt = igetc();
					DcQuant.DcQuant = ((ImgPlane.Flags2 & 0x1F) << 3) + ((TempInt & 0xE0) >> 5);
				}
				else if (DcQuant.ChMode == WDP_CH_SEPARATE) {
					TempInt1 = igetc();
					TempInt = igetc();
					DcQuant.DcQuantY = ((ImgPlane.Flags2 & 0x1F) << 3) + ((TempInt1 & 0xE0) >> 5);
					DcQuant.DcQuantUV = ((TempInt1 & 0x1F) << 3) + ((TempInt & 0xE0) >> 5);
				}
				//@TODO: Implement
				else if (DcQuant.ChMode == WDP_CH_INDEPENDENT) {
					ilSetError(IL_FORMAT_NOT_SUPPORTED);
					return IL_FALSE;
				}
				else {  // Not a valid channel mode
					ilSetError(IL_ILLEGAL_FILE_VALUE);
					return IL_FALSE;
				}
			}
			else {
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}

			// Bitstream is in spatial mode.
			if ((ImgHead.Flags[0] & WDP_BITSTREAM_FMT) == 0) {
//@TODO: Find out what exactly these are.
				iseek(6, IL_SEEK_CUR);
				Tile.StartCode = (igetc() << 16) + (igetc() << 8) + igetc();
				UInt(&Tile.StartCode);
				Tile.HashAndType = igetc();

				if (Tile.StartCode != 1 && (Tile.HashAndType & WDP_TILE_TYPE) != WDP_FLEXBITS_TILE) {  // Invalid tile
					ilSetError(IL_ILLEGAL_FILE_VALUE);
					return IL_FALSE;
				}

				//@TODO: Implement
				if (ImgHead.Flags[1] & WDP_TRIM_FLEXBITS) {
					ilSetError(IL_FORMAT_NOT_SUPPORTED);
					return IL_FALSE;
				}

				// Here's where TILE_SPATIAL goes.
				//ImgPlane.Flags1 & WDP_BANDS_PRESENT  // No need to shift, since lower.
			}
			// Bitstream is in frequency mode.
			//@TODO: Implement this.
			else {
				ilSetError(IL_FORMAT_NOT_SUPPORTED);
				return IL_FALSE;
			}



			if (!ilTexImage(ImgHead.Width, ImgHead.Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL)) {
				//@TODO: Free any memory from above.
				return IL_FALSE;
			}


			break;

		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	

	return IL_TRUE;
}


ILuint VLWESC()
{
	ILubyte	First, Second;
	ILuint	Value;

	First = (ILubyte)igetc();
	if (First < 0xfb) {
		Second = (ILubyte)igetc();
		Value = First * 256 + Second;
	}
	else if (First == 0x0fb) {
		Value = GetBigUInt();
	}
    //else if (First == 0x0fc) {
    //    EIGHT_BYTES
    //    Value = EIGHT_BYTES
    //}
	else { // FIRST_BYTE == 0xfd||0xfe||0xff 
		Value = 0; // Escape Mode
	}

	return Value;
}



#endif//IL_NO_WDP
