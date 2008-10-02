//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/20/2002 <--Y2K Compliant! =]
//
// Filename: src-ILU/src/ilu_error.c
//
// Description: Error functions
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"

#ifdef _UNICODE
//#include <tchar.h>
ILconst_string iluErrorStringsW[IL_FILE_READ_ERROR - IL_INVALID_ENUM + 1] = {
	L"invalid enumerant",
    L"out of memory",
	L"format not supported yet",
	L"internal error",
	L"invalid value",
    L"illegal operation",
	L"illegal file value",
	L"invalid file header",
	L"invalid parameter",
	L"could not open file",
	L"invalid extension",
	L"file already exists",
	L"out format equivalent",
	L"stack overflow",
    L"stack underflow",
	L"invalid conversion",
	L"bad dimensions",
	L"file read error"
};

ILconst_string iluLibErrorStringsW[IL_LIB_MNG_ERROR - IL_LIB_GIF_ERROR + 1] = {
	L"gif library error",
	L"jpeg library error",
	L"png library error",
	L"tiff library error",
	L"mng library error"
};
#endif//_UNICODE

const char *iluErrorStrings[IL_FILE_READ_ERROR - IL_INVALID_ENUM + 1] = {
	"invalid enumerant",
    "out of memory",
	"format not supported yet",
	"internal error",
	"invalid value",
    "illegal operation",
	"illegal file value",
	"invalid file header",
	"invalid parameter",
	"could not open file",
	"invalid extension",
	"file already exists",
	"out format equivalent",
	"stack overflow",
    "stack underflow",
	"invalid conversion",
	"bad dimensions",
	"file read error"
};

const char *iluLibErrorStrings[IL_LIB_MNG_ERROR - IL_LIB_GIF_ERROR + 1] = {
	"gif library error",
	"jpeg library error",
	"png library error",
	"tiff library error",
	"mng library error"
};


ILstring ILAPIENTRY iluErrorString(ILenum Error)
{
#ifndef _UNICODE
	if (Error == IL_NO_ERROR) {
		return "no error";
	}
	if (Error == IL_UNKNOWN_ERROR) {
		return "unknown error";
	}
	if (Error >= IL_INVALID_ENUM && Error <= IL_FILE_READ_ERROR) {
		return iluErrorStrings[Error - IL_INVALID_ENUM];
	}
	if (Error >= IL_LIB_GIF_ERROR && Error <= IL_LIB_MNG_ERROR) {
		return iluLibErrorStrings[Error - IL_LIB_GIF_ERROR];
	}

	// Siigron: changed this from NULL to "no error"
	return "no error";
#else
	if (ilIsDisabled(IL_UNICODE)) {
		if (Error == IL_NO_ERROR) {
			return (ILstring)"no error";
		}
		if (Error == IL_UNKNOWN_ERROR) {
			return (ILstring)"unknown error";
		}
		if (Error >= IL_INVALID_ENUM && Error <= IL_FILE_READ_ERROR) {
			return (ILstring)iluErrorStrings[Error - IL_INVALID_ENUM];
		}
		if (Error >= IL_LIB_GIF_ERROR && Error <= IL_LIB_MNG_ERROR) {
			return (ILstring)iluLibErrorStrings[Error - IL_LIB_GIF_ERROR];
		}

		return (ILstring)"no error";
	}

	// Now we are dealing with Unicode strings.
	if (Error == IL_NO_ERROR) {
		return L"no error";
	}
	if (Error == IL_UNKNOWN_ERROR) {
		return L"unknown error";
	}
	if (Error >= IL_INVALID_ENUM && Error <= IL_FILE_READ_ERROR) {
		return (ILstring)iluErrorStringsW[Error - IL_INVALID_ENUM];
	}
	if (Error >= IL_LIB_GIF_ERROR && Error <= IL_LIB_MNG_ERROR) {
		return (ILstring)iluLibErrorStringsW[Error - IL_LIB_GIF_ERROR];
	}

	return L"no error";
#endif
}
