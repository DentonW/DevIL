//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods (this file by thakis)
// Last modified: 12/06/2006
//
// Filename: src-IL/src/il_bmp.c
//
// Description: Reads a RADIANCE High Dynamic Range Image
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_HDR
#include "il_hdr.h"
#include "il_endian.h"

//! Checks if the file specified in FileName is a valid .hdr file.
ILboolean ilIsValidHdr(ILconst_string FileName)
{
	ILHANDLE	HdrFile;
	ILboolean	bHdr = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("hdr"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bHdr;
	}

	HdrFile = iopenr(FileName);
	if (HdrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bHdr;
	}

	bHdr = ilIsValidHdrF(HdrFile);
	icloser(HdrFile);

	return bHdr;
}


//! Checks if the ILHANDLE contains a valid .hdr file at the current position.
ILboolean ilIsValidHdrF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidHdr();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid .hdr lump.
ILboolean ilIsValidHdrL(const ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidHdr();
}


// Internal function used to get the .hdr header from the current file.
ILboolean iGetHdrHead(HDRHEADER *Header)
{
	ILboolean done = IL_FALSE;
	char a, b;
	char x[3], y[3]; //changed 20050217: added space for the '\0' char
	char buff[80];
	ILuint count = 0;

	iread(Header->Signature, 1, 10);

	//skip lines until an empty line is found.
	//this marks the end of header information,
	//the next line contains the image's dimension.

	//TODO: read header contents into variables
	//(EXPOSURE, orientation, xyz correction, ...)

	if (iread(&a, 1, 1) != 1)
		return IL_FALSE;

	while(!done) {
		if (iread(&b, 1, 1) != 1)
			return IL_FALSE;
		if (b == '\n' && a == '\n')
			done = IL_TRUE;
		else
			a = b;
	}

	//read dimensions (note that this assumes a somewhat valid image)
	if (iread(&a, 1, 1) != 1)
		return IL_FALSE;
	while (a != '\n') {
		buff[count] = a;
		if (iread(&a, 1, 1) != 1)
			return IL_FALSE;
		++count;
	}
	buff[count] = '\0';

	//note that this is not the 100% correct way to load hdr images:
	//in a perfect world we would check if there's a +/- X/Y in front
	//of width and heigth and mirror + rotate the image after decoding
	//according to that. But HDRShop doesn't do that either (and that's
	//my reference program :) ) and it's just a rotate and a mirror,
	//nothing that really changes the appearance of the loaded image...
	//(The code as it is now assumes that y contains "-Y" and x contains
	//"+X" after the following line)
	//Furthermore, this crashes if the read strings are longer than 2 chars o_O
	sscanf(buff, "%s %d %s %d", y, &Header->Height, x, &Header->Width);

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidHdr()
{
	char	Head[10];
	ILint	Read;

	Read = iread(Head, 1, 10);
	iseek(-Read, IL_SEEK_CUR);  // Go ahead and restore to previous state
	if (Read != 10)
		return IL_FALSE;

	return
		strnicmp(Head, "#?RADIANCE", 10) == 0
		|| strnicmp(Head, "#?RGBE", 6) == 0;
}


// Internal function used to check if the HEADER is a valid .hdr header.
ILboolean iCheckHdr(HDRHEADER *Header)
{
	return
		strnicmp(Header->Signature, "#?RADIANCE", 10) == 0
		|| strnicmp(Header->Signature, "#?RGBE", 6) == 0;
}


//! Reads a .hdr file
ILboolean ilLoadHdr(ILconst_string FileName)
{
	ILHANDLE	HdrFile;
	ILboolean	bHdr = IL_FALSE;

	HdrFile = iopenr(FileName);
	if (HdrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bHdr;
	}

	bHdr = ilLoadHdrF(HdrFile);
	icloser(HdrFile);

	return bHdr;
}


//! Reads an already-opened .hdr file
ILboolean ilLoadHdrF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadHdrInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .hdr
ILboolean ilLoadHdrL(const ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadHdrInternal();
}


// Internal function used to load the .hdr.
ILboolean iLoadHdrInternal()
{
	HDRHEADER	Header;
	ILfloat *data;
	ILubyte *scanline;
	ILuint i, j, e, r, g, b;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetHdrHead(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	if (!iCheckHdr(&Header)) {
		//iseek(-(ILint)sizeof(BMPHEAD), IL_SEEK_CUR);
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	// Update the current image with the new dimensions
	if (!ilTexImage(Header.Width, Header.Height, 1, 3, IL_RGB, IL_FLOAT, NULL)) {
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	//read image data
	if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST)
		iPreCache(iCurImage->Width / 8 * iCurImage->Height);

	data = (ILfloat*)iCurImage->Data;
	scanline = (ILubyte*)ialloc(Header.Width*4);
	for (i = 0; i < Header.Height; ++i) {
		ReadScanline(scanline, Header.Width);

		//convert hdrs internal format to floats
		for (j = 0; j < 4*Header.Width; j += 4) {
			ILuint *ee;
			ILfloat t, *ff;
			e = scanline[j + 3];
			r = scanline[j + 0];
			g = scanline[j + 1];
			b = scanline[j + 2];

			//t = (float)pow(2.f, ((ILint)e) - 128);
			if (e != 0)
				e = (e - 1) << 23;
			
			// All this just to avoid stric-aliasing warnings...
			// was: t = *(ILfloat*)&e
			ee = &e;
			ff = (ILfloat*)ee;
			t = *ff;
			
			data[0] = (r/255.0f)*t;
			data[1] = (g/255.0f)*t;
			data[2] = (b/255.0f)*t;
			data += 3;
		}
	}
	iUnCache();
	ifree(scanline);

	return ilFixImage();
}

ILvoid ReadScanline(ILubyte *scanline, ILuint w) {
	ILubyte *runner;
	ILuint r, g, b, e, read, shift;

	r = igetc();
	g = igetc();
	b = igetc();
	e = igetc();

	//check if the scanline is in the new format
	//if so, e, r, g, g are stored separated and are
	//rle-compressed independently.
	if (r == 2 && g == 2) {
		ILuint length = (b << 8) | e;
		ILuint j, t, k;
		if (length > w)
			length = w; //fix broken files
		for (k = 0; k < 4; ++k) {
			runner = scanline + k;
			j = 0;
			while (j < length) {
				t = igetc();
				if (t > 128) { //Run?
					ILubyte val = igetc();
					t &= 127;
					//copy current byte
					while (t > 0 && j < length) {
						*runner = val;
						runner += 4;
						--t;
						++j;
					}
				}
				else { //No Run.
					//read new bytes
					while (t > 0 && j < length) {
						*runner = igetc();
						runner += 4;
						--t;
						++j;
					}
				}
			}
		}
		return; //done decoding a scanline in separated format
	}

	//if we come here, we are dealing with old-style scanlines
	shift = 0;
	read = 0;
	runner = scanline;
	while (read < w) {
		if (read != 0) {
			r = igetc();
			g = igetc();
			b = igetc();
			e = igetc();
		}

		//if all three mantissas are 1, then this is a rle
		//count dword
		if (r == 1 && g == 1 && b == 1) {
			ILuint length = e;
			ILuint j;
			for (j = length << shift; j > 0 && read < w; --j) {
				memcpy(runner, runner - 4, 4);
				runner += 4;
				++read;
			}
			//if more than one rle count dword is read
			//consecutively, they are higher order bytes
			//of the first read value. shift keeps track of
			//that.
			shift += 8;
		}
		else {
			runner[0] = r;
			runner[1] = g;
			runner[2] = b;
			runner[3] = e;

			shift = 0;
			runner += 4;
			++read;
		}
	}
}



#endif//IL_NO_HDR
