#ifndef MACOSX
#define MACOSX 1
#endif

#undef ILUT_USE_OPENGL
#define IL_USE_JPEGLIB_UNMODIFIED

// to disable any of the image formats you should define it to 1
// #define IL_NO_BMP 1 and delete the line #undef IL_NO_BMP
#undef IL_NO_BMP
#undef IL_NO_CUT
#undef IL_NO_DCX
#undef IL_NO_DDS
#undef IL_NO_DOOM
#undef IL_NO_GIF
#undef IL_NO_ICO
#undef IL_NO_JPG
#undef IL_NO_LCMS
#undef IL_NO_LIF
#undef IL_NO_MDL
#undef IL_NO_MNG
#undef IL_NO_PCD
#undef IL_NO_PCX
#undef IL_NO_PIC
#undef IL_NO_PIX
#undef IL_NO_PNG
#undef IL_NO_PNM
#undef IL_NO_PSD
#undef IL_NO_PSP
#undef IL_NO_PXR
#undef IL_NO_RAW
#undef IL_NO_SGI
#undef IL_NO_TGA
#undef IL_NO_TIF
#undef IL_NO_WAL
#undef IL_NO_XPM
#undef IL_USE_JPEGLIB_UNMODIFIEDs


// will be supported only when will be no need of external libraries
// #define ALTIVEC 1 

// No Code written
// #define PPC_ASM 1