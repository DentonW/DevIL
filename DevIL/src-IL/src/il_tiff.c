//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 12/16/2001 <--Big Endian Changes =]
//
// Filename: openil/tiff.c
//
// Description: Tiff (.tif) functions
//
//-----------------------------------------------------------------------------


// No way I know of with libtiff to use already-opened files or memory lumps...

#include "il_internal.h"
#ifndef IL_NO_TIF
#include <tiffio.h>
#include <time.h>
#include "il_manip.h"

#define MAGIC_HEADER1	0x4949
#define MAGIC_HEADER2	0x4D4D

/*----------------------------------------------------------------------------*/

// No need for a separate header
ILboolean	iLoadTiffInternal(ILvoid);
char*		iMakeString(ILvoid);
TIFF*		iTIFFOpen();

/*----------------------------------------------------------------------------*/

ILboolean ilisValidTiffExtension(const ILstring FileName)
{
	if (!iCheckExtension((ILstring)FileName, IL_TEXT("tif")) &&
		!iCheckExtension((ILstring)FileName, IL_TEXT("tiff")))
		return IL_FALSE;
	else
		return IL_TRUE;
}

/*----------------------------------------------------------------------------*/

//! Checks if the file specified in FileName is a valid tiff file.
ILboolean ilIsValidTiff(const ILstring FileName)
{
	ILHANDLE	TiffFile;
	ILboolean	bTiff = IL_FALSE;

	if (!ilisValidTiffExtension((ILstring) FileName))
	{
		ilSetError(IL_INVALID_EXTENSION);
		return bTiff;
	}

	TiffFile = iopenr((ILstring)FileName);
	if (TiffFile == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTiff;
	}

	bTiff = ilIsValidTiffF(TiffFile);
	icloser(TiffFile);

	return bTiff;
}

/*----------------------------------------------------------------------------*/

ILboolean ilisValidTiffFunc(ILushort Header1)
{
	ILushort Header2;
	
	if (Header1 != MAGIC_HEADER1 && Header1 != MAGIC_HEADER2)
		return IL_FALSE;

	if (Header1 == MAGIC_HEADER1)
		Header2 = GetLittleUShort();
	else
		Header2 = GetBigUShort();

	if (Header2 != 42)
		return IL_FALSE;

	return IL_TRUE;
	
}

/*----------------------------------------------------------------------------*/

//! Checks if the ILHANDLE contains a valid tiff file at the current position.
ILboolean ilIsValidTiffF(ILHANDLE File)
{
	ILushort Header1;

	iread(&Header1, sizeof(ILushort), 1);
	
	return ilisValidTiffFunc(Header1);
}

/*----------------------------------------------------------------------------*/

//! Checks if Lump is a valid Tiff lump.
ILboolean ilIsValidTiffL(ILvoid *Lump, ILuint Size)
{
    ILushort Header1;

	iSetInputLump(Lump, Size);

    Header1 = GetLittleUShort();
    
	return ilisValidTiffFunc(Header1);
}

/*----------------------------------------------------------------------------*/

//! Reads a Tiff file
ILboolean ilLoadTiff(const ILstring FileName)
{
	ILHANDLE	TiffFile;
	ILboolean	bTiff = IL_FALSE;

	TiffFile = iopenr(FileName);
	if (TiffFile == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
	}
	else
	{
		bTiff = ilLoadTiffF(TiffFile);
		icloser(TiffFile);
	}
	
	return bTiff;
}

/*----------------------------------------------------------------------------*/

//! Reads an already-opened Tiff file
ILboolean ilLoadTiffF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadTiffInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}

/*----------------------------------------------------------------------------*/

//! Reads from a memory "lump" that contains a Tiff
ILboolean ilLoadTiffL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadTiffInternal();
}

/*----------------------------------------------------------------------------*/

// Internal function used to load the Tiff.
ILboolean iLoadTiffInternal()
{
	TIFF		*tif;
	ILushort	bpp;
	ILushort	*sampleinfo, extrasamples;
	ILuint		ixPixel;
	ILubyte		*pImageData;
	ILuint		ProfileLen;
	ILvoid		*Buffer;
	
	if (iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

    tif = iTIFFOpen();
    if (tif == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &iCurImage->Width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &iCurImage->Height);
	TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL, &bpp);

	if (bpp == 4)
	{
		TIFFGetFieldDefaulted(	tif, TIFFTAG_EXTRASAMPLES, &extrasamples,
								&sampleinfo); 
		if (!sampleinfo || 
			 sampleinfo[0] == EXTRASAMPLE_UNSPECIFIED)
		{
			static ILushort si = EXTRASAMPLE_ASSOCALPHA;
			TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, 1, &si);
		}
	}

	if (!ilTexImage(iCurImage->Width, iCurImage->Height, 1, 4, IL_RGBA,
					IL_UNSIGNED_BYTE, NULL))
	{
		TIFFClose(tif);
		return IL_FALSE;
	}

	// Siigron: added u_long cast to shut up compiler warning
    if (!TIFFReadRGBAImage( tif, iCurImage->Width, iCurImage->Height,
							(uint32*) iCurImage->Data, 1))
	{
		TIFFClose(tif);
		ilSetError(IL_LIB_TIFF_ERROR);
		return IL_FALSE;
	}

	if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &ProfileLen, &Buffer)) {
		if (iCurImage->Profile && iCurImage->ProfileSize)
			ifree(iCurImage->Profile);
		iCurImage->Profile = (ILubyte*)ialloc(ProfileLen);
		if (iCurImage->Profile == NULL) {
			ilSetError(IL_OUT_OF_MEMORY);
			TIFFClose(tif);
			return IL_FALSE;
		}

		memcpy(iCurImage->Profile, Buffer, ProfileLen);
		iCurImage->ProfileSize = ProfileLen;

		_TIFFfree(Buffer);
	}

	TIFFClose(tif);

	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;  // eiu...dunno if this is right

	switch (bpp)
	{
		case 1:
#ifdef __LITTLE_ENDIAN__
			ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
#endif			
			break;

		case 3:
#ifdef __LITTLE_ENDIAN__
			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
#endif			
			break; 

		case 4:
			pImageData = iCurImage->Data;
				
#ifdef __LITTLE_ENDIAN__
			pImageData += 3;
#endif			
				
			for (	ixPixel=iCurImage->Width*iCurImage->Height;
					ixPixel>0; 
					ixPixel--)
			{
				*pImageData ^= 255;
				pImageData += 4;
			}
			break;
	}

	//iCurImage->SwapEndian = IL_TRUE;
	ilFixImage();

	return IL_TRUE;
}

/*----------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
// Extension to load tiff files from memory
// Marco Fabbricatore (fabbrica@ai-lab.fh-furtwangen.de)
/////////////////////////////////////////////////////////////////////////////////////////

static tsize_t
_tiffFileReadProc(thandle_t fd, tdata_t pData, tsize_t tSize)
{
	return iread(pData, 1, tSize);
}

/*----------------------------------------------------------------------------*/

static tsize_t
_tiffFileWriteProc(thandle_t fd, tdata_t pData, tsize_t tSize)
{
	TIFFWarning("TIFFMemFile", "_tiffFileWriteProc() Not implemented");
	return(0);
}

/*----------------------------------------------------------------------------*/

static toff_t
_tiffFileSeekProc(thandle_t fd, toff_t tOff, int whence)
{
	/* we use this as a special code, so avoid accepting it */
	if (tOff == 0xFFFFFFFF)
		return 0xFFFFFFFF;

	iseek(tOff, whence);
	return tOff;
}

/*----------------------------------------------------------------------------*/

static int
_tiffFileCloseProc(thandle_t fd)
{
   	return (0);
}

/*----------------------------------------------------------------------------*/

static toff_t
_tiffFileSizeProc(thandle_t fd)
{
	ILint Offset, Size;
	Offset = itell();
	iseek(0, IL_SEEK_END);
	Size = itell();
	iseek(Offset, IL_SEEK_SET);

	return Size;
}

/*----------------------------------------------------------------------------*/

#ifdef __BORLANDC__
#pragma argsused
#endif
static int
_tiffDummyMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	return 0;
}

/*----------------------------------------------------------------------------*/

#ifdef __BORLANDC__
#pragma argsused
#endif
static void
_tiffDummyUnmapProc(thandle_t fd, tdata_t base, toff_t size)
{
	return;
}

/*----------------------------------------------------------------------------*/

TIFF *iTIFFOpen()
{
    TIFF *tif;

	tif = TIFFClientOpen("TIFFMemFile", "r",
						 NULL,
					    _tiffFileReadProc, _tiffFileWriteProc,
					    _tiffFileSeekProc, _tiffFileCloseProc,
						_tiffFileSizeProc, _tiffDummyMapProc,
						_tiffDummyUnmapProc);

    return tif;
}

/*----------------------------------------------------------------------------*/

// @TODO:  Accept palettes!
ILboolean ilSaveTiff(const char *FileName)
{
	ILenum	Format;
	ILpal	*TempPal = NULL;
	ILenum	Compression;
	ILuint	ixLine;
	TIFF	*File;
	char	Description[512];

	if (iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (FileName == NULL || strlen(FileName) < 5)
	{
		ilSetError(IL_INVALID_VALUE);
		return IL_FALSE;
	}

	if (!ilisValidTiffExtension((ILstring) FileName))
	{
		ilSetError(IL_INVALID_EXTENSION);
		return IL_FALSE;
	}

	if (ilGetBoolean(IL_FILE_MODE) == IL_FALSE)
	{
		if (iFileExists((ILstring)FileName))
		{
			ilSetError(IL_FILE_ALREADY_EXISTS);
			return IL_FALSE;
		}
	}

	if (iGetHint(IL_COMPRESSION_HINT) == IL_USE_COMPRESSION)
		Compression = COMPRESSION_PACKBITS;
	else
		Compression = COMPRESSION_NONE;

	File = TIFFOpen(FileName, "w");
	if (File == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	sprintf(Description, "Tiff generated by %s", ilGetString(IL_VERSION));

	TIFFSetField(File, TIFFTAG_IMAGEWIDTH, iCurImage->Width);
	TIFFSetField(File, TIFFTAG_IMAGELENGTH, iCurImage->Height);
	TIFFSetField(File, TIFFTAG_COMPRESSION, Compression);
	TIFFSetField(File, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(File, TIFFTAG_BITSPERSAMPLE, iCurImage->Bpc << 3);
	TIFFSetField(File, TIFFTAG_SAMPLESPERPIXEL, iCurImage->Bpp);
	TIFFSetField(File, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(File, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(File, TIFFTAG_SOFTWARE, ilGetString(IL_VERSION));
	TIFFSetField(File, TIFFTAG_DOCUMENTNAME,
						iGetString(IL_TIF_DOCUMENTNAME_STRING) ?
						iGetString(IL_TIF_DOCUMENTNAME_STRING) : FileName);
	if (iGetString(IL_TIF_AUTHNAME_STRING))
		TIFFSetField(File, TIFFTAG_ARTIST, iGetString(IL_TIF_AUTHNAME_STRING));
	if (iGetString(IL_TIF_HOSTCOMPUTER_STRING))
		TIFFSetField(File, TIFFTAG_HOSTCOMPUTER,
					iGetString(IL_TIF_HOSTCOMPUTER_STRING));
	if (iGetString(IL_TIF_DESCRIPTION_STRING))
		TIFFSetField(File, TIFFTAG_IMAGEDESCRIPTION,
					iGetString(IL_TIF_DESCRIPTION_STRING));
	TIFFSetField(File, TIFFTAG_DATETIME, iMakeString());
	
	TIFFSetField(File, TIFFTAG_ORIENTATION,
		iCurImage->Origin == IL_ORIGIN_UPPER_LEFT ? ORIENTATION_TOPLEFT :
													 ORIENTATION_BOTLEFT);

	Format = iCurImage->Format;
	if (Format == IL_BGR || Format == IL_BGRA)
		ilSwapColours();

	for (ixLine = 0; ixLine < iCurImage->Height; ++ixLine)
	{
		if (TIFFWriteScanline(	File, iCurImage->Data + ixLine * iCurImage->Bps,
								ixLine, 0) < 0)
		{
			TIFFClose(File);
			ilSetError(IL_LIB_TIFF_ERROR);
			return IL_FALSE;
		}
	}

	if (Format == IL_BGR || Format == IL_BGRA)
		ilSwapColours();

	TIFFClose(File);

	return IL_TRUE;
}

/*----------------------------------------------------------------------------*/

// Makes a neat date string for the date field.
char *iMakeString()
{
	static char TimeStr[255];
	time_t		Time;
	struct tm	*CurTime;

	memset(TimeStr, 0, 255);

	time(&Time);
#ifdef _WIN32
	_tzset();
#endif
	CurTime = localtime(&Time);

	strftime(TimeStr, 255, "%#c (%z)", CurTime);

	return TimeStr;
}

/*----------------------------------------------------------------------------*/

#endif//IL_NO_TIF
