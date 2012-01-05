#ifndef _MEMORY_H
#define _MEMORY_H

//Define GMEM to use the custom memory allocator module. If not defined, the
//standard functions will be used instead.
#define GMEM

//Size of the segment allocated for the use of the memory allocator module.
//Metadata and data are stored both in the segment.
#define GMEMSIZE 500

void *gmalloc (unsigned size);
void gfree (void *ptr);
void gprintmem();

#endif


