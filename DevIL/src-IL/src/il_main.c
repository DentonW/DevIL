//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 02/16/2002 <--Y2K Compliant! =]
//
// Filename: il/il_main.c
//
// Description: Startup function
//
//-----------------------------------------------------------------------------


#include "il_internal.h"

#ifdef _WIN32

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	hModule;  ul_reason_for_call;  lpReserved;

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		//ilInit();
	}

	return TRUE;
}

#endif


