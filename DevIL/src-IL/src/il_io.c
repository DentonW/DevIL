//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 11/07/2008
//
// Filename: src-IL/src/il_io.c
//
// Description: Determines image types and loads images
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#include "il_register.h"
#include "il_pal.h"
#include <string.h>


// Returns a widened version of a string.
// Make sure to free this after it is used.  Code help from
//  https://buildsecurityin.us-cert.gov/daisy/bsi-rules/home/g1/769-BSI.html
#if defined(_UNICODE)
wchar_t *WideFromMultiByte(const char *Multi)
{
	ILint	Length;
	wchar_t	*Temp;

	Length = mbstowcs(NULL, (const char*)Multi, 0) + 1; // note error return of -1 is possible
	if (Length == 0) {
		ilSetError(IL_INVALID_PARAM);
		return NULL;
	}
	if (Length > ULONG_MAX/sizeof(wchar_t)) {
		ilSetError(IL_INTERNAL_ERROR);
		return NULL;
	}
	Temp = (wchar_t*)ialloc(Length * sizeof(wchar_t));
	mbstowcs(Temp, (const char*)Multi, Length); 

	return Temp;
}
#endif


ILAPI ILenum ILAPIENTRY ilTypeFromExt(ILconst_string FileName)
{
	ILenum		Type;
	ILstring	Ext;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_TYPE_UNKNOWN;
	}

	Ext = iGetExtension(FileName);
	//added 2003-08-31: fix sf bug 789535
	if (Ext == NULL) {
		return IL_TYPE_UNKNOWN;
	}

	if (!iStrCmp(Ext, IL_TEXT("tga")) || !iStrCmp(Ext, IL_TEXT("vda")) ||
		!iStrCmp(Ext, IL_TEXT("icb")) || !iStrCmp(Ext, IL_TEXT("vst")))
		Type = IL_TGA;
	else if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpe")) || !iStrCmp(Ext, IL_TEXT("jpeg")))
		Type = IL_JPG;
	else if (!iStrCmp(Ext, IL_TEXT("jp2")))
		Type = IL_JP2;
	else if (!iStrCmp(Ext, IL_TEXT("dds")))
		Type = IL_DDS;
	else if (!iStrCmp(Ext, IL_TEXT("png")))
		Type = IL_PNG;
	else if (!iStrCmp(Ext, IL_TEXT("bmp")) || !iStrCmp(Ext, IL_TEXT("dib")))
		Type = IL_BMP;
	else if (!iStrCmp(Ext, IL_TEXT("gif")))
		Type = IL_GIF;
	else if (!iStrCmp(Ext, IL_TEXT("cut")))
		Type = IL_CUT;
	else if (!iStrCmp(Ext, IL_TEXT("hdr")))
		Type = IL_HDR;
	else if (!iStrCmp(Ext, IL_TEXT("ico")) || !iStrCmp(Ext, IL_TEXT("cur")))
		Type = IL_ICO;
	else if (!iStrCmp(Ext, IL_TEXT("icns")))
		Type = IL_ICNS;
	else if (!iStrCmp(Ext, IL_TEXT("jng")))
		Type = IL_JNG;
	else if (!iStrCmp(Ext, IL_TEXT("lif")))
		Type = IL_LIF;
	else if (!iStrCmp(Ext, IL_TEXT("mdl")))
		Type = IL_MDL;
	else if (!iStrCmp(Ext, IL_TEXT("mng")) || !iStrCmp(Ext, IL_TEXT("jng")))
		Type = IL_MNG;
	else if (!iStrCmp(Ext, IL_TEXT("pcd")))
		Type = IL_PCD;
	else if (!iStrCmp(Ext, IL_TEXT("pcx")))
		Type = IL_PCX;
	else if (!iStrCmp(Ext, IL_TEXT("pic")))
		Type = IL_PIC;
	else if (!iStrCmp(Ext, IL_TEXT("pix")))
		Type = IL_PIX;
	else if (!iStrCmp(Ext, IL_TEXT("pbm")) || !iStrCmp(Ext, IL_TEXT("pgm")) ||
		!iStrCmp(Ext, IL_TEXT("pnm")) || !iStrCmp(Ext, IL_TEXT("ppm")))
		Type = IL_PNM;
	else if (!iStrCmp(Ext, IL_TEXT("psd")) || !iStrCmp(Ext, IL_TEXT("pdd")))
		Type = IL_PSD;
	else if (!iStrCmp(Ext, IL_TEXT("psp")))
		Type = IL_PSP;
	else if (!iStrCmp(Ext, IL_TEXT("pxr")))
		Type = IL_PXR;
	else if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
		!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba")))
		Type = IL_SGI;
	else if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff")))
		Type = IL_TIF;
	else if (!iStrCmp(Ext, IL_TEXT("vtf")))
		Type = IL_VTF;
	else if (!iStrCmp(Ext, IL_TEXT("wal")))
		Type = IL_WAL;
	else if (!iStrCmp(Ext, IL_TEXT("wdp")) || !iStrCmp(Ext, IL_TEXT("hdp")))
		Type = IL_WDP;
	else if (!iStrCmp(Ext, IL_TEXT("xpm")))
		Type = IL_XPM;
	else
		Type = IL_TYPE_UNKNOWN;

	return Type;
}


//ILenum ilDetermineTypeF(ILHANDLE File);

//changed 2003-09-17 to ILAPIENTRY
ILAPI ILenum ILAPIENTRY ilDetermineType(ILconst_string FileName)
{
	ILHANDLE	File;
	ILenum		Type;

	if (FileName == NULL)
		return IL_TYPE_UNKNOWN;

	File = iopenr(FileName);
	if (File == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}
	Type = ilDetermineTypeF(File);
	icloser(File);

	return Type;
}


ILenum ILAPIENTRY ilDetermineTypeF(ILHANDLE File)
{
	if (File == NULL)
		return IL_TYPE_UNKNOWN;

	#ifndef IL_NO_JPG
	if (ilIsValidJpgF(File))
		return IL_JPG;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidDdsF(File))
		return IL_DDS;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidPngF(File))
		return IL_PNG;
	#endif

	#ifndef IL_NO_BMP
	if (ilIsValidBmpF(File))
		return IL_BMP;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidGifF(File))
		return IL_GIF;
	#endif

	#ifndef IL_NO_HDR
	if (ilIsValidHdrF(File))
		return IL_HDR;
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

	#ifndef IL_NO_PNM
	if (ilIsValidPnmF(File))
		return IL_PNM;
	#endif

	#ifndef IL_NO_PSD
	if (ilIsValidPsdF(File))
		return IL_PSD;
	#endif

	#ifndef IL_NO_PSP
	if (ilIsValidPspF(File))
		return IL_PSP;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidSgiF(File))
		return IL_SGI;
	#endif

	#ifndef IL_NO_TIF
	if (ilIsValidTiffF(File))
		return IL_TIF;
	#endif

	//moved tga to end of list because it has no magic number
	//in header to assure that this is really a tga... (20040218)
	#ifndef IL_NO_TGA
	if (ilIsValidTgaF(File))
		return IL_TGA;
	#endif
	
	return IL_TYPE_UNKNOWN;
}


ILenum ilDetermineTypeL(const void *Lump, ILuint Size)
{
	if (Lump == NULL)
		return IL_TYPE_UNKNOWN;

	#ifndef IL_NO_JPG
	if (ilIsValidJpgL(Lump, Size))
		return IL_JPG;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidDdsL(Lump, Size))
		return IL_DDS;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidPngL(Lump, Size))
		return IL_PNG;
	#endif

	#ifndef IL_NO_BMP
	if (ilIsValidBmpL(Lump, Size))
		return IL_BMP;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidGifL(Lump, Size))
		return IL_GIF;
	#endif

	#ifndef IL_NO_HDR
	if (ilIsValidHdrL(Lump, Size))
		return IL_HDR;
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

	#ifndef IL_NO_PNM
	if (ilIsValidPnmL(Lump, Size))
		return IL_PNM;
	#endif

	#ifndef IL_NO_PSD
	if (ilIsValidPsdL(Lump, Size))
		return IL_PSD;
	#endif

	#ifndef IL_NO_PSP
	if (ilIsValidPspL(Lump, Size))
		return IL_PSP;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidSgiL(Lump, Size))
		return IL_SGI;
	#endif

	#ifndef IL_NO_TIF
	if (ilIsValidTiffL(Lump, Size))
		return IL_TIF;
	#endif

	//moved tga to end of list because it has no magic number
	//in header to assure that this is really a tga... (20040218)
	#ifndef IL_NO_TGA
	if (ilIsValidTgaL(Lump, Size))
		return IL_TGA;
	#endif

	return IL_TYPE_UNKNOWN;
}


ILboolean ILAPIENTRY ilIsValid(ILenum Type, ILstring FileName)
{
	if (FileName == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidTga(FileName);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpg(FileName);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDds(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPng(FileName);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmp(FileName);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidGif(FileName);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidHdr(FileName);
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

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnm(FileName);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidPsd(FileName);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidPsp(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgi(FileName);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValidTiff(FileName);
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
		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidTgaF(File);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpgF(File);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDdsF(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPngF(File);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmpF(File);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidGifF(File);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidHdrF(File);
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

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnmF(File);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidPsdF(File);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidPspF(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgiF(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilIsValidL(ILenum Type, void *Lump, ILuint Size)
{
	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidTgaL(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpgL(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDdsL(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPngL(Lump, Size);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmpL(Lump, Size);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidGifL(Lump, Size);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidHdrL(Lump, Size);
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

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnmL(Lump, Size);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidPsdL(Lump, Size);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidPspL(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgiL(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilLoad(ILenum Type, ILconst_string FileName)
{
	ILboolean	bRet;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			bRet = ilLoadImage(FileName);
			break;

		#ifndef IL_NO_TGA
		case IL_TGA:
			bRet = ilLoadTarga(FileName);
			break;
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			bRet = ilLoadJpeg(FileName);
			break;
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			bRet = ilLoadJp2(FileName);
			break;
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			bRet = ilLoadDds(FileName);
			break;
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			bRet = ilLoadPng(FileName);
			break;
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			bRet = ilLoadBmp(FileName);
			break;
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			bRet = ilLoadGif(FileName);
			break;
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			bRet = ilLoadHdr(FileName);
			break;
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			bRet = ilLoadCut(FileName);
			break;
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			bRet = ilLoadDoom(FileName);
			break;
		case IL_DOOM_FLAT:
			bRet = ilLoadDoomFlat(FileName);
			break;
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			bRet = ilLoadIcon(FileName);
			break;
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			bRet = ilLoadIcns(FileName);
			break;
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			bRet = ilLoadLif(FileName);
			break;
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			bRet = ilLoadMdl(FileName);
			break;
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			bRet = ilLoadMng(FileName);
			break;
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			bRet = IL_FALSE;//ilLoadPcd(FileName);
			break;
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			bRet = ilLoadPcx(FileName);
			break;
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			bRet = ilLoadPic(FileName);
			break;
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			bRet = ilLoadPix(FileName);
			break;
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			bRet = ilLoadPnm(FileName);
			break;
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			bRet = ilLoadPsd(FileName);
			break;
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			bRet = ilLoadPsp(FileName);
			break;
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			bRet = ilLoadPxr(FileName);
			break;
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			bRet = ilLoadRaw(FileName);
			break;
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			bRet = ilLoadSgi(FileName);
			break;
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			//#ifndef _UNICODE
			bRet = ilLoadTiff(FileName);
			//#else
			//	wcstombs(AnsiName, FileName, 512);
			//	//WideCharToMultiByte(CP_ACP, 0, FileName, -1, AnsiName, 512, NULL, NULL);
			//	return ilLoadTiff(AnsiName);
			//#endif//_UNICODE
			break;
		#endif

		#ifndef IL_NO_VTF
		case IL_WDP:
			bRet = ilLoadVtf(FileName);
			break;
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			bRet = ilLoadWal(FileName);
			break;
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			bRet = ilLoadXpm(FileName);
			break;
		#endif

		#ifndef IL_NO_WDP
		case IL_WDP:
			bRet = ilLoadWdp(FileName);
			break;
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			bRet = ilLoadExr(FileName);
			break;
		#endif

		default:
			ilSetError(IL_INVALID_ENUM);
			bRet = IL_FALSE;
	}

	return bRet;
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

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadTargaF(File);
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				return ilLoadJpegF(File);
			#endif
		#endif

		// There is not a way to do this with the JasPer library.
		/*#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadJp2F(File);
		#endif*/

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDdsF(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPngF(File);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmpF(File);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadGifF(File);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadHdrF(File);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCutF(File);
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

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadIcnsF(File);
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
			return IL_FALSE;//return ilLoadPcdF(File);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcxF(File);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPicF(File);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadPixF(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnmF(File);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadPsdF(File);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadPspF(File);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadPxrF(File);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadRawF(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgiF(File);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadTiffF(File);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadVtfF(File);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWalF(File);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadXpmF(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilLoadL(ILenum Type, const void *Lump, ILuint Size) {
	if (Lump == NULL || Size == 0) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Type == IL_TYPE_UNKNOWN)
		Type = ilDetermineTypeL(Lump, Size);
	
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return IL_FALSE;

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadTargaL(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilLoadJpegL(Lump, Size);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadJp2L(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDdsL(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPngL(Lump, Size);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmpL(Lump, Size);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadGifL(Lump, Size);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadHdrL(Lump, Size);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCutL(Lump, Size);
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

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadIcnsL(Lump, Size);
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
			return IL_FALSE;//return ilLoadPcdL(Lump, Size);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcxL(Lump, Size);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPicL(Lump, Size);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadPixL(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnmL(Lump, Size);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadPsdL(Lump, Size);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadPspL(Lump, Size);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadPxrL(Lump, Size);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadRawL(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgiL(Lump, Size);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadTiffL(Lump, Size);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadVtfL(Lump, Size);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWalL(Lump, Size);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadXpmL(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image with various different methods before failing - very generic.
ILboolean ILAPIENTRY ilLoadImage(ILconst_string FileName)
{
	ILstring	Ext;
	ILenum		Type;
	ILboolean	bRet = IL_FALSE;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	Ext = iGetExtension(FileName);

	// Try registered procedures first (so users can override default lib functions).
	if (Ext) {
		if (iRegisterLoad(FileName))
			return IL_TRUE;

		#ifndef IL_NO_TGA
		if (!iStrCmp(Ext, IL_TEXT("tga")) || !iStrCmp(Ext, IL_TEXT("vda")) ||
			!iStrCmp(Ext, IL_TEXT("icb")) || !iStrCmp(Ext, IL_TEXT("vst"))) {
			bRet = ilLoadTarga(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_JPG
		if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpe")) ||
			!iStrCmp(Ext, IL_TEXT("jpeg"))) {
			bRet = ilLoadJpeg(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_JP2
		if (!iStrCmp(Ext, IL_TEXT("jp2"))) {
			bRet = ilLoadJp2(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_DDS
		if (!iStrCmp(Ext, IL_TEXT("dds"))) {
			bRet = ilLoadDds(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PNG
		if (!iStrCmp(Ext, IL_TEXT("png"))) {
			bRet = ilLoadPng(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_BMP
		if (!iStrCmp(Ext, IL_TEXT("bmp")) || !iStrCmp(Ext, IL_TEXT("dib"))) {
			bRet = ilLoadBmp(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_GIF
		if (!iStrCmp(Ext, IL_TEXT("gif"))) {
			bRet = ilLoadGif(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_HDR
		if (!iStrCmp(Ext, IL_TEXT("hdr"))) {
			bRet = ilLoadHdr(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_CUT
		if (!iStrCmp(Ext, IL_TEXT("cut"))) {
			bRet = ilLoadCut(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_DCX
		if (!iStrCmp(Ext, IL_TEXT("dcx"))) {
			bRet = ilLoadDcx(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_ICO
		if (!iStrCmp(Ext, IL_TEXT("ico")) || !iStrCmp(Ext, IL_TEXT("cur"))) {
			bRet = ilLoadIcon(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_ICNS
		if (!iStrCmp(Ext, IL_TEXT("icns"))) {
			bRet = ilLoadIcns(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_LIF
		if (!iStrCmp(Ext, IL_TEXT("lif"))) {
			bRet = ilLoadLif(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_MDL
		if (!iStrCmp(Ext, IL_TEXT("mdl"))) {
			bRet = ilLoadMdl(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_MNG
		if (!iStrCmp(Ext, IL_TEXT("mng")) || !iStrCmp(Ext, IL_TEXT("jng"))) {
			bRet = ilLoadMng(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PCD
		if (!iStrCmp(Ext, IL_TEXT("pcd"))) {
			bRet = IL_FALSE;//bRet = ilLoadPcd(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PCX
		if (!iStrCmp(Ext, IL_TEXT("pcx"))) {
			bRet = ilLoadPcx(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PIC
		if (!iStrCmp(Ext, IL_TEXT("pic"))) {
			bRet = ilLoadPic(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PIX
		if (!iStrCmp(Ext, IL_TEXT("pix"))) {
			bRet = ilLoadPix(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PNM
		if (!iStrCmp(Ext, IL_TEXT("pbm"))) {
			bRet = ilLoadPnm(FileName);
			goto finish;
		}
		if (!iStrCmp(Ext, IL_TEXT("pgm"))) {
			bRet = ilLoadPnm(FileName);
			goto finish;
		}
		if (!iStrCmp(Ext, IL_TEXT("pnm"))) {
			bRet = ilLoadPnm(FileName);
			goto finish;
		}
		if (!iStrCmp(Ext, IL_TEXT("ppm"))) {
			bRet = ilLoadPnm(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PSD
		if (!iStrCmp(Ext, IL_TEXT("psd")) || !iStrCmp(Ext, IL_TEXT("pdd"))) {
			bRet = ilLoadPsd(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PSP
		if (!iStrCmp(Ext, IL_TEXT("psp"))) {
			bRet = ilLoadPsp(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PXR
		if (!iStrCmp(Ext, IL_TEXT("pxr"))) {
			bRet = ilLoadPxr(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_SGI
		if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
			!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba"))) {
			bRet = ilLoadSgi(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_TIF
		if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff"))) {
			bRet = ilLoadTiff(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_VTF
		if (!iStrCmp(Ext, IL_TEXT("vtf"))) {
			bRet = ilLoadVtf(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_WAL
		if (!iStrCmp(Ext, IL_TEXT("wal"))) {
			bRet = ilLoadWal(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_WDP
		if (!iStrCmp(Ext, IL_TEXT("wdp")) || !iStrCmp(Ext, IL_TEXT("hdp")) ) {
			bRet = ilLoadWdp(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_XPM
		if (!iStrCmp(Ext, IL_TEXT("xpm"))) {
			bRet = ilLoadXpm(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_EXR
		if (!iStrCmp(Ext, IL_TEXT("exr"))) {
			bRet = ilLoadExr(FileName);
			goto finish;
		}
		#endif
	}

	// As a last-ditch effort, try to identify the image
	Type = ilDetermineType(FileName);
	if (Type == IL_TYPE_UNKNOWN) {
		ilSetError(IL_INVALID_EXTENSION);
		return IL_FALSE;
	}
	return ilLoad(Type, FileName);

finish:
	return bRet;
}


ILboolean ILAPIENTRY ilSave(ILenum Type, ILstring FileName)
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

		#ifndef IL_NO_DDS
		case IL_DDS:
    		return ilSaveDds(FileName);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilSaveHdr(FileName);
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

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilSavePsd(FileName);
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


ILuint ILAPIENTRY ilSaveF(ILenum Type, ILHANDLE File)
{
	ILboolean Ret;

	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return 0;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			Ret = ilSaveBmpF(File);
			break;
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			Ret = ilSaveDdsF(File);
			break;
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			Ret = ilSaveHdrF(File);
			break;
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				Ret = ilSaveJpegF(File);
				break;
			#endif
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			Ret = ilSavePnmF(File);
			break;
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			Ret = ilSavePngF(File);
			break;	
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			Ret = ilSavePsdF(File);
			break;
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			Ret = ilSaveRawF(File);
			break;
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			Ret = ilSaveSgiF(File);
			break;
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			Ret = ilSaveTargaF(File);
			break;
		#endif

		/*#ifndef IL_NO_TIF
		case IL_TIF:
			Ret = ilSaveTiffF(File);
			break;
		#endif*/

		default:
			ilSetError(IL_INVALID_ENUM);
			return 0;
	}

	if (Ret == IL_FALSE)
		return 0;

	return itellw();
}


ILuint ILAPIENTRY ilSaveL(ILenum Type, void *Lump, ILuint Size)
{
	ILboolean Ret;

	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return 0;
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			Ret = ilSaveBmpL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			Ret = ilSaveHdrL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			Ret = ilSaveJpegL(Lump, Size);
			break;
		#endif


		#ifndef IL_NO_PNG
		case IL_PNG:
			Ret = ilSavePngL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			Ret = ilSavePnmL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			Ret = ilSavePsdL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			Ret = ilSaveRawL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			Ret = ilSaveSgiL(Lump, Size);
			break;
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			Ret = ilSaveTargaL(Lump, Size);
			break;
		#endif
		
		case IL_DDS:
			Ret = ilSaveDdsL(Lump, Size);
			break;

		/*#ifndef IL_NO_TIF
		case IL_TIF:
			Ret = ilSaveTiffL(Lump, Size);
			break;
		#endif*/

		default:
			ilSetError(IL_INVALID_ENUM);
			return 0;
	}

	if (Ret == IL_FALSE)
		return 0;

	return itellw();
}


//! Determines what image type to save based on the extension and attempts to save
//	the current image based on the extension given in FileName.
ILboolean ILAPIENTRY ilSaveImage(ILconst_string FileName)
{
	ILstring Ext;
	ILboolean	bRet = IL_FALSE;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Ext = iGetExtension(FileName);
	if (Ext == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	#ifndef IL_NO_BMP
	if (!iStrCmp(Ext, IL_TEXT("bmp"))) {
		bRet = ilSaveBmp(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_CHEAD
	if (!iStrCmp(Ext, IL_TEXT("h"))) {
		bRet = ilSaveCHeader(FileName, "IL_IMAGE");
		goto finish;
	}
	#endif

	#ifndef IL_NO_DDS
	if (!iStrCmp(Ext, IL_TEXT("dds"))) {
		bRet = ilSaveDds(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_HDR
	if (!iStrCmp(Ext, IL_TEXT("hdr"))) {
		bRet = ilSaveHdr(FileName);
		goto finish;
	}
	#endif

#ifndef IL_NO_JPG
	if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpeg")) || !iStrCmp(Ext, IL_TEXT("jpe"))) {
		bRet = ilSaveJpeg(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PCX
	if (!iStrCmp(Ext, IL_TEXT("pcx"))) {
		bRet = ilSavePcx(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PNG
	if (!iStrCmp(Ext, IL_TEXT("png"))) {
		bRet = ilSavePng(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PNM  // Not sure if binary or ascii should be defaulted...maybe an option?
	if (!iStrCmp(Ext, IL_TEXT("pbm"))) {
		bRet = ilSavePnm(FileName);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("pgm"))) {
		bRet = ilSavePnm(FileName);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("ppm"))) {
		bRet = ilSavePnm(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PSD
	if (!iStrCmp(Ext, IL_TEXT("psd"))) {
		bRet = ilSavePsd(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_RAW
	if (!iStrCmp(Ext, IL_TEXT("raw"))) {
		bRet = ilSaveRaw(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_SGI
	if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
		!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba"))) {
		bRet = ilSaveSgi(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_TGA
	if (!iStrCmp(Ext, IL_TEXT("tga"))) {
		bRet = ilSaveTarga(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_TIF
	if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff"))) {
		bRet = ilSaveTiff(FileName);
		goto finish;
	}
	#endif

	// Check if we just want to save the palette.
	if (!iStrCmp(Ext, IL_TEXT("pal"))) {
		bRet = ilSavePal(FileName);
		goto finish;
	}

	// Try registered procedures
	if (iRegisterSave(FileName))
		return IL_TRUE;

	ilSetError(IL_INVALID_EXTENSION);
	return IL_FALSE;

finish:
	return bRet;
}
