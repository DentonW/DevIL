//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/istack.h
//
// Description: The main image stack
//
//-----------------------------------------------------------------------------

#ifndef IMAGESTACK_H
#define IMAGESTACK_H

#include "il_internal.h"


// Just a guess...seems large enough
#define I_STACK_INCREMENT 1024

typedef struct iFree
{
	ILuint	Name;
	void	*Next;
} iFree;


// Internal functions
ILboolean	iEnlargeStack(ILvoid);
ILvoid		iFreeMem(ILvoid);

// In alloc.c
void		AddToAtexit();

// Globals for istack.c
ILuint		StackSize = 0;
ILuint		LastUsed = 0;
ILuint		CurName = 0;
ILimage		**ImageStack = NULL;
iFree		*FreeNames = NULL;
ILboolean	OnExit = IL_FALSE;
ILboolean	ParentImage = IL_TRUE;


#endif//IMAGESTACK_H
