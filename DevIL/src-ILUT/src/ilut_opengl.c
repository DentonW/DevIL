//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/28/2001 <--Y2K Compliant! =]
//
// Filename: openilut/opengl.c
//
// Description: OpenGL functions for images
//
//-----------------------------------------------------------------------------


#include "ilut_internal.h"
#ifdef ILUT_USE_OPENGL
#include "ilut_opengl.h"
#include <stdio.h>
#include <string.h>


#ifdef  _MSC_VER
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
#endif//_MSC_VER


ILint MaxTexW = 256, MaxTexH = 256;  // maximum texture widths and heights


// Absolutely *have* to call this if planning on using the image library with OpenGL.
//	Call this after OpenGL has initialized.
ILboolean ilutGLInit()
{
	// Use PROXY_TEXTURE_2D with glTexImage2D() to test more accurately...
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTexW);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTexH);
	if (MaxTexW == 0 || MaxTexH == 0)
		MaxTexW = MaxTexH = 256;  // Trying this because of the VooDoo series of cards...

	// Should we really be setting all this ourselves?  Seems too much like a glu(t) approach...
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	return IL_TRUE;
}


ILvoid iGLSetMaxW(ILuint Width)
{
	MaxTexW = Width;
	return;
}

ILvoid iGLSetMaxH(ILuint Height)
{
	MaxTexH = Height;
	return;
}


// @TODO:  Check what dimensions an image has and use the appropriate IL_IMAGE_XD #define!

GLuint ILAPIENTRY ilutGLBindTexImage()
{
	GLuint	TexID = 0;

	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, IL_FALSE);

	if (!ilutGLTexImage(0)) {
		glDeleteTextures(1, &TexID);
		return 0;
	}

	return TexID;
}


// We assume *all* states have been set by the user, including 2d texturing!
ILboolean ILAPIENTRY ilutGLTexImage(GLuint Level)
{
	ILimage	*Image;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Image = MakeGLCompliant(iCurImage);
	if (Image == NULL)
		return IL_FALSE;

	glTexImage2D(GL_TEXTURE_2D, 0, ilutGLFormat(Image->Format, Image->Bpp), Image->Width,
				Image->Height, 0, Image->Format, Image->Type, Image->Data);

	if (Image != iCurImage)
		ilCloseImage(Image);

	return IL_TRUE;
}


GLuint ILAPIENTRY ilutGLBindMipmaps()
{
	GLuint	TexID = 0;

//	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (!ilutGLBuildMipmaps()) {
		glDeleteTextures(1, &TexID);
		return 0;
	}

//	glPopAttrib();

	return TexID;
}


ILboolean ILAPIENTRY ilutGLBuildMipmaps()
{
	ILimage	*Image;

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Image = MakeGLCompliant(iCurImage);
	if (Image == NULL)
		return IL_FALSE;

	gluBuild2DMipmaps(GL_TEXTURE_2D, ilutGLFormat(Image->Format, Image->Bpp), Image->Width,
						Image->Height, Image->Format, Image->Type, Image->Data);

	if (Image != iCurImage)
		ilCloseImage(Image);
	
	return IL_TRUE;
}


ILimage* MakeGLCompliant(ILimage *Src)
{
	ILimage		*Dest = Src, *Temp;
	ILboolean	Created = IL_FALSE;
	ILenum		Filter;
	ILubyte		*Flipped;

	if (Src->Pal.Palette != NULL && Src->Pal.PalSize != 0 && Src->Pal.PalType != IL_PAL_NONE) {
		//ilSetCurImage(Src);
		Dest = iConvertImage(ilGetPalBaseType(Src->Pal.PalType), IL_UNSIGNED_BYTE);
		//Dest = iConvertImage(IL_BGR);
		//ilSetCurImage(iCurImage);
		if (Dest == NULL)
			return NULL;

		Created = IL_TRUE;

		// Change here!


		// Set Dest's palette stuff here
		Dest->Pal.PalType = IL_PAL_NONE;
	}

	if (Src->Width != ilNextPower2(Src->Width) || Src->Height != ilNextPower2(Src->Height) ||
		(ILint)Src->Width > MaxTexW || (ILint)Src->Height > MaxTexH) {
		if (!Created) {
			Dest = ilCopyImage_(Src);
			if (Dest == NULL) {
				return NULL;
			}
			Created = IL_TRUE;
		}

		Filter = iluGetInteger(ILU_FILTER);
		if (Src->Format == IL_COLOUR_INDEX) {
			iluImageParameter(ILU_FILTER, ILU_NEAREST);
			Temp = iluScale_(Dest, min(MaxTexW, (ILint)ilNextPower2(Dest->Width)), min(MaxTexH, (ILint)ilNextPower2(Dest->Height)), 1);
			iluImageParameter(ILU_FILTER, Filter);
		}
		else {
			iluImageParameter(ILU_FILTER, ILU_BILINEAR);
			Temp = iluScale_(Dest, min(MaxTexW, (ILint)ilNextPower2(Dest->Width)), min(MaxTexH, (ILint)ilNextPower2(Dest->Height)), 1);
			iluImageParameter(ILU_FILTER, Filter);
		}

		ilCloseImage(Dest);
		if (!Temp) {
			return NULL;
		}
		Dest = Temp;
	}

	if (Dest->Origin != (ILenum)ilGetInteger(IL_ORIGIN_MODE)) {
		Flipped = iGetFlipped(Dest);
		free(Dest->Data);
		Dest->Data = Flipped;
	}

	return Dest;
}


//! Just a convenience function.
#ifndef _WIN32_WCE
GLuint ILAPIENTRY ilutGLLoadImage(const ILstring FileName)
{
	GLuint	TexId;
	//ILuint	Id;

	iBindImageTemp();
	//ilGenImages(1, &Id);
	//ilBindImage(Id);

	if (!ilLoadImage(FileName))
		return 0;

	TexId = ilutGLBindTexImage();

	//ilDeleteImages(1, &Id);

	return TexId;
}
#endif//_WIN32_WCE


#ifndef _WIN32_WCE
ILboolean ILAPIENTRY ilutGLSaveImage(const ILstring FileName, GLuint TexID)
{
	ILuint		CurName;
	ILboolean	Saved;
	
	CurName = ilGetCurName();

	iBindImageTemp();

	if (!ilutGLSetTex(TexID)) {
		ilBindImage(CurName);
		return IL_FALSE;
	}

	Saved = ilSaveImage(FileName);
	ilBindImage(CurName);

	return Saved;
}
#endif//_WIN32_WCE


//! Takes a screenshot of the current OpenGL window.
ILboolean ILAPIENTRY ilutGLScreen()
{
	ILuint	ViewPort[4];

	iCurImage = ilGetCurImage();
	if (iCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	glGetIntegerv(GL_VIEWPORT, ViewPort);

	ilTexImage(ViewPort[2], ViewPort[3], 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	glReadPixels(0, 0, ViewPort[2], ViewPort[3], GL_RGB, GL_UNSIGNED_BYTE, iCurImage->Data);

	return IL_TRUE;
}


#ifndef _WIN32_WCE
ILboolean ILAPIENTRY ilutGLScreenie()
{
	FILE		*File;
	char		Buff[255];
	ILuint		i, CurName;
	ILboolean	ReturnVal = IL_TRUE;

	CurName = ilGetCurName();

	// Could go above 128 easily...
	for (i = 0; i < 128; i++) {
		sprintf(Buff, "screen%d.tga", i);
		File = fopen(Buff, "rb");
		if (!File)
			break;
		fclose(File);
	}

	if (i == 127) {
		ilSetError(ILUT_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	iBindImageTemp();
	if (!ilutGLScreen()) {
		ReturnVal = IL_FALSE;
	}

	if (ReturnVal)
		ilSave(IL_TGA, Buff);

	ilBindImage(CurName);

	return ReturnVal;
}
#endif//_WIN32_WCE


ILboolean ILAPIENTRY ilutGLSetTex(GLuint TexID)
{
	ILubyte *Data;
	ILuint Width, Height;

	glBindTexture(GL_TEXTURE_2D, TexID);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &Width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &Height);

	Data = (ILubyte*)malloc(Width * Height * 4);
	if (Data == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	glGetTexImage(GL_TEXTURE_2D, 0, IL_BGRA, GL_UNSIGNED_BYTE, Data);

	if (!ilTexImage(Width, Height, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, Data)) {
		free(Data);
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	free(Data);
	return IL_TRUE;
}


ILenum ilutGLFormat(ILenum Format, ILubyte Bpp)
{
	if (Format == IL_RGB || Format == IL_BGR) {
		if (ilutIsEnabled(ILUT_OPENGL_CONV)) {
			return GL_RGB8;
		}
	}
	else if (Format == IL_RGBA || Format == IL_BGRA) {
		if (ilutIsEnabled(ILUT_OPENGL_CONV)) {
			return GL_RGBA8;
		}
	}

	return Bpp;
}

#endif//ILUT_USE_OPENGL
