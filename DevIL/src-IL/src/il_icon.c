//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2001-2002 by Denton Woods
// Last modified: 06/13/2001 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_icon.c
//
// Description: Reads from a Windows icon (.ico) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_ICO
#include "il_icon.h"


//! Reads an icon file.
ILboolean ilLoadIcon(ILconst_string FileName)
{
	ILHANDLE	IconFile;
	ILboolean	bIcon = IL_FALSE;

	IconFile = iopenr(FileName);
	if (IconFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bIcon;
	}

	bIcon = ilLoadIconF(IconFile);
	icloser(IconFile);

	return bIcon;
}


//! Reads an already-opened icon file.
ILboolean ilLoadIconF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadIconInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains an icon.
ILboolean ilLoadIconL(const ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadIconInternal();
}


// Internal function used to load the icon.
ILboolean iLoadIconInternal()
{
	ICODIR		IconDir;
	ICODIRENTRY	*DirEntries = NULL;
	ICOIMAGE	*IconImages = NULL;
	ILimage		*Image = NULL;
	ILint		i;
	ILuint		Size, PadSize, ANDPadSize, j, k, l, m, x, w, CurAndByte, AndBytes;
	ILboolean	BaseCreated = IL_FALSE;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}


	IconDir.Reserved = GetLittleShort();

	IconDir.Type = GetLittleShort();

	IconDir.Count = GetLittleShort();

	if (ieof())
		goto file_read_error;

	DirEntries = (ICODIRENTRY*)ialloc(sizeof(ICODIRENTRY) * IconDir.Count);
	IconImages = (ICOIMAGE*)ialloc(sizeof(ICOIMAGE) * IconDir.Count);
	if (DirEntries == NULL || IconImages == NULL) {
		ifree(DirEntries);
		ifree(IconImages);
		return IL_FALSE;
	}

	// Make it easier for file_read_error.
	for (i = 0; i < IconDir.Count; i++)
		imemclear(&IconImages[i], sizeof(ICOIMAGE));

	for (i = 0; i < IconDir.Count; ++i) {

		DirEntries[i].Width = igetc();

		DirEntries[i].Height = igetc();

		DirEntries[i].NumColours = igetc();

		DirEntries[i].Reserved = igetc();

		DirEntries[i].Planes = GetLittleShort();

		DirEntries[i].Bpp = GetLittleShort();

		DirEntries[i].SizeOfData = GetLittleUInt();

		DirEntries[i].Offset = GetLittleUInt();



		if(ieof())

			goto file_read_error;

	}

	for (i = 0; i < IconDir.Count; i++) {
		iseek(DirEntries[i].Offset, IL_SEEK_SET);



		IconImages[i].Head.Size = GetLittleInt();

		IconImages[i].Head.Width = GetLittleInt();

		IconImages[i].Head.Height = GetLittleInt();

		IconImages[i].Head.Planes = GetLittleShort();

		IconImages[i].Head.BitCount = GetLittleShort();

		IconImages[i].Head.Compression = GetLittleInt();

		IconImages[i].Head.SizeImage = GetLittleInt();

		IconImages[i].Head.XPixPerMeter = GetLittleInt();

		IconImages[i].Head.YPixPerMeter = GetLittleInt();

		IconImages[i].Head.ColourUsed = GetLittleInt();

		IconImages[i].Head.ColourImportant = GetLittleInt();


		if (ieof())
			goto file_read_error;

		if (IconImages[i].Head.BitCount < 8) {
			if (IconImages[i].Head.ColourUsed == 0) {
				switch (IconImages[i].Head.BitCount)
				{
					case 1:
						IconImages[i].Head.ColourUsed = 2;
						break;
					case 4:
						IconImages[i].Head.ColourUsed = 16;
						break;
				}
			}
			IconImages[i].Pal = (ILubyte*)ialloc(IconImages[i].Head.ColourUsed * 4);
			if (IconImages[i].Pal == NULL)
				goto file_read_error;  // @TODO: Rename the label.
			if (iread(IconImages[i].Pal, IconImages[i].Head.ColourUsed * 4, 1) != 1)
				goto file_read_error;
		}
		else if (IconImages[i].Head.BitCount == 8) {
			IconImages[i].Pal = (ILubyte*)ialloc(256 * 4);
			if (IconImages[i].Pal == NULL)
				goto file_read_error;
			if (iread(IconImages[i].Pal, 1, 256 * 4) != 256*4)
				goto file_read_error;
		}
		else {
			IconImages[i].Pal = NULL;
		}

		PadSize = (4 - ((IconImages[i].Head.Width*IconImages[i].Head.BitCount + 7) / 8) % 4) % 4;  // Has to be DWORD-aligned.

		ANDPadSize = (4 - ((IconImages[i].Head.Width + 7) / 8) % 4) % 4;  // AND is 1 bit/pixel

		Size = ((IconImages[i].Head.Width*IconImages[i].Head.BitCount + 7) / 8 + PadSize)

							* (IconImages[i].Head.Height / 2);


		IconImages[i].Data = (ILubyte*)ialloc(Size);
		if (IconImages[i].Data == NULL)
			goto file_read_error;
		if (iread(IconImages[i].Data, 1, Size) != Size)
			goto file_read_error;

		Size = (((IconImages[i].Head.Width + 7) /8) + ANDPadSize) * (IconImages[i].Head.Height / 2);
		IconImages[i].AND = (ILubyte*)ialloc(Size);
		if (IconImages[i].AND == NULL)
			goto file_read_error;
		if (iread(IconImages[i].AND, 1, Size) != Size)
			goto file_read_error;
	}


	for (i = 0; i < IconDir.Count; i++) {
		if (IconImages[i].Head.BitCount != 1 && IconImages[i].Head.BitCount != 4 &&
			IconImages[i].Head.BitCount != 8 && IconImages[i].Head.BitCount != 24 &&

			IconImages[i].Head.BitCount != 32)
			continue;

		if (!BaseCreated) {
			ilTexImage(IconImages[i].Head.Width, IconImages[i].Head.Height / 2, 1, 4, IL_BGRA, IL_UNSIGNED_BYTE, NULL);
			iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;
			Image = iCurImage;
			BaseCreated = IL_TRUE;
		}
		else {
			Image->Next = ilNewImage(IconImages[i].Head.Width, IconImages[i].Head.Height / 2, 1, 4, 1);
			Image = Image->Next;
			Image->Format = IL_BGRA;
		}
		Image->Type = IL_UNSIGNED_BYTE;

		j = 0;  k = 0;  l = 128;  CurAndByte = 0; x = 0;

		w = IconImages[i].Head.Width;
		PadSize = (4 - ((w*IconImages[i].Head.BitCount + 7) / 8) % 4) % 4;  // Has to be DWORD-aligned.

		ANDPadSize = (4 - ((w + 7) / 8) % 4) % 4;  // AND is 1 bit/pixel
		AndBytes = (w + 7) / 8;

		if (IconImages[i].Head.BitCount == 1) {
			for (; j < Image->SizeOfData; k++) {
				for (m = 128; m && x < w; m >>= 1) {
					Image->Data[j] = IconImages[i].Pal[!!(IconImages[i].Data[k] & m) * 4];
					Image->Data[j+1] = IconImages[i].Pal[!!(IconImages[i].Data[k] & m) * 4 + 1];
					Image->Data[j+2] = IconImages[i].Pal[!!(IconImages[i].Data[k] & m) * 4 + 2];
					Image->Data[j+3] = (IconImages[i].AND[CurAndByte] & l) != 0 ? 0 : 255;
					j += 4;
					l >>= 1;

					++x;
				}
				if (l == 0 || x == w) {
					l = 128;
					CurAndByte++;
					if (x == w) {
						CurAndByte += ANDPadSize;
						k += PadSize;
						x = 0;
					}

				}
			}
		}
		else if (IconImages[i].Head.BitCount == 4) {
			for (; j < Image->SizeOfData; j += 8, k++) {
				Image->Data[j] = IconImages[i].Pal[((IconImages[i].Data[k] & 0xF0) >> 4) * 4];
				Image->Data[j+1] = IconImages[i].Pal[((IconImages[i].Data[k] & 0xF0) >> 4) * 4 + 1];
				Image->Data[j+2] = IconImages[i].Pal[((IconImages[i].Data[k] & 0xF0) >> 4) * 4 + 2];
				Image->Data[j+3] = (IconImages[i].AND[CurAndByte] & l) != 0 ? 0 : 255;
				l >>= 1;

				++x;

				if(x < w) {
					Image->Data[j+4] = IconImages[i].Pal[(IconImages[i].Data[k] & 0x0F) * 4];
					Image->Data[j+5] = IconImages[i].Pal[(IconImages[i].Data[k] & 0x0F) * 4 + 1];
					Image->Data[j+6] = IconImages[i].Pal[(IconImages[i].Data[k] & 0x0F) * 4 + 2];
					Image->Data[j+7] = (IconImages[i].AND[CurAndByte] & l) != 0 ? 0 : 255;
					l >>= 1;

					++x;

				}

				else

					j -= 4;


				if (l == 0 || x == w) {
					l = 128;
					CurAndByte++;
					if (x == w) {
						CurAndByte += ANDPadSize;

						k += PadSize;
						x = 0;
					}
				}
			}
		}
		else if (IconImages[i].Head.BitCount == 8) {
			for (; j < Image->SizeOfData; j += 4, k++) {
				Image->Data[j] = IconImages[i].Pal[IconImages[i].Data[k] * 4];
				Image->Data[j+1] = IconImages[i].Pal[IconImages[i].Data[k] * 4 + 1];
				Image->Data[j+2] = IconImages[i].Pal[IconImages[i].Data[k] * 4 + 2];
				Image->Data[j+3] = (IconImages[i].AND[CurAndByte] & l) != 0 ? 0 : 255;
				l >>= 1;

				++x;
				if (l == 0 || x == w) {
					l = 128;
					CurAndByte++;
					if (x == w) {
						CurAndByte += ANDPadSize;

						k += PadSize;
						x = 0;
					}
				}
			}
		}
		else if (IconImages[i].Head.BitCount == 24) {
			for (; j < Image->SizeOfData; j += 4, k += 3) {
				Image->Data[j] = IconImages[i].Data[k];
				Image->Data[j+1] = IconImages[i].Data[k+1];
				Image->Data[j+2] = IconImages[i].Data[k+2];
				Image->Data[j+3] = (IconImages[i].AND[CurAndByte] & l) != 0 ? 0 : 255;
				l >>= 1;

				++x;
				if (l == 0 || x == w) {
					l = 128;
					CurAndByte++;
					if (x == w) {
						CurAndByte += ANDPadSize;

						k += PadSize;
						x = 0;
					}
				}
			}
		}

		else if (IconImages[i].Head.BitCount == 32) {

			for (; j < Image->SizeOfData; j += 4, k += 4) {

				Image->Data[j] = IconImages[i].Data[k];

				Image->Data[j+1] = IconImages[i].Data[k+1];

				Image->Data[j+2] = IconImages[i].Data[k+2];

				//If the icon has 4 channels, use 4th channel for alpha...

				//(for winxp style icons with true alpha channel

				Image->Data[j+3] = IconImages[i].Data[k+3];

			}

		}
	}


	for (i = 0; i < IconDir.Count; i++) {
		ifree(IconImages[i].Pal);
		ifree(IconImages[i].Data);
		ifree(IconImages[i].AND);
	}
	ifree(IconImages);
	ifree(DirEntries);

	ilFixImage();

	return IL_TRUE;

file_read_error:
	if (IconImages) {
		for (i = 0; i < IconDir.Count; i++) {
			if (IconImages[i].Pal)
				ifree(IconImages[i].Pal);
			if (IconImages[i].Data)
				ifree(IconImages[i].Data);
			if (IconImages[i].AND)
				ifree(IconImages[i].AND);
		}
		ifree(IconImages);
	}
	if (DirEntries)
		ifree(DirEntries);
	return IL_FALSE;
}


#endif
