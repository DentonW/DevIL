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

#include "il_exr.h"
#include <ImfRgba.h>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
//#include <ImfTiledRgbaFile.h>
//#include <ImfInputFile.h>
//#include <ImfTiledInputFile.h>
//#include <ImfPreviewImage.h>
//#include <ImfChannelList.h>


//! Reads an .exr file.
ILboolean ilLoadExr(ILconst_string FileName)
{
	return iLoadExrInternal(FileName);
}



using namespace Imath;
using namespace Imf;
using namespace std;


ILboolean iLoadExrInternal(const char FileName[])
{
	Array<Rgba> pixels;
	Box2i displayWindow, dataWindow;
	float pixelAspectRatio;
    RgbaInputFile in (FileName);

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

    //try
    //{
		in.readPixels (dataWindow.min.y, dataWindow.max.y);
    //}
    //catch (const exception &e)
    //{
	//
	// If some of the pixels in the file cannot be read,
	// print an error message, and return a partial image
	// to the caller.
	//

	//	return IL_FALSE;
    //}

	return ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
}



#endif //IL_NO_EXR
