//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 05/28/2001 <--Y2K Compliant! =]
//
// Filename: openilut/opengl.h
//
// Description: OpenGL functions for images
//
//-----------------------------------------------------------------------------

#ifndef ILUT_OPENGL_H
#define ILUT_OPENGL_H


#include "ilut_internal.h"


#ifndef min
#define min(a, b)	(((a) < (b)) ? (a) : (b))
#endif


ILvoid iGLSetMaxW(ILuint Width);
ILvoid iGLSetMaxH(ILuint Height);

ILenum		ilutGLFormat(ILenum, ILubyte);
ILimage*	MakeGLCompliant(ILimage *Src);
ILboolean	iIsExtensionSupported(const char *Ext);


#endif//ILUT_OPENGL_H
