//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openilu/error.c
//
// Description: Error functions
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"
#include "ilu_alloc.h"

// We can use ilSetError() in OpenILU, because it *is* exported, but there is
//	no prototype in il.h, so we define it in error.h of OpenILU

static const ILstring iluErrorStrings[IL_INVALID_CONVERSION - IL_INVALID_ENUM + 1] = {
	TEXT("invalid enumerant"),
    TEXT("out of memory"),
	TEXT("format not supported yet"),
	TEXT("internal error"),
	TEXT("invalid value"),
    TEXT("illegal operation"),
	TEXT("illegal file value"),
	TEXT("invalid file header"),
	TEXT("invalid parameter"),
	TEXT("could not open file"),
	TEXT("invalid extension"),
	TEXT("file already exists"),
	TEXT("out format equivalent"),
	TEXT("stack overflow"),
    TEXT("stack underflow"),
	TEXT("invalid conversion")
};

static const ILstring iluLibErrorStrings[IL_LIB_MNG_ERROR - IL_LIB_GIF_ERROR + 1] = {
	TEXT("gif library error"),
	TEXT("jpeg library error"),
	TEXT("png library error"),
	TEXT("tiff library error"),
	TEXT("mng library error")
};


const ILstring ILAPIENTRY iluErrorString(ILenum Error)
{
	if (Error == IL_NO_ERROR) {
		return (const ILstring)"no error";
	}
	if (Error == IL_UNKNOWN_ERROR) {
		return (const ILstring)"unknown error";
	}
	if (Error >= IL_INVALID_ENUM && Error <= IL_INVALID_CONVERSION) {
		return iluErrorStrings[Error - IL_INVALID_ENUM];
	}
	if (Error >= IL_LIB_GIF_ERROR && Error <= IL_LIB_MNG_ERROR) {
		return iluLibErrorStrings[Error - IL_LIB_GIF_ERROR];
	}

	// Siigron: changed this from NULL to "no error"
	return TEXT("no error");
}
