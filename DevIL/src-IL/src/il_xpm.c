//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/27/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_xpm.c
//
// Description: Reads from an .xpm file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_XPM
#include <ctype.h>


ILboolean iLoadXpmInternal(ILvoid);


// Reads an .xpm file
ILboolean ilLoadXpm(const ILstring FileName)
{
	ILHANDLE	XpmFile;
	ILboolean	bXpm = IL_FALSE;

	XpmFile = iopenr(FileName);
	if (XpmFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bXpm;
	}

	iSetInputFile(XpmFile);

	bXpm = ilLoadXpmF(XpmFile);

	icloser(XpmFile);

	return bXpm;
}


//! Reads an already-opened .xpm file
ILboolean ilLoadXpmF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadXpmInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains an .xpm
ILboolean ilLoadXpmL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadXpmInternal();
}


ILint XpmGetsInternal(char *Buffer, ILint MaxLen)
{
	ILint	i = 0, Current;

	if (ieof())
		return IL_EOF;

	while ((Current = igetc()) != -1 && i < MaxLen - 1) {
		if (Current == IL_EOF)
			return 0;
		if (Current == '\n')
			break;

		Buffer[i++] = Current;
	}

	Buffer[i++] = 0;

	return i;
}


ILint XpmGets(char *Buffer, ILint MaxLen)
{
	ILint		Size, i;
	ILboolean	NotComment = IL_FALSE, InsideComment = IL_FALSE;

	do {
		Size = XpmGetsInternal(Buffer, MaxLen);
		if (Size == 0)
			continue;
		if (Size == IL_EOF)
			return IL_EOF;
		if (Buffer[0] == '/' && Buffer[1] == '*') {
			for (i = 2; i < Size; i++) {
				if (Buffer[i] == '*' && Buffer[i+1] == '/') {
					break;
				}
			}
			if (i >= Size)
				InsideComment = IL_TRUE;
		}
		else if (InsideComment) {
			for (i = 0; i < Size; i++) {
				if (Buffer[i] == '*' && Buffer[i+1] == '/') {
					break;
				}
			}
			if (i < Size)
				InsideComment = IL_FALSE;
		}
		else {
			NotComment = IL_TRUE;
		}
	} while (!NotComment);

	return Size;
}


ILint XpmGetInt(char *Buffer, ILint Size, ILint *Position)
{
	static char Buff[1024];
	static ILint i, j;
	ILboolean IsInNum = IL_FALSE;

	for (i = *Position, j = 0; i < Size; i++) {
		if (isdigit(Buffer[i])) {
			IsInNum = IL_TRUE;
			Buff[j++] = Buffer[i];
		}
		else {
			if (IsInNum) {
				Buff[j] = 0;
				*Position = i;
				return atoi(Buff);
			}
		}
	}

	return -1;
}


typedef ILubyte XpmPixel[4];

ILvoid XpmPredefCol(char *Buff, XpmPixel *Colour)
{
	if (!stricmp(Buff, "none")) {
		(*Colour)[0] = 0;
		(*Colour)[1] = 0;
		(*Colour)[2] = 0;
		(*Colour)[3] = 0;
		return;
	}

	(*Colour)[3] = 255;

	if (!stricmp(Buff, "black")) {
		(*Colour)[0] = 0;
		(*Colour)[1] = 0;
		(*Colour)[2] = 0;
		return;
	}
	if (!stricmp(Buff, "red")) {
		(*Colour)[0] = 255;
		(*Colour)[1] = 0;
		(*Colour)[2] = 0;
		return;
	}
	if (!stricmp(Buff, "green")) {
		(*Colour)[0] = 0;
		(*Colour)[1] = 255;
		(*Colour)[2] = 0;
		return;
	}
	if (!stricmp(Buff, "blue")) {
		(*Colour)[0] = 0;
		(*Colour)[1] = 0;
		(*Colour)[2] = 255;
		return;
	}
	if (!stricmp(Buff, "yellow")) {
		(*Colour)[0] = 255;
		(*Colour)[1] = 255;
		(*Colour)[2] = 0;
		return;
	}
	if (!stricmp(Buff, "cyan")) {
		(*Colour)[0] = 0;
		(*Colour)[1] = 255;
		(*Colour)[2] = 255;
		return;
	}
	if (!stricmp(Buff, "gray")) {
		(*Colour)[0] = 128;
		(*Colour)[1] = 128;
		(*Colour)[2] = 128;
		return;
	}



	// No colour defined, so use transparent.
	(*Colour)[0] = 0;
	(*Colour)[1] = 0;
	(*Colour)[2] = 0;
	(*Colour)[3] = 0;

	return;
}


ILboolean XpmGetColour(char *Buffer, ILint Size, XpmPixel *Colours)
{
	ILint	i = 0, j;
	ILubyte	Pos, ColBuff[3];
	char	Buff[1024];

	for ( ; i < Size; i++) {
		if (Buffer[i] == '\"')
			break;
	}
	i++;  // Skip the quotes.

	if (i >= Size)
		return IL_FALSE;

	// Get the character.
	Pos = Buffer[i++];

	// Skip to the colour definition.
	for ( ; i < Size; i++) {
		if (Buffer[i] == 'c')
			break;
	}
	i++;  // Skip the 'c'.

	if (i >= Size)
		return IL_FALSE;

	for ( ; i < Size; i++) {
		if (Buffer[i] != ' ')
			break;
	}

	if (i >= Size)
		return IL_FALSE;

	if (Buffer[i] == '#') {
		for (j = 0; j < 3; j++) {
			ColBuff[0] = Buffer[++i];
			ColBuff[1] = Buffer[++i];
			ColBuff[2] = 0;
			Colours[Pos][j] = (ILubyte)strtol(ColBuff, NULL, 16);
		}
		Colours[Pos][3] = 255;  // Full alpha.
	}
	else {
		for (j = 0; i < Size; i++) {
			if (!isalnum(Buffer[i]))
				break;
			Buff[j++] = Buffer[i];
		}
		Buff[j] = 0;

		if (i >= Size)
			return IL_FALSE;

		XpmPredefCol(Buff, &Colours[Pos]);
	}

	return IL_TRUE;
}


ILboolean iLoadXpmInternal()
{
	char		Buffer[1024];
	ILint		Size, Pos, Width, Height, NumColours, i, x, y, Offset;
	XpmPixel	*Colours;

	Size = XpmGetsInternal(Buffer, 1024);
	if (strncmp("/* XPM */", Buffer, strlen("/* XPM */"))) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	Size = XpmGets(Buffer, 1024);
	// @TODO:  Actually check the variable name here.

	Size = XpmGets(Buffer, 1024);
	Pos = 0;
	Width = XpmGetInt(Buffer, Size, &Pos);
	Height = XpmGetInt(Buffer, Size, &Pos);
	NumColours = XpmGetInt(Buffer, Size, &Pos);

	Colours = ialloc(256 * sizeof(XpmPixel));
	if (Colours == NULL)
		return IL_FALSE;

	for (i = 0; i < NumColours; i++) {
		Size = XpmGets(Buffer, 1024);
		if (!XpmGetColour(Buffer, Size, Colours)) {
			ifree(Colours);
			return IL_FALSE;
		}
	}
	
	if (!ilTexImage(Width, Height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;
	
	for (y = 0; y < Height; y++) {
		Size = XpmGets(Buffer, 1024);
		for (x = 0; x < Width; x++) {
			Offset = y * iCurImage->Bps + x * iCurImage->Bpp;
			iCurImage->Data[Offset++] = Colours[Buffer[x+1]][0];
			iCurImage->Data[Offset++] = Colours[Buffer[x+1]][1];
			iCurImage->Data[Offset++] = Colours[Buffer[x+1]][2];
			iCurImage->Data[Offset++] = Colours[Buffer[x+1]][3];
		}
	}

	ifree(Colours);

	return IL_TRUE;
}

#endif//IL_NO_XPM