//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/pcd.c
//
// Description: Reads from a Kodak PhotoCD (.pcd) file.
//		Note:  The code here is sloppy - I had to convert it from Pascal,
//				which I've never even attempted to read before...enjoy! =)
//				I have nooooooo idea if it works...don't have a .pcd!
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_PCD
#include "il_manip.h"


ILboolean iLoadPcdInternal(ILuint PicNum);

//! Reads a .pcd file
ILboolean ilLoadPcd(const ILstring FileName, ILuint PicNum)
{
	ILHANDLE	PcdFile;
	ILboolean	bPcd = IL_FALSE;

	PcdFile = iopenr(FileName);
	if (PcdFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPcd;
	}

	bPcd = ilLoadPcdF(PcdFile, PicNum);
	icloser(PcdFile);

	return bPcd;
}


//! Reads an already-opened .pcd file
ILboolean ilLoadPcdF(ILHANDLE File, ILuint PicNum)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPcdInternal(PicNum);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .pcd file
ILboolean ilLoadPcdL(ILvoid *Lump, ILuint Size, ILuint PicNum)
{
	iSetInputLump(Lump, Size);
	return iLoadPcdInternal(PicNum);
}


ILvoid YCbCr2RGB(ILubyte Y, ILubyte Cb, ILubyte Cr, ILubyte *r, ILubyte *g, ILubyte *b)
{
	static const ILdouble c11 = 0.0054980*256;
	static const ILdouble c12 = 0.0000000*256;
	static const ILdouble c13 = 0.0051681*256;
	static const ILdouble c21 = 0.0054980*256;
	static const ILdouble c22 =-0.0015446*256;
	static const ILdouble c23 =-0.0026325*256;
	static const ILdouble c31 = 0.0054980*256;
	static const ILdouble c32 = 0.0079533*256;
	static const ILdouble c33 = 0.0000000*256;
	static ILint r1, g1, b1;

	r1 = (ILint)(c11*Y + c12*(Cb-156) + c13*(Cr-137));
	g1 = (ILint)(c21*Y + c22*(Cb-156) + c23*(Cr-137));
	b1 = (ILint)(c31*Y + c32*(Cb-156) + c33*(Cr-137));

	if (r1 < 0)
		*r = 0;
	else if (r1 > 255)
		*r = 255;
	else
		*r = r1;

	if (g1 < 0)
		*g = 0;
	else if (g1 > 255)
		*g = 255;
	else
		*g = g1;

	if (b1 < 0)
		*b = 0;
	else if (b1 > 255)
		*b = 255;
	else
		*b = b1;

	return;
}


ILboolean iLoadPcdInternal(ILuint PicNum)
{
	ILenum	VertOrientation;
	ILuint	Width, Height, i, Total, x, CurPos = 0;
	ILubyte	*Y1, *Y2, *CbCr, r = 0, g = 0, b = 0;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iseek(72, IL_SEEK_CUR);
	iread(&VertOrientation, 1, 1);

	iseek(-72, IL_SEEK_CUR);  // Can't rewind

	switch (PicNum)
	{
		case 0:
			iseek(0x02000, IL_SEEK_CUR);
			Width = 192;
			Height = 128;
			break;
		case 1:
			iseek(0x0b800, IL_SEEK_CUR);
			Width = 384;
			Height = 256;
			break;
		case 2:
			iseek(0x30000, IL_SEEK_CUR);
			Width = 768;
			Height = 512;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}

	Y1 = (ILubyte*)ialloc(Width);
	Y2 = (ILubyte*)ialloc(Width);
	CbCr = (ILubyte*)ialloc(Width);
	if (Y1 == NULL || Y2 == NULL || CbCr == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	if (!ilTexImage(Width, Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL)) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	Total = Height >> 1;
	for (i = 0; i < Total; i++) {
		iread(Y1, 1, Width);
		iread(Y2, 1, Width);
		iread(CbCr, 1, Width);

		for (x = 0; x < Width; x++) {
			YCbCr2RGB(Y1[x], CbCr[x / 2], CbCr[(Width / 2) + (x / 2)], &r, &g, &b);
			iCurImage->Data[CurPos++] = r;
			iCurImage->Data[CurPos++] = g;
			iCurImage->Data[CurPos++] = b;
		}

		for (x = 0; x < Width; x++) {
			YCbCr2RGB(Y2[x], CbCr[x / 2], CbCr[(Width / 2) + (x / 2)], &r, &g, &b);
			iCurImage->Data[CurPos++] = r;
			iCurImage->Data[CurPos++] = g;
			iCurImage->Data[CurPos++] = b;
		}
	}

	ifree(Y1);
	ifree(Y2);
	ifree(CbCr);

	// Not sure how it is...the documentation is hard to understand
	if (!VertOrientation)
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	ilFixImage();

	return IL_TRUE;
}


#endif//IL_NO_PCD
