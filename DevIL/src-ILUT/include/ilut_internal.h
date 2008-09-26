//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/07/2002 <--Y2K Compliant! =]
//
// Filename: src-ILUT/include/ilut_internal.h
//
// Description: Internal stuff for ILUT
//
//-----------------------------------------------------------------------------


#ifndef INTERNAL_H
#define INTERNAL_H

#define _IL_BUILD_LIBRARY
#define _ILU_BUILD_LIBRARY
#define _ILUT_BUILD_LIBRARY

//#define	WIN32_LEAN_AND_MEAN

#include <string.h>

#ifdef _WIN32
	#ifdef _MSC_VER
		#if _MSC_VER > 1000
			#pragma warning(disable: 4996)	// "The POSIX name for this item is deprecated." AND "This function or variable may be unsafe."
		#endif // _MSC_VER > 1000
	#endif
#endif

/*
#ifdef _MSC_VER
	#if _MSC_VER > 1000
		pragma once
		pragma intrinsic(memcpy)
		pragma intrinsic(memset)
	#endif // _MSC_VER > 1000
#endif
*/

#ifdef HAVE_CONFIG_H
#include <IL/config.h>
#endif

#include <IL/ilut.h>
#include <IL/devil_internal_exports.h>

#include <stdlib.h>

extern ILimage *ilutCurImage;

ILvoid	ilutDefaultStates(void);

/* These defition should be collected in just one file */
#ifdef _WIN32_WCE
	#include <windows.h>
	#define IL_TEXT(s) ((char*)TEXT(s))
#elif _WIN32
	#include <windows.h>
	#define IL_TEXT(s) TEXT(s)
#else
	#define IL_TEXT(s) (s)
	#define TEXT(s) (s)
#endif

// ImageLib Utility Toolkit's OpenGL Functions
#ifdef ILUT_USE_OPENGL
	ILboolean ilutGLInit();
#endif

// ImageLib Utility Toolkit's Win32 Functions
#ifdef ILUT_USE_WIN32
	ILboolean ilutWin32Init();
#endif

// ImageLib Utility Toolkit's Win32 Functions
#ifdef ILUT_USE_DIRECTX8
	ILboolean ilutD3D8Init();
#endif

#ifdef ILUT_USE_DIRECTX9
	ILboolean ilutD3D9Init();
#endif

#define CUBEMAP_SIDES 6
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if((p)!=NULL){(p)->lpVtbl->Release(p);(p)=NULL;}}
#endif


#endif//INTERNAL_H
