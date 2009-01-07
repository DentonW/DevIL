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
	ilOutputHandler(ILuint Width, ILuint Height, ILenum DxtType)
	{
		ILuint size;

		Width = Width + (4 - (Width % 4)) % 4;    // Operates on 4x4 blocks,
		Height = Height + (4 - (Height % 4)) % 4; //  so gives extra room.
		
		switch (DxtType)
		{
			case IL_DXT1:
				size = Width * Height / 2;
				break;
			case IL_DXT2:
			case IL_DXT3:
			case IL_DXT4:
			case IL_DXT5:
				size = Width * Height;
				break;

			default:
				// Should error somehow...
				break;
		}
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
	inputOptions.setMipmapGeneration(false, -1);  //@TODO: Use this in certain cases.

	OutputOptions outputOptions;
	ilOutputHandler outputHandler(Width, Height, IL_DXT1);
	outputOptions.setOutputHeader(false);
	outputOptions.setOutputHandler(&outputHandler);

	CompressionOptions compressionOptions;
	compressionOptions.setFormat(Format_DXT1);

	Compressor compressor;
	compressor.process(inputOptions, compressionOptions, outputOptions);

	return outputHandler.NewData;
}


#endif//IL_NO_DXTC_NVIDIA