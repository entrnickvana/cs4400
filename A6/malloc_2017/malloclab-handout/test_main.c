

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"

int main()
{

	mm_init();
	void* ptr = mm_malloc(1);

	return 0;
}