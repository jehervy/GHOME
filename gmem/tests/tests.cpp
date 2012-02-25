#include "tests.h"

#ifdef TESTING

/**
 * Global pointer.
 *
 * @var int*
 */
int *ptr;

/**
 * Helper function to count blocks allocated.
 *
 * @return int
 */
static int countBlocks()
{
	int count;
	Block *block = (Block *) head;

	if (!head)
	{
		return 1;
	}

	count = 0;
	do
	{
		count++;
		block = block->next;
	} while (block);

	return count;
}

/**
 * Tests memory allocation on a basic case.
 *
 * @param test The test framework
 */
void GmemTestsAlloc(Test *test)
{
	Block *block;
	int nbBlocks;

	nbBlocks = countBlocks();
	ptr = (int *) gmalloc(sizeof(int));
	block = (Block *) head;

	test->assert(countBlocks() == nbBlocks + 1, "A new block is created if there is enough space");
	test->assert(block->free == 0 && block->next->free == 1, "The allocated block is marked as not free, the next one is free.");
	test->assert(block->size == sizeof(int), "The allocated block has the size of an int");
}

/**
 * Tests memory liberation on a basic case.
 *
 * @param test The test framework
 */
void GmemTestsFree(Test *test)
{
	Block *block;
	int nbBlocks;
	int *ptr2;
	int *ptr3;

	block = (Block *) head;
	ptr2 = (int *) gmalloc(sizeof(int));
	ptr3 = (int *) gmalloc(sizeof(int));
	nbBlocks = countBlocks();

	gfree(ptr);
	test->assert(countBlocks() == nbBlocks, "When there is no empty block on the left and on the right, nothing is merged...");
	test->assert(block->free == 1, "... but he block is marked as free.");

	gfree(ptr3);
	test->assert(countBlocks() == --nbBlocks, "The block is merged with free space on his right.");

	gfree(ptr2);
	nbBlocks -= 2;
	test->assert(countBlocks() == nbBlocks, "The block is merged with free space on his left AND right.");
	test->assert(block->size == GMEM_SIZE - sizeof(Block), "The block has the size of the initial pool");
}

/**
 * Tests memory overflow on allocation.
 *
 * @param test The test framework
 */
void GmemTestsOverflow(Test *test)
{
	ptr = (int *) gmalloc(GMEM_SIZE + 1);
	test->assert(ptr == NULL, "When tying to allocate to much memory, gmalloc returns NULL.");
}

/**
 * Registers all tests of gmem module.
 *
 * @param test The test framework
 */
void GmemTests(Test *test)
{
	test->add(&GmemTestsAlloc, "Gmem");
	test->add(&GmemTestsFree, "Gmem");
	test->add(&GmemTestsOverflow, "Gmem");
}

#endif

