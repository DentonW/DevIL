//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 03/02/2002 <--Y2K Compliant! =]
//
// Filename: openil/il_psp.h
//
// Description: Reads a Paint Shop Pro file.
//
//-----------------------------------------------------------------------------


#ifndef PSP_H
#define PSP_H

#include "il_internal.h"

#define MAJOR_VERSION	3
#define MINOR_VERSION	0

// Block identifiers
enum PSPBlockID {
	PSP_IMAGE_BLOCK = 0,	// General Image Attributes Block (main)
	PSP_CREATOR_BLOCK,		// Creator Data Block (main)
	PSP_COLOR_BLOCK,		// Color Palette Block (main and sub)
	PSP_LAYER_START_BLOCK,	// Layer Bank Block (main)
	PSP_LAYER_BLOCK,		// Layer Block (sub)
	PSP_CHANNEL_BLOCK,		// Channel Block (sub)
	PSP_SELECTION_BLOCK,	// Selection Block (main)
	PSP_ALPHA_BANK_BLOCK,	// Alpha Bank Block (main)
	PSP_ALPHA_CHANNEL_BLOCK,// Alpha Channel Block (sub)
	PSP_THUMBNAIL_BLOCK,	// Thumbnail Block (main)
	PSP_EXTENDED_DATA_BLOCK,// Extended Data Block (main)
	PSP_TUBE_BLOCK,			// Picture Tube Data Block (main)
};



#endif//PSP_H