//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 08/30/2001 <--Y2K Compliant! =]
//
// Filename: openilut/states.c
//
// Description: The state machine
//
//-----------------------------------------------------------------------------


#include "ilut_internal.h"
#include "ilut_states.h"
#include "ilut_opengl.h"


const char *_ilutVendor		= "Abysmal Software";
const char *_ilutVersion	= "Developer's Image Library Utility Toolkit (ILUT) 1.3.0";


// Set all states to their defaults
ILvoid ilutDefaultStates()
{
	ilutStates[ilutCurrentPos].ilutUsePalettes = IL_FALSE;
	ilutStates[ilutCurrentPos].ilutOglConv = IL_FALSE;  // IL_TRUE ?
	ilutStates[ilutCurrentPos].D3DMipLevels = 0;
}


ILvoid ILAPIENTRY ilutD3D8MipFunc(ILuint NumLevels)
{
	ilutStates[ilutCurrentPos].D3DMipLevels = NumLevels;
	return;
}


const char* ILAPIENTRY ilutGetString(ILenum StringName)
{
	switch (StringName)
	{
		case IL_VENDOR:
			return (const char*)_ilutVendor;
		case IL_VERSION:
			return (const char*)_ilutVersion;
		default:
			ilSetError(ILUT_INVALID_PARAM);
			break;
	}
	return NULL;
}


ILboolean ILAPIENTRY ilutEnable(ILenum Mode)
{
	return ilutAble(Mode, IL_TRUE);
}


ILboolean ILAPIENTRY ilutDisable(ILenum Mode)
{
	return ilutAble(Mode, IL_FALSE);
}


ILboolean ilutAble(ILenum Mode, ILboolean Flag)
{
	switch (Mode)
	{
		case ILUT_PALETTE_MODE:
      		ilutStates[ilutCurrentPos].ilutUsePalettes = Flag;
      		break;

		case ILUT_OPENGL_CONV:
      		ilutStates[ilutCurrentPos].ilutOglConv = Flag;
      		break;


		default:
			ilSetError(ILUT_INVALID_ENUM);
			return IL_FALSE;
	}

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutIsEnabled(ILenum Mode)
{
	switch (Mode)
	{
		case ILUT_PALETTE_MODE:
			return ilutStates[ilutCurrentPos].ilutUsePalettes;

		case ILUT_OPENGL_CONV:
      		return ilutStates[ilutCurrentPos].ilutOglConv;


		default:
			ilSetError(ILUT_INVALID_ENUM);
	}

	return IL_FALSE;
}


ILboolean ILAPIENTRY ilutIsDisabled(ILenum Mode)
{
	return !ilutIsEnabled(Mode);
}


ILvoid ILAPIENTRY ilutGetBooleanv(ILenum Mode, ILboolean *Param)
{
	switch (Mode)
	{
		case ILUT_PALETTE_MODE:
			*Param = ilutStates[ilutCurrentPos].ilutUsePalettes;
			break;

		case ILUT_OPENGL_CONV:
      		*Param = ilutStates[ilutCurrentPos].ilutOglConv;
      		break;


		default:
			ilSetError(ILUT_INVALID_ENUM);
	}
	return;
}


ILboolean ILAPIENTRY ilutGetBoolean(ILenum Mode)
{
	static ILboolean Temp;
	Temp = IL_FALSE;
	ilutGetBooleanv(Mode, &Temp);
	return Temp;
}


ILvoid ILAPIENTRY ilutGetIntegerv(ILenum Mode, ILint *Param)
{
	switch (Mode)
	{
		/*case IL_ORIGIN_MODE:
			*Param = ilutStates[ilutCurrentPos].ilutOriginMode;
			break;*/
		case ILUT_VERSION_NUM:
			*Param = ILUT_VERSION;
			break;
		case ILUT_PALETTE_MODE:
			*Param = ilutStates[ilutCurrentPos].ilutUsePalettes;
			break;
		case ILUT_OPENGL_CONV:
      		*Param = ilutStates[ilutCurrentPos].ilutOglConv;
      		break;
		case ILUT_D3D_MIPLEVELS:
			*Param = ilutStates[ilutCurrentPos].D3DMipLevels;
			break;

		default:
			ilSetError(ILUT_INVALID_ENUM);
	}
	return;
}


ILint ILAPIENTRY ilutGetInteger(ILenum Mode)
{
	static ILint Temp;
	Temp = 0;
	ilutGetIntegerv(Mode, &Temp);
	return Temp;
}


ILvoid ILAPIENTRY ilutSetInteger(ILenum Mode, ILint Param)
{
	switch (Mode)
	{
#ifdef ILUT_USE_OPENGL
		case ILUT_MAXTEX_WIDTH:
			iGLSetMaxW(Param);
			break;
		case ILUT_MAXTEX_HEIGHT:
			iGLSetMaxH(Param);
			break;
		case ILUT_MAXTEX_DEPTH:

			break;
#endif//ILUT_USE_OPENGL

		default:
			ilSetError(ILUT_INVALID_ENUM);
	}
	return;
}


ILvoid ILAPIENTRY ilutPushAttrib(ILuint Bits)
{
	// Should we check here to see if ilCurrentPos is negative?

	if (ilutCurrentPos >= ILUT_ATTRIB_STACK_MAX - 1) {
		ilutCurrentPos = ILUT_ATTRIB_STACK_MAX - 1;
		ilSetError(ILUT_STACK_OVERFLOW);
		return;
	}

	ilutCurrentPos++;

	//memcpy(&ilutStates[ilutCurrentPos], &ilutStates[ilutCurrentPos - 1], sizeof(ILUT_STATES));

	if (Bits & ILUT_OPENGL_BIT) {
		ilutStates[ilutCurrentPos].ilutUsePalettes = ilutStates[ilutCurrentPos-1].ilutUsePalettes;
		ilutStates[ilutCurrentPos].ilutOglConv = ilutStates[ilutCurrentPos-1].ilutOglConv;
	}
	if (Bits & ILUT_D3D_BIT) {
		ilutStates[ilutCurrentPos].D3DMipLevels = ilutStates[ilutCurrentPos-1].D3DMipLevels;
	}

	return;
}


ILvoid ILAPIENTRY ilutPopAttrib()
{
	if (ilutCurrentPos <= 0) {
		ilutCurrentPos = 0;
		ilSetError(ILUT_STACK_UNDERFLOW);
		return;
	}

	// Should we check here to see if ilutCurrentPos is too large?
	ilutCurrentPos--;

	return;
}


ILboolean ILAPIENTRY ilutRenderer(ILenum Renderer)
{
	if (Renderer > ILUT_WIN32) {
		ilSetError(ILUT_INVALID_VALUE);
		return IL_FALSE;
	}

	switch (Renderer)
	{
		#ifdef ILUT_USE_OPENGL
		case ILUT_OPENGL:
			return ilutGLInit();
		#endif

		#ifdef ILUT_USE_WIN32
		case ILUT_WIN32:
			return ilutWin32Init();
		#endif

		default:
			ilSetError(ILUT_NOT_SUPPORTED);
	}

	return IL_FALSE;
}
