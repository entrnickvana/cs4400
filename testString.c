
#include <stdio.h>
#include <stdlib.h>


int main()
{

	char str[] = "my name is jonas";


	int i;
	for(i = 0; str[i] != '\0'; i++)
	{

		printf("%c", str[i]);

	}

	printf("\n");

	return 0;
}