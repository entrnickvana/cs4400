
#include <stdio.h>

typedef unsigned char BYTE;

int* permute(unsigned long seed, int argc, int numArr[]){

	unsigned long randi = seed;
	BYTE result; 

	randi = randi << 2;

	int i;

	int nuArr[argc];
	int temp;

	for (i = 0; i < 2000; i++)
	{
			result = result | (BYTE)((randi & 1073742079) >> 30);
			result = result | (BYTE)((randi & 268435711) >> 28);
			randi = (randi << 2) + result;
			result = result << 2;
			if(i % 10 == 0){
			printf("result: %u,    randi: %lu \n", (BYTE)result, randi);
			}
	}

		printf("\n");
	return 0;

}