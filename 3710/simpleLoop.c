

#include <stdio.h>
#include <stdlib.h>

int sum(char arg[]);

int main(int argc, char* argv[])
{


	printf("%d\n", sum(argv[1]));

	return 0;

}

int sum(char arg[])
{

	int i;

	int sum = 0;

	for(i = 0; i < sizeof(arg); i++)
	{
		sum += arg[i];
	}

	return sum;
}

