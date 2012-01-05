#include <stdlib.h>
#include <stdio.h>
#include "gmem.h"

typedef struct Block Block;
struct Block
{
	Block *next;
	unsigned size;
	int free;
	char data[1];
};

static void *head;

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
		head = malloc(GMEMSIZE);
		block = (Block *) head;
		block->next = NULL;
		block->size = GMEMSIZE - sizeof(Block);
		block->free = 1;
	}

	block = findBlockForSize(&previous, size);
	if (block == NULL)
	{
		return NULL;
	}

	if (block->size >= sizeof(Block))
	{
		splitBlock(block, size);
	}
	block->free = 0;

	return (void *) block->data;
#endif
}

void gfree (void *ptr)
{
#ifndef GMEM
	return free(ptr);
#else
	Block *block, *previous = NULL;

	block = findBlockForAddress(&previous, ptr);

	if (block == NULL)
	{
		return;
	}

	//Mark the block as free.
	block->free = 1;

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
	} while (block != NULL);
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
	} while (block != NULL);

	return NULL;
}

static Block *findBlockForAddress(Block **previous, void *ptr)
{
	Block *block = head;

	do
	{
		if ((char *) ptr == block->data)
		{
			return block;
		}

		*previous = block;
		block = block->next;
	} while (block != NULL);

	return NULL;
}

static void splitBlock(Block *block, unsigned size)
{
	Block *newBlock;
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

	if (!block || !block->free || !block->next || !block->next->free)
	{
		return;
	}

	size = block->size + block->next->size + sizeof(Block);
	block->next = block->next->next;
	block->size = size;
}
