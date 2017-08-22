
/*typedef bool;
#define true 1
#define false 0
*/

//Matchlab

#include <stdio.h>

void basicPrint(char arr[]);

int main(int argc, char* argv[])
{

	/// Check number of Flag arguments

		/*

		- 45
		a 97
		b 98
		c 99
		t 116

		*/


		//Case 1: 2 Flag Arguments
		if(argc < 3) // No -t flag
		{
			basicPrint(argv[0]);			
			basicPrint(argv[1]);			
			//basicPrint(argv[2]);


		}else if(argc == 3)  //Case 2: 3 Flag Arguments
		{



		}	

		return 0;




}

void basicPrint(char* arr)
{
	char* character;
	character = NULL;


	for(character = arr; *character != '\0'; character++)
	{
		printf("%c", *character);
	}


	printf("\n");

}

