//-----------------------------------------------------------------------------
//
// ImageLib Stress Test Source
// Copyright (C) 2001 by Denton Woods
// Last modified: 06/27/2001 <--Y2K Compliant! =]
//
// Filename: testil/stress/stresstest.c
//
// Description:  Thorougly tests several aspects of DevIL, in order
//					to try to test DevIL's stability and reliability.
//					So far, the results have been positive and promising.
//
//-----------------------------------------------------------------------------


#if !defined (__APPLE__)
	#include <conio.h>
#else
#include <signal.h>
#include <unistd.h>
	int intr_signal = 0;
	void intrhandler( int sig );
#endif

#include <limits.h>
#include <il/il.h>
#include <il/ilu.h>


int main(int argc, char **argv)
{
#if defined (__APPLE__)
	struct sigaction intraction;
#endif
	ILuint id, Error, i;
	ILuint id1[2000], id2[2000], id3[2000], id4[2000], id5[2000];

	// We use the filename specified in the first argument of the command-line.
	if (argc < 2) {
		printf("Please specify a filename.\n");
		return 1;
	}

	// Check if the shared lib's version matches the executable's version.
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION) {
		printf("DevIL version is different...exiting!\n");
		return 2;
	}

	// Required for Djgpp.
	ilInit();

	// Allows us to overwrite the same output file multiple times.
	ilEnable(IL_FILE_OVERWRITE);

	// Tests the reliability of ilGenImages() and its resizing capabilities.
	ilGenImages(2000, id1);
	ilGenImages(2000, id2);
	ilGenImages(2000, id3);
	ilGenImages(2000, id4);
	ilGenImages(2000, id5);
	// Actually use the last image generated to make sure it's reliable.
	ilGenImages(1, &id);

	// Bind the image name.
	ilBindImage(id);
	// Load the image specified in argv[1] into the image named by id.
	ilLoadImage(argv[1]);

	// Display the image's dimensions to the end user.
	printf("Width: %d  Height: %d  Depth: %d  Bpp: %d\n", ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_DEPTH), ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));

	// Bilinear interpolation is the most exhaustive of the scaling functions.
	iluImageParameter(ILU_FILTER, ILU_BILINEAR);

	// Perform the test for-freakin'-ever or until the user presses a key.
#if defined (__APPLE__)
	intraction.sa_handler = intrhandler;
	intraction.sa_mask = 0;
	intraction.sa_flags = 0;
	sigaction( SIGINT, &intraction, NULL );
	for (i = 0; i < UINT_MAX-20 && !intr_signal; i++) {
#else
 	for (i = 0; i < UINT_MAX-20 && !kbhit(); i++) {
#endif
		printf("%d\n", i);  // Show where we are in the test.
		ilLoadImage(argv[1]);  // Test loading.
		iluScale(64, 64, 1);  // Test scaling
		ilSaveImage("test.tga");  // Test saving

		// Checks if any errors have occurred.
		while ((Error = ilGetError())) {
			i = UINT_MAX;  // Terminate the testing loop early.
			printf("Error: %s\n", iluErrorString(Error));  // Print the error.
		}
	}

	// Tests the reliability of ilDeleteImages().
	ilDeleteImages(2000, id1);
	ilDeleteImages(2000, id2);
	ilDeleteImages(2000, id3);
	ilDeleteImages(2000, id4);
	ilDeleteImages(2000, id5);
	ilDeleteImages(1, &id);

	// Tests ilGenImages()/ilDeleteImages() ability to reuse deleted image names.
	ilGenImages(1, &id);
	ilDeleteImages(1, &id);

	// Check for any errors that have occurred after the testing loop.
	while ((Error = ilGetError())) {
		printf("Error: %s\n", iluErrorString(Error));  // Print the error.
	}

	return 0;
}

#if defined (__APPLE__)
void intrhandler( int sig )
{
	intr_signal = 1;
}
#endif
