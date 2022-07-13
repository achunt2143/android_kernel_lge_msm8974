<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * __put_user functions.
 *
 * (C) Copyright 2005 Linus Torvalds
 * (C) Copyright 2005 Andi Kleen
 * (C) Copyright 2008 Glauber Costa
 *
 * These functions have a non-standard call interface
 * to make them more efficient, especially as they
 * return an error value in addition to the "real"
 * return value.
 */
<<<<<<< HEAD
#include <linux/linkage.h>
#include <asm/dwarf2.h>
#include <asm/thread_info.h>
#include <asm/errno.h>
#include <asm/asm.h>

=======
#include <linux/export.h>
#include <linux/linkage.h>
#include <asm/thread_info.h>
#include <asm/errno.h>
#include <asm/asm.h>
#include <asm/smap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * __put_user_X
 *
 * Inputs:	%eax[:%edx] contains the data
 *		%ecx contains the address
 *
<<<<<<< HEAD
 * Outputs:	%eax is error code (0 or -EFAULT)
=======
 * Outputs:	%ecx is error code (0 or -EFAULT)
 *
 * Clobbers:	%ebx needed for task pointer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * These functions should not modify any other registers,
 * as they get called from within inline assembly.
 */

<<<<<<< HEAD
#define ENTER	CFI_STARTPROC ; \
		GET_THREAD_INFO(%_ASM_BX)
#define EXIT	ret ; \
		CFI_ENDPROC

.text
ENTRY(__put_user_1)
	ENTER
	cmp TI_addr_limit(%_ASM_BX),%_ASM_CX
	jae bad_put_user
1:	movb %al,(%_ASM_CX)
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_1)

ENTRY(__put_user_2)
	ENTER
	mov TI_addr_limit(%_ASM_BX),%_ASM_BX
	sub $1,%_ASM_BX
	cmp %_ASM_BX,%_ASM_CX
	jae bad_put_user
2:	movw %ax,(%_ASM_CX)
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_2)

ENTRY(__put_user_4)
	ENTER
	mov TI_addr_limit(%_ASM_BX),%_ASM_BX
	sub $3,%_ASM_BX
	cmp %_ASM_BX,%_ASM_CX
	jae bad_put_user
3:	movl %eax,(%_ASM_CX)
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_4)

ENTRY(__put_user_8)
	ENTER
	mov TI_addr_limit(%_ASM_BX),%_ASM_BX
	sub $7,%_ASM_BX
	cmp %_ASM_BX,%_ASM_CX
	jae bad_put_user
4:	mov %_ASM_AX,(%_ASM_CX)
#ifdef CONFIG_X86_32
5:	movl %edx,4(%_ASM_CX)
#endif
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_8)

bad_put_user:
	CFI_STARTPROC
	movl $-EFAULT,%eax
	EXIT
END(bad_put_user)

.section __ex_table,"a"
	_ASM_PTR 1b,bad_put_user
	_ASM_PTR 2b,bad_put_user
	_ASM_PTR 3b,bad_put_user
	_ASM_PTR 4b,bad_put_user
#ifdef CONFIG_X86_32
	_ASM_PTR 5b,bad_put_user
#endif
.previous
=======
.macro check_range size:req
.if IS_ENABLED(CONFIG_X86_64)
	mov %rcx, %rbx
	sar $63, %rbx
	or %rbx, %rcx
.else
	cmp $TASK_SIZE_MAX-\size+1, %ecx
	jae .Lbad_put_user
.endif
.endm

.text
SYM_FUNC_START(__put_user_1)
	check_range size=1
	ASM_STAC
1:	movb %al,(%_ASM_CX)
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_1)
EXPORT_SYMBOL(__put_user_1)

SYM_FUNC_START(__put_user_nocheck_1)
	ASM_STAC
2:	movb %al,(%_ASM_CX)
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_nocheck_1)
EXPORT_SYMBOL(__put_user_nocheck_1)

SYM_FUNC_START(__put_user_2)
	check_range size=2
	ASM_STAC
3:	movw %ax,(%_ASM_CX)
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_2)
EXPORT_SYMBOL(__put_user_2)

SYM_FUNC_START(__put_user_nocheck_2)
	ASM_STAC
4:	movw %ax,(%_ASM_CX)
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_nocheck_2)
EXPORT_SYMBOL(__put_user_nocheck_2)

SYM_FUNC_START(__put_user_4)
	check_range size=4
	ASM_STAC
5:	movl %eax,(%_ASM_CX)
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_4)
EXPORT_SYMBOL(__put_user_4)

SYM_FUNC_START(__put_user_nocheck_4)
	ASM_STAC
6:	movl %eax,(%_ASM_CX)
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_nocheck_4)
EXPORT_SYMBOL(__put_user_nocheck_4)

SYM_FUNC_START(__put_user_8)
	check_range size=8
	ASM_STAC
7:	mov %_ASM_AX,(%_ASM_CX)
#ifdef CONFIG_X86_32
8:	movl %edx,4(%_ASM_CX)
#endif
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_8)
EXPORT_SYMBOL(__put_user_8)

SYM_FUNC_START(__put_user_nocheck_8)
	ASM_STAC
9:	mov %_ASM_AX,(%_ASM_CX)
#ifdef CONFIG_X86_32
10:	movl %edx,4(%_ASM_CX)
#endif
	xor %ecx,%ecx
	ASM_CLAC
	RET
SYM_FUNC_END(__put_user_nocheck_8)
EXPORT_SYMBOL(__put_user_nocheck_8)

SYM_CODE_START_LOCAL(__put_user_handle_exception)
	ASM_CLAC
.Lbad_put_user:
	movl $-EFAULT,%ecx
	RET
SYM_CODE_END(__put_user_handle_exception)

	_ASM_EXTABLE_UA(1b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(2b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(3b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(4b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(5b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(6b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(7b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(9b, __put_user_handle_exception)
#ifdef CONFIG_X86_32
	_ASM_EXTABLE_UA(8b, __put_user_handle_exception)
	_ASM_EXTABLE_UA(10b, __put_user_handle_exception)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
