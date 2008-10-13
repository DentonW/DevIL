//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 10/12/2008
//
// Filename: src-ILU/include/ilu_err_spanish.h
//
// Description: Error functions in Spanish
//
//-----------------------------------------------------------------------------


#ifndef ILU_ERR_SPANISH_H
#define ILU_ERR_SPANISH_H

#include "ilu_internal.h"

ILconst_string iluErrorStringsSpanish[IL_FILE_READ_ERROR - IL_INVALID_ENUM + 1] = {
	L"enumerador incorrecto",
    L"no queda memoria disponible",
	L"formato no soportado todavía",
	L"error interno",
	L"valor incorrecto",
    L"operación ilegal",
	L"valor de fichero ilegal",
	L"cabecera incorrecta",
	L"parámetro incorrecto",
	L"no se puede abrir el fichero",
	L"extensión desconocida",
	L"el fichero ya existe",
	L"formato de salida equivalente",
	L"desbordamiento superior de pila",
    L"desbordamiento inferior de pila",
	L"conversión incorrecta",
	L"número de dimensiones incorrecto",
	L"error de lectura en el fichero"
};

#endif//ILU_ERR_SPANISH_H