//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2017 by Denton Woods
// Last modified: 12/27/2008
//
// Filename: src-IL/include/il_dds.h
//
// Description: Reads from a DirectDraw Surface (.dds) file.
//
//-----------------------------------------------------------------------------


#ifndef DDS_H
#define DDS_H

#include "il_internal.h"


#ifdef _WIN32
	#pragma pack(push, dds_struct, 1)
#endif
typedef struct DDSHEAD
{
	ILbyte	Signature[4];

	ILuint	Size1;				// size of the structure (minus MagicNum)
	ILuint	Flags1; 			// determines what fields are valid
	ILuint	Height; 			// height of surface to be created
	ILuint	Width;				// width of input surface
	ILuint	LinearSize; 		// Formless late-allocated optimized surface size
	ILuint	Depth;				// Depth if a volume texture
	ILuint	MipMapCount;		// number of mip-map levels requested
	ILuint	AlphaBitDepth;		// depth of alpha buffer requested

	ILuint	NotUsed[10];

	ILuint	Size2;				// size of structure
	ILuint	Flags2;				// pixel format flags
	ILuint	FourCC;				// (FOURCC code)
	ILuint	RGBBitCount;		// how many bits per pixel
	ILuint	RBitMask;			// mask for red bit
	ILuint	GBitMask;			// mask for green bits
	ILuint	BBitMask;			// mask for blue bits
	ILuint	RGBAlphaBitMask;	// mask for alpha channel

	ILuint	ddsCaps1, ddsCaps2, ddsCaps3, ddsCaps4; // direct draw surface capabilities
	ILuint	TextureStage;
} IL_PACKSTRUCT DDSHEAD;
#ifdef _WIN32
	#pragma pack(pop, dds_struct)
#endif


#ifdef _WIN32
#pragma pack(push, dxt10_struct, 1)
#endif
typedef struct DXT10HEAD
{
	ILuint	dxgiFormat;
	ILuint	resourceDimension;
	ILuint	miscFlag;
	ILuint	arraySize;
	ILuint	miscFlags2;
} IL_PACKSTRUCT DXT10HEAD;
#ifdef _WIN32
#pragma pack(pop, dxt10_struct)
#endif


// use cast to struct instead of RGBA_MAKE as struct is
//  much
typedef struct Color8888
{
	ILubyte r;		// change the order of names to change the 
	ILubyte g;		//  order of the output ARGB or BGRA, etc...
	ILubyte b;		//  Last one is MSB, 1st is LSB.
	ILubyte a;
} Color8888;


typedef struct Color888
{
	ILubyte r;		// change the order of names to change the 
	ILubyte g;		//  order of the output ARGB or BGRA, etc...
	ILubyte b;		//  Last one is MSB, 1st is LSB.
} Color888;


typedef struct Color565
{
	unsigned nBlue  : 5;		// order of names changes
	unsigned nGreen : 6;		//  byte order of output to 32 bit
	unsigned nRed	: 5;
} Color565;


typedef struct DXTColBlock
{
	ILshort col0;
	ILshort col1;

	// no bit fields - use bytes
	ILbyte row[4];
} DXTColBlock;

typedef struct DXTAlphaBlockExplicit
{
	ILshort row[4];
} DXTAlphaBlockExplicit;

typedef struct DXTAlphaBlock3BitLinear
{
	ILbyte alpha0;
	ILbyte alpha1;

	ILbyte stuff[6];
} DXTAlphaBlock3BitLinear;


// Defines

//Those 4 were added on 20040516 to make
//the written dds files more standard compliant
#define DDS_CAPS				0x00000001L
#define DDS_HEIGHT				0x00000002L
#define DDS_WIDTH				0x00000004L

#define DDS_RGB					0x00000040L
#define DDS_PIXELFORMAT			0x00001000L

#define DDS_LUMINANCE			0x00020000L

#define DDS_ALPHAPIXELS			0x00000001L
#define DDS_ALPHA				0x00000002L
#define DDS_FOURCC				0x00000004L
#define DDS_PITCH				0x00000008L
#define DDS_COMPLEX				0x00000008L
#define DDS_TEXTURE				0x00001000L
#define DDS_MIPMAPCOUNT			0x00020000L
#define DDS_LINEARSIZE			0x00080000L
#define DDS_VOLUME				0x00200000L
#define DDS_MIPMAP				0x00400000L
#define DDS_DEPTH				0x00800000L

#define DDS_CUBEMAP				0x00000200L
#define DDS_CUBEMAP_POSITIVEX	0x00000400L
#define DDS_CUBEMAP_NEGATIVEX	0x00000800L
#define DDS_CUBEMAP_POSITIVEY	0x00001000L
#define DDS_CUBEMAP_NEGATIVEY	0x00002000L
#define DDS_CUBEMAP_POSITIVEZ	0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ	0x00008000L


#define IL_MAKEFOURCC(ch0, ch1, ch2, ch3) \
			((ILint)(ILbyte)(ch0) | ((ILint)(ILbyte)(ch1) << 8) |	\
			((ILint)(ILbyte)(ch2) << 16) | ((ILint)(ILbyte)(ch3) << 24 ))

enum PixFormat
{
	PF_ARGB,
	PF_RGB,
	PF_DXT1,
	PF_DXT2,
	PF_DXT3,
	PF_DXT4,
	PF_DXT5,
	PF_3DC,
	PF_ATI1N,
	PF_LUMINANCE,
	PF_LUMINANCE_ALPHA,
	PF_RXGB, //Doom3 normal maps
	PF_A16B16G16R16,
	PF_R16F,
	PF_G16R16F,
	PF_A16B16G16R16F,
	PF_R32F,
	PF_G32R32F,
	PF_A32B32G32R32F,
	PF_DX10,
	PF_UNKNOWN = 0xFF
};

// From https://msdn.microsoft.com/en-us/library/windows/desktop/bb173059(v=vs.85).aspx
typedef enum DXGI_FORMAT
{
	DXGI_FORMAT_UNKNOWN = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
	DXGI_FORMAT_R32G32B32A32_UINT = 3,
	DXGI_FORMAT_R32G32B32A32_SINT = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS = 5,
	DXGI_FORMAT_R32G32B32_FLOAT = 6,
	DXGI_FORMAT_R32G32B32_UINT = 7,
	DXGI_FORMAT_R32G32B32_SINT = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM = 11,
	DXGI_FORMAT_R16G16B16A16_UINT = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM = 13,
	DXGI_FORMAT_R16G16B16A16_SINT = 14,
	DXGI_FORMAT_R32G32_TYPELESS = 15,
	DXGI_FORMAT_R32G32_FLOAT = 16,
	DXGI_FORMAT_R32G32_UINT = 17,
	DXGI_FORMAT_R32G32_SINT = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM = 24,
	DXGI_FORMAT_R10G10B10A2_UINT = 25,
	DXGI_FORMAT_R11G11B10_FLOAT = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	DXGI_FORMAT_R8G8B8A8_UINT = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM = 31,
	DXGI_FORMAT_R8G8B8A8_SINT = 32,
	DXGI_FORMAT_R16G16_TYPELESS = 33,
	DXGI_FORMAT_R16G16_FLOAT = 34,
	DXGI_FORMAT_R16G16_UNORM = 35,
	DXGI_FORMAT_R16G16_UINT = 36,
	DXGI_FORMAT_R16G16_SNORM = 37,
	DXGI_FORMAT_R16G16_SINT = 38,
	DXGI_FORMAT_R32_TYPELESS = 39,
	DXGI_FORMAT_D32_FLOAT = 40,
	DXGI_FORMAT_R32_FLOAT = 41,
	DXGI_FORMAT_R32_UINT = 42,
	DXGI_FORMAT_R32_SINT = 43,
	DXGI_FORMAT_R24G8_TYPELESS = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
	DXGI_FORMAT_R8G8_TYPELESS = 48,
	DXGI_FORMAT_R8G8_UNORM = 49,
	DXGI_FORMAT_R8G8_UINT = 50,
	DXGI_FORMAT_R8G8_SNORM = 51,
	DXGI_FORMAT_R8G8_SINT = 52,
	DXGI_FORMAT_R16_TYPELESS = 53,
	DXGI_FORMAT_R16_FLOAT = 54,
	DXGI_FORMAT_D16_UNORM = 55,
	DXGI_FORMAT_R16_UNORM = 56,
	DXGI_FORMAT_R16_UINT = 57,
	DXGI_FORMAT_R16_SNORM = 58,
	DXGI_FORMAT_R16_SINT = 59,
	DXGI_FORMAT_R8_TYPELESS = 60,
	DXGI_FORMAT_R8_UNORM = 61,
	DXGI_FORMAT_R8_UINT = 62,
	DXGI_FORMAT_R8_SNORM = 63,
	DXGI_FORMAT_R8_SINT = 64,
	DXGI_FORMAT_A8_UNORM = 65,
	DXGI_FORMAT_R1_UNORM = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
	DXGI_FORMAT_BC1_TYPELESS = 70,
	DXGI_FORMAT_BC1_UNORM = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB = 72,
	DXGI_FORMAT_BC2_TYPELESS = 73,
	DXGI_FORMAT_BC2_UNORM = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB = 75,
	DXGI_FORMAT_BC3_TYPELESS = 76,
	DXGI_FORMAT_BC3_UNORM = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB = 78,
	DXGI_FORMAT_BC4_TYPELESS = 79,
	DXGI_FORMAT_BC4_UNORM = 80,
	DXGI_FORMAT_BC4_SNORM = 81,
	DXGI_FORMAT_BC5_TYPELESS = 82,
	DXGI_FORMAT_BC5_UNORM = 83,
	DXGI_FORMAT_BC5_SNORM = 84,
	DXGI_FORMAT_B5G6R5_UNORM = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	DXGI_FORMAT_BC6H_TYPELESS = 94,
	DXGI_FORMAT_BC6H_UF16 = 95,
	DXGI_FORMAT_BC6H_SF16 = 96,
	DXGI_FORMAT_BC7_TYPELESS = 97,
	DXGI_FORMAT_BC7_UNORM = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB = 99,
	DXGI_FORMAT_AYUV = 100,
	DXGI_FORMAT_Y410 = 101,
	DXGI_FORMAT_Y416 = 102,
	DXGI_FORMAT_NV12 = 103,
	DXGI_FORMAT_P010 = 104,
	DXGI_FORMAT_P016 = 105,
	DXGI_FORMAT_420_OPAQUE = 106,
	DXGI_FORMAT_YUY2 = 107,
	DXGI_FORMAT_Y210 = 108,
	DXGI_FORMAT_Y216 = 109,
	DXGI_FORMAT_NV11 = 110,
	DXGI_FORMAT_AI44 = 111,
	DXGI_FORMAT_IA44 = 112,
	DXGI_FORMAT_P8 = 113,
	DXGI_FORMAT_A8P8 = 114,
	DXGI_FORMAT_B4G4R4A4_UNORM = 115,
	DXGI_FORMAT_P208 = 130,
	DXGI_FORMAT_V208 = 131,
	DXGI_FORMAT_V408 = 132,
	DXGI_FORMAT_FORCE_UINT = 0xffffffff
} DXGI_FORMAT;

// From https://msdn.microsoft.com/en-us/library/windows/desktop/bb172411(v=vs.85).aspx
typedef enum D3D10_RESOURCE_DIMENSION
{
	D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
	D3D10_RESOURCE_DIMENSION_BUFFER = 1,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
	D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4
} D3D10_RESOURCE_DIMENSION;

#define CUBEMAP_SIDES 6

#ifdef __cplusplus
extern "C" {
#endif

// Internal functions
ILboolean	iLoadDdsInternal(void);
ILboolean	iIsValidDds(void);
ILboolean	iCheckDds(DDSHEAD *Head);
void		AdjustVolumeTexture(DDSHEAD *Head, ILuint CompFormat, ILboolean IsDXT10);
ILboolean	ReadData(ILuint CompFormat, ILboolean IsDXT10);
ILboolean	AllocImage(ILuint CompFormat, ILboolean IsDXT10);
ILboolean	DdsDecompress(ILuint CompFormat, ILboolean IsDXT10);
ILboolean	ReadMipmaps(ILuint CompFormat, ILboolean IsDXT10);
ILuint		DecodePixelFormat(ILuint *CompFormat);
ILuint		GetDX10BlockSize();
void		DxtcReadColor(ILushort Data, Color8888* Out);
void		DxtcReadColors(const ILubyte* Data, Color8888* Out);
ILboolean	DecompressARGB(ILuint CompFormat);
ILboolean	DecompressARGB16(ILuint CompFormat);
ILboolean	DecompressARGBDX10(ILuint CompFormat);
ILboolean	DecompressDXT1(ILimage *lImage, ILubyte *lCompData);
ILboolean	DecompressDXT2(ILimage *lImage, ILubyte *lCompData);
ILboolean	DecompressDXT3(ILimage *lImage, ILubyte *lCompData);
ILboolean	DecompressDXT4(ILimage *lImage, ILubyte *lCompData);
ILboolean	DecompressDXT5(ILimage *lImage, ILubyte *lCompData);
ILboolean	DecompressBC7UNORM(ILimage* lImage, ILubyte* lCompData);
ILboolean	Decompress3Dc();
ILboolean	DecompressAti1n();
ILboolean	DecompressRXGB();
ILboolean	iConvFloat16ToFloat32(ILuint* dest, ILushort* src, ILuint size);
ILboolean	DecompressFloat(ILuint lCompFormat);
void		CorrectPreMult();
void		GetBitsFromMask(ILuint Mask, ILuint *ShiftLeft, ILuint *ShiftRight);
ILboolean	iSaveDdsInternal(void);
ILboolean	WriteHeader(ILimage *Image, ILenum DXTCFormat, ILuint CubeFlags);
ILushort	*CompressTo565(ILimage *Image);
ILubyte		*CompressTo88(ILimage *Image);
ILuint		Compress(ILimage *Image, ILenum DXTCFormat);
ILboolean	GetBlock(ILushort *Block, ILushort *Data, ILimage *Image, ILuint XPos, ILuint YPos);
ILboolean	GetAlphaBlock(ILubyte *Block, ILubyte *Data, ILimage *Image, ILuint XPos, ILuint YPos);
ILboolean	Get3DcBlock(ILubyte *Block, ILubyte *Data, ILimage *Image, ILuint XPos, ILuint YPos, int channel);
void		ShortToColor565(ILushort Pixel, Color565 *Colour);
void		ShortToColor888(ILushort Pixel, Color888 *Colour);
ILushort	Color565ToShort(Color565 *Colour);
ILushort	Color888ToShort(Color888 *Colour);
ILuint		GenBitMask(ILushort ex0, ILushort ex1, ILuint NumCols, ILushort *In, ILubyte *Alpha, Color888 *OutCol);
void		GenAlphaBitMask(ILubyte a0, ILubyte a1, ILubyte *In, ILubyte *Mask, ILubyte *Out);
ILuint		RMSAlpha(ILubyte *Orig, ILubyte *Test);
ILuint		Distance(Color888 *c1, Color888 *c2);
void		ChooseEndpoints(ILushort *Block, ILushort *ex0, ILushort *ex1);
void		ChooseAlphaEndpoints(ILubyte *Block, ILubyte *a0, ILubyte *a1);
void		CorrectEndDXT1(ILushort *ex0, ILushort *ex1, ILboolean HasAlpha);
void		PreMult(ILushort *Data, ILubyte *Alpha);


extern ILuint CubemapDirections[CUBEMAP_SIDES];

#ifdef __cplusplus
}
#endif

#endif//DDS_H
