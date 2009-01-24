//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/24/2009
//
// Filename: src-IL/src/il_size.c
//
// Description: Determines the size of output files for lump writing.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"


ILuint CurPos;  // Fake "file" pointer.
ILuint MaxPos;


//! Fake seek function
ILint ILAPIENTRY iSizeSeek(ILint Offset, ILuint Mode)
{
	switch (Mode)
	{
		case IL_SEEK_SET:
			CurPos = Offset;
			if (CurPos > MaxPos)
				MaxPos = CurPos;
			break;

		case IL_SEEK_CUR:
			CurPos = CurPos + Offset;
			break;

		case IL_SEEK_END:
			CurPos = MaxPos + Offset;  // Offset should be negative in this case.
			break;

		default:
			ilSetError(IL_INTERNAL_ERROR);  // Should never happen!
			return -1;  // Error code
	}

	if (CurPos > MaxPos)
		MaxPos = CurPos;

	return 0;  // Code for success
}

ILint ILAPIENTRY iSizeTell(void)
{
	return CurPos;
}

ILint ILAPIENTRY iSizePutc(ILubyte Char)
{
	CurPos++;
	if (CurPos > MaxPos)
		MaxPos = CurPos;
	return Char;
}

ILint ILAPIENTRY iSizeWrite(const void *Buffer, ILuint Size, ILuint Number)
{
	CurPos += Size * Number;
	if (CurPos > MaxPos)
		MaxPos = CurPos;
	return Number;
}


//@TODO: Do computations for uncompressed formats without going through the
//       whole writing process.

//! Returns the size of the memory buffer needed to save the current image into this Type.
//  A return value of 0 is an error.
ILuint ilDetermineSize(ILenum Type)
{
	MaxPos = CurPos = 0;
	iSetOutputFake();  // Sets iputc, iwrite, etc. to functions above.

	switch (Type)
	{
		case IL_BMP:
			ilSaveBmpL(NULL, 0);
			break;

		case IL_DDS:
			ilSaveDdsL(NULL, 0);
			break;

		case IL_EXR:
			ilSaveExrL(NULL, 0);
			break;

		case IL_HDR:
			ilSaveHdrL(NULL, 0);
			break;

		case IL_JP2:
			ilSaveJp2L(NULL, 0);
			break;

		case IL_JPG:
			ilSaveJpegL(NULL, 0);
			break;

		case IL_PCX:
			ilSavePcxL(NULL, 0);
			break;

		case IL_PNG:
			ilSavePngL(NULL, 0);
			break;

		case IL_PNM:
			ilSavePnmL(NULL, 0);
			break;

		case IL_PSD:
			ilSavePsdL(NULL, 0);
			break;

		case IL_RAW:
			ilSaveRawL(NULL, 0);
			break;

		case IL_SGI:
			ilSaveSgiL(NULL, 0);
			break;

		case IL_TGA:
			ilSaveTargaL(NULL, 0);
			break;

		case IL_TIF:
			ilSaveTiffL(NULL, 0);
			break;

		case IL_WBMP:
			ilSaveWbmpL(NULL, 0);
			break;

		default:
			// 0 is an error for this.
			ilSetError(IL_INVALID_PARAM);
			return 0;
	}

	return MaxPos;
}