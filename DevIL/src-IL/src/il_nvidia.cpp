//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/05/2009
//
// Filename: src-IL/src/il_nvidia.c
//
// Description: Implements access to the nVidia Texture Tools library.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include "il_dds.h"
#include "il_manip.h"
#include <limits.h>

//#define IL_NO_DXTC_NVIDIA

#ifdef IL_USE_DXTC_NVIDIA
#include <nvtt/nvtt.h>
#include <nvcore/Memory.h>

using namespace nvtt;

#if defined(_WIN32) && defined(IL_USE_PRAGMA_LIBS)
	#if defined(_MSC_VER) || defined(__BORLANDC__)
		#ifndef _DEBUG
			#pragma comment(lib, "nvcore.lib")
			#pragma comment(lib, "nvtt.lib")
		#else
			#pragma comment(lib, "nvcore-d.lib")
			#pragma comment(lib, "nvtt-d.lib")
		#endif
	#endif
#endif


struct ilOutputHandler : public nvtt::OutputHandler
{
	ilOutputHandler(ILuint Width, ILuint Height)
	{
		ILuint size = Width * Height * 2;
		NewData = (ILubyte*)ialloc(size);
		if (NewData == NULL)
			return;
		Temp = NewData;
	}

	virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel)
	{
		// ignore.
	}
	virtual bool writeData(const void *data, int size)
	{
		memcpy(Temp, data, size);
		Temp += size;
		return true;
	}

	ILubyte *NewData, *Temp;
};


ILAPI ILubyte* ILAPIENTRY ilnVidiaCompressDXT1(ILubyte *Data, ILuint Width, ILuint Height)
{
	InputOptions inputOptions;
	inputOptions.setTextureLayout(TextureType_2D, Width, Height);
	inputOptions.setMipmapData(Data, Width, Height);

	OutputOptions outputOptions;
	ilOutputHandler outputHandler(Width, Height);
	outputOptions.setOutputHeader(false);
	outputOptions.setOutputHandler(&outputHandler);

	CompressionOptions compressionOptions;
	compressionOptions.setFormat(Format_DXT1);

	Compressor compressor;
	compressor.process(inputOptions, compressionOptions, outputOptions);

	return outputHandler.NewData;
}


#endif//IL_NO_DXTC_NVIDIA