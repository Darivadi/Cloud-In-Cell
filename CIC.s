	.file	"CIC.c"
	.text
	.p2align 4,,15
	.globl	W
	.type	W, @function
W:
.LFB64:
	.cfi_startproc
	movsd	.LC1(%rip), %xmm4
	xorpd	%xmm5, %xmm5
	andpd	%xmm4, %xmm0
	ucomisd	%xmm0, %xmm3
	jbe	.L2
	divsd	%xmm3, %xmm0
	movsd	.LC2(%rip), %xmm5
	subsd	%xmm0, %xmm5
.L2:
	andpd	%xmm4, %xmm1
	xorpd	%xmm0, %xmm0
	ucomisd	%xmm1, %xmm3
	jbe	.L4
	divsd	%xmm3, %xmm1
	movsd	.LC2(%rip), %xmm0
	subsd	%xmm1, %xmm0
.L4:
	andpd	%xmm2, %xmm4
	xorpd	%xmm1, %xmm1
	ucomisd	%xmm4, %xmm3
	jbe	.L6
	divsd	%xmm3, %xmm4
	movsd	.LC2(%rip), %xmm1
	subsd	%xmm4, %xmm1
.L6:
	mulsd	%xmm5, %xmm0
	mulsd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE64:
	.size	W, .-W
	.p2align 4,,15
	.globl	locateCell
	.type	locateCell, @function
locateCell:
.LFB65:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rsi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movslq	%edi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$56, %rsp
	.cfi_def_cfa_offset 96
	movl	GV(%rip), %ebp
	movsd	GV+8(%rip), %xmm4
	movsd	%xmm2, 40(%rsp)
	divsd	%xmm4, %xmm0
	cvtsi2sd	%ebp, %xmm5
	movsd	%xmm4, 8(%rsp)
	movsd	%xmm1, 24(%rsp)
	movsd	%xmm5, 16(%rsp)
	mulsd	%xmm5, %xmm0
	call	floor
	movsd	24(%rsp), %xmm1
	movsd	%xmm0, 32(%rsp)
	divsd	8(%rsp), %xmm1
	movsd	16(%rsp), %xmm0
	mulsd	%xmm1, %xmm0
	call	floor
	movsd	40(%rsp), %xmm2
	movsd	%xmm0, 24(%rsp)
	divsd	8(%rsp), %xmm2
	movsd	16(%rsp), %xmm0
	mulsd	%xmm2, %xmm0
	call	floor
	movsd	32(%rsp), %xmm3
	movsd	24(%rsp), %xmm1
	cvttsd2si	%xmm3, %ebx
	cvttsd2si	%xmm1, %eax
	imull	%ebp, %ebx
	addl	%eax, %ebx
	cvttsd2si	%xmm0, %eax
	imull	%ebp, %ebx
	addl	%eax, %ebx
	movslq	%ebx, %rbx
	leaq	0(,%rbx,8), %rax
	salq	$6, %rbx
	subq	%rax, %rbx
	addq	%r13, %rbx
	movl	(%rbx), %eax
	movq	8(%rbx), %rdi
	leal	1(%rax), %esi
	movl	%esi, (%rbx)
	movslq	%esi, %rsi
	salq	$3, %rsi
	call	realloc
	movslq	(%rbx), %rdx
	movq	%rax, 8(%rbx)
	movq	%r12, -8(%rax,%rdx,8)
	addq	$56, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE65:
	.size	locateCell, .-locateCell
	.p2align 4,,15
	.globl	mod
	.type	mod, @function
mod:
.LFB66:
	.cfi_startproc
	movl	%edi, %eax
	cltd
	idivl	%esi
	testl	%edx, %edx
	jns	.L21
	.p2align 4,,10
	.p2align 3
.L22:
	addl	%esi, %edx
	js	.L22
.L21:
	movl	%edx, %eax
	ret
	.cfi_endproc
.LFE66:
	.size	mod, .-mod
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"grep -v \"#\" %s | grep -v \"^$\" | gawk -F\"=\" '{print $2}' > %s.dump"
	.text
	.p2align 4,,15
	.globl	conf2dump
	.type	conf2dump, @function
conf2dump:
.LFB67:
	.cfi_startproc
	subq	$1016, %rsp
	.cfi_def_cfa_offset 1024
	movq	%rdi, %r8
	movq	%rdi, %r9
	movq	%fs:40, %rax
	movq	%rax, 1000(%rsp)
	xorl	%eax, %eax
	movl	$1000, %edx
	movl	$.LC3, %ecx
	movl	$1, %esi
	movq	%rsp, %rdi
	call	__sprintf_chk
	movq	%rsp, %rdi
	call	system
	xorl	%eax, %eax
	movq	1000(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L26
	addq	$1016, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L26:
	.cfi_restore_state
	call	__stack_chk_fail
	.cfi_endproc
.LFE67:
	.size	conf2dump, .-conf2dump
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	"r"
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"  * The file '%s' doesn't exist!\n"
	.section	.rodata.str1.1
.LC6:
	.string	"%s.dump"
.LC7:
	.string	"%d"
.LC8:
	.string	"%s"
.LC9:
	.string	"%lf"
	.section	.rodata.str1.8
	.align 8
.LC10:
	.string	"  * The file '%s' has been loaded!\n"
	.section	.rodata.str1.1
.LC11:
	.string	"rm -rf %s.dump"
	.text
	.p2align 4,,15
	.globl	read_parameters
	.type	read_parameters, @function
read_parameters:
.LFB68:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$.LC4, %esi
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$2024, %rsp
	.cfi_def_cfa_offset 2048
	movq	%fs:40, %rax
	movq	%rax, 2008(%rsp)
	xorl	%eax, %eax
	call	fopen
	testq	%rax, %rax
	je	.L32
	movq	%rax, %rdi
	call	fclose
	leaq	1008(%rsp), %rdi
	movq	%rbp, %r9
	movq	%rbp, %r8
	movl	$.LC3, %ecx
	movl	$1000, %edx
	movl	$1, %esi
	xorl	%eax, %eax
	call	__sprintf_chk
	leaq	1008(%rsp), %rdi
	call	system
	leaq	1008(%rsp), %rdi
	movq	%rbp, %r8
	movl	$.LC6, %ecx
	movl	$1000, %edx
	movl	$1, %esi
	xorl	%eax, %eax
	call	__sprintf_chk
	leaq	1008(%rsp), %rdi
	movl	$.LC4, %esi
	call	fopen
	movl	$GV, %edx
	movq	%rax, %rbx
	movq	%rax, %rdi
	movl	$.LC7, %esi
	xorl	%eax, %eax
	call	__isoc99_fscanf
	movl	$GV+64, %edx
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	__isoc99_fscanf
	movl	$GV+1064, %edx
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	__isoc99_fscanf
	movl	$GV+1072, %edx
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	__isoc99_fscanf
	movl	$GV+1080, %edx
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	__isoc99_fscanf
	movl	$GV+1096, %edx
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	__isoc99_fscanf
	movsd	.LC2(%rip), %xmm0
	movq	%rbx, %rdi
	movsd	GV+1080(%rip), %xmm1
	addsd	%xmm0, %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, GV+1088(%rip)
	call	fclose
	movq	%rbp, %rdx
	movl	$.LC10, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movq	%rbp, %r8
	movl	$.LC11, %ecx
	movl	$1000, %edx
	movl	$1, %esi
	movq	%rsp, %rdi
	xorl	%eax, %eax
	call	__sprintf_chk
	movq	%rsp, %rdi
	call	system
	xorl	%eax, %eax
.L29:
	movq	2008(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L33
	addq	$2024, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L32:
	.cfi_restore_state
	movq	%rbp, %rdx
	movl	$.LC5, %esi
	movl	$1, %edi
	call	__printf_chk
	movl	$1, %eax
	jmp	.L29
.L33:
	call	__stack_chk_fail
	.cfi_endproc
.LFE68:
	.size	read_parameters, .-read_parameters
	.section	.rodata.str1.8
	.align 8
.LC12:
	.string	"-----------------------------------------------"
	.section	.rodata.str1.1
.LC13:
	.string	"Cosmological parameters:"
	.section	.rodata.str1.8
	.align 8
.LC14:
	.string	"OmegaM0=%lf OmegaL0=%lf redshift=%lf HubbleParam=%lf\n"
	.section	.rodata.str1.1
.LC15:
	.string	"Simulation parameters:"
.LC16:
	.string	"NpTotal=%ld L=%lf\n"
	.text
	.p2align 4,,15
	.globl	read_binary
	.type	read_binary, @function
read_binary:
.LFB69:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	$.LC4, %esi
	movl	$GV+64, %edi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$32, %rsp
	.cfi_def_cfa_offset 64
	call	fopen
	movl	$1, %edx
	movq	%rax, %rbp
	movq	%rax, %rcx
	movl	$8, %esi
	movl	$GV+8, %edi
	call	fread
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+16, %edi
	call	fread
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1064, %edi
	call	fread
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1072, %edi
	call	fread
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1080, %edi
	call	fread
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1096, %edi
	call	fread
	movl	$.LC12, %edi
	call	puts
	movl	$.LC13, %edi
	call	puts
	movsd	GV+1096(%rip), %xmm3
	movl	$.LC14, %esi
	movsd	GV+1080(%rip), %xmm2
	movl	$1, %edi
	movsd	GV+1072(%rip), %xmm1
	movl	$4, %eax
	movsd	GV+1064(%rip), %xmm0
	call	__printf_chk
	movl	$.LC12, %edi
	call	puts
	movl	$.LC15, %edi
	call	puts
	movq	GV+16(%rip), %rdx
	movsd	GV+8(%rip), %xmm0
	movl	$.LC16, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movl	$.LC12, %edi
	call	puts
	movq	GV+16(%rip), %rbx
	movl	$48, %esi
	movq	%rbx, %rdi
	call	calloc
	testq	%rbx, %rbx
	movq	%rax, part(%rip)
	jle	.L36
	movq	%rax, %rdi
	movl	$1, %r12d
	xorl	%ebx, %ebx
	jmp	.L37
	.p2align 4,,10
	.p2align 3
.L40:
	movq	part(%rip), %rdi
.L37:
	addq	%rbx, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$8, %esi
	call	fread
	movq	%rbx, %rdi
	addq	part(%rip), %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	addq	$40, %rdi
	call	fread
	movq	%rbp, %rcx
	movl	$3, %edx
	movl	$8, %esi
	movq	%rsp, %rdi
	call	fread
	movq	%rbx, %rdx
	addq	part(%rip), %rdx
	movq	%rbp, %rcx
	movsd	(%rsp), %xmm0
	movl	$8, %esi
	addq	$48, %rbx
	movsd	%xmm0, 8(%rdx)
	leaq	32(%rdx), %rdi
	movsd	8(%rsp), %xmm0
	movsd	%xmm0, 16(%rdx)
	movsd	16(%rsp), %xmm0
	movsd	%xmm0, 24(%rdx)
	movl	$1, %edx
	call	fread
	movq	%r12, %rax
	addq	$1, %r12
	cmpq	GV+16(%rip), %rax
	jl	.L40
.L36:
	movq	%rbp, %rdi
	call	fclose
	addq	$32, %rsp
	.cfi_def_cfa_offset 32
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE69:
	.size	read_binary, .-read_binary
	.section	.rodata.str1.1
.LC17:
	.string	"w"
	.section	.rodata.str1.8
	.align 8
.LC18:
	.string	"./../Processed_data/CIC_DenCon_field.bin"
	.text
	.p2align 4,,15
	.globl	write_binary
	.type	write_binary, @function
write_binary:
.LFB70:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$.LC17, %esi
	movl	$.LC18, %edi
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	call	fopen
	movl	$1, %edx
	movq	%rax, %rbx
	movq	%rax, %rcx
	movl	$8, %esi
	movl	$GV+8, %edi
	call	fwrite
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1064, %edi
	call	fwrite
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1072, %edi
	call	fwrite
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1080, %edi
	call	fwrite
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$8, %esi
	movl	$GV+1096, %edi
	call	fwrite
	movl	GV+24(%rip), %eax
	movl	$0, 12(%rsp)
	testl	%eax, %eax
	jle	.L43
	.p2align 4,,10
	.p2align 3
.L45:
	leaq	12(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	movslq	12(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	leaq	0(,%rdi,8), %rax
	salq	$6, %rdi
	subq	%rax, %rdi
	addq	cells(%rip), %rdi
	call	fwrite
	movslq	12(%rsp), %rdx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$8, %esi
	leaq	0(,%rdx,8), %rax
	salq	$6, %rdx
	subq	%rax, %rdx
	addq	cells(%rip), %rdx
	movsd	32(%rdx), %xmm0
	movsd	%xmm0, 16(%rsp)
	movsd	40(%rdx), %xmm0
	movsd	%xmm0, 24(%rsp)
	movsd	48(%rdx), %xmm0
	movl	$3, %edx
	movsd	%xmm0, 32(%rsp)
	call	fwrite
	movslq	12(%rsp), %rdx
	movq	%rbx, %rcx
	movl	$8, %esi
	leaq	0(,%rdx,8), %rax
	salq	$6, %rdx
	subq	%rax, %rdx
	addq	cells(%rip), %rdx
	leaq	16(%rdx), %rdi
	movl	$1, %edx
	call	fwrite
	movl	12(%rsp), %eax
	addl	$1, %eax
	cmpl	%eax, GV+24(%rip)
	movl	%eax, 12(%rsp)
	jg	.L45
.L43:
	movq	%rbx, %rdi
	call	fclose
	addq	$48, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE70:
	.size	write_binary, .-write_binary
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"Error: Incomplete number of parameters. Execute as follows:"
	.section	.rodata.str1.1
.LC20:
	.string	"%s Parameters_file\n"
.LC21:
	.string	"Reading parameters file"
	.section	.rodata.str1.8
	.align 8
.LC22:
	.string	"--------------------------------------------------\n"
	.section	.rodata.str1.1
.LC23:
	.string	"Parameters file read!"
.LC24:
	.string	"Reading data file"
.LC25:
	.string	"Data file read!"
.LC26:
	.string	"Computing mean density"
	.section	.rodata.str1.8
	.align 8
.LC28:
	.string	"NGRID=%d NGRID3=%d Particle_Mass=%lf NpTotal=%ld \nrhoMean=%lf L=%lf volCell=%lf dx=%lf \nFilename=%s\n"
	.section	.rodata.str1.1
.LC29:
	.string	"Allocating memory"
.LC30:
	.string	"Memory allocated"
.LC31:
	.string	"Locating cells"
.LC32:
	.string	"Particles located in the grid"
	.section	.rodata.str1.8
	.align 8
.LC33:
	.string	"Performing the mass assignment"
	.align 8
.LC35:
	.string	"Total number of particles:%10d\n"
	.section	.rodata.str1.1
.LC36:
	.string	"Mass CIC = %lf\n"
.LC37:
	.string	"Mass Simulation = %lf\n"
	.section	.rodata.str1.8
	.align 8
.LC38:
	.string	"Code has finished successfully"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB71:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	subq	$104, %rsp
	.cfi_def_cfa_offset 160
	cmpl	$1, %edi
	jle	.L115
	movq	8(%rsi), %rbx
	movl	$.LC21, %edi
	call	puts
	movl	$.LC22, %edi
	call	puts
	movq	%rbx, %rdi
	call	read_parameters
	movl	$.LC23, %edi
	call	puts
	movl	$.LC22, %edi
	call	puts
	movl	$.LC24, %edi
	call	puts
	movl	$.LC22, %edi
	call	puts
	call	read_binary
	movl	GV(%rip), %eax
	movsd	GV+8(%rip), %xmm0
	movl	$.LC25, %edi
	cvtsi2sd	%eax, %xmm1
	movl	%eax, %edx
	imull	%eax, %edx
	imull	%eax, %edx
	movl	%edx, GV+24(%rip)
	divsd	%xmm1, %xmm0
	movapd	%xmm0, %xmm1
	movsd	%xmm0, GV+56(%rip)
	mulsd	%xmm0, %xmm1
	mulsd	%xmm0, %xmm1
	movsd	%xmm1, GV+48(%rip)
	call	puts
	movl	$.LC22, %edi
	call	puts
	movl	$.LC26, %edi
	call	puts
	movl	$.LC22, %edi
	call	puts
	movq	GV+16(%rip), %rdx
	testq	%rdx, %rdx
	jle	.L94
	xorpd	%xmm6, %xmm6
	movq	part(%rip), %rax
	leaq	(%rdx,%rdx,2), %rdx
	salq	$4, %rdx
	movsd	%xmm6, 56(%rsp)
	movapd	%xmm6, %xmm0
	addq	%rax, %rdx
.L51:
	addsd	32(%rax), %xmm0
	addq	$48, %rax
	cmpq	%rdx, %rax
	jne	.L51
	movsd	%xmm0, 24(%rsp)
.L50:
	movsd	.LC27(%rip), %xmm1
	movsd	GV+8(%rip), %xmm0
	call	pow
	movsd	24(%rsp), %xmm6
	movl	$.LC12, %edi
	divsd	%xmm0, %xmm6
	movsd	%xmm6, GV+32(%rip)
	call	puts
	movl	$.LC13, %edi
	call	puts
	movsd	GV+1096(%rip), %xmm3
	movl	$.LC14, %esi
	movsd	GV+1080(%rip), %xmm2
	movl	$1, %edi
	movsd	GV+1072(%rip), %xmm1
	movl	$4, %eax
	movsd	GV+1064(%rip), %xmm0
	call	__printf_chk
	movl	$.LC12, %edi
	call	puts
	movl	$.LC15, %edi
	call	puts
	movl	GV+24(%rip), %ecx
	movq	GV+16(%rip), %r8
	movl	$GV+64, %r9d
	movl	GV(%rip), %edx
	movsd	GV+56(%rip), %xmm4
	movsd	GV+48(%rip), %xmm3
	movl	$.LC28, %esi
	movsd	GV+8(%rip), %xmm2
	movl	$1, %edi
	movsd	GV+32(%rip), %xmm1
	movl	$5, %eax
	movsd	GV+40(%rip), %xmm0
	call	__printf_chk
	movl	$.LC12, %edi
	call	puts
	movl	$.LC29, %edi
	call	puts
	movl	$.LC12, %edi
	call	puts
	movslq	GV+24(%rip), %rdi
	movl	$56, %esi
	call	calloc
	movl	$.LC30, %edi
	movq	%rax, cells(%rip)
	call	puts
	movl	$.LC12, %edi
	call	puts
	movl	GV+24(%rip), %ecx
	testl	%ecx, %ecx
	jle	.L55
	movq	cells(%rip), %rdx
	subl	$1, %ecx
	leaq	0(,%rcx,8), %rsi
	salq	$6, %rcx
	leaq	56(%rdx), %rax
	subq	%rsi, %rcx
	addq	%rax, %rcx
.L56:
	cmpq	%rcx, %rax
	movl	$0, (%rdx)
	movq	$0, 16(%rdx)
	movq	$0, 24(%rdx)
	movq	%rax, %rdx
	je	.L55
	addq	$56, %rax
	jmp	.L56
.L115:
	movl	$.LC19, %edi
	call	puts
	movq	(%rbx), %rdx
	movl	$1, %edi
	movl	$.LC20, %esi
	xorl	%eax, %eax
	call	__printf_chk
	xorl	%edi, %edi
	call	exit
.L94:
	xorpd	%xmm6, %xmm6
	movsd	%xmm6, 56(%rsp)
	movsd	%xmm6, 24(%rsp)
	jmp	.L50
.L55:
	movl	$.LC31, %edi
	xorl	%ebp, %ebp
	xorl	%ebx, %ebx
	call	puts
	movl	$.LC12, %edi
	call	puts
	cmpq	$0, GV+16(%rip)
	jle	.L54
.L104:
	movq	%rbp, %rax
	addq	part(%rip), %rax
	movq	cells(%rip), %rsi
	movl	%ebx, %edi
	addq	$48, %rbp
	addq	$1, %rbx
	movsd	8(%rax), %xmm0
	movsd	24(%rax), %xmm2
	movsd	16(%rax), %xmm1
	call	locateCell
	cmpq	%rbx, GV+16(%rip)
	jg	.L104
.L54:
	movl	$.LC32, %edi
	call	puts
	movl	$.LC12, %edi
	call	puts
	movl	$.LC33, %edi
	call	puts
	movl	$.LC12, %edi
	call	puts
	movl	GV(%rip), %ecx
	testl	%ecx, %ecx
	jle	.L116
	movl	%ecx, %eax
	movq	part(%rip), %r14
	movq	cells(%rip), %r12
	imull	%ecx, %eax
	movsd	.LC2(%rip), %xmm13
	xorl	%r13d, %r13d
	movsd	GV+56(%rip), %xmm14
	movapd	%xmm13, %xmm15
	movq	%r12, 72(%rsp)
	cltq
	leaq	0(,%rax,8), %rdx
	salq	$6, %rax
	movq	%rax, 80(%rsp)
	movslq	%ecx, %rax
	subq	%rdx, 80(%rsp)
	leaq	0(,%rax,8), %rdx
	salq	$6, %rax
	movq	%rax, 88(%rsp)
	subq	%rdx, 88(%rsp)
	movq	%r14, %rax
	movl	%r13d, %r14d
	movq	%rax, %r13
.L60:
	movq	72(%rsp), %rax
	xorl	%ebp, %ebp
	movq	%rax, 64(%rsp)
.L87:
	movq	64(%rsp), %rax
	xorl	%r10d, %r10d
	movq	%rax, 40(%rsp)
.L85:
	movq	40(%rsp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	movl	%eax, 36(%rsp)
	jle	.L82
	cvtsi2sd	%r14d, %xmm5
	movq	40(%rsp), %rax
	xorl	%ebx, %ebx
	cvtsi2sd	%ebp, %xmm12
	cvtsi2sd	%r10d, %xmm7
	movsd	.LC34(%rip), %xmm0
	movq	8(%rax), %rax
	movsd	.LC1(%rip), %xmm6
	movq	%rax, 48(%rsp)
	addsd	%xmm0, %xmm5
	addsd	%xmm0, %xmm12
	addsd	%xmm0, %xmm7
	movsd	%xmm5, 16(%rsp)
.L83:
	movq	48(%rsp), %rax
	movl	$-1, %r11d
	movslq	(%rax,%rbx,8), %rax
	leaq	(%rax,%rax,2), %rax
	salq	$4, %rax
	addq	%r13, %rax
	movsd	8(%rax), %xmm5
	movsd	16(%rax), %xmm11
	movsd	%xmm5, 8(%rsp)
	xorpd	%xmm5, %xmm5
	movsd	24(%rax), %xmm9
	movsd	32(%rax), %xmm8
.L64:
	cvtsi2sd	%r11d, %xmm2
	leal	(%r11,%r14), %eax
	movl	$-1, %r9d
	cltd
	idivl	%ecx
	movl	%edx, %r8d
	addsd	16(%rsp), %xmm2
	mulsd	%xmm14, %xmm2
	subsd	8(%rsp), %xmm2
	andpd	%xmm6, %xmm2
.L81:
	cvtsi2sd	%r9d, %xmm1
	leal	(%r9,%rbp), %eax
	movl	$-1, %esi
	cltd
	idivl	%ecx
	movl	%edx, %edi
	addsd	%xmm12, %xmm1
	mulsd	%xmm14, %xmm1
	subsd	%xmm11, %xmm1
	andpd	%xmm6, %xmm1
.L79:
	leal	(%rsi,%r10), %eax
	cltd
	idivl	%ecx
	testl	%edx, %edx
	jns	.L65
	.p2align 4,,10
	.p2align 3
.L66:
	addl	%ecx, %edx
	js	.L66
.L65:
	testl	%edi, %edi
	movl	%edi, %eax
	jns	.L67
	.p2align 4,,10
	.p2align 3
.L68:
	addl	%ecx, %eax
	js	.L68
.L67:
	testl	%r8d, %r8d
	movl	%r8d, %r15d
	jns	.L69
	.p2align 4,,10
	.p2align 3
.L70:
	addl	%ecx, %r15d
	js	.L70
.L69:
	cvtsi2sd	%esi, %xmm3
	imull	%ecx, %r15d
	movapd	%xmm5, %xmm4
	addl	%r15d, %eax
	imull	%ecx, %eax
	addl	%eax, %edx
	ucomisd	%xmm2, %xmm14
	addsd	%xmm7, %xmm3
	mulsd	%xmm14, %xmm3
	subsd	%xmm9, %xmm3
	jbe	.L71
	movapd	%xmm2, %xmm0
	movsd	.LC2(%rip), %xmm4
	divsd	%xmm14, %xmm0
	subsd	%xmm0, %xmm4
.L71:
	ucomisd	%xmm1, %xmm14
	movapd	%xmm5, %xmm0
	jbe	.L73
	movapd	%xmm1, %xmm0
	movapd	%xmm13, %xmm10
	divsd	%xmm14, %xmm0
	subsd	%xmm0, %xmm10
	movapd	%xmm10, %xmm0
.L73:
	andpd	%xmm6, %xmm3
	ucomisd	%xmm3, %xmm14
	jbe	.L109
	divsd	%xmm14, %xmm3
	movapd	%xmm15, %xmm10
	subsd	%xmm3, %xmm10
	movapd	%xmm10, %xmm3
.L75:
	mulsd	%xmm4, %xmm0
	movslq	%edx, %rdx
	addl	$1, %esi
	leaq	0(,%rdx,8), %rax
	salq	$6, %rdx
	subq	%rax, %rdx
	mulsd	%xmm3, %xmm0
	addq	%r12, %rdx
	cmpl	$2, %esi
	mulsd	%xmm8, %xmm0
	addsd	24(%rdx), %xmm0
	movsd	%xmm0, 24(%rdx)
	jne	.L79
	addl	$1, %r9d
	cmpl	$2, %r9d
	jne	.L81
	addl	$1, %r11d
	cmpl	$2, %r11d
	jne	.L64
	addq	$1, %rbx
	cmpl	%ebx, 36(%rsp)
	jg	.L83
.L82:
	addl	$1, %r10d
	addq	$56, 40(%rsp)
	cmpl	%ecx, %r10d
	jne	.L85
	addl	$1, %ebp
	movq	88(%rsp), %rax
	addq	%rax, 64(%rsp)
	cmpl	%ecx, %ebp
	jne	.L87
	addl	$1, %r14d
	movq	80(%rsp), %rax
	addq	%rax, 72(%rsp)
	cmpl	%ecx, %r14d
	jne	.L60
	movq	%r13, %r14
	jmp	.L86
	.p2align 4,,10
	.p2align 3
.L109:
	movapd	%xmm5, %xmm3
	jmp	.L75
.L116:
	movq	part(%rip), %r14
.L86:
	movq	%r14, %rdi
	call	free
	movl	GV(%rip), %ecx
	testl	%ecx, %ecx
	jle	.L100
	movl	%ecx, %r10d
	movslq	%ecx, %r8
	xorl	%edx, %edx
	imull	%ecx, %r10d
	xorpd	%xmm0, %xmm0
	movsd	GV+56(%rip), %xmm5
	xorl	%ebx, %ebx
	movsd	GV+48(%rip), %xmm8
	movslq	%r10d, %r10
	movsd	GV+32(%rip), %xmm7
	leaq	0(,%r10,8), %rax
	salq	$6, %r10
	movsd	.LC34(%rip), %xmm4
	subq	%rax, %r10
	movq	cells(%rip), %rax
	movsd	.LC2(%rip), %xmm6
	leaq	24(%rax), %r9
	leaq	0(,%r8,8), %rax
	salq	$6, %r8
	subq	%rax, %r8
.L89:
	cvtsi2sd	%edx, %xmm1
	movq	%r9, %r11
	xorl	%esi, %esi
	addsd	%xmm4, %xmm1
	mulsd	%xmm5, %xmm1
.L93:
	cvtsi2sd	%esi, %xmm2
	movq	%r11, %rax
	xorl	%edi, %edi
	addsd	%xmm4, %xmm2
	mulsd	%xmm5, %xmm2
.L92:
	cvtsi2sd	%edi, %xmm3
	addl	$1, %edi
	addl	-24(%rax), %ebx
	movsd	%xmm1, 8(%rax)
	addq	$56, %rax
	movsd	%xmm2, -40(%rax)
	addsd	%xmm4, %xmm3
	mulsd	%xmm5, %xmm3
	movsd	%xmm3, -32(%rax)
	movsd	-56(%rax), %xmm3
	addsd	%xmm3, %xmm0
	divsd	%xmm8, %xmm3
	movsd	%xmm3, -56(%rax)
	divsd	%xmm7, %xmm3
	subsd	%xmm6, %xmm3
	movsd	%xmm3, -64(%rax)
	cmpl	%ecx, %edi
	jne	.L92
	addl	$1, %esi
	addq	%r8, %r11
	cmpl	%ecx, %esi
	jne	.L93
	addl	$1, %edx
	addq	%r10, %r9
	cmpl	%ecx, %edx
	jne	.L89
	jmp	.L88
.L100:
	xorpd	%xmm0, %xmm0
	xorl	%ebx, %ebx
.L88:
	movsd	%xmm0, 8(%rsp)
	call	write_binary
	movl	%ebx, %edx
	movl	$.LC35, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movsd	8(%rsp), %xmm0
	movl	$.LC36, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movsd	24(%rsp), %xmm0
	movl	$.LC37, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movq	cells(%rip), %rdi
	call	free
	movl	$.LC38, %edi
	call	puts
	addq	$104, %rsp
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE71:
	.size	main, .-main
	.comm	cells,8,8
	.comm	Header,256,32
	.comm	part,8,8
	.comm	GV,1104,32
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	4294967295
	.long	2147483647
	.long	0
	.long	0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC2:
	.long	0
	.long	1072693248
	.align 8
.LC27:
	.long	0
	.long	1074266112
	.align 8
.LC34:
	.long	0
	.long	1071644672
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
