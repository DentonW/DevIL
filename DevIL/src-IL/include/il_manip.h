//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2001-2002 by Denton Woods
// Last modified: 05/25/2001 <--Y2K Compliant! =]
//
// Filename: src-IL/include/il_manip.h
//
// Description: Image manipulation
//
//-----------------------------------------------------------------------------

#ifndef MANIP_H
#define MANIP_H

#ifdef _cplusplus
extern "C" {
#endif

ILboolean ilFlipImage(ILvoid);
ILboolean ilMirrorImage(ILvoid);	//@JASON New routine created 03/28/2001

#ifdef _cplusplus
}
#endif

#endif//MANIP_H
