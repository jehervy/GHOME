#ifndef _GMEM_PRIVATE_H
#define _GMEM_PRIVATE_H

#ifdef TESTING
#define PRIVATE
#else
#define PRIVATE static
#endif

/**
 * Metadata of a memory block.
 *
 * @var struct
 */
typedef struct Block Block;
struct Block
{
	unsigned size; //Size of the block, in bytes
	Block *next;   //Pointer to the next block, NULL if none
	char free;     //Is the block free?
};

#endif /* _GMEM_PRIVATE_H */

