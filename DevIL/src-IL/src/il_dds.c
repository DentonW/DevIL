//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 12/03/2001 <--Y2K Compliant! =]
//
// Filename: openil/dds.c
//
// Description: Reads from a DirectDraw Surface (.dds) file.
//
//-----------------------------------------------------------------------------


//
//
// Note:  Almost all this code is from nVidia's DDS-loading example at
//	http://www.nvidia.com/view.asp?IO=dxtc_decompression_code
//
//


#include "il_internal.h"
#ifndef IL_NO_DDS
#include "il_dds.h"


//! Checks if the file specified in FileName is a valid .bmp file.
ILboolean ilIsValidDds(const ILstring FileName)
{
	ILHANDLE	DdsFile;
	ILboolean	bDds = IL_FALSE;

	if (!iCheckExtension(FileName, TEXT("dds"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bDds;
	}

	DdsFile = iopenr(FileName);
	if (DdsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDds;
	}

	bDds = ilIsValidDdsF(DdsFile);
	icloser(DdsFile);

	return bDds;
}


//! Checks if the ILHANDLE contains a valid .bmp file at the current position.
ILboolean ilIsValidDdsF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidDds();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid .bmp lump.
ILboolean ilIsValidDdsL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidDds();
}


// Internal function used to get the .bmp header from the current file.
ILvoid iGetDdsHead(DDSHEAD *Header)
{
	iread(Header->Signature, 1, 4);
	Header->Size1 = GetLittleInt();
	Header->Flags1 = GetLittleInt();
	Header->Height = GetLittleInt();
	Header->Width = GetLittleInt();
	Header->LinearSize = GetLittleInt();
	Header->Depth = GetLittleInt();
	Header->MipMapCount = GetLittleInt();
	Header->AlphaBitDepth = GetLittleInt();
	iread(Header->NotUsed, sizeof(ILint), 10);
	Header->Size2 = GetLittleInt();
	Header->Flags2 = GetLittleInt();
	Header->FourCC = GetLittleInt();
	Header->RGBBitCount = GetLittleInt();
	Header->RBitMask = GetLittleInt();
	Header->GBitMask = GetLittleInt();
	Header->BBitMask = GetLittleInt();
	Header->RGBAlphaBitMask = GetLittleInt();
	Header->ddsCaps1 = GetLittleInt();
	Header->ddsCaps2 = GetLittleInt();
	Header->ddsCaps3 = GetLittleInt();
	Header->ddsCaps4 = GetLittleInt();
	Header->TextureStage = GetLittleInt();

	return;
}


// Internal function to get the header and check it.
ILboolean iIsValidDds()
{
	ILboolean	IsValid;
	DDSHEAD		Head;

	iGetDdsHead(&Head);
	iseek(-(ILint)sizeof(DDSHEAD), IL_SEEK_CUR);  // Go ahead and restore to previous state

	IsValid = iCheckDds(&Head);

	return IsValid;
}


// Internal function used to check if the HEADER is a valid .bmp header.
ILboolean iCheckDds(DDSHEAD *Head)
{
	if (strnicmp(Head->Signature, "DDS ", 4))
		return IL_FALSE;
	if (Head->Size1 != 124)
		return IL_FALSE;
	if (Head->Size2 != 32)
		return IL_FALSE;
	if (Head->Width == 0 || Head->Height == 0)
		return IL_FALSE;
	return IL_TRUE;
}


//! Reads a .bmp file
ILboolean ilLoadDds(const ILstring FileName)
{
	ILHANDLE	DdsFile;
	ILboolean	bDds = IL_FALSE;

	DdsFile = iopenr(FileName);
	if (DdsFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDds;
	}

	bDds = ilLoadDdsF(DdsFile);
	icloser(DdsFile);

	return bDds;
}


//! Reads an already-opened .bmp file
ILboolean ilLoadDdsF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadDdsInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .bmp
ILboolean ilLoadDdsL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadDdsInternal();
}


ILboolean iLoadDdsInternal()
{
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iGetDdsHead(&Head);

	if (!ilTexImage(Head.Width, Head.Height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL)) {
		free(CompData);
		return IL_FALSE;
	}
	DecompData = iCurImage->Data;
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	ReadData();
	DecodePixelFormat();
	Decompress();

	free(CompData);

	ilFixImage();

	return IL_TRUE;
}


// Reads the compressed data
ILboolean ReadData()
{
	ILuint	Bps;
	ILint	i;
	ILubyte	*Temp;

	if (Head.Flags1 & DDS_LINEARSIZE) {
		CompData = (ILubyte*)calloc(Head.LinearSize, 1);
		if (CompData == NULL) {
			ilSetError(IL_OUT_OF_MEMORY);
			return IL_FALSE;
		}

		iread(CompData, 1, Head.LinearSize);
    }
    else {
		Bps = Head.Width * Head.RGBBitCount / 8;
		CompSize = Head.LinearSize * Head.Height;
		CompLineSize = Bps;

		CompData = (ILubyte*)calloc(Head.LinearSize * Head.Height, 1);
		if (CompData == NULL) {
			ilSetError(IL_OUT_OF_MEMORY);
			return IL_FALSE;
		}

        Temp = CompData;
        for (i = 0; i < Head.Height; i++) {
            iread(Temp, 1, Bps);
            Temp += Head.LinearSize;
        }
    }

	return IL_TRUE;
}


ILvoid DecodePixelFormat()
{
    switch (Head.FourCC)
    {
        case 0:
            // This dds texture isn't compressed so write out ARGB format
			CompFormat = PF_ARGB;
            break;

        case MAKEFOURCC('D','X','T','1'):
			CompFormat = PF_DXT1;
            break;

        case MAKEFOURCC('D','X','T','2'):
			CompFormat = PF_DXT2;
            break;

        case MAKEFOURCC('D','X','T','3'):
			CompFormat = PF_DXT3;
            break;

        case MAKEFOURCC('D','X','T','4'):
			CompFormat = PF_DXT4;
            break;

        case MAKEFOURCC('D','X','T','5'):
			CompFormat = PF_DXT5;
            break;
		default:
			CompFormat = PF_UNKNOWN;
			break;
    }
	return;
}


ILboolean Decompress()
{
	switch (CompFormat)
	{
		case PF_DXT1 :
			return DecompressDXT1();

		case PF_DXT2 :
			return DecompressDXT2();

		case PF_DXT3 :
			return DecompressDXT3();	

		case PF_DXT4 :
			return DecompressDXT4();

		case PF_DXT5 :
			return DecompressDXT5();		

		case PF_UNKNOWN :
			return IL_FALSE;

	}

	return IL_FALSE;
}


ILvoid GetColorBlockColors(DXTColBlock * pBlock, Color8888 *col_0, Color8888 * col_1, 
									Color8888 *col_2, Color8888 * col_3, ILshort *werd)
{
	// There are 4 methods to use - see the Time_ functions.
	// 1st = shift = does normal approach per byte for color comps
	// 2nd = use freak variable bit field color565 for component extraction
	// 3rd = use super-freak ILint adds BEFORE shifting the color components
	//  This lets you do only 1 add per color instead of 3 ILbyte adds and
	//  might be faster
	// Call RunTimingSession() to run each of them & output result to txt file

 
	// freak variable bit structure method
	// normal math
	// This method is fastest

	Color565 * pCol;
	ILshort wrd;

	pCol = (Color565*) & (pBlock->col0 );

	col_0->a = 0xff;
	col_0->r = pCol->nRed;
	col_0->r <<= 3;				// shift to full precision
	col_0->g = pCol->nGreen;
	col_0->g <<= 2;
	col_0->b = pCol->nBlue;
	col_0->b <<= 3;

	pCol = (Color565*) & (pBlock->col1 );
	col_1->a = 0xff;
	col_1->r = pCol->nRed;
	col_1->r <<= 3;				// shift to full precision
	col_1->g = pCol->nGreen;
	col_1->g <<= 2;
	col_1->b = pCol->nBlue;
	col_1->b <<= 3;


	if (pBlock->col0 > pBlock->col1) {
		// Four-color block: derive the other two colors.
		// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
		// These two bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block.

		*werd = wrd = ((ILshort)col_0->r * 2 + (ILshort)col_1->r )/3;
											// no +1 for rounding
											// as bits have been shifted to 888
		col_2->r = (ILbyte)wrd;

		wrd = ((ILshort)col_0->g * 2 + (ILshort)col_1->g )/3;
		col_2->g = (ILbyte)wrd;

		wrd = ((ILshort)col_0->b * 2 + (ILshort)col_1->b )/3;
		col_2->b = (ILbyte)wrd;
		col_2->a = 0xff;

		wrd = ((ILshort)col_0->r + (ILshort)col_1->r *2 )/3;
		col_3->r = (ILbyte)wrd;

		wrd = ((ILshort)col_0->g + (ILshort)col_1->g *2 )/3;
		col_3->g = (ILbyte)wrd;

		wrd = ((ILshort)col_0->b + (ILshort)col_1->b *2 )/3;
		col_3->b = (ILbyte)wrd;
		col_3->a = 0xff;

	}
	else
	{
		// Three-color block: derive the other color.
		// 00 = color_0,  01 = color_1,  10 = color_2,
		// 11 = transparent.
		// These two bit codes correspond to the 2-bit fields
		// stored in the 64-bit block.

		// explicit for each component, unlike some refrasts...
		
		// TRACE("block has alpha\n");

		wrd = ((ILshort)col_0->r + (ILshort)col_1->r )/2;
		col_2->r = (ILbyte)wrd;
		wrd = ((ILshort)col_0->g + (ILshort)col_1->g )/2;
		col_2->g = (ILbyte)wrd;
		wrd = ((ILshort)col_0->b + (ILshort)col_1->b )/2;
		col_2->b = (ILbyte)wrd;
		col_2->a = 0xff;

		/*col_3->r = 0x00;		// random color to indicate alpha
		col_3->g = 0xff;
		col_3->b = 0xff;
		col_3->a = 0x00;*/

		wrd = ((ILshort)col_0->r + (ILshort)col_1->r *2 )/3;
		col_3->r = (ILbyte)wrd;
		wrd = ((ILshort)col_0->g + (ILshort)col_1->g *2 )/3;
		col_3->g = (ILbyte)wrd;
		wrd = ((ILshort)col_0->b + (ILshort)col_1->b *2 )/3;
		col_3->b = (ILbyte)wrd;
		col_3->a = 0x00;

	}
}			//  Get color block colors (...)


void DecodeColorBlock( ILint * pImPos, DXTColBlock * pColorBlock, int width,
								ILint * col_0,
								ILint * col_1, ILint * col_2, ILint * col_3 )
{
	// width is width of image in pixels


	ILint bits;
	int r,n;

	// bit masks = 00000011, 00001100, 00110000, 11000000
	const ILint masks[] = { 3, 12, 3 << 4, 3 << 6 };
	const int   shift[] = { 0, 2, 4, 6 };

	// r steps through lines in y
	for( r=0; r < 4; r++, pImPos += width-4 )	// no width*4 as ILint ptr inc will *4
	{

		// width * 4 bytes per pixel per line
		// each j dxtc row is 4 lines of pixels

		// pImPos = (ILint*)((ILint)pBase + i*16 + (r+j*4) * m_nWidth * 4 );

		// n steps through pixels
		for( n=0; n < 4; n++ )
		{
			bits =		pColorBlock->row[r] & masks[n];
			bits >>=	shift[n];

			switch( bits )
			{
			case 0 :
				*pImPos = *col_0;
				pImPos++;		// increment to next ILint
				break;
			case 1 :
				*pImPos = *col_1;
				pImPos++;
				break;
			case 2 :
				*pImPos = *col_2;
				pImPos++;
				break;
			case 3 :
				*pImPos = *col_3;
				pImPos++;
				break;
			default:
				//TRACE("Your logic is jacked! bits == 0x%x\n", bits );
				pImPos++;
				break;
			}
		}
	}
}


ILvoid DecodeAlphaExplicit( ILint * pImPos, DXTAlphaBlockExplicit * pAlphaBlock,
								  int width, ILint alphazero )
{
	// alphazero is a bit mask that when & with the image color
	//  will zero the alpha bits, so if the image ILints  are
	//  ARGB then alphazero will be 0x00ffffff or if
	//  RGBA then alphazero will be 0xffffff00
	//  alphazero constructed automaticaly from field order of Color8888 structure

	// decodes to 32 bit format only


	int row, pix;

	ILshort wrd;

	Color8888 col;
	col.r = col.g = col.b = 0;


	//TRACE("\n");

	for( row=0; row < 4; row++, pImPos += width-4 )
	{
		// pImPow += pImPos += width-4 moves to next row down

		wrd = pAlphaBlock->row[ row ];

		// TRACE("0x%.8x\t\t", wrd);

		for( pix = 0; pix < 4; pix++ )
		{
			// zero the alpha bits of image pixel
			*pImPos &= alphazero;

			col.a = wrd & 0x000f;		// get only low 4 bits
//			col.a <<= 4;				// shift to full byte precision
										// NOTE:  with just a << 4 you'll never have alpha
										// of 0xff,  0xf0 is max so pure shift doesn't quite
										// cover full alpha range.
										// It's much cheaper than divide & scale though.
										// To correct for this, and get 0xff for max alpha,
										//  or the low bits back in after left shifting
			col.a = col.a | (col.a << 4 );	// This allows max 4 bit alpha to be 0xff alpha
											//  in final image, and is crude approach to full 
											//  range scale

			*pImPos |= *((ILint*)&col);	// or the bits into the prev. nulled alpha

			wrd >>= 4;		// move next bits to lowest 4

			pImPos++;		// move to next pixel in the row

		}
	}
}



ILbyte		gBits[4][4];
ILshort		gAlphas[8];
Color8888	gACol[4][4];


ILvoid DecodeAlpha3BitLinear( ILint * pImPos, DXTAlphaBlock3BitLinear * pAlphaBlock,
									int width, ILint alphazero)
{
	const ILint mask = 0x00000007;		// bits = 00 00 01 11
	ILint bits;
	int row, pix;

	gAlphas[0] = pAlphaBlock->alpha0;
	gAlphas[1] = pAlphaBlock->alpha1;

	
	// 8-alpha or 6-alpha block?    

	if( gAlphas[0] > gAlphas[1] )
	{
		// 8-alpha block:  derive the other 6 alphas.    
		// 000 = alpha_0, 001 = alpha_1, others are interpolated

		gAlphas[2] = ( 6 * gAlphas[0] +     gAlphas[1]) / 7;	// bit code 010
		gAlphas[3] = ( 5 * gAlphas[0] + 2 * gAlphas[1]) / 7;	// Bit code 011    
		gAlphas[4] = ( 4 * gAlphas[0] + 3 * gAlphas[1]) / 7;	// Bit code 100    
		gAlphas[5] = ( 3 * gAlphas[0] + 4 * gAlphas[1]) / 7;	// Bit code 101
		gAlphas[6] = ( 2 * gAlphas[0] + 5 * gAlphas[1]) / 7;	// Bit code 110    
		gAlphas[7] = (     gAlphas[0] + 6 * gAlphas[1]) / 7;	// Bit code 111
	}    
	else
	{
		// 6-alpha block:  derive the other alphas.    
		// 000 = alpha_0, 001 = alpha_1, others are interpolated

		gAlphas[2] = (4 * gAlphas[0] +     gAlphas[1]) / 5;	// Bit code 010
		gAlphas[3] = (3 * gAlphas[0] + 2 * gAlphas[1]) / 5;	// Bit code 011    
		gAlphas[4] = (2 * gAlphas[0] + 3 * gAlphas[1]) / 5;	// Bit code 100    
		gAlphas[5] = (    gAlphas[0] + 4 * gAlphas[1]) / 5;	// Bit code 101
		gAlphas[6] = 0;										// Bit code 110
		gAlphas[7] = 255;									// Bit code 111
	}


	// Decode 3-bit fields into array of 16 ILbyteS with same value

	// first two rows of 4 pixels each:
	// pRows = (Alpha3BitRows*) & ( pAlphaBlock->stuff[0] );

	bits = *( (ILint*) & ( pAlphaBlock->stuff[0] ));

	gBits[0][0] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[0][1] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[0][2] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[0][3] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[1][0] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[1][1] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[1][2] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[1][3] = (ILbyte)( bits & mask );

	// now for last two rows:

	bits = *( (ILint*) & ( pAlphaBlock->stuff[3] ));		// last 3 bytes

	gBits[2][0] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[2][1] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[2][2] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[2][3] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[3][0] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[3][1] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[3][2] = (ILbyte)( bits & mask );
	bits >>= 3;
	gBits[3][3] = (ILbyte)( bits & mask );


	// decode the codes into alpha values
	for( row = 0; row < 4; row++ )
	{
		for( pix=0; pix < 4; pix++ )
		{
			gACol[row][pix].a = (ILbyte) gAlphas[ gBits[row][pix] ];

			/*ASSERT( gACol[row][pix].r == 0 );
			ASSERT( gACol[row][pix].g == 0 );
			ASSERT( gACol[row][pix].b == 0 );*/
		}
	}



	// Write out alpha values to the image bits

	for( row=0; row < 4; row++, pImPos += width-4 )
	{
		// pImPow += pImPos += width-4 moves to next row down

		for( pix = 0; pix < 4; pix++ )
		{
			// zero the alpha bits of image pixel
			*pImPos &=  alphazero;

			*pImPos |=  *((ILint*) &(gACol[row][pix]));	// or the bits into the prev. nulled alpha
			pImPos++;
		}
	}
}


ILboolean DecompressDXT1()
{
	// This was hacked up pretty quick & slopily
	// decompresses to 32 bit format 0xARGB

	ILint	xblocks, yblocks;
	ILint	i, j;
	ILint	*pBase;
	ILint	*pImPos;	// pos in decompressed data
	ILshort	*pPos;		// pos in compressed data
	DXTColBlock *pBlock;
	Color8888 col_0, col_1, col_2, col_3;
	ILshort wrd;

/*#ifdef DEBUG
	if( (ddsd.dwWidth % 4 ) != 0 )
	{
		TRACE("****** warning width not div by 4!  %d\n", ddsd.dwWidth );
	}
	if( (ddsd.dwHeight %4 ) != 0 )
	{
		TRACE("****** warning Height not div by 4! %d\n", ddsd.dwHeight );
	}
	TRACE("end check\n");
#endif */

	xblocks = Head.Width / 4;
	yblocks = Head.Height / 4;

	

	pBase  = (ILint*)DecompData;
	pImPos = (ILint*)pBase;			// pos in decompressed data
	pPos   = (ILshort*)CompData;	// pos in compressed data



	/*
	// yes, ptr++ does work!  duh! =) 
	ILint * pptr = 0;
	for( j=0; j < 10; j++ )
	{

		TRACE(" %x\n", pptr );
		pptr++;
	}
	*/



	//	TRACE("blocks: x: %d    y: %d\n", xblocks, yblocks );



	for( j=0; j < yblocks; j++ )
	{
		// 8 bytes per block
		pBlock = (DXTColBlock*) ( (ILint)CompData + j * xblocks * 8 );


		for( i=0; i < xblocks; i++, pBlock++ )
		{

			// inline func:
			GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3, &wrd );


			// now decode the color block into the bitmap bits
			// inline func:

			pImPos = (ILint*)((ILint)pBase + i*16 + (j*4) * Head.Width * 4 );


			DecodeColorBlock( pImPos, pBlock, Head.Width, (ILint*)&col_0, (ILint*)&col_1,
								(ILint*)&col_2, (ILint*)&col_3 );


			// Set to RGB test pattern
			//	pImPos = (ILint*)((ILint)pBase + i*4 + j*m_nWidth*4);
			//	*pImPos = ((i*4) << 16) | ((j*4) << 8 ) | ( (63-i)*4 );

			// checkerboard of only col_0 and col_1 basis colors:
			//	pImPos = (ILint*)((ILint)pBase + i*8 + j*m_nWidth*8);
			//	*pImPos = *((ILint*)&col_0);
			//	pImPos += 1 + m_nWidth;
			//	*pImPos = *((ILint*)&col_1);

		}
	}

	return IL_TRUE;
}


ILboolean DecompressDXT2()
{
	// Can do color & alpha same as dxt3, but color is pre-multiplied 
	//   so the result will be wrong unless corrected. 
	// DecompressDXT3();

	return IL_FALSE;
}


ILboolean DecompressDXT3()
{
	int xblocks, yblocks;
	int i, j;
	ILint * pBase;
	ILint * pImPos;	// pos in decompressed data
	ILshort  * pPos;	// pos in compressed data
	DXTColBlock				* pBlock;
	DXTAlphaBlockExplicit	* pAlphaBlock;
	Color8888 col_0, col_1, col_2, col_3;
	ILshort wrd;
	ILint alphazero;

	xblocks = Head.Width / 4;
	yblocks = Head.Height / 4;

	pBase  = (ILint*)  DecompData;
	pImPos = (ILint*)  pBase;		// pos in decompressed data
	pPos   = (ILshort*)   CompData;	// pos in compressed data


	// fill alphazero with appropriate value to zero out alpha when
	//  alphazero is ANDed with the image color 32 bit ILint:
	col_0.a = 0;
	col_0.r = col_0.g = col_0.b = 0xff;
	alphazero = *((ILint*) &col_0);


	//	TRACE("blocks: x: %d    y: %d\n", xblocks, yblocks );

	for (j=0; j < yblocks; j++) {
		// 8 bytes per block
		// 1 block for alpha, 1 block for color

		pBlock = (DXTColBlock*) ( (ILint)CompData + j * xblocks * 16 );

		for( i=0; i < xblocks; i++, pBlock ++ )
		{

			// inline
			// Get alpha block

			pAlphaBlock = (DXTAlphaBlockExplicit*) pBlock;

			// inline func:
			// Get color block & colors
			pBlock++;
			GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3, &wrd );

			// Decode the color block into the bitmap bits
			// inline func:

			pImPos = (ILint*)((ILint)pBase + i*16 + (j*4) * Head.Width * 4 );


			DecodeColorBlock( pImPos, pBlock, Head.Width, (ILint*)&col_0, (ILint*)&col_1,
								(ILint*)&col_2, (ILint*)&col_3 );

			// Overwrite the previous alpha bits with the alpha block
			//  info
			// inline func:
			DecodeAlphaExplicit( pImPos, pAlphaBlock, Head.Width, alphazero );


		}
	}

	return IL_TRUE;
}


ILboolean DecompressDXT4()
{
	// Can do color & alpha same as dxt5, but color is pre-multiplied 
	//   so the result will be wrong unless corrected. 
	// DecompressDXT5();

	return IL_FALSE;
}


ILboolean DecompressDXT5()
{
	ILint	xblocks, yblocks;
	ILint	*pBase;
	ILint	*pImPos;	// pos in decompressed data
	ILshort  *pPos;	// pos in compressed data
	ILint i,j;
	DXTColBlock				*pBlock;
	DXTAlphaBlock3BitLinear *pAlphaBlock;
	Color8888 col_0, col_1, col_2, col_3;
	ILshort wrd;
	ILint alphazero;

/*#ifdef DEBUG
	if( (ddsd.dwWidth % 4 ) != 0 )
	{
		TRACE("****** warning width not div by 4!  %d\n", ddsd.dwWidth );
	}
	if( (ddsd.dwHeight %4 ) != 0 )
	{
		TRACE("****** warning Height not div by 4! %d\n", ddsd.dwHeight );
	}
	TRACE("end check\n");
#endif*/

	xblocks = Head.Width / 4;
	yblocks = Head.Height / 4;
	

	pBase  = (ILint*)DecompData;
	pImPos = (ILint*)pBase;			// pos in decompressed data
	pPos   = (ILshort*)CompData;	// pos in compressed data


	// fill alphazero with appropriate value to zero out alpha when
	//  alphazero is ANDed with the image color 32 bit ILint:
	col_0.a = 0;
	col_0.r = col_0.g = col_0.b = 0xff;
	alphazero = *((ILint*) &col_0);

	////////////////////////////////
	//	TRACE("blocks: x: %d    y: %d\n", xblocks, yblocks );

	for( j=0; j < yblocks; j++ )
	{
		// 8 bytes per block
		// 1 block for alpha, 1 block for color

		pBlock = (DXTColBlock*) ( (ILint)CompData + j * xblocks * 16 );

		for( i=0; i < xblocks; i++, pBlock ++ )
		{

			// inline
			// Get alpha block

			pAlphaBlock = (DXTAlphaBlock3BitLinear*)pBlock;

			// inline func:
			// Get color block & colors
			pBlock++;

			// TRACE("pBlock:   0x%.8x\n", pBlock );

			GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3, &wrd);

			// Decode the color block into the bitmap bits
			// inline func:

			pImPos = (ILint*)((ILint)pBase + i*16 + (j*4) * Head.Width * 4 );


			DecodeColorBlock( pImPos, pBlock, Head.Width, (ILint*)&col_0, (ILint*)&col_1,
								(ILint*)&col_2, (ILint*)&col_3 );

			// Overwrite the previous alpha bits with the alpha block
			//  info

			DecodeAlpha3BitLinear( pImPos, pAlphaBlock, Head.Width, alphazero );


		}
	}

	return IL_TRUE;
}


#endif//IL_NO_DDS
