//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2017 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: src-IL/include/il_pnm.h
//
// Description: Reads/writes to/from pbm/pgm/ppm formats
//
//-----------------------------------------------------------------------------


#ifndef PPMPGM_H
#define PPMPGM_H

#include "il_internal.h"
#include <string>

using namespace std;


#define IL_PBM_ASCII	0x0001
#define IL_PGM_ASCII	0x0002
#define IL_PPM_ASCII	0x0003
#define IL_PBM_BINARY	0x0004
#define IL_PGM_BINARY	0x0005
#define IL_PPM_BINARY	0x0006
#define IL_PAM			0x0007

enum PamTuples { PAM_BW = 1, PAM_GRAY, PAM_RGB, PAM_BW_ALPHA, PAM_GRAY_ALPHA, PAM_RGB_ALPHA };

typedef struct PPMINFO
{
	ILenum	Type;
	ILuint	Width;
	ILuint	Height;
	ILuint	Depth;
	ILuint	MaxColour;
	ILubyte	Bpp;
	ILubyte	TuplType;
} PPMINFO;

ILboolean	iIsValidPnm(void);
ILboolean	iCheckPnm(char Header[2]);
ILboolean	iLoadPnmInternal(void);
ILboolean	iSavePnmInternal(void);
ILimage		*ilReadAsciiPpm(PPMINFO *Info);
ILimage		*ilReadBinaryPpm(PPMINFO *Info);
ILimage		*ilReadBitPbm(PPMINFO *Info);
ILboolean	ilReadPam(void);
ILboolean	iGetWord(ILboolean);
void		PbmMaximize(ILimage *Image);
ILint		DecodeTupleType(string &TupleStr);


#endif//PPMPGM_H
