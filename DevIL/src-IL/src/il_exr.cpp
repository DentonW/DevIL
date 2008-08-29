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

#include <ImfRgba.h>
#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include <ImfTiledRgbaFile.h>
#include <ImfInputFile.h>
#include <ImfTiledInputFile.h>
#include <ImfPreviewImage.h>
#include <ImfChannelList.h>

using namespace Imath;
using namespace Imf;
using namespace std;


//! Reads an icon file.
ILboolean ilLoadIcns(ILconst_string FileName)
{

	return IL_TRUE;
}





void
loadImage (const char fileName[],
	   Box2i &displayWindow,
	   Box2i &dataWindow,
	   float &pixelAspectRatio,
	   Array<Rgba> &pixels)
{
    RgbaInputFile in (fileName);

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
	//
	// If some of the pixels in the file cannot be read,
	// print an error message, and return a partial image
	// to the caller.
	//

	cerr << e.what() << endl;
    }
}



#endif //IL_NO_EXR
