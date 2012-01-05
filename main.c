#include <stdlib.h>
#include <stdio.h>
#include "gmem/gmem.h"

int main()
{
	int *ptr;
	long *ptr2;

	ptr = gmalloc(sizeof(int));
	printf("main: ptr allocated\n");
	gprintmem();
	printf("main: ptr freed\n");
	gfree(ptr);
	gprintmem();

	ptr = gmalloc(sizeof(int));
	printf("main: ptr allocated\n");
	gprintmem();
	ptr2 = gmalloc(sizeof(long));
	printf("main: ptr2 allocated\n");
	gprintmem();
	gfree(ptr);
	printf("main: ptr freed\n");
	gprintmem();
	gfree(ptr2);
	printf("main: ptr2 freed\n");
	gprintmem();

	printf("main: terminated\n");

	return 0;
}
