#include "il_internal.h"

#define IL_ENDIAN_C

#include "il_endian.h"

ILvoid EndianSwapData(void *_Image) {
	ILuint		i;
	ILubyte		*temp, *s, *d;
	ILushort	*ShortS, *ShortD;
	ILuint		*IntS, *IntD;
	ILfloat		*FltS, *FltD;
	ILdouble	*DblS, *DblD;

	ILimage *Image = (ILimage*)_Image;

	switch (Image->Type) {
		case IL_BYTE:
		case IL_UNSIGNED_BYTE:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					s = Image->Data;
					d = temp;

					for( i = Image->Width * Image->Height; i > 0; i-- ) {
						*d++ = *(s+2);
						*d++ = *(s+1);
						*d++ = *s;
						s += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					s = Image->Data;
					d = temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*d++ = *(s+3);
						*d++ = *(s+2);
						*d++ = *(s+1);
						*d++ = *s;
						s += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_SHORT:
		case IL_UNSIGNED_SHORT:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					ShortS = (ILushort*)Image->Data;
					ShortD = (ILushort*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*(ShortD+2) = *(ShortS+2);iSwapUShort(ShortD+2);
						*(ShortD+1) = *(ShortS+1);iSwapUShort(ShortD+1);
						*ShortD     = *(ShortS);  iSwapUShort(ShortD);
						ShortD += 3;
						ShortS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					ShortS = (ILushort*)Image->Data;
					ShortD = (ILushort*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*(ShortD+3) = *(ShortS+3);iSwapUShort(ShortD+3);
						*(ShortD+2) = *(ShortS+2);iSwapUShort(ShortD+2);
						*(ShortD+1) = *(ShortS+1);iSwapUShort(ShortD+1);
						*ShortD     = *(ShortS);  iSwapUShort(ShortD);
						ShortD += 4;
						ShortS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_INT:
		case IL_UNSIGNED_INT:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					IntS = (ILuint*)Image->Data;
					IntD = (ILuint*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*(IntD+2) = *(IntS+2);iSwapUInt(IntD+2);
						*(IntD+1) = *(IntS+1);iSwapUInt(IntD+1);
						*IntD     = *IntS;    iSwapUInt(IntD);
						IntD += 3;
						IntS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					IntS = (ILuint*)Image->Data;
					IntD = (ILuint*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*(IntD+3) = *(IntS+3);iSwapUInt(IntD+3);
						*(IntD+2) = *(IntS+2);iSwapUInt(IntD+2);
						*(IntD+1) = *(IntS+1);iSwapUInt(IntD+1);
						*IntD     = *IntS;    iSwapUInt(IntD);
						IntD += 4;
						IntS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_FLOAT:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					FltS = (ILfloat*)Image->Data;
					FltD = (ILfloat*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapFloat(FltS);
						iSwapFloat(FltS+1);
						iSwapFloat(FltS+2);
						*FltD++ = *(FltS+2);
						*FltD++ = *(FltS+1);
						*FltD++ = *FltS;
						FltS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					FltS = (ILfloat*)Image->Data;
					FltD = (ILfloat*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapFloat(FltS);
						iSwapFloat(FltS+1);
						iSwapFloat(FltS+2);
						iSwapFloat(FltS+3);
						*FltD++ = *(FltS+3);
						*FltD++ = *(FltS+2);
						*FltD++ = *(FltS+1);
						*FltD++ = *FltS;
						FltS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_DOUBLE:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					DblS = (ILdouble*)Image->Data;
					DblD = (ILdouble*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapDouble(DblS);
						iSwapDouble(DblS+1);
						iSwapDouble(DblS+2);
						*DblD++ = *(DblS+2);
						*DblD++ = *(DblS+1);
						*DblD++ = *DblS;
						DblS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					DblS = (ILdouble*)Image->Data;
					DblD = (ILdouble*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapDouble(DblS);
						iSwapDouble(DblS+1);
						iSwapDouble(DblS+2);
						iSwapDouble(DblS+3);
						*DblD++ = *(DblS+3);
						*DblD++ = *(DblS+2);
						*DblD++ = *(DblS+1);
						*DblD++ = *DblS;
						DblS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		switch (iCurImage->Pal.PalType)
		{
			case IL_PAL_RGB24:
			case IL_PAL_BGR24:
				temp = ialloc(Image->Pal.PalSize);
				if (temp == NULL)
					return;
				s = Image->Pal.Palette;
				d = temp;

				for (i = Image->Pal.PalSize / 3; i > 0; i--) {
					*d++ = *(s+2);
					*d++ = *(s+1);
					*d++ = *s;
					s += 3;
				}

				ifree(Image->Pal.Palette);
				Image->Pal.Palette = temp;
				break;

			case IL_PAL_RGBA32:
			case IL_PAL_RGB32:
			case IL_PAL_BGRA32:
			case IL_PAL_BGR32:
				temp = ialloc(Image->Pal.PalSize);
				if (temp == NULL)
					return;
				s = Image->Pal.Palette;
				d = temp;

				for (i = Image->Pal.PalSize / 4; i > 0; i--) {
					*d++ = *(s+3);
					*d++ = *(s+2);
					*d++ = *(s+1);
					*d++ = *s;
					s += 4;
				}

				ifree(Image->Pal.Palette);
				Image->Pal.Palette = temp;
				break;
		}
	}
	return;
}
