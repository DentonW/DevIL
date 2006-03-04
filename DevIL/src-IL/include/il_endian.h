//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 01/29/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/include/il_endian.h
//
// Description: Handles Endian-ness
//
//-----------------------------------------------------------------------------

#ifndef IL_ENDIAN_H
#define IL_ENDIAN_H

#include "il_internal.h"

#ifdef WORDS_BIGENDIAN //this is defined by ./configure
	#ifndef __BIG_ENDIAN__
	#define __BIG_ENDIAN__
	#endif
#endif

#ifdef __BIG_ENDIAN__
	#define Short(s) iSwapShort(s)
	#define UShort(s) iSwapUShort(s)
	#define Int(i) iSwapInt(i)
	#define UInt(i) iSwapUInt(i)
	#define Float(f) iSwapFloat(f)
	#define Double(d) iSwapDouble(d)
 
	#define BigShort(s)  
	#define BigUShort(s)  
	#define BigInt(i)  
	#define BigUInt(i)  
	#define BigFloat(f)  
	#define BigDouble(d)  
#else
	#undef __LITTLE_ENDIAN__  // Not sure if it's defined by any compiler...
	#define __LITTLE_ENDIAN__
	#define Short(s)  
	#define UShort(s)  
	#define Int(i)  
	#define UInt(i)  
	#define Float(f)  
	#define Double(d)  

	#define BigShort(s) iSwapShort(s)
	#define BigUShort(s) iSwapUShort(s)
	#define BigInt(i) iSwapInt(i)
	#define BigUInt(i) iSwapUInt(i)
	#define BigFloat(f) iSwapFloat(f)
	#define BigDouble(d) iSwapDouble(d)
#endif


ILvoid	iSwapUShort(ILushort *s);
ILvoid	iSwapShort(ILshort *s);
ILvoid	iSwapUInt(ILuint *i);
ILvoid	iSwapInt(ILint *i);
ILvoid	iSwapFloat(ILfloat *f);
ILvoid	iSwapDouble(ILdouble *d);


// Only seem to work on unsigned values properly...
#define SwapShort(s) ((s>>8) | (s<<8))
#define SwapInt(i) ((i>>24) | ((i>>8) & 0xff00) | ((i<<8) & 0xff0000) | (i<<24))
//#define SwapFloat(f) ((f>>24) | ((f>>8) & 0xff00) | ((f<<8) & 0xff0000) | (f<<24))
//#define SwapDouble(d) ((f>>56) | ((f>>40) & 0xff00) | ((f>>24) & 0xff0000) |
//	((f>>8) & 0xff000000) | ((f<<8) & 0xff00000000) | ((f<<24) & 0xff0000000000) | ((f<<40) & 0xff000000000000) | (f<<56))


ILushort	GetLittleUShort(ILvoid);
ILshort		GetLittleShort(ILvoid);
ILuint		GetLittleUInt(ILvoid);
ILint		GetLittleInt(ILvoid);
ILfloat		GetLittleFloat(ILvoid);
ILdouble	GetLittleDouble(ILvoid);
ILushort	GetBigUShort(ILvoid);
ILshort		GetBigShort(ILvoid);
ILuint		GetBigUInt(ILvoid);
ILint		GetBigInt(ILvoid);
ILfloat		GetBigFloat(ILvoid);
ILdouble	GetBigDouble(ILvoid);

ILubyte		SaveLittleUShort(ILushort _s);
ILubyte		SaveLittleShort(ILshort _s);
ILubyte		SaveLittleUInt(ILuint _i);
ILubyte		SaveLittleInt(ILint _i);
ILubyte		SaveLittleFloat(ILfloat _f);
ILubyte		SaveLittleDouble(ILdouble _d);
ILubyte		SaveBigUShort(ILushort _s);
ILubyte		SaveBigShort(ILshort _s);
ILubyte		SaveBigUInt(ILuint _i);
ILubyte		SaveBigInt(ILint _i);
ILubyte		SaveBigFloat(ILfloat _f);
ILubyte		SaveBigDouble(ILdouble _d);

ILvoid		EndianSwapData(void *_Image);

#endif//ENDIAN_H
