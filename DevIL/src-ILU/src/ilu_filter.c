//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openilu/filter.c
//
// Description: Applies filters to an image.
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_filter.h"
#include "ilu_alloc.h"
#include <math.h>
#include <limits.h>


// Could probably be optimized a bit...too many nested for loops.
//! Pixelizes an image
ILboolean ILAPIENTRY iluPixelize(ILuint PixSize)
{
	ILuint		x, y, z, i, j, k, c, Total, Tested;
	ILushort	*ShortPtr;
	ILuint		*IntPtr;
	ILdouble	*DblPtr, DblTotal, DblTested;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX)
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);

	switch (iCurImage->Bpc)
	{
		case 1:
			for (z = 0; z < iCurImage->Depth; z += PixSize) {
				for (y = 0; y < iCurImage->Height; y += PixSize) {
					for (x = 0; x < iCurImage->Width; x += PixSize) {
						for (c = 0; c < iCurImage->Bpp; c++) {
							Total = 0;  Tested = 0;
							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, Tested++) {
										Total += iCurImage->Data[(z+k) * iCurImage->SizeOfPlane + 
											(y+j) * iCurImage->Bps + (x+i) * iCurImage->Bpp + c];
									}
								}
							}
							Total /= Tested;

							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, Tested++) {
										iCurImage->Data[(z+k) * iCurImage->SizeOfPlane + (y+j)
											* iCurImage->Bps + (x+i) * iCurImage->Bpp + c] =
											Total;
									}
								}
							}
						}
					}
				}
			}
			break;
		case 2:
			ShortPtr = (ILushort*)iCurImage->Data;
			iCurImage->Bps /= 2;
			for (z = 0; z < iCurImage->Depth; z += PixSize) {
				for (y = 0; y < iCurImage->Height; y += PixSize) {
					for (x = 0; x < iCurImage->Width; x += PixSize) {
						for (c = 0; c < iCurImage->Bpp; c++) {
							Total = 0;  Tested = 0;
							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, Tested++) {
										Total += ShortPtr[(z+k) * iCurImage->SizeOfPlane + 
											(y+j) * iCurImage->Bps + (x+i) * iCurImage->Bpp + c];
									}
								}
							}
							Total /= Tested;

							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, Tested++) {
										ShortPtr[(z+k) * iCurImage->SizeOfPlane + (y+j)
											* iCurImage->Bps + (x+i) * iCurImage->Bpp + c] =
											Total;
									}
								}
							}
						}
					}
				}
			}
			iCurImage->Bps *= 2;
			break;
		case 4:
			IntPtr = (ILuint*)iCurImage->Data;
			iCurImage->Bps /= 4;
			for (z = 0; z < iCurImage->Depth; z += PixSize) {
				for (y = 0; y < iCurImage->Height; y += PixSize) {
					for (x = 0; x < iCurImage->Width; x += PixSize) {
						for (c = 0; c < iCurImage->Bpp; c++) {
							Total = 0;  Tested = 0;
							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, Tested++) {
										Total += IntPtr[(z+k) * iCurImage->SizeOfPlane + 
											(y+j) * iCurImage->Bps + (x+i) * iCurImage->Bpp + c];
									}
								}
							}
							Total /= Tested;

							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, Tested++) {
										IntPtr[(z+k) * iCurImage->SizeOfPlane + (y+j)
											* iCurImage->Bps + (x+i) * iCurImage->Bpp + c] =
											Total;
									}
								}
							}
						}
					}
				}
			}
			iCurImage->Bps *= 4;
			break;
		case 8:
			DblPtr = (ILdouble*)iCurImage->Data;
			iCurImage->Bps /= 8;
			for (z = 0; z < iCurImage->Depth; z += PixSize) {
				for (y = 0; y < iCurImage->Height; y += PixSize) {
					for (x = 0; x < iCurImage->Width; x += PixSize) {
						for (c = 0; c < iCurImage->Bpp; c++) {
							DblTotal = 0.0;  DblTested = 0.0;
							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, DblTested++) {
										DblTotal += DblPtr[(z+k) * iCurImage->SizeOfPlane + 
											(y+j) * iCurImage->Bps + (x+i) * iCurImage->Bpp + c];
									}
								}
							}
							DblTotal /= DblTested;

							for (k = 0; k < PixSize && z+k < iCurImage->Depth; k++) {
								for (j = 0; j < PixSize && y+j < iCurImage->Height; j++) {
									for (i = 0; i < PixSize && x+i < iCurImage->Width; i++, DblTested++) {
										DblPtr[(z+k) * iCurImage->SizeOfPlane + (y+j)
											* iCurImage->Bps + (x+i) * iCurImage->Bpp + c] =
											DblTotal;
									}
								}
							}
						}
					}
				}
			}
			iCurImage->Bps *= 8;
			break;
	}

	return IL_TRUE;
}


// Everything here was taken from a tutorial on "Elementary Digital Filtering"
//	by Ender Wiggen, found at http://www.gamedev.net/reference/programming/features/edf/



// Needs some SERIOUS optimization.
ILubyte *Filter(ILimage *Image, ILushort Filt)
{
    ILint		x, y, c, LastX, LastY, Offsets[9];
	ILuint		i, Temp, z;
	ILubyte		*Data, *ImgData, *NewData;
	ILdouble	Num;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return NULL;
	}

	Data = (ILubyte*)malloc(Image->SizeOfData);
	if (Data == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return NULL;
	}

	Filt *= 11;
	// Preserve original data.
	ImgData = Image->Data;
	NewData = Data;

	for (z = 0; z < Image->Depth; z++) {
		LastX = Image->Width  - 1;
		LastY = Image->Height - 1;
		for (y = 1; y < LastY; y++) {
			for (x = 1; x < LastX; x++) {
				Offsets[0] = ((y-1) * Image->Width + (x-1)) * Image->Bpp;
				Offsets[1] = ((y-1) * Image->Width + (x  )) * Image->Bpp;
				Offsets[2] = ((y-1) * Image->Width + (x+1)) * Image->Bpp;
				Offsets[3] = ((y  ) * Image->Width + (x-1)) * Image->Bpp;
				Offsets[4] = ((y  ) * Image->Width + (x  )) * Image->Bpp;
				Offsets[5] = ((y  ) * Image->Width + (x+1)) * Image->Bpp;
				Offsets[6] = ((y+1) * Image->Width + (x-1)) * Image->Bpp;
				Offsets[7] = ((y+1) * Image->Width + (x  )) * Image->Bpp;
				Offsets[8] = ((y+1) * Image->Width + (x-1)) * Image->Bpp;

				// Always has at least one, so get rid of all those +c's
				Num =   Image->Data[Offsets[0]] * Filters[Filt] +
						Image->Data[Offsets[1]] * Filters[Filt+1]+
						Image->Data[Offsets[2]] * Filters[Filt+2]+
						Image->Data[Offsets[3]] * Filters[Filt+3]+
						Image->Data[Offsets[4]] * Filters[Filt+4]+
						Image->Data[Offsets[5]] * Filters[Filt+5]+
						Image->Data[Offsets[6]] * Filters[Filt+6]+
						Image->Data[Offsets[7]] * Filters[Filt+7]+
						Image->Data[Offsets[8]] * Filters[Filt+8];

				Temp = (ILuint)fabs((Num / (ILdouble)Filters[Filt+9]) + Filters[Filt+10]);
				if (Temp > 255)
					Data[Offsets[4]] = 255;
				else
					Data[Offsets[4]] = Temp;

				for (c = 1; c < Image->Bpp; c++) {
					Num =   Image->Data[Offsets[0]+c] * Filters[Filt] +
							Image->Data[Offsets[1]+c] * Filters[Filt+1]+
							Image->Data[Offsets[2]+c] * Filters[Filt+2]+
							Image->Data[Offsets[3]+c] * Filters[Filt+3]+
							Image->Data[Offsets[4]+c] * Filters[Filt+4]+
							Image->Data[Offsets[5]+c] * Filters[Filt+5]+
							Image->Data[Offsets[6]+c] * Filters[Filt+6]+
							Image->Data[Offsets[7]+c] * Filters[Filt+7]+
							Image->Data[Offsets[8]+c] * Filters[Filt+8];

					Temp = (ILuint)fabs((Num / (ILdouble)Filters[Filt+9]) + Filters[Filt+10]);
					if (Temp > 255)
						Data[Offsets[4]+c] = 255;
					else
						Data[Offsets[4]+c] = Temp;
				}
			}
		}


		// Copy 4 corners
		for (c = 0; c < Image->Bpp; c++) {
			Data[c] = Image->Data[c];
			Data[Image->Bps - Image->Bpp + c] = Image->Data[Image->Bps - Image->Bpp + c];
			Data[(Image->Height - 1) * Image->Bps + c] = Image->Data[(Image->Height - 1) * Image->Bps + c];
			Data[Image->Height * Image->Bps - Image->Bpp + c] = 
				Image->Data[Image->Height * Image->Bps - Image->Bpp + c];
		}


		// If we only copy the edge pixels, then they receive no filtering, making them
		//	look out of place after several passes of an image.  So we filter the edge
		//	rows/columns, duplicating the edge pixels for one side of the "matrix".

		// First row
		for (x = 1; x < (ILint)Image->Width-1; x++) {
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[(x-1) * Image->Bpp + c] * Filters[Filt] +
						Image->Data[x * Image->Bpp + c] * Filters[Filt+1]+
						Image->Data[(x+1) * Image->Bpp + c] * Filters[Filt+2]+
						Image->Data[(x-1) * Image->Bpp + c] * Filters[Filt+3]+
						Image->Data[x * Image->Bpp + c] * Filters[Filt+4]+
						Image->Data[(x+1) * Image->Bpp + c] * Filters[Filt+5]+
						Image->Data[(Image->Width + (x-1)) * Image->Bpp + c] * Filters[Filt+6]+
						Image->Data[(Image->Width + (x  )) * Image->Bpp + c] * Filters[Filt+7]+
						Image->Data[(Image->Width + (x-1)) * Image->Bpp + c] * Filters[Filt+8];

					Temp = (ILuint)fabs((Num / (ILdouble)Filters[Filt+9]) + Filters[Filt+10]);
					if (Temp > 255)
						Data[x * Image->Bpp + c] = 255;
					else
						Data[x * Image->Bpp + c] = Temp;
			}
		}

		// Last row
		y = (Image->Height - 1) * Image->Bps;
		for (x = 1; x < (ILint)Image->Width-1; x++) {
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[y - Image->Bps + (x-1) * Image->Bpp + c] * Filters[Filt] +
						Image->Data[y - Image->Bps + x * Image->Bpp + c] * Filters[Filt+1]+
						Image->Data[y - Image->Bps + (x+1) * Image->Bpp + c] * Filters[Filt+2]+
						Image->Data[y + (x-1) * Image->Bpp + c] * Filters[Filt+3]+
						Image->Data[y + x * Image->Bpp + c] * Filters[Filt+4]+
						Image->Data[y + (x+1) * Image->Bpp + c] * Filters[Filt+5]+
						Image->Data[y + (x-1) * Image->Bpp + c] * Filters[Filt+6]+
						Image->Data[y + x * Image->Bpp + c] * Filters[Filt+7]+
						Image->Data[y + (x-1) * Image->Bpp + c] * Filters[Filt+8];

					Temp = (ILuint)fabs((Num / (ILdouble)Filters[Filt+9]) + Filters[Filt+10]);
					if (Temp > 255)
						Data[y + x * Image->Bpp + c] = 255;
					else
						Data[y + x * Image->Bpp + c] = Temp;
			}
		}

		// Left side
		for (i = 1, y = Image->Bps; i < Image->Height-1; i++, y += Image->Bps) {
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[y - Image->Bps + c] * Filters[Filt] +
						Image->Data[y - Image->Bps + Image->Bpp + c] * Filters[Filt+1]+
						Image->Data[y - Image->Bps + 2 * Image->Bpp + c] * Filters[Filt+2]+
						Image->Data[y + c] * Filters[Filt+3]+
						Image->Data[y + Image->Bpp + c] * Filters[Filt+4]+
						Image->Data[y + 2 * Image->Bpp + c] * Filters[Filt+5]+
						Image->Data[y + Image->Bps + c] * Filters[Filt+6]+
						Image->Data[y + Image->Bps + Image->Bpp + c] * Filters[Filt+7]+
						Image->Data[y + Image->Bps + 2 * Image->Bpp + c] * Filters[Filt+8];

					Temp = (ILuint)fabs((Num / (ILdouble)Filters[Filt+9]) + Filters[Filt+10]);
					if (Temp > 255)
						Data[y + c] = 255;
					else
						Data[y + c] = Temp;
			}
		}

		// Right side
		for (i = 1, y = Image->Bps * 2 - Image->Bpp; i < Image->Height-1; i++, y += Image->Bps) {
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[y - Image->Bps + c] * Filters[Filt] +
						Image->Data[y - Image->Bps + Image->Bpp + c] * Filters[Filt+1]+
						Image->Data[y - Image->Bps + 2 * Image->Bpp + c] * Filters[Filt+2]+
						Image->Data[y + c] * Filters[Filt+3]+
						Image->Data[y + Image->Bpp + c] * Filters[Filt+4]+
						Image->Data[y + 2 * Image->Bpp + c] * Filters[Filt+5]+
						Image->Data[y + Image->Bps + c] * Filters[Filt+6]+
						Image->Data[y + Image->Bps + Image->Bpp + c] * Filters[Filt+7]+
						Image->Data[y + Image->Bps + 2 * Image->Bpp + c] * Filters[Filt+8];

					Temp = (ILuint)fabs((Num / (ILdouble)Filters[Filt+9]) + Filters[Filt+10]);
					if (Temp > 255)
						Data[y + c] = 255;
					else
						Data[y + c] = Temp;
			}
		}

		// Go to next "plane".
		Image->Data += Image->SizeOfPlane;
		Data += Image->SizeOfPlane;
	}

	// Restore original data.
	Image->Data = ImgData;
	Data = NewData;

	return Data;
}


ILboolean ILAPIENTRY iluEdgeDetectP()
{
	ILubyte		*HPass, *VPass;
	ILuint		i;
	ILboolean	Palette = IL_FALSE;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);
	}

	HPass = Filter(iCurImage, 4);
	VPass = Filter(iCurImage, 5);
	if (!HPass || !VPass)
		return IL_FALSE;

	// Combine the two passes
	//	Optimization by Matt Denham
	for (i = 0; i < iCurImage->SizeOfData; i++) {
		if (HPass[i] == 0)
			iCurImage->Data[i] = VPass[i];
		else if (VPass[i] == 0)
			iCurImage->Data[i] = HPass[i];
		else
			iCurImage->Data[i] = (ILubyte)sqrt(HPass[i]*HPass[i]+VPass[i]*VPass[i]);
	}

	/*for (i = 0; i < iCurImage->SizeOfData; i++) {
		iCurImage->Data[i] = (ILubyte)sqrt(HPass[i]*HPass[i]+VPass[i]*VPass[i]);
	}*/
	
	free(HPass);
	free(VPass);

	if (Palette)
		ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);

	return IL_TRUE;
}


ILboolean ILAPIENTRY iluEdgeDetectS()
{
	ILubyte		*HPass, *VPass;
	ILuint		i;
	ILboolean	Palette = IL_FALSE;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);
	}

	HPass = Filter(iCurImage, 2);
	VPass = Filter(iCurImage, 3);
	if (!HPass || !VPass)
		return IL_FALSE;

	// Combine the two passes
	//	Optimization by Matt Denham
	for (i = 0; i < iCurImage->SizeOfData; i++) {
		if (HPass[i] == 0)
			iCurImage->Data[i] = VPass[i];
		else if (VPass[i] == 0)
			iCurImage->Data[i] = HPass[i];
		else
			iCurImage->Data[i] = (ILubyte)sqrt(HPass[i]*HPass[i]+VPass[i]*VPass[i]);
	}

	/*for (i = 0; i < iCurImage->SizeOfData; i++) {
		iCurImage->Data[i] = (ILubyte)sqrt(HPass[i]*HPass[i]+VPass[i]*VPass[i]);
	}*/
	
	free(HPass);
	free(VPass);

	if (Palette)
		ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);

	return IL_TRUE;
}


ILboolean ILAPIENTRY iluBlurAvg(ILuint Iter)
{
	ILubyte		*Data;
	ILuint		i;
	ILboolean	Palette = IL_FALSE;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);
	}

	for (i = 0; i < Iter; i++) {
		Data = Filter(iCurImage, 0);
		if (!Data)
			return IL_FALSE;
		free(iCurImage->Data);
		iCurImage->Data = Data;
	}

	if (Palette)
		ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);

	return IL_TRUE;
}


ILboolean ILAPIENTRY iluBlurGaussian(ILuint Iter)
{
	ILubyte		*Data;
	ILuint		i;
	ILboolean	Palette = IL_FALSE;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);
	}

	for (i = 0; i < Iter; i++) {
		Data = Filter(iCurImage, 1);
		if (!Data)
			return IL_FALSE;
		free(iCurImage->Data);
		iCurImage->Data = Data;
	}

	if (Palette)
		ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);

	return IL_TRUE;
}


ILboolean ILAPIENTRY iluEmboss()
{
	ILubyte		*Data;
	ILboolean	Palette = IL_FALSE;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);
	}

	Data = Filter(iCurImage, 6);
	if (!Data)
		return IL_FALSE;
	free(iCurImage->Data);
	iCurImage->Data = Data;

	if (Palette)
		ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);

	return IL_TRUE;
}

/*ILboolean ILAPIENTRY iluEmboss()
{
	ILuint	x, y, c, i = 0;
	ILubyte	*Data;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Data = (ILubyte*)malloc(iCurImage->SizeOfData);
	if (Data == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	for (y = 0; y < iCurImage->Height; y++, i += iCurImage->Bps) {
		for (x = 0; x < (ILint)iCurImage->Bps - iCurImage->Bpp; x += iCurImage->Bpp) {
			for (c = 0; c < iCurImage->Bpp; c++) {
				Data[i+x+c] = iCurImage->Data[i+x+c] - iCurImage->Data[i+iCurImage->Bpp+c] + 128;
			}
		}
		Data[i+x] = 128;
	}

	free(iCurImage->Data);
	iCurImage->Data = Data;

	return IL_TRUE;
}*/


ILboolean ILAPIENTRY iluEdgeDetectE()
{
	ILubyte		*Data;
	ILboolean	Palette = IL_FALSE;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		ilConvertImage(ilGetPalBaseType(iCurImage->Pal.PalType), IL_UNSIGNED_BYTE);
	}

	Data = Filter(iCurImage, 7);
	if (!Data)
		return IL_FALSE;
	free(iCurImage->Data);
	iCurImage->Data = Data;

	if (Palette)
		ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);

	return IL_TRUE;
}


// Everything here was taken from "Grafica Obscura",
//	found at http://www.sgi.com/grafica/matrix/index.html


// @TODO:  fast float to byte
//! Scales image colours
ILboolean ILAPIENTRY iluScaleColours(ILfloat r, ILfloat g, ILfloat b)
{
	ILuint		i;
	ILint		red, grn, blu, grey;
	ILushort	*ShortPtr;
	ILuint		*IntPtr, NumPix;
	ILdouble	*DblPtr;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	switch (iCurImage->Format)
	{
		case IL_RGB:
		case IL_RGBA:
			for (i = 0; i < iCurImage->SizeOfData; i += iCurImage->Bpp) {
				red = (ILint)(iCurImage->Data[i] * r);
				grn = (ILint)(iCurImage->Data[i+1] * g);
				blu = (ILint)(iCurImage->Data[i+2] * b);
				if (red > UCHAR_MAX) red = UCHAR_MAX;
				if (red < 0) red = 0;
				if (grn > UCHAR_MAX) grn = UCHAR_MAX;
				if (grn < 0) grn = 0;
				if (blu > UCHAR_MAX) blu = UCHAR_MAX;
				if (blu < 0) blu = 0;
				iCurImage->Data[i]   = (ILubyte)red;
				iCurImage->Data[i+1] = (ILubyte)grn;
				iCurImage->Data[i+2] = (ILubyte)blu;
			}
			break;

		case IL_BGR:
		case IL_BGRA:
			for (i = 0; i < iCurImage->SizeOfData; i += iCurImage->Bpp) {
				red = (ILint)(iCurImage->Data[i+2] * r);
				grn = (ILint)(iCurImage->Data[i+1] * g);
				blu = (ILint)(iCurImage->Data[i] * b);
				if (red > UCHAR_MAX) red = UCHAR_MAX;
				if (red < 0) red = 0;
				if (grn > UCHAR_MAX) grn = UCHAR_MAX;
				if (grn < 0) grn = 0;
				if (blu > UCHAR_MAX) blu = UCHAR_MAX;
				if (blu < 0) blu = 0;
				iCurImage->Data[i+2] = (ILubyte)red;
				iCurImage->Data[i+1] = (ILubyte)grn;
				iCurImage->Data[i]   = (ILubyte)blu;
			}
			break;

		case IL_LUMINANCE:
			NumPix = iCurImage->SizeOfData / iCurImage->Bpc;
			switch (iCurImage->Bpc)
			{
				case 1:
					for (i = 0; i < NumPix; i++) {
						grey = (ILint)(iCurImage->Data[i] * r);
						if (grey > UCHAR_MAX) grey = UCHAR_MAX;
						if (grey < 0) grey = 0;
						iCurImage->Data[i] = (ILubyte)grey;
					}
					break;

				case 2:
					ShortPtr = (ILushort*)iCurImage->Data;
					for (i = 0; i < NumPix; i++) {
						grey = (ILint)(ShortPtr[i] * r);
						if (grey > USHRT_MAX) grey = USHRT_MAX;
						if (grey < 0) grey = 0;
						ShortPtr[i] = (ILushort)grey;
					}
					break;

				case 4:
					IntPtr = (ILuint*)iCurImage->Data;
					for (i = 0; i < NumPix; i++) {
						grey = (ILint)(IntPtr[i] * r);
						if (grey < 0) grey = 0;
						IntPtr[i] = (ILuint)grey;
					}
					break;

				case 8:
					DblPtr = (ILdouble*)iCurImage->Data;
					for (i = 0; i < NumPix; i++) {
						DblPtr[i] = DblPtr[i] * r;
					}
					break;
			}
			break;

		case IL_COLOUR_INDEX:
			switch (iCurImage->Pal.PalType)
			{
				case IL_PAL_RGB24:
				case IL_PAL_RGB32:
				case IL_PAL_RGBA32:
					for (i = 0; i < iCurImage->Pal.PalSize; i += ilGetInteger(IL_PALETTE_BPP)) {
						red = (ILint)(iCurImage->Data[i] * r);
						grn = (ILint)(iCurImage->Data[i+1] * g);
						blu = (ILint)(iCurImage->Data[i+2] * b);
						if (red > UCHAR_MAX) red = UCHAR_MAX;
						if (red < 0) red = 0;
						if (grn > UCHAR_MAX) grn = UCHAR_MAX;
						if (grn < 0) grn = 0;
						if (blu > UCHAR_MAX) blu = UCHAR_MAX;
						if (blu < 0) blu = 0;
						iCurImage->Data[i]   = (ILubyte)red;
						iCurImage->Data[i+1] = (ILubyte)grn;
						iCurImage->Data[i+2] = (ILubyte)blu;
					}
					break;

				case IL_PAL_BGR24:
				case IL_PAL_BGR32:
				case IL_PAL_BGRA32:
					for (i = 0; i < iCurImage->Pal.PalSize; i += ilGetInteger(IL_PALETTE_BPP)) {
						red = (ILint)(iCurImage->Data[i+2] * r);
						grn = (ILint)(iCurImage->Data[i+1] * g);
						blu = (ILint)(iCurImage->Data[i] * b);
						if (red > UCHAR_MAX) red = UCHAR_MAX;
						if (red < 0) red = 0;
						if (grn > UCHAR_MAX) grn = UCHAR_MAX;
						if (grn < 0) grn = 0;
						if (blu > UCHAR_MAX) blu = UCHAR_MAX;
						if (blu < 0) blu = 0;
						iCurImage->Data[i+2]   = (ILubyte)red;
						iCurImage->Data[i+1] = (ILubyte)grn;
						iCurImage->Data[i] = (ILubyte)blu;
					}
					break;


				default:
					ilSetError(ILU_ILLEGAL_OPERATION);
					return IL_FALSE;
			}

		default:
			ilSetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
	}

	return IL_TRUE;
}


/*ILboolean ILAPIENTRY iluGammaCorrectScale(ILfloat Gamma)
{
	return iluScaleColours(Gamma, Gamma, Gamma);
}*/


ILboolean ILAPIENTRY iluGammaCorrect(ILfloat Gamma)
{
	ILfloat		Table[256];
	ILuint		i, NumPix;
	ILushort	*ShortPtr;
	ILuint		*IntPtr;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	for (i = 0; i < 256; i++) {
		Table[i] = (ILfloat)pow(i / 255.0, 1.0 / Gamma);
	}

	// Do we need to clamp here?
	if (iCurImage->Format == IL_COLOUR_INDEX) {
		for (i = 0; i < iCurImage->Pal.PalSize; i++) {
			iCurImage->Pal.Palette[i] = (ILubyte)(Table[iCurImage->Pal.Palette[i]] * 255);
		}
	}
	else {
		// Not too sure if this is the correct way of handling multiple bpc's.
		switch (iCurImage->Bpc)
		{
			case 1:
				for (i = 0; i < iCurImage->SizeOfData; i++) {
					iCurImage->Data[i] = (ILubyte)(Table[iCurImage->Data[i]] * UCHAR_MAX);
				}
				break;
			case 2:
				NumPix = iCurImage->SizeOfData / 2;
				ShortPtr = (ILushort*)iCurImage->Data;
				for (i = 0; i < NumPix; i++) {
					ShortPtr[i] = (ILushort)(Table[ShortPtr[i] >> 8] * USHRT_MAX);
				}
				break;
			case 4:
				NumPix = iCurImage->SizeOfData / 4;
				IntPtr = (ILuint*)iCurImage->Data;
				for (i = 0; i < NumPix; i++) {
					IntPtr[i] = (ILuint)(Table[IntPtr[i] >> 24] * UINT_MAX);
				}
				break;
		}
	}

	return IL_TRUE;
}


//
// Colour matrix stuff follows
// ---------------------------
//


ILvoid iApplyMatrix(ILimage *Image, ILfloat Mat[3][3])
{
	ILubyte	*Data = Image->Data;
	ILuint	i;
	ILubyte	r, g, b;

	switch (Image->Format)
	{
		case IL_RGB:
		case IL_RGBA:
			for (i = 0; i < Image->SizeOfData; ) {
				r = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
				g = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				b = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
				Data[i]   = r;
				Data[i+1] = g;
				Data[i+2] = b;
				i += Image->Bpp;
			}
			break;

		case IL_BGR:
		case IL_BGRA:
			for (i = 0; i < Image->SizeOfData; ) {
				r = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
				g = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				b = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
				Data[i]   = b;
				Data[i+1] = g;
				Data[i+2] = r;
				i += Image->Bpp;
			}
			break;

		default:
			ilSetError(ILU_ILLEGAL_OPERATION);
			return;
	}

	return;
}


ILvoid iIdentity(ILfloat *Matrix)
{
    *Matrix++ = 1.0;    // row 1
    *Matrix++ = 0.0;
    *Matrix++ = 0.0;
    //*Matrix++ = 0.0;
    *Matrix++ = 0.0;    // row 2
    *Matrix++ = 1.0;
    *Matrix++ = 0.0;
    //*Matrix++ = 0.0;
    *Matrix++ = 0.0;    // row 3
    *Matrix++ = 0.0;
    *Matrix++ = 1.0;
    //*Matrix++ = 0.0;
    /**Matrix++ = 0.0;    // row 4
    *Matrix++ = 0.0;
    *Matrix++ = 0.0;
    *Matrix++ = 1.0;*/
}


ILboolean ILAPIENTRY iluSaturate1f(ILfloat Saturation)
{
	return iluSaturate4f(0.3086f, 0.6094f, 0.0820f, Saturation);
}


ILboolean ILAPIENTRY iluSaturate4f(ILfloat r, ILfloat g, ILfloat b, ILfloat Saturation)
{
	ILfloat Mat[3][3];
	ILfloat s = Saturation;

	Mat[0][0] = (1.0f - s) * r + s;
	Mat[0][1] = (1.0f - s) * r;
	Mat[0][2] = (1.0f - s) * r;
	Mat[0][3] = 0.0f;

	Mat[1][0] = (1.0f - s) * g;
	Mat[1][1] = (1.0f - s) * g + s;
	Mat[1][2] = (1.0f - s) * g;
	Mat[1][3] = 0.0f;

	Mat[2][0] = (1.0f - s) * b;
	Mat[2][1] = (1.0f - s) * b;
	Mat[2][2] = (1.0f - s) * b + s;
	Mat[2][3] = 0.0f;

	Mat[3][0] = 0.0f;
	Mat[3][1] = 0.0f;
	Mat[3][2] = 0.0f;
	Mat[3][3] = 1.0f;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iApplyMatrix(iCurImage, Mat);

	return IL_TRUE;
}



//! Funny as hell filter that I stumbled upon accidentally
ILboolean ILAPIENTRY iluAlienify(ILvoid)
{
	ILfloat	Mat[3][3];
	ILubyte	*Data;
	ILuint	i;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	Data = iCurImage->Data;

	iIdentity((ILfloat*)Mat);

	switch (iCurImage->Format)
	{
		case IL_BGR:
		case IL_BGRA:
			for (i = 0; i < iCurImage->SizeOfData; i += iCurImage->Bpp) {
				Data[i+2] = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
				Data[i+1] = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				Data[i]   = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
			}
		break;

		case IL_RGB:
		case IL_RGBA:
			for (i = 0; i < iCurImage->SizeOfData; i += iCurImage->Bpp) {
				Data[i]   = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
				Data[i+1] = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				Data[i+2] = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
			}
		break;

		default:
			//ilSetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
	}

	return IL_TRUE;
}




// Everything here was taken from
//	http://bae.fse.missouri.edu/luw/course/image/project1/project1.html
//	and http://www.sgi.com/grafica/interp/index.html



ILvoid iIntExtImg(ILimage *Image1, ILimage *Image2, ILfloat a)
{
	ILuint	i;
	ILint	d;
	ILubyte	*Data1, *Data2;

	Data1 = Image1->Data;
	Data2 = Image2->Data;

	for (i = 0; i < Image2->SizeOfData; i++) {
		d = (ILint)((1 - a) * *Data1 + a * *Data2);
		// Limit a pixel value to the range [0, 255]
		if (d < 0) 
			d = 0;
		else if (d > 255)
			d = 255;
		*Data2 = d;

		Data1++;
		Data2++;
	}
			
	return;
}


// Works best when Gamma is in the range [0.0, 2.0].
/*ILboolean ILAPIENTRY iluGammaCorrectInter(ILfloat Gamma)
{
	ILimage	*Black;
	ILuint	i;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Black = ilNewImage(iCurImage->Width, iCurImage->Height, iCurImage->Depth, iCurImage->Bpp, iCurImage->Bpc);
	if (Black == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	// Make Black all 0's
	for (i = 0; i < Black->SizeOfData; i++) {
		Black->Data[i] = 0;
	}

	iIntExtImg(Black, iCurImage, Gamma);
	ilCloseImage(Black);

	return IL_TRUE;
}*/


// Works best when Contrast is in the range [-0.5, 1.7].
ILboolean ILAPIENTRY iluContrast(ILfloat Contrast)
{
	ILimage	*Grey;
	ILuint	i;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Grey = ilNewImage(iCurImage->Width, iCurImage->Height, iCurImage->Depth, iCurImage->Bpp, iCurImage->Bpc);
	if (Grey == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	// Make Grey all 127's
	for (i = 0; i < Grey->SizeOfData; i++) {
		Grey->Data[i] = 127;
	}

	iIntExtImg(Grey, iCurImage, Contrast);
	ilCloseImage(Grey);

	return IL_TRUE;
}


// Sharpens when Factor is in the range [1.0, 2.5].
// Blurs when Factor is in the range [0.0, 1.0].
ILboolean ILAPIENTRY iluSharpen(ILfloat Factor, ILuint Iter)
{
	ILimage	*Blur, *CurImage;  // iluBlur() changes iCurImage
	ILuint	i;
	
	CurImage = ilGetCurImage();
	if (CurImage == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Blur = ilNewImage(CurImage->Width, CurImage->Height, CurImage->Depth, CurImage->Bpp, CurImage->Bpc);
	if (Blur == NULL) {
		ilSetError(ILU_OUT_OF_MEMORY);
		return IL_FALSE;
	}
	ilCopyImageAttr(Blur, CurImage);

	ilCopyPixels(0, 0, 0, CurImage->Width, CurImage->Height, 1, CurImage->Format, CurImage->Type, Blur->Data);
	ilSetCurImage(Blur);
	iluBlurGaussian(1);

	for (i = 0; i < Iter; i++) {
		iIntExtImg(Blur, CurImage, Factor);
	}

	ilCloseImage(Blur);
	ilSetCurImage(CurImage);

	return IL_TRUE;
}
