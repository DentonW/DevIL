//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/13/2002 <--Y2K Compliant! =]
//
// Filename: il/il_gif.c
//
// Description: Reads from a Graphics Interchange Format (.gif) file.
//
//  This code is based on code released to the public domain by Javier Arevalo
//    that can be found at http://www.programmersheaven.com/zone10/cat452/
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_GIF

#include "il_gif.h"


//! Checks if the file specified in FileName is a valid Gif file.
ILboolean ilIsValidGif(const ILstring FileName)
{
	ILHANDLE	GifFile;
	ILboolean	bGif = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("gif"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bGif;
	}

	GifFile = iopenr(FileName);
	if (GifFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bGif;
	}

	bGif = ilIsValidGifF(GifFile);
	icloser(GifFile);

	return bGif;
}


//! Checks if the ILHANDLE contains a valid Gif file at the current position.
ILboolean ilIsValidGifF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidGif();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid Gif lump.
ILboolean ilIsValidGifL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidGif();
}


// Internal function to get the header and check it.
ILboolean iIsValidGif()
{
	char Header[6];
	
	iread(Header, 1, 6);
	iseek(-6, IL_SEEK_CUR);

	if (!strnicmp(Header, "GIF87A", 6))
		return IL_TRUE;
	if (!strnicmp(Header, "GIF89A", 6))
		return IL_TRUE;

	return IL_FALSE;
}


//! Reads a Gif file
ILboolean ilLoadGif(const ILstring FileName)
{
	ILHANDLE	GifFile;
	ILboolean	bGif = IL_FALSE;

	GifFile = iopenr(FileName);
	if (GifFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bGif;
	}

	bGif = ilLoadGifF(GifFile);
	icloser(GifFile);

	return bGif;
}


//! Reads an already-opened Gif file
ILboolean ilLoadGifF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadGifInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Gif
ILboolean ilLoadGifL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadGifInternal();
}

ILubyte *Data;

// Internal function used to load the Gif.
ILboolean iLoadGifInternal()
{
	GIFHEAD		Header;
	IMAGEDESC	ImageDesc;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iIsValidGif())
		return IL_FALSE;

	iread(&Header, sizeof(Header), 1);

	ilTexImage(Header.Width, Header.Height, 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL);
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	// Check for a global colour map.
	if (Header.ColourInfo & (1 << 7)) {
		iCurImage->Pal.PalSize = (1 << ((Header.ColourInfo & 0x7) + 1)) * 3;
		iCurImage->Pal.PalType = IL_PAL_RGB24;
		iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
		if (iCurImage->Pal.Palette == NULL)
			return IL_FALSE;
		iread(iCurImage->Pal.Palette, 1, iCurImage->Pal.PalSize);
	}

	iread(&ImageDesc, sizeof(ImageDesc), 1);

	Data = iCurImage->Data;
	if (!GifGetData())
		return IL_FALSE;

	ilFixImage();

	return IL_TRUE;
}


#define MAX_CODES 4096

ILint	curr_size, clear, ending, newcodes, top_slot, slot, navail_bytes = 0, nbits_left = 0;
ILubyte	b1;
ILubyte	byte_buff[257];
ILubyte	*pbytes;
ILubyte	*stack;
ILubyte	*suffix;
ILshort	*prefix;

static unsigned long code_mask[13] =
{
   0L,
   0x0001L, 0x0003L,
   0x0007L, 0x000FL,
   0x001FL, 0x003FL,
   0x007FL, 0x00FFL,
   0x01FFL, 0x03FFL,
   0x07FFL, 0x0FFFL
};


ILint get_next_code(void)
{
	ILint  i;
	static ILuint ret;

	if (!nbits_left) {
		if (navail_bytes <= 0) {
			pbytes = byte_buff;
			navail_bytes = igetc();
			if (navail_bytes) {
				for (i = 0; i < navail_bytes; i++) {
					byte_buff[i] = igetc();
				}
			}
		}
		b1 = *pbytes++;
		nbits_left = 8;
		navail_bytes--;
	}

	ret = b1 >> (8 - nbits_left);
	while (curr_size > nbits_left) {
		if (navail_bytes <= 0) {
			pbytes = byte_buff;
			navail_bytes = igetc();
			if (navail_bytes) {
				for (i = 0; i < navail_bytes; i++) {
					byte_buff[i] = igetc();
				}
			}
		}
		b1 = *pbytes++;
		ret |= b1 << nbits_left;
		nbits_left += 8;
		navail_bytes--;
	}
	nbits_left -= curr_size;

	return (ret & code_mask[curr_size]);
}


ILboolean GifGetData()
{
	ILubyte	*sp;
	ILint	code, fc, oc;
	ILubyte	size;
	ILint	c;

	size = igetc();
	if (size < 2 || 9 < size) {
		return IL_FALSE;
	}

	stack  = (ILbyte*)ialloc(MAX_CODES + 1);
	suffix = (ILbyte*)ialloc(MAX_CODES + 1);
	prefix = (ILshort*)ialloc(sizeof(*prefix) * (MAX_CODES + 1));
	if (!stack || !suffix || !prefix)
		return IL_FALSE;

	curr_size = size + 1;
	top_slot = 1 << curr_size;
	clear = 1 << size;
	ending = clear + 1;
	slot = newcodes = ending + 1;
	navail_bytes = nbits_left = 0;
	oc = fc = 0;
	sp = stack;

	while ((c = get_next_code()) != ending) {
		if (c == clear) {
			curr_size = size + 1;
			slot = newcodes;
			top_slot = 1 << curr_size;
			while ((c = get_next_code()) == clear);
			if (c == ending)
				break;
			if (c >= slot)
				c = 0;
			oc = fc = c;
			*Data++ = c;
		}
		else {
			code = c;
			if (code >= slot) {
				code = oc;
				*sp++ = fc;
			}
			while (code >= newcodes) {
				*sp++ = suffix[code];
				code = prefix[code];
			}
			*sp++ = (ILbyte)code;
			if (slot < top_slot) {
				fc = code;
				suffix[slot]   = fc;
				prefix[slot++] = oc;
				oc = c;
			}
			if (slot >= top_slot && curr_size < 12) {
				top_slot <<= 1;
				curr_size++;
			}
			while (sp > stack) {
				sp--;
				*Data++ = *sp;
			}
		}
	}

	ifree(stack);
	ifree(suffix);
	ifree(prefix);

	return IL_TRUE;
}

#endif //IL_NO_GIF
