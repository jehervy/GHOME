#ifndef _GMEM_H
#define _GMEM_H

#include "../config.h"

//Size of the segment allocated for the use of the memory allocator module.
//Metadata and data are stored both in the segment.
#define GMEM_SIZE 500

#ifdef GMEM
void *operator new(size_t size);

void operator delete(void* mem);
#endif

/**
 * Allocates memory of the specified size. It only reserves the memory, the
 * content of the returned space is random.
 *
 * @param size The size in bytes
 * @return void* Pointer to the free space
 */
void *gmalloc (unsigned size);

/**
 * Free memory starting at the address given as parameter. Memory is free
 * until the end of the block.
 *
 * @param void* Pointer to the space to free
 */
void gfree (void *ptr);

/**
 * Displays the statuts of the memory managed by this module.
 */
void gprintmem();

#endif


