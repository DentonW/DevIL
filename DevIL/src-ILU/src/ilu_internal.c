//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openilu/internal.c
//
// Description: Internal stuff for ILU
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_alloc.h"


ILdouble IL_PI      = 3.14159265359f;
ILdouble IL_DEGCONV = 0.01745329251f;
#if !defined(__APPLE__)
	ILimage *iCurImage = NULL;
#endif


// Anyway we can inline these next 3 functions in pure C without making them macros?
ILfloat ilCos(ILfloat Angle)
{
	return (ILfloat)(cos(Angle * IL_DEGCONV));
}


ILfloat ilSin(ILfloat Angle)
{
	return (ILfloat)(sin(Angle * IL_DEGCONV));
}


ILint ilRound(ILfloat Num)
{
	return (ILint)(Num + 0.5);
}
