//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/21/2001 <--Y2K Compliant! =]
//
// Filename: openil/io.c
//
// Description: Determines image types and loads images
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#include "il_register.h"
#include "il_pal.h"
#include <string.h>


ILAPI ILenum ILAPIENTRY ilTypeFromExt(const ILstring FileName)
{
	ILstring Ext = iGetExtension(FileName);

#ifndef _WIN32_WCE
	if (FileName == NULL || strlen(FileName) < 1) {
#else
	if (FileName == NULL || wcslen(FileName) < 1) {
#endif//_WIN32_WCE
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (!iStrCmp(Ext, TEXT("bmp")))
		return IL_BMP;
	if (!iStrCmp(Ext, TEXT("cut")))
		return IL_CUT;
	if (!iStrCmp(Ext, TEXT("dds")))
		return IL_DDS;
	if (!iStrCmp(Ext, TEXT("gif")))
		return IL_GIF;
	if (!iStrCmp(Ext, TEXT("ico")) || !iStrCmp(Ext, TEXT("cur")))
		return IL_ICO;
	if (!iStrCmp(Ext, TEXT("jpg")) || !iStrCmp(Ext, TEXT("jpe")) || !iStrCmp(Ext, TEXT("jpeg")))
		return IL_JPG;
	if (!iStrCmp(Ext, TEXT("jng")))
		return IL_JNG;
	if (!iStrCmp(Ext, TEXT("lif")))
		return IL_LIF;
	if (!iStrCmp(Ext, TEXT("mdl")))
		return IL_MDL;
	if (!iStrCmp(Ext, TEXT("mng")))
		return IL_MNG;
	if (!iStrCmp(Ext, TEXT("pcd")))
		return IL_PCD;
	if (!iStrCmp(Ext, TEXT("pcx")))
		return IL_PCX;
	if (!iStrCmp(Ext, TEXT("pic")))
		return IL_PIC;
	if (!iStrCmp(Ext, TEXT("png")))
		return IL_PNG;
	if (!iStrCmp(Ext, TEXT("pbm")) || !iStrCmp(Ext, TEXT("pgm")) ||
		!iStrCmp(Ext, TEXT("pnm")) || !iStrCmp(Ext, TEXT("ppm")))
		return IL_PNM;
	if (!iStrCmp(Ext, TEXT("sgi")) || !iStrCmp(Ext, TEXT("bw")) ||
		!iStrCmp(Ext, TEXT("rgb")) || !iStrCmp(Ext, TEXT("rgba")))
		return IL_SGI;
	if (!iStrCmp(Ext, TEXT("tga")) || !iStrCmp(Ext, TEXT("vda")) ||
		!iStrCmp(Ext, TEXT("icb")) || !iStrCmp(Ext, TEXT("vst")))
		return IL_TGA;
	if (!iStrCmp(Ext, TEXT("tif")) || !iStrCmp(Ext, TEXT("tiff")))
		return IL_TIF;
	if (!iStrCmp(Ext, TEXT("wal")))
		return IL_WAL;

	return IL_TYPE_UNKNOWN;
}


ILenum ilDetermineTypeF(ILHANDLE File);

ILenum ilDetermineType(const ILstring FileName)
{
	ILHANDLE	File;
	ILenum		Type;

	if (FileName == NULL)
		return IL_TYPE_UNKNOWN;

	// LibTiff only works with filenames, not file streams.
	#ifndef IL_NO_TIF
	if (ilIsValidTiff(FileName))
		return IL_TIF;
	#endif
	#ifndef IL_NO_GIF
	if (ilIsValidGif(FileName))
		return IL_GIF;
	#endif

	File = iopenr(FileName);
	if (File == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}
	Type = ilDetermineTypeF(File);
	icloser(File);

	return Type;
}


ILenum ilDetermineTypeF(ILHANDLE File)
{
	if (File == NULL)
		return IL_TYPE_UNKNOWN;

	#ifndef IL_NO_BMP
	if (ilIsValidBmpF(File))
		return IL_BMP;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidDdsF(File))
		return IL_DDS;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidGifF(File))
		return IL_GIF;
	#endif

	#ifndef IL_NO_JPG
	if (ilIsValidJpgF(File))
		return IL_JPG;
	#endif

	#ifndef IL_NO_LIF
	if (ilIsValidLifF(File))
		return IL_LIF;
	#endif

	#ifndef IL_NO_PCX
	if (ilIsValidPcxF(File))
		return IL_PCX;
	#endif

	#ifndef IL_NO_PIC
	if (ilIsValidPicF(File))
		return IL_PIC;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidPngF(File))
		return IL_PNG;
	#endif

	#ifndef IL_NO_PNM
	if (ilIsValidPnmF(File))
		return IL_PNM;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidSgiF(File))
		return IL_SGI;
	#endif

	#ifndef IL_NO_TGA
	if (ilIsValidTgaF(File))
		return IL_TGA;
	#endif

	return IL_TYPE_UNKNOWN;
}


ILenum ilDetermineTypeL(ILvoid *Lump, ILuint Size)
{
	if (Lump == NULL)
		return IL_TYPE_UNKNOWN;

	#ifndef IL_NO_BMP
	if (ilIsValidBmpL(Lump, Size))
		return IL_BMP;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidDdsL(Lump, Size))
		return IL_DDS;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidGifL(Lump, Size))
		return IL_GIF;
	#endif

	#ifndef IL_NO_JPG
	if (ilIsValidJpgL(Lump, Size))
		return IL_JPG;
	#endif

	#ifndef IL_NO_LIF
	if (ilIsValidLifL(Lump, Size))
		return IL_LIF;
	#endif

	#ifndef IL_NO_PCX
	if (ilIsValidPcxL(Lump, Size))
		return IL_PCX;
	#endif

	#ifndef IL_NO_PIC
	if (ilIsValidPicL(Lump, Size))
		return IL_PIC;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidPngL(Lump, Size))
		return IL_PNG;
	#endif

	#ifndef IL_NO_PNM
	if (ilIsValidPnmL(Lump, Size))
		return IL_PNM;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidSgiL(Lump, Size))
		return IL_SGI;
	#endif

	#ifndef IL_NO_TGA
	if (ilIsValidTgaL(Lump, Size))
		return IL_TGA;
	#endif

	return IL_TYPE_UNKNOWN;
}


ILboolean ILAPIENTRY ilIsValid(ILenum Type, const ILstring FileName)
{
	if (FileName == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmp(FileName);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDds(FileName);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpg(FileName);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidLif(FileName);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidPcx(FileName);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidPic(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPng(FileName);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnm(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgi(FileName);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidTga(FileName);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilIsValidF(ILenum Type, ILHANDLE File)
{
	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmpF(File);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDdsF(File);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpgF(File);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidLifF(File);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidPcxF(File);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidPicF(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPngF(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnmF(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgiF(File);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidTgaF(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilIsValidL(ILenum Type, ILvoid *Lump, ILuint Size)
{
	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmpL(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDdsL(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpgL(Lump, Size);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidLifL(Lump, Size);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidPcxL(Lump, Size);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidPicL(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPngL(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnmL(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgiL(Lump, Size);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidTgaL(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilLoad(ILenum Type, const ILstring FileName)
{
#ifndef _WIN32_WCE
	if (FileName == NULL || strlen(FileName) < 1) {
#else
	char AnsiName[512];  // @TODO:  May want to use MAX_LEN...
	if (FileName == NULL || wcslen(FileName) < 1) {
#endif//_WIN32_WCE
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return ilLoadImage(FileName);

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmp(FileName);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCut(FileName);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDds(FileName);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadDoom(FileName);
		case IL_DOOM_FLAT:
			return ilLoadDoomFlat(FileName);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadGif(FileName);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadIcon(FileName);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilLoadJpeg(FileName);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadLif(FileName);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadMdl(FileName);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadMng(FileName);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadPcd(FileName, 3);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcx(FileName);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPic(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPng(FileName);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnm(FileName);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadRaw(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgi(FileName);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadTarga(FileName);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			#ifndef _WIN32_WCE
				return ilLoadTiff(FileName);
			#else
				WideCharToMultiByte(CP_ACP, 0, FileName, -1, AnsiName, 512, NULL, NULL);
				return ilLoadTiff(AnsiName);
			#endif//_WIN32_WCE
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWal(FileName);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilLoadF(ILenum Type, ILHANDLE File)
{
	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Type == IL_TYPE_UNKNOWN)
		Type = ilDetermineTypeF(File);
	
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return IL_FALSE;

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmpF(File);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCutF(File);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDdsF(File);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadDoomF(File);
		case IL_DOOM_FLAT:
			return ilLoadDoomFlatF(File);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadIconF(File);
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				return ilLoadJpegF(File);
			#endif
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadLifF(File);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadMdlF(File);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadMngF(File);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadPcdF(File, 3);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcxF(File);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPicF(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPngF(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnmF(File);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadRawF(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgiF(File);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadTargaF(File);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWalF(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilLoadL(ILenum Type, ILvoid *Lump, ILuint Size)
{
	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Type == IL_TYPE_UNKNOWN)
		Type = ilDetermineTypeL(Lump, Size);
	
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return IL_FALSE;

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmpL(Lump, Size);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCutL(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDdsL(Lump, Size);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadDoomL(Lump, Size);
		case IL_DOOM_FLAT:
			return ilLoadDoomFlatL(Lump, Size);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadIconL(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilLoadJpegL(Lump, Size);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadLifL(Lump, Size);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadMdlL(Lump, Size);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadMngL(Lump, Size);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadPcdL(Lump, Size, 3);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcxL(Lump, Size);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPicL(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPngL(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnmL(Lump, Size);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadRawL(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgiL(Lump, Size);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadTargaL(Lump, Size);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWalL(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image with various different methods before failing - very generic.
ILboolean ILAPIENTRY ilLoadImage(const ILstring FileName)
{
	ILstring	Ext = iGetExtension(FileName);
	ILenum		Type;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

#ifndef _WIN32_WCE
	if (FileName == NULL || strlen(FileName) < 1) {
#else
	if (FileName == NULL || wcslen(FileName) < 1) {
#endif//_WIN32_WCE
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	// Try registered procedures first (so users can override default lib functions).
	if (Ext) {
		if (iRegisterLoad(FileName))
			return IL_TRUE;

		#ifndef IL_NO_BMP
		if (!iStrCmp(Ext, TEXT("bmp"))) {
			return ilLoadBmp(FileName);
		}
		#endif

		#ifndef IL_NO_CUT
		if (!iStrCmp(Ext, TEXT("cut"))) {
			return ilLoadCut(FileName);
		}
		#endif

		#ifndef IL_NO_DCX
		if (!iStrCmp(Ext, TEXT("dcx"))) {
			return ilLoadDcx(FileName);
		}
		#endif

		#ifndef IL_NO_DDS
		if (!iStrCmp(Ext, TEXT("dds"))) {
			return ilLoadDds(FileName);
		}
		#endif

		#ifndef IL_NO_GIF
		if (!iStrCmp(Ext, TEXT("gif"))) {
			return ilLoadGif(FileName);
		}
		#endif

		#ifndef IL_NO_ICO
		if (!iStrCmp(Ext, TEXT("ico")) || !iStrCmp(Ext, TEXT("cur"))) {
			return ilLoadIcon(FileName);
		}
		#endif

		#ifndef IL_NO_JPG
		if (!iStrCmp(Ext, TEXT("jpg")) || !iStrCmp(Ext, TEXT("jpe")) || !iStrCmp(Ext, TEXT("jpeg"))) {
			return ilLoadJpeg(FileName);
		}
		#endif

		#ifndef IL_NO_LIF
		if (!iStrCmp(Ext, TEXT("lif"))) {
			return ilLoadLif(FileName);
		}
		#endif

		#ifndef IL_NO_MDL
		if (!iStrCmp(Ext, TEXT("mdl"))) {
			return ilLoadMdl(FileName);
		}
		#endif

		#ifndef IL_NO_MNG
		if (!iStrCmp(Ext, TEXT("mng")) || !iStrCmp(Ext, TEXT("jng"))) {
			return ilLoadMng(FileName);
		}
		#endif

		#ifndef IL_NO_PCD
		if (!iStrCmp(Ext, TEXT("pcd"))) {
			return ilLoadPcd(FileName, 3);
		}
		#endif

		#ifndef IL_NO_PCX
		if (!iStrCmp(Ext, TEXT("pcx"))) {
			return ilLoadPcx(FileName);
		}
		#endif

		#ifndef IL_NO_PIC
		if (!iStrCmp(Ext, TEXT("pic"))) {
			return ilLoadPic(FileName);
		}
		#endif

		#ifndef IL_NO_PNG
		if (!iStrCmp(Ext, TEXT("png"))) {
			return ilLoadPng(FileName);
		}
		#endif

		#ifndef IL_NO_PNM
		if (!iStrCmp(Ext, TEXT("pbm"))) {
			return ilLoadPnm(FileName);
		}
		if (!iStrCmp(Ext, TEXT("pgm"))) {
			return ilLoadPnm(FileName);
		}
		if (!iStrCmp(Ext, TEXT("pnm"))) {
			return ilLoadPnm(FileName);
		}
		if (!iStrCmp(Ext, TEXT("ppm"))) {
			return ilLoadPnm(FileName);
		}
		#endif

		#ifndef IL_NO_SGI
		if (!iStrCmp(Ext, TEXT("sgi")) || !iStrCmp(Ext, TEXT("bw")) ||
			!iStrCmp(Ext, TEXT("rgb")) || !iStrCmp(Ext, TEXT("rgba"))) {
			return ilLoadSgi(FileName);
		}
		#endif

		#ifndef IL_NO_TGA
		if (!iStrCmp(Ext, TEXT("tga")) || !iStrCmp(Ext, TEXT("vda")) ||
			!iStrCmp(Ext, TEXT("icb")) || !iStrCmp(Ext, TEXT("vst"))) {
			return ilLoadTarga(FileName);
		}
		#endif

		#ifndef IL_NO_TIF
		if (!iStrCmp(Ext, TEXT("tif")) || !iStrCmp(Ext, TEXT("tiff"))) {
			return ilLoadTiff(FileName);
		}
		#endif

		#ifndef IL_NO_WAL
		if (!iStrCmp(Ext, TEXT("wal"))) {
			return ilLoadWal(FileName);
		}
		#endif
	}

	// As a last-ditch effort, try to identify the image
	Type = ilDetermineType(FileName);
	if (Type == IL_TYPE_UNKNOWN)
		return IL_FALSE;
	return ilLoad(Type, FileName);
}					 


ILboolean ILAPIENTRY ilSave(ILenum Type, const ILstring FileName)
{
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return ilSaveImage(FileName);

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSaveBmp(FileName);
		#endif

		#ifndef IL_NO_CHEAD
		case IL_CHEAD:
			return ilSaveCHeader(FileName, "IL_IMAGE");
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilSaveJpeg(FileName);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilSavePcx(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilSavePng(FileName);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSavePnm(FileName);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilSaveRaw(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSaveSgi(FileName);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSaveTarga(FileName);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilSaveTiff(FileName);
		#endif

		case IL_JASC_PAL:
			return ilSaveJascPal(FileName);
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilSaveF(ILenum Type, ILHANDLE File)
{
	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSaveBmpF(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSavePnmF(File);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilSaveRawF(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSaveSgiF(File);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSaveTargaF(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilSaveL(ILenum Type, ILvoid *Lump, ILuint Size)
{
	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSaveBmpL(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSavePnmL(Lump, Size);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilSaveRawL(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSaveSgiL(Lump, Size);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSaveTargaL(Lump, Size);
		#endif

	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Determines what image type to save based on the extension and attempts to save
//	the current image based on the extension given in FileName.
ILboolean ILAPIENTRY ilSaveImage(const ILstring FileName)
{
	ILstring Ext = iGetExtension(FileName);

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

#ifndef _WIN32_WCE
	if (FileName == NULL || strlen(FileName) < 1 || Ext == NULL) {
#else
	if (FileName == NULL || wcslen(FileName) < 1 || Ext == NULL) {
#endif//_WIN32_WCE
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	#ifndef IL_NO_BMP
	if (!iStrCmp(Ext, TEXT("bmp"))) {
		return ilSaveBmp(FileName);
	}
	#endif

	#ifndef IL_NO_CHEAD
	if (!iStrCmp(Ext, TEXT("h"))) {
		return ilSaveCHeader(FileName, "IL_IMAGE");
	}
	#endif

	#ifndef IL_NO_JPG
	if (!iStrCmp(Ext, TEXT("jpg")) || !iStrCmp(Ext, TEXT("jpeg")) || !iStrCmp(Ext, TEXT("jpe"))) {
		return ilSaveJpeg(FileName);
	}
	#endif

	#ifndef IL_NO_PCX
	if (!iStrCmp(Ext, TEXT("pcx"))) {
		return ilSavePcx(FileName);
	}
	#endif

	#ifndef IL_NO_PNG
	if (!iStrCmp(Ext, TEXT("png"))) {
		return ilSavePng(FileName);
	}
	#endif

	#ifndef IL_NO_PNM  // Not sure if binary or ascii should be defaulted...maybe an option?
	if (!iStrCmp(Ext, TEXT("pbm"))) {
		return ilSavePnm(FileName);
	}
	if (!iStrCmp(Ext, TEXT("pgm"))) {
		return ilSavePnm(FileName);
	}
	if (!iStrCmp(Ext, TEXT("ppm"))) {
		return ilSavePnm(FileName);
	}
	#endif

	#ifndef IL_NO_RAW
	if (!iStrCmp(Ext, TEXT("raw"))) {
		return ilSaveRaw(FileName);
	}
	#endif

	#ifndef IL_NO_SGI
	if (!iStrCmp(Ext, TEXT("sgi")) || !iStrCmp(Ext, TEXT("bw")) ||
		!iStrCmp(Ext, TEXT("rgb")) || !iStrCmp(Ext, TEXT("rgba"))) {
		return ilSaveSgi(FileName);
	}
	#endif

	#ifndef IL_NO_TGA
	if (!iStrCmp(Ext, TEXT("tga"))) {
		return ilSaveTarga(FileName);
	}
	#endif

	#ifndef IL_NO_TIF
	if (!iStrCmp(Ext, TEXT("tif")) || !iStrCmp(Ext, TEXT("tiff"))) {
		return ilSaveTiff(FileName);
	}
	#endif

	// Check if we just want to save the palette.
	if (!iStrCmp(Ext, TEXT("pal"))) {
		return ilSavePal(FileName);
	}

	// Try registered procedures
	if (iRegisterSave(FileName))
		return IL_TRUE;

	ilSetError(IL_INVALID_EXTENSION);
	return IL_FALSE;
}
