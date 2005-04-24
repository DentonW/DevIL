//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Last modified: 17/04/2005
// by Meloni Dario
// 
// Description: Common altivec function.
//
//-----------------------------------------------------------------------------

#include <IL/il.h>

#ifdef ALTIVEC_GCC
#ifndef ALTIVEC_COMMON
#define ALTIVEC_COMMON

typedef union {
	vector unsigned int vuint;
	unsigned int suint[4];
	vector unsigned char vuchar;
	unsigned char suchar[4];
	vector float vf;
	float sf[4];
} vector_t;

// Loads 16 byte from the specified address, aligned or not
//vector unsigned char load_unaligned( unsigned char *buffer );

// Fills a vector with the specified value
vector float fill_vector_f( float value );

// Format conversion function
void abc2cba_byte( ILubyte *data, ILuint length, ILubyte *newdata );
void abc2cba_short( ILushort *data, ILuint length, ILushort *newdata );
void abc2cba_int( ILuint *data, ILuint length, ILuint *newdata );
void abc2cba_double( ILdouble *data, ILuint length, ILdouble *newdata );

#endif
#endif