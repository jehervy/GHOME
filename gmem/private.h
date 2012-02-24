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

/**
 * Finds the metadata of a block as big enough to contain data of the
 * specified size.
 *
 * @param previous Last visited block
 * @param size Size of the space, in bytes
 * @return The metadata block
 */
PRIVATE Block *findBlockForSize(Block **previous, unsigned size);

/**
 * Finds the metadata of the block associated with the given pointer. The
 * pointer must point at the begining of the space.
 *
 * @param previous Last visited block
 * @param ptr Pointer to the space
 * @return The metadata block
 */
PRIVATE Block *findBlockForAddress(Block **previous, void *ptr);

/**
 * Splits a block to match the exact size given as second parameter. The
 * block's size is reduced and a new block is created in the free space.
 *
 * @param *block The block to merge, if possible
 * @param size The wished size of the block
 */
PRIVATE void splitBlock(Block *block, unsigned size);

/**
 * Merges a block with the next block if both are free. The block given as
 * parameter is extended to its new size and the next is removed from the list.
 *
 * @param *block The block to merge, if possible
 */
PRIVATE void mergeBlock(Block *block);

#endif /* _GMEM_PRIVATE_H */

