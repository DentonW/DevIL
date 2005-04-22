//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 06/13/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_alloc.c
//
// Description: File handling for DevIL
//
//-----------------------------------------------------------------------------


#define __ALLOC_C
#include "il_internal.h"
#include <stdlib.h>
#include <math.h>

mAlloc ialloc_ptr = NULL;
mFree  ifree_ptr = NULL;

#ifdef ALTIVEC_GCC
ILvoid *vec_malloc( ILuint size ) {
	size =  size % 16 > 0 ? size + 16 - (size % 16) : size; // align size value
	
	#ifdef POSIX_MEMALIGN
		return posix_memalign(&buffer, 16, size);
	#else
	#ifdef VALLOC
		return valloc( size );
	#else
	#ifdef MEMALIGN
		return memalign( 16, size );
	#else
		#error NO ALIGNED MEMORY ALLOCATION FUNCTIONS...
	#endif
	#endif
	#endif
}

ILvoid *ivec_align_buffer( ILvoid *buffer, ILuint size ) {
	if( (size_t)buffer % 16 != 0 ) {
        void *aligned_buffer = vec_malloc( size );
        memcpy( aligned_buffer, buffer, size );
        ifree( buffer );
        return aligned_buffer;
    }
    return buffer;
}
#endif

ILvoid* ILAPIENTRY DefaultAllocFunc(ILuint Size)
{
        #ifdef ALTIVEC_GCC
            return (ILvoid*)vec_malloc(Size);
        #else
            return malloc(Size);
        #endif
}


ILvoid* ILAPIENTRY ialloc(ILuint Size)
{
        ILvoid *Ptr;

        Ptr = ialloc_ptr(Size);
        if (Ptr == NULL)
                ilSetError(IL_OUT_OF_MEMORY);
	return Ptr;
}


ILvoid ILAPIENTRY ifree(ILvoid *Ptr)
{
        ifree_ptr(Ptr);
        return;
}


ILvoid* ILAPIENTRY icalloc(ILuint Size, ILuint Num)
{
	ILvoid *Ptr;
        Ptr = ialloc(Size * Num);
        if (Ptr == NULL)
            return NULL;

        imemclear(Ptr, Size * Num);
        return Ptr;
}


ILvoid ILAPIENTRY DefaultFreeFunc(ILvoid *Ptr)
{
        if (Ptr)
            free(Ptr);
}


ILvoid ILAPIENTRY ilResetMemory()
{
	ialloc_ptr = DefaultAllocFunc;
	ifree_ptr = DefaultFreeFunc;
	return;
}


ILvoid ILAPIENTRY ilSetMemory(mAlloc AllocFunc, mFree FreeFunc)
{
	/*if (AllocFunc == NULL || FreeFunc == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return;
	}*/

	if (AllocFunc == NULL || FreeFunc == NULL) {
		if (ialloc_ptr == NULL || ifree_ptr == NULL) {
			ialloc_ptr = DefaultAllocFunc;
			ifree_ptr = DefaultFreeFunc;
		}

		// If not, we have the special case used by ilInit, so that
		//	ilSetMemory can be called before ilInit to control all
		//	memory usage of DevIL.

		return;
	}

	ialloc_ptr = AllocFunc;
	ifree_ptr = FreeFunc;
	return;
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
