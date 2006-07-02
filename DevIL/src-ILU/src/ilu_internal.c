
#include "ilu_internal.h"

ILdouble IL_PI      = 3.1415926535897932384626;
ILdouble IL_DEGCONV = 0.0174532925199432957692;
ILimage *iluCurImage = NULL;

#ifdef INLINE
#undef INLINE
#define INLINE
#endif

INLINE ILfloat ilCos(ILfloat Angle) {
	return (ILfloat)(cos(Angle * IL_DEGCONV));
}


INLINE ILfloat ilSin(ILfloat Angle) {
	return (ILfloat)(sin(Angle * IL_DEGCONV));
}


INLINE ILint ilRound(ILfloat Num) {
	return (ILint)(Num + 0.5);
}
