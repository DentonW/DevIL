//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 06/12/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_endian.c
//
// Description: Handles Endian-ness
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include "il_endian.h"

ILvoid _SwapUShort(ILushort *s)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov ebx, s
			mov al, [ebx+1]
			mov ah, [ebx  ]
			mov [ebx], ax
		}
	#else
		*s = (*s>>8) | (*s<<8);
	#endif//USE_WIN32_ASM
}

ILvoid _SwapShort(ILshort *s)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov ebx, s
			mov al, [ebx+1]
			mov ah, [ebx  ]
			mov [ebx], ax
		}
	#else
		*s = (*s>>8) | (*s<<8);
	#endif//USE_WIN32_ASM
}

ILvoid _SwapUInt(ILuint *i)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov ebx, i
			mov eax, [ebx]
			bswap eax
			mov [ebx], eax
		}
	#else
		*i = (*i>>24) | ((*i>>8) & 0xff00) | ((*i<<8) & 0xff0000) | (*i<<24);
	#endif//USE_WIN32_ASM
}

ILvoid _SwapInt(ILint *i)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov ebx, i
			mov eax, [ebx]
			bswap eax
			mov [ebx], eax
		}
	#else
		*i = (*i>>24) | ((*i>>8) & 0xff00) | ((*i<<8) & 0xff0000) | (*i<<24);
	#endif//USE_WIN32_ASM
}

/*ILfloat _SwapFloat(ILfloat f)
{
	return (f>>24) | ((f>>8) & 0xff00) | ((f<<8) & 0xff0000) | (f<<24);
}

ILdouble _SwapDouble(ILdouble d)
{
	return	(f>>56) | ((f>>40) & 0xff00) | ((f>>24) & 0xff0000) |
			((f>>8) & 0xff000000) | ((f<<8) & 0xff00000000) | ((f<<24) & 0xff0000000000) | ((f<<40) & 0xff000000000000) | (f<<56);
}*/

ILvoid _SwapFloat(ILfloat *f)
{
	static ILubyte Temp[4];
	*((ILfloat*)Temp) = *f;
	((ILubyte*)f)[0] = Temp[3];
	((ILubyte*)f)[1] = Temp[2];
	((ILubyte*)f)[2] = Temp[1];
	((ILubyte*)f)[3] = Temp[0];
}

ILvoid _SwapDouble(ILdouble *d)
{
	static ILubyte Temp[8];
	*((ILdouble*)Temp) = *d;
	((ILubyte*)d)[0] = Temp[7];
	((ILubyte*)d)[1] = Temp[6];
	((ILubyte*)d)[2] = Temp[5];
	((ILubyte*)d)[3] = Temp[4];
	((ILubyte*)d)[4] = Temp[3];
	((ILubyte*)d)[5] = Temp[2];
	((ILubyte*)d)[6] = Temp[1];
	((ILubyte*)d)[7] = Temp[0];
}


ILushort GetLittleUShort()
{
	static ILushort s;
	iread(&s, sizeof(ILushort), 1);
#ifdef __BIG_ENDIAN__
	_SwapUShort(&s);
#endif
	return s;
}


ILshort GetLittleShort()
{
	static ILshort s;
	iread(&s, sizeof(ILshort), 1);
#ifdef __BIG_ENDIAN__
	_SwapShort(&s);
#endif
	return s;
}


ILuint GetLittleUInt()
{
	static ILuint i;
	iread(&i, sizeof(ILuint), 1);
#ifdef __BIG_ENDIAN__
	_SwapUInt(&i);
#endif
	return i;
}


ILint GetLittleInt()
{
	static ILint i;
	iread(&i, sizeof(ILint), 1);
#ifdef __BIG_ENDIAN__
	_SwapInt(&i);
#endif
	return i;
}


ILfloat GetLittleFloat()
{
	static ILfloat f;
	iread(&f, sizeof(ILfloat), 1);
#ifdef __BIG_ENDIAN__
	_SwapFloat(&f);
#endif
	return f;
}


ILdouble GetLittleDouble()
{
	static ILdouble d;
	iread(&d, sizeof(ILdouble), 1);
#ifdef __BIG_ENDIAN__
	_SwapDouble(&d);
#endif
	return d;
}


ILushort GetBigUShort()
{
	static ILushort s;
	iread(&s, sizeof(ILushort), 1);
#ifdef __LITTLE_ENDIAN__
	_SwapUShort(&s);
#endif
	return s;
}


ILshort GetBigShort()
{
	static ILshort s;
	iread(&s, sizeof(ILshort), 1);
#ifdef __LITTLE_ENDIAN__
	_SwapShort(&s);
#endif
	return s;
}


ILuint GetBigUInt()
{
	static ILuint i;
	iread(&i, sizeof(ILuint), 1);
#ifdef __LITTLE_ENDIAN__
	_SwapUInt(&i);
#endif
	return i;
}


ILint GetBigInt()
{
	static ILint i;
	iread(&i, sizeof(ILint), 1);
#ifdef __LITTLE_ENDIAN__
	_SwapInt(&i);
#endif
	return i;
}


ILfloat GetBigFloat()
{
	static ILfloat f;
	iread(&f, sizeof(ILfloat), 1);
#ifdef __LITTLE_ENDIAN__
	_SwapFloat(&f);
#endif
	return f;
}


ILdouble GetBigDouble()
{
	static ILdouble d;
	iread(&d, sizeof(ILdouble), 1);
#ifdef __LITTLE_ENDIAN__
	_SwapDouble(&d);
#endif
	return d;
}


ILubyte SaveLittleUShort(ILushort s)
{
#ifdef __BIG_ENDIAN__
	_SwapShort(&s);
#endif
	return iwrite(&s, sizeof(ILushort), 1);
}


ILubyte SaveLittleShort(ILshort s)
{
#ifdef __BIG_ENDIAN__
	_SwapShort(&s);
#endif
	return iwrite(&s, sizeof(ILshort), 1);
}


ILubyte SaveLittleUInt(ILuint i)
{
#ifdef __BIG_ENDIAN__
	_SwapUInt(&i);
#endif
	return iwrite(&i, sizeof(ILuint), 1);
}


ILubyte SaveLittleInt(ILint i)
{
#ifdef __BIG_ENDIAN__
	_SwapInt(&i);
#endif
	return iwrite(&i, sizeof(ILint), 1);
}


ILubyte SaveLittleFloat(ILfloat f)
{
#ifdef __BIG_ENDIAN__
	_SwapFloat(&f);
#endif
	return iwrite(&f, sizeof(ILfloat), 1);
}


ILubyte SaveLittleDouble(ILdouble d)
{
#ifdef __BIG_ENDIAN__
	_SwapDouble(&d);
#endif
	return iwrite(&d, sizeof(ILdouble), 1);
}


ILubyte SaveBigUShort(ILushort s)
{
#ifdef __LITTLE_ENDIAN__
	_SwapUShort(&s);
#endif
	return iwrite(&s, sizeof(ILushort), 1);
}


ILubyte SaveBigShort(ILshort s)
{
#ifdef __LITTLE_ENDIAN__
	_SwapShort(&s);
#endif
	return iwrite(&s, sizeof(ILshort), 1);
}


ILubyte SaveBigUInt(ILuint i)
{
#ifdef __LITTLE_ENDIAN__
	_SwapUInt(&i);
#endif
	return iwrite(&i, sizeof(ILuint), 1);
}


ILubyte SaveBigInt(ILint i)
{
#ifdef __LITTLE_ENDIAN__
	_SwapInt(&i);
#endif
	return iwrite(&i, sizeof(ILint), 1);
}


ILubyte SaveBigFloat(ILfloat f)
{
#ifdef __LITTLE_ENDIAN__
	_SwapFloat(&f);
#endif
	return iwrite(&f, sizeof(ILfloat), 1);
}


ILubyte SaveBigDouble(ILdouble d)
{
#ifdef __LITTLE_ENDIAN__
	_SwapDouble(&d);
#endif
	return iwrite(&d, sizeof(ILdouble), 1);
}


ILvoid EndianSwapData(void *_Image)
{
	static ILuint	i;
	static ILubyte	*temp, *s, *d;
	static ILushort	*ShortS, *ShortD;
	static ILuint	*IntS, *IntD;
	static ILfloat	*FltS, *FltD;
	static ILdouble	*DblS, *DblD;

	ILimage *Image = (ILimage*)_Image;

	switch (Image->Type)
	{
		case IL_BYTE:
		case IL_UNSIGNED_BYTE:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					s = Image->Data;
					d = temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
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
					ShortS = (ILushort*)Image->Data;
					ShortD = (ILushort*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*ShortD++ = SwapShort(*(ShortS+2));
						*ShortD++ = SwapShort(*(ShortS+1));
						*ShortD++ = SwapShort(*ShortS);
						ShortS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					ShortS = (ILushort*)Image->Data;
					ShortD = (ILushort*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*ShortD++ = SwapShort(*(ShortS+3));
						*ShortD++ = SwapShort(*(ShortS+2));
						*ShortD++ = SwapShort(*(ShortS+1));
						*ShortD++ = SwapShort(*ShortS);
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
					IntS = (ILuint*)Image->Data;
					IntD = (ILuint*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*IntD++ = SwapInt(*(IntS+2));
						*IntD++ = SwapInt(*(IntS+1));
						*IntD++ = SwapInt(*IntS);
						IntS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					IntS = (ILuint*)Image->Data;
					IntD = (ILuint*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*IntD++ = SwapInt(*(IntS+3));
						*IntD++ = SwapInt(*(IntS+2));
						*IntD++ = SwapInt(*(IntS+1));
						*IntD++ = SwapInt(*IntS);
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
					FltS = (ILfloat*)Image->Data;
					FltD = (ILfloat*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						_SwapFloat(FltS);
						_SwapFloat(FltS+1);
						_SwapFloat(FltS+2);
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
					FltS = (ILfloat*)Image->Data;
					FltD = (ILfloat*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						_SwapFloat(FltS);
						_SwapFloat(FltS+1);
						_SwapFloat(FltS+2);
						_SwapFloat(FltS+3);
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
					DblS = (ILdouble*)Image->Data;
					DblD = (ILdouble*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						_SwapDouble(DblS);
						_SwapDouble(DblS+1);
						_SwapDouble(DblS+2);
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
					DblS = (ILdouble*)Image->Data;
					DblD = (ILdouble*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						_SwapDouble(DblS);
						_SwapDouble(DblS+1);
						_SwapDouble(DblS+2);
						_SwapDouble(DblS+3);
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
