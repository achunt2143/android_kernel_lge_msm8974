<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * atomic64_t for 386/486
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

/* if you want SMP support, implement these with real spinlocks */
.macro LOCK reg
	pushfl_cfi
	cli
.endm

.macro UNLOCK reg
	popfl_cfi
.endm

#define BEGIN(op) \
.macro endp; \
	CFI_ENDPROC; \
ENDPROC(atomic64_##op##_386); \
.purgem endp; \
.endm; \
ENTRY(atomic64_##op##_386); \
	CFI_STARTPROC; \
	LOCK v;

#define ENDP endp

#define RET \
	UNLOCK v; \
	ret

#define RET_ENDP \
	RET; \
	ENDP

#define v %ecx
BEGIN(read)
	movl  (v), %eax
	movl 4(v), %edx
RET_ENDP
#undef v

#define v %esi
BEGIN(set)
	movl %ebx,  (v)
	movl %ecx, 4(v)
RET_ENDP
#undef v

#define v  %esi
BEGIN(xchg)
=======
 */

#include <linux/linkage.h>
#include <asm/alternative.h>

/* if you want SMP support, implement these with real spinlocks */
.macro IRQ_SAVE reg
	pushfl
	cli
.endm

.macro IRQ_RESTORE reg
	popfl
.endm

#define BEGIN_IRQ_SAVE(op) \
.macro endp; \
SYM_FUNC_END(atomic64_##op##_386); \
.purgem endp; \
.endm; \
SYM_FUNC_START(atomic64_##op##_386); \
	IRQ_SAVE v;

#define ENDP endp

#define RET_IRQ_RESTORE \
	IRQ_RESTORE v; \
	RET

#define v %ecx
BEGIN_IRQ_SAVE(read)
	movl  (v), %eax
	movl 4(v), %edx
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %esi
BEGIN_IRQ_SAVE(set)
	movl %ebx,  (v)
	movl %ecx, 4(v)
	RET_IRQ_RESTORE
ENDP
#undef v

#define v  %esi
BEGIN_IRQ_SAVE(xchg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	movl  (v), %eax
	movl 4(v), %edx
	movl %ebx,  (v)
	movl %ecx, 4(v)
<<<<<<< HEAD
RET_ENDP
#undef v

#define v %ecx
BEGIN(add)
	addl %eax,  (v)
	adcl %edx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(add_return)
=======
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %ecx
BEGIN_IRQ_SAVE(add)
	addl %eax,  (v)
	adcl %edx, 4(v)
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %ecx
BEGIN_IRQ_SAVE(add_return)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addl  (v), %eax
	adcl 4(v), %edx
	movl %eax,  (v)
	movl %edx, 4(v)
<<<<<<< HEAD
RET_ENDP
#undef v

#define v %ecx
BEGIN(sub)
	subl %eax,  (v)
	sbbl %edx, 4(v)
RET_ENDP
#undef v

#define v %ecx
BEGIN(sub_return)
=======
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %ecx
BEGIN_IRQ_SAVE(sub)
	subl %eax,  (v)
	sbbl %edx, 4(v)
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %ecx
BEGIN_IRQ_SAVE(sub_return)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	negl %edx
	negl %eax
	sbbl $0, %edx
	addl  (v), %eax
	adcl 4(v), %edx
	movl %eax,  (v)
	movl %edx, 4(v)
<<<<<<< HEAD
RET_ENDP
#undef v

#define v %esi
BEGIN(inc)
	addl $1,  (v)
	adcl $0, 4(v)
RET_ENDP
#undef v

#define v %esi
BEGIN(inc_return)
=======
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %esi
BEGIN_IRQ_SAVE(inc)
	addl $1,  (v)
	adcl $0, 4(v)
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %esi
BEGIN_IRQ_SAVE(inc_return)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	movl  (v), %eax
	movl 4(v), %edx
	addl $1, %eax
	adcl $0, %edx
	movl %eax,  (v)
	movl %edx, 4(v)
<<<<<<< HEAD
RET_ENDP
#undef v

#define v %esi
BEGIN(dec)
	subl $1,  (v)
	sbbl $0, 4(v)
RET_ENDP
#undef v

#define v %esi
BEGIN(dec_return)
=======
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %esi
BEGIN_IRQ_SAVE(dec)
	subl $1,  (v)
	sbbl $0, 4(v)
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %esi
BEGIN_IRQ_SAVE(dec_return)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	movl  (v), %eax
	movl 4(v), %edx
	subl $1, %eax
	sbbl $0, %edx
	movl %eax,  (v)
	movl %edx, 4(v)
<<<<<<< HEAD
RET_ENDP
#undef v

#define v %esi
BEGIN(add_unless)
=======
	RET_IRQ_RESTORE
ENDP
#undef v

#define v %esi
BEGIN_IRQ_SAVE(add_unless)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addl %eax, %ecx
	adcl %edx, %edi
	addl  (v), %eax
	adcl 4(v), %edx
	cmpl %eax, %ecx
	je 3f
1:
	movl %eax,  (v)
	movl %edx, 4(v)
	movl $1, %eax
2:
<<<<<<< HEAD
	RET
=======
	RET_IRQ_RESTORE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
3:
	cmpl %edx, %edi
	jne 1b
	xorl %eax, %eax
	jmp 2b
ENDP
#undef v

#define v %esi
<<<<<<< HEAD
BEGIN(inc_not_zero)
=======
BEGIN_IRQ_SAVE(inc_not_zero)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	movl  (v), %eax
	movl 4(v), %edx
	testl %eax, %eax
	je 3f
1:
	addl $1, %eax
	adcl $0, %edx
	movl %eax,  (v)
	movl %edx, 4(v)
	movl $1, %eax
2:
<<<<<<< HEAD
	RET
=======
	RET_IRQ_RESTORE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
3:
	testl %edx, %edx
	jne 1b
	jmp 2b
ENDP
#undef v

#define v %esi
<<<<<<< HEAD
BEGIN(dec_if_positive)
=======
BEGIN_IRQ_SAVE(dec_if_positive)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	movl  (v), %eax
	movl 4(v), %edx
	subl $1, %eax
	sbbl $0, %edx
	js 1f
	movl %eax,  (v)
	movl %edx, 4(v)
1:
<<<<<<< HEAD
RET_ENDP
=======
	RET_IRQ_RESTORE
ENDP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef v
