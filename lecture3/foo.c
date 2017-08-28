

// Understand calling calling convention or ABI

// RDI is the fist arg, RSI is the second arg, RDX is the third arg

//R is 64 bit

//

//Return value goes in EAX

int foo(long x, int y, char z, int* p)
{
	long temp = x; //movq %rdi, %rax
	char temp2 = z; // movl %edx, %ecx
	temp <<= temp2;  //salq %cl, %rax  // cl is the first byte of %ecx
	return temp;



	// IMplementation 1 return y + x;

}