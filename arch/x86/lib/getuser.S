<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * __get_user functions.
 *
 * (C) Copyright 1998 Linus Torvalds
 * (C) Copyright 2005 Andi Kleen
 * (C) Copyright 2008 Glauber Costa
 *
 * These functions have a non-standard call interface
 * to make them more efficient, especially as they
 * return an error value in addition to the "real"
 * return value.
 */

/*
 * __get_user_X
 *
 * Inputs:	%[r|e]ax contains the address.
<<<<<<< HEAD
 *		The register is modified, but all changes are undone
 *		before returning because the C code doesn't know about it.
 *
 * Outputs:	%[r|e]ax is error code (0 or -EFAULT)
 *		%[r|e]dx contains zero-extended value
=======
 *
 * Outputs:	%[r|e]ax is error code (0 or -EFAULT)
 *		%[r|e]dx contains zero-extended value
 *		%ecx contains the high half for 32-bit __get_user_8
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *
 * These functions should not modify any other registers,
 * as they get called from within inline assembly.
 */

<<<<<<< HEAD
#include <linux/linkage.h>
#include <asm/dwarf2.h>
=======
#include <linux/export.h>
#include <linux/linkage.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/page_types.h>
#include <asm/errno.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <asm/asm.h>
<<<<<<< HEAD

	.text
ENTRY(__get_user_1)
	CFI_STARTPROC
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user
1:	movzb (%_ASM_AX),%edx
	xor %eax,%eax
	ret
	CFI_ENDPROC
ENDPROC(__get_user_1)

ENTRY(__get_user_2)
	CFI_STARTPROC
	add $1,%_ASM_AX
	jc bad_get_user
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user
2:	movzwl -1(%_ASM_AX),%edx
	xor %eax,%eax
	ret
	CFI_ENDPROC
ENDPROC(__get_user_2)

ENTRY(__get_user_4)
	CFI_STARTPROC
	add $3,%_ASM_AX
	jc bad_get_user
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user
3:	mov -3(%_ASM_AX),%edx
	xor %eax,%eax
	ret
	CFI_ENDPROC
ENDPROC(__get_user_4)

#ifdef CONFIG_X86_64
ENTRY(__get_user_8)
	CFI_STARTPROC
	add $7,%_ASM_AX
	jc bad_get_user
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae	bad_get_user
4:	movq -7(%_ASM_AX),%_ASM_DX
	xor %eax,%eax
	ret
	CFI_ENDPROC
ENDPROC(__get_user_8)
#endif

bad_get_user:
	CFI_STARTPROC
	xor %edx,%edx
	mov $(-EFAULT),%_ASM_AX
	ret
	CFI_ENDPROC
END(bad_get_user)

.section __ex_table,"a"
	_ASM_PTR 1b,bad_get_user
	_ASM_PTR 2b,bad_get_user
	_ASM_PTR 3b,bad_get_user
#ifdef CONFIG_X86_64
	_ASM_PTR 4b,bad_get_user
=======
#include <asm/smap.h>

#define ASM_BARRIER_NOSPEC ALTERNATIVE "", "lfence", X86_FEATURE_LFENCE_RDTSC

.macro check_range size:req
.if IS_ENABLED(CONFIG_X86_64)
	mov %rax, %rdx
	sar $63, %rdx
	or %rdx, %rax
.else
	cmp $TASK_SIZE_MAX-\size+1, %eax
	jae .Lbad_get_user
	sbb %edx, %edx		/* array_index_mask_nospec() */
	and %edx, %eax
.endif
.endm

	.text
SYM_FUNC_START(__get_user_1)
	check_range size=1
	ASM_STAC
1:	movzbl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_1)
EXPORT_SYMBOL(__get_user_1)

SYM_FUNC_START(__get_user_2)
	check_range size=2
	ASM_STAC
2:	movzwl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_2)
EXPORT_SYMBOL(__get_user_2)

SYM_FUNC_START(__get_user_4)
	check_range size=4
	ASM_STAC
3:	movl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_4)
EXPORT_SYMBOL(__get_user_4)

SYM_FUNC_START(__get_user_8)
	check_range size=8
	ASM_STAC
#ifdef CONFIG_X86_64
4:	movq (%_ASM_AX),%rdx
#else
4:	movl (%_ASM_AX),%edx
5:	movl 4(%_ASM_AX),%ecx
#endif
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_8)
EXPORT_SYMBOL(__get_user_8)

/* .. and the same for __get_user, just without the range checks */
SYM_FUNC_START(__get_user_nocheck_1)
	ASM_STAC
	ASM_BARRIER_NOSPEC
6:	movzbl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_nocheck_1)
EXPORT_SYMBOL(__get_user_nocheck_1)

SYM_FUNC_START(__get_user_nocheck_2)
	ASM_STAC
	ASM_BARRIER_NOSPEC
7:	movzwl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_nocheck_2)
EXPORT_SYMBOL(__get_user_nocheck_2)

SYM_FUNC_START(__get_user_nocheck_4)
	ASM_STAC
	ASM_BARRIER_NOSPEC
8:	movl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_nocheck_4)
EXPORT_SYMBOL(__get_user_nocheck_4)

SYM_FUNC_START(__get_user_nocheck_8)
	ASM_STAC
	ASM_BARRIER_NOSPEC
#ifdef CONFIG_X86_64
9:	movq (%_ASM_AX),%rdx
#else
9:	movl (%_ASM_AX),%edx
10:	movl 4(%_ASM_AX),%ecx
#endif
	xor %eax,%eax
	ASM_CLAC
	RET
SYM_FUNC_END(__get_user_nocheck_8)
EXPORT_SYMBOL(__get_user_nocheck_8)


SYM_CODE_START_LOCAL(__get_user_handle_exception)
	ASM_CLAC
.Lbad_get_user:
	xor %edx,%edx
	mov $(-EFAULT),%_ASM_AX
	RET
SYM_CODE_END(__get_user_handle_exception)

#ifdef CONFIG_X86_32
SYM_CODE_START_LOCAL(__get_user_8_handle_exception)
	ASM_CLAC
bad_get_user_8:
	xor %edx,%edx
	xor %ecx,%ecx
	mov $(-EFAULT),%_ASM_AX
	RET
SYM_CODE_END(__get_user_8_handle_exception)
#endif

/* get_user */
	_ASM_EXTABLE_UA(1b, __get_user_handle_exception)
	_ASM_EXTABLE_UA(2b, __get_user_handle_exception)
	_ASM_EXTABLE_UA(3b, __get_user_handle_exception)
#ifdef CONFIG_X86_64
	_ASM_EXTABLE_UA(4b, __get_user_handle_exception)
#else
	_ASM_EXTABLE_UA(4b, __get_user_8_handle_exception)
	_ASM_EXTABLE_UA(5b, __get_user_8_handle_exception)
#endif

/* __get_user */
	_ASM_EXTABLE_UA(6b, __get_user_handle_exception)
	_ASM_EXTABLE_UA(7b, __get_user_handle_exception)
	_ASM_EXTABLE_UA(8b, __get_user_handle_exception)
#ifdef CONFIG_X86_64
	_ASM_EXTABLE_UA(9b, __get_user_handle_exception)
#else
	_ASM_EXTABLE_UA(9b, __get_user_8_handle_exception)
	_ASM_EXTABLE_UA(10b, __get_user_8_handle_exception)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
