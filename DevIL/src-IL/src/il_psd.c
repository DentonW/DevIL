//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 01/29/2002 <--Y2K Compliant! =]
//
// Filename: openil/il_psd.c
//
// Description: Reads from a PhotoShop (.psd) file.
//
//-----------------------------------------------------------------------------

// Information about the .psd format was taken from Adobe's PhotoShop SDK at
//  http://partners.adobe.com/asn/developer/gapsdk/PhotoshopSDK.html
//  Information about the Packbits compression scheme was found at
//	http://partners.adobe.com/asn/developer/PDFS/TN/TIFF6.pdf

#include "il_internal.h"
#ifndef IL_NO_PSD
#include "il_psd.h"


//! Checks if the file specified in FileName is a valid Psd file.
ILboolean ilIsValidPsd(const ILstring FileName)
{
	ILHANDLE	PsdFile;
	ILboolean	bPsd = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("psd"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bPsd;
	}

	PsdFile = iopenr(FileName);
	if (PsdFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPsd;
	}

	bPsd = ilIsValidPsdF(PsdFile);
	icloser(PsdFile);

	return bPsd;
}


//! Checks if the ILHANDLE contains a valid Psd file at the current position.
ILboolean ilIsValidPsdF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidPsd();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid Psd lump.
ILboolean ilIsValidPsdL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidPsd();
}


// Internal function used to get the Psd header from the current file.
ILvoid iGetPsdHead(PSDHEAD *Header)
{
	iread(Header->Signature, 1, 4);
	Header->Version = GetBigUShort();
	iread(Header->Reserved, 1, 6);
	Header->Channels = GetBigUShort();
	Header->Height = GetBigUInt();
	Header->Width = GetBigUInt();
	Header->Depth = GetBigUShort();
	Header->Mode = GetBigUShort();

	return;
}


// Internal function to get the header and check it.
ILboolean iIsValidPsd()
{
	PSDHEAD	Head;

	iGetPsdHead(&Head);
	iseek(-(ILint)sizeof(PSDHEAD), IL_SEEK_CUR);

	return iCheckPsd(&Head);
}


// Internal function used to check if the HEADER is a valid Psd header.
ILboolean iCheckPsd(PSDHEAD *Header)
{
	ILuint i;

	if (strncmp(Header->Signature, "8BPS", 4))
		return IL_FALSE;
	if (Header->Version != 1)
		return IL_FALSE;
	for (i = 0; i < 6; i++) {
		if (Header->Reserved[i] != 0)
			return IL_FALSE;
	}
	if (Header->Channels < 1 || Header->Channels > 24)
		return IL_FALSE;
	if (Header->Height < 1 || Header->Width < 1)
		return IL_FALSE;
	if (Header->Depth != 1 && Header->Depth != 8 && Header->Depth != 16)
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads a Psd file
ILboolean ilLoadPsd(const ILstring FileName)
{
	ILHANDLE	PsdFile;
	ILboolean	bPsd = IL_FALSE;

	PsdFile = iopenr(FileName);
	if (PsdFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPsd;
	}

	bPsd = ilLoadPsdF(PsdFile);
	icloser(PsdFile);

	return bPsd;
}


//! Reads an already-opened Psd file
ILboolean ilLoadPsdF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPsdInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Psd
ILboolean ilLoadPsdL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadPsdInternal();
}


// Internal function used to load the Psd.
ILboolean iLoadPsdInternal()
{
	PSDHEAD		Header;
	ILboolean	bPsd;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iGetPsdHead(&Header);
	if (!iCheckPsd(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	bPsd = ReadPsd(&Header);
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	ilFixImage();

	return bPsd;
}


ILboolean ReadPsd(PSDHEAD *Head)
{
	switch (Head->Mode)
	{
		case 1:  // Greyscale
			return ReadGrey(Head);
		case 2:  // Indexed
			return ReadIndexed(Head);
		case 3:  // RGB
			return ReadRGB(Head);
		case 4:  // CMYK
			return ReadCMYK(Head);
	}

	ilSetError(IL_FORMAT_NOT_SUPPORTED);
	return IL_FALSE;
}


ILboolean ReadGrey(PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo;
	ILushort	Compressed;
	ILenum		Type;
	ILubyte		*Resources;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	iseek(ColorMode, IL_SEEK_CUR);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iread(Resources, 1, ResourceSize);

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	Channels = Head->Channels;
	Head->Channels = 1;  // Temporary to read only one channel...some greyscale .psd files have 2.
	if (Head->Channels != 1) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	switch (Head->Depth)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_UNSIGNED_SHORT;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	if (!ilTexImage(Head->Width, Head->Height, 1, 1, IL_LUMINANCE, Type, NULL))
		return IL_FALSE;

	if (!GetData(Head, iCurImage->Data, (ILboolean)Compressed))
		return IL_FALSE;

	ParseResources(ResourceSize, Resources);
	ifree(Resources);

	return IL_TRUE;
}


ILboolean ReadIndexed(PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo, i, j, NumEnt;
	ILushort	Compressed;
	ILubyte		*Palette, *Resources;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	if (ColorMode % 3 != 0) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	Palette = (ILubyte*)ialloc(ColorMode);
	if (Palette == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iread(Palette, 1, ColorMode);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iread(Resources, 1, ResourceSize);

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	if (Head->Channels != 1 || Head->Depth != 8) {
		ifree(Palette);
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	Channels = Head->Channels;

	if (!ilTexImage(Head->Width, Head->Height, 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;

	iCurImage->Pal.Palette = (ILubyte*)ialloc(ColorMode);
	if (iCurImage->Pal.Palette == NULL) {
		ifree(Palette);
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iCurImage->Pal.PalSize = ColorMode;
	iCurImage->Pal.PalType = IL_PAL_RGB24;

	NumEnt = iCurImage->Pal.PalSize / 3;
	for (i = 0, j = 0; i < iCurImage->Pal.PalSize; i += 3, j++) {
		iCurImage->Pal.Palette[i  ] = Palette[j];
		iCurImage->Pal.Palette[i+1] = Palette[j+NumEnt];
		iCurImage->Pal.Palette[i+2] = Palette[j+NumEnt*2];
	}
	ifree(Palette);

	if (!GetData(Head, iCurImage->Data, (ILboolean)Compressed))
		return IL_FALSE;

	ParseResources(ResourceSize, Resources);
	ifree(Resources);

	return IL_TRUE;
}


ILboolean ReadRGB(PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo;
	ILushort	Compressed;
	ILenum		Format, Type;
	ILubyte		*Resources;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	iseek(ColorMode, IL_SEEK_CUR);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iread(Resources, 1, ResourceSize);

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	Channels = Head->Channels;
	switch (Head->Channels)
	{
		case 3:
			Format = IL_RGB;
			break;
		case 4:
			Format = IL_RGBA;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	switch (Head->Depth)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_UNSIGNED_SHORT;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	if (!ilTexImage(Head->Width, Head->Height, 1, (ILubyte)Head->Channels, Format, Type, NULL))
		return IL_FALSE;

	if (!GetData(Head, iCurImage->Data, (ILboolean)Compressed))
		return IL_FALSE;

	ParseResources(ResourceSize, Resources);
	ifree(Resources);

	return IL_TRUE;
}


ILboolean ReadCMYK(PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo, Size, i, j;
	ILushort	Compressed;
	ILenum		Format, Type;
	ILubyte		*Resources, *KChannel;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	iseek(ColorMode, IL_SEEK_CUR);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iread(Resources, 1, ResourceSize);

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	switch (Head->Channels)
	{
		case 4:
			Format = IL_RGB;
			Channels = 4;
			Head->Channels = 3;
			break;
		case 5:
			Format = IL_RGBA;
			Channels = 5;
			Head->Channels = 4;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	switch (Head->Depth)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_UNSIGNED_SHORT;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	if (!ilTexImage(Head->Width, Head->Height, 1, (ILubyte)Head->Channels, Format, Type, NULL))
		return IL_FALSE;

	if (!GetData(Head, iCurImage->Data, (ILboolean)Compressed))
		return IL_FALSE;

	Size = iCurImage->Bpc * iCurImage->Width * iCurImage->Height;
	KChannel = (ILubyte*)ialloc(Size);
	if (KChannel == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	if (!GetSingleChannel(Head, KChannel, (ILboolean)Compressed)) {
		ifree(KChannel);
		return IL_FALSE;
	}

	if (Format == IL_RGB) {
		for (i = 0, j = 0; i < iCurImage->SizeOfData; i += 3, j++) {
			iCurImage->Data[i  ] = (iCurImage->Data[i  ] * KChannel[j]) >> 8;
			iCurImage->Data[i+1] = (iCurImage->Data[i+1] * KChannel[j]) >> 8;
			iCurImage->Data[i+2] = (iCurImage->Data[i+2] * KChannel[j]) >> 8;
		}
	}
	else {  // IL_RGBA
		// The KChannel array really holds the alpha channel on this one.
		for (i = 0, j = 0; i < iCurImage->SizeOfData; i += 4, j++) {
			iCurImage->Data[i  ] = (iCurImage->Data[i  ] * iCurImage->Data[i+3]) >> 8;
			iCurImage->Data[i+1] = (iCurImage->Data[i+1] * iCurImage->Data[i+3]) >> 8;
			iCurImage->Data[i+2] = (iCurImage->Data[i+2] * iCurImage->Data[i+3]) >> 8;
			iCurImage->Data[i+3] = KChannel[j];  // Swap 'K' with alpha channel.
		}
	}

	ParseResources(ResourceSize, Resources);
	ifree(Resources);
	ifree(KChannel);

	return IL_FALSE;
}


ILuint *GetCompChanLen(PSDHEAD *Head)
{
	ILushort	*RleTable;
	ILuint		*ChanLen, c, i, j;

	RleTable = (ILushort*)ialloc(Head->Height * Channels * sizeof(ILushort));
	ChanLen = (ILuint*)ialloc(Channels * sizeof(ILuint));
	if (RleTable == NULL || ChanLen == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return NULL;
	}

	iread(RleTable, sizeof(ILushort), Head->Height * Channels);
#ifdef __LITTLE_ENDIAN__
	for (i = 0; i < Head->Height * Channels; i++) {
		RleTable[i] = SwapShort(RleTable[i]);
	}
#endif

	memset(ChanLen, 0, Channels * sizeof(ILuint));
	for (c = 0; c < Channels; c++) {
		j = c * Head->Height;
		for (i = 0; i < Head->Height; i++) {
			ChanLen[c] += RleTable[i + j];
		}
	}

	ifree(RleTable);

	return ChanLen;
}


ILboolean GetData(PSDHEAD *Head, ILvoid *Buffer, ILboolean Compressed)
{
	ILuint		c, x, y, i;
	ILubyte		*Channel;
	ILushort	*ShortPtr;
	ILbyte		HeadByte;
	ILubyte		Run;
	ILuint		*ChanLen;
	ILboolean	PreCache = IL_FALSE;

	Channel = (ILubyte*)ialloc(Head->Width * Head->Height * iCurImage->Bpc);
	if (Channel == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	ShortPtr = (ILushort*)Channel;

	// @TODO: Add support for this in, though I have yet to run across a .psd
	//	file that uses this.
	if (Compressed && iCurImage->Type == IL_UNSIGNED_SHORT) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}

	if (!Compressed) {
		if (iCurImage->Bpc == 1) {
			for (c = 0; c < Head->Channels; c++) {
				i = 0;
				iread(Channel, 1, Head->Width * Head->Height);
				for (y = 0; y < Head->Height * iCurImage->Bps; y += iCurImage->Bps) {
					for (x = 0; x < iCurImage->Bps; x += iCurImage->Bpp, i++) {
						iCurImage->Data[y + x + c] = Channel[i];
					}
				}
			}
		}
		else {  // iCurImage->Bpc == 2
			for (c = 0; c < Head->Channels; c++) {
				i = 0;
				iread(Channel, 1, Head->Width * Head->Height * 2);
				iCurImage->Bps /= 2;
				for (y = 0; y < Head->Height * iCurImage->Bps; y += iCurImage->Bps) {
					for (x = 0; x < iCurImage->Bps; x += iCurImage->Bpp, i++) {
						((ILushort*)iCurImage->Data)[y + x + c] = ShortPtr[i];
					}
				}
				iCurImage->Bps *= 2;
			}
		}
	}
	else {
		ChanLen = GetCompChanLen(Head);
		if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST)
			PreCache = IL_TRUE;

		for (c = 0; c < Head->Channels; c++) {
			if (PreCache)
				iPreCache(ChanLen[c]);
			for (i = 0; i < Head->Width * Head->Height; ) {
				HeadByte = igetc();

				if (HeadByte >= 0) {  //  && HeadByte <= 127
					iread(Channel + i, 1, HeadByte + 1);
					i += HeadByte + 1;
				}
				if (HeadByte >= -127 && HeadByte <= -1) {
					Run = igetc();
					memset(Channel + i, Run, -HeadByte + 1);
					i += -HeadByte + 1;
				}
				if (HeadByte == -128)
				{ }  // Noop
			}
			if (PreCache)
				iUnCache();

			i = 0;
			for (y = 0; y < Head->Height * iCurImage->Bps; y += iCurImage->Bps) {
				for (x = 0; x < iCurImage->Bps; x += iCurImage->Bpp, i++) {
					iCurImage->Data[y + x + c] = Channel[i];
				}
			}
		}

		ifree(ChanLen);
	}

	ifree(Channel);

	return IL_TRUE;
}


ILboolean ParseResources(ILuint ResourceSize, ILubyte *Resources)
{
	ILushort	ID;
	ILubyte		NameLen;
	ILuint		Size;

	if (Resources == NULL) {
		ilSetError(IL_INTERNAL_ERROR);
		return IL_FALSE;
	}

	while (ResourceSize) {
		if (strncmp("8BIM", (ILbyte*)Resources, 4)) {
			return IL_FALSE;
		}
		Resources += 4;

		ID = *((ILushort*)Resources);
		ID = BigUShort(ID);
		Resources += 2;

		NameLen = *Resources++;
		// NameLen + the byte it occupies must be padded to an even number, so NameLen must be odd.
		NameLen = NameLen + (NameLen & 1 ? 0 : 1);
		Resources += NameLen;

		// Get the resource data size.
		Size = *((ILuint*)Resources);
		Size = BigUInt(Size);
		Resources += 4;

		ResourceSize -= (4 + 2 + 1 + NameLen);

		switch (ID)
		{
			case 0x040F:  // ICC Profile
				iCurImage->Profile = (ILubyte*)ialloc(Size);
				if (iCurImage->Profile == NULL) {
					ilSetError(IL_OUT_OF_MEMORY);
					return IL_FALSE;
				}
				memcpy(iCurImage->Profile, Resources, Size);
				iCurImage->ProfileSize = Size;
				break;

			default:
				break;
		}

		if (Size & 1)  // Must be an even number.
			Size++;
		ResourceSize -= Size;
		Resources += Size;
	}

	return IL_TRUE;
}


ILboolean GetSingleChannel(PSDHEAD *Head, ILubyte *Buffer, ILboolean Compressed)
{
	ILuint		i;
	ILushort	*ShortPtr;
	ILbyte		HeadByte;
	ILubyte		Run;

	ShortPtr = (ILushort*)Buffer;

	if (!Compressed) {
		if (iCurImage->Bpc == 1) {
			iread(Buffer, 1, Head->Width * Head->Height);
		}
		else {  // iCurImage->Bpc == 2
			iread(Buffer, 1, Head->Width * Head->Height * 2);
		}
	}
	else {
		for (i = 0; i < Head->Width * Head->Height; ) {
			HeadByte = igetc();

			if (HeadByte >= 0) {  //  && HeadByte <= 127
				iread(Buffer + i, 1, HeadByte + 1);
				i += HeadByte + 1;
			}
			if (HeadByte >= -127 && HeadByte <= -1) {
				Run = igetc();
				memset(Buffer + i, Run, -HeadByte + 1);
				i += -HeadByte + 1;
			}
			if (HeadByte == -128)
			{ }  // Noop
		}
	}

	return IL_TRUE;
}

#endif//IL_NO_PSD
