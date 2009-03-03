//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/02/2009
//
// Filename: src-IL/include/il_utx.h
//
// Description: Reads from an Unreal and Unreal Tournament Texture (.utx) file.
//				Specifications can be found at
//				http://wiki.beyondunreal.com/Legacy:Package_File_Format.
//
//-----------------------------------------------------------------------------


#ifndef UTX_H
#define UTX_H

#include "il_internal.h"

typedef struct UTXHEADER
{
	ILuint		Signature;
	ILushort	Version;
	ILushort	LicenseMode;
	ILuint		Flags;
	ILuint		NameCount;
	ILuint		NameOffset;
	ILuint		ExportCount;
	ILuint		ExportOffset;
	ILuint		ImportCount;
	ILuint		ImportOffset;
} UTXHEADER;

typedef struct UTXENTRYNAME
{
	char	*Name;
	ILuint	Flags;
} UTXENTRYNAME;

typedef struct UTXEXPORTTABLE
{
	ILint	Class;
	ILint	Super;
	ILint	Group;
	ILint	ObjectName;
	ILuint	ObjectFlags;
	ILint	SerialSize;
	ILint	SerialOffset;

	ILboolean	ClassImported;
	ILboolean	SuperImported;
	ILboolean	GroupImported;
} UTXEXPORTTABLE;

typedef struct UTXIMPORTTABLE
{
	ILint		ClassPackage;
	ILint		ClassName;
	ILint		Package;
	ILint		ObjectName;

	ILboolean	PackageImported;
} UTXIMPORTTABLE;

typedef struct UTXPALETTE
{
	//char	*Name;
	ILubyte	*Pal;
	ILuint	Count;
	ILuint	Name;
} UTXPALETTE;

// Data formats
#define UTX_P8		0x00
#define UTX_DXT1	0x03

ILboolean iLoadUtxInternal(void);

#endif//UTX_H
