//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Last modified: 08/17/2008
//
// Filename: src-IL/src/il_alloc.c
//
// Description: Memory allocation functions
//
//-----------------------------------------------------------------------------


#define __ALLOC_C
#include "il_internal.h"
#include <stdlib.h>
#include <math.h>

#ifdef MM_MALLOC
#include <mm_malloc.h>
#endif

static ILvoid ILAPIENTRY DefaultFreeFunc(const ILvoid * CONST_RESTRICT Ptr);
static ILvoid* ILAPIENTRY DefaultAllocFunc(const ILuint Size);

static mAlloc ialloc_ptr = DefaultAllocFunc;
static mFree  ifree_ptr = DefaultFreeFunc;

/*** Vector Allocation/Deallocation Function ***/
#ifdef VECTORMEM
ILvoid *vec_malloc(const ILuint size)
{
	const ILuint _size =  size % 16 > 0 ? size + 16 - (size % 16) : size; // align size value
	
#ifdef MM_MALLOC
	return _mm_malloc(_size,16);
#else
#ifdef VALLOC
	return valloc( _size );
#else
#ifdef POSIX_MEMALIGN
	char *buffer;
	return posix_memalign(&buffer, 16, _size) == 0 ? buffer : NULL;
#else
#ifdef MEMALIGN
	return memalign( 16, _size );
#else
	// Memalign hack from ffmpeg for MinGW
	void *ptr;
	int diff;
	ptr = malloc(_size+16+1);
	diff= ((-(int)ptr - 1)&15) + 1;
	ptr += diff;
	((char*)ptr)[-1]= diff;
	return ptr;
#endif //MEMALIGN
#endif //POSIX_MEMALIGN
#endif //VALLOC
#endif //MM_MALLOC
}

ILvoid *ivec_align_buffer(ILvoid *buffer, const ILuint size)
{
	if( (size_t)buffer % 16 != 0 ) {
        void *aligned_buffer = vec_malloc( size );
        memcpy( aligned_buffer, buffer, size );
        ifree( buffer );
        return aligned_buffer;
    }
    return buffer;
}
#endif


/*** Allocation/Deallocation Function ***/
ILvoid* ILAPIENTRY ialloc(const ILuint Size) {
	ILvoid *Ptr = ialloc_ptr(Size);
	if (Ptr == NULL)
		ilSetError(IL_OUT_OF_MEMORY);
	return Ptr;
}

ILvoid ILAPIENTRY ifree(const ILvoid * CONST_RESTRICT Ptr) {
	ifree_ptr(Ptr);
	return;
}

ILvoid* ILAPIENTRY icalloc(const ILuint Size, const ILuint Num)
{
    ILvoid *Ptr = ialloc(Size * Num);
    if (Ptr == NULL)
    	return Ptr;
    imemclear(Ptr, Size * Num);
    return Ptr;
}

/*** Default Allocation/Deallocation Function ***/
static ILvoid* ILAPIENTRY DefaultAllocFunc(const ILuint Size)
{
#ifdef VECTORMEM
	return (ILvoid*)vec_malloc(Size);
#else
	return malloc(Size);
#endif //VECTORMEM
}

static ILvoid ILAPIENTRY DefaultFreeFunc(const ILvoid * CONST_RESTRICT ptr)
{
	if (ptr)
	{
#ifdef MM_MALLOC
	    _mm_free((ILvoid*)ptr);
#else
#if defined(VECTORMEM) & !defined(POSIX_MEMALIGN) & !defined(VALLOC) & !defined(MEMALIGN) & !defined(MM_MALLOC)
	    free(ptr - ((char*)ptr)[-1]);
#else	    
	    free((void*)ptr);
#endif //OTHERS...
#endif //MM_MALLOC
	}
}

/*** Manipulate Allocation/Deallocation Function ***/
ILvoid ILAPIENTRY ilResetMemory()  // Deprecated
{
	ialloc_ptr = DefaultAllocFunc;
	ifree_ptr = DefaultFreeFunc;
}

ILvoid ILAPIENTRY ilSetMemory(mAlloc AllocFunc, mFree FreeFunc)
{
	ialloc_ptr = AllocFunc == NULL ? DefaultAllocFunc : AllocFunc;
	ifree_ptr = FreeFunc == NULL ? DefaultFreeFunc : FreeFunc;
}


/*#if defined(_WIN32) && defined(_MEM_DEBUG)
#include <windows.h>

int bAtexit = 0;


typedef struct ALLOC_INFO
{
	unsigned long	address;
	unsigned long	size;
	char			file[64];
	unsigned long	line;
	struct ALLOC_INFO *Next;
} ALLOC_INFO;
ALLOC_INFO *AllocList;


void AddTrack(unsigned long addr, unsigned long size, const char *file, unsigned long line)
{
	ALLOC_INFO *Temp;
	
	if (AllocList == NULL) {
		AllocList = (ALLOC_INFO*)malloc(sizeof(ALLOC_INFO));  // Just assume it succeeds.
		AllocList->address = addr;
		AllocList->size = size;
		AllocList->line = line;
		strncpy(AllocList->file, file, 63);
		AllocList->Next = NULL;
	}
	else {
		Temp = AllocList;
		AllocList = (ALLOC_INFO*)malloc(sizeof(ALLOC_INFO));  // Just assume it succeeds.
		AllocList->address = addr;
		AllocList->size = size;
		AllocList->line = line;
		strncpy(AllocList->file, file, 63);
		AllocList->Next = Temp;
	}
	
	return;
}


void RemoveTrack(unsigned long addr)
{
	ALLOC_INFO *Temp, *Prev;
	
	Temp = AllocList;
	Prev = NULL;
	
	if (Temp == NULL)
		return;
	
	while (Temp != NULL) {
		if (Temp->address == addr) {
			if (Prev == NULL) {
				AllocList = Temp->Next;
				free(Temp);
			}
			else {
				Prev->Next = Temp->Next;
				free(Temp);
			}
			break;
		}
		Prev = Temp;
		Temp = Temp->Next;
	}
	
	return;
}


void DumpUnfreed(void)
{
	unsigned long TotalSize = 0;
	char buf[1024];
	ALLOC_INFO *i = AllocList;
	
	OutputDebugString("DevIL Unfreed Information:\n");
	while (i != NULL) {
		sprintf(buf, "%s(%d) : %d bytes unfreed at %d\n", i->file, i->line, i->size, i->address);
		OutputDebugString(buf);
		TotalSize += i->size;
		
		AllocList = i->Next;
		free(i);
		i = AllocList;
	}
	
	sprintf(buf, "-----------------------------------------------------------\n");
	OutputDebugString(buf);
	sprintf(buf, "Total Unfreed: %d bytes\n\n\n", TotalSize);
	OutputDebugString(buf);
}

void AddToAtexit()
{
	if (bAtexit)
		return;
	atexit(DumpUnfreed);
	bAtexit = 1;
}

void *c_alloc(unsigned long size, unsigned long num, const char *file, unsigned long line)
{
	void *ptr;
	ptr = calloc(size, num);
	if (!ptr)
		return NULL;
	AddToAtexit();
	AddTrack((unsigned long)ptr, size * num, file, line);
	return ptr;
}


void *m_alloc(unsigned long size, const char *file, unsigned long line)
{
	void *ptr;
	ptr = malloc(size);
	if (!ptr)
		return NULL;
	AddToAtexit();
	AddTrack((unsigned long)ptr, size, file, line);
	return ptr;
}


void f_ree(void *ptr)
{
	RemoveTrack((unsigned long)ptr);
	free(ptr);
	return;
}

#endif//defined(_WIN32) && defined(_MEM_DEBUG)*/
