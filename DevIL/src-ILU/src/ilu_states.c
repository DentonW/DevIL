//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 10/12/2001 <--Y2K Compliant! =]
//
// Filename: openilu/states.c
//
// Description: The state machine
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_states.h"
#include "ilu_alloc.h"


const ILstring _iluVendor		= TEXT("Abysmal Software");
const ILstring _iluVersion		= TEXT("Developer's Image Library Utilities (ILU) 1.2.2");


const ILstring ILAPIENTRY iluGetString(ILenum StringName)
{
	switch (StringName)
	{
		case IL_VENDOR:
			return (const ILstring)_iluVendor;
		case IL_VERSION:
			return (const ILstring)_iluVersion;
		default:
			ilSetError(ILU_INVALID_PARAM);
			break;
	}
	return NULL;
}


ILvoid ILAPIENTRY iluGetIntegerv(ILenum Mode, ILint *Param)
{
	switch (Mode)
	{
		case ILU_VERSION_NUM:
			*Param = ILU_VERSION;
			break;

		case ILU_FILTER:
			*Param = iluFilter;
			break;

		default:
			ilSetError(ILU_INVALID_ENUM);
	}
	return;
}


ILint ILAPIENTRY iluGetInteger(ILenum Mode)
{
	static ILint Temp;
	Temp = 0;
	iluGetIntegerv(Mode, &Temp);
	return Temp;
}


ILenum iluFilter = ILU_NEAREST;
ILenum iluPlacement = ILU_CENTER;

ILvoid ILAPIENTRY iluImageParameter(ILenum PName, ILenum Param)
{
	switch (PName)
	{
		case ILU_FILTER:
			switch (Param)
			{
				case ILU_NEAREST:
				case ILU_LINEAR:
				case ILU_BILINEAR:
				case ILU_SCALE_BOX:
				case ILU_SCALE_TRIANGLE:
				case ILU_SCALE_BELL:
				case ILU_SCALE_BSPLINE:
				case ILU_SCALE_LANCZOS3:
				case ILU_SCALE_MITCHELL:
					iluFilter = Param;
					break;
				default:
					ilSetError(ILU_INVALID_ENUM);
					return;
			}
			break;

		case ILU_PLACEMENT:
			switch (Param)
			{
				case ILU_LOWER_LEFT:
				case ILU_LOWER_RIGHT:
				case ILU_UPPER_LEFT:
				case ILU_UPPER_RIGHT:
				case ILU_CENTER:
					iluPlacement = Param;
					break;
				default:
					ilSetError(ILU_INVALID_ENUM);
					return;
			}
			break;

		default:
			ilSetError(ILU_INVALID_ENUM);
			return;
	}

	return;
}
