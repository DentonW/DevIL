//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 12/25/2001 <--Y2K Compliant! =]
//
// Filename: openilut/ilut_directx.h
//
// Description: DirectX functions for textures
//
//-----------------------------------------------------------------------------

#include "ilut_internal.h"
#ifdef ILUT_USE_DIRECTX8

#include <d3d8.h>
#include <d3dx8tex.h>
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")

ILimage*	MakeD3D8Compliant(D3DFORMAT *DestFormat);
ILenum		GetD3D8Compat(ILenum Format);
D3DFORMAT	GetD3DFormat(ILenum Format);

#ifndef _WIN32_WCE
ILboolean ILAPIENTRY ilutD3D8TexFromFile(IDirect3DDevice8 *Device, char *FileName, IDirect3DTexture8 **Texture)
{
	iBindImageTemp();
	if (!ilLoadImage(FileName))
		return IL_FALSE;

	*Texture = ilutD3D8Texture(Device);

	return IL_TRUE;
}
#endif//_WIN32_WCE


#ifndef _WIN32_WCE
ILboolean ILAPIENTRY ilutD3D8VolTexFromFile(IDirect3DDevice8 *Device, char *FileName, IDirect3DVolumeTexture8 **Texture)
{
	iBindImageTemp();
	if (!ilLoadImage(FileName))
		return IL_FALSE;

	*Texture = ilutD3D8VolumeTexture(Device);

	return IL_TRUE;
}
#endif//_WIN32_WCE


ILboolean ILAPIENTRY ilutD3D8TexFromFileInMemory(IDirect3DDevice8 *Device, ILvoid *Lump, ILuint Size, IDirect3DTexture8 **Texture)
{
	iBindImageTemp();
	if (!ilLoadL(IL_TYPE_UNKNOWN, Lump, Size))
		return IL_FALSE;

	*Texture = ilutD3D8Texture(Device);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D8VolTexFromFileInMemory(IDirect3DDevice8 *Device, ILvoid *Lump, ILuint Size, IDirect3DVolumeTexture8 **Texture)
{
	iBindImageTemp();
	if (!ilLoadL(IL_TYPE_UNKNOWN, Lump, Size))
		return IL_FALSE;

	*Texture = ilutD3D8VolumeTexture(Device);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D8TexFromResource(IDirect3DDevice8 *Device, HMODULE SrcModule, char *SrcResource, IDirect3DTexture8 **Texture)
{
	static HRSRC Resource;
	static ILubyte *Data;

	iBindImageTemp();

	Resource = (HRSRC)LoadResource(SrcModule, FindResource(SrcModule, SrcResource, RT_BITMAP));
	Data = (ILubyte*)LockResource(Resource);
	if (!ilLoadL(IL_TYPE_UNKNOWN, Data, SizeofResource(SrcModule, FindResource(SrcModule, SrcResource, RT_BITMAP))))
		return IL_FALSE;

	*Texture = ilutD3D8Texture(Device);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D8VolTexFromResource(IDirect3DDevice8 *Device, HMODULE SrcModule, char *SrcResource, IDirect3DVolumeTexture8 **Texture)
{
	static HRSRC Resource;
	static ILubyte *Data;

	iBindImageTemp();

	Resource = (HRSRC)LoadResource(SrcModule, FindResource(SrcModule, SrcResource, RT_BITMAP));
	Data = (ILubyte*)LockResource(Resource);
	if (!ilLoadL(IL_TYPE_UNKNOWN, Data, SizeofResource(SrcModule, FindResource(SrcModule, SrcResource, RT_BITMAP))))
		return IL_FALSE;

	*Texture = ilutD3D8VolumeTexture(Device);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D8TexFromFileHandle(IDirect3DDevice8 *Device, ILHANDLE File, IDirect3DTexture8 **Texture)
{
	iBindImageTemp();
	if (!ilLoadF(IL_TYPE_UNKNOWN, File))
		return IL_FALSE;

	*Texture = ilutD3D8Texture(Device);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D8VolTexFromFileHandle(IDirect3DDevice8 *Device, ILHANDLE File, IDirect3DVolumeTexture8 **Texture)
{
	iBindImageTemp();
	if (!ilLoadF(IL_TYPE_UNKNOWN, File))
		return IL_FALSE;

	*Texture = ilutD3D8VolumeTexture(Device);

	return IL_TRUE;
}


IDirect3DTexture8* ILAPIENTRY ilutD3D8Texture(IDirect3DDevice8 *Device)
{
	static IDirect3DTexture8 *Texture;
	static D3DLOCKED_RECT Rect;
	static D3DFORMAT Format;
	static ILimage *Image;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	Image = MakeD3D8Compliant(&Format);
	if (Image == NULL)
		return NULL;
	if (FAILED(IDirect3DDevice8_CreateTexture(Device, Image->Width, Image->Height, ilutGetInteger(ILUT_D3D_MIPLEVELS), 0, Format, D3DPOOL_MANAGED, &Texture)))
		return NULL;
	if (FAILED(IDirect3DTexture8_LockRect(Texture, 0, &Rect, NULL, 0)))
		return NULL;

	memcpy(Rect.pBits, Image->Data, Image->SizeOfPlane);
	IDirect3DTexture8_UnlockRect(Texture, 0);
	// Just let D3DX filter for us.
	D3DXFilterTexture(Texture, NULL, D3DX_DEFAULT, D3DX_FILTER_BOX);

	if (Image != ilutCurImage)
		ilCloseImage(Image);

	return Texture;
}


IDirect3DVolumeTexture8* ILAPIENTRY ilutD3D8VolumeTexture(IDirect3DDevice8 *Device)
{
	static IDirect3DVolumeTexture8 *Texture;
	static D3DLOCKED_BOX Box;
	static D3DFORMAT Format;
	static ILimage *Image;

	ilutCurImage = ilGetCurImage();
	if (ilutCurImage == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	Image = MakeD3D8Compliant(&Format);
	if (Image == NULL)
		return NULL;
	if (FAILED(IDirect3DDevice8_CreateVolumeTexture(Device, Image->Width, Image->Height, Image->Depth, 1, 0, Format, D3DPOOL_MANAGED, &Texture)))
		return NULL;	
	if (FAILED(IDirect3DVolumeTexture8_LockBox(Texture, 0, &Box, NULL, 0)))
		return NULL;

	memcpy(Box.pBits, Image->Data, Image->SizeOfData);
	IDirect3DVolumeTexture8_UnlockBox(Texture, 0);

	// We don't want to have mipmaps for such a large image.

	if (Image != ilutCurImage)
		ilCloseImage(Image);

	return Texture;
}


ILimage *MakeD3D8Compliant(D3DFORMAT *DestFormat)
{
	static ILubyte *Data;
	static ILimage *Converted, *Scaled, *CurImage;

	*DestFormat = D3DFMT_A8R8G8B8;

	// Images must be in BGRA format.
	if (ilutCurImage->Format != IL_BGRA) {
		Converted = iConvertImage(IL_BGRA, IL_UNSIGNED_BYTE);
		if (Converted == NULL)
			return NULL;
	}
	else {
		Converted = ilutCurImage;
	}

	// Images must have their origin in the upper left.
	if (Converted->Origin != IL_ORIGIN_UPPER_LEFT) {
		CurImage = ilutCurImage;
		ilSetCurImage(Converted);
		iluFlipImage();
		ilSetCurImage(CurImage);
	}

	// Images must have powers-of-2 dimensions.
	if (ilNextPower2(ilutCurImage->Width) != ilutCurImage->Width ||
		ilNextPower2(ilutCurImage->Height) != ilutCurImage->Height ||
		ilNextPower2(ilutCurImage->Depth) != ilutCurImage->Depth) {
			Scaled = iluScale_(Converted, ilNextPower2(ilutCurImage->Width),
						ilNextPower2(ilutCurImage->Height), ilNextPower2(ilutCurImage->Depth));
			if (Converted != ilutCurImage) {
				ilCloseImage(Converted);
			}
			if (Scaled == NULL) {
				return NULL;
			}
			Converted = Scaled;
	}

	return Converted;
}



//
// SaveSurfaceToFile.cpp
//
// Copyright (c) 2001 David Galeano
//
// Permission to use, copy, modify and distribute this software
// is hereby granted, provided that both the copyright notice and 
// this permission notice appear in all copies of the software, 
// derivative works or modified versions.
//
// THE AUTHOR ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
// CONDITION AND DISCLAIMS ANY LIABILITY OF ANY KIND FOR ANY DAMAGES 
// WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
//

ILAPI ILboolean ILAPIENTRY ilutD3D8LoadSurface(IDirect3DDevice8 *Device, IDirect3DSurface8 *Surface)
{
	HRESULT				hr;
	D3DSURFACE_DESC		d3dsd;
	LPDIRECT3DSURFACE8	SurfaceCopy;
	D3DLOCKED_RECT		d3dLR;
	ILboolean			bHasAlpha;
	ILubyte				*Image, *ImageAux, *Data;
	ILuint				y, x;
	ILushort			dwColor;

	IDirect3DSurface8_GetDesc(Surface, &d3dsd);

	bHasAlpha = (d3dsd.Format == D3DFMT_A8R8G8B8 || d3dsd.Format == D3DFMT_A1R5G5B5);

	if (bHasAlpha) {
		if (!ilTexImage(d3dsd.Width, d3dsd.Height, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, NULL)) {
			return IL_FALSE;
		}
	}
	else {
		if (!ilTexImage(d3dsd.Width, d3dsd.Height, 1, 3, IL_BGR, IL_UNSIGNED_BYTE, NULL)) {
			return IL_FALSE;
		}
	}

	hr = IDirect3DDevice8_CreateImageSurface(Device, d3dsd.Width, d3dsd.Height, d3dsd.Format, &SurfaceCopy);
	if (FAILED(hr)) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	hr = IDirect3DDevice8_CopyRects(Device, Surface, NULL, 0, SurfaceCopy, NULL);
	if (FAILED(hr)) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	hr = IDirect3DSurface8_LockRect(SurfaceCopy, &d3dLR, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	if (FAILED(hr)) {
		IDirect3DSurface8_Release(SurfaceCopy);
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Image = (ILubyte*)d3dLR.pBits;
	Data = ilutCurImage->Data;

	for (y = 0; y < d3dsd.Height; y++) {
		if (d3dsd.Format == D3DFMT_X8R8G8B8) {
			ImageAux = Image;
			for (x = 0; x < d3dsd.Width; x++) {
				Data[0] = ImageAux[0];
				Data[1] = ImageAux[1];
				Data[2] = ImageAux[2];

				Data += 3;
				ImageAux += 4;
			}
		}
		else if (d3dsd.Format == D3DFMT_A8R8G8B8) {
			memcpy(Data, Image, d3dsd.Width * 4);
		}
		else if (d3dsd.Format == D3DFMT_R5G6B5) {
			ImageAux = Image;
			for (x = 0; x < d3dsd.Width; x++) {
				dwColor = *((ILushort*)ImageAux);

				Data[0] = (ILubyte)((dwColor&0x001f)<<3);
				Data[1] = (ILubyte)(((dwColor&0x7e0)>>5)<<2);
				Data[2] = (ILubyte)(((dwColor&0xf800)>>11)<<3);

				Data += 3;
				ImageAux += 2;
			}
		}
		else if (d3dsd.Format == D3DFMT_X1R5G5B5) {
			ImageAux = Image;
			for (x = 0; x < d3dsd.Width; x++) {
				dwColor = *((ILushort*)ImageAux);

				Data[0] = (ILubyte)((dwColor&0x001f)<<3);
				Data[1] = (ILubyte)(((dwColor&0x3e0)>>5)<<3);
				Data[2] = (ILubyte)(((dwColor&0x7c00)>>10)<<3);

				Data += 3;
				ImageAux += 2;
			}
		}
		else if (d3dsd.Format == D3DFMT_A1R5G5B5) {
			ImageAux = Image;
			for (x = 0; x < d3dsd.Width; x++) {
				dwColor = *((ILushort*)ImageAux);

				Data[0] = (ILubyte)((dwColor&0x001f)<<3);
				Data[1] = (ILubyte)(((dwColor&0x3e0)>>5)<<3);
				Data[2] = (ILubyte)(((dwColor&0x7c00)>>10)<<3);
				Data[3] = (ILubyte)(((dwColor&0x8000)>>15)*255);

				Data += 4;
				ImageAux += 2;
			}
		}

		Image += d3dLR.Pitch;
	}

	IDirect3DSurface8_UnlockRect(SurfaceCopy);
	IDirect3DSurface8_Release(SurfaceCopy);

	return IL_TRUE;
}

#endif//ILUT_USE_DIRECTX8
