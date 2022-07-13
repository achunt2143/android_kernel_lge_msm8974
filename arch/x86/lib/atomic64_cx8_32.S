<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * atomic64_t for 586+
 *
 * Copyright © 2010  Luca Barbieri
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/linkage.h>
#include <asm/alternative-asm.h>
#include <asm/dwarf2.h>

.macro SAVE reg
	pushl_cfi %\reg
	CFI_REL_OFFSET \reg, 0
.endm

.macro RESTORE reg
	popl_cfi %\reg
	CFI_RESTORE \reg
.endm
=======
 */

#include <linux/linkage.h>
#include <asm/alternative.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

.macro read64 reg
	movl %ebx, %eax
	movl %ecx, %edx
/* we need LOCK_PREFIX since otherwise cmpxchg8b always does the write */
	LOCK_PREFIX
	cmpxchg8b (\reg)
.endm

<<<<<<< HEAD
ENTRY(atomic64_read_cx8)
	CFI_STARTPROC

	read64 %ecx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_read_cx8)

ENTRY(atomic64_set_cx8)
	CFI_STARTPROC

=======
SYM_FUNC_START(atomic64_read_cx8)
	read64 %ecx
	RET
SYM_FUNC_END(atomic64_read_cx8)

SYM_FUNC_START(atomic64_set_cx8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
1:
/* we don't need LOCK_PREFIX since aligned 64-bit writes
 * are atomic on 586 and newer */
	cmpxchg8b (%esi)
	jne 1b

<<<<<<< HEAD
	ret
	CFI_ENDPROC
ENDPROC(atomic64_set_cx8)

ENTRY(atomic64_xchg_cx8)
	CFI_STARTPROC

=======
	RET
SYM_FUNC_END(atomic64_set_cx8)

SYM_FUNC_START(atomic64_xchg_cx8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
1:
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

<<<<<<< HEAD
	ret
	CFI_ENDPROC
ENDPROC(atomic64_xchg_cx8)

.macro addsub_return func ins insc
ENTRY(atomic64_\func\()_return_cx8)
	CFI_STARTPROC
	SAVE ebp
	SAVE ebx
	SAVE esi
	SAVE edi
=======
	RET
SYM_FUNC_END(atomic64_xchg_cx8)

.macro addsub_return func ins insc
SYM_FUNC_START(atomic64_\func\()_return_cx8)
	pushl %ebp
	pushl %ebx
	pushl %esi
	pushl %edi
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	movl %eax, %esi
	movl %edx, %edi
	movl %ecx, %ebp

	read64 %ecx
1:
	movl %eax, %ebx
	movl %edx, %ecx
	\ins\()l %esi, %ebx
	\insc\()l %edi, %ecx
	LOCK_PREFIX
	cmpxchg8b (%ebp)
	jne 1b

10:
	movl %ebx, %eax
	movl %ecx, %edx
<<<<<<< HEAD
	RESTORE edi
	RESTORE esi
	RESTORE ebx
	RESTORE ebp
	ret
	CFI_ENDPROC
ENDPROC(atomic64_\func\()_return_cx8)
=======
	popl %edi
	popl %esi
	popl %ebx
	popl %ebp
	RET
SYM_FUNC_END(atomic64_\func\()_return_cx8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
.endm

addsub_return add add adc
addsub_return sub sub sbb

.macro incdec_return func ins insc
<<<<<<< HEAD
ENTRY(atomic64_\func\()_return_cx8)
	CFI_STARTPROC
	SAVE ebx
=======
SYM_FUNC_START(atomic64_\func\()_return_cx8)
	pushl %ebx
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	read64 %esi
1:
	movl %eax, %ebx
	movl %edx, %ecx
	\ins\()l $1, %ebx
	\insc\()l $0, %ecx
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

10:
	movl %ebx, %eax
	movl %ecx, %edx
<<<<<<< HEAD
	RESTORE ebx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_\func\()_return_cx8)
=======
	popl %ebx
	RET
SYM_FUNC_END(atomic64_\func\()_return_cx8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
.endm

incdec_return inc add adc
incdec_return dec sub sbb

<<<<<<< HEAD
ENTRY(atomic64_dec_if_positive_cx8)
	CFI_STARTPROC
	SAVE ebx
=======
SYM_FUNC_START(atomic64_dec_if_positive_cx8)
	pushl %ebx
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	read64 %esi
1:
	movl %eax, %ebx
	movl %edx, %ecx
	subl $1, %ebx
	sbb $0, %ecx
	js 2f
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

2:
	movl %ebx, %eax
	movl %ecx, %edx
<<<<<<< HEAD
	RESTORE ebx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_dec_if_positive_cx8)

ENTRY(atomic64_add_unless_cx8)
	CFI_STARTPROC
	SAVE ebp
	SAVE ebx
/* these just push these two parameters on the stack */
	SAVE edi
	SAVE ecx
=======
	popl %ebx
	RET
SYM_FUNC_END(atomic64_dec_if_positive_cx8)

SYM_FUNC_START(atomic64_add_unless_cx8)
	pushl %ebp
	pushl %ebx
/* these just push these two parameters on the stack */
	pushl %edi
	pushl %ecx
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	movl %eax, %ebp
	movl %edx, %edi

	read64 %esi
1:
	cmpl %eax, 0(%esp)
	je 4f
2:
	movl %eax, %ebx
	movl %edx, %ecx
	addl %ebp, %ebx
	adcl %edi, %ecx
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

	movl $1, %eax
3:
	addl $8, %esp
<<<<<<< HEAD
	CFI_ADJUST_CFA_OFFSET -8
	RESTORE ebx
	RESTORE ebp
	ret
=======
	popl %ebx
	popl %ebp
	RET
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
4:
	cmpl %edx, 4(%esp)
	jne 2b
	xorl %eax, %eax
	jmp 3b
<<<<<<< HEAD
	CFI_ENDPROC
ENDPROC(atomic64_add_unless_cx8)

ENTRY(atomic64_inc_not_zero_cx8)
	CFI_STARTPROC
	SAVE ebx
=======
SYM_FUNC_END(atomic64_add_unless_cx8)

SYM_FUNC_START(atomic64_inc_not_zero_cx8)
	pushl %ebx
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	read64 %esi
1:
	movl %eax, %ecx
	orl %edx, %ecx
	jz 3f
	movl %eax, %ebx
	xorl %ecx, %ecx
	addl $1, %ebx
	adcl %edx, %ecx
	LOCK_PREFIX
	cmpxchg8b (%esi)
	jne 1b

	movl $1, %eax
3:
<<<<<<< HEAD
	RESTORE ebx
	ret
	CFI_ENDPROC
ENDPROC(atomic64_inc_not_zero_cx8)
=======
	popl %ebx
	RET
SYM_FUNC_END(atomic64_inc_not_zero_cx8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
