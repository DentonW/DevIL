//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/files.c
//
// Description: File handling for DevIL
//
//-----------------------------------------------------------------------------


#define __FILES_C
#include "il_internal.h"
#include <stdarg.h>


// All specific to the next set of functions
ILboolean	ILAPIENTRY iEofFile(ILvoid);
ILboolean	ILAPIENTRY iEofLump(ILvoid);
ILint		ILAPIENTRY iGetcFile(ILvoid);
ILint		ILAPIENTRY iGetcLump(ILvoid);
ILuint		ILAPIENTRY iReadFile(ILvoid *Buffer, ILuint Size, ILuint Number);
ILuint		ILAPIENTRY iReadLump(ILvoid *Buffer, ILuint Size, ILuint Number);
ILuint		ILAPIENTRY iSeekRFile(ILint Offset, ILuint Mode);
ILuint		ILAPIENTRY iSeekRLump(ILint Offset, ILuint Mode);
ILuint		ILAPIENTRY iSeekWFile(ILint Offset, ILuint Mode);
ILuint		ILAPIENTRY iSeekWLump(ILint Offset, ILuint Mode);
ILuint		ILAPIENTRY iTellRFile(ILvoid);
ILuint		ILAPIENTRY iTellRLump(ILvoid);
ILuint		ILAPIENTRY iTellWFile(ILvoid);
ILuint		ILAPIENTRY iTellWLump(ILvoid);
ILint		ILAPIENTRY iPutcFile(ILubyte Char);
ILint		ILAPIENTRY iPutcLump(ILubyte Char);
ILint		ILAPIENTRY iWriteFile(const ILvoid *Buffer, ILuint Size, ILuint Number);
ILint		ILAPIENTRY iWriteLump(const ILvoid *Buffer, ILuint Size, ILuint Number);
ILHANDLE	FileRead = NULL, FileWrite = NULL;
ILvoid		*ReadLump = NULL, *WriteLump = NULL;
ILuint		ReadLumpPos = 0, ReadLumpSize = 0, ReadFileStart = 0, WriteFileStart = 0;
ILuint		WriteLumpPos = 0, WriteLumpSize = 0;

fGetcProc	GetcProcCopy;
fReadProc	ReadProcCopy;
fSeekRProc	SeekProcCopy;
fTellRProc	TellProcCopy;
ILHANDLE	(ILAPIENTRY *iopenCopy)(const ILstring);
ILvoid		(ILAPIENTRY *icloseCopy)(ILHANDLE);

ILboolean	UseCache = IL_FALSE;
ILubyte		*Cache = NULL;
ILuint		CacheSize, CachePos;


// Just preserves the current read functions and replaces
//	the current read functions with the default read funcs.
ILvoid ILAPIENTRY iPreserveReadFuncs()
{
	// Create backups
	GetcProcCopy = GetcProc;
	ReadProcCopy = ReadProc;
	SeekProcCopy = SeekRProc;
	TellProcCopy = TellRProc;
	iopenCopy = iopenr;
	icloseCopy = icloser;

	// Set the standard procs to read
	ilResetRead();

	return;
}


// Restores the read functions - must be used after iPreserveReadFuncs().
ILvoid ILAPIENTRY iRestoreReadFuncs()
{
	GetcProc = GetcProcCopy;
	ReadProc = ReadProcCopy;
	SeekRProc = SeekProcCopy;
	TellRProc = TellProcCopy;
	iopenr = iopenCopy;
	icloser = icloseCopy;

	return;
}


// Next 7 functions are the default read functions

ILHANDLE ILAPIENTRY iDefaultOpenR(const ILstring FileName)
{
#ifndef _WIN32_WCE
	return (ILHANDLE)fopen(FileName, "rb");
#else
	return (ILHANDLE)_wfopen(FileName, L"rb");
#endif//_WIN32_WCE
}


ILvoid ILAPIENTRY iDefaultCloseR(ILHANDLE Handle)
{
	fclose((FILE*)Handle);
	return;
}


ILboolean ILAPIENTRY iDefaultEof(ILHANDLE Handle)
{
	static ILuint OrigPos, FileSize;

	// Find out the filesize for checking for the end of file
	OrigPos = itell();
	iseek(0, IL_SEEK_END);
	FileSize = itell();
	iseek(OrigPos, IL_SEEK_SET);

	if (itell() >= FileSize)
		return IL_TRUE;
	return IL_FALSE;
}


ILint ILAPIENTRY iDefaultGetc(ILHANDLE Handle)
{
	return fgetc((FILE*)Handle);
}


ILint ILAPIENTRY iDefaultRead(ILvoid *Buffer, ILuint Size, ILuint Number, ILHANDLE Handle)
{
	return fread(Buffer, Size, Number, (FILE*)Handle);
}


ILint ILAPIENTRY iDefaultRSeek(ILHANDLE Handle, ILint Offset, ILint Mode)
{
	return fseek((FILE*)Handle, Offset, Mode);
}


ILint ILAPIENTRY iDefaultWSeek(ILHANDLE Handle, ILint Offset, ILint Mode)
{
	return fseek((FILE*)Handle, Offset, Mode);
}


ILint ILAPIENTRY iDefaultRTell(ILHANDLE Handle)
{
	return ftell((FILE*)Handle);
}


ILint ILAPIENTRY iDefaultWTell(ILHANDLE Handle)
{
	return ftell((FILE*)Handle);
}


ILHANDLE ILAPIENTRY iDefaultOpenW(const ILstring FileName)
{
#ifndef _WIN32_WCE
	return (ILHANDLE)fopen(FileName, "wb");
#else
	return (ILHANDLE)_wfopen(FileName, L"wb");
#endif//_WIN32_WCE
}


ILvoid ILAPIENTRY iDefaultCloseW(ILHANDLE Handle)
{
	fclose((FILE*)Handle);
	return;
}


ILint ILAPIENTRY iDefaultPutc(ILubyte Char, ILHANDLE Handle)
{
	return fputc(Char, (FILE*)Handle);
}


ILint ILAPIENTRY iDefaultWrite(const ILvoid *Buffer, ILuint Size, ILuint Number, ILHANDLE Handle)
{
	return fwrite(Buffer, Size, Number, (FILE*)Handle);
}


ILvoid ILAPIENTRY ilResetRead()
{
	ilSetRead(iDefaultOpenR, iDefaultCloseR, iDefaultEof, iDefaultGetc, 
				iDefaultRead, iDefaultRSeek, iDefaultRTell);
	return;
}


ILvoid ILAPIENTRY ilResetWrite()
{
	ilSetWrite(iDefaultOpenW, iDefaultCloseW, iDefaultPutc,
				iDefaultWSeek, iDefaultWTell, iDefaultWrite);
	return;
}


//! Allows you to override the default file-reading functions.
ILvoid ILAPIENTRY ilSetRead(fOpenRProc Open, fCloseRProc Close, fEofProc Eof, fGetcProc Getc, fReadProc Read, fSeekRProc Seek, fTellRProc Tell)
{
	iopenr    = Open;
	icloser   = Close;
	EofProc   = Eof;
	GetcProc  = Getc;
	ReadProc  = Read;
	SeekRProc = Seek;
	TellRProc = Tell;

	return;
}


//! Allows you to override the default file-writing functions.
ILvoid ILAPIENTRY ilSetWrite(fOpenRProc Open, fCloseRProc Close, fPutcProc Putc, fSeekWProc Seek, fTellWProc Tell, fWriteProc Write)
{
	iopenw    = Open;
	iclosew   = Close;
	PutcProc  = Putc;
	WriteProc = Write;
	SeekWProc = Seek;
	TellWProc = Tell;

	return;
}


// Tells DevIL that we're reading from a file, not a lump
ILvoid iSetInputFile(ILHANDLE File)
{
	ieof  = iEofFile;
	igetc = iGetcFile;
	iread = iReadFile;
	iseek = iSeekRFile;
	itell = iTellRFile;
	FileRead = File;
	ReadFileStart = itell();
}


// Tells DevIL that we're reading from a lump, not a file
ILvoid iSetInputLump(ILvoid *Lump, ILuint Size)
{
	ieof  = iEofLump;
	igetc = iGetcLump;
	iread = iReadLump;
	iseek = iSeekRLump;
	itell = iTellRLump;
	ReadLump = Lump;
	ReadLumpPos = 0;
	ReadLumpSize = Size;
}


// Tells DevIL that we're writing to a file, not a lump
ILvoid iSetOutputFile(ILHANDLE File)
{
	// Helps with ilGetLumpPos().
	WriteLump = NULL;
	WriteLumpPos = 0;
	WriteLumpSize = 0;

	iputc  = iPutcFile;
	iseekw = iSeekWFile;
	itellw = iTellWFile;
	iwrite = iWriteFile;
	FileWrite = File;
}


// Tells DevIL that we're writing to a lump, not a file
ILvoid iSetOutputLump(ILvoid *Lump, ILuint Size)
{
	iputc  = iPutcLump;
	iseekw = iSeekWLump;
	itellw = iTellWLump;
	iwrite = iWriteLump;
	WriteLump = Lump;
	WriteLumpPos = 0;
	WriteLumpSize = Size;
}


ILuint ILAPIENTRY ilGetLumpPos()
{
	if (WriteLump)
		return WriteLumpPos;
	return 0;
}


ILuint ILAPIENTRY iprintf(const char *Line, ...)
{
	static char		Buffer[2048];  // Hope this is large enough
	static va_list	VaLine;
	static ILuint	i;

	va_start(VaLine, Line);
	vsprintf(Buffer, Line, VaLine);
	va_end(VaLine);

	i = strlen(Buffer);
	iwrite(Buffer, 1, i);

	return i;
}


// To pad zeros where needed...
ILvoid ipad(ILuint NumZeros)
{
	ILuint i = 0;
	for (; i < NumZeros; i++)
		iputc(0);
	return;
}


//
// The rest of the functions following in this file are quite
//	self-explanatory, except where commented.
//

// Next 12 functions are the default write functions

ILboolean ILAPIENTRY iEofFile(ILvoid)
{
	return EofProc((FILE*)FileRead);
}


ILboolean ILAPIENTRY iEofLump(ILvoid)
{
	if (ReadLumpSize)
		return (ReadLumpPos >= ReadLumpSize);
	return IL_FALSE;
}


ILint ILAPIENTRY iGetcFile(ILvoid)
{
	if (!UseCache) {
		return GetcProc(FileRead);
	}
	if (CachePos >= CacheSize) {
		iPreCache(CacheSize);
	}

	return Cache[CachePos++];
}


ILint ILAPIENTRY iGetcLump(ILvoid)
{
	// If ReadLumpSize is 0, don't even check to see if we've gone past the bounds.
	if (ReadLumpSize > 0) {
		if (ReadLumpPos + 1 > ReadLumpSize) {
			ReadLumpPos--;
			return EOF;
		}
	}

	return *((ILubyte*)ReadLump + ReadLumpPos++);
}


ILuint ILAPIENTRY iReadFile(ILvoid *Buffer, ILuint Size, ILuint Number)
{
	ILuint	TotalBytes = 0, BytesCopied;
	ILuint	BuffSize = Size * Number;

	if (!UseCache) {
		return ReadProc(Buffer, Size, Number, FileRead);
	}

	/*if (Cache == NULL || CacheSize == 0) {  // Shouldn't happen, but we check anyway.
		return ReadProc(Buffer, Size, Number, FileRead);
	}*/

	if (BuffSize < CacheSize - CachePos) {
		memcpy(Buffer, Cache + CachePos, BuffSize);
		CachePos += BuffSize;
		return BuffSize;
	}
	else {
		while (TotalBytes < BuffSize) {
			// If loop through more than once, after first, CachePos is 0.
			if (TotalBytes + CacheSize - CachePos > BuffSize)
				BytesCopied = BuffSize - TotalBytes;
			else
				BytesCopied = CacheSize - CachePos;

			memcpy((ILubyte*)Buffer + TotalBytes, Cache + CachePos, BytesCopied);
			TotalBytes += BytesCopied;
			CachePos += BytesCopied;
			if (TotalBytes < BuffSize) {
				iPreCache(CacheSize);
			}
		}
	}

	return TotalBytes;
}


ILuint ILAPIENTRY iReadLump(ILvoid *Buffer, ILuint Size, ILuint Number)
{
	ILuint i, ByteSize = Size * Number;

	for (i = 0; i < ByteSize; i++) {
		*((ILubyte*)Buffer + i) = *((ILubyte*)ReadLump + ReadLumpPos + i);
		if (ReadLumpSize > 0) {  // ReadLumpSize is too large to care about apparently
			if (ReadLumpPos + i > ReadLumpSize) {
				ReadLumpPos += i;
				return i;
			}
		}
	}

	ReadLumpPos += i;
	return i;
}


ILboolean iPreCache(ILuint Size)
{
	// Reading from a memory lump, so don't cache.
	if (iread == iReadLump) {
		iUnCache();
		return IL_TRUE;
	}

	if (Cache) {
		ifree(Cache);
	}

	if (Size == 0) {
		Size = 1;
	}

	Cache = (ILubyte*)ialloc(Size);
	if (Cache == NULL) {
		ilSetError(IL_OUT_OF_MEMORY);
		return IL_FALSE;
	}

	UseCache = IL_FALSE;
	CacheSize = iread(Cache, 1, Size);

	//CacheSize = Size;
	CachePos = 0;
	UseCache = IL_TRUE;

	return IL_TRUE;
}


ILvoid iUnCache()
{
	CacheSize = 0;
	CachePos = 0;
	if (Cache) {
		ifree(Cache);
		Cache = NULL;
	}
	UseCache = IL_FALSE;

	return;
}


ILuint ILAPIENTRY iSeekRFile(ILint Offset, ILuint Mode)
{
	if (Mode == IL_SEEK_SET)
		Offset += ReadFileStart;  // This allows us to use IL_SEEK_SET in the middle of a file.
	return SeekRProc(FileRead, Offset, Mode);
}


// Returns 1 on error, 0 on success
ILuint ILAPIENTRY iSeekRLump(ILint Offset, ILuint Mode)
{
	switch (Mode)
	{
		case IL_SEEK_SET:
			if (Offset > (ILint)ReadLumpSize)
				return 1;
			ReadLumpPos = Offset;
			break;

		case IL_SEEK_CUR:
			if (ReadLumpPos + Offset > ReadLumpSize)
				return 1;
			ReadLumpPos += Offset;
			break;

		case IL_SEEK_END:
			if (Offset > 0)
				return 1;
			// Should we use >= instead?
			if (abs(Offset) > (ILint)ReadLumpSize)  // If ReadLumpSize == 0, too bad
				return 1;
			ReadLumpPos = ReadLumpSize + Offset;
			break;

		default:
			return 1;
	}

	return 0;
}


ILuint ILAPIENTRY iTellRFile(ILvoid)
{
	return TellRProc(FileRead);
}


ILuint ILAPIENTRY iTellRLump(ILvoid)
{
	return ReadLumpPos;
}


ILHANDLE ILAPIENTRY iGetFile(ILvoid)
{
	return FileRead;
}


ILubyte* ILAPIENTRY iGetLump(ILvoid)
{
	return ReadLump;
}



// Next 4 functions are the default write functions

ILint ILAPIENTRY iPutcFile(ILubyte Char)
{
	return PutcProc(Char, FileWrite);
}


ILint ILAPIENTRY iPutcLump(ILubyte Char)
{
	if (WriteLumpPos >= WriteLumpSize)
		return -1;  // EOF
	*((ILubyte*)(WriteLump) + WriteLumpPos++) = Char;
	return Char;
}


ILint ILAPIENTRY iWriteFile(const ILvoid *Buffer, ILuint Size, ILuint Number)
{
	return WriteProc(Buffer, Size, Number, FileWrite);
}


ILint ILAPIENTRY iWriteLump(const ILvoid *Buffer, ILuint Size, ILuint Number)
{
	ILuint SizeBytes = Size * Number;
	ILuint i = 0;

	for (; i < SizeBytes; i++) {
		if (WriteLumpSize > 0) {
			if (WriteLumpPos + i >= WriteLumpSize) {  // Should we use > instead?
				WriteLumpPos += i;
				return i;
			}
		}

		*((ILubyte*)WriteLump + WriteLumpPos + i) = *((ILubyte*)Buffer + i);
	}

	WriteLumpPos += SizeBytes;
	
	return SizeBytes;
}


ILuint ILAPIENTRY iSeekWFile(ILint Offset, ILuint Mode)
{
	if (Mode == IL_SEEK_SET)
		Offset += WriteFileStart;  // This allows us to use IL_SEEK_SET in the middle of a file.
	return SeekWProc(FileWrite, Offset, Mode);
}


// Returns 1 on error, 0 on success
ILuint ILAPIENTRY iSeekWLump(ILint Offset, ILuint Mode)
{
	switch (Mode)
	{
		case IL_SEEK_SET:
			if (Offset > (ILint)WriteLumpSize)
				return 1;
			WriteLumpPos = Offset;
			break;

		case IL_SEEK_CUR:
			if (WriteLumpPos + Offset > WriteLumpSize)
				return 1;
			WriteLumpPos += Offset;
			break;

		case IL_SEEK_END:
			if (Offset > 0)
				return 1;
			// Should we use >= instead?
			if (abs(Offset) > (ILint)WriteLumpSize)  // If WriteLumpSize == 0, too bad
				return 1;
			WriteLumpPos = WriteLumpSize + Offset;
			break;

		default:
			return 1;
	}

	return 0;
}


ILuint ILAPIENTRY iTellWFile(ILvoid)
{
	return TellWProc(FileWrite);
}


ILuint ILAPIENTRY iTellWLump(ILvoid)
{
	return WriteLumpPos;
}
