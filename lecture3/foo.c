

// Understand calling calling convention or ABI

// RDI is the fist arg, RSI is the second arg, RDX is the third arg

//R is 64 bit

//

//Return value goes in EAX

int foo(long x, int y, char z, int* p)
{

	//movl %edi, %eax
	//addl (%rcx), %eax
	//movl %eax, (%rcx)
	//ret


	*p = temp;	  // leal (%rsi, %rdi), %eax
	return temp;  // %eax, (%rcx)
	// ret

	///* Part 3

	x = x -y;							//subl %esi, %edi
	int temp = (int) z;					//movsbl %dl, %edx
	int ret_val = x;					// mol %edl, %eax
	ret_val = temp * ret_val;			// imull %edx, %eax
	return ret_val;						//ret

	or

	return z * (x - y);
	//*/


	///* Part 2
	long temp = x; //movq %rdi, %rax
	char temp2 = z; // movl %edx, %ecx
	temp <<= temp2;  //salq %cl, %rax  // cl is the first byte of %ecx
	return temp;
	//*/


	// IMplementation 1 return y + x;

}