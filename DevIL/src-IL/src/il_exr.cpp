//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 08/29/2008
//
// Filename: src-IL/src/il_exr.cpp
//
// Description: Reads from an OpenEXR (.exr) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_EXR

#ifndef HAVE_CONFIG_H //we are probably on a Windows box 
//#define OPENEXR_DLL
#define HALF_EXPORTS
#endif //HAVE_CONFIG_H

#include "il_exr.h"
#include <ImfRgba.h>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
//#include <ImfTiledRgbaFile.h>
//#include <ImfInputFile.h>
//#include <ImfTiledInputFile.h>
//#include <ImfPreviewImage.h>
//#include <ImfChannelList.h>



#if (defined(_WIN32) || defined(_WIN64)) && defined(IL_USE_PRAGMA_LIBS)
	#if defined(_MSC_VER) || defined(__BORLANDC__)
		#ifndef _DEBUG
			#pragma comment(lib, "openexr.lib")
		#else
			#pragma comment(lib, "openexr-d.lib")
		#endif
	#endif
#endif


// @TODO: Determine how to load from file streams and lumps.
//   Probably need to overload RgbaInputFile.

//! Reads an .exr file.
ILboolean ilLoadExr(ILconst_string FileName)
{
	ILboolean bRet;
	bRet = iLoadExrInternal(FileName);

	return bRet;
}



using namespace Imath;
using namespace Imf;
using namespace std;


ILboolean iLoadExrInternal(ILconst_string FileName)
{
	Array<Rgba> pixels;
	Box2i displayWindow, dataWindow;
	float pixelAspectRatio;
    RgbaInputFile in ((const char*)FileName);  // @TODO: This will completely fail with Unicode.

	//RgbaInputFile in("E:/DevIL/extlibs/OpenEXR/openexr-images-1.5.0/ScanLines/Tree.exr");

	displayWindow = in.displayWindow();
    dataWindow = in.dataWindow();
    pixelAspectRatio = in.pixelAspectRatio();

    int dw, dh, dx, dy;

	dw = dataWindow.max.x - dataWindow.min.x + 1;
    dh = dataWindow.max.y - dataWindow.min.y + 1;
    dx = dataWindow.min.x;
    dy = dataWindow.min.y;

    pixels.resizeErase (dw * dh);
	in.setFrameBuffer (pixels - dx - dy * dw, 1, dw);

	try
    {
		in.readPixels (dataWindow.min.y, dataWindow.max.y);
    }
    catch (const exception &e)
    {
	// If some of the pixels in the file cannot be read,
	// print an error message, and return a partial image
	// to the caller.

		return IL_FALSE;
    }

	//if (ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL) == IL_FALSE)
	//if (ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_UNSIGNED_SHORT, NULL) == IL_FALSE)
	if (ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_FLOAT, NULL) == IL_FALSE)
		return IL_FALSE;
	
	if (in.lineOrder() == INCREASING_Y)
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	else
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;


	for (int i = 0; i < dw * dh; i++)
	{
		// Too much data lost
		//iCurImage->Data[i * 4 + 0] = (ILubyte)(pixels[i].r.bits() >> 8);
		//iCurImage->Data[i * 4 + 1] = (ILubyte)(pixels[i].g.bits() >> 8);
		//iCurImage->Data[i * 4 + 2] = (ILubyte)(pixels[i].b.bits() >> 8);
		//iCurImage->Data[i * 4 + 3] = (ILubyte)(pixels[i].a.bits() >> 8);

		// The images look kind of washed out with this.
		//((ILshort*)(iCurImage->Data))[i * 4 + 0] = pixels[i].r.bits();
		//((ILshort*)(iCurImage->Data))[i * 4 + 1] = pixels[i].g.bits();
		//((ILshort*)(iCurImage->Data))[i * 4 + 2] = pixels[i].b.bits();
		//((ILshort*)(iCurImage->Data))[i * 4 + 3] = pixels[i].a.bits();

		// This seems to give the best results, but the images can be saturated quite a bit.
		((ILfloat*)(iCurImage->Data))[i * 4 + 0] = pixels[i].r;
		((ILfloat*)(iCurImage->Data))[i * 4 + 1] = pixels[i].g;
		((ILfloat*)(iCurImage->Data))[i * 4 + 2] = pixels[i].b;
		((ILfloat*)(iCurImage->Data))[i * 4 + 3] = pixels[i].a;

	}

	return IL_TRUE;
}



#endif //IL_NO_EXR
