//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/04/2009
//
// Filename: src-IL/src/il_exr.cpp
//
// Description: Reads from an OpenEXR (.exr) file using the OpenEXR library.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_EXR

#ifndef HAVE_CONFIG_H // We are probably on a Windows box .
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


//! Checks if the file specified in FileName is a valid EXR file.
ILboolean ilIsValidExr(ILconst_string FileName)
{
	ILHANDLE	ExrFile;
	ILboolean	bExr = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("exr"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bExr;
	}
	
	ExrFile = iopenr(FileName);
	if (ExrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bExr;
	}
	
	bExr = ilIsValidExrF(ExrFile);
	icloser(ExrFile);
	
	return bExr;
}


//! Checks if the ILHANDLE contains a valid EXR file at the current position.
ILboolean ilIsValidExrF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidExr();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid EXR lump.
ILboolean ilIsValidExrL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidExr();
}


// Internal function used to get the EXR header from the current file.
ILboolean iGetExrHead(EXRHEAD *Header)
{
	Header->MagicNumber = GetLittleUInt();
	Header->Version = GetLittleUInt();

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidExr()
{
	EXRHEAD Head;

	if (!iGetExrHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(EXRHEAD), IL_SEEK_CUR);
	
	return iCheckExr(&Head);
}


// Internal function used to check if the HEADER is a valid EXR header.
ILboolean iCheckExr(EXRHEAD *Header)
{
	// The file magic number (signature) is 0x76, 0x2f, 0x31, 0x01
	if (Header->MagicNumber != 0x01312F76)
		return IL_FALSE;
	// The only valid version so far is version 2.  The upper value has
	//  to do with tiling.
	if (Header->Version != 0x002 && Header->Version != 0x202)
		return IL_FALSE;

	return IL_TRUE;
}


// Nothing to do here in the constructor.
ilIStream::ilIStream() : Imf::IStream("N/A")
{
	return;
}


bool ilIStream::read(char c[], int n)
{
	if (iread(c, 1, n) != n)
		return false;
	return true;
}


//@TODO: Make this work with 64-bit values.
Imf::Int64 ilIStream::tellg()
{
	Imf::Int64 Pos;

	// itell only returns a 32-bit value!
	Pos = itell();

	return Pos;
}


// Note that there is no return value here, even though there probably should be.
//@TODO: Make this work with 64-bit values.
void ilIStream::seekg(Imf::Int64 Pos)
{
	// iseek only uses a 32-bit value!
	iseek((ILint)Pos, IL_SEEK_SET);  // I am assuming this is seeking from the beginning.
	return;
}


void ilIStream::clear()
{
	return;
}


//! Reads an .exr file.
ILboolean ilLoadExr(ILconst_string FileName)
{
	ILHANDLE	ExrFile;
	ILboolean	bExr = IL_FALSE;
	
	ExrFile = iopenr(FileName);
	if (ExrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bExr;
	}

	bExr = ilLoadExrF(ExrFile);
	icloser(ExrFile);

	return bExr;
}


//! Reads an already-opened .exr file
ILboolean ilLoadExrF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadExrInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains an .exr
ILboolean ilLoadExrL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadExrInternal();
}


using namespace Imath;
using namespace Imf;
using namespace std;


ILboolean iLoadExrInternal()
{
	Array<Rgba> pixels;
	Box2i dataWindow;
	float pixelAspectRatio;
	ILfloat *FloatData;

	ilIStream File;
	RgbaInputFile in(File);

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
		ilSetError(IL_LIB_EXR_ERROR);  // Could I use something a bit more descriptive based on e?
		e;  // Prevent the compiler from yelling at us about this being unused.
		return IL_FALSE;
    }

	//if (ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL) == IL_FALSE)
	//if (ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_UNSIGNED_SHORT, NULL) == IL_FALSE)
	if (ilTexImage(dw, dh, 1, 4, IL_RGBA, IL_FLOAT, NULL) == IL_FALSE)
		return IL_FALSE;

	// Determine where the origin is in the original file.
	if (in.lineOrder() == INCREASING_Y)
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	else
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Better to access FloatData instead of recasting everytime.
	FloatData = (ILfloat*)iCurImage->Data;

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

		// This gives the best results, since no data is lost.
		FloatData[i * 4]     = pixels[i].r;
		FloatData[i * 4 + 1] = pixels[i].g;
		FloatData[i * 4 + 2] = pixels[i].b;
		FloatData[i * 4 + 3] = pixels[i].a;
	}

	// Converts the image to predefined type, format and/or origin if needed.
	ilFixImage();

	return IL_TRUE;
}



#endif //IL_NO_EXR
