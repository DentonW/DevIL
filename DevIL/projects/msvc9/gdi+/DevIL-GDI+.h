//-----------------------------------------------------------------------------
//
// DevIL GDI+ Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/13/2001 <--Y2K Compliant! =]
//
// Filename: gdi+/devil-gdi+.h
//
// Description: GDI+ CBitmap loading routines
//
//-----------------------------------------------------------------------------


#ifndef __gdi_il_h_
#ifndef __GDI_IL_H__

#define __gdi_il_h_
#define __GDI_IL_H__

#include <windows.h>
#include <shlobj.h>  // Needed to #include <Gdiplus.h>
#include <Gdiplus.h>
#include <IL/ilut.h>
using namespace Gdiplus;

Bitmap *ilutConvertToBitmap();
Bitmap *ilutGDILoadImage(const ILstring FileName);

#endif // __GDI_IL_H__
#endif // __gdi_il_h__
