//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 08/26/2001 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_rle.c
//
// Description: Functions for run-length encoding
//
//-----------------------------------------------------------------------------

// RLE code from TrueVision's TGA sample code available as Tgautils.zip at
//	ftp://ftp.truevision.com/pub/TGA.File.Format.Spec/PC.Version


#include "il_internal.h"
#include "il_rle.h"

INLINE ILuint GetPix(ILubyte *p, ILuint bpp) {
	ILuint Pixel;
	Pixel = (ILuint)*p++;
	
	/*
	switch(bpp) {
		case 1:
			// direct byte access
		case 2:
			// direct short access
		case 3:
			// short + byte
		case 4:
			// direct int access
	}
	Can be optimized, there are just 4 different values.
	1,2,3,4
	
	*/
	
	while( bpp-- > 1 ) {
		Pixel <<= 8;
		Pixel |= (ILuint)*p++;
	}
	return Pixel;
}


INLINE ILint CountDiffPixels(ILubyte *p, ILuint bpp, ILuint pixCnt) {
	ILuint	pixel;
	ILuint	nextPixel = 0;
	ILint	n;

	n = 0;
	if (pixCnt == 1)
		return pixCnt;
	pixel = GetPix(p, bpp);

	while (pixCnt > 1) {
		p += bpp;
		nextPixel = GetPix(p, bpp);
		if (nextPixel == pixel)
			break;
		pixel = nextPixel;
		++n;
		--pixCnt;
	}

	if (nextPixel == pixel)
		return n;
	return n + 1;
}


INLINE ILint CountSamePixels(ILubyte *p, ILuint bpp, ILuint pixCnt) {
	ILuint	pixel;
	ILuint	nextPixel;
	ILint	n;

	n = 1;
	pixel = GetPix(p, bpp);
	pixCnt--;

	while (pixCnt > 0) {
		p += bpp;
		nextPixel = GetPix(p, bpp);
		if (nextPixel != pixel)
			break;
		++n;
		--pixCnt;
	}

	return n;
}


ILboolean ilRleCompressLine(ILubyte *p, ILuint n, ILubyte bpp,
			ILubyte *q, ILuint *DestWidth, ILenum CompressMode) {
	
	ILint		DiffCount;		// pixel count until two identical
	ILint		SameCount;		// number of identical adjacent pixels
	ILint		RLEBufSize;		// count of number of bytes encoded
	ILint		MaxRun;
	const ILint bmp_pad_to_even = 1 - ((long)q - *DestWidth) % 2;

	switch( CompressMode ) {
		case IL_TGACOMP:
			MaxRun = TGA_MAX_RUN;
			break;
		case IL_SGICOMP:
			MaxRun = SGI_MAX_RUN;
			break;
		case IL_BMPCOMP:
			MaxRun = BMP_MAX_RUN;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}

	RLEBufSize = 0;

	while (n > 0) {
		
		// Analyze pixels
		DiffCount = CountDiffPixels(p, bpp, n);
		SameCount = CountSamePixels(p, bpp, n);
		if( DiffCount > MaxRun ) DiffCount = MaxRun;
		if( SameCount > MaxRun ) SameCount = MaxRun;

		if( CompressMode == IL_BMPCOMP ) {
			ILint remaining_data = n-(DiffCount > SameCount ? DiffCount : SameCount);
			if( remaining_data < 3 ) { // check is the end of line
				if( DiffCount > 0 ) {
					if( n < MaxRun ) {
						// end line merging last bytes in another absolute run
						DiffCount = n;
					} else {
						// must limit the line
						DiffCount = n-3;
					}
				} else { // Compressed ending
					// no compression if it has not enough data
					if( SameCount < 2 ) {
						// make a run with the compressed run and EOF
						SameCount = 0;
						DiffCount = n;
					} else {
						// limit compressed range
						SameCount -= 3-remaining_data;
					}
				}
			}
		}

		if( DiffCount > 0 ) { // create a raw packet (bmp absolute run)
			switch(CompressMode) {
				case IL_TGACOMP:
					*q++ = (ILbyte)(DiffCount - 1);
					break;
				case IL_BMPCOMP:
					*q++ = 0x00; RLEBufSize++;
					*q++ = (ILbyte)DiffCount;
					break;
				case IL_SGICOMP:
					*q++ = (ILbyte)(DiffCount | 0x80);
					break;
			}
			n -= DiffCount;
			RLEBufSize += (DiffCount * bpp) + 1;

			while( DiffCount > 0 ) {
				switch(bpp) {
					case 4:	*q++ = *p++;
					case 3: *q++ = *p++;
					case 2: *q++ = *p++;
					case 1: *q++ = *p++;
				}
				DiffCount--;
			}
		
			if( CompressMode == IL_BMPCOMP ) {
				if( (long)q % 2 == bmp_pad_to_even ) {
					*q++ = 0x00; // insert padding
				}
			}
		}

		if( SameCount > 1 ) { // create a RLE packet
			switch(CompressMode) {
				case IL_TGACOMP:
					*q++ = (ILbyte)((SameCount - 1) | 0x80);
					break;
				case IL_SGICOMP:
				case IL_BMPCOMP:
					*q++ = (ILbyte)(SameCount);
					break;
			}
			n -= SameCount;
			RLEBufSize += bpp + 1;
			p += (SameCount - 1) * bpp;
			*q++ = *p++;
			switch(bpp) {
				case 4:	*q++ = *p++;
				case 3: *q++ = *p++;
				case 2: *q++ = *p++;
				case 1: *q++ = *p++;
			}
		}
	}

	// write line termination code
	switch(CompressMode) {
		case IL_SGICOMP:
			++RLEBufSize;
			*q++ = 0;
			break;
		case IL_BMPCOMP: 
			*q++ = 0x00; RLEBufSize++;
			*q++ = 0x00; RLEBufSize++;
			break;
	}
	
	*DestWidth = RLEBufSize;
	
	return IL_TRUE;
}


// Compresses an entire image using run-length encoding
ILuint ilRleCompress(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp,
		ILubyte *Dest, ILenum CompressMode, ILuint *ScanTable) {
	ILuint DestW = 0, i, j, LineLen, Bps = Width * Bpp, SizeOfPlane = Width * Height * Bpp;

	memset(ScanTable,0,Height*sizeof(ILuint));
	for( j = 0; j < Depth; j++ ) {
		for( i = 0; i < Height; i++ ) {
			if( ScanTable )
				*ScanTable++ = DestW;
			ilRleCompressLine(Data + j * SizeOfPlane + i * Bps, Width, Bpp, Dest + DestW, &LineLen, CompressMode);
			DestW += LineLen;
		}
	}
	
	if( CompressMode == IL_BMPCOMP ) { // add end of image
		*(Data+DestW) = 0x00; DestW++;
		*(Data+DestW) = 0x01; DestW++;
	}

	return DestW;
}
