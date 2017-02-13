	.file	"test6.c"
	.section	.rodata
.LC0:
	.string	"little_type"
.LC1:
	.string	"big_type"
.LC2:
	.string	"unkown"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$67305985, -12(%rbp)
	leaq	-12(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$1, %al
	jne	.L2
	movl	$.LC0, %edi
	call	puts
	jmp	.L3
.L2:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$4, %al
	jne	.L4
	movl	$.LC1, %edi
	call	puts
	jmp	.L3
.L4:
	movl	$.LC2, %edi
	call	puts
.L3:
	movl	$0, %edi
	call	exit
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-4)"
	.section	.note.GNU-stack,"",@progbits
