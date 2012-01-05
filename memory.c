#include <stdlib.h>
//#define GMEMORY

typedef struct Block Block;
struct Block {
	Block *next;
	unsigned size;
	void *ptr;
};

void *gmalloc (unsigned size) {
#ifndef GMEMORY
	return malloc(size);
#else
#endif
}

void gfree (void *ptr) {
#ifndef GMEMORY
	return free(ptr);
#else
#endif
}
