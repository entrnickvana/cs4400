
#include <stdio.h>
#include <stdlib.h>

long mult2(long a, long b);

void multstore(long x, long y, long* dest)
{
	long t = mult2(x, y);
	*dest = t;

}
