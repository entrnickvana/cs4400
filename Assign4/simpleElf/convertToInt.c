
#include <stdlib.h>

void convertToInt(int* argc, char* arr[], int nuArr[]){

	int i;

	int size = *argc;

	for (i = 0; i < size - 1; i++)
	{
		nuArr[i] = atoi(arr[i + 1]);
	}

}