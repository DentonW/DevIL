//-----------------------------------------------------------------------------
//
// ImageLib GL Test Source
// Copyright (C) 2000 by Denton Woods
// Last modified:  11/23/2000 <--Y2K Compliant! =]
//
// Filename:  testil/gltest/gltest.c
//
// Description:  Sample implementation of an OpenGL image viewer.
//
//-----------------------------------------------------------------------------

//
// We use FreeGlut in Windows, because it's more stable.
//
#ifdef _WIN32
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif
#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "gltest.h"
#include <math.h>
#include <string.h>

#ifdef  _DEBUG
#define IL_DEBUG
#endif//_DEBUG

//
// Easier to do this than change the project's settings in MSVC++. (I'm lazy!)
//
#ifdef _MSC_VER
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "freeglut13.lib")

	// Prevent the console window from popping up.
	#pragma comment(linker, "/entry:mainCRTStartup")
	#pragma comment(linker, "/subsystem:windows")
#endif

char	*FileName;
ILuint	Width, Height, Window;

int main(int argc, char** argv)
{
	// No filename is specified on the command-line.
	if (argc < 2) {
		return 1;
	}
	FileName = argv[1];  // Set filename equal to the first argument.

	//
	// Check if the shared lib's version matches the executable's version.
	//
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
		return 2;
	}

	// Needed for Djgpp
	ilInit();
	// GL cannot use palettes anyway, so convert early.
	ilEnable(IL_CONV_PAL);
	// Gets rid of dithering on some nVidia-based cards.
	ilutEnable(ILUT_OPENGL_CONV);

	glutInit(&argc, argv);  // Standard glut initialization.

	// Generate the main image name to use.
	ilGenImages(1, &ImgId);
	// Bind this image name.
	ilBindImage(ImgId);
	// Loads the image specified by File into the ImgId image.
	ilLoadImage(FileName);

	// Make sure the window is in the same proportions as the image.
	//  Generate the appropriate width x height less than or equal to MAX_X x MAX_Y.
	//	Instead of just clipping Width x Height to MAX_X x MAX_Y, we scale to
	//	an appropriate size, so the image isn't stretched/squished.
	Width  = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	if (Width > 0) {  // Don't want a divide by 0...
		if (Width > MAX_X) {
			Width = MAX_X;
			Height = (ILuint)(MAX_X / (ILfloat)ilGetInteger(IL_IMAGE_WIDTH) * Height);
		}
	}
	if (Height > 0) {  // Don't want a divide by 0...
		if (Height > MAX_Y) {
			Height = MAX_Y;
			Width = (ILuint)(MAX_Y / (ILfloat)ilGetInteger(IL_IMAGE_HEIGHT) * Width);
		}
	}

	// Standard glut initializations.
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Width, Height);
	Window = glutCreateWindow("Developer's Image Library (DevIL) Test");
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);

	// Goes into our setup function.
	if (Setup() == IL_FALSE)
		return 1;

    // Enter the main (Free)GLUT processing loop
    glutMainLoop();

	// Clean up any loose ends.
	CleanUp();

    return 0;
}


//
// Standard glut resize function.
//
void ResizeFunc(int NewWidth, int NewHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    glViewport(0, 0, NewWidth, NewHeight);
	glOrtho(0, Width, 0, Height, -1, 1);
}


//
// Standard glut display function.
//
void DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);  // Clear the colour buffer.

	// Texture a quad with our image that fills the entire window.
	glBindTexture(GL_TEXTURE_2D, TexID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3i(Width, 0, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3i(Width, Height, 0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3i(0, Height, 0);
	glEnd();

	glutSwapBuffers();  // We use double buffering, so swap the buffers.
}


//
// Standard glut idle function
//
void IdleFunc()
{
	glutShowWindow();
	glutPostRedisplay();
}


//
// Any keypress closes the window - standard glut keypress function.
//
void KeyboardFunc(unsigned char cChar, int nMouseX, int nMouseY)
{
	CleanUp();  // Clean up OpenGL.
	glutDestroyWindow(Window);  // Destroy the window.
#ifndef _WIN32
	// Must use this with regular glut, since it never returns control to main().
	exit(0);
#endif
}


//
// Setup OpenGL to use our image.
//
ILboolean Setup()
{
	glEnable(GL_TEXTURE_2D);  // Enable texturing.
	glMatrixMode(GL_PROJECTION);  // We want to use the projection matrix.
	glLoadIdentity();  // Loads the identity matrix into the current matrix.

	// Lets OpenILUT know to use its OpenGL functions.
	ilutRenderer(ILUT_OPENGL);
	// Goes through all steps of sending the image to OpenGL.
	TexID = ilutGLBindTexImage();
	// We're done with our image, so we go ahead and delete it.
	ilDeleteImages(1, &ImgId);

	glOrtho(0, Width, 0, Height, -1, 1);  // Set up an orthographic projection with OpenGL.

	return IL_TRUE;
}


//
// Cleans up any loose ends.
//
void CleanUp()
{
	if (!bCleaned)  // Can only delete the texture once.
		glDeleteTextures(1, &TexID);  // Delete our OpenGL texture.
	bCleaned = IL_TRUE;  // Want to make sure we only delete it once.
}
