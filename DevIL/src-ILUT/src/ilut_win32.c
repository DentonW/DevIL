//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/28/2001 <--Y2K Compliant! =]
//
// Filename: src-ILUT/src/ilut_win32.c
//
// Description: Windows functions for images
//
//-----------------------------------------------------------------------------


#include "ilut_internal.h"
#ifdef ILUT_USE_WIN32
#include <wininet.h>
#include <windows.h>

// For ilutWinLoadUrl().
#pragma comment(lib, "wininet.lib")

#if !_WIN32_WCE && (_WIN32 && __GNUC__)
	PRINTDLG	Pd;
#endif//_WIN32_WCE

ILboolean ilutWin32Init()
{


	return IL_TRUE;
}


HBITMAP ILAPIENTRY ilutConvertToHBitmap(HDC hDC)
{
	ILubyte		*Data, *BmpBits;
	HBITMAP		hBitmap;
	BITMAPINFO	bmi;
	ILimage		*TempImage;

	TempImage = ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	// @TODO:  Add support for palette'd images.
	if (ilutCurImage->Bpp < 3)
		TempImage = iConvertImage(ilutCurImage, IL_BGR, IL_UNSIGNED_BYTE);
	else if (ilutCurImage->Bpc > 1)
		TempImage = iConvertImage(ilutCurImage, ilutCurImage->Format, IL_UNSIGNED_BYTE);
	if (TempImage == NULL)
		return 0;

	//changed 2003-09-09: use Temp!
	ilSetCurImage(TempImage);

	Data = ilutGetPaddedData();
	if (Data == NULL) {
		//added 2003-09-09: clean up
		ilSetCurImage(ilutCurImage);
		if (TempImage != ilutCurImage)
			ilCloseImage(TempImage);
		return 0;
	}

	ilutGetBmpInfo(&bmi);

	/*DIB_PAL_COLORS*/
	hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&BmpBits, NULL, 0);
	if (!hBitmap || !BmpBits) {
		ilSetCurImage(ilutCurImage);
		if (TempImage != ilutCurImage)
			ilCloseImage(TempImage);
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return 0;
	}

	memcpy(BmpBits, Data, (ilutCurImage->Bps + (4 - (ilutCurImage->Bps % 4)) % 4) * ilutCurImage->Height);

	// Platform SDK - Windows GDI - Device-Independent Bitmaps
	//Bitmap = CreateBitmap(ilutCurImage->Width, ilutCurImage->Height, 1, ilutCurImage->Bpp * 8, Data);
	//SetBitmapBits(Bitmap, ilutCurImage->SizeOfData, ilutCurImage->Data);

	ifree(Data);

	//changed 2003-09-09: re-set the original current image
	ilSetCurImage(ilutCurImage);

	if (TempImage != ilutCurImage)
		ilCloseImage(TempImage);

	return hBitmap;
}


ILubyte* ILAPIENTRY iGetPaddedData(ILimage *Image)
{
	ILubyte	*NewData = NULL, *TempBuff = NULL;
	ILuint	i, CurPos = 0, PadSize;
	ILubyte	*TempData = NULL;

	if (Image == NULL) {
		ilSetError(ILUT_INVALID_PARAM);
		return NULL;
	}

	if (Image->Origin != IL_ORIGIN_LOWER_LEFT) {
		TempData = iGetFlipped(Image);
	}
	else {
		TempData = Image->Data;
	}

	if (Image->Format == IL_RGB || Image->Format == IL_RGBA) {
		TempBuff = (ILubyte*)ialloc(Image->SizeOfData);
		if (TempBuff == NULL) {
			return NULL;
		}
		// Swap red and blue.
		for (i = 0; i < Image->SizeOfData; i += Image->Bpp) {
			TempBuff[i] = TempData[i+2];
			TempBuff[i+1] = TempData[i+1];
			TempBuff[i+2] = TempData[i];
		}
	}
	else {
		TempBuff = TempData;
	}

	PadSize = (4 - (Image->Bps % 4)) % 4;
	NewData = (ILubyte*)ialloc((Image->Width + PadSize) * Image->Height * Image->Bpp);
	if (NewData == NULL) {
		return NULL;
	}

	for (i = 0; i < Image->Height; i++) {
		memcpy(NewData + CurPos, TempBuff + Image->Bps * i, Image->Bps);
		CurPos += Image->Bps;
		memset(NewData + CurPos, 0, PadSize);		
		CurPos += PadSize;
	}

	if (TempData != TempBuff && TempData != Image->Data)
		ifree(TempData);
	if (TempBuff != Image->Data)
		ifree(TempBuff);

	return NewData;
}


ILvoid ILAPIENTRY ilutFreePaddedData(ILubyte *Data)
{
	ifree(Data);
	return;
}


// DirectX/GDI insists that all scanlines end on a dword boundary. =(
ILubyte* ILAPIENTRY ilutGetPaddedData()
{
	return iGetPaddedData(ilGetCurImage());
}


// @TODO:  Figure how to mess with multiple bpc's!
ILvoid ILAPIENTRY ilutGetBmpInfo(BITMAPINFO *Info)
{
	ILuint NewBps, Padding;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return;
	}

	Padding = (4 - (ilutCurImage->Bps % 4)) % 4;
	NewBps = ilutCurImage->Bps/* + Padding*/;

	Info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info->bmiHeader.biWidth = ilutCurImage->Width;
	Info->bmiHeader.biHeight = ilutCurImage->Height;
	Info->bmiHeader.biPlanes = 1;
	Info->bmiHeader.biBitCount = ilutCurImage->Bpp << 3;
	Info->bmiHeader.biCompression = BI_RGB;
	Info->bmiHeader.biSizeImage = NewBps * ilutCurImage->Height;
	Info->bmiHeader.biXPelsPerMeter = 0;
	Info->bmiHeader.biYPelsPerMeter = 0;
	Info->bmiHeader.biClrUsed = ilutCurImage->Bpp == 1 ? 255 : 0;
	if (Info->bmiHeader.biClrUsed < 24)
		Info->bmiHeader.biClrImportant = Info->bmiHeader.biClrUsed;
	else
		Info->bmiHeader.biClrImportant = 0;

	return;
}


//! Just a convenience function.
HBITMAP ILAPIENTRY ilutWinLoadImage(const ILstring FileName, HDC hDC)
{
	HBITMAP	Bitmap;

	iBindImageTemp();
	if (!ilLoadImage(FileName))
		return 0;

	Bitmap = ilutConvertToHBitmap(hDC);

	return Bitmap;
}


#ifndef _WIN32_WCE
ILboolean ILAPIENTRY ilutWinSaveImage(const ILstring FileName, HBITMAP Bitmap)
{
	ILuint		CurName;
	ILboolean	Saved;
	
	CurName = ilGetCurName();

	iBindImageTemp();

	if (!ilutSetHBitmap(Bitmap)) {
		ilBindImage(CurName);
		return IL_FALSE;
	}

	Saved = ilSaveImage(FileName);
	ilBindImage(CurName);

	return Saved;
}
#endif//_WIN32_WCE


// @TODO:  Just create a copy of the palette!
// Credit for this goes to the OpenGL SuperBible.
HPALETTE ILAPIENTRY ilutGetHPal()
{
	HPALETTE	Palette;
	LOGPALETTE	*LogPal;
	ILuint		NumEntries, i;
	ILenum		CurPalType;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	if (!ilutCurImage->Pal.Palette || !ilutCurImage->Pal.PalSize || ilutCurImage->Pal.PalType == IL_PAL_NONE) {
		//ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	CurPalType = ilutCurImage->Pal.PalType;
	if (!ilConvertPal(IL_PAL_RGB24)) {
		return NULL;  // ilConvertPal already sets the error
	}
	NumEntries = ilutCurImage->Pal.PalSize / 3;

	LogPal = (LOGPALETTE*)ialloc(sizeof(LOGPALETTE) + NumEntries * sizeof(PALETTEENTRY));
	if (!LogPal) {
		return NULL;
	}

	LogPal->palVersion = 0x300;
	LogPal->palNumEntries = NumEntries;

	for (i = 0; i < NumEntries; i++) {
		LogPal->palPalEntry[i].peRed   = ilutCurImage->Pal.Palette[i * 3];
		LogPal->palPalEntry[i].peGreen = ilutCurImage->Pal.Palette[i * 3 + 1];
		LogPal->palPalEntry[i].peBlue  = ilutCurImage->Pal.Palette[i * 3 + 2];
		LogPal->palPalEntry[i].peFlags = 0;
	}

	Palette = CreatePalette(LogPal);
	ifree(LogPal);

	ilConvertPal(CurPalType);  // Should we check the return value?

	return Palette;
}


ILboolean ILAPIENTRY ilutSetHBitmap(HBITMAP Bitmap)
{
#ifndef _WIN32_WCE
	BITMAPINFO	Info[2];
	HWND		hWnd;
	HDC			hDC;
	ILubyte		*Buffer1 = NULL, *Buffer2 = NULL;
	ILuint		i, j, PadSize, Bps;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	hWnd = GetForegroundWindow();
	hDC = GetDC(hWnd);

	// Query the dimensions
	memset(&Info, 0, sizeof(BITMAPINFO));
	Info[0].bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(hDC, Bitmap, 0, 0, NULL, Info, DIB_RGB_COLORS);

	// @TODO:  Implement this shitz0rz!
	if (Info[0].bmiHeader.biBitCount < 24) {
		return IL_FALSE;
	}

	Buffer1 = (ILubyte*)ialloc(Info[0].bmiHeader.biSizeImage);
	Buffer2 = (ILubyte*)ialloc(Info[0].bmiHeader.biSizeImage);
	if (Buffer1 == NULL || Buffer2 == NULL) {
		ifree(Buffer1);
		ifree(Buffer2);
		return IL_FALSE;
	}

	//GetBitmapBits
	GetDIBits(hDC, Bitmap, 0, Info[0].bmiHeader.biHeight, Buffer1, Info, DIB_RGB_COLORS);

	Bps = Info[0].bmiHeader.biWidth * (Info[0].bmiHeader.biBitCount >> 3);
	PadSize = (4 - (Bps % 4)) % 4;

	// Remove the padding.
	for (i = 0, j = 0; i < Info[0].bmiHeader.biSizeImage; i += Bps + PadSize, j += Bps) {
		memcpy(Buffer2 + j, Buffer1 + i, Bps);
	}

	if (Info[0].bmiHeader.biBitCount == 24) {
		ilTexImage(Info[0].bmiHeader.biWidth, Info[0].bmiHeader.biHeight, 1, 
			(ILubyte)(Info[0].bmiHeader.biBitCount >> 3), IL_BGR, IL_UNSIGNED_BYTE, Buffer2);
	}
	else if (Info[0].bmiHeader.biBitCount == 32) {
		ilTexImage(Info[0].bmiHeader.biWidth, Info[0].bmiHeader.biHeight, 1, 
			(ILubyte)(Info[0].bmiHeader.biBitCount >> 3), IL_BGRA, IL_UNSIGNED_BYTE, Buffer2);
	}
	ilutCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	ifree(Buffer1);
	ifree(Buffer2);

#endif//_WIN32_WCE

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutSetHPal(HPALETTE Pal)
{
	LPPALETTEENTRY	PalEntries;
	ILuint			NumEntries, i;
	ILubyte			*TempPal;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	NumEntries = GetPaletteEntries(Pal, 0, 0, NULL);
	if (NumEntries == 0)
		return IL_TRUE;  // @TODO:  Determine if correct...

	PalEntries = (LPPALETTEENTRY)ialloc(NumEntries * sizeof(PALETTEENTRY));
	if (PalEntries == NULL) {
		return IL_FALSE;
	}

	NumEntries = GetPaletteEntries(Pal, 0, NumEntries, PalEntries);

	TempPal = (ILubyte*)ialloc(NumEntries * 3);
	if (TempPal == NULL) {
		ifree(PalEntries);
		return IL_FALSE;
	}
	if (ilutCurImage->Pal.Palette)
		ifree(ilutCurImage->Pal.Palette);
	ilutCurImage->Pal.Palette = TempPal;
	ilutCurImage->Pal.PalSize = NumEntries * 3;
	ilutCurImage->Pal.PalType = IL_PAL_RGB24;

	for (i = 0; i < NumEntries; i++) {
		*TempPal++ = PalEntries[i].peRed;
		*TempPal++ = PalEntries[i].peGreen;
		*TempPal++ = PalEntries[i].peBlue;
	}

	ifree(PalEntries);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutSetWinClipboard()
{
	HBITMAP	Bitmap;
	HANDLE	Handle;
	HWND	hWnd;
	HDC		hDC;
	ILimage	*TempImage, *CurImage;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (ilutCurImage->Format != IL_BGR || ilutCurImage->Bps > 1) {
		TempImage = iConvertImage(ilutCurImage, IL_BGR, IL_UNSIGNED_BYTE);
		if (TempImage == NULL)
			return IL_FALSE;
	}
	else
		TempImage = ilutCurImage;

	CurImage = ilutCurImage;
	ilSetCurImage(TempImage);

	hWnd = GetForegroundWindow();
	hDC = GetDC(hWnd);

	if (!OpenClipboard(NULL)) {
		if (TempImage != ilutCurImage)
			ilCloseImage(TempImage);
		ilSetCurImage(CurImage);
		ilSetError(ILUT_ILLEGAL_OPERATION);  // Dunno if this is the correct error.
		return IL_FALSE;
	}

	Bitmap = ilutConvertToHBitmap(hDC);

	EmptyClipboard();
	Handle = SetClipboardData(CF_BITMAP, Bitmap);

	CloseClipboard();

	//DeleteObject(Bitmap);  // Needed?

	if (TempImage != ilutCurImage)
		ilCloseImage(TempImage);
	ilSetCurImage(CurImage);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutGetWinClipboard()
{
	HBITMAP		Bitmap;
	HWND		hWnd;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	hWnd = GetForegroundWindow();

	if (!OpenClipboard(hWnd)) {
		ilSetError(ILUT_ILLEGAL_OPERATION);  // Dunno if this is the correct error.
		return IL_FALSE;
	}

	Bitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	if (!Bitmap) {
		CloseClipboard();
		return IL_FALSE;  // No error?
	}

	if (!ilutSetHBitmap(Bitmap)) {
		CloseClipboard();
		return IL_FALSE;
	}

	CloseClipboard();

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutWinPrint(ILuint XPos, ILuint YPos, ILuint Width, ILuint Height, HDC hDC)
{
#if !defined(_WIN32_WCE) && !(defined(_WIN32) && defined(__GNUC__))
	PRINTDLG	Pd;
	DOCINFO		Di;
	HBITMAP		Bitmap, hReplaced;
	HDC			hMemDC;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// Needs error checking!
	hMemDC = CreateCompatibleDC(hDC);
	Bitmap = ilutConvertToHBitmap(hDC);
	hReplaced = (HBITMAP)SelectObject(hMemDC, Bitmap);

	memset(&Pd, 0, sizeof(PRINTDLG));
	Pd.lStructSize = sizeof(PRINTDLG);
	Pd.hwndOwner = GetForegroundWindow();
	Pd.Flags = PD_RETURNDC;
	Pd.nCopies = 1;
	Pd.nFromPage = 0xFFFF;
	Pd.nToPage = 0xFFFF;
	Pd.nMinPage = 1;
	Pd.nMaxPage = 0xFFFF;

	if (!PrintDlg(&Pd))
		return (0L);

	Di.cbSize = sizeof(DOCINFO);
	Di.lpszDocName = "DevIL Print Job";
	Di.lpszOutput = NULL;
	Di.lpszDatatype = NULL;
	Di.fwType = 0;

	StartDoc(Pd.hDC, &Di);
	StartPage(Pd.hDC);

	StretchBlt(Pd.hDC, XPos, YPos, Width, Height, hMemDC, 0, 0, ilutCurImage->Width, ilutCurImage->Height, SRCCOPY);

	EndPage(Pd.hDC);
	EndDoc(Pd.hDC);
	DeleteObject(Bitmap);
	DeleteObject(hReplaced);
	DeleteDC(Pd.hDC);

#endif

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutLoadResource(HINSTANCE hInst, ILint ID, const ILstring ResourceType, ILenum Type)
{
	HRSRC Resource = (HRSRC)LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(ID), ResourceType));
	ILubyte *Data = (ILubyte*)LockResource(Resource);

	return ilLoadL(Type, Data, SizeofResource(hInst, FindResource(hInst, MAKEINTRESOURCE(ID), ResourceType)));
}


#if !defined(_WIN32_WCE) && !(defined(_WIN32) && defined(__GNUC__))
#define BUFFSIZE 8192  // Change to suit the efficiency.
ILboolean ILAPIENTRY ilutWinLoadUrl(const ILstring Url)
{
	HINTERNET	Handle, UrlHandle;
	DWORD		BytesRead = 0, Context = 1;
	ILubyte		Buff[BUFFSIZE], *Buffer, *TempBuff;
	ILuint		BufferSize = 0, i;
	ILboolean	Is404 = IL_TRUE;
	char		Buffer404[] = { '<', 'h', 't', 'm', 'l', '>' };

	Buffer = (ILubyte*)ialloc(0);
	if (Buffer == NULL) {
		return IL_FALSE;
	}

	Handle = InternetOpen("Developer's Image Library", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (Handle == NULL) {
		ifree(Buffer);
		ilSetError(ILUT_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	// Try again if fails the first time, loading only from the cache.
	UrlHandle = InternetOpenUrl(Handle, Url, NULL, 0, 0, Context);
	if (UrlHandle == NULL) {
		InternetCloseHandle(Handle);
		Handle = InternetOpen("Developer's Image Library", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_FROM_CACHE);
		if (Handle == NULL) {
			ifree(Buffer);
			ilSetError(ILUT_COULD_NOT_OPEN_FILE);
			return IL_FALSE;
		}
		UrlHandle = InternetOpenUrl(Handle, Url, NULL, 0, 0, Context);
		if (UrlHandle == NULL) {
			ifree(Buffer);
			InternetCloseHandle(Handle);
			ilSetError(ILUT_COULD_NOT_OPEN_FILE);
			return IL_FALSE;
		}
	}

	do {
		if (!InternetReadFile(UrlHandle, Buff, BUFFSIZE, &BytesRead)) {
			InternetCloseHandle(UrlHandle);
			InternetCloseHandle(Handle);
			ifree(Buffer);
			ilSetError(ILUT_COULD_NOT_OPEN_FILE);
			return IL_FALSE;
		}

		TempBuff = (ILubyte*)ialloc(BufferSize + BytesRead);
		if (TempBuff == NULL) {
			ifree(Buffer);
			return IL_FALSE;
		}

		memcpy(TempBuff, Buffer, BufferSize);
		memcpy(TempBuff + BufferSize, Buff, BytesRead);
		ifree(Buffer);
		Buffer = TempBuff;

		BufferSize += BytesRead;
	} while (BytesRead > 0);

	InternetCloseHandle(UrlHandle);
	InternetCloseHandle(Handle);

	// If the image does not exist, the server usually returns a 404 HTML page.
	for (i = 0; i < sizeof(Buffer404) && i < BufferSize; i++) {
		if (tolower(Buffer[i]) != Buffer404[i]) {
			Is404 = IL_FALSE;
			break;
		}
	}

	if (!Is404) {
		if (!ilLoadL(ilTypeFromExt(Url), Buffer, BufferSize)) {
			if (!ilLoadL(IL_TYPE_UNKNOWN, Buffer, BufferSize)) {
				ifree(Buffer);
				return IL_FALSE;
			}
		}
	}

	ifree(Buffer);

	return IL_TRUE;
}
#endif


#endif//ILUT_USE_WIN32
