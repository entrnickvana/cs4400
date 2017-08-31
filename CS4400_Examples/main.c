#include <stdio.h>
#include <stdlib.h>

void multstore(long x, long y, long* dest);

int mult2(long a, long b)
{
	return a*b;	
}

int main()
{
	long r;

	multstore(2, 3, &r);

	printf("%d\n", r);

	return 0;


}