	.file	"matchlab.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Error, null character"
	.text
	.globl	identifyArgType
	.type	identifyArgType, @function
identifyArgType:
.LFB21:
	.cfi_startproc
	movzbl	(%rdi), %eax
	testb	%al, %al
	jne	.L2
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	stderr(%rip), %rcx
	movl	$21, %edx
	movl	$1, %esi
	movl	$.LC0, %edi
	call	fwrite
	movl	$5, %eax
	jmp	.L3
.L2:
	.cfi_def_cfa_offset 8
	cmpb	$45, %al
	jne	.L7
	.p2align 4,,4
	jne	.L4
	cmpb	$97, 1(%rdi)
	.p2align 4,,5
	je	.L8
.L4:
	cmpb	$45, %al
	.p2align 4,,5
	jne	.L5
	cmpb	$98, 1(%rdi)
	je	.L9
.L5:
	cmpb	$45, %al
	.p2align 4,,3
	jne	.L6
	cmpb	$99, 1(%rdi)
	je	.L10
.L6:
	cmpb	$45, %al
	.p2align 4,,3
	jne	.L11
	cmpb	$116, 1(%rdi)
	je	.L12
	movl	$4, %eax
	ret
.L7:
	movl	$4, %eax
	ret
.L8:
	movl	$0, %eax
	ret
.L9:
	movl	$1, %eax
	ret
.L10:
	movl	$2, %eax
	ret
.L11:
	movl	$4, %eax
	ret
.L12:
	movl	$3, %eax
	ret
.L3:
	.cfi_def_cfa_offset 16
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE21:
	.size	identifyArgType, .-identifyArgType
	.section	.rodata.str1.1
.LC1:
	.string	"Billy"
.LC2:
	.string	"Billy%d"
.LC3:
	.string	"Wow"
.LC4:
	.string	"A\n"
.LC5:
	.string	"B\n"
.LC6:
	.string	"C\n"
.LC7:
	.string	"T\n"
.LC8:
	.string	"CMD_ARG\n"
.LC9:
	.string	"Error\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB20:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	%edi, %ebp
	movq	%rsi, %r12
	movq	stderr(%rip), %rcx
	movl	$5, %edx
	movl	$1, %esi
	movl	$.LC1, %edi
	call	fwrite
	movl	$3, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %ebx
	jmp	.L16
.L23:
	movq	stderr(%rip), %rcx
	movl	$3, %edx
	movl	$1, %esi
	movl	$.LC3, %edi
	call	fwrite
	movslq	%ebx, %rax
	movq	(%r12,%rax,8), %rdi
	call	identifyArgType
	testl	%eax, %eax
	jne	.L17
	movq	stderr(%rip), %rcx
	movl	$2, %edx
	movl	$1, %esi
	movl	$.LC4, %edi
	call	fwrite
	jmp	.L18
.L17:
	cmpl	$1, %eax
	jne	.L19
	movq	stderr(%rip), %rcx
	movl	$2, %edx
	movl	$1, %esi
	movl	$.LC5, %edi
	call	fwrite
	jmp	.L18
.L19:
	cmpl	$2, %eax
	jne	.L20
	movq	stderr(%rip), %rcx
	movl	$2, %edx
	movl	$1, %esi
	movl	$.LC6, %edi
	call	fwrite
	jmp	.L18
.L20:
	cmpl	$3, %eax
	jne	.L21
	movq	stderr(%rip), %rcx
	movl	$2, %edx
	movl	$1, %esi
	movl	$.LC7, %edi
	call	fwrite
	jmp	.L18
.L21:
	cmpl	$4, %eax
	jne	.L22
	movq	stderr(%rip), %rcx
	movl	$8, %edx
	movl	$1, %esi
	movl	$.LC8, %edi
	call	fwrite
	jmp	.L18
.L22:
	movq	stderr(%rip), %rcx
	movl	$6, %edx
	movl	$1, %esi
	movl	$.LC9, %edi
	call	fwrite
.L18:
	addl	$1, %ebx
.L16:
	cmpl	%ebp, %ebx
	jl	.L23
	movl	$0, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE20:
	.size	main, .-main
	.globl	basicPrint
	.type	basicPrint, @function
basicPrint:
.LFB22:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	jmp	.L26
.L27:
	movsbl	%dil, %edi
	call	putchar
	addq	$1, %rbx
.L26:
	movzbl	(%rbx), %edi
	testb	%dil, %dil
	jne	.L27
	movl	$10, %edi
	call	putchar
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	basicPrint, .-basicPrint
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
