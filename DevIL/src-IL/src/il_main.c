//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: openil/main.c
//
// Description: Startup functions
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


ILvoid iFreeMem();

// ONLY call at startup.
ILvoid ILAPIENTRY ilInit()
{
	ilResetMemory();  // Do first, since it handles allocations.
	ilSetError(IL_NO_ERROR);
	ilDefaultStates();  // Set states to their defaults.
	// Sets default file-reading callbacks.
	ilResetRead();
	ilResetWrite();
#ifndef _WIN32_WCE
	atexit((void*)ilRemoveRegistered);
#endif//_WIN32_WCE
	iFreeMem();
	iSetImage0();  // Beware!  Clears all existing textures!
	iBindImageTemp();  // Go ahead and create the temporary image.
	return;
}
