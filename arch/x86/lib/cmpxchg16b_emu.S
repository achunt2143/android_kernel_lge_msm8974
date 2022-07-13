<<<<<<< HEAD
/*
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; version 2
 *	of the License.
 *
 */
#include <linux/linkage.h>
#include <asm/alternative-asm.h>
#include <asm/frame.h>
#include <asm/dwarf2.h>

#ifdef CONFIG_SMP
#define SEG_PREFIX %gs:
#else
#define SEG_PREFIX
#endif
=======
/* SPDX-License-Identifier: GPL-2.0-only */
#include <linux/linkage.h>
#include <asm/percpu.h>
#include <asm/processor-flags.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

.text

/*
<<<<<<< HEAD
=======
 * Emulate 'cmpxchg16b %gs:(%rsi)'
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Inputs:
 * %rsi : memory location to compare
 * %rax : low 64 bits of old value
 * %rdx : high 64 bits of old value
 * %rbx : low 64 bits of new value
 * %rcx : high 64 bits of new value
<<<<<<< HEAD
 * %al  : Operation successful
 */
ENTRY(this_cpu_cmpxchg16b_emu)
CFI_STARTPROC

#
# Emulate 'cmpxchg16b %gs:(%rsi)' except we return the result in %al not
# via the ZF.  Caller will access %al to get result.
#
# Note that this is only useful for a cpuops operation.  Meaning that we
# do *not* have a fully atomic operation but just an operation that is
# *atomic* on a single cpu (as provided by the this_cpu_xx class of
# macros).
#
this_cpu_cmpxchg16b_emu:
	pushf
	cli

	cmpq SEG_PREFIX(%rsi), %rax
	jne not_same
	cmpq SEG_PREFIX 8(%rsi), %rdx
	jne not_same

	movq %rbx, SEG_PREFIX(%rsi)
	movq %rcx, SEG_PREFIX 8(%rsi)

	popf
	mov $1, %al
	ret

 not_same:
	popf
	xor %al,%al
	ret

CFI_ENDPROC

ENDPROC(this_cpu_cmpxchg16b_emu)
=======
 *
 * Notably this is not LOCK prefixed and is not safe against NMIs
 */
SYM_FUNC_START(this_cpu_cmpxchg16b_emu)

	pushfq
	cli

	/* if (*ptr == old) */
	cmpq	__percpu (%rsi), %rax
	jne	.Lnot_same
	cmpq	__percpu 8(%rsi), %rdx
	jne	.Lnot_same

	/* *ptr = new */
	movq	%rbx, __percpu (%rsi)
	movq	%rcx, __percpu 8(%rsi)

	/* set ZF in EFLAGS to indicate success */
	orl	$X86_EFLAGS_ZF, (%rsp)

	popfq
	RET

.Lnot_same:
	/* *ptr != old */

	/* old = *ptr */
	movq	__percpu (%rsi), %rax
	movq	__percpu 8(%rsi), %rdx

	/* clear ZF in EFLAGS to indicate failure */
	andl	$(~X86_EFLAGS_ZF), (%rsp)

	popfq
	RET

SYM_FUNC_END(this_cpu_cmpxchg16b_emu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
