#include <stdlib.h>
#include <stdio.h>
#include "gmem.h"

/**
 * Meta-data of a memory block.
 */
typedef struct Block Block;
struct Block
{
	unsigned size; //size of the block, in bytes
	Block *next;   //pointer to the next block, NULL if none
	char free;     //is the block free?
	char data[1];  //pointer to the first byte of data
};

/**
 * Pointer to the head of allocated space for the gmem module.
 */
static void *head = NULL;

static Block *findBlockForSize(Block **previous, unsigned size);
static Block *findBlockForAddress(Block **previous, void *ptr);
static void splitBlock(Block *block, unsigned size);
static void mergeBlock(Block *block);

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
	if (block->size >= sizeof(Block))
	{
		splitBlock(block, size);
	}

	//We mark the block as no longer free.
	block->free = 0;

	//We can return the pointer to the data segment.
	return (void *) block->data;
#endif
}

void gfree (void *ptr)
{
#ifndef GMEM
	return free(ptr);
#else
	Block *block, *previous = NULL;

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
#endif
}

void gprintmem()
{
	Block *block = head;
	unsigned metaTotal = 0;
	unsigned dataTotal = 0;

	printf("--- Memory map ---\n");
	do
	{
		if (block->free)
		{
			printf("Free");
		}
		else
		{
			printf("Occuped");
		}

		printf(" block of size %d bytes (meta @%p, data @%p)\n", block->size, block, block->data);
		dataTotal += block->size;
		metaTotal += sizeof(Block);
		block = block->next;
	} while (block);
	printf("--- End of memory map (meta: %d bytes, data %d bytes, total %d bytes) ---\n", metaTotal, dataTotal, dataTotal + metaTotal);
}

static Block *findBlockForSize(Block **previous, unsigned size)
{
	Block* block = head;

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
 * Find the metadata of the block associated with the given pointer. This
 * version use a loop in order to return the previous block too.
 *
 * @param Block **previous Last visited block
 * @param void *ptr
 */
static Block *findBlockForAddress(Block **previous, void *ptr)
{
	Block *block = head;

	//Checks that the pointed space is the result of a previous gmalloc call.
	if (ptr >= head + GMEM_SIZE)
	{
		return NULL;
	}

	//Loop over blocks to find the block where si pointed space is in.
	do
	{
		if ((char *) ptr == block->data)
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

static void splitBlock(Block *block, unsigned size)
{
	Block *newBlock;

	//The new block begins after the block data segment reduced to the specified size.
	newBlock = block->data + size;

	newBlock->next = block->next;
	newBlock->size = block->size - size - sizeof(Block);
	newBlock->free = 1;

	block->next = newBlock;
	block->size = size;
}

static void mergeBlock(Block *block)
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
