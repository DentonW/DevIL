//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/28/2001 <--Y2K Compliant! =]
//
// Filename: openilut/main.c
//
// Description: Startup functions
//
//-----------------------------------------------------------------------------


#include "ilut_internal.h"

#ifdef _WIN32

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	hModule;  lpReserved;

	// only initialize when attached to a new process. setup can cause errors in OpenIL
	// when called on a per thread basis
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		//ilutInit();
	}

	return TRUE;
}


#else  // Should check if gcc?

// Should be able to condense this...
static void GccMain() __attribute__((constructor));
static void GccMain()
{
	//ilutInit();
}

#endif


ILvoid ILAPIENTRY ilutInit()
{
	ilutDefaultStates();  // Set states to their defaults
	// Can cause crashes if OpenIL is not initialized yet

#ifdef ILUT_USE_OPENGL
	ilutGLInit();  // default renderer is OpenGL
#endif

	return;
}
