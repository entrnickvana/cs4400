
/*typedef bool;
#define true 1
#define false 0
*/

//Matchlab

#include <stdio.h>
#include <stdlib.h>

typedef enum {A, B, C, T, CMD_ARG, ERROR_ARG } arg_type;

void basicPrint(char arr[]);
arg_type identifyArgType(char* cmdArg);


int main(int argc, char* argv[])
{


		int x = 3;
	/// Check number of Flag arguments
		fprintf(stderr, "Billy");
		printf("Billy%d", x);


		/*
	
		- 45
		a 97
		b 98
		c 99
		t 116

		*/
		int i;
		for(i = 1; i < argc; i++)
		{
			fprintf(stderr, "Wow");
			arg_type ID = identifyArgType(argv[i]);
			if(ID == A)
			{
				fprintf(stderr, "A\n");
			}else if(ID == B)
			{
				fprintf(stderr, "B\n");

			}else if(ID == C)
			{
				fprintf(stderr, "C\n");

			}else if(ID == T)
			{

				fprintf(stderr, "T\n");
			}else if(ID == CMD_ARG)
			{
				fprintf(stderr, "CMD_ARG\n");
			}else
			{
				fprintf(stderr, "Error\n");
			}


		}

		return 0;
}


/*
	Returns an enum indicating the type of flag
*/
arg_type identifyArgType(char* cmdArg)
{


	if(cmdArg[0] == '\0')
	{

		fprintf(stderr, "Error, null character");

		return ERROR_ARG;

	} 
	

	if(cmdArg[0] != '-')
	{

		return CMD_ARG;

	}else if(cmdArg[0] == '-' && cmdArg[1] == 'a')
	{

		return A;

	}else if(*cmdArg == '-' && *(cmdArg +1) == 'b')
	{

		return B;
	}else if(*cmdArg == '-' && *(cmdArg +1) == 'c')
	{

		return C;
	}else if(*cmdArg == '-' && *(cmdArg +1) == 't')
	{

		return T;
	}else
	{
		return CMD_ARG;
	}

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

