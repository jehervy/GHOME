#ifdef TESTING

#include "tests.h"

int *ptr;
int countBlocks();

void testAlloc(Test *test)
{
	Block *block;

	ptr = (int *) gmalloc(sizeof(int));
	test->assert(countBlocks() == 2, "One new block is created after malloc for int");

	block = (Block *) head;
	test->assert(block->free == 0 && block->next->free == 1, "The new block is not free, the next is free");
	test->assert(block->size == sizeof(int), "The new block has the size of an int");
}

void testFree(Test *test)
{
	Block *block;

	gfree(ptr);
	test->assert(countBlocks() == 1, "One new block is back after free");

	block = (Block *) head;
	test->assert(block->free == 1, "The block is free");
	test->assert(block->size == GMEM_SIZE - sizeof(Block), "The block has the size of the initial pool");
}

int countBlocks()
{
	int count = 0;
	Block *block = (Block *) head;

	do
	{
		count++;
		block = block->next;
	} while (block);

	return count;
}

void GmemTests(Test *test)
{
	test->add(&testAlloc, "Gmem");
	test->add(&testFree, "Gmem");
}

#endif

