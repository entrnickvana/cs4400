

//#define DEBUG 1
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
int matchRange(char x[], int rangeLength, char strToCmpr[]);
int matchRangeOfType( ascii_type x, int rangeLength, char strToCmpr[]);

arg_type identifyArgType(char* cmdArg);
int detectChar(char x, char arr[], int resultArr[]);
int compareChar(char lhs, char rhs);

void matchA(char* arr[], int numStr, int hasT_Flag);
int matchA_Arg(char arr[]);
void matchB(char* arr[], int numStr, int hasT_Flag);
int matchB_Arg(char arr[]);
void matchC(char* arr[], int numStr, int hasT_Flag);
int matchC_Arg(char arr[]);

ascii_type detectAsciiType(char x);
cmd_type identifyCMD_Type(int numOfArgs, char* arrOfArgs[] );
void reverse(char arr[]);

void add_X_after_Y(char* in, char* out);
int isEven(char a);
int isEvenNum(int a);
int includedChars(char included[], int Upper, int Lower, int Dec, char arr[]);
void printC(char arr[]);
void printB(char arr[]);

//Global variables
int debugOn = 1;

int main(int argc, char* argv[])
{

		DEBUG_PRINT(("Entered Main\n"));


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

		//fprintf(stderr, "Error, null character");

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
		DEBUG_PRINT(("%c", *character));
	}


	DEBUG_PRINT(("\n"));

}


void printArr(char* arr[], int sizeOfArr)
{
	int i;

	for(i = 0; i < sizeOfArr; i++)
	{
		DEBUG_PRINT(("%s\n", arr[i]));
	}

	DEBUG_PRINT(("\n\n\n"));

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




void matchA(char* arr[], int numStr, int hasT_Flag)
{
	if(hasT_Flag == 0)
	{
		DEBUG_PRINT(("Type A_\n"));
		printArr(arr, numStr);


		int i;
		for(i = 2; i < numStr; i++)
		{
			if((matchA_Arg(arr[i])))
			{

				printf("yes\n");
				DEBUG_PRINT(("$$$  %s\n", arr[i]));
				DEBUG_PRINT(("////////////////////////////////////////\n\n\n"));

			}
		}




	}else
	{
		DEBUG_PRINT(("Type A_T\n"));
		printArr(arr, numStr);

		int i;
		for(i = 3; i < numStr; i++)
		{
			if((matchA_Arg(arr[i])))
			{		

				reverse(arr[i]);
				DEBUG_PRINT(("$$$  %s\n", arr[i]));
				DEBUG_PRINT(("////////////////////////////////////////\n\n\n"));
			}
		}



	}

}

int matchA_Arg(char arr[])
{
	
	// check for 4 or 5 g's
	int check5 = 0;
	int check4 = 0;

	check4 = matchRange("gggg", 4, arr);
	check5 = matchRange("ggggg", 5, arr);

	if((check5 == 0 && check4 == 0))
	{
		DEBUG_PRINT(("Failed g test, check5 = %d, check 4 = %d\n", check5, check4));
		return 0;
	}
	
// int detectChar(char x, char arr[], int resultArr[]);
	int len = strLength(arr) - 1;

	int resultArr[len];

	int result = detectChar('z', arr, resultArr);

	int mask = 1;

// odd number of z's
	if((result & mask) != 1)
	{
		DEBUG_PRINT(("Failed z test\n"));
		return 0;
	}

	int underTestArr[len];
	result = detectChar('_', arr, underTestArr);

	if(result != 1)
	{
		DEBUG_PRINT(("Failed _ test\n"));
		return 0;
	}

	int equalTestArr[len];
	result = detectChar('=', arr, equalTestArr);

	if(result != 1)
	{
		DEBUG_PRINT(("Failed = test\n"));
		return 0;
	}

	int resultArr5[len];
	int result5 = matchRangeOfType(DEC, 1, arr);
	int result6 = matchRangeOfType(DEC, 2, arr);
	int result7 = matchRangeOfType(DEC, 3, arr);

	if(result5 == 0 || result6 == 0 || result7 == 0)
	{
		DEBUG_PRINT(("Failed DEC test\n"));
		return 0;
	}

	if(includedChars("g=z_", 1,1,1,arr) <= 0)
	{
		DEBUG_PRINT(("A: Failed Exlusion test\n"));
		return 0;
	}

	return 1;

}



/*
any even number (including zero) repetitions of the letter “m”;
exactly one “:”;
an odd number of uppercase letters — call this sequence X;
1 or more repetitions of the letter “r”;
exactly one “:”;
the same characters as the even-positioned characters in X; and
between 1 and 3 (inclusive) decimal digits.
*/
void matchB(char* arr[], int numStr, int hasT_Flag)
{


if(hasT_Flag == 0)
	{
		DEBUG_PRINT(("Type B_\n"));
		printArr(arr, numStr);

		int i;
		for(i = 2; i < numStr; i++)
		{
			if((matchB_Arg(arr[i])))
			{

			printf("yes\n");
				DEBUG_PRINT(("$$$  %s\n", arr[i]));
				DEBUG_PRINT(("////////////////////////////////////////\n\n\n"));

			}

		}

	}else
	{
		DEBUG_PRINT(("Type B_T\n"));
		printArr(arr, numStr);

		int i;
		for(i = 3; i < numStr; i++)
		{
			if((matchB_Arg(arr[i])))
			{	

				printB(arr[i]);
				DEBUG_PRINT(("$$$  %s\n", arr[i]));
				DEBUG_PRINT(("////////////////////////////////////////\n\n\n"));
			}
		}



	}
}


/*
any even number (including zero) repetitions of the letter “m”;
exactly one “:”;
an odd number of uppercase letters — call this sequence X;
1 or more repetitions of the letter “r”;
exactly one “:”;
the same characters as the even-positioned characters in X; and
between 1 and 3 (inclusive) decimal digits.

int detectChar(char x, char arr[], int resultArr[])

*/
int matchB_Arg(char arr[])
{

	// any even number (including zero) repetitions of the letter “m” ///////////////////////
	int length_of_string = strLength(arr);

	int resultArr[length_of_string];

	int result = detectChar('m', arr, resultArr);

	if(isEvenNum(result) != 1)
	{
		DEBUG_PRINT(("B: result = %d\n", result));
		DEBUG_PRINT(("B: Failed m test\n"));
		
	}

	//	an odd number of uppercase letters — call this sequence X ////////////////////////////
	int resultArr2;

	int i;
	int lengthOfMatch = 0;
	int matchIndex = -1;

		DEBUG_PRINT(("Length of String %d\n", length_of_string));

	int startIndex = length_of_string;

	for(i = startIndex; i >= 1; i = i - 1)
	{
		if(!isEvenNum(i))
		{
			matchIndex = matchRangeOfType(UPPER, i, arr);
			if(matchIndex >= 1)
			{

				lengthOfMatch = i;

				DEBUG_PRINT(("I: %d\n", i));
				i = 0;
			}
		}

		DEBUG_PRINT(("i = %d, matchIndex = %d\n" , i, matchIndex));

	}

	int lengthOf_X_Sequence = lengthOfMatch;

	if(lengthOf_X_Sequence == 0)
	{
		DEBUG_PRINT(("B: Failed X Sequence test\n"));
		DEBUG_PRINT(("B: X Sequence Match = %d\n", lengthOf_X_Sequence));
		return 0;		
	}


	// exactly one : /////////////////////////////////////////////////////////////////////////

	int resultArr3[length_of_string];

	int result3 = detectChar(':', arr, resultArr3);

	if(result3 != 2)
	{
		DEBUG_PRINT(("B: Failed : test\n"));
		return 0;
	}

	// 1 or more repetitions of the letter “r”  //////////////////////////////////////////////////

	int resultArr4[length_of_string];

	int result4 = detectChar('r', arr, resultArr4);

	if(result4 < 1)
	{
		DEBUG_PRINT(("B: Failed r test\n"));
		DEBUG_PRINT(("B: result4 = %d\n", result4));
		return 0;		
	}

	//  the same characters as the even-positioned characters in X //////////////////////////////////
	int lengthOfEvens = 0;

	if(isEvenNum(lengthOfMatch) == 1)
	{
		lengthOfEvens = (int)(((unsigned)lengthOfMatch) >> 1);

	}else if(isEvenNum(matchIndex) == 1 && isEvenNum(lengthOfMatch) == 0)
	{
		lengthOfEvens = (int)((((unsigned)lengthOfMatch) + 1) >> 1);
	}else
	{
		lengthOfEvens = (int)((((unsigned)lengthOfMatch) - 1) >> 1);
	}

	if(lengthOfEvens == 0)
		lengthOfEvens = 1;

	DEBUG_PRINT(("Length of evens: %d\n", lengthOfEvens));

	char evens_Of_X[lengthOfEvens];
	int nuIndex = 0;

	int k;
	for(k = matchIndex; k < lengthOfMatch + matchIndex; k = k + 2)
	{

			evens_Of_X[nuIndex++] =  arr[k];
			DEBUG_PRINT(("The char: %c\n", arr[k]));


	}

	int resultEvens = matchRange(evens_Of_X, nuIndex, arr);

	if(resultEvens < 0)
	{

		DEBUG_PRINT(("B: Failed Evense test\n"));
		DEBUG_PRINT(("B: MatchIndex %d\n", matchIndex));
		DEBUG_PRINT(("Range length = %d\n", lengthOfMatch));
		DEBUG_PRINT(("Evanes string = %s\n", arr));
		DEBUG_PRINT(("Evanes string = %s\n", evens_Of_X));
		return 0;		
	}

	

	/// Rang 1- 3 Decimal gigits

	int result5 = matchRangeOfType(UPPER, 1, arr);
	int result6 = matchRangeOfType(UPPER, 2, arr);
	int result7 = matchRangeOfType(UPPER, 3, arr);

	if(result5 < 0 && result6 < 0 && result7 < 0)
	{

		DEBUG_PRINT(("B: Failed 3 digit\n"));	
		DEBUG_PRINT(("B: 1: %d  2:%d  3: %d digit\n", result5, result6, result7));
		return 0;

	}

	if(includedChars("mr:", 1,1,1,arr) <= 0)
	{
		DEBUG_PRINT(("B: Failed Exlusion test\n"));
		return 0;
	}


	return 1;
	
}

void matchC(char* arr[], int numStr, int hasT_Flag)
{

	
	if(hasT_Flag == 0)
		{
			DEBUG_PRINT(("Type C_\n"));
			printArr(arr, numStr);

			int i;
			for(i = 2; i < numStr; i++)
			{
				if((matchC_Arg(arr[i])))
				{

				printf("yes\n");
				DEBUG_PRINT(("$$$  %s\n", arr[i]));
				DEBUG_PRINT(("////////////////////////////////////////\n\n\n"));
				}


			}

		}else
		{
			DEBUG_PRINT(("Type C_T\n"));
			printArr(arr, numStr);

			int i;
			for(i = 3; i < numStr; i++)
			{
				if((matchC_Arg(arr[i])))
				{

				printC(arr[i]);
				DEBUG_PRINT(("$$$  %s\n", arr[i]));
				DEBUG_PRINT(("////////////////////////////////////////\n\n\n"));
				}
			}



		}
	
}


/*
4 or more repetitions of the letter “i”;
exactly one “,”;
an odd number of uppercase letters — call this sequence X;
any odd number of repetitions of the letter “s”;
exactly one “_”;
between 1 and 3 (inclusive) decimal digits; and
the same characters as X repeated 3 times.

// int detectChar(char x, char arr[], int resultArr[])

*/
int matchC_Arg(char arr[])
{
	
	int length_of_string;
	length_of_string = strLength(arr);
	int resultArr1[length_of_string];
	int r1;
	r1 = detectChar('i', arr, resultArr1);

	

	if(r1 < 4)
	{
		DEBUG_PRINT(("C Failed i test: result %d\n", r1));
	}
	int resultArr[length_of_string];
	int result = detectChar(',', arr, resultArr);

	if(result != 1)
	{

		DEBUG_PRINT(("C: failed , test ',':result = %d\n", result));
	}


	// Odd sequence of upper

	int resultArr2;

	int i;
	int lengthOfMatch = 0;
	int matchIndex = -1;

		DEBUG_PRINT(("C : Length of String %d\n", length_of_string));

	int startIndex = 0;

	if(isEvenNum( length_of_string) == 1)
		startIndex = length_of_string -1;
	else
		startIndex = length_of_string;

	for(i = 1; i < startIndex; i = i + 2)
	{
		matchIndex = matchRangeOfType(UPPER, i, arr);
		if(matchIndex >= 1)
		{
			lengthOfMatch = i;

			DEBUG_PRINT(("I: %d\n", i));
			i = startIndex;

		}

		DEBUG_PRINT(("C: i = %d, matchIndex = %d\n" , i, matchIndex));

	}

//	printf(" X = %s\n", arr + matchIndex);

	int lengthOf_X_Sequence = lengthOfMatch;

	if(lengthOf_X_Sequence == 0)
	{
		DEBUG_PRINT(("C: Failed X Sequence test\n"));
		DEBUG_PRINT(("C: X Sequence Match = %d\n", lengthOf_X_Sequence));
		return 0;
	}


	// any odd number of repititions of the letter s
	

	int j;
	int result5 = -1;
	for(j = length_of_string; j > 0; j = j - 1)
	{
			if(isEvenNum(j))
			{
				char tempChar[j];

				int z;
				for(z = 0; z < j; z++)
					tempChar[z] = 's';

				result5 = matchRange(tempChar, j , arr);

				if( result5 >= 1)
				{
					j = 0;
				}
			}
	}

	if(result5 <= 0)
	{
		DEBUG_PRINT(("Failed s test\n"));
		return 0;
	}


	// Inclusive 1-3 Dec
	int resultArr5[length_of_string];
	int result9 = matchRangeOfType(DEC, 1, arr);
	int result6 = matchRangeOfType(DEC, 2, arr);
	int result7 = matchRangeOfType(DEC, 3, arr);

	if(result9 == 0 || result6 == 0 || result7 == 0)
	{
		DEBUG_PRINT(("C: Failed DEC test\n"));
		return 0;
	}


	int lengthOf_3X = lengthOf_X_Sequence + lengthOf_X_Sequence + lengthOf_X_Sequence;

	DEBUG_PRINT(("Length of sequence 3x = %d\n", lengthOf_3X));
	char arr3[lengthOf_3X];

	for(i = 0 ; i < lengthOfMatch; i++)
	{
		if((matchIndex + i) < length_of_string)
		{
			arr3[i] = arr[matchIndex + i];
			arr3[i + lengthOf_X_Sequence] = arr[matchIndex + i];
			arr3[i + lengthOf_X_Sequence + lengthOf_X_Sequence] = arr[matchIndex + i];
		}
	}


	int result8 = matchRange(arr3 + 0, lengthOf_3X, arr);

	if(result8 == 0)
	{

		DEBUG_PRINT(("Failed 3X test\n"));
		DEBUG_PRINT(("Failed 3X = %s \n", arr3));
		return 0;
	
	}

	if(includedChars("is_,", 1,1,1,arr) <= 0)
	{
		DEBUG_PRINT(("C: Failed Exlusion test\n"));
		return 0;
	}

	return 1;
}

int includedChars(char included[], int Upper, int Lower, int Dec, char arr[])
{
	
	int k;
	int l = strLength(included);
	int stringL = strLength(arr);
	int resultArr[l];
	int result = -1;


		for(k = 0; k < stringL; k++)
		{
				if(detectChar(arr[k], included, resultArr) <= 0)
				{

					/*
					if(	   ((detectAsciiType(arr[k]) != UPPER) && Upper)
						|| ((detectAsciiType(arr[k]) != LOWER) && Lower)
						|| ((detectAsciiType(arr[k]) != DEC) && Dec)
					*/
					if((detectAsciiType(arr[k]) == OTHER_ASCII))
					{
							DEBUG_PRINT(("Failed Exclusion Test, %c not incl!uded\n", arr[k]));
							return 0;
					}
				}

		}



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
			case 20: detectAsciiType(strToCmpr[19 + itr]) == x[19] ? matches++:nonMatches++;
			case 19: detectAsciiType(strToCmpr[18 + itr]) == x[18] ? matches++:nonMatches++;
			case 18: detectAsciiType(strToCmpr[17 + itr]) == x[17] ? matches++:nonMatches++;
			case 17: detectAsciiType(strToCmpr[16 + itr]) == x[16] ? matches++:nonMatches++;
			case 16: detectAsciiType(strToCmpr[15 + itr]) == x[15] ? matches++:nonMatches++;
			case 15: detectAsciiType(strToCmpr[14 + itr]) == x[14] ? matches++:nonMatches++;
			case 14: detectAsciiType(strToCmpr[13 + itr]) == x[13] ? matches++:nonMatches++;
			case 13: detectAsciiType(strToCmpr[12 + itr]) == x[12] ? matches++:nonMatches++;
			case 12: detectAsciiType(strToCmpr[11 + itr]) == x[11] ? matches++:nonMatches++;
			case 11: detectAsciiType(strToCmpr[10 + itr]) == x[10] ? matches++:nonMatches++;
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
				return itr;

		}else if( itr != 0 && itr != (strLen - rangeLength))	// Field is in a middle iteration, check whether one before and one after are not type X
		{
			if( (detectAsciiType(strToCmpr[rangeLength + 1]) != X) 
				&& (detectAsciiType(strToCmpr[itr - 1]) != X)  && matches == rangeLength)
				return itr;
		}else if( itr == 0 && itr != (strLen - rangeLength))
		{
			if(detectAsciiType(strToCmpr[rangeLength]) != X && matches == rangeLength)
				return itr;
		}
		else
		{
			if(matches == rangeLength)
				return itr;

		}

		matches = 0; nonMatches = 0;		

		// Field is in last iteration, check whether on char before field is not type X
						

	}

	return -1;

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
			case 20: strToCmpr[19 + itr] == x[19] ? matches++:nonMatches++;
			case 19: strToCmpr[18 + itr] == x[18] ? matches++:nonMatches++;
			case 18: strToCmpr[17 + itr] == x[17] ? matches++:nonMatches++;
			case 17: strToCmpr[16 + itr] == x[16] ? matches++:nonMatches++;
			case 16: strToCmpr[15 + itr] == x[15] ? matches++:nonMatches++;
			case 15: strToCmpr[14 + itr] == x[14] ? matches++:nonMatches++;
			case 14: strToCmpr[13 + itr] == x[13] ? matches++:nonMatches++;
			case 13: strToCmpr[12 + itr] == x[12] ? matches++:nonMatches++;
			case 12: strToCmpr[11 + itr] == x[11] ? matches++:nonMatches++;
			case 11: strToCmpr[10 + itr] == x[10] ? matches++:nonMatches++;
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
			return itr;

		matches = 0; nonMatches = 0;

	}

	return -1;

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


void reverse(char arr[])
{
	int i;
	for(i = 0; arr[i] != '\0'; i++)
	{

	}

	int k;
	for(k = i -1; k >= 0; k = k -1)
	{
		printf("%c", arr[k]);
	}
		printf("\n");
}

void printC(char arr[])
{
	int k;
	for(k = 0; arr[k] != '\0'; k++)
	{
		if(arr[k] == 'F' || arr[k] == 'C')
		{

		}else
		{
			printf("%c", arr[k]);
		}	
	}
		printf("\n");

}

void printB(char arr[])
{
	int k;
	for(k = 0; arr[k] != '\0'; k++)
	{
		printf("%c", arr[k]);

		if(arr[k] == 'E')
	printf("%c", 'H');
	}
		printf("\n");

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

	if(	x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' ||
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

int isEvenNum(int a)
{
	unsigned int mask = 1;

	if( (mask & ((unsigned int)a)) == 1)
		return 0;

	return 1;

}


