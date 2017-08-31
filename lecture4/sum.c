
#include <stdio.h>
#include <stdlib.h>

int sum (int* a, int len);

int main()
{

	int arr[3];
	int length = 3;

	int result = sum(arr, length);

	cout << result  << endl;

}


int sum (int* a, int len)
{

	int index = 0;

	int result1 = 0;

	loop: 
	if(index >= len)
		goto result;

		result1 += a[index]; 
		index++;
	goto loop;

	result:

	return result1;
}


