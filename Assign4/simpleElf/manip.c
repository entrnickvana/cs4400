
#include<stdlib.h>
#include<stdio.h>

#include "myMax.h"
#include "convertToInt.h"
#include "permute.h"


int main(int argc, char* argv[]){

	// Given an arry of chars representing ints

	printf("Beginning: argc = %d\n", argc);

	int* length = malloc(sizeof(int));
	*length = argc;

	int* numsArr = malloc(argc*sizeof(int));
	convertToInt(length, argv, numsArr);

	int i;

	for (i = 0; i < argc - 1; i++)
	{
		printf("%d", ((int*)numsArr)[i]);
	}

	// Find max

	int result = myMax(*length, numsArr);
	printf("The max is: %d\n", result);

	// Permute and output
	int* numsToPermute = malloc(argc*sizeof(int));	
	permute(1217, *length, numsToPermute);

	// sum

	return 0;

}

