
#include <stdlib.h>
#include <stdio.h>

int ifstmt(int a, int b);

int loop(int start, int limit);



int main()
{
	loop(666, 111);
	return 0;


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

int loop(int start, int limit) {
  int x = 0;
  int y = 0;
  int i;

  for(i = start; i < limit; i += 4) {
    x += i;
    y++;
  }

  return x + y;
}

int ifstmt(int a, int b) {
 int x = 0;
 if(a < b) {
   x = 2 * a;
   x += b;
 }
 else {
   x = b - 12;
   x -= a;
 }
 return x;
}


