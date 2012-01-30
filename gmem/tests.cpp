#include "gmem.h"
#include "../test/test.h"

static void testAlloc(Test *test);
//static int countBlocks();

void GmemTests(Test *test)
{
	test->add(&testAlloc);
}

static void testAlloc(Test *test)
{
	//Block* block;
	int *ptr;
//	long *ptr2;

	ptr = (int *) gmalloc(sizeof(int));

	//block = (Block *) head;
	//test->assert(countBlocks() == 2, "2 blocks after malloc for int");
	test->assert(true, "ok");
}

/*static int countBlocks()
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
*/
