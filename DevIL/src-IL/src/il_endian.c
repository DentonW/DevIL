//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 01/05/2002 <--Y2K Compliant! =]
//
// Filename: openil/il_endian.c
//
// Description: Handles Endian-ness
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include "il_endian.h"


// From the DooM sources
ILushort _SwapUShort(ILushort s)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov cx, s
			mov al, ch
			mov ah, cl
		}
	#else
		// No masking with 0xFF should be necessary.
		return (s>>8) | (s<<8);
	#endif//USE_WIN32_ASM
}

ILshort _SwapShort(ILshort s)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov cx, s
			mov al, ch
			mov ah, cl
		}
	#else
		// No masking with 0xFF should be necessary.
		return (s>>8) | (s<<8);
	#endif//USE_WIN32_ASM
}

ILuint _SwapUInt(ILuint i)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov eax, i
			bswap eax
		}
	#else
		return (i>>24) | ((i>>8) & 0xff00) | ((i<<8) & 0xff0000) | (i<<24);
	#endif//USE_WIN32_ASM
}

ILint _SwapInt(ILint i)
{
	#ifdef USE_WIN32_ASM
		__asm {
			mov eax, i
			bswap eax
		}
	#else
		return (i>>24) | ((i>>8) & 0xff00) | ((i<<8) & 0xff0000) | (i<<24);
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

ILfloat _SwapFloat(ILfloat f)
{
	static ILubyte Temp[4];
	*((ILfloat*)Temp) = f;
	((ILubyte*)&f)[0] = Temp[3];
	((ILubyte*)&f)[1] = Temp[2];
	((ILubyte*)&f)[2] = Temp[1];
	((ILubyte*)&f)[3] = Temp[0];
	return f;
}

ILdouble _SwapDouble(ILdouble d)
{
	static ILubyte Temp[8];
	*((ILdouble*)Temp) = d;
	((ILubyte*)&d)[0] = Temp[7];
	((ILubyte*)&d)[1] = Temp[6];
	((ILubyte*)&d)[2] = Temp[5];
	((ILubyte*)&d)[3] = Temp[4];
	((ILubyte*)&d)[4] = Temp[3];
	((ILubyte*)&d)[5] = Temp[2];
	((ILubyte*)&d)[6] = Temp[1];
	((ILubyte*)&d)[7] = Temp[0];
	return d;
}


ILushort GetLittleUShort()
{
	static ILushort s;
	iread(&s, sizeof(ILushort), 1);
#ifdef __BIG_ENDIAN__
	s = SwapShort(s);
#endif
	return s;
}


ILshort GetLittleShort()
{
	static ILshort s;
	iread(&s, sizeof(ILshort), 1);
#ifdef __BIG_ENDIAN__
	s = _SwapShort(s);
#endif
	return s;
}


ILuint GetLittleUInt()
{
	static ILuint i;
	iread(&i, sizeof(ILuint), 1);
#ifdef __BIG_ENDIAN__
	i = SwapInt(i);
#endif
	return i;
}


ILint GetLittleInt()
{
	static ILint i;
	iread(&i, sizeof(ILint), 1);
#ifdef __BIG_ENDIAN__
	i = _SwapInt(i);
#endif
	return i;
}


ILfloat GetLittleFloat()
{
	static ILfloat f;
	iread(&f, sizeof(ILfloat), 1);
#ifdef __BIG_ENDIAN__
	f = _SwapFloat(f);
#endif
	return f;
}


ILdouble GetLittleDouble()
{
	static ILdouble d;
	iread(&d, sizeof(ILdouble), 1);
#ifdef __BIG_ENDIAN__
	d = _SwapDouble(d);
#endif
	return d;
}


ILushort GetBigUShort()
{
	static ILushort s;
	iread(&s, sizeof(ILushort), 1);
#ifdef __LITTLE_ENDIAN__
	s = SwapShort(s);
#endif
	return s;
}


ILshort GetBigShort()
{
	static ILshort s;
	iread(&s, sizeof(ILshort), 1);
#ifdef __LITTLE_ENDIAN__
	s = _SwapShort(s);
#endif
	return s;
}


ILuint GetBigUInt()
{
	static ILuint i;
	iread(&i, sizeof(ILuint), 1);
#ifdef __LITTLE_ENDIAN__
	i = SwapInt(i);
#endif
	return i;
}


ILint GetBigInt()
{
	static ILint i;
	iread(&i, sizeof(ILint), 1);
#ifdef __LITTLE_ENDIAN__
	i = _SwapInt(i);
#endif
	return i;
}


ILfloat GetBigFloat()
{
	static ILfloat f;
	iread(&f, sizeof(ILfloat), 1);
#ifdef __LITTLE_ENDIAN__
	f = _SwapFloat(f);
#endif
	return f;
}


ILdouble GetBigDouble()
{
	static ILdouble d;
	iread(&d, sizeof(ILdouble), 1);
#ifdef __LITTLE_ENDIAN__
	d = _SwapDouble(d);
#endif
	return d;
}


ILubyte SaveLittleUShort(ILushort _s)
{
	static ILushort s;
#ifdef __BIG_ENDIAN__
	s = SwapShort(_s);
#else
	s = _s;
#endif
	return iwrite(&s, sizeof(ILushort), 1);
}


ILubyte SaveLittleShort(ILshort _s)
{
	static ILshort s;
#ifdef __BIG_ENDIAN__
	s = _SwapShort(_s);
#else
	s = _s;
#endif
	return iwrite(&s, sizeof(ILshort), 1);
}


ILubyte SaveLittleUInt(ILuint _i)
{
	static ILuint i;
#ifdef __BIG_ENDIAN__
	i = SwapInt(_i);
#else
	i = _i;
#endif
	return iwrite(&i, sizeof(ILuint), 1);
}


ILubyte SaveLittleInt(ILint _i)
{
	static ILint i;
#ifdef __BIG_ENDIAN__
	i = _SwapInt(_i);
#else
	i = _i;
#endif
	return iwrite(&i, sizeof(ILint), 1);
}


ILubyte SaveLittleFloat(ILfloat _f)
{
	static ILfloat f;
#ifdef __BIG_ENDIAN__
	f = _SwapFloat(_f);
#else
	f = _f;
#endif
	return iwrite(&f, sizeof(ILfloat), 1);
}


ILubyte SaveLittleDouble(ILdouble _d)
{
	static ILdouble d;
#ifdef __BIG_ENDIAN__
	d = _SwapDouble(_d);
#else
	d = _d;
#endif
	return iwrite(&d, sizeof(ILdouble), 1);
}


ILubyte SaveBigUShort(ILushort _s)
{
	static ILushort s;
#ifdef __LITTLE_ENDIAN__
	s = SwapShort(_s);
#else
	s = _s;
#endif
	return iwrite(&s, sizeof(ILushort), 1);
}


ILubyte SaveBigShort(ILshort _s)
{
	static ILshort s;
#ifdef __LITTLE_ENDIAN__
	s = _SwapShort(_s);
#else
	s = _s;
#endif
	return iwrite(&s, sizeof(ILshort), 1);
}


ILubyte SaveBigUInt(ILuint _i)
{
	static ILuint i;
#ifdef __LITTLE_ENDIAN__
	i = SwapInt(_i);
#else
	i = _i;
#endif
	return iwrite(&i, sizeof(ILuint), 1);
}


ILubyte SaveBigInt(ILint _i)
{
	static ILint i;
#ifdef __LITTLE_ENDIAN__
	i = _SwapInt(_i);
#else
	i = _i;
#endif
	return iwrite(&i, sizeof(ILint), 1);
}


ILubyte SaveBigFloat(ILfloat _f)
{
	static ILfloat f;
#ifdef __LITTLE_ENDIAN__
	f = _SwapFloat(_f);
#else
	f = _f;
#endif
	return iwrite(&f, sizeof(ILfloat), 1);
}


ILubyte SaveBigDouble(ILdouble _d)
{
	static ILdouble d;
#ifdef __LITTLE_ENDIAN__
	d = _SwapDouble(_d);
#else
	d = _d;
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
						*ShortD++ = Short(*(ShortS+2));
						*ShortD++ = Short(*(ShortS+1));
						*ShortD++ = Short(*ShortS);
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
						*ShortD++ = Short(*(ShortS+3));
						*ShortD++ = Short(*(ShortS+2));
						*ShortD++ = Short(*(ShortS+1));
						*ShortD++ = Short(*ShortS);
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
						*IntD++ = Int(*(IntS+2));
						*IntD++ = Int(*(IntS+1));
						*IntD++ = Int(*IntS);
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
						*IntD++ = Int(*(IntS+3));
						*IntD++ = Int(*(IntS+2));
						*IntD++ = Int(*(IntS+1));
						*IntD++ = Int(*IntS);
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
						*FltD++ = Float(*(FltS+2));
						*FltD++ = Float(*(FltS+1));
						*FltD++ = Float(*FltS);
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
						*FltD++ = Float(*(FltS+3));
						*FltD++ = Float(*(FltS+2));
						*FltD++ = Float(*(FltS+1));
						*FltD++ = Float(*FltS);
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
						*DblD++ = Double(*(DblS+2));
						*DblD++ = Double(*(DblS+1));
						*DblD++ = Double(*DblS);
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
						*DblD++ = Double(*(DblS+3));
						*DblD++ = Double(*(DblS+2));
						*DblD++ = Double(*(DblS+1));
						*DblD++ = Double(*DblS);
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
