//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 06/23/2002 <--Y2K Compliant! =]
//
// Filename: IL/ilut.h
//
// Description: The main include file for ILUT
//
//-----------------------------------------------------------------------------


#ifndef __ilut_h_
#ifndef __ILUT_H__

#define __ilut_h_
#define __ILUT_H__

#include <IL/il.h>
#include <IL/ilu.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	#ifndef IL_STATIC_LIB
		#if defined(_MSC_VER) || defined(__BORLANDC__)
			#ifndef _ILUT_BUILD_LIBRARY
				#ifdef IL_DEBUG
					#pragma comment(lib, "ilut-d.lib")
				#else
					#pragma comment(lib, "ilut.lib")
				#endif//_DEBUG
			#endif//_ILUT_BUILD_LIBRARY
		#endif//_MSC_VER || __BORLANDC__
	#endif//IL_STATIC_LIB
#endif//_WIN32


#define ILUT_VERSION_1_6_1					1
#define ILUT_VERSION						161


// Attribute Bits
#define ILUT_OPENGL_BIT						0x00000001
#define ILUT_D3D_BIT						0x00000002
#define ILUT_ALL_ATTRIB_BITS				0x000FFFFF


// Error Types
#define ILUT_INVALID_ENUM					0x0501
#define ILUT_OUT_OF_MEMORY					0x0502
#define ILUT_INVALID_VALUE					0x0505
#define ILUT_ILLEGAL_OPERATION				0x0506
#define ILUT_INVALID_PARAM					0x0509
#define ILUT_COULD_NOT_OPEN_FILE			0x050A
#define ILUT_STACK_OVERFLOW					0x050E
#define ILUT_STACK_UNDERFLOW				0x050F
#define ILUT_BAD_DIMENSIONS					0x0511
#define ILUT_NOT_SUPPORTED					0x0550


// State Definitions
#define ILUT_PALETTE_MODE					0x0600
#define ILUT_OPENGL_CONV					0x0610
#define ILUT_D3D_MIPLEVELS					0x0620
#define ILUT_MAXTEX_WIDTH					0x0630
#define ILUT_MAXTEX_HEIGHT					0x0631
#define ILUT_MAXTEX_DEPTH					0x0632
#define ILUT_GL_USE_S3TC					0x0634
#define ILUT_D3D_USE_DXTC					0x0634
#define ILUT_GL_GEN_S3TC					0x0635
#define ILUT_D3D_GEN_DXTC					0x0635
#define ILUT_S3TC_FORMAT					0x0705
#define ILUT_DXTC_FORMAT					0x0705
#define ILUT_D3D_POOL						0x0706


// Values
#define ILUT_VERSION_NUM					0x0DE2


// ImageLib Utility Toolkit Functions
ILAPI ILboolean		ILAPIENTRY ilutDisable(ILenum Mode);
ILAPI ILboolean		ILAPIENTRY ilutEnable(ILenum Mode);
ILAPI ILboolean		ILAPIENTRY ilutGetBoolean(ILenum Mode);
ILAPI ILvoid		ILAPIENTRY ilutGetBooleanv(ILenum Mode, ILboolean *Param);
ILAPI ILint			ILAPIENTRY ilutGetInteger(ILenum Mode);
ILAPI ILvoid		ILAPIENTRY ilutGetIntegerv(ILenum Mode, ILint *Param);
ILAPI const char*	ILAPIENTRY ilutGetString(ILenum StringName);
ILAPI ILvoid		ILAPIENTRY ilutInit(ILvoid);
ILAPI ILboolean		ILAPIENTRY ilutIsDisabled(ILenum Mode);
ILAPI ILboolean		ILAPIENTRY ilutIsEnabled(ILenum Mode);
ILAPI ILvoid		ILAPIENTRY ilutPopAttrib(ILvoid);
ILAPI ILvoid		ILAPIENTRY ilutPushAttrib(ILuint Bits);
ILAPI ILvoid		ILAPIENTRY ilutSetInteger(ILenum Mode, ILint Param);


// The different rendering api's...more to be added later?
#define ILUT_OPENGL		0
#define ILUT_ALLEGRO	1
#define ILUT_WIN32		2
#define ILUT_DIRECT3D8	3


ILAPI ILboolean	ILAPIENTRY ilutRenderer(ILenum Renderer);

// Includes specific config
#ifdef DJGPP
	#define ILUT_USE_ALLEGRO
#elif _WIN32_WCE
	#define ILUT_USE_WIN32
#elif _WIN32
	//#ifdef __GNUC__ //__CYGWIN32__ (Cygwin seems to not define this with DevIL builds)
		#include "config.h"

		/*// Temporary fix for the SDL main() linker bug.
		#ifdef  ILUT_USE_SDL
		#undef  ILUT_USE_SDL
		#endif//ILUT_USE_SDL*/

	/*#else
	  	#define ILUT_USE_WIN32
		#define ILUT_USE_OPENGL
		#define ILUT_USE_SDL
		#define ILUT_USE_DIRECTX8
	#endif*/
#elif BEOS  // Don't know the #define
	#define ILUT_USE_BEOS
	#define ILUT_USE_OPENGL
#elif MACOSX
	#define ILUT_USE_OPENGL
#else
	/*
	* We are surely using a *nix so the configure script
	* may have written the configured config.h header
	*/
	#include "config.h"
#endif

// ImageLib Utility Toolkit's OpenGL Functions
#ifdef ILUT_USE_OPENGL
	#ifdef _MSC_VER
		//#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif//_MSC_VER

	#ifdef __APPLE__
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
	#else
	 	#include <GL/gl.h>
 		#include <GL/glu.h>
	#endif//__APPLE__

	ILAPI GLuint	ILAPIENTRY ilutGLBindTexImage(ILvoid);
	ILAPI GLuint	ILAPIENTRY ilutGLBindMipmaps(ILvoid);
	ILAPI ILboolean	ILAPIENTRY ilutGLBuildMipmaps(ILvoid);
	ILAPI GLuint	ILAPIENTRY ilutGLLoadImage(const ILstring FileName);
	ILAPI ILboolean	ILAPIENTRY ilutGLScreen(ILvoid);
	ILAPI ILboolean	ILAPIENTRY ilutGLScreenie(ILvoid);
	ILAPI ILboolean	ILAPIENTRY ilutGLSaveImage(const ILstring FileName, GLuint TexID);
	ILAPI ILboolean	ILAPIENTRY ilutGLSetTex(GLuint TexID);
	ILAPI ILboolean	ILAPIENTRY ilutGLTexImage(GLuint Level);

#endif//ILUT_USE_OPENGL


// ImageLib Utility Toolkit's Allegro Functions
#ifdef ILUT_USE_ALLEGRO
	#include <allegro.h>
	ILAPI BITMAP* ILAPIENTRY ilutAllegLoadImage(const ILstring FileName);
	ILAPI BITMAP* ILAPIENTRY ilutConvertToAlleg(PALETTE Pal);
#endif//ILUT_USE_ALLEGRO


// ImageLib Utility Toolkit's SDL Functions
#ifdef ILUT_USE_SDL
	#include <SDL.h>
	ILAPI SDL_Surface*	ILAPIENTRY ilutConvertToSDLSurface(unsigned int flags);
	ILAPI SDL_Surface*	ILAPIENTRY ilutSDLSurfaceLoadImage(const ILstring FileName);
	ILAPI ILboolean		ILAPIENTRY ilutSDLSurfaceFromBitmap(SDL_Surface *Bitmap);
#endif//ILUT_USE_SDL


// ImageLib Utility Toolkit's BeOS Functions
#ifdef  ILUT_USE_BEOS
	ILAPI BBitmap ILAPIENTRY ilutConvertToBBitmap(ILvoid);
#endif//ILUT_USE_BEOS


// ImageLib Utility Toolkit's Win32 GDI Functions
#ifdef ILUT_USE_WIN32
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		ILAPI HBITMAP	ILAPIENTRY ilutConvertToHBitmap(HDC hDC);
		ILAPI ILvoid	ILAPIENTRY ilutFreePaddedData(ILubyte *Data);
		ILAPI ILvoid	ILAPIENTRY ilutGetBmpInfo(BITMAPINFO *Info);
		ILAPI HPALETTE	ILAPIENTRY ilutGetHPal(ILvoid);
		ILAPI ILubyte*	ILAPIENTRY ilutGetPaddedData(ILvoid);
		ILAPI ILboolean	ILAPIENTRY ilutGetWinClipboard(ILvoid);
		ILAPI ILboolean	ILAPIENTRY ilutLoadResource(HINSTANCE hInst, ILint ID, const ILstring ResourceType, ILenum Type);
		ILAPI ILboolean	ILAPIENTRY ilutSetHBitmap(HBITMAP Bitmap);
		ILAPI ILboolean	ILAPIENTRY ilutSetHPal(HPALETTE Pal);
		ILAPI ILboolean	ILAPIENTRY ilutSetWinClipboard(ILvoid);
		ILAPI HBITMAP	ILAPIENTRY ilutWinLoadImage(const ILstring FileName, HDC hDC);
		ILAPI ILboolean	ILAPIENTRY ilutWinLoadUrl(const ILstring Url);
		ILAPI ILboolean ILAPIENTRY ilutWinPrint(ILuint XPos, ILuint YPos, ILuint Width, ILuint Height, HDC hDC);
		ILAPI ILboolean	ILAPIENTRY ilutWinSaveImage(const ILstring FileName, HBITMAP Bitmap);

	#endif//_WIN32
#endif//ILUT_USE_WIN32


// ImageLib Utility Toolkit's DirectX 8 Functions
#ifdef ILUT_USE_DIRECTX8
	#ifdef _WIN32
		#include <d3d8.h>
		ILAPI ILvoid	ILAPIENTRY ilutD3D8MipFunc(ILuint NumLevels);
		ILAPI IDirect3DTexture8* ILAPIENTRY ilutD3D8Texture(IDirect3DDevice8 *Device);
		ILAPI IDirect3DVolumeTexture8* ILAPIENTRY ilutD3D8VolumeTexture(IDirect3DDevice8 *Device);
		ILAPI ILboolean	ILAPIENTRY ilutD3D8TexFromFile(IDirect3DDevice8 *Device, char *FileName, IDirect3DTexture8 **Texture);
		ILAPI ILboolean	ILAPIENTRY ilutD3D8VolTexFromFile(IDirect3DDevice8 *Device, char *FileName, IDirect3DVolumeTexture8 **Texture);
		ILAPI ILboolean	ILAPIENTRY ilutD3D8TexFromFileInMemory(IDirect3DDevice8 *Device, ILvoid *Lump, ILuint Size, IDirect3DTexture8 **Texture);
		ILAPI ILboolean	ILAPIENTRY ilutD3D8VolTexFromFileInMemory(IDirect3DDevice8 *Device, ILvoid *Lump, ILuint Size, IDirect3DVolumeTexture8 **Texture);
		ILAPI ILboolean	ILAPIENTRY ilutD3D8TexFromFileHandle(IDirect3DDevice8 *Device, ILHANDLE File, IDirect3DTexture8 **Texture);
		ILAPI ILboolean	ILAPIENTRY ilutD3D8VolTexFromFileHandle(IDirect3DDevice8 *Device, ILHANDLE File, IDirect3DVolumeTexture8 **Texture);
		// These two are not tested yet.
		ILAPI ILboolean ILAPIENTRY ilutD3D8TexFromResource(IDirect3DDevice8 *Device, HMODULE SrcModule, char *SrcResource, IDirect3DTexture8 **Texture);
		ILAPI ILboolean ILAPIENTRY ilutD3D8VolTexFromResource(IDirect3DDevice8 *Device, HMODULE SrcModule, char *SrcResource, IDirect3DVolumeTexture8 **Texture);

		ILAPI ILboolean ILAPIENTRY ilutD3D8LoadSurface(IDirect3DDevice8 *Device, IDirect3DSurface8 *Surface);
	#endif//_WIN32
#endif//ILUT_USE_DIRECTX8


#ifdef __cplusplus
}
#endif

#endif // __ILUT_H__
#endif // __ilut_h_
