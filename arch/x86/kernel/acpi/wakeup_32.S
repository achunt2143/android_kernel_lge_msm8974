<<<<<<< HEAD
	.section .text..page_aligned
=======
/* SPDX-License-Identifier: GPL-2.0-only */
	.text
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/linkage.h>
#include <asm/segment.h>
#include <asm/page_types.h>

<<<<<<< HEAD
# Copyright 2003, 2008 Pavel Machek <pavel@suse.cz>, distribute under GPLv2
=======
# Copyright 2003, 2008 Pavel Machek <pavel@suse.cz
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.code32
	ALIGN

<<<<<<< HEAD
ENTRY(wakeup_pmode_return)
wakeup_pmode_return:
	movw	$__KERNEL_DS, %ax
	movw	%ax, %ss
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	# reload the gdt, as we need the full 32 bit address
	lgdt	saved_gdt
=======
SYM_CODE_START(wakeup_pmode_return)
	movw	$__KERNEL_DS, %ax
	movw	%ax, %ss
	movw	%ax, %fs
	movw	%ax, %gs

	movw	$__USER_DS, %ax
	movw	%ax, %ds
	movw	%ax, %es

	# reload the gdt, as we need the full 32 bit address
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lidt	saved_idt
	lldt	saved_ldt
	ljmp	$(__KERNEL_CS), $1f
1:
	movl	%cr3, %eax
	movl	%eax, %cr3
	wbinvd

	# and restore the stack ... but you need gdt for this to work
	movl	saved_context_esp, %esp

	movl	%cs:saved_magic, %eax
	cmpl	$0x12345678, %eax
	jne	bogus_magic

	# jump to place where we left off
	movl	saved_eip, %eax
	jmp	*%eax
<<<<<<< HEAD
=======
SYM_CODE_END(wakeup_pmode_return)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

bogus_magic:
	jmp	bogus_magic



save_registers:
<<<<<<< HEAD
	sgdt	saved_gdt
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sidt	saved_idt
	sldt	saved_ldt
	str	saved_tss

	leal	4(%esp), %eax
	movl	%eax, saved_context_esp
	movl	%ebx, saved_context_ebx
	movl	%ebp, saved_context_ebp
	movl	%esi, saved_context_esi
	movl	%edi, saved_context_edi
	pushfl
	popl	saved_context_eflags

	movl	$ret_point, saved_eip
<<<<<<< HEAD
	ret
=======
	RET
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


restore_registers:
	movl	saved_context_ebp, %ebp
	movl	saved_context_ebx, %ebx
	movl	saved_context_esi, %esi
	movl	saved_context_edi, %edi
	pushl	saved_context_eflags
	popfl
<<<<<<< HEAD
	ret

ENTRY(do_suspend_lowlevel)
	call	save_processor_state
	call	save_registers
	call	acpi_enter_s3
=======
	RET

SYM_CODE_START(do_suspend_lowlevel)
	call	save_processor_state
	call	save_registers
	pushl	$3
	call	x86_acpi_enter_sleep_state
	addl	$4, %esp
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#	In case of S3 failure, we'll emerge here.  Jump
# 	to ret_point to recover
	jmp	ret_point
	.p2align 4,,7
ret_point:
	call	restore_registers
	call	restore_processor_state
<<<<<<< HEAD
	ret

.data
ALIGN
ENTRY(saved_magic)	.long	0
ENTRY(saved_eip)	.long	0

# saved registers
saved_gdt:	.long	0,0
=======
	RET
SYM_CODE_END(do_suspend_lowlevel)

.data
ALIGN
SYM_DATA(saved_magic,	.long 0)
saved_eip:		.long 0

# saved registers
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
saved_idt:	.long	0,0
saved_ldt:	.long	0
saved_tss:	.long	0

