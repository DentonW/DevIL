//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 08/03/2001 <--Y2K Compliant! =]
//
// Filename: openil/states.h
//
// Description: State machine
//
//-----------------------------------------------------------------------------


#ifndef STATES_H
#define STATES_H

#include "il_internal.h"


ILboolean ilAble(ILenum Mode, ILboolean Flag);


#define IL_ATTRIB_STACK_MAX 32

ILuint ilCurrentPos = 0;  // Which position on the stack

//
// Various states
//

typedef struct IL_STATES
{
	// Origin states
	ILboolean	ilOriginSet;
	ILenum		ilOriginMode;
	// Format and type states
	ILboolean	ilFormatSet;
	ILboolean	ilTypeSet;
	ILenum		ilFormatMode;
	ILenum		ilTypeMode;
	// File mode states
	ILboolean	ilOverWriteFiles;
	// Palette states
	ILboolean	ilAutoConvPal;
	// Load fail states
	ILboolean	ilDefaultOnFail;
	// Key colour states
	ILboolean	ilUseKeyColour;
	// Compression states
	ILenum		ilCompression;
	// Interlace states
	ILenum		ilInterlace;
	// Quantization states
	ILenum		ilQuantMode;
	ILuint		ilNeuSample;


	//
	// Format-specific states
	//

	ILboolean	ilTgaCreateStamp;
	ILuint		ilJpgQuality;
	ILboolean	ilPngInterlace;
	ILboolean	ilTgaRle;
	ILboolean	ilBmpRle;
	ILboolean	ilSgiRle;
	ILenum		ilJpgFormat;
	ILenum		ilDxtcFormat;
	ILenum		ilPcdPicNum;


	//
	// Format-specific strings
	//

	char		*ilTgaId;
	char		*ilTgaAuthName;
	char		*ilTgaAuthComment;
	char		*ilPngAuthName;
	char		*ilPngTitle;
	char		*ilPngDescription;
	char		*ilCHeader;




} IL_STATES;

IL_STATES ilStates[IL_ATTRIB_STACK_MAX];


typedef struct IL_HINTS
{
	// Memory vs. Speed trade-off
	ILenum		MemVsSpeedHint;
	// Compression hints
	ILenum		CompressHint;

} IL_HINTS;

IL_HINTS ilHints;


#endif//STATES_H
