

#define DEBUG 1
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif


//Matchlab

#include <stdio.h>
#include <stdlib.h>

typedef enum {A, B, C, T, CMD_ARG, ERROR_ARG } arg_type;
typedef enum {A_, B_, C_, A_T, B_T, C_T, ERR_CMD} cmd_type;


void basicPrint(char arr[]);
void printArr(char* arr[]);
int arrLen(char arr[], unsigned int typeSize);

arg_type identifyArgType(char* cmdArg);
int detectChar(char x, char arr[], int resultArr[]);
int compareChar(char lhs, char rhs);

void matchA(char arr[], int numStr, int hasT_Flag);
void matchB(char arr[], int numStr, int hasT_Flag);
void matchC(char arr[], int numStr, int hasT_Flag);

int detectUppercase(char*);
cmd_type identifyCMD_Type(int numOfArgs, char* arrOfArgs[] );
void reverse(char* in, char* out);

void add_X_after_Y(char* in, char* out);
int isEven(char a);


//Global variables
int debugOn = 1;

int main(int argc, char* argv[])
{

		DEBUG_PRINT(("Entered Main"));

		cmd_type command = identifyCMD_Type(argc, argv);

		switch(command)
		{
			case A_: 		matchA(argv[2], argc, 0); 	break;
			case B_: 		matchB(argv[2], argc, 0);	break;
			case C_: 		matchC(argv[2], argc, 0); 	break;
			case A_T: 		matchA(argv[3], argc, 1); 	break;
			case B_T: 		matchB(argv[3], argc, 1); 	break;
			case C_T: 		matchC(argv[3], argc, 1); 	break;
			case ERR_CMD: 								break;
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

	for(char1 = arr; char1 != '\0'; char1++)
	{
		if(x == *char1)

		resultArr[counter] = numMatches++;

		counter++;
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


void printArr(char* arr[])
{
	int i;
	for(i = 0; i < arrLen(*arr, 8); i++)
	{
		printf("%s\n", arr[i]);
	}

}

int arrLen(char arr[], unsigned int typeSize)
{

	return sizeof(arr)/typeSize;
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
void matchA(char arr[], int numStr, int hasT_Flag)
{
	if(hasT_Flag == 0)
	{
		printf("yes\n");
		printArr(arr);

	}else
	{

		printArr(arr);

	}

}

void matchB(char arr[], int numStr, int hasT_Flag)
{
	int size = sizeof(arr);	

	if(hasT_Flag == 0)
	{
		printf("yes\n");
		printArr(arr);

	}else
	{

		printArr(arr);

	}
}

void matchC(char arr[], int numStr, int hasT_Flag)
{

	if(hasT_Flag == 0)
	{
		printf("yes\n");
		printArr(arr);

	}else
	{

		printArr(arr);

	}
}

cmd_type identifyCMD_Type(int numOfArgs, char* arrOfArgs[])
{
	if(numOfArgs == 1)
		return ERR_CMD;


	arg_type arg2 = identifyArgType(arrOfArgs[2]);
	arg_type arg1 = identifyArgType(arrOfArgs[1]);

		switch(arg1)
		{
			case A: return arg1 == T? A_T: arg1 == CMD_ARG? A_: ERR_CMD;
			case B: return arg1 == T? B_T: arg1 == CMD_ARG? B_: ERR_CMD;
			case C: return arg1 == T? C_T: arg1 == CMD_ARG? C_: ERR_CMD;
			case T: return arg1 == A? A_T: arg1 == B ? B_T: arg1 == C ? C_T: ERR_CMD;
			case CMD_ARG: return A_;
			case ERROR_ARG: return ERR_CMD;
		}

	return A_;
}


void reverse(char* in, char* out)
{


}

void add_X_after_Y(char* in, char* out)
{


}

int isEven(char a)
{

	return 1;
}


