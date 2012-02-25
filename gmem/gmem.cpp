#include "gmem.h"

/**
 * Pointer to the head of allocated space for the gmem module.
 *
 * @var void*
 */
PRIVATE void *head = NULL;

/**
 * Finds the metadata of a block as big enough to contain data of the
 * specified size.
 *
 * @param previous Last visited block
 * @param size Size of the space, in bytes
 * @return The metadata block
 */
static Block *findBlockForSize(Block **previous, unsigned size);

/**
 * Finds the metadata of the block associated with the given pointer. The
 * pointer must point at the begining of the space.
 *
 * @param previous Last visited block
 * @param ptr Pointer to the space
 * @return The metadata block
 */
static Block *findBlockForAddress(Block **previous, void *ptr);

/**
 * Splits a block to match the exact size given as second parameter. The
 * block's size is reduced and a new block is created in the free space.
 *
 * @param *block The block to merge, if possible
 * @param size The wished size of the block
 */
static void splitBlock(Block *block, unsigned size);

/**
 * Merges a block with the next block if both are free. The block given as
 * parameter is extended to its new size and the next is removed from the list.
 *
 * @param *block The block to merge, if possible
 */
static void mergeBlock(Block *block);

#ifdef GMEM
#ifndef TESTING
/**
 * Overloads the new operator to use our gmalloc function. Overloading
 * this operator only overload the memory allocation part, the constructor
 * is still automatically called after the allocation.
 *
 * @throw std::bad_alloc If there was an error while allocating the memory
 * @return void* Pointer to the allocated zone
 */
void *operator new(size_t size) throw (std::bad_alloc)
{
	void *p = gmalloc(size);
	if (p == NULL)
	{
		throw new std::bad_alloc;
	}

	return p;
}

/**
 * Overloads the delete operator to use our gfree function. Overloading
 * this operator only overload the memory disallocation part, the destructor
 * is still automatically called after the disallocation.
 */
void operator delete(void *mem)
{
	gfree(mem);
}
#endif
#endif

/**
 * Marks a memory block as non free to reserve the managed space. If the block
 * is bigger than the size, split the block into two blocks.
 *
 * @param size The size of the block to allocate
 * @return The allocated space
 */
void *gmalloc (unsigned size)
{
#ifndef GMEM
	return malloc(size);
#else
	Block *block, *previous = NULL;

	//If head is null, this is the first call of gmalloc and we have to
	//reserve one memory pool (only one time).
	if (head == NULL)
	{
		head = malloc(GMEM_SIZE);
		block = (Block *) head;
		block->next = NULL;
		block->size = GMEM_SIZE - sizeof(Block);
		block->free = 1;
	}

#ifdef DBG
	printf("gmalloc(%d): begin\n", size);
	gprintmem();
#endif

	//We use the first-fit algorithm to find a free block.
	block = findBlockForSize(&previous, size);

	//If no block was found, there is not enough space managed by our library.
	if (block == NULL)
	{
		return NULL;
	}

	//If block size is greater than the size of a block struct, we can split
	//it. We use the size of the struct because a minimal data segment is
	//already included within it (char data).
	if (block->size >= size + sizeof(Block))
	{
		splitBlock(block, size);
	}

	//We mark the block as no longer free.
	block->free = 0;

#ifdef DBG
	gprintmem();
	printf("gmalloc: end\n");
#endif

	//We can return the pointer to the data segment.
	return (void *) (block + 1);
#endif
}

/**
 * Marks the block as free and merges it with the contiguous blocks if they
 * are free to limit the memory fragmentation.
 *
 * @param ptr The memory space to free
 */
void gfree (void *ptr)
{
#ifndef GMEM
	return free(ptr);
#else
	Block *block, *previous = NULL;

#ifdef DBG
	printf("gfree(%p): begin\n", ptr);
	gprintmem();
#endif

	//Find the meta-data associated with the pointed space.
	block = findBlockForAddress(&previous, ptr);

	//If no block was found, the pointer is surely not the result of a
	//previous gmalloc call.
	if (!block)
	{
		return;
	}

	//Mark the block as free.
	block->free = 1;

	//Merge the block with the next block, and the previous block with the
	//block if needed to avoid fragmentation of the free space.
	mergeBlock(block);
	mergeBlock(previous);

#ifdef DBG
	gprintmem();
	printf("gfree: end\n");
#endif
#endif
}

/**
 * Loop over all blocks and displays the status and the size of each of them.
 */
void gprintmem()
{
	Block *block = (Block *) head;
	unsigned metaTotal = 0;
	unsigned dataTotal = 0;

	printf("--- Memory map ---\n");
	if (!head)
	{
		printf("Empty\n");
	}
	else
	{
		do
		{
			if (block->free)
			{
				printf("Free");
			}
			else
			{
				printf("Occupied");
			}

			printf(" block of size %d bytes (meta @%p, data @%p)\n", block->size, block, (block + 1));
			dataTotal += block->size;
			metaTotal += sizeof(Block);
			block = block->next;
		} while (block);
	}
	printf("--- End of memory map (meta: %d bytes, data %d bytes, total %d bytes) ---\n", metaTotal, dataTotal, dataTotal + metaTotal);
}

/**
 * This function uses the first-fit algorithm and returns the first block with
 * enough space.
 */
Block *findBlockForSize(Block **previous, unsigned size)
{
	Block* block = (Block *) head;

	do
	{
		if (block->size >= size && block->free)
		{
			return block;
		}

		*previous = block;
		block = block->next;
	} while (block);

	return NULL;
}

/**
 * This function uses a loop in order to return the previous block too.
 */
Block *findBlockForAddress(Block **previous, void *ptr)
{
	Block *block = (Block *) head;

	//Checks that the pointed space is the result of a previous gmalloc call.
	if (ptr >= block + GMEM_SIZE)
	{
		return NULL;
	}

	//Loop over blocks to find the block where si pointed space is in.
	do
	{
		if (ptr == (block + 1))
		{
			return block;
		}

		*previous = block;
		block = block->next;
	} while (block);

	//The pointer was not pointing the begining of a data segment, but
	//somewhere in the middle of a segment.
	return NULL;
}

/**
 * Inserts a new block after the one given in parameter and rebuilds the
 * linked list.
 */
void splitBlock(Block *block, unsigned size)
{
	Block *newBlock;

	//The new block begins after the block data segment reduced to the specified size.
	//This is correct because the size of a char is a byte, and the size variable is
	//also in bytes (cf. pointer arithmetic).
	newBlock = (Block *) (((char *) (block + 1)) + size);

	newBlock->next = block->next;
	newBlock->size = block->size - size - sizeof(Block);
	newBlock->free = 1;

	block->next = newBlock;
	block->size = size;
}

/**
 * Merges two blocks if they are both free.
 */
void mergeBlock(Block *block)
{
	unsigned size;

	//We can merge two blocks only if the block and its successor exist
	//and if they are both free.
	if (!block || !block->free || !block->next || !block->next->free)
	{
		return;
	}

	//New size is the sum of the two sizes plus the size of the block struct
	//with the included data segment taken in account.
	size = block->size + block->next->size + sizeof(Block);

	block->next = block->next->next;
	block->size = size;
}
