//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 200005/25/2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openilu/manip.c
//
// Description: Manipulates an image in several ways.
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_states.h"
#include <float.h>
#include <limits.h>
#include "ilu_alloc.h"


ILboolean iluCrop2D(ILuint XOff, ILuint YOff, ILuint Width, ILuint Height)
{
	ILuint	x, y, c, OldBps;
	ILubyte	*Data;
	ILenum	Origin;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Data = (ILubyte*)malloc(iCurImage->SizeOfData);
	if (Data == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	// Uh-oh, what about 0 dimensions?!
	if (Width > iCurImage->Width || Height > iCurImage->Height) {
		free(Data);
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	OldBps = iCurImage->Bps;
	Origin = iCurImage->Origin;
	ilCopyPixels(0, 0, 0, iCurImage->Width, iCurImage->Height, 1, iCurImage->Format, iCurImage->Type, Data);
	if (!ilTexImage(Width, Height, iCurImage->Depth, iCurImage->Bpp, iCurImage->Format, iCurImage->Type, NULL)) {
		free(Data);
		return IL_FALSE;
	}
	iCurImage->Origin = Origin;

	// @TODO:  Optimize!  (Especially XOff * iCurImage->Bpp...get rid of it!)
	for (y = 0; y < iCurImage->Height; y++) {
		for (x = 0; x < iCurImage->Bps; x += iCurImage->Bpp) {
			for (c = 0; c < iCurImage->Bpp; c++) {
				iCurImage->Data[y * iCurImage->Bps + x + c] = 
					Data[(y + YOff) * OldBps + x + XOff * iCurImage->Bpp + c];
			}
		}
	}

	free(Data);

	return IL_TRUE;
}


ILboolean iluCrop3D(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth)
{
	ILuint	x, y, z, c, OldBps, OldPlane;
	ILubyte	*Data;
	ILenum	Origin;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Data = (ILubyte*)malloc(iCurImage->SizeOfData);
	if (Data == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	// Uh-oh, what about 0 dimensions?!
	if (Width > iCurImage->Width || Height > iCurImage->Height || Depth > iCurImage->Depth) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		free(Data);
		return IL_FALSE;
	}

	OldBps = iCurImage->Bps;
	OldPlane = iCurImage->SizeOfPlane;
	Origin = iCurImage->Origin;
	ilCopyPixels(0, 0, 0, iCurImage->Width, iCurImage->Height, iCurImage->Depth, iCurImage->Format, iCurImage->Type, Data);
	if (!ilTexImage(Width - XOff, Height - YOff, Depth - ZOff, iCurImage->Bpp, iCurImage->Format, iCurImage->Type, NULL)) {
		free(Data);
	}
	iCurImage->Origin = Origin;

	for (z = 0; z < iCurImage->Depth; z++) {
		for (y = 0; y < iCurImage->Height; y++) {
			for (x = 0; x < iCurImage->Bps; x += iCurImage->Bpp) {
				for (c = 0; c < iCurImage->Bpp; c++) {
					iCurImage->Data[z * iCurImage->SizeOfPlane + y * iCurImage->Bps + x + c] = 
						Data[(z + ZOff) * OldPlane + (y + YOff) * OldBps + (x + XOff) + c];
				}
			}
		}
	}

	free(Data);

	return IL_TRUE;
}


ILboolean ILAPIENTRY iluCrop(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth)
{
	if (ZOff <= 1)
		return iluCrop2D(XOff, YOff, Width, Height);
	return iluCrop3D(XOff, YOff, ZOff, Width, Height, Depth);
}


//! Enlarges the canvas
ILboolean ILAPIENTRY iluEnlargeCanvas(ILuint Width, ILuint Height, ILuint Depth)
{
	ILubyte	*Data/*, Clear[4]*/;
	ILuint	x, y, z, OldBps, OldH, OldD, OldPlane, AddX, AddY;
	ILenum	Origin;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// Uh-oh, what about 0 dimensions?!
	if (Width < iCurImage->Width || Height < iCurImage->Height || Depth < iCurImage->Depth) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Origin == IL_ORIGIN_LOWER_LEFT) {
		switch (iluPlacement)
		{
			case ILU_LOWER_LEFT:
				AddX = 0;
				AddY = 0;
				break;
			case ILU_LOWER_RIGHT:
				AddX = Width - iCurImage->Width;
				AddY = 0;
				break;
			case ILU_UPPER_LEFT:
				AddX = 0;
				AddY = Height - iCurImage->Height;
				break;
			case ILU_UPPER_RIGHT:
				AddX = Width - iCurImage->Width;
				AddY = Height - iCurImage->Height;
				break;
			case ILU_CENTER:
				AddX = (Width - iCurImage->Width) >> 1;
				AddY = (Height - iCurImage->Height) >> 1;
				break;
			default:
				ilSetError(ILU_INVALID_PARAM);
				return IL_FALSE;
		}
	}
	else {  // IL_ORIGIN_UPPER_LEFT
		switch (iluPlacement)
		{
			case ILU_LOWER_LEFT:
				AddX = 0;
				AddY = Height - iCurImage->Height;
				break;
			case ILU_LOWER_RIGHT:
				AddX = Width - iCurImage->Width;
				AddY = Height - iCurImage->Height;
				break;
			case ILU_UPPER_LEFT:
				AddX = 0;
				AddY = 0;
				break;
			case ILU_UPPER_RIGHT:
				AddX = Width - iCurImage->Width;
				AddY = 0;
				break;
			case ILU_CENTER:
				AddX = (Width - iCurImage->Width) >> 1;
				AddY = (Height - iCurImage->Height) >> 1;
				break;
			default:
				ilSetError(ILU_INVALID_PARAM);
				return IL_FALSE;
		}
	}

	AddX *= iCurImage->Bpp;

	Data = (ILubyte*)malloc(iCurImage->SizeOfData);
	if (Data == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	// Preserve old data.
	OldPlane = iCurImage->SizeOfPlane;
	OldBps   = iCurImage->Bps;
	OldH     = iCurImage->Height;
	OldD     = iCurImage->Depth;
	Origin   = iCurImage->Origin;
	ilCopyPixels(0, 0, 0, iCurImage->Width, iCurImage->Height, OldD, iCurImage->Format, iCurImage->Type, Data);

	ilTexImage(Width, Height, Depth, iCurImage->Bpp, iCurImage->Format, iCurImage->Type, NULL);
	iCurImage->Origin = Origin;

	ilClearImage();
	/*ilGetClear(Clear);
	if (iCurImage->Bpp == 1) {
		memset(iCurImage->Data, Clear[3], iCurImage->SizeOfData);
	}
	else {
		for (x = 0; x < iCurImage->SizeOfData; x += iCurImage->Bpp) {
			for (y = 0; y < iCurImage->Bpp; y++) {
				iCurImage->Data[y] = Clear[y];
			}
		}
	}*/

	for (z = 0; z < OldD; z++) {
		for (y = 0; y < OldH; y++) {
			for (x = 0; x < OldBps; x++) {
				iCurImage->Data[z * iCurImage->SizeOfPlane + (y + AddY) * iCurImage->Bps + x + AddX] =
					Data[z * OldPlane + y * OldBps + x];
			}
		}
	}

	free(Data);

	return IL_TRUE;
}


//! Flips an image over its x axis
ILboolean ILAPIENTRY iluFlipImage()
{
	ILubyte *StartPtr, *EndPtr;
	ILuint x, y, d;
	ILubyte *Flipped;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Flipped = (ILubyte*)malloc(iCurImage->SizeOfData);
	if (Flipped == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	for (d = 0; d < iCurImage->Depth; d++) {
		StartPtr = Flipped + d * iCurImage->SizeOfPlane;
		EndPtr = iCurImage->Data + d * iCurImage->SizeOfPlane + iCurImage->SizeOfPlane;

		for (x = 0; x < iCurImage->Height; x++) {
			EndPtr -= iCurImage->Bps;
			for (y = 0; y < iCurImage->Bps; y++) {
				StartPtr[y] = EndPtr[y];
			}
			StartPtr += iCurImage->Bps;
		}
	}

	free(iCurImage->Data);
	iCurImage->Data = Flipped;

	return IL_TRUE;
}


//! Mirrors an image over its y axis
ILboolean ILAPIENTRY iluMirror()
{
	ILubyte		*Data, *DataPtr, *Temp;
	ILuint		y, d, PixLine;
	ILint		x, c;
	ILushort	*ShortPtr, *TempShort;
	ILuint		*IntPtr, *TempInt;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Data = (ILubyte*)malloc(iCurImage->SizeOfData);
	if (Data == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	PixLine = iCurImage->Bps / iCurImage->Bpc;
	switch (iCurImage->Bpc)
	{
		case 1:
			Temp = iCurImage->Data;
			for (d = 0; d < iCurImage->Depth; d++) {
				DataPtr = Data + d * iCurImage->SizeOfPlane;
				for (y = 0; y < iCurImage->Height; y++) {
					for (x = iCurImage->Width - 1; x >= 0; x--) {
						for (c = 0; c < iCurImage->Bpp; c++, Temp++) {
							DataPtr[y * PixLine + x * iCurImage->Bpp + c] = *Temp;
						}
					}
				}
			}
			break;

		case 2:
			TempShort = (ILushort*)iCurImage->Data;
			for (d = 0; d < iCurImage->Depth; d++) {
				ShortPtr = (ILushort*)(Data + d * iCurImage->SizeOfPlane);
				for (y = 0; y < iCurImage->Height; y++) {
					for (x = iCurImage->Width - 1; x >= 0; x--) {
						for (c = 0; c < iCurImage->Bpp; c++, TempShort++) {
							ShortPtr[y * PixLine + x * iCurImage->Bpp + c] = *TempShort;
						}
					}
				}
			}
			break;

		case 4:
			TempInt = (ILuint*)iCurImage->Data;
			for (d = 0; d < iCurImage->Depth; d++) {
				IntPtr = (ILuint*)(Data + d * iCurImage->SizeOfPlane);
				for (y = 0; y < iCurImage->Height; y++) {
					for (x = iCurImage->Width - 1; x >= 0; x--) {
						for (c = 0; c < iCurImage->Bpp; c++, TempInt++) {
							IntPtr[y * PixLine + x * iCurImage->Bpp + c] = *TempInt;
						}
					}
				}
			}
			break;
	}

	free(iCurImage->Data);
	iCurImage->Data = Data;

	return IL_TRUE;
}


//! Inverts the colours in the image
ILboolean ILAPIENTRY iluNegative()
{
	ILuint		i, j, *IntPtr, NumPix;
	ILubyte		*Data;
	ILushort	*ShortPtr;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		if (!iCurImage->Pal.Palette || !iCurImage->Pal.PalSize || iCurImage->Pal.PalType == IL_PAL_NONE) {
			ilSetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
		}
		Data = iCurImage->Pal.Palette;
		i = iCurImage->Pal.PalSize;
	}
	else {
		Data = iCurImage->Data;
		i = iCurImage->SizeOfData;
	}

	
	// @TODO:  Optimize this some.

	NumPix = i / iCurImage->Bpc;
	switch (iCurImage->Bpc)
	{
		case 1:
			for (j = 0; j < NumPix; j++, Data++) {
				*(Data) = ~*(Data);
			}
			break;

		case 2:
			ShortPtr = (ILushort*)Data;
			for (j = 0; j < NumPix; j++, ShortPtr++) {
				*(ShortPtr) = ~*(ShortPtr);
			}
			break;

		case 4:
			IntPtr = (ILuint*)Data;
			for (j = 0; j < NumPix; j++, IntPtr++) {
				*(IntPtr) = ~*(IntPtr);
			}
			break;
	}

	return IL_TRUE;
}


// Taken from
//	http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue2.html#Insight
//	Hope they don't mind too much. =]
ILboolean ILAPIENTRY iluWave(ILfloat Angle)
{
	ILint	Delta;
	ILuint	y;
	ILubyte	*DataPtr, *TempBuff;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	TempBuff = malloc(iCurImage->SizeOfData);
	if (TempBuff == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	for (y = 0; y < iCurImage->Height; y++) {
		Delta = (ILint)
			(30 * sin((10 * Angle + y) * IL_DEGCONV) +
			 15 * sin(( 7 * Angle + 3 * y) * IL_DEGCONV));

		DataPtr = iCurImage->Data + y * iCurImage->Bps;

		if (Delta < 0) {
			Delta = -Delta;
			memcpy(TempBuff, DataPtr, iCurImage->Bpp * Delta);
			memcpy(DataPtr, DataPtr + iCurImage->Bpp * Delta, iCurImage->Bpp * (iCurImage->Width - Delta));
			memcpy(DataPtr + iCurImage->Bpp * (iCurImage->Width - Delta), TempBuff, iCurImage->Bpp * Delta);
		}
		else if (Delta > 0) {
			memcpy(TempBuff, DataPtr, iCurImage->Bpp * (iCurImage->Width - Delta));
			memcpy(DataPtr, DataPtr + iCurImage->Bpp * (iCurImage->Width - Delta), iCurImage->Bpp * Delta);
			memcpy(DataPtr + iCurImage->Bpp * Delta, TempBuff, iCurImage->Bpp * (iCurImage->Width - Delta));
		}
	}

	free(TempBuff);

	return IL_TRUE;
}


// Swaps the colour order of the current image (rgb(a)->bgr(a) or vice-versa).
//	Must be either an 8, 24 or 32-bit (coloured) image (or palette).
ILboolean ILAPIENTRY iluSwapColours()
{
	ILuint	i = 0, Size;
	ILbyte	Temp, PalBpp;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Size = iCurImage->Bps * iCurImage->Height;
	PalBpp = ilGetBppPal(iCurImage->Pal.PalType);

	if ((iCurImage->Bpp != 1 && iCurImage->Bpp != 3 && iCurImage->Bpp != 4)
		|| (PalBpp == 2)) {
		ilSetError(ILU_INVALID_VALUE);
		return IL_FALSE;
	}

	if (iCurImage->Bpp == 1) {
		if (PalBpp == 0 || iCurImage->Format != IL_COLOUR_INDEX) {
			ilSetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
		}

		for (; i < iCurImage->Pal.PalSize; i += PalBpp) {
			Temp = iCurImage->Pal.Palette[i];
			iCurImage->Pal.Palette[i] = iCurImage->Pal.Palette[i+2];
			iCurImage->Pal.Palette[i+2] = Temp;
		}
	}
	else {
		for (; i < Size; i += iCurImage->Bpp) {
			Temp = iCurImage->Data[i];
			iCurImage->Data[i] = iCurImage->Data[i+2];
			iCurImage->Data[i+2] = Temp;
		}
	}

	// @TODO:  Change palette format!
	switch (iCurImage->Format)
	{
		case IL_RGB:
			iCurImage->Format = IL_BGR;
			break;
		case IL_RGBA:
			iCurImage->Format = IL_BGRA;
			break;
		case IL_BGR:
			iCurImage->Format = IL_RGB;
			break;
		case IL_BGRA:
			iCurImage->Format = IL_RGBA;
			break;
	}

	return IL_TRUE;
}


typedef struct BUCKET { ILubyte Colours[4];  struct BUCKET *Next; } BUCKET;

ILuint ILAPIENTRY iluColoursUsed()
{
	ILuint i, c, Bpp, ColVal, SizeData, BucketPos = 0, NumCols = 0;
	BUCKET Buckets[8192], *Temp;
	ILubyte ColTemp[4];
	ILboolean Matched;
	BUCKET *Heap[9];
	ILuint HeapPos = 0, HeapPtr = 0, HeapSize;

	memset(Buckets, 0, sizeof(BUCKET) * 8192);
	for (c = 0; c < 9; c++) {
		Heap[c] = 0;
	}

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	Bpp = iCurImage->Bpp;
	SizeData = iCurImage->SizeOfData;

	// Create our miniature memory heap.
	// I have determined that the average number of colours versus
	//	the number of pixels is about a 1:8 ratio, so divide by 8.
	HeapSize = iCurImage->SizeOfData / iCurImage->Bpp / 8;
	Heap[0] = (BUCKET*)malloc(HeapSize * sizeof(BUCKET));

	for (i = 0; i < SizeData; i += Bpp) {
		*(ILuint*)ColTemp = 0;
		/*for (c = 0; c < Bpp; c++) {
			ColTemp[c] = iCurImage->Data[i + c];
		}*/
		ColTemp[0] = iCurImage->Data[i];
		if (Bpp > 1) {
			ColTemp[1] = iCurImage->Data[i + 1];
			ColTemp[2] = iCurImage->Data[i + 2];
		}
		if (Bpp > 3)
			ColTemp[3] = iCurImage->Data[i + 3];

		BucketPos = *(ILuint*)ColTemp % 8192;

		// Add to hash table
		if (Buckets[BucketPos].Next == NULL) {
			NumCols++;
			//Buckets[BucketPos].Next = (BUCKET*)malloc(sizeof(BUCKET));
			Buckets[BucketPos].Next = Heap[HeapPos] + HeapPtr++;
			if (HeapPtr >= HeapSize) {
				Heap[++HeapPos] = (BUCKET*)malloc(HeapSize * sizeof(BUCKET));
				HeapPtr = 0;
			}
			*(ILuint*)Buckets[BucketPos].Next->Colours = *(ILuint*)ColTemp;
			Buckets[BucketPos].Next->Next = NULL;
		}
		else {
			Matched = IL_FALSE;
			Temp = Buckets[BucketPos].Next;

			ColVal = *(ILuint*)ColTemp;
			while (Temp->Next != NULL) {
				if (ColVal == *(ILuint*)Temp->Colours) {
					Matched = IL_TRUE;
					break;
				}
				Temp = Temp->Next;
			}
			if (!Matched) {
				if (ColVal != *(ILuint*)Temp->Colours) {  // Check against last entry
					NumCols++;
					Temp = Buckets[BucketPos].Next;
					//Buckets[BucketPos].Next = (BUCKET*)malloc(sizeof(BUCKET));
					Buckets[BucketPos].Next = Heap[HeapPos] + HeapPtr++;
					if (HeapPtr >= HeapSize) {
						Heap[++HeapPos] = (BUCKET*)malloc(HeapSize * sizeof(BUCKET));
						HeapPtr = 0;
					}
					Buckets[BucketPos].Next->Next = Temp;
					*(ILuint*)Buckets[BucketPos].Next->Colours = *(ILuint*)ColTemp;
				}
			}
		}
	}

	// Delete our mini heap.
	for (i = 0; i < 9; i++) {
		if (Heap[i] == NULL)
			break;
		free(Heap[i]);
	}

	return NumCols;
}


ILboolean ILAPIENTRY iluCompareImage(ILuint Comp)
{
	ILimage		*Original;
	ILuint		OrigName, i;
	ILboolean	Same = IL_TRUE;

	Original = iCurImage = ilGetCurImage();
	OrigName = ilGetCurName();

	// Same image, so return true.
	if (ilGetCurName() == Comp)
		return IL_TRUE;

	if (iCurImage == NULL || ilIsImage(Comp) == IL_FALSE) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	ilBindImage(Comp);


	// @TODO:  Should we check palettes, too?
	if (Original->Bpp != iCurImage->Bpp ||
		Original->Depth != iCurImage->Bpp ||
		Original->Format != iCurImage->Format ||
		Original->Height != iCurImage->Height ||
		Original->Origin != iCurImage->Origin ||
		Original->Type != iCurImage->Type ||
		Original->Width != iCurImage->Width) {
			ilBindImage(OrigName);
			return IL_FALSE;
	}

	for (i = 0; i < iCurImage->SizeOfData; i++) {
		if (Original->Data[i] != iCurImage->Data[i]) {
			Same = IL_FALSE;
			break;
		}
	}

	ilBindImage(OrigName);
	return Same;
}


// @TODO:  FIX ILGETCLEARCALL!
ILboolean ILAPIENTRY iluReplaceColour(ILubyte Red, ILubyte Green, ILubyte Blue, ILfloat Tolerance)
{
	ILubyte	ClearCol[4];
	ILint	TolVal, Distance, Dist1, Dist2, Dist3;
	ILuint	i, NumPix;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	ilGetClear(ClearCol, IL_RGBA, IL_UNSIGNED_BYTE);
	if (Tolerance > 1.0f || Tolerance < -1.0f)
		Tolerance = 1.0f;  // Clamp it.
	TolVal = (ILuint)(fabs(Tolerance) * UCHAR_MAX);  // To be changed.
	NumPix = iCurImage->Width * iCurImage->Height * iCurImage->Depth;

	if (Tolerance <= FLT_EPSILON && Tolerance >= 0) {


	}
	else {
		switch (iCurImage->Format)
		{
			case IL_RGB:
			case IL_RGBA:
				for (i = 0; i < iCurImage->SizeOfData; i += iCurImage->Bpp) {
					Dist1 = (ILint)iCurImage->Data[i] - (ILint)ClearCol[0];
					Dist2 = (ILint)iCurImage->Data[i+1] - (ILint)ClearCol[1];
					Dist3 = (ILint)iCurImage->Data[i+2] - (ILint)ClearCol[2];
					Distance = (ILint)sqrt(Dist1 * Dist1 + Dist2 * Dist2 + Dist3 * Dist3);
					if (Distance >= -TolVal && Distance <= TolVal) {
						iCurImage->Data[i] = Red;
						iCurImage->Data[i+1] = Green;
						iCurImage->Data[i+2] = Blue;
					}
				}
				break;
			case IL_BGR:
			case IL_BGRA:
				for (i = 0; i < iCurImage->SizeOfData; i += iCurImage->Bpp) {
					Dist1 = (ILint)iCurImage->Data[i] - (ILint)ClearCol[0];
					Dist2 = (ILint)iCurImage->Data[i+1] - (ILint)ClearCol[1];
					Dist3 = (ILint)iCurImage->Data[i+2] - (ILint)ClearCol[2];
					Distance = (ILint)sqrt(Dist1 * Dist1 + Dist2 * Dist2 + Dist3 * Dist3);
					if (Distance >= -TolVal && Distance <= TolVal) {
						iCurImage->Data[i+2] = Red;
						iCurImage->Data[i+1] = Green;
						iCurImage->Data[i] = Blue;
					}
				}
				break;
			//case IL_LUMINANCE:  // @TODO:  Finish!
			//case IL_COLOUR_INDEX:  // @TODO:  Finish!
		}
	}

	return IL_TRUE;
}


// Credit goes to Lionel Brits for this (refer to credits.txt)
ILboolean ILAPIENTRY iluEqualize()
{
	ILuint	Histogram[256]; // image Histogram
	ILuint	SumHistm[256]; // normalized Histogram and LUT
    ILuint	i = 0; // index variable
	ILuint	j = 0; // index variable
	ILuint	Sum=0;
	ILuint	NumPixels;
	ILint	Intensity;
	ILfloat	Scale;
	ILint	IntensityNew;
	ILimage	*LumImage;
	ILuint	NewColour[4];

	ILubyte		*BytePtr;
	ILushort	*ShortPtr;
	ILuint		*IntPtr;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	NumPixels = iCurImage->Width * iCurImage->Height * iCurImage->Depth;

	// @TODO:  Change to work with other types!
	/*if (iCurImage->Bpc > 1) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}*/

	// Clear the tables.
	memset(Histogram, 0, 256 * sizeof(ILuint));
	memset(SumHistm, 0, 256 * sizeof(ILuint));

	LumImage = iConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	if (LumImage == NULL)
		return IL_FALSE;
	for (i = 0; i < NumPixels; i++) {
		Histogram[LumImage->Data[i]]++;
	}

	// Calculate normalized Sum of Histogram.
	for (i = 0; i < 256; i++) {
		for (j = 0; j < i; j++)
			Sum += Histogram[j];

		SumHistm[i] = (Sum << 8) / NumPixels;
		Sum = 0;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX)
		NumPixels = iCurImage->Pal.PalSize / ilGetBppPal(iCurImage->Pal.PalType);

	BytePtr = (iCurImage->Format == IL_COLOUR_INDEX) ? iCurImage->Pal.Palette : iCurImage->Data;
	ShortPtr = (ILushort*)iCurImage->Data;
	IntPtr = (ILuint*)iCurImage->Data;

	// Transform image using new SumHistm as a LUT
    for (i = 0; i < NumPixels; i++) {
		Intensity = LumImage->Data[i];

		// Look up the normalized intensity
		IntensityNew = (ILint)SumHistm[Intensity];

		// Find out by how much the intensity has been Scaled
		Scale = (ILfloat)IntensityNew / (ILfloat)Intensity;

		switch (iCurImage->Bpc)
		{
			case 1:
				// Calculate new pixel
				NewColour[0] = (ILuint)(BytePtr[i * iCurImage->Bpp] * Scale);
				NewColour[1] = (ILuint)(BytePtr[i * iCurImage->Bpp + 1] * Scale);
				NewColour[2] = (ILuint)(BytePtr[i * iCurImage->Bpp + 2] * Scale);

				// Clamp values
				if (NewColour[0] > UCHAR_MAX)
					NewColour[0] = UCHAR_MAX;
				if (NewColour[1] > UCHAR_MAX)
					NewColour[1] = UCHAR_MAX;
				if (NewColour[2] > UCHAR_MAX)
					NewColour[2] = UCHAR_MAX;

				// Store pixel
				BytePtr[i * iCurImage->Bpp]		= (ILubyte)NewColour[0];
				BytePtr[i * iCurImage->Bpp + 1]	= (ILubyte)NewColour[1];
				BytePtr[i * iCurImage->Bpp + 2]	= (ILubyte)NewColour[2];
				break;

			case 2:
				// Calculate new pixel
				NewColour[0] = (ILuint)(ShortPtr[i * iCurImage->Bpp] * Scale);
				NewColour[1] = (ILuint)(ShortPtr[i * iCurImage->Bpp + 1] * Scale);
				NewColour[2] = (ILuint)(ShortPtr[i * iCurImage->Bpp + 2] * Scale);

				// Clamp values
				if (NewColour[0] > USHRT_MAX)
					NewColour[0] = USHRT_MAX;
				if (NewColour[1] > USHRT_MAX)
					NewColour[1] = USHRT_MAX;
				if (NewColour[2] > USHRT_MAX)
					NewColour[2] = USHRT_MAX;

				// Store pixel
				ShortPtr[i * iCurImage->Bpp]		= (ILushort)NewColour[0];
				ShortPtr[i * iCurImage->Bpp + 1]	= (ILushort)NewColour[1];
				ShortPtr[i * iCurImage->Bpp + 2]	= (ILushort)NewColour[2];
				break;

			case 4:
				// Calculate new pixel
				NewColour[0] = (ILuint)(IntPtr[i * iCurImage->Bpp] * Scale);
				NewColour[1] = (ILuint)(IntPtr[i * iCurImage->Bpp + 1] * Scale);
				NewColour[2] = (ILuint)(IntPtr[i * iCurImage->Bpp + 2] * Scale);

				// Clamp values
				if (NewColour[0] > UINT_MAX)
					NewColour[0] = UINT_MAX;
				if (NewColour[1] > UINT_MAX)
					NewColour[1] = UINT_MAX;
				if (NewColour[2] > UINT_MAX)
					NewColour[2] = UINT_MAX;

				// Store pixel
				IntPtr[i * 4 * iCurImage->Bpp]		= NewColour[0];
				IntPtr[i * 4 * iCurImage->Bpp + 1]	= NewColour[1];
				IntPtr[i * 4 * iCurImage->Bpp + 2]	= NewColour[2];
				break;
		}
	}

	ilCloseImage(LumImage);

	return IL_TRUE;
}
