//
//  Little cms
//  Copyright (C) 1998-2003 Marti Maria
//
// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software 
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//     Input/Output


#include "lcms.h"
#include <time.h>

cmsHPROFILE   LCMSEXPORT cmsOpenProfileFromFile(const char *ICCProfile, const char *sAccess);
cmsHPROFILE   LCMSEXPORT cmsOpenProfileFromMem(LPVOID MemPtr, DWORD dwSize);
cmsHPROFILE   LCMSEXPORT cmsCreateRGBProfile(LPcmsCIExyY WhitePoint,
                                        LPcmsCIExyYTRIPLE Primaries,
                                        LPGAMMATABLE TransferFunction[3]);

cmsHPROFILE   LCMSEXPORT cmsCreateGrayProfile(LPcmsCIExyY WhitePoint,
                                              LPGAMMATABLE TransferFunction);

BOOL          LCMSEXPORT cmsCloseProfile(cmsHPROFILE hProfile);

LPLUT         LCMSEXPORT cmsReadICCLut(cmsHPROFILE hProfile, icTagSignature sig);
LPGAMMATABLE  LCMSEXPORT cmsReadICCGamma(cmsHPROFILE hProfile, icTagSignature sig);
LPGAMMATABLE  LCMSEXPORT cmsReadICCGammaReversed(cmsHPROFILE hProfile, icTagSignature sig);
int           cdecl      cmsReadICCnamedColorList(cmsHTRANSFORM xform, cmsHPROFILE hProfile, icTagSignature sig);

BOOL          LCMSEXPORT cmsTakeMediaWhitePoint(LPcmsCIEXYZ Dest, cmsHPROFILE hProfile);
BOOL          LCMSEXPORT cmsTakeMediaBlackPoint(LPcmsCIEXYZ Dest, cmsHPROFILE hProfile);
BOOL          LCMSEXPORT cmsTakeIluminant(LPcmsCIEXYZ Dest, cmsHPROFILE hProfile);
BOOL          LCMSEXPORT cmsTakeColorants(LPcmsCIEXYZTRIPLE Dest, cmsHPROFILE hProfile);
BOOL          LCMSEXPORT cmsIsTag(cmsHPROFILE hProfile, icTagSignature sig);

const char*   LCMSEXPORT cmsTakeManufacturer(cmsHPROFILE hProfile);
const char*   LCMSEXPORT cmsTakeModel(cmsHPROFILE hProfile);

void          LCMSEXPORT cmsSetProfileID(cmsHPROFILE hProfile, LPBYTE ProfileID);
const LPBYTE  LCMSEXPORT cmsTakeProfileID(cmsHPROFILE hProfile);

const char*   LCMSEXPORT cmsTakeProductName(cmsHPROFILE hProfile);

icColorSpaceSignature   LCMSEXPORT cmsGetPCS(cmsHPROFILE hProfile);
icColorSpaceSignature   LCMSEXPORT cmsGetColorSpace(cmsHPROFILE hProfile);
icProfileClassSignature LCMSEXPORT cmsGetDeviceClass(cmsHPROFILE hProfile);


BOOL LCMSEXPORT _cmsAddLUTTag(cmsHPROFILE hProfile, icTagSignature sig, LPVOID lut);
BOOL LCMSEXPORT _cmsAddGammaTag(cmsHPROFILE hProfile, icTagSignature sig, LPGAMMATABLE TransferFunction);
BOOL LCMSEXPORT _cmsAddChromaticityTag(cmsHPROFILE hProfile, icTagSignature sig, LPcmsCIExyYTRIPLE Chrm);

BOOL LCMSEXPORT cmsAddTag(cmsHPROFILE hProfile, icTagSignature sig, LPVOID Tag);
BOOL LCMSEXPORT _cmsSaveProfile(cmsHPROFILE hProfile, const char* FileName);
BOOL LCMSEXPORT _cmsSaveProfileToMem(cmsHPROFILE hProfile, LPVOID MemPtr, size_t* BytesSaved);

// ------------------- implementation -----------------------------------


#define ALIGNLONG(x) (((x)+3) & ~(3))         // Aligns to DWORD boundary

static size_t UsedSpace;            // Using this makes writting of profiles non-reentrant!
static int    GlobalLanguageCode;   // Language & country descriptors, for ICC 4.0 support
static int    GlobalCountryCode;

                           
#ifdef __BEOS__
#       define USE_CUSTOM_SWAB  1
#endif


#ifdef USE_CUSTOM_SWAB

// Replacement to swab function, thanks to YNOP
// for providing the BeOS port
//
// from: @(#)swab.c       5.10 (Berkeley)  3/6/91

static
void xswab(const void *from, void *to, size_t len)
{
         register unsigned long temp;
         register int n;
         register char *fp, *tp;

         n = (len >> 1) + 1;
         fp = (char *)from;
         tp = (char *)to;
#define STEP    temp = *fp++,*tp++ = *fp++,*tp++ = temp
         /* round to multiple of 8 */
         while ((--n) & 07)
                 STEP;
         n >>= 3;
         while (--n >= 0) {

                 STEP; STEP; STEP; STEP;
                 STEP; STEP; STEP; STEP;
         }
#undef STEP
}
#else
#define xswab swab
#endif


//
//      Little-Endian to Big-Endian
//

#ifdef USE_BIG_ENDIAN
#define AdjustEndianess16(a)
#define AdjustEndianess32(a)
#define AdjustEndianessArray16(a, b)
#else

static
void AdjustEndianess16(LPBYTE pByte)
{
       BYTE tmp;

       tmp = pByte[0];
       pByte[0] = pByte[1];
       pByte[1] = tmp;
}

static
void AdjustEndianess32(LPBYTE pByte)
{
        BYTE temp1;
        BYTE temp2;

        temp1 = *pByte++;
        temp2 = *pByte++;
        *(pByte-1) = *pByte;
        *pByte++ = temp2;
        *(pByte-3) = *pByte;
        *pByte = temp1;
}


// swap bytes in a array of words

static
void AdjustEndianessArray16(LPWORD p, size_t num_words)
{
       xswab((char*) p, (char*)p, (int) num_words * sizeof(WORD));
}

#endif


// Fixed point conversion

static
double Convert8Fixed8(WORD fixed8)
{
       BYTE msb, lsb;

       lsb = (BYTE) (fixed8 & 0xff);
       msb = (BYTE) (((WORD) fixed8 >> 8) & 0xff);

       return (double) ((double) msb + ((double) lsb / 256.0));
}

static
double Convert15Fixed16(icS15Fixed16Number fix32)
{
    double floater, sign, mid, hack;
    int Whole, FracPart;


    AdjustEndianess32((LPBYTE) &fix32);

    sign  = (fix32 < 0 ? -1 : 1);
    fix32 = abs(fix32);

    Whole = LOWORD(fix32 >> 16);
    FracPart  = LOWORD(fix32 & 0x0000ffffL);

    hack    = 65536.0;
    mid     = (double) FracPart / hack;
    floater = (double) Whole + mid;

    return sign * floater;
}


// Memory-based I/O ---------------------------------------------------

typedef struct {
                LPBYTE Block;       // Points to allocated memory
                size_t Size;        // Size of allocated memory
                int Pointer;        // Points to current location

                } FILEMEM;

static
LPVOID MemoryOpen(LPBYTE Block, size_t Size, char Mode)
{
    FILEMEM* fm = (FILEMEM*) malloc(sizeof(FILEMEM));
    ZeroMemory(fm, sizeof(FILEMEM));

    if (Mode == 'r') {

        fm ->Block   = (LPBYTE) malloc(Size);
        if (fm ->Block == NULL) {
            free(fm);
            return NULL;
        }
    

    CopyMemory(fm->Block, Block, Size);
    }
    else fm ->Block = NULL;

    fm ->Size    = Size;
    fm ->Pointer = 0;

    return (LPVOID) fm;
}


static
size_t MemoryRead(LPVOID buffer, size_t size, size_t count, LPVOID f)
{   
     FILEMEM* ResData = (FILEMEM*) f;
     LPBYTE Ptr;
     size_t len = size * count;
     

    if (ResData -> Pointer + len > ResData -> Size)
            len = (ResData -> Size - ResData -> Pointer);

    Ptr  = ResData -> Block;
    Ptr += ResData -> Pointer;
    CopyMemory(buffer, Ptr, len);        
    ResData -> Pointer += (int) len;

    return count;
}

// SEEK_CUR is assumed

static
BOOL MemorySeek(LPVOID f, size_t offset)
{
    FILEMEM* ResData = (FILEMEM*) f;

    ResData ->Pointer = (DWORD) offset; 
    return FALSE; 
}

// FTell

static
size_t MemoryTell(LPVOID f)
{
    FILEMEM* ResData = (FILEMEM*) f;

    return ResData -> Pointer;     
}


// Writes data to memory, also keeps used space for further reference

static
BOOL MemoryWrite(void *OutStream, size_t size, void *Ptr)
{
        FILEMEM* ResData = (FILEMEM*) OutStream;

       if (size == 0) return TRUE;
       
       if (ResData != NULL) 
           CopyMemory(ResData ->Block + UsedSpace, Ptr, size);

       UsedSpace += size;    

       return TRUE;
}


static
BOOL MemoryClose(LPVOID f)
{
    FILEMEM* ResData = (FILEMEM*) f;

    if (ResData ->Block) free(ResData ->Block);
    free(ResData);
    return 0;
}


// File-based I/O -------------------------------------------------------

static
LPVOID FileOpen(const char* filename)
{
    return (void*) fopen(filename, "rb");
}

static
size_t FileRead(void *buffer, size_t size, size_t count, LPVOID stream)
{
    size_t nReaded = fread(buffer, size, count, (FILE*) stream);
    if (nReaded != count) {
            cmsSignalError(LCMS_ERRC_WARNING, "Read error. Got %d bytes, block should be of %d bytes", nReaded * size, count * size);         
    }

    return nReaded;
}


static
BOOL FileSeek(LPVOID stream, size_t offset)
{
    if (fseek((FILE*) stream, (long) offset, SEEK_SET) != 0) {

       cmsSignalError(LCMS_ERRC_ABORTED, "Seek error; probably corrupted file");
       return TRUE;
    }

    return FALSE;
}


static
size_t FileTell(LPVOID stream)
{    
    return ftell((FILE*) stream);
}

// Writes data to stream, also keeps used space for further reference


static
BOOL FileWrite(void* OutStream, size_t size, LPVOID Ptr)
{
       if (size == 0) return TRUE;
       
       UsedSpace += size;

       if (OutStream == NULL) {

              return TRUE;
       }

       return (fwrite(Ptr, size, 1, (FILE*) OutStream) == 1);
}


static
BOOL FileClose(LPVOID stream)
{
    return fclose((FILE*) stream);
}

// ----------------------------------------------------------------------------------------------------


// Jun-21-2000: Some profiles (those that comes with W2K) comes
// with the media white (media black?) x 100. Add a sanity check

static
void NormalizeXYZ(LPcmsCIEXYZ Dest)
{
    while (Dest -> X > 2. &&
           Dest -> Y > 2. &&
           Dest -> Z > 2.) {

               Dest -> X /= 10.;
               Dest -> Y /= 10.;
               Dest -> Z /= 10.;
       }
}

// Evaluates a XYZ tristimulous across chromatic adaptation matrix

static
void EvalCHRM(LPcmsCIEXYZ Dest, LPMAT3 Chrm, LPcmsCIEXYZ Src)
{
    VEC3 d, s;

    s.n[VX] = Src -> X;
    s.n[VY] = Src -> Y;
    s.n[VZ] = Src -> Z;

    MAT3eval(&d, Chrm, &s);

    Dest ->X = d.n[VX];
    Dest ->Y = d.n[VY];
    Dest ->Z = d.n[VZ];

}


// ----------------------------------------------------------------------------------------------------


// Allocate ICC struct. I/O routines are passed through

static
LPLCMSICCPROFILE ICCAllocStruct( 
                                size_t (* Read)(void *buffer, size_t size, size_t count, LPVOID stream),                        
                                BOOL   (* Seek)(LPVOID stream, size_t offset),
                                size_t (* Tell)(LPVOID stream),
                                BOOL   (* Close)(LPVOID stream))

{
    LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) malloc(sizeof(LCMSICCPROFILE));
    if (Icc == NULL) return NULL;

    ZeroMemory(Icc, sizeof(LCMSICCPROFILE));

    Icc ->Read  = Read; 
    Icc ->Seek  = Seek;
    Icc ->Tell  = Tell;
    Icc ->Close = Close;
    Icc ->Write = NULL;
    
    Icc -> Illuminant.X    = D50X;
    Icc -> Illuminant.Y    = D50Y;
    Icc -> Illuminant.Z    = D50Z;

    Icc -> TagCount   = 0;

    return Icc;
}


cmsHPROFILE _cmsCreateProfilePlaceholder(void)
{
    return (cmsHPROFILE) ICCAllocStruct(NULL, NULL, NULL, NULL);
}


// Does create & init a struct holding profile directory and some aids

static
LPLCMSICCPROFILE CreateICCProfileHandler(LPVOID ICCfile,
                             size_t (* Read)(void *buffer, size_t size, size_t count, void *stream),                            
                             BOOL   (* Seek)(LPVOID stream, size_t offset),
                             size_t (* Tell)(LPVOID stream),
                             BOOL   (* Close)(LPVOID stream))
{
       LPLCMSICCPROFILE Icc;
       icTag Tag;
       icHeader Header;
       icInt32Number TagCount, i;
       
       Icc = ICCAllocStruct(Read, Seek, Tell, Close);
       if (Icc == NULL) return NULL;

       Icc -> stream = ICCfile;
       Icc -> Read(&Header, sizeof(icHeader), 1, ICCfile);

       // Convert endian

       AdjustEndianess32((LPBYTE) &Header.size);
       AdjustEndianess32((LPBYTE) &Header.cmmId);
       AdjustEndianess32((LPBYTE) &Header.version);
       AdjustEndianess32((LPBYTE) &Header.deviceClass);
       AdjustEndianess32((LPBYTE) &Header.colorSpace);
       AdjustEndianess32((LPBYTE) &Header.pcs);
       AdjustEndianess32((LPBYTE) &Header.magic);
       AdjustEndianess32((LPBYTE) &Header.flags);
       AdjustEndianess32((LPBYTE) &Header.renderingIntent);

       // Validate it

       if (Header.magic != icMagicNumber) goto ErrorCleanup;
             
      
       if (Icc ->Read(&TagCount, sizeof(icInt32Number), 1, ICCfile) != 1)
                     goto ErrorCleanup;

       AdjustEndianess32((LPBYTE) &TagCount);

       Icc -> DeviceClass     = Header.deviceClass;
       Icc -> ColorSpace      = Header.colorSpace;
       Icc -> PCS             = Header.pcs;
       Icc -> RenderingIntent = (icRenderingIntent) Header.renderingIntent;
       Icc -> flags           = Header.flags;
       Icc -> Illuminant.X    = Convert15Fixed16(Header.illuminant.X);
       Icc -> Illuminant.Y    = Convert15Fixed16(Header.illuminant.Y);
       Icc -> Illuminant.Z    = Convert15Fixed16(Header.illuminant.Z);
       Icc -> Version         = Header.version;
       
       // Fix illuminant, some profiles are broken in this field!
       
       Icc ->Illuminant = *cmsD50_XYZ();

       // The profile ID are 16 raw bytes

       CopyMemory(Icc ->ProfileID, Header.reserved, 16);

       // Get rid of possible wrong profiles

       NormalizeXYZ(&Icc  -> Illuminant);

       // Read tag directory

       Icc -> TagCount   = TagCount;
       for (i=0; i < TagCount; i++)
       {
              Icc ->Read(&Tag, sizeof(icTag), 1, ICCfile);

              AdjustEndianess32((LPBYTE) &Tag.offset);
              AdjustEndianess32((LPBYTE) &Tag.size);
              AdjustEndianess32((LPBYTE) &Tag.sig);            // Signature

              Icc -> TagNames[i]   = Tag.sig;
              Icc -> TagOffsets[i] = Tag.offset;
              Icc -> TagSizes[i]   = Tag.size;
       }
       
       return Icc;


ErrorCleanup:

              Icc ->Close(ICCfile);
              free(Icc);
              return NULL;
}


// Does search for a specific tag in tag dictionary
// Returns position or -1 if tag not found

static
icInt32Number  SearchTag(LPLCMSICCPROFILE Profile, icTagSignature sig)
{
       icInt32Number i;

       if (sig == 0) return -1;     // 0 identifies a special tag holding raw memory.

       for (i=0; i < Profile -> TagCount; i++)
       {
              if (sig == Profile -> TagNames[i])
                            return i;
       }

       return -1;
}


// Search for a particular tag, replace if found or add new one else

static
LPVOID InitTag(LPLCMSICCPROFILE Icc, icTagSignature sig, size_t size, const LPVOID Init)
{
       LPVOID Ptr;
       icInt32Number i;

       i = SearchTag(Icc, sig);
       if (i >=0) {

              if (Icc -> TagPtrs[i]) free(Icc -> TagPtrs[i]);
       }
       else  {
             i = Icc -> TagCount;
             Icc -> TagCount++;
             }


       Ptr = malloc(size);
       CopyMemory(Ptr, Init, size);

       Icc ->TagNames[i] = sig;
       Icc ->TagSizes[i] = size;
       Icc ->TagPtrs[i]  = Ptr;

       return Ptr;
}





static
int SizeOfGammaTab(LPGAMMATABLE In)
{
       return sizeof(GAMMATABLE) + (In -> nEntries - 1)*sizeof(WORD);
}



static
unsigned int uipow(unsigned int a, unsigned int b) {
        unsigned int rv = 1;
        for (; b > 0; b--)
                rv *= a;
        return rv;
}



// Convert between notations.

#define TO16_TAB(x)      (WORD) (((x) << 8) | (x))


// LUT8 can come only in Lab space. There is a fatal flaw in
// converting from Lut8 to Lut16. Due to particular encoding 
// of Lab, different actions should be taken from input and 
// output Lab8 LUTS. For input, is as easy as applying a << 8,
// since numbers comes in fixed point. However, for output LUT
// things goes a bit more complex.... LUT 16 is supposed to
// have a domain of 0..ff00, so we should remap the LUT in order
// to get things working. Affected signatures are B2Axx tags, 
// preview and gamut.

// I do solve it by multiplying input matrix by:
//  
//  | 0xffff/0xff00   0                0              |
//  |       0         0xffff/0xff00    0              |
//  |       0         0                0xffff/0xff00  |
//
// The input values got then remapped to adequate domain

static
void FixLUT8(LPLUT Lut, icTagSignature sig, size_t nTabSize)
{   
    MAT3 Fixup, Original, Result;
    LPWORD PtrW;
    size_t i;

    switch (sig) {
 

       case icSigBToA0Tag:
       case icSigBToA1Tag:
       case icSigBToA2Tag:
       case icSigGamutTag:
       case icSigPreview0Tag:
       case icSigPreview1Tag:
       case icSigPreview2Tag: 
               
           
                VEC3init(&Fixup.v[0], (double) 0xFFFF/0xFF00, 0, 0);
                VEC3init(&Fixup.v[1], 0, (double) 0xFFFF/0xFF00, 0);
                VEC3init(&Fixup.v[2], 0, 0, (double) 0xFFFF/0xFF00);
                          

                MAT3fromFix(&Original, &Lut->Matrix);               
                MAT3per(&Result, &Original, &Fixup);
                MAT3toFix(&Lut->Matrix, &Result);

                Lut -> wFlags |= LUT_HASMATRIX;
                break;

       // For input, clear low part since this has to be
       // Lab in fixed point

       default:        

                PtrW = Lut -> T;
                for (i = 0; i < nTabSize; i++) {

                             *PtrW++ &= 0xFF00;
                }
    }
    
}

// The infamous LUT 8

static
void ReadLUT8(LPLCMSICCPROFILE Icc, LPLUT NewLUT, icTagSignature sig)
{
    icLut8 LUT8;
    LPBYTE Temp;
    size_t nTabSize;
    unsigned int i, j;
    unsigned int AllLinear;
    LPWORD PtrW;

       Icc ->Read(&LUT8, sizeof(icLut8) - SIZEOF_UINT8_ALIGNED, 1, Icc -> stream);
       
       NewLUT -> wFlags        = LUT_HASTL1|LUT_HASTL2|LUT_HAS3DGRID;
       NewLUT -> cLutPoints    = LUT8.clutPoints;
       NewLUT -> InputChan     = LUT8.inputChan;
       NewLUT -> OutputChan    = LUT8.outputChan;
       NewLUT -> InputEntries  = 256;
       NewLUT -> OutputEntries = 256;


       AdjustEndianess32((LPBYTE) &LUT8.e00);
       AdjustEndianess32((LPBYTE) &LUT8.e01);
       AdjustEndianess32((LPBYTE) &LUT8.e02);
       AdjustEndianess32((LPBYTE) &LUT8.e10);
       AdjustEndianess32((LPBYTE) &LUT8.e11);
       AdjustEndianess32((LPBYTE) &LUT8.e12);
       AdjustEndianess32((LPBYTE) &LUT8.e20);
       AdjustEndianess32((LPBYTE) &LUT8.e21);
       AdjustEndianess32((LPBYTE) &LUT8.e22);


       // Matrix handling

       NewLUT -> Matrix.v[0].n[0] = (Fixed32) LUT8.e00;
       NewLUT -> Matrix.v[0].n[1] = (Fixed32) LUT8.e01;
       NewLUT -> Matrix.v[0].n[2] = (Fixed32) LUT8.e02;
       NewLUT -> Matrix.v[1].n[0] = (Fixed32) LUT8.e10;
       NewLUT -> Matrix.v[1].n[1] = (Fixed32) LUT8.e11;
       NewLUT -> Matrix.v[1].n[2] = (Fixed32) LUT8.e12;
       NewLUT -> Matrix.v[2].n[0] = (Fixed32) LUT8.e20;
       NewLUT -> Matrix.v[2].n[1] = (Fixed32) LUT8.e21;
       NewLUT -> Matrix.v[2].n[2] = (Fixed32) LUT8.e22;


       // Only operates if not identity...

       if (!MAT3isIdentity(&NewLUT -> Matrix, 0.0001)) {

              NewLUT -> wFlags |= LUT_HASMATRIX;
       }


       // Copy input tables

       Temp = (LPBYTE) malloc(256);
       AllLinear = 0;
       for (i=0; i < NewLUT -> InputChan; i++) {

              PtrW = (LPWORD) malloc(sizeof(WORD) * 256);
              NewLUT -> L1[i] = PtrW;
              Icc ->Read(Temp, 1, 256, Icc -> stream);
              for (j=0; j < 256; j++)
                     PtrW[j] = TO16_TAB(Temp[j]);
                     AllLinear += cmsIsLinear(NewLUT -> L1[i], NewLUT -> InputEntries);
              }

       // Linear input, so ignore full step

       if (AllLinear == NewLUT -> InputChan) {

              NewLUT -> wFlags &= ~LUT_HASTL1;
       }

       free(Temp);

       // Copy 3D CLUT

       nTabSize = (NewLUT -> OutputChan * uipow(NewLUT->cLutPoints,
                                                NewLUT->InputChan));

       if (nTabSize > 0) {

            PtrW = (LPWORD) malloc(sizeof(WORD) * nTabSize);
            Temp = (LPBYTE) malloc(nTabSize);
            Icc ->Read(Temp, 1, nTabSize, Icc -> stream);

            NewLUT -> T = PtrW;
            NewLUT -> Tsize = (unsigned int) (nTabSize * sizeof(WORD));

            for (i = 0; i < nTabSize; i++) {

                     *PtrW++ = TO16_TAB(Temp[i]);
            }
            free(Temp);
       }
       else {
           NewLUT ->T = NULL;
           NewLUT ->Tsize = 0;
           NewLUT -> wFlags &= ~LUT_HAS3DGRID;
       }



       // Copy output tables

       Temp = (LPBYTE) malloc(256);
       AllLinear = 0;
       for (i=0; i < NewLUT -> OutputChan; i++) {

              PtrW = (LPWORD) malloc(sizeof(WORD) * 256);
              NewLUT -> L2[i] = PtrW;
              Icc ->Read(Temp, 1, 256, Icc -> stream);              
              for (j=0; j < 256; j++)
                     PtrW[j] = TO16_TAB(Temp[j]);
                     AllLinear += cmsIsLinear(NewLUT -> L2[i], 256);
              }

       // Linear input, so ignore full step

       if (AllLinear == NewLUT -> OutputChan) {

              NewLUT -> wFlags &= ~LUT_HASTL2;
       }


       free(Temp);

       cmsCalcL16Params(NewLUT -> InputEntries,  &NewLUT -> In16params);
       cmsCalcL16Params(NewLUT -> OutputEntries, &NewLUT -> Out16params);
       cmsCalcCLUT16Params(NewLUT -> cLutPoints,  NewLUT -> InputChan,
                                                  NewLUT -> OutputChan,
                                                 &NewLUT -> CLut16params);
       // Fixup
       if (Icc ->PCS == icSigLabData) 
                FixLUT8(NewLUT, sig, nTabSize);

}




// Case LUT 16

static
void ReadLUT16(LPLCMSICCPROFILE Icc, LPLUT NewLUT)
{
    icLut16 LUT16;
    size_t nTabSize;
    unsigned int i;
    unsigned int AllLinear;
    LPWORD PtrW;


       Icc ->Read(&LUT16, sizeof(icLut16)- SIZEOF_UINT16_ALIGNED, 1, Icc -> stream);

       NewLUT -> wFlags        = LUT_HASTL1 | LUT_HASTL2 | LUT_HAS3DGRID;
       NewLUT -> cLutPoints    = LUT16.clutPoints;
       NewLUT -> InputChan     = LUT16.inputChan;
       NewLUT -> OutputChan    = LUT16.outputChan;

       AdjustEndianess16((LPBYTE) &LUT16.inputEnt);
       AdjustEndianess16((LPBYTE) &LUT16.outputEnt);

       NewLUT -> InputEntries  = LUT16.inputEnt;
       NewLUT -> OutputEntries = LUT16.outputEnt;


       // Matrix handling

       AdjustEndianess32((LPBYTE) &LUT16.e00);
       AdjustEndianess32((LPBYTE) &LUT16.e01);
       AdjustEndianess32((LPBYTE) &LUT16.e02);
       AdjustEndianess32((LPBYTE) &LUT16.e10);
       AdjustEndianess32((LPBYTE) &LUT16.e11);
       AdjustEndianess32((LPBYTE) &LUT16.e12);
       AdjustEndianess32((LPBYTE) &LUT16.e20);
       AdjustEndianess32((LPBYTE) &LUT16.e21);
       AdjustEndianess32((LPBYTE) &LUT16.e22);

       NewLUT -> Matrix.v[0].n[0] = (Fixed32) LUT16.e00;
       NewLUT -> Matrix.v[0].n[1] = (Fixed32) LUT16.e01;
       NewLUT -> Matrix.v[0].n[2] = (Fixed32) LUT16.e02;
       NewLUT -> Matrix.v[1].n[0] = (Fixed32) LUT16.e10;
       NewLUT -> Matrix.v[1].n[1] = (Fixed32) LUT16.e11;
       NewLUT -> Matrix.v[1].n[2] = (Fixed32) LUT16.e12;
       NewLUT -> Matrix.v[2].n[0] = (Fixed32) LUT16.e20;
       NewLUT -> Matrix.v[2].n[1] = (Fixed32) LUT16.e21;
       NewLUT -> Matrix.v[2].n[2] = (Fixed32) LUT16.e22;

       // Only operates if not identity...

       if (!MAT3isIdentity(&NewLUT -> Matrix, 0.0001)) {

              NewLUT -> wFlags |= LUT_HASMATRIX;
       }


       // Copy input tables

       AllLinear = 0;
       for (i=0; i < NewLUT -> InputChan; i++) {

              PtrW = (LPWORD) malloc(sizeof(WORD) * NewLUT -> InputEntries);
              NewLUT -> L1[i] = PtrW;
              Icc ->Read(PtrW, sizeof(WORD), NewLUT -> InputEntries, Icc -> stream);
              AdjustEndianessArray16(PtrW, NewLUT -> InputEntries);
              AllLinear += cmsIsLinear(NewLUT -> L1[i], NewLUT -> InputEntries);
              }

       // Linear input, so ignore full step

       if (AllLinear == NewLUT -> InputChan) {

              NewLUT -> wFlags &= ~LUT_HASTL1;
       }


       // Copy 3D CLUT

       nTabSize = (NewLUT -> OutputChan * uipow(NewLUT->cLutPoints,
                                                NewLUT->InputChan));
       if (nTabSize > 0) {

           PtrW = (LPWORD) malloc(sizeof(WORD) * nTabSize);

           NewLUT -> T = PtrW;
           NewLUT -> Tsize = (unsigned int) (nTabSize * sizeof(WORD));

           Icc -> Read(PtrW, sizeof(WORD), nTabSize, Icc -> stream);
           AdjustEndianessArray16(NewLUT -> T, nTabSize);
       }
       else {
           NewLUT ->T = NULL;
           NewLUT ->Tsize = 0;
           NewLUT -> wFlags &= ~LUT_HAS3DGRID;
       }

       // Copy output tables

       AllLinear = 0;
       for (i=0; i < NewLUT -> OutputChan; i++) {

              PtrW = (LPWORD) malloc(sizeof(WORD) * NewLUT -> OutputEntries);
              NewLUT -> L2[i] = PtrW;
              Icc ->Read(PtrW, sizeof(WORD), NewLUT -> OutputEntries, Icc -> stream);
              AdjustEndianessArray16(PtrW, NewLUT -> OutputEntries);
              AllLinear += cmsIsLinear(NewLUT -> L2[i], NewLUT -> OutputEntries);
              }

       // Linear output, ignore step

       if (AllLinear == NewLUT -> OutputChan)
       {
              NewLUT -> wFlags &= ~LUT_HASTL2;
       }


       cmsCalcL16Params(NewLUT -> InputEntries,  &NewLUT -> In16params);
       cmsCalcL16Params(NewLUT -> OutputEntries, &NewLUT -> Out16params);
       cmsCalcCLUT16Params(NewLUT -> cLutPoints,  NewLUT -> InputChan,
                                                  NewLUT -> OutputChan,
                                                  &NewLUT -> CLut16params);
}


// This is a shared routine for reading curves. It can handle v2 curves
// as linear, single gamma and table-based as well as v4 parametric curves.

static
LPGAMMATABLE ReadCurve(LPLCMSICCPROFILE  Icc)
{    
    icUInt32Number  Count;
    LPGAMMATABLE    NewGamma;
    icTagBase       Base;
    int             n;

       Icc ->Read(&Base, 1, sizeof(icTagBase), Icc -> stream);
       AdjustEndianess32((LPBYTE) &Base.sig);


       switch (Base.sig) {


       case 0x9478ee00L:    // Monaco 2 profiler is BROKEN!
       case icSigCurveType:

           Icc ->Read(&Count, sizeof(icUInt32Number), 1, Icc -> stream);
           AdjustEndianess32((LPBYTE) &Count);


           switch (Count) {

           case 0:   // Linear.

                     NewGamma = cmsAllocGamma(2);
                     if (!NewGamma) return NULL;
                     NewGamma -> GammaTable[0] = 0;
                     NewGamma -> GammaTable[1] = 0xFFFF;
                     return NewGamma;

           case 1:  {
                     WORD SingleGammaFixed;

                     Icc ->Read(&SingleGammaFixed, sizeof(WORD), 1, Icc -> stream);
                     AdjustEndianess16((LPBYTE) &SingleGammaFixed);
                     return cmsBuildGamma(4096, Convert8Fixed8(SingleGammaFixed));
                     }

           default: { // Curve
                    
                     NewGamma = cmsAllocGamma(Count);
                     if (!NewGamma) return NULL;

                     Icc ->Read(NewGamma -> GammaTable, sizeof(WORD), Count, Icc -> stream);
                     
                     AdjustEndianessArray16(NewGamma -> GammaTable, Count);

                     return NewGamma;
                    }
              }
              break;

    
       // Parametric curves
       case icSigParametricCurveType: {
           
           int ParamsByType[] = { 1, 3, 4, 5, 7 };
           double Params[10];
           icS15Fixed16Number Num;
           icUInt32Number Reserved;
           icUInt16Number   Type;
           int i;
           
           Icc -> Read(&Type, sizeof(icUInt16Number), 1, Icc -> stream);
           Icc -> Read(&Reserved, sizeof(icUInt16Number), 1, Icc -> stream);
           
           AdjustEndianess16((LPBYTE) &Type);
           if (Type > 5) {

                cmsSignalError(LCMS_ERRC_ABORTED, "Unknown parametric curve type '%d' found.", Type);
                return NULL;
           }
        
          ZeroMemory(Params, 10* sizeof(double));
          n = ParamsByType[Type];

          for (i=0; i < n; i++) {
                Num = 0;
                Icc -> Read(&Num, sizeof(icS15Fixed16Number), 1, Icc -> stream);
                Params[i] = Convert15Fixed16(Num);
          }


           NewGamma = cmsBuildParametricGamma(4096, Type+1, Params);
           return NewGamma;
          }
    

       default:
              cmsSignalError(LCMS_ERRC_ABORTED, "Bad tag signature '%lx' found.", Base.sig);
              return NULL;
       }
    
}


// Similar to anterior, but curve is reversed

static
LPGAMMATABLE ReadCurveReversed(LPLCMSICCPROFILE Icc)
{
          
     icTagBase      Base;
     LPGAMMATABLE   NewGamma, ReturnGamma;
     icUInt32Number Count;
     int            n;
      
       Icc -> Read(&Base, 1, sizeof(icTagBase), Icc -> stream);
       AdjustEndianess32((LPBYTE) &Base.sig);


       switch (Base.sig) {


       case 0x9478ee00L:    // Monaco 2 profiler is BROKEN!
       case icSigCurveType:

           Icc -> Read(&Count, sizeof(icUInt32Number), 1, Icc -> stream);
           AdjustEndianess32((LPBYTE) &Count);


           switch (Count) {

           case 0:   // Linear, reverse is same.

                     NewGamma = cmsAllocGamma(2);
                     if (!NewGamma) return NULL;
                     NewGamma -> GammaTable[0] = 0;
                     NewGamma -> GammaTable[1] = 0xFFFF;
                     return NewGamma;

           case 1:  {
                     WORD SingleGammaFixed;

                     Icc -> Read(&SingleGammaFixed, sizeof(WORD), 1, Icc -> stream);
                     AdjustEndianess16((LPBYTE) &SingleGammaFixed);
                     return cmsBuildGamma(4096, 1./Convert8Fixed8(SingleGammaFixed));
                     }

           default: { // Curve. Do our best to trying to reverse the curve
                    
                     NewGamma = cmsAllocGamma(Count);
                     if (!NewGamma) return NULL;

                     Icc -> Read(NewGamma -> GammaTable, sizeof(WORD), Count, Icc -> stream);
                     
                     AdjustEndianessArray16(NewGamma -> GammaTable, Count);

                     ReturnGamma = cmsReverseGamma(Count, NewGamma);
                     cmsFreeGamma(NewGamma);

                     return ReturnGamma;
                    }
              }
              break;

    
       // Parametric curves
       case icSigParametricCurveType: {
           
           int ParamsByType[] = { 1, 3, 4, 5, 7 };
           double Params[10];
           icS15Fixed16Number Num;
           icUInt32Number Reserved;           
           icUInt16Number   Type;
           int i;


           Icc -> Read(&Type, sizeof(icUInt16Number), 1, Icc -> stream);
           Icc -> Read(&Reserved, sizeof(icUInt16Number), 1, Icc -> stream);
           
           AdjustEndianess16((LPBYTE) &Type);
           if (Type > 5) {

                cmsSignalError(LCMS_ERRC_ABORTED, "Unknown parametric curve type '%d' found.", Type);
                return NULL;
           }
        
          ZeroMemory(Params, 10* sizeof(double));
          n = ParamsByType[Type];

          for (i=0; i < n; i++) {
                Icc -> Read(&Num, sizeof(icS15Fixed16Number), 1, Icc -> stream);
                Params[i] = Convert15Fixed16(Num);
          }


           // Negative type as a mark of reversed curve
           NewGamma = cmsBuildParametricGamma(4096, -(Type+1), Params);
           return NewGamma;
          }
    

       default:
              cmsSignalError(LCMS_ERRC_ABORTED, "Bad tag signature '%lx' found.", Base.sig);
              return NULL;
       }

}


// V4 stuff. Read matrix for LutAtoB and LutBtoA

static
BOOL ReadMatrixOffset(LPLCMSICCPROFILE Icc, size_t Offset, LPLUT NewLUT, DWORD dwFlags)
{

    icS15Fixed16Number All[12];
    int i;
    MAT3 m;
    VEC3 o;

    if (Icc -> Seek(Icc -> stream, Offset)) return FALSE;

    Icc ->Read(All, sizeof(icS15Fixed16Number), 12, Icc -> stream);

    for (i=0; i < 12;  i++)
              AdjustEndianess32((LPBYTE) &All[i]);


       m.v[0].n[0] = FIXED_TO_DOUBLE((Fixed32) All[0]);
       m.v[0].n[1] = FIXED_TO_DOUBLE((Fixed32) All[1]);
       m.v[0].n[2] = FIXED_TO_DOUBLE((Fixed32) All[2]);
       m.v[1].n[0] = FIXED_TO_DOUBLE((Fixed32) All[3]);
       m.v[1].n[1] = FIXED_TO_DOUBLE((Fixed32) All[4]);
       m.v[1].n[2] = FIXED_TO_DOUBLE((Fixed32) All[5]);
       m.v[2].n[0] = FIXED_TO_DOUBLE((Fixed32) All[6]);
       m.v[2].n[1] = FIXED_TO_DOUBLE((Fixed32) All[7]);
       m.v[2].n[2] = FIXED_TO_DOUBLE((Fixed32) All[8]);

       o.n[0] = FIXED_TO_DOUBLE((Fixed32) All[9]);
       o.n[1] = FIXED_TO_DOUBLE((Fixed32) All[10]);
       o.n[2] = FIXED_TO_DOUBLE((Fixed32) All[11]);

       cmsSetMatrixLUT4(NewLUT, &m, &o, dwFlags);

       return TRUE;
}


//  V4 stuff. Read CLUT part for LutAtoB and LutBtoA

static
BOOL ReadCLUT(LPLCMSICCPROFILE Icc, size_t Offset, LPLUT NewLUT)
{

    icCLutStruct CLUT;

    if (Icc -> Seek(Icc -> stream, Offset)) return FALSE;
    Icc ->Read(&CLUT, sizeof(icCLutStruct), 1, Icc -> stream);


    cmsAlloc3DGrid(NewLUT, CLUT.gridPoints[0], NewLUT ->InputChan, 
                                               NewLUT ->OutputChan);

    // Precission can be 1 or 2 bytes

    if (CLUT.prec == 1) {

        BYTE v;
        unsigned int i;

        for (i=0; i < NewLUT->Tsize; i++) {
                Icc ->Read(&v, sizeof(BYTE), 1, Icc -> stream);
                NewLUT->T[i] = TO16_TAB(v);
        }
                
    }
    else  
        if (CLUT.prec == 2) {
        
         Icc ->Read(NewLUT ->T, sizeof(WORD), 
                    NewLUT->Tsize / sizeof(WORD), Icc -> stream);

        AdjustEndianessArray16(NewLUT ->T, NewLUT->Tsize / sizeof(WORD));
    }
    else {
        cmsSignalError(LCMS_ERRC_ABORTED, "Unknow precission of '%d'", CLUT.prec); 
        return FALSE;
    }

    return TRUE;
}

// Read a set of curves from specific offset
static
BOOL ReadSetOfCurves(LPLCMSICCPROFILE Icc, size_t Offset, LPLUT NewLUT, int nLocation)
{     
    LPGAMMATABLE Curves[MAXCHANNELS];
    unsigned int i, nCurves;

    if (Icc -> Seek(Icc -> stream, Offset)) return FALSE;
    

    if (nLocation == 1 ||  nLocation == 3) 

        nCurves = NewLUT ->InputChan;    
    else 
        nCurves = NewLUT ->OutputChan;    

    for (i=0; i < nCurves; i++) {
        Curves[i] = ReadCurve((LPLCMSICCPROFILE) Icc);                     
    }
    
    NewLUT = cmsAllocLinearTable(NewLUT, Curves, nLocation);
    
    for (i=0; i < nCurves; i++) 
        cmsFreeGamma(Curves[i]);

    return TRUE;

}

// V4 stuff. LutAtoB type 
//
//  [L1] -> [CLUT] -> [L4] -> [Mat4] -> [Ofs4] -> [L2]
//
//  Mat, Mat3, Ofs3, L3 are missing
//   L1 = A curves
//   L4 = M curves
//   L2 = B curves

static
BOOL ReadLUT_A2B(LPLCMSICCPROFILE Icc, LPLUT NewLUT, size_t BaseOffset, icTagSignature sig)
{
    icLutAtoB LUT16;
     
       Icc ->Read(&LUT16, sizeof(icLutAtoB), 1, Icc -> stream);

       NewLUT -> InputChan     = LUT16.inputChan;
       NewLUT -> OutputChan    = LUT16.outputChan;

       AdjustEndianess32((LPBYTE) &LUT16.offsetB);
       AdjustEndianess32((LPBYTE) &LUT16.offsetMat);
       AdjustEndianess32((LPBYTE) &LUT16.offsetM);
       AdjustEndianess32((LPBYTE) &LUT16.offsetC);
       AdjustEndianess32((LPBYTE) &LUT16.offsetA);
           
      
       if (LUT16.offsetB != 0)                                         
                ReadSetOfCurves(Icc, BaseOffset + LUT16.offsetB, NewLUT, 2);

       if (LUT16.offsetMat != 0)            
            ReadMatrixOffset(Icc, BaseOffset + LUT16.offsetMat, NewLUT, LUT_HASMATRIX4);
       

       if (LUT16.offsetM != 0) 
                ReadSetOfCurves(Icc, BaseOffset + LUT16.offsetM, NewLUT, 4);

       if (LUT16.offsetC != 0) 
           ReadCLUT(Icc, BaseOffset + LUT16.offsetC, NewLUT);

       if (LUT16.offsetA!= 0) 
                ReadSetOfCurves(Icc, BaseOffset + LUT16.offsetA, NewLUT, 1);

        // Convert to v2 PCS

       if (Icc ->PCS == icSigLabData) {

       switch (sig) {

       case icSigAToB0Tag:
       case icSigAToB1Tag:
       case icSigAToB2Tag:
       case icSigGamutTag:
       case icSigPreview0Tag:
       case icSigPreview1Tag:
       case icSigPreview2Tag: 

               NewLUT ->wFlags |= LUT_V4_INPUT_EMULATE_V2;
               break;

       default:;
       }
       }

       
       return TRUE;
}

// V4 stuff. LutBtoA type 

static
BOOL ReadLUT_B2A(LPLCMSICCPROFILE Icc, LPLUT NewLUT,  size_t BaseOffset, icTagSignature sig)
{ 
  icLutBtoA LUT16;
     
       Icc ->Read(&LUT16, sizeof(icLutBtoA), 1, Icc -> stream);

       NewLUT -> InputChan     = LUT16.inputChan;
       NewLUT -> OutputChan    = LUT16.outputChan;

       AdjustEndianess32((LPBYTE) &LUT16.offsetB);
       AdjustEndianess32((LPBYTE) &LUT16.offsetMat);
       AdjustEndianess32((LPBYTE) &LUT16.offsetM);
       AdjustEndianess32((LPBYTE) &LUT16.offsetC);
       AdjustEndianess32((LPBYTE) &LUT16.offsetA);
           
      
       if (LUT16.offsetB != 0)                                         
                ReadSetOfCurves(Icc, BaseOffset + LUT16.offsetB, NewLUT, 1);

       if (LUT16.offsetMat != 0)            
            ReadMatrixOffset(Icc, BaseOffset + LUT16.offsetMat, NewLUT, LUT_HASMATRIX3);
       

       if (LUT16.offsetM != 0) 
                ReadSetOfCurves(Icc, BaseOffset + LUT16.offsetM, NewLUT, 3);

       if (LUT16.offsetC != 0) 
           ReadCLUT(Icc, BaseOffset + LUT16.offsetC, NewLUT);

       if (LUT16.offsetA!= 0) 
                ReadSetOfCurves(Icc, BaseOffset + LUT16.offsetA, NewLUT, 2);
      

       // Convert to v2 PCS

       if (Icc ->PCS == icSigLabData) {

       switch (sig) {

       case icSigBToA0Tag:
       case icSigBToA1Tag:
       case icSigBToA2Tag:
       case icSigGamutTag:
       case icSigPreview0Tag:
       case icSigPreview1Tag:
       case icSigPreview2Tag: 

               NewLUT ->wFlags |= LUT_V4_OUTPUT_EMULATE_V2;
               break;

       default:;
       }
       }

       return TRUE;
}

// CLUT main reader

LPLUT LCMSEXPORT cmsReadICCLut(cmsHPROFILE hProfile, icTagSignature sig)
{

    LPLCMSICCPROFILE    Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    icTagBase           Base;
    int                 n;
    size_t              offset;
    LPLUT               NewLUT;

    n = SearchTag(Icc, sig);
    if (n < 0)
    {
    cmsSignalError(LCMS_ERRC_ABORTED, "Tag not found");
    return NULL;
    }


    // If is in memory, the LUT is already there, so throw a copy
    if (!Icc -> stream) {
                
        return cmsDupLUT((LPLUT) Icc ->TagPtrs[n]);
    }

    offset = Icc -> TagOffsets[n];


    if (Icc -> Seek(Icc -> stream, offset))
            return NULL;


    Icc ->Read(&Base, sizeof(icTagBase), 1, Icc -> stream);
    AdjustEndianess32((LPBYTE) &Base.sig);

    NewLUT = cmsAllocLUT();
    if (!NewLUT)
    {
       cmsSignalError(LCMS_ERRC_ABORTED, "cmsAllocLUT() failed");
       return NULL;
    }


    switch (Base.sig) {

    case icSigLut8Type: ReadLUT8(Icc, NewLUT, sig); break;
    case icSigLut16Type: ReadLUT16(Icc, NewLUT); break;

    case icSiglutAtoBType: ReadLUT_A2B(Icc, NewLUT, offset, sig); break;
    case icSiglutBtoAType: ReadLUT_B2A(Icc, NewLUT, offset, sig); break;

    default:  cmsSignalError(LCMS_ERRC_ABORTED, "Bad tag signature %lx found.", Base.sig);
              cmsFreeLUT(NewLUT);
              return NULL;
    }


    return NewLUT;
}


// Sets the language & country preferences. Used only in ICC 4.0 profiles

void LCMSEXPORT cmsSetLanguage(int LanguageCode, int CountryCode)
{
    GlobalLanguageCode = LanguageCode;
    GlobalCountryCode  = CountryCode;
}



// Some tags (e.g, 'pseq') can have text tags embedded. This function
// handles such special case.

static
int ReadEmbeddedTextTag(LPLCMSICCPROFILE Icc, size_t size, char* Name)
{
    icTagBase           Base;
       

    Icc ->Read(&Base, sizeof(icTagBase), 1, Icc -> stream);
    size -= sizeof(icTagBase);

    AdjustEndianess32((LPBYTE) &Base.sig);

    switch (Base.sig) {

    case icSigTextDescriptionType: {

           icUInt32Number  AsciiCount;
           icUInt32Number  i, UnicodeCode, UnicodeCount;
           icUInt16Number  ScriptCodeCode, Dummy;
           icUInt8Number   ScriptCodeCount;
           
           Icc ->Read(&AsciiCount, sizeof(icUInt32Number), 1, Icc -> stream);
           size -= sizeof(icUInt32Number);

           AdjustEndianess32((LPBYTE) &AsciiCount);
           Icc ->Read(Name, 1, AsciiCount, Icc -> stream);
           size -= AsciiCount;

           // Skip Unicode code

           Icc ->Read(&UnicodeCode,  sizeof(icUInt32Number), 1, Icc -> stream);
           size -= sizeof(icUInt32Number);

           Icc ->Read(&UnicodeCount, sizeof(icUInt32Number), 1, Icc -> stream);
           size -= sizeof(icUInt32Number);

           AdjustEndianess32((LPBYTE) &UnicodeCount);

           if (UnicodeCount > size) return (int) size;

           for (i=0; i < UnicodeCount; i++) 
                Icc ->Read(&Dummy, sizeof(icUInt16Number), 1, Icc -> stream);
           
           size -= UnicodeCount * sizeof(icUInt16Number);
            
          // Skip ScriptCode code

           Icc ->Read(&ScriptCodeCode,  sizeof(icUInt16Number), 1, Icc -> stream);
           size -= sizeof(icUInt16Number);
           Icc ->Read(&ScriptCodeCount, sizeof(icUInt8Number), 1, Icc -> stream);
           size -= sizeof(icUInt8Number);

           if (size < 67) return (int) size;

           for (i=0; i < 67; i++) 
                Icc ->Read(&Dummy, sizeof(icUInt8Number), 1, Icc -> stream);

           size -= 67;                      
           }
           break;


    case icSigCopyrightTag:   // Broken profiles from agfa does store copyright info in such type
    case icSigTextType:
    
           
           Icc -> Read(Name, 1, size, Icc -> stream);
           break;

    // multiLocalizedUnicodeType, V4 only

    case icSigMultiLocalizedUnicodeType: {

        icUInt32Number Count, RecLen;       
        icUInt16Number Language, Country;
        icUInt32Number ThisLen, ThisOffset;
        size_t         Offset = 0;
        size_t         Len    = 0;      
        size_t         i;
        wchar_t*       wchar = L"";
                    

            Icc ->Read(&Count, sizeof(icUInt32Number), 1, Icc -> stream);                   
            AdjustEndianess32((LPBYTE) &Count);
            Icc ->Read(&RecLen, sizeof(icUInt32Number), 1, Icc -> stream);                  
            AdjustEndianess32((LPBYTE) &RecLen);

            if (RecLen != 12) {

                    cmsSignalError(LCMS_ERRC_ABORTED, "multiLocalizedUnicodeType of len != 12 is not supported.");
                    return -1;
            }

            for (i=0; i < Count; i++) {
                
                Icc ->Read(&Language, sizeof(icUInt16Number), 1, Icc -> stream);                    
                AdjustEndianess16((LPBYTE) &Language);
                Icc ->Read(&Country, sizeof(icUInt16Number), 1, Icc -> stream);                 
                AdjustEndianess16((LPBYTE) &Country);
    
                Icc ->Read(&ThisLen, sizeof(icUInt32Number), 1, Icc -> stream);                 
                AdjustEndianess32((LPBYTE) &ThisLen);
    
                Icc ->Read(&ThisOffset, sizeof(icUInt32Number), 1, Icc -> stream);                  
                AdjustEndianess32((LPBYTE) &ThisOffset);
    
                if (Language == GlobalLanguageCode || Offset == 0) {

                    Len = ThisLen; Offset = ThisOffset; 
                    if (Country == GlobalCountryCode) 
                                    break;              // Found                        
                }
                        
            }
            

            if (Offset == 0) {

                    strcpy(Name, "(no info)");
                    break;
            }
            
            // Compute true offset
            Offset -= 12 * Count + 8 + sizeof(icTagBase);

            // Skip unused bytes
            for (i=0; i < Offset; i++) {
                    char Discard;

                    Icc ->Read(&Discard, 1, 1, Icc -> stream);
            }

            wchar = (wchar_t*) malloc(Len+2);
            if (!wchar) return -1;
            
            Icc ->Read(wchar, 1, Len, Icc ->stream);
            AdjustEndianessArray16((LPWORD) wchar, Len / 2);

            wchar[Len / 2] = L'\0';
            wcstombs(Name, wchar, 2047 );                       
            free((void*) wchar);
            }
            break;

    default:
              cmsSignalError(LCMS_ERRC_ABORTED, "Bad tag signature %lx found.", Base.sig);
              return -1;
    }

    return (int) size;
}


// Take an ASCII item


int LCMSEXPORT cmsReadICCText(cmsHPROFILE hProfile, icTagSignature sig, char *Name)
{
    LPLCMSICCPROFILE    Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    size_t              offset, size;
    int                 n;

    n = SearchTag(Icc, sig);
    if (n < 0) {

        cmsSignalError(LCMS_ERRC_ABORTED, "Tag not found");
        return -1;
    }

    if (!Icc -> stream) {

        CopyMemory(Name, Icc -> TagPtrs[n],
                             Icc -> TagSizes[n]);

        return (int) Icc -> TagSizes[n];
    }

    offset = Icc -> TagOffsets[n];
    size   = Icc -> TagSizes[n];

    if (Icc -> Seek(Icc ->stream, offset))
            return -1;
      
    return ReadEmbeddedTextTag(Icc, size, Name);

}



// Take an XYZ item

static
int ReadICCXYZ(cmsHPROFILE hProfile, icTagSignature sig, LPcmsCIEXYZ Value, BOOL lIsFatal)
{
    LPLCMSICCPROFILE    Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    icTagBase           Base;
    size_t              offset;
    int                 n;
    icXYZNumber         XYZ;

    n = SearchTag(Icc, sig);
    if (n < 0)
    {
    // Tag not found
    return -1;
    }


    if (!Icc -> stream) {

    CopyMemory(Value, Icc -> TagPtrs[n],
                      Icc -> TagSizes[n]);

    return (int) Icc -> TagSizes[n];
    }

    offset = Icc -> TagOffsets[n];

    if (Icc -> Seek(Icc->stream, offset))
            return -1;

    
    Icc -> Read(&Base, 1, sizeof(icTagBase), Icc -> stream);
    AdjustEndianess32((LPBYTE) &Base.sig);

    switch (Base.sig)
    {
        
    case 0x7c3b10cL:    // Some apple broken embedded profiles does not have correct type   
    case icSigXYZType:

           Icc ->Read(&XYZ, sizeof(icXYZNumber), 1, Icc -> stream);
           Value -> X = Convert15Fixed16(XYZ.X);
           Value -> Y = Convert15Fixed16(XYZ.Y);
           Value -> Z = Convert15Fixed16(XYZ.Z);
           break;

    // Aug/21-2001 - Monaco 2 does have WRONG values.

    default:
           if (lIsFatal)
                cmsSignalError(LCMS_ERRC_ABORTED, "Bad tag signature %lx found.", Base.sig);
           return -1;
    }

    return 1;
}


// Read a icSigS15Fixed16ArrayType (currently only a 3x3 matrix)           

static
int ReadICCXYZArray(cmsHPROFILE hProfile, icTagSignature sig, LPMAT3 v)
{
    LPLCMSICCPROFILE    Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    icTagBase           Base;
    size_t              offset, sz;
    int                 i, n;
    icXYZNumber         XYZ[3];
    cmsCIEXYZ           XYZdbl[3];


    n = SearchTag(Icc, sig);
    if (n < 0) {    
            return -1; // Not found
    }


    if (!Icc -> stream) {

    CopyMemory(v, Icc -> TagPtrs[n], Icc -> TagSizes[n]);

    return (int) Icc -> TagSizes[n];
    }

    offset = Icc -> TagOffsets[n];

    if (Icc -> Seek(Icc->stream, offset))
            return -1;

    
    Icc -> Read(&Base, 1, sizeof(icTagBase), Icc -> stream);
    AdjustEndianess32((LPBYTE) &Base.sig);

    switch (Base.sig) {

    case icSigS15Fixed16ArrayType:

         sz = Icc ->TagSizes[n] / sizeof(icXYZNumber);

         if (sz != 3) {
             cmsSignalError(LCMS_ERRC_ABORTED, "Bad array size of %d entries.", sz);
             return -1;
         }
                
         Icc ->Read(XYZ, sizeof(icXYZNumber), 3, Icc -> stream);

         for (i=0; i < 3; i++) {

            XYZdbl[i].X = Convert15Fixed16(XYZ[i].X);
            XYZdbl[i].Y = Convert15Fixed16(XYZ[i].Y);
            XYZdbl[i].Z = Convert15Fixed16(XYZ[i].Z);
         }

         CopyMemory(v, XYZdbl, 3*sizeof(cmsCIEXYZ));
         break;

    default:
         cmsSignalError(LCMS_ERRC_ABORTED, "Bad tag signature %lx found.", Base.sig);
         return -1;

    }

   return sizeof(MAT3);
}



// White point must be in XYZ to avoid extra calculation on
// absolute intents

BOOL LCMSEXPORT cmsTakeMediaWhitePoint(LPcmsCIEXYZ Dest, cmsHPROFILE hProfile)
{

     LPLCMSICCPROFILE    Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

     *Dest = Icc -> MediaWhitePoint;
    
      return TRUE;
}


BOOL LCMSEXPORT cmsTakeMediaBlackPoint(LPcmsCIEXYZ Dest, cmsHPROFILE hProfile)
{
      LPLCMSICCPROFILE    Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

      *Dest = Icc -> MediaBlackPoint; 
  
       return TRUE;
}

BOOL  LCMSEXPORT cmsTakeIluminant(LPcmsCIEXYZ Dest, cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       *Dest = Icc -> Illuminant;
       
       return TRUE;
}

int LCMSEXPORT cmsTakeRenderingIntent(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return (int) Icc -> RenderingIntent;
}

void LCMSEXPORT cmsSetRenderingIntent(cmsHPROFILE hProfile, int RenderingIntent)
{
    LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    Icc -> RenderingIntent = (icRenderingIntent) RenderingIntent;
}


DWORD LCMSEXPORT cmsTakeHeaderFlags(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return (DWORD) Icc -> flags;
}

void LCMSEXPORT cmsSetHeaderFlags(cmsHPROFILE hProfile, DWORD Flags)
{
    LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    Icc -> flags = (icUInt32Number) Flags;
}



// ProfileID (v4) -- take a local copy from header

const LPBYTE LCMSEXPORT cmsTakeProfileID(cmsHPROFILE hProfile)
{
    static BYTE ProfileID[16];
    LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    CopyMemory(ProfileID, Icc ->ProfileID, 16);

    return ProfileID;
}

void LCMSEXPORT cmsSetProfileID(cmsHPROFILE hProfile, LPBYTE ProfileID)
{
    LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    CopyMemory(Icc -> ProfileID, ProfileID, 16);

}



// Primaries are to be in xyY notation

BOOL LCMSEXPORT cmsTakeColorants(LPcmsCIEXYZTRIPLE Dest, cmsHPROFILE hProfile)
{
       if (ReadICCXYZ(hProfile, icSigRedColorantTag, &Dest -> Red, TRUE) < 0) return FALSE;
       if (ReadICCXYZ(hProfile, icSigGreenColorantTag, &Dest -> Green, TRUE) < 0) return FALSE;
       if (ReadICCXYZ(hProfile, icSigBlueColorantTag, &Dest -> Blue, TRUE) < 0) return FALSE;

       return TRUE;
}


BOOL cmsReadICCMatrixRGB2XYZ(LPMAT3 r, cmsHPROFILE hProfile)
{
       cmsCIEXYZTRIPLE Primaries;

       if (!cmsTakeColorants(&Primaries, hProfile)) return FALSE;

       VEC3init(&r -> v[0], Primaries.Red.X, Primaries.Green.X,  Primaries.Blue.X);
       VEC3init(&r -> v[1], Primaries.Red.Y, Primaries.Green.Y,  Primaries.Blue.Y);
       VEC3init(&r -> v[2], Primaries.Red.Z, Primaries.Green.Z,  Primaries.Blue.Z);

       return TRUE;

}


// Always return a suitable matrix

BOOL cmsReadChromaticAdaptationMatrix(LPMAT3 r, cmsHPROFILE hProfile)
{    
      
    if (ReadICCXYZArray(hProfile, icSigChromaticAdaptationTag, r) < 0) {

       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       // For display profiles, revert to bradford. Else take identity.

       MAT3identity(r);

       // Emissive devices have non-identity chad

       if ((cmsGetDeviceClass(hProfile) == icSigDisplayClass) ||
           cmsTakeHeaderFlags(hProfile) & icTransparency) {

            // NULL for cone defaults to Bradford, from media to D50
            cmsAdaptationMatrix(r, NULL, &Icc ->MediaWhitePoint, &Icc ->Illuminant);        
        }
    }
    
    return TRUE;
}



LPGAMMATABLE LCMSEXPORT cmsReadICCGamma(cmsHPROFILE hProfile, icTagSignature sig)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       size_t         offset;
       int            n;


       n = SearchTag(Icc, sig);
       if (n < 0) {

       cmsSignalError(LCMS_ERRC_ABORTED, "Tag not found");
       return NULL;
       }


       if (!Icc -> stream) {

       return cmsDupGamma((LPGAMMATABLE) Icc -> TagPtrs[n]);
       }

       offset = Icc -> TagOffsets[n];

       if (Icc -> Seek(Icc ->stream, offset))
            return NULL;

       return ReadCurve(Icc);
     
}




// Some ways have analytical revese. This function accounts for that

LPGAMMATABLE LCMSEXPORT cmsReadICCGammaReversed(cmsHPROFILE hProfile, icTagSignature sig)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       size_t         offset;
       int            n;


       n = SearchTag(Icc, sig);
       if (n < 0) {

       cmsSignalError(LCMS_ERRC_ABORTED, "Tag not found");
       return NULL;
       }


       if (!Icc -> stream) {

       return cmsReverseGamma(256, (LPGAMMATABLE) Icc -> TagPtrs[n]);
       }

       offset = Icc -> TagOffsets[n];

       if (Icc -> Seek(Icc ->stream, offset))
            return NULL;
       
       return ReadCurveReversed(Icc);       
}

// Check Named color header

static
BOOL CheckHeader(LPcmsNAMEDCOLORLIST v, icNamedColor2* nc2)
{
    if (v ->Prefix[0] == 0 && v ->Suffix[0] == 0 && v ->ColorantCount == 0) return TRUE;

    if (stricmp(v ->Prefix, nc2 ->prefix) != 0) return FALSE;
    if (stricmp(v ->Suffix, nc2 ->suffix) != 0) return FALSE;

    return ((int) v ->ColorantCount == (int) nc2 ->nDeviceCoords);
}

// Read named color list

int cmsReadICCnamedColorList(cmsHTRANSFORM xform, cmsHPROFILE hProfile, icTagSignature sig)
{
       _LPcmsTRANSFORM v = (_LPcmsTRANSFORM) xform;

       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       int            n;
       icTagBase      Base;
       size_t         offset;
       
       n = SearchTag(Icc, sig);
       if (n < 0) {

       cmsSignalError(LCMS_ERRC_WARNING, "Named color tag not found");
       return 0;
       }

       if (!Icc -> stream) {
           
            // This replaces actual named color list.                       
            size_t size   = Icc -> TagSizes[n];  

            if (v ->NamedColorList) cmsFreeNamedColorList(v ->NamedColorList);
            v -> NamedColorList = (LPcmsNAMEDCOLORLIST) malloc(size);           
            CopyMemory(v -> NamedColorList, Icc ->TagPtrs[n], size);    
            return v ->NamedColorList->nColors;           
       }

       offset = Icc -> TagOffsets[n];

       if (Icc -> Seek(Icc ->stream, offset))
            return 0;

       Icc -> Read(&Base, 1, sizeof(icTagBase), Icc -> stream);
       AdjustEndianess32((LPBYTE) &Base.sig);

       switch (Base.sig) {

        // I never have seen one of these. Probably is not worth of implementing.

       case icSigNamedColorType: {

              cmsSignalError(LCMS_ERRC_WARNING, "Ancient named color profiles are not supported.");
              return 0;                 
            }
           
        // The named color struct
        
       case icSigNamedColor2Type: {

                icNamedColor2 nc2;
                unsigned int i, j;
                                
                Icc -> Read(&nc2, sizeof(icNamedColor2) - SIZEOF_UINT8_ALIGNED, 1, Icc -> stream);              
                AdjustEndianess32((LPBYTE) &nc2.vendorFlag);
                AdjustEndianess32((LPBYTE) &nc2.count);
                AdjustEndianess32((LPBYTE) &nc2.nDeviceCoords);

                if (!CheckHeader(v->NamedColorList, &nc2)) {
                     cmsSignalError(LCMS_ERRC_WARNING, "prefix/suffix/device for named color profiles mismatch.");
                }

                strncpy(v ->NamedColorList->Prefix, nc2.prefix, 32);
                strncpy(v ->NamedColorList->Suffix, nc2.suffix, 32);
                v ->NamedColorList->Prefix[32] = v->NamedColorList->Suffix[32] = 0;
                
                v ->NamedColorList ->ColorantCount = nc2.nDeviceCoords;
                
                for (i=0; i < nc2.count; i++) {

                    WORD PCS[3];
                    WORD Colorant[MAXCHANNELS];
                    char Root[33];

                    ZeroMemory(Colorant, sizeof(WORD) * MAXCHANNELS);
                    Icc -> Read(Root, 1, 32, Icc ->stream);
                    Icc -> Read(PCS,  3, sizeof(WORD), Icc ->stream);

                    for (j=0; j < 3; j++)
                        AdjustEndianess16((LPBYTE) &PCS[j]);
                    
                    Icc -> Read(Colorant, sizeof(WORD), nc2.nDeviceCoords, Icc ->stream);

                    for (j=0; j < nc2.nDeviceCoords; j++) 
                            AdjustEndianess16((LPBYTE) &Colorant[j]);
                    
                    cmsAppendNamedColor(v, Root, PCS, Colorant);
                }

            return v ->NamedColorList->nColors;
            }
            break;

       default:
              cmsSignalError(LCMS_ERRC_WARNING, "Bad tag signature '%lx' found.", Base.sig);
              return 0;
       }

       // It would never reach here
       // return 0;
}



icColorSpaceSignature LCMSEXPORT cmsGetPCS(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return Icc -> PCS;
}


void LCMSEXPORT cmsSetPCS(cmsHPROFILE hProfile, icColorSpaceSignature pcs)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       Icc -> PCS = pcs;
}



icColorSpaceSignature LCMSEXPORT cmsGetColorSpace(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return Icc -> ColorSpace;
}



void LCMSEXPORT cmsSetColorSpace(cmsHPROFILE hProfile, icColorSpaceSignature sig)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       Icc -> ColorSpace = sig;
}

// Check existance

BOOL LCMSEXPORT cmsIsTag(cmsHPROFILE hProfile, icTagSignature sig)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return SearchTag(Icc, sig) >= 0;

}

icProfileClassSignature LCMSEXPORT cmsGetDeviceClass(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return Icc -> DeviceClass;
}

DWORD LCMSEXPORT cmsGetProfileICCversion(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       return (DWORD) Icc -> Version;
}



void LCMSEXPORT cmsSetDeviceClass(cmsHPROFILE hProfile, icProfileClassSignature sig)
{
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       Icc -> DeviceClass = sig;
}

// Uncooked manufacturer

const char* LCMSEXPORT cmsTakeManufacturer(cmsHPROFILE hProfile)
{

    static char Manufacturer[512] = "";

       Manufacturer[0] = 0;   

       if (cmsIsTag(hProfile, icSigDeviceMfgDescTag)) {

       cmsReadICCText(hProfile, icSigDeviceMfgDescTag, Manufacturer);
       }

    return Manufacturer;
}

// Uncooked model

const char* LCMSEXPORT cmsTakeModel(cmsHPROFILE hProfile)
{

    static char Model[512] = "";

       Model[0] = 0;   

       if (cmsIsTag(hProfile, icSigDeviceModelDescTag)) {

       cmsReadICCText(hProfile, icSigDeviceModelDescTag, Model);
       }

    return Model;
}


const char* LCMSEXPORT cmsTakeCopyright(cmsHPROFILE hProfile)
{

    static char Copyright[512] = "";

       Copyright[0] = 0;   

       if (cmsIsTag(hProfile, icSigCopyrightTag)) {

       cmsReadICCText(hProfile, icSigCopyrightTag, Copyright);
       }

    return Copyright;
}


// We compute name with model - manufacturer

const char*  LCMSEXPORT cmsTakeProductName(cmsHPROFILE hProfile)
{
       static char Name[2048];
       char Manufacturer[512], Model[512];

       Name[0] = '\0';
       Manufacturer[0] = Model[0] = '\0';

       if (cmsIsTag(hProfile, icSigDeviceMfgDescTag))
       {
       cmsReadICCText(hProfile, icSigDeviceMfgDescTag, Manufacturer);
       }

       if (cmsIsTag(hProfile, icSigDeviceModelDescTag))
       {
       cmsReadICCText(hProfile, icSigDeviceModelDescTag, Model);
       }

       if (!Manufacturer[0] && !Model[0])
       {
              if (cmsIsTag(hProfile, icSigProfileDescriptionTag))
              {
              cmsReadICCText(hProfile, icSigProfileDescriptionTag, Name);
              return Name;
              }
              else return "{no name}";
       }


       if (!Manufacturer[0] || strncmp(Model, Manufacturer, 8) == 0 ||
                               strlen(Model) > 30)
              strcpy(Name, Model);
       else
              sprintf(Name, "%s - %s", Model, Manufacturer);

       return Name;

}


// We compute desc with manufacturer - model

const char*  LCMSEXPORT cmsTakeProductDesc(cmsHPROFILE hProfile)
{
       static char Name[2048];

       if (cmsIsTag(hProfile, icSigProfileDescriptionTag)) {

              cmsReadICCText(hProfile, icSigProfileDescriptionTag, Name);
       }
       else return cmsTakeProductName(hProfile);

       if (strncmp(Name, "Copyrig", 7) == 0)
              return cmsTakeProductName(hProfile);

       return Name;
}


const char*  LCMSEXPORT cmsTakeProductInfo(cmsHPROFILE hProfile)
{
       // Simply typecast
       LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       static char Info[4096];

       Info[0] = '\0';

       if (cmsIsTag(hProfile, icSigProfileDescriptionTag))
       {
       char Desc[1024];

       cmsReadICCText(hProfile, icSigProfileDescriptionTag, Desc);
       strcat(Info, Desc);
       strcat(Info, "\r\n\r\n");
       }


       if (cmsIsTag(hProfile, icSigCopyrightTag))
       {
       char Copyright[2048];

       cmsReadICCText(hProfile, icSigCopyrightTag, Copyright);
       strcat(Info, Copyright);
       strcat(Info, "\r\n\r\n");
       }



// KODAK private tag... But very useful

#define K007         (icTagSignature)0x4B303037

       // MonCal

       if (cmsIsTag(hProfile, K007))
       {
       char MonCal[1024];

       cmsReadICCText(hProfile, K007, MonCal);
       strcat(Info, MonCal);
       strcat(Info, "\r\n\r\n");
       }
       else
       {
       cmsCIEXYZ WhitePt;
       char WhiteStr[1024];

       cmsTakeMediaWhitePoint(&WhitePt, hProfile);
       _cmsIdentifyWhitePoint(WhiteStr, &WhitePt);
       strcat(WhiteStr, "\r\n\r\n");
       strcat(Info, WhiteStr);
       }


       if (Icc -> stream) {
              strcat(Info, Icc -> PhysicalFile);
       }
       return Info;
}

// Extract the target data as a big string. Does not signal if tag is not present.

BOOL LCMSEXPORT cmsTakeCharTargetData(cmsHPROFILE hProfile, char** Data, size_t* len)
{
    LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    int n;

    *Data = NULL;
    *len  = 0;

    n = SearchTag(Icc, icSigCharTargetTag);
    if (n < 0) return FALSE;                


    *len =  Icc -> TagSizes[n];
    *Data = (char*) malloc(*len + 1);

    if (!*Data) {

        cmsSignalError(LCMS_ERRC_ABORTED, "Out of memory allocating CharTarget space!");
        return FALSE;
    }

    if (cmsReadICCText(hProfile, icSigCharTargetTag, *Data) < 0) 
        return FALSE;

    (*Data)[*len] = 0;  // Force a zero marker. Shouldn't be needed, but is 
                        // here to simplify things.

    return TRUE;    
}



// PSEQ Tag, used in devicelink profiles

LPcmsSEQ LCMSEXPORT cmsReadProfileSequenceDescription(cmsHPROFILE hProfile)
{
    LPLCMSICCPROFILE  Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    int n;
    icUInt32Number i, Count;
    icDescStruct   DescStruct;
    icTagBase      Base;
    size_t         size, offset;
    LPcmsSEQ       OutSeq;


    n = SearchTag(Icc, icSigProfileSequenceDescTag);
    if (n < 0) return NULL;                

    if (!Icc -> stream) {
            
            size   = Icc -> TagSizes[n];            
            OutSeq = (LPcmsSEQ) malloc(size);           
            CopyMemory(OutSeq, Icc ->TagPtrs[n], size);    
            return OutSeq;
    }

    offset = Icc -> TagOffsets[n];
    size   = Icc -> TagSizes[n];

    if (Icc -> Seek(Icc ->stream, offset))
            return NULL;

    Icc -> Read(&Base, 1, sizeof(icTagBase), Icc -> stream);
    AdjustEndianess32((LPBYTE) &Base.sig);

    if (Base.sig != icSigProfileSequenceDescType) return 0;

    Icc ->Read(&Count, sizeof(icUInt32Number), 1, Icc -> stream);
    AdjustEndianess32((LPBYTE) &Count);
    
    size = sizeof(int) + Count * sizeof(cmsPSEQDESC);
    OutSeq = (LPcmsSEQ) malloc(size);

    OutSeq ->n = Count;
    
    // Get structures as well

    for (i=0; i < Count; i++) {
        
        LPcmsPSEQDESC sec = &OutSeq -> seq[i];

        Icc -> Read(&DescStruct, sizeof(icDescStruct) - SIZEOF_UINT8_ALIGNED, 1, Icc -> stream);

        AdjustEndianess32((LPBYTE) &DescStruct.deviceMfg);
        AdjustEndianess32((LPBYTE) &DescStruct.deviceModel);
        AdjustEndianess32((LPBYTE) &DescStruct.technology);
        AdjustEndianess32((LPBYTE) &DescStruct.attributes[0]);
        AdjustEndianess32((LPBYTE) &DescStruct.attributes[1]);

        sec ->attributes[0] = DescStruct.attributes[0];
        sec ->attributes[1] = DescStruct.attributes[1];
        sec ->deviceMfg     = DescStruct.deviceMfg;
        sec ->deviceModel   = DescStruct.deviceModel;
        sec ->technology    = DescStruct.technology;

        if (ReadEmbeddedTextTag(Icc, size, sec ->Manufacturer) < 0) return NULL;
        if (ReadEmbeddedTextTag(Icc, size, sec ->Model) < 0) return NULL;

    }

    return OutSeq;
}


// Read a few tags that are hardly required


static
void ReadCriticalTags(LPLCMSICCPROFILE Icc)
{
    cmsHPROFILE hProfile = (cmsHPROFILE) Icc;
     
    if (Icc ->Version >= 0x4000000) {

        MAT3 ChrmCanonical;

         if (ReadICCXYZ(hProfile, 
                      icSigMediaWhitePointTag, 
                      &Icc ->MediaWhitePoint, FALSE) < 0) {

              Icc ->MediaWhitePoint = *cmsD50_XYZ();              
       }

       // Read media black

       if (ReadICCXYZ(hProfile, 
                      icSigMediaBlackPointTag, 
                      &Icc ->MediaBlackPoint, FALSE) < 0) {

              Icc ->MediaBlackPoint.X = 0;
              Icc ->MediaBlackPoint.Y = 0; 
              Icc ->MediaBlackPoint.X = 0;

       }

       NormalizeXYZ(&Icc ->MediaWhitePoint);
       NormalizeXYZ(&Icc ->MediaBlackPoint);

      if (ReadICCXYZArray(hProfile, 
                                icSigChromaticAdaptationTag, 
                                &ChrmCanonical) > 0) {

                MAT3inverse(&ChrmCanonical, &Icc ->ChromaticAdaptation);
        
      }
      else {

                MAT3identity(&Icc ->ChromaticAdaptation);
      }
      

      // Convert media white, black to absolute

      EvalCHRM(&Icc ->MediaWhitePoint, &Icc ->ChromaticAdaptation, &Icc ->MediaWhitePoint);
      EvalCHRM(&Icc ->MediaBlackPoint, &Icc ->ChromaticAdaptation, &Icc ->MediaBlackPoint);
      

    }
    else {

    static  MAT3 Brfd = {{ // Bradford matrix by default
                        {{  0.8951,  0.2664, -0.1614 }},
                        {{ -0.7502,  1.7135,  0.0367 }},
                        {{  0.0389, -0.0685,  1.0296 }}
                        }};


       // Read media white

       if (ReadICCXYZ(hProfile, 
                      icSigMediaWhitePointTag, 
                      &Icc ->MediaWhitePoint, FALSE) < 0) {

              Icc ->MediaWhitePoint = *cmsD50_XYZ();              
       }

       // Read media black

       if (ReadICCXYZ(hProfile, 
                      icSigMediaBlackPointTag, 
                      &Icc ->MediaBlackPoint, FALSE) < 0) {

              Icc ->MediaBlackPoint.X = 0;
              Icc ->MediaBlackPoint.Y = 0; 
              Icc ->MediaBlackPoint.X = 0;

       }

       NormalizeXYZ(&Icc ->MediaWhitePoint);
       NormalizeXYZ(&Icc ->MediaBlackPoint);


       // Take Bradford as default

       cmsAdaptationMatrix(&Icc -> ChromaticAdaptation, 
                            &Brfd, 
                            &Icc -> Illuminant, 
                            &Icc -> MediaWhitePoint);

    }

}


// Create profile from disk file

cmsHPROFILE LCMSEXPORT cmsOpenProfileFromFile(const char *lpFileName, const char *sAccess)
{
       LPLCMSICCPROFILE NewIcc;
       LPVOID ICCfile;
       cmsHPROFILE hEmpty;
       
       if (*sAccess == 'W' || *sAccess == 'w') {

           hEmpty = _cmsCreateProfilePlaceholder();
           NewIcc = (LPLCMSICCPROFILE) (LPSTR) hEmpty;
           NewIcc -> IsWrite = TRUE;
           strncpy(NewIcc ->PhysicalFile, lpFileName, MAX_PATH-1);

           // Save LUT as 8 bit

           sAccess++;
           if (*sAccess == '8') NewIcc ->SaveAs8Bits = TRUE;

           return hEmpty;
       }


       ICCfile = FileOpen(lpFileName);
       if (!ICCfile) {
              cmsSignalError(LCMS_ERRC_ABORTED, "File '%s' not found", lpFileName);
              return NULL;  
       }

       NewIcc = CreateICCProfileHandler(ICCfile, FileRead, FileSeek, FileTell, FileClose);
       if (!NewIcc) {

              cmsSignalError(LCMS_ERRC_ABORTED, "Corrupted profile: '%s'", lpFileName);
              return NULL;  // Urecoverable
       }

       ReadCriticalTags(NewIcc);

       strncpy(NewIcc -> PhysicalFile, lpFileName, MAX_PATH-1);
       NewIcc -> IsWrite     = FALSE;

       return (cmsHPROFILE) (LPSTR) NewIcc;
}




// Open from mem

cmsHPROFILE LCMSEXPORT cmsOpenProfileFromMem(LPVOID MemPtr, DWORD dwSize)
{
       LPLCMSICCPROFILE NewIcc;
       LPVOID ICCfile;
                    
       ICCfile = MemoryOpen((LPBYTE) MemPtr, (size_t) dwSize, 'r');
       if (!ICCfile) {
              cmsSignalError(LCMS_ERRC_ABORTED, "Couldn't allocate %ld bytes for profile", dwSize);
              return NULL;  
       }

       NewIcc = CreateICCProfileHandler(ICCfile, MemoryRead, MemorySeek, MemoryTell, MemoryClose);
       if (!NewIcc) {

              cmsSignalError(LCMS_ERRC_ABORTED, "Corrupted memory profile");
              return NULL;  // Urecoverable
       }

       ReadCriticalTags(NewIcc);

       NewIcc -> PhysicalFile[0] = 0;       
       NewIcc -> IsWrite         = FALSE;

       return (cmsHPROFILE) (LPSTR) NewIcc;

}



BOOL LCMSEXPORT cmsCloseProfile(cmsHPROFILE hProfile)
{
       LPLCMSICCPROFILE icco = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       FILE *file;
       BOOL rc = TRUE;

       if (!icco) return FALSE;


       // Was open in write mode?   
       if (icco ->IsWrite) {

           icco ->IsWrite = FALSE;      // Assure no further writting
           rc = _cmsSaveProfile(hProfile, icco ->PhysicalFile);        
       }


       file = (FILE*) icco -> stream;

       if (!file)
       {
              icInt32Number i;

              for (i=0; i < icco -> TagCount; i++)
              {
                  if (icco -> TagPtrs[i])
                            free(icco -> TagPtrs[i]);
              }

       }
       else   {
              icco -> Close(file);            
              }

       free(icco);

       return rc;
}



// Write profile ------------------------------------------------------------

// Transports to properly encoded values - note that icc profiles does use
// big endian notation.

static
icInt32Number TransportValue32(icInt32Number Value)
{
       icInt32Number Temp = Value;

       AdjustEndianess32((LPBYTE) &Temp);
       return Temp;
}

static
WORD TransportValue16(WORD Value)
{
       WORD Temp = Value;

       AdjustEndianess16((LPBYTE) &Temp);
       return Temp;
}




static
BOOL SaveWordsTable(FILE* OutStream, int nEntries, LPWORD Tab, LPLCMSICCPROFILE Icc)
{
   size_t nTabSize = sizeof(WORD) * nEntries;
   LPWORD PtrW = (LPWORD) malloc(nTabSize);

   if (!PtrW) return FALSE;
   CopyMemory(PtrW, Tab, nTabSize);
   AdjustEndianessArray16(PtrW, nEntries);
   Icc ->Write(OutStream, nTabSize, PtrW);
   free(PtrW);
   
   return TRUE;
}

// Encodes now to date/time field

static
void EncodeDateTime(icDateTimeNumber* DateTime)
{
       time_t timer;
       struct tm *tmstr;

       time(&timer);
       tmstr = localtime(&timer);

       DateTime -> year    = TransportValue16((WORD) (tmstr -> tm_year + 1900));
       DateTime -> month   = TransportValue16((WORD) (tmstr -> tm_mon + 1));
       DateTime -> day     = TransportValue16((WORD) tmstr -> tm_mday);
       DateTime -> hours   = TransportValue16((WORD) tmstr -> tm_hour);
       DateTime -> minutes = TransportValue16((WORD) tmstr -> tm_min);
       DateTime -> seconds = TransportValue16((WORD) tmstr -> tm_sec);

}



// Saves profile header

static
BOOL SaveHeader(void *OutStream, LPLCMSICCPROFILE Icc)
{
  icHeader Header;

       Header.size        = TransportValue32((icInt32Number) UsedSpace);
       Header.cmmId       = TransportValue32(lcmsSignature);
       Header.version     = TransportValue32((icInt32Number) 0x02300000);
       Header.deviceClass = (icProfileClassSignature) TransportValue32(Icc -> DeviceClass);
       Header.colorSpace  = (icColorSpaceSignature) TransportValue32(Icc -> ColorSpace);
       Header.pcs         = (icColorSpaceSignature) TransportValue32(Icc -> PCS);

       EncodeDateTime(&Header.date);

       Header.magic       = TransportValue32(icMagicNumber);
       Header.platform    = (icPlatformSignature)TransportValue32(icSigMicrosoft);  // Sorry, I must put something here

       Header.flags        = TransportValue32(Icc -> flags);
       Header.manufacturer = TransportValue32(lcmsSignature);
       Header.model        = TransportValue32(0);
       Header.attributes[0]= TransportValue32(0);              // Reflective, Glossy
       Header.attributes[1]= TransportValue32(0);

       Header.renderingIntent = TransportValue32(Icc -> RenderingIntent);

       // Illuminant is D50

       Header.illuminant.X = TransportValue32(DOUBLE_TO_FIXED(Icc -> Illuminant.X));
       Header.illuminant.Y = TransportValue32(DOUBLE_TO_FIXED(Icc -> Illuminant.Y));
       Header.illuminant.Z = TransportValue32(DOUBLE_TO_FIXED(Icc -> Illuminant.Z));

       Header.creator      = TransportValue32(lcmsSignature);
        
       ZeroMemory(&Header.reserved, sizeof(Header.reserved));

       // Set profile ID
       CopyMemory(Header.reserved, Icc ->ProfileID, 16);
      

       UsedSpace = 0; // Mark as begin-of-file

       return Icc ->Write(OutStream, sizeof(icHeader), &Header);
}



// Setup base marker

static
BOOL SetupBase(FILE *OutStream, icTagTypeSignature sig, LPLCMSICCPROFILE Icc)
{
    icTagBase  Base;

    Base.sig = (icTagTypeSignature) TransportValue32(sig);
    ZeroMemory(&Base.reserved, sizeof(Base.reserved));
    return Icc -> Write(OutStream, sizeof(icTagBase), &Base);
}


// Store an XYZ tag

static
BOOL SaveXYZNumber(FILE *OutStream, LPcmsCIEXYZ Value, LPLCMSICCPROFILE Icc)
{

    icXYZNumber XYZ;

    if (!SetupBase(OutStream, icSigXYZType, Icc)) return FALSE;

    XYZ.X = TransportValue32(DOUBLE_TO_FIXED(Value -> X));
    XYZ.Y = TransportValue32(DOUBLE_TO_FIXED(Value -> Y));
    XYZ.Z = TransportValue32(DOUBLE_TO_FIXED(Value -> Z));


    return Icc -> Write(OutStream, sizeof(icXYZNumber), &XYZ);
}



// Store a curve type.

static
BOOL SaveGamma(FILE *OutStream, LPGAMMATABLE Gamma, LPLCMSICCPROFILE Icc)
{
    icInt32Number Count;
    int i;


    if (!SetupBase(OutStream, icSigCurveType, Icc)) return FALSE;

    Count = TransportValue32(Gamma->nEntries);

    if (!Icc ->Write(OutStream, sizeof(icInt32Number), &Count)) return FALSE;

    for (i=0; i < Gamma->nEntries; i++)
    {
    WORD Val = TransportValue16(Gamma->GammaTable[i]);

    if (!Icc -> Write(OutStream, sizeof(WORD), &Val))
              return FALSE;
    }

    return TRUE;
}




// Save an DESC Tag 


static
BOOL SaveDescription(FILE *OutStream, const char *Text, LPLCMSICCPROFILE Icc)
{

    icUInt32Number len, Count, TotalSize, AlignedSize;
    char Filler[256];

    len = (icUInt32Number) (strlen(Text) + 1);

    // * icInt8Number         desc[count]     * NULL terminated ascii string
    // * icUInt32Number       ucLangCode;     * UniCode language code
    // * icUInt32Number       ucCount;        * UniCode description length
    // * icInt16Number        ucDesc[ucCount];* The UniCode description
    // * icUInt16Number       scCode;         * ScriptCode code
    // * icUInt8Number        scCount;        * ScriptCode count
    // * icInt8Number         scDesc[67];     * ScriptCode Description

    TotalSize = sizeof(icTagBase) + sizeof(icUInt32Number) + len + 
                sizeof(icUInt32Number) + sizeof(icUInt32Number) +
                sizeof(icUInt16Number) + sizeof(icUInt8Number) + 67; 

    AlignedSize = TotalSize;  // Can be unaligned!!
    
    if (!SetupBase(OutStream, icSigTextDescriptionType, Icc)) return FALSE;
    AlignedSize -= sizeof(icTagBase);

    Count = TransportValue32(len);
    if (!Icc ->Write(OutStream, sizeof(icUInt32Number), &Count)) return FALSE;
    AlignedSize -= sizeof(icUInt32Number);

    if (!Icc ->Write(OutStream, len, (LPVOID)Text)) return FALSE;
    AlignedSize -= len;

    ZeroMemory(Filler, AlignedSize);
    if (!Icc ->Write(OutStream, AlignedSize, Filler)) return FALSE;

    return TRUE;
}

// Save an ASCII Tag 

static
BOOL SaveText(FILE *OutStream, const char *Text, LPLCMSICCPROFILE Icc)
{
    size_t len = strlen(Text) + 1;

    if (!SetupBase(OutStream, icSigTextType, Icc)) return FALSE;
    if (!Icc ->Write(OutStream, len, (LPVOID) Text)) return FALSE;
    return TRUE;
}


// Save one of these new chromaticity values

static
BOOL SaveOneChromaticity(FILE *OutStream, double x, double y, LPLCMSICCPROFILE Icc)
{
       Fixed32 xf, yf;

       xf = TransportValue32(DOUBLE_TO_FIXED(x));
       yf = TransportValue32(DOUBLE_TO_FIXED(y));

       if (!Icc ->Write(OutStream, sizeof(Fixed32), &xf)) return FALSE;
       if (!Icc ->Write(OutStream, sizeof(Fixed32), &yf)) return FALSE;

       return TRUE;
}


// New tag added in Addendum II of old spec.

static
BOOL SaveChromaticities(FILE *OutStream, LPcmsCIExyYTRIPLE chrm, LPLCMSICCPROFILE Icc)
{
       WORD nChans, Table;

       if (!SetupBase(OutStream, icSigChromaticityType, Icc)) return FALSE;
     
       nChans = TransportValue16(3);
       if (!Icc ->Write(OutStream, sizeof(WORD) , &nChans)) return FALSE;
       Table =  TransportValue16(0);
       if (!Icc ->Write(OutStream, sizeof(WORD) , &Table)) return FALSE;

       if (!SaveOneChromaticity(OutStream, chrm -> Red.x, chrm -> Red.y, Icc)) return FALSE;
       if (!SaveOneChromaticity(OutStream, chrm -> Green.x, chrm -> Green.y, Icc)) return FALSE;
       if (!SaveOneChromaticity(OutStream, chrm -> Blue.x, chrm -> Blue.y, Icc)) return FALSE;

       return TRUE;
}


static
BOOL SaveSequenceDescriptionTag(FILE *OutStream, LPcmsSEQ seq, LPLCMSICCPROFILE Icc)
{
    icUInt32Number nSeqs;
    icDescStruct   DescStruct;
    int i, n = seq ->n;
    LPcmsPSEQDESC pseq = seq ->seq;

    if (!SetupBase(OutStream, icSigProfileSequenceDescType, Icc)) return FALSE;
    
    nSeqs = TransportValue32(n);

    if (!Icc ->Write(OutStream, sizeof(icUInt32Number) , &nSeqs)) return FALSE;

    for (i=0; i < n; i++) {
        
        LPcmsPSEQDESC sec = pseq + i;

       
        DescStruct.deviceMfg    = (icTagTypeSignature) TransportValue32(sec ->deviceMfg);
        DescStruct.deviceModel  = (icTagTypeSignature) TransportValue32(sec ->deviceModel);
        DescStruct.technology   = (icTechnologySignature) TransportValue32(sec ->technology);
        DescStruct.attributes[0]= TransportValue32(sec ->attributes[0]);
        DescStruct.attributes[1]= TransportValue32(sec ->attributes[1]);

        if (!Icc ->Write(OutStream, sizeof(icDescStruct) - SIZEOF_UINT8_ALIGNED, &DescStruct)) return FALSE;
        
        if (!SaveDescription(OutStream, sec ->Manufacturer, Icc)) return FALSE;
        if (!SaveDescription(OutStream, sec ->Model, Icc)) return FALSE;        
    }

    return TRUE;
}


// Saves a named color list into a named color profile
static
BOOL SaveNamedColorList(FILE* OutStream, LPcmsNAMEDCOLORLIST NamedColorList, LPLCMSICCPROFILE Icc)
{

    icUInt32Number      vendorFlag;     // Bottom 16 bits for IC use 
    icUInt32Number      count;          // Count of named colors 
    icUInt32Number      nDeviceCoords;  // Num of device coordinates 
    icInt8Number        prefix[32];     // Prefix for each color name 
    icInt8Number        suffix[32];     // Suffix for each color name 
    int i;

    if (!SetupBase(OutStream, icSigNamedColor2Type, Icc)) return FALSE;

    vendorFlag    = TransportValue32(0);
    count         = TransportValue32(NamedColorList ->nColors);
    nDeviceCoords = TransportValue32(NamedColorList ->ColorantCount);

    strncpy(prefix, NamedColorList->Prefix, 32);
    strncpy(suffix, NamedColorList->Suffix, 32);
                  
    if (!Icc ->Write(OutStream, sizeof(icUInt32Number), &vendorFlag)) return FALSE;
    if (!Icc ->Write(OutStream, sizeof(icUInt32Number), &count)) return FALSE;
    if (!Icc ->Write(OutStream, sizeof(icUInt32Number), &nDeviceCoords)) return FALSE;
    if (!Icc ->Write(OutStream, 32 , prefix)) return FALSE;
    if (!Icc ->Write(OutStream, 32 , suffix)) return FALSE;

    for (i=0; i < NamedColorList ->nColors; i++) {

          icUInt16Number PCS[3];
          icUInt16Number Colorant[MAXCHANNELS];
          icInt8Number root[32];
          LPcmsNAMEDCOLOR Color;
          int j;

                    Color = NamedColorList ->List + i;

                    strncpy(root, Color ->Name, 32);
                    if (!Icc ->Write(OutStream, 32 , root)) return FALSE;
                    
                    for (j=0; j < 3; j++)
                        PCS[j] = TransportValue16(Color ->PCS[j]);

                    if (!Icc ->Write(OutStream, 3 * sizeof(icUInt16Number), PCS)) return FALSE;

                    for (j=0; j < NamedColorList ->ColorantCount; j++)
                        Colorant[j] = TransportValue16(Color ->DeviceColorant[j]);
                        
                    if (!Icc ->Write(OutStream, 
                            NamedColorList ->ColorantCount * sizeof(icUInt16Number), Colorant)) return FALSE;                                                           
    }


    return TRUE;
}



// Does serialization of LUT and writes to disk. I'm always using LUT16 type
// because it seems to me (only a feeling, not a proven fact) that the
// interpolation makes more accurate a table of 16 bps that same with double
// of nodes on LUT8. Anyway, this should be regarded more carefully

static
BOOL SaveLUT(FILE* OutStream, const LPLUT NewLUT, LPLCMSICCPROFILE Icc)
{
       icLut16 LUT16;
       unsigned int i;
       size_t nTabSize;
       WORD NullTbl[2] = { 0, 0xFFFFU};


       if (!SetupBase(OutStream, icSigLut16Type, Icc)) return FALSE;

       LUT16.clutPoints = (icUInt8Number) NewLUT -> cLutPoints;
       LUT16.inputChan  = (icUInt8Number) NewLUT -> InputChan;
       LUT16.outputChan = (icUInt8Number) NewLUT -> OutputChan;

       LUT16.inputEnt   = TransportValue16((WORD) ((NewLUT -> wFlags & LUT_HASTL1) ? NewLUT -> InputEntries  : 2));
       LUT16.outputEnt  = TransportValue16((WORD) ((NewLUT -> wFlags & LUT_HASTL2) ? NewLUT -> OutputEntries : 2));

       if (NewLUT -> wFlags & LUT_HASMATRIX) {

       LUT16.e00 = TransportValue32(NewLUT -> Matrix.v[0].n[0]);
       LUT16.e01 = TransportValue32(NewLUT -> Matrix.v[0].n[1]);
       LUT16.e02 = TransportValue32(NewLUT -> Matrix.v[0].n[2]);
       LUT16.e10 = TransportValue32(NewLUT -> Matrix.v[1].n[0]);
       LUT16.e11 = TransportValue32(NewLUT -> Matrix.v[1].n[1]);
       LUT16.e12 = TransportValue32(NewLUT -> Matrix.v[1].n[2]);
       LUT16.e20 = TransportValue32(NewLUT -> Matrix.v[2].n[0]);
       LUT16.e21 = TransportValue32(NewLUT -> Matrix.v[2].n[1]);
       LUT16.e22 = TransportValue32(NewLUT -> Matrix.v[2].n[2]);
       }
       else {

       LUT16.e00 = TransportValue32(DOUBLE_TO_FIXED(1));
       LUT16.e01 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT16.e02 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT16.e10 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT16.e11 = TransportValue32(DOUBLE_TO_FIXED(1));
       LUT16.e12 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT16.e20 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT16.e21 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT16.e22 = TransportValue32(DOUBLE_TO_FIXED(1));
       }


       // Save header

       Icc -> Write(OutStream,  sizeof(icLut16)- SIZEOF_UINT16_ALIGNED, &LUT16);

       // The prelinearization table

       for (i=0; i < NewLUT -> InputChan; i++) {

        if (NewLUT -> wFlags & LUT_HASTL1) {

               if (!SaveWordsTable(OutStream, 
                                NewLUT -> InputEntries, 
                                NewLUT -> L1[i], Icc)) return FALSE;

        }
        else Icc -> Write(OutStream, sizeof(WORD)* 2, NullTbl);
       }


       nTabSize = (NewLUT -> OutputChan * uipow(NewLUT->cLutPoints,
                                                 NewLUT->InputChan));
       // The 3D CLUT.

       if (!SaveWordsTable(OutStream, (int) nTabSize, NewLUT -> T, Icc)) return FALSE;

       // The postlinearization table

       for (i=0; i < NewLUT -> OutputChan; i++) {

        if (NewLUT -> wFlags & LUT_HASTL2) {

                if (!SaveWordsTable(OutStream,  
                                    NewLUT -> OutputEntries, 
                                    NewLUT -> L2[i], Icc)) return FALSE;
        }
        else Icc -> Write(OutStream, sizeof(WORD)* 2, NullTbl);

       }

        return TRUE;
}



// Special case: Saves a LUT8 ver2 -- This is used only for making smaller profiles

static
BOOL SaveLUT8(FILE* OutStream, const LPLUT NewLUT, LPLCMSICCPROFILE Icc)
{
       icLut8 LUT8;
       unsigned int i, j;
       size_t nTabSize;
       BYTE val;

       // Sanity check
       
       if (NewLUT -> wFlags & LUT_HASTL1) {

           if (NewLUT -> InputEntries != 256) {
                cmsSignalError(LCMS_ERRC_ABORTED, "LUT8 needs 256 entries on prelinearization");
                return FALSE;
           }

       }


       if (NewLUT -> wFlags & LUT_HASTL2) {

           if (NewLUT -> OutputEntries != 256) {
                cmsSignalError(LCMS_ERRC_ABORTED, "LUT8 needs 256 entries on postlinearization");
                return FALSE;
           }
       }

       

       if (!SetupBase(OutStream, icSigLut8Type, Icc)) return FALSE;

       LUT8.clutPoints = (icUInt8Number) NewLUT -> cLutPoints;
       LUT8.inputChan  = (icUInt8Number) NewLUT -> InputChan;
       LUT8.outputChan = (icUInt8Number) NewLUT -> OutputChan;
       

       if (NewLUT -> wFlags & LUT_HASMATRIX) {

       LUT8.e00 = TransportValue32(NewLUT -> Matrix.v[0].n[0]);
       LUT8.e01 = TransportValue32(NewLUT -> Matrix.v[0].n[1]);
       LUT8.e02 = TransportValue32(NewLUT -> Matrix.v[0].n[2]);
       LUT8.e10 = TransportValue32(NewLUT -> Matrix.v[1].n[0]);
       LUT8.e11 = TransportValue32(NewLUT -> Matrix.v[1].n[1]);
       LUT8.e12 = TransportValue32(NewLUT -> Matrix.v[1].n[2]);
       LUT8.e20 = TransportValue32(NewLUT -> Matrix.v[2].n[0]);
       LUT8.e21 = TransportValue32(NewLUT -> Matrix.v[2].n[1]);
       LUT8.e22 = TransportValue32(NewLUT -> Matrix.v[2].n[2]);
       }
       else {

       LUT8.e00 = TransportValue32(DOUBLE_TO_FIXED(1));
       LUT8.e01 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT8.e02 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT8.e10 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT8.e11 = TransportValue32(DOUBLE_TO_FIXED(1));
       LUT8.e12 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT8.e20 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT8.e21 = TransportValue32(DOUBLE_TO_FIXED(0));
       LUT8.e22 = TransportValue32(DOUBLE_TO_FIXED(1));
       }


       // Save header

       Icc -> Write(OutStream,  sizeof(icLut8)- SIZEOF_UINT8_ALIGNED, &LUT8);

       // The prelinearization table

       for (i=0; i < NewLUT -> InputChan; i++) {

           for (j=0; j < 256; j++) {

               if (NewLUT -> wFlags & LUT_HASTL1)
                        val = (BYTE) floor(NewLUT ->L1[i][j] / 257.0 + .5);
               else
                        val = (BYTE) j;

               Icc ->Write(OutStream, 1, &val);
           }
               
       }


       nTabSize = (NewLUT -> OutputChan * uipow(NewLUT->cLutPoints,
                                                 NewLUT->InputChan));
       // The 3D CLUT.

       for (j=0; j < nTabSize; j++) {

              val = (BYTE) floor(NewLUT ->T[j] / 257.0 + .5);
              Icc ->Write(OutStream, 1, &val);
       }

       // The postlinearization table

       for (i=0; i < NewLUT -> OutputChan; i++) {

           for (j=0; j < 256; j++) {

               if (NewLUT -> wFlags & LUT_HASTL2)
                    val = (BYTE) floor(NewLUT ->L2[i][j] / 257.0 + .5);
               else
                    val = (BYTE) j;

               Icc ->Write(OutStream, 1, &val);
           }

       }

        return TRUE;
}

void LCMSEXPORT _cmsSetLUTdepth(cmsHPROFILE hProfile, int depth)
{
    LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    switch (depth) {

    case 8:  Icc ->SaveAs8Bits = TRUE; break;
    case 16: Icc ->SaveAs8Bits = FALSE; break;      
        
    default:
        cmsSignalError(LCMS_ERRC_ABORTED, "%d is an unsupported as bitdepth, use 8 or 16 only.", depth);
    }
}


// Saves Tag directory

static
BOOL SaveTagDirectory(void *OutStream, LPLCMSICCPROFILE Icc)
{
       icInt32Number i;
       icTag Tag;       
       icInt32Number Count = 0;

       // Get true count
       for (i=0;  i < Icc -> TagCount; i++) {
            if (Icc ->TagNames[i] != 0)
                    Count++;
       }

       Count = TransportValue32(Count);
       if (!Icc ->Write(OutStream, sizeof(icInt32Number) , &Count)) return FALSE;

       for (i=0; i < Icc -> TagCount; i++) {

          if (Icc ->TagNames[i] == 0) continue;

          Tag.sig    = (icTagSignature)TransportValue32(Icc -> TagNames[i]);
          Tag.offset = TransportValue32((icInt32Number) Icc -> TagOffsets[i]);
          Tag.size   = TransportValue32((icInt32Number) Icc -> TagSizes[i]);

          if (!Icc ->Write(OutStream, sizeof(icTag), &Tag)) return FALSE;
       }

       return TRUE;
}


// Dump tag contents

static
BOOL SaveTags(void *OutStream, LPLCMSICCPROFILE Icc)
{

    LPBYTE Data;
    icInt32Number i;
    size_t Begin;
    size_t AlignedSpace, FillerSize;


    for (i=0; i < Icc -> TagCount; i++) {

         if (Icc ->TagNames[i] == 0) continue;
        
        // Align to DWORD boundary, following new spec.
        
        AlignedSpace = ALIGNLONG(UsedSpace);
        FillerSize  = AlignedSpace - UsedSpace;
        if (FillerSize > 0)  {
            
            BYTE Filler[20];

            ZeroMemory(Filler, 16);
            if (!Icc ->Write(OutStream, FillerSize, Filler)) return FALSE;
        }
        
        
       Icc -> TagOffsets[i] = Begin = UsedSpace;
       Data = (LPBYTE) Icc -> TagPtrs[i];
       if (!Data)
              continue;

       switch (Icc -> TagNames[i]) {

       case icSigProfileDescriptionTag: 
       case icSigDeviceMfgDescTag:
       case icSigDeviceModelDescTag:
              if (!SaveDescription((FILE*) OutStream, (const char *) Data, Icc)) return FALSE;
              break;

       case icSigRedColorantTag:
       case icSigGreenColorantTag:
       case icSigBlueColorantTag:
       case icSigMediaWhitePointTag:
       case icSigMediaBlackPointTag:           
               if (!SaveXYZNumber((FILE*) OutStream, (LPcmsCIEXYZ) Data, Icc)) return FALSE;
               break;


       case icSigRedTRCTag:
       case icSigGreenTRCTag:
       case icSigBlueTRCTag:
       case icSigGrayTRCTag:
               if (!SaveGamma((FILE*) OutStream, (LPGAMMATABLE) Data, Icc)) return FALSE;
               break;

       case icSigCharTargetTag:
       case icSigCopyrightTag:      
              if (!SaveText((FILE*) OutStream, (const char *) Data, Icc)) return FALSE;
              break;

       case icSigChromaticityTag:
              if (!SaveChromaticities((FILE*) OutStream, (LPcmsCIExyYTRIPLE) Data, Icc)) return FALSE;
              break;

       // Save LUT 

       case icSigAToB0Tag:
       case icSigAToB1Tag:
       case icSigAToB2Tag:
       case icSigBToA0Tag:
       case icSigBToA1Tag:
       case icSigBToA2Tag:
       case icSigGamutTag:
       case icSigPreview0Tag:
       case icSigPreview1Tag:
       case icSigPreview2Tag:

                if (Icc ->SaveAs8Bits) {

                        if (!SaveLUT8((FILE*) OutStream, (LPLUT) Data, Icc)) return FALSE;
                }
                else {

                    if (!SaveLUT((FILE*) OutStream, (LPLUT) Data, Icc)) return FALSE;
                }
                break;

       case icSigProfileSequenceDescTag:               
              if (!SaveSequenceDescriptionTag((FILE*) OutStream, (LPcmsSEQ) Data, Icc)) return FALSE;              
              break;


       case icSigNamedColor2Type:
             if (!SaveNamedColorList((FILE*) OutStream, (LPcmsNAMEDCOLORLIST) Data, Icc)) return FALSE;              
             break;


       default:
              return FALSE;
       }

       Icc -> TagSizes[i] = (UsedSpace - Begin);
       }

        

       return TRUE;
}

BOOL LCMSEXPORT _cmsAddXYZTag(cmsHPROFILE hProfile, icTagSignature sig, const LPcmsCIEXYZ XYZ)
{
       LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       
       InitTag(Icc, sig, sizeof(cmsCIEXYZ), XYZ);
       return TRUE;
}


BOOL LCMSEXPORT _cmsAddTextTag(cmsHPROFILE hProfile, icTagSignature sig, const char* Text)
{
       LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

       InitTag(Icc, sig, strlen(Text)+1, (LPVOID) Text);
       return TRUE;
}


static
LPVOID DupBlock(LPLCMSICCPROFILE Icc, LPVOID Block, size_t size)
{
    if (Block != NULL && size > 0) 
        return InitTag(Icc, (icTagSignature) 0, size, Block);
    else
        return NULL;

}

// This is tricky, since LUT structs does have pointers

BOOL LCMSEXPORT _cmsAddLUTTag(cmsHPROFILE hProfile, icTagSignature sig, LPVOID lut)
{
       LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
       LPLUT Orig, Stored;
       unsigned int i;
       
       // The struct itself

       Orig   = (LPLUT) lut;
       Stored = (LPLUT) InitTag(Icc, (icTagSignature) sig, sizeof(LUT), lut);

       // dup' the memory blocks
       for (i=0; i < Orig ->InputChan; i++) 
            Stored -> L1[i] = (LPWORD) DupBlock(Icc, (LPWORD) Orig ->L1[i], 
                                            sizeof(WORD) * Orig ->In16params.nSamples);

       for (i=0; i < Orig ->OutputChan; i++)
            Stored -> L2[i] = (LPWORD) DupBlock(Icc, (LPWORD) Orig ->L2[i], 
                                            sizeof(WORD) * Orig ->Out16params.nSamples);  
       
       Stored -> T     = (LPWORD) DupBlock(Icc, (LPWORD) Orig ->T, Orig -> Tsize);

       return TRUE;
}


BOOL LCMSEXPORT _cmsAddGammaTag(cmsHPROFILE hProfile, icTagSignature sig, LPGAMMATABLE TransferFunction)
{
    LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    InitTag(Icc, sig, SizeOfGammaTab(TransferFunction), TransferFunction);
    return TRUE;
}


BOOL LCMSEXPORT _cmsAddChromaticityTag(cmsHPROFILE hProfile, icTagSignature sig, LPcmsCIExyYTRIPLE Chrm)
{
    LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;

    InitTag(Icc, sig, sizeof(cmsCIExyYTRIPLE), Chrm);
    return TRUE;
}


BOOL LCMSEXPORT _cmsAddSequenceDescriptionTag(cmsHPROFILE hProfile, icTagSignature sig, LPcmsSEQ pseq)
{
    LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
    
    InitTag(Icc, sig, sizeof(int) + pseq -> n * sizeof(cmsPSEQDESC), pseq);
    return TRUE;

}


BOOL LCMSEXPORT _cmsAddNamedColorTag(cmsHPROFILE hProfile, icTagSignature sig, LPcmsNAMEDCOLORLIST nc)
{
    LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;
   
    InitTag(Icc, sig, sizeof(cmsNAMEDCOLORLIST) + (nc ->nColors - 1) * sizeof(cmsNAMEDCOLOR), nc);
    return FALSE;
}



// Add tags to profile structure

BOOL LCMSEXPORT cmsAddTag(cmsHPROFILE hProfile, icTagSignature sig, LPVOID Tag)
{
   BOOL rc;

   switch (sig) {

       case icSigCharTargetTag:
       case icSigCopyrightTag:             
       case icSigProfileDescriptionTag: 
       case icSigDeviceMfgDescTag:
       case icSigDeviceModelDescTag:
              rc = _cmsAddTextTag(hProfile, sig, (const char*) Tag);
              break;

       case icSigRedColorantTag:
       case icSigGreenColorantTag:
       case icSigBlueColorantTag:
       case icSigMediaWhitePointTag:
       case icSigMediaBlackPointTag:           
              rc = _cmsAddXYZTag(hProfile, sig, (const LPcmsCIEXYZ) Tag);
              break; 

       case icSigRedTRCTag:
       case icSigGreenTRCTag:
       case icSigBlueTRCTag:
       case icSigGrayTRCTag:
              rc =  _cmsAddGammaTag(hProfile, sig, (LPGAMMATABLE) Tag);
              break;
                     
       case icSigAToB0Tag:
       case icSigAToB1Tag:
       case icSigAToB2Tag:
       case icSigBToA0Tag:
       case icSigBToA1Tag:
       case icSigBToA2Tag:
       case icSigGamutTag:
       case icSigPreview0Tag:
       case icSigPreview1Tag:
       case icSigPreview2Tag:
              rc =  _cmsAddLUTTag(hProfile, sig, Tag);
              break;

       case icSigChromaticityTag:
              rc =  _cmsAddChromaticityTag(hProfile, sig, (LPcmsCIExyYTRIPLE) Tag);              
              break;
        
       case icSigProfileSequenceDescTag:
              rc = _cmsAddSequenceDescriptionTag(hProfile, sig, (LPcmsSEQ) Tag);
              break;

       case icSigNamedColor2Tag:
              rc = _cmsAddNamedColorTag(hProfile, sig, (LPcmsNAMEDCOLORLIST) Tag);
             break;

       default:
            cmsSignalError(LCMS_ERRC_ABORTED, "cmsAddTag: Tag '%x' is unsupported", sig);
            return FALSE;
   }

   // Check for critical tags

   switch (sig) {

   case icSigMediaWhitePointTag:
   case icSigMediaBlackPointTag:
   case icSigChromaticAdaptationTag:
       
        ReadCriticalTags((LPLCMSICCPROFILE) hProfile);
        break;

   default:;
   }

   return rc;

}

// Low-level save to disk
BOOL LCMSEXPORT _cmsSaveProfile(cmsHPROFILE hProfile, const char* FileName)
{
       FILE *OutStream;
       LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;


       Icc ->Write       = FileWrite;   // Save to disk
             
       // Pass #1 does compute offsets
     
       if (!SaveHeader(NULL, Icc)) return FALSE;
       if (!SaveTagDirectory(NULL, Icc)) return FALSE;
       if (!SaveTags(NULL, Icc)) return FALSE;

       OutStream = fopen(FileName, "wb");
       if (!OutStream) return FALSE;

       // Pass #2 does save to file
     
       if (!SaveHeader(OutStream, Icc)) goto CleanUp;
       if (!SaveTagDirectory(OutStream, Icc)) goto CleanUp;
       if (!SaveTags(OutStream, Icc)) goto CleanUp;
      
       return (fclose(OutStream) == 0);

   CleanUp:
     
       fclose(OutStream);
       unlink(FileName);
       return FALSE;
}


// Low-level save from open stream
BOOL LCMSEXPORT _cmsSaveProfileToMem(cmsHPROFILE hProfile, void *MemPtr, 
                                                           size_t* BytesNeeded)
{      FILEMEM* OutStream; 
       LPLCMSICCPROFILE Icc = (LPLCMSICCPROFILE) (LPSTR) hProfile;     
      

       Icc ->Write       = MemoryWrite;   

       // Pass #1 does compute offsets

       if (!SaveHeader(NULL, Icc)) return FALSE;
       if (!SaveTagDirectory(NULL, Icc)) return FALSE;
       if (!SaveTags(NULL, Icc)) return FALSE;              

       if (!MemPtr) {

          // update BytesSaved so caller knows how many bytes are needed for MemPtr
           *BytesNeeded = UsedSpace;
           return TRUE;
       }        

       if (*BytesNeeded < UsedSpace) {
           // need at least UsedSpace in MemPtr to continue       
            return FALSE;
       }

       OutStream = (FILEMEM*) MemoryOpen((LPBYTE) MemPtr, *BytesNeeded, 'w');
       if (!OutStream) return FALSE;
       
       OutStream ->Block = (LPBYTE) MemPtr;
                  
       // Pass #2 does save to file into supplied stream     
       if (!SaveHeader(OutStream, Icc)) goto CleanUp;
       if (!SaveTagDirectory(OutStream, Icc)) goto CleanUp;
       if (!SaveTags(OutStream, Icc)) goto CleanUp;
      
       // update BytesSaved so caller knows how many bytes put into stream
       *BytesNeeded = UsedSpace;
       
       free(OutStream);
       return TRUE;

   CleanUp:
           
       free(OutStream);
       return FALSE;
}

