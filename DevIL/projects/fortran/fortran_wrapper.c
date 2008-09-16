#include <IL/il.h>
#include <string.h>
#include <malloc.h>

// NOTE!
//  The biggest problem about using DevIL from Fortran is the
//  fact that Fortran does arrays in colum-major format.
//
//  We are using DevIL return values as the first parameter.
//


// From http://www.ibiblio.org/pub/languages/fortran/ch1-11.html
//  This is how Fortran passes strings.
struct descriptor						/* VMS fixed length string    */
{										/* descriptor used in FORTRAN */
	unsigned short  length;
	unsigned char   data_type,			/* = 14      */
					dsc_class;			/* = 1       */
	char            *string_ptr;
}; 


void ilFActiveImage_(int *RetVal, int *Number)
{
	*RetVal = ilActiveImage((unsigned int)*Number);
	return;
}

void ilFActiveLayer_(int *RetVal, int *Number)
{
	*RetVal = ilActiveImage((unsigned int)*Number);
	return;
}

void ilFActiveMipmap_(int *RetVal, int *Number)
{
	*RetVal = ilActiveImage((unsigned int)*Number);
	return;
}

void ilFApplyPal_(int *RetVal, struct descriptor *Desc)
{
	char *Filename = malloc(Desc->length + 1);
	strncpy(Filename, Desc->string_ptr, Desc->length);
	Filename[Desc->length] = (char)NULL;
	*RetVal = ilApplyPal(Filename);

	free(Filename);
	return;
}

void ilFBindImage_(int *Image)
{
	ilBindImage((unsigned int)*Image);
	return;
}

void ilFBlit_(int *RetVal, int *Source, int *DestX, int *DestY, int *DestZ, int *SrcX, int *SrcY, int *SrcZ, int *Width, int *Height, int *Depth)
{
	*RetVal = ilBlit(*Source, *DestX, *DestY, *DestZ, *SrcX, *SrcY, *SrcZ, *Width, *Height, *Depth);
	return;
}

void ilFClearColour_(float *Red, float *Green, float *Blue, float *Alpha)
{
	ilClearColour(*Red, *Green, *Blue, *Alpha);
	return;
}

ILAPI ILboolean ILAPIENTRY ilClearImage(void);
ILAPI ILuint    ILAPIENTRY ilCloneCurImage(void);
ILAPI ILboolean ILAPIENTRY ilCompressFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILboolean ILAPIENTRY ilConvertPal(ILenum DestFormat);

void ilFClearImage_(int *RetVal)
{
	*RetVal = ilClearImage();
	return;
}

void ilFCloneCurImage_(int *RetVal)
{
	*RetVal = ilCloneCurImage();
	return;
}

void ilFCompressFunc_(int *RetVal, int *Mode)
{
	*RetVal = ilCompressFunc(*Mode);
	return;
}

void ilFConvertImage_(int *RetVal, int *DestFormat, int *DestType)
{
	*RetVal = ilConvertImage(*DestFormat, *DestType);
	return;
}

