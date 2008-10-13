//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 10/12/2008
//
// Filename: src-ILU/include/ilu_err_dutch.h
//
// Description: Error functions in Dutch.
//
//-----------------------------------------------------------------------------


#ifndef ILU_ERR_DUTCH_H
#define ILU_ERR_DUTCH_H

#include "ilu_internal.h"

ILconst_string iluErrorStringsDutch[IL_FILE_READ_ERROR - IL_INVALID_ENUM + 1] = {
	L"Ongeldige enumerant",
	L"Geen vrij geheugen meer",
	L"Format wordt nog niet ondersteund",
	L"Interne fout",
	L"Ongeldige waarde",
	L"Foute bewerking",
	L"Foute bestandswaarde",
	L"Foute bestandsbegin",
	L"Ongeldige parameter",
	L"Kan het bestand niet openen",
	L"ongeldige",
	L"Bestand bestaat reeds",
	L"uitgaand formaat equivalent",
	L"stapel overstroming",
	L"stapel onderstroming",
	L"ongeldige omzetting",
	L"slechte afmetingen",
	L"Leesfout in bestand",
};

ILconst_string iluLibErrorStrings[IL_LIB_MNG_ERROR - IL_LIB_GIF_ERROR + 1] = {
	L"fout in gif bibliotheek",
	L"fout in jpeg bibliotheek",
	L"fout in png bibliotheek",
	L"fout in tiff bibliotheek",
	L"fout in mng bibliotheek",
};

ILconst_string iluMiscErrorDutch[2] = {
	L"geen fout",
	L"onbekende fout"
};

#endif//ILU_ERR_DUTCH_H