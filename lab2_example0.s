	.file	"lab2_example0.c"
	.text
	.globl	shared_variable
	.bss
	.align 4
	.type	shared_variable, @object
	.size	shared_variable, 4
shared_variable:
	.zero	4
	.globl	mutex
	.align 32
	.type	mutex, @object
	.size	mutex, 40
mutex:
	.zero	40
	.section	.rodata
.LC0:
	.string	"\n Usage for %s : \n"
	.align 8
.LC1:
	.string	"    -t: num thread, must be bigger than 0 ( e.g. 4 )"
	.align 8
.LC2:
	.string	"    -i: iteration count, must be bigger than 0 ( e.g. 100000 ) "
	.align 8
.LC3:
	.string	"    -s: sync mode setting ( e.g. m : pthread mutex , o : original ) "
	.text
	.globl	lab2_ex_usage
	.type	lab2_ex_usage, @function
lab2_ex_usage:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	leaq	.LC3(%rip), %rdi
	call	puts@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	lab2_ex_usage, .-lab2_ex_usage
	.section	.rodata
.LC4:
	.string	"\n Example : "
	.align 8
.LC5:
	.string	"    #sudo %s -t 4 -i 1000000 -s o \n"
	.align 8
.LC6:
	.string	"    #sudo %s -t 4 -i 1000000 -s m \n\n"
	.text
	.globl	lab2_ex_example
	.type	lab2_ex_example, @function
lab2_ex_example:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	lab2_ex_example, .-lab2_ex_example
	.section	.rodata
.LC7:
	.string	"original(race condition)"
.LC8:
	.string	"pthread mutex"
.LC9:
	.string	"\n Experiment info "
	.align 8
.LC10:
	.string	"    num_threads         : %d \n"
	.align 8
.LC11:
	.string	"    num_iterations      : %d \n"
	.align 8
.LC12:
	.string	"    experiment type     : %s \n"
.LC13:
	.string	"\n Experiment result : "
	.align 8
.LC14:
	.string	"    expected result     : %d \n"
	.align 8
.LC15:
	.string	"    result              : %d \n\n"
	.text
	.type	print_result, @function
print_result:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	%edx, -44(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC7(%rip), %rax
	movq	%rax, -32(%rbp)
	leaq	.LC8(%rip), %rax
	movq	%rax, -24(%rbp)
	leaq	.LC9(%rip), %rdi
	call	puts@PLT
	movl	-36(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC10(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-40(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-44(%rbp), %eax
	cltq
	movq	-32(%rbp,%rax,8), %rax
	movq	%rax, %rsi
	leaq	.LC12(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC13(%rip), %rdi
	call	puts@PLT
	movl	-36(%rbp), %eax
	imull	-40(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC14(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	shared_variable(%rip), %eax
	movl	%eax, %esi
	leaq	.LC15(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L4
	call	__stack_chk_fail@PLT
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	print_result, .-print_result
	.type	add_shared_variable, @function
add_shared_variable:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	20(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -12(%rbp)
	movl	$0, -20(%rbp)
	cmpl	$0, -12(%rbp)
	je	.L6
	movl	$0, -20(%rbp)
	jmp	.L7
.L8:
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_lock@PLT
	movl	shared_variable(%rip), %eax
	addl	$1, %eax
	movl	%eax, shared_variable(%rip)
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	addl	$1, -20(%rbp)
.L7:
	movl	-20(%rbp), %eax
	cmpl	-16(%rbp), %eax
	jl	.L8
	jmp	.L9
.L6:
	movl	$0, -20(%rbp)
	jmp	.L10
.L11:
	movl	shared_variable(%rip), %eax
	addl	$1, %eax
	movl	%eax, shared_variable(%rip)
	addl	$1, -20(%rbp)
.L10:
	movl	-20(%rbp), %eax
	cmpl	-16(%rbp), %eax
	jl	.L11
.L9:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	add_shared_variable, .-add_shared_variable
	.section	.rodata
	.align 8
.LC17:
	.string	" Error: _perf_metadata - pthread_create error "
	.text
	.globl	mutex_test
	.type	mutex_test, @function
mutex_test:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	%edi, -100(%rbp)
	movl	%esi, -104(%rbp)
	movl	%edx, -108(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -88(%rbp)
	movl	$-1, -92(%rbp)
	fldz
	fstpt	-80(%rbp)
	movl	-108(%rbp), %eax
	movl	%eax, -40(%rbp)
	movl	-104(%rbp), %eax
	movl	%eax, -44(%rbp)
	movl	-100(%rbp), %eax
	cltq
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -88(%rbp)
	movl	-100(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-88(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movl	$0, -96(%rbp)
	jmp	.L13
.L16:
	movl	-96(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-88(%rbp), %rax
	leaq	(%rdx,%rax), %rdi
	leaq	-64(%rbp), %rax
	movq	%rax, %rcx
	leaq	add_shared_variable(%rip), %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	movl	%eax, -92(%rbp)
	cmpl	$-1, -92(%rbp)
	jne	.L14
	leaq	.LC17(%rip), %rdi
	call	puts@PLT
	nop
.L15:
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$-1, %eax
	jmp	.L20
.L14:
	addl	$1, -96(%rbp)
.L13:
	movl	-96(%rbp), %eax
	cmpl	-100(%rbp), %eax
	jl	.L16
	movl	$0, -96(%rbp)
	jmp	.L17
.L18:
	movl	-96(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-88(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	addl	$1, -96(%rbp)
.L17:
	movl	-96(%rbp), %eax
	cmpl	-100(%rbp), %eax
	jl	.L18
	movl	-108(%rbp), %edx
	movl	-104(%rbp), %ecx
	movl	-100(%rbp), %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	print_result
	movl	$0, %eax
.L20:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L21
	call	__stack_chk_fail@PLT
.L21:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	mutex_test, .-mutex_test
	.section	.rodata
.LC19:
	.string	"o"
.LC20:
	.string	"m"
.LC21:
	.string	"t:i:s:"
	.text
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -12(%rbp)
	movl	$0, -8(%rbp)
	movl	$-1, -4(%rbp)
	movl	$0, optind(%rip)
	jmp	.L23
.L30:
	movsbl	-13(%rbp), %eax
	cmpl	$115, %eax
	je	.L25
	cmpl	$116, %eax
	je	.L26
	cmpl	$105, %eax
	je	.L27
	jmp	.L32
.L26:
	movq	optarg(%rip), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -12(%rbp)
	jmp	.L23
.L27:
	movq	optarg(%rip), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -8(%rbp)
	jmp	.L23
.L25:
	movq	optarg(%rip), %rax
	leaq	.LC19(%rip), %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L28
	movl	$0, -4(%rbp)
	jmp	.L23
.L28:
	movq	optarg(%rip), %rax
	leaq	.LC20(%rip), %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L32
	movl	$1, -4(%rbp)
	nop
.L23:
	movq	-32(%rbp), %rcx
	movl	-20(%rbp), %eax
	leaq	.LC21(%rip), %rdx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	getopt@PLT
	movb	%al, -13(%rbp)
	cmpb	$-1, -13(%rbp)
	jne	.L30
	cmpl	$0, -12(%rbp)
	jle	.L33
	cmpl	$0, -8(%rbp)
	jle	.L33
	cmpl	$-1, -4(%rbp)
	je	.L33
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %ecx
	movl	-12(%rbp), %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	call	mutex_test
	movl	$0, %eax
	jmp	.L31
.L32:
	nop
	jmp	.L29
.L33:
	nop
.L29:
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	lab2_ex_usage
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	lab2_ex_example
	movl	$-1, %eax
.L31:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
