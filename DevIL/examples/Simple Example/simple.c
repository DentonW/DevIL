//-----------------------------------------------------------------------------
//
// DevIL Source Example
// Copyright (C) 2000-2002 by Denton Woods
// Last modified:  4/22/2002 <--Y2K Compliant! =]
//
// Filename: examples/Simple Example/simple.c
//
// Description: Simplest implementation of an DevIL application.
//              Loads an image and saves it to a new image.
//              The images can be of any format and can be different.
//
//-----------------------------------------------------------------------------

// Required include files.
#include <IL/il.h>
#include <IL/ilu.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	ILuint	ImgId;
	ILenum	Error;

	// We use the filename specified in the first argument of the command-line.
	if (argc < 2) {
		fprintf(stderr, "DevIL_test : DevIL simple command line application.\n");
		fprintf(stderr, "Usage : DevIL_test <file> [output]\n");
		fprintf(stderr, "Default output is test.tga\n");
		return 1;
	}

	// Check if the shared lib's version matches the executable's version.
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) {
		printf("DevIL version is different...exiting!\n");
		return 2;
	}

	// Initialize DevIL.
	ilInit();

	// Generate the main image name to use.
	ilGenImages(1, &ImgId);

	// Bind this image name.
	ilBindImage(ImgId);

	// Loads the image specified by File into the image named by ImgId.
	if (!ilLoadImage(argv[1])) {
		printf("Could not open file...exiting.\n");
		return 3;
	}

	// Display the image's dimensions to the end user.
	printf("Width: %d  Height: %d  Depth: %d  Bpp: %d\n",
	       ilGetInteger(IL_IMAGE_WIDTH),
	       ilGetInteger(IL_IMAGE_HEIGHT),
	       ilGetInteger(IL_IMAGE_DEPTH),
	       ilGetInteger(IL_IMAGE_BITS_PER_PIXEL));

	// Enable this to let us overwrite the destination file if it already exists.
	ilEnable(IL_FILE_OVERWRITE);

	// If argv[2] is present, we save to this filename, else we save to test.tga.
	if (argc > 2)
		ilSaveImage(argv[2]);
	else
		ilSaveImage("test.tga");

	// We're done with the image, so let's delete it.
	ilDeleteImages(1, &ImgId);

	// Simple Error detection loop that displays the Error to the user in a human-readable form.
	while ((Error = ilGetError())) {
		printf("Error: %s\n", iluErrorString(Error));
	}

	return 0;

}
