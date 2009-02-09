//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 09/26/2008
//
// Filename: src-IL/src/il_wdp.c
//
// Description: Reads a microsoft hd photo (.wdp)
//   based very much on the Microsoft HD Photo Device Porting Kit 1.0
//   available at 
//   http://www.microsoft.com/downloads/details.aspx?FamilyID=285eeffd-d86c-48c3-ab93-3abd5ee7f1ce&displaylang=en.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_WDP
#include <WMPGlue.h>
#include "il_wdp.h"


//! Reads a .wdp file
ILboolean ilLoadWdp(ILconst_string FileName)
{
	ILboolean	bWdp = IL_FALSE;

	bWdp = iLoadWdpInternal(FileName);

	return bWdp;
}


ILboolean iLoadWdpInternal(ILconst_string FileName)
{
	PKFactory* pFactory = NULL;
	PKCodecFactory* pCodecFactory = NULL;
	PKImageDecode* pDecoder = NULL;
    PKPixelInfo PI;
	PKPixelFormatGUID guidPixFormat;
	PKFormatConverter* pConverter = NULL;
    U32 cFrame = 0, i = 0;
	PKRect Rect;
	U8 *Data;


	//Call(PKCreateFactory(&pFactory, PK_SDK_VERSION));
	//Call(PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION));
	//Call(pCodecFactory->CreateDecoderFromFile(FileName, &pDecoder));
	PKCreateFactory(&pFactory, PK_SDK_VERSION);
	PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION);
	pCodecFactory->CreateDecoderFromFile(FileName, &pDecoder);

	guidPixFormat = GUID_PKPixelFormat24bppRGB;

    // == color transcoding,
    if(IsEqualGUID(&guidPixFormat, &GUID_PKPixelFormat8bppGray) || IsEqualGUID(&guidPixFormat, &GUID_PKPixelFormat16bppGray)){ // ** => Y transcoding
        pDecoder->guidPixFormat = guidPixFormat;
        pDecoder->WMP.wmiI.cfColorFormat = Y_ONLY;
    }
	else if(IsEqualGUID(&guidPixFormat, &GUID_PKPixelFormat24bppRGB) && pDecoder->WMP.wmiI.cfColorFormat == CMYK){ // CMYK = > RGB
		pDecoder->WMP.wmiI.cfColorFormat = CF_RGB;
		pDecoder->guidPixFormat = guidPixFormat;
		pDecoder->WMP.wmiI.bRGB = 1; //RGB
	}

	PI.pGUIDPixFmt = &guidPixFormat;
    PixelFormatLookup(&PI, LOOKUP_FORWARD);

    pDecoder->WMP.wmiSCP.bfBitstreamFormat = 0;
    pDecoder->WMP.wmiSCP.uAlphaMode = 0;
    pDecoder->WMP.wmiSCP.sbSubband = SB_ALL;
    pDecoder->WMP.bIgnoreOverlap = FALSE;

    pDecoder->WMP.wmiI.cfColorFormat = PI.cfColorFormat;

    pDecoder->WMP.wmiI.bdBitDepth = PI.bdBitDepth;
    pDecoder->WMP.wmiI.cBitsPerUnit = PI.cbitUnit;

	//==== Validate thumbnail decode parameters =====
    pDecoder->WMP.wmiI.cThumbnailWidth = pDecoder->WMP.wmiI.cWidth;
    pDecoder->WMP.wmiI.cThumbnailHeight = pDecoder->WMP.wmiI.cHeight;
    pDecoder->WMP.wmiI.bSkipFlexbits = FALSE;

    pDecoder->GetFrameCount(pDecoder, &cFrame);

    //for (i = 0; ; ++i)
    //{

	// Right now, we are just assuming one frame.
	//pDecoder->GetRawStream(pDecoder, pDecoder->pStream);
	Data = ialloc(pDecoder->uWidth * pDecoder->uHeight * 3);
	Rect.Height = pDecoder->uHeight; Rect.Width = pDecoder->uWidth; Rect.X = 0; Rect.Y = 0;
	//for (i = 0; i < pDecoder->uHeight; i++)
	//{
	//	Rect.Y = i;
	//	pDecoder->Copy(pDecoder, &Rect, Data + pDecoder->uWidth * 3 * i, 0);
	//}
	pDecoder->Copy(pDecoder, &Rect, Data, pDecoder->uWidth);
	//pCodecFactory->CreateFormatConverter(&pConverter));

    //    // multi-frame support NYI
    //    if (i + 1 == cFrame)
    //    {
    //        break;
    //    }
    //    pDecoder->SelectFrame(pDecoder, i + 1);
    //}



	ilTexImage(pDecoder->uWidth, pDecoder->uHeight, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, Data);
	ifree(Data);

	pDecoder->Release(&pDecoder);

	return IL_TRUE;
}

#endif//IL_NO_WDP
