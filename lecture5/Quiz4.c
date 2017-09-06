
#include <stdio.h>
#include <stdlib.h>

int f(int n) {
   if(n == 1)
      return 1;   // this line
    return n + f(n-1);  }

int main(){
    int result = f(5);
    printf("The factorial of 5 is %d\n", result);
}