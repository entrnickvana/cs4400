
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

int detectChar(char x, char arr[]);
int compareChar(char lhs, char rhs);


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



int detectChar(char x, char arr[], int resultArr[])
{
	char* char1;
	int counter = 0;
	int numMatches = 0;

	for(char1 = arr; char1 != "\0"; char1++)
	{
		if(x == *char1)
		resultArr[counter] = numMatches++;

		counter++
	}

	return numMatches;
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

int arrLen(char arr[])
{

	return sizeof(arr)/8;
}


/*

	-a mode
	Match a sequence of the following, with nothing else before, after, or in between:
	between 4 and 5 repetitions (inclusive) of the letter “g”;
	exactly one “=”;
	any odd number of repetitions of the letter “z”;
	exactly one “_”; and
	between 1 and 3 (inclusive) decimal digits.
	For matches, perform the following conversion:
	reverse all of the characters.
	http://crystalis.cs.utah.edu:5577/assignment?uid=u0682219
	1/58/21/2017
	Match Assignment for u0682219
	Example arguments that match, followed by their conversions:
	gggg=z_185
	→ 581_z=gggg
	gggg=z_552
	→ 255_z=gggg
	gggg=z_838
	→ 838_z=gggg
	gggg=zzzzz_533
	→ 335_zzzzz=gggg
	gggg=zzzzzzzzz_969
	→ 969_zzzzzzzzz=gggg


*/
void matchA(char arr[])
{
	int size = sizeof(arr);

}

