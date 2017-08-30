
#include <stdlib.h>
#include <stdio.h>

char ctest(int a, int b);

int main()
{


	return 0;
}

char ctest(int a, int b)
{
	char t1 = a >= b;
	char t2 = (unsigned int)b <= (unsigned int)a;
	return t1 + t2;
}