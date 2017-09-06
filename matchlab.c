

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
typedef enum {UPPER, DEC, LOWER, OTHER_ASCII} ascii_type;


void basicPrint(char arr[]);
void printArr(char* arr[], int sizeOfArr);
int arrLen(char arr[], int typeSize);
int arrLenPtr(char* arr[]);
int strLength(char* arr);
int matchA_Arg(char arr[]);
int matchRange(char x[], int rangeLength, char strToCmpr[]);
int matchRangeOfType( ascii_type x, int rangeLength, char strToCmpr[]);

arg_type identifyArgType(char* cmdArg);
int detectChar(char x, char arr[], int resultArr[]);
int compareChar(char lhs, char rhs);

void matchA(char* arr[], int numStr, int hasT_Flag);
void matchB(char* arr[], int numStr, int hasT_Flag);
void matchC(char* arr[], int numStr, int hasT_Flag);

ascii_type detectAsciiType(char x);
cmd_type identifyCMD_Type(int numOfArgs, char* arrOfArgs[] );
void reverse(char* in, char* out);

void add_X_after_Y(char* in, char* out);
int isEven(char a);


//Global variables
int debugOn = 1;

int main(int argc, char* argv[])
{

		DEBUG_PRINT(("Entered Main\n"));

		int x1 = strLength(argv[2]);
		int x2 =  strLength(argv[3]);
		printf("Length of arg1 = %d", x1); 
		printf("%s\n");
		printf("Length of arg2 = %d", x2);
		printf("%s\n");

		if(matchRange(argv[3], 3, argv[2]) == 1)
		DEBUG_PRINT(("%s is a match for 4 DEC\n", argv[2]));




		/*
		if(detectAsciiType('1') == DEC)
		{
			DEBUG_PRINT(("Correct\n"));
		}

		if(detectAsciiType('A') == UPPER)
			DEBUG_PRINT(("A is an Upper\n"));

		if(detectAsciiType('z') == LOWER)
			DEBUG_PRINT(("z is a LOWER\n"));

		if(detectAsciiType('.') == OTHER_ASCII)
			DEBUG_PRINT((". is a OTHER_ASCII\n"));
		*/


		/*
		char charArr[] = "Billy Bob";

		int l = strLength(charArr) - 1;
		int myArr[l];

		int result1 = detectChar('y', charArr, myArr);

		printf("%d\n", result1 );
		*/

		//range test

		
		/*
		int a1 = 2;
		int b2 = 3;
		int c3 = 4;
		int* arrOfPtr[3] = {&a1, &b2, &c3};
*/

		//printf("The size of type char* arrOfPtr = %d\n", (int)sizeof(arrOfPtr)/( (int)sizeof(int*)));
		//printf(" the size of a char is %d\n", (int) sizeof(int*));

		cmd_type command = identifyCMD_Type(argc, argv);

		switch(command)
		{
			case A_: 		matchA(argv, argc, 0); 	break;
			case B_: 		matchB(argv, argc, 0);	break;
			case C_: 		matchC(argv, argc, 0); 	break;
			case A_T: 		matchA(argv, argc, 1); 	break;
			case B_T: 		matchB(argv, argc, 1); 	break;
			case C_T: 		matchC(argv, argc, 1); 	break;
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
	int counter = 0;
	int numMatches = 0;

	int i;
	for(i = 0; arr[i] != '\0'; i++)
	{
		if(x == arr[i])
		resultArr[counter] = numMatches++;

		counter++;
	}

	resultArr[0] = numMatches;



	return numMatches;
}



void basicPrint(char* arr)
{
	char* character;
	character = NULL;


	for(character = arr; *character != 0; character++)
	{
		printf("%c", *character);
	}


	printf("\n");

}


void printArr(char* arr[], int sizeOfArr)
{
	int i;

	for(i = 0; i < sizeOfArr; i++)
	{
		printf("%s\n", arr[i]);
	}

}

/*

void printLength(int intArray[])
{
    printf("Length of parameter: %d\n", (int)( sizeof(intArray) / sizeof(intArray[0]) ));
}

*/


int arrLen(char arr[], int typeSize)
{

	return (int)(sizeof(arr))/typeSize;
}

int arrLenPtr(char* arr[])
{

	return (int)(sizeof(arr))/(int)sizeof(char*);
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
void matchA(char* arr[], int numStr, int hasT_Flag)
{
	if(hasT_Flag == 0)
	{
		DEBUG_PRINT(("Type A_\n"));
		printArr(arr, numStr);

		int i;
		for(i = 2; i < numStr; i++)
		{
			if(! (matchA_Arg(arr[i])))
				return;

			printf("yes\n");
		}




	}else
	{
		DEBUG_PRINT(("Type A_T\n"));
		printArr(arr, numStr);

		int i;
		for(i = 3; i < numStr; i++)
		{
			if(! (matchA_Arg(arr[i])))
				return;

			printf("yesT\n");
		}



	}

}

int matchA_Arg(char arr[])
{
	
	// check for 4 or 5 g's
	int check5 = matchRange("ggggg", 5, arr);
	int check4 = matchRange("gggg", 4, arr);

	if(!(check4 || check5))
		return 0;
	
// int detectChar(char x, char arr[], int resultArr[]);
	int len = strLength(arr) - 1;

	int resultArr[len];

	int result = detectChar('z', arr, resultArr);

	int mask = 1;

// odd number of z's
	if((result & mask) != 1)
		return 0;

	int underTestArr[len];
	result = detectChar('_', arr, underTestArr);

	if(result != 1)
		return 0;

	int equalTestArr[len];
	result = detectChar('=', arr, equalTestArr);

	if(result != 1)
		return 0;

	return 1;

}

int matchRangeOfType( ascii_type X, int rangeLength, char strToCmpr[])
{
	
	int itr;
	int strLen = strLength(strToCmpr);

	ascii_type x[strLen];

	int k;
	for(k = 0; k < strLen; k++)
	{
		if( k < rangeLength)
		x[k] = X;
		else
		x[k] = OTHER_ASCII;
	}



	int matches = 0;
	int nonMatches = 0;

	if(strLen < rangeLength)
		return 0;

	for(itr = 0; itr < (strLen - rangeLength) + 1; itr++)
	{
		switch(rangeLength)
		{
			case 10: detectAsciiType(strToCmpr[9 + itr]) == x[9] ? matches++:nonMatches++;
			case  9: detectAsciiType(strToCmpr[8 + itr]) == x[8] ? matches++:nonMatches++;
			case  8: detectAsciiType(strToCmpr[7 + itr]) == x[7] ? matches++:nonMatches++;
			case  7: detectAsciiType(strToCmpr[6 + itr]) == x[6] ? matches++:nonMatches++;
			case  6: detectAsciiType(strToCmpr[5 + itr]) == x[5] ? matches++:nonMatches++;
			case  5: detectAsciiType(strToCmpr[4 + itr]) == x[4] ? matches++:nonMatches++;
			case  4: detectAsciiType(strToCmpr[3 + itr]) == x[3] ? matches++:nonMatches++;
			case  3: detectAsciiType(strToCmpr[2 + itr]) == x[2] ? matches++:nonMatches++;
			case  2: detectAsciiType(strToCmpr[1 + itr]) == x[1] ? matches++:nonMatches++;
			case  1: detectAsciiType(strToCmpr[0 + itr]) == x[0] ? matches++:nonMatches++;

		}

		// Field is at first iteration, check whether one char past field is not type X
		if(itr == (strLen - rangeLength) && itr != 0)
		{
			if(detectAsciiType(strToCmpr[itr - 1]) != X && matches == rangeLength)
				return 1;

		}else if( itr != 0 && itr != (strLen - rangeLength))	// Field is in a middle iteration, check whether one before and one after are not type X
		{
			if( (detectAsciiType(strToCmpr[rangeLength + 1]) != X) 
				&& (detectAsciiType(strToCmpr[itr - 1]) != X)  && matches == rangeLength)
				return 1;
		}else if( itr == 0 && itr != (strLen - rangeLength))
		{
			if(detectAsciiType(strToCmpr[rangeLength]) != X && matches == rangeLength)
				return 1;
		}
		else
		{
			if(matches == rangeLength)
				return 1;

		}

		matches = 0; nonMatches = 0;		

		// Field is in last iteration, check whether on char before field is not type X
						

	}

	return 0;

}


int matchRange(char x[], int rangeLength, char strToCmpr[])
{
	int itr;

	int strLen = strLength(strToCmpr) - 1;

	int matches = 0;
	int nonMatches = 0;

	if(strLen < rangeLength)
		return 0;

	for(itr = 0; itr < (strLen - rangeLength); itr++)
	{
		switch(rangeLength)
		{
			case 10: strToCmpr[9 + itr] == x[9] ? matches++:nonMatches++;
			case  9: strToCmpr[8 + itr] == x[8] ? matches++:nonMatches++;
			case  8: strToCmpr[7 + itr] == x[7] ? matches++:nonMatches++;
			case  7: strToCmpr[6 + itr] == x[6] ? matches++:nonMatches++;
			case  6: strToCmpr[5 + itr] == x[5] ? matches++:nonMatches++;
			case  5: strToCmpr[4 + itr] == x[4] ? matches++:nonMatches++;
			case  4: strToCmpr[3 + itr] == x[3] ? matches++:nonMatches++;
			case  3: strToCmpr[2 + itr] == x[2] ? matches++:nonMatches++;
			case  2: strToCmpr[1 + itr] == x[1] ? matches++:nonMatches++;
			case  1: strToCmpr[0 + itr] == x[0] ? matches++:nonMatches++;

		}

		if(matches == rangeLength)
			return 1;

		matches = 0; nonMatches = 0;

	}

	return 0;

}

void matchB(char* arr[], int numStr, int hasT_Flag)
{


	if(hasT_Flag == 0)
	{
		DEBUG_PRINT(("Type B_\n"));		
		printf("yes\n");
		printArr(arr, numStr);

	}else
	{
		DEBUG_PRINT(("Type B_T\n"));
		printArr(arr, numStr);

	}
}

void matchC(char* arr[], int numStr, int hasT_Flag)
{

	if(hasT_Flag == 0)
	{
		DEBUG_PRINT(("Type C_\n"));
		printf("yes\n");
		printArr(arr, numStr);

	}else
	{
		DEBUG_PRINT(("Type C_T\n"));
		printArr(arr, numStr);

	}
}

cmd_type identifyCMD_Type(int numOfArgs, char* arrOfArgs[])
{
	if(numOfArgs == 1)
		return ERR_CMD;

	arg_type arg1 = identifyArgType(arrOfArgs[1]);
	arg_type arg2 = identifyArgType(arrOfArgs[2]);

		switch(arg1)
		{
			case A: return arg2 == T? A_T: arg2 == CMD_ARG? A_: ERR_CMD;
			case B: return arg2 == T? B_T: arg2 == CMD_ARG? B_: ERR_CMD;
			case C: return arg2 == T? C_T: arg2 == CMD_ARG? C_: ERR_CMD;
			case T: return arg2 == A? A_T: arg2 == B ? B_T: arg2 == C ? C_T: ERR_CMD;
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

ascii_type detectAsciiType(char x)
{

	if(	x == 'A' || x == 'B' || x == 'C' || x == 'D' || x == 'E' || x == 'F' || x == 'G' || x == 'H' ||
	   	x == 'I' || x == 'J' || x == 'K' || x == 'L' || x == 'M' || x == 'N' || x == 'O' || x == 'P' ||
		x == 'Q' || x == 'R' || x == 'S' || x == 'T' || x == 'U' || x == 'V' || x == 'W' || x == 'X' ||
		x == 'Y' || x == 'Z'
	  ) return UPPER;


	if(	x == 'a' || x == 'b' || x == 'c' || x == 'c' || x == 'e' || x == 'f' || x == 'g' || x == 'h' ||
	   	x == 'i' || x == 'j' || x == 'k' || x == 'l' || x == 'm' || x == 'n' || x == 'o' || x == 'p' ||
		x == 'q' || x == 'r' || x == 's' || x == 't' || x == 'u' || x == 'v' || x == 'w' || x == 'x' ||
		x == 'y' || x == 'z'
	  ) return LOWER;

	if(	x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' ||
		x == '9'
	  ) return DEC;

	return OTHER_ASCII;
}

int isEven(char a)
{

	return 1;
}

int strLength(char* arr)
{

	char* character;

	int counter = 0;

	for(character = arr; *character != '\0'; character++)
	{
		counter++;
	}

	return counter;

}


