//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/pic.c
//
// Description: Softimage Pic (.pic) functions
//
//-----------------------------------------------------------------------------

// @TODO:  Test these extensively...haven't even been tested yet!!!

#include "il_internal.h"
#ifndef IL_NO_PIC
#include "il_pic.h"
#include "il_manip.h"
#include <string.h>


//! Checks if the file specified in FileName is a valid .pic file.
ILboolean ilIsValidPic(const ILstring FileName)
{
	ILHANDLE	PicFile;
	ILboolean	bPic = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("pic"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bPic;
	}

	PicFile = iopenr(FileName);
	if (PicFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPic;
	}

	bPic = ilIsValidPicF(PicFile);
	icloser(PicFile);

	return bPic;
}


//! Checks if the ILHANDLE contains a valid .pic file at the current position.
ILboolean ilIsValidPicF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidPic();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid .pic lump.
ILboolean ilIsValidPicL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidPic();
}


// Internal function used to get the .pic header from the current file.
ILvoid iGetPicHead(PIC_HEAD *Header)
{
	Header->Magic = GetLittleInt();
	iread(&Header->Version, sizeof(ILfloat), 1);  // wtf?  A float?!
	iread(&Header->Comment, 1, 80);
	iread(&Header->Id, 1, 4);
	Header->Width = GetLittleShort();
	Header->Height = GetLittleShort();
	iread(&Header->Ratio, sizeof(ILfloat), 1);
	Header->Fields = GetLittleShort();
	Header->Padding = GetLittleShort();

	return;
}


// Internal function to get the header and check it.
ILboolean iIsValidPic()
{
	PIC_HEAD	Head;

	iGetPicHead(&Head);
	iseek(-(ILint)sizeof(PIC_HEAD), IL_SEEK_CUR);  // Go ahead and restore to previous state

	return iCheckPic(&Head);
}


// Internal function used to check if the header is a valid .pic header.
ILboolean iCheckPic(PIC_HEAD *Header)
{
	if (Header->Magic != 0x5380F634)
		return IL_FALSE;
	if (strncmp(Header->Id, "PICT", 4))
		return IL_FALSE;
	if (Header->Width == 0)
		return IL_FALSE;
	if (Header->Height == 0)
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads a .pic file
ILboolean ilLoadPic(const ILstring FileName)
{
	ILHANDLE	PicFile;
	ILboolean	bPic = IL_FALSE;

	PicFile = iopenr(FileName);
	if (PicFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPic;
	}

	bPic = ilLoadPicF(PicFile);
	icloser(PicFile);

	return bPic;
}


//! Reads an already-opened .pic file
ILboolean ilLoadPicF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPicInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .pic
ILboolean ilLoadPicL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadPicInternal();
}


// Internal function used to load the .pic
ILboolean iLoadPicInternal()
{
	ILuint		Alpha = IL_FALSE;
	ILubyte		Chained;
	CHANNEL		*Channel = NULL, *Channels, *Prev;
	PIC_HEAD	Header;
	ILboolean	Read;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iGetPicHead(&Header);
	if (!iCheckPic(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	if (!ilTexImage(Header.Width, Header.Height, 1, 1, 0, IL_UNSIGNED_BYTE, NULL)) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Read channels
	do {
		if (Channel == NULL)
			Channel = Channels = (CHANNEL*)ialloc(sizeof(CHANNEL));
		else {
			Channels->Next = (CHANNEL*)ialloc(sizeof(CHANNEL));
			Channels = Channels->Next;
		}
		Channels->Next = NULL;

		Chained = igetc();
		Channels->Size = igetc();
		Channels->Type = igetc();
		Channels->Chan = igetc();
		
		// See if we have an alpha channel in there
		if (Channels->Chan & PIC_ALPHA_CHANNEL)
			Alpha = IL_TRUE;
		
	} while (Chained);

	Read = readScanlines((ILuint*)iCurImage->Data, Header.Width, Header.Height, Channel, Alpha);

	// Destroy channels
	while (Channel) {
		Prev = Channel;
		Channel = Channel->Next;
		ifree(Prev);
	}

	if (Read == IL_FALSE) {
		ilSetError(IL_ILLEGAL_FILE_VALUE);
		return IL_FALSE;
	}

	ilFixImage();

	return IL_TRUE;
}


static ILuint readScanlines(ILuint *image,
							 ILint width, ILint height, 
							 CHANNEL *channel, ILuint alpha)
{
	ILint i;
	
	(ILvoid)alpha;
	
	for (i = height - 1; i >= 0; i--) {
		ILuint *scan = image + i * width;
		
		if (!readScanline((ILubyte *)scan, width, channel, 4))
			return IL_FALSE;
	}
	
	return IL_TRUE;
}

static ILuint readScanline(ILubyte *scan, ILint width, CHANNEL *channel, ILint bytes)
{
	ILint		noCol, status;
	ILint		off[4];
	
	while (channel) {
		noCol = 0;
//#ifndef sgi
		if(channel->Chan & PIC_RED_CHANNEL) {
			off[noCol] = 0;
			noCol++;
		}
		if(channel->Chan & PIC_GREEN_CHANNEL) {
			off[noCol] = 1;
			noCol++;
		}
		if(channel->Chan & PIC_BLUE_CHANNEL) {
			off[noCol] = 2;
			noCol++;
		}
		if(channel->Chan & PIC_ALPHA_CHANNEL) {
			off[noCol] = 3;
			noCol++;
		}
/*#else
		if(channel->channels & PIC_RED_CHANNEL) {
			off[noCol] = 3;
			noCol++;
		}
		if(channel->channels & PIC_GREEN_CHANNEL) {
			off[noCol] = 2;
			noCol++;
		}
		if(channel->channels & PIC_BLUE_CHANNEL) {
			off[noCol] = 1;
			noCol++;
		}
		if(channel->channels & PIC_ALPHA_CHANNEL) {
			off[noCol] = 0;
			noCol++;
		}
#endif*/
		
		switch(channel->Type & 0x0F) {
			case PIC_UNCOMPRESSED:
				status = channelReadRaw(scan, width, noCol, off, bytes);
				break;
			case PIC_PURE_RUN_LENGTH:
				status = channelReadPure(scan, width, noCol, off, bytes);
				break;
			case PIC_MIXED_RUN_LENGTH:
				status = channelReadMixed(scan, width, noCol, off, bytes);
				break;
		}
		if (!status)
			break;
		
		channel = channel->Next;
	}
	return status;
}

static ILuint channelReadRaw(ILubyte *scan, ILint width, ILint noCol, ILint *off, ILint bytes)
{
	int i, j;
	
	for (i = 0; i < width; i++) {
		if (ieof())
			return IL_FALSE;
		for (j = 0; j < noCol; j++)
			scan[off[j]] = (ILubyte)igetc();
		scan += bytes;
	}
	return IL_TRUE;
}

static ILuint channelReadPure(ILubyte *scan, ILint width, ILint noCol, ILint *off, ILint bytes)
{
	ILubyte		col[4];
	ILint		count;
	int			i, j, k;
	
	for (i = width; i > 0; ) {
		count = (unsigned char)igetc();
		if (count > width)
			count = width;
		i -= count;
		
		if (ieof())
			return IL_FALSE;
		
		for (j = 0; j < noCol; j++)
			col[j] = (ILubyte)igetc();
		
		for (k = 0; k < count; k++, scan += bytes) {
			for(j = 0; j < noCol; j++)
				scan[off[j] + k] = col[j];
		}
	}
	return IL_TRUE;
}

static ILuint channelReadMixed(ILubyte *scan, ILint width, ILint noCol, ILint *off, ILint bytes)
{
	ILint	count;
	int		i, j, k;
	ILubyte	col[4];
	
	for(i = 0; i < width; i += count) {
		if (ieof())
			return IL_FALSE;
		
		count = (ILubyte)igetc();
		
		if (count >= 128) {		// Repeated sequence
			if (count == 128)	// Long run
				iread(&count, 2, 1);
			else
				count -= 127;
			
			// We've run past...
			if ((i + count) > width) {
				fprintf(stderr, "ERROR: FF_PIC_load(): Overrun scanline (Repeat) [%d + %d > %d] (NC=%d)\n", i, count, width, noCol);
				return IL_FALSE;
			}
			
			for (j = 0; j < noCol; j++)
				col[j] = (ILubyte)igetc();
			
			for (k = 0; k < count; k++, scan += bytes) {
				for (j = 0; j < noCol; j++)
					scan[off[j]] = col[j];
			}
		} else {				// Raw sequence
			count++;
			if ((i + count) > width) {
				fprintf(stderr, "ERROR: FF_PIC_load(): Overrun scanline (Raw) [%d + %d > %d] (NC=%d)\n", i, count, width, noCol);
				return IL_FALSE;
			}
			
			for (k = count; k > 0; k--, scan += bytes) {
				for (j = 0; j < noCol; j++)
					scan[off[j]] = (ILubyte)igetc();
			}
		}
	}

	return IL_TRUE;
}




#endif//IL_NO_PIC
