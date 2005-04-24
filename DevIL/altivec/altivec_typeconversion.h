/*
 *  altivec_typeconversion.h
 *  DevIL
 *
 *  Created by Dario on 24/04/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "altivec_common.h"
#ifdef ALTIVEC_GCC

// Used to convert RGB <-> BGR in various data types
void abc2cba_byte( ILubyte *data, ILuint length, ILubyte *newdata );
void abc2cba_short( ILushort *data, ILuint length, ILushort *newdata );
void abc2cba_int( ILuint *data, ILuint length, ILuint *newdata );
void abc2cba_double( ILdouble *data, ILuint length, ILdouble *newdata );

#endif